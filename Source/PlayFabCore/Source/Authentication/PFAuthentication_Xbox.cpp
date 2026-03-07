#include "stdafx.h"
#include "Generated/Authentication.h"
#include "PFCoreGlobalState.h"
#include "JsonUtils.h"
#include "PlayFabXUser.h"
#include "Authentication/LoginHandler.h"
#include "ApiXAsyncProvider.h"
#include "ApiHelpers.h"

namespace PlayFab
{
namespace Authentication
{

class LoginWithXUserHandler : public ILoginHandler, public std::enable_shared_from_this<LoginWithXUserHandler>
{
public:
    LoginWithXUserHandler(XUser&& platformUser, const PFAuthenticationLoginWithXUserRequest& request);

public: // ILoginHandler
    AsyncOp<CombinedLoginResult> Login(
        SharedPtr<class PFCoreGlobalState> state,
        SharedPtr<ServiceConfig> serviceConfig,
        bool createAccount,
        RunContext rc
    ) noexcept override;

    AsyncOp<void> ReLogin(
        SharedPtr<Entity> entity,
        RunContext rc
    ) noexcept override;

private:
    XUser m_platformUser;
    LoginWithXboxRequest m_request;
};

LoginWithXUserHandler::LoginWithXUserHandler(XUser&& platformUser, const PFAuthenticationLoginWithXUserRequest& request)
    : m_platformUser{ std::move(platformUser) },
    m_request{ PFAuthenticationLoginWithXboxRequest
    {
        request.createAccount,
        request.customTags,
        request.customTagsCount,
        request.infoRequestParameters,
        request.playerSecret,
        nullptr // XboxToken, will be populated later
    }
    }
{
}

AsyncOp<CombinedLoginResult> LoginWithXUserHandler::Login(
    SharedPtr<class PFCoreGlobalState> state,
    SharedPtr<ServiceConfig> serviceConfig,
    bool createAccount,
    RunContext rc
) noexcept
{
    SharedPtr<ILoginHandler> loginHandler{ shared_from_this() };

    return m_platformUser.GetTokenAndSignature(rc).Then([createAccount, serviceConfig, rc ](Result<TokenAndSignature> getTokenResult) mutable -> AsyncOp<ServiceResponse>
    {
        RETURN_IF_FAILED(getTokenResult.hr);

        Authentication::LoginWithXboxRequest request;
        request.SetCreateAccount(createAccount);
        request.SetXboxToken(getTokenResult.Payload().token);

        JsonValue requestBody{ request.ToJson() };
        RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));

        return serviceConfig->HttpClient()->MakePostRequest(
            "/Client/LoginWithXbox",
            UnorderedMap<String, String>{},
            std::move(requestBody),
            CacheId::AuthenticationLoginWithXbox,
            rc.Derive()
        );

    }).Then([state{ std::move(state) }, serviceConfig, loginHandler{ shared_from_this() }](Result<ServiceResponse> result)->Result<Authentication::CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode == 200)
        {
            return Authentication::CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginHandler);
        }
        else
        {
            return Result<Authentication::CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> LoginWithXUserHandler::ReLogin(
    SharedPtr<Entity> entity,
    RunContext rc
) noexcept
{
    SharedPtr<ILoginHandler> loginHandler{ shared_from_this() };

    return m_platformUser.GetTokenAndSignature(rc).Then([=](Result<TokenAndSignature> getTokenResult) mutable -> AsyncOp<ServiceResponse>
    {
        RETURN_IF_FAILED(getTokenResult.hr);

        Authentication::LoginWithXboxRequest request;
        request.SetXboxToken(getTokenResult.Payload().token);

        JsonValue requestBody{ request.ToJson() };
        RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", entity->ServiceConfig()->TitleId()));

        return entity->ServiceConfig()->HttpClient()->MakePostRequest(
            "/Client/LoginWithXbox",
            UnorderedMap<String, String>{},
            std::move(requestBody),
            CacheId::AuthenticationLoginWithXbox,
            rc.Derive()
        );

    }).Then([=](Result<ServiceResponse> result)->Result<void>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode == 200)
        {
            Authentication::EntityTokenResponse entityToken;
            RETURN_IF_FAILED(JsonUtils::ObjectGetMember(serviceResponse.Data, "EntityToken", entityToken));
            RETURN_IF_FAILED(entity->OnEntityTokenRefreshed(entityToken));
            RETURN_IF_FAILED(entity->OnLoginContextUpdated(loginHandler));
            return S_OK;
        }
        else
        {
            return Result<void>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

} // Authentication
} // PlayFab

using namespace PlayFab;
using namespace PlayFab::Authentication;

extern "C" PF_API PFAuthenticationLoginWithXUserAsync(
    _In_ PFServiceConfigHandle contextHandle,
    _In_ const PFAuthenticationLoginWithXUserRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(contextHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<ServiceConfig> context;
    RETURN_IF_FAILED(state->ServiceConfigs().FromHandle(contextHandle, context));

    auto duplicateXUserResult = PlayFab::XUser::Duplicate(request->user);
    RETURN_IF_FAILED(duplicateXUserResult.hr);

    SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithXUserHandler>(duplicateXUserResult.ExtractPayload(), *request);

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFAuthenticationLoginWithXUserAsync),
        std::bind(&ILoginHandler::LoginHelper, std::move(loginHandler), state, std::move(context), std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

extern "C" PF_API PFAuthenticationLoginWithXUserGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

extern "C" PF_API PFAuthenticationLoginWithXUserGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);

    Vector<char> tempBuffer;
    if (!bufferSize || !buffer)
    {
        // The buffer for the extended LoginResult is optional, but we report the result size to XAsync when the async
        // call completes (not when the client requests the result). XAsyncGetResult requires the specified buffer size
        // so create and pass it a temporary one if the client didn't provide one

        RETURN_HR_IF(E_INVALIDARG, result);
        RETURN_IF_FAILED(XAsyncGetResultSize(async, &bufferSize));
        tempBuffer.resize(bufferSize);
        buffer = tempBuffer.data();
    }

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    PFCombinedLoginResult* combinedLoginResult = reinterpret_cast<PFCombinedLoginResult*>(buffer);
    *entityHandle = combinedLoginResult->entityHandle;

    if (result)
    {
        *result = combinedLoginResult->loginResult;
    }

    return S_OK;
}

extern "C" PF_API PFAuthenticationReLoginWithXUserAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithXUserRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(entityHandle);
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<PFCoreGlobalState> state{ nullptr };
    RETURN_IF_FAILED(PFCoreGlobalState::Get(state));

    SharedPtr<Entity> entity;
    RETURN_IF_FAILED(state->Entities().FromHandle(entityHandle, entity));

    auto duplicateXUserResult = PlayFab::XUser::Duplicate(request->user);
    RETURN_IF_FAILED(duplicateXUserResult.hr);

    SharedPtr<ILoginHandler> loginHandler = MakeShared<LoginWithXUserHandler>(duplicateXUserResult.ExtractPayload(), *request);

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFAuthenticationReLoginWithXUserAsync),
        std::bind(&ILoginHandler::ReLoginHelper, std::move(loginHandler), std::move(entity), std::placeholders::_1)
    );

    return XAsyncProviderBase::Run(std::move(provider));
}

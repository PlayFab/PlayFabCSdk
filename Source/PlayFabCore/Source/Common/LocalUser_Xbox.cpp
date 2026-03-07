#include "stdafx.h"
#include "LocalUser_Xbox.h"
#include <playfab/core/cpp/AuthenticationTypeWrappers.h>
#include "Generated/Authentication.h"
#include "JsonUtils.h"

namespace PlayFab
{

class XboxLocalUserLoginHandler : public Authentication::ILoginHandler
{
public:
    XboxLocalUserLoginHandler(SharedPtr<XboxLocalUser> user);

    // ILoginHandler
    AsyncOp<Authentication::CombinedLoginResult> Login(
        SharedPtr<PFCoreGlobalState> state,
        SharedPtr<ServiceConfig> serviceConfig,
        bool createAccount,
        RunContext rc
    ) noexcept override;

    AsyncOp<void> ReLogin(
        SharedPtr<PlayFab::Entity> entity,
        RunContext rc
    ) noexcept override;

private:
    WeakPtr<XboxLocalUser> m_localUser;
};

Result<SharedPtr<LocalUser>> MakeXboxUser(
    _In_ SharedPtr<ServiceConfig> serviceConfig,
    _In_ XUserHandle user,
    _In_opt_ void* clientContext
)
{
    auto makeXUserResult = XboxLocalUser::Make(std::move(serviceConfig), user, clientContext);
    RETURN_IF_FAILED(makeXUserResult.hr);
    return makeXUserResult.ExtractPayload();
}

XboxLocalUser::XboxLocalUser(
    SharedPtr<ServiceConfig> serviceConfig,
    void* clientContext,
    XUser user,
    String localId
) :
    LocalUser{ std::move(serviceConfig), clientContext },
    m_user{ std::move(user) },
    m_localId{ std::move(localId) }
{
}

Result<SharedPtr<XboxLocalUser>> XboxLocalUser::Make(
    SharedPtr<ServiceConfig> serviceConfig,
    XUserHandle userHandle,
    void* clientContext
)
{
    auto copyUserResult = XUser::Duplicate(userHandle);
    RETURN_IF_FAILED(copyUserResult.hr);

    Stringstream localId;
    localId << "XUser_" << copyUserResult.Payload().Id();

    Allocator<XboxLocalUser> a;
    SharedPtr<XboxLocalUser> user{ new (a.allocate(1)) XboxLocalUser{ serviceConfig, clientContext, copyUserResult.ExtractPayload(), localId.str() }, Deleter<XboxLocalUser>{}, a };
    user->m_loginHandler = MakeShared<XboxLocalUserLoginHandler>(user);

    return std::move(user);
}

String XboxLocalUser::LocalId() const
{
    return m_localId;
}

SharedPtr<Authentication::ILoginHandler> XboxLocalUser::LoginHandler() const noexcept
{
    return m_loginHandler;
}

XboxLocalUserLoginHandler::XboxLocalUserLoginHandler(SharedPtr<XboxLocalUser> user) : m_localUser{ std::move(user) }
{
}

AsyncOp<Authentication::CombinedLoginResult> XboxLocalUserLoginHandler::Login(
    SharedPtr<PFCoreGlobalState> state,
    SharedPtr<ServiceConfig> serviceConfig,
    bool createAccount,
    RunContext rc
) noexcept
{
    SharedPtr<XboxLocalUser> localUser = m_localUser.lock();
    if (!localUser)
    {
        TRACE_INFORMATION("Unable to relogin because the LocalUser was destroyed");
        return E_FAIL;
    }

    return localUser->m_user.GetTokenAndSignature(rc).Then([createAccount, serviceConfig, rc](Result<TokenAndSignature> getTokenResult) mutable -> AsyncOp<ServiceResponse>
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

    }).Then([state{ std::move(state) }, serviceConfig, loginHandler{ localUser->LoginHandler() } ](Result<ServiceResponse> result)->Result<Authentication::CombinedLoginResult>
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

AsyncOp<void> XboxLocalUserLoginHandler::ReLogin(
    SharedPtr<PlayFab::Entity> entity,
    RunContext rc
) noexcept
{
    SharedPtr<XboxLocalUser> localUser = m_localUser.lock();
    if (!localUser)
    {
        TRACE_INFORMATION("Unable to relogin because the LocalUser was destroyed");
        return E_FAIL;
    }

    return localUser->m_user.GetTokenAndSignature(rc).Then([entity, rc](Result<TokenAndSignature> getTokenResult) mutable -> AsyncOp<ServiceResponse>
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

    }).Then([entity](Result<ServiceResponse> result)->Result<void>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode == 200)
        {
            Authentication::EntityTokenResponse entityToken;
            RETURN_IF_FAILED(JsonUtils::ObjectGetMember(serviceResponse.Data, "EntityToken", entityToken));
            RETURN_IF_FAILED(entity->OnEntityTokenRefreshed(entityToken));
            return S_OK;
        }
        else
        {
            return Result<void>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

}

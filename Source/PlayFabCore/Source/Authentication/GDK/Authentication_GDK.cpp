#include "stdafx.h"
#include "Generated/Authentication.h"
#include "GlobalState.h"
#include "JsonUtils.h"
#include "GDK/PlatformUser_GDK.h"

namespace PlayFab
{
namespace Authentication
{

class XUserLoginContext : public LoginContext
{
public:
    XUserLoginContext(XUser&& platformUser, String titleId, const PFAuthenticationLoginWithXUserRequest& request);

    AsyncOp<JsonValue> GetRequestBody(RunContext runContext) const override;

private:
    XUser m_platformUser;
    String m_titleId;
    LoginWithXboxRequest m_request;
};

XUserLoginContext::XUserLoginContext(XUser&& platformUser, String titleId, const PFAuthenticationLoginWithXUserRequest& request)
    : LoginContext{ "/Client/LoginWithXbox", CacheId::AuthenticationLoginWithXbox },
    m_platformUser{ std::move(platformUser) },
    m_titleId{ std::move(titleId) },
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

AsyncOp<JsonValue> XUserLoginContext::GetRequestBody(RunContext runContext) const
{
    return m_platformUser.GetTokenAndSignature(runContext).Then([request{ this->m_request }, titleId{ this->m_titleId }](Result<Platform::TokenAndSignature> getTokenResult) mutable->Result<JsonValue>
    {
        RETURN_IF_FAILED(getTokenResult.hr);
        request.SetXboxToken(getTokenResult.Payload().token);
        auto requestJson = request.ToJson();
        RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestJson, "TitleId", titleId));
        return requestJson;
    });
}

AsyncOp<CombinedLoginResult> AuthenticationAPI::LoginWithXUser(
    SharedPtr<GlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    const LoginWithXUserRequest& request,
    RunContext rc
)
{
    auto duplicateUserHandleResult = XUser::Duplicate(request.Model().user);
    RETURN_IF_FAILED(duplicateUserHandleResult.hr);

    auto loginContext = MakeShared<XUserLoginContext>(duplicateUserHandleResult.ExtractPayload(), serviceConfig->TitleId(), request.Model());

    return loginContext->GetRequestBody(rc).Then([rc, serviceConfig, loginContext](Result<JsonValue> requestBodyResult) mutable -> AsyncOp<ServiceResponse>
    {
        const char* path{ "/Client/LoginWithXbox" };
        JsonValue requestBody{ requestBodyResult.ExtractPayload() };

        return serviceConfig->HttpClient()->MakePostRequest(
            path,
            UnorderedMap<String, String>{},
            std::move(requestBody),
            CacheId::AuthenticationLoginWithXbox,
            rc.Derive()
        );

    }).Then([state, serviceConfig, loginContext](Result<ServiceResponse> result)->Result<CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode == 200)
        {
            return CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginContext);
        }
        else
        {
            return Result<CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AuthenticationAPI::ReLoginWithXUser(
    SharedPtr<Entity> entity,
    const LoginWithXUserRequest& request,
    RunContext rc
)
{
    auto duplicateUserHandleResult = XUser::Duplicate(request.Model().user);
    RETURN_IF_FAILED(duplicateUserHandleResult.hr);

    // Create a new XUserLoginContext. TODO, we need a way to set this in the entity
    auto loginContext = MakeShared<XUserLoginContext>(duplicateUserHandleResult.ExtractPayload(), entity->ServiceConfig()->TitleId(), request.Model());

    return loginContext->GetRequestBody(rc).Then([rc, entity](Result<JsonValue> requestBodyResult) mutable -> AsyncOp<ServiceResponse>
    {
        const char* path{ "/Client/LoginWithXbox" };
        JsonValue requestBody{ requestBodyResult.ExtractPayload() };

        return entity->ServiceConfig()->HttpClient()->MakePostRequest(
            path,
            UnorderedMap<String, String>{},
            std::move(requestBody),
            CacheId::AuthenticationLoginWithXbox,
            rc.Derive()
        );

    }).Then([entity, loginContext](Result<ServiceResponse> result)->Result<void>
    {
        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode == 200)
        {
            EntityTokenResponse entityToken;
            RETURN_IF_FAILED(JsonUtils::ObjectGetMember(serviceResponse.Data, "EntityToken", entityToken));
            RETURN_IF_FAILED(entity->OnEntityTokenRefreshed(entityToken));
            RETURN_IF_FAILED(entity->OnLoginContextUpdated(loginContext));

            return S_OK;
        }
        else
        {
            return Result<void>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

}
}

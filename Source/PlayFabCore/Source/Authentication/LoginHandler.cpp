#include "stdafx.h"
#include "LoginHandler.h"
#include "JsonUtils.h"

namespace PlayFab
{
namespace Authentication
{

AsyncOp<CombinedLoginResult> ILoginHandler::LoginHelper(
    SharedPtr<ILoginHandler> loginHandler,
    SharedPtr<PFCoreGlobalState> state,
    SharedPtr<ServiceConfig> serviceConfig,
    RunContext rc
) noexcept
{
    return loginHandler->Login(std::move(state), std::move(serviceConfig), true, std::move(rc));
}

AsyncOp<void> ILoginHandler::ReLoginHelper(
    SharedPtr<ILoginHandler> loginHandler,
    SharedPtr<Entity> entity,
    RunContext rc
) noexcept
{
    return loginHandler->ReLogin(std::move(entity), std::move(rc));
}

LoginWithRequestHandler::LoginWithRequestHandler(
    const char* path,
    JsonValue&& requestBody,
    CacheId retryCacheId
) noexcept :
    m_path{ path },
    m_cacheId{ retryCacheId }
{
    m_requestBody = std::move(requestBody);
}

AsyncOp<CombinedLoginResult> LoginWithRequestHandler::Login(
    SharedPtr<PFCoreGlobalState> state,
    SharedPtr<ServiceConfig> serviceConfig,
    bool /*createAccount*/,
    RunContext rc
) noexcept
{
    return MakeLoginRequest(serviceConfig, std::move(rc)).Then([state, serviceConfig, loginHandler{ shared_from_this() }](Result<ServiceResponse> result) -> Result<CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginHandler);
        }
        else
        {
            return Result<CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> LoginWithRequestHandler::ReLogin(
    SharedPtr<Entity> entity,
    RunContext rc
) noexcept
{
    return MakeLoginRequest(entity->ServiceConfig(), std::move(rc)).Then([entity, loginHandler{ shared_from_this() }](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            EntityTokenResponse entityToken;
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

AsyncOp<ServiceResponse> LoginWithRequestHandler::MakeLoginRequest(
    SharedPtr<ServiceConfig const> serviceConfig,
    RunContext rc
) noexcept
{
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(m_requestBody, "TitleId", serviceConfig->TitleId()));

    return serviceConfig->HttpClient()->MakePostRequest(
        m_path.data(),
        UnorderedMap<String, String>{},
        m_requestBody,
        m_cacheId,
        rc.Derive()
    );
}

}
}

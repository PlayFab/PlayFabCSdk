#include "stdafx.h"
#include "Authentication.h"
#include "PFCoreGlobalState.h"
#include "JsonUtils.h"

namespace PlayFab
{
namespace Authentication
{


AsyncOp<RegisterPlayFabUserResult> AuthenticationAPI::RegisterPlayFabUser(
    SharedPtr<ServiceConfig const> serviceConfig,
    const RegisterPlayFabUserRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/RegisterPlayFabUser" };
    JsonValue requestBody{ request.ToJson() };
    CacheId retryCacheId = CacheId::AuthenticationRegisterPlayFabUser;

    auto requestOp = serviceConfig->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<RegisterPlayFabUserResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            RegisterPlayFabUserResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<RegisterPlayFabUserResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerCombinedLoginResult> AuthenticationAPI::ServerLoginWithAndroidDeviceID(
    SharedPtr<PFCoreGlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    String&& secretKey,
    const ServerLoginWithAndroidDeviceIDRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/LoginWithAndroidDeviceID" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationServerLoginWithAndroidDeviceID;

    auto requestOp = serviceConfig->HttpClient()->MakeSecretKeyRequest(
        serviceConfig,
        secretKey,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([state, serviceConfig, secretKey](Result<ServiceResponse> result) -> Result<ServerCombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return ServerCombinedLoginResult::FromJson(serviceResponse.Data);
        }
        else
        {
            return Result<ServerCombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerCombinedLoginResult> AuthenticationAPI::ServerLoginWithBattleNet(
    SharedPtr<PFCoreGlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    String&& secretKey,
    const ServerLoginWithBattleNetRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/LoginWithBattleNet" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationServerLoginWithBattleNet;

    auto requestOp = serviceConfig->HttpClient()->MakeSecretKeyRequest(
        serviceConfig,
        secretKey,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([state, serviceConfig, secretKey](Result<ServiceResponse> result) -> Result<ServerCombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return ServerCombinedLoginResult::FromJson(serviceResponse.Data);
        }
        else
        {
            return Result<ServerCombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerCombinedLoginResult> AuthenticationAPI::ServerLoginWithCustomID(
    SharedPtr<PFCoreGlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    String&& secretKey,
    const ServerLoginWithCustomIDRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/LoginWithCustomID" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationServerLoginWithCustomID;

    auto requestOp = serviceConfig->HttpClient()->MakeSecretKeyRequest(
        serviceConfig,
        secretKey,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([state, serviceConfig, secretKey](Result<ServiceResponse> result) -> Result<ServerCombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return ServerCombinedLoginResult::FromJson(serviceResponse.Data);
        }
        else
        {
            return Result<ServerCombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerCombinedLoginResult> AuthenticationAPI::ServerLoginWithIOSDeviceID(
    SharedPtr<PFCoreGlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    String&& secretKey,
    const ServerLoginWithIOSDeviceIDRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/LoginWithIOSDeviceID" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationServerLoginWithIOSDeviceID;

    auto requestOp = serviceConfig->HttpClient()->MakeSecretKeyRequest(
        serviceConfig,
        secretKey,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([state, serviceConfig, secretKey](Result<ServiceResponse> result) -> Result<ServerCombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return ServerCombinedLoginResult::FromJson(serviceResponse.Data);
        }
        else
        {
            return Result<ServerCombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerCombinedLoginResult> AuthenticationAPI::ServerLoginWithPSN(
    SharedPtr<PFCoreGlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    String&& secretKey,
    const ServerLoginWithPSNRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/LoginWithPSN" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationServerLoginWithPSN;

    auto requestOp = serviceConfig->HttpClient()->MakeSecretKeyRequest(
        serviceConfig,
        secretKey,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([state, serviceConfig, secretKey](Result<ServiceResponse> result) -> Result<ServerCombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return ServerCombinedLoginResult::FromJson(serviceResponse.Data);
        }
        else
        {
            return Result<ServerCombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerCombinedLoginResult> AuthenticationAPI::ServerLoginWithServerCustomId(
    SharedPtr<PFCoreGlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    String&& secretKey,
    const LoginWithServerCustomIdRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/LoginWithServerCustomId" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationServerLoginWithServerCustomId;

    auto requestOp = serviceConfig->HttpClient()->MakeSecretKeyRequest(
        serviceConfig,
        secretKey,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([state, serviceConfig, secretKey](Result<ServiceResponse> result) -> Result<ServerCombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return ServerCombinedLoginResult::FromJson(serviceResponse.Data);
        }
        else
        {
            return Result<ServerCombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerCombinedLoginResult> AuthenticationAPI::ServerLoginWithSteamId(
    SharedPtr<PFCoreGlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    String&& secretKey,
    const LoginWithSteamIdRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/LoginWithSteamId" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationServerLoginWithSteamId;

    auto requestOp = serviceConfig->HttpClient()->MakeSecretKeyRequest(
        serviceConfig,
        secretKey,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([state, serviceConfig, secretKey](Result<ServiceResponse> result) -> Result<ServerCombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return ServerCombinedLoginResult::FromJson(serviceResponse.Data);
        }
        else
        {
            return Result<ServerCombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerCombinedLoginResult> AuthenticationAPI::ServerLoginWithTwitch(
    SharedPtr<PFCoreGlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    String&& secretKey,
    const ServerLoginWithTwitchRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/LoginWithTwitch" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationServerLoginWithTwitch;

    auto requestOp = serviceConfig->HttpClient()->MakeSecretKeyRequest(
        serviceConfig,
        secretKey,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([state, serviceConfig, secretKey](Result<ServiceResponse> result) -> Result<ServerCombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return ServerCombinedLoginResult::FromJson(serviceResponse.Data);
        }
        else
        {
            return Result<ServerCombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerCombinedLoginResult> AuthenticationAPI::ServerLoginWithXbox(
    SharedPtr<PFCoreGlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    String&& secretKey,
    const ServerLoginWithXboxRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/LoginWithXbox" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationServerLoginWithXbox;

    auto requestOp = serviceConfig->HttpClient()->MakeSecretKeyRequest(
        serviceConfig,
        secretKey,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([state, serviceConfig, secretKey](Result<ServiceResponse> result) -> Result<ServerCombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return ServerCombinedLoginResult::FromJson(serviceResponse.Data);
        }
        else
        {
            return Result<ServerCombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ServerCombinedLoginResult> AuthenticationAPI::ServerLoginWithXboxId(
    SharedPtr<PFCoreGlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    String&& secretKey,
    const LoginWithXboxIdRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/LoginWithXboxId" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationServerLoginWithXboxId;

    auto requestOp = serviceConfig->HttpClient()->MakeSecretKeyRequest(
        serviceConfig,
        secretKey,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([state, serviceConfig, secretKey](Result<ServiceResponse> result) -> Result<ServerCombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return ServerCombinedLoginResult::FromJson(serviceResponse.Data);
        }
        else
        {
            return Result<ServerCombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<AuthenticateGameServerResult> AuthenticationAPI::AuthenticateGameServerWithCustomId(
    SharedPtr<PFCoreGlobalState> state,
    SharedPtr<Entity> entity,
    const AuthenticateCustomIdRequest& request,
    RunContext rc
)
{
    const char* path{ "/GameServerIdentity/AuthenticateGameServerWithCustomId" };
    JsonValue requestBody{ request.ToJson() };
    CacheId retryCacheId = CacheId::AuthenticationAuthenticateGameServerWithCustomId;

    auto requestOp = entity->ServiceConfig()->HttpClient()->MakeEntityRequest(
        entity,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([state, serviceConfig{ entity->ServiceConfig() }](Result<ServiceResponse> result) -> Result<AuthenticateGameServerResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return AuthenticateGameServerResult::FromJson(serviceResponse.Data, state, serviceConfig);
        }
        else
        {
            return Result<AuthenticateGameServerResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AuthenticationAPI::Delete(
    SharedPtr<Entity> entity,
    const DeleteRequest& request,
    RunContext rc
)
{
    const char* path{ "/GameServerIdentity/Delete" };
    JsonValue requestBody{ request.ToJson() };
    CacheId retryCacheId = CacheId::AuthenticationDelete;

    auto requestOp = entity->ServiceConfig()->HttpClient()->MakeEntityRequest(
        entity,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return S_OK;
        }
        else
        {
            return Result<void>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<SharedPtr<Entity>> AuthenticationAPI::GetEntity(
    SharedPtr<PFCoreGlobalState> state,
    SharedPtr<Entity> entity,
    const GetEntityRequest& request,
    RunContext rc
)
{
    const char* path{ "/Authentication/GetEntityToken" };
    JsonValue requestBody{ request.ToJson() };
    CacheId retryCacheId = CacheId::AuthenticationGetEntity;

    auto requestOp = entity->ServiceConfig()->HttpClient()->MakeEntityRequest(
        entity,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([state, serviceConfig{ entity->ServiceConfig() }](Result<ServiceResponse> result) -> Result<SharedPtr<Entity>>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetEntityTokenResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return Entity::Make(std::move(resultModel), serviceConfig, state->RunContext(), state->TokenExpiredHandler(), state->TokenRefreshedHandler());
        }
        else
        {
            return Result<SharedPtr<Entity>>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<SharedPtr<Entity>> AuthenticationAPI::GetEntityWithSecretKey(
    SharedPtr<PFCoreGlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    String&& secretKey,
    const GetEntityRequest& request,
    RunContext rc
)
{
    const char* path{ "/Authentication/GetEntityToken" };
    JsonValue requestBody{ request.ToJson() };
    CacheId retryCacheId = CacheId::AuthenticationGetEntityWithSecretKey;

    auto requestOp = serviceConfig->HttpClient()->MakeSecretKeyRequest(
        serviceConfig,
        secretKey,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([state, serviceConfig, secretKey](Result<ServiceResponse> result) -> Result<SharedPtr<Entity>>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetEntityTokenResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return Entity::Make(std::move(resultModel), serviceConfig, state->RunContext(), state->TokenExpiredHandler(), state->TokenRefreshedHandler(), secretKey);
        }
        else
        {
            return Result<SharedPtr<Entity>>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<ValidateEntityTokenResponse> AuthenticationAPI::ValidateEntityToken(
    SharedPtr<Entity> entity,
    const ValidateEntityTokenRequest& request,
    RunContext rc
)
{
    const char* path{ "/Authentication/ValidateEntityToken" };
    JsonValue requestBody{ request.ToJson() };
    CacheId retryCacheId = CacheId::AuthenticationValidateEntityToken;

    auto requestOp = entity->ServiceConfig()->HttpClient()->MakeEntityRequest(
        entity,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<ValidateEntityTokenResponse>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            ValidateEntityTokenResponse resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<ValidateEntityTokenResponse>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

} // namespace Authentication
} // namespace PlayFab

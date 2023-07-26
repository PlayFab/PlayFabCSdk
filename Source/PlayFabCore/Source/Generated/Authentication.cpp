#include "stdafx.h"
#include "Authentication.h"
#include "GlobalState.h"
#include "JsonUtils.h"

namespace PlayFab
{
namespace Authentication
{


AsyncOp<GetPhotonAuthenticationTokenResult> AuthenticationAPI::GetPhotonAuthenticationToken(
    SharedPtr<Entity> entity,
    const GetPhotonAuthenticationTokenRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetPhotonAuthenticationToken" };
    JsonValue requestBody{ request.ToJson() };
    CacheId retryCacheId = CacheId::AuthenticationGetPhotonAuthenticationToken;

    auto requestOp = entity->ServiceConfig()->HttpClient()->MakeEntityRequest(
        entity,
        path,
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetPhotonAuthenticationTokenResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetPhotonAuthenticationTokenResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetPhotonAuthenticationTokenResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<GetTitlePublicKeyResult> AuthenticationAPI::GetTitlePublicKey(
    SharedPtr<ServiceConfig const> serviceConfig,
    const GetTitlePublicKeyRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/GetTitlePublicKey" };
    JsonValue requestBody{ request.ToJson() };
    CacheId retryCacheId = CacheId::AuthenticationGetTitlePublicKey;

    auto requestOp = serviceConfig->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<GetTitlePublicKeyResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            GetTitlePublicKeyResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<GetTitlePublicKeyResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<CombinedLoginResult> AuthenticationAPI::LoginWithAndroidDeviceID(
    SharedPtr<GlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    const LoginWithAndroidDeviceIDRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LoginWithAndroidDeviceID" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationLoginWithAndroidDeviceID;

    auto requestOp = serviceConfig->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([state, serviceConfig, loginContext](Result<ServiceResponse> result) -> Result<CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginContext);
        }
        else
        {
            return Result<CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AuthenticationAPI::ReLoginWithAndroidDeviceID(
    SharedPtr<Entity> entity,
    const LoginWithAndroidDeviceIDRequest& request,
    RunContext rc)
{
    const char* path{ "/Client/LoginWithAndroidDeviceID" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", entity->ServiceConfig()->TitleId()));

    CacheId retryCacheId = CacheId::AuthenticationLoginWithAndroidDeviceID;
    auto requestOp = entity->ServiceConfig()->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        std::move(requestBody),
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([entity, loginContext](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

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

AsyncOp<CombinedLoginResult> AuthenticationAPI::LoginWithApple(
    SharedPtr<GlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    const LoginWithAppleRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LoginWithApple" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationLoginWithApple;

    auto requestOp = serviceConfig->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([state, serviceConfig, loginContext](Result<ServiceResponse> result) -> Result<CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginContext);
        }
        else
        {
            return Result<CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AuthenticationAPI::ReLoginWithApple(
    SharedPtr<Entity> entity,
    const LoginWithAppleRequest& request,
    RunContext rc)
{
    const char* path{ "/Client/LoginWithApple" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", entity->ServiceConfig()->TitleId()));

    CacheId retryCacheId = CacheId::AuthenticationLoginWithApple;
    auto requestOp = entity->ServiceConfig()->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        std::move(requestBody),
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([entity, loginContext](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

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

AsyncOp<CombinedLoginResult> AuthenticationAPI::LoginWithCustomID(
    SharedPtr<GlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    const LoginWithCustomIDRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LoginWithCustomID" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationLoginWithCustomID;

    auto requestOp = serviceConfig->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([state, serviceConfig, loginContext](Result<ServiceResponse> result) -> Result<CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginContext);
        }
        else
        {
            return Result<CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AuthenticationAPI::ReLoginWithCustomID(
    SharedPtr<Entity> entity,
    const LoginWithCustomIDRequest& request,
    RunContext rc)
{
    const char* path{ "/Client/LoginWithCustomID" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", entity->ServiceConfig()->TitleId()));

    CacheId retryCacheId = CacheId::AuthenticationLoginWithCustomID;
    auto requestOp = entity->ServiceConfig()->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        std::move(requestBody),
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([entity, loginContext](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

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

AsyncOp<CombinedLoginResult> AuthenticationAPI::LoginWithEmailAddress(
    SharedPtr<GlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    const LoginWithEmailAddressRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LoginWithEmailAddress" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationLoginWithEmailAddress;

    auto requestOp = serviceConfig->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([state, serviceConfig, loginContext](Result<ServiceResponse> result) -> Result<CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginContext);
        }
        else
        {
            return Result<CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AuthenticationAPI::ReLoginWithEmailAddress(
    SharedPtr<Entity> entity,
    const LoginWithEmailAddressRequest& request,
    RunContext rc)
{
    const char* path{ "/Client/LoginWithEmailAddress" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", entity->ServiceConfig()->TitleId()));

    CacheId retryCacheId = CacheId::AuthenticationLoginWithEmailAddress;
    auto requestOp = entity->ServiceConfig()->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        std::move(requestBody),
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([entity, loginContext](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

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

AsyncOp<CombinedLoginResult> AuthenticationAPI::LoginWithFacebook(
    SharedPtr<GlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    const LoginWithFacebookRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LoginWithFacebook" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationLoginWithFacebook;

    auto requestOp = serviceConfig->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([state, serviceConfig, loginContext](Result<ServiceResponse> result) -> Result<CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginContext);
        }
        else
        {
            return Result<CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AuthenticationAPI::ReLoginWithFacebook(
    SharedPtr<Entity> entity,
    const LoginWithFacebookRequest& request,
    RunContext rc)
{
    const char* path{ "/Client/LoginWithFacebook" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", entity->ServiceConfig()->TitleId()));

    CacheId retryCacheId = CacheId::AuthenticationLoginWithFacebook;
    auto requestOp = entity->ServiceConfig()->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        std::move(requestBody),
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([entity, loginContext](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

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

AsyncOp<CombinedLoginResult> AuthenticationAPI::LoginWithFacebookInstantGamesId(
    SharedPtr<GlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    const LoginWithFacebookInstantGamesIdRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LoginWithFacebookInstantGamesId" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationLoginWithFacebookInstantGamesId;

    auto requestOp = serviceConfig->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([state, serviceConfig, loginContext](Result<ServiceResponse> result) -> Result<CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginContext);
        }
        else
        {
            return Result<CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AuthenticationAPI::ReLoginWithFacebookInstantGamesId(
    SharedPtr<Entity> entity,
    const LoginWithFacebookInstantGamesIdRequest& request,
    RunContext rc)
{
    const char* path{ "/Client/LoginWithFacebookInstantGamesId" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", entity->ServiceConfig()->TitleId()));

    CacheId retryCacheId = CacheId::AuthenticationLoginWithFacebookInstantGamesId;
    auto requestOp = entity->ServiceConfig()->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        std::move(requestBody),
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([entity, loginContext](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

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

AsyncOp<CombinedLoginResult> AuthenticationAPI::LoginWithGameCenter(
    SharedPtr<GlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    const LoginWithGameCenterRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LoginWithGameCenter" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationLoginWithGameCenter;

    auto requestOp = serviceConfig->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([state, serviceConfig, loginContext](Result<ServiceResponse> result) -> Result<CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginContext);
        }
        else
        {
            return Result<CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AuthenticationAPI::ReLoginWithGameCenter(
    SharedPtr<Entity> entity,
    const LoginWithGameCenterRequest& request,
    RunContext rc)
{
    const char* path{ "/Client/LoginWithGameCenter" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", entity->ServiceConfig()->TitleId()));

    CacheId retryCacheId = CacheId::AuthenticationLoginWithGameCenter;
    auto requestOp = entity->ServiceConfig()->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        std::move(requestBody),
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([entity, loginContext](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

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

AsyncOp<CombinedLoginResult> AuthenticationAPI::LoginWithGoogleAccount(
    SharedPtr<GlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    const LoginWithGoogleAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LoginWithGoogleAccount" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationLoginWithGoogleAccount;

    auto requestOp = serviceConfig->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([state, serviceConfig, loginContext](Result<ServiceResponse> result) -> Result<CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginContext);
        }
        else
        {
            return Result<CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AuthenticationAPI::ReLoginWithGoogleAccount(
    SharedPtr<Entity> entity,
    const LoginWithGoogleAccountRequest& request,
    RunContext rc)
{
    const char* path{ "/Client/LoginWithGoogleAccount" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", entity->ServiceConfig()->TitleId()));

    CacheId retryCacheId = CacheId::AuthenticationLoginWithGoogleAccount;
    auto requestOp = entity->ServiceConfig()->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        std::move(requestBody),
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([entity, loginContext](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

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

AsyncOp<CombinedLoginResult> AuthenticationAPI::LoginWithGooglePlayGamesServices(
    SharedPtr<GlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    const LoginWithGooglePlayGamesServicesRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LoginWithGooglePlayGamesServices" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationLoginWithGooglePlayGamesServices;

    auto requestOp = serviceConfig->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([state, serviceConfig, loginContext](Result<ServiceResponse> result) -> Result<CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginContext);
        }
        else
        {
            return Result<CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AuthenticationAPI::ReLoginWithGooglePlayGamesServices(
    SharedPtr<Entity> entity,
    const LoginWithGooglePlayGamesServicesRequest& request,
    RunContext rc)
{
    const char* path{ "/Client/LoginWithGooglePlayGamesServices" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", entity->ServiceConfig()->TitleId()));

    CacheId retryCacheId = CacheId::AuthenticationLoginWithGooglePlayGamesServices;
    auto requestOp = entity->ServiceConfig()->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        std::move(requestBody),
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([entity, loginContext](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

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

AsyncOp<CombinedLoginResult> AuthenticationAPI::LoginWithIOSDeviceID(
    SharedPtr<GlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    const LoginWithIOSDeviceIDRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LoginWithIOSDeviceID" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationLoginWithIOSDeviceID;

    auto requestOp = serviceConfig->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([state, serviceConfig, loginContext](Result<ServiceResponse> result) -> Result<CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginContext);
        }
        else
        {
            return Result<CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AuthenticationAPI::ReLoginWithIOSDeviceID(
    SharedPtr<Entity> entity,
    const LoginWithIOSDeviceIDRequest& request,
    RunContext rc)
{
    const char* path{ "/Client/LoginWithIOSDeviceID" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", entity->ServiceConfig()->TitleId()));

    CacheId retryCacheId = CacheId::AuthenticationLoginWithIOSDeviceID;
    auto requestOp = entity->ServiceConfig()->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        std::move(requestBody),
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([entity, loginContext](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

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

AsyncOp<CombinedLoginResult> AuthenticationAPI::LoginWithKongregate(
    SharedPtr<GlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    const LoginWithKongregateRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LoginWithKongregate" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationLoginWithKongregate;

    auto requestOp = serviceConfig->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([state, serviceConfig, loginContext](Result<ServiceResponse> result) -> Result<CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginContext);
        }
        else
        {
            return Result<CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AuthenticationAPI::ReLoginWithKongregate(
    SharedPtr<Entity> entity,
    const LoginWithKongregateRequest& request,
    RunContext rc)
{
    const char* path{ "/Client/LoginWithKongregate" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", entity->ServiceConfig()->TitleId()));

    CacheId retryCacheId = CacheId::AuthenticationLoginWithKongregate;
    auto requestOp = entity->ServiceConfig()->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        std::move(requestBody),
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([entity, loginContext](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

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

AsyncOp<CombinedLoginResult> AuthenticationAPI::LoginWithNintendoServiceAccount(
    SharedPtr<GlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    const LoginWithNintendoServiceAccountRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LoginWithNintendoServiceAccount" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationLoginWithNintendoServiceAccount;

    auto requestOp = serviceConfig->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([state, serviceConfig, loginContext](Result<ServiceResponse> result) -> Result<CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginContext);
        }
        else
        {
            return Result<CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AuthenticationAPI::ReLoginWithNintendoServiceAccount(
    SharedPtr<Entity> entity,
    const LoginWithNintendoServiceAccountRequest& request,
    RunContext rc)
{
    const char* path{ "/Client/LoginWithNintendoServiceAccount" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", entity->ServiceConfig()->TitleId()));

    CacheId retryCacheId = CacheId::AuthenticationLoginWithNintendoServiceAccount;
    auto requestOp = entity->ServiceConfig()->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        std::move(requestBody),
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([entity, loginContext](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

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

AsyncOp<CombinedLoginResult> AuthenticationAPI::LoginWithNintendoSwitchDeviceId(
    SharedPtr<GlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    const LoginWithNintendoSwitchDeviceIdRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LoginWithNintendoSwitchDeviceId" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationLoginWithNintendoSwitchDeviceId;

    auto requestOp = serviceConfig->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([state, serviceConfig, loginContext](Result<ServiceResponse> result) -> Result<CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginContext);
        }
        else
        {
            return Result<CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AuthenticationAPI::ReLoginWithNintendoSwitchDeviceId(
    SharedPtr<Entity> entity,
    const LoginWithNintendoSwitchDeviceIdRequest& request,
    RunContext rc)
{
    const char* path{ "/Client/LoginWithNintendoSwitchDeviceId" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", entity->ServiceConfig()->TitleId()));

    CacheId retryCacheId = CacheId::AuthenticationLoginWithNintendoSwitchDeviceId;
    auto requestOp = entity->ServiceConfig()->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        std::move(requestBody),
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([entity, loginContext](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

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

AsyncOp<CombinedLoginResult> AuthenticationAPI::LoginWithOpenIdConnect(
    SharedPtr<GlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    const LoginWithOpenIdConnectRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LoginWithOpenIdConnect" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationLoginWithOpenIdConnect;

    auto requestOp = serviceConfig->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([state, serviceConfig, loginContext](Result<ServiceResponse> result) -> Result<CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginContext);
        }
        else
        {
            return Result<CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AuthenticationAPI::ReLoginWithOpenIdConnect(
    SharedPtr<Entity> entity,
    const LoginWithOpenIdConnectRequest& request,
    RunContext rc)
{
    const char* path{ "/Client/LoginWithOpenIdConnect" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", entity->ServiceConfig()->TitleId()));

    CacheId retryCacheId = CacheId::AuthenticationLoginWithOpenIdConnect;
    auto requestOp = entity->ServiceConfig()->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        std::move(requestBody),
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([entity, loginContext](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

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

AsyncOp<CombinedLoginResult> AuthenticationAPI::LoginWithPlayFab(
    SharedPtr<GlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    const LoginWithPlayFabRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LoginWithPlayFab" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationLoginWithPlayFab;

    auto requestOp = serviceConfig->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([state, serviceConfig, loginContext](Result<ServiceResponse> result) -> Result<CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginContext);
        }
        else
        {
            return Result<CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AuthenticationAPI::ReLoginWithPlayFab(
    SharedPtr<Entity> entity,
    const LoginWithPlayFabRequest& request,
    RunContext rc)
{
    const char* path{ "/Client/LoginWithPlayFab" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", entity->ServiceConfig()->TitleId()));

    CacheId retryCacheId = CacheId::AuthenticationLoginWithPlayFab;
    auto requestOp = entity->ServiceConfig()->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        std::move(requestBody),
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([entity, loginContext](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

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

AsyncOp<CombinedLoginResult> AuthenticationAPI::LoginWithPSN(
    SharedPtr<GlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    const LoginWithPSNRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LoginWithPSN" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationLoginWithPSN;

    auto requestOp = serviceConfig->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([state, serviceConfig, loginContext](Result<ServiceResponse> result) -> Result<CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginContext);
        }
        else
        {
            return Result<CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AuthenticationAPI::ReLoginWithPSN(
    SharedPtr<Entity> entity,
    const LoginWithPSNRequest& request,
    RunContext rc)
{
    const char* path{ "/Client/LoginWithPSN" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", entity->ServiceConfig()->TitleId()));

    CacheId retryCacheId = CacheId::AuthenticationLoginWithPSN;
    auto requestOp = entity->ServiceConfig()->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        std::move(requestBody),
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([entity, loginContext](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

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

AsyncOp<CombinedLoginResult> AuthenticationAPI::LoginWithSteam(
    SharedPtr<GlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    const LoginWithSteamRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LoginWithSteam" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationLoginWithSteam;

    auto requestOp = serviceConfig->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([state, serviceConfig, loginContext](Result<ServiceResponse> result) -> Result<CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginContext);
        }
        else
        {
            return Result<CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AuthenticationAPI::ReLoginWithSteam(
    SharedPtr<Entity> entity,
    const LoginWithSteamRequest& request,
    RunContext rc)
{
    const char* path{ "/Client/LoginWithSteam" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", entity->ServiceConfig()->TitleId()));

    CacheId retryCacheId = CacheId::AuthenticationLoginWithSteam;
    auto requestOp = entity->ServiceConfig()->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        std::move(requestBody),
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([entity, loginContext](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

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

AsyncOp<CombinedLoginResult> AuthenticationAPI::LoginWithTwitch(
    SharedPtr<GlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    const LoginWithTwitchRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LoginWithTwitch" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationLoginWithTwitch;

    auto requestOp = serviceConfig->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([state, serviceConfig, loginContext](Result<ServiceResponse> result) -> Result<CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginContext);
        }
        else
        {
            return Result<CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AuthenticationAPI::ReLoginWithTwitch(
    SharedPtr<Entity> entity,
    const LoginWithTwitchRequest& request,
    RunContext rc)
{
    const char* path{ "/Client/LoginWithTwitch" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", entity->ServiceConfig()->TitleId()));

    CacheId retryCacheId = CacheId::AuthenticationLoginWithTwitch;
    auto requestOp = entity->ServiceConfig()->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        std::move(requestBody),
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([entity, loginContext](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

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

AsyncOp<CombinedLoginResult> AuthenticationAPI::LoginWithXbox(
    SharedPtr<GlobalState> state,
    SharedPtr<ServiceConfig const> serviceConfig,
    const LoginWithXboxRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/LoginWithXbox" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", serviceConfig->TitleId()));
    CacheId retryCacheId = CacheId::AuthenticationLoginWithXbox;

    auto requestOp = serviceConfig->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        requestBody,
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([state, serviceConfig, loginContext](Result<ServiceResponse> result) -> Result<CombinedLoginResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return CombinedLoginResult::FromJson(serviceResponse.Data, state, serviceConfig, loginContext);
        }
        else
        {
            return Result<CombinedLoginResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> AuthenticationAPI::ReLoginWithXbox(
    SharedPtr<Entity> entity,
    const LoginWithXboxRequest& request,
    RunContext rc)
{
    const char* path{ "/Client/LoginWithXbox" };
    JsonValue requestBody{ request.ToJson() };
    RETURN_IF_FAILED(JsonUtils::ObjectAddMember(requestBody, "TitleId", entity->ServiceConfig()->TitleId()));

    CacheId retryCacheId = CacheId::AuthenticationLoginWithXbox;
    auto requestOp = entity->ServiceConfig()->HttpClient()->MakePostRequest(
        path,
        UnorderedMap<String, String>{},
        std::move(requestBody),
        retryCacheId,
        rc.Derive()
    );

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([entity, loginContext](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

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

AsyncOp<void> AuthenticationAPI::SetPlayerSecret(
    SharedPtr<Entity> entity,
    const SetPlayerSecretRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/SetPlayerSecret" };
    JsonValue requestBody{ request.ToJson() };
    CacheId retryCacheId = CacheId::AuthenticationSetPlayerSecret;

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

AsyncOp<ServerCombinedLoginResult> AuthenticationAPI::ServerLoginWithServerCustomId(
    SharedPtr<GlobalState> state,
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

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([state, serviceConfig, loginContext, secretKey](Result<ServiceResponse> result) -> Result<ServerCombinedLoginResult>
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
    SharedPtr<GlobalState> state,
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

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([state, serviceConfig, loginContext, secretKey](Result<ServiceResponse> result) -> Result<ServerCombinedLoginResult>
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
    SharedPtr<GlobalState> state,
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

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([state, serviceConfig, loginContext, secretKey](Result<ServiceResponse> result) -> Result<ServerCombinedLoginResult>
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
    SharedPtr<GlobalState> state,
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

    SharedPtr<LoginContext> loginContext = MakeShared<LoginContext>(path, std::move(requestBody), retryCacheId);
    return requestOp.Then([state, serviceConfig, loginContext, secretKey](Result<ServiceResponse> result) -> Result<ServerCombinedLoginResult>
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

#if HC_PLATFORM != HC_PLATFORM_GDK
AsyncOp<AuthenticateCustomIdResult> AuthenticationAPI::AuthenticateGameServerWithCustomId(
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

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<AuthenticateCustomIdResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            AuthenticateCustomIdResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<AuthenticateCustomIdResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}
#endif

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
    SharedPtr<GlobalState> state,
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
    SharedPtr<GlobalState> state,
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

#include "stdafx.h"
#include "PlatformSpecific.h"
#include "GlobalState.h"

namespace PlayFab
{
namespace PlatformSpecific
{


AsyncOp<void> PlatformSpecificAPI::ClientAndroidDevicePushNotificationRegistration(
    Entity const& entity,
    const AndroidDevicePushNotificationRegistrationRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/AndroidDevicePushNotificationRegistration" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::PlatformSpecificClientAndroidDevicePushNotificationRegistration,
        entity,
        path,
        requestBody,
        std::move(rc)
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

AsyncOp<void> PlatformSpecificAPI::ClientRefreshPSNAuthToken(
    Entity const& entity,
    const RefreshPSNAuthTokenRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/RefreshPSNAuthToken" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::PlatformSpecificClientRefreshPSNAuthToken,
        entity,
        path,
        requestBody,
        std::move(rc)
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

AsyncOp<void> PlatformSpecificAPI::ClientRegisterForIOSPushNotification(
    Entity const& entity,
    const RegisterForIOSPushNotificationRequest& request,
    RunContext rc
)
{
    const char* path{ "/Client/RegisterForIOSPushNotification" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeEntityRequest(
        ServicesCacheId::PlatformSpecificClientRegisterForIOSPushNotification,
        entity,
        path,
        requestBody,
        std::move(rc)
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

AsyncOp<AwardSteamAchievementResult> PlatformSpecificAPI::ServerAwardSteamAchievement(
    Entity const& entity,
    const AwardSteamAchievementRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/AwardSteamAchievement" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::PlatformSpecificServerAwardSteamAchievement,
        entity,
        path,
        requestBody,
        std::move(rc)
    );

    return requestOp.Then([](Result<ServiceResponse> result) -> Result<AwardSteamAchievementResult>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            AwardSteamAchievementResult resultModel;
            RETURN_IF_FAILED(resultModel.FromJson(serviceResponse.Data));
            return resultModel;
        }
        else
        {
            return Result<AwardSteamAchievementResult>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

} // namespace PlatformSpecific
} // namespace PlayFab

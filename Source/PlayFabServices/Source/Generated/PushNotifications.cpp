#include "stdafx.h"
#include "PushNotifications.h"
#include "GlobalState.h"

namespace PlayFab
{
namespace PushNotifications
{


AsyncOp<void> PushNotificationsAPI::ServerSendPushNotification(
    Entity const& entity,
    const SendPushNotificationRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/SendPushNotification" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::PushNotificationsServerSendPushNotification,
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

AsyncOp<void> PushNotificationsAPI::ServerSendPushNotificationFromTemplate(
    Entity const& entity,
    const SendPushNotificationFromTemplateRequest& request,
    RunContext rc
)
{
    const char* path{ "/Server/SendPushNotificationFromTemplate" };
    JsonValue requestBody{ request.ToJson() };

    auto requestOp = ServicesHttpClient::MakeSecretKeyRequest(
        ServicesCacheId::PushNotificationsServerSendPushNotificationFromTemplate,
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

} // namespace PushNotifications
} // namespace PlayFab

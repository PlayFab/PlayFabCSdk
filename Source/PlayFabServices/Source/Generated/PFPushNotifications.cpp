#include "stdafx.h"
#include <playfab/services/PFPushNotifications.h>
#include "Generated/PushNotifications.h"
#include "ApiXAsyncProvider.h"
#include "GlobalState.h"
#include <playfab/core/cpp/Entity.h>

using namespace PlayFab;
using namespace PlayFab::PushNotifications;

#if HC_PLATFORM == HC_PLATFORM_WIN32
PF_API PFPushNotificationsServerSendPushNotificationAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPushNotificationsSendPushNotificationRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPushNotificationsServerSendPushNotificationAsync),
        std::bind(&PushNotificationsAPI::ServerSendPushNotification, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
PF_API PFPushNotificationsServerSendPushNotificationFromTemplateAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFPushNotificationsSendPushNotificationFromTemplateRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFPushNotificationsServerSendPushNotificationFromTemplateAsync),
        std::bind(&PushNotificationsAPI::ServerSendPushNotificationFromTemplate, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}
#endif


#pragma once

#include <playfab/services/PFTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "PushNotificationsTypes.h"
#include "GlobalState.h"
#include "Generated/Types.h"

namespace PlayFab
{
namespace PushNotifications
{

class PushNotificationsAPI
{
public:
    PushNotificationsAPI() = delete;
    PushNotificationsAPI(const PushNotificationsAPI& source) = delete;
    PushNotificationsAPI& operator=(const PushNotificationsAPI& source) = delete;
    ~PushNotificationsAPI() = default;

    // ------------ Generated API calls
    static AsyncOp<void> ServerSendPushNotification(Entity const& entity, const SendPushNotificationRequest& request, RunContext rc);
    static AsyncOp<void> ServerSendPushNotificationFromTemplate(Entity const& entity, const SendPushNotificationFromTemplateRequest& request, RunContext rc);
};

} // namespace PushNotifications
} // namespace PlayFab

#pragma once

#include <playfab/services/PFTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "PlatformSpecificTypes.h"
#include "GlobalState.h"
#include "Generated/Types.h"

namespace PlayFab
{
namespace PlatformSpecific
{

class PlatformSpecificAPI
{
public:
    PlatformSpecificAPI() = delete;
    PlatformSpecificAPI(const PlatformSpecificAPI& source) = delete;
    PlatformSpecificAPI& operator=(const PlatformSpecificAPI& source) = delete;
    ~PlatformSpecificAPI() = default;

    // ------------ Generated API calls
    static AsyncOp<void> ClientAndroidDevicePushNotificationRegistration(Entity const& entity, const AndroidDevicePushNotificationRegistrationRequest& request, RunContext rc);
    static AsyncOp<void> ClientRefreshPSNAuthToken(Entity const& entity, const RefreshPSNAuthTokenRequest& request, RunContext rc);
    static AsyncOp<void> ClientRegisterForIOSPushNotification(Entity const& entity, const RegisterForIOSPushNotificationRequest& request, RunContext rc);
    static AsyncOp<AwardSteamAchievementResult> ServerAwardSteamAchievement(Entity const& entity, const AwardSteamAchievementRequest& request, RunContext rc);
};

} // namespace PlatformSpecific
} // namespace PlayFab

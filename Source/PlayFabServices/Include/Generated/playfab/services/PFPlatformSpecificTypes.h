// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/PFPal.h>
#include <playfab/core/PFTypes.h>
#include <playfab/services/PFTypes.h>

extern "C"
{

#pragma push_macro("IN")
#undef IN

/// <summary>
/// PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequest data model. More information can
/// be found on configuring your game for the Google Cloud Messaging service in the Google developer documentation,
/// here: http://developer.android.com/google/gcm/client.html. The steps to configure and send Push Notifications
/// is described in the PlayFab tutorials, here: https://docs.microsoft.com/gaming/playfab/features/engagement/push-notifications/quickstart.
/// </summary>
typedef struct PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequest
{
    /// <summary>
    /// (Optional) Message to display when confirming push notification.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* confirmationMessage;

    /// <summary>
    /// Registration ID provided by the Google Cloud Messaging service when the title registered to receive
    /// push notifications (see the GCM documentation, here: http://developer.android.com/google/gcm/client.html).
    /// </summary>
    _Null_terminated_ const char* deviceToken;

    /// <summary>
    /// (Optional) If true, send a test push message immediately after sucessful registration. Defaults
    /// to false.
    /// </summary>
    _Maybenull_ bool const* sendPushNotificationConfirmation;

} PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequest;

/// <summary>
/// PFPlatformSpecificRefreshPSNAuthTokenRequest data model.
/// </summary>
typedef struct PFPlatformSpecificRefreshPSNAuthTokenRequest
{
    /// <summary>
    /// Auth code returned by PlayStation :tm: Network OAuth system.
    /// </summary>
    _Null_terminated_ const char* authCode;

    /// <summary>
    /// (Optional) Id of the PlayStation :tm: Network issuer environment. If null, defaults to production
    /// environment.
    /// </summary>
    _Maybenull_ int32_t const* issuerId;

    /// <summary>
    /// Redirect URI supplied to PlayStation :tm: Network when requesting an auth code.
    /// </summary>
    _Null_terminated_ const char* redirectUri;

} PFPlatformSpecificRefreshPSNAuthTokenRequest;

/// <summary>
/// PFPlatformSpecificRegisterForIOSPushNotificationRequest data model. The steps to configure and send
/// Push Notifications is described in the PlayFab tutorials, here: https://docs.microsoft.com/gaming/playfab/features/engagement/push-notifications/quickstart.
/// </summary>
typedef struct PFPlatformSpecificRegisterForIOSPushNotificationRequest
{
    /// <summary>
    /// (Optional) Message to display when confirming push notification.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* confirmationMessage;

    /// <summary>
    /// Unique token generated by the Apple Push Notification service when the title registered to receive
    /// push notifications.
    /// </summary>
    _Null_terminated_ const char* deviceToken;

    /// <summary>
    /// (Optional) If true, send a test push message immediately after sucessful registration. Defaults
    /// to false.
    /// </summary>
    _Maybenull_ bool const* sendPushNotificationConfirmation;

} PFPlatformSpecificRegisterForIOSPushNotificationRequest;

/// <summary>
/// PFPlatformSpecificAwardSteamAchievementItem data model.
/// </summary>
typedef struct PFPlatformSpecificAwardSteamAchievementItem
{
    /// <summary>
    /// Unique Steam achievement name.
    /// </summary>
    _Null_terminated_ const char* achievementName;

    /// <summary>
    /// Unique PlayFab assigned ID of the user on whom the operation will be performed.
    /// </summary>
    _Null_terminated_ const char* playFabId;

    /// <summary>
    /// Result of the award attempt (only valid on response, not on request).
    /// </summary>
    bool result;

} PFPlatformSpecificAwardSteamAchievementItem;

/// <summary>
/// PFPlatformSpecificAwardSteamAchievementRequest data model.
/// </summary>
typedef struct PFPlatformSpecificAwardSteamAchievementRequest
{
    /// <summary>
    /// Array of achievements to grant and the users to whom they are to be granted.
    /// </summary>
    _Field_size_(achievementsCount) PFPlatformSpecificAwardSteamAchievementItem const* const* achievements;

    /// <summary>
    /// Count of achievements
    /// </summary>
    uint32_t achievementsCount;

} PFPlatformSpecificAwardSteamAchievementRequest;

/// <summary>
/// PFPlatformSpecificAwardSteamAchievementResult data model.
/// </summary>
typedef struct PFPlatformSpecificAwardSteamAchievementResult
{
    /// <summary>
    /// (Optional) Array of achievements granted.
    /// </summary>
    _Maybenull_ _Field_size_(achievementResultsCount) PFPlatformSpecificAwardSteamAchievementItem const* const* achievementResults;

    /// <summary>
    /// Count of achievementResults
    /// </summary>
    uint32_t achievementResultsCount;

} PFPlatformSpecificAwardSteamAchievementResult;

#pragma pop_macro("IN")

}

// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

namespace PlayFab
{

/// <summary>
/// Internal cacheId used by libHttpClient for Retry-After caching
/// </summary>
enum class CacheId : uint32_t
{
    AuthenticationGetPhotonAuthenticationToken = 1001,
    AuthenticationGetTitlePublicKey = 1002,
    AuthenticationLoginWithAndroidDeviceID = 1003,
    AuthenticationLoginWithApple = 1004,
    AuthenticationLoginWithCustomID = 1005,
    AuthenticationLoginWithEmailAddress = 1006,
    AuthenticationLoginWithFacebook = 1007,
    AuthenticationLoginWithFacebookInstantGamesId = 1008,
    AuthenticationLoginWithGameCenter = 1009,
    AuthenticationLoginWithGoogleAccount = 1010,
    AuthenticationLoginWithGooglePlayGamesServices = 1011,
    AuthenticationLoginWithIOSDeviceID = 1012,
    AuthenticationLoginWithKongregate = 1013,
    AuthenticationLoginWithNintendoServiceAccount = 1014,
    AuthenticationLoginWithNintendoSwitchDeviceId = 1015,
    AuthenticationLoginWithOpenIdConnect = 1016,
    AuthenticationLoginWithPlayFab = 1017,
    AuthenticationLoginWithPSN = 1018,
    AuthenticationLoginWithSteam = 1019,
    AuthenticationLoginWithTwitch = 1020,
    AuthenticationLoginWithXbox = 1021,
    AuthenticationRegisterPlayFabUser = 1022,
    AuthenticationSetPlayerSecret = 1023,
    AuthenticationServerLoginWithServerCustomId = 1024,
    AuthenticationServerLoginWithSteamId = 1025,
    AuthenticationServerLoginWithXbox = 1026,
    AuthenticationServerLoginWithXboxId = 1027,
    AuthenticationAuthenticateGameServerWithCustomId = 1028,
    AuthenticationDelete = 1029,
    AuthenticationGetEntity = 1030,
    AuthenticationGetEntityWithSecretKey = 1031,
    AuthenticationValidateEntityToken = 1032,
    EventsCreateTelemetryKey = 1033,
    EventsDeleteTelemetryKey = 1034,
    EventsGetTelemetryKey = 1035,
    EventsListTelemetryKeys = 1036,
    EventsSetTelemetryKeyActive = 1037,
    EventsWriteEvents = 1038,
    EventsWriteTelemetryEvents = 1039,
};

} // namespace PlayFab

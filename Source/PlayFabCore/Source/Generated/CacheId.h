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
    AuthenticationLoginWithAndroidDeviceID = 1001,
    AuthenticationLoginWithApple = 1002,
    AuthenticationLoginWithCustomID = 1003,
    AuthenticationLoginWithEmailAddress = 1004,
    AuthenticationLoginWithFacebook = 1005,
    AuthenticationLoginWithFacebookInstantGamesId = 1006,
    AuthenticationLoginWithGameCenter = 1007,
    AuthenticationLoginWithGoogleAccount = 1008,
    AuthenticationLoginWithGooglePlayGamesServices = 1009,
    AuthenticationLoginWithIOSDeviceID = 1010,
    AuthenticationLoginWithKongregate = 1011,
    AuthenticationLoginWithNintendoServiceAccount = 1012,
    AuthenticationLoginWithNintendoSwitchDeviceId = 1013,
    AuthenticationLoginWithOpenIdConnect = 1014,
    AuthenticationLoginWithPlayFab = 1015,
    AuthenticationLoginWithPSN = 1016,
    AuthenticationLoginWithSteam = 1017,
    AuthenticationLoginWithTwitch = 1018,
    AuthenticationLoginWithXbox = 1019,
    AuthenticationRegisterPlayFabUser = 1020,
    AuthenticationServerLoginWithPSN = 1021,
    AuthenticationServerLoginWithServerCustomId = 1022,
    AuthenticationServerLoginWithSteamId = 1023,
    AuthenticationServerLoginWithXbox = 1024,
    AuthenticationServerLoginWithXboxId = 1025,
    AuthenticationAuthenticateGameServerWithCustomId = 1026,
    AuthenticationDelete = 1027,
    AuthenticationGetEntity = 1028,
    AuthenticationGetEntityWithSecretKey = 1029,
    AuthenticationValidateEntityToken = 1030,
    EventsWriteEvents = 1031,
    EventsWriteTelemetryEvents = 1032,
};

} // namespace PlayFab

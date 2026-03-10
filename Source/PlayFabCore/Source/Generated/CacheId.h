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
    AuthenticationLoginWithBattleNet = 1003,
    AuthenticationLoginWithCustomID = 1004,
    AuthenticationLoginWithEmailAddress = 1005,
    AuthenticationLoginWithFacebook = 1006,
    AuthenticationLoginWithFacebookInstantGamesId = 1007,
    AuthenticationLoginWithGameCenter = 1008,
    AuthenticationLoginWithGoogleAccount = 1009,
    AuthenticationLoginWithGooglePlayGamesServices = 1010,
    AuthenticationLoginWithIOSDeviceID = 1011,
    AuthenticationLoginWithKongregate = 1012,
    AuthenticationLoginWithNintendoServiceAccount = 1013,
    AuthenticationLoginWithNintendoSwitchDeviceId = 1014,
    AuthenticationLoginWithOpenIdConnect = 1015,
    AuthenticationLoginWithPlayFab = 1016,
    AuthenticationLoginWithPSN = 1017,
    AuthenticationLoginWithSteam = 1018,
    AuthenticationLoginWithTwitch = 1019,
    AuthenticationLoginWithXbox = 1020,
    AuthenticationRegisterPlayFabUser = 1021,
    AuthenticationServerLoginWithAndroidDeviceID = 1022,
    AuthenticationServerLoginWithBattleNet = 1023,
    AuthenticationServerLoginWithCustomID = 1024,
    AuthenticationServerLoginWithIOSDeviceID = 1025,
    AuthenticationServerLoginWithPSN = 1026,
    AuthenticationServerLoginWithServerCustomId = 1027,
    AuthenticationServerLoginWithSteamId = 1028,
    AuthenticationServerLoginWithTwitch = 1029,
    AuthenticationServerLoginWithXbox = 1030,
    AuthenticationServerLoginWithXboxId = 1031,
    AuthenticationAuthenticateGameServerWithCustomId = 1032,
    AuthenticationDelete = 1033,
    AuthenticationGetEntity = 1034,
    AuthenticationGetEntityWithSecretKey = 1035,
    AuthenticationValidateEntityToken = 1036,
    EventsDeleteDataConnection = 1037,
    EventsGetDataConnection = 1038,
    EventsListDataConnections = 1039,
    EventsSetDataConnection = 1040,
    EventsSetDataConnectionActive = 1041,
    EventsWriteEvents = 1042,
    EventsWriteTelemetryEvents = 1043,
};

} // namespace PlayFab

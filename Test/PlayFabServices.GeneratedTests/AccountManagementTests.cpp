#include "TestAppPch.h"
#include "AccountManagementTests.h"

namespace PlayFab
{
namespace Test
{

void AccountManagementTests::AddTests()
{
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientAddOrUpdateContactEmail", &AccountManagementTests::TestClientAddOrUpdateContactEmail);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientAddUsernamePassword", &AccountManagementTests::TestClientAddUsernamePassword);
#endif
    AddTest("TestClientGetAccountInfo", &AccountManagementTests::TestClientGetAccountInfo);
    AddTest("TestClientGetPlayerCombinedInfo", &AccountManagementTests::TestClientGetPlayerCombinedInfo);
    AddTest("TestClientGetPlayerProfile", &AccountManagementTests::TestClientGetPlayerProfile);
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID
    AddTest("TestClientGetPlayFabIDsFromFacebookIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromFacebookIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientGetPlayFabIDsFromFacebookInstantGamesIds", &AccountManagementTests::TestClientGetPlayFabIDsFromFacebookInstantGamesIds);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientGetPlayFabIDsFromGameCenterIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromGameCenterIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID
    AddTest("TestClientGetPlayFabIDsFromGoogleIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromGoogleIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID
    AddTest("TestClientGetPlayFabIDsFromGooglePlayGamesPlayerIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromGooglePlayGamesPlayerIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientGetPlayFabIDsFromKongregateIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromKongregateIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientGetPlayFabIDsFromNintendoServiceAccountIds", &AccountManagementTests::TestClientGetPlayFabIDsFromNintendoServiceAccountIds);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientGetPlayFabIDsFromNintendoSwitchDeviceIds", &AccountManagementTests::TestClientGetPlayFabIDsFromNintendoSwitchDeviceIds);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientGetPlayFabIDsFromPSNAccountIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromPSNAccountIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientGetPlayFabIDsFromSteamIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromSteamIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientGetPlayFabIDsFromTwitchIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromTwitchIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientGetPlayFabIDsFromXboxLiveIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromXboxLiveIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID
    AddTest("TestClientLinkAndroidDeviceID", &AccountManagementTests::TestClientLinkAndroidDeviceID);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientLinkApple", &AccountManagementTests::TestClientLinkApple);
#endif
    AddTest("TestClientLinkCustomID", &AccountManagementTests::TestClientLinkCustomID);
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID
    AddTest("TestClientLinkFacebookAccount", &AccountManagementTests::TestClientLinkFacebookAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientLinkFacebookInstantGamesId", &AccountManagementTests::TestClientLinkFacebookInstantGamesId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientLinkGameCenterAccount", &AccountManagementTests::TestClientLinkGameCenterAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID
    AddTest("TestClientLinkGoogleAccount", &AccountManagementTests::TestClientLinkGoogleAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID
    AddTest("TestClientLinkGooglePlayGamesServicesAccount", &AccountManagementTests::TestClientLinkGooglePlayGamesServicesAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientLinkIOSDeviceID", &AccountManagementTests::TestClientLinkIOSDeviceID);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientLinkKongregate", &AccountManagementTests::TestClientLinkKongregate);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientLinkNintendoServiceAccount", &AccountManagementTests::TestClientLinkNintendoServiceAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientLinkNintendoSwitchDeviceId", &AccountManagementTests::TestClientLinkNintendoSwitchDeviceId);
#endif
    AddTest("TestClientLinkOpenIdConnect", &AccountManagementTests::TestClientLinkOpenIdConnect);
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
    AddTest("TestClientLinkPSNAccount", &AccountManagementTests::TestClientLinkPSNAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientLinkSteamAccount", &AccountManagementTests::TestClientLinkSteamAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientLinkTwitch", &AccountManagementTests::TestClientLinkTwitch);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientLinkXboxAccount", &AccountManagementTests::TestClientLinkXboxAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientRemoveContactEmail", &AccountManagementTests::TestClientRemoveContactEmail);
#endif
    AddTest("TestClientReportPlayer", &AccountManagementTests::TestClientReportPlayer);
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientSendAccountRecoveryEmail", &AccountManagementTests::TestClientSendAccountRecoveryEmail);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID
    AddTest("TestClientUnlinkAndroidDeviceID", &AccountManagementTests::TestClientUnlinkAndroidDeviceID);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientUnlinkApple", &AccountManagementTests::TestClientUnlinkApple);
#endif
    AddTest("TestClientUnlinkCustomID", &AccountManagementTests::TestClientUnlinkCustomID);
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID
    AddTest("TestClientUnlinkFacebookAccount", &AccountManagementTests::TestClientUnlinkFacebookAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientUnlinkFacebookInstantGamesId", &AccountManagementTests::TestClientUnlinkFacebookInstantGamesId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientUnlinkGameCenterAccount", &AccountManagementTests::TestClientUnlinkGameCenterAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID
    AddTest("TestClientUnlinkGoogleAccount", &AccountManagementTests::TestClientUnlinkGoogleAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID
    AddTest("TestClientUnlinkGooglePlayGamesServicesAccount", &AccountManagementTests::TestClientUnlinkGooglePlayGamesServicesAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientUnlinkIOSDeviceID", &AccountManagementTests::TestClientUnlinkIOSDeviceID);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientUnlinkKongregate", &AccountManagementTests::TestClientUnlinkKongregate);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientUnlinkNintendoServiceAccount", &AccountManagementTests::TestClientUnlinkNintendoServiceAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientUnlinkNintendoSwitchDeviceId", &AccountManagementTests::TestClientUnlinkNintendoSwitchDeviceId);
#endif
    AddTest("TestClientUnlinkOpenIdConnect", &AccountManagementTests::TestClientUnlinkOpenIdConnect);
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientUnlinkPSNAccount", &AccountManagementTests::TestClientUnlinkPSNAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientUnlinkSteamAccount", &AccountManagementTests::TestClientUnlinkSteamAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientUnlinkTwitch", &AccountManagementTests::TestClientUnlinkTwitch);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientUnlinkXboxAccount", &AccountManagementTests::TestClientUnlinkXboxAccount);
#endif
    AddTest("TestClientUpdateAvatarUrl", &AccountManagementTests::TestClientUpdateAvatarUrl);
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestClientUpdateUserTitleDisplayName", &AccountManagementTests::TestClientUpdateUserTitleDisplayName);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerBanUsers", &AccountManagementTests::TestServerBanUsers);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerDeletePlayer", &AccountManagementTests::TestServerDeletePlayer);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerGetPlayerCombinedInfo", &AccountManagementTests::TestServerGetPlayerCombinedInfo);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerGetPlayerProfile", &AccountManagementTests::TestServerGetPlayerProfile);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID
    AddTest("TestServerGetPlayFabIDsFromFacebookIDs", &AccountManagementTests::TestServerGetPlayFabIDsFromFacebookIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerGetPlayFabIDsFromFacebookInstantGamesIds", &AccountManagementTests::TestServerGetPlayFabIDsFromFacebookInstantGamesIds);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerGetPlayFabIDsFromNintendoServiceAccountIds", &AccountManagementTests::TestServerGetPlayFabIDsFromNintendoServiceAccountIds);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerGetPlayFabIDsFromNintendoSwitchDeviceIds", &AccountManagementTests::TestServerGetPlayFabIDsFromNintendoSwitchDeviceIds);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerGetPlayFabIDsFromPSNAccountIDs", &AccountManagementTests::TestServerGetPlayFabIDsFromPSNAccountIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerGetPlayFabIDsFromSteamIDs", &AccountManagementTests::TestServerGetPlayFabIDsFromSteamIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerGetPlayFabIDsFromTwitchIDs", &AccountManagementTests::TestServerGetPlayFabIDsFromTwitchIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerGetPlayFabIDsFromXboxLiveIDs", &AccountManagementTests::TestServerGetPlayFabIDsFromXboxLiveIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerGetServerCustomIDsFromPlayFabIDs", &AccountManagementTests::TestServerGetServerCustomIDsFromPlayFabIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerGetUserAccountInfo", &AccountManagementTests::TestServerGetUserAccountInfo);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerGetUserBans", &AccountManagementTests::TestServerGetUserBans);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerLinkNintendoServiceAccount", &AccountManagementTests::TestServerLinkNintendoServiceAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerLinkNintendoServiceAccountSubject", &AccountManagementTests::TestServerLinkNintendoServiceAccountSubject);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerLinkNintendoSwitchDeviceId", &AccountManagementTests::TestServerLinkNintendoSwitchDeviceId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerLinkPSNAccount", &AccountManagementTests::TestServerLinkPSNAccount);
#endif
#if 0
    AddTest("TestServerLinkPSNId", &AccountManagementTests::TestServerLinkPSNId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerLinkServerCustomId", &AccountManagementTests::TestServerLinkServerCustomId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerLinkSteamId", &AccountManagementTests::TestServerLinkSteamId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerLinkXboxAccount", &AccountManagementTests::TestServerLinkXboxAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerRevokeAllBansForUser", &AccountManagementTests::TestServerRevokeAllBansForUser);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerRevokeBans", &AccountManagementTests::TestServerRevokeBans);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerSendCustomAccountRecoveryEmail", &AccountManagementTests::TestServerSendCustomAccountRecoveryEmail);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerSendEmailFromTemplate", &AccountManagementTests::TestServerSendEmailFromTemplate);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerUnlinkNintendoServiceAccount", &AccountManagementTests::TestServerUnlinkNintendoServiceAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerUnlinkNintendoSwitchDeviceId", &AccountManagementTests::TestServerUnlinkNintendoSwitchDeviceId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerUnlinkPSNAccount", &AccountManagementTests::TestServerUnlinkPSNAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerUnlinkServerCustomId", &AccountManagementTests::TestServerUnlinkServerCustomId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerUnlinkSteamId", &AccountManagementTests::TestServerUnlinkSteamId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerUnlinkXboxAccount", &AccountManagementTests::TestServerUnlinkXboxAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerUpdateAvatarUrl", &AccountManagementTests::TestServerUpdateAvatarUrl);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerUpdateBans", &AccountManagementTests::TestServerUpdateBans);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestGetTitlePlayersFromXboxLiveIDs", &AccountManagementTests::TestGetTitlePlayersFromXboxLiveIDs);
#endif

}

}
}

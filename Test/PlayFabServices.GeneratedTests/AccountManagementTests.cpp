#include "TestAppPch.h"
#include "AccountManagementTests.h"

namespace PlayFab
{
namespace Test
{

void AccountManagementTests::AddTests()
{
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientAddOrUpdateContactEmail", &AccountManagementTests::TestClientAddOrUpdateContactEmail);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientAddUsernamePassword", &AccountManagementTests::TestClientAddUsernamePassword, true);
#endif
    AddTest("TestClientGetAccountInfo", &AccountManagementTests::TestClientGetAccountInfo);
    AddTest("TestClientGetPlayerCombinedInfo", &AccountManagementTests::TestClientGetPlayerCombinedInfo);
    AddTest("TestClientGetPlayerProfile", &AccountManagementTests::TestClientGetPlayerProfile);
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientGetPlayFabIDsFromFacebookIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromFacebookIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientGetPlayFabIDsFromFacebookInstantGamesIds", &AccountManagementTests::TestClientGetPlayFabIDsFromFacebookInstantGamesIds);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientGetPlayFabIDsFromGameCenterIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromGameCenterIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientGetPlayFabIDsFromGoogleIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromGoogleIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientGetPlayFabIDsFromGooglePlayGamesPlayerIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromGooglePlayGamesPlayerIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientGetPlayFabIDsFromKongregateIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromKongregateIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientGetPlayFabIDsFromNintendoServiceAccountIds", &AccountManagementTests::TestClientGetPlayFabIDsFromNintendoServiceAccountIds);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientGetPlayFabIDsFromNintendoSwitchDeviceIds", &AccountManagementTests::TestClientGetPlayFabIDsFromNintendoSwitchDeviceIds);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientGetPlayFabIDsFromPSNAccountIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromPSNAccountIDs);
#endif
#if 0
    AddTest("TestClientGetPlayFabIDsFromPSNOnlineIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromPSNOnlineIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientGetPlayFabIDsFromSteamIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromSteamIDs);
#endif
#if 0
    AddTest("TestClientGetPlayFabIDsFromSteamNames", &AccountManagementTests::TestClientGetPlayFabIDsFromSteamNames);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientGetPlayFabIDsFromTwitchIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromTwitchIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientGetPlayFabIDsFromXboxLiveIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromXboxLiveIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientLinkAndroidDeviceID", &AccountManagementTests::TestClientLinkAndroidDeviceID);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientLinkApple", &AccountManagementTests::TestClientLinkApple);
#endif
    AddTest("TestClientLinkCustomID", &AccountManagementTests::TestClientLinkCustomID, true);
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientLinkFacebookAccount", &AccountManagementTests::TestClientLinkFacebookAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientLinkFacebookInstantGamesId", &AccountManagementTests::TestClientLinkFacebookInstantGamesId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientLinkGameCenterAccount", &AccountManagementTests::TestClientLinkGameCenterAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientLinkGoogleAccount", &AccountManagementTests::TestClientLinkGoogleAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientLinkGooglePlayGamesServicesAccount", &AccountManagementTests::TestClientLinkGooglePlayGamesServicesAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientLinkIOSDeviceID", &AccountManagementTests::TestClientLinkIOSDeviceID);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientLinkKongregate", &AccountManagementTests::TestClientLinkKongregate);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientLinkNintendoServiceAccount", &AccountManagementTests::TestClientLinkNintendoServiceAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientLinkNintendoSwitchDeviceId", &AccountManagementTests::TestClientLinkNintendoSwitchDeviceId);
#endif
    AddTest("TestClientLinkOpenIdConnect", &AccountManagementTests::TestClientLinkOpenIdConnect);
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientLinkPSNAccount", &AccountManagementTests::TestClientLinkPSNAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientLinkSteamAccount", &AccountManagementTests::TestClientLinkSteamAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientLinkTwitch", &AccountManagementTests::TestClientLinkTwitch);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientLinkXboxAccount", &AccountManagementTests::TestClientLinkXboxAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientRemoveContactEmail", &AccountManagementTests::TestClientRemoveContactEmail);
#endif
    AddTest("TestClientReportPlayer", &AccountManagementTests::TestClientReportPlayer);
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientSendAccountRecoveryEmail", &AccountManagementTests::TestClientSendAccountRecoveryEmail);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientUnlinkAndroidDeviceID", &AccountManagementTests::TestClientUnlinkAndroidDeviceID);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientUnlinkApple", &AccountManagementTests::TestClientUnlinkApple);
#endif
    AddTest("TestClientUnlinkCustomID", &AccountManagementTests::TestClientUnlinkCustomID);
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientUnlinkFacebookAccount", &AccountManagementTests::TestClientUnlinkFacebookAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientUnlinkFacebookInstantGamesId", &AccountManagementTests::TestClientUnlinkFacebookInstantGamesId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientUnlinkGameCenterAccount", &AccountManagementTests::TestClientUnlinkGameCenterAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientUnlinkGoogleAccount", &AccountManagementTests::TestClientUnlinkGoogleAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientUnlinkGooglePlayGamesServicesAccount", &AccountManagementTests::TestClientUnlinkGooglePlayGamesServicesAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientUnlinkIOSDeviceID", &AccountManagementTests::TestClientUnlinkIOSDeviceID);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientUnlinkKongregate", &AccountManagementTests::TestClientUnlinkKongregate);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientUnlinkNintendoServiceAccount", &AccountManagementTests::TestClientUnlinkNintendoServiceAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientUnlinkNintendoSwitchDeviceId", &AccountManagementTests::TestClientUnlinkNintendoSwitchDeviceId);
#endif
    AddTest("TestClientUnlinkOpenIdConnect", &AccountManagementTests::TestClientUnlinkOpenIdConnect);
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientUnlinkPSNAccount", &AccountManagementTests::TestClientUnlinkPSNAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientUnlinkSteamAccount", &AccountManagementTests::TestClientUnlinkSteamAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientUnlinkTwitch", &AccountManagementTests::TestClientUnlinkTwitch);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientUnlinkXboxAccount", &AccountManagementTests::TestClientUnlinkXboxAccount);
#endif
    AddTest("TestClientUpdateAvatarUrl", &AccountManagementTests::TestClientUpdateAvatarUrl);
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestClientUpdateUserTitleDisplayName", &AccountManagementTests::TestClientUpdateUserTitleDisplayName);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerBanUsers", &AccountManagementTests::TestServerBanUsers);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerDeletePlayer", &AccountManagementTests::TestServerDeletePlayer);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetPlayerCombinedInfo", &AccountManagementTests::TestServerGetPlayerCombinedInfo);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetPlayerProfile", &AccountManagementTests::TestServerGetPlayerProfile);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetPlayFabIDsFromFacebookIDs", &AccountManagementTests::TestServerGetPlayFabIDsFromFacebookIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetPlayFabIDsFromFacebookInstantGamesIds", &AccountManagementTests::TestServerGetPlayFabIDsFromFacebookInstantGamesIds);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetPlayFabIDsFromNintendoServiceAccountIds", &AccountManagementTests::TestServerGetPlayFabIDsFromNintendoServiceAccountIds);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetPlayFabIDsFromNintendoSwitchDeviceIds", &AccountManagementTests::TestServerGetPlayFabIDsFromNintendoSwitchDeviceIds);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetPlayFabIDsFromPSNAccountIDs", &AccountManagementTests::TestServerGetPlayFabIDsFromPSNAccountIDs);
#endif
#if 0
    AddTest("TestServerGetPlayFabIDsFromPSNOnlineIDs", &AccountManagementTests::TestServerGetPlayFabIDsFromPSNOnlineIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetPlayFabIDsFromSteamIDs", &AccountManagementTests::TestServerGetPlayFabIDsFromSteamIDs);
#endif
#if 0
    AddTest("TestServerGetPlayFabIDsFromSteamNames", &AccountManagementTests::TestServerGetPlayFabIDsFromSteamNames);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetPlayFabIDsFromTwitchIDs", &AccountManagementTests::TestServerGetPlayFabIDsFromTwitchIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetPlayFabIDsFromXboxLiveIDs", &AccountManagementTests::TestServerGetPlayFabIDsFromXboxLiveIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetServerCustomIDsFromPlayFabIDs", &AccountManagementTests::TestServerGetServerCustomIDsFromPlayFabIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetUserAccountInfo", &AccountManagementTests::TestServerGetUserAccountInfo);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetUserBans", &AccountManagementTests::TestServerGetUserBans);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerLinkNintendoServiceAccount", &AccountManagementTests::TestServerLinkNintendoServiceAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerLinkNintendoServiceAccountSubject", &AccountManagementTests::TestServerLinkNintendoServiceAccountSubject);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerLinkNintendoSwitchDeviceId", &AccountManagementTests::TestServerLinkNintendoSwitchDeviceId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerLinkPSNAccount", &AccountManagementTests::TestServerLinkPSNAccount);
#endif
#if 0
    AddTest("TestServerLinkPSNId", &AccountManagementTests::TestServerLinkPSNId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerLinkServerCustomId", &AccountManagementTests::TestServerLinkServerCustomId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerLinkSteamId", &AccountManagementTests::TestServerLinkSteamId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerLinkXboxAccount", &AccountManagementTests::TestServerLinkXboxAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerRevokeAllBansForUser", &AccountManagementTests::TestServerRevokeAllBansForUser);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerRevokeBans", &AccountManagementTests::TestServerRevokeBans);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerSendCustomAccountRecoveryEmail", &AccountManagementTests::TestServerSendCustomAccountRecoveryEmail);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerSendEmailFromTemplate", &AccountManagementTests::TestServerSendEmailFromTemplate);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerUnlinkNintendoServiceAccount", &AccountManagementTests::TestServerUnlinkNintendoServiceAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerUnlinkNintendoSwitchDeviceId", &AccountManagementTests::TestServerUnlinkNintendoSwitchDeviceId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerUnlinkPSNAccount", &AccountManagementTests::TestServerUnlinkPSNAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerUnlinkServerCustomId", &AccountManagementTests::TestServerUnlinkServerCustomId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerUnlinkSteamId", &AccountManagementTests::TestServerUnlinkSteamId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerUnlinkXboxAccount", &AccountManagementTests::TestServerUnlinkXboxAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerUpdateAvatarUrl", &AccountManagementTests::TestServerUpdateAvatarUrl);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerUpdateBans", &AccountManagementTests::TestServerUpdateBans);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestGetTitlePlayersFromXboxLiveIDs", &AccountManagementTests::TestGetTitlePlayersFromXboxLiveIDs);
#endif
#if 0
    AddTest("TestSetDisplayName", &AccountManagementTests::TestSetDisplayName);
#endif

}

}
}

#include "TestAppPch.h"
#include "AccountManagementTests.h"

namespace PlayFab
{
namespace Test
{

void AccountManagementTests::AddTests()
{
#if 0
    AddTest("TestClientAddOrUpdateContactEmail", &AccountManagementTests::TestClientAddOrUpdateContactEmail);
#endif
#if 0
    AddTest("TestClientAddUsernamePassword", &AccountManagementTests::TestClientAddUsernamePassword);
#endif
    AddTest("TestClientGetAccountInfo", &AccountManagementTests::TestClientGetAccountInfo);
    AddTest("TestClientGetPlayerCombinedInfo", &AccountManagementTests::TestClientGetPlayerCombinedInfo);
    AddTest("TestClientGetPlayerProfile", &AccountManagementTests::TestClientGetPlayerProfile);
#if 0
    AddTest("TestClientGetPlayFabIDsFromFacebookIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromFacebookIDs);
#endif
#if 0
    AddTest("TestClientGetPlayFabIDsFromFacebookInstantGamesIds", &AccountManagementTests::TestClientGetPlayFabIDsFromFacebookInstantGamesIds);
#endif
#if 0
    AddTest("TestClientGetPlayFabIDsFromGameCenterIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromGameCenterIDs);
#endif
#if 0
    AddTest("TestClientGetPlayFabIDsFromGoogleIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromGoogleIDs);
#endif
#if 0
    AddTest("TestClientGetPlayFabIDsFromGooglePlayGamesPlayerIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromGooglePlayGamesPlayerIDs);
#endif
#if 0
    AddTest("TestClientGetPlayFabIDsFromKongregateIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromKongregateIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH
    AddTest("TestClientGetPlayFabIDsFromNintendoServiceAccountIds", &AccountManagementTests::TestClientGetPlayFabIDsFromNintendoServiceAccountIds);
#endif
#if 0
    AddTest("TestClientGetPlayFabIDsFromNintendoSwitchDeviceIds", &AccountManagementTests::TestClientGetPlayFabIDsFromNintendoSwitchDeviceIds);
#endif
#if HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
    AddTest("TestClientGetPlayFabIDsFromPSNAccountIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromPSNAccountIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientGetPlayFabIDsFromSteamIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromSteamIDs);
#endif
#if 0
    AddTest("TestClientGetPlayFabIDsFromTwitchIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromTwitchIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
    AddTest("TestClientGetPlayFabIDsFromXboxLiveIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromXboxLiveIDs);
#endif
#if 0
    AddTest("TestClientLinkAndroidDeviceID", &AccountManagementTests::TestClientLinkAndroidDeviceID);
#endif
#if 0
    AddTest("TestClientLinkApple", &AccountManagementTests::TestClientLinkApple);
#endif
    AddTest("TestClientLinkCustomID", &AccountManagementTests::TestClientLinkCustomID);
#if 0
    AddTest("TestClientLinkFacebookAccount", &AccountManagementTests::TestClientLinkFacebookAccount);
#endif
#if 0
    AddTest("TestClientLinkFacebookInstantGamesId", &AccountManagementTests::TestClientLinkFacebookInstantGamesId);
#endif
#if 0
    AddTest("TestClientLinkGameCenterAccount", &AccountManagementTests::TestClientLinkGameCenterAccount);
#endif
#if 0
    AddTest("TestClientLinkGoogleAccount", &AccountManagementTests::TestClientLinkGoogleAccount);
#endif
#if 0
    AddTest("TestClientLinkGooglePlayGamesServicesAccount", &AccountManagementTests::TestClientLinkGooglePlayGamesServicesAccount);
#endif
#if 0
    AddTest("TestClientLinkIOSDeviceID", &AccountManagementTests::TestClientLinkIOSDeviceID);
#endif
#if 0
    AddTest("TestClientLinkKongregate", &AccountManagementTests::TestClientLinkKongregate);
#endif
#if HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH
    AddTest("TestClientLinkNintendoServiceAccount", &AccountManagementTests::TestClientLinkNintendoServiceAccount);
#endif
#if 0
    AddTest("TestClientLinkNintendoSwitchDeviceId", &AccountManagementTests::TestClientLinkNintendoSwitchDeviceId);
#endif
    AddTest("TestClientLinkOpenIdConnect", &AccountManagementTests::TestClientLinkOpenIdConnect);
#if HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
    AddTest("TestClientLinkPSNAccount", &AccountManagementTests::TestClientLinkPSNAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientLinkSteamAccount", &AccountManagementTests::TestClientLinkSteamAccount);
#endif
#if 0
    AddTest("TestClientLinkTwitch", &AccountManagementTests::TestClientLinkTwitch);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
    AddTest("TestClientLinkXboxAccount", &AccountManagementTests::TestClientLinkXboxAccount);
#endif
#if 0
    AddTest("TestClientRemoveContactEmail", &AccountManagementTests::TestClientRemoveContactEmail);
#endif
    AddTest("TestClientReportPlayer", &AccountManagementTests::TestClientReportPlayer);
#if 0
    AddTest("TestClientSendAccountRecoveryEmail", &AccountManagementTests::TestClientSendAccountRecoveryEmail);
#endif
#if 0
    AddTest("TestClientUnlinkAndroidDeviceID", &AccountManagementTests::TestClientUnlinkAndroidDeviceID);
#endif
#if 0
    AddTest("TestClientUnlinkApple", &AccountManagementTests::TestClientUnlinkApple);
#endif
    AddTest("TestClientUnlinkCustomID", &AccountManagementTests::TestClientUnlinkCustomID);
#if 0
    AddTest("TestClientUnlinkFacebookAccount", &AccountManagementTests::TestClientUnlinkFacebookAccount);
#endif
#if 0
    AddTest("TestClientUnlinkFacebookInstantGamesId", &AccountManagementTests::TestClientUnlinkFacebookInstantGamesId);
#endif
#if 0
    AddTest("TestClientUnlinkGameCenterAccount", &AccountManagementTests::TestClientUnlinkGameCenterAccount);
#endif
#if 0
    AddTest("TestClientUnlinkGoogleAccount", &AccountManagementTests::TestClientUnlinkGoogleAccount);
#endif
#if 0
    AddTest("TestClientUnlinkGooglePlayGamesServicesAccount", &AccountManagementTests::TestClientUnlinkGooglePlayGamesServicesAccount);
#endif
#if 0
    AddTest("TestClientUnlinkIOSDeviceID", &AccountManagementTests::TestClientUnlinkIOSDeviceID);
#endif
#if 0
    AddTest("TestClientUnlinkKongregate", &AccountManagementTests::TestClientUnlinkKongregate);
#endif
#if HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH
    AddTest("TestClientUnlinkNintendoServiceAccount", &AccountManagementTests::TestClientUnlinkNintendoServiceAccount);
#endif
#if 0
    AddTest("TestClientUnlinkNintendoSwitchDeviceId", &AccountManagementTests::TestClientUnlinkNintendoSwitchDeviceId);
#endif
    AddTest("TestClientUnlinkOpenIdConnect", &AccountManagementTests::TestClientUnlinkOpenIdConnect);
#if HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
    AddTest("TestClientUnlinkPSNAccount", &AccountManagementTests::TestClientUnlinkPSNAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientUnlinkSteamAccount", &AccountManagementTests::TestClientUnlinkSteamAccount);
#endif
#if 0
    AddTest("TestClientUnlinkTwitch", &AccountManagementTests::TestClientUnlinkTwitch);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
    AddTest("TestClientUnlinkXboxAccount", &AccountManagementTests::TestClientUnlinkXboxAccount);
#endif
    AddTest("TestClientUpdateAvatarUrl", &AccountManagementTests::TestClientUpdateAvatarUrl);
#if 0
    AddTest("TestClientUpdateUserTitleDisplayName", &AccountManagementTests::TestClientUpdateUserTitleDisplayName);
#endif
#if 0
    AddTest("TestServerBanUsers", &AccountManagementTests::TestServerBanUsers);
#endif
#if 0
    AddTest("TestServerDeletePlayer", &AccountManagementTests::TestServerDeletePlayer);
#endif
#if 0
    AddTest("TestServerGetPlayerCombinedInfo", &AccountManagementTests::TestServerGetPlayerCombinedInfo);
#endif
#if 0
    AddTest("TestServerGetPlayerProfile", &AccountManagementTests::TestServerGetPlayerProfile);
#endif
#if 0
    AddTest("TestServerGetPlayFabIDsFromFacebookIDs", &AccountManagementTests::TestServerGetPlayFabIDsFromFacebookIDs);
#endif
#if 0
    AddTest("TestServerGetPlayFabIDsFromFacebookInstantGamesIds", &AccountManagementTests::TestServerGetPlayFabIDsFromFacebookInstantGamesIds);
#endif
#if 0
    AddTest("TestServerGetPlayFabIDsFromNintendoServiceAccountIds", &AccountManagementTests::TestServerGetPlayFabIDsFromNintendoServiceAccountIds);
#endif
#if 0
    AddTest("TestServerGetPlayFabIDsFromNintendoSwitchDeviceIds", &AccountManagementTests::TestServerGetPlayFabIDsFromNintendoSwitchDeviceIds);
#endif
#if 0
    AddTest("TestServerGetPlayFabIDsFromPSNAccountIDs", &AccountManagementTests::TestServerGetPlayFabIDsFromPSNAccountIDs);
#endif
#if 0
    AddTest("TestServerGetPlayFabIDsFromSteamIDs", &AccountManagementTests::TestServerGetPlayFabIDsFromSteamIDs);
#endif
#if 0
    AddTest("TestServerGetPlayFabIDsFromTwitchIDs", &AccountManagementTests::TestServerGetPlayFabIDsFromTwitchIDs);
#endif
#if 0
    AddTest("TestServerGetPlayFabIDsFromXboxLiveIDs", &AccountManagementTests::TestServerGetPlayFabIDsFromXboxLiveIDs);
#endif
#if 0
    AddTest("TestServerGetServerCustomIDsFromPlayFabIDs", &AccountManagementTests::TestServerGetServerCustomIDsFromPlayFabIDs);
#endif
#if 0
    AddTest("TestServerGetUserAccountInfo", &AccountManagementTests::TestServerGetUserAccountInfo);
#endif
#if 0
    AddTest("TestServerGetUserBans", &AccountManagementTests::TestServerGetUserBans);
#endif
#if 0
    AddTest("TestServerLinkNintendoServiceAccount", &AccountManagementTests::TestServerLinkNintendoServiceAccount);
#endif
#if 0
    AddTest("TestServerLinkNintendoSwitchDeviceId", &AccountManagementTests::TestServerLinkNintendoSwitchDeviceId);
#endif
#if 0
    AddTest("TestServerLinkPSNAccount", &AccountManagementTests::TestServerLinkPSNAccount);
#endif
#if 0
    AddTest("TestServerLinkPSNId", &AccountManagementTests::TestServerLinkPSNId);
#endif
#if 0
    AddTest("TestServerLinkServerCustomId", &AccountManagementTests::TestServerLinkServerCustomId);
#endif
#if 0
    AddTest("TestServerLinkSteamId", &AccountManagementTests::TestServerLinkSteamId);
#endif
#if 0
    AddTest("TestServerLinkXboxAccount", &AccountManagementTests::TestServerLinkXboxAccount);
#endif
#if 0
    AddTest("TestServerRevokeAllBansForUser", &AccountManagementTests::TestServerRevokeAllBansForUser);
#endif
#if 0
    AddTest("TestServerRevokeBans", &AccountManagementTests::TestServerRevokeBans);
#endif
#if 0
    AddTest("TestServerSendCustomAccountRecoveryEmail", &AccountManagementTests::TestServerSendCustomAccountRecoveryEmail);
#endif
#if 0
    AddTest("TestServerSendEmailFromTemplate", &AccountManagementTests::TestServerSendEmailFromTemplate);
#endif
#if 0
    AddTest("TestServerUnlinkNintendoServiceAccount", &AccountManagementTests::TestServerUnlinkNintendoServiceAccount);
#endif
#if 0
    AddTest("TestServerUnlinkNintendoSwitchDeviceId", &AccountManagementTests::TestServerUnlinkNintendoSwitchDeviceId);
#endif
#if 0
    AddTest("TestServerUnlinkPSNAccount", &AccountManagementTests::TestServerUnlinkPSNAccount);
#endif
#if 0
    AddTest("TestServerUnlinkServerCustomId", &AccountManagementTests::TestServerUnlinkServerCustomId);
#endif
#if 0
    AddTest("TestServerUnlinkSteamId", &AccountManagementTests::TestServerUnlinkSteamId);
#endif
#if 0
    AddTest("TestServerUnlinkXboxAccount", &AccountManagementTests::TestServerUnlinkXboxAccount);
#endif
#if 0
    AddTest("TestServerUpdateAvatarUrl", &AccountManagementTests::TestServerUpdateAvatarUrl);
#endif
#if 0
    AddTest("TestServerUpdateBans", &AccountManagementTests::TestServerUpdateBans);
#endif
#if 0
    AddTest("TestGetTitlePlayersFromXboxLiveIDs", &AccountManagementTests::TestGetTitlePlayersFromXboxLiveIDs);
#endif

}

}
}

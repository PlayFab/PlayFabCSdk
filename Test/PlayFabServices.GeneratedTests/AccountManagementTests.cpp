#include "TestAppPch.h"
#include "AccountManagementTests.h"

namespace PlayFab
{
namespace Test
{

void AccountManagementTests::AddTests()
{
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientAddOrUpdateContactEmail", &AccountManagementTests::TestClientAddOrUpdateContactEmail);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientAddUsernamePassword", &AccountManagementTests::TestClientAddUsernamePassword);
#endif
    AddTest("TestClientGetAccountInfo", &AccountManagementTests::TestClientGetAccountInfo);
    AddTest("TestClientGetPlayerCombinedInfo", &AccountManagementTests::TestClientGetPlayerCombinedInfo);
    AddTest("TestClientGetPlayerProfile", &AccountManagementTests::TestClientGetPlayerProfile);
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientGetPlayFabIDsFromFacebookIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromFacebookIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientGetPlayFabIDsFromFacebookInstantGamesIds", &AccountManagementTests::TestClientGetPlayFabIDsFromFacebookInstantGamesIds);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientGetPlayFabIDsFromGameCenterIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromGameCenterIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientGetPlayFabIDsFromGoogleIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromGoogleIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientGetPlayFabIDsFromGooglePlayGamesPlayerIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromGooglePlayGamesPlayerIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientGetPlayFabIDsFromKongregateIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromKongregateIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH
    AddTest("TestClientGetPlayFabIDsFromNintendoServiceAccountIds", &AccountManagementTests::TestClientGetPlayFabIDsFromNintendoServiceAccountIds);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientGetPlayFabIDsFromNintendoSwitchDeviceIds", &AccountManagementTests::TestClientGetPlayFabIDsFromNintendoSwitchDeviceIds);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
    AddTest("TestClientGetPlayFabIDsFromPSNAccountIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromPSNAccountIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientGetPlayFabIDsFromSteamIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromSteamIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientGetPlayFabIDsFromTwitchIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromTwitchIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
    AddTest("TestClientGetPlayFabIDsFromXboxLiveIDs", &AccountManagementTests::TestClientGetPlayFabIDsFromXboxLiveIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientLinkAndroidDeviceID", &AccountManagementTests::TestClientLinkAndroidDeviceID);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientLinkApple", &AccountManagementTests::TestClientLinkApple);
#endif
    AddTest("TestClientLinkCustomID", &AccountManagementTests::TestClientLinkCustomID);
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientLinkFacebookAccount", &AccountManagementTests::TestClientLinkFacebookAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientLinkFacebookInstantGamesId", &AccountManagementTests::TestClientLinkFacebookInstantGamesId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientLinkGameCenterAccount", &AccountManagementTests::TestClientLinkGameCenterAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientLinkGoogleAccount", &AccountManagementTests::TestClientLinkGoogleAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientLinkGooglePlayGamesServicesAccount", &AccountManagementTests::TestClientLinkGooglePlayGamesServicesAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientLinkIOSDeviceID", &AccountManagementTests::TestClientLinkIOSDeviceID);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientLinkKongregate", &AccountManagementTests::TestClientLinkKongregate);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH
    AddTest("TestClientLinkNintendoServiceAccount", &AccountManagementTests::TestClientLinkNintendoServiceAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientLinkNintendoSwitchDeviceId", &AccountManagementTests::TestClientLinkNintendoSwitchDeviceId);
#endif
    AddTest("TestClientLinkOpenIdConnect", &AccountManagementTests::TestClientLinkOpenIdConnect);
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
    AddTest("TestClientLinkPSNAccount", &AccountManagementTests::TestClientLinkPSNAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientLinkSteamAccount", &AccountManagementTests::TestClientLinkSteamAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientLinkTwitch", &AccountManagementTests::TestClientLinkTwitch);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
    AddTest("TestClientLinkXboxAccount", &AccountManagementTests::TestClientLinkXboxAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientRemoveContactEmail", &AccountManagementTests::TestClientRemoveContactEmail);
#endif
    AddTest("TestClientReportPlayer", &AccountManagementTests::TestClientReportPlayer);
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientSendAccountRecoveryEmail", &AccountManagementTests::TestClientSendAccountRecoveryEmail);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientUnlinkAndroidDeviceID", &AccountManagementTests::TestClientUnlinkAndroidDeviceID);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientUnlinkApple", &AccountManagementTests::TestClientUnlinkApple);
#endif
    AddTest("TestClientUnlinkCustomID", &AccountManagementTests::TestClientUnlinkCustomID);
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientUnlinkFacebookAccount", &AccountManagementTests::TestClientUnlinkFacebookAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientUnlinkFacebookInstantGamesId", &AccountManagementTests::TestClientUnlinkFacebookInstantGamesId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientUnlinkGameCenterAccount", &AccountManagementTests::TestClientUnlinkGameCenterAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientUnlinkGoogleAccount", &AccountManagementTests::TestClientUnlinkGoogleAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientUnlinkGooglePlayGamesServicesAccount", &AccountManagementTests::TestClientUnlinkGooglePlayGamesServicesAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientUnlinkIOSDeviceID", &AccountManagementTests::TestClientUnlinkIOSDeviceID);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientUnlinkKongregate", &AccountManagementTests::TestClientUnlinkKongregate);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH
    AddTest("TestClientUnlinkNintendoServiceAccount", &AccountManagementTests::TestClientUnlinkNintendoServiceAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientUnlinkNintendoSwitchDeviceId", &AccountManagementTests::TestClientUnlinkNintendoSwitchDeviceId);
#endif
    AddTest("TestClientUnlinkOpenIdConnect", &AccountManagementTests::TestClientUnlinkOpenIdConnect);
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
    AddTest("TestClientUnlinkPSNAccount", &AccountManagementTests::TestClientUnlinkPSNAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientUnlinkSteamAccount", &AccountManagementTests::TestClientUnlinkSteamAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientUnlinkTwitch", &AccountManagementTests::TestClientUnlinkTwitch);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
    AddTest("TestClientUnlinkXboxAccount", &AccountManagementTests::TestClientUnlinkXboxAccount);
#endif
    AddTest("TestClientUpdateAvatarUrl", &AccountManagementTests::TestClientUpdateAvatarUrl);
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestClientUpdateUserTitleDisplayName", &AccountManagementTests::TestClientUpdateUserTitleDisplayName);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerBanUsers", &AccountManagementTests::TestServerBanUsers);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerDeletePlayer", &AccountManagementTests::TestServerDeletePlayer);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerGetPlayerCombinedInfo", &AccountManagementTests::TestServerGetPlayerCombinedInfo);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerGetPlayerProfile", &AccountManagementTests::TestServerGetPlayerProfile);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerGetPlayFabIDsFromFacebookIDs", &AccountManagementTests::TestServerGetPlayFabIDsFromFacebookIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerGetPlayFabIDsFromFacebookInstantGamesIds", &AccountManagementTests::TestServerGetPlayFabIDsFromFacebookInstantGamesIds);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerGetPlayFabIDsFromNintendoServiceAccountIds", &AccountManagementTests::TestServerGetPlayFabIDsFromNintendoServiceAccountIds);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerGetPlayFabIDsFromNintendoSwitchDeviceIds", &AccountManagementTests::TestServerGetPlayFabIDsFromNintendoSwitchDeviceIds);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerGetPlayFabIDsFromPSNAccountIDs", &AccountManagementTests::TestServerGetPlayFabIDsFromPSNAccountIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerGetPlayFabIDsFromSteamIDs", &AccountManagementTests::TestServerGetPlayFabIDsFromSteamIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerGetPlayFabIDsFromTwitchIDs", &AccountManagementTests::TestServerGetPlayFabIDsFromTwitchIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerGetPlayFabIDsFromXboxLiveIDs", &AccountManagementTests::TestServerGetPlayFabIDsFromXboxLiveIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerGetServerCustomIDsFromPlayFabIDs", &AccountManagementTests::TestServerGetServerCustomIDsFromPlayFabIDs);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerGetUserAccountInfo", &AccountManagementTests::TestServerGetUserAccountInfo);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerGetUserBans", &AccountManagementTests::TestServerGetUserBans);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerLinkNintendoServiceAccount", &AccountManagementTests::TestServerLinkNintendoServiceAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerLinkNintendoSwitchDeviceId", &AccountManagementTests::TestServerLinkNintendoSwitchDeviceId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerLinkPSNAccount", &AccountManagementTests::TestServerLinkPSNAccount);
#endif
#if 0
    AddTest("TestServerLinkPSNId", &AccountManagementTests::TestServerLinkPSNId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerLinkServerCustomId", &AccountManagementTests::TestServerLinkServerCustomId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerLinkSteamId", &AccountManagementTests::TestServerLinkSteamId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerLinkXboxAccount", &AccountManagementTests::TestServerLinkXboxAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerRevokeAllBansForUser", &AccountManagementTests::TestServerRevokeAllBansForUser);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerRevokeBans", &AccountManagementTests::TestServerRevokeBans);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerSendCustomAccountRecoveryEmail", &AccountManagementTests::TestServerSendCustomAccountRecoveryEmail);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerSendEmailFromTemplate", &AccountManagementTests::TestServerSendEmailFromTemplate);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerUnlinkNintendoServiceAccount", &AccountManagementTests::TestServerUnlinkNintendoServiceAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerUnlinkNintendoSwitchDeviceId", &AccountManagementTests::TestServerUnlinkNintendoSwitchDeviceId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerUnlinkPSNAccount", &AccountManagementTests::TestServerUnlinkPSNAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerUnlinkServerCustomId", &AccountManagementTests::TestServerUnlinkServerCustomId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerUnlinkSteamId", &AccountManagementTests::TestServerUnlinkSteamId);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerUnlinkXboxAccount", &AccountManagementTests::TestServerUnlinkXboxAccount);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerUpdateAvatarUrl", &AccountManagementTests::TestServerUpdateAvatarUrl);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerUpdateBans", &AccountManagementTests::TestServerUpdateBans);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestGetTitlePlayersFromXboxLiveIDs", &AccountManagementTests::TestGetTitlePlayersFromXboxLiveIDs);
#endif

}

}
}

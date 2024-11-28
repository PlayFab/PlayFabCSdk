#pragma once

#include <playfab/services/PFTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "AccountManagementTypes.h"
#include "GlobalState.h"
#include "Generated/Types.h"

namespace PlayFab
{
namespace AccountManagement
{

class AccountManagementAPI
{
public:
    AccountManagementAPI() = delete;
    AccountManagementAPI(const AccountManagementAPI& source) = delete;
    AccountManagementAPI& operator=(const AccountManagementAPI& source) = delete;
    ~AccountManagementAPI() = default;

    // ------------ Generated API calls
    static AsyncOp<void> ClientAddOrUpdateContactEmail(Entity const& entity, const AddOrUpdateContactEmailRequest& request, RunContext rc);
    static AsyncOp<AddUsernamePasswordResult> ClientAddUsernamePassword(Entity const& entity, const AddUsernamePasswordRequest& request, RunContext rc);
    static AsyncOp<GetAccountInfoResult> ClientGetAccountInfo(Entity const& entity, const GetAccountInfoRequest& request, RunContext rc);
    static AsyncOp<GetPlayerCombinedInfoResult> ClientGetPlayerCombinedInfo(Entity const& entity, const GetPlayerCombinedInfoRequest& request, RunContext rc);
    static AsyncOp<GetPlayerProfileResult> ClientGetPlayerProfile(Entity const& entity, const GetPlayerProfileRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromFacebookIDsResult> ClientGetPlayFabIDsFromFacebookIDs(Entity const& entity, const GetPlayFabIDsFromFacebookIDsRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromFacebookInstantGamesIdsResult> ClientGetPlayFabIDsFromFacebookInstantGamesIds(Entity const& entity, const GetPlayFabIDsFromFacebookInstantGamesIdsRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromGameCenterIDsResult> ClientGetPlayFabIDsFromGameCenterIDs(Entity const& entity, const GetPlayFabIDsFromGameCenterIDsRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromGoogleIDsResult> ClientGetPlayFabIDsFromGoogleIDs(Entity const& entity, const GetPlayFabIDsFromGoogleIDsRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromGooglePlayGamesPlayerIDsResult> ClientGetPlayFabIDsFromGooglePlayGamesPlayerIDs(Entity const& entity, const GetPlayFabIDsFromGooglePlayGamesPlayerIDsRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromKongregateIDsResult> ClientGetPlayFabIDsFromKongregateIDs(Entity const& entity, const GetPlayFabIDsFromKongregateIDsRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromNintendoServiceAccountIdsResult> ClientGetPlayFabIDsFromNintendoServiceAccountIds(Entity const& entity, const GetPlayFabIDsFromNintendoServiceAccountIdsRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromNintendoSwitchDeviceIdsResult> ClientGetPlayFabIDsFromNintendoSwitchDeviceIds(Entity const& entity, const GetPlayFabIDsFromNintendoSwitchDeviceIdsRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromPSNAccountIDsResult> ClientGetPlayFabIDsFromPSNAccountIDs(Entity const& entity, const GetPlayFabIDsFromPSNAccountIDsRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromPSNOnlineIDsResult> ClientGetPlayFabIDsFromPSNOnlineIDs(Entity const& entity, const GetPlayFabIDsFromPSNOnlineIDsRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromSteamIDsResult> ClientGetPlayFabIDsFromSteamIDs(Entity const& entity, const GetPlayFabIDsFromSteamIDsRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromSteamNamesResult> ClientGetPlayFabIDsFromSteamNames(Entity const& entity, const GetPlayFabIDsFromSteamNamesRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromTwitchIDsResult> ClientGetPlayFabIDsFromTwitchIDs(Entity const& entity, const GetPlayFabIDsFromTwitchIDsRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromXboxLiveIDsResult> ClientGetPlayFabIDsFromXboxLiveIDs(Entity const& entity, const GetPlayFabIDsFromXboxLiveIDsRequest& request, RunContext rc);
    static AsyncOp<void> ClientLinkAndroidDeviceID(Entity const& entity, const LinkAndroidDeviceIDRequest& request, RunContext rc);
    static AsyncOp<void> ClientLinkApple(Entity const& entity, const LinkAppleRequest& request, RunContext rc);
    static AsyncOp<void> ClientLinkCustomID(Entity const& entity, const LinkCustomIDRequest& request, RunContext rc);
    static AsyncOp<void> ClientLinkFacebookAccount(Entity const& entity, const LinkFacebookAccountRequest& request, RunContext rc);
    static AsyncOp<void> ClientLinkFacebookInstantGamesId(Entity const& entity, const LinkFacebookInstantGamesIdRequest& request, RunContext rc);
    static AsyncOp<void> ClientLinkGameCenterAccount(Entity const& entity, const LinkGameCenterAccountRequest& request, RunContext rc);
    static AsyncOp<void> ClientLinkGoogleAccount(Entity const& entity, const LinkGoogleAccountRequest& request, RunContext rc);
    static AsyncOp<void> ClientLinkGooglePlayGamesServicesAccount(Entity const& entity, const LinkGooglePlayGamesServicesAccountRequest& request, RunContext rc);
    static AsyncOp<void> ClientLinkIOSDeviceID(Entity const& entity, const LinkIOSDeviceIDRequest& request, RunContext rc);
    static AsyncOp<void> ClientLinkKongregate(Entity const& entity, const LinkKongregateAccountRequest& request, RunContext rc);
    static AsyncOp<void> ClientLinkNintendoServiceAccount(Entity const& entity, const ClientLinkNintendoServiceAccountRequest& request, RunContext rc);
    static AsyncOp<void> ClientLinkNintendoSwitchDeviceId(Entity const& entity, const ClientLinkNintendoSwitchDeviceIdRequest& request, RunContext rc);
    static AsyncOp<void> ClientLinkOpenIdConnect(Entity const& entity, const LinkOpenIdConnectRequest& request, RunContext rc);
    static AsyncOp<void> ClientLinkPSNAccount(Entity const& entity, const ClientLinkPSNAccountRequest& request, RunContext rc);
    static AsyncOp<void> ClientLinkSteamAccount(Entity const& entity, const LinkSteamAccountRequest& request, RunContext rc);
    static AsyncOp<void> ClientLinkTwitch(Entity const& entity, const LinkTwitchAccountRequest& request, RunContext rc);
    static AsyncOp<void> ClientLinkXboxAccount(Entity const& entity, const ClientLinkXboxAccountRequest& request, RunContext rc);
    static AsyncOp<void> ClientRemoveContactEmail(Entity const& entity, const RemoveContactEmailRequest& request, RunContext rc);
    static AsyncOp<ReportPlayerClientResult> ClientReportPlayer(Entity const& entity, const ReportPlayerClientRequest& request, RunContext rc);
    static AsyncOp<void> ClientSendAccountRecoveryEmail(ServiceConfig const& serviceConfig, const SendAccountRecoveryEmailRequest& request, RunContext rc);
    static AsyncOp<void> ClientUnlinkAndroidDeviceID(Entity const& entity, const UnlinkAndroidDeviceIDRequest& request, RunContext rc);
    static AsyncOp<void> ClientUnlinkApple(Entity const& entity, const UnlinkAppleRequest& request, RunContext rc);
    static AsyncOp<void> ClientUnlinkCustomID(Entity const& entity, const UnlinkCustomIDRequest& request, RunContext rc);
    static AsyncOp<void> ClientUnlinkFacebookAccount(Entity const& entity, const UnlinkFacebookAccountRequest& request, RunContext rc);
    static AsyncOp<void> ClientUnlinkFacebookInstantGamesId(Entity const& entity, const UnlinkFacebookInstantGamesIdRequest& request, RunContext rc);
    static AsyncOp<void> ClientUnlinkGameCenterAccount(Entity const& entity, const UnlinkGameCenterAccountRequest& request, RunContext rc);
    static AsyncOp<void> ClientUnlinkGoogleAccount(Entity const& entity, const UnlinkGoogleAccountRequest& request, RunContext rc);
    static AsyncOp<void> ClientUnlinkGooglePlayGamesServicesAccount(Entity const& entity, const UnlinkGooglePlayGamesServicesAccountRequest& request, RunContext rc);
    static AsyncOp<void> ClientUnlinkIOSDeviceID(Entity const& entity, const UnlinkIOSDeviceIDRequest& request, RunContext rc);
    static AsyncOp<void> ClientUnlinkKongregate(Entity const& entity, const UnlinkKongregateAccountRequest& request, RunContext rc);
    static AsyncOp<void> ClientUnlinkNintendoServiceAccount(Entity const& entity, const ClientUnlinkNintendoServiceAccountRequest& request, RunContext rc);
    static AsyncOp<void> ClientUnlinkNintendoSwitchDeviceId(Entity const& entity, const ClientUnlinkNintendoSwitchDeviceIdRequest& request, RunContext rc);
    static AsyncOp<void> ClientUnlinkOpenIdConnect(Entity const& entity, const UnlinkOpenIdConnectRequest& request, RunContext rc);
    static AsyncOp<void> ClientUnlinkPSNAccount(Entity const& entity, const ClientUnlinkPSNAccountRequest& request, RunContext rc);
    static AsyncOp<void> ClientUnlinkSteamAccount(Entity const& entity, const UnlinkSteamAccountRequest& request, RunContext rc);
    static AsyncOp<void> ClientUnlinkTwitch(Entity const& entity, const UnlinkTwitchAccountRequest& request, RunContext rc);
    static AsyncOp<void> ClientUnlinkXboxAccount(Entity const& entity, const ClientUnlinkXboxAccountRequest& request, RunContext rc);
    static AsyncOp<void> ClientUpdateAvatarUrl(Entity const& entity, const ClientUpdateAvatarUrlRequest& request, RunContext rc);
    static AsyncOp<UpdateUserTitleDisplayNameResult> ClientUpdateUserTitleDisplayName(Entity const& entity, const UpdateUserTitleDisplayNameRequest& request, RunContext rc);
    static AsyncOp<BanUsersResult> ServerBanUsers(Entity const& entity, const BanUsersRequest& request, RunContext rc);
    static AsyncOp<void> ServerDeletePlayer(Entity const& entity, const DeletePlayerRequest& request, RunContext rc);
    static AsyncOp<GetPlayerCombinedInfoResult> ServerGetPlayerCombinedInfo(Entity const& entity, const GetPlayerCombinedInfoRequest& request, RunContext rc);
    static AsyncOp<GetPlayerProfileResult> ServerGetPlayerProfile(Entity const& entity, const GetPlayerProfileRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromFacebookIDsResult> ServerGetPlayFabIDsFromFacebookIDs(Entity const& entity, const GetPlayFabIDsFromFacebookIDsRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromFacebookInstantGamesIdsResult> ServerGetPlayFabIDsFromFacebookInstantGamesIds(Entity const& entity, const GetPlayFabIDsFromFacebookInstantGamesIdsRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromNintendoServiceAccountIdsResult> ServerGetPlayFabIDsFromNintendoServiceAccountIds(Entity const& entity, const GetPlayFabIDsFromNintendoServiceAccountIdsRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromNintendoSwitchDeviceIdsResult> ServerGetPlayFabIDsFromNintendoSwitchDeviceIds(Entity const& entity, const GetPlayFabIDsFromNintendoSwitchDeviceIdsRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromPSNAccountIDsResult> ServerGetPlayFabIDsFromPSNAccountIDs(Entity const& entity, const GetPlayFabIDsFromPSNAccountIDsRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromPSNOnlineIDsResult> ServerGetPlayFabIDsFromPSNOnlineIDs(Entity const& entity, const GetPlayFabIDsFromPSNOnlineIDsRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromSteamIDsResult> ServerGetPlayFabIDsFromSteamIDs(Entity const& entity, const GetPlayFabIDsFromSteamIDsRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromSteamNamesResult> ServerGetPlayFabIDsFromSteamNames(Entity const& entity, const GetPlayFabIDsFromSteamNamesRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromTwitchIDsResult> ServerGetPlayFabIDsFromTwitchIDs(Entity const& entity, const GetPlayFabIDsFromTwitchIDsRequest& request, RunContext rc);
    static AsyncOp<GetPlayFabIDsFromXboxLiveIDsResult> ServerGetPlayFabIDsFromXboxLiveIDs(Entity const& entity, const GetPlayFabIDsFromXboxLiveIDsRequest& request, RunContext rc);
    static AsyncOp<GetServerCustomIDsFromPlayFabIDsResult> ServerGetServerCustomIDsFromPlayFabIDs(Entity const& entity, const GetServerCustomIDsFromPlayFabIDsRequest& request, RunContext rc);
    static AsyncOp<GetUserAccountInfoResult> ServerGetUserAccountInfo(Entity const& entity, const GetUserAccountInfoRequest& request, RunContext rc);
    static AsyncOp<GetUserBansResult> ServerGetUserBans(Entity const& entity, const GetUserBansRequest& request, RunContext rc);
    static AsyncOp<void> ServerLinkNintendoServiceAccount(Entity const& entity, const ServerLinkNintendoServiceAccountRequest& request, RunContext rc);
    static AsyncOp<void> ServerLinkNintendoServiceAccountSubject(Entity const& entity, const LinkNintendoServiceAccountSubjectRequest& request, RunContext rc);
    static AsyncOp<void> ServerLinkNintendoSwitchDeviceId(Entity const& entity, const ServerLinkNintendoSwitchDeviceIdRequest& request, RunContext rc);
    static AsyncOp<void> ServerLinkPSNAccount(Entity const& entity, const ServerLinkPSNAccountRequest& request, RunContext rc);
    static AsyncOp<void> ServerLinkPSNId(Entity const& entity, const LinkPSNIdRequest& request, RunContext rc);
    static AsyncOp<void> ServerLinkServerCustomId(Entity const& entity, const LinkServerCustomIdRequest& request, RunContext rc);
    static AsyncOp<void> ServerLinkSteamId(Entity const& entity, const LinkSteamIdRequest& request, RunContext rc);
    static AsyncOp<void> ServerLinkXboxAccount(Entity const& entity, const ServerLinkXboxAccountRequest& request, RunContext rc);
    static AsyncOp<RevokeAllBansForUserResult> ServerRevokeAllBansForUser(Entity const& entity, const RevokeAllBansForUserRequest& request, RunContext rc);
    static AsyncOp<RevokeBansResult> ServerRevokeBans(Entity const& entity, const RevokeBansRequest& request, RunContext rc);
    static AsyncOp<void> ServerSendCustomAccountRecoveryEmail(Entity const& entity, const SendCustomAccountRecoveryEmailRequest& request, RunContext rc);
    static AsyncOp<void> ServerSendEmailFromTemplate(Entity const& entity, const SendEmailFromTemplateRequest& request, RunContext rc);
    static AsyncOp<void> ServerUnlinkNintendoServiceAccount(Entity const& entity, const ServerUnlinkNintendoServiceAccountRequest& request, RunContext rc);
    static AsyncOp<void> ServerUnlinkNintendoSwitchDeviceId(Entity const& entity, const ServerUnlinkNintendoSwitchDeviceIdRequest& request, RunContext rc);
    static AsyncOp<void> ServerUnlinkPSNAccount(Entity const& entity, const ServerUnlinkPSNAccountRequest& request, RunContext rc);
    static AsyncOp<void> ServerUnlinkServerCustomId(Entity const& entity, const UnlinkServerCustomIdRequest& request, RunContext rc);
    static AsyncOp<void> ServerUnlinkSteamId(Entity const& entity, const UnlinkSteamIdRequest& request, RunContext rc);
    static AsyncOp<void> ServerUnlinkXboxAccount(Entity const& entity, const ServerUnlinkXboxAccountRequest& request, RunContext rc);
    static AsyncOp<void> ServerUpdateAvatarUrl(Entity const& entity, const ServerUpdateAvatarUrlRequest& request, RunContext rc);
    static AsyncOp<UpdateBansResult> ServerUpdateBans(Entity const& entity, const UpdateBansRequest& request, RunContext rc);
    static AsyncOp<GetTitlePlayersFromProviderIDsResponse> GetTitlePlayersFromXboxLiveIDs(Entity const& entity, const GetTitlePlayersFromXboxLiveIDsRequest& request, RunContext rc);
    static AsyncOp<SetDisplayNameResponse> SetDisplayName(Entity const& entity, const SetDisplayNameRequest& request, RunContext rc);
};

} // namespace AccountManagement
} // namespace PlayFab

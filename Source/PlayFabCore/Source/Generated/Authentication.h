#pragma once

#include "AuthenticationTypes.h"
#include "Common/Entity.h"
#include "Common/PFCoreGlobalState.h"
#include "Authentication/CombinedLoginResult.h"

namespace PlayFab
{
namespace Authentication
{

class AuthenticationAPI
{
public:
    AuthenticationAPI() = delete;
    AuthenticationAPI(const AuthenticationAPI& source) = delete;
    AuthenticationAPI& operator=(const AuthenticationAPI& source) = delete;
    ~AuthenticationAPI() = default;

    // ------------ Generated API calls
    static AsyncOp<CombinedLoginResult> LoginWithAndroidDeviceID(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, const LoginWithAndroidDeviceIDRequest& request, RunContext rc);
    static AsyncOp<void> ReLoginWithAndroidDeviceID(SharedPtr<Entity> entity, const LoginWithAndroidDeviceIDRequest& request, RunContext rc);
    static AsyncOp<CombinedLoginResult> LoginWithApple(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, const LoginWithAppleRequest& request, RunContext rc);
    static AsyncOp<void> ReLoginWithApple(SharedPtr<Entity> entity, const LoginWithAppleRequest& request, RunContext rc);
    static AsyncOp<CombinedLoginResult> LoginWithCustomID(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, const LoginWithCustomIDRequest& request, RunContext rc);
    static AsyncOp<void> ReLoginWithCustomID(SharedPtr<Entity> entity, const LoginWithCustomIDRequest& request, RunContext rc);
    static AsyncOp<CombinedLoginResult> LoginWithEmailAddress(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, const LoginWithEmailAddressRequest& request, RunContext rc);
    static AsyncOp<void> ReLoginWithEmailAddress(SharedPtr<Entity> entity, const LoginWithEmailAddressRequest& request, RunContext rc);
    static AsyncOp<CombinedLoginResult> LoginWithFacebook(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, const LoginWithFacebookRequest& request, RunContext rc);
    static AsyncOp<void> ReLoginWithFacebook(SharedPtr<Entity> entity, const LoginWithFacebookRequest& request, RunContext rc);
    static AsyncOp<CombinedLoginResult> LoginWithFacebookInstantGamesId(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, const LoginWithFacebookInstantGamesIdRequest& request, RunContext rc);
    static AsyncOp<void> ReLoginWithFacebookInstantGamesId(SharedPtr<Entity> entity, const LoginWithFacebookInstantGamesIdRequest& request, RunContext rc);
    static AsyncOp<CombinedLoginResult> LoginWithGameCenter(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, const LoginWithGameCenterRequest& request, RunContext rc);
    static AsyncOp<void> ReLoginWithGameCenter(SharedPtr<Entity> entity, const LoginWithGameCenterRequest& request, RunContext rc);
    static AsyncOp<CombinedLoginResult> LoginWithGoogleAccount(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, const LoginWithGoogleAccountRequest& request, RunContext rc);
    static AsyncOp<void> ReLoginWithGoogleAccount(SharedPtr<Entity> entity, const LoginWithGoogleAccountRequest& request, RunContext rc);
    static AsyncOp<CombinedLoginResult> LoginWithGooglePlayGamesServices(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, const LoginWithGooglePlayGamesServicesRequest& request, RunContext rc);
    static AsyncOp<void> ReLoginWithGooglePlayGamesServices(SharedPtr<Entity> entity, const LoginWithGooglePlayGamesServicesRequest& request, RunContext rc);
    static AsyncOp<CombinedLoginResult> LoginWithIOSDeviceID(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, const LoginWithIOSDeviceIDRequest& request, RunContext rc);
    static AsyncOp<void> ReLoginWithIOSDeviceID(SharedPtr<Entity> entity, const LoginWithIOSDeviceIDRequest& request, RunContext rc);
    static AsyncOp<CombinedLoginResult> LoginWithKongregate(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, const LoginWithKongregateRequest& request, RunContext rc);
    static AsyncOp<void> ReLoginWithKongregate(SharedPtr<Entity> entity, const LoginWithKongregateRequest& request, RunContext rc);
    static AsyncOp<CombinedLoginResult> LoginWithNintendoServiceAccount(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, const LoginWithNintendoServiceAccountRequest& request, RunContext rc);
    static AsyncOp<void> ReLoginWithNintendoServiceAccount(SharedPtr<Entity> entity, const LoginWithNintendoServiceAccountRequest& request, RunContext rc);
    static AsyncOp<CombinedLoginResult> LoginWithNintendoSwitchDeviceId(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, const LoginWithNintendoSwitchDeviceIdRequest& request, RunContext rc);
    static AsyncOp<void> ReLoginWithNintendoSwitchDeviceId(SharedPtr<Entity> entity, const LoginWithNintendoSwitchDeviceIdRequest& request, RunContext rc);
    static AsyncOp<CombinedLoginResult> LoginWithOpenIdConnect(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, const LoginWithOpenIdConnectRequest& request, RunContext rc);
    static AsyncOp<void> ReLoginWithOpenIdConnect(SharedPtr<Entity> entity, const LoginWithOpenIdConnectRequest& request, RunContext rc);
    static AsyncOp<CombinedLoginResult> LoginWithPlayFab(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, const LoginWithPlayFabRequest& request, RunContext rc);
    static AsyncOp<void> ReLoginWithPlayFab(SharedPtr<Entity> entity, const LoginWithPlayFabRequest& request, RunContext rc);
    static AsyncOp<CombinedLoginResult> LoginWithPSN(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, const LoginWithPSNRequest& request, RunContext rc);
    static AsyncOp<void> ReLoginWithPSN(SharedPtr<Entity> entity, const LoginWithPSNRequest& request, RunContext rc);
    static AsyncOp<CombinedLoginResult> LoginWithSteam(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, const LoginWithSteamRequest& request, RunContext rc);
    static AsyncOp<void> ReLoginWithSteam(SharedPtr<Entity> entity, const LoginWithSteamRequest& request, RunContext rc);
    static AsyncOp<CombinedLoginResult> LoginWithTwitch(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, const LoginWithTwitchRequest& request, RunContext rc);
    static AsyncOp<void> ReLoginWithTwitch(SharedPtr<Entity> entity, const LoginWithTwitchRequest& request, RunContext rc);
    static AsyncOp<CombinedLoginResult> LoginWithXbox(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, const LoginWithXboxRequest& request, RunContext rc);
    static AsyncOp<void> ReLoginWithXbox(SharedPtr<Entity> entity, const LoginWithXboxRequest& request, RunContext rc);
#if HC_PLATFORM == HC_PLATFORM_GDK
    static AsyncOp<CombinedLoginResult> LoginWithXUser(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, const LoginWithXUserRequest& request, RunContext rc);
    static AsyncOp<void> ReLoginWithXUser(SharedPtr<Entity> entity, const LoginWithXUserRequest& request, RunContext rc);
#endif
    static AsyncOp<RegisterPlayFabUserResult> RegisterPlayFabUser(SharedPtr<ServiceConfig const> config, const RegisterPlayFabUserRequest& request, RunContext rc);
    static AsyncOp<ServerCombinedLoginResult> ServerLoginWithServerCustomId(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, String&& secretKey, const LoginWithServerCustomIdRequest& request, RunContext rc);
    static AsyncOp<ServerCombinedLoginResult> ServerLoginWithSteamId(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, String&& secretKey, const LoginWithSteamIdRequest& request, RunContext rc);
    static AsyncOp<ServerCombinedLoginResult> ServerLoginWithXbox(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, String&& secretKey, const ServerLoginWithXboxRequest& request, RunContext rc);
    static AsyncOp<ServerCombinedLoginResult> ServerLoginWithXboxId(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, String&& secretKey, const LoginWithXboxIdRequest& request, RunContext rc);
#if HC_PLATFORM != HC_PLATFORM_GDK
    static AsyncOp<AuthenticateCustomIdResult> AuthenticateGameServerWithCustomId(SharedPtr<Entity> entity, const AuthenticateCustomIdRequest& request, RunContext rc);
#endif
    static AsyncOp<void> Delete(SharedPtr<Entity> entity, const DeleteRequest& request, RunContext rc);
    static AsyncOp<SharedPtr<Entity>> GetEntity(SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> entity, const GetEntityRequest& request, RunContext rc);
    static AsyncOp<SharedPtr<Entity>> GetEntityWithSecretKey(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, String&& secretKey, const GetEntityRequest& request, RunContext rc);
    static AsyncOp<ValidateEntityTokenResponse> ValidateEntityToken(SharedPtr<Entity> entity, const ValidateEntityTokenRequest& request, RunContext rc);
};

} // namespace Authentication
} // namespace PlayFab

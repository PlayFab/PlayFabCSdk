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
    static AsyncOp<RegisterPlayFabUserResult> RegisterPlayFabUser(SharedPtr<ServiceConfig const> config, const RegisterPlayFabUserRequest& request, RunContext rc);
    static AsyncOp<ServerCombinedLoginResult> ServerLoginWithAndroidDeviceID(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, String&& secretKey, const ServerLoginWithAndroidDeviceIDRequest& request, RunContext rc);
    static AsyncOp<ServerCombinedLoginResult> ServerLoginWithBattleNet(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, String&& secretKey, const ServerLoginWithBattleNetRequest& request, RunContext rc);
    static AsyncOp<ServerCombinedLoginResult> ServerLoginWithCustomID(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, String&& secretKey, const ServerLoginWithCustomIDRequest& request, RunContext rc);
    static AsyncOp<ServerCombinedLoginResult> ServerLoginWithIOSDeviceID(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, String&& secretKey, const ServerLoginWithIOSDeviceIDRequest& request, RunContext rc);
    static AsyncOp<ServerCombinedLoginResult> ServerLoginWithPSN(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, String&& secretKey, const ServerLoginWithPSNRequest& request, RunContext rc);
    static AsyncOp<ServerCombinedLoginResult> ServerLoginWithServerCustomId(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, String&& secretKey, const LoginWithServerCustomIdRequest& request, RunContext rc);
    static AsyncOp<ServerCombinedLoginResult> ServerLoginWithSteamId(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, String&& secretKey, const LoginWithSteamIdRequest& request, RunContext rc);
    static AsyncOp<ServerCombinedLoginResult> ServerLoginWithTwitch(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, String&& secretKey, const ServerLoginWithTwitchRequest& request, RunContext rc);
    static AsyncOp<ServerCombinedLoginResult> ServerLoginWithXbox(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, String&& secretKey, const ServerLoginWithXboxRequest& request, RunContext rc);
    static AsyncOp<ServerCombinedLoginResult> ServerLoginWithXboxId(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, String&& secretKey, const LoginWithXboxIdRequest& request, RunContext rc);
    static AsyncOp<AuthenticateGameServerResult> AuthenticateGameServerWithCustomId(SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> entity, const AuthenticateCustomIdRequest& request, RunContext rc);
    static AsyncOp<void> Delete(SharedPtr<Entity> entity, const DeleteRequest& request, RunContext rc);
    static AsyncOp<SharedPtr<Entity>> GetEntity(SharedPtr<PFCoreGlobalState> state, SharedPtr<Entity> entity, const GetEntityRequest& request, RunContext rc);
    static AsyncOp<SharedPtr<Entity>> GetEntityWithSecretKey(SharedPtr<PFCoreGlobalState> state, SharedPtr<ServiceConfig const> config, String&& secretKey, const GetEntityRequest& request, RunContext rc);
    static AsyncOp<ValidateEntityTokenResponse> ValidateEntityToken(SharedPtr<Entity> entity, const ValidateEntityTokenRequest& request, RunContext rc);
};

} // namespace Authentication
} // namespace PlayFab

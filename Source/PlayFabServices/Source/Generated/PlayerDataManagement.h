#pragma once

#include <playfab/services/PFTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "PlayerDataManagementTypes.h"
#include "GlobalState.h"
#include "Generated/Types.h"

namespace PlayFab
{
namespace PlayerDataManagement
{

class PlayerDataManagementAPI
{
public:
    PlayerDataManagementAPI() = delete;
    PlayerDataManagementAPI(const PlayerDataManagementAPI& source) = delete;
    PlayerDataManagementAPI& operator=(const PlayerDataManagementAPI& source) = delete;
    ~PlayerDataManagementAPI() = default;

    // ------------ Generated API calls
    static AsyncOp<ClientDeletePlayerCustomPropertiesResult> ClientDeletePlayerCustomProperties(Entity const& entity, const ClientDeletePlayerCustomPropertiesRequest& request, RunContext rc);
    static AsyncOp<ClientGetPlayerCustomPropertyResult> ClientGetPlayerCustomProperty(Entity const& entity, const ClientGetPlayerCustomPropertyRequest& request, RunContext rc);
    static AsyncOp<ClientGetUserDataResult> ClientGetUserData(Entity const& entity, const GetUserDataRequest& request, RunContext rc);
    static AsyncOp<ClientGetUserDataResult> ClientGetUserPublisherData(Entity const& entity, const GetUserDataRequest& request, RunContext rc);
    static AsyncOp<ClientGetUserDataResult> ClientGetUserPublisherReadOnlyData(Entity const& entity, const GetUserDataRequest& request, RunContext rc);
    static AsyncOp<ClientGetUserDataResult> ClientGetUserReadOnlyData(Entity const& entity, const GetUserDataRequest& request, RunContext rc);
    static AsyncOp<ClientListPlayerCustomPropertiesResult> ClientListPlayerCustomProperties(Entity const& entity, RunContext rc);
    static AsyncOp<ClientUpdatePlayerCustomPropertiesResult> ClientUpdatePlayerCustomProperties(Entity const& entity, const ClientUpdatePlayerCustomPropertiesRequest& request, RunContext rc);
    static AsyncOp<UpdateUserDataResult> ClientUpdateUserData(Entity const& entity, const ClientUpdateUserDataRequest& request, RunContext rc);
    static AsyncOp<UpdateUserDataResult> ClientUpdateUserPublisherData(Entity const& entity, const ClientUpdateUserDataRequest& request, RunContext rc);
    static AsyncOp<ServerDeletePlayerCustomPropertiesResult> ServerDeletePlayerCustomProperties(Entity const& entity, const ServerDeletePlayerCustomPropertiesRequest& request, RunContext rc);
    static AsyncOp<ServerGetPlayerCustomPropertyResult> ServerGetPlayerCustomProperty(Entity const& entity, const ServerGetPlayerCustomPropertyRequest& request, RunContext rc);
    static AsyncOp<ServerGetUserDataResult> ServerGetUserData(Entity const& entity, const GetUserDataRequest& request, RunContext rc);
    static AsyncOp<ServerGetUserDataResult> ServerGetUserInternalData(Entity const& entity, const GetUserDataRequest& request, RunContext rc);
    static AsyncOp<ServerGetUserDataResult> ServerGetUserPublisherData(Entity const& entity, const GetUserDataRequest& request, RunContext rc);
    static AsyncOp<ServerGetUserDataResult> ServerGetUserPublisherInternalData(Entity const& entity, const GetUserDataRequest& request, RunContext rc);
    static AsyncOp<ServerGetUserDataResult> ServerGetUserPublisherReadOnlyData(Entity const& entity, const GetUserDataRequest& request, RunContext rc);
    static AsyncOp<ServerGetUserDataResult> ServerGetUserReadOnlyData(Entity const& entity, const GetUserDataRequest& request, RunContext rc);
    static AsyncOp<ServerListPlayerCustomPropertiesResult> ServerListPlayerCustomProperties(Entity const& entity, const ListPlayerCustomPropertiesRequest& request, RunContext rc);
    static AsyncOp<ServerUpdatePlayerCustomPropertiesResult> ServerUpdatePlayerCustomProperties(Entity const& entity, const ServerUpdatePlayerCustomPropertiesRequest& request, RunContext rc);
    static AsyncOp<UpdateUserDataResult> ServerUpdateUserData(Entity const& entity, const ServerUpdateUserDataRequest& request, RunContext rc);
    static AsyncOp<UpdateUserDataResult> ServerUpdateUserInternalData(Entity const& entity, const UpdateUserInternalDataRequest& request, RunContext rc);
    static AsyncOp<UpdateUserDataResult> ServerUpdateUserPublisherData(Entity const& entity, const ServerUpdateUserDataRequest& request, RunContext rc);
    static AsyncOp<UpdateUserDataResult> ServerUpdateUserPublisherInternalData(Entity const& entity, const UpdateUserInternalDataRequest& request, RunContext rc);
    static AsyncOp<UpdateUserDataResult> ServerUpdateUserPublisherReadOnlyData(Entity const& entity, const ServerUpdateUserDataRequest& request, RunContext rc);
    static AsyncOp<UpdateUserDataResult> ServerUpdateUserReadOnlyData(Entity const& entity, const ServerUpdateUserDataRequest& request, RunContext rc);
};

} // namespace PlayerDataManagement
} // namespace PlayFab

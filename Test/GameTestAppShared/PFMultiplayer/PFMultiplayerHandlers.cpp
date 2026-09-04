#include "pch.h"

#include "PFMultiplayerHandlers.h"
#include <PFMultiplayer.h>
#include <PFLobby.h>
#include <PFMatchmaking.h>

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

static PFMultiplayerHandle s_multiplayerHandle{};

CommandResultPayload HandlePFLobbyGetLobbyId(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const char* id{};
            HRESULT hr = PFLobbyGetLobbyId(nullptr, &id);
            LogToWindowFormat("PFLobbyGetLobbyId (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyGetMaxMemberCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t maxMemberCount{};
            HRESULT hr = PFLobbyGetMaxMemberCount(nullptr, &maxMemberCount);
            if (SUCCEEDED(hr)) { payload.result = maxMemberCount; }
            LogToWindowFormat("PFLobbyGetMaxMemberCount (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyGetOwner(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const PFEntityKey* owner{};
            HRESULT hr = PFLobbyGetOwner(nullptr, &owner);
            LogToWindowFormat("PFLobbyGetOwner (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyGetOwnerMigrationPolicy(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PFLobbyOwnerMigrationPolicy policy{};
            HRESULT hr = PFLobbyGetOwnerMigrationPolicy(nullptr, &policy);
            if (SUCCEEDED(hr)) { payload.result = static_cast<int>(policy); }
            LogToWindowFormat("PFLobbyGetOwnerMigrationPolicy (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyGetAccessPolicy(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PFLobbyAccessPolicy policy{};
            HRESULT hr = PFLobbyGetAccessPolicy(nullptr, &policy);
            if (SUCCEEDED(hr)) { payload.result = static_cast<int>(policy); }
            LogToWindowFormat("PFLobbyGetAccessPolicy (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyGetMembershipLock(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PFLobbyMembershipLock lockState{};
            HRESULT hr = PFLobbyGetMembershipLock(nullptr, &lockState);
            if (SUCCEEDED(hr)) { payload.result = static_cast<int>(lockState); }
            LogToWindowFormat("PFLobbyGetMembershipLock (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyGetConnectionString(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const char* connectionString{};
            HRESULT hr = PFLobbyGetConnectionString(nullptr, &connectionString);
            LogToWindowFormat("PFLobbyGetConnectionString (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyGetMembers(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t memberCount{};
            const PFEntityKey* members{};
            HRESULT hr = PFLobbyGetMembers(nullptr, &memberCount, &members);
            if (SUCCEEDED(hr)) { payload.result = memberCount; }
            LogToWindowFormat("PFLobbyGetMembers (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyAddMember(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PFLobbyAddMember(nullptr, nullptr, 0, nullptr, nullptr, nullptr);
            LogToWindowFormat("PFLobbyAddMember (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyAddMemberWithEntityHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFLobbyAddMemberWithEntityHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyForceRemoveMember(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PFLobbyForceRemoveMember(nullptr, nullptr, false, nullptr);
            LogToWindowFormat("PFLobbyForceRemoveMember (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyLeave(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PFLobbyLeave(nullptr, nullptr, nullptr);
            LogToWindowFormat("PFLobbyLeave (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyLeaveWithEntityHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFLobbyLeaveWithEntityHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyGetSearchPropertyKeys(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t propertyCount{};
            const char* const* keys{};
            HRESULT hr = PFLobbyGetSearchPropertyKeys(nullptr, &propertyCount, &keys);
            if (SUCCEEDED(hr)) { payload.result = propertyCount; }
            LogToWindowFormat("PFLobbyGetSearchPropertyKeys (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyGetSearchProperty(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const char* value{};
            HRESULT hr = PFLobbyGetSearchProperty(nullptr, "key", &value);
            LogToWindowFormat("PFLobbyGetSearchProperty (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyGetLobbyPropertyKeys(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t propertyCount{};
            const char* const* keys{};
            HRESULT hr = PFLobbyGetLobbyPropertyKeys(nullptr, &propertyCount, &keys);
            if (SUCCEEDED(hr)) { payload.result = propertyCount; }
            LogToWindowFormat("PFLobbyGetLobbyPropertyKeys (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyGetLobbyProperty(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const char* value{};
            HRESULT hr = PFLobbyGetLobbyProperty(nullptr, "key", &value);
            LogToWindowFormat("PFLobbyGetLobbyProperty (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyGetMemberPropertyKeys(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t propertyCount{};
            const char* const* keys{};
            HRESULT hr = PFLobbyGetMemberPropertyKeys(nullptr, nullptr, &propertyCount, &keys);
            if (SUCCEEDED(hr)) { payload.result = propertyCount; }
            LogToWindowFormat("PFLobbyGetMemberPropertyKeys (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyGetMemberProperty(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const char* value{};
            HRESULT hr = PFLobbyGetMemberProperty(nullptr, nullptr, "key", &value);
            LogToWindowFormat("PFLobbyGetMemberProperty (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyGetMemberConnectionStatus(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PFLobbyMemberConnectionStatus connectionStatus{};
            HRESULT hr = PFLobbyGetMemberConnectionStatus(nullptr, nullptr, &connectionStatus);
            if (SUCCEEDED(hr)) { payload.result = static_cast<int>(connectionStatus); }
            LogToWindowFormat("PFLobbyGetMemberConnectionStatus (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyGetServer(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const PFEntityKey* server{};
            HRESULT hr = PFLobbyGetServer(nullptr, &server);
            LogToWindowFormat("PFLobbyGetServer (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyGetServerPropertyKeys(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t propertyCount{};
            const char* const* keys{};
            HRESULT hr = PFLobbyGetServerPropertyKeys(nullptr, &propertyCount, &keys);
            if (SUCCEEDED(hr)) { payload.result = propertyCount; }
            LogToWindowFormat("PFLobbyGetServerPropertyKeys (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyGetServerProperty(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const char* value{};
            HRESULT hr = PFLobbyGetServerProperty(nullptr, "key", &value);
            LogToWindowFormat("PFLobbyGetServerProperty (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyGetServerConnectionStatus(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PFLobbyServerConnectionStatus connectionStatus{};
            HRESULT hr = PFLobbyGetServerConnectionStatus(nullptr, &connectionStatus);
            if (SUCCEEDED(hr)) { payload.result = static_cast<int>(connectionStatus); }
            LogToWindowFormat("PFLobbyGetServerConnectionStatus (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyPostUpdate(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PFLobbyPostUpdate(nullptr, nullptr, nullptr, nullptr, nullptr);
            LogToWindowFormat("PFLobbyPostUpdate (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyPostUpdateWithEntityHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFLobbyPostUpdateWithEntityHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbySendInvite(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PFLobbySendInvite(nullptr, nullptr, nullptr, nullptr);
            LogToWindowFormat("PFLobbySendInvite (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbySendInviteWithEntityHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFLobbySendInviteWithEntityHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyGetCustomContext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            void* customContext{};
            HRESULT hr = PFLobbyGetCustomContext(nullptr, &customContext);
            LogToWindowFormat("PFLobbyGetCustomContext (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbySetCustomContext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PFLobbySetCustomContext(nullptr, nullptr);
            LogToWindowFormat("PFLobbySetCustomContext (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerStartProcessingLobbyStateChanges(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t stateChangeCount{};
            const PFLobbyStateChange* const* stateChanges{};
            HRESULT hr = PFMultiplayerStartProcessingLobbyStateChanges(s_multiplayerHandle, &stateChangeCount, &stateChanges);
            if (SUCCEEDED(hr)) { payload.result = stateChangeCount; }
            LogToWindowFormat("PFMultiplayerStartProcessingLobbyStateChanges (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerFinishProcessingLobbyStateChanges(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PFMultiplayerFinishProcessingLobbyStateChanges(s_multiplayerHandle, 0, nullptr);
            LogToWindowFormat("PFMultiplayerFinishProcessingLobbyStateChanges (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerCreateAndJoinLobby(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PFLobbyHandle lobby{};
            HRESULT hr = PFMultiplayerCreateAndJoinLobby(s_multiplayerHandle, nullptr, nullptr, nullptr, nullptr, &lobby);
            LogToWindowFormat("PFMultiplayerCreateAndJoinLobby (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerCreateAndJoinLobbyWithEntityHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFMultiplayerCreateAndJoinLobbyWithEntityHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerJoinLobby(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PFLobbyHandle lobby{};
            HRESULT hr = PFMultiplayerJoinLobby(s_multiplayerHandle, nullptr, "connectionString", nullptr, nullptr, &lobby);
            LogToWindowFormat("PFMultiplayerJoinLobby (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerJoinLobbyWithEntityHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFMultiplayerJoinLobbyWithEntityHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerConnectToLobby(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PFLobbyHandle lobby{};
            HRESULT hr = PFMultiplayerConnectToLobby(s_multiplayerHandle, nullptr, "lobbyId", nullptr, &lobby);
            LogToWindowFormat("PFMultiplayerConnectToLobby (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerConnectToLobbyWithEntityHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFMultiplayerConnectToLobbyWithEntityHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerJoinArrangedLobby(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PFLobbyHandle lobby{};
            HRESULT hr = PFMultiplayerJoinArrangedLobby(s_multiplayerHandle, nullptr, "arrangementString", nullptr, nullptr, &lobby);
            LogToWindowFormat("PFMultiplayerJoinArrangedLobby (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerJoinArrangedLobbyWithEntityHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFMultiplayerJoinArrangedLobbyWithEntityHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerFindLobbies(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PFMultiplayerFindLobbies(s_multiplayerHandle, nullptr, nullptr, nullptr);
            LogToWindowFormat("PFMultiplayerFindLobbies (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerFindLobbiesWithEntityHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFMultiplayerFindLobbiesWithEntityHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerStartListeningForLobbyInvites(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PFMultiplayerStartListeningForLobbyInvites(s_multiplayerHandle, nullptr);
            LogToWindowFormat("PFMultiplayerStartListeningForLobbyInvites (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerStartListeningForLobbyInvitesWithEntityHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFMultiplayerStartListeningForLobbyInvitesWithEntityHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerStopListeningForLobbyInvites(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PFMultiplayerStopListeningForLobbyInvites(s_multiplayerHandle, nullptr);
            LogToWindowFormat("PFMultiplayerStopListeningForLobbyInvites (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerStopListeningForLobbyInvitesWithEntityHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFMultiplayerStopListeningForLobbyInvitesWithEntityHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerGetLobbyInviteListenerStatus(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PFLobbyInviteListenerStatus status{};
            HRESULT hr = PFMultiplayerGetLobbyInviteListenerStatus(s_multiplayerHandle, nullptr, &status);
            if (SUCCEEDED(hr)) { payload.result = static_cast<int>(status); }
            LogToWindowFormat("PFMultiplayerGetLobbyInviteListenerStatus (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerCreateAndClaimServerLobby(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFMultiplayerCreateAndClaimServerLobby (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerCreateAndClaimServerLobbyWithEntityHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFMultiplayerCreateAndClaimServerLobbyWithEntityHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerClaimServerLobby(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFMultiplayerClaimServerLobby (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerClaimServerLobbyWithEntityHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFMultiplayerClaimServerLobbyWithEntityHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerJoinLobbyAsServer(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFMultiplayerJoinLobbyAsServer (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerJoinLobbyAsServerWithEntityHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFMultiplayerJoinLobbyAsServerWithEntityHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyServerPostUpdate(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFLobbyServerPostUpdate (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyServerPostUpdateAsServer(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFLobbyServerPostUpdateAsServer (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyServerLeaveAsServer(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFLobbyServerLeaveAsServer (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFLobbyServerDeleteLobby(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFLobbyServerDeleteLobby (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerStartProcessingMatchmakingStateChanges(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t stateChangeCount{};
            const PFMatchmakingStateChange* const* stateChanges{};
            HRESULT hr = PFMultiplayerStartProcessingMatchmakingStateChanges(s_multiplayerHandle, &stateChangeCount, &stateChanges);
            if (SUCCEEDED(hr)) { payload.result = stateChangeCount; }
            LogToWindowFormat("PFMultiplayerStartProcessingMatchmakingStateChanges (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerFinishProcessingMatchmakingStateChanges(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PFMultiplayerFinishProcessingMatchmakingStateChanges(s_multiplayerHandle, 0, nullptr);
            LogToWindowFormat("PFMultiplayerFinishProcessingMatchmakingStateChanges (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerCreateMatchmakingTicket(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PFMatchmakingTicketHandle ticket{};
            HRESULT hr = PFMultiplayerCreateMatchmakingTicket(s_multiplayerHandle, 0, nullptr, nullptr, nullptr, nullptr, &ticket);
            LogToWindowFormat("PFMultiplayerCreateMatchmakingTicket (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerCreateMatchmakingTicketWithEntityHandles(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFMultiplayerCreateMatchmakingTicketWithEntityHandles (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerJoinMatchmakingTicketFromId(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PFMatchmakingTicketHandle ticket{};
            HRESULT hr = PFMultiplayerJoinMatchmakingTicketFromId(s_multiplayerHandle, 0, nullptr, nullptr, "ticketId", "queueName", nullptr, &ticket);
            LogToWindowFormat("PFMultiplayerJoinMatchmakingTicketFromId (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerJoinMatchmakingTicketFromIdWithEntityHandles(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFMultiplayerJoinMatchmakingTicketFromIdWithEntityHandles (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerDestroyMatchmakingTicket(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PFMultiplayerDestroyMatchmakingTicket(s_multiplayerHandle, nullptr);
            LogToWindowFormat("PFMultiplayerDestroyMatchmakingTicket (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMatchmakingTicketGetStatus(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PFMatchmakingTicketStatus status{};
            HRESULT hr = PFMatchmakingTicketGetStatus(nullptr, &status);
            if (SUCCEEDED(hr)) { payload.result = static_cast<int>(status); }
            LogToWindowFormat("PFMatchmakingTicketGetStatus (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMatchmakingTicketCancel(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PFMatchmakingTicketCancel(nullptr);
            LogToWindowFormat("PFMatchmakingTicketCancel (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMatchmakingTicketGetTicketId(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const char* id{};
            HRESULT hr = PFMatchmakingTicketGetTicketId(nullptr, &id);
            LogToWindowFormat("PFMatchmakingTicketGetTicketId (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMatchmakingTicketGetMatch(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const PFMatchmakingMatchDetails* match{};
            HRESULT hr = PFMatchmakingTicketGetMatch(nullptr, &match);
            LogToWindowFormat("PFMatchmakingTicketGetMatch (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMatchmakingTicketGetCustomContext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            void* customContext{};
            HRESULT hr = PFMatchmakingTicketGetCustomContext(nullptr, &customContext);
            LogToWindowFormat("PFMatchmakingTicketGetCustomContext (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMatchmakingTicketSetCustomContext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PFMatchmakingTicketSetCustomContext(nullptr, nullptr);
            LogToWindowFormat("PFMatchmakingTicketSetCustomContext (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerCreateServerBackfillTicket(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFMultiplayerCreateServerBackfillTicket (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerCreateServerBackfillTicketWithEntityHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = E_NOTIMPL;
            LogToWindowFormat("PFMultiplayerCreateServerBackfillTicketWithEntityHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerGetErrorMessage(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const char* message = PFMultiplayerGetErrorMessage(S_OK);
            HRESULT hr = (message != nullptr) ? S_OK : E_FAIL;
            LogToWindowFormat("PFMultiplayerGetErrorMessage (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerSetMemoryCallbacks(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PFMultiplayerSetMemoryCallbacks(nullptr, nullptr);
            LogToWindowFormat("PFMultiplayerSetMemoryCallbacks (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerSetThreadAffinityMask(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PFMultiplayerSetThreadAffinityMask(PFMultiplayerThreadId::Networking, PFMultiplayerAnyProcessor);
            LogToWindowFormat("PFMultiplayerSetThreadAffinityMask (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerInitialize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
#ifdef _GAMING_DESKTOP
            MultiplayerInitializationConfiguration config{};
            config.titleId = "TestTitle";
            config.multiplayerTaskQueue = nullptr;
            HRESULT hr = PFMultiplayerInitialize(&config, &s_multiplayerHandle);
#else
            HRESULT hr = PFMultiplayerInitialize("TestTitle", &s_multiplayerHandle);
#endif
            LogToWindowFormat("PFMultiplayerInitialize (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerUninitialize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PFMultiplayerUninitialize(s_multiplayerHandle);
            LogToWindowFormat("PFMultiplayerUninitialize (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerSetEntityToken(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PFMultiplayerSetEntityToken(s_multiplayerHandle, nullptr, "token");
            LogToWindowFormat("PFMultiplayerSetEntityToken (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "PFLobbyAddMember", HandlePFLobbyAddMember },
    { "PFLobbyAddMemberWithEntityHandle", HandlePFLobbyAddMemberWithEntityHandle },
    { "PFLobbyForceRemoveMember", HandlePFLobbyForceRemoveMember },
    { "PFLobbyGetAccessPolicy", HandlePFLobbyGetAccessPolicy },
    { "PFLobbyGetConnectionString", HandlePFLobbyGetConnectionString },
    { "PFLobbyGetCustomContext", HandlePFLobbyGetCustomContext },
    { "PFLobbyGetLobbyId", HandlePFLobbyGetLobbyId },
    { "PFLobbyGetLobbyProperty", HandlePFLobbyGetLobbyProperty },
    { "PFLobbyGetLobbyPropertyKeys", HandlePFLobbyGetLobbyPropertyKeys },
    { "PFLobbyGetMaxMemberCount", HandlePFLobbyGetMaxMemberCount },
    { "PFLobbyGetMemberConnectionStatus", HandlePFLobbyGetMemberConnectionStatus },
    { "PFLobbyGetMemberProperty", HandlePFLobbyGetMemberProperty },
    { "PFLobbyGetMemberPropertyKeys", HandlePFLobbyGetMemberPropertyKeys },
    { "PFLobbyGetMembers", HandlePFLobbyGetMembers },
    { "PFLobbyGetMembershipLock", HandlePFLobbyGetMembershipLock },
    { "PFLobbyGetOwner", HandlePFLobbyGetOwner },
    { "PFLobbyGetOwnerMigrationPolicy", HandlePFLobbyGetOwnerMigrationPolicy },
    { "PFLobbyGetSearchProperty", HandlePFLobbyGetSearchProperty },
    { "PFLobbyGetSearchPropertyKeys", HandlePFLobbyGetSearchPropertyKeys },
    { "PFLobbyGetServer", HandlePFLobbyGetServer },
    { "PFLobbyGetServerConnectionStatus", HandlePFLobbyGetServerConnectionStatus },
    { "PFLobbyGetServerProperty", HandlePFLobbyGetServerProperty },
    { "PFLobbyGetServerPropertyKeys", HandlePFLobbyGetServerPropertyKeys },
    { "PFLobbyLeave", HandlePFLobbyLeave },
    { "PFLobbyLeaveWithEntityHandle", HandlePFLobbyLeaveWithEntityHandle },
    { "PFLobbyPostUpdate", HandlePFLobbyPostUpdate },
    { "PFLobbyPostUpdateWithEntityHandle", HandlePFLobbyPostUpdateWithEntityHandle },
    { "PFLobbySendInvite", HandlePFLobbySendInvite },
    { "PFLobbySendInviteWithEntityHandle", HandlePFLobbySendInviteWithEntityHandle },
    { "PFLobbyServerDeleteLobby", HandlePFLobbyServerDeleteLobby },
    { "PFLobbyServerLeaveAsServer", HandlePFLobbyServerLeaveAsServer },
    { "PFLobbyServerPostUpdate", HandlePFLobbyServerPostUpdate },
    { "PFLobbyServerPostUpdateAsServer", HandlePFLobbyServerPostUpdateAsServer },
    { "PFLobbySetCustomContext", HandlePFLobbySetCustomContext },
    { "PFMatchmakingTicketCancel", HandlePFMatchmakingTicketCancel },
    { "PFMatchmakingTicketGetCustomContext", HandlePFMatchmakingTicketGetCustomContext },
    { "PFMatchmakingTicketGetMatch", HandlePFMatchmakingTicketGetMatch },
    { "PFMatchmakingTicketGetStatus", HandlePFMatchmakingTicketGetStatus },
    { "PFMatchmakingTicketGetTicketId", HandlePFMatchmakingTicketGetTicketId },
    { "PFMatchmakingTicketSetCustomContext", HandlePFMatchmakingTicketSetCustomContext },
    { "PFMultiplayerClaimServerLobby", HandlePFMultiplayerClaimServerLobby },
    { "PFMultiplayerClaimServerLobbyWithEntityHandle", HandlePFMultiplayerClaimServerLobbyWithEntityHandle },
    { "PFMultiplayerConnectToLobby", HandlePFMultiplayerConnectToLobby },
    { "PFMultiplayerConnectToLobbyWithEntityHandle", HandlePFMultiplayerConnectToLobbyWithEntityHandle },
    { "PFMultiplayerCreateAndClaimServerLobby", HandlePFMultiplayerCreateAndClaimServerLobby },
    { "PFMultiplayerCreateAndClaimServerLobbyWithEntityHandle", HandlePFMultiplayerCreateAndClaimServerLobbyWithEntityHandle },
    { "PFMultiplayerCreateAndJoinLobby", HandlePFMultiplayerCreateAndJoinLobby },
    { "PFMultiplayerCreateAndJoinLobbyWithEntityHandle", HandlePFMultiplayerCreateAndJoinLobbyWithEntityHandle },
    { "PFMultiplayerCreateMatchmakingTicket", HandlePFMultiplayerCreateMatchmakingTicket },
    { "PFMultiplayerCreateMatchmakingTicketWithEntityHandles", HandlePFMultiplayerCreateMatchmakingTicketWithEntityHandles },
    { "PFMultiplayerCreateServerBackfillTicket", HandlePFMultiplayerCreateServerBackfillTicket },
    { "PFMultiplayerCreateServerBackfillTicketWithEntityHandle", HandlePFMultiplayerCreateServerBackfillTicketWithEntityHandle },
    { "PFMultiplayerDestroyMatchmakingTicket", HandlePFMultiplayerDestroyMatchmakingTicket },
    { "PFMultiplayerFindLobbies", HandlePFMultiplayerFindLobbies },
    { "PFMultiplayerFindLobbiesWithEntityHandle", HandlePFMultiplayerFindLobbiesWithEntityHandle },
    { "PFMultiplayerFinishProcessingLobbyStateChanges", HandlePFMultiplayerFinishProcessingLobbyStateChanges },
    { "PFMultiplayerFinishProcessingMatchmakingStateChanges", HandlePFMultiplayerFinishProcessingMatchmakingStateChanges },
    { "PFMultiplayerGetErrorMessage", HandlePFMultiplayerGetErrorMessage },
    { "PFMultiplayerGetLobbyInviteListenerStatus", HandlePFMultiplayerGetLobbyInviteListenerStatus },
    { "PFMultiplayerInitialize", HandlePFMultiplayerInitialize },
    { "PFMultiplayerJoinArrangedLobby", HandlePFMultiplayerJoinArrangedLobby },
    { "PFMultiplayerJoinArrangedLobbyWithEntityHandle", HandlePFMultiplayerJoinArrangedLobbyWithEntityHandle },
    { "PFMultiplayerJoinLobby", HandlePFMultiplayerJoinLobby },
    { "PFMultiplayerJoinLobbyAsServer", HandlePFMultiplayerJoinLobbyAsServer },
    { "PFMultiplayerJoinLobbyAsServerWithEntityHandle", HandlePFMultiplayerJoinLobbyAsServerWithEntityHandle },
    { "PFMultiplayerJoinLobbyWithEntityHandle", HandlePFMultiplayerJoinLobbyWithEntityHandle },
    { "PFMultiplayerJoinMatchmakingTicketFromId", HandlePFMultiplayerJoinMatchmakingTicketFromId },
    { "PFMultiplayerJoinMatchmakingTicketFromIdWithEntityHandles", HandlePFMultiplayerJoinMatchmakingTicketFromIdWithEntityHandles },
    { "PFMultiplayerSetEntityToken", HandlePFMultiplayerSetEntityToken },
    { "PFMultiplayerSetMemoryCallbacks", HandlePFMultiplayerSetMemoryCallbacks },
    { "PFMultiplayerSetThreadAffinityMask", HandlePFMultiplayerSetThreadAffinityMask },
    { "PFMultiplayerStartListeningForLobbyInvites", HandlePFMultiplayerStartListeningForLobbyInvites },
    { "PFMultiplayerStartListeningForLobbyInvitesWithEntityHandle", HandlePFMultiplayerStartListeningForLobbyInvitesWithEntityHandle },
    { "PFMultiplayerStartProcessingLobbyStateChanges", HandlePFMultiplayerStartProcessingLobbyStateChanges },
    { "PFMultiplayerStartProcessingMatchmakingStateChanges", HandlePFMultiplayerStartProcessingMatchmakingStateChanges },
    { "PFMultiplayerStopListeningForLobbyInvites", HandlePFMultiplayerStopListeningForLobbyInvites },
    { "PFMultiplayerStopListeningForLobbyInvitesWithEntityHandle", HandlePFMultiplayerStopListeningForLobbyInvitesWithEntityHandle },
    { "PFMultiplayerUninitialize", HandlePFMultiplayerUninitialize }
});

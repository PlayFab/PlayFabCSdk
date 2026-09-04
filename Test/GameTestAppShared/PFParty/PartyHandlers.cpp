#include "pch.h"

#include "PartyHandlers.h"
#include <Party_c.h>
#include <PartyXboxLive_c.h>

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

static PARTY_HANDLE s_partyHandle{};
static PARTY_XBL_HANDLE s_partyXblHandle{};

CommandResultPayload HandlePartyXblChatUserIsLocal(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PartyBool isLocal{};
            PartyError err = PartyXblChatUserIsLocal(nullptr, &isLocal);
            if (err == 0) { payload.result = static_cast<bool>(isLocal); }
            LogToWindowFormat("PartyXblChatUserIsLocal (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblChatUserGetXboxUserId(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint64_t xboxUserId{};
            PartyError err = PartyXblChatUserGetXboxUserId(nullptr, &xboxUserId);
            if (err == 0) { payload.result = xboxUserId; }
            LogToWindowFormat("PartyXblChatUserGetXboxUserId (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblChatUserSetCustomContext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyXblChatUserSetCustomContext(nullptr, nullptr);
            LogToWindowFormat("PartyXblChatUserSetCustomContext (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblChatUserGetCustomContext(
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
            PartyError err = PartyXblChatUserGetCustomContext(nullptr, &customContext);
            LogToWindowFormat("PartyXblChatUserGetCustomContext (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblLocalChatUserGetAccessibilitySettings(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_XBL_ACCESSIBILITY_SETTINGS settings{};
            PartyError err = PartyXblLocalChatUserGetAccessibilitySettings(nullptr, &settings);
            LogToWindowFormat("PartyXblLocalChatUserGetAccessibilitySettings (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblLocalChatUserGetRequiredChatPermissionInfo(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_XBL_CHAT_PERMISSION_INFO chatPermissionInfo{};
            PartyError err = PartyXblLocalChatUserGetRequiredChatPermissionInfo(nullptr, nullptr, &chatPermissionInfo);
            LogToWindowFormat("PartyXblLocalChatUserGetRequiredChatPermissionInfo (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblLocalChatUserGetCrossNetworkCommunicationPrivacySetting(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PARTY_XBL_CROSS_NETWORK_COMMUNICATION_PRIVACY_SETTING setting{};
            PartyError err = PartyXblLocalChatUserGetCrossNetworkCommunicationPrivacySetting(nullptr, &setting);
            if (err == 0) { payload.result = static_cast<int>(setting); }
            LogToWindowFormat("PartyXblLocalChatUserGetCrossNetworkCommunicationPrivacySetting (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblGetErrorMessage(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyString errorMessage{};
            PartyError err = PartyXblGetErrorMessage(0, &errorMessage);
            LogToWindowFormat("PartyXblGetErrorMessage (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblSetMemoryCallbacks(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyXblSetMemoryCallbacks(nullptr, nullptr);
            LogToWindowFormat("PartyXblSetMemoryCallbacks (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblGetMemoryCallbacks(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_MEM_ALLOC_FUNC allocFunc{};
            PARTY_MEM_FREE_FUNC freeFunc{};
            PartyError err = PartyXblGetMemoryCallbacks(&allocFunc, &freeFunc);
            LogToWindowFormat("PartyXblGetMemoryCallbacks (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblSetProfilingCallbacksForMethodEntryExit(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyXblSetProfilingCallbacksForMethodEntryExit(nullptr, nullptr);
            LogToWindowFormat("PartyXblSetProfilingCallbacksForMethodEntryExit (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblGetProfilingCallbacksForMethodEntryExit(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_PROFILING_METHOD_ENTRANCE_FUNC entranceFunc{};
            PARTY_PROFILING_METHOD_EXIT_FUNC exitFunc{};
            PartyError err = PartyXblGetProfilingCallbacksForMethodEntryExit(&entranceFunc, &exitFunc);
            LogToWindowFormat("PartyXblGetProfilingCallbacksForMethodEntryExit (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblSetThreadAffinityMask(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyXblSetThreadAffinityMask(PARTY_XBL_THREAD_ID_WEB_REQUEST, 0xFFFFFFFFFFFFFFFF);
            LogToWindowFormat("PartyXblSetThreadAffinityMask (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblGetThreadAffinityMask(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint64_t affinityMask{};
            PartyError err = PartyXblGetThreadAffinityMask(PARTY_XBL_THREAD_ID_WEB_REQUEST, &affinityMask);
            if (err == 0) { payload.result = affinityMask; }
            LogToWindowFormat("PartyXblGetThreadAffinityMask (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblInitialize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyXblInitialize(s_partyHandle, "TestTitle", &s_partyXblHandle);
            LogToWindowFormat("PartyXblInitialize (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblCleanup(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyXblCleanup(s_partyXblHandle);
            LogToWindowFormat("PartyXblCleanup (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblStartProcessingStateChanges(
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
            const PARTY_XBL_STATE_CHANGE* const* stateChanges{};
            PartyError err = PartyXblStartProcessingStateChanges(s_partyXblHandle, &stateChangeCount, &stateChanges);
            if (err == 0) { payload.result = stateChangeCount; }
            LogToWindowFormat("PartyXblStartProcessingStateChanges (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblFinishProcessingStateChanges(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyXblFinishProcessingStateChanges(s_partyXblHandle, 0, nullptr);
            LogToWindowFormat("PartyXblFinishProcessingStateChanges (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblCompleteGetTokenAndSignatureRequest(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyXblCompleteGetTokenAndSignatureRequest(s_partyXblHandle, 0, true, nullptr, nullptr);
            LogToWindowFormat("PartyXblCompleteGetTokenAndSignatureRequest (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblCreateLocalChatUser(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_XBL_CHAT_USER_HANDLE chatUser{};
            PartyError err = PartyXblCreateLocalChatUser(s_partyXblHandle, 0, nullptr, &chatUser);
            LogToWindowFormat("PartyXblCreateLocalChatUser (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblCreateRemoteChatUser(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_XBL_CHAT_USER_HANDLE chatUser{};
            PartyError err = PartyXblCreateRemoteChatUser(s_partyXblHandle, 0, &chatUser);
            LogToWindowFormat("PartyXblCreateRemoteChatUser (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblDestroyChatUser(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyXblDestroyChatUser(s_partyXblHandle, nullptr);
            LogToWindowFormat("PartyXblDestroyChatUser (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblGetChatUsers(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t chatUserCount{};
            const PARTY_XBL_CHAT_USER_HANDLE* chatUsers{};
            PartyError err = PartyXblGetChatUsers(s_partyXblHandle, &chatUserCount, &chatUsers);
            if (err == 0) { payload.result = chatUserCount; }
            LogToWindowFormat("PartyXblGetChatUsers (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblLoginToPlayFab(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyXblLoginToPlayFab(nullptr, nullptr);
            LogToWindowFormat("PartyXblLoginToPlayFab (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyXblGetEntityIdsFromXboxLiveUserIds(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyXblGetEntityIdsFromXboxLiveUserIds(s_partyXblHandle, 0, nullptr, nullptr, nullptr);
            LogToWindowFormat("PartyXblGetEntityIdsFromXboxLiveUserIds (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyLocalUserGetEntityId(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyString entityId{};
            PartyError err = PartyLocalUserGetEntityId(nullptr, &entityId);
            LogToWindowFormat("PartyLocalUserGetEntityId (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyLocalUserGetEntityType(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyString entityType{};
            PartyError err = PartyLocalUserGetEntityType(nullptr, &entityType);
            LogToWindowFormat("PartyLocalUserGetEntityType (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyLocalUserUpdateEntityToken(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyLocalUserUpdateEntityToken(nullptr, "token");
            LogToWindowFormat("PartyLocalUserUpdateEntityToken (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyLocalUserGetCustomContext(
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
            PartyError err = PartyLocalUserGetCustomContext(nullptr, &customContext);
            LogToWindowFormat("PartyLocalUserGetCustomContext (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyLocalUserSetCustomContext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyLocalUserSetCustomContext(nullptr, nullptr);
            LogToWindowFormat("PartyLocalUserSetCustomContext (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyEndpointGetLocalUser(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_LOCAL_USER_HANDLE localUser{};
            PartyError err = PartyEndpointGetLocalUser(nullptr, &localUser);
            LogToWindowFormat("PartyEndpointGetLocalUser (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyEndpointSendMessage(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyEndpointSendMessage(nullptr, 0, nullptr, PARTY_SEND_MESSAGE_OPTIONS_GUARANTEED_DELIVERY, nullptr, 0, nullptr, nullptr);
            LogToWindowFormat("PartyEndpointSendMessage (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyEndpointCancelMessages(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t canceledCount{};
            PartyError err = PartyEndpointCancelMessages(nullptr, 0, nullptr, PARTY_CANCEL_MESSAGES_FILTER_EXPRESSION_NONE, 0, 0, &canceledCount);
            if (err == 0) { payload.result = canceledCount; }
            LogToWindowFormat("PartyEndpointCancelMessages (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyEndpointFlushMessages(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyEndpointFlushMessages(nullptr, 0, nullptr);
            LogToWindowFormat("PartyEndpointFlushMessages (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyEndpointGetEndpointStatistics(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PARTY_ENDPOINT_STATISTIC statType = PARTY_ENDPOINT_STATISTIC_CURRENTLY_QUEUED_SEND_MESSAGES;
            uint64_t statValue{};
            PartyError err = PartyEndpointGetEndpointStatistics(nullptr, 0, nullptr, 1, &statType, &statValue);
            if (err == 0) { payload.result = statValue; }
            LogToWindowFormat("PartyEndpointGetEndpointStatistics (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyEndpointIsLocal(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PartyBool isLocal{};
            PartyError err = PartyEndpointIsLocal(nullptr, &isLocal);
            if (err == 0) { payload.result = static_cast<bool>(isLocal); }
            LogToWindowFormat("PartyEndpointIsLocal (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyEndpointGetEntityId(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyString entityId{};
            PartyError err = PartyEndpointGetEntityId(nullptr, &entityId);
            LogToWindowFormat("PartyEndpointGetEntityId (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyEndpointGetEntityType(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyString entityType{};
            PartyError err = PartyEndpointGetEntityType(nullptr, &entityType);
            LogToWindowFormat("PartyEndpointGetEntityType (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyEndpointGetNetwork(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_NETWORK_HANDLE network{};
            PartyError err = PartyEndpointGetNetwork(nullptr, &network);
            LogToWindowFormat("PartyEndpointGetNetwork (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyEndpointGetDevice(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_DEVICE_HANDLE device{};
            PartyError err = PartyEndpointGetDevice(nullptr, &device);
            LogToWindowFormat("PartyEndpointGetDevice (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyEndpointGetUniqueIdentifier(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint16_t uniqueId{};
            PartyError err = PartyEndpointGetUniqueIdentifier(nullptr, &uniqueId);
            if (err == 0) { payload.result = uniqueId; }
            LogToWindowFormat("PartyEndpointGetUniqueIdentifier (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyEndpointGetSharedProperty(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_DATA_BUFFER value{};
            PartyError err = PartyEndpointGetSharedProperty(nullptr, "key", &value);
            LogToWindowFormat("PartyEndpointGetSharedProperty (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyEndpointGetSharedPropertyKeys(
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
            const PartyString* keys{};
            PartyError err = PartyEndpointGetSharedPropertyKeys(nullptr, &propertyCount, &keys);
            if (err == 0) { payload.result = propertyCount; }
            LogToWindowFormat("PartyEndpointGetSharedPropertyKeys (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyEndpointSetSharedProperties(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyEndpointSetSharedProperties(nullptr, 0, nullptr, nullptr);
            LogToWindowFormat("PartyEndpointSetSharedProperties (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyEndpointGetCustomContext(
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
            PartyError err = PartyEndpointGetCustomContext(nullptr, &customContext);
            LogToWindowFormat("PartyEndpointGetCustomContext (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyEndpointSetCustomContext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyEndpointSetCustomContext(nullptr, nullptr);
            LogToWindowFormat("PartyEndpointSetCustomContext (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyDeviceCreateChatControl(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_CHAT_CONTROL_HANDLE chatControl{};
            PartyError err = PartyDeviceCreateChatControl(nullptr, nullptr, nullptr, nullptr, &chatControl);
            LogToWindowFormat("PartyDeviceCreateChatControl (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyDeviceDestroyChatControl(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyDeviceDestroyChatControl(nullptr, nullptr, nullptr);
            LogToWindowFormat("PartyDeviceDestroyChatControl (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyDeviceIsLocal(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PartyBool isLocal{};
            PartyError err = PartyDeviceIsLocal(nullptr, &isLocal);
            if (err == 0) { payload.result = static_cast<bool>(isLocal); }
            LogToWindowFormat("PartyDeviceIsLocal (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyDeviceGetChatControls(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t chatControlCount{};
            const PARTY_CHAT_CONTROL_HANDLE* chatControls{};
            PartyError err = PartyDeviceGetChatControls(nullptr, &chatControlCount, &chatControls);
            if (err == 0) { payload.result = chatControlCount; }
            LogToWindowFormat("PartyDeviceGetChatControls (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyDeviceGetSharedProperty(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_DATA_BUFFER value{};
            PartyError err = PartyDeviceGetSharedProperty(nullptr, "key", &value);
            LogToWindowFormat("PartyDeviceGetSharedProperty (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyDeviceGetSharedPropertyKeys(
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
            const PartyString* keys{};
            PartyError err = PartyDeviceGetSharedPropertyKeys(nullptr, &propertyCount, &keys);
            if (err == 0) { payload.result = propertyCount; }
            LogToWindowFormat("PartyDeviceGetSharedPropertyKeys (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyDeviceSetSharedProperties(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyDeviceSetSharedProperties(nullptr, 0, nullptr, nullptr);
            LogToWindowFormat("PartyDeviceSetSharedProperties (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyDeviceGetCustomContext(
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
            PartyError err = PartyDeviceGetCustomContext(nullptr, &customContext);
            LogToWindowFormat("PartyDeviceGetCustomContext (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyDeviceSetCustomContext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyDeviceSetCustomContext(nullptr, nullptr);
            LogToWindowFormat("PartyDeviceSetCustomContext (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyInvitationGetCreatorEntityId(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyString entityId{};
            PartyError err = PartyInvitationGetCreatorEntityId(nullptr, &entityId);
            LogToWindowFormat("PartyInvitationGetCreatorEntityId (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyInvitationGetInvitationConfiguration(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const PARTY_INVITATION_CONFIGURATION* configuration{};
            PartyError err = PartyInvitationGetInvitationConfiguration(nullptr, &configuration);
            LogToWindowFormat("PartyInvitationGetInvitationConfiguration (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyInvitationGetCustomContext(
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
            PartyError err = PartyInvitationGetCustomContext(nullptr, &customContext);
            LogToWindowFormat("PartyInvitationGetCustomContext (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyInvitationSetCustomContext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyInvitationSetCustomContext(nullptr, nullptr);
            LogToWindowFormat("PartyInvitationSetCustomContext (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkAuthenticateLocalUser(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyNetworkAuthenticateLocalUser(nullptr, nullptr, "invitation", nullptr);
            LogToWindowFormat("PartyNetworkAuthenticateLocalUser (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkRemoveLocalUser(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyNetworkRemoveLocalUser(nullptr, nullptr, nullptr);
            LogToWindowFormat("PartyNetworkRemoveLocalUser (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkCreateInvitation(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_INVITATION_HANDLE invitation{};
            PartyError err = PartyNetworkCreateInvitation(nullptr, nullptr, nullptr, nullptr, &invitation);
            LogToWindowFormat("PartyNetworkCreateInvitation (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkRevokeInvitation(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyNetworkRevokeInvitation(nullptr, nullptr, nullptr, nullptr);
            LogToWindowFormat("PartyNetworkRevokeInvitation (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkGetInvitations(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t invitationCount{};
            const PARTY_INVITATION_HANDLE* invitations{};
            PartyError err = PartyNetworkGetInvitations(nullptr, &invitationCount, &invitations);
            if (err == 0) { payload.result = invitationCount; }
            LogToWindowFormat("PartyNetworkGetInvitations (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkCreateEndpoint(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_ENDPOINT_HANDLE endpoint{};
            PartyError err = PartyNetworkCreateEndpoint(nullptr, nullptr, 0, nullptr, nullptr, nullptr, &endpoint);
            LogToWindowFormat("PartyNetworkCreateEndpoint (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkDestroyEndpoint(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyNetworkDestroyEndpoint(nullptr, nullptr, nullptr);
            LogToWindowFormat("PartyNetworkDestroyEndpoint (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkLeaveNetwork(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyNetworkLeaveNetwork(nullptr, nullptr);
            LogToWindowFormat("PartyNetworkLeaveNetwork (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkGetEndpoints(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t endpointCount{};
            const PARTY_ENDPOINT_HANDLE* endpoints{};
            PartyError err = PartyNetworkGetEndpoints(nullptr, &endpointCount, &endpoints);
            if (err == 0) { payload.result = endpointCount; }
            LogToWindowFormat("PartyNetworkGetEndpoints (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkGetEndpointsByUserType(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t endpointCount{};
            const PARTY_ENDPOINT_HANDLE* endpoints{};
            PartyError err = PartyNetworkGetEndpointsByUserType(nullptr, PARTY_ENDPOINT_USER_TYPE_FILTER_NO_USER_FILTER, PARTY_ENDPOINT_LOCATION_FILTER_NO_LOCATION_FILTER, &endpointCount, &endpoints);
            if (err == 0) { payload.result = endpointCount; }
            LogToWindowFormat("PartyNetworkGetEndpointsByUserType (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkFindEndpointByUniqueIdentifier(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_ENDPOINT_HANDLE endpoint{};
            PartyError err = PartyNetworkFindEndpointByUniqueIdentifier(nullptr, 0, &endpoint);
            LogToWindowFormat("PartyNetworkFindEndpointByUniqueIdentifier (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkGetDevices(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t deviceCount{};
            const PARTY_DEVICE_HANDLE* devices{};
            PartyError err = PartyNetworkGetDevices(nullptr, &deviceCount, &devices);
            if (err == 0) { payload.result = deviceCount; }
            LogToWindowFormat("PartyNetworkGetDevices (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkGetLocalUsers(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t userCount{};
            const PARTY_LOCAL_USER_HANDLE* users{};
            PartyError err = PartyNetworkGetLocalUsers(nullptr, &userCount, &users);
            if (err == 0) { payload.result = userCount; }
            LogToWindowFormat("PartyNetworkGetLocalUsers (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkGetNetworkDescriptor(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_NETWORK_DESCRIPTOR networkDescriptor{};
            PartyError err = PartyNetworkGetNetworkDescriptor(nullptr, &networkDescriptor);
            LogToWindowFormat("PartyNetworkGetNetworkDescriptor (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkGetNetworkConfiguration(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const PARTY_NETWORK_CONFIGURATION* networkConfiguration{};
            PartyError err = PartyNetworkGetNetworkConfiguration(nullptr, &networkConfiguration);
            LogToWindowFormat("PartyNetworkGetNetworkConfiguration (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkKickDevice(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyNetworkKickDevice(nullptr, nullptr, nullptr);
            LogToWindowFormat("PartyNetworkKickDevice (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkKickUser(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyNetworkKickUser(nullptr, "entityId", nullptr);
            LogToWindowFormat("PartyNetworkKickUser (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkGetSharedProperty(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_DATA_BUFFER value{};
            PartyError err = PartyNetworkGetSharedProperty(nullptr, "key", &value);
            LogToWindowFormat("PartyNetworkGetSharedProperty (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkGetSharedPropertyKeys(
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
            const PartyString* keys{};
            PartyError err = PartyNetworkGetSharedPropertyKeys(nullptr, &propertyCount, &keys);
            if (err == 0) { payload.result = propertyCount; }
            LogToWindowFormat("PartyNetworkGetSharedPropertyKeys (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkSetSharedProperties(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyNetworkSetSharedProperties(nullptr, 0, nullptr, nullptr);
            LogToWindowFormat("PartyNetworkSetSharedProperties (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkConnectChatControl(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyNetworkConnectChatControl(nullptr, nullptr, nullptr);
            LogToWindowFormat("PartyNetworkConnectChatControl (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkDisconnectChatControl(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyNetworkDisconnectChatControl(nullptr, nullptr, nullptr);
            LogToWindowFormat("PartyNetworkDisconnectChatControl (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkGetChatControls(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t chatControlCount{};
            const PARTY_CHAT_CONTROL_HANDLE* chatControls{};
            PartyError err = PartyNetworkGetChatControls(nullptr, &chatControlCount, &chatControls);
            if (err == 0) { payload.result = chatControlCount; }
            LogToWindowFormat("PartyNetworkGetChatControls (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkGetNetworkStatistics(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PARTY_NETWORK_STATISTIC statType = PARTY_NETWORK_STATISTIC_AVERAGE_RELAY_SERVER_ROUND_TRIP_LATENCY_IN_MILLISECONDS;
            uint64_t statValue{};
            PartyError err = PartyNetworkGetNetworkStatistics(nullptr, 1, &statType, &statValue);
            if (err == 0) { payload.result = statValue; }
            LogToWindowFormat("PartyNetworkGetNetworkStatistics (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkGetCustomContext(
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
            PartyError err = PartyNetworkGetCustomContext(nullptr, &customContext);
            LogToWindowFormat("PartyNetworkGetCustomContext (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkSetCustomContext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyNetworkSetCustomContext(nullptr, nullptr);
            LogToWindowFormat("PartyNetworkSetCustomContext (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyNetworkGetDeviceConnectionType(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PARTY_DEVICE_CONNECTION_TYPE connectionType{};
            PartyError err = PartyNetworkGetDeviceConnectionType(nullptr, nullptr, &connectionType);
            if (err == 0) { payload.result = static_cast<int>(connectionType); }
            LogToWindowFormat("PartyNetworkGetDeviceConnectionType (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetLocalUser(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_LOCAL_USER_HANDLE localUser{};
            PartyError err = PartyChatControlGetLocalUser(nullptr, &localUser);
            LogToWindowFormat("PartyChatControlGetLocalUser (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlSetPermissions(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyChatControlSetPermissions(nullptr, nullptr, PARTY_CHAT_PERMISSION_OPTIONS_NONE);
            LogToWindowFormat("PartyChatControlSetPermissions (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetPermissions(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PARTY_CHAT_PERMISSION_OPTIONS options{};
            PartyError err = PartyChatControlGetPermissions(nullptr, nullptr, &options);
            if (err == 0) { payload.result = static_cast<int>(options); }
            LogToWindowFormat("PartyChatControlGetPermissions (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlSendText(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyChatControlSendText(nullptr, 0, nullptr, "test", 0, nullptr);
            LogToWindowFormat("PartyChatControlSendText (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlSetAudioInput(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyChatControlSetAudioInput(nullptr, PARTY_AUDIO_DEVICE_SELECTION_TYPE_NONE, nullptr, nullptr);
            LogToWindowFormat("PartyChatControlSetAudioInput (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetAudioInput(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_AUDIO_DEVICE_SELECTION_TYPE selectionType{};
            PartyString selectionContext{};
            PartyString deviceId{};
            PartyError err = PartyChatControlGetAudioInput(nullptr, &selectionType, &selectionContext, &deviceId);
            LogToWindowFormat("PartyChatControlGetAudioInput (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlSetAudioOutput(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyChatControlSetAudioOutput(nullptr, PARTY_AUDIO_DEVICE_SELECTION_TYPE_NONE, nullptr, nullptr);
            LogToWindowFormat("PartyChatControlSetAudioOutput (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetAudioOutput(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_AUDIO_DEVICE_SELECTION_TYPE selectionType{};
            PartyString selectionContext{};
            PartyString deviceId{};
            PartyError err = PartyChatControlGetAudioOutput(nullptr, &selectionType, &selectionContext, &deviceId);
            LogToWindowFormat("PartyChatControlGetAudioOutput (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlPopulateAvailableTextToSpeechProfiles(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyChatControlPopulateAvailableTextToSpeechProfiles(nullptr, nullptr);
            LogToWindowFormat("PartyChatControlPopulateAvailableTextToSpeechProfiles (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetAvailableTextToSpeechProfiles(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t profileCount{};
            const PARTY_TEXT_TO_SPEECH_PROFILE_HANDLE* profiles{};
            PartyError err = PartyChatControlGetAvailableTextToSpeechProfiles(nullptr, &profileCount, &profiles);
            if (err == 0) { payload.result = profileCount; }
            LogToWindowFormat("PartyChatControlGetAvailableTextToSpeechProfiles (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlSetTextToSpeechProfile(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyChatControlSetTextToSpeechProfile(nullptr, PARTY_SYNTHESIZE_TEXT_TO_SPEECH_TYPE_NARRATION, nullptr, nullptr);
            LogToWindowFormat("PartyChatControlSetTextToSpeechProfile (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetTextToSpeechProfile(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_TEXT_TO_SPEECH_PROFILE_HANDLE profile{};
            PartyError err = PartyChatControlGetTextToSpeechProfile(nullptr, PARTY_SYNTHESIZE_TEXT_TO_SPEECH_TYPE_NARRATION, &profile);
            LogToWindowFormat("PartyChatControlGetTextToSpeechProfile (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlSynthesizeTextToSpeech(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyChatControlSynthesizeTextToSpeech(nullptr, PARTY_SYNTHESIZE_TEXT_TO_SPEECH_TYPE_NARRATION, "test", nullptr);
            LogToWindowFormat("PartyChatControlSynthesizeTextToSpeech (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlSetLanguage(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyChatControlSetLanguage(nullptr, "en-US", nullptr);
            LogToWindowFormat("PartyChatControlSetLanguage (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetLanguage(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyString languageCode{};
            PartyError err = PartyChatControlGetLanguage(nullptr, &languageCode);
            LogToWindowFormat("PartyChatControlGetLanguage (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlSetTranscriptionOptions(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyChatControlSetTranscriptionOptions(nullptr, PARTY_VOICE_CHAT_TRANSCRIPTION_OPTIONS_NONE, nullptr);
            LogToWindowFormat("PartyChatControlSetTranscriptionOptions (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetTranscriptionOptions(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PARTY_VOICE_CHAT_TRANSCRIPTION_OPTIONS options{};
            PartyError err = PartyChatControlGetTranscriptionOptions(nullptr, &options);
            if (err == 0) { payload.result = static_cast<int>(options); }
            LogToWindowFormat("PartyChatControlGetTranscriptionOptions (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlSetTextChatOptions(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyChatControlSetTextChatOptions(nullptr, PARTY_TEXT_CHAT_OPTIONS_NONE, nullptr);
            LogToWindowFormat("PartyChatControlSetTextChatOptions (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetTextChatOptions(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PARTY_TEXT_CHAT_OPTIONS options{};
            PartyError err = PartyChatControlGetTextChatOptions(nullptr, &options);
            if (err == 0) { payload.result = static_cast<int>(options); }
            LogToWindowFormat("PartyChatControlGetTextChatOptions (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlSetAudioRenderVolume(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyChatControlSetAudioRenderVolume(nullptr, nullptr, 1.0f);
            LogToWindowFormat("PartyChatControlSetAudioRenderVolume (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetAudioRenderVolume(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            float volume{};
            PartyError err = PartyChatControlGetAudioRenderVolume(nullptr, nullptr, &volume);
            if (err == 0) { payload.result = volume; }
            LogToWindowFormat("PartyChatControlGetAudioRenderVolume (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlSetAudioInputMuted(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyChatControlSetAudioInputMuted(nullptr, false);
            LogToWindowFormat("PartyChatControlSetAudioInputMuted (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetAudioInputMuted(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PartyBool muted{};
            PartyError err = PartyChatControlGetAudioInputMuted(nullptr, &muted);
            if (err == 0) { payload.result = static_cast<bool>(muted); }
            LogToWindowFormat("PartyChatControlGetAudioInputMuted (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlSetIncomingAudioMuted(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyChatControlSetIncomingAudioMuted(nullptr, nullptr, false);
            LogToWindowFormat("PartyChatControlSetIncomingAudioMuted (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetIncomingAudioMuted(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PartyBool muted{};
            PartyError err = PartyChatControlGetIncomingAudioMuted(nullptr, nullptr, &muted);
            if (err == 0) { payload.result = static_cast<bool>(muted); }
            LogToWindowFormat("PartyChatControlGetIncomingAudioMuted (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlSetIncomingTextMuted(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyChatControlSetIncomingTextMuted(nullptr, nullptr, false);
            LogToWindowFormat("PartyChatControlSetIncomingTextMuted (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetIncomingTextMuted(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PartyBool muted{};
            PartyError err = PartyChatControlGetIncomingTextMuted(nullptr, nullptr, &muted);
            if (err == 0) { payload.result = static_cast<bool>(muted); }
            LogToWindowFormat("PartyChatControlGetIncomingTextMuted (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlSetVoiceAudioOptions(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyChatControlSetVoiceAudioOptions(nullptr, PARTY_VOICE_AUDIO_OPTIONS_NONE);
            LogToWindowFormat("PartyChatControlSetVoiceAudioOptions (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetVoiceAudioOptions(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PARTY_VOICE_AUDIO_OPTIONS options{};
            PartyError err = PartyChatControlGetVoiceAudioOptions(nullptr, &options);
            if (err == 0) { payload.result = static_cast<int>(options); }
            LogToWindowFormat("PartyChatControlGetVoiceAudioOptions (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetLocalChatIndicator(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PARTY_LOCAL_CHAT_CONTROL_CHAT_INDICATOR chatIndicator{};
            PartyError err = PartyChatControlGetLocalChatIndicator(nullptr, &chatIndicator);
            if (err == 0) { payload.result = static_cast<int>(chatIndicator); }
            LogToWindowFormat("PartyChatControlGetLocalChatIndicator (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetChatIndicator(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PARTY_CHAT_CONTROL_CHAT_INDICATOR chatIndicator{};
            PartyError err = PartyChatControlGetChatIndicator(nullptr, nullptr, &chatIndicator);
            if (err == 0) { payload.result = static_cast<int>(chatIndicator); }
            LogToWindowFormat("PartyChatControlGetChatIndicator (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlSetAudioEncoderBitrate(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyChatControlSetAudioEncoderBitrate(nullptr, 24000, nullptr);
            LogToWindowFormat("PartyChatControlSetAudioEncoderBitrate (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetAudioEncoderBitrate(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t bitrate{};
            PartyError err = PartyChatControlGetAudioEncoderBitrate(nullptr, &bitrate);
            if (err == 0) { payload.result = bitrate; }
            LogToWindowFormat("PartyChatControlGetAudioEncoderBitrate (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlIsLocal(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PartyBool isLocal{};
            PartyError err = PartyChatControlIsLocal(nullptr, &isLocal);
            if (err == 0) { payload.result = static_cast<bool>(isLocal); }
            LogToWindowFormat("PartyChatControlIsLocal (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetDevice(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_DEVICE_HANDLE device{};
            PartyError err = PartyChatControlGetDevice(nullptr, &device);
            LogToWindowFormat("PartyChatControlGetDevice (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetEntityId(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyString entityId{};
            PartyError err = PartyChatControlGetEntityId(nullptr, &entityId);
            LogToWindowFormat("PartyChatControlGetEntityId (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetEntityType(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyString entityType{};
            PartyError err = PartyChatControlGetEntityType(nullptr, &entityType);
            LogToWindowFormat("PartyChatControlGetEntityType (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetSharedProperty(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_DATA_BUFFER value{};
            PartyError err = PartyChatControlGetSharedProperty(nullptr, "key", &value);
            LogToWindowFormat("PartyChatControlGetSharedProperty (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetSharedPropertyKeys(
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
            const PartyString* keys{};
            PartyError err = PartyChatControlGetSharedPropertyKeys(nullptr, &propertyCount, &keys);
            if (err == 0) { payload.result = propertyCount; }
            LogToWindowFormat("PartyChatControlGetSharedPropertyKeys (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlSetSharedProperties(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyChatControlSetSharedProperties(nullptr, 0, nullptr, nullptr);
            LogToWindowFormat("PartyChatControlSetSharedProperties (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetNetworks(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t networkCount{};
            const PARTY_NETWORK_HANDLE* networks{};
            PartyError err = PartyChatControlGetNetworks(nullptr, &networkCount, &networks);
            if (err == 0) { payload.result = networkCount; }
            LogToWindowFormat("PartyChatControlGetNetworks (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlSetCustomContext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyChatControlSetCustomContext(nullptr, nullptr);
            LogToWindowFormat("PartyChatControlSetCustomContext (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetCustomContext(
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
            PartyError err = PartyChatControlGetCustomContext(nullptr, &customContext);
            LogToWindowFormat("PartyChatControlGetCustomContext (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlConfigureAudioManipulationVoiceStream(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyChatControlConfigureAudioManipulationVoiceStream(nullptr, nullptr, nullptr);
            LogToWindowFormat("PartyChatControlConfigureAudioManipulationVoiceStream (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetAudioManipulationVoiceStream(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_AUDIO_MANIPULATION_SOURCE_STREAM_HANDLE stream{};
            PartyError err = PartyChatControlGetAudioManipulationVoiceStream(nullptr, &stream);
            LogToWindowFormat("PartyChatControlGetAudioManipulationVoiceStream (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlConfigureAudioManipulationCaptureStream(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyChatControlConfigureAudioManipulationCaptureStream(nullptr, nullptr, nullptr);
            LogToWindowFormat("PartyChatControlConfigureAudioManipulationCaptureStream (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetAudioManipulationCaptureStream(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_AUDIO_MANIPULATION_SINK_STREAM_HANDLE stream{};
            PartyError err = PartyChatControlGetAudioManipulationCaptureStream(nullptr, &stream);
            LogToWindowFormat("PartyChatControlGetAudioManipulationCaptureStream (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlConfigureAudioManipulationRenderStream(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyChatControlConfigureAudioManipulationRenderStream(nullptr, nullptr, nullptr);
            LogToWindowFormat("PartyChatControlConfigureAudioManipulationRenderStream (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyChatControlGetAudioManipulationRenderStream(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_AUDIO_MANIPULATION_SINK_STREAM_HANDLE stream{};
            PartyError err = PartyChatControlGetAudioManipulationRenderStream(nullptr, &stream);
            LogToWindowFormat("PartyChatControlGetAudioManipulationRenderStream (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyTextToSpeechProfileGetIdentifier(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyString identifier{};
            PartyError err = PartyTextToSpeechProfileGetIdentifier(nullptr, &identifier);
            LogToWindowFormat("PartyTextToSpeechProfileGetIdentifier (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyTextToSpeechProfileGetName(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyString name{};
            PartyError err = PartyTextToSpeechProfileGetName(nullptr, &name);
            LogToWindowFormat("PartyTextToSpeechProfileGetName (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyTextToSpeechProfileGetLanguageCode(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyString languageCode{};
            PartyError err = PartyTextToSpeechProfileGetLanguageCode(nullptr, &languageCode);
            LogToWindowFormat("PartyTextToSpeechProfileGetLanguageCode (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyTextToSpeechProfileGetGender(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PARTY_GENDER gender{};
            PartyError err = PartyTextToSpeechProfileGetGender(nullptr, &gender);
            if (err == 0) { payload.result = static_cast<int>(gender); }
            LogToWindowFormat("PartyTextToSpeechProfileGetGender (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyTextToSpeechProfileGetCustomContext(
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
            PartyError err = PartyTextToSpeechProfileGetCustomContext(nullptr, &customContext);
            LogToWindowFormat("PartyTextToSpeechProfileGetCustomContext (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyTextToSpeechProfileSetCustomContext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyTextToSpeechProfileSetCustomContext(nullptr, nullptr);
            LogToWindowFormat("PartyTextToSpeechProfileSetCustomContext (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyAudioManipulationSourceStreamGetConfiguration(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_AUDIO_MANIPULATION_SOURCE_STREAM_CONFIGURATION configuration{};
            PartyError err = PartyAudioManipulationSourceStreamGetConfiguration(nullptr, &configuration);
            LogToWindowFormat("PartyAudioManipulationSourceStreamGetConfiguration (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyAudioManipulationSourceStreamGetFormat(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_AUDIO_FORMAT format{};
            PartyError err = PartyAudioManipulationSourceStreamGetFormat(nullptr, &format);
            LogToWindowFormat("PartyAudioManipulationSourceStreamGetFormat (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyAudioManipulationSourceStreamGetAvailableBufferCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t count{};
            PartyError err = PartyAudioManipulationSourceStreamGetAvailableBufferCount(nullptr, &count);
            if (err == 0) { payload.result = count; }
            LogToWindowFormat("PartyAudioManipulationSourceStreamGetAvailableBufferCount (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyAudioManipulationSourceStreamGetNextBuffer(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_MUTABLE_DATA_BUFFER buffer{};
            PartyError err = PartyAudioManipulationSourceStreamGetNextBuffer(nullptr, &buffer);
            LogToWindowFormat("PartyAudioManipulationSourceStreamGetNextBuffer (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyAudioManipulationSourceStreamReturnBuffer(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyAudioManipulationSourceStreamReturnBuffer(nullptr, nullptr);
            LogToWindowFormat("PartyAudioManipulationSourceStreamReturnBuffer (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyAudioManipulationSourceStreamGetCustomContext(
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
            PartyError err = PartyAudioManipulationSourceStreamGetCustomContext(nullptr, &customContext);
            LogToWindowFormat("PartyAudioManipulationSourceStreamGetCustomContext (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyAudioManipulationSourceStreamSetCustomContext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyAudioManipulationSourceStreamSetCustomContext(nullptr, nullptr);
            LogToWindowFormat("PartyAudioManipulationSourceStreamSetCustomContext (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyAudioManipulationSinkStreamGetConfiguration(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_AUDIO_MANIPULATION_SINK_STREAM_CONFIGURATION configuration{};
            PartyError err = PartyAudioManipulationSinkStreamGetConfiguration(nullptr, &configuration);
            LogToWindowFormat("PartyAudioManipulationSinkStreamGetConfiguration (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyAudioManipulationSinkStreamGetFormat(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_AUDIO_FORMAT format{};
            PartyError err = PartyAudioManipulationSinkStreamGetFormat(nullptr, &format);
            LogToWindowFormat("PartyAudioManipulationSinkStreamGetFormat (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyAudioManipulationSinkStreamSubmitBuffer(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyAudioManipulationSinkStreamSubmitBuffer(nullptr, nullptr);
            LogToWindowFormat("PartyAudioManipulationSinkStreamSubmitBuffer (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyAudioManipulationSinkStreamGetCustomContext(
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
            PartyError err = PartyAudioManipulationSinkStreamGetCustomContext(nullptr, &customContext);
            LogToWindowFormat("PartyAudioManipulationSinkStreamGetCustomContext (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyAudioManipulationSinkStreamSetCustomContext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyAudioManipulationSinkStreamSetCustomContext(nullptr, nullptr);
            LogToWindowFormat("PartyAudioManipulationSinkStreamSetCustomContext (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartySetOption(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartySetOption(nullptr, PARTY_OPTION_LOCAL_UDP_SOCKET_BIND_ADDRESS, nullptr);
            LogToWindowFormat("PartySetOption (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyGetOption(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            void* value{};
            PartyError err = PartyGetOption(nullptr, PARTY_OPTION_LOCAL_UDP_SOCKET_BIND_ADDRESS, &value);
            LogToWindowFormat("PartyGetOption (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyGetErrorMessage(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyString errorMessage{};
            PartyError err = PartyGetErrorMessage(0, &errorMessage);
            LogToWindowFormat("PartyGetErrorMessage (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartySerializeNetworkDescriptor(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_NETWORK_DESCRIPTOR descriptor{};
            char serialized[PARTY_MAX_SERIALIZED_NETWORK_DESCRIPTOR_STRING_LENGTH + 1]{};
            PartyError err = PartySerializeNetworkDescriptor(&descriptor, serialized);
            LogToWindowFormat("PartySerializeNetworkDescriptor (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyDeserializeNetworkDescriptor(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_NETWORK_DESCRIPTOR descriptor{};
            PartyError err = PartyDeserializeNetworkDescriptor("", &descriptor);
            LogToWindowFormat("PartyDeserializeNetworkDescriptor (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartySetMemoryCallbacks(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartySetMemoryCallbacks(nullptr, nullptr);
            LogToWindowFormat("PartySetMemoryCallbacks (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyGetMemoryCallbacks(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_MEM_ALLOC_FUNC allocFunc{};
            PARTY_MEM_FREE_FUNC freeFunc{};
            PartyError err = PartyGetMemoryCallbacks(&allocFunc, &freeFunc);
            LogToWindowFormat("PartyGetMemoryCallbacks (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartySetProfilingCallbacksForMethodEntryExit(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartySetProfilingCallbacksForMethodEntryExit(nullptr, nullptr);
            LogToWindowFormat("PartySetProfilingCallbacksForMethodEntryExit (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyGetProfilingCallbacksForMethodEntryExit(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_PROFILING_METHOD_ENTRANCE_FUNC entranceFunc{};
            PARTY_PROFILING_METHOD_EXIT_FUNC exitFunc{};
            PartyError err = PartyGetProfilingCallbacksForMethodEntryExit(&entranceFunc, &exitFunc);
            LogToWindowFormat("PartyGetProfilingCallbacksForMethodEntryExit (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartySetThreadAffinityMask(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartySetThreadAffinityMask(PARTY_THREAD_ID_NETWORKING, 0xFFFFFFFFFFFFFFFF);
            LogToWindowFormat("PartySetThreadAffinityMask (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyGetThreadAffinityMask(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint64_t affinityMask{};
            PartyError err = PartyGetThreadAffinityMask(PARTY_THREAD_ID_NETWORKING, &affinityMask);
            if (err == 0) { payload.result = affinityMask; }
            LogToWindowFormat("PartyGetThreadAffinityMask (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartySetWorkMode(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartySetWorkMode(PARTY_THREAD_ID_NETWORKING, PARTY_WORK_MODE_AUTOMATIC);
            LogToWindowFormat("PartySetWorkMode (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyGetWorkMode(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            PARTY_WORK_MODE workMode{};
            PartyError err = PartyGetWorkMode(PARTY_THREAD_ID_NETWORKING, &workMode);
            if (err == 0) { payload.result = static_cast<int>(workMode); }
            LogToWindowFormat("PartyGetWorkMode (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyInitialize(
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
            PARTY_INITIALIZATION_CONFIGURATION config{};
            config.titleId = "TestTitle";
            config.audioTaskQueue = nullptr;
            config.networkingTaskQueue = nullptr;
            PartyError err = PartyInitialize(&config, &s_partyHandle);
#else
            PartyError err = PartyInitialize("TestTitle", &s_partyHandle);
#endif
            LogToWindowFormat("PartyInitialize (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyCleanup(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyCleanup(s_partyHandle);
            LogToWindowFormat("PartyCleanup (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyStartProcessingStateChanges(
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
            const PARTY_STATE_CHANGE* const* stateChanges{};
            PartyError err = PartyStartProcessingStateChanges(s_partyHandle, &stateChangeCount, &stateChanges);
            if (err == 0) { payload.result = stateChangeCount; }
            LogToWindowFormat("PartyStartProcessingStateChanges (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyFinishProcessingStateChanges(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyFinishProcessingStateChanges(s_partyHandle, 0, nullptr);
            LogToWindowFormat("PartyFinishProcessingStateChanges (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyDoWork(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyDoWork(s_partyHandle, PARTY_THREAD_ID_NETWORKING);
            LogToWindowFormat("PartyDoWork (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyGetRegions(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t regionCount{};
            const PARTY_REGION* regions{};
            PartyError err = PartyGetRegions(s_partyHandle, &regionCount, &regions);
            if (err == 0) { payload.result = regionCount; }
            LogToWindowFormat("PartyGetRegions (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyCreateNewNetwork(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_NETWORK_DESCRIPTOR descriptor{};
            PartyError err = PartyCreateNewNetwork(s_partyHandle, nullptr, nullptr, 0, nullptr, nullptr, nullptr, &descriptor, nullptr);
            LogToWindowFormat("PartyCreateNewNetwork (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyConnectToNetwork(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_NETWORK_DESCRIPTOR descriptor{};
            PARTY_NETWORK_HANDLE network{};
            PartyError err = PartyConnectToNetwork(s_partyHandle, &descriptor, nullptr, &network);
            LogToWindowFormat("PartyConnectToNetwork (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartySynchronizeMessagesBetweenEndpoints(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartySynchronizeMessagesBetweenEndpoints(s_partyHandle, 0, nullptr, PARTY_SYNCHRONIZE_MESSAGES_BETWEEN_ENDPOINTS_OPTIONS_NONE, nullptr);
            LogToWindowFormat("PartySynchronizeMessagesBetweenEndpoints (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyGetLocalDevice(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PARTY_DEVICE_HANDLE localDevice{};
            PartyError err = PartyGetLocalDevice(s_partyHandle, &localDevice);
            LogToWindowFormat("PartyGetLocalDevice (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyCreateLocalUser(
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
            PARTY_LOCAL_USER_HANDLE localUser{};
            PartyError err = PartyCreateLocalUser(s_partyHandle, nullptr, &localUser);
#else
            PARTY_LOCAL_USER_HANDLE localUser{};
            PartyError err = PartyCreateLocalUser(s_partyHandle, "entityId", "entityToken", &localUser);
#endif
            LogToWindowFormat("PartyCreateLocalUser (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyDestroyLocalUser(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PartyError err = PartyDestroyLocalUser(s_partyHandle, nullptr, nullptr);
            LogToWindowFormat("PartyDestroyLocalUser (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyGetLocalUsers(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t userCount{};
            const PARTY_LOCAL_USER_HANDLE* users{};
            PartyError err = PartyGetLocalUsers(s_partyHandle, &userCount, &users);
            if (err == 0) { payload.result = userCount; }
            LogToWindowFormat("PartyGetLocalUsers (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyGetNetworks(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t networkCount{};
            const PARTY_NETWORK_HANDLE* networks{};
            PartyError err = PartyGetNetworks(s_partyHandle, &networkCount, &networks);
            if (err == 0) { payload.result = networkCount; }
            LogToWindowFormat("PartyGetNetworks (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

CommandResultPayload HandlePartyGetChatControls(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t chatControlCount{};
            const PARTY_CHAT_CONTROL_HANDLE* chatControls{};
            PartyError err = PartyGetChatControls(s_partyHandle, &chatControlCount, &chatControls);
            if (err == 0) { payload.result = chatControlCount; }
            LogToWindowFormat("PartyGetChatControls (err=0x%08X)", static_cast<uint32_t>(err));
            return static_cast<HRESULT>(err);
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "PartyAudioManipulationSinkStreamGetConfiguration", HandlePartyAudioManipulationSinkStreamGetConfiguration },
    { "PartyAudioManipulationSinkStreamGetCustomContext", HandlePartyAudioManipulationSinkStreamGetCustomContext },
    { "PartyAudioManipulationSinkStreamGetFormat", HandlePartyAudioManipulationSinkStreamGetFormat },
    { "PartyAudioManipulationSinkStreamSetCustomContext", HandlePartyAudioManipulationSinkStreamSetCustomContext },
    { "PartyAudioManipulationSinkStreamSubmitBuffer", HandlePartyAudioManipulationSinkStreamSubmitBuffer },
    { "PartyAudioManipulationSourceStreamGetAvailableBufferCount", HandlePartyAudioManipulationSourceStreamGetAvailableBufferCount },
    { "PartyAudioManipulationSourceStreamGetConfiguration", HandlePartyAudioManipulationSourceStreamGetConfiguration },
    { "PartyAudioManipulationSourceStreamGetCustomContext", HandlePartyAudioManipulationSourceStreamGetCustomContext },
    { "PartyAudioManipulationSourceStreamGetFormat", HandlePartyAudioManipulationSourceStreamGetFormat },
    { "PartyAudioManipulationSourceStreamGetNextBuffer", HandlePartyAudioManipulationSourceStreamGetNextBuffer },
    { "PartyAudioManipulationSourceStreamReturnBuffer", HandlePartyAudioManipulationSourceStreamReturnBuffer },
    { "PartyAudioManipulationSourceStreamSetCustomContext", HandlePartyAudioManipulationSourceStreamSetCustomContext },
    { "PartyChatControlConfigureAudioManipulationCaptureStream", HandlePartyChatControlConfigureAudioManipulationCaptureStream },
    { "PartyChatControlConfigureAudioManipulationRenderStream", HandlePartyChatControlConfigureAudioManipulationRenderStream },
    { "PartyChatControlConfigureAudioManipulationVoiceStream", HandlePartyChatControlConfigureAudioManipulationVoiceStream },
    { "PartyChatControlGetAudioEncoderBitrate", HandlePartyChatControlGetAudioEncoderBitrate },
    { "PartyChatControlGetAudioInput", HandlePartyChatControlGetAudioInput },
    { "PartyChatControlGetAudioInputMuted", HandlePartyChatControlGetAudioInputMuted },
    { "PartyChatControlGetAudioManipulationCaptureStream", HandlePartyChatControlGetAudioManipulationCaptureStream },
    { "PartyChatControlGetAudioManipulationRenderStream", HandlePartyChatControlGetAudioManipulationRenderStream },
    { "PartyChatControlGetAudioManipulationVoiceStream", HandlePartyChatControlGetAudioManipulationVoiceStream },
    { "PartyChatControlGetAudioOutput", HandlePartyChatControlGetAudioOutput },
    { "PartyChatControlGetAudioRenderVolume", HandlePartyChatControlGetAudioRenderVolume },
    { "PartyChatControlGetAvailableTextToSpeechProfiles", HandlePartyChatControlGetAvailableTextToSpeechProfiles },
    { "PartyChatControlGetChatIndicator", HandlePartyChatControlGetChatIndicator },
    { "PartyChatControlGetCustomContext", HandlePartyChatControlGetCustomContext },
    { "PartyChatControlGetDevice", HandlePartyChatControlGetDevice },
    { "PartyChatControlGetEntityId", HandlePartyChatControlGetEntityId },
    { "PartyChatControlGetEntityType", HandlePartyChatControlGetEntityType },
    { "PartyChatControlGetIncomingAudioMuted", HandlePartyChatControlGetIncomingAudioMuted },
    { "PartyChatControlGetIncomingTextMuted", HandlePartyChatControlGetIncomingTextMuted },
    { "PartyChatControlGetLanguage", HandlePartyChatControlGetLanguage },
    { "PartyChatControlGetLocalChatIndicator", HandlePartyChatControlGetLocalChatIndicator },
    { "PartyChatControlGetLocalUser", HandlePartyChatControlGetLocalUser },
    { "PartyChatControlGetNetworks", HandlePartyChatControlGetNetworks },
    { "PartyChatControlGetPermissions", HandlePartyChatControlGetPermissions },
    { "PartyChatControlGetSharedProperty", HandlePartyChatControlGetSharedProperty },
    { "PartyChatControlGetSharedPropertyKeys", HandlePartyChatControlGetSharedPropertyKeys },
    { "PartyChatControlGetTextChatOptions", HandlePartyChatControlGetTextChatOptions },
    { "PartyChatControlGetTextToSpeechProfile", HandlePartyChatControlGetTextToSpeechProfile },
    { "PartyChatControlGetTranscriptionOptions", HandlePartyChatControlGetTranscriptionOptions },
    { "PartyChatControlGetVoiceAudioOptions", HandlePartyChatControlGetVoiceAudioOptions },
    { "PartyChatControlIsLocal", HandlePartyChatControlIsLocal },
    { "PartyChatControlPopulateAvailableTextToSpeechProfiles", HandlePartyChatControlPopulateAvailableTextToSpeechProfiles },
    { "PartyChatControlSendText", HandlePartyChatControlSendText },
    { "PartyChatControlSetAudioEncoderBitrate", HandlePartyChatControlSetAudioEncoderBitrate },
    { "PartyChatControlSetAudioInput", HandlePartyChatControlSetAudioInput },
    { "PartyChatControlSetAudioInputMuted", HandlePartyChatControlSetAudioInputMuted },
    { "PartyChatControlSetAudioOutput", HandlePartyChatControlSetAudioOutput },
    { "PartyChatControlSetAudioRenderVolume", HandlePartyChatControlSetAudioRenderVolume },
    { "PartyChatControlSetCustomContext", HandlePartyChatControlSetCustomContext },
    { "PartyChatControlSetIncomingAudioMuted", HandlePartyChatControlSetIncomingAudioMuted },
    { "PartyChatControlSetIncomingTextMuted", HandlePartyChatControlSetIncomingTextMuted },
    { "PartyChatControlSetLanguage", HandlePartyChatControlSetLanguage },
    { "PartyChatControlSetPermissions", HandlePartyChatControlSetPermissions },
    { "PartyChatControlSetSharedProperties", HandlePartyChatControlSetSharedProperties },
    { "PartyChatControlSetTextChatOptions", HandlePartyChatControlSetTextChatOptions },
    { "PartyChatControlSetTextToSpeechProfile", HandlePartyChatControlSetTextToSpeechProfile },
    { "PartyChatControlSetTranscriptionOptions", HandlePartyChatControlSetTranscriptionOptions },
    { "PartyChatControlSetVoiceAudioOptions", HandlePartyChatControlSetVoiceAudioOptions },
    { "PartyChatControlSynthesizeTextToSpeech", HandlePartyChatControlSynthesizeTextToSpeech },
    { "PartyCleanup", HandlePartyCleanup },
    { "PartyConnectToNetwork", HandlePartyConnectToNetwork },
    { "PartyCreateLocalUser", HandlePartyCreateLocalUser },
    { "PartyCreateNewNetwork", HandlePartyCreateNewNetwork },
    { "PartyDeserializeNetworkDescriptor", HandlePartyDeserializeNetworkDescriptor },
    { "PartyDestroyLocalUser", HandlePartyDestroyLocalUser },
    { "PartyDeviceCreateChatControl", HandlePartyDeviceCreateChatControl },
    { "PartyDeviceDestroyChatControl", HandlePartyDeviceDestroyChatControl },
    { "PartyDeviceGetChatControls", HandlePartyDeviceGetChatControls },
    { "PartyDeviceGetCustomContext", HandlePartyDeviceGetCustomContext },
    { "PartyDeviceGetSharedProperty", HandlePartyDeviceGetSharedProperty },
    { "PartyDeviceGetSharedPropertyKeys", HandlePartyDeviceGetSharedPropertyKeys },
    { "PartyDeviceIsLocal", HandlePartyDeviceIsLocal },
    { "PartyDeviceSetCustomContext", HandlePartyDeviceSetCustomContext },
    { "PartyDeviceSetSharedProperties", HandlePartyDeviceSetSharedProperties },
    { "PartyDoWork", HandlePartyDoWork },
    { "PartyEndpointCancelMessages", HandlePartyEndpointCancelMessages },
    { "PartyEndpointFlushMessages", HandlePartyEndpointFlushMessages },
    { "PartyEndpointGetCustomContext", HandlePartyEndpointGetCustomContext },
    { "PartyEndpointGetDevice", HandlePartyEndpointGetDevice },
    { "PartyEndpointGetEndpointStatistics", HandlePartyEndpointGetEndpointStatistics },
    { "PartyEndpointGetEntityId", HandlePartyEndpointGetEntityId },
    { "PartyEndpointGetEntityType", HandlePartyEndpointGetEntityType },
    { "PartyEndpointGetLocalUser", HandlePartyEndpointGetLocalUser },
    { "PartyEndpointGetNetwork", HandlePartyEndpointGetNetwork },
    { "PartyEndpointGetSharedProperty", HandlePartyEndpointGetSharedProperty },
    { "PartyEndpointGetSharedPropertyKeys", HandlePartyEndpointGetSharedPropertyKeys },
    { "PartyEndpointGetUniqueIdentifier", HandlePartyEndpointGetUniqueIdentifier },
    { "PartyEndpointIsLocal", HandlePartyEndpointIsLocal },
    { "PartyEndpointSendMessage", HandlePartyEndpointSendMessage },
    { "PartyEndpointSetCustomContext", HandlePartyEndpointSetCustomContext },
    { "PartyEndpointSetSharedProperties", HandlePartyEndpointSetSharedProperties },
    { "PartyFinishProcessingStateChanges", HandlePartyFinishProcessingStateChanges },
    { "PartyGetChatControls", HandlePartyGetChatControls },
    { "PartyGetErrorMessage", HandlePartyGetErrorMessage },
    { "PartyGetLocalDevice", HandlePartyGetLocalDevice },
    { "PartyGetLocalUsers", HandlePartyGetLocalUsers },
    { "PartyGetMemoryCallbacks", HandlePartyGetMemoryCallbacks },
    { "PartyGetNetworks", HandlePartyGetNetworks },
    { "PartyGetOption", HandlePartyGetOption },
    { "PartyGetProfilingCallbacksForMethodEntryExit", HandlePartyGetProfilingCallbacksForMethodEntryExit },
    { "PartyGetRegions", HandlePartyGetRegions },
    { "PartyGetThreadAffinityMask", HandlePartyGetThreadAffinityMask },
    { "PartyGetWorkMode", HandlePartyGetWorkMode },
    { "PartyInitialize", HandlePartyInitialize },
    { "PartyInvitationGetCreatorEntityId", HandlePartyInvitationGetCreatorEntityId },
    { "PartyInvitationGetCustomContext", HandlePartyInvitationGetCustomContext },
    { "PartyInvitationGetInvitationConfiguration", HandlePartyInvitationGetInvitationConfiguration },
    { "PartyInvitationSetCustomContext", HandlePartyInvitationSetCustomContext },
    { "PartyLocalUserGetCustomContext", HandlePartyLocalUserGetCustomContext },
    { "PartyLocalUserGetEntityId", HandlePartyLocalUserGetEntityId },
    { "PartyLocalUserGetEntityType", HandlePartyLocalUserGetEntityType },
    { "PartyLocalUserSetCustomContext", HandlePartyLocalUserSetCustomContext },
    { "PartyLocalUserUpdateEntityToken", HandlePartyLocalUserUpdateEntityToken },
    { "PartyNetworkAuthenticateLocalUser", HandlePartyNetworkAuthenticateLocalUser },
    { "PartyNetworkConnectChatControl", HandlePartyNetworkConnectChatControl },
    { "PartyNetworkCreateEndpoint", HandlePartyNetworkCreateEndpoint },
    { "PartyNetworkCreateInvitation", HandlePartyNetworkCreateInvitation },
    { "PartyNetworkDestroyEndpoint", HandlePartyNetworkDestroyEndpoint },
    { "PartyNetworkDisconnectChatControl", HandlePartyNetworkDisconnectChatControl },
    { "PartyNetworkFindEndpointByUniqueIdentifier", HandlePartyNetworkFindEndpointByUniqueIdentifier },
    { "PartyNetworkGetChatControls", HandlePartyNetworkGetChatControls },
    { "PartyNetworkGetCustomContext", HandlePartyNetworkGetCustomContext },
    { "PartyNetworkGetDeviceConnectionType", HandlePartyNetworkGetDeviceConnectionType },
    { "PartyNetworkGetDevices", HandlePartyNetworkGetDevices },
    { "PartyNetworkGetEndpoints", HandlePartyNetworkGetEndpoints },
    { "PartyNetworkGetEndpointsByUserType", HandlePartyNetworkGetEndpointsByUserType },
    { "PartyNetworkGetInvitations", HandlePartyNetworkGetInvitations },
    { "PartyNetworkGetLocalUsers", HandlePartyNetworkGetLocalUsers },
    { "PartyNetworkGetNetworkConfiguration", HandlePartyNetworkGetNetworkConfiguration },
    { "PartyNetworkGetNetworkDescriptor", HandlePartyNetworkGetNetworkDescriptor },
    { "PartyNetworkGetNetworkStatistics", HandlePartyNetworkGetNetworkStatistics },
    { "PartyNetworkGetSharedProperty", HandlePartyNetworkGetSharedProperty },
    { "PartyNetworkGetSharedPropertyKeys", HandlePartyNetworkGetSharedPropertyKeys },
    { "PartyNetworkKickDevice", HandlePartyNetworkKickDevice },
    { "PartyNetworkKickUser", HandlePartyNetworkKickUser },
    { "PartyNetworkLeaveNetwork", HandlePartyNetworkLeaveNetwork },
    { "PartyNetworkRemoveLocalUser", HandlePartyNetworkRemoveLocalUser },
    { "PartyNetworkRevokeInvitation", HandlePartyNetworkRevokeInvitation },
    { "PartyNetworkSetCustomContext", HandlePartyNetworkSetCustomContext },
    { "PartyNetworkSetSharedProperties", HandlePartyNetworkSetSharedProperties },
    { "PartySerializeNetworkDescriptor", HandlePartySerializeNetworkDescriptor },
    { "PartySetMemoryCallbacks", HandlePartySetMemoryCallbacks },
    { "PartySetOption", HandlePartySetOption },
    { "PartySetProfilingCallbacksForMethodEntryExit", HandlePartySetProfilingCallbacksForMethodEntryExit },
    { "PartySetThreadAffinityMask", HandlePartySetThreadAffinityMask },
    { "PartySetWorkMode", HandlePartySetWorkMode },
    { "PartyStartProcessingStateChanges", HandlePartyStartProcessingStateChanges },
    { "PartySynchronizeMessagesBetweenEndpoints", HandlePartySynchronizeMessagesBetweenEndpoints },
    { "PartyTextToSpeechProfileGetCustomContext", HandlePartyTextToSpeechProfileGetCustomContext },
    { "PartyTextToSpeechProfileGetGender", HandlePartyTextToSpeechProfileGetGender },
    { "PartyTextToSpeechProfileGetIdentifier", HandlePartyTextToSpeechProfileGetIdentifier },
    { "PartyTextToSpeechProfileGetLanguageCode", HandlePartyTextToSpeechProfileGetLanguageCode },
    { "PartyTextToSpeechProfileGetName", HandlePartyTextToSpeechProfileGetName },
    { "PartyTextToSpeechProfileSetCustomContext", HandlePartyTextToSpeechProfileSetCustomContext },
    { "PartyXblChatUserGetCustomContext", HandlePartyXblChatUserGetCustomContext },
    { "PartyXblChatUserGetXboxUserId", HandlePartyXblChatUserGetXboxUserId },
    { "PartyXblChatUserIsLocal", HandlePartyXblChatUserIsLocal },
    { "PartyXblChatUserSetCustomContext", HandlePartyXblChatUserSetCustomContext },
    { "PartyXblCleanup", HandlePartyXblCleanup },
    { "PartyXblCompleteGetTokenAndSignatureRequest", HandlePartyXblCompleteGetTokenAndSignatureRequest },
    { "PartyXblCreateLocalChatUser", HandlePartyXblCreateLocalChatUser },
    { "PartyXblCreateRemoteChatUser", HandlePartyXblCreateRemoteChatUser },
    { "PartyXblDestroyChatUser", HandlePartyXblDestroyChatUser },
    { "PartyXblFinishProcessingStateChanges", HandlePartyXblFinishProcessingStateChanges },
    { "PartyXblGetChatUsers", HandlePartyXblGetChatUsers },
    { "PartyXblGetEntityIdsFromXboxLiveUserIds", HandlePartyXblGetEntityIdsFromXboxLiveUserIds },
    { "PartyXblGetErrorMessage", HandlePartyXblGetErrorMessage },
    { "PartyXblGetMemoryCallbacks", HandlePartyXblGetMemoryCallbacks },
    { "PartyXblGetProfilingCallbacksForMethodEntryExit", HandlePartyXblGetProfilingCallbacksForMethodEntryExit },
    { "PartyXblGetThreadAffinityMask", HandlePartyXblGetThreadAffinityMask },
    { "PartyXblInitialize", HandlePartyXblInitialize },
    { "PartyXblLocalChatUserGetAccessibilitySettings", HandlePartyXblLocalChatUserGetAccessibilitySettings },
    { "PartyXblLocalChatUserGetCrossNetworkCommunicationPrivacySetting", HandlePartyXblLocalChatUserGetCrossNetworkCommunicationPrivacySetting },
    { "PartyXblLocalChatUserGetRequiredChatPermissionInfo", HandlePartyXblLocalChatUserGetRequiredChatPermissionInfo },
    { "PartyXblLoginToPlayFab", HandlePartyXblLoginToPlayFab },
    { "PartyXblSetMemoryCallbacks", HandlePartyXblSetMemoryCallbacks },
    { "PartyXblSetProfilingCallbacksForMethodEntryExit", HandlePartyXblSetProfilingCallbacksForMethodEntryExit },
    { "PartyXblSetThreadAffinityMask", HandlePartyXblSetThreadAffinityMask },
    { "PartyXblStartProcessingStateChanges", HandlePartyXblStartProcessingStateChanges }
});

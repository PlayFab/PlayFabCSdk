#include "pch.h"

#include "GameChat2Handlers.h"
#include <GameChat2_c.h>

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

CommandResultPayload HandleChatManagerSetMemFunctions(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = ChatManagerSetMemFunctions(nullptr, nullptr);
            LogToWindowFormat("ChatManagerSetMemFunctions (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerSetDebugMemoryMode(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = ChatManagerSetDebugMemoryMode(GC_DEBUG_MEMORY_MODE_NONE);
            LogToWindowFormat("ChatManagerSetDebugMemoryMode (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerGetDebugMemoryMode(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            GC_DEBUG_MEMORY_MODE mode{};
            HRESULT hr = ChatManagerGetDebugMemoryMode(&mode);
            if (SUCCEEDED(hr)) { payload.result = static_cast<int>(mode); }
            LogToWindowFormat("ChatManagerGetDebugMemoryMode (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerGetMemFunctions(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            GC_MEM_ALLOC_FUNC allocFunc{};
            GC_MEM_FREE_FUNC freeFunc{};
            HRESULT hr = ChatManagerGetMemFunctions(&allocFunc, &freeFunc);
            LogToWindowFormat("ChatManagerGetMemFunctions (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerSetThreadProcessor(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = ChatManagerSetThreadProcessor(GC_THREAD_ID_AUDIO, GC_ANY_PROCESSOR_NUMBER);
            LogToWindowFormat("ChatManagerSetThreadProcessor (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerGetThreadProcessor(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t processorNumber{};
            HRESULT hr = ChatManagerGetThreadProcessor(GC_THREAD_ID_AUDIO, &processorNumber);
            if (SUCCEEDED(hr)) { payload.result = processorNumber; }
            LogToWindowFormat("ChatManagerGetThreadProcessor (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerSetThreadAffinityMask(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = ChatManagerSetThreadAffinityMask(GC_THREAD_ID_AUDIO, GC_ANY_PROCESSOR);
            LogToWindowFormat("ChatManagerSetThreadAffinityMask (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerGetThreadAffinityMask(
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
            HRESULT hr = ChatManagerGetThreadAffinityMask(GC_THREAD_ID_AUDIO, &affinityMask);
            if (SUCCEEDED(hr)) { payload.result = affinityMask; }
            LogToWindowFormat("ChatManagerGetThreadAffinityMask (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerSetLegacyUwpEraCompatModeEnabled(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = ChatManagerSetLegacyUwpEraCompatModeEnabled(false);
            LogToWindowFormat("ChatManagerSetLegacyUwpEraCompatModeEnabled (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerInitialize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = ChatManagerInitialize(
                4,
                1.0f,
                GC_COMMUNICATION_RELATIONSHIP_NONE,
                GC_SHARED_DEVICE_COMMUNICATION_RELATIONSHIP_RESOLUTION_MODE_RESTRICTIVE,
                GC_SPEECH_TO_TEXT_CONVERSION_MODE_AUTOMATIC,
                GC_AUDIO_MANIPULATION_MODE_NONE);
            LogToWindowFormat("ChatManagerInitialize (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerCleanup(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = ChatManagerCleanup();
            LogToWindowFormat("ChatManagerCleanup (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerSetAudioEncodingBitrate(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = ChatManagerSetAudioEncodingBitrate(GC_AUDIO_ENCODING_BITRATE_KILOBITS_PER_SECOND_24);
            LogToWindowFormat("ChatManagerSetAudioEncodingBitrate (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerGetAudioEncodingBitrate(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            GC_AUDIO_ENCODING_BITRATE bitrate{};
            HRESULT hr = ChatManagerGetAudioEncodingBitrate(&bitrate);
            if (SUCCEEDED(hr)) { payload.result = static_cast<int>(bitrate); }
            LogToWindowFormat("ChatManagerGetAudioEncodingBitrate (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerStartProcessingDataFrames(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t dataFrameCount{};
            const GC_DATA_FRAME* const* dataFrames{};
            HRESULT hr = ChatManagerStartProcessingDataFrames(&dataFrameCount, &dataFrames);
            if (SUCCEEDED(hr)) { payload.result = dataFrameCount; }
            LogToWindowFormat("ChatManagerStartProcessingDataFrames (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerFinishProcessingDataFrames(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = ChatManagerFinishProcessingDataFrames(nullptr);
            LogToWindowFormat("ChatManagerFinishProcessingDataFrames (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerStartProcessingStateChanges(
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
            const GC_STATE_CHANGE* const* stateChanges{};
            HRESULT hr = ChatManagerStartProcessingStateChanges(&stateChangeCount, &stateChanges);
            if (SUCCEEDED(hr)) { payload.result = stateChangeCount; }
            LogToWindowFormat("ChatManagerStartProcessingStateChanges (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerFinishProcessingStateChanges(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = ChatManagerFinishProcessingStateChanges(nullptr);
            LogToWindowFormat("ChatManagerFinishProcessingStateChanges (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerAddLocalUser(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            CHAT_USER_HANDLE chatUser{};
            HRESULT hr = ChatManagerAddLocalUser(L"TestUser", &chatUser);
            LogToWindowFormat("ChatManagerAddLocalUser (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerAddRemoteUser(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            CHAT_USER_HANDLE chatUser{};
            HRESULT hr = ChatManagerAddRemoteUser(L"RemoteUser", 1, &chatUser);
            LogToWindowFormat("ChatManagerAddRemoteUser (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerRemoveUser(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = ChatManagerRemoveUser(nullptr);
            LogToWindowFormat("ChatManagerRemoveUser (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatUserGetXboxUserId(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PCWSTR xboxUserId{};
            HRESULT hr = ChatUserGetXboxUserId(nullptr, &xboxUserId);
            LogToWindowFormat("ChatUserGetXboxUserId (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatUserGetIsLocal(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            bool isLocal{};
            HRESULT hr = ChatUserGetIsLocal(nullptr, &isLocal);
            if (SUCCEEDED(hr)) { payload.result = isLocal; }
            LogToWindowFormat("ChatUserGetIsLocal (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatUserSetCustomUserContext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = ChatUserSetCustomUserContext(nullptr, nullptr);
            LogToWindowFormat("ChatUserSetCustomUserContext (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatUserGetCustomUserContext(
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
            HRESULT hr = ChatUserGetCustomUserContext(nullptr, &customContext);
            LogToWindowFormat("ChatUserGetCustomUserContext (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatUserSetCommunicationRelationship(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = ChatUserSetCommunicationRelationship(nullptr, nullptr, GC_COMMUNICATION_RELATIONSHIP_NONE);
            LogToWindowFormat("ChatUserSetCommunicationRelationship (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatUserGetEffectiveCommunicationRelationship(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            GC_COMMUNICATION_RELATIONSHIP relationship{};
            GC_COMMUNICATION_RELATIONSHIP_ADJUSTER adjuster{};
            HRESULT hr = ChatUserGetEffectiveCommunicationRelationship(nullptr, nullptr, &relationship, &adjuster);
            LogToWindowFormat("ChatUserGetEffectiveCommunicationRelationship (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatUserGetAudioRenderVolume(
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
            HRESULT hr = ChatUserGetAudioRenderVolume(nullptr, nullptr, &volume);
            if (SUCCEEDED(hr)) { payload.result = volume; }
            LogToWindowFormat("ChatUserGetAudioRenderVolume (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatUserSetAudioRenderVolume(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = ChatUserSetAudioRenderVolume(nullptr, nullptr, 1.0f);
            LogToWindowFormat("ChatUserSetAudioRenderVolume (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatUserGetMicrophoneMuted(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            bool muted{};
            HRESULT hr = ChatUserGetMicrophoneMuted(nullptr, &muted);
            if (SUCCEEDED(hr)) { payload.result = muted; }
            LogToWindowFormat("ChatUserGetMicrophoneMuted (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatUserSetMicrophoneMuted(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = ChatUserSetMicrophoneMuted(nullptr, false);
            LogToWindowFormat("ChatUserSetMicrophoneMuted (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatUserGetRemoteUserMuted(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            bool muted{};
            HRESULT hr = ChatUserGetRemoteUserMuted(nullptr, nullptr, &muted);
            if (SUCCEEDED(hr)) { payload.result = muted; }
            LogToWindowFormat("ChatUserGetRemoteUserMuted (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatUserSetRemoteUserMuted(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = ChatUserSetRemoteUserMuted(nullptr, nullptr, false);
            LogToWindowFormat("ChatUserSetRemoteUserMuted (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatUserSendChatText(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = ChatUserSendChatText(nullptr, L"test");
            LogToWindowFormat("ChatUserSendChatText (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatUserSynthesizeTextToSpeech(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = ChatUserSynthesizeTextToSpeech(nullptr, L"test");
            LogToWindowFormat("ChatUserSynthesizeTextToSpeech (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatUserGetSpeechToTextConversionPreferenceEnabled(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            bool enabled{};
            HRESULT hr = ChatUserGetSpeechToTextConversionPreferenceEnabled(nullptr, &enabled);
            if (SUCCEEDED(hr)) { payload.result = enabled; }
            LogToWindowFormat("ChatUserGetSpeechToTextConversionPreferenceEnabled (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatUserGetTextToSpeechConversionPreferenceEnabled(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            bool enabled{};
            HRESULT hr = ChatUserGetTextToSpeechConversionPreferenceEnabled(nullptr, &enabled);
            if (SUCCEEDED(hr)) { payload.result = enabled; }
            LogToWindowFormat("ChatUserGetTextToSpeechConversionPreferenceEnabled (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatUserGetChatIndicator(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            GC_CHAT_USER_CHAT_INDICATOR indicator{};
            HRESULT hr = ChatUserGetChatIndicator(nullptr, &indicator);
            if (SUCCEEDED(hr)) { payload.result = static_cast<int>(indicator); }
            LogToWindowFormat("ChatUserGetChatIndicator (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerProcessIncomingPacket(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            uint8_t buffer[1]{};
            HRESULT hr = ChatManagerProcessIncomingPacket(0, sizeof(buffer), buffer);
            LogToWindowFormat("ChatManagerProcessIncomingPacket (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerGetAudioManipulationMode(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            GC_AUDIO_MANIPULATION_MODE mode{};
            HRESULT hr = ChatManagerGetAudioManipulationMode(&mode);
            if (SUCCEEDED(hr)) { payload.result = static_cast<int>(mode); }
            LogToWindowFormat("ChatManagerGetAudioManipulationMode (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerGetChatUsers(
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
            const CHAT_USER_HANDLE* chatUsers{};
            HRESULT hr = ChatManagerGetChatUsers(&chatUserCount, &chatUsers);
            if (SUCCEEDED(hr)) { payload.result = chatUserCount; }
            LogToWindowFormat("ChatManagerGetChatUsers (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerGetPreEncodeAudioStreams(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t streamCount{};
            PRE_ENCODE_AUDIO_STREAM_ARRAY streams{};
            HRESULT hr = ChatManagerGetPreEncodeAudioStreams(&streamCount, &streams);
            if (SUCCEEDED(hr)) { payload.result = streamCount; }
            LogToWindowFormat("ChatManagerGetPreEncodeAudioStreams (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerGetPostDecodeAudioSourceStreams(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t streamCount{};
            POST_DECODE_AUDIO_SOURCE_STREAM_ARRAY streams{};
            HRESULT hr = ChatManagerGetPostDecodeAudioSourceStreams(&streamCount, &streams);
            if (SUCCEEDED(hr)) { payload.result = streamCount; }
            LogToWindowFormat("ChatManagerGetPostDecodeAudioSourceStreams (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerGetPostDecodeAudioSinkStreams(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t streamCount{};
            POST_DECODE_AUDIO_SINK_STREAM_ARRAY streams{};
            HRESULT hr = ChatManagerGetPostDecodeAudioSinkStreams(&streamCount, &streams);
            if (SUCCEEDED(hr)) { payload.result = streamCount; }
            LogToWindowFormat("ChatManagerGetPostDecodeAudioSinkStreams (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerStartProcessingStreamStateChanges(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t changeCount{};
            GC_STREAM_STATE_CHANGE_ARRAY changes{};
            HRESULT hr = ChatManagerStartProcessingStreamStateChanges(&changeCount, &changes);
            if (SUCCEEDED(hr)) { payload.result = changeCount; }
            LogToWindowFormat("ChatManagerStartProcessingStreamStateChanges (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleChatManagerFinishProcessingStreamStateChanges(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = ChatManagerFinishProcessingStreamStateChanges(nullptr);
            LogToWindowFormat("ChatManagerFinishProcessingStreamStateChanges (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePreEncodeAudioStreamGetUsers(
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
            CHAT_USER_ARRAY chatUsers{};
            HRESULT hr = PreEncodeAudioStreamGetUsers(nullptr, &chatUserCount, &chatUsers);
            if (SUCCEEDED(hr)) { payload.result = chatUserCount; }
            LogToWindowFormat("PreEncodeAudioStreamGetUsers (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePreEncodeAudioStreamGetPreprocessedFormat(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            GC_AUDIO_FORMAT format{};
            HRESULT hr = PreEncodeAudioStreamGetPreprocessedFormat(nullptr, &format);
            LogToWindowFormat("PreEncodeAudioStreamGetPreprocessedFormat (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePreEncodeAudioStreamSetProcessedFormat(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            GC_AUDIO_FORMAT format{};
            HRESULT hr = PreEncodeAudioStreamSetProcessedFormat(nullptr, format);
            LogToWindowFormat("PreEncodeAudioStreamSetProcessedFormat (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePreEncodeAudioStreamGetAvailableBufferCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t bufferCount{};
            HRESULT hr = PreEncodeAudioStreamGetAvailableBufferCount(nullptr, &bufferCount);
            if (SUCCEEDED(hr)) { payload.result = bufferCount; }
            LogToWindowFormat("PreEncodeAudioStreamGetAvailableBufferCount (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePreEncodeAudioStreamGetNextBuffer(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t byteCount{};
            void* buffer{};
            HRESULT hr = PreEncodeAudioStreamGetNextBuffer(nullptr, &byteCount, &buffer);
            if (SUCCEEDED(hr)) { payload.result = byteCount; }
            LogToWindowFormat("PreEncodeAudioStreamGetNextBuffer (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePreEncodeAudioStreamReturnBuffer(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PreEncodeAudioStreamReturnBuffer(nullptr, nullptr);
            LogToWindowFormat("PreEncodeAudioStreamReturnBuffer (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePreEncodeAudioStreamSubmitBuffer(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PreEncodeAudioStreamSubmitBuffer(nullptr, 0, nullptr);
            LogToWindowFormat("PreEncodeAudioStreamSubmitBuffer (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePreEncodeAudioStreamSetCustomStreamContext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PreEncodeAudioStreamSetCustomStreamContext(nullptr, nullptr);
            LogToWindowFormat("PreEncodeAudioStreamSetCustomStreamContext (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePreEncodeAudioStreamGetCustomStreamContext(
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
            HRESULT hr = PreEncodeAudioStreamGetCustomStreamContext(nullptr, &customContext);
            LogToWindowFormat("PreEncodeAudioStreamGetCustomStreamContext (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePreEncodeAudioStreamIsOpen(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            bool isClosed{};
            HRESULT hr = PreEncodeAudioStreamIsOpen(nullptr, &isClosed);
            if (SUCCEEDED(hr)) { payload.result = isClosed; }
            LogToWindowFormat("PreEncodeAudioStreamIsOpen (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePostDecodeAudioSourceStreamGetUsers(
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
            CHAT_USER_ARRAY chatUsers{};
            HRESULT hr = PostDecodeAudioSourceStreamGetUsers(nullptr, &chatUserCount, &chatUsers);
            if (SUCCEEDED(hr)) { payload.result = chatUserCount; }
            LogToWindowFormat("PostDecodeAudioSourceStreamGetUsers (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePostDecodeAudioSourceStreamGetPreprocessedFormat(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            GC_AUDIO_FORMAT format{};
            HRESULT hr = PostDecodeAudioSourceStreamGetPreprocessedFormat(nullptr, &format);
            LogToWindowFormat("PostDecodeAudioSourceStreamGetPreprocessedFormat (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePostDecodeAudioSourceStreamGetAvailableBufferCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t bufferCount{};
            HRESULT hr = PostDecodeAudioSourceStreamGetAvailableBufferCount(nullptr, &bufferCount);
            if (SUCCEEDED(hr)) { payload.result = bufferCount; }
            LogToWindowFormat("PostDecodeAudioSourceStreamGetAvailableBufferCount (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePostDecodeAudioSourceStreamGetNextBuffer(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t byteCount{};
            void* buffer{};
            HRESULT hr = PostDecodeAudioSourceStreamGetNextBuffer(nullptr, &byteCount, &buffer);
            if (SUCCEEDED(hr)) { payload.result = byteCount; }
            LogToWindowFormat("PostDecodeAudioSourceStreamGetNextBuffer (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePostDecodeAudioSourceStreamReturnBuffer(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PostDecodeAudioSourceStreamReturnBuffer(nullptr, nullptr);
            LogToWindowFormat("PostDecodeAudioSourceStreamReturnBuffer (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePostDecodeAudioSourceStreamSetCustomStreamContext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PostDecodeAudioSourceStreamSetCustomStreamContext(nullptr, nullptr);
            LogToWindowFormat("PostDecodeAudioSourceStreamSetCustomStreamContext (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePostDecodeAudioSourceStreamGetCustomStreamContext(
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
            HRESULT hr = PostDecodeAudioSourceStreamGetCustomStreamContext(nullptr, &customContext);
            LogToWindowFormat("PostDecodeAudioSourceStreamGetCustomStreamContext (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePostDecodeAudioSourceStreamIsOpen(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            bool isClosed{};
            HRESULT hr = PostDecodeAudioSourceStreamIsOpen(nullptr, &isClosed);
            if (SUCCEEDED(hr)) { payload.result = isClosed; }
            LogToWindowFormat("PostDecodeAudioSourceStreamIsOpen (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePostDecodeAudioSinkStreamGetUsers(
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
            CHAT_USER_ARRAY chatUsers{};
            HRESULT hr = PostDecodeAudioSinkStreamGetUsers(nullptr, &chatUserCount, &chatUsers);
            if (SUCCEEDED(hr)) { payload.result = chatUserCount; }
            LogToWindowFormat("PostDecodeAudioSinkStreamGetUsers (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePostDecodeAudioSinkStreamSetProcessedFormat(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            GC_AUDIO_FORMAT format{};
            HRESULT hr = PostDecodeAudioSinkStreamSetProcessedFormat(nullptr, format);
            LogToWindowFormat("PostDecodeAudioSinkStreamSetProcessedFormat (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePostDecodeAudioSinkStreamSubmitMixedBuffer(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PostDecodeAudioSinkStreamSubmitMixedBuffer(nullptr, 0, nullptr);
            LogToWindowFormat("PostDecodeAudioSinkStreamSubmitMixedBuffer (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePostDecodeAudioSinkStreamCanReceiveAudioFromSourceStream(
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
            bool canReceiveAudio{};
            HRESULT hr = PostDecodeAudioSinkStreamCanReceiveAudioFromSourceStream(nullptr, nullptr, &volume, &canReceiveAudio);
            if (SUCCEEDED(hr)) { payload.result = canReceiveAudio; }
            LogToWindowFormat("PostDecodeAudioSinkStreamCanReceiveAudioFromSourceStream (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePostDecodeAudioSinkStreamGetDeviceId(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            PCWSTR deviceIdStr{};
            HRESULT hr = PostDecodeAudioSinkStreamGetDeviceId(nullptr, &deviceIdStr);
            LogToWindowFormat("PostDecodeAudioSinkStreamGetDeviceId (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePostDecodeAudioSinkStreamSetCustomStreamContext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = PostDecodeAudioSinkStreamSetCustomStreamContext(nullptr, nullptr);
            LogToWindowFormat("PostDecodeAudioSinkStreamSetCustomStreamContext (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePostDecodeAudioSinkStreamGetCustomStreamContext(
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
            HRESULT hr = PostDecodeAudioSinkStreamGetCustomStreamContext(nullptr, &customContext);
            LogToWindowFormat("PostDecodeAudioSinkStreamGetCustomStreamContext (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandlePostDecodeAudioSinkStreamIsOpen(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            bool isClosed{};
            HRESULT hr = PostDecodeAudioSinkStreamIsOpen(nullptr, &isClosed);
            if (SUCCEEDED(hr)) { payload.result = isClosed; }
            LogToWindowFormat("PostDecodeAudioSinkStreamIsOpen (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleGameChatFailFastWithInform(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindowFormat("GameChatFailFastWithInform: skipped (__declspec(noreturn))");
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "ChatManagerAddLocalUser", HandleChatManagerAddLocalUser },
    { "ChatManagerAddRemoteUser", HandleChatManagerAddRemoteUser },
    { "ChatManagerCleanup", HandleChatManagerCleanup },
    { "ChatManagerFinishProcessingDataFrames", HandleChatManagerFinishProcessingDataFrames },
    { "ChatManagerFinishProcessingStateChanges", HandleChatManagerFinishProcessingStateChanges },
    { "ChatManagerFinishProcessingStreamStateChanges", HandleChatManagerFinishProcessingStreamStateChanges },
    { "ChatManagerGetAudioEncodingBitrate", HandleChatManagerGetAudioEncodingBitrate },
    { "ChatManagerGetAudioManipulationMode", HandleChatManagerGetAudioManipulationMode },
    { "ChatManagerGetChatUsers", HandleChatManagerGetChatUsers },
    { "ChatManagerGetDebugMemoryMode", HandleChatManagerGetDebugMemoryMode },
    { "ChatManagerGetMemFunctions", HandleChatManagerGetMemFunctions },
    { "ChatManagerGetPostDecodeAudioSinkStreams", HandleChatManagerGetPostDecodeAudioSinkStreams },
    { "ChatManagerGetPostDecodeAudioSourceStreams", HandleChatManagerGetPostDecodeAudioSourceStreams },
    { "ChatManagerGetPreEncodeAudioStreams", HandleChatManagerGetPreEncodeAudioStreams },
    { "ChatManagerGetThreadAffinityMask", HandleChatManagerGetThreadAffinityMask },
    { "ChatManagerGetThreadProcessor", HandleChatManagerGetThreadProcessor },
    { "ChatManagerInitialize", HandleChatManagerInitialize },
    { "ChatManagerProcessIncomingPacket", HandleChatManagerProcessIncomingPacket },
    { "ChatManagerRemoveUser", HandleChatManagerRemoveUser },
    { "ChatManagerSetAudioEncodingBitrate", HandleChatManagerSetAudioEncodingBitrate },
    { "ChatManagerSetDebugMemoryMode", HandleChatManagerSetDebugMemoryMode },
    { "ChatManagerSetLegacyUwpEraCompatModeEnabled", HandleChatManagerSetLegacyUwpEraCompatModeEnabled },
    { "ChatManagerSetMemFunctions", HandleChatManagerSetMemFunctions },
    { "ChatManagerSetThreadAffinityMask", HandleChatManagerSetThreadAffinityMask },
    { "ChatManagerSetThreadProcessor", HandleChatManagerSetThreadProcessor },
    { "ChatManagerStartProcessingDataFrames", HandleChatManagerStartProcessingDataFrames },
    { "ChatManagerStartProcessingStateChanges", HandleChatManagerStartProcessingStateChanges },
    { "ChatManagerStartProcessingStreamStateChanges", HandleChatManagerStartProcessingStreamStateChanges },
    { "ChatUserGetAudioRenderVolume", HandleChatUserGetAudioRenderVolume },
    { "ChatUserGetChatIndicator", HandleChatUserGetChatIndicator },
    { "ChatUserGetCustomUserContext", HandleChatUserGetCustomUserContext },
    { "ChatUserGetEffectiveCommunicationRelationship", HandleChatUserGetEffectiveCommunicationRelationship },
    { "ChatUserGetIsLocal", HandleChatUserGetIsLocal },
    { "ChatUserGetMicrophoneMuted", HandleChatUserGetMicrophoneMuted },
    { "ChatUserGetRemoteUserMuted", HandleChatUserGetRemoteUserMuted },
    { "ChatUserGetSpeechToTextConversionPreferenceEnabled", HandleChatUserGetSpeechToTextConversionPreferenceEnabled },
    { "ChatUserGetTextToSpeechConversionPreferenceEnabled", HandleChatUserGetTextToSpeechConversionPreferenceEnabled },
    { "ChatUserGetXboxUserId", HandleChatUserGetXboxUserId },
    { "ChatUserSendChatText", HandleChatUserSendChatText },
    { "ChatUserSetAudioRenderVolume", HandleChatUserSetAudioRenderVolume },
    { "ChatUserSetCommunicationRelationship", HandleChatUserSetCommunicationRelationship },
    { "ChatUserSetCustomUserContext", HandleChatUserSetCustomUserContext },
    { "ChatUserSetMicrophoneMuted", HandleChatUserSetMicrophoneMuted },
    { "ChatUserSetRemoteUserMuted", HandleChatUserSetRemoteUserMuted },
    { "ChatUserSynthesizeTextToSpeech", HandleChatUserSynthesizeTextToSpeech },
    { "GameChatFailFastWithInform", HandleGameChatFailFastWithInform },
    { "PostDecodeAudioSinkStreamCanReceiveAudioFromSourceStream", HandlePostDecodeAudioSinkStreamCanReceiveAudioFromSourceStream },
    { "PostDecodeAudioSinkStreamGetCustomStreamContext", HandlePostDecodeAudioSinkStreamGetCustomStreamContext },
    { "PostDecodeAudioSinkStreamGetDeviceId", HandlePostDecodeAudioSinkStreamGetDeviceId },
    { "PostDecodeAudioSinkStreamGetUsers", HandlePostDecodeAudioSinkStreamGetUsers },
    { "PostDecodeAudioSinkStreamIsOpen", HandlePostDecodeAudioSinkStreamIsOpen },
    { "PostDecodeAudioSinkStreamSetCustomStreamContext", HandlePostDecodeAudioSinkStreamSetCustomStreamContext },
    { "PostDecodeAudioSinkStreamSetProcessedFormat", HandlePostDecodeAudioSinkStreamSetProcessedFormat },
    { "PostDecodeAudioSinkStreamSubmitMixedBuffer", HandlePostDecodeAudioSinkStreamSubmitMixedBuffer },
    { "PostDecodeAudioSourceStreamGetAvailableBufferCount", HandlePostDecodeAudioSourceStreamGetAvailableBufferCount },
    { "PostDecodeAudioSourceStreamGetCustomStreamContext", HandlePostDecodeAudioSourceStreamGetCustomStreamContext },
    { "PostDecodeAudioSourceStreamGetNextBuffer", HandlePostDecodeAudioSourceStreamGetNextBuffer },
    { "PostDecodeAudioSourceStreamGetPreprocessedFormat", HandlePostDecodeAudioSourceStreamGetPreprocessedFormat },
    { "PostDecodeAudioSourceStreamGetUsers", HandlePostDecodeAudioSourceStreamGetUsers },
    { "PostDecodeAudioSourceStreamIsOpen", HandlePostDecodeAudioSourceStreamIsOpen },
    { "PostDecodeAudioSourceStreamReturnBuffer", HandlePostDecodeAudioSourceStreamReturnBuffer },
    { "PostDecodeAudioSourceStreamSetCustomStreamContext", HandlePostDecodeAudioSourceStreamSetCustomStreamContext },
    { "PreEncodeAudioStreamGetAvailableBufferCount", HandlePreEncodeAudioStreamGetAvailableBufferCount },
    { "PreEncodeAudioStreamGetCustomStreamContext", HandlePreEncodeAudioStreamGetCustomStreamContext },
    { "PreEncodeAudioStreamGetNextBuffer", HandlePreEncodeAudioStreamGetNextBuffer },
    { "PreEncodeAudioStreamGetPreprocessedFormat", HandlePreEncodeAudioStreamGetPreprocessedFormat },
    { "PreEncodeAudioStreamGetUsers", HandlePreEncodeAudioStreamGetUsers },
    { "PreEncodeAudioStreamIsOpen", HandlePreEncodeAudioStreamIsOpen },
    { "PreEncodeAudioStreamReturnBuffer", HandlePreEncodeAudioStreamReturnBuffer },
    { "PreEncodeAudioStreamSetCustomStreamContext", HandlePreEncodeAudioStreamSetCustomStreamContext },
    { "PreEncodeAudioStreamSetProcessedFormat", HandlePreEncodeAudioStreamSetProcessedFormat },
    { "PreEncodeAudioStreamSubmitBuffer", HandlePreEncodeAudioStreamSubmitBuffer }
});

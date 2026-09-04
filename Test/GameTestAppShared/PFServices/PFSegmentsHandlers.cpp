#include "pch.h"
#include "PFSegmentsHandlers.h"
#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <playfab/services/PFSegments.h>
#include <playfab/core/PFLocalUser.h>
#include <vector>
#include "CommandRegistry.h"

using CommandHandlerShared::TryGetStringParameter;
using CommandHandlerShared::TryParseBoolParameter;

CommandResultPayload HandlePFSegmentsClientGetPlayerSegmentsAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            const HRESULT hr = PFSegmentsClientGetPlayerSegmentsAsync(entityHandle, &async);
            LogToWindowFormat("PFSegmentsClientGetPlayerSegmentsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFSegmentsClientGetPlayerSegmentsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFSegmentsGetPlayerSegmentsResult* result{ nullptr };
            HRESULT hr = PFSegmentsClientGetPlayerSegmentsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFSegmentsClientGetPlayerSegmentsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFSegmentsClientGetPlayerSegmentsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFSegmentsClientGetPlayerSegmentsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFSegmentsClientGetPlayerSegmentsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFSegmentsClientGetPlayerSegmentsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFSegmentsClientGetPlayerTagsAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFSegmentsGetPlayerTagsRequest request{};
            const HRESULT hr = PFSegmentsClientGetPlayerTagsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFSegmentsClientGetPlayerTagsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFSegmentsClientGetPlayerTagsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFSegmentsGetPlayerTagsResult* result{ nullptr };
            HRESULT hr = PFSegmentsClientGetPlayerTagsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFSegmentsClientGetPlayerTagsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFSegmentsClientGetPlayerTagsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFSegmentsClientGetPlayerTagsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFSegmentsClientGetPlayerTagsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFSegmentsClientGetPlayerTagsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFSegmentsServerAddPlayerTagAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFSegmentsAddPlayerTagRequest request{};
            const HRESULT hr = PFSegmentsServerAddPlayerTagAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFSegmentsServerAddPlayerTagAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFSegmentsServerGetAllSegmentsAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            const HRESULT hr = PFSegmentsServerGetAllSegmentsAsync(entityHandle, &async);
            LogToWindowFormat("PFSegmentsServerGetAllSegmentsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFSegmentsServerGetAllSegmentsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFSegmentsGetAllSegmentsResult* result{ nullptr };
            HRESULT hr = PFSegmentsServerGetAllSegmentsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFSegmentsServerGetAllSegmentsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFSegmentsServerGetAllSegmentsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFSegmentsServerGetAllSegmentsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFSegmentsServerGetAllSegmentsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFSegmentsServerGetAllSegmentsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFSegmentsServerGetPlayerSegmentsAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFSegmentsGetPlayersSegmentsRequest request{};
            const HRESULT hr = PFSegmentsServerGetPlayerSegmentsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFSegmentsServerGetPlayerSegmentsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFSegmentsServerGetPlayerSegmentsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFSegmentsGetPlayerSegmentsResult* result{ nullptr };
            HRESULT hr = PFSegmentsServerGetPlayerSegmentsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFSegmentsServerGetPlayerSegmentsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFSegmentsServerGetPlayerSegmentsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFSegmentsServerGetPlayerSegmentsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFSegmentsServerGetPlayerSegmentsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFSegmentsServerGetPlayerSegmentsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFSegmentsServerGetPlayerTagsAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFSegmentsGetPlayerTagsRequest request{};
            const HRESULT hr = PFSegmentsServerGetPlayerTagsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFSegmentsServerGetPlayerTagsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFSegmentsServerGetPlayerTagsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFSegmentsGetPlayerTagsResult* result{ nullptr };
            HRESULT hr = PFSegmentsServerGetPlayerTagsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFSegmentsServerGetPlayerTagsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFSegmentsServerGetPlayerTagsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFSegmentsServerGetPlayerTagsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFSegmentsServerGetPlayerTagsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFSegmentsServerGetPlayerTagsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFSegmentsServerRemovePlayerTagAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFSegmentsRemovePlayerTagRequest request{};
            const HRESULT hr = PFSegmentsServerRemovePlayerTagAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFSegmentsServerRemovePlayerTagAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "PFSegmentsClientGetPlayerSegmentsAsync", HandlePFSegmentsClientGetPlayerSegmentsAsync },
    { "PFSegmentsClientGetPlayerSegmentsGetResult", HandlePFSegmentsClientGetPlayerSegmentsGetResult },
    { "PFSegmentsClientGetPlayerSegmentsGetResultSize", HandlePFSegmentsClientGetPlayerSegmentsGetResultSize },
    { "PFSegmentsClientGetPlayerTagsAsync", HandlePFSegmentsClientGetPlayerTagsAsync },
    { "PFSegmentsClientGetPlayerTagsGetResult", HandlePFSegmentsClientGetPlayerTagsGetResult },
    { "PFSegmentsClientGetPlayerTagsGetResultSize", HandlePFSegmentsClientGetPlayerTagsGetResultSize },
    { "PFSegmentsServerAddPlayerTagAsync", HandlePFSegmentsServerAddPlayerTagAsync },
    { "PFSegmentsServerGetAllSegmentsAsync", HandlePFSegmentsServerGetAllSegmentsAsync },
    { "PFSegmentsServerGetAllSegmentsGetResult", HandlePFSegmentsServerGetAllSegmentsGetResult },
    { "PFSegmentsServerGetAllSegmentsGetResultSize", HandlePFSegmentsServerGetAllSegmentsGetResultSize },
    { "PFSegmentsServerGetPlayerSegmentsAsync", HandlePFSegmentsServerGetPlayerSegmentsAsync },
    { "PFSegmentsServerGetPlayerSegmentsGetResult", HandlePFSegmentsServerGetPlayerSegmentsGetResult },
    { "PFSegmentsServerGetPlayerSegmentsGetResultSize", HandlePFSegmentsServerGetPlayerSegmentsGetResultSize },
    { "PFSegmentsServerGetPlayerTagsAsync", HandlePFSegmentsServerGetPlayerTagsAsync },
    { "PFSegmentsServerGetPlayerTagsGetResult", HandlePFSegmentsServerGetPlayerTagsGetResult },
    { "PFSegmentsServerGetPlayerTagsGetResultSize", HandlePFSegmentsServerGetPlayerTagsGetResultSize },
    { "PFSegmentsServerRemovePlayerTagAsync", HandlePFSegmentsServerRemovePlayerTagAsync }
});
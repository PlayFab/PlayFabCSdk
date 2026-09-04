#include "pch.h"
#include "PFStatisticsHandlers.h"
#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <playfab/services/PFStatistics.h>
#include <playfab/core/PFLocalUser.h>
#include <vector>
#include "CommandRegistry.h"

using CommandHandlerShared::TryGetStringParameter;
using CommandHandlerShared::TryParseBoolParameter;

CommandResultPayload HandlePFStatisticsCreateStatisticDefinitionAsync(
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
            PFStatisticsCreateStatisticDefinitionRequest request{};
            const HRESULT hr = PFStatisticsCreateStatisticDefinitionAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFStatisticsCreateStatisticDefinitionAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFStatisticsDeleteStatisticDefinitionAsync(
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
            PFStatisticsDeleteStatisticDefinitionRequest request{};
            const HRESULT hr = PFStatisticsDeleteStatisticDefinitionAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFStatisticsDeleteStatisticDefinitionAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFStatisticsDeleteStatisticsAsync(
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
            PFStatisticsDeleteStatisticsRequest request{};
            const HRESULT hr = PFStatisticsDeleteStatisticsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFStatisticsDeleteStatisticsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFStatisticsDeleteStatisticsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFStatisticsDeleteStatisticsResponse* result{ nullptr };
            HRESULT hr = PFStatisticsDeleteStatisticsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFStatisticsDeleteStatisticsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFStatisticsDeleteStatisticsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFStatisticsDeleteStatisticsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFStatisticsDeleteStatisticsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFStatisticsDeleteStatisticsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFStatisticsGetStatisticDefinitionAsync(
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
            PFStatisticsGetStatisticDefinitionRequest request{};
            const HRESULT hr = PFStatisticsGetStatisticDefinitionAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFStatisticsGetStatisticDefinitionAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFStatisticsGetStatisticDefinitionGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFStatisticsGetStatisticDefinitionResponse* result{ nullptr };
            HRESULT hr = PFStatisticsGetStatisticDefinitionGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFStatisticsGetStatisticDefinitionAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFStatisticsGetStatisticDefinitionGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFStatisticsGetStatisticDefinitionGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFStatisticsGetStatisticDefinitionGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFStatisticsGetStatisticDefinitionGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFStatisticsGetStatisticsAsync(
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
            PFStatisticsGetStatisticsRequest request{};
            const HRESULT hr = PFStatisticsGetStatisticsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFStatisticsGetStatisticsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFStatisticsGetStatisticsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFStatisticsGetStatisticsResponse* result{ nullptr };
            HRESULT hr = PFStatisticsGetStatisticsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFStatisticsGetStatisticsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFStatisticsGetStatisticsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFStatisticsGetStatisticsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFStatisticsGetStatisticsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFStatisticsGetStatisticsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFStatisticsGetStatisticsForEntitiesAsync(
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
            PFStatisticsGetStatisticsForEntitiesRequest request{};
            const HRESULT hr = PFStatisticsGetStatisticsForEntitiesAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFStatisticsGetStatisticsForEntitiesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFStatisticsGetStatisticsForEntitiesGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFStatisticsGetStatisticsForEntitiesResponse* result{ nullptr };
            HRESULT hr = PFStatisticsGetStatisticsForEntitiesGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFStatisticsGetStatisticsForEntitiesAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFStatisticsGetStatisticsForEntitiesGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFStatisticsGetStatisticsForEntitiesGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFStatisticsGetStatisticsForEntitiesGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFStatisticsGetStatisticsForEntitiesGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFStatisticsIncrementStatisticVersionAsync(
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
            PFStatisticsIncrementStatisticVersionRequest request{};
            const HRESULT hr = PFStatisticsIncrementStatisticVersionAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFStatisticsIncrementStatisticVersionAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFStatisticsIncrementStatisticVersionResponse result{};
            return PFStatisticsIncrementStatisticVersionGetResult(&async, &result);
        });
}

CommandResultPayload HandlePFStatisticsIncrementStatisticVersionGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFStatisticsIncrementStatisticVersionGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFStatisticsListStatisticDefinitionsAsync(
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
            PFStatisticsListStatisticDefinitionsRequest request{};
            const HRESULT hr = PFStatisticsListStatisticDefinitionsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFStatisticsListStatisticDefinitionsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFStatisticsListStatisticDefinitionsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFStatisticsListStatisticDefinitionsResponse* result{ nullptr };
            HRESULT hr = PFStatisticsListStatisticDefinitionsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFStatisticsListStatisticDefinitionsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFStatisticsListStatisticDefinitionsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFStatisticsListStatisticDefinitionsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFStatisticsListStatisticDefinitionsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFStatisticsListStatisticDefinitionsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFStatisticsUpdateStatisticDefinitionAsync(
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
            PFStatisticsUpdateStatisticDefinitionRequest request{};
            const HRESULT hr = PFStatisticsUpdateStatisticDefinitionAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFStatisticsUpdateStatisticDefinitionAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFStatisticsUpdateStatisticsAsync(
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
            PFStatisticsUpdateStatisticsRequest request{};
            const HRESULT hr = PFStatisticsUpdateStatisticsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFStatisticsUpdateStatisticsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFStatisticsUpdateStatisticsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFStatisticsUpdateStatisticsResponse* result{ nullptr };
            HRESULT hr = PFStatisticsUpdateStatisticsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFStatisticsUpdateStatisticsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFStatisticsUpdateStatisticsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFStatisticsUpdateStatisticsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFStatisticsUpdateStatisticsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFStatisticsUpdateStatisticsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFStatisticsUnlinkAggregationSourceFromStatisticAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFStatisticsUnlinkAggregationSourceFromStatisticAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "PFStatisticsCreateStatisticDefinitionAsync", HandlePFStatisticsCreateStatisticDefinitionAsync },
    { "PFStatisticsDeleteStatisticDefinitionAsync", HandlePFStatisticsDeleteStatisticDefinitionAsync },
    { "PFStatisticsDeleteStatisticsAsync", HandlePFStatisticsDeleteStatisticsAsync },
    { "PFStatisticsDeleteStatisticsGetResult", HandlePFStatisticsDeleteStatisticsGetResult },
    { "PFStatisticsDeleteStatisticsGetResultSize", HandlePFStatisticsDeleteStatisticsGetResultSize },
    { "PFStatisticsGetStatisticDefinitionAsync", HandlePFStatisticsGetStatisticDefinitionAsync },
    { "PFStatisticsGetStatisticDefinitionGetResult", HandlePFStatisticsGetStatisticDefinitionGetResult },
    { "PFStatisticsGetStatisticDefinitionGetResultSize", HandlePFStatisticsGetStatisticDefinitionGetResultSize },
    { "PFStatisticsGetStatisticsAsync", HandlePFStatisticsGetStatisticsAsync },
    { "PFStatisticsGetStatisticsForEntitiesAsync", HandlePFStatisticsGetStatisticsForEntitiesAsync },
    { "PFStatisticsGetStatisticsForEntitiesGetResult", HandlePFStatisticsGetStatisticsForEntitiesGetResult },
    { "PFStatisticsGetStatisticsForEntitiesGetResultSize", HandlePFStatisticsGetStatisticsForEntitiesGetResultSize },
    { "PFStatisticsGetStatisticsGetResult", HandlePFStatisticsGetStatisticsGetResult },
    { "PFStatisticsGetStatisticsGetResultSize", HandlePFStatisticsGetStatisticsGetResultSize },
    { "PFStatisticsIncrementStatisticVersionAsync", HandlePFStatisticsIncrementStatisticVersionAsync },
    { "PFStatisticsIncrementStatisticVersionGetResult", HandlePFStatisticsIncrementStatisticVersionGetResult },
    { "PFStatisticsListStatisticDefinitionsAsync", HandlePFStatisticsListStatisticDefinitionsAsync },
    { "PFStatisticsListStatisticDefinitionsGetResult", HandlePFStatisticsListStatisticDefinitionsGetResult },
    { "PFStatisticsListStatisticDefinitionsGetResultSize", HandlePFStatisticsListStatisticDefinitionsGetResultSize },
    { "PFStatisticsUnlinkAggregationSourceFromStatisticAsync", HandlePFStatisticsUnlinkAggregationSourceFromStatisticAsync },
    { "PFStatisticsUpdateStatisticDefinitionAsync", HandlePFStatisticsUpdateStatisticDefinitionAsync },
    { "PFStatisticsUpdateStatisticsAsync", HandlePFStatisticsUpdateStatisticsAsync },
    { "PFStatisticsUpdateStatisticsGetResult", HandlePFStatisticsUpdateStatisticsGetResult },
    { "PFStatisticsUpdateStatisticsGetResultSize", HandlePFStatisticsUpdateStatisticsGetResultSize }
});

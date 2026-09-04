#include "pch.h"
#include "PFMultiplayerServerHandlers.h"
#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <playfab/services/PFMultiplayerServer.h>
#include <playfab/core/PFLocalUser.h>
#include <vector>
#include "CommandRegistry.h"

using CommandHandlerShared::TryGetStringParameter;
using CommandHandlerShared::TryParseBoolParameter;

CommandResultPayload HandlePFMultiplayerServerListBuildAliasesAsync(
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
            PFMultiplayerServerListBuildAliasesRequest request{};
            const HRESULT hr = PFMultiplayerServerListBuildAliasesAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFMultiplayerServerListBuildAliasesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFMultiplayerServerListBuildAliasesGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFMultiplayerServerListBuildAliasesResponse* result{ nullptr };
            HRESULT hr = PFMultiplayerServerListBuildAliasesGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFMultiplayerServerListBuildAliasesAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerServerListBuildAliasesGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFMultiplayerServerListBuildAliasesGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFMultiplayerServerListBuildAliasesGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFMultiplayerServerListBuildAliasesGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFMultiplayerServerListBuildSummariesV2Async(
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
            PFMultiplayerServerListBuildSummariesRequest request{};
            const HRESULT hr = PFMultiplayerServerListBuildSummariesV2Async(entityHandle, &request, &async);
            LogToWindowFormat("PFMultiplayerServerListBuildSummariesV2Async (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFMultiplayerServerListBuildSummariesV2GetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFMultiplayerServerListBuildSummariesResponse* result{ nullptr };
            HRESULT hr = PFMultiplayerServerListBuildSummariesV2GetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFMultiplayerServerListBuildSummariesV2Async: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerServerListBuildSummariesV2GetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFMultiplayerServerListBuildSummariesV2GetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFMultiplayerServerListBuildSummariesV2GetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFMultiplayerServerListBuildSummariesV2GetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFMultiplayerServerListQosServersForTitleAsync(
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
            PFMultiplayerServerListQosServersForTitleRequest request{};
            const HRESULT hr = PFMultiplayerServerListQosServersForTitleAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFMultiplayerServerListQosServersForTitleAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFMultiplayerServerListQosServersForTitleGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFMultiplayerServerListQosServersForTitleResponse* result{ nullptr };
            HRESULT hr = PFMultiplayerServerListQosServersForTitleGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFMultiplayerServerListQosServersForTitleAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerServerListQosServersForTitleGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFMultiplayerServerListQosServersForTitleGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFMultiplayerServerListQosServersForTitleGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFMultiplayerServerListQosServersForTitleGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFMultiplayerServerRequestMultiplayerServerAsync(
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
            
            // Setup request with required fields
            PFMultiplayerServerRequestMultiplayerServerRequest request{};
            
            // Set build ID
            request.buildId = "00000000-0000-0000-0000-000000000000";
            
            // Set preferred regions
            const char* regions[] = { "EastUs" };
            request.preferredRegions = regions;
            request.preferredRegionsCount = 1;
            
            // Set session ID
            request.sessionId = "00000000-0000-0000-0000-000000000001";
            
            const HRESULT hr = PFMultiplayerServerRequestMultiplayerServerAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFMultiplayerServerRequestMultiplayerServerAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFMultiplayerServerRequestMultiplayerServerGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFMultiplayerServerRequestMultiplayerServerResponse* result{ nullptr };
            HRESULT hr = PFMultiplayerServerRequestMultiplayerServerGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFMultiplayerServerRequestMultiplayerServerAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFMultiplayerServerRequestMultiplayerServerGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFMultiplayerServerRequestMultiplayerServerGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFMultiplayerServerRequestMultiplayerServerGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFMultiplayerServerRequestMultiplayerServerGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFMultiplayerServerDeleteSecretAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFMultiplayerServerDeleteSecretAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFMultiplayerServerListSecretSummariesAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFMultiplayerServerListSecretSummariesAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFMultiplayerServerListSecretSummariesGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFMultiplayerServerListSecretSummariesGetResultSize: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFMultiplayerServerListSecretSummariesGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFMultiplayerServerListSecretSummariesGetResult: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFMultiplayerServerRequestPartyServiceAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFMultiplayerServerRequestPartyServiceAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFMultiplayerServerRequestPartyServiceGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFMultiplayerServerRequestPartyServiceGetResultSize: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFMultiplayerServerRequestPartyServiceGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFMultiplayerServerRequestPartyServiceGetResult: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFMultiplayerServerUploadSecretAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFMultiplayerServerUploadSecretAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "PFMultiplayerServerDeleteSecretAsync", HandlePFMultiplayerServerDeleteSecretAsync },
    { "PFMultiplayerServerListBuildAliasesAsync", HandlePFMultiplayerServerListBuildAliasesAsync },
    { "PFMultiplayerServerListBuildAliasesGetResult", HandlePFMultiplayerServerListBuildAliasesGetResult },
    { "PFMultiplayerServerListBuildAliasesGetResultSize", HandlePFMultiplayerServerListBuildAliasesGetResultSize },
    { "PFMultiplayerServerListBuildSummariesV2Async", HandlePFMultiplayerServerListBuildSummariesV2Async },
    { "PFMultiplayerServerListBuildSummariesV2GetResult", HandlePFMultiplayerServerListBuildSummariesV2GetResult },
    { "PFMultiplayerServerListBuildSummariesV2GetResultSize", HandlePFMultiplayerServerListBuildSummariesV2GetResultSize },
    { "PFMultiplayerServerListQosServersForTitleAsync", HandlePFMultiplayerServerListQosServersForTitleAsync },
    { "PFMultiplayerServerListQosServersForTitleGetResult", HandlePFMultiplayerServerListQosServersForTitleGetResult },
    { "PFMultiplayerServerListQosServersForTitleGetResultSize", HandlePFMultiplayerServerListQosServersForTitleGetResultSize },
    { "PFMultiplayerServerListSecretSummariesAsync", HandlePFMultiplayerServerListSecretSummariesAsync },
    { "PFMultiplayerServerListSecretSummariesGetResult", HandlePFMultiplayerServerListSecretSummariesGetResult },
    { "PFMultiplayerServerListSecretSummariesGetResultSize", HandlePFMultiplayerServerListSecretSummariesGetResultSize },
    { "PFMultiplayerServerRequestMultiplayerServerAsync", HandlePFMultiplayerServerRequestMultiplayerServerAsync },
    { "PFMultiplayerServerRequestMultiplayerServerGetResult", HandlePFMultiplayerServerRequestMultiplayerServerGetResult },
    { "PFMultiplayerServerRequestMultiplayerServerGetResultSize", HandlePFMultiplayerServerRequestMultiplayerServerGetResultSize },
    { "PFMultiplayerServerRequestPartyServiceAsync", HandlePFMultiplayerServerRequestPartyServiceAsync },
    { "PFMultiplayerServerRequestPartyServiceGetResult", HandlePFMultiplayerServerRequestPartyServiceGetResult },
    { "PFMultiplayerServerRequestPartyServiceGetResultSize", HandlePFMultiplayerServerRequestPartyServiceGetResultSize },
    { "PFMultiplayerServerUploadSecretAsync", HandlePFMultiplayerServerUploadSecretAsync }
});

#include "pch.h"
#include "PFCloudScriptHandlers.h"
#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <playfab/services/PFCloudScript.h>
#include <playfab/core/PFLocalUser.h>
#include <vector>
#include "CommandRegistry.h"

using CommandHandlerShared::TryGetStringParameter;
using CommandHandlerShared::TryParseBoolParameter;

CommandResultPayload HandlePFCloudScriptClientExecuteCloudScriptAsync(
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
            PFCloudScriptExecuteCloudScriptRequest request{};
            request.functionName = "helloWorld";
            const HRESULT hr = PFCloudScriptClientExecuteCloudScriptAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCloudScriptClientExecuteCloudScriptAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFCloudScriptClientExecuteCloudScriptGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFCloudScriptExecuteCloudScriptResult* result{ nullptr };
            HRESULT hr = PFCloudScriptClientExecuteCloudScriptGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFCloudScriptClientExecuteCloudScriptAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFCloudScriptClientExecuteCloudScriptGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCloudScriptClientExecuteCloudScriptGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCloudScriptClientExecuteCloudScriptGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCloudScriptClientExecuteCloudScriptGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCloudScriptServerExecuteCloudScriptAsync(
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
            PFCloudScriptExecuteCloudScriptServerRequest request{};
            const HRESULT hr = PFCloudScriptServerExecuteCloudScriptAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCloudScriptServerExecuteCloudScriptAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFCloudScriptServerExecuteCloudScriptGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFCloudScriptExecuteCloudScriptResult* result{ nullptr };
            HRESULT hr = PFCloudScriptServerExecuteCloudScriptGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFCloudScriptServerExecuteCloudScriptAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFCloudScriptServerExecuteCloudScriptGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCloudScriptServerExecuteCloudScriptGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCloudScriptServerExecuteCloudScriptGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCloudScriptServerExecuteCloudScriptGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCloudScriptExecuteEntityCloudScriptAsync(
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
            PFCloudScriptExecuteEntityCloudScriptRequest request{};
            request.functionName = "helloWorld";
            const HRESULT hr = PFCloudScriptExecuteEntityCloudScriptAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCloudScriptExecuteEntityCloudScriptAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFCloudScriptExecuteEntityCloudScriptGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFCloudScriptExecuteCloudScriptResult* result{ nullptr };
            HRESULT hr = PFCloudScriptExecuteEntityCloudScriptGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFCloudScriptExecuteEntityCloudScriptAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFCloudScriptExecuteEntityCloudScriptGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCloudScriptExecuteEntityCloudScriptGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCloudScriptExecuteEntityCloudScriptGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCloudScriptExecuteEntityCloudScriptGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCloudScriptExecuteFunctionAsync(
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
            PFCloudScriptExecuteFunctionRequest request{};
            request.functionName = "helloWorld";
            const HRESULT hr = PFCloudScriptExecuteFunctionAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFCloudScriptExecuteFunctionAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFCloudScriptExecuteFunctionGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFCloudScriptExecuteFunctionResult* result{ nullptr };
            HRESULT hr = PFCloudScriptExecuteFunctionGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFCloudScriptExecuteFunctionAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFCloudScriptExecuteFunctionGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCloudScriptExecuteFunctionGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCloudScriptExecuteFunctionGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCloudScriptExecuteFunctionGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFCloudScriptListEventHubFunctionsAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCloudScriptListEventHubFunctionsAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFCloudScriptListEventHubFunctionsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCloudScriptListEventHubFunctionsGetResultSize: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFCloudScriptListEventHubFunctionsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCloudScriptListEventHubFunctionsGetResult: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFCloudScriptRegisterEventHubFunctionAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFCloudScriptRegisterEventHubFunctionAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "PFCloudScriptClientExecuteCloudScriptAsync", HandlePFCloudScriptClientExecuteCloudScriptAsync },
    { "PFCloudScriptClientExecuteCloudScriptGetResult", HandlePFCloudScriptClientExecuteCloudScriptGetResult },
    { "PFCloudScriptClientExecuteCloudScriptGetResultSize", HandlePFCloudScriptClientExecuteCloudScriptGetResultSize },
    { "PFCloudScriptExecuteEntityCloudScriptAsync", HandlePFCloudScriptExecuteEntityCloudScriptAsync },
    { "PFCloudScriptExecuteEntityCloudScriptGetResult", HandlePFCloudScriptExecuteEntityCloudScriptGetResult },
    { "PFCloudScriptExecuteEntityCloudScriptGetResultSize", HandlePFCloudScriptExecuteEntityCloudScriptGetResultSize },
    { "PFCloudScriptExecuteFunctionAsync", HandlePFCloudScriptExecuteFunctionAsync },
    { "PFCloudScriptExecuteFunctionGetResult", HandlePFCloudScriptExecuteFunctionGetResult },
    { "PFCloudScriptExecuteFunctionGetResultSize", HandlePFCloudScriptExecuteFunctionGetResultSize },
    { "PFCloudScriptListEventHubFunctionsAsync", HandlePFCloudScriptListEventHubFunctionsAsync },
    { "PFCloudScriptListEventHubFunctionsGetResult", HandlePFCloudScriptListEventHubFunctionsGetResult },
    { "PFCloudScriptListEventHubFunctionsGetResultSize", HandlePFCloudScriptListEventHubFunctionsGetResultSize },
    { "PFCloudScriptRegisterEventHubFunctionAsync", HandlePFCloudScriptRegisterEventHubFunctionAsync },
    { "PFCloudScriptServerExecuteCloudScriptAsync", HandlePFCloudScriptServerExecuteCloudScriptAsync },
    { "PFCloudScriptServerExecuteCloudScriptGetResult", HandlePFCloudScriptServerExecuteCloudScriptGetResult },
    { "PFCloudScriptServerExecuteCloudScriptGetResultSize", HandlePFCloudScriptServerExecuteCloudScriptGetResultSize }
});
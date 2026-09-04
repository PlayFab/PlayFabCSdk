#include "pch.h"
#include "PFTitleDataManagementHandlers.h"
#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <playfab/services/PFTitleDataManagement.h>
#include <playfab/core/PFLocalUser.h>
#include <vector>
#include "CommandRegistry.h"

using CommandHandlerShared::TryGetStringParameter;
using CommandHandlerShared::TryParseBoolParameter;

CommandResultPayload HandlePFTitleDataManagementClientGetPublisherDataAsync(
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
            PFTitleDataManagementGetPublisherDataRequest request{};
            const HRESULT hr = PFTitleDataManagementClientGetPublisherDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFTitleDataManagementClientGetPublisherDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFTitleDataManagementClientGetPublisherDataGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFTitleDataManagementGetPublisherDataResult* result{ nullptr };
            HRESULT hr = PFTitleDataManagementClientGetPublisherDataGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFTitleDataManagementClientGetPublisherDataAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFTitleDataManagementClientGetPublisherDataGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFTitleDataManagementClientGetPublisherDataGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFTitleDataManagementClientGetPublisherDataGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFTitleDataManagementClientGetPublisherDataGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFTitleDataManagementClientGetTimeAsync(
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
            const HRESULT hr = PFTitleDataManagementClientGetTimeAsync(entityHandle, &async);
            LogToWindowFormat("PFTitleDataManagementClientGetTimeAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFTitleDataManagementGetTimeResult result{};
            return PFTitleDataManagementClientGetTimeGetResult(&async, &result);
        });
}

CommandResultPayload HandlePFTitleDataManagementClientGetTimeGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFTitleDataManagementClientGetTimeGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFTitleDataManagementClientGetTitleDataAsync(
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
            PFTitleDataManagementGetTitleDataRequest request{};
            const HRESULT hr = PFTitleDataManagementClientGetTitleDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFTitleDataManagementClientGetTitleDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFTitleDataManagementClientGetTitleDataGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFTitleDataManagementGetTitleDataResult* result{ nullptr };
            HRESULT hr = PFTitleDataManagementClientGetTitleDataGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFTitleDataManagementClientGetTitleDataAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFTitleDataManagementClientGetTitleDataGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFTitleDataManagementClientGetTitleDataGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFTitleDataManagementClientGetTitleDataGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFTitleDataManagementClientGetTitleDataGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFTitleDataManagementClientGetTitleNewsAsync(
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
            PFTitleDataManagementGetTitleNewsRequest request{};
            const HRESULT hr = PFTitleDataManagementClientGetTitleNewsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFTitleDataManagementClientGetTitleNewsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFTitleDataManagementClientGetTitleNewsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFTitleDataManagementGetTitleNewsResult* result{ nullptr };
            HRESULT hr = PFTitleDataManagementClientGetTitleNewsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFTitleDataManagementClientGetTitleNewsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFTitleDataManagementClientGetTitleNewsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFTitleDataManagementClientGetTitleNewsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFTitleDataManagementClientGetTitleNewsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFTitleDataManagementClientGetTitleNewsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFTitleDataManagementServerGetPublisherDataAsync(
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
            PFTitleDataManagementGetPublisherDataRequest request{};
            const HRESULT hr = PFTitleDataManagementServerGetPublisherDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFTitleDataManagementServerGetPublisherDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFTitleDataManagementServerGetPublisherDataGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFTitleDataManagementGetPublisherDataResult* result{ nullptr };
            HRESULT hr = PFTitleDataManagementServerGetPublisherDataGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFTitleDataManagementServerGetPublisherDataAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFTitleDataManagementServerGetPublisherDataGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFTitleDataManagementServerGetPublisherDataGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFTitleDataManagementServerGetPublisherDataGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFTitleDataManagementServerGetPublisherDataGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFTitleDataManagementServerGetTimeAsync(
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
            const HRESULT hr = PFTitleDataManagementServerGetTimeAsync(entityHandle, &async);
            LogToWindowFormat("PFTitleDataManagementServerGetTimeAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFTitleDataManagementGetTimeResult result{};
            return PFTitleDataManagementServerGetTimeGetResult(&async, &result);
        });
}

CommandResultPayload HandlePFTitleDataManagementServerGetTimeGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFTitleDataManagementServerGetTimeGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFTitleDataManagementServerGetTitleDataAsync(
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
            PFTitleDataManagementGetTitleDataRequest request{};
            const HRESULT hr = PFTitleDataManagementServerGetTitleDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFTitleDataManagementServerGetTitleDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFTitleDataManagementServerGetTitleDataGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFTitleDataManagementGetTitleDataResult* result{ nullptr };
            HRESULT hr = PFTitleDataManagementServerGetTitleDataGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFTitleDataManagementServerGetTitleDataAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFTitleDataManagementServerGetTitleDataGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFTitleDataManagementServerGetTitleDataGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFTitleDataManagementServerGetTitleDataGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFTitleDataManagementServerGetTitleDataGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFTitleDataManagementServerGetTitleInternalDataAsync(
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
            PFTitleDataManagementGetTitleDataRequest request{};
            const HRESULT hr = PFTitleDataManagementServerGetTitleInternalDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFTitleDataManagementServerGetTitleInternalDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFTitleDataManagementServerGetTitleInternalDataGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFTitleDataManagementGetTitleDataResult* result{ nullptr };
            HRESULT hr = PFTitleDataManagementServerGetTitleInternalDataGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFTitleDataManagementServerGetTitleInternalDataAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFTitleDataManagementServerGetTitleInternalDataGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFTitleDataManagementServerGetTitleInternalDataGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFTitleDataManagementServerGetTitleInternalDataGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFTitleDataManagementServerGetTitleInternalDataGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFTitleDataManagementServerGetTitleNewsAsync(
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
            PFTitleDataManagementGetTitleNewsRequest request{};
            const HRESULT hr = PFTitleDataManagementServerGetTitleNewsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFTitleDataManagementServerGetTitleNewsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFTitleDataManagementServerGetTitleNewsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFTitleDataManagementGetTitleNewsResult* result{ nullptr };
            HRESULT hr = PFTitleDataManagementServerGetTitleNewsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFTitleDataManagementServerGetTitleNewsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFTitleDataManagementServerGetTitleNewsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFTitleDataManagementServerGetTitleNewsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFTitleDataManagementServerGetTitleNewsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFTitleDataManagementServerGetTitleNewsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFTitleDataManagementServerSetPublisherDataAsync(
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
            PFTitleDataManagementSetPublisherDataRequest request{};
            const HRESULT hr = PFTitleDataManagementServerSetPublisherDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFTitleDataManagementServerSetPublisherDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFTitleDataManagementServerSetTitleDataAsync(
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
            PFTitleDataManagementSetTitleDataRequest request{};
            const HRESULT hr = PFTitleDataManagementServerSetTitleDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFTitleDataManagementServerSetTitleDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFTitleDataManagementServerSetTitleInternalDataAsync(
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
            PFTitleDataManagementSetTitleDataRequest request{};
            const HRESULT hr = PFTitleDataManagementServerSetTitleInternalDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFTitleDataManagementServerSetTitleInternalDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "PFTitleDataManagementClientGetPublisherDataAsync", HandlePFTitleDataManagementClientGetPublisherDataAsync },
    { "PFTitleDataManagementClientGetPublisherDataGetResult", HandlePFTitleDataManagementClientGetPublisherDataGetResult },
    { "PFTitleDataManagementClientGetPublisherDataGetResultSize", HandlePFTitleDataManagementClientGetPublisherDataGetResultSize },
    { "PFTitleDataManagementClientGetTimeAsync", HandlePFTitleDataManagementClientGetTimeAsync },
    { "PFTitleDataManagementClientGetTimeGetResult", HandlePFTitleDataManagementClientGetTimeGetResult },
    { "PFTitleDataManagementClientGetTitleDataAsync", HandlePFTitleDataManagementClientGetTitleDataAsync },
    { "PFTitleDataManagementClientGetTitleDataGetResult", HandlePFTitleDataManagementClientGetTitleDataGetResult },
    { "PFTitleDataManagementClientGetTitleDataGetResultSize", HandlePFTitleDataManagementClientGetTitleDataGetResultSize },
    { "PFTitleDataManagementClientGetTitleNewsAsync", HandlePFTitleDataManagementClientGetTitleNewsAsync },
    { "PFTitleDataManagementClientGetTitleNewsGetResult", HandlePFTitleDataManagementClientGetTitleNewsGetResult },
    { "PFTitleDataManagementClientGetTitleNewsGetResultSize", HandlePFTitleDataManagementClientGetTitleNewsGetResultSize },
    { "PFTitleDataManagementServerGetPublisherDataAsync", HandlePFTitleDataManagementServerGetPublisherDataAsync },
    { "PFTitleDataManagementServerGetPublisherDataGetResult", HandlePFTitleDataManagementServerGetPublisherDataGetResult },
    { "PFTitleDataManagementServerGetPublisherDataGetResultSize", HandlePFTitleDataManagementServerGetPublisherDataGetResultSize },
    { "PFTitleDataManagementServerGetTimeAsync", HandlePFTitleDataManagementServerGetTimeAsync },
    { "PFTitleDataManagementServerGetTimeGetResult", HandlePFTitleDataManagementServerGetTimeGetResult },
    { "PFTitleDataManagementServerGetTitleDataAsync", HandlePFTitleDataManagementServerGetTitleDataAsync },
    { "PFTitleDataManagementServerGetTitleDataGetResult", HandlePFTitleDataManagementServerGetTitleDataGetResult },
    { "PFTitleDataManagementServerGetTitleDataGetResultSize", HandlePFTitleDataManagementServerGetTitleDataGetResultSize },
    { "PFTitleDataManagementServerGetTitleInternalDataAsync", HandlePFTitleDataManagementServerGetTitleInternalDataAsync },
    { "PFTitleDataManagementServerGetTitleInternalDataGetResult", HandlePFTitleDataManagementServerGetTitleInternalDataGetResult },
    { "PFTitleDataManagementServerGetTitleInternalDataGetResultSize", HandlePFTitleDataManagementServerGetTitleInternalDataGetResultSize },
    { "PFTitleDataManagementServerGetTitleNewsAsync", HandlePFTitleDataManagementServerGetTitleNewsAsync },
    { "PFTitleDataManagementServerGetTitleNewsGetResult", HandlePFTitleDataManagementServerGetTitleNewsGetResult },
    { "PFTitleDataManagementServerGetTitleNewsGetResultSize", HandlePFTitleDataManagementServerGetTitleNewsGetResultSize },
    { "PFTitleDataManagementServerSetPublisherDataAsync", HandlePFTitleDataManagementServerSetPublisherDataAsync },
    { "PFTitleDataManagementServerSetTitleDataAsync", HandlePFTitleDataManagementServerSetTitleDataAsync },
    { "PFTitleDataManagementServerSetTitleInternalDataAsync", HandlePFTitleDataManagementServerSetTitleInternalDataAsync }
});
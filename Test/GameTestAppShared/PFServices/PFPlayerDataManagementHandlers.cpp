#include "pch.h"
#include "PFPlayerDataManagementHandlers.h"
#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <playfab/services/PFPlayerDataManagement.h>
#include <playfab/core/PFLocalUser.h>
#include <vector>
#include "CommandRegistry.h"

using CommandHandlerShared::TryGetStringParameter;
using CommandHandlerShared::TryParseBoolParameter;

CommandResultPayload HandlePFPlayerDataManagementClientDeletePlayerCustomPropertiesAsync(
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
            PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequest request{};
            const HRESULT hr = PFPlayerDataManagementClientDeletePlayerCustomPropertiesAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementClientDeletePlayerCustomPropertiesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFPlayerDataManagementClientDeletePlayerCustomPropertiesGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFPlayerDataManagementClientDeletePlayerCustomPropertiesResult* result{ nullptr };
            HRESULT hr = PFPlayerDataManagementClientDeletePlayerCustomPropertiesGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFPlayerDataManagementClientDeletePlayerCustomPropertiesAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientDeletePlayerCustomPropertiesGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementClientDeletePlayerCustomPropertiesGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientDeletePlayerCustomPropertiesGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementClientDeletePlayerCustomPropertiesGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientGetPlayerCustomPropertyAsync(
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
            PFPlayerDataManagementClientGetPlayerCustomPropertyRequest request{};
            const HRESULT hr = PFPlayerDataManagementClientGetPlayerCustomPropertyAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementClientGetPlayerCustomPropertyAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFPlayerDataManagementClientGetPlayerCustomPropertyGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFPlayerDataManagementClientGetPlayerCustomPropertyResult* result{ nullptr };
            HRESULT hr = PFPlayerDataManagementClientGetPlayerCustomPropertyGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFPlayerDataManagementClientGetPlayerCustomPropertyAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientGetPlayerCustomPropertyGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementClientGetPlayerCustomPropertyGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientGetPlayerCustomPropertyGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementClientGetPlayerCustomPropertyGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientGetUserDataAsync(
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
            PFPlayerDataManagementGetUserDataRequest request{};
            const HRESULT hr = PFPlayerDataManagementClientGetUserDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementClientGetUserDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFPlayerDataManagementClientGetUserDataGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFPlayerDataManagementClientGetUserDataResult* result{ nullptr };
            HRESULT hr = PFPlayerDataManagementClientGetUserDataGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFPlayerDataManagementClientGetUserDataAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientGetUserDataGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementClientGetUserDataGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientGetUserDataGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementClientGetUserDataGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientGetUserPublisherDataAsync(
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
            PFPlayerDataManagementGetUserDataRequest request{};
            const HRESULT hr = PFPlayerDataManagementClientGetUserPublisherDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementClientGetUserPublisherDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFPlayerDataManagementClientGetUserPublisherDataGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFPlayerDataManagementClientGetUserDataResult* result{ nullptr };
            HRESULT hr = PFPlayerDataManagementClientGetUserPublisherDataGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFPlayerDataManagementClientGetUserPublisherDataAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientGetUserPublisherDataGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementClientGetUserPublisherDataGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientGetUserPublisherDataGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementClientGetUserPublisherDataGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientGetUserPublisherReadOnlyDataAsync(
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
            PFPlayerDataManagementGetUserDataRequest request{};
            const HRESULT hr = PFPlayerDataManagementClientGetUserPublisherReadOnlyDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementClientGetUserPublisherReadOnlyDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFPlayerDataManagementClientGetUserPublisherReadOnlyDataGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFPlayerDataManagementClientGetUserDataResult* result{ nullptr };
            HRESULT hr = PFPlayerDataManagementClientGetUserPublisherReadOnlyDataGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFPlayerDataManagementClientGetUserPublisherReadOnlyDataAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientGetUserPublisherReadOnlyDataGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementClientGetUserPublisherReadOnlyDataGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientGetUserPublisherReadOnlyDataGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementClientGetUserPublisherReadOnlyDataGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientGetUserReadOnlyDataAsync(
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
            PFPlayerDataManagementGetUserDataRequest request{};
            const HRESULT hr = PFPlayerDataManagementClientGetUserReadOnlyDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementClientGetUserReadOnlyDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFPlayerDataManagementClientGetUserReadOnlyDataGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFPlayerDataManagementClientGetUserDataResult* result{ nullptr };
            HRESULT hr = PFPlayerDataManagementClientGetUserReadOnlyDataGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFPlayerDataManagementClientGetUserReadOnlyDataAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientGetUserReadOnlyDataGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementClientGetUserReadOnlyDataGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientGetUserReadOnlyDataGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementClientGetUserReadOnlyDataGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientListPlayerCustomPropertiesAsync(
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
            const HRESULT hr = PFPlayerDataManagementClientListPlayerCustomPropertiesAsync(entityHandle, &async);
            LogToWindowFormat("PFPlayerDataManagementClientListPlayerCustomPropertiesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFPlayerDataManagementClientListPlayerCustomPropertiesGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFPlayerDataManagementClientListPlayerCustomPropertiesResult* result{ nullptr };
            HRESULT hr = PFPlayerDataManagementClientListPlayerCustomPropertiesGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFPlayerDataManagementClientListPlayerCustomPropertiesAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientListPlayerCustomPropertiesGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementClientListPlayerCustomPropertiesGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientListPlayerCustomPropertiesGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementClientListPlayerCustomPropertiesGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientUpdatePlayerCustomPropertiesAsync(
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
            PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequest request{};
            PFJsonObject propValue{ "\"testValue\"" };
            PFPlayerDataManagementUpdateProperty prop{};
            prop.name = "testProp";
            prop.value = propValue;
            const PFPlayerDataManagementUpdateProperty* propPtr = &prop;
            request.properties = &propPtr;
            request.propertiesCount = 1;
            const HRESULT hr = PFPlayerDataManagementClientUpdatePlayerCustomPropertiesAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementClientUpdatePlayerCustomPropertiesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFPlayerDataManagementClientUpdatePlayerCustomPropertiesResult result{};
            return PFPlayerDataManagementClientUpdatePlayerCustomPropertiesGetResult(&async, &result);
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientUpdatePlayerCustomPropertiesGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementClientUpdatePlayerCustomPropertiesGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientUpdateUserDataAsync(
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
            PFPlayerDataManagementClientUpdateUserDataRequest request{};
            PFStringDictionaryEntry dataEntry{ "testKey", "testValue" };
            request.data = &dataEntry;
            request.dataCount = 1;
            const HRESULT hr = PFPlayerDataManagementClientUpdateUserDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementClientUpdateUserDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFPlayerDataManagementUpdateUserDataResult result{};
            return PFPlayerDataManagementClientUpdateUserDataGetResult(&async, &result);
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientUpdateUserDataGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementClientUpdateUserDataGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientUpdateUserPublisherDataAsync(
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
            PFPlayerDataManagementClientUpdateUserDataRequest request{};
            const HRESULT hr = PFPlayerDataManagementClientUpdateUserPublisherDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementClientUpdateUserPublisherDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFPlayerDataManagementUpdateUserDataResult result{};
            return PFPlayerDataManagementClientUpdateUserPublisherDataGetResult(&async, &result);
        });
}

CommandResultPayload HandlePFPlayerDataManagementClientUpdateUserPublisherDataGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementClientUpdateUserPublisherDataGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerDeletePlayerCustomPropertiesAsync(
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
            PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequest request{};
            const HRESULT hr = PFPlayerDataManagementServerDeletePlayerCustomPropertiesAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementServerDeletePlayerCustomPropertiesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFPlayerDataManagementServerDeletePlayerCustomPropertiesGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFPlayerDataManagementServerDeletePlayerCustomPropertiesResult* result{ nullptr };
            HRESULT hr = PFPlayerDataManagementServerDeletePlayerCustomPropertiesGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFPlayerDataManagementServerDeletePlayerCustomPropertiesAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerDeletePlayerCustomPropertiesGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerDeletePlayerCustomPropertiesGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerDeletePlayerCustomPropertiesGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerDeletePlayerCustomPropertiesGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerGetPlayerCustomPropertyAsync(
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
            PFPlayerDataManagementServerGetPlayerCustomPropertyRequest request{};
            const HRESULT hr = PFPlayerDataManagementServerGetPlayerCustomPropertyAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementServerGetPlayerCustomPropertyAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFPlayerDataManagementServerGetPlayerCustomPropertyGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFPlayerDataManagementServerGetPlayerCustomPropertyResult* result{ nullptr };
            HRESULT hr = PFPlayerDataManagementServerGetPlayerCustomPropertyGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFPlayerDataManagementServerGetPlayerCustomPropertyAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerGetPlayerCustomPropertyGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerGetPlayerCustomPropertyGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerGetPlayerCustomPropertyGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerGetPlayerCustomPropertyGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerGetUserDataAsync(
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
            PFPlayerDataManagementGetUserDataRequest request{};
            const HRESULT hr = PFPlayerDataManagementServerGetUserDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementServerGetUserDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFPlayerDataManagementServerGetUserDataGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFPlayerDataManagementServerGetUserDataResult* result{ nullptr };
            HRESULT hr = PFPlayerDataManagementServerGetUserDataGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFPlayerDataManagementServerGetUserDataAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerGetUserDataGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerGetUserDataGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerGetUserDataGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerGetUserDataGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerGetUserInternalDataAsync(
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
            PFPlayerDataManagementGetUserDataRequest request{};
            const HRESULT hr = PFPlayerDataManagementServerGetUserInternalDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementServerGetUserInternalDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFPlayerDataManagementServerGetUserInternalDataGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFPlayerDataManagementServerGetUserDataResult* result{ nullptr };
            HRESULT hr = PFPlayerDataManagementServerGetUserInternalDataGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFPlayerDataManagementServerGetUserInternalDataAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerGetUserInternalDataGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerGetUserInternalDataGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerGetUserInternalDataGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerGetUserInternalDataGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerGetUserPublisherDataAsync(
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
            PFPlayerDataManagementGetUserDataRequest request{};
            const HRESULT hr = PFPlayerDataManagementServerGetUserPublisherDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementServerGetUserPublisherDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFPlayerDataManagementServerGetUserPublisherDataGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFPlayerDataManagementServerGetUserDataResult* result{ nullptr };
            HRESULT hr = PFPlayerDataManagementServerGetUserPublisherDataGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFPlayerDataManagementServerGetUserPublisherDataAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerGetUserPublisherDataGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerGetUserPublisherDataGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerGetUserPublisherDataGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerGetUserPublisherDataGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerGetUserPublisherInternalDataAsync(
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
            PFPlayerDataManagementGetUserDataRequest request{};
            const HRESULT hr = PFPlayerDataManagementServerGetUserPublisherInternalDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementServerGetUserPublisherInternalDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFPlayerDataManagementServerGetUserPublisherInternalDataGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFPlayerDataManagementServerGetUserDataResult* result{ nullptr };
            HRESULT hr = PFPlayerDataManagementServerGetUserPublisherInternalDataGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFPlayerDataManagementServerGetUserPublisherInternalDataAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerGetUserPublisherInternalDataGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerGetUserPublisherInternalDataGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerGetUserPublisherInternalDataGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerGetUserPublisherInternalDataGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerGetUserPublisherReadOnlyDataAsync(
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
            PFPlayerDataManagementGetUserDataRequest request{};
            const HRESULT hr = PFPlayerDataManagementServerGetUserPublisherReadOnlyDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementServerGetUserPublisherReadOnlyDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFPlayerDataManagementServerGetUserPublisherReadOnlyDataGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFPlayerDataManagementServerGetUserDataResult* result{ nullptr };
            HRESULT hr = PFPlayerDataManagementServerGetUserPublisherReadOnlyDataGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFPlayerDataManagementServerGetUserPublisherReadOnlyDataAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerGetUserPublisherReadOnlyDataGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerGetUserPublisherReadOnlyDataGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerGetUserPublisherReadOnlyDataGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerGetUserPublisherReadOnlyDataGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerGetUserReadOnlyDataAsync(
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
            PFPlayerDataManagementGetUserDataRequest request{};
            const HRESULT hr = PFPlayerDataManagementServerGetUserReadOnlyDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementServerGetUserReadOnlyDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFPlayerDataManagementServerGetUserReadOnlyDataGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFPlayerDataManagementServerGetUserDataResult* result{ nullptr };
            HRESULT hr = PFPlayerDataManagementServerGetUserReadOnlyDataGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFPlayerDataManagementServerGetUserReadOnlyDataAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerGetUserReadOnlyDataGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerGetUserReadOnlyDataGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerGetUserReadOnlyDataGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerGetUserReadOnlyDataGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerListPlayerCustomPropertiesAsync(
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
            PFPlayerDataManagementListPlayerCustomPropertiesRequest request{};
            const HRESULT hr = PFPlayerDataManagementServerListPlayerCustomPropertiesAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementServerListPlayerCustomPropertiesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFPlayerDataManagementServerListPlayerCustomPropertiesGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFPlayerDataManagementServerListPlayerCustomPropertiesResult* result{ nullptr };
            HRESULT hr = PFPlayerDataManagementServerListPlayerCustomPropertiesGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFPlayerDataManagementServerListPlayerCustomPropertiesAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerListPlayerCustomPropertiesGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerListPlayerCustomPropertiesGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerListPlayerCustomPropertiesGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerListPlayerCustomPropertiesGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerUpdatePlayerCustomPropertiesAsync(
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
            PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequest request{};
            const HRESULT hr = PFPlayerDataManagementServerUpdatePlayerCustomPropertiesAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementServerUpdatePlayerCustomPropertiesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFPlayerDataManagementServerUpdatePlayerCustomPropertiesGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResult* result{ nullptr };
            HRESULT hr = PFPlayerDataManagementServerUpdatePlayerCustomPropertiesGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFPlayerDataManagementServerUpdatePlayerCustomPropertiesAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerUpdatePlayerCustomPropertiesGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerUpdatePlayerCustomPropertiesGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerUpdatePlayerCustomPropertiesGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerUpdatePlayerCustomPropertiesGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerUpdateUserDataAsync(
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
            PFPlayerDataManagementServerUpdateUserDataRequest request{};
            const HRESULT hr = PFPlayerDataManagementServerUpdateUserDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementServerUpdateUserDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFPlayerDataManagementUpdateUserDataResult result{};
            return PFPlayerDataManagementServerUpdateUserDataGetResult(&async, &result);
        });
}


CommandResultPayload HandlePFPlayerDataManagementServerUpdateUserDataGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerUpdateUserDataGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerUpdateUserInternalDataAsync(
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
            PFPlayerDataManagementUpdateUserInternalDataRequest request{};
            const HRESULT hr = PFPlayerDataManagementServerUpdateUserInternalDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementServerUpdateUserInternalDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFPlayerDataManagementUpdateUserDataResult result{};
            return PFPlayerDataManagementServerUpdateUserInternalDataGetResult(&async, &result);
        });
}


CommandResultPayload HandlePFPlayerDataManagementServerUpdateUserInternalDataGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerUpdateUserInternalDataGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerUpdateUserPublisherDataAsync(
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
            PFPlayerDataManagementServerUpdateUserDataRequest request{};
            const HRESULT hr = PFPlayerDataManagementServerUpdateUserPublisherDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementServerUpdateUserPublisherDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFPlayerDataManagementUpdateUserDataResult result{};
            return PFPlayerDataManagementServerUpdateUserPublisherDataGetResult(&async, &result);
        });
}


CommandResultPayload HandlePFPlayerDataManagementServerUpdateUserPublisherDataGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerUpdateUserPublisherDataGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerUpdateUserPublisherInternalDataAsync(
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
            PFPlayerDataManagementUpdateUserInternalDataRequest request{};
            const HRESULT hr = PFPlayerDataManagementServerUpdateUserPublisherInternalDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementServerUpdateUserPublisherInternalDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFPlayerDataManagementUpdateUserDataResult result{};
            return PFPlayerDataManagementServerUpdateUserPublisherInternalDataGetResult(&async, &result);
        });
}


CommandResultPayload HandlePFPlayerDataManagementServerUpdateUserPublisherInternalDataGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerUpdateUserPublisherInternalDataGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerUpdateUserPublisherReadOnlyDataAsync(
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
            PFPlayerDataManagementServerUpdateUserDataRequest request{};
            const HRESULT hr = PFPlayerDataManagementServerUpdateUserPublisherReadOnlyDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementServerUpdateUserPublisherReadOnlyDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFPlayerDataManagementUpdateUserDataResult result{};
            return PFPlayerDataManagementServerUpdateUserPublisherReadOnlyDataGetResult(&async, &result);
        });
}


CommandResultPayload HandlePFPlayerDataManagementServerUpdateUserPublisherReadOnlyDataGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerUpdateUserPublisherReadOnlyDataGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlayerDataManagementServerUpdateUserReadOnlyDataAsync(
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
            PFPlayerDataManagementServerUpdateUserDataRequest request{};
            const HRESULT hr = PFPlayerDataManagementServerUpdateUserReadOnlyDataAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPlayerDataManagementServerUpdateUserReadOnlyDataAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFPlayerDataManagementUpdateUserDataResult result{};
            return PFPlayerDataManagementServerUpdateUserReadOnlyDataGetResult(&async, &result);
        });
}


CommandResultPayload HandlePFPlayerDataManagementServerUpdateUserReadOnlyDataGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlayerDataManagementServerUpdateUserReadOnlyDataGetResult: called inline by Async handler");
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "PFPlayerDataManagementClientDeletePlayerCustomPropertiesAsync", HandlePFPlayerDataManagementClientDeletePlayerCustomPropertiesAsync },
    { "PFPlayerDataManagementClientDeletePlayerCustomPropertiesGetResult", HandlePFPlayerDataManagementClientDeletePlayerCustomPropertiesGetResult },
    { "PFPlayerDataManagementClientDeletePlayerCustomPropertiesGetResultSize", HandlePFPlayerDataManagementClientDeletePlayerCustomPropertiesGetResultSize },
    { "PFPlayerDataManagementClientGetPlayerCustomPropertyAsync", HandlePFPlayerDataManagementClientGetPlayerCustomPropertyAsync },
    { "PFPlayerDataManagementClientGetPlayerCustomPropertyGetResult", HandlePFPlayerDataManagementClientGetPlayerCustomPropertyGetResult },
    { "PFPlayerDataManagementClientGetPlayerCustomPropertyGetResultSize", HandlePFPlayerDataManagementClientGetPlayerCustomPropertyGetResultSize },
    { "PFPlayerDataManagementClientGetUserDataAsync", HandlePFPlayerDataManagementClientGetUserDataAsync },
    { "PFPlayerDataManagementClientGetUserDataGetResult", HandlePFPlayerDataManagementClientGetUserDataGetResult },
    { "PFPlayerDataManagementClientGetUserDataGetResultSize", HandlePFPlayerDataManagementClientGetUserDataGetResultSize },
    { "PFPlayerDataManagementClientGetUserPublisherDataAsync", HandlePFPlayerDataManagementClientGetUserPublisherDataAsync },
    { "PFPlayerDataManagementClientGetUserPublisherDataGetResult", HandlePFPlayerDataManagementClientGetUserPublisherDataGetResult },
    { "PFPlayerDataManagementClientGetUserPublisherDataGetResultSize", HandlePFPlayerDataManagementClientGetUserPublisherDataGetResultSize },
    { "PFPlayerDataManagementClientGetUserPublisherReadOnlyDataAsync", HandlePFPlayerDataManagementClientGetUserPublisherReadOnlyDataAsync },
    { "PFPlayerDataManagementClientGetUserPublisherReadOnlyDataGetResult", HandlePFPlayerDataManagementClientGetUserPublisherReadOnlyDataGetResult },
    { "PFPlayerDataManagementClientGetUserPublisherReadOnlyDataGetResultSize", HandlePFPlayerDataManagementClientGetUserPublisherReadOnlyDataGetResultSize },
    { "PFPlayerDataManagementClientGetUserReadOnlyDataAsync", HandlePFPlayerDataManagementClientGetUserReadOnlyDataAsync },
    { "PFPlayerDataManagementClientGetUserReadOnlyDataGetResult", HandlePFPlayerDataManagementClientGetUserReadOnlyDataGetResult },
    { "PFPlayerDataManagementClientGetUserReadOnlyDataGetResultSize", HandlePFPlayerDataManagementClientGetUserReadOnlyDataGetResultSize },
    { "PFPlayerDataManagementClientListPlayerCustomPropertiesAsync", HandlePFPlayerDataManagementClientListPlayerCustomPropertiesAsync },
    { "PFPlayerDataManagementClientListPlayerCustomPropertiesGetResult", HandlePFPlayerDataManagementClientListPlayerCustomPropertiesGetResult },
    { "PFPlayerDataManagementClientListPlayerCustomPropertiesGetResultSize", HandlePFPlayerDataManagementClientListPlayerCustomPropertiesGetResultSize },
    { "PFPlayerDataManagementClientUpdatePlayerCustomPropertiesAsync", HandlePFPlayerDataManagementClientUpdatePlayerCustomPropertiesAsync },
    { "PFPlayerDataManagementClientUpdatePlayerCustomPropertiesGetResult", HandlePFPlayerDataManagementClientUpdatePlayerCustomPropertiesGetResult },
    { "PFPlayerDataManagementClientUpdateUserDataAsync", HandlePFPlayerDataManagementClientUpdateUserDataAsync },
    { "PFPlayerDataManagementClientUpdateUserDataGetResult", HandlePFPlayerDataManagementClientUpdateUserDataGetResult },
    { "PFPlayerDataManagementClientUpdateUserPublisherDataAsync", HandlePFPlayerDataManagementClientUpdateUserPublisherDataAsync },
    { "PFPlayerDataManagementClientUpdateUserPublisherDataGetResult", HandlePFPlayerDataManagementClientUpdateUserPublisherDataGetResult },
    { "PFPlayerDataManagementServerDeletePlayerCustomPropertiesAsync", HandlePFPlayerDataManagementServerDeletePlayerCustomPropertiesAsync },
    { "PFPlayerDataManagementServerDeletePlayerCustomPropertiesGetResult", HandlePFPlayerDataManagementServerDeletePlayerCustomPropertiesGetResult },
    { "PFPlayerDataManagementServerDeletePlayerCustomPropertiesGetResultSize", HandlePFPlayerDataManagementServerDeletePlayerCustomPropertiesGetResultSize },
    { "PFPlayerDataManagementServerGetPlayerCustomPropertyAsync", HandlePFPlayerDataManagementServerGetPlayerCustomPropertyAsync },
    { "PFPlayerDataManagementServerGetPlayerCustomPropertyGetResult", HandlePFPlayerDataManagementServerGetPlayerCustomPropertyGetResult },
    { "PFPlayerDataManagementServerGetPlayerCustomPropertyGetResultSize", HandlePFPlayerDataManagementServerGetPlayerCustomPropertyGetResultSize },
    { "PFPlayerDataManagementServerGetUserDataAsync", HandlePFPlayerDataManagementServerGetUserDataAsync },
    { "PFPlayerDataManagementServerGetUserDataGetResult", HandlePFPlayerDataManagementServerGetUserDataGetResult },
    { "PFPlayerDataManagementServerGetUserDataGetResultSize", HandlePFPlayerDataManagementServerGetUserDataGetResultSize },
    { "PFPlayerDataManagementServerGetUserInternalDataAsync", HandlePFPlayerDataManagementServerGetUserInternalDataAsync },
    { "PFPlayerDataManagementServerGetUserInternalDataGetResult", HandlePFPlayerDataManagementServerGetUserInternalDataGetResult },
    { "PFPlayerDataManagementServerGetUserInternalDataGetResultSize", HandlePFPlayerDataManagementServerGetUserInternalDataGetResultSize },
    { "PFPlayerDataManagementServerGetUserPublisherDataAsync", HandlePFPlayerDataManagementServerGetUserPublisherDataAsync },
    { "PFPlayerDataManagementServerGetUserPublisherDataGetResult", HandlePFPlayerDataManagementServerGetUserPublisherDataGetResult },
    { "PFPlayerDataManagementServerGetUserPublisherDataGetResultSize", HandlePFPlayerDataManagementServerGetUserPublisherDataGetResultSize },
    { "PFPlayerDataManagementServerGetUserPublisherInternalDataAsync", HandlePFPlayerDataManagementServerGetUserPublisherInternalDataAsync },
    { "PFPlayerDataManagementServerGetUserPublisherInternalDataGetResult", HandlePFPlayerDataManagementServerGetUserPublisherInternalDataGetResult },
    { "PFPlayerDataManagementServerGetUserPublisherInternalDataGetResultSize", HandlePFPlayerDataManagementServerGetUserPublisherInternalDataGetResultSize },
    { "PFPlayerDataManagementServerGetUserPublisherReadOnlyDataAsync", HandlePFPlayerDataManagementServerGetUserPublisherReadOnlyDataAsync },
    { "PFPlayerDataManagementServerGetUserPublisherReadOnlyDataGetResult", HandlePFPlayerDataManagementServerGetUserPublisherReadOnlyDataGetResult },
    { "PFPlayerDataManagementServerGetUserPublisherReadOnlyDataGetResultSize", HandlePFPlayerDataManagementServerGetUserPublisherReadOnlyDataGetResultSize },
    { "PFPlayerDataManagementServerGetUserReadOnlyDataAsync", HandlePFPlayerDataManagementServerGetUserReadOnlyDataAsync },
    { "PFPlayerDataManagementServerGetUserReadOnlyDataGetResult", HandlePFPlayerDataManagementServerGetUserReadOnlyDataGetResult },
    { "PFPlayerDataManagementServerGetUserReadOnlyDataGetResultSize", HandlePFPlayerDataManagementServerGetUserReadOnlyDataGetResultSize },
    { "PFPlayerDataManagementServerListPlayerCustomPropertiesAsync", HandlePFPlayerDataManagementServerListPlayerCustomPropertiesAsync },
    { "PFPlayerDataManagementServerListPlayerCustomPropertiesGetResult", HandlePFPlayerDataManagementServerListPlayerCustomPropertiesGetResult },
    { "PFPlayerDataManagementServerListPlayerCustomPropertiesGetResultSize", HandlePFPlayerDataManagementServerListPlayerCustomPropertiesGetResultSize },
    { "PFPlayerDataManagementServerUpdatePlayerCustomPropertiesAsync", HandlePFPlayerDataManagementServerUpdatePlayerCustomPropertiesAsync },
    { "PFPlayerDataManagementServerUpdatePlayerCustomPropertiesGetResult", HandlePFPlayerDataManagementServerUpdatePlayerCustomPropertiesGetResult },
    { "PFPlayerDataManagementServerUpdatePlayerCustomPropertiesGetResultSize", HandlePFPlayerDataManagementServerUpdatePlayerCustomPropertiesGetResultSize },
    { "PFPlayerDataManagementServerUpdateUserDataAsync", HandlePFPlayerDataManagementServerUpdateUserDataAsync },
    { "PFPlayerDataManagementServerUpdateUserDataGetResult", HandlePFPlayerDataManagementServerUpdateUserDataGetResult },
    { "PFPlayerDataManagementServerUpdateUserInternalDataAsync", HandlePFPlayerDataManagementServerUpdateUserInternalDataAsync },
    { "PFPlayerDataManagementServerUpdateUserInternalDataGetResult", HandlePFPlayerDataManagementServerUpdateUserInternalDataGetResult },
    { "PFPlayerDataManagementServerUpdateUserPublisherDataAsync", HandlePFPlayerDataManagementServerUpdateUserPublisherDataAsync },
    { "PFPlayerDataManagementServerUpdateUserPublisherDataGetResult", HandlePFPlayerDataManagementServerUpdateUserPublisherDataGetResult },
    { "PFPlayerDataManagementServerUpdateUserPublisherInternalDataAsync", HandlePFPlayerDataManagementServerUpdateUserPublisherInternalDataAsync },
    { "PFPlayerDataManagementServerUpdateUserPublisherInternalDataGetResult", HandlePFPlayerDataManagementServerUpdateUserPublisherInternalDataGetResult },
    { "PFPlayerDataManagementServerUpdateUserPublisherReadOnlyDataAsync", HandlePFPlayerDataManagementServerUpdateUserPublisherReadOnlyDataAsync },
    { "PFPlayerDataManagementServerUpdateUserPublisherReadOnlyDataGetResult", HandlePFPlayerDataManagementServerUpdateUserPublisherReadOnlyDataGetResult },
    { "PFPlayerDataManagementServerUpdateUserReadOnlyDataAsync", HandlePFPlayerDataManagementServerUpdateUserReadOnlyDataAsync },
    { "PFPlayerDataManagementServerUpdateUserReadOnlyDataGetResult", HandlePFPlayerDataManagementServerUpdateUserReadOnlyDataGetResult }
});
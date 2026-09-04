#include "pch.h"
#include "PFDataHandlers.h"
#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <playfab/services/PFData.h>
#include <playfab/core/PFLocalUser.h>
#include <vector>
#include "CommandRegistry.h"

using CommandHandlerShared::TryGetStringParameter;
using CommandHandlerShared::TryParseBoolParameter;

CommandResultPayload HandlePFDataAbortFileUploadsAsync(
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
            PFDataAbortFileUploadsRequest request{};
            PFEntityKey entityKey{ state->entityId.c_str(), state->entityType.c_str() };
            request.entity = &entityKey;
            const char* fileNames[] = { "testfile.txt" };
            request.fileNames = fileNames;
            request.fileNamesCount = 1;
            const HRESULT hr = PFDataAbortFileUploadsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFDataAbortFileUploadsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFDataAbortFileUploadsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFDataAbortFileUploadsResponse* result{ nullptr };
            HRESULT hr = PFDataAbortFileUploadsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFDataAbortFileUploadsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFDataAbortFileUploadsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFDataAbortFileUploadsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFDataAbortFileUploadsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFDataAbortFileUploadsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFDataDeleteFilesAsync(
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
            PFDataDeleteFilesRequest request{};
            PFEntityKey entityKey{ state->entityId.c_str(), state->entityType.c_str() };
            request.entity = &entityKey;
            const char* fileNames[] = { "testfile.txt" };
            request.fileNames = fileNames;
            request.fileNamesCount = 1;
            const HRESULT hr = PFDataDeleteFilesAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFDataDeleteFilesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFDataDeleteFilesGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFDataDeleteFilesResponse* result{ nullptr };
            HRESULT hr = PFDataDeleteFilesGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFDataDeleteFilesAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFDataDeleteFilesGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFDataDeleteFilesGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFDataDeleteFilesGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFDataDeleteFilesGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFDataFinalizeFileUploadsAsync(
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
            PFDataFinalizeFileUploadsRequest request{};
            PFEntityKey entityKey{ state->entityId.c_str(), state->entityType.c_str() };
            request.entity = &entityKey;
            const char* fileNames[] = { "testfile.txt" };
            request.fileNames = fileNames;
            request.fileNamesCount = 1;
            const HRESULT hr = PFDataFinalizeFileUploadsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFDataFinalizeFileUploadsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFDataFinalizeFileUploadsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFDataFinalizeFileUploadsResponse* result{ nullptr };
            HRESULT hr = PFDataFinalizeFileUploadsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFDataFinalizeFileUploadsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFDataFinalizeFileUploadsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFDataFinalizeFileUploadsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFDataFinalizeFileUploadsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFDataFinalizeFileUploadsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFDataGetFilesAsync(
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
            PFDataGetFilesRequest request{};
            PFEntityKey entityKey{ state->entityId.c_str(), state->entityType.c_str() };
            request.entity = &entityKey;
            const HRESULT hr = PFDataGetFilesAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFDataGetFilesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFDataGetFilesGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFDataGetFilesResponse* result{ nullptr };
            HRESULT hr = PFDataGetFilesGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFDataGetFilesAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFDataGetFilesGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFDataGetFilesGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFDataGetFilesGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFDataGetFilesGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFDataGetObjectsAsync(
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
            PFDataGetObjectsRequest request{};
            PFEntityKey entityKey{ state->entityId.c_str(), state->entityType.c_str() };
            request.entity = &entityKey;
            const HRESULT hr = PFDataGetObjectsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFDataGetObjectsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFDataGetObjectsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFDataGetObjectsResponse* result{ nullptr };
            HRESULT hr = PFDataGetObjectsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFDataGetObjectsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFDataGetObjectsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFDataGetObjectsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFDataGetObjectsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFDataGetObjectsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFDataInitiateFileUploadsAsync(
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
            PFDataInitiateFileUploadsRequest request{};
            PFEntityKey entityKey{ state->entityId.c_str(), state->entityType.c_str() };
            request.entity = &entityKey;
            const char* fileNames[] = { "testfile.txt" };
            request.fileNames = fileNames;
            request.fileNamesCount = 1;
            const HRESULT hr = PFDataInitiateFileUploadsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFDataInitiateFileUploadsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFDataInitiateFileUploadsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFDataInitiateFileUploadsResponse* result{ nullptr };
            HRESULT hr = PFDataInitiateFileUploadsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFDataInitiateFileUploadsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFDataInitiateFileUploadsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFDataInitiateFileUploadsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFDataInitiateFileUploadsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFDataInitiateFileUploadsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFDataSetObjectsAsync(
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
            PFDataSetObjectsRequest request{};
            PFEntityKey entityKey{ state->entityId.c_str(), state->entityType.c_str() };
            request.entity = &entityKey;
            PFJsonObject dataObject{ "{\"key\":\"value\"}" };
            PFDataSetObject setObject{};
            setObject.dataObject = dataObject;
            setObject.objectName = "testObject";
            const PFDataSetObject* setObjectPtr = &setObject;
            request.objects = &setObjectPtr;
            request.objectsCount = 1;
            const HRESULT hr = PFDataSetObjectsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFDataSetObjectsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFDataSetObjectsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFDataSetObjectsResponse* result{ nullptr };
            HRESULT hr = PFDataSetObjectsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFDataSetObjectsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFDataSetObjectsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFDataSetObjectsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFDataSetObjectsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFDataSetObjectsGetResult: called inline by Async handler");
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "PFDataAbortFileUploadsAsync", HandlePFDataAbortFileUploadsAsync },
    { "PFDataAbortFileUploadsGetResult", HandlePFDataAbortFileUploadsGetResult },
    { "PFDataAbortFileUploadsGetResultSize", HandlePFDataAbortFileUploadsGetResultSize },
    { "PFDataDeleteFilesAsync", HandlePFDataDeleteFilesAsync },
    { "PFDataDeleteFilesGetResult", HandlePFDataDeleteFilesGetResult },
    { "PFDataDeleteFilesGetResultSize", HandlePFDataDeleteFilesGetResultSize },
    { "PFDataFinalizeFileUploadsAsync", HandlePFDataFinalizeFileUploadsAsync },
    { "PFDataFinalizeFileUploadsGetResult", HandlePFDataFinalizeFileUploadsGetResult },
    { "PFDataFinalizeFileUploadsGetResultSize", HandlePFDataFinalizeFileUploadsGetResultSize },
    { "PFDataGetFilesAsync", HandlePFDataGetFilesAsync },
    { "PFDataGetFilesGetResult", HandlePFDataGetFilesGetResult },
    { "PFDataGetFilesGetResultSize", HandlePFDataGetFilesGetResultSize },
    { "PFDataGetObjectsAsync", HandlePFDataGetObjectsAsync },
    { "PFDataGetObjectsGetResult", HandlePFDataGetObjectsGetResult },
    { "PFDataGetObjectsGetResultSize", HandlePFDataGetObjectsGetResultSize },
    { "PFDataInitiateFileUploadsAsync", HandlePFDataInitiateFileUploadsAsync },
    { "PFDataInitiateFileUploadsGetResult", HandlePFDataInitiateFileUploadsGetResult },
    { "PFDataInitiateFileUploadsGetResultSize", HandlePFDataInitiateFileUploadsGetResultSize },
    { "PFDataSetObjectsAsync", HandlePFDataSetObjectsAsync },
    { "PFDataSetObjectsGetResult", HandlePFDataSetObjectsGetResult },
    { "PFDataSetObjectsGetResultSize", HandlePFDataSetObjectsGetResultSize }
});

#include "pch.h"

#include "XPersistentLocalStorageHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <XPersistentLocalStorage.h>
#include "CommandRegistry.h"

CommandResultPayload HandleXPersistentLocalStorageGetPathSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            size_t pathSize = 0;
            const HRESULT hr = XPersistentLocalStorageGetPathSize(&pathSize);
            LogToWindowFormat("XPersistentLocalStorageGetPathSize (size=%zu, hr=0x%08X)", pathSize, static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["pathSize"] = pathSize;
            return S_OK;
        });
}

CommandResultPayload HandleXPersistentLocalStorageGetPath(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            size_t pathSize = 0;
            HRESULT hr = XPersistentLocalStorageGetPathSize(&pathSize);
            RETURN_IF_FAILED(hr);
            std::vector<char> pathBuffer(pathSize);
            size_t pathUsed = 0;
            hr = XPersistentLocalStorageGetPath(pathSize, pathBuffer.data(), &pathUsed);
            LogToWindowFormat("XPersistentLocalStorageGetPath (path=%s, hr=0x%08X)", pathBuffer.data(), static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["path"] = std::string(pathBuffer.data());
            return S_OK;
        });
}

CommandResultPayload HandleXPersistentLocalStorageGetSpaceInfo(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XPersistentLocalStorageSpaceInfo spaceInfo{};
            const HRESULT hr = XPersistentLocalStorageGetSpaceInfo(&spaceInfo);
            LogToWindowFormat("XPersistentLocalStorageGetSpaceInfo (available=%llu, total=%llu, used=%llu, hr=0x%08X)",
                static_cast<unsigned long long>(spaceInfo.availableFreeBytes),
                static_cast<unsigned long long>(spaceInfo.totalBytes),
                static_cast<unsigned long long>(spaceInfo.usedBytes),
                static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["availableFreeBytes"] = spaceInfo.availableFreeBytes;
            payload.result["totalFreeBytes"] = spaceInfo.totalFreeBytes;
            payload.result["usedBytes"] = spaceInfo.usedBytes;
            payload.result["totalBytes"] = spaceInfo.totalBytes;
            return S_OK;
        });
}

CommandResultPayload HandleXPersistentLocalStoragePromptUserForSpaceAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XPersistentLocalStoragePromptUserForSpaceAsync(1024 * 1024, &async);
            LogToWindowFormat("XPersistentLocalStoragePromptUserForSpaceAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XPersistentLocalStoragePromptUserForSpaceResult(&async);
            LogToWindowFormat("XPersistentLocalStoragePromptUserForSpaceResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXPersistentLocalStoragePromptUserForSpaceResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XPersistentLocalStoragePromptUserForSpaceResult: called inline by async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXPersistentLocalStorageMountForPackage(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            char packageId[XPACKAGE_IDENTIFIER_MAX_LENGTH]{};
            HRESULT hr = XPackageGetCurrentProcessPackageIdentifier(sizeof(packageId), packageId);
            RETURN_IF_FAILED(hr);
            XPackageMountHandle mount = nullptr;
            hr = XPersistentLocalStorageMountForPackage(packageId, &mount);
            LogToWindowFormat("XPersistentLocalStorageMountForPackage (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (mount) { XPackageCloseMountHandle(mount); }
            return hr;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XPersistentLocalStorageGetPath", HandleXPersistentLocalStorageGetPath },
    { "XPersistentLocalStorageGetPathSize", HandleXPersistentLocalStorageGetPathSize },
    { "XPersistentLocalStorageGetSpaceInfo", HandleXPersistentLocalStorageGetSpaceInfo },
    { "XPersistentLocalStorageMountForPackage", HandleXPersistentLocalStorageMountForPackage },
    { "XPersistentLocalStoragePromptUserForSpaceAsync", HandleXPersistentLocalStoragePromptUserForSpaceAsync },
    { "XPersistentLocalStoragePromptUserForSpaceResult", HandleXPersistentLocalStoragePromptUserForSpaceResult }
});

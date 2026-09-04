#include "pch.h"

#include "XPackageHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <XPackage.h>
#include "CommandRegistry.h"

CommandResultPayload HandleXPackageGetCurrentProcessPackageIdentifier(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            char buffer[XPACKAGE_IDENTIFIER_MAX_LENGTH]{};
            const HRESULT hr = XPackageGetCurrentProcessPackageIdentifier(sizeof(buffer), buffer);
            LogToWindowFormat("XPackageGetCurrentProcessPackageIdentifier (id=%s, hr=0x%08X)", buffer, static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["packageIdentifier"] = buffer;
            return S_OK;
        });
}

CommandResultPayload HandleXPackageIsPackagedProcess(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const bool result = XPackageIsPackagedProcess();
            LogToWindowFormat("XPackageIsPackagedProcess (result=%s)", result ? "true" : "false");
            payload.result["isPackaged"] = result;
            return S_OK;
        });
}

CommandResultPayload HandleXPackageCreateInstallationMonitor(
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
            XPackageInstallationMonitorHandle monitor = nullptr;
            hr = XPackageCreateInstallationMonitor(packageId, 0, nullptr, 0, state->taskQueue, &monitor);
            LogToWindowFormat("XPackageCreateInstallationMonitor (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (monitor) { XPackageCloseInstallationMonitorHandle(monitor); }
            return hr;
        });
}

CommandResultPayload HandleXPackageCloseInstallationMonitorHandle(
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
            XPackageInstallationMonitorHandle monitor = nullptr;
            hr = XPackageCreateInstallationMonitor(packageId, 0, nullptr, 0, state->taskQueue, &monitor);
            RETURN_IF_FAILED(hr);
            XPackageCloseInstallationMonitorHandle(monitor);
            LogToWindowFormat("XPackageCloseInstallationMonitorHandle (hr=0x%08X)", static_cast<uint32_t>(S_OK));
            return S_OK;
        });
}

CommandResultPayload HandleXPackageGetInstallationProgress(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            char packageId[XPACKAGE_IDENTIFIER_MAX_LENGTH]{};
            HRESULT hr = XPackageGetCurrentProcessPackageIdentifier(sizeof(packageId), packageId);
            RETURN_IF_FAILED(hr);
            XPackageInstallationMonitorHandle monitor = nullptr;
            hr = XPackageCreateInstallationMonitor(packageId, 0, nullptr, 0, state->taskQueue, &monitor);
            RETURN_IF_FAILED(hr);
            XPackageInstallationProgress progress{};
            XPackageGetInstallationProgress(monitor, &progress);
            LogToWindowFormat("XPackageGetInstallationProgress (completed=%s, launchable=%s)",
                progress.completed ? "true" : "false", progress.launchable ? "true" : "false");
            payload.result["completed"] = progress.completed;
            payload.result["launchable"] = progress.launchable;
            payload.result["totalBytes"] = progress.totalBytes;
            payload.result["installedBytes"] = progress.installedBytes;
            XPackageCloseInstallationMonitorHandle(monitor);
            return S_OK;
        });
}

CommandResultPayload HandleXPackageUpdateInstallationMonitor(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            char packageId[XPACKAGE_IDENTIFIER_MAX_LENGTH]{};
            HRESULT hr = XPackageGetCurrentProcessPackageIdentifier(sizeof(packageId), packageId);
            RETURN_IF_FAILED(hr);
            XPackageInstallationMonitorHandle monitor = nullptr;
            hr = XPackageCreateInstallationMonitor(packageId, 0, nullptr, 0, state->taskQueue, &monitor);
            RETURN_IF_FAILED(hr);
            const bool updated = XPackageUpdateInstallationMonitor(monitor);
            LogToWindowFormat("XPackageUpdateInstallationMonitor (updated=%s)", updated ? "true" : "false");
            payload.result["updated"] = updated;
            XPackageCloseInstallationMonitorHandle(monitor);
            return S_OK;
        });
}

CommandResultPayload HandleXPackageRegisterInstallationProgressChanged(
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
            XPackageInstallationMonitorHandle monitor = nullptr;
            hr = XPackageCreateInstallationMonitor(packageId, 0, nullptr, 1000, state->taskQueue, &monitor);
            RETURN_IF_FAILED(hr);
            XTaskQueueRegistrationToken token{};
            hr = XPackageRegisterInstallationProgressChanged(monitor, nullptr,
                [](void*, XPackageInstallationMonitorHandle) {}, &token);
            LogToWindowFormat("XPackageRegisterInstallationProgressChanged (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr)) { XPackageUnregisterInstallationProgressChanged(monitor, token, true); }
            XPackageCloseInstallationMonitorHandle(monitor);
            return hr;
        });
}

CommandResultPayload HandleXPackageUnregisterInstallationProgressChanged(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            char packageId[XPACKAGE_IDENTIFIER_MAX_LENGTH]{};
            HRESULT hr = XPackageGetCurrentProcessPackageIdentifier(sizeof(packageId), packageId);
            RETURN_IF_FAILED(hr);
            XPackageInstallationMonitorHandle monitor = nullptr;
            hr = XPackageCreateInstallationMonitor(packageId, 0, nullptr, 1000, state->taskQueue, &monitor);
            RETURN_IF_FAILED(hr);
            XTaskQueueRegistrationToken token{};
            hr = XPackageRegisterInstallationProgressChanged(monitor, nullptr,
                [](void*, XPackageInstallationMonitorHandle) {}, &token);
            RETURN_IF_FAILED(hr);
            const bool result = XPackageUnregisterInstallationProgressChanged(monitor, token, true);
            LogToWindowFormat("XPackageUnregisterInstallationProgressChanged (result=%s)", result ? "true" : "false");
            payload.result["unregistered"] = result;
            XPackageCloseInstallationMonitorHandle(monitor);
            return S_OK;
        });
}

CommandResultPayload HandleXPackageGetUserLocale(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            char locale[256]{};
            const HRESULT hr = XPackageGetUserLocale(sizeof(locale), locale);
            LogToWindowFormat("XPackageGetUserLocale (locale=%s, hr=0x%08X)", locale, static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["locale"] = locale;
            return S_OK;
        });
}

CommandResultPayload HandleXPackageFindChunkAvailability(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            char packageId[XPACKAGE_IDENTIFIER_MAX_LENGTH]{};
            HRESULT hr = XPackageGetCurrentProcessPackageIdentifier(sizeof(packageId), packageId);
            RETURN_IF_FAILED(hr);
            XPackageChunkAvailability availability{};
            hr = XPackageFindChunkAvailability(packageId, 0, nullptr, &availability);
            LogToWindowFormat("XPackageFindChunkAvailability (availability=%u, hr=0x%08X)",
                static_cast<uint32_t>(availability), static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["availability"] = static_cast<uint32_t>(availability);
            return S_OK;
        });
}

CommandResultPayload HandleXPackageEnumerateChunkAvailability(
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
            hr = XPackageEnumerateChunkAvailability(packageId, XPackageChunkSelectorType::Chunk, nullptr,
                [](void*, const XPackageChunkSelector*, XPackageChunkAvailability) -> bool { return true; });
            LogToWindowFormat("XPackageEnumerateChunkAvailability (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXPackageChangeChunkInstallOrder(
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
            hr = XPackageChangeChunkInstallOrder(packageId, 0, nullptr);
            LogToWindowFormat("XPackageChangeChunkInstallOrder (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXPackageInstallChunks(
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
            XPackageInstallationMonitorHandle monitor = nullptr;
            hr = XPackageInstallChunks(packageId, 0, nullptr, 0, true, state->taskQueue, &monitor);
            LogToWindowFormat("XPackageInstallChunks (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (monitor) { XPackageCloseInstallationMonitorHandle(monitor); }
            return hr;
        });
}

CommandResultPayload HandleXPackageInstallChunksAsync(
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
            char packageId[XPACKAGE_IDENTIFIER_MAX_LENGTH]{};
            HRESULT hr = XPackageGetCurrentProcessPackageIdentifier(sizeof(packageId), packageId);
            RETURN_IF_FAILED(hr);
            hr = XPackageInstallChunksAsync(packageId, 0, nullptr, 0, true, &async);
            LogToWindowFormat("XPackageInstallChunksAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            XPackageInstallationMonitorHandle monitor = nullptr;
            const HRESULT hr = XPackageInstallChunksResult(&async, &monitor);
            LogToWindowFormat("XPackageInstallChunksResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (monitor) { XPackageCloseInstallationMonitorHandle(monitor); }
            return hr;
        });
}

CommandResultPayload HandleXPackageInstallChunksResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XPackageInstallChunksResult: called inline by InstallChunksAsync handler");
            return S_OK;
        });
}

CommandResultPayload HandleXPackageEstimateDownloadSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            char packageId[XPACKAGE_IDENTIFIER_MAX_LENGTH]{};
            HRESULT hr = XPackageGetCurrentProcessPackageIdentifier(sizeof(packageId), packageId);
            RETURN_IF_FAILED(hr);
            uint64_t downloadSize = 0;
            bool shouldPrompt = false;
            hr = XPackageEstimateDownloadSize(packageId, 0, nullptr, &downloadSize, &shouldPrompt);
            LogToWindowFormat("XPackageEstimateDownloadSize (size=%llu, hr=0x%08X)",
                static_cast<unsigned long long>(downloadSize), static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["downloadSize"] = downloadSize;
            payload.result["shouldPresentUserConfirmation"] = shouldPrompt;
            return S_OK;
        });
}

CommandResultPayload HandleXPackageUninstallChunks(
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
            hr = XPackageUninstallChunks(packageId, 0, nullptr);
            LogToWindowFormat("XPackageUninstallChunks (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXPackageEnumeratePackages(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t count = 0;
            const HRESULT hr = XPackageEnumeratePackages(XPackageKind::Game, XPackageEnumerationScope::ThisAndRelated, &count,
                [](void* context, const XPackageDetails*) -> bool
                {
                    auto* c = static_cast<uint32_t*>(context);
                    ++(*c);
                    return true;
                });
            LogToWindowFormat("XPackageEnumeratePackages (count=%u, hr=0x%08X)", count, static_cast<uint32_t>(hr));
            payload.result["count"] = count;
            return hr;
        });
}

CommandResultPayload HandleXPackageRegisterPackageInstalled(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XTaskQueueRegistrationToken token{};
            const HRESULT hr = XPackageRegisterPackageInstalled(state->taskQueue, nullptr,
                [](void*, const XPackageDetails*) {}, &token);
            LogToWindowFormat("XPackageRegisterPackageInstalled (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr)) { XPackageUnregisterPackageInstalled(token, true); }
            return hr;
        });
}

CommandResultPayload HandleXPackageUnregisterPackageInstalled(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XTaskQueueRegistrationToken token{};
            HRESULT hr = XPackageRegisterPackageInstalled(state->taskQueue, nullptr,
                [](void*, const XPackageDetails*) {}, &token);
            RETURN_IF_FAILED(hr);
            const bool result = XPackageUnregisterPackageInstalled(token, true);
            LogToWindowFormat("XPackageUnregisterPackageInstalled (result=%s)", result ? "true" : "false");
            payload.result["unregistered"] = result;
            return S_OK;
        });
}

CommandResultPayload HandleXPackageEnumerateFeatures(
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
            hr = XPackageEnumerateFeatures(packageId, nullptr,
                [](void*, const XPackageFeature*) -> bool { return true; });
            LogToWindowFormat("XPackageEnumerateFeatures (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXPackageMountWithUiAsync(
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
            char packageId[XPACKAGE_IDENTIFIER_MAX_LENGTH]{};
            HRESULT hr = XPackageGetCurrentProcessPackageIdentifier(sizeof(packageId), packageId);
            RETURN_IF_FAILED(hr);
            hr = XPackageMountWithUiAsync(packageId, &async);
            LogToWindowFormat("XPackageMountWithUiAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            XPackageMountHandle mount = nullptr;
            const HRESULT hr = XPackageMountWithUiResult(&async, &mount);
            LogToWindowFormat("XPackageMountWithUiResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (mount) { XPackageCloseMountHandle(mount); }
            return hr;
        });
}

CommandResultPayload HandleXPackageMountWithUiResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XPackageMountWithUiResult: called inline by MountWithUiAsync handler");
            return S_OK;
        });
}

CommandResultPayload HandleXPackageGetMountPathSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            char packageId[XPACKAGE_IDENTIFIER_MAX_LENGTH]{};
            HRESULT hr = XPackageGetCurrentProcessPackageIdentifier(sizeof(packageId), packageId);
            RETURN_IF_FAILED(hr);
            XAsyncBlock mountAsync{};
            mountAsync.queue = state->taskQueue;
            hr = XPackageMountWithUiAsync(packageId, &mountAsync);
            if (SUCCEEDED(hr)) { hr = XAsyncGetStatus(&mountAsync, true); }
            XPackageMountHandle mount = nullptr;
            if (SUCCEEDED(hr)) { hr = XPackageMountWithUiResult(&mountAsync, &mount); }
            RETURN_IF_FAILED(hr);
            size_t pathSize = 0;
            hr = XPackageGetMountPathSize(mount, &pathSize);
            LogToWindowFormat("XPackageGetMountPathSize (pathSize=%zu, hr=0x%08X)", pathSize, static_cast<uint32_t>(hr));
            payload.result["pathSize"] = pathSize;
            XPackageCloseMountHandle(mount);
            return hr;
        });
}

CommandResultPayload HandleXPackageGetMountPath(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            char packageId[XPACKAGE_IDENTIFIER_MAX_LENGTH]{};
            HRESULT hr = XPackageGetCurrentProcessPackageIdentifier(sizeof(packageId), packageId);
            RETURN_IF_FAILED(hr);
            XAsyncBlock mountAsync{};
            mountAsync.queue = state->taskQueue;
            hr = XPackageMountWithUiAsync(packageId, &mountAsync);
            if (SUCCEEDED(hr)) { hr = XAsyncGetStatus(&mountAsync, true); }
            XPackageMountHandle mount = nullptr;
            if (SUCCEEDED(hr)) { hr = XPackageMountWithUiResult(&mountAsync, &mount); }
            RETURN_IF_FAILED(hr);
            size_t pathSize = 0;
            hr = XPackageGetMountPathSize(mount, &pathSize);
            if (SUCCEEDED(hr) && pathSize > 0)
            {
                std::vector<char> pathBuffer(pathSize);
                hr = XPackageGetMountPath(mount, pathSize, pathBuffer.data());
                LogToWindowFormat("XPackageGetMountPath (path=%s, hr=0x%08X)", pathBuffer.data(), static_cast<uint32_t>(hr));
                if (SUCCEEDED(hr)) { payload.result["path"] = std::string(pathBuffer.data()); }
            }
            else
            {
                LogToWindowFormat("XPackageGetMountPath: GetMountPathSize failed (hr=0x%08X)", static_cast<uint32_t>(hr));
            }
            XPackageCloseMountHandle(mount);
            return hr;
        });
}

CommandResultPayload HandleXPackageCloseMountHandle(
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
            XAsyncBlock mountAsync{};
            mountAsync.queue = state->taskQueue;
            hr = XPackageMountWithUiAsync(packageId, &mountAsync);
            if (SUCCEEDED(hr)) { hr = XAsyncGetStatus(&mountAsync, true); }
            XPackageMountHandle mount = nullptr;
            if (SUCCEEDED(hr)) { hr = XPackageMountWithUiResult(&mountAsync, &mount); }
            RETURN_IF_FAILED(hr);
            XPackageCloseMountHandle(mount);
            LogToWindowFormat("XPackageCloseMountHandle (hr=0x%08X)", static_cast<uint32_t>(S_OK));
            return S_OK;
        });
}

CommandResultPayload HandleXPackageGetWriteStats(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XPackageWriteStats stats{};
            const HRESULT hr = XPackageGetWriteStats(&stats);
            LogToWindowFormat("XPackageGetWriteStats (interval=%llu, budget=%llu, elapsed=%llu, written=%llu, hr=0x%08X)",
                static_cast<unsigned long long>(stats.interval), static_cast<unsigned long long>(stats.budget),
                static_cast<unsigned long long>(stats.elapsed), static_cast<unsigned long long>(stats.bytesWritten),
                static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["interval"] = stats.interval;
            payload.result["budget"] = stats.budget;
            payload.result["elapsed"] = stats.elapsed;
            payload.result["bytesWritten"] = stats.bytesWritten;
            return S_OK;
        });
}

CommandResultPayload HandleXPackageUninstallUWPInstance(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XPackageUninstallUWPInstance("DummyPackageName");
            LogToWindowFormat("XPackageUninstallUWPInstance (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXPackageUninstallPackage(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            // Skipping actual call to avoid uninstalling a real package
            const bool result = XPackageUninstallPackage("invalid-package-id");
            LogToWindowFormat("XPackageUninstallPackage (result=%s)", result ? "true" : "false");
            payload.result["uninstalled"] = result;
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XPackageChangeChunkInstallOrder", HandleXPackageChangeChunkInstallOrder },
    { "XPackageCloseInstallationMonitorHandle", HandleXPackageCloseInstallationMonitorHandle },
    { "XPackageCloseMountHandle", HandleXPackageCloseMountHandle },
    { "XPackageCreateInstallationMonitor", HandleXPackageCreateInstallationMonitor },
    { "XPackageEnumerateChunkAvailability", HandleXPackageEnumerateChunkAvailability },
    { "XPackageEnumerateFeatures", HandleXPackageEnumerateFeatures },
    { "XPackageEnumeratePackages", HandleXPackageEnumeratePackages },
    { "XPackageEstimateDownloadSize", HandleXPackageEstimateDownloadSize },
    { "XPackageFindChunkAvailability", HandleXPackageFindChunkAvailability },
    { "XPackageGetCurrentProcessPackageIdentifier", HandleXPackageGetCurrentProcessPackageIdentifier },
    { "XPackageGetInstallationProgress", HandleXPackageGetInstallationProgress },
    { "XPackageGetMountPath", HandleXPackageGetMountPath },
    { "XPackageGetMountPathSize", HandleXPackageGetMountPathSize },
    { "XPackageGetUserLocale", HandleXPackageGetUserLocale },
    { "XPackageGetWriteStats", HandleXPackageGetWriteStats },
    { "XPackageInstallChunks", HandleXPackageInstallChunks },
    { "XPackageInstallChunksAsync", HandleXPackageInstallChunksAsync },
    { "XPackageInstallChunksResult", HandleXPackageInstallChunksResult },
    { "XPackageIsPackagedProcess", HandleXPackageIsPackagedProcess },
    { "XPackageMountWithUiAsync", HandleXPackageMountWithUiAsync },
    { "XPackageMountWithUiResult", HandleXPackageMountWithUiResult },
    { "XPackageRegisterInstallationProgressChanged", HandleXPackageRegisterInstallationProgressChanged },
    { "XPackageRegisterPackageInstalled", HandleXPackageRegisterPackageInstalled },
    { "XPackageUninstallChunks", HandleXPackageUninstallChunks },
    { "XPackageUninstallPackage", HandleXPackageUninstallPackage },
    { "XPackageUninstallUWPInstance", HandleXPackageUninstallUWPInstance },
    { "XPackageUnregisterInstallationProgressChanged", HandleXPackageUnregisterInstallationProgressChanged },
    { "XPackageUnregisterPackageInstalled", HandleXPackageUnregisterPackageInstalled },
    { "XPackageUpdateInstallationMonitor", HandleXPackageUpdateInstallationMonitor }
});

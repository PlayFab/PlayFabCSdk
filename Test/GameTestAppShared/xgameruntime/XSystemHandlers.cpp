#include "pch.h"

#include "XSystemHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <XSystem.h>
#include "CommandRegistry.h"

CommandResultPayload HandleXSystemGetAnalyticsInfo(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const XSystemAnalyticsInfo info = XSystemGetAnalyticsInfo();
            LogToWindowFormat("XSystemGetAnalyticsInfo (family=%s, form=%s)", info.family, info.form);
            payload.result["family"] = info.family;
            payload.result["form"] = info.form;
            return S_OK;
        });
}

CommandResultPayload HandleXSystemGetConsoleId(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            char consoleId[XSystemConsoleIdBytes]{};
            size_t used = 0;
            const HRESULT hr = XSystemGetConsoleId(sizeof(consoleId), consoleId, &used);
            LogToWindowFormat("XSystemGetConsoleId (id=%s, hr=0x%08X)", consoleId, static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["consoleId"] = consoleId;
            return S_OK;
        });
}

CommandResultPayload HandleXSystemGetXboxLiveSandboxId(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            char sandboxId[XSystemXboxLiveSandboxIdMaxBytes]{};
            size_t used = 0;
            const HRESULT hr = XSystemGetXboxLiveSandboxId(sizeof(sandboxId), sandboxId, &used);
            LogToWindowFormat("XSystemGetXboxLiveSandboxId (id=%s, hr=0x%08X)", sandboxId, static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["sandboxId"] = sandboxId;
            return S_OK;
        });
}

CommandResultPayload HandleXSystemGetDeviceType(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const XSystemDeviceType deviceType = XSystemGetDeviceType();
            LogToWindowFormat("XSystemGetDeviceType (type=%u)", static_cast<uint32_t>(deviceType));
            payload.result["deviceType"] = static_cast<uint32_t>(deviceType);
            return S_OK;
        });
}

CommandResultPayload HandleXSystemGetAppSpecificDeviceId(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            char appDeviceId[XSystemAppSpecificDeviceIdBytes]{};
            size_t used = 0;
            const HRESULT hr = XSystemGetAppSpecificDeviceId(sizeof(appDeviceId), appDeviceId, &used);
            LogToWindowFormat("XSystemGetAppSpecificDeviceId (id=%s, hr=0x%08X)", appDeviceId, static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["appSpecificDeviceId"] = appDeviceId;
            return S_OK;
        });
}

CommandResultPayload HandleXSystemHandleTrack(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XSystemHandleTrack(
                [](XSystemHandle, XSystemHandleType, XSystemHandleCallbackReason, void*) {}, nullptr);
            LogToWindowFormat("XSystemHandleTrack (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXSystemIsHandleValid(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const bool result = XSystemIsHandleValid(nullptr);
            LogToWindowFormat("XSystemIsHandleValid (result=%s)", result ? "true" : "false");
            payload.result["isValid"] = result;
            return S_OK;
        });
}

CommandResultPayload HandleXSystemGetRuntimeInfo(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const XSystemRuntimeInfo info = XSystemGetRuntimeInfo();
            LogToWindowFormat("XSystemGetRuntimeInfo (runtime=%u.%u.%u.%u)",
                info.runtimeVersion.major, info.runtimeVersion.minor,
                info.runtimeVersion.build, info.runtimeVersion.revision);
            payload.result["runtimeMajor"] = info.runtimeVersion.major;
            payload.result["runtimeMinor"] = info.runtimeVersion.minor;
            payload.result["runtimeBuild"] = info.runtimeVersion.build;
            payload.result["runtimeRevision"] = info.runtimeVersion.revision;
            return S_OK;
        });
}

CommandResultPayload HandleXSystemAllowFullDownloadBandwidth(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XSystemAllowFullDownloadBandwidth(true);
            LogToWindow("XSystemAllowFullDownloadBandwidth called with enable=true");
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XSystemAllowFullDownloadBandwidth", HandleXSystemAllowFullDownloadBandwidth },
    { "XSystemGetAnalyticsInfo", HandleXSystemGetAnalyticsInfo },
    { "XSystemGetAppSpecificDeviceId", HandleXSystemGetAppSpecificDeviceId },
    { "XSystemGetConsoleId", HandleXSystemGetConsoleId },
    { "XSystemGetDeviceType", HandleXSystemGetDeviceType },
    { "XSystemGetRuntimeInfo", HandleXSystemGetRuntimeInfo },
    { "XSystemGetXboxLiveSandboxId", HandleXSystemGetXboxLiveSandboxId },
    { "XSystemHandleTrack", HandleXSystemHandleTrack },
    { "XSystemIsHandleValid", HandleXSystemIsHandleValid }
});

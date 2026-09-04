#include "pch.h"

#include "XDisplayHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <XDisplay.h>
#include "CommandRegistry.h"

CommandResultPayload HandleXDisplayTryEnableHdrMode(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XDisplayHdrModeInfo hdrInfo{};
            const XDisplayHdrModeResult result = XDisplayTryEnableHdrMode(XDisplayHdrModePreference::PreferHdr, &hdrInfo);
            LogToWindowFormat("XDisplayTryEnableHdrMode (result=%u, minLum=%.2f, maxLum=%.2f)",
                static_cast<uint32_t>(result), hdrInfo.minToneMapLuminance, hdrInfo.maxToneMapLuminance);
            payload.result["hdrModeResult"] = static_cast<uint32_t>(result);
            return S_OK;
        });
}

CommandResultPayload HandleXDisplayAcquireTimeoutDeferral(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XDisplayTimeoutDeferralHandle handle = nullptr;
            const HRESULT hr = XDisplayAcquireTimeoutDeferral(&handle);
            LogToWindowFormat("XDisplayAcquireTimeoutDeferral (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (handle) { XDisplayCloseTimeoutDeferralHandle(handle); }
            return hr;
        });
}

CommandResultPayload HandleXDisplayCloseTimeoutDeferralHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XDisplayTimeoutDeferralHandle handle = nullptr;
            const HRESULT hr = XDisplayAcquireTimeoutDeferral(&handle);
            RETURN_IF_FAILED(hr);
            XDisplayCloseTimeoutDeferralHandle(handle);
            LogToWindowFormat("XDisplayCloseTimeoutDeferralHandle (hr=0x%08X)", static_cast<uint32_t>(S_OK));
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XDisplayAcquireTimeoutDeferral", HandleXDisplayAcquireTimeoutDeferral },
    { "XDisplayCloseTimeoutDeferralHandle", HandleXDisplayCloseTimeoutDeferralHandle },
    { "XDisplayTryEnableHdrMode", HandleXDisplayTryEnableHdrMode }
});

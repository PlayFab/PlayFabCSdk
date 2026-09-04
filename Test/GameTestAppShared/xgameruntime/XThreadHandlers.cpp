#include "pch.h"

#include "XThreadHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <XThread.h>
#include "CommandRegistry.h"

CommandResultPayload HandleXThreadSetTimeSensitive(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XThreadSetTimeSensitive(false);
            LogToWindowFormat("XThreadSetTimeSensitive (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXThreadIsTimeSensitive(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const bool result = XThreadIsTimeSensitive();
            LogToWindowFormat("XThreadIsTimeSensitive (result=%s)", result ? "true" : "false");
            payload.result["isTimeSensitive"] = result;
            return S_OK;
        });
}

CommandResultPayload HandleXThreadAssertNotTimeSensitive(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XThreadAssertNotTimeSensitive();
            LogToWindow("XThreadAssertNotTimeSensitive called");
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XThreadAssertNotTimeSensitive", HandleXThreadAssertNotTimeSensitive },
    { "XThreadIsTimeSensitive", HandleXThreadIsTimeSensitive },
    { "XThreadSetTimeSensitive", HandleXThreadSetTimeSensitive }
});

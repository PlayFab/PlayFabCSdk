#include "pch.h"

#include "XLauncherHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <XLauncher.h>
#include "CommandRegistry.h"

CommandResultPayload HandleXLaunchUri(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XLaunchUri(state->xuser, "https://www.microsoft.com");
            LogToWindowFormat("XLaunchUri (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XLaunchUri", HandleXLaunchUri }
});

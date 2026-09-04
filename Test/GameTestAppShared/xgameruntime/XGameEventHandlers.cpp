#include "pch.h"

#include "XGameEventHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <XGameEvent.h>
#include "CommandRegistry.h"

CommandResultPayload HandleXGameEventWrite(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);
            const HRESULT hr = XGameEventWrite(
                state->xuser,
                "00000000-0000-0000-0000-000000000000",
                "00000000-0000-0000-0000-000000000000",
                "TestEvent",
                "{}",
                "{}");
            LogToWindowFormat("XGameEventWrite (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XGameEventWrite", HandleXGameEventWrite }
});

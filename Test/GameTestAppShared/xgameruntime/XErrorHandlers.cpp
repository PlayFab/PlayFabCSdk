#include "pch.h"

#include "XErrorHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <XError.h>
#include "CommandRegistry.h"

CommandResultPayload HandleXErrorSetCallback(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XErrorSetCallback(
                [](HRESULT hr, const char* msg, void*) -> bool
                {
                    LogToWindowFormat("XError: %s (hr=0x%08X)", msg, static_cast<uint32_t>(hr));
                    return true;
                },
                nullptr);
            LogToWindow("XErrorSetCallback configured");
            return S_OK;
        });
}

CommandResultPayload HandleXErrorSetOptions(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XErrorSetOptions(XErrorOptions::OutputDebugStringOnError, XErrorOptions::None);
            LogToWindow("XErrorSetOptions configured");
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XErrorSetCallback", HandleXErrorSetCallback },
    { "XErrorSetOptions", HandleXErrorSetOptions }
});

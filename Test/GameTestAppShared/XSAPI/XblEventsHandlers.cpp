#include "pch.h"

#include "XblEventsHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

CommandResultPayload HandleXblEventsWriteInGameEvent(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string eventName, error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "eventName", eventName, error))
            {
                eventName = "PuzzleSolved";
            }

            std::string dimensionsJson, measurementsJson;
            CommandHandlerShared::TryGetStringParameter(parameters, "dimensionsJson", dimensionsJson, error);
            CommandHandlerShared::TryGetStringParameter(parameters, "measurementsJson", measurementsJson, error);

            const HRESULT hr = XblEventsWriteInGameEvent(
                state->xblContext,
                eventName.c_str(),
                dimensionsJson.empty() ? nullptr : dimensionsJson.c_str(),
                measurementsJson.empty() ? nullptr : measurementsJson.c_str());
            LogToWindowFormat("XblEventsWriteInGameEvent (event=%s, hr=0x%08X)", eventName.c_str(), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblEventsSetStorageAllotment(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblEventsSetStorageAllotment: API not available in this build configuration");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandleXblEventsSetMaxFileSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblEventsSetMaxFileSize: API not available in this build configuration");
            return E_NOTIMPL;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XblEventsSetMaxFileSize", HandleXblEventsSetMaxFileSize },
    { "XblEventsSetStorageAllotment", HandleXblEventsSetStorageAllotment },
    { "XblEventsWriteInGameEvent", HandleXblEventsWriteInGameEvent }
});

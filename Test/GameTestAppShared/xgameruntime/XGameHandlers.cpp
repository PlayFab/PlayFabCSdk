#include "pch.h"

#include "XGameHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <XGame.h>
#include "CommandRegistry.h"

CommandResultPayload HandleXGameGetXboxTitleId(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t titleId = 0;
            const HRESULT hr = XGameGetXboxTitleId(&titleId);
            LogToWindowFormat("XGameGetXboxTitleId (titleId=0x%08X, hr=0x%08X)", titleId, static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["titleId"] = titleId;
            return S_OK;
        });
}

CommandResultPayload HandleXLaunchNewGame(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            // XLaunchNewGame never returns - skipping to avoid terminating test process
            LogToWindow("XLaunchNewGame: skipped to avoid terminating test process");
            payload.result["skipped"] = true;
            return S_OK;
        });
}

CommandResultPayload HandleXLaunchRestartOnCrash(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XLaunchRestartOnCrash(nullptr, 0);
            LogToWindowFormat("XLaunchRestartOnCrash (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XGameGetXboxTitleId", HandleXGameGetXboxTitleId },
    { "XLaunchNewGame", HandleXLaunchNewGame },
    { "XLaunchRestartOnCrash", HandleXLaunchRestartOnCrash }
});

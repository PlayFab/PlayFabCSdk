#include "pch.h"

#include "XblGameInviteHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

CommandResultPayload HandleXblGameInviteAddNotificationHandler(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblGameInviteAddNotificationHandler: API not available in this build configuration");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandleXblGameInviteRemoveNotificationHandler(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblGameInviteRemoveNotificationHandler: API not available in this build configuration");
            return E_NOTIMPL;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XblGameInviteAddNotificationHandler", HandleXblGameInviteAddNotificationHandler },
    { "XblGameInviteRemoveNotificationHandler", HandleXblGameInviteRemoveNotificationHandler }
});

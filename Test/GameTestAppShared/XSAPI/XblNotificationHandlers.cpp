#include "pch.h"

#include "XblNotificationHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

CommandResultPayload HandleXblNotificationSubscribeToNotificationsAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblNotificationSubscribeToNotificationsAsync: API not available in this build configuration");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandleXblNotificationUnsubscribeFromNotificationsAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblNotificationUnsubscribeFromNotificationsAsync: API not available in this build configuration");
            return E_NOTIMPL;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XblNotificationSubscribeToNotificationsAsync", HandleXblNotificationSubscribeToNotificationsAsync },
    { "XblNotificationUnsubscribeFromNotificationsAsync", HandleXblNotificationUnsubscribeFromNotificationsAsync }
});

#include "pch.h"
#include "PFPushNotificationsHandlers.h"
#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <playfab/services/PFPushNotifications.h>
#include <playfab/core/PFLocalUser.h>
#include <vector>
#include "CommandRegistry.h"

using CommandHandlerShared::TryGetStringParameter;
using CommandHandlerShared::TryParseBoolParameter;

CommandResultPayload HandlePFPushNotificationsServerSendPushNotificationAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFPushNotificationsSendPushNotificationRequest request{};
            const HRESULT hr = PFPushNotificationsServerSendPushNotificationAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPushNotificationsServerSendPushNotificationAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFPushNotificationsServerSendPushNotificationFromTemplateAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFPushNotificationsSendPushNotificationFromTemplateRequest request{};
            const HRESULT hr = PFPushNotificationsServerSendPushNotificationFromTemplateAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFPushNotificationsServerSendPushNotificationFromTemplateAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "PFPushNotificationsServerSendPushNotificationAsync", HandlePFPushNotificationsServerSendPushNotificationAsync },
    { "PFPushNotificationsServerSendPushNotificationFromTemplateAsync", HandlePFPushNotificationsServerSendPushNotificationFromTemplateAsync }
});

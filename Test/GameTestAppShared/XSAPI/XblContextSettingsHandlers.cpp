#include "pch.h"

#include "XblContextSettingsHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

CommandResultPayload HandleXblContextDuplicateHandle(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);
            XblContextHandle duplicatedHandle{ nullptr };
            const HRESULT hr = XblContextDuplicateHandle(state->xblContext, &duplicatedHandle);
            LogToWindowFormat("XblContextDuplicateHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                XblContextCloseHandle(state->xblContext);
                state->xblContext = duplicatedHandle;
            }
            return hr;
        });
}

CommandResultPayload HandleXblContextGetUser(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);
            XblUserHandle user{ nullptr };
            const HRESULT hr = XblContextGetUser(state->xblContext, &user);
            LogToWindowFormat("XblContextGetUser (user=%s, hr=0x%08X)", user ? "non-null" : "null", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["userIsNonNull"] = (user != nullptr);
            }
            return hr;
        });
}

CommandResultPayload HandleXblContextGetXboxUserId(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);
            uint64_t xboxUserId{};
            const HRESULT hr = XblContextGetXboxUserId(state->xblContext, &xboxUserId);
            LogToWindowFormat("XblContextGetXboxUserId (xuid=%llu, hr=0x%08X)", xboxUserId, static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["xboxUserId"] = xboxUserId;
            }
            return hr;
        });
}

CommandResultPayload HandleXblContextSettingsGetLongHttpTimeout(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);
            uint32_t timeoutInSeconds{};
            const HRESULT hr = XblContextSettingsGetLongHttpTimeout(state->xblContext, &timeoutInSeconds);
            LogToWindowFormat("XblContextSettingsGetLongHttpTimeout (timeout=%u, hr=0x%08X)", timeoutInSeconds, static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["timeoutInSeconds"] = timeoutInSeconds;
            }
            return hr;
        });
}

CommandResultPayload HandleXblContextSettingsSetLongHttpTimeout(
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
            int64_t val{};
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "timeoutInSeconds", val, error))
            {
                val = 180;
            }
            const HRESULT hr = XblContextSettingsSetLongHttpTimeout(state->xblContext, static_cast<uint32_t>(val));
            LogToWindowFormat("XblContextSettingsSetLongHttpTimeout (timeout=%u, hr=0x%08X)", static_cast<uint32_t>(val), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblContextSettingsGetHttpRetryDelay(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);
            uint32_t delayInSeconds{};
            const HRESULT hr = XblContextSettingsGetHttpRetryDelay(state->xblContext, &delayInSeconds);
            LogToWindowFormat("XblContextSettingsGetHttpRetryDelay (delay=%u, hr=0x%08X)", delayInSeconds, static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["delayInSeconds"] = delayInSeconds;
            }
            return hr;
        });
}

CommandResultPayload HandleXblContextSettingsSetHttpRetryDelay(
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
            int64_t val{};
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "delayInSeconds", val, error))
            {
                val = 2;
            }
            const HRESULT hr = XblContextSettingsSetHttpRetryDelay(state->xblContext, static_cast<uint32_t>(val));
            LogToWindowFormat("XblContextSettingsSetHttpRetryDelay (delay=%u, hr=0x%08X)", static_cast<uint32_t>(val), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblContextSettingsGetHttpTimeoutWindow(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);
            uint32_t timeoutWindowInSeconds{};
            const HRESULT hr = XblContextSettingsGetHttpTimeoutWindow(state->xblContext, &timeoutWindowInSeconds);
            LogToWindowFormat("XblContextSettingsGetHttpTimeoutWindow (timeoutWindow=%u, hr=0x%08X)", timeoutWindowInSeconds, static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["timeoutWindowInSeconds"] = timeoutWindowInSeconds;
            }
            return hr;
        });
}

CommandResultPayload HandleXblContextSettingsSetHttpTimeoutWindow(
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
            int64_t val{};
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "timeoutWindowInSeconds", val, error))
            {
                val = 20;
            }
            const HRESULT hr = XblContextSettingsSetHttpTimeoutWindow(state->xblContext, static_cast<uint32_t>(val));
            LogToWindowFormat("XblContextSettingsSetHttpTimeoutWindow (timeoutWindow=%u, hr=0x%08X)", static_cast<uint32_t>(val), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblContextSettingsGetWebsocketTimeoutWindow(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);
            uint32_t timeoutWindowInSeconds{};
            const HRESULT hr = XblContextSettingsGetWebsocketTimeoutWindow(state->xblContext, &timeoutWindowInSeconds);
            LogToWindowFormat("XblContextSettingsGetWebsocketTimeoutWindow (timeoutWindow=%u, hr=0x%08X)", timeoutWindowInSeconds, static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["timeoutWindowInSeconds"] = timeoutWindowInSeconds;
            }
            return hr;
        });
}

CommandResultPayload HandleXblContextSettingsSetWebsocketTimeoutWindow(
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
            int64_t val{};
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "timeoutWindowInSeconds", val, error))
            {
                val = 300;
            }
            const HRESULT hr = XblContextSettingsSetWebsocketTimeoutWindow(state->xblContext, static_cast<uint32_t>(val));
            LogToWindowFormat("XblContextSettingsSetWebsocketTimeoutWindow (timeoutWindow=%u, hr=0x%08X)", static_cast<uint32_t>(val), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblContextSettingsGetUseCrossPlatformQosServers(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);
            bool value{};
            const HRESULT hr = XblContextSettingsGetUseCrossPlatformQosServers(state->xblContext, &value);
            LogToWindowFormat("XblContextSettingsGetUseCrossPlatformQosServers (value=%s, hr=0x%08X)", value ? "true" : "false", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["value"] = value;
            }
            return hr;
        });
}

CommandResultPayload HandleXblContextSettingsSetUseCrossPlatformQosServers(
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
            bool value{};
            std::string error;
            if (!CommandHandlerShared::TryParseBoolParameter(parameters, "value", value, error))
            {
                value = true;
            }
            const HRESULT hr = XblContextSettingsSetUseCrossPlatformQosServers(state->xblContext, value);
            LogToWindowFormat("XblContextSettingsSetUseCrossPlatformQosServers (value=%s, hr=0x%08X)", value ? "true" : "false", static_cast<uint32_t>(hr));
            return hr;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XblContextDuplicateHandle", HandleXblContextDuplicateHandle },
    { "XblContextGetUser", HandleXblContextGetUser },
    { "XblContextGetXboxUserId", HandleXblContextGetXboxUserId },
    { "XblContextSettingsGetHttpRetryDelay", HandleXblContextSettingsGetHttpRetryDelay },
    { "XblContextSettingsGetHttpTimeoutWindow", HandleXblContextSettingsGetHttpTimeoutWindow },
    { "XblContextSettingsGetLongHttpTimeout", HandleXblContextSettingsGetLongHttpTimeout },
    { "XblContextSettingsGetUseCrossPlatformQosServers", HandleXblContextSettingsGetUseCrossPlatformQosServers },
    { "XblContextSettingsGetWebsocketTimeoutWindow", HandleXblContextSettingsGetWebsocketTimeoutWindow },
    { "XblContextSettingsSetHttpRetryDelay", HandleXblContextSettingsSetHttpRetryDelay },
    { "XblContextSettingsSetHttpTimeoutWindow", HandleXblContextSettingsSetHttpTimeoutWindow },
    { "XblContextSettingsSetLongHttpTimeout", HandleXblContextSettingsSetLongHttpTimeout },
    { "XblContextSettingsSetUseCrossPlatformQosServers", HandleXblContextSettingsSetUseCrossPlatformQosServers },
    { "XblContextSettingsSetWebsocketTimeoutWindow", HandleXblContextSettingsSetWebsocketTimeoutWindow }
});

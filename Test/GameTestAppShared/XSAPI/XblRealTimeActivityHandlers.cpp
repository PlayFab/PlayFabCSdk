#include "pch.h"

#include "XblRealTimeActivityHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

static void CALLBACK OnConnectionStateChanged(void* /*context*/, XblRealTimeActivityConnectionState connectionState)
{
    LogToWindowFormat("RTA connection state changed: %d", static_cast<int>(connectionState));
}

static void CALLBACK OnResync(void* /*context*/)
{
    LogToWindow("RTA resync notification received");
}

CommandResultPayload HandleXblRealTimeActivityAddConnectionStateChangeHandler(
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
            XblFunctionContext token = XblRealTimeActivityAddConnectionStateChangeHandler(
                state->xblContext, OnConnectionStateChanged, nullptr);
            payload.result["handlerToken"] = static_cast<int64_t>(token);
            LogToWindowFormat("XblRealTimeActivityAddConnectionStateChangeHandler (token=%lld)", static_cast<long long>(token));
            return S_OK;
        });
}

CommandResultPayload HandleXblRealTimeActivityRemoveConnectionStateChangeHandler(
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
            int64_t token{};
            std::string error;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "token", token, error);

            XblRealTimeActivityRemoveConnectionStateChangeHandler(
                state->xblContext, static_cast<XblFunctionContext>(token));
            LogToWindowFormat("XblRealTimeActivityRemoveConnectionStateChangeHandler (token=%lld)",
                static_cast<long long>(token));
            return S_OK;
        });
}

CommandResultPayload HandleXblRealTimeActivityAddResyncHandler(
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
            XblFunctionContext token = XblRealTimeActivityAddResyncHandler(
                state->xblContext, OnResync, nullptr);
            payload.result["handlerToken"] = static_cast<int64_t>(token);
            LogToWindowFormat("XblRealTimeActivityAddResyncHandler (token=%lld)", static_cast<long long>(token));
            return S_OK;
        });
}

CommandResultPayload HandleXblRealTimeActivityRemoveResyncHandler(
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
            int64_t token{};
            std::string error;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "token", token, error);

            XblRealTimeActivityRemoveResyncHandler(
                state->xblContext, static_cast<XblFunctionContext>(token));
            LogToWindowFormat("XblRealTimeActivityRemoveResyncHandler (token=%lld)",
                static_cast<long long>(token));
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XblRealTimeActivityAddConnectionStateChangeHandler", HandleXblRealTimeActivityAddConnectionStateChangeHandler },
    { "XblRealTimeActivityAddResyncHandler", HandleXblRealTimeActivityAddResyncHandler },
    { "XblRealTimeActivityRemoveConnectionStateChangeHandler", HandleXblRealTimeActivityRemoveConnectionStateChangeHandler },
    { "XblRealTimeActivityRemoveResyncHandler", HandleXblRealTimeActivityRemoveResyncHandler }
});

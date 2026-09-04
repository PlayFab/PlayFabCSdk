#include "pch.h"

#include "XblLocalStorageHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

CommandResultPayload HandleXblLocalStorageSetHandlers(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblLocalStorageSetHandlers: API not available in this build configuration");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandleXblLocalStorageWriteComplete(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblLocalStorageWriteComplete: API not available in this build configuration");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandleXblLocalStorageReadComplete(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblLocalStorageReadComplete: API not available in this build configuration");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandleXblLocalStorageClearComplete(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblLocalStorageClearComplete: API not available in this build configuration");
            return E_NOTIMPL;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XblLocalStorageClearComplete", HandleXblLocalStorageClearComplete },
    { "XblLocalStorageReadComplete", HandleXblLocalStorageReadComplete },
    { "XblLocalStorageSetHandlers", HandleXblLocalStorageSetHandlers },
    { "XblLocalStorageWriteComplete", HandleXblLocalStorageWriteComplete }
});

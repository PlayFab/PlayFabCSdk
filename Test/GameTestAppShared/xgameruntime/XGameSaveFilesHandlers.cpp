#include "pch.h"

#include "XGameSaveFilesHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <XGameSaveFiles.h>
#include "CommandRegistry.h"

CommandResultPayload HandleXGameSaveFilesGetFolderWithUiAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);
            const HRESULT hr = XGameSaveFilesGetFolderWithUiAsync(state->xuser, "00000000-0000-0000-0000-000000000000", &async);
            LogToWindowFormat("XGameSaveFilesGetFolderWithUiAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            char folder[1024]{};
            const HRESULT hr = XGameSaveFilesGetFolderWithUiResult(&async, sizeof(folder), folder);
            LogToWindowFormat("XGameSaveFilesGetFolderWithUiResult (folder=%s, hr=0x%08X)", folder, static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr)) { payload.result["folder"] = folder; }
            return hr;
        });
}

CommandResultPayload HandleXGameSaveFilesGetFolderWithUiResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XGameSaveFilesGetFolderWithUiResult: called inline by GetFolderWithUiAsync handler");
            return S_OK;
        });
}

CommandResultPayload HandleXGameSaveFilesGetRemainingQuota(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);
            int64_t remainingQuota = 0;
            const HRESULT hr = XGameSaveFilesGetRemainingQuota(state->xuser, "00000000-0000-0000-0000-000000000000", &remainingQuota);
            LogToWindowFormat("XGameSaveFilesGetRemainingQuota (quota=%lld, hr=0x%08X)",
                static_cast<long long>(remainingQuota), static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["remainingQuota"] = remainingQuota;
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XGameSaveFilesGetFolderWithUiAsync", HandleXGameSaveFilesGetFolderWithUiAsync },
    { "XGameSaveFilesGetFolderWithUiResult", HandleXGameSaveFilesGetFolderWithUiResult },
    { "XGameSaveFilesGetRemainingQuota", HandleXGameSaveFilesGetRemainingQuota }
});

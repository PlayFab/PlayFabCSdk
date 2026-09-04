#include "pch.h"
#include "PFExperimentationHandlers.h"
#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <playfab/services/PFExperimentation.h>
#include <playfab/core/PFLocalUser.h>
#include <vector>
#include "CommandRegistry.h"

using CommandHandlerShared::TryGetStringParameter;
using CommandHandlerShared::TryParseBoolParameter;

CommandResultPayload HandlePFExperimentationGetTreatmentAssignmentAsync(
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
            PFExperimentationGetTreatmentAssignmentRequest request{};
            const HRESULT hr = PFExperimentationGetTreatmentAssignmentAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFExperimentationGetTreatmentAssignmentAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFExperimentationGetTreatmentAssignmentGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFExperimentationGetTreatmentAssignmentResult* result{ nullptr };
            HRESULT hr = PFExperimentationGetTreatmentAssignmentGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFExperimentationGetTreatmentAssignmentAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFExperimentationGetTreatmentAssignmentGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFExperimentationGetTreatmentAssignmentGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFExperimentationGetTreatmentAssignmentGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFExperimentationGetTreatmentAssignmentGetResult: called inline by Async handler");
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "PFExperimentationGetTreatmentAssignmentAsync", HandlePFExperimentationGetTreatmentAssignmentAsync },
    { "PFExperimentationGetTreatmentAssignmentGetResult", HandlePFExperimentationGetTreatmentAssignmentGetResult },
    { "PFExperimentationGetTreatmentAssignmentGetResultSize", HandlePFExperimentationGetTreatmentAssignmentGetResultSize }
});

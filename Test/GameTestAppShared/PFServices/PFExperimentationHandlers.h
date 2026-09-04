#pragma once
#include "CommandHandlerShared.h"

struct DeviceGameSaveState;

CommandResultPayload HandlePFExperimentationGetTreatmentAssignmentAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFExperimentationGetTreatmentAssignmentGetResultSize(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFExperimentationGetTreatmentAssignmentGetResult(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);

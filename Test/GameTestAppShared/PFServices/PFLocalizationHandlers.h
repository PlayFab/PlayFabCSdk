#pragma once
#include "CommandHandlerShared.h"

struct DeviceGameSaveState;

CommandResultPayload HandlePFLocalizationGetLanguageListAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLocalizationGetLanguageListGetResultSize(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLocalizationGetLanguageListGetResult(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);

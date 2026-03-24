#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "DeviceCommandHandlers.h"

struct DeviceGameSaveState;

struct WriteGameSaveDataResult
{
    HRESULT hr{ S_OK };
    std::string errorMessage;
    std::vector<nlohmann::json> mutations;
    int chaosMutationsApplied{ 0 };
    int scriptMutationsApplied{ 0 };
    std::string saveFolder;
};

WriteGameSaveDataResult ExecuteWriteGameSaveData(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const nlohmann::json& parameters);

CommandResultPayload HandleWriteGameSaveData(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

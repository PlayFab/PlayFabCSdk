#pragma once

#include <string>

#include "DeviceCommandHandlers.h"

struct DeviceGameSaveState;

struct GetDebugStatsResult
{
    HRESULT hr{ S_OK };
    std::string errorMessage;
    std::string deviceName;
    std::string statsJson;
};

GetDebugStatsResult ExecuteGetDebugStats(DeviceGameSaveState* state);

CommandResultPayload HandleGetDebugStats(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

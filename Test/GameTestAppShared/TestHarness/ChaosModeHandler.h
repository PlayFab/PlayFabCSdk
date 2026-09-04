#pragma once

#include "DeviceCommandHandlers.h"

CommandResultPayload HandleDoChaosMode(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

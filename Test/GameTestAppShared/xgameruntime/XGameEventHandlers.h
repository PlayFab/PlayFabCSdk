#pragma once

#include "DeviceCommandHandlers.h"

CommandResultPayload HandleXGameEventWrite(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

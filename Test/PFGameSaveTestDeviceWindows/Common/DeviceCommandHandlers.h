// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

#include <string>

#include <nlohmann/json.hpp>

struct DeviceGameSaveState;

struct CommandResultPayload
{
    nlohmann::json result;
    int elapsedMs{ 0 };
};

CommandResultPayload BuildActionResult(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

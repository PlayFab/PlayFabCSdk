// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

#include "DeviceCommandHandlers.h"

CommandResultPayload HandleBeginOfflineBatch(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

CommandResultPayload HandleGetOfflineBatchResults(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

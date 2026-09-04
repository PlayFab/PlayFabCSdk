#pragma once

#include "DeviceCommandHandlers.h"

CommandResultPayload HandleXblLocalStorageSetHandlers(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

CommandResultPayload HandleXblLocalStorageWriteComplete(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

CommandResultPayload HandleXblLocalStorageReadComplete(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

CommandResultPayload HandleXblLocalStorageClearComplete(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

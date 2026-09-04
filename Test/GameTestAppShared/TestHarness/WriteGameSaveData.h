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

CommandResultPayload HandleVerifyFileExists(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

CommandResultPayload HandleVerifyFileContent(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

// Background ring-buffer writer: simulates periodic autosave churn by cycling
// 1MB single-byte payloads through a fixed set of ring files on a background
// thread, so writes can be in-flight when a suspend/PLM/power event lands.
CommandResultPayload HandleStartRingBufferWriter(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

// Stops the writer (if running), joins it, and returns the authoritative
// ring manifest (per-slot last byte + size) used as the data-loss ground truth.
CommandResultPayload HandleStopRingBufferWriter(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

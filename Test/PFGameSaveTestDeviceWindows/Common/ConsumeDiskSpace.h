#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "DeviceCommandHandlers.h"

struct DeviceGameSaveState;

struct ConsumeDiskSpaceResult
{
    HRESULT hr{ S_OK };
    std::string errorMessage;
    std::string scope;
    std::filesystem::path resolvedPath;
    int64_t bytesRequested{ 0 };
    int64_t bytesAllocated{ 0 };
    int filesCreated{ 0 };
    int directoriesCreated{ 0 };
    std::string token;
    std::vector<std::string> notes;
    std::vector<std::filesystem::path> fillerFiles;
};

ConsumeDiskSpaceResult ExecuteConsumeDiskSpace(
    DeviceGameSaveState* state,
    int64_t bytes,
    std::string scope,
    std::string customPath,
    std::string token);

struct ReleaseDiskSpaceResult
{
    HRESULT hr{ S_OK };
    std::string errorMessage;
    std::string scope;
    std::string token;
    int filesRemoved{ 0 };
    int directoriesRemoved{ 0 };
    int64_t bytesFreed{ 0 };
    std::vector<std::string> residualFiles;
    std::vector<std::string> notes;
};

ReleaseDiskSpaceResult ExecuteReleaseDiskSpace(
    DeviceGameSaveState* state,
    std::string scope,
    std::string token);

CommandResultPayload HandleConsumeDiskSpace(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

CommandResultPayload HandleReleaseDiskSpace(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

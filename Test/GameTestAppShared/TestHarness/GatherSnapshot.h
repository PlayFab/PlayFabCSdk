#pragma once

#include <cstdint>
#include <string>

#include "DeviceCommandHandlers.h"

struct DeviceGameSaveState;

struct GatherSnapshotResult
{
    HRESULT hr{ S_OK };
    std::string errorMessage;
    std::string deviceName;
    std::string saveFolder;
    std::string archivePath;
    std::string archiveFileName;
    std::uint64_t archiveSize{ 0 };
    std::uint64_t bytesReturned{ 0 };
    bool truncated{ false };
    std::string base64Content;
};

GatherSnapshotResult ExecuteGatherSnapshot(DeviceGameSaveState* state);

CommandResultPayload HandleGatherSnapshot(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

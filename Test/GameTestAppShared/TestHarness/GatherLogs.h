#pragma once

#include <cstdint>
#include <string>

#include "DeviceCommandHandlers.h"

struct DeviceGameSaveState;

struct GatherLogsResult
{
    HRESULT hr{ S_OK };
    std::string errorMessage;
    std::string deviceName;
    std::string logPath;
    std::string logFileName;
    std::uint64_t fileSize{ 0 };
    std::uint64_t bytesToTransfer{ 0 };
    std::uint64_t fileStartOffset{ 0 };
    bool truncated{ false };
    std::string summaryLogPath;
    std::string summaryLogFileName;
    std::uint64_t summaryFileSize{ 0 };
    std::uint64_t summaryBytesToTransfer{ 0 };
    std::uint64_t summaryFileStartOffset{ 0 };
    bool summaryTruncated{ false };
};

GatherLogsResult ExecuteGatherLogs(DeviceGameSaveState* state);

CommandResultPayload HandleGatherLogs(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

CommandResultPayload HandleGatherLogsChunk(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

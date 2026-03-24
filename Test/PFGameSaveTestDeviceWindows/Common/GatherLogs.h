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
    std::uint64_t bytesReturned{ 0 };
    bool truncated{ false };
    std::string logContent;
    std::string summaryLogPath;
    std::string summaryLogFileName;
    std::uint64_t summaryFileSize{ 0 };
    std::uint64_t summaryBytesReturned{ 0 };
    bool summaryTruncated{ false };
    std::string summaryLogContent;
};

GatherLogsResult ExecuteGatherLogs(DeviceGameSaveState* state);

CommandResultPayload HandleGatherLogs(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

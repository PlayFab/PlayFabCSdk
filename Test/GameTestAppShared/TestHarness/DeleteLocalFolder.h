#pragma once

#include <cstdint>
#include <string>

#include "DeviceCommandHandlers.h"

struct DeleteLocalFolderResult
{
    HRESULT hr{ S_OK };
    std::string errorMessage;
    std::string folderPath;
    std::uintmax_t entriesRemoved{ 0 };
    bool folderExisted{ false };
    bool folderRemoved{ false };
};

DeleteLocalFolderResult ExecuteDeleteLocalFolder(
    std::string folderPath);

CommandResultPayload HandleDeleteLocalFolder(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

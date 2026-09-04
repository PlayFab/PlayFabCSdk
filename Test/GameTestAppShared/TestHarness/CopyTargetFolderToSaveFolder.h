#pragma once

#include <cstdint>
#include <string>

#include "DeviceCommandHandlers.h"

struct CopyTargetFolderToSaveFolderResult
{
    HRESULT hr{ S_OK };
    std::string errorMessage;
    std::string sourceFolder;
    std::string destinationFolder;
    std::uintmax_t filesCopied{ 0 };
    std::uintmax_t directoriesCreated{ 0 };
    std::uintmax_t totalBytesCopied{ 0 };
};

CopyTargetFolderToSaveFolderResult ExecuteCopyTargetFolderToSaveFolder(
    DeviceGameSaveState* state,
    std::string sourceFolder);

CommandResultPayload HandleCopyTargetFolderToSaveFolder(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

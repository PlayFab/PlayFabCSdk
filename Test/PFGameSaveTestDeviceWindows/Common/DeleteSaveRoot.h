#pragma once

#include <string>
#include <vector>

#include "DeviceCommandHandlers.h"

struct DeviceGameSaveState;

struct DeleteSaveRootResult
{
    HRESULT hr{ S_OK };
    std::string errorMessage;
    int filesDeleted{ 0 };
    int directoriesDeleted{ 0 };
    bool manifestPreserved{ false };
    std::vector<std::string> preservedEntries;
    std::string saveFolder;
};

DeleteSaveRootResult ExecuteDeleteSaveRoot(
    DeviceGameSaveState* state,
    bool preserveManifest);

CommandResultPayload HandleDeleteSaveRoot(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "DeviceCommandHandlers.h"

struct DeviceGameSaveState;

struct CaptureSaveContainerSnapshotResult
{
    HRESULT hr{ S_OK };
    std::string errorMessage;
    std::string saveFolder;
    bool includeHashes{ true };
    int filesCaptured{ 0 };
    int directoriesCaptured{ 0 };
    int cloudsyncEntriesSkipped{ 0 };
    uint64_t totalBytes{ 0 };
    std::vector<std::string> skippedEntries;
    nlohmann::json snapshot;
};

CaptureSaveContainerSnapshotResult ExecuteCaptureSaveContainerSnapshot(
    DeviceGameSaveState* state,
    bool includeHashes);

CommandResultPayload HandleCaptureSaveContainerSnapshot(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

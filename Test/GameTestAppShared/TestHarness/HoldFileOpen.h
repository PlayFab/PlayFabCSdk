#pragma once

#include <string>

#include "DeviceCommandHandlers.h"

// Test-only control over local file availability.
//
// Holds a save file open with a restrictive share mode so the SDK's upload path meets the same
// condition a real title creates when it rewrites a save (or its thumbnail) while an upload is in
// flight - the trigger behind bug 4544470, where the upload surfaced 0x80070057 E_INVALIDARG.
//
// Locks are process-wide and released by ReleaseHeldFiles or at app teardown, so a scenario that
// fails mid-way cannot leave a file wedged for later scenarios.

CommandResultPayload HandleHoldFileOpen(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

CommandResultPayload HandleReleaseHeldFiles(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

// Releases every outstanding lock. Safe to call when none are held.
void ReleaseAllHeldFiles();

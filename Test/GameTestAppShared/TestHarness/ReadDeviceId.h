#pragma once

#include <string>

#include "DeviceCommandHandlers.h"

struct DeviceGameSaveState;

// Result of reading the SDK-generated device id from <saveRoot>/cloudsync/info.json.
struct ReadDeviceIdResult
{
    HRESULT hr{ S_OK };
    std::string errorMessage;
    std::string deviceId;   // DeviceID persisted in info.json (empty if not found)
    bool infoJsonPresent{ false };
    std::string infoJsonPath;
};

ReadDeviceIdResult ExecuteReadDeviceId(DeviceGameSaveState* state);

// Reads the device id the in-process SDK persists in cloudsync/info.json and
// returns it as the "deviceId" result field (so scenarios can capture it as a
// ${Role.deviceId} variable and compare it across relaunch sessions).
//
// Optional parameters:
//   expectedDeviceId (string) : if non-empty, the read device id MUST equal this
//                               value or the command fails (used to assert the id
//                               is stable across a relaunch).
//   requirePresent   (bool)   : default true. If info.json is missing/unreadable
//                               the command fails (that is the customer-reported
//                               failure mode: the id/file did not persist).
CommandResultPayload HandleReadDeviceId(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

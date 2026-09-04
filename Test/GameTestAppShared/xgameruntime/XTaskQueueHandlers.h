#pragma once

#include "DeviceCommandHandlers.h"

CommandResultPayload HandleXTaskQueueCreate(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

CommandResultPayload HandleXTaskQueueCloseHandle(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

// Pumps a Manual task queue port for a bounded window so a scenario can drive the queue
// and then deliberately stop ("park") it.
CommandResultPayload HandleXTaskQueueDispatch(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

// Arms the app's PLM suspend handler to perform a blocking XTaskQueueTerminate(wait=true) on the
// scenario's task queue. Models the PlayFabMultiplayer suspend-time teardown in bug 63050439.
CommandResultPayload HandleArmSuspendQueueTerminate(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);
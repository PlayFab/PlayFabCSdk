#pragma once

#include "DeviceCommandHandlers.h"

CommandResultPayload HandleHCTraceInit(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCTraceCleanup(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCSettingsSetTraceLevel(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCSettingsGetTraceLevel(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCTraceSetClientCallback(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCTraceSetTraceToDebugger(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCTraceSetEtwEnabled(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCTraceSetPlatformCallbacks(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCTraceImplMessage(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCTraceImplMessage_v(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCTraceImplScopeId(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

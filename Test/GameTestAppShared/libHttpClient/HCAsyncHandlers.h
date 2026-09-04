#pragma once

#include "DeviceCommandHandlers.h"

CommandResultPayload HandleXAsyncGetStatus(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleXAsyncGetResultSize(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleXAsyncCancel(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleXAsyncRun(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleXAsyncBegin(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleXAsyncComplete(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleXAsyncGetResult(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleXAsyncSchedule(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

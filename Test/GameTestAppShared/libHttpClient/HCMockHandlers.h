#pragma once

#include "DeviceCommandHandlers.h"

CommandResultPayload HandleHCMockCallCreate(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCMockCallDuplicateHandle(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCMockCallCloseHandle(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCMockAddMock(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCMockRemoveMock(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCMockClearMocks(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCMockSetMockMatchedCallback(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCMockResponseSetResponseBodyBytes(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCMockResponseSetStatusCode(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCMockResponseSetNetworkErrorCode(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCMockResponseSetPlatformNetworkErrorMessage(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCMockResponseSetHeader(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

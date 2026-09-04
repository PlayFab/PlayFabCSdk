#pragma once

#include "DeviceCommandHandlers.h"

CommandResultPayload HandleXblPrivacyGetAvoidListAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

CommandResultPayload HandleXblPrivacyGetMuteListAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

CommandResultPayload HandleXblPrivacyCheckPermissionAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

CommandResultPayload HandleXblPrivacyCheckPermissionForAnonymousUserAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

CommandResultPayload HandleXblPrivacyBatchCheckPermissionAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

CommandResultPayload HandleXblPrivacyGetAvoidListResultCount(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

CommandResultPayload HandleXblPrivacyGetAvoidListResult(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

CommandResultPayload HandleXblPrivacyCheckPermissionResultSize(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

CommandResultPayload HandleXblPrivacyCheckPermissionResult(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

CommandResultPayload HandleXblPrivacyCheckPermissionForAnonymousUserResultSize(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

CommandResultPayload HandleXblPrivacyCheckPermissionForAnonymousUserResult(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

CommandResultPayload HandleXblPrivacyBatchCheckPermissionResultSize(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

CommandResultPayload HandleXblPrivacyBatchCheckPermissionResult(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

CommandResultPayload HandleXblPrivacyGetMuteListResultCount(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

CommandResultPayload HandleXblPrivacyGetMuteListResult(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId);

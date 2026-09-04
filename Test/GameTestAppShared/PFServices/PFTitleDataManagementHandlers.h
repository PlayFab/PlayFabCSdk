#pragma once
#include "CommandHandlerShared.h"

struct DeviceGameSaveState;

CommandResultPayload HandlePFTitleDataManagementClientGetPublisherDataAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementClientGetPublisherDataGetResultSize(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementClientGetPublisherDataGetResult(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementClientGetTimeAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementClientGetTimeGetResult(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementClientGetTitleDataAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementClientGetTitleDataGetResultSize(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementClientGetTitleDataGetResult(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementClientGetTitleNewsAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementClientGetTitleNewsGetResultSize(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementClientGetTitleNewsGetResult(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementServerGetPublisherDataAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementServerGetPublisherDataGetResultSize(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementServerGetPublisherDataGetResult(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementServerGetTimeAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementServerGetTimeGetResult(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementServerGetTitleDataAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementServerGetTitleDataGetResultSize(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementServerGetTitleDataGetResult(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementServerGetTitleInternalDataAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementServerGetTitleInternalDataGetResultSize(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementServerGetTitleInternalDataGetResult(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementServerGetTitleNewsAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementServerGetTitleNewsGetResultSize(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementServerGetTitleNewsGetResult(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementServerSetPublisherDataAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementServerSetTitleDataAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFTitleDataManagementServerSetTitleInternalDataAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);

#pragma once
#include "CommandHandlerShared.h"

struct DeviceGameSaveState;

CommandResultPayload HandlePFLeaderboardsCreateLeaderboardDefinitionAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsDeleteLeaderboardDefinitionAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsDeleteLeaderboardEntriesAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsGetFriendLeaderboardForEntityAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsGetFriendLeaderboardForEntityGetResultSize(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsGetFriendLeaderboardForEntityGetResult(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsGetLeaderboardAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsGetLeaderboardGetResultSize(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsGetLeaderboardGetResult(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsGetLeaderboardAroundEntityAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsGetLeaderboardAroundEntityGetResultSize(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsGetLeaderboardAroundEntityGetResult(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsGetLeaderboardDefinitionAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsGetLeaderboardDefinitionGetResultSize(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsGetLeaderboardDefinitionGetResult(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsGetLeaderboardForEntitiesAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsGetLeaderboardForEntitiesGetResultSize(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsGetLeaderboardForEntitiesGetResult(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsIncrementLeaderboardVersionAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsIncrementLeaderboardVersionGetResult(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsListLeaderboardDefinitionsAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsListLeaderboardDefinitionsGetResultSize(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsListLeaderboardDefinitionsGetResult(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsUnlinkLeaderboardFromStatisticAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsUpdateLeaderboardDefinitionAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFLeaderboardsUpdateLeaderboardEntriesAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);

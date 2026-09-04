#pragma once
#include "CommandHandlerShared.h"

struct DeviceGameSaveState;

CommandResultPayload HandlePFFriendsClientAddFriendAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFFriendsClientAddFriendGetResult(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFFriendsClientGetFriendsListAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFFriendsClientGetFriendsListGetResultSize(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFFriendsClientGetFriendsListGetResult(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFFriendsClientRemoveFriendAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFFriendsClientSetFriendTagsAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFFriendsServerAddFriendAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFFriendsServerGetFriendsListAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFFriendsServerGetFriendsListGetResultSize(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFFriendsServerGetFriendsListGetResult(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFFriendsServerRemoveFriendAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);
CommandResultPayload HandlePFFriendsServerSetFriendTagsAsync(DeviceGameSaveState* state, const std::string& commandId, const std::string& command, const nlohmann::json& parameters, const std::string& deviceId);

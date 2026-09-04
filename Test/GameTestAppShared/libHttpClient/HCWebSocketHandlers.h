#pragma once

#include "DeviceCommandHandlers.h"

CommandResultPayload HandleHCWebSocketCreate(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCWebSocketConnectAsync(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCGetWebSocketConnectResult(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCWebSocketSendMessageAsync(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCWebSocketSendBinaryMessageAsync(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCGetWebSocketSendMessageResult(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCWebSocketDisconnect(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCWebSocketDuplicateHandle(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCWebSocketCloseHandle(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCWebSocketAssertClosed(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCWebSocketSetProxyUri(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCWebSocketSetProxyDecryptsHttps(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCWebSocketSetHeader(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCWebSocketSetPingInterval(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCWebSocketSetMaxReceiveBufferSize(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCWebSocketSetBinaryMessageFragmentEventFunction(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCWebSocketGetEventFunctions(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCWebSocketGetBinaryMessageFragmentEventFunction(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCWebSocketGetProxyUri(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCWebSocketGetHeader(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCWebSocketGetNumHeaders(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCWebSocketGetHeaderAtIndex(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCWebSocketGetPingInterval(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

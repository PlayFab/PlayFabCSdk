#pragma once

#include "DeviceCommandHandlers.h"
#include <httpClient/httpClient.h>

// Custom memory hook functions installed at app startup via HCMemSetFunctions.
// Allocations/frees are counted and exposed via HandleHCMemGetAllocStats.
void* STDAPIVCALLTYPE CustomHCMemAlloc(_In_ size_t size, _In_ HCMemoryType memoryType);
void STDAPIVCALLTYPE CustomHCMemFree(_In_ _Post_invalid_ void* pointer, _In_ HCMemoryType memoryType);

CommandResultPayload HandleHCMemSetFunctions(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCMemGetFunctions(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCInitialize(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCIsInitialized(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCCleanup(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCCleanupAsync(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCGetLibVersion(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCAddCallRoutedHandler(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCRemoveCallRoutedHandler(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCAddWebSocketRoutedHandler(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCRemoveWebSocketRoutedHandler(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCSetGlobalProxy(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCSettingsSetGlobalRequestLimit(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCSettingsGetGlobalRequestLimit(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCSetHttpCallPerformFunction(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCGetHttpCallPerformFunction(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCSetWebSocketFunctions(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCGetWebSocketFunctions(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCHttpDisableAssertsForSSLValidationInDevSandboxes(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

CommandResultPayload HandleHCMemGetAllocStats(
    DeviceGameSaveState* state, const std::string& commandId, const std::string& command,
    const nlohmann::json& parameters, const std::string& deviceId);

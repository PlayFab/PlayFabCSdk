#include "pch.h"
#include "PFPlatformSpecificHandlers.h"
#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <playfab/services/PFPlatformSpecific.h>
#include <playfab/core/PFLocalUser.h>
#include <vector>
#include "CommandRegistry.h"

using CommandHandlerShared::TryGetStringParameter;
using CommandHandlerShared::TryParseBoolParameter;

CommandResultPayload HandlePFPlatformSpecificServerAwardSteamAchievementAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            const HRESULT hr = PFPlatformSpecificServerAwardSteamAchievementAsync(entityHandle, nullptr, &async);
            LogToWindowFormat("PFPlatformSpecificServerAwardSteamAchievementAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            RETURN_IF_FAILED(PFPlatformSpecificServerAwardSteamAchievementGetResultSize(&async, &bufferSize));
            std::vector<uint8_t> buffer(bufferSize);
            HRESULT hr = PFPlatformSpecificServerAwardSteamAchievementGetResult(&async, buffer.size(), buffer.data(), nullptr, nullptr);
            LogToWindowFormat("PFPlatformSpecificServerAwardSteamAchievementAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFPlatformSpecificServerAwardSteamAchievementGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlatformSpecificServerAwardSteamAchievementGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlatformSpecificServerAwardSteamAchievementGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlatformSpecificServerAwardSteamAchievementGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFPlatformSpecificClientAndroidDevicePushNotificationRegistrationAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlatformSpecificClientAndroidDevicePushNotificationRegistrationAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFPlatformSpecificClientRefreshPSNAuthTokenAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlatformSpecificClientRefreshPSNAuthTokenAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFPlatformSpecificClientRegisterForIOSPushNotificationAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFPlatformSpecificClientRegisterForIOSPushNotificationAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "PFPlatformSpecificClientAndroidDevicePushNotificationRegistrationAsync", HandlePFPlatformSpecificClientAndroidDevicePushNotificationRegistrationAsync },
    { "PFPlatformSpecificClientRefreshPSNAuthTokenAsync", HandlePFPlatformSpecificClientRefreshPSNAuthTokenAsync },
    { "PFPlatformSpecificClientRegisterForIOSPushNotificationAsync", HandlePFPlatformSpecificClientRegisterForIOSPushNotificationAsync },
    { "PFPlatformSpecificServerAwardSteamAchievementAsync", HandlePFPlatformSpecificServerAwardSteamAchievementAsync },
    { "PFPlatformSpecificServerAwardSteamAchievementGetResult", HandlePFPlatformSpecificServerAwardSteamAchievementGetResult },
    { "PFPlatformSpecificServerAwardSteamAchievementGetResultSize", HandlePFPlatformSpecificServerAwardSteamAchievementGetResultSize }
});

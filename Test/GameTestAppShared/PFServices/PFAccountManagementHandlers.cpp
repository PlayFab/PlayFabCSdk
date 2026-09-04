#include "pch.h"
#include "PFAccountManagementHandlers.h"
#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <playfab/services/PFAccountManagement.h>
#include <playfab/core/PFLocalUser.h>
#include <vector>
#include "CommandRegistry.h"

using CommandHandlerShared::TryGetStringParameter;
using CommandHandlerShared::TryParseBoolParameter;

CommandResultPayload HandlePFAccountManagementClientAddOrUpdateContactEmailAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementAddOrUpdateContactEmailRequest request{};
            const HRESULT hr = PFAccountManagementClientAddOrUpdateContactEmailAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientAddOrUpdateContactEmailAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementClientGetAccountInfoAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetAccountInfoRequest request{};
            const HRESULT hr = PFAccountManagementClientGetAccountInfoAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientGetAccountInfoAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementClientGetAccountInfoGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetAccountInfoResult* result{ nullptr };
            HRESULT hr = PFAccountManagementClientGetAccountInfoGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementClientGetAccountInfoAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetAccountInfoGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetAccountInfoGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetAccountInfoGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetAccountInfoGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayerCombinedInfoAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayerCombinedInfoRequest request{};
            const HRESULT hr = PFAccountManagementClientGetPlayerCombinedInfoAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientGetPlayerCombinedInfoAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementClientGetPlayerCombinedInfoGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayerCombinedInfoResult* result{ nullptr };
            HRESULT hr = PFAccountManagementClientGetPlayerCombinedInfoGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementClientGetPlayerCombinedInfoAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayerCombinedInfoGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayerCombinedInfoGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayerCombinedInfoGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayerCombinedInfoGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayerProfileAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayerProfileRequest request{};
            const HRESULT hr = PFAccountManagementClientGetPlayerProfileAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientGetPlayerProfileAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementClientGetPlayerProfileGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayerProfileResult* result{ nullptr };
            HRESULT hr = PFAccountManagementClientGetPlayerProfileGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementClientGetPlayerProfileAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayerProfileGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayerProfileGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayerProfileGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayerProfileGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromBattleNetAccountIdsRequest request{};
            const HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromBattleNetAccountIdsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromFacebookIDsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromFacebookIDsRequest request{};
            const HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromFacebookIDsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromFacebookIDsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementClientGetPlayFabIDsFromFacebookIDsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromFacebookIDsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromFacebookIDsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromFacebookIDsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromFacebookIDsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromFacebookIDsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromFacebookIDsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromFacebookIDsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequest request{};
            const HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromGameCenterIDsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequest request{};
            const HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromGameCenterIDsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromGameCenterIDsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromGameCenterIDsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromGoogleIDsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromGoogleIDsRequest request{};
            const HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromGoogleIDsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromGoogleIDsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementClientGetPlayFabIDsFromGoogleIDsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromGoogleIDsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromGoogleIDsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromGoogleIDsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromGoogleIDsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromGoogleIDsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromGoogleIDsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromGoogleIDsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequest request{};
            const HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromKongregateIDsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromKongregateIDsRequest request{};
            const HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromKongregateIDsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromKongregateIDsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementClientGetPlayFabIDsFromKongregateIDsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromKongregateIDsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromKongregateIDsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromKongregateIDsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromKongregateIDsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromKongregateIDsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromKongregateIDsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromKongregateIDsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequest request{};
            const HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequest request{};
            const HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequest request{};
            const HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequest request{};
            const HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromSteamIDsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromSteamIDsRequest request{};
            const HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromSteamIDsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromSteamIDsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementClientGetPlayFabIDsFromSteamIDsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromSteamIDsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromSteamIDsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromSteamIDsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromSteamIDsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromSteamIDsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromSteamIDsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromSteamIDsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromSteamNamesAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromSteamNamesRequest request{};
            const HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromSteamNamesAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromSteamNamesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementClientGetPlayFabIDsFromSteamNamesGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromSteamNamesResult* result{ nullptr };
            HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromSteamNamesGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromSteamNamesAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromSteamNamesGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromSteamNamesGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromSteamNamesGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromSteamNamesGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromTwitchIDsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromTwitchIDsRequest request{};
            const HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromTwitchIDsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromTwitchIDsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementClientGetPlayFabIDsFromTwitchIDsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromTwitchIDsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromTwitchIDsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromTwitchIDsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromTwitchIDsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromTwitchIDsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromTwitchIDsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromTwitchIDsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequest request{};
            const char* xboxLiveIds[] = { "2533274790412952" };
            request.xboxLiveAccountIDs = xboxLiveIds;
            request.xboxLiveAccountIDsCount = 1;
            const HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientLinkBattleNetAccountAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementClientLinkBattleNetAccountRequest request{};
            const HRESULT hr = PFAccountManagementClientLinkBattleNetAccountAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientLinkBattleNetAccountAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementClientLinkCustomIDAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    // Get customId from parameters or use default
    std::string customIdStr;
    std::string error;
    if (!CommandHandlerShared::TryGetStringParameter(parameters, "customId", customIdStr, error))
    {
        customIdStr = "test-link-custom-id";
    }
    
    // Get forceLink from parameters (default false)
    bool forceLinkValue = false;
    CommandHandlerShared::TryParseBoolParameter(parameters, "forceLink", forceLinkValue, error);

    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [state, customIdStr, forceLinkValue](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFEntityHandle entityHandle{ nullptr };
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            
            bool forceLink = forceLinkValue;
            PFAccountManagementLinkCustomIDRequest request{};
            request.customId = customIdStr.c_str();
            request.forceLink = forceLink ? &forceLink : nullptr;
            
            LogToWindowFormat("PFAccountManagementClientLinkCustomIDAsync: customId=%s, forceLink=%s", 
                request.customId, forceLink ? "true" : "false");
            
            const HRESULT hr = PFAccountManagementClientLinkCustomIDAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientLinkCustomIDAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementClientLinkOpenIdConnectAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementLinkOpenIdConnectRequest request{};
            const HRESULT hr = PFAccountManagementClientLinkOpenIdConnectAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientLinkOpenIdConnectAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementClientLinkSteamAccountAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementLinkSteamAccountRequest request{};
            const HRESULT hr = PFAccountManagementClientLinkSteamAccountAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientLinkSteamAccountAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

// Helper to perform the actual link Xbox account call
static HRESULT DoLinkXboxAccount(DeviceGameSaveState* state, XAsyncBlock& async, bool forceLink)
{
    PFEntityHandle entityHandle{ nullptr };
    RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));

    PFAccountManagementClientLinkXboxAccountRequest request{};
    request.forceLink = forceLink ? &forceLink : nullptr;
#if HC_PLATFORM == HC_PLATFORM_GDK
    request.user = state->xuser;
#endif
    const HRESULT hr = PFAccountManagementClientLinkXboxAccountAsync(entityHandle, &request, &async);
    LogToWindowFormat("PFAccountManagementClientLinkXboxAccountAsync (forceLink=%s, hr=0x%08X)", 
                      forceLink ? "true" : "false", static_cast<uint32_t>(hr));
    PFEntityCloseHandle(entityHandle);
    return hr;
}

// Helper to unlink Xbox account synchronously (blocking wait)
static HRESULT DoUnlinkXboxAccountSync(DeviceGameSaveState* state)
{
    XAsyncBlock unlinkAsync{};
    unlinkAsync.queue = state->taskQueue;
    
    PFEntityHandle entityHandle{ nullptr };
    RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));

    PFAccountManagementClientUnlinkXboxAccountRequest request{};
    HRESULT hr = PFAccountManagementClientUnlinkXboxAccountAsync(entityHandle, &request, &unlinkAsync);
    LogToWindowFormat("PFAccountManagementClientUnlinkXboxAccountAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
    PFEntityCloseHandle(entityHandle);
    
    if (FAILED(hr))
    {
        return hr;
    }
    
    // Wait for unlink to complete
    hr = XAsyncGetStatus(&unlinkAsync, true);
    LogToWindowFormat("PFAccountManagementClientUnlinkXboxAccountAsync completed (hr=0x%08X)", static_cast<uint32_t>(hr));
    return hr;
}

CommandResultPayload HandlePFAccountManagementClientLinkXboxAccountAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    // Parse parameters
    bool forceLinkValue = false;
    bool unlinkIfNeededValue = false;
    std::string error;
    CommandHandlerShared::TryParseBoolParameter(parameters, "forceLink", forceLinkValue, error);
    CommandHandlerShared::TryParseBoolParameter(parameters, "unlinkIfNeeded", unlinkIfNeededValue, error);
    
    LogToWindowFormat("HandlePFAccountManagementClientLinkXboxAccountAsync: forceLink=%s, unlinkIfNeeded=%s",
                      forceLinkValue ? "true" : "false", unlinkIfNeededValue ? "true" : "false");

    // Manual async handling (not using helper) to support retry logic
    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);
    const auto start = std::chrono::steady_clock::now();
    
    XAsyncBlock async{};
    async.queue = state->taskQueue;
    
    // First attempt
    HRESULT hr = DoLinkXboxAccount(state, async, forceLinkValue);
    if (SUCCEEDED(hr))
    {
        hr = XAsyncGetStatus(&async, true);
    }
    
    // Check for link errors and retry with unlink if requested.
    // 0x8923542c = ACCOUNT_ALREADY_LINKED (this entity already has a different Xbox linked)
    // 0x8923542d = E_PF_LINKED_ACCOUNT_ALREADY_CLAIMED (Xbox linked to a different entity)
    // 0x87DD0033 = PlayFab service error for LinkedAccountAlreadyClaimed (raw HTTP error code path)
    static const HRESULT E_PF_LINKED_ACCOUNT_CLAIMED_RAW = 0x87DD0033;
    static const HRESULT E_PF_ACCOUNT_ALREADY_LINKED_RAW = static_cast<HRESULT>(0x8923542c);
    if ((hr == E_PF_LINKED_ACCOUNT_ALREADY_CLAIMED || hr == E_PF_LINKED_ACCOUNT_CLAIMED_RAW || hr == E_PF_ACCOUNT_ALREADY_LINKED_RAW) && unlinkIfNeededValue)
    {
        LogToWindowFormat("Link failed (0x%08X), attempting unlink then relink...",
                          static_cast<uint32_t>(hr));
        
        // Unlink the Xbox account from the current entity
        HRESULT unlinkHr = DoUnlinkXboxAccountSync(state);
        // Unlink may fail if the Xbox isn't linked to THIS entity (0x87DD0033 case) — that's OK,
        // we still retry the link with forceLink=true which should steal it from the other entity.
        if (FAILED(unlinkHr))
        {
            LogToWindowFormat("Unlink returned (hr=0x%08X), proceeding with forceLink retry anyway...",
                              static_cast<uint32_t>(unlinkHr));
        }
        
        // Retry the link with a new async block and forceLink=true
        XAsyncBlock retryAsync{};
        retryAsync.queue = state->taskQueue;
        
        hr = DoLinkXboxAccount(state, retryAsync, true /*forceLink*/);
        if (SUCCEEDED(hr))
        {
            hr = XAsyncGetStatus(&retryAsync, true);
        }
        LogToWindowFormat("Retry link completed (hr=0x%08X)", static_cast<uint32_t>(hr));
    }
    
    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);
    CommandHandlerShared::SetHResult(payload.result, hr);
    if (FAILED(hr))
    {
        CommandHandlerShared::MarkFailure(payload.result, hr, command + " failed");
    }
    else
    {
        CommandHandlerShared::MarkSuccess(payload.result);
    }
    return payload;
}

CommandResultPayload HandlePFAccountManagementClientRemoveContactEmailAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementRemoveContactEmailRequest request{};
            const HRESULT hr = PFAccountManagementClientRemoveContactEmailAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientRemoveContactEmailAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementClientReportPlayerAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementReportPlayerClientRequest request{};
            request.reporteeId = state->entityId.c_str();
            const HRESULT hr = PFAccountManagementClientReportPlayerAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientReportPlayerAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFAccountManagementReportPlayerClientResult result{};
            return PFAccountManagementClientReportPlayerGetResult(&async, &result);
        });
}

CommandResultPayload HandlePFAccountManagementClientReportPlayerGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientReportPlayerGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientUnlinkBattleNetAccountAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementClientUnlinkBattleNetAccountRequest request{};
            const HRESULT hr = PFAccountManagementClientUnlinkBattleNetAccountAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientUnlinkBattleNetAccountAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementClientUnlinkCustomIDAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementUnlinkCustomIDRequest request{};
            const HRESULT hr = PFAccountManagementClientUnlinkCustomIDAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientUnlinkCustomIDAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementClientUnlinkOpenIdConnectAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementUnlinkOpenIdConnectRequest request{};
            const HRESULT hr = PFAccountManagementClientUnlinkOpenIdConnectAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientUnlinkOpenIdConnectAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementClientUnlinkSteamAccountAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementUnlinkSteamAccountRequest request{};
            const HRESULT hr = PFAccountManagementClientUnlinkSteamAccountAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientUnlinkSteamAccountAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementClientUnlinkXboxAccountAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementClientUnlinkXboxAccountRequest request{};
            const HRESULT hr = PFAccountManagementClientUnlinkXboxAccountAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientUnlinkXboxAccountAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementClientUpdateAvatarUrlAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementClientUpdateAvatarUrlRequest request{};
            request.imageUrl = "https://example.com/avatar.png";
            const HRESULT hr = PFAccountManagementClientUpdateAvatarUrlAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientUpdateAvatarUrlAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementClientUpdateUserTitleDisplayNameAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            PFAccountManagementUpdateUserTitleDisplayNameRequest request{};
            request.displayName = "TestUser";
            const HRESULT hr = PFAccountManagementClientUpdateUserTitleDisplayNameAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementClientUpdateUserTitleDisplayNameAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementClientUpdateUserTitleDisplayNameGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementUpdateUserTitleDisplayNameResult* result{ nullptr };
            HRESULT hr = PFAccountManagementClientUpdateUserTitleDisplayNameGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementClientUpdateUserTitleDisplayNameAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementClientUpdateUserTitleDisplayNameGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientUpdateUserTitleDisplayNameGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientUpdateUserTitleDisplayNameGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientUpdateUserTitleDisplayNameGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerBanUsersAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementBanUsersRequest request{};
            const HRESULT hr = PFAccountManagementServerBanUsersAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerBanUsersAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementServerBanUsersGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementBanUsersResult* result{ nullptr };
            HRESULT hr = PFAccountManagementServerBanUsersGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementServerBanUsersAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementServerBanUsersGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerBanUsersGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerBanUsersGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerBanUsersGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerDeletePlayerAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementDeletePlayerRequest request{};
            const HRESULT hr = PFAccountManagementServerDeletePlayerAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerDeletePlayerAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayerCombinedInfoAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayerCombinedInfoRequest request{};
            const HRESULT hr = PFAccountManagementServerGetPlayerCombinedInfoAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerGetPlayerCombinedInfoAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementServerGetPlayerCombinedInfoGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayerCombinedInfoResult* result{ nullptr };
            HRESULT hr = PFAccountManagementServerGetPlayerCombinedInfoGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementServerGetPlayerCombinedInfoAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayerCombinedInfoGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayerCombinedInfoGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayerCombinedInfoGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayerCombinedInfoGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayerProfileAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayerProfileRequest request{};
            const HRESULT hr = PFAccountManagementServerGetPlayerProfileAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerGetPlayerProfileAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementServerGetPlayerProfileGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayerProfileResult* result{ nullptr };
            HRESULT hr = PFAccountManagementServerGetPlayerProfileGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementServerGetPlayerProfileAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayerProfileGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayerProfileGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayerProfileGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayerProfileGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromBattleNetAccountIdsRequest request{};
            const HRESULT hr = PFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromBattleNetAccountIdsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromFacebookIDsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromFacebookIDsRequest request{};
            const HRESULT hr = PFAccountManagementServerGetPlayFabIDsFromFacebookIDsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerGetPlayFabIDsFromFacebookIDsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementServerGetPlayFabIDsFromFacebookIDsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromFacebookIDsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementServerGetPlayFabIDsFromFacebookIDsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementServerGetPlayFabIDsFromFacebookIDsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromFacebookIDsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromFacebookIDsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromFacebookIDsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromFacebookIDsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequest request{};
            const HRESULT hr = PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequest request{};
            const HRESULT hr = PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequest request{};
            const HRESULT hr = PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequest request{};
            const HRESULT hr = PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequest request{};
            const HRESULT hr = PFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromSteamIDsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromSteamIDsRequest request{};
            const HRESULT hr = PFAccountManagementServerGetPlayFabIDsFromSteamIDsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerGetPlayFabIDsFromSteamIDsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementServerGetPlayFabIDsFromSteamIDsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromSteamIDsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementServerGetPlayFabIDsFromSteamIDsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementServerGetPlayFabIDsFromSteamIDsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromSteamIDsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromSteamIDsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromSteamIDsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromSteamIDsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromSteamNamesAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromSteamNamesRequest request{};
            const HRESULT hr = PFAccountManagementServerGetPlayFabIDsFromSteamNamesAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerGetPlayFabIDsFromSteamNamesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementServerGetPlayFabIDsFromSteamNamesGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromSteamNamesResult* result{ nullptr };
            HRESULT hr = PFAccountManagementServerGetPlayFabIDsFromSteamNamesGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementServerGetPlayFabIDsFromSteamNamesAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromSteamNamesGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromSteamNamesGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromSteamNamesGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromSteamNamesGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromTwitchIDsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromTwitchIDsRequest request{};
            const HRESULT hr = PFAccountManagementServerGetPlayFabIDsFromTwitchIDsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerGetPlayFabIDsFromTwitchIDsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementServerGetPlayFabIDsFromTwitchIDsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromTwitchIDsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementServerGetPlayFabIDsFromTwitchIDsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementServerGetPlayFabIDsFromTwitchIDsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromTwitchIDsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromTwitchIDsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromTwitchIDsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromTwitchIDsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequest request{};
            const HRESULT hr = PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetServerCustomIDsFromPlayFabIDsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequest request{};
            const HRESULT hr = PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetServerCustomIDsFromPlayFabIDsResult* result{ nullptr };
            HRESULT hr = PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetServerCustomIDsFromPlayFabIDsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetServerCustomIDsFromPlayFabIDsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetUserAccountInfoAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetUserAccountInfoRequest request{};
            const HRESULT hr = PFAccountManagementServerGetUserAccountInfoAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerGetUserAccountInfoAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementServerGetUserAccountInfoGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetUserAccountInfoResult* result{ nullptr };
            HRESULT hr = PFAccountManagementServerGetUserAccountInfoGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementServerGetUserAccountInfoAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetUserAccountInfoGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetUserAccountInfoGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetUserAccountInfoGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetUserAccountInfoGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetUserBansAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetUserBansRequest request{};
            const HRESULT hr = PFAccountManagementServerGetUserBansAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerGetUserBansAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementServerGetUserBansGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetUserBansResult* result{ nullptr };
            HRESULT hr = PFAccountManagementServerGetUserBansGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementServerGetUserBansAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetUserBansGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetUserBansGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetUserBansGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetUserBansGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerLinkBattleNetAccountAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementServerLinkBattleNetAccountRequest request{};
            const HRESULT hr = PFAccountManagementServerLinkBattleNetAccountAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerLinkBattleNetAccountAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementServerLinkNintendoServiceAccountAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementServerLinkNintendoServiceAccountRequest request{};
            const HRESULT hr = PFAccountManagementServerLinkNintendoServiceAccountAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerLinkNintendoServiceAccountAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementServerLinkNintendoServiceAccountSubjectAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementLinkNintendoServiceAccountSubjectRequest request{};
            const HRESULT hr = PFAccountManagementServerLinkNintendoServiceAccountSubjectAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerLinkNintendoServiceAccountSubjectAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementServerLinkNintendoSwitchDeviceIdAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementServerLinkNintendoSwitchDeviceIdRequest request{};
            const HRESULT hr = PFAccountManagementServerLinkNintendoSwitchDeviceIdAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerLinkNintendoSwitchDeviceIdAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementServerLinkPSNAccountAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementServerLinkPSNAccountRequest request{};
            const HRESULT hr = PFAccountManagementServerLinkPSNAccountAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerLinkPSNAccountAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementServerLinkPSNIdAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementLinkPSNIdRequest request{};
            const HRESULT hr = PFAccountManagementServerLinkPSNIdAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerLinkPSNIdAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementServerLinkServerCustomIdAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementLinkServerCustomIdRequest request{};
            const HRESULT hr = PFAccountManagementServerLinkServerCustomIdAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerLinkServerCustomIdAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementServerLinkSteamIdAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementLinkSteamIdRequest request{};
            const HRESULT hr = PFAccountManagementServerLinkSteamIdAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerLinkSteamIdAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementServerLinkXboxAccountAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementServerLinkXboxAccountRequest request{};
            const HRESULT hr = PFAccountManagementServerLinkXboxAccountAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerLinkXboxAccountAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementServerRevokeAllBansForUserAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementRevokeAllBansForUserRequest request{};
            const HRESULT hr = PFAccountManagementServerRevokeAllBansForUserAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerRevokeAllBansForUserAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementServerRevokeAllBansForUserGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementRevokeAllBansForUserResult* result{ nullptr };
            HRESULT hr = PFAccountManagementServerRevokeAllBansForUserGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementServerRevokeAllBansForUserAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementServerRevokeAllBansForUserGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerRevokeAllBansForUserGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerRevokeAllBansForUserGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerRevokeAllBansForUserGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerRevokeBansAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementRevokeBansRequest request{};
            const HRESULT hr = PFAccountManagementServerRevokeBansAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerRevokeBansAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementServerRevokeBansGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementRevokeBansResult* result{ nullptr };
            HRESULT hr = PFAccountManagementServerRevokeBansGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementServerRevokeBansAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementServerRevokeBansGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerRevokeBansGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerRevokeBansGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerRevokeBansGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerSendCustomAccountRecoveryEmailAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementSendCustomAccountRecoveryEmailRequest request{};
            const HRESULT hr = PFAccountManagementServerSendCustomAccountRecoveryEmailAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerSendCustomAccountRecoveryEmailAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementServerSendEmailFromTemplateAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementSendEmailFromTemplateRequest request{};
            const HRESULT hr = PFAccountManagementServerSendEmailFromTemplateAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerSendEmailFromTemplateAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementServerUnlinkBattleNetAccountAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementServerUnlinkBattleNetAccountRequest request{};
            const HRESULT hr = PFAccountManagementServerUnlinkBattleNetAccountAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerUnlinkBattleNetAccountAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementServerUnlinkNintendoServiceAccountAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementServerUnlinkNintendoServiceAccountRequest request{};
            const HRESULT hr = PFAccountManagementServerUnlinkNintendoServiceAccountAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerUnlinkNintendoServiceAccountAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementServerUnlinkNintendoSwitchDeviceIdAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequest request{};
            const HRESULT hr = PFAccountManagementServerUnlinkNintendoSwitchDeviceIdAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerUnlinkNintendoSwitchDeviceIdAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementServerUnlinkPSNAccountAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementServerUnlinkPSNAccountRequest request{};
            const HRESULT hr = PFAccountManagementServerUnlinkPSNAccountAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerUnlinkPSNAccountAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementServerUnlinkServerCustomIdAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementUnlinkServerCustomIdRequest request{};
            const HRESULT hr = PFAccountManagementServerUnlinkServerCustomIdAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerUnlinkServerCustomIdAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementServerUnlinkSteamIdAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementUnlinkSteamIdRequest request{};
            const HRESULT hr = PFAccountManagementServerUnlinkSteamIdAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerUnlinkSteamIdAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementServerUnlinkXboxAccountAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementServerUnlinkXboxAccountRequest request{};
            const HRESULT hr = PFAccountManagementServerUnlinkXboxAccountAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerUnlinkXboxAccountAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementServerUpdateAvatarUrlAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementServerUpdateAvatarUrlRequest request{};
            const HRESULT hr = PFAccountManagementServerUpdateAvatarUrlAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerUpdateAvatarUrlAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFAccountManagementServerUpdateBansAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementUpdateBansRequest request{};
            const HRESULT hr = PFAccountManagementServerUpdateBansAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementServerUpdateBansAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementServerUpdateBansGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementUpdateBansResult* result{ nullptr };
            HRESULT hr = PFAccountManagementServerUpdateBansGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementServerUpdateBansAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementServerUpdateBansGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerUpdateBansGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementServerUpdateBansGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerUpdateBansGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementGetTitlePlayersFromXboxLiveIDsAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            // Pass empty request - test caller can customize via parameters
            PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequest request{};
            const char* xboxLiveIds[] = { "2533274790412952" };
            request.xboxLiveIds = xboxLiveIds;
            request.xboxLiveIdsCount = 1;
            request.sandbox = "RETAIL";
            const HRESULT hr = PFAccountManagementGetTitlePlayersFromXboxLiveIDsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementGetTitlePlayersFromXboxLiveIDsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementGetTitlePlayersFromXboxLiveIDsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementGetTitlePlayersFromProviderIDsResponse* result{ nullptr };
            HRESULT hr = PFAccountManagementGetTitlePlayersFromXboxLiveIDsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementGetTitlePlayersFromXboxLiveIDsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementGetTitlePlayersFromXboxLiveIDsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementGetTitlePlayersFromXboxLiveIDsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementGetTitlePlayersFromXboxLiveIDsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementGetTitlePlayersFromXboxLiveIDsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementSetDisplayNameAsync(
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
            RETURN_IF_FAILED(state->entityHandle ? PFEntityDuplicateHandle(state->entityHandle, &entityHandle) : PFLocalUserTryGetEntityHandle(state->localUserHandle, &entityHandle));
            PFAccountManagementSetDisplayNameRequest request{};
            request.displayName = "TestUser";
            const HRESULT hr = PFAccountManagementSetDisplayNameAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFAccountManagementSetDisplayNameAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFAccountManagementSetDisplayNameGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFAccountManagementSetDisplayNameResponse* result{ nullptr };
            HRESULT hr = PFAccountManagementSetDisplayNameGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFAccountManagementSetDisplayNameAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFAccountManagementSetDisplayNameGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementSetDisplayNameGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementSetDisplayNameGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementSetDisplayNameGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFAccountManagementClientAddUsernamePasswordAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientAddUsernamePasswordAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientAddUsernamePasswordGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientAddUsernamePasswordGetResultSize: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientAddUsernamePasswordGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientAddUsernamePasswordGetResult: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromOpenIdSubjectIdentifiersAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromOpenIdSubjectIdentifiersAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResultSize: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResult: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientLinkAndroidDeviceIDAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientLinkAndroidDeviceIDAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientLinkAppleAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientLinkAppleAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientLinkFacebookAccountAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientLinkFacebookAccountAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientLinkFacebookInstantGamesIdAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientLinkFacebookInstantGamesIdAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientLinkGameCenterAccountAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientLinkGameCenterAccountAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientLinkGoogleAccountAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientLinkGoogleAccountAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientLinkGooglePlayGamesServicesAccountAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientLinkGooglePlayGamesServicesAccountAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientLinkIOSDeviceIDAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientLinkIOSDeviceIDAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientLinkKongregateAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientLinkKongregateAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientLinkNintendoServiceAccountAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientLinkNintendoServiceAccountAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientLinkNintendoSwitchDeviceIdAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientLinkNintendoSwitchDeviceIdAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientLinkPSNAccountAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientLinkPSNAccountAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientLinkTwitchAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientLinkTwitchAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientSendAccountRecoveryEmailAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientSendAccountRecoveryEmailAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientUnlinkAndroidDeviceIDAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientUnlinkAndroidDeviceIDAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientUnlinkAppleAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientUnlinkAppleAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientUnlinkFacebookAccountAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientUnlinkFacebookAccountAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientUnlinkFacebookInstantGamesIdAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientUnlinkFacebookInstantGamesIdAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientUnlinkGameCenterAccountAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientUnlinkGameCenterAccountAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientUnlinkGoogleAccountAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientUnlinkGoogleAccountAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientUnlinkGooglePlayGamesServicesAccountAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientUnlinkGooglePlayGamesServicesAccountAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientUnlinkIOSDeviceIDAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientUnlinkIOSDeviceIDAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientUnlinkKongregateAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientUnlinkKongregateAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientUnlinkNintendoServiceAccountAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientUnlinkNintendoServiceAccountAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientUnlinkNintendoSwitchDeviceIdAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientUnlinkNintendoSwitchDeviceIdAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientUnlinkPSNAccountAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientUnlinkPSNAccountAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementClientUnlinkTwitchAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementClientUnlinkTwitchAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromOpenIdSubjectIdentifiersAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromOpenIdSubjectIdentifiersAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResultSize: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementServerGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResult: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementServerLinkTwitchAccountAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerLinkTwitchAccountAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementServerLinkXboxIdAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerLinkXboxIdAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementServerUnlinkFacebookAccountAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerUnlinkFacebookAccountAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementServerUnlinkFacebookInstantGamesIdAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerUnlinkFacebookInstantGamesIdAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandlePFAccountManagementServerUnlinkTwitchAccountAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFAccountManagementServerUnlinkTwitchAccountAsync: API not available in local headers");
            return E_NOTIMPL;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "PFAccountManagementClientAddOrUpdateContactEmailAsync", HandlePFAccountManagementClientAddOrUpdateContactEmailAsync },
    { "PFAccountManagementClientAddUsernamePasswordAsync", HandlePFAccountManagementClientAddUsernamePasswordAsync },
    { "PFAccountManagementClientAddUsernamePasswordGetResult", HandlePFAccountManagementClientAddUsernamePasswordGetResult },
    { "PFAccountManagementClientAddUsernamePasswordGetResultSize", HandlePFAccountManagementClientAddUsernamePasswordGetResultSize },
    { "PFAccountManagementClientGetAccountInfoAsync", HandlePFAccountManagementClientGetAccountInfoAsync },
    { "PFAccountManagementClientGetAccountInfoGetResult", HandlePFAccountManagementClientGetAccountInfoGetResult },
    { "PFAccountManagementClientGetAccountInfoGetResultSize", HandlePFAccountManagementClientGetAccountInfoGetResultSize },
    { "PFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsAsync", HandlePFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsAsync },
    { "PFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsGetResult", HandlePFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsGetResult },
    { "PFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsGetResultSize", HandlePFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsGetResultSize },
    { "PFAccountManagementClientGetPlayFabIDsFromFacebookIDsAsync", HandlePFAccountManagementClientGetPlayFabIDsFromFacebookIDsAsync },
    { "PFAccountManagementClientGetPlayFabIDsFromFacebookIDsGetResult", HandlePFAccountManagementClientGetPlayFabIDsFromFacebookIDsGetResult },
    { "PFAccountManagementClientGetPlayFabIDsFromFacebookIDsGetResultSize", HandlePFAccountManagementClientGetPlayFabIDsFromFacebookIDsGetResultSize },
    { "PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsAsync", HandlePFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsAsync },
    { "PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsGetResult", HandlePFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsGetResult },
    { "PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsGetResultSize", HandlePFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsGetResultSize },
    { "PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsAsync", HandlePFAccountManagementClientGetPlayFabIDsFromGameCenterIDsAsync },
    { "PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsGetResult", HandlePFAccountManagementClientGetPlayFabIDsFromGameCenterIDsGetResult },
    { "PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsGetResultSize", HandlePFAccountManagementClientGetPlayFabIDsFromGameCenterIDsGetResultSize },
    { "PFAccountManagementClientGetPlayFabIDsFromGoogleIDsAsync", HandlePFAccountManagementClientGetPlayFabIDsFromGoogleIDsAsync },
    { "PFAccountManagementClientGetPlayFabIDsFromGoogleIDsGetResult", HandlePFAccountManagementClientGetPlayFabIDsFromGoogleIDsGetResult },
    { "PFAccountManagementClientGetPlayFabIDsFromGoogleIDsGetResultSize", HandlePFAccountManagementClientGetPlayFabIDsFromGoogleIDsGetResultSize },
    { "PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsAsync", HandlePFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsAsync },
    { "PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsGetResult", HandlePFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsGetResult },
    { "PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsGetResultSize", HandlePFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsGetResultSize },
    { "PFAccountManagementClientGetPlayFabIDsFromKongregateIDsAsync", HandlePFAccountManagementClientGetPlayFabIDsFromKongregateIDsAsync },
    { "PFAccountManagementClientGetPlayFabIDsFromKongregateIDsGetResult", HandlePFAccountManagementClientGetPlayFabIDsFromKongregateIDsGetResult },
    { "PFAccountManagementClientGetPlayFabIDsFromKongregateIDsGetResultSize", HandlePFAccountManagementClientGetPlayFabIDsFromKongregateIDsGetResultSize },
    { "PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsAsync", HandlePFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsAsync },
    { "PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsGetResult", HandlePFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsGetResult },
    { "PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsGetResultSize", HandlePFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsGetResultSize },
    { "PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync", HandlePFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync },
    { "PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResult", HandlePFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResult },
    { "PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResultSize", HandlePFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResultSize },
    { "PFAccountManagementClientGetPlayFabIDsFromOpenIdSubjectIdentifiersAsync", HandlePFAccountManagementClientGetPlayFabIDsFromOpenIdSubjectIdentifiersAsync },
    { "PFAccountManagementClientGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResult", HandlePFAccountManagementClientGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResult },
    { "PFAccountManagementClientGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResultSize", HandlePFAccountManagementClientGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResultSize },
    { "PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsAsync", HandlePFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsAsync },
    { "PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsGetResult", HandlePFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsGetResult },
    { "PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsGetResultSize", HandlePFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsGetResultSize },
    { "PFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsAsync", HandlePFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsAsync },
    { "PFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsGetResult", HandlePFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsGetResult },
    { "PFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsGetResultSize", HandlePFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsGetResultSize },
    { "PFAccountManagementClientGetPlayFabIDsFromSteamIDsAsync", HandlePFAccountManagementClientGetPlayFabIDsFromSteamIDsAsync },
    { "PFAccountManagementClientGetPlayFabIDsFromSteamIDsGetResult", HandlePFAccountManagementClientGetPlayFabIDsFromSteamIDsGetResult },
    { "PFAccountManagementClientGetPlayFabIDsFromSteamIDsGetResultSize", HandlePFAccountManagementClientGetPlayFabIDsFromSteamIDsGetResultSize },
    { "PFAccountManagementClientGetPlayFabIDsFromSteamNamesAsync", HandlePFAccountManagementClientGetPlayFabIDsFromSteamNamesAsync },
    { "PFAccountManagementClientGetPlayFabIDsFromSteamNamesGetResult", HandlePFAccountManagementClientGetPlayFabIDsFromSteamNamesGetResult },
    { "PFAccountManagementClientGetPlayFabIDsFromSteamNamesGetResultSize", HandlePFAccountManagementClientGetPlayFabIDsFromSteamNamesGetResultSize },
    { "PFAccountManagementClientGetPlayFabIDsFromTwitchIDsAsync", HandlePFAccountManagementClientGetPlayFabIDsFromTwitchIDsAsync },
    { "PFAccountManagementClientGetPlayFabIDsFromTwitchIDsGetResult", HandlePFAccountManagementClientGetPlayFabIDsFromTwitchIDsGetResult },
    { "PFAccountManagementClientGetPlayFabIDsFromTwitchIDsGetResultSize", HandlePFAccountManagementClientGetPlayFabIDsFromTwitchIDsGetResultSize },
    { "PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsAsync", HandlePFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsAsync },
    { "PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsGetResult", HandlePFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsGetResult },
    { "PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsGetResultSize", HandlePFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsGetResultSize },
    { "PFAccountManagementClientGetPlayerCombinedInfoAsync", HandlePFAccountManagementClientGetPlayerCombinedInfoAsync },
    { "PFAccountManagementClientGetPlayerCombinedInfoGetResult", HandlePFAccountManagementClientGetPlayerCombinedInfoGetResult },
    { "PFAccountManagementClientGetPlayerCombinedInfoGetResultSize", HandlePFAccountManagementClientGetPlayerCombinedInfoGetResultSize },
    { "PFAccountManagementClientGetPlayerProfileAsync", HandlePFAccountManagementClientGetPlayerProfileAsync },
    { "PFAccountManagementClientGetPlayerProfileGetResult", HandlePFAccountManagementClientGetPlayerProfileGetResult },
    { "PFAccountManagementClientGetPlayerProfileGetResultSize", HandlePFAccountManagementClientGetPlayerProfileGetResultSize },
    { "PFAccountManagementClientLinkAndroidDeviceIDAsync", HandlePFAccountManagementClientLinkAndroidDeviceIDAsync },
    { "PFAccountManagementClientLinkAppleAsync", HandlePFAccountManagementClientLinkAppleAsync },
    { "PFAccountManagementClientLinkBattleNetAccountAsync", HandlePFAccountManagementClientLinkBattleNetAccountAsync },
    { "PFAccountManagementClientLinkCustomIDAsync", HandlePFAccountManagementClientLinkCustomIDAsync },
    { "PFAccountManagementClientLinkFacebookAccountAsync", HandlePFAccountManagementClientLinkFacebookAccountAsync },
    { "PFAccountManagementClientLinkFacebookInstantGamesIdAsync", HandlePFAccountManagementClientLinkFacebookInstantGamesIdAsync },
    { "PFAccountManagementClientLinkGameCenterAccountAsync", HandlePFAccountManagementClientLinkGameCenterAccountAsync },
    { "PFAccountManagementClientLinkGoogleAccountAsync", HandlePFAccountManagementClientLinkGoogleAccountAsync },
    { "PFAccountManagementClientLinkGooglePlayGamesServicesAccountAsync", HandlePFAccountManagementClientLinkGooglePlayGamesServicesAccountAsync },
    { "PFAccountManagementClientLinkIOSDeviceIDAsync", HandlePFAccountManagementClientLinkIOSDeviceIDAsync },
    { "PFAccountManagementClientLinkKongregateAsync", HandlePFAccountManagementClientLinkKongregateAsync },
    { "PFAccountManagementClientLinkNintendoServiceAccountAsync", HandlePFAccountManagementClientLinkNintendoServiceAccountAsync },
    { "PFAccountManagementClientLinkNintendoSwitchDeviceIdAsync", HandlePFAccountManagementClientLinkNintendoSwitchDeviceIdAsync },
    { "PFAccountManagementClientLinkOpenIdConnectAsync", HandlePFAccountManagementClientLinkOpenIdConnectAsync },
    { "PFAccountManagementClientLinkPSNAccountAsync", HandlePFAccountManagementClientLinkPSNAccountAsync },
    { "PFAccountManagementClientLinkSteamAccountAsync", HandlePFAccountManagementClientLinkSteamAccountAsync },
    { "PFAccountManagementClientLinkTwitchAsync", HandlePFAccountManagementClientLinkTwitchAsync },
    { "PFAccountManagementClientLinkXboxAccountAsync", HandlePFAccountManagementClientLinkXboxAccountAsync },
    { "PFAccountManagementClientRemoveContactEmailAsync", HandlePFAccountManagementClientRemoveContactEmailAsync },
    { "PFAccountManagementClientReportPlayerAsync", HandlePFAccountManagementClientReportPlayerAsync },
    { "PFAccountManagementClientReportPlayerGetResult", HandlePFAccountManagementClientReportPlayerGetResult },
    { "PFAccountManagementClientSendAccountRecoveryEmailAsync", HandlePFAccountManagementClientSendAccountRecoveryEmailAsync },
    { "PFAccountManagementClientUnlinkAndroidDeviceIDAsync", HandlePFAccountManagementClientUnlinkAndroidDeviceIDAsync },
    { "PFAccountManagementClientUnlinkAppleAsync", HandlePFAccountManagementClientUnlinkAppleAsync },
    { "PFAccountManagementClientUnlinkBattleNetAccountAsync", HandlePFAccountManagementClientUnlinkBattleNetAccountAsync },
    { "PFAccountManagementClientUnlinkCustomIDAsync", HandlePFAccountManagementClientUnlinkCustomIDAsync },
    { "PFAccountManagementClientUnlinkFacebookAccountAsync", HandlePFAccountManagementClientUnlinkFacebookAccountAsync },
    { "PFAccountManagementClientUnlinkFacebookInstantGamesIdAsync", HandlePFAccountManagementClientUnlinkFacebookInstantGamesIdAsync },
    { "PFAccountManagementClientUnlinkGameCenterAccountAsync", HandlePFAccountManagementClientUnlinkGameCenterAccountAsync },
    { "PFAccountManagementClientUnlinkGoogleAccountAsync", HandlePFAccountManagementClientUnlinkGoogleAccountAsync },
    { "PFAccountManagementClientUnlinkGooglePlayGamesServicesAccountAsync", HandlePFAccountManagementClientUnlinkGooglePlayGamesServicesAccountAsync },
    { "PFAccountManagementClientUnlinkIOSDeviceIDAsync", HandlePFAccountManagementClientUnlinkIOSDeviceIDAsync },
    { "PFAccountManagementClientUnlinkKongregateAsync", HandlePFAccountManagementClientUnlinkKongregateAsync },
    { "PFAccountManagementClientUnlinkNintendoServiceAccountAsync", HandlePFAccountManagementClientUnlinkNintendoServiceAccountAsync },
    { "PFAccountManagementClientUnlinkNintendoSwitchDeviceIdAsync", HandlePFAccountManagementClientUnlinkNintendoSwitchDeviceIdAsync },
    { "PFAccountManagementClientUnlinkOpenIdConnectAsync", HandlePFAccountManagementClientUnlinkOpenIdConnectAsync },
    { "PFAccountManagementClientUnlinkPSNAccountAsync", HandlePFAccountManagementClientUnlinkPSNAccountAsync },
    { "PFAccountManagementClientUnlinkSteamAccountAsync", HandlePFAccountManagementClientUnlinkSteamAccountAsync },
    { "PFAccountManagementClientUnlinkTwitchAsync", HandlePFAccountManagementClientUnlinkTwitchAsync },
    { "PFAccountManagementClientUnlinkXboxAccountAsync", HandlePFAccountManagementClientUnlinkXboxAccountAsync },
    { "PFAccountManagementClientUpdateAvatarUrlAsync", HandlePFAccountManagementClientUpdateAvatarUrlAsync },
    { "PFAccountManagementClientUpdateUserTitleDisplayNameAsync", HandlePFAccountManagementClientUpdateUserTitleDisplayNameAsync },
    { "PFAccountManagementClientUpdateUserTitleDisplayNameGetResult", HandlePFAccountManagementClientUpdateUserTitleDisplayNameGetResult },
    { "PFAccountManagementClientUpdateUserTitleDisplayNameGetResultSize", HandlePFAccountManagementClientUpdateUserTitleDisplayNameGetResultSize },
    { "PFAccountManagementGetTitlePlayersFromXboxLiveIDsAsync", HandlePFAccountManagementGetTitlePlayersFromXboxLiveIDsAsync },
    { "PFAccountManagementGetTitlePlayersFromXboxLiveIDsGetResult", HandlePFAccountManagementGetTitlePlayersFromXboxLiveIDsGetResult },
    { "PFAccountManagementGetTitlePlayersFromXboxLiveIDsGetResultSize", HandlePFAccountManagementGetTitlePlayersFromXboxLiveIDsGetResultSize },
    { "PFAccountManagementServerBanUsersAsync", HandlePFAccountManagementServerBanUsersAsync },
    { "PFAccountManagementServerBanUsersGetResult", HandlePFAccountManagementServerBanUsersGetResult },
    { "PFAccountManagementServerBanUsersGetResultSize", HandlePFAccountManagementServerBanUsersGetResultSize },
    { "PFAccountManagementServerDeletePlayerAsync", HandlePFAccountManagementServerDeletePlayerAsync },
    { "PFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsAsync", HandlePFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsAsync },
    { "PFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsGetResult", HandlePFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsGetResult },
    { "PFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsGetResultSize", HandlePFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsGetResultSize },
    { "PFAccountManagementServerGetPlayFabIDsFromFacebookIDsAsync", HandlePFAccountManagementServerGetPlayFabIDsFromFacebookIDsAsync },
    { "PFAccountManagementServerGetPlayFabIDsFromFacebookIDsGetResult", HandlePFAccountManagementServerGetPlayFabIDsFromFacebookIDsGetResult },
    { "PFAccountManagementServerGetPlayFabIDsFromFacebookIDsGetResultSize", HandlePFAccountManagementServerGetPlayFabIDsFromFacebookIDsGetResultSize },
    { "PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsAsync", HandlePFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsAsync },
    { "PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsGetResult", HandlePFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsGetResult },
    { "PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsGetResultSize", HandlePFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsGetResultSize },
    { "PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsAsync", HandlePFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsAsync },
    { "PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsGetResult", HandlePFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsGetResult },
    { "PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsGetResultSize", HandlePFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsGetResultSize },
    { "PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync", HandlePFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync },
    { "PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResult", HandlePFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResult },
    { "PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResultSize", HandlePFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResultSize },
    { "PFAccountManagementServerGetPlayFabIDsFromOpenIdSubjectIdentifiersAsync", HandlePFAccountManagementServerGetPlayFabIDsFromOpenIdSubjectIdentifiersAsync },
    { "PFAccountManagementServerGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResult", HandlePFAccountManagementServerGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResult },
    { "PFAccountManagementServerGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResultSize", HandlePFAccountManagementServerGetPlayFabIDsFromOpenIdSubjectIdentifiersGetResultSize },
    { "PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsAsync", HandlePFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsAsync },
    { "PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsGetResult", HandlePFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsGetResult },
    { "PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsGetResultSize", HandlePFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsGetResultSize },
    { "PFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsAsync", HandlePFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsAsync },
    { "PFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsGetResult", HandlePFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsGetResult },
    { "PFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsGetResultSize", HandlePFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsGetResultSize },
    { "PFAccountManagementServerGetPlayFabIDsFromSteamIDsAsync", HandlePFAccountManagementServerGetPlayFabIDsFromSteamIDsAsync },
    { "PFAccountManagementServerGetPlayFabIDsFromSteamIDsGetResult", HandlePFAccountManagementServerGetPlayFabIDsFromSteamIDsGetResult },
    { "PFAccountManagementServerGetPlayFabIDsFromSteamIDsGetResultSize", HandlePFAccountManagementServerGetPlayFabIDsFromSteamIDsGetResultSize },
    { "PFAccountManagementServerGetPlayFabIDsFromSteamNamesAsync", HandlePFAccountManagementServerGetPlayFabIDsFromSteamNamesAsync },
    { "PFAccountManagementServerGetPlayFabIDsFromSteamNamesGetResult", HandlePFAccountManagementServerGetPlayFabIDsFromSteamNamesGetResult },
    { "PFAccountManagementServerGetPlayFabIDsFromSteamNamesGetResultSize", HandlePFAccountManagementServerGetPlayFabIDsFromSteamNamesGetResultSize },
    { "PFAccountManagementServerGetPlayFabIDsFromTwitchIDsAsync", HandlePFAccountManagementServerGetPlayFabIDsFromTwitchIDsAsync },
    { "PFAccountManagementServerGetPlayFabIDsFromTwitchIDsGetResult", HandlePFAccountManagementServerGetPlayFabIDsFromTwitchIDsGetResult },
    { "PFAccountManagementServerGetPlayFabIDsFromTwitchIDsGetResultSize", HandlePFAccountManagementServerGetPlayFabIDsFromTwitchIDsGetResultSize },
    { "PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsAsync", HandlePFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsAsync },
    { "PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsGetResult", HandlePFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsGetResult },
    { "PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsGetResultSize", HandlePFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsGetResultSize },
    { "PFAccountManagementServerGetPlayerCombinedInfoAsync", HandlePFAccountManagementServerGetPlayerCombinedInfoAsync },
    { "PFAccountManagementServerGetPlayerCombinedInfoGetResult", HandlePFAccountManagementServerGetPlayerCombinedInfoGetResult },
    { "PFAccountManagementServerGetPlayerCombinedInfoGetResultSize", HandlePFAccountManagementServerGetPlayerCombinedInfoGetResultSize },
    { "PFAccountManagementServerGetPlayerProfileAsync", HandlePFAccountManagementServerGetPlayerProfileAsync },
    { "PFAccountManagementServerGetPlayerProfileGetResult", HandlePFAccountManagementServerGetPlayerProfileGetResult },
    { "PFAccountManagementServerGetPlayerProfileGetResultSize", HandlePFAccountManagementServerGetPlayerProfileGetResultSize },
    { "PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsAsync", HandlePFAccountManagementServerGetServerCustomIDsFromPlayFabIDsAsync },
    { "PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsGetResult", HandlePFAccountManagementServerGetServerCustomIDsFromPlayFabIDsGetResult },
    { "PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsGetResultSize", HandlePFAccountManagementServerGetServerCustomIDsFromPlayFabIDsGetResultSize },
    { "PFAccountManagementServerGetUserAccountInfoAsync", HandlePFAccountManagementServerGetUserAccountInfoAsync },
    { "PFAccountManagementServerGetUserAccountInfoGetResult", HandlePFAccountManagementServerGetUserAccountInfoGetResult },
    { "PFAccountManagementServerGetUserAccountInfoGetResultSize", HandlePFAccountManagementServerGetUserAccountInfoGetResultSize },
    { "PFAccountManagementServerGetUserBansAsync", HandlePFAccountManagementServerGetUserBansAsync },
    { "PFAccountManagementServerGetUserBansGetResult", HandlePFAccountManagementServerGetUserBansGetResult },
    { "PFAccountManagementServerGetUserBansGetResultSize", HandlePFAccountManagementServerGetUserBansGetResultSize },
    { "PFAccountManagementServerLinkBattleNetAccountAsync", HandlePFAccountManagementServerLinkBattleNetAccountAsync },
    { "PFAccountManagementServerLinkNintendoServiceAccountAsync", HandlePFAccountManagementServerLinkNintendoServiceAccountAsync },
    { "PFAccountManagementServerLinkNintendoServiceAccountSubjectAsync", HandlePFAccountManagementServerLinkNintendoServiceAccountSubjectAsync },
    { "PFAccountManagementServerLinkNintendoSwitchDeviceIdAsync", HandlePFAccountManagementServerLinkNintendoSwitchDeviceIdAsync },
    { "PFAccountManagementServerLinkPSNAccountAsync", HandlePFAccountManagementServerLinkPSNAccountAsync },
    { "PFAccountManagementServerLinkPSNIdAsync", HandlePFAccountManagementServerLinkPSNIdAsync },
    { "PFAccountManagementServerLinkServerCustomIdAsync", HandlePFAccountManagementServerLinkServerCustomIdAsync },
    { "PFAccountManagementServerLinkSteamIdAsync", HandlePFAccountManagementServerLinkSteamIdAsync },
    { "PFAccountManagementServerLinkTwitchAccountAsync", HandlePFAccountManagementServerLinkTwitchAccountAsync },
    { "PFAccountManagementServerLinkXboxAccountAsync", HandlePFAccountManagementServerLinkXboxAccountAsync },
    { "PFAccountManagementServerLinkXboxIdAsync", HandlePFAccountManagementServerLinkXboxIdAsync },
    { "PFAccountManagementServerRevokeAllBansForUserAsync", HandlePFAccountManagementServerRevokeAllBansForUserAsync },
    { "PFAccountManagementServerRevokeAllBansForUserGetResult", HandlePFAccountManagementServerRevokeAllBansForUserGetResult },
    { "PFAccountManagementServerRevokeAllBansForUserGetResultSize", HandlePFAccountManagementServerRevokeAllBansForUserGetResultSize },
    { "PFAccountManagementServerRevokeBansAsync", HandlePFAccountManagementServerRevokeBansAsync },
    { "PFAccountManagementServerRevokeBansGetResult", HandlePFAccountManagementServerRevokeBansGetResult },
    { "PFAccountManagementServerRevokeBansGetResultSize", HandlePFAccountManagementServerRevokeBansGetResultSize },
    { "PFAccountManagementServerSendCustomAccountRecoveryEmailAsync", HandlePFAccountManagementServerSendCustomAccountRecoveryEmailAsync },
    { "PFAccountManagementServerSendEmailFromTemplateAsync", HandlePFAccountManagementServerSendEmailFromTemplateAsync },
    { "PFAccountManagementServerUnlinkBattleNetAccountAsync", HandlePFAccountManagementServerUnlinkBattleNetAccountAsync },
    { "PFAccountManagementServerUnlinkFacebookAccountAsync", HandlePFAccountManagementServerUnlinkFacebookAccountAsync },
    { "PFAccountManagementServerUnlinkFacebookInstantGamesIdAsync", HandlePFAccountManagementServerUnlinkFacebookInstantGamesIdAsync },
    { "PFAccountManagementServerUnlinkNintendoServiceAccountAsync", HandlePFAccountManagementServerUnlinkNintendoServiceAccountAsync },
    { "PFAccountManagementServerUnlinkNintendoSwitchDeviceIdAsync", HandlePFAccountManagementServerUnlinkNintendoSwitchDeviceIdAsync },
    { "PFAccountManagementServerUnlinkPSNAccountAsync", HandlePFAccountManagementServerUnlinkPSNAccountAsync },
    { "PFAccountManagementServerUnlinkServerCustomIdAsync", HandlePFAccountManagementServerUnlinkServerCustomIdAsync },
    { "PFAccountManagementServerUnlinkSteamIdAsync", HandlePFAccountManagementServerUnlinkSteamIdAsync },
    { "PFAccountManagementServerUnlinkTwitchAccountAsync", HandlePFAccountManagementServerUnlinkTwitchAccountAsync },
    { "PFAccountManagementServerUnlinkXboxAccountAsync", HandlePFAccountManagementServerUnlinkXboxAccountAsync },
    { "PFAccountManagementServerUpdateAvatarUrlAsync", HandlePFAccountManagementServerUpdateAvatarUrlAsync },
    { "PFAccountManagementServerUpdateBansAsync", HandlePFAccountManagementServerUpdateBansAsync },
    { "PFAccountManagementServerUpdateBansGetResult", HandlePFAccountManagementServerUpdateBansGetResult },
    { "PFAccountManagementServerUpdateBansGetResultSize", HandlePFAccountManagementServerUpdateBansGetResultSize },
    { "PFAccountManagementSetDisplayNameAsync", HandlePFAccountManagementSetDisplayNameAsync },
    { "PFAccountManagementSetDisplayNameGetResult", HandlePFAccountManagementSetDisplayNameGetResult },
    { "PFAccountManagementSetDisplayNameGetResultSize", HandlePFAccountManagementSetDisplayNameGetResultSize }
});
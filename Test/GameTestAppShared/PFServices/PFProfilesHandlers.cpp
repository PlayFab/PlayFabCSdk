#include "pch.h"
#include "PFProfilesHandlers.h"
#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <playfab/services/PFProfiles.h>
#include <playfab/core/PFLocalUser.h>
#include <vector>
#include "CommandRegistry.h"

using CommandHandlerShared::TryGetStringParameter;
using CommandHandlerShared::TryParseBoolParameter;

CommandResultPayload HandlePFProfilesGetProfileAsync(
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
            PFProfilesGetEntityProfileRequest request{};
            const HRESULT hr = PFProfilesGetProfileAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFProfilesGetProfileAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFProfilesGetProfileGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFProfilesGetEntityProfileResponse* result{ nullptr };
            HRESULT hr = PFProfilesGetProfileGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFProfilesGetProfileAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFProfilesGetProfileGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFProfilesGetProfileGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFProfilesGetProfileGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFProfilesGetProfileGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFProfilesGetProfilesAsync(
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
            PFProfilesGetEntityProfilesRequest request{};
            PFEntityKey selfKey{ state->entityId.c_str(), state->entityType.c_str() };
            const PFEntityKey* entityKeyPtr = &selfKey;
            request.entities = &entityKeyPtr;
            request.entitiesCount = 1;
            const HRESULT hr = PFProfilesGetProfilesAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFProfilesGetProfilesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFProfilesGetProfilesGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFProfilesGetEntityProfilesResponse* result{ nullptr };
            HRESULT hr = PFProfilesGetProfilesGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFProfilesGetProfilesAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFProfilesGetProfilesGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFProfilesGetProfilesGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFProfilesGetProfilesGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFProfilesGetProfilesGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFProfilesGetTitlePlayersFromMasterPlayerAccountIdsAsync(
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
            PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsRequest request{};
            const char* masterIds[] = { state->entityId.c_str() };
            request.masterPlayerAccountIds = masterIds;
            request.masterPlayerAccountIdsCount = 1;
            const HRESULT hr = PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsResponse* result{ nullptr };
            HRESULT hr = PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFProfilesGetTitlePlayersFromMasterPlayerAccountIdsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFProfilesGetTitlePlayersFromMasterPlayerAccountIdsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFProfilesSetProfileLanguageAsync(
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
            PFProfilesSetProfileLanguageRequest request{};
            const HRESULT hr = PFProfilesSetProfileLanguageAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFProfilesSetProfileLanguageAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFProfilesSetProfileLanguageGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFProfilesSetProfileLanguageResponse* result{ nullptr };
            HRESULT hr = PFProfilesSetProfileLanguageGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFProfilesSetProfileLanguageAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFProfilesSetProfileLanguageGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFProfilesSetProfileLanguageGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFProfilesSetProfileLanguageGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFProfilesSetProfileLanguageGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFProfilesSetProfilePolicyAsync(
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
            PFProfilesSetEntityProfilePolicyRequest request{};
            PFEntityKey entityKey{ state->entityId.c_str(), state->entityType.c_str() };
            request.entity = &entityKey;
            request.statements = nullptr;
            request.statementsCount = 0;
            const HRESULT hr = PFProfilesSetProfilePolicyAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFProfilesSetProfilePolicyAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFProfilesSetProfilePolicyGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFProfilesSetEntityProfilePolicyResponse* result{ nullptr };
            HRESULT hr = PFProfilesSetProfilePolicyGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFProfilesSetProfilePolicyAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFProfilesSetProfilePolicyGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFProfilesSetProfilePolicyGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFProfilesSetProfilePolicyGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFProfilesSetProfilePolicyGetResult: called inline by Async handler");
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "PFProfilesGetProfileAsync", HandlePFProfilesGetProfileAsync },
    { "PFProfilesGetProfileGetResult", HandlePFProfilesGetProfileGetResult },
    { "PFProfilesGetProfileGetResultSize", HandlePFProfilesGetProfileGetResultSize },
    { "PFProfilesGetProfilesAsync", HandlePFProfilesGetProfilesAsync },
    { "PFProfilesGetProfilesGetResult", HandlePFProfilesGetProfilesGetResult },
    { "PFProfilesGetProfilesGetResultSize", HandlePFProfilesGetProfilesGetResultSize },
    { "PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsAsync", HandlePFProfilesGetTitlePlayersFromMasterPlayerAccountIdsAsync },
    { "PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsGetResult", HandlePFProfilesGetTitlePlayersFromMasterPlayerAccountIdsGetResult },
    { "PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsGetResultSize", HandlePFProfilesGetTitlePlayersFromMasterPlayerAccountIdsGetResultSize },
    { "PFProfilesSetProfileLanguageAsync", HandlePFProfilesSetProfileLanguageAsync },
    { "PFProfilesSetProfileLanguageGetResult", HandlePFProfilesSetProfileLanguageGetResult },
    { "PFProfilesSetProfileLanguageGetResultSize", HandlePFProfilesSetProfileLanguageGetResultSize },
    { "PFProfilesSetProfilePolicyAsync", HandlePFProfilesSetProfilePolicyAsync },
    { "PFProfilesSetProfilePolicyGetResult", HandlePFProfilesSetProfilePolicyGetResult },
    { "PFProfilesSetProfilePolicyGetResultSize", HandlePFProfilesSetProfilePolicyGetResultSize }
});

#include "pch.h"
#include "PFFriendsHandlers.h"
#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <playfab/services/PFFriends.h>
#include <playfab/core/PFLocalUser.h>
#include <vector>
#include "CommandRegistry.h"

using CommandHandlerShared::TryGetStringParameter;
using CommandHandlerShared::TryParseBoolParameter;

CommandResultPayload HandlePFFriendsClientAddFriendAsync(
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
            PFFriendsClientAddFriendRequest request{};
            request.friendPlayFabId = "ABCDEF1234567890";
            const HRESULT hr = PFFriendsClientAddFriendAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFFriendsClientAddFriendAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFFriendsAddFriendResult result{};
            return PFFriendsClientAddFriendGetResult(&async, &result);
        });
}

CommandResultPayload HandlePFFriendsClientAddFriendGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFFriendsClientAddFriendGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFFriendsClientGetFriendsListAsync(
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
            PFFriendsClientGetFriendsListRequest request{};
            const HRESULT hr = PFFriendsClientGetFriendsListAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFFriendsClientGetFriendsListAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFFriendsClientGetFriendsListGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFFriendsGetFriendsListResult* result{ nullptr };
            HRESULT hr = PFFriendsClientGetFriendsListGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFFriendsClientGetFriendsListAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFFriendsClientGetFriendsListGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFFriendsClientGetFriendsListGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFFriendsClientGetFriendsListGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFFriendsClientGetFriendsListGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFFriendsClientRemoveFriendAsync(
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
            PFFriendsClientRemoveFriendRequest request{};
            request.friendPlayFabId = "ABCDEF1234567890";
            const HRESULT hr = PFFriendsClientRemoveFriendAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFFriendsClientRemoveFriendAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFFriendsClientSetFriendTagsAsync(
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
            PFFriendsClientSetFriendTagsRequest request{};
            request.friendPlayFabId = "ABCDEF1234567890";
            const HRESULT hr = PFFriendsClientSetFriendTagsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFFriendsClientSetFriendTagsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFFriendsServerAddFriendAsync(
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
            PFFriendsServerAddFriendRequest request{};
            const HRESULT hr = PFFriendsServerAddFriendAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFFriendsServerAddFriendAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFFriendsServerGetFriendsListAsync(
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
            PFFriendsServerGetFriendsListRequest request{};
            const HRESULT hr = PFFriendsServerGetFriendsListAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFFriendsServerGetFriendsListAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFFriendsServerGetFriendsListGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFFriendsGetFriendsListResult* result{ nullptr };
            HRESULT hr = PFFriendsServerGetFriendsListGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFFriendsServerGetFriendsListAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFFriendsServerGetFriendsListGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFFriendsServerGetFriendsListGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFFriendsServerGetFriendsListGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFFriendsServerGetFriendsListGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFFriendsServerRemoveFriendAsync(
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
            PFFriendsServerRemoveFriendRequest request{};
            const HRESULT hr = PFFriendsServerRemoveFriendAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFFriendsServerRemoveFriendAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFFriendsServerSetFriendTagsAsync(
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
            PFFriendsServerSetFriendTagsRequest request{};
            const HRESULT hr = PFFriendsServerSetFriendTagsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFFriendsServerSetFriendTagsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "PFFriendsClientAddFriendAsync", HandlePFFriendsClientAddFriendAsync },
    { "PFFriendsClientAddFriendGetResult", HandlePFFriendsClientAddFriendGetResult },
    { "PFFriendsClientGetFriendsListAsync", HandlePFFriendsClientGetFriendsListAsync },
    { "PFFriendsClientGetFriendsListGetResult", HandlePFFriendsClientGetFriendsListGetResult },
    { "PFFriendsClientGetFriendsListGetResultSize", HandlePFFriendsClientGetFriendsListGetResultSize },
    { "PFFriendsClientRemoveFriendAsync", HandlePFFriendsClientRemoveFriendAsync },
    { "PFFriendsClientSetFriendTagsAsync", HandlePFFriendsClientSetFriendTagsAsync },
    { "PFFriendsServerAddFriendAsync", HandlePFFriendsServerAddFriendAsync },
    { "PFFriendsServerGetFriendsListAsync", HandlePFFriendsServerGetFriendsListAsync },
    { "PFFriendsServerGetFriendsListGetResult", HandlePFFriendsServerGetFriendsListGetResult },
    { "PFFriendsServerGetFriendsListGetResultSize", HandlePFFriendsServerGetFriendsListGetResultSize },
    { "PFFriendsServerRemoveFriendAsync", HandlePFFriendsServerRemoveFriendAsync },
    { "PFFriendsServerSetFriendTagsAsync", HandlePFFriendsServerSetFriendTagsAsync }
});
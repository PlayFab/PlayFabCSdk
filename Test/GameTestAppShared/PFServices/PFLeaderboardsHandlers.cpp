#include "pch.h"
#include "PFLeaderboardsHandlers.h"
#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <playfab/services/PFLeaderboards.h>
#include <playfab/core/PFLocalUser.h>
#include <vector>
#include "CommandRegistry.h"

using CommandHandlerShared::TryGetStringParameter;
using CommandHandlerShared::TryParseBoolParameter;

CommandResultPayload HandlePFLeaderboardsCreateLeaderboardDefinitionAsync(
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
            PFLeaderboardsCreateLeaderboardDefinitionRequest request{};
            const HRESULT hr = PFLeaderboardsCreateLeaderboardDefinitionAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFLeaderboardsCreateLeaderboardDefinitionAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFLeaderboardsDeleteLeaderboardDefinitionAsync(
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
            PFLeaderboardsDeleteLeaderboardDefinitionRequest request{};
            const HRESULT hr = PFLeaderboardsDeleteLeaderboardDefinitionAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFLeaderboardsDeleteLeaderboardDefinitionAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFLeaderboardsDeleteLeaderboardEntriesAsync(
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
            PFLeaderboardsDeleteLeaderboardEntriesRequest request{};
            const HRESULT hr = PFLeaderboardsDeleteLeaderboardEntriesAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFLeaderboardsDeleteLeaderboardEntriesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFLeaderboardsGetFriendLeaderboardForEntityAsync(
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
            PFLeaderboardsGetFriendLeaderboardForEntityRequest request{};
            const HRESULT hr = PFLeaderboardsGetFriendLeaderboardForEntityAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFLeaderboardsGetFriendLeaderboardForEntityAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFLeaderboardsGetFriendLeaderboardForEntityGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFLeaderboardsGetEntityLeaderboardResponse* result{ nullptr };
            HRESULT hr = PFLeaderboardsGetFriendLeaderboardForEntityGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFLeaderboardsGetFriendLeaderboardForEntityAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFLeaderboardsGetFriendLeaderboardForEntityGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFLeaderboardsGetFriendLeaderboardForEntityGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFLeaderboardsGetFriendLeaderboardForEntityGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFLeaderboardsGetFriendLeaderboardForEntityGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFLeaderboardsGetLeaderboardAsync(
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
            PFLeaderboardsGetEntityLeaderboardRequest request{};
            const HRESULT hr = PFLeaderboardsGetLeaderboardAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFLeaderboardsGetLeaderboardAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFLeaderboardsGetLeaderboardGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFLeaderboardsGetEntityLeaderboardResponse* result{ nullptr };
            HRESULT hr = PFLeaderboardsGetLeaderboardGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFLeaderboardsGetLeaderboardAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFLeaderboardsGetLeaderboardGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFLeaderboardsGetLeaderboardGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFLeaderboardsGetLeaderboardGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFLeaderboardsGetLeaderboardGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFLeaderboardsGetLeaderboardAroundEntityAsync(
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
            PFLeaderboardsGetLeaderboardAroundEntityRequest request{};
            const HRESULT hr = PFLeaderboardsGetLeaderboardAroundEntityAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFLeaderboardsGetLeaderboardAroundEntityAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFLeaderboardsGetLeaderboardAroundEntityGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFLeaderboardsGetEntityLeaderboardResponse* result{ nullptr };
            HRESULT hr = PFLeaderboardsGetLeaderboardAroundEntityGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFLeaderboardsGetLeaderboardAroundEntityAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFLeaderboardsGetLeaderboardAroundEntityGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFLeaderboardsGetLeaderboardAroundEntityGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFLeaderboardsGetLeaderboardAroundEntityGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFLeaderboardsGetLeaderboardAroundEntityGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFLeaderboardsGetLeaderboardDefinitionAsync(
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
            PFLeaderboardsGetLeaderboardDefinitionRequest request{};
            const HRESULT hr = PFLeaderboardsGetLeaderboardDefinitionAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFLeaderboardsGetLeaderboardDefinitionAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFLeaderboardsGetLeaderboardDefinitionGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFLeaderboardsGetLeaderboardDefinitionResponse* result{ nullptr };
            HRESULT hr = PFLeaderboardsGetLeaderboardDefinitionGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFLeaderboardsGetLeaderboardDefinitionAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFLeaderboardsGetLeaderboardDefinitionGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFLeaderboardsGetLeaderboardDefinitionGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFLeaderboardsGetLeaderboardDefinitionGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFLeaderboardsGetLeaderboardDefinitionGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFLeaderboardsGetLeaderboardForEntitiesAsync(
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
            PFLeaderboardsGetLeaderboardForEntitiesRequest request{};
            const HRESULT hr = PFLeaderboardsGetLeaderboardForEntitiesAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFLeaderboardsGetLeaderboardForEntitiesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFLeaderboardsGetLeaderboardForEntitiesGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFLeaderboardsGetEntityLeaderboardResponse* result{ nullptr };
            HRESULT hr = PFLeaderboardsGetLeaderboardForEntitiesGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFLeaderboardsGetLeaderboardForEntitiesAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFLeaderboardsGetLeaderboardForEntitiesGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFLeaderboardsGetLeaderboardForEntitiesGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFLeaderboardsGetLeaderboardForEntitiesGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFLeaderboardsGetLeaderboardForEntitiesGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFLeaderboardsIncrementLeaderboardVersionAsync(
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
            PFLeaderboardsIncrementLeaderboardVersionRequest request{};
            const HRESULT hr = PFLeaderboardsIncrementLeaderboardVersionAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFLeaderboardsIncrementLeaderboardVersionAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFLeaderboardsIncrementLeaderboardVersionResponse result{};
            return PFLeaderboardsIncrementLeaderboardVersionGetResult(&async, &result);
        });
}

CommandResultPayload HandlePFLeaderboardsIncrementLeaderboardVersionGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFLeaderboardsIncrementLeaderboardVersionGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFLeaderboardsListLeaderboardDefinitionsAsync(
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
            PFLeaderboardsListLeaderboardDefinitionsRequest request{};
            const HRESULT hr = PFLeaderboardsListLeaderboardDefinitionsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFLeaderboardsListLeaderboardDefinitionsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFLeaderboardsListLeaderboardDefinitionsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFLeaderboardsListLeaderboardDefinitionsResponse* result{ nullptr };
            HRESULT hr = PFLeaderboardsListLeaderboardDefinitionsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFLeaderboardsListLeaderboardDefinitionsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFLeaderboardsListLeaderboardDefinitionsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFLeaderboardsListLeaderboardDefinitionsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFLeaderboardsListLeaderboardDefinitionsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFLeaderboardsListLeaderboardDefinitionsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFLeaderboardsUnlinkLeaderboardFromStatisticAsync(
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
            PFLeaderboardsUnlinkLeaderboardFromStatisticRequest request{};
            const HRESULT hr = PFLeaderboardsUnlinkLeaderboardFromStatisticAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFLeaderboardsUnlinkLeaderboardFromStatisticAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFLeaderboardsUpdateLeaderboardDefinitionAsync(
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
            PFLeaderboardsUpdateLeaderboardDefinitionRequest request{};
            const HRESULT hr = PFLeaderboardsUpdateLeaderboardDefinitionAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFLeaderboardsUpdateLeaderboardDefinitionAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFLeaderboardsUpdateLeaderboardEntriesAsync(
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
            PFLeaderboardsUpdateLeaderboardEntriesRequest request{};
            const HRESULT hr = PFLeaderboardsUpdateLeaderboardEntriesAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFLeaderboardsUpdateLeaderboardEntriesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "PFLeaderboardsCreateLeaderboardDefinitionAsync", HandlePFLeaderboardsCreateLeaderboardDefinitionAsync },
    { "PFLeaderboardsDeleteLeaderboardDefinitionAsync", HandlePFLeaderboardsDeleteLeaderboardDefinitionAsync },
    { "PFLeaderboardsDeleteLeaderboardEntriesAsync", HandlePFLeaderboardsDeleteLeaderboardEntriesAsync },
    { "PFLeaderboardsGetFriendLeaderboardForEntityAsync", HandlePFLeaderboardsGetFriendLeaderboardForEntityAsync },
    { "PFLeaderboardsGetFriendLeaderboardForEntityGetResult", HandlePFLeaderboardsGetFriendLeaderboardForEntityGetResult },
    { "PFLeaderboardsGetFriendLeaderboardForEntityGetResultSize", HandlePFLeaderboardsGetFriendLeaderboardForEntityGetResultSize },
    { "PFLeaderboardsGetLeaderboardAroundEntityAsync", HandlePFLeaderboardsGetLeaderboardAroundEntityAsync },
    { "PFLeaderboardsGetLeaderboardAroundEntityGetResult", HandlePFLeaderboardsGetLeaderboardAroundEntityGetResult },
    { "PFLeaderboardsGetLeaderboardAroundEntityGetResultSize", HandlePFLeaderboardsGetLeaderboardAroundEntityGetResultSize },
    { "PFLeaderboardsGetLeaderboardAsync", HandlePFLeaderboardsGetLeaderboardAsync },
    { "PFLeaderboardsGetLeaderboardDefinitionAsync", HandlePFLeaderboardsGetLeaderboardDefinitionAsync },
    { "PFLeaderboardsGetLeaderboardDefinitionGetResult", HandlePFLeaderboardsGetLeaderboardDefinitionGetResult },
    { "PFLeaderboardsGetLeaderboardDefinitionGetResultSize", HandlePFLeaderboardsGetLeaderboardDefinitionGetResultSize },
    { "PFLeaderboardsGetLeaderboardForEntitiesAsync", HandlePFLeaderboardsGetLeaderboardForEntitiesAsync },
    { "PFLeaderboardsGetLeaderboardForEntitiesGetResult", HandlePFLeaderboardsGetLeaderboardForEntitiesGetResult },
    { "PFLeaderboardsGetLeaderboardForEntitiesGetResultSize", HandlePFLeaderboardsGetLeaderboardForEntitiesGetResultSize },
    { "PFLeaderboardsGetLeaderboardGetResult", HandlePFLeaderboardsGetLeaderboardGetResult },
    { "PFLeaderboardsGetLeaderboardGetResultSize", HandlePFLeaderboardsGetLeaderboardGetResultSize },
    { "PFLeaderboardsIncrementLeaderboardVersionAsync", HandlePFLeaderboardsIncrementLeaderboardVersionAsync },
    { "PFLeaderboardsIncrementLeaderboardVersionGetResult", HandlePFLeaderboardsIncrementLeaderboardVersionGetResult },
    { "PFLeaderboardsListLeaderboardDefinitionsAsync", HandlePFLeaderboardsListLeaderboardDefinitionsAsync },
    { "PFLeaderboardsListLeaderboardDefinitionsGetResult", HandlePFLeaderboardsListLeaderboardDefinitionsGetResult },
    { "PFLeaderboardsListLeaderboardDefinitionsGetResultSize", HandlePFLeaderboardsListLeaderboardDefinitionsGetResultSize },
    { "PFLeaderboardsUnlinkLeaderboardFromStatisticAsync", HandlePFLeaderboardsUnlinkLeaderboardFromStatisticAsync },
    { "PFLeaderboardsUpdateLeaderboardDefinitionAsync", HandlePFLeaderboardsUpdateLeaderboardDefinitionAsync },
    { "PFLeaderboardsUpdateLeaderboardEntriesAsync", HandlePFLeaderboardsUpdateLeaderboardEntriesAsync }
});

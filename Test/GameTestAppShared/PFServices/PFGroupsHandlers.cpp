#include "pch.h"
#include "PFGroupsHandlers.h"
#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include <playfab/services/PFGroups.h>
#include <playfab/core/PFLocalUser.h>
#include <vector>
#include "CommandRegistry.h"

using CommandHandlerShared::TryGetStringParameter;
using CommandHandlerShared::TryParseBoolParameter;

CommandResultPayload HandlePFGroupsAcceptGroupApplicationAsync(
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
            PFGroupsAcceptGroupApplicationRequest request{};
            PFEntityKey groupKey{ state->lastGroupId.c_str(), state->lastGroupType.c_str() };
            PFEntityKey entityKey{ state->entityId.c_str(), state->entityType.c_str() };
            request.group = &groupKey;
            request.entity = &entityKey;
            const HRESULT hr = PFGroupsAcceptGroupApplicationAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsAcceptGroupApplicationAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFGroupsAcceptGroupInvitationAsync(
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
            PFGroupsAcceptGroupInvitationRequest request{};
            PFEntityKey groupKey{ state->lastGroupId.c_str(), state->lastGroupType.c_str() };
            request.group = &groupKey;
            const HRESULT hr = PFGroupsAcceptGroupInvitationAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsAcceptGroupInvitationAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFGroupsAddMembersAsync(
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
            PFGroupsAddMembersRequest request{};
            PFEntityKey groupKey{ state->lastGroupId.c_str(), state->lastGroupType.c_str() };
            request.group = &groupKey;
            const HRESULT hr = PFGroupsAddMembersAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsAddMembersAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFGroupsApplyToGroupAsync(
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
            PFGroupsApplyToGroupRequest request{};
            PFEntityKey groupKey{ state->lastGroupId.c_str(), state->lastGroupType.c_str() };
            request.group = &groupKey;
            const HRESULT hr = PFGroupsApplyToGroupAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsApplyToGroupAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFGroupsApplyToGroupGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFGroupsApplyToGroupResponse* result{ nullptr };
            HRESULT hr = PFGroupsApplyToGroupGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFGroupsApplyToGroupAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFGroupsApplyToGroupGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsApplyToGroupGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsApplyToGroupGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsApplyToGroupGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsBlockEntityAsync(
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
            PFGroupsBlockEntityRequest request{};
            PFEntityKey groupKey{ state->lastGroupId.c_str(), state->lastGroupType.c_str() };
            PFEntityKey entityKey{ state->entityId.c_str(), state->entityType.c_str() };
            request.group = &groupKey;
            request.entity = &entityKey;
            const HRESULT hr = PFGroupsBlockEntityAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsBlockEntityAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFGroupsChangeMemberRoleAsync(
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
            PFGroupsChangeMemberRoleRequest request{};
            PFEntityKey groupKey{ state->lastGroupId.c_str(), state->lastGroupType.c_str() };
            request.group = &groupKey;
            request.originRoleId = "members";
            const HRESULT hr = PFGroupsChangeMemberRoleAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsChangeMemberRoleAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFGroupsCreateGroupAsync(
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
            PFGroupsCreateGroupRequest request{};
            auto now = std::chrono::system_clock::now().time_since_epoch();
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
            std::string groupName = "TestGroup_" + state->currentScenarioId + "_" + std::to_string(ms);
            request.groupName = groupName.c_str();
            const HRESULT hr = PFGroupsCreateGroupAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsCreateGroupAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFGroupsCreateGroupGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFGroupsCreateGroupResponse* result{ nullptr };
            HRESULT hr = PFGroupsCreateGroupGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFGroupsCreateGroupAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            if (SUCCEEDED(hr) && result && result->group)
            {
                state->lastGroupId = result->group->id ? result->group->id : "";
                state->lastGroupType = result->group->type ? result->group->type : "";
                LogToWindowFormat("PFGroupsCreateGroupAsync: groupId=%s", state->lastGroupId.c_str());
            }
            return hr;
        });
}

CommandResultPayload HandlePFGroupsCreateGroupGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsCreateGroupGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsCreateGroupGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsCreateGroupGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsCreateRoleAsync(
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
            PFGroupsCreateGroupRoleRequest request{};
            PFEntityKey groupKey{ state->lastGroupId.c_str(), state->lastGroupType.c_str() };
            request.group = &groupKey;
            request.roleId = "testrole";
            request.roleName = "TestRole";
            const HRESULT hr = PFGroupsCreateRoleAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsCreateRoleAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFGroupsCreateRoleGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFGroupsCreateGroupRoleResponse* result{ nullptr };
            HRESULT hr = PFGroupsCreateRoleGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFGroupsCreateRoleAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFGroupsCreateRoleGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsCreateRoleGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsCreateRoleGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsCreateRoleGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsDeleteGroupAsync(
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
            PFGroupsDeleteGroupRequest request{};
            PFEntityKey groupKey{ state->lastGroupId.c_str(), state->lastGroupType.c_str() };
            request.group = &groupKey;
            const HRESULT hr = PFGroupsDeleteGroupAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsDeleteGroupAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFGroupsDeleteRoleAsync(
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
            PFGroupsDeleteRoleRequest request{};
            PFEntityKey groupKey{ state->lastGroupId.c_str(), state->lastGroupType.c_str() };
            request.group = &groupKey;
            request.roleId = "testrole";
            const HRESULT hr = PFGroupsDeleteRoleAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsDeleteRoleAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFGroupsGetGroupAsync(
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
            PFGroupsGetGroupRequest request{};
            std::string groupName = "TestGroup_" + state->currentScenarioId;
            request.groupName = groupName.c_str();
            const HRESULT hr = PFGroupsGetGroupAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsGetGroupAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFGroupsGetGroupGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFGroupsGetGroupResponse* result{ nullptr };
            HRESULT hr = PFGroupsGetGroupGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFGroupsGetGroupAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFGroupsGetGroupGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsGetGroupGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsGetGroupGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsGetGroupGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsInviteToGroupAsync(
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
            PFGroupsInviteToGroupRequest request{};
            PFEntityKey groupKey{ state->lastGroupId.c_str(), state->lastGroupType.c_str() };
            PFEntityKey entityKey{ state->entityId.c_str(), state->entityType.c_str() };
            request.group = &groupKey;
            request.entity = &entityKey;
            const HRESULT hr = PFGroupsInviteToGroupAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsInviteToGroupAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFGroupsInviteToGroupGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFGroupsInviteToGroupResponse* result{ nullptr };
            HRESULT hr = PFGroupsInviteToGroupGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFGroupsInviteToGroupAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFGroupsInviteToGroupGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsInviteToGroupGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsInviteToGroupGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsInviteToGroupGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsIsMemberAsync(
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
            PFGroupsIsMemberRequest request{};
            PFEntityKey entityKey{ state->entityId.c_str(), state->entityType.c_str() };
            PFEntityKey groupKey{ state->entityId.c_str(), "group" };
            request.entity = &entityKey;
            request.group = &groupKey;
            const HRESULT hr = PFGroupsIsMemberAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsIsMemberAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            PFGroupsIsMemberResponse result{};
            return PFGroupsIsMemberGetResult(&async, &result);
        });
}

CommandResultPayload HandlePFGroupsIsMemberGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsIsMemberGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsListGroupApplicationsAsync(
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
            PFGroupsListGroupApplicationsRequest request{};
            PFEntityKey groupKey{ state->lastGroupId.c_str(), state->lastGroupType.c_str() };
            request.group = &groupKey;
            const HRESULT hr = PFGroupsListGroupApplicationsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsListGroupApplicationsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFGroupsListGroupApplicationsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFGroupsListGroupApplicationsResponse* result{ nullptr };
            HRESULT hr = PFGroupsListGroupApplicationsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFGroupsListGroupApplicationsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFGroupsListGroupApplicationsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsListGroupApplicationsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsListGroupApplicationsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsListGroupApplicationsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsListGroupBlocksAsync(
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
            PFGroupsListGroupBlocksRequest request{};
            PFEntityKey groupKey{ state->lastGroupId.c_str(), state->lastGroupType.c_str() };
            request.group = &groupKey;
            const HRESULT hr = PFGroupsListGroupBlocksAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsListGroupBlocksAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFGroupsListGroupBlocksGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFGroupsListGroupBlocksResponse* result{ nullptr };
            HRESULT hr = PFGroupsListGroupBlocksGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFGroupsListGroupBlocksAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFGroupsListGroupBlocksGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsListGroupBlocksGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsListGroupBlocksGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsListGroupBlocksGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsListGroupInvitationsAsync(
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
            PFGroupsListGroupInvitationsRequest request{};
            PFEntityKey groupKey{ state->lastGroupId.c_str(), state->lastGroupType.c_str() };
            request.group = &groupKey;
            const HRESULT hr = PFGroupsListGroupInvitationsAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsListGroupInvitationsAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFGroupsListGroupInvitationsGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFGroupsListGroupInvitationsResponse* result{ nullptr };
            HRESULT hr = PFGroupsListGroupInvitationsGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFGroupsListGroupInvitationsAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFGroupsListGroupInvitationsGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsListGroupInvitationsGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsListGroupInvitationsGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsListGroupInvitationsGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsListGroupMembersAsync(
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
            PFGroupsListGroupMembersRequest request{};
            PFEntityKey groupKey{ state->lastGroupId.c_str(), state->lastGroupType.c_str() };
            request.group = &groupKey;
            const HRESULT hr = PFGroupsListGroupMembersAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsListGroupMembersAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFGroupsListGroupMembersGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFGroupsListGroupMembersResponse* result{ nullptr };
            HRESULT hr = PFGroupsListGroupMembersGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFGroupsListGroupMembersAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFGroupsListGroupMembersGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsListGroupMembersGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsListGroupMembersGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsListGroupMembersGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsListMembershipAsync(
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
            PFGroupsListMembershipRequest request{};
            const HRESULT hr = PFGroupsListMembershipAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsListMembershipAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFGroupsListMembershipGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFGroupsListMembershipResponse* result{ nullptr };
            HRESULT hr = PFGroupsListMembershipGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFGroupsListMembershipAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFGroupsListMembershipGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsListMembershipGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsListMembershipGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsListMembershipGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsListMembershipOpportunitiesAsync(
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
            PFGroupsListMembershipOpportunitiesRequest request{};
            const HRESULT hr = PFGroupsListMembershipOpportunitiesAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsListMembershipOpportunitiesAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFGroupsListMembershipOpportunitiesGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFGroupsListMembershipOpportunitiesResponse* result{ nullptr };
            HRESULT hr = PFGroupsListMembershipOpportunitiesGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFGroupsListMembershipOpportunitiesAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFGroupsListMembershipOpportunitiesGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsListMembershipOpportunitiesGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsListMembershipOpportunitiesGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsListMembershipOpportunitiesGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsRemoveGroupApplicationAsync(
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
            PFGroupsRemoveGroupApplicationRequest request{};
            PFEntityKey groupKey{ state->lastGroupId.c_str(), state->lastGroupType.c_str() };
            request.group = &groupKey;
            const HRESULT hr = PFGroupsRemoveGroupApplicationAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsRemoveGroupApplicationAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFGroupsRemoveGroupInvitationAsync(
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
            PFGroupsRemoveGroupInvitationRequest request{};
            PFEntityKey groupKey{ state->lastGroupId.c_str(), state->lastGroupType.c_str() };
            request.group = &groupKey;
            const HRESULT hr = PFGroupsRemoveGroupInvitationAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsRemoveGroupInvitationAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFGroupsRemoveMembersAsync(
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
            PFGroupsRemoveMembersRequest request{};
            PFEntityKey groupKey{ state->lastGroupId.c_str(), state->lastGroupType.c_str() };
            request.group = &groupKey;
            const HRESULT hr = PFGroupsRemoveMembersAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsRemoveMembersAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFGroupsUnblockEntityAsync(
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
            PFGroupsUnblockEntityRequest request{};
            PFEntityKey groupKey{ state->lastGroupId.c_str(), state->lastGroupType.c_str() };
            PFEntityKey entityKey{ state->entityId.c_str(), state->entityType.c_str() };
            request.group = &groupKey;
            request.entity = &entityKey;
            const HRESULT hr = PFGroupsUnblockEntityAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsUnblockEntityAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [](XAsyncBlock&, CommandResultPayload&) -> HRESULT { return S_OK; });
}

CommandResultPayload HandlePFGroupsUpdateGroupAsync(
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
            PFGroupsUpdateGroupRequest request{};
            PFEntityKey groupKey{ state->lastGroupId.c_str(), state->lastGroupType.c_str() };
            request.group = &groupKey;
            const HRESULT hr = PFGroupsUpdateGroupAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsUpdateGroupAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFGroupsUpdateGroupGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFGroupsUpdateGroupResponse* result{ nullptr };
            HRESULT hr = PFGroupsUpdateGroupGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFGroupsUpdateGroupAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFGroupsUpdateGroupGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsUpdateGroupGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsUpdateGroupGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsUpdateGroupGetResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsUpdateRoleAsync(
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
            PFGroupsUpdateGroupRoleRequest request{};
            PFEntityKey groupKey{ state->lastGroupId.c_str(), state->lastGroupType.c_str() };
            request.group = &groupKey;
            request.roleName = "TestRole";
            const HRESULT hr = PFGroupsUpdateRoleAsync(entityHandle, &request, &async);
            LogToWindowFormat("PFGroupsUpdateRoleAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            PFEntityCloseHandle(entityHandle);
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            if (FAILED(PFGroupsUpdateRoleGetResultSize(&async, &bufferSize))) { bufferSize = 0; }
            std::vector<uint8_t> buffer(bufferSize > 0 ? bufferSize : 1);
            PFGroupsUpdateGroupRoleResponse* result{ nullptr };
            HRESULT hr = PFGroupsUpdateRoleGetResult(&async, buffer.size(), buffer.data(), &result, nullptr);
            LogToWindowFormat("PFGroupsUpdateRoleAsync: result bufferSize=%zu, hr=0x%08X", bufferSize, static_cast<uint32_t>(hr));
            payload.result["bufferSize"] = bufferSize;
            return hr;
        });
}

CommandResultPayload HandlePFGroupsUpdateRoleGetResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsUpdateRoleGetResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandlePFGroupsUpdateRoleGetResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("PFGroupsUpdateRoleGetResult: called inline by Async handler");
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "PFGroupsAcceptGroupApplicationAsync", HandlePFGroupsAcceptGroupApplicationAsync },
    { "PFGroupsAcceptGroupInvitationAsync", HandlePFGroupsAcceptGroupInvitationAsync },
    { "PFGroupsAddMembersAsync", HandlePFGroupsAddMembersAsync },
    { "PFGroupsApplyToGroupAsync", HandlePFGroupsApplyToGroupAsync },
    { "PFGroupsApplyToGroupGetResult", HandlePFGroupsApplyToGroupGetResult },
    { "PFGroupsApplyToGroupGetResultSize", HandlePFGroupsApplyToGroupGetResultSize },
    { "PFGroupsBlockEntityAsync", HandlePFGroupsBlockEntityAsync },
    { "PFGroupsChangeMemberRoleAsync", HandlePFGroupsChangeMemberRoleAsync },
    { "PFGroupsCreateGroupAsync", HandlePFGroupsCreateGroupAsync },
    { "PFGroupsCreateGroupGetResult", HandlePFGroupsCreateGroupGetResult },
    { "PFGroupsCreateGroupGetResultSize", HandlePFGroupsCreateGroupGetResultSize },
    { "PFGroupsCreateRoleAsync", HandlePFGroupsCreateRoleAsync },
    { "PFGroupsCreateRoleGetResult", HandlePFGroupsCreateRoleGetResult },
    { "PFGroupsCreateRoleGetResultSize", HandlePFGroupsCreateRoleGetResultSize },
    { "PFGroupsDeleteGroupAsync", HandlePFGroupsDeleteGroupAsync },
    { "PFGroupsDeleteRoleAsync", HandlePFGroupsDeleteRoleAsync },
    { "PFGroupsGetGroupAsync", HandlePFGroupsGetGroupAsync },
    { "PFGroupsGetGroupGetResult", HandlePFGroupsGetGroupGetResult },
    { "PFGroupsGetGroupGetResultSize", HandlePFGroupsGetGroupGetResultSize },
    { "PFGroupsInviteToGroupAsync", HandlePFGroupsInviteToGroupAsync },
    { "PFGroupsInviteToGroupGetResult", HandlePFGroupsInviteToGroupGetResult },
    { "PFGroupsInviteToGroupGetResultSize", HandlePFGroupsInviteToGroupGetResultSize },
    { "PFGroupsIsMemberAsync", HandlePFGroupsIsMemberAsync },
    { "PFGroupsIsMemberGetResult", HandlePFGroupsIsMemberGetResult },
    { "PFGroupsListGroupApplicationsAsync", HandlePFGroupsListGroupApplicationsAsync },
    { "PFGroupsListGroupApplicationsGetResult", HandlePFGroupsListGroupApplicationsGetResult },
    { "PFGroupsListGroupApplicationsGetResultSize", HandlePFGroupsListGroupApplicationsGetResultSize },
    { "PFGroupsListGroupBlocksAsync", HandlePFGroupsListGroupBlocksAsync },
    { "PFGroupsListGroupBlocksGetResult", HandlePFGroupsListGroupBlocksGetResult },
    { "PFGroupsListGroupBlocksGetResultSize", HandlePFGroupsListGroupBlocksGetResultSize },
    { "PFGroupsListGroupInvitationsAsync", HandlePFGroupsListGroupInvitationsAsync },
    { "PFGroupsListGroupInvitationsGetResult", HandlePFGroupsListGroupInvitationsGetResult },
    { "PFGroupsListGroupInvitationsGetResultSize", HandlePFGroupsListGroupInvitationsGetResultSize },
    { "PFGroupsListGroupMembersAsync", HandlePFGroupsListGroupMembersAsync },
    { "PFGroupsListGroupMembersGetResult", HandlePFGroupsListGroupMembersGetResult },
    { "PFGroupsListGroupMembersGetResultSize", HandlePFGroupsListGroupMembersGetResultSize },
    { "PFGroupsListMembershipAsync", HandlePFGroupsListMembershipAsync },
    { "PFGroupsListMembershipGetResult", HandlePFGroupsListMembershipGetResult },
    { "PFGroupsListMembershipGetResultSize", HandlePFGroupsListMembershipGetResultSize },
    { "PFGroupsListMembershipOpportunitiesAsync", HandlePFGroupsListMembershipOpportunitiesAsync },
    { "PFGroupsListMembershipOpportunitiesGetResult", HandlePFGroupsListMembershipOpportunitiesGetResult },
    { "PFGroupsListMembershipOpportunitiesGetResultSize", HandlePFGroupsListMembershipOpportunitiesGetResultSize },
    { "PFGroupsRemoveGroupApplicationAsync", HandlePFGroupsRemoveGroupApplicationAsync },
    { "PFGroupsRemoveGroupInvitationAsync", HandlePFGroupsRemoveGroupInvitationAsync },
    { "PFGroupsRemoveMembersAsync", HandlePFGroupsRemoveMembersAsync },
    { "PFGroupsUnblockEntityAsync", HandlePFGroupsUnblockEntityAsync },
    { "PFGroupsUpdateGroupAsync", HandlePFGroupsUpdateGroupAsync },
    { "PFGroupsUpdateGroupGetResult", HandlePFGroupsUpdateGroupGetResult },
    { "PFGroupsUpdateGroupGetResultSize", HandlePFGroupsUpdateGroupGetResultSize },
    { "PFGroupsUpdateRoleAsync", HandlePFGroupsUpdateRoleAsync },
    { "PFGroupsUpdateRoleGetResult", HandlePFGroupsUpdateRoleGetResult },
    { "PFGroupsUpdateRoleGetResultSize", HandlePFGroupsUpdateRoleGetResultSize }
});

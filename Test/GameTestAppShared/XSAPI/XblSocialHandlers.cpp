#include "pch.h"

#include "XblSocialHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

static XblSocialRelationshipResultHandle s_socialRelationshipResult = nullptr;
static XblSocialManagerUserGroupHandle s_socialManagerUserGroup = nullptr;

// ============================================================================
// Social (social_c.h)
// ============================================================================

CommandResultPayload HandleXblSocialGetSocialRelationshipsAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string error;
            int64_t xboxUserId = 0;
            int64_t socialRelationshipFilter = 0;
            int64_t startIndex = 0;
            int64_t maxItems = 0;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "xboxUserId", xboxUserId, error))
            {
                RETURN_HR_IF(E_POINTER, !state->xuser);
                uint64_t userId{};
                RETURN_IF_FAILED(XUserGetId(state->xuser, &userId));
                xboxUserId = static_cast<int64_t>(userId);
            }
            CommandHandlerShared::TryGetInt64Parameter(parameters, "socialRelationshipFilter", socialRelationshipFilter, error);
            CommandHandlerShared::TryGetInt64Parameter(parameters, "startIndex", startIndex, error);
            CommandHandlerShared::TryGetInt64Parameter(parameters, "maxItems", maxItems, error);

            HRESULT hr = XblSocialGetSocialRelationshipsAsync(
                state->xblContext,
                static_cast<uint64_t>(xboxUserId),
                static_cast<XblSocialRelationshipFilter>(socialRelationshipFilter),
                static_cast<size_t>(startIndex),
                static_cast<size_t>(maxItems),
                &async);
            LogToWindowFormat("XblSocialGetSocialRelationshipsAsync (xuid=%llu, hr=0x%08X)",
                static_cast<unsigned long long>(xboxUserId), static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            if (s_socialRelationshipResult) { XblSocialRelationshipResultCloseHandle(s_socialRelationshipResult); }
            HRESULT hr = XblSocialGetSocialRelationshipsResult(&async, &s_socialRelationshipResult);
            return hr;
        });
}

CommandResultPayload HandleXblSocialRelationshipResultGetRelationships(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_socialRelationshipResult);

            const XblSocialRelationship* relationships = nullptr;
            size_t relationshipsCount = 0;
            const HRESULT hr = XblSocialRelationshipResultGetRelationships(s_socialRelationshipResult, &relationships, &relationshipsCount);
            LogToWindowFormat("XblSocialRelationshipResultGetRelationships (count=%zu, hr=0x%08X)",
                relationshipsCount, static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["relationshipsCount"] = relationshipsCount;
            }
            return hr;
        });
}

CommandResultPayload HandleXblSocialRelationshipResultHasNext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_socialRelationshipResult);

            bool hasNext = false;
            const HRESULT hr = XblSocialRelationshipResultHasNext(s_socialRelationshipResult, &hasNext);
            LogToWindowFormat("XblSocialRelationshipResultHasNext (hasNext=%s, hr=0x%08X)",
                hasNext ? "true" : "false", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["hasNext"] = hasNext;
            }
            return hr;
        });
}

CommandResultPayload HandleXblSocialRelationshipResultGetTotalCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_socialRelationshipResult);

            size_t totalCount = 0;
            const HRESULT hr = XblSocialRelationshipResultGetTotalCount(s_socialRelationshipResult, &totalCount);
            LogToWindowFormat("XblSocialRelationshipResultGetTotalCount (totalCount=%zu, hr=0x%08X)",
                totalCount, static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["totalCount"] = totalCount;
            }
            return hr;
        });
}

CommandResultPayload HandleXblSocialRelationshipResultGetNextAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_socialRelationshipResult);

            std::string error;
            int64_t maxItems = 0;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "maxItems", maxItems, error)) { return E_INVALIDARG; }

            HRESULT hr = XblSocialRelationshipResultGetNextAsync(
                state->xblContext,
                s_socialRelationshipResult,
                static_cast<size_t>(maxItems),
                &async);
            LogToWindowFormat("XblSocialRelationshipResultGetNextAsync (maxItems=%u, hr=0x%08X)",
                static_cast<uint32_t>(maxItems), static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            XblSocialRelationshipResultCloseHandle(s_socialRelationshipResult);
            HRESULT hr = XblSocialRelationshipResultGetNextResult(&async, &s_socialRelationshipResult);
            return hr;
        });
}

CommandResultPayload HandleXblSocialRelationshipResultDuplicateHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_socialRelationshipResult);

            XblSocialRelationshipResultHandle duplicated = nullptr;
            const HRESULT hr = XblSocialRelationshipResultDuplicateHandle(s_socialRelationshipResult, &duplicated);
            LogToWindowFormat("XblSocialRelationshipResultDuplicateHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                XblSocialRelationshipResultCloseHandle(s_socialRelationshipResult);
                s_socialRelationshipResult = duplicated;
            }
            return hr;
        });
}

CommandResultPayload HandleXblSocialRelationshipResultCloseHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_socialRelationshipResult);
            XblSocialRelationshipResultCloseHandle(s_socialRelationshipResult);
            s_socialRelationshipResult = nullptr;
            LogToWindowFormat("XblSocialRelationshipResultCloseHandle");
            return S_OK;
        });
}

CommandResultPayload HandleXblSocialAddSocialRelationshipChangedHandler(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            XblFunctionContext token = XblSocialAddSocialRelationshipChangedHandler(
                state->xblContext,
                [](const XblSocialRelationshipChangeEventArgs* args, void* /*context*/)
                {
                    if (args)
                    {
                        LogToWindowFormat("SocialRelationshipChanged (callingXuid=%llu, socialNotification=%d)",
                            static_cast<unsigned long long>(args->callerXboxUserId),
                            static_cast<int>(args->socialNotification));
                    }
                },
                nullptr);
            LogToWindowFormat("XblSocialAddSocialRelationshipChangedHandler (token=%u)", token);
            payload.result["token"] = token;
            return S_OK;
        });
}

CommandResultPayload HandleXblSocialRemoveSocialRelationshipChangedHandler(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string error;
            int64_t token = 0;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "token", token, error);

            XblSocialRemoveSocialRelationshipChangedHandler(state->xblContext, static_cast<XblFunctionContext>(token));
            LogToWindowFormat("XblSocialRemoveSocialRelationshipChangedHandler (token=%lld)", token);
            return S_OK;
        });
}

CommandResultPayload HandleXblSocialSubmitReputationFeedbackAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCall(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string error;
            int64_t xboxUserId = 0;
            int64_t reputationFeedbackType = 0;
            std::string reasonMessage;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "xboxUserId", xboxUserId, error))
            {
                xboxUserId = 2814639011617876LL;
            }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "reputationFeedbackType", reputationFeedbackType, error))
            {
                reputationFeedbackType = 0; // XblReputationFeedbackType::PositiveHelpfulPlayer
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "reasonMessage", reasonMessage, error))
            {
                reasonMessage = "Helpful player";
            }

            HRESULT hr = XblSocialSubmitReputationFeedbackAsync(
                state->xblContext,
                static_cast<uint64_t>(xboxUserId),
                static_cast<XblReputationFeedbackType>(reputationFeedbackType),
                nullptr,
                reasonMessage.c_str(),
                nullptr,
                &async);
            LogToWindowFormat("XblSocialSubmitReputationFeedbackAsync (xuid=%llu, hr=0x%08X)",
                static_cast<unsigned long long>(xboxUserId), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblSocialSubmitBatchReputationFeedbackAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCall(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            std::string error;
            int64_t xboxUserId = 0;
            int64_t feedbackType = 0;
            std::string reasonMessage;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "xboxUserId", xboxUserId, error))
            {
                xboxUserId = 2814639011617876LL;
            }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "feedbackType", feedbackType, error))
            {
                feedbackType = 0; // XblReputationFeedbackType::PositiveHelpfulPlayer
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "reasonMessage", reasonMessage, error))
            {
                reasonMessage = "Helpful player";
            }

            XblReputationFeedbackItem feedbackItem{};
            feedbackItem.xboxUserId = static_cast<uint64_t>(xboxUserId);
            feedbackItem.feedbackType = static_cast<XblReputationFeedbackType>(feedbackType);
            feedbackItem.reasonMessage = reasonMessage.c_str();

            HRESULT hr = XblSocialSubmitBatchReputationFeedbackAsync(
                state->xblContext,
                &feedbackItem,
                1,
                &async);
            LogToWindowFormat("XblSocialSubmitBatchReputationFeedbackAsync (xuid=%llu, hr=0x%08X)",
                static_cast<unsigned long long>(xboxUserId), static_cast<uint32_t>(hr));
            return hr;
        });
}

// ============================================================================
// Social Manager (social_manager_c.h)
// ============================================================================

CommandResultPayload HandleXblSocialManagerAddLocalUser(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            std::string error;
            int64_t extraLevelDetail = 0;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "extraLevelDetail", extraLevelDetail, error);

            const HRESULT hr = XblSocialManagerAddLocalUser(
                state->xuser,
                static_cast<XblSocialManagerExtraDetailLevel>(extraLevelDetail),
                nullptr);
            LogToWindowFormat("XblSocialManagerAddLocalUser (extraLevelDetail=%lld, hr=0x%08X)",
                extraLevelDetail, static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblSocialManagerRemoveLocalUser(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            const HRESULT hr = XblSocialManagerRemoveLocalUser(state->xuser);
            LogToWindowFormat("XblSocialManagerRemoveLocalUser (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblSocialManagerDoWork(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const XblSocialManagerEvent* socialEvents = nullptr;
            size_t socialEventsCount = 0;
            const HRESULT hr = XblSocialManagerDoWork(&socialEvents, &socialEventsCount);
            LogToWindowFormat("XblSocialManagerDoWork (socialEventsCount=%zu, hr=0x%08X)",
                socialEventsCount, static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["socialEventsCount"] = socialEventsCount;
            }
            return hr;
        });
}

CommandResultPayload HandleXblSocialManagerCreateSocialUserGroupFromFilters(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            std::string error;
            int64_t presenceFilter = 0;
            int64_t relationshipFilter = 0;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "presenceFilter", presenceFilter, error);
            CommandHandlerShared::TryGetInt64Parameter(parameters, "relationshipFilter", relationshipFilter, error);

            if (s_socialManagerUserGroup) { XblSocialManagerDestroySocialUserGroup(s_socialManagerUserGroup); s_socialManagerUserGroup = nullptr; }
            const HRESULT hr = XblSocialManagerCreateSocialUserGroupFromFilters(
                state->xuser,
                static_cast<XblPresenceFilter>(presenceFilter),
                static_cast<XblRelationshipFilter>(relationshipFilter),
                &s_socialManagerUserGroup);
            LogToWindowFormat("XblSocialManagerCreateSocialUserGroupFromFilters (presenceFilter=%lld, relationshipFilter=%lld, hr=0x%08X)",
                presenceFilter, relationshipFilter, static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblSocialManagerCreateSocialUserGroupFromList(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            auto xboxUserIdList = CommandHandlerShared::GetUint64Array(parameters, "xboxUserIdList");
            if (xboxUserIdList.empty())
            {
                RETURN_HR_IF(E_POINTER, !state->xuser);
                uint64_t userId{};
                RETURN_IF_FAILED(XUserGetId(state->xuser, &userId));
                xboxUserIdList.push_back(userId);
            }

            if (s_socialManagerUserGroup) { XblSocialManagerDestroySocialUserGroup(s_socialManagerUserGroup); s_socialManagerUserGroup = nullptr; }
            const HRESULT hr = XblSocialManagerCreateSocialUserGroupFromList(
                state->xuser,
                xboxUserIdList.data(),
                xboxUserIdList.size(),
                &s_socialManagerUserGroup);
            LogToWindowFormat("XblSocialManagerCreateSocialUserGroupFromList (count=%zu, hr=0x%08X)",
                xboxUserIdList.size(), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblSocialManagerDestroySocialUserGroup(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_socialManagerUserGroup);

            const HRESULT hr = XblSocialManagerDestroySocialUserGroup(s_socialManagerUserGroup);
            LogToWindowFormat("XblSocialManagerDestroySocialUserGroup (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                s_socialManagerUserGroup = nullptr;
            }
            return hr;
        });
}

CommandResultPayload HandleXblSocialManagerUpdateSocialUserGroup(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_socialManagerUserGroup);

            auto users = CommandHandlerShared::GetUint64Array(parameters, "users");
            RETURN_HR_IF(E_INVALIDARG, users.empty());

            const HRESULT hr = XblSocialManagerUpdateSocialUserGroup(
                s_socialManagerUserGroup,
                users.data(),
                users.size());
            LogToWindowFormat("XblSocialManagerUpdateSocialUserGroup (count=%zu, hr=0x%08X)",
                users.size(), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblSocialManagerGetLocalUserCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const size_t count = XblSocialManagerGetLocalUserCount();
            payload.result["count"] = count;
            LogToWindowFormat("XblSocialManagerGetLocalUserCount (count=%zu)", count);
            return S_OK;
        });
}

CommandResultPayload HandleXblSocialManagerGetLocalUsers(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const size_t count = XblSocialManagerGetLocalUserCount();
            if (count == 0)
            {
                payload.result["userCount"] = 0;
                LogToWindowFormat("XblSocialManagerGetLocalUsers (userCount=0)");
                return S_OK;
            }

            std::vector<XUserHandle> users(count);
            const HRESULT hr = XblSocialManagerGetLocalUsers(count, users.data());
            LogToWindowFormat("XblSocialManagerGetLocalUsers (userCount=%zu, hr=0x%08X)",
                count, static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["userCount"] = count;
            }
            return hr;
        });
}

CommandResultPayload HandleXblSocialManagerUserGroupGetType(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_socialManagerUserGroup);

            XblSocialUserGroupType type{};
            const HRESULT hr = XblSocialManagerUserGroupGetType(s_socialManagerUserGroup, &type);
            LogToWindowFormat("XblSocialManagerUserGroupGetType (type=%d, hr=0x%08X)",
                static_cast<int>(type), static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["type"] = static_cast<int>(type);
            }
            return hr;
        });
}

CommandResultPayload HandleXblSocialManagerUserGroupGetLocalUser(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_socialManagerUserGroup);

            XUserHandle user = nullptr;
            const HRESULT hr = XblSocialManagerUserGroupGetLocalUser(s_socialManagerUserGroup, &user);
            LogToWindowFormat("XblSocialManagerUserGroupGetLocalUser (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblSocialManagerUserGroupGetFilters(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_socialManagerUserGroup);

            XblPresenceFilter presenceFilter{};
            XblRelationshipFilter relationshipFilter{};
            const HRESULT hr = XblSocialManagerUserGroupGetFilters(s_socialManagerUserGroup, &presenceFilter, &relationshipFilter);
            LogToWindowFormat("XblSocialManagerUserGroupGetFilters (presenceFilter=%d, relationshipFilter=%d, hr=0x%08X)",
                static_cast<int>(presenceFilter), static_cast<int>(relationshipFilter), static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["presenceFilter"] = static_cast<int>(presenceFilter);
                payload.result["relationshipFilter"] = static_cast<int>(relationshipFilter);
            }
            return hr;
        });
}

CommandResultPayload HandleXblSocialManagerUserGroupGetUsers(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_socialManagerUserGroup);

            XblSocialManagerUserPtrArray users = nullptr;
            size_t usersCount = 0;
            const HRESULT hr = XblSocialManagerUserGroupGetUsers(s_socialManagerUserGroup, &users, &usersCount);
            LogToWindowFormat("XblSocialManagerUserGroupGetUsers (usersCount=%zu, hr=0x%08X)",
                usersCount, static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["usersCount"] = usersCount;
            }
            return hr;
        });
}

CommandResultPayload HandleXblSocialManagerUserGroupGetUsersTrackedByGroup(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_socialManagerUserGroup);

            const uint64_t* trackedUsers = nullptr;
            size_t trackedUsersCount = 0;
            const HRESULT hr = XblSocialManagerUserGroupGetUsersTrackedByGroup(s_socialManagerUserGroup, &trackedUsers, &trackedUsersCount);
            LogToWindowFormat("XblSocialManagerUserGroupGetUsersTrackedByGroup (count=%zu, hr=0x%08X)",
                trackedUsersCount, static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["trackedUsersCount"] = trackedUsersCount;
            }
            return hr;
        });
}

CommandResultPayload HandleXblSocialManagerSetRichPresencePollingStatus(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            std::string error;
            bool shouldEnablePolling = false;
            CommandHandlerShared::TryParseBoolParameter(parameters, "shouldEnablePolling", shouldEnablePolling, error);

            const HRESULT hr = XblSocialManagerSetRichPresencePollingStatus(state->xuser, shouldEnablePolling);
            LogToWindowFormat("XblSocialManagerSetRichPresencePollingStatus (shouldEnablePolling=%s, hr=0x%08X)",
                shouldEnablePolling ? "true" : "false", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblSocialAddFriendRequestCountChangedHandler(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblSocialAddFriendRequestCountChangedHandler: API not available in this build configuration");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandleXblSocialRemoveFriendRequestCountChangedHandler(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblSocialRemoveFriendRequestCountChangedHandler: API not available in this build configuration");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandleXblSocialGetSocialRelationshipsResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblSocialGetSocialRelationshipsResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblSocialRelationshipResultGetNextResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblSocialRelationshipResultGetNextResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblSocialManagerPresenceRecordIsUserPlayingTitle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XblSocialManagerPresenceRecord presenceRecord{};
            uint32_t titleId = 0;
            bool isPlaying = XblSocialManagerPresenceRecordIsUserPlayingTitle(&presenceRecord, titleId);
            LogToWindowFormat("XblSocialManagerPresenceRecordIsUserPlayingTitle (isPlaying=%s)", isPlaying ? "true" : "false");
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XblSocialAddFriendRequestCountChangedHandler", HandleXblSocialAddFriendRequestCountChangedHandler },
    { "XblSocialAddSocialRelationshipChangedHandler", HandleXblSocialAddSocialRelationshipChangedHandler },
    { "XblSocialGetSocialRelationshipsAsync", HandleXblSocialGetSocialRelationshipsAsync },
    { "XblSocialGetSocialRelationshipsResult", HandleXblSocialGetSocialRelationshipsResult },
    { "XblSocialManagerAddLocalUser", HandleXblSocialManagerAddLocalUser },
    { "XblSocialManagerCreateSocialUserGroupFromFilters", HandleXblSocialManagerCreateSocialUserGroupFromFilters },
    { "XblSocialManagerCreateSocialUserGroupFromList", HandleXblSocialManagerCreateSocialUserGroupFromList },
    { "XblSocialManagerDestroySocialUserGroup", HandleXblSocialManagerDestroySocialUserGroup },
    { "XblSocialManagerDoWork", HandleXblSocialManagerDoWork },
    { "XblSocialManagerGetLocalUserCount", HandleXblSocialManagerGetLocalUserCount },
    { "XblSocialManagerGetLocalUsers", HandleXblSocialManagerGetLocalUsers },
    { "XblSocialManagerPresenceRecordIsUserPlayingTitle", HandleXblSocialManagerPresenceRecordIsUserPlayingTitle },
    { "XblSocialManagerRemoveLocalUser", HandleXblSocialManagerRemoveLocalUser },
    { "XblSocialManagerSetRichPresencePollingStatus", HandleXblSocialManagerSetRichPresencePollingStatus },
    { "XblSocialManagerUpdateSocialUserGroup", HandleXblSocialManagerUpdateSocialUserGroup },
    { "XblSocialManagerUserGroupGetFilters", HandleXblSocialManagerUserGroupGetFilters },
    { "XblSocialManagerUserGroupGetLocalUser", HandleXblSocialManagerUserGroupGetLocalUser },
    { "XblSocialManagerUserGroupGetType", HandleXblSocialManagerUserGroupGetType },
    { "XblSocialManagerUserGroupGetUsers", HandleXblSocialManagerUserGroupGetUsers },
    { "XblSocialManagerUserGroupGetUsersTrackedByGroup", HandleXblSocialManagerUserGroupGetUsersTrackedByGroup },
    { "XblSocialRelationshipResultCloseHandle", HandleXblSocialRelationshipResultCloseHandle },
    { "XblSocialRelationshipResultDuplicateHandle", HandleXblSocialRelationshipResultDuplicateHandle },
    { "XblSocialRelationshipResultGetNextAsync", HandleXblSocialRelationshipResultGetNextAsync },
    { "XblSocialRelationshipResultGetNextResult", HandleXblSocialRelationshipResultGetNextResult },
    { "XblSocialRelationshipResultGetRelationships", HandleXblSocialRelationshipResultGetRelationships },
    { "XblSocialRelationshipResultGetTotalCount", HandleXblSocialRelationshipResultGetTotalCount },
    { "XblSocialRelationshipResultHasNext", HandleXblSocialRelationshipResultHasNext },
    { "XblSocialRemoveFriendRequestCountChangedHandler", HandleXblSocialRemoveFriendRequestCountChangedHandler },
    { "XblSocialRemoveSocialRelationshipChangedHandler", HandleXblSocialRemoveSocialRelationshipChangedHandler },
    { "XblSocialSubmitBatchReputationFeedbackAsync", HandleXblSocialSubmitBatchReputationFeedbackAsync },
    { "XblSocialSubmitReputationFeedbackAsync", HandleXblSocialSubmitReputationFeedbackAsync }
});

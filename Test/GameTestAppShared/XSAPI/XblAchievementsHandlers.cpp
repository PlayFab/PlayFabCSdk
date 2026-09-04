#include "pch.h"

#include "XblAchievementsHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

static XblAchievementsResultHandle s_achievementsResult = nullptr;
static XblAchievementsManagerResultHandle s_achievementsManagerResult = nullptr;

// ============================================================================
// Achievements (achievements_c.h)
// ============================================================================

CommandResultPayload HandleXblAchievementsGetAchievementsForTitleIdAsync(
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
            int64_t titleId = 0;
            int64_t type = 1;
            bool unlockedOnly = false;
            int64_t orderBy = 0;
            int64_t skipItems = 0;
            int64_t maxItems = 100;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "xboxUserId", xboxUserId, error))
            {
                RETURN_HR_IF(E_POINTER, !state->xuser);
                uint64_t userId{};
                RETURN_IF_FAILED(XUserGetId(state->xuser, &userId));
                xboxUserId = static_cast<int64_t>(userId);
            }
            CommandHandlerShared::TryGetInt64Parameter(parameters, "titleId", titleId, error);
            CommandHandlerShared::TryGetInt64Parameter(parameters, "type", type, error);
            CommandHandlerShared::TryParseBoolParameter(parameters, "unlockedOnly", unlockedOnly, error);
            CommandHandlerShared::TryGetInt64Parameter(parameters, "orderBy", orderBy, error);
            CommandHandlerShared::TryGetInt64Parameter(parameters, "skipItems", skipItems, error);
            CommandHandlerShared::TryGetInt64Parameter(parameters, "maxItems", maxItems, error);

            HRESULT hr = XblAchievementsGetAchievementsForTitleIdAsync(
                state->xblContext,
                static_cast<uint64_t>(xboxUserId),
                static_cast<uint32_t>(titleId),
                static_cast<XblAchievementType>(type),
                unlockedOnly,
                static_cast<XblAchievementOrderBy>(orderBy),
                static_cast<uint32_t>(skipItems),
                static_cast<uint32_t>(maxItems),
                &async);
            LogToWindowFormat("XblAchievementsGetAchievementsForTitleIdAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            if (s_achievementsResult) { XblAchievementsResultCloseHandle(s_achievementsResult); }
            HRESULT hr = XblAchievementsGetAchievementsForTitleIdResult(&async, &s_achievementsResult);
            return hr;
        });
}

CommandResultPayload HandleXblAchievementsGetAchievementAsync(
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
            std::string serviceConfigurationId;
            std::string achievementId;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "xboxUserId", xboxUserId, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "serviceConfigurationId", serviceConfigurationId, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "achievementId", achievementId, error)) { return E_INVALIDARG; }

            HRESULT hr = XblAchievementsGetAchievementAsync(
                state->xblContext,
                static_cast<uint64_t>(xboxUserId),
                serviceConfigurationId.c_str(),
                achievementId.c_str(),
                &async);
            LogToWindowFormat("XblAchievementsGetAchievementAsync (achievementId=%s, hr=0x%08X)",
                achievementId.c_str(), static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            if (s_achievementsResult) { XblAchievementsResultCloseHandle(s_achievementsResult); }
            HRESULT hr = XblAchievementsGetAchievementResult(&async, &s_achievementsResult);
            return hr;
        });
}

CommandResultPayload HandleXblAchievementsUpdateAchievementAsync(
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
            std::string achievementId;
            int64_t percentComplete = 0;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "xboxUserId", xboxUserId, error))
            {
                RETURN_HR_IF(E_POINTER, !state->xuser);
                uint64_t userId{};
                RETURN_IF_FAILED(XUserGetId(state->xuser, &userId));
                xboxUserId = static_cast<int64_t>(userId);
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "achievementId", achievementId, error))
            {
                achievementId = "1";
            }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "percentComplete", percentComplete, error))
            {
                percentComplete = 100;
            }

            HRESULT hr = XblAchievementsUpdateAchievementAsync(
                state->xblContext,
                static_cast<uint64_t>(xboxUserId),
                achievementId.c_str(),
                static_cast<uint32_t>(percentComplete),
                &async);
            LogToWindowFormat("XblAchievementsUpdateAchievementAsync (achievementId=%s, percent=%u, hr=0x%08X)",
                achievementId.c_str(), static_cast<uint32_t>(percentComplete), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblAchievementsUpdateAchievementForTitleIdAsync(
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
            int64_t titleId = 0;
            std::string serviceConfigurationId;
            std::string achievementId;
            int64_t percentComplete = 0;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "xboxUserId", xboxUserId, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "titleId", titleId, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "serviceConfigurationId", serviceConfigurationId, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "achievementId", achievementId, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "percentComplete", percentComplete, error)) { return E_INVALIDARG; }

            HRESULT hr = XblAchievementsUpdateAchievementForTitleIdAsync(
                state->xblContext,
                static_cast<uint64_t>(xboxUserId),
                static_cast<uint32_t>(titleId),
                serviceConfigurationId.c_str(),
                achievementId.c_str(),
                static_cast<uint32_t>(percentComplete),
                &async);
            LogToWindowFormat("XblAchievementsUpdateAchievementForTitleIdAsync (achievementId=%s, titleId=%u, hr=0x%08X)",
                achievementId.c_str(), static_cast<uint32_t>(titleId), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblAchievementsResultGetAchievements(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_achievementsResult);

            const XblAchievement* achievements = nullptr;
            size_t achievementsCount = 0;
            const HRESULT hr = XblAchievementsResultGetAchievements(s_achievementsResult, &achievements, &achievementsCount);
            LogToWindowFormat("XblAchievementsResultGetAchievements (count=%zu, hr=0x%08X)",
                achievementsCount, static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["achievementsCount"] = achievementsCount;
            }
            return hr;
        });
}

CommandResultPayload HandleXblAchievementsResultHasNext(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_achievementsResult);

            bool hasNext = false;
            const HRESULT hr = XblAchievementsResultHasNext(s_achievementsResult, &hasNext);
            LogToWindowFormat("XblAchievementsResultHasNext (hasNext=%s, hr=0x%08X)",
                hasNext ? "true" : "false", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["hasNext"] = hasNext;
            }
            return hr;
        });
}

CommandResultPayload HandleXblAchievementsResultGetNextAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_achievementsResult);

            std::string error;
            int64_t maxItems = 0;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "maxItems", maxItems, error)) { return E_INVALIDARG; }

            HRESULT hr = XblAchievementsResultGetNextAsync(
                s_achievementsResult,
                static_cast<uint32_t>(maxItems),
                &async);
            LogToWindowFormat("XblAchievementsResultGetNextAsync (maxItems=%u, hr=0x%08X)",
                static_cast<uint32_t>(maxItems), static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            XblAchievementsResultCloseHandle(s_achievementsResult);
            HRESULT hr = XblAchievementsResultGetNextResult(&async, &s_achievementsResult);
            return hr;
        });
}

CommandResultPayload HandleXblAchievementsResultDuplicateHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_achievementsResult);

            XblAchievementsResultHandle duplicated = nullptr;
            const HRESULT hr = XblAchievementsResultDuplicateHandle(s_achievementsResult, &duplicated);
            LogToWindowFormat("XblAchievementsResultDuplicateHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                XblAchievementsResultCloseHandle(s_achievementsResult);
                s_achievementsResult = duplicated;
            }
            return hr;
        });
}

CommandResultPayload HandleXblAchievementsResultCloseHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_achievementsResult);
            XblAchievementsResultCloseHandle(s_achievementsResult);
            s_achievementsResult = nullptr;
            LogToWindowFormat("XblAchievementsResultCloseHandle");
            return S_OK;
        });
}

CommandResultPayload HandleXblAchievementUnlockAddNotificationHandler(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblAchievementUnlockAddNotificationHandler: API not available in this build configuration");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandleXblAchievementUnlockRemoveNotificationHandler(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblAchievementUnlockRemoveNotificationHandler: API not available in this build configuration");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandleXblAchievementsAddAchievementProgressChangeHandler(
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

            XblFunctionContext token = XblAchievementsAddAchievementProgressChangeHandler(
                state->xblContext,
                [](const XblAchievementProgressChangeEventArgs* args, void* /*context*/)
                {
                    if (args)
                    {
                        LogToWindowFormat("AchievementProgressChangeNotification (entryCount=%zu)",
                            args->entryCount);
                    }
                },
                nullptr);
            LogToWindowFormat("XblAchievementsAddAchievementProgressChangeHandler (token=%u)", token);
            payload.result["token"] = token;
            return S_OK;
        });
}

CommandResultPayload HandleXblAchievementsRemoveAchievementProgressChangeHandler(
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
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "token", token, error)) { return E_INVALIDARG; }

            XblAchievementsRemoveAchievementProgressChangeHandler(state->xblContext, static_cast<XblFunctionContext>(token));
            LogToWindowFormat("XblAchievementsRemoveAchievementProgressChangeHandler (token=%lld)", token);
            return S_OK;
        });
}

// ============================================================================
// Achievements Manager (achievements_manager_c.h)
// ============================================================================

CommandResultPayload HandleXblAchievementsManagerAddLocalUser(
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

            XTaskQueueHandle queue = nullptr;
            // Use provided queue parameter if present, otherwise nullptr
            std::string error;
            bool useQueue = false;
            CommandHandlerShared::TryParseBoolParameter(parameters, "useQueue", useQueue, error);
            if (useQueue)
            {
                queue = state->taskQueue;
            }

            const HRESULT hr = XblAchievementsManagerAddLocalUser(state->xuser, queue);
            LogToWindowFormat("XblAchievementsManagerAddLocalUser (queue=%s, hr=0x%08X)",
                queue ? "non-null" : "null", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblAchievementsManagerRemoveLocalUser(
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

            const HRESULT hr = XblAchievementsManagerRemoveLocalUser(state->xuser);
            LogToWindowFormat("XblAchievementsManagerRemoveLocalUser (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblAchievementsManagerIsUserInitialized(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            std::string error;
            int64_t xboxUserId = 0;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "xboxUserId", xboxUserId, error))
            {
                RETURN_HR_IF(E_POINTER, !state->xuser);
                uint64_t userId{};
                RETURN_IF_FAILED(XUserGetId(state->xuser, &userId));
                xboxUserId = static_cast<int64_t>(userId);
            }

            bool isInitialized = false;
            const HRESULT hr = XblAchievementsManagerIsUserInitialized(static_cast<uint64_t>(xboxUserId));
            isInitialized = SUCCEEDED(hr);
            LogToWindowFormat("XblAchievementsManagerIsUserInitialized (xuid=%llu, initialized=%s, hr=0x%08X)",
                static_cast<unsigned long long>(xboxUserId), isInitialized ? "true" : "false", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["isInitialized"] = isInitialized;
            }
            return hr;
        });
}

CommandResultPayload HandleXblAchievementsManagerDoWork(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const XblAchievementsManagerEvent* events = nullptr;
            size_t eventsCount = 0;
            const HRESULT hr = XblAchievementsManagerDoWork(&events, &eventsCount);
            LogToWindowFormat("XblAchievementsManagerDoWork (eventsCount=%zu, hr=0x%08X)",
                eventsCount, static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["eventsCount"] = eventsCount;
            }
            return hr;
        });
}

CommandResultPayload HandleXblAchievementsManagerGetAchievement(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string error;
            int64_t xboxUserId = 0;
            std::string achievementId;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "xboxUserId", xboxUserId, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "achievementId", achievementId, error)) { return E_INVALIDARG; }

            if (s_achievementsManagerResult) { XblAchievementsManagerResultCloseHandle(s_achievementsManagerResult); }
            const HRESULT hr = XblAchievementsManagerGetAchievement(
                static_cast<uint64_t>(xboxUserId),
                achievementId.c_str(),
                &s_achievementsManagerResult);
            LogToWindowFormat("XblAchievementsManagerGetAchievement (achievementId=%s, hr=0x%08X)",
                achievementId.c_str(), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblAchievementsManagerGetAchievements(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string error;
            int64_t xboxUserId = 0;
            int64_t sortField = 0;
            int64_t sortOrder = 0;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "xboxUserId", xboxUserId, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "sortField", sortField, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "sortOrder", sortOrder, error)) { return E_INVALIDARG; }

            if (s_achievementsManagerResult) { XblAchievementsManagerResultCloseHandle(s_achievementsManagerResult); }
            const HRESULT hr = XblAchievementsManagerGetAchievements(
                static_cast<uint64_t>(xboxUserId),
                static_cast<XblAchievementOrderBy>(sortField),
                static_cast<XblAchievementsManagerSortOrder>(sortOrder),
                &s_achievementsManagerResult);
            LogToWindowFormat("XblAchievementsManagerGetAchievements (xuid=%llu, hr=0x%08X)",
                static_cast<unsigned long long>(xboxUserId), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblAchievementsManagerGetAchievementsByState(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string error;
            int64_t xboxUserId = 0;
            int64_t sortField = 0;
            int64_t sortOrder = 0;
            int64_t achievementState = 0;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "xboxUserId", xboxUserId, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "sortField", sortField, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "sortOrder", sortOrder, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "achievementState", achievementState, error)) { return E_INVALIDARG; }

            if (s_achievementsManagerResult) { XblAchievementsManagerResultCloseHandle(s_achievementsManagerResult); }
            const HRESULT hr = XblAchievementsManagerGetAchievementsByState(
                static_cast<uint64_t>(xboxUserId),
                static_cast<XblAchievementOrderBy>(sortField),
                static_cast<XblAchievementsManagerSortOrder>(sortOrder),
                static_cast<XblAchievementProgressState>(achievementState),
                &s_achievementsManagerResult);
            LogToWindowFormat("XblAchievementsManagerGetAchievementsByState (xuid=%llu, state=%lld, hr=0x%08X)",
                static_cast<unsigned long long>(xboxUserId), achievementState, static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblAchievementsManagerUpdateAchievement(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string error;
            int64_t xboxUserId = 0;
            std::string achievementId;
            int64_t currentProgress = 0;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "xboxUserId", xboxUserId, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "achievementId", achievementId, error)) { return E_INVALIDARG; }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "currentProgress", currentProgress, error)) { return E_INVALIDARG; }

            const HRESULT hr = XblAchievementsManagerUpdateAchievement(
                static_cast<uint64_t>(xboxUserId),
                achievementId.c_str(),
                static_cast<uint8_t>(currentProgress));
            LogToWindowFormat("XblAchievementsManagerUpdateAchievement (achievementId=%s, progress=%u, hr=0x%08X)",
                achievementId.c_str(), static_cast<uint8_t>(currentProgress), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblAchievementsManagerResultGetAchievements(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_achievementsManagerResult);

            const XblAchievement* achievements = nullptr;
            size_t achievementsCount = 0;
            const HRESULT hr = XblAchievementsManagerResultGetAchievements(s_achievementsManagerResult, &achievements, &achievementsCount);
            LogToWindowFormat("XblAchievementsManagerResultGetAchievements (count=%zu, hr=0x%08X)",
                achievementsCount, static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["achievementsCount"] = achievementsCount;
            }
            return hr;
        });
}

CommandResultPayload HandleXblAchievementsManagerResultDuplicateHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_achievementsManagerResult);

            XblAchievementsManagerResultHandle duplicated = nullptr;
            const HRESULT hr = XblAchievementsManagerResultDuplicateHandle(s_achievementsManagerResult, &duplicated);
            LogToWindowFormat("XblAchievementsManagerResultDuplicateHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                XblAchievementsManagerResultCloseHandle(s_achievementsManagerResult);
                s_achievementsManagerResult = duplicated;
            }
            return hr;
        });
}

CommandResultPayload HandleXblAchievementsManagerResultCloseHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_achievementsManagerResult);
            XblAchievementsManagerResultCloseHandle(s_achievementsManagerResult);
            s_achievementsManagerResult = nullptr;
            LogToWindowFormat("XblAchievementsManagerResultCloseHandle");
            return S_OK;
        });
}

CommandResultPayload HandleXblAchievementsResultGetNextResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblAchievementsResultGetNextResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblAchievementsGetAchievementsForTitleIdResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblAchievementsGetAchievementsForTitleIdResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblAchievementsGetAchievementResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblAchievementsGetAchievementResult: called inline by Async handler");
            return S_OK;
        });
}


// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XblAchievementUnlockAddNotificationHandler", HandleXblAchievementUnlockAddNotificationHandler },
    { "XblAchievementUnlockRemoveNotificationHandler", HandleXblAchievementUnlockRemoveNotificationHandler },
    { "XblAchievementsAddAchievementProgressChangeHandler", HandleXblAchievementsAddAchievementProgressChangeHandler },
    { "XblAchievementsGetAchievementAsync", HandleXblAchievementsGetAchievementAsync },
    { "XblAchievementsGetAchievementResult", HandleXblAchievementsGetAchievementResult },
    { "XblAchievementsGetAchievementsForTitleIdAsync", HandleXblAchievementsGetAchievementsForTitleIdAsync },
    { "XblAchievementsGetAchievementsForTitleIdResult", HandleXblAchievementsGetAchievementsForTitleIdResult },
    { "XblAchievementsManagerAddLocalUser", HandleXblAchievementsManagerAddLocalUser },
    { "XblAchievementsManagerDoWork", HandleXblAchievementsManagerDoWork },
    { "XblAchievementsManagerGetAchievement", HandleXblAchievementsManagerGetAchievement },
    { "XblAchievementsManagerGetAchievements", HandleXblAchievementsManagerGetAchievements },
    { "XblAchievementsManagerGetAchievementsByState", HandleXblAchievementsManagerGetAchievementsByState },
    { "XblAchievementsManagerIsUserInitialized", HandleXblAchievementsManagerIsUserInitialized },
    { "XblAchievementsManagerRemoveLocalUser", HandleXblAchievementsManagerRemoveLocalUser },
    { "XblAchievementsManagerResultCloseHandle", HandleXblAchievementsManagerResultCloseHandle },
    { "XblAchievementsManagerResultDuplicateHandle", HandleXblAchievementsManagerResultDuplicateHandle },
    { "XblAchievementsManagerResultGetAchievements", HandleXblAchievementsManagerResultGetAchievements },
    { "XblAchievementsManagerUpdateAchievement", HandleXblAchievementsManagerUpdateAchievement },
    { "XblAchievementsRemoveAchievementProgressChangeHandler", HandleXblAchievementsRemoveAchievementProgressChangeHandler },
    { "XblAchievementsResultCloseHandle", HandleXblAchievementsResultCloseHandle },
    { "XblAchievementsResultDuplicateHandle", HandleXblAchievementsResultDuplicateHandle },
    { "XblAchievementsResultGetAchievements", HandleXblAchievementsResultGetAchievements },
    { "XblAchievementsResultGetNextAsync", HandleXblAchievementsResultGetNextAsync },
    { "XblAchievementsResultGetNextResult", HandleXblAchievementsResultGetNextResult },
    { "XblAchievementsResultHasNext", HandleXblAchievementsResultHasNext },
    { "XblAchievementsUpdateAchievementAsync", HandleXblAchievementsUpdateAchievementAsync },
    { "XblAchievementsUpdateAchievementForTitleIdAsync", HandleXblAchievementsUpdateAchievementForTitleIdAsync }
});

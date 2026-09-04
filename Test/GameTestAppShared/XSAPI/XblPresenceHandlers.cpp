#include "pch.h"

#include "XblPresenceHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

static XblPresenceRecordHandle s_presenceRecord = nullptr;
static std::vector<XblPresenceRecordHandle> s_presenceRecords;

CommandResultPayload HandleXblPresenceRecordGetXuid(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_presenceRecord);

            uint64_t xuid{};
            const HRESULT hr = XblPresenceRecordGetXuid(s_presenceRecord, &xuid);
            LogToWindowFormat("XblPresenceRecordGetXuid (xuid=%llu, hr=0x%08X)", xuid, static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["xuid"] = xuid;
            }
            return hr;
        });
}

CommandResultPayload HandleXblPresenceRecordGetUserState(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_presenceRecord);

            XblPresenceUserState userState{};
            const HRESULT hr = XblPresenceRecordGetUserState(s_presenceRecord, &userState);
            LogToWindowFormat("XblPresenceRecordGetUserState (userState=%d, hr=0x%08X)", static_cast<int>(userState), static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["userState"] = static_cast<int>(userState);
            }
            return hr;
        });
}

CommandResultPayload HandleXblPresenceRecordGetDeviceRecords(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_presenceRecord);

            const XblPresenceDeviceRecord* deviceRecords{};
            size_t deviceRecordsCount{};
            const HRESULT hr = XblPresenceRecordGetDeviceRecords(s_presenceRecord, &deviceRecords, &deviceRecordsCount);
            LogToWindowFormat("XblPresenceRecordGetDeviceRecords (count=%zu, hr=0x%08X)", deviceRecordsCount, static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["deviceRecordsCount"] = deviceRecordsCount;
            }
            return hr;
        });
}

CommandResultPayload HandleXblPresenceRecordDuplicateHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_presenceRecord);

            XblPresenceRecordHandle duplicatedHandle{ nullptr };
            const HRESULT hr = XblPresenceRecordDuplicateHandle(s_presenceRecord, &duplicatedHandle);
            LogToWindowFormat("XblPresenceRecordDuplicateHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                XblPresenceRecordCloseHandle(s_presenceRecord);
                s_presenceRecord = duplicatedHandle;
            }
            return hr;
        });
}

CommandResultPayload HandleXblPresenceRecordCloseHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (s_presenceRecord)
            {
                XblPresenceRecordCloseHandle(s_presenceRecord);
                s_presenceRecord = nullptr;
            }
            LogToWindowFormat("XblPresenceRecordCloseHandle (hr=0x%08X)", static_cast<uint32_t>(S_OK));
            return S_OK;
        });
}

CommandResultPayload HandleXblPresenceSetPresenceAsync(
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

            bool isUserActiveInTitle = true;
            std::string error;
            CommandHandlerShared::TryParseBoolParameter(parameters, "isUserActiveInTitle", isUserActiveInTitle, error);

            HRESULT hr = XblPresenceSetPresenceAsync(
                state->xblContext,
                isUserActiveInTitle,
                nullptr,
                &async);
            LogToWindowFormat("XblPresenceSetPresenceAsync (isUserActiveInTitle=%s, hr=0x%08X)",
                isUserActiveInTitle ? "true" : "false", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblPresenceGetPresenceAsync(
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

            int64_t xuid{};
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "xuid", xuid, error))
            {
                RETURN_HR_IF(E_POINTER, !state->xuser);
                uint64_t userId{};
                RETURN_IF_FAILED(XUserGetId(state->xuser, &userId));
                xuid = static_cast<int64_t>(userId);
            }

            HRESULT hr = XblPresenceGetPresenceAsync(state->xblContext, static_cast<uint64_t>(xuid), &async);
            LogToWindowFormat("XblPresenceGetPresenceAsync (xuid=%llu, hr=0x%08X)",
                static_cast<uint64_t>(xuid), static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            if (s_presenceRecord)
            {
                XblPresenceRecordCloseHandle(s_presenceRecord);
                s_presenceRecord = nullptr;
            }
            HRESULT hr = XblPresenceGetPresenceResult(&async, &s_presenceRecord);
            return hr;
        });
}

CommandResultPayload HandleXblPresenceGetPresenceForMultipleUsersAsync(
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

            auto xuids = CommandHandlerShared::GetUint64Array(parameters, "xuids");
            if (xuids.empty())
            {
                RETURN_HR_IF(E_POINTER, !state->xuser);
                uint64_t userId{};
                RETURN_IF_FAILED(XUserGetId(state->xuser, &userId));
                xuids.push_back(userId);
            }

            HRESULT hr = XblPresenceGetPresenceForMultipleUsersAsync(
                state->xblContext,
                xuids.data(),
                xuids.size(),
                nullptr,
                &async);
            LogToWindowFormat("XblPresenceGetPresenceForMultipleUsersAsync (count=%zu, hr=0x%08X)",
                xuids.size(), static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t resultCount{};
            HRESULT hr = XblPresenceGetPresenceForMultipleUsersResultCount(&async, &resultCount);
            if (SUCCEEDED(hr) && resultCount > 0)
            {
                for (auto& h : s_presenceRecords) { if (h) XblPresenceRecordCloseHandle(h); }
                s_presenceRecords.resize(resultCount);
                hr = XblPresenceGetPresenceForMultipleUsersResult(&async, s_presenceRecords.data(), resultCount);
                payload.result["resultCount"] = resultCount;
            }
            return hr;
        });
}

CommandResultPayload HandleXblPresenceGetPresenceForSocialGroupAsync(
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

            std::string socialGroupName, error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "socialGroupName", socialGroupName, error))
            {
                socialGroupName = "Favorites";
            }

            HRESULT hr = XblPresenceGetPresenceForSocialGroupAsync(
                state->xblContext,
                socialGroupName.c_str(),
                nullptr,
                nullptr,
                &async);
            LogToWindowFormat("XblPresenceGetPresenceForSocialGroupAsync (socialGroupName=%s, hr=0x%08X)",
                socialGroupName.c_str(), static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t resultCount{};
            HRESULT hr = XblPresenceGetPresenceForSocialGroupResultCount(&async, &resultCount);
            if (SUCCEEDED(hr) && resultCount > 0)
            {
                for (auto& h : s_presenceRecords) { if (h) XblPresenceRecordCloseHandle(h); }
                s_presenceRecords.resize(resultCount);
                hr = XblPresenceGetPresenceForSocialGroupResult(&async, s_presenceRecords.data(), resultCount);
                payload.result["resultCount"] = resultCount;
            }
            return hr;
        });
}

CommandResultPayload HandleXblPresenceAddDevicePresenceChangedHandler(
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

            XblFunctionContext token = XblPresenceAddDevicePresenceChangedHandler(
                state->xblContext,
                [](void* /*context*/, uint64_t xuid, XblPresenceDeviceType deviceType, bool isUserLoggedOnDevice)
                {
                    LogToWindowFormat("DevicePresenceChanged (xuid=%llu, deviceType=%d, isLoggedOn=%s)",
                        xuid, static_cast<int>(deviceType), isUserLoggedOnDevice ? "true" : "false");
                },
                nullptr);
            LogToWindowFormat("XblPresenceAddDevicePresenceChangedHandler (token=%lld)", static_cast<int64_t>(token));
            payload.result["token"] = static_cast<int64_t>(token);
            return S_OK;
        });
}

CommandResultPayload HandleXblPresenceRemoveDevicePresenceChangedHandler(
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

            int64_t token{};
            std::string error;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "token", token, error);

            XblPresenceRemoveDevicePresenceChangedHandler(
                state->xblContext,
                static_cast<XblFunctionContext>(token));
            LogToWindowFormat("XblPresenceRemoveDevicePresenceChangedHandler (token=%lld)", token);
            return S_OK;
        });
}

CommandResultPayload HandleXblPresenceAddTitlePresenceChangedHandler(
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

            XblFunctionContext token = XblPresenceAddTitlePresenceChangedHandler(
                state->xblContext,
                [](void* /*context*/, uint64_t xuid, uint32_t titleId, XblPresenceTitleState titleState)
                {
                    LogToWindowFormat("TitlePresenceChanged (xuid=%llu, titleId=%u, titleState=%d)",
                        xuid, titleId, static_cast<int>(titleState));
                },
                nullptr);
            LogToWindowFormat("XblPresenceAddTitlePresenceChangedHandler (token=%lld)", static_cast<int64_t>(token));
            payload.result["token"] = static_cast<int64_t>(token);
            return S_OK;
        });
}

CommandResultPayload HandleXblPresenceRemoveTitlePresenceChangedHandler(
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

            int64_t token{};
            std::string error;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "token", token, error);

            XblPresenceRemoveTitlePresenceChangedHandler(
                state->xblContext,
                static_cast<XblFunctionContext>(token));
            LogToWindowFormat("XblPresenceRemoveTitlePresenceChangedHandler (token=%lld)", token);
            return S_OK;
        });
}

CommandResultPayload HandleXblPresenceTrackUsers(
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

            auto xuids = CommandHandlerShared::GetUint64Array(parameters, "xuids");
            if (xuids.empty())
            {
                RETURN_HR_IF(E_POINTER, !state->xuser);
                uint64_t userId{};
                RETURN_IF_FAILED(XUserGetId(state->xuser, &userId));
                xuids.push_back(userId);
            }

            const HRESULT hr = XblPresenceTrackUsers(state->xblContext, xuids.data(), xuids.size());
            LogToWindowFormat("XblPresenceTrackUsers (count=%zu, hr=0x%08X)", xuids.size(), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblPresenceStopTrackingUsers(
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

            auto xuids = CommandHandlerShared::GetUint64Array(parameters, "xuids");
            if (xuids.empty())
            {
                RETURN_HR_IF(E_POINTER, !state->xuser);
                uint64_t userId{};
                RETURN_IF_FAILED(XUserGetId(state->xuser, &userId));
                xuids.push_back(userId);
            }

            const HRESULT hr = XblPresenceStopTrackingUsers(state->xblContext, xuids.data(), xuids.size());
            LogToWindowFormat("XblPresenceStopTrackingUsers (count=%zu, hr=0x%08X)", xuids.size(), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblPresenceTrackAdditionalTitles(
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

            auto ids64 = CommandHandlerShared::GetUint64Array(parameters, "titleIds");
            std::vector<uint32_t> titleIds;
            for (auto id : ids64) { titleIds.push_back(static_cast<uint32_t>(id)); }
            RETURN_HR_IF(E_INVALIDARG, titleIds.empty());

            const HRESULT hr = XblPresenceTrackAdditionalTitles(state->xblContext, titleIds.data(), titleIds.size());
            LogToWindowFormat("XblPresenceTrackAdditionalTitles (count=%zu, hr=0x%08X)", titleIds.size(), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblPresenceStopTrackingAdditionalTitles(
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

            auto ids64 = CommandHandlerShared::GetUint64Array(parameters, "titleIds");
            std::vector<uint32_t> titleIds;
            for (auto id : ids64) { titleIds.push_back(static_cast<uint32_t>(id)); }
            RETURN_HR_IF(E_INVALIDARG, titleIds.empty());

            const HRESULT hr = XblPresenceStopTrackingAdditionalTitles(state->xblContext, titleIds.data(), titleIds.size());
            LogToWindowFormat("XblPresenceStopTrackingAdditionalTitles (count=%zu, hr=0x%08X)", titleIds.size(), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblPresenceGetPresenceResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblPresenceGetPresenceResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblPresenceGetPresenceForMultipleUsersResultCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblPresenceGetPresenceForMultipleUsersResultCount: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblPresenceGetPresenceForMultipleUsersResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblPresenceGetPresenceForMultipleUsersResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblPresenceGetPresenceForSocialGroupResultCount(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblPresenceGetPresenceForSocialGroupResultCount: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblPresenceGetPresenceForSocialGroupResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblPresenceGetPresenceForSocialGroupResult: called inline by Async handler");
            return S_OK;
        });
}


// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XblPresenceAddDevicePresenceChangedHandler", HandleXblPresenceAddDevicePresenceChangedHandler },
    { "XblPresenceAddTitlePresenceChangedHandler", HandleXblPresenceAddTitlePresenceChangedHandler },
    { "XblPresenceGetPresenceAsync", HandleXblPresenceGetPresenceAsync },
    { "XblPresenceGetPresenceForMultipleUsersAsync", HandleXblPresenceGetPresenceForMultipleUsersAsync },
    { "XblPresenceGetPresenceForMultipleUsersResult", HandleXblPresenceGetPresenceForMultipleUsersResult },
    { "XblPresenceGetPresenceForMultipleUsersResultCount", HandleXblPresenceGetPresenceForMultipleUsersResultCount },
    { "XblPresenceGetPresenceForSocialGroupAsync", HandleXblPresenceGetPresenceForSocialGroupAsync },
    { "XblPresenceGetPresenceForSocialGroupResult", HandleXblPresenceGetPresenceForSocialGroupResult },
    { "XblPresenceGetPresenceForSocialGroupResultCount", HandleXblPresenceGetPresenceForSocialGroupResultCount },
    { "XblPresenceGetPresenceResult", HandleXblPresenceGetPresenceResult },
    { "XblPresenceRecordCloseHandle", HandleXblPresenceRecordCloseHandle },
    { "XblPresenceRecordDuplicateHandle", HandleXblPresenceRecordDuplicateHandle },
    { "XblPresenceRecordGetDeviceRecords", HandleXblPresenceRecordGetDeviceRecords },
    { "XblPresenceRecordGetUserState", HandleXblPresenceRecordGetUserState },
    { "XblPresenceRecordGetXuid", HandleXblPresenceRecordGetXuid },
    { "XblPresenceRemoveDevicePresenceChangedHandler", HandleXblPresenceRemoveDevicePresenceChangedHandler },
    { "XblPresenceRemoveTitlePresenceChangedHandler", HandleXblPresenceRemoveTitlePresenceChangedHandler },
    { "XblPresenceSetPresenceAsync", HandleXblPresenceSetPresenceAsync },
    { "XblPresenceStopTrackingAdditionalTitles", HandleXblPresenceStopTrackingAdditionalTitles },
    { "XblPresenceStopTrackingUsers", HandleXblPresenceStopTrackingUsers },
    { "XblPresenceTrackAdditionalTitles", HandleXblPresenceTrackAdditionalTitles },
    { "XblPresenceTrackUsers", HandleXblPresenceTrackUsers }
});

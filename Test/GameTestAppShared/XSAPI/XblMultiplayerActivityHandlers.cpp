#include "pch.h"
#include "XblMultiplayerActivityHandlers.h"
#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

static std::vector<uint8_t> s_activityBuffer;

CommandResultPayload HandleXblMultiplayerActivityUpdateRecentPlayers(
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

            std::vector<XblMultiplayerActivityRecentPlayerUpdate> updates(xuids.size());
            for (size_t i = 0; i < xuids.size(); ++i)
            {
                updates[i].xuid = xuids[i];
                updates[i].encounterType = XblMultiplayerActivityEncounterType::Default;
            }

            HRESULT hr = XblMultiplayerActivityUpdateRecentPlayers(state->xblContext, updates.data(), updates.size());
            LogToWindowFormat("XblMultiplayerActivityUpdateRecentPlayers (count=%zu, hr=0x%08X)",
                xuids.size(), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerActivityFlushRecentPlayersAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCall(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            HRESULT hr = XblMultiplayerActivityFlushRecentPlayersAsync(state->xblContext, &async);
            LogToWindowFormat("XblMultiplayerActivityFlushRecentPlayersAsync (hr=0x%08X)",
                static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerActivitySetActivityAsync(
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

            std::string connectionString;
            if (parameters.contains("connectionString") && parameters["connectionString"].is_string())
            {
                connectionString = parameters["connectionString"].get<std::string>();
            }
            else
            {
                connectionString = "dummyConnectionString";
            }

            int64_t joinRestriction = 0;
            if (parameters.contains("joinRestriction"))
            {
                CommandHandlerShared::TryGetInt64Parameter(parameters, "joinRestriction", joinRestriction, error);
            }
            else
            {
                joinRestriction = 1;
            }

            uint32_t maxPlayers = 0;
            if (parameters.contains("maxPlayers") && parameters["maxPlayers"].is_number())
            {
                maxPlayers = parameters["maxPlayers"].get<uint32_t>();
            }
            else
            {
                maxPlayers = 10;
            }

            uint32_t currentPlayers = 0;
            if (parameters.contains("currentPlayers") && parameters["currentPlayers"].is_number())
            {
                currentPlayers = parameters["currentPlayers"].get<uint32_t>();
            }
            else
            {
                currentPlayers = 1;
            }

            std::string groupId;
            if (parameters.contains("groupId") && parameters["groupId"].is_string())
            {
                groupId = parameters["groupId"].get<std::string>();
            }
            else
            {
                groupId = "dummyGroupId";
            }

            bool allowCrossPlatformJoin = false;
            CommandHandlerShared::TryParseBoolParameter(parameters, "allowCrossPlatformJoin", allowCrossPlatformJoin, error);

            XblMultiplayerActivityInfo info{};
            info.connectionString = connectionString.empty() ? nullptr : connectionString.c_str();
            info.joinRestriction = static_cast<XblMultiplayerActivityJoinRestriction>(joinRestriction);
            info.maxPlayers = maxPlayers;
            info.currentPlayers = currentPlayers;
            info.groupId = groupId.empty() ? nullptr : groupId.c_str();

            HRESULT hr = XblMultiplayerActivitySetActivityAsync(
                state->xblContext,
                &info,
                allowCrossPlatformJoin,
                &async);
            LogToWindowFormat("XblMultiplayerActivitySetActivityAsync (hr=0x%08X)",
                static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerActivityGetActivityAsync(
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

            HRESULT hr = XblMultiplayerActivityGetActivityAsync(
                state->xblContext,
                xuids.data(),
                xuids.size(),
                &async);
            LogToWindowFormat("XblMultiplayerActivityGetActivityAsync (count=%zu, hr=0x%08X)",
                xuids.size(), static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t resultSize{};
            HRESULT hr = XblMultiplayerActivityGetActivityResultSize(&async, &resultSize);
            if (SUCCEEDED(hr) && resultSize > 0)
            {
                s_activityBuffer.resize(resultSize);
                XblMultiplayerActivityInfo* results{};
                size_t resultCount{};
                hr = XblMultiplayerActivityGetActivityResult(&async, resultSize, s_activityBuffer.data(), &results, &resultCount, nullptr);
                if (SUCCEEDED(hr))
                {
                    payload.result["resultCount"] = resultCount;
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerActivityDeleteActivityAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCall(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            HRESULT hr = XblMultiplayerActivityDeleteActivityAsync(state->xblContext, &async);
            LogToWindowFormat("XblMultiplayerActivityDeleteActivityAsync (hr=0x%08X)",
                static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMultiplayerActivityAddInviteHandler(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMultiplayerActivityAddInviteHandler: API not available in this build configuration");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandleXblMultiplayerActivityRemoveInviteHandler(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMultiplayerActivityRemoveInviteHandler: API not available in this build configuration");
            return E_NOTIMPL;
        });
}

CommandResultPayload HandleXblMultiplayerActivityGetActivityResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMultiplayerActivityGetActivityResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerActivityGetActivityResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblMultiplayerActivityGetActivityResult: called inline by Async handler");
            return S_OK;
        });
}


// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XblMultiplayerActivityAddInviteHandler", HandleXblMultiplayerActivityAddInviteHandler },
    { "XblMultiplayerActivityDeleteActivityAsync", HandleXblMultiplayerActivityDeleteActivityAsync },
    { "XblMultiplayerActivityFlushRecentPlayersAsync", HandleXblMultiplayerActivityFlushRecentPlayersAsync },
    { "XblMultiplayerActivityGetActivityAsync", HandleXblMultiplayerActivityGetActivityAsync },
    { "XblMultiplayerActivityGetActivityResult", HandleXblMultiplayerActivityGetActivityResult },
    { "XblMultiplayerActivityGetActivityResultSize", HandleXblMultiplayerActivityGetActivityResultSize },
    { "XblMultiplayerActivityRemoveInviteHandler", HandleXblMultiplayerActivityRemoveInviteHandler },
    { "XblMultiplayerActivitySetActivityAsync", HandleXblMultiplayerActivitySetActivityAsync },
    { "XblMultiplayerActivityUpdateRecentPlayers", HandleXblMultiplayerActivityUpdateRecentPlayers }
});

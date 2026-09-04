#include "pch.h"

#include "XblLeaderboardHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

static std::vector<uint8_t> s_leaderboardBuffer;

CommandResultPayload HandleXblLeaderboardGetLeaderboardAsync(
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

            int64_t xboxUserId{};
            std::string scid, leaderboardName, statName, error;

            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "xboxUserId", xboxUserId, error))
            {
                RETURN_HR_IF(E_POINTER, !state->xuser);
                uint64_t userId{};
                RETURN_IF_FAILED(XUserGetId(state->xuser, &userId));
                xboxUserId = static_cast<int64_t>(userId);
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "scid", scid, error))
            {
                const char* configScid = nullptr;
                RETURN_IF_FAILED(XblGetScid(&configScid));
                scid = configScid;
            }
            CommandHandlerShared::TryGetStringParameter(parameters, "leaderboardName", leaderboardName, error);
            CommandHandlerShared::TryGetStringParameter(parameters, "statName", statName, error);

            int64_t maxItems = 10;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "maxItems", maxItems, error);

            int64_t socialGroup{};
            CommandHandlerShared::TryGetInt64Parameter(parameters, "socialGroup", socialGroup, error);

            int64_t skipToXboxUserId{};
            CommandHandlerShared::TryGetInt64Parameter(parameters, "skipToXboxUserId", skipToXboxUserId, error);

            int64_t skipResultToRank{};
            CommandHandlerShared::TryGetInt64Parameter(parameters, "skipResultToRank", skipResultToRank, error);

            int64_t order{};
            CommandHandlerShared::TryGetInt64Parameter(parameters, "order", order, error);

            XblLeaderboardQuery query{};
            query.xboxUserId = static_cast<uint64_t>(xboxUserId);
            strcpy_s(query.scid, scid.c_str());
            query.leaderboardName = leaderboardName.empty() ? nullptr : leaderboardName.c_str();
            query.statName = statName.empty() ? nullptr : statName.c_str();
            query.socialGroup = static_cast<XblSocialGroupType>(socialGroup);
            query.skipToXboxUserId = static_cast<uint64_t>(skipToXboxUserId);
            query.skipResultToRank = static_cast<uint32_t>(skipResultToRank);
            query.maxItems = static_cast<uint32_t>(maxItems);
            query.order = static_cast<XblLeaderboardSortOrder>(order);

            HRESULT hr = XblLeaderboardGetLeaderboardAsync(state->xblContext, query, &async);
            LogToWindowFormat("XblLeaderboardGetLeaderboardAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t resultSize{};
            HRESULT hr = XblLeaderboardGetLeaderboardResultSize(&async, &resultSize);
            if (SUCCEEDED(hr))
            {
                s_leaderboardBuffer.resize(resultSize);
                XblLeaderboardResult* result{};
                hr = XblLeaderboardGetLeaderboardResult(&async, resultSize, s_leaderboardBuffer.data(), &result, nullptr);
                if (SUCCEEDED(hr) && result)
                {
                    payload.result["rowsCount"] = result->rowsCount;
                    payload.result["columnsCount"] = result->columnsCount;
                    payload.result["hasNext"] = result->hasNext;
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXblLeaderboardResultGetNextAsync(
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

            if (s_leaderboardBuffer.empty())
            {
                LogToWindowFormat("HandleXblLeaderboardResultGetNextAsync: no previous leaderboard result available");
                return E_UNEXPECTED;
            }

            auto* previousResult = reinterpret_cast<XblLeaderboardResult*>(s_leaderboardBuffer.data());
            if (!previousResult->hasNext)
            {
                LogToWindowFormat("HandleXblLeaderboardResultGetNextAsync: previous result has no next page");
                return E_UNEXPECTED;
            }

            int64_t maxItems{};
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "maxItems", maxItems, error))
            {
                return E_INVALIDARG;
            }

            HRESULT hr = XblLeaderboardResultGetNextAsync(state->xblContext, previousResult, static_cast<uint32_t>(maxItems), &async);
            LogToWindowFormat("XblLeaderboardResultGetNextAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t resultSize{};
            HRESULT hr = XblLeaderboardResultGetNextResultSize(&async, &resultSize);
            if (SUCCEEDED(hr))
            {
                s_leaderboardBuffer.resize(resultSize);
                XblLeaderboardResult* result{};
                hr = XblLeaderboardResultGetNextResult(&async, resultSize, s_leaderboardBuffer.data(), &result, nullptr);
                if (SUCCEEDED(hr) && result)
                {
                    payload.result["rowsCount"] = result->rowsCount;
                    payload.result["columnsCount"] = result->columnsCount;
                    payload.result["hasNext"] = result->hasNext;
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXblLeaderboardGetLeaderboardResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblLeaderboardGetLeaderboardResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblLeaderboardGetLeaderboardResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblLeaderboardGetLeaderboardResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblLeaderboardResultGetNextResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblLeaderboardResultGetNextResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblLeaderboardResultGetNextResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblLeaderboardResultGetNextResult: called inline by Async handler");
            return S_OK;
        });
}


// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XblLeaderboardGetLeaderboardAsync", HandleXblLeaderboardGetLeaderboardAsync },
    { "XblLeaderboardGetLeaderboardResult", HandleXblLeaderboardGetLeaderboardResult },
    { "XblLeaderboardGetLeaderboardResultSize", HandleXblLeaderboardGetLeaderboardResultSize },
    { "XblLeaderboardResultGetNextAsync", HandleXblLeaderboardResultGetNextAsync },
    { "XblLeaderboardResultGetNextResult", HandleXblLeaderboardResultGetNextResult },
    { "XblLeaderboardResultGetNextResultSize", HandleXblLeaderboardResultGetNextResultSize }
});

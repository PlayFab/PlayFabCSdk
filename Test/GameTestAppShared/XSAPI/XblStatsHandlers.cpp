#include "pch.h"

#include "XblStatsHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

static std::vector<uint8_t> s_statsBuffer;

static void CALLBACK OnStatisticChanged(XblStatisticChangeEventArgs args, void* /*context*/)
{
    LogToWindowFormat("Statistic changed: %s", args.latestStatistic.statisticName);
}

// ============================================================================
// Title Managed Stats (title_managed_statistics_c.h)
// ============================================================================

CommandResultPayload HandleXblTitleManagedStatsWriteAsync(
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
            std::string statisticName;
            std::string statisticValue;
            int64_t statisticType = 0;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "xboxUserId", xboxUserId, error))
            {
                RETURN_HR_IF(E_POINTER, !state->xuser);
                uint64_t userId{};
                RETURN_IF_FAILED(XUserGetId(state->xuser, &userId));
                xboxUserId = static_cast<int64_t>(userId);
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "statisticName", statisticName, error))
            {
                statisticName = "TotalPuzzlesSolved";
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "statisticValue", statisticValue, error))
            {
                statisticValue = "100";
            }
            CommandHandlerShared::TryGetInt64Parameter(parameters, "statisticType", statisticType, error);

            XblTitleManagedStatistic stat{};
            stat.statisticName = statisticName.c_str();
            stat.statisticType = static_cast<XblTitleManagedStatType>(statisticType);
            stat.numberValue = std::stod(statisticValue);
            stat.stringValue = statisticValue.c_str();

            HRESULT hr = XblTitleManagedStatsWriteAsync(
                state->xblContext,
                static_cast<uint64_t>(xboxUserId),
                &stat,
                1,
                &async);
            LogToWindowFormat("XblTitleManagedStatsWriteAsync (statisticName=%s, hr=0x%08X)",
                statisticName.c_str(), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblTitleManagedStatsUpdateStatsAsync(
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
            std::string statisticName;
            std::string statisticValue;
            int64_t statisticType = 0;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "statisticName", statisticName, error))
            {
                statisticName = "TotalPuzzlesSolved";
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "statisticValue", statisticValue, error))
            {
                statisticValue = "100";
            }
            CommandHandlerShared::TryGetInt64Parameter(parameters, "statisticType", statisticType, error);

            XblTitleManagedStatistic stat{};
            stat.statisticName = statisticName.c_str();
            stat.statisticType = static_cast<XblTitleManagedStatType>(statisticType);
            stat.numberValue = std::stod(statisticValue);
            stat.stringValue = statisticValue.c_str();

            HRESULT hr = XblTitleManagedStatsUpdateStatsAsync(
                state->xblContext,
                &stat,
                1,
                &async);
            LogToWindowFormat("XblTitleManagedStatsUpdateStatsAsync (statisticName=%s, hr=0x%08X)",
                statisticName.c_str(), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblTitleManagedStatsDeleteStatsAsync(
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

            std::vector<std::string> nameStrings;
            std::vector<const char*> namePtrs;
            if (parameters.contains("statisticNames") && parameters["statisticNames"].is_array())
            {
                for (const auto& s : parameters["statisticNames"])
                {
                    if (s.is_string()) nameStrings.push_back(s.get<std::string>());
                }
                for (const auto& s : nameStrings) namePtrs.push_back(s.c_str());
            }
            if (namePtrs.empty())
            {
                nameStrings.push_back("TotalPuzzlesSolved");
                namePtrs.push_back(nameStrings.back().c_str());
            }

            HRESULT hr = XblTitleManagedStatsDeleteStatsAsync(
                state->xblContext,
                namePtrs.data(),
                namePtrs.size(),
                &async);
            LogToWindowFormat("XblTitleManagedStatsDeleteStatsAsync (count=%zu, hr=0x%08X)",
                namePtrs.size(), static_cast<uint32_t>(hr));
            return hr;
        });
}

// ============================================================================
// User Statistics (user_statistics_c.h)
// ============================================================================

CommandResultPayload HandleXblUserStatisticsGetSingleUserStatisticAsync(
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
            std::string statisticName;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "xboxUserId", xboxUserId, error))
            {
                RETURN_HR_IF(E_POINTER, !state->xuser);
                uint64_t userId{};
                RETURN_IF_FAILED(XUserGetId(state->xuser, &userId));
                xboxUserId = static_cast<int64_t>(userId);
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "serviceConfigurationId", serviceConfigurationId, error))
            {
                const char* configScid = nullptr;
                RETURN_IF_FAILED(XblGetScid(&configScid));
                serviceConfigurationId = configScid;
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "statisticName", statisticName, error))
            {
                statisticName = "TotalPuzzlesSolved";
            }

            HRESULT hr = XblUserStatisticsGetSingleUserStatisticAsync(
                state->xblContext,
                static_cast<uint64_t>(xboxUserId),
                serviceConfigurationId.c_str(),
                statisticName.c_str(),
                &async);
            LogToWindowFormat("XblUserStatisticsGetSingleUserStatisticAsync (statisticName=%s, hr=0x%08X)",
                statisticName.c_str(), static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t resultSize{};
            HRESULT hr = XblUserStatisticsGetSingleUserStatisticResultSize(&async, &resultSize);
            if (SUCCEEDED(hr))
            {
                s_statsBuffer.resize(resultSize);
                XblUserStatisticsResult* result{};
                hr = XblUserStatisticsGetSingleUserStatisticResult(
                    &async, resultSize, s_statsBuffer.data(), &result, nullptr);
                if (SUCCEEDED(hr) && result)
                {
                    payload.result["xboxUserId"] = result->xboxUserId;
                    if (result->serviceConfigStatisticsCount > 0 &&
                        result->serviceConfigStatistics[0].statisticsCount > 0)
                    {
                        auto& stat = result->serviceConfigStatistics[0].statistics[0];
                        payload.result["statisticName"] = stat.statisticName;
                        payload.result["statisticType"] = stat.statisticType;
                        payload.result["value"] = stat.value;
                    }
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXblUserStatisticsGetSingleUserStatisticsAsync(
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
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "xboxUserId", xboxUserId, error))
            {
                RETURN_HR_IF(E_POINTER, !state->xuser);
                uint64_t userId{};
                RETURN_IF_FAILED(XUserGetId(state->xuser, &userId));
                xboxUserId = static_cast<int64_t>(userId);
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "serviceConfigurationId", serviceConfigurationId, error))
            {
                const char* configScid = nullptr;
                RETURN_IF_FAILED(XblGetScid(&configScid));
                serviceConfigurationId = configScid;
            }

            std::vector<std::string> nameStrings;
            std::vector<const char*> namePtrs;
            if (parameters.contains("statisticNames") && parameters["statisticNames"].is_array())
            {
                for (const auto& s : parameters["statisticNames"])
                {
                    if (s.is_string()) nameStrings.push_back(s.get<std::string>());
                }
                for (const auto& s : nameStrings) namePtrs.push_back(s.c_str());
            }
            if (namePtrs.empty())
            {
                nameStrings.push_back("TotalPuzzlesSolved");
                namePtrs.push_back(nameStrings.back().c_str());
            }

            HRESULT hr = XblUserStatisticsGetSingleUserStatisticsAsync(
                state->xblContext,
                static_cast<uint64_t>(xboxUserId),
                serviceConfigurationId.c_str(),
                namePtrs.data(),
                namePtrs.size(),
                &async);
            LogToWindowFormat("XblUserStatisticsGetSingleUserStatisticsAsync (count=%zu, hr=0x%08X)",
                namePtrs.size(), static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t resultSize{};
            HRESULT hr = XblUserStatisticsGetSingleUserStatisticsResultSize(&async, &resultSize);
            if (SUCCEEDED(hr))
            {
                s_statsBuffer.resize(resultSize);
                XblUserStatisticsResult* result{};
                hr = XblUserStatisticsGetSingleUserStatisticsResult(
                    &async, resultSize, s_statsBuffer.data(), &result, nullptr);
                if (SUCCEEDED(hr) && result)
                {
                    payload.result["xboxUserId"] = result->xboxUserId;
                    payload.result["serviceConfigStatisticsCount"] = result->serviceConfigStatisticsCount;
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXblUserStatisticsGetMultipleUserStatisticsAsync(
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
            std::string serviceConfigurationId;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "serviceConfigurationId", serviceConfigurationId, error))
            {
                const char* configScid = nullptr;
                RETURN_IF_FAILED(XblGetScid(&configScid));
                serviceConfigurationId = configScid;
            }

            auto xuids = CommandHandlerShared::GetUint64Array(parameters, "xboxUserIds");
            if (xuids.empty())
            {
                RETURN_HR_IF(E_POINTER, !state->xuser);
                uint64_t userId{};
                RETURN_IF_FAILED(XUserGetId(state->xuser, &userId));
                xuids.push_back(userId);
            }

            std::vector<std::string> nameStrings;
            std::vector<const char*> namePtrs;
            if (parameters.contains("statisticNames") && parameters["statisticNames"].is_array())
            {
                for (const auto& s : parameters["statisticNames"])
                {
                    if (s.is_string()) nameStrings.push_back(s.get<std::string>());
                }
                for (const auto& s : nameStrings) namePtrs.push_back(s.c_str());
            }
            if (namePtrs.empty())
            {
                nameStrings.push_back("TotalPuzzlesSolved");
                namePtrs.push_back(nameStrings.back().c_str());
            }

            HRESULT hr = XblUserStatisticsGetMultipleUserStatisticsAsync(
                state->xblContext,
                xuids.data(),
                xuids.size(),
                serviceConfigurationId.c_str(),
                namePtrs.data(),
                namePtrs.size(),
                &async);
            LogToWindowFormat("XblUserStatisticsGetMultipleUserStatisticsAsync (users=%zu, stats=%zu, hr=0x%08X)",
                xuids.size(), namePtrs.size(), static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t resultSize{};
            HRESULT hr = XblUserStatisticsGetMultipleUserStatisticsResultSize(&async, &resultSize);
            if (SUCCEEDED(hr))
            {
                s_statsBuffer.resize(resultSize);
                XblUserStatisticsResult* results{};
                size_t resultsCount{};
                hr = XblUserStatisticsGetMultipleUserStatisticsResult(
                    &async, resultSize, s_statsBuffer.data(), &results, &resultsCount, nullptr);
                if (SUCCEEDED(hr))
                {
                    payload.result["resultsCount"] = resultsCount;
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXblUserStatisticsAddStatisticChangedHandler(
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

            XblFunctionContext token = XblUserStatisticsAddStatisticChangedHandler(
                state->xblContext,
                OnStatisticChanged,
                nullptr);
            LogToWindowFormat("XblUserStatisticsAddStatisticChangedHandler (token=%lld)", static_cast<int64_t>(token));
            payload.result["token"] = static_cast<int64_t>(token);
            return S_OK;
        });
}

CommandResultPayload HandleXblUserStatisticsRemoveStatisticChangedHandler(
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
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "token", token, error))
            {
                return E_INVALIDARG;
            }

            XblUserStatisticsRemoveStatisticChangedHandler(
                state->xblContext,
                static_cast<XblFunctionContext>(token));
            LogToWindowFormat("XblUserStatisticsRemoveStatisticChangedHandler (token=%lld)", token);
            return S_OK;
        });
}

CommandResultPayload HandleXblUserStatisticsTrackStatistics(
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
            std::string serviceConfigurationId;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "serviceConfigurationId", serviceConfigurationId, error))
            {
                const char* configScid = nullptr;
                RETURN_IF_FAILED(XblGetScid(&configScid));
                serviceConfigurationId = configScid;
            }

            auto xuids = CommandHandlerShared::GetUint64Array(parameters, "xboxUserIds");
            if (xuids.empty())
            {
                RETURN_HR_IF(E_POINTER, !state->xuser);
                uint64_t userId{};
                RETURN_IF_FAILED(XUserGetId(state->xuser, &userId));
                xuids.push_back(userId);
            }

            std::vector<std::string> nameStrings;
            std::vector<const char*> namePtrs;
            if (parameters.contains("statisticNames") && parameters["statisticNames"].is_array())
            {
                for (const auto& s : parameters["statisticNames"])
                {
                    if (s.is_string()) nameStrings.push_back(s.get<std::string>());
                }
                for (const auto& s : nameStrings) namePtrs.push_back(s.c_str());
            }
            if (namePtrs.empty())
            {
                nameStrings.push_back("TotalPuzzlesSolved");
                namePtrs.push_back(nameStrings.back().c_str());
            }

            const HRESULT hr = XblUserStatisticsTrackStatistics(
                state->xblContext,
                xuids.data(),
                xuids.size(),
                serviceConfigurationId.c_str(),
                namePtrs.data(),
                namePtrs.size());
            LogToWindowFormat("XblUserStatisticsTrackStatistics (users=%zu, stats=%zu, hr=0x%08X)",
                xuids.size(), namePtrs.size(), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblUserStatisticsStopTrackingStatistics(
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
            std::string serviceConfigurationId;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "serviceConfigurationId", serviceConfigurationId, error))
            {
                const char* configScid = nullptr;
                RETURN_IF_FAILED(XblGetScid(&configScid));
                serviceConfigurationId = configScid;
            }

            auto xuids = CommandHandlerShared::GetUint64Array(parameters, "xboxUserIds");
            if (xuids.empty())
            {
                RETURN_HR_IF(E_POINTER, !state->xuser);
                uint64_t userId{};
                RETURN_IF_FAILED(XUserGetId(state->xuser, &userId));
                xuids.push_back(userId);
            }

            std::vector<std::string> nameStrings;
            std::vector<const char*> namePtrs;
            if (parameters.contains("statisticNames") && parameters["statisticNames"].is_array())
            {
                for (const auto& s : parameters["statisticNames"])
                {
                    if (s.is_string()) nameStrings.push_back(s.get<std::string>());
                }
                for (const auto& s : nameStrings) namePtrs.push_back(s.c_str());
            }
            if (namePtrs.empty())
            {
                nameStrings.push_back("TotalPuzzlesSolved");
                namePtrs.push_back(nameStrings.back().c_str());
            }

            const HRESULT hr = XblUserStatisticsStopTrackingStatistics(
                state->xblContext,
                xuids.data(),
                xuids.size(),
                serviceConfigurationId.c_str(),
                namePtrs.data(),
                namePtrs.size());
            LogToWindowFormat("XblUserStatisticsStopTrackingStatistics (users=%zu, stats=%zu, hr=0x%08X)",
                xuids.size(), namePtrs.size(), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblUserStatisticsStopTrackingUsers(
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

            auto xuids = CommandHandlerShared::GetUint64Array(parameters, "xboxUserIds");
            RETURN_HR_IF(E_INVALIDARG, xuids.empty());

            const HRESULT hr = XblUserStatisticsStopTrackingUsers(
                state->xblContext,
                xuids.data(),
                xuids.size());
            LogToWindowFormat("XblUserStatisticsStopTrackingUsers (count=%zu, hr=0x%08X)",
                xuids.size(), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblUserStatisticsGetSingleUserStatisticResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblUserStatisticsGetSingleUserStatisticResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblUserStatisticsGetSingleUserStatisticResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblUserStatisticsGetSingleUserStatisticResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblUserStatisticsGetSingleUserStatisticsResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblUserStatisticsGetSingleUserStatisticsResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblUserStatisticsGetSingleUserStatisticsResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblUserStatisticsGetSingleUserStatisticsResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblUserStatisticsGetMultipleUserStatisticsResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblUserStatisticsGetMultipleUserStatisticsResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblUserStatisticsGetMultipleUserStatisticsResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblUserStatisticsGetMultipleUserStatisticsResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblUserStatisticsGetMultipleUserStatisticsForMultipleServiceConfigurationsAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);

            auto xuids = CommandHandlerShared::GetUint64Array(parameters, "xboxUserIds");
            if (xuids.empty())
            {
                RETURN_HR_IF(E_POINTER, !state->xuser);
                uint64_t userId{};
                RETURN_IF_FAILED(XUserGetId(state->xuser, &userId));
                xuids.push_back(userId);
            }

            std::string error;
            std::string serviceConfigurationId;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "serviceConfigurationId", serviceConfigurationId, error))
            {
                const char* configScid = nullptr;
                RETURN_IF_FAILED(XblGetScid(&configScid));
                serviceConfigurationId = configScid;
            }

            std::vector<std::string> nameStrings;
            std::vector<const char*> namePtrs;
            if (parameters.contains("statisticNames") && parameters["statisticNames"].is_array())
            {
                for (const auto& s : parameters["statisticNames"])
                {
                    if (s.is_string()) nameStrings.push_back(s.get<std::string>());
                }
                for (const auto& s : nameStrings) namePtrs.push_back(s.c_str());
            }
            if (namePtrs.empty())
            {
                nameStrings.push_back("TotalPuzzlesSolved");
                namePtrs.push_back(nameStrings.back().c_str());
            }

            XblRequestedStatistics requestedStats{};
            strncpy_s(requestedStats.serviceConfigurationId, serviceConfigurationId.c_str(), _TRUNCATE);
            requestedStats.statistics = namePtrs.data();
            requestedStats.statisticsCount = static_cast<uint32_t>(namePtrs.size());

            HRESULT hr = XblUserStatisticsGetMultipleUserStatisticsForMultipleServiceConfigurationsAsync(
                state->xblContext,
                xuids.data(),
                static_cast<uint32_t>(xuids.size()),
                &requestedStats,
                1,
                &async);
            LogToWindowFormat("XblUserStatisticsGetMultipleUserStatisticsForMultipleServiceConfigurationsAsync (hr=0x%08X)",
                static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t resultSize{};
            HRESULT hr = XblUserStatisticsGetMultipleUserStatisticsForMultipleServiceConfigurationsResultSize(&async, &resultSize);
            if (SUCCEEDED(hr))
            {
                s_statsBuffer.resize(resultSize);
                XblUserStatisticsResult* results{};
                size_t resultsCount{};
                hr = XblUserStatisticsGetMultipleUserStatisticsForMultipleServiceConfigurationsResult(
                    &async, resultSize, s_statsBuffer.data(), &results, &resultsCount, nullptr);
                if (SUCCEEDED(hr))
                {
                    payload.result["resultsCount"] = resultsCount;
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXblUserStatisticsGetMultipleUserStatisticsForMultipleServiceConfigurationsResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblUserStatisticsGetMultipleUserStatisticsForMultipleServiceConfigurationsResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXblUserStatisticsGetMultipleUserStatisticsForMultipleServiceConfigurationsResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XblUserStatisticsGetMultipleUserStatisticsForMultipleServiceConfigurationsResult: called inline by Async handler");
            return S_OK;
        });
}


// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XblTitleManagedStatsDeleteStatsAsync", HandleXblTitleManagedStatsDeleteStatsAsync },
    { "XblTitleManagedStatsUpdateStatsAsync", HandleXblTitleManagedStatsUpdateStatsAsync },
    { "XblTitleManagedStatsWriteAsync", HandleXblTitleManagedStatsWriteAsync },
    { "XblUserStatisticsAddStatisticChangedHandler", HandleXblUserStatisticsAddStatisticChangedHandler },
    { "XblUserStatisticsGetMultipleUserStatisticsAsync", HandleXblUserStatisticsGetMultipleUserStatisticsAsync },
    { "XblUserStatisticsGetMultipleUserStatisticsForMultipleServiceConfigurationsAsync", HandleXblUserStatisticsGetMultipleUserStatisticsForMultipleServiceConfigurationsAsync },
    { "XblUserStatisticsGetMultipleUserStatisticsForMultipleServiceConfigurationsResult", HandleXblUserStatisticsGetMultipleUserStatisticsForMultipleServiceConfigurationsResult },
    { "XblUserStatisticsGetMultipleUserStatisticsForMultipleServiceConfigurationsResultSize", HandleXblUserStatisticsGetMultipleUserStatisticsForMultipleServiceConfigurationsResultSize },
    { "XblUserStatisticsGetMultipleUserStatisticsResult", HandleXblUserStatisticsGetMultipleUserStatisticsResult },
    { "XblUserStatisticsGetMultipleUserStatisticsResultSize", HandleXblUserStatisticsGetMultipleUserStatisticsResultSize },
    { "XblUserStatisticsGetSingleUserStatisticAsync", HandleXblUserStatisticsGetSingleUserStatisticAsync },
    { "XblUserStatisticsGetSingleUserStatisticResult", HandleXblUserStatisticsGetSingleUserStatisticResult },
    { "XblUserStatisticsGetSingleUserStatisticResultSize", HandleXblUserStatisticsGetSingleUserStatisticResultSize },
    { "XblUserStatisticsGetSingleUserStatisticsAsync", HandleXblUserStatisticsGetSingleUserStatisticsAsync },
    { "XblUserStatisticsGetSingleUserStatisticsResult", HandleXblUserStatisticsGetSingleUserStatisticsResult },
    { "XblUserStatisticsGetSingleUserStatisticsResultSize", HandleXblUserStatisticsGetSingleUserStatisticsResultSize },
    { "XblUserStatisticsRemoveStatisticChangedHandler", HandleXblUserStatisticsRemoveStatisticChangedHandler },
    { "XblUserStatisticsStopTrackingStatistics", HandleXblUserStatisticsStopTrackingStatistics },
    { "XblUserStatisticsStopTrackingUsers", HandleXblUserStatisticsStopTrackingUsers },
    { "XblUserStatisticsTrackStatistics", HandleXblUserStatisticsTrackStatistics }
});

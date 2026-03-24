#include "pch.h"

#include "GetDebugStats.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"

#include <playfab/gamesave/PFGameSaveFiles.h>
#include "PFGameSaveFilesForDebug.h"

#include <algorithm>
#include <chrono>
#include <string>
#include <vector>
#include <cstring>

namespace
{
    std::string ResolveDeviceName(DeviceGameSaveState* state)
    {
        if (state != nullptr && !state->inputDeviceId.empty())
        {
            return state->inputDeviceId;
        }

        return std::string("Device");
    }
}

GetDebugStatsResult ExecuteGetDebugStats(DeviceGameSaveState* state)
{
    GetDebugStatsResult result{};
    result.deviceName = ResolveDeviceName(state);

    if (!state)
    {
        result.hr = E_POINTER;
        result.errorMessage = "Device state was null.";
        return result;
    }

    if (!state->localUserHandle)
    {
        result.hr = HRESULT_FROM_WIN32(ERROR_INVALID_STATE);
        result.errorMessage = "PFGameSave local user handle is not available.";
        return result;
    }

    size_t size = 0;
    HRESULT hr = PFGameSaveFilesGetStatsJsonSizeForDebug(state->localUserHandle, &size);
    if (FAILED(hr))
    {
        result.hr = hr;
        result.errorMessage = "PFGameSaveFilesGetStatsJsonSizeForDebug failed.";
        return result;
    }

    if (size == 0)
    {
        result.statsJson.clear();
        result.hr = S_OK;
        LogToWindowFormat("GetDebugStats returning empty stats for '%s'", result.deviceName.c_str());
        return result;
    }

    std::vector<char> buffer(size ? size : 1);
    size_t used = 0;
    hr = PFGameSaveFilesGetStatsJsonForDebug(state->localUserHandle, buffer.size(), buffer.data(), &used);
    if (FAILED(hr))
    {
        result.hr = hr;
        result.errorMessage = "PFGameSaveFilesGetStatsJsonForDebug failed.";
        return result;
    }

    size_t length = 0;
    if (used > 0 && used <= buffer.size())
    {
        length = used - 1; // Exclude null terminator.
    }
    else
    {
        length = strnlen_s(buffer.data(), buffer.size());
    }

    result.statsJson.assign(buffer.data(), buffer.data() + length);
    result.hr = S_OK;

    LogToWindowFormat(
        "GetDebugStats retrieved %llu bytes of stats for '%s'",
        static_cast<unsigned long long>(result.statsJson.size()),
        result.deviceName.c_str());

    return result;
}

CommandResultPayload HandleGetDebugStats(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    UNREFERENCED_PARAMETER(parameters);

    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);

    const auto start = std::chrono::steady_clock::now();
    const GetDebugStatsResult result = ExecuteGetDebugStats(state);
    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);

    if (FAILED(result.hr))
    {
        CommandHandlerShared::MarkFailure(payload.result, result.hr, result.errorMessage);
        CommandHandlerShared::SetHResult(payload.result, result.hr);
        return payload;
    }

    payload.result["deviceName"] = result.deviceName;
    payload.result["statsJson"] = result.statsJson;

    CommandHandlerShared::MarkSuccess(payload.result);
    CommandHandlerShared::SetHResult(payload.result, S_OK);
    return payload;
}

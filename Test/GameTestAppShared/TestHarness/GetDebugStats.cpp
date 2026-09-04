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
#include "CommandRegistry.h"

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

    // Parse a JSON array length from the stats JSON.  Returns -1 on missing key.
    int GetArrayCount(const nlohmann::json& statsRoot, const std::string& key)
    {
        try
        {
            if (!statsRoot.contains(key))
            {
                return -1;
            }
            const auto& arr = statsRoot[key];
            if (!arr.is_array())
            {
                return -1;
            }
            return static_cast<int>(arr.size());
        }
        catch (...)
        {
            return -1;
        }
    }

    // Read a nested integer: root[outerKey][innerKey].  Returns -1 on missing.
    int GetNestedInt(const nlohmann::json& statsRoot, const std::string& outerKey, const std::string& innerKey)
    {
        try
        {
            if (!statsRoot.contains(outerKey))
            {
                return -1;
            }
            const auto& outer = statsRoot[outerKey];
            if (!outer.is_object() || !outer.contains(innerKey))
            {
                return -1;
            }
            const auto& val = outer[innerKey];
            if (val.is_number())
            {
                return static_cast<int>(val.get<int64_t>());
            }
            return -1;
        }
        catch (...)
        {
            return -1;
        }
    }

    struct AssertionCheck
    {
        std::string fieldName;
        int expectedValue;
        int actualValue;
        bool passed;
    };

    // Build a single assertion result
    AssertionCheck CheckField(
        const nlohmann::json& statsRoot,
        const nlohmann::json& parameters,
        const std::string& paramName,
        const std::string& statsKey,
        const std::string& nestedKey = "")
    {
        AssertionCheck check{};
        check.fieldName = paramName;
        check.passed = true; // default: skip if param not specified
        check.expectedValue = -1;
        check.actualValue = -1;

        try
        {
            if (!parameters.is_object() || !parameters.contains(paramName))
            {
                return check;
            }

            const auto& paramVal = parameters[paramName];
            if (paramVal.is_number())
            {
                check.expectedValue = paramVal.get<int>();
            }
            else if (paramVal.is_string())
            {
                check.expectedValue = std::stoi(paramVal.get<std::string>());
            }
            else
            {
                LogToWindowFormat("AssertDebugStats: unexpected type for '%s'", paramName.c_str());
                check.passed = false;
                return check;
            }

            if (nestedKey.empty())
            {
                check.actualValue = GetArrayCount(statsRoot, statsKey);
            }
            else
            {
                check.actualValue = GetNestedInt(statsRoot, statsKey, nestedKey);
            }

            check.passed = (check.actualValue == check.expectedValue);
        }
        catch (const std::exception& ex)
        {
            LogToWindowFormat("AssertDebugStats: CheckField exception for '%s': %s", paramName.c_str(), ex.what());
            check.passed = false;
        }

        return check;
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

    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const GetDebugStatsResult result = ExecuteGetDebugStats(state);
            RETURN_IF_FAILED(result.hr);

            payload.result["deviceName"] = result.deviceName;
            payload.result["statsJson"] = result.statsJson;
            return S_OK;
        });
}

CommandResultPayload HandleAssertDebugStats(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            LogToWindowFormat("AssertDebugStats: starting");

            // Collect the debug stats JSON from the SDK
            const GetDebugStatsResult statsResult = ExecuteGetDebugStats(state);
            if (FAILED(statsResult.hr))
            {
                LogToWindowFormat("AssertDebugStats: failed to get stats: 0x%08X %s",
                    statsResult.hr, statsResult.errorMessage.c_str());
                return statsResult.hr;
            }

            LogToWindowFormat("AssertDebugStats: got stats (%zu bytes)", statsResult.statsJson.size());

            // Parse the stats JSON
            nlohmann::json statsRoot;
            if (!statsResult.statsJson.empty())
            {
                try
                {
                    statsRoot = nlohmann::json::parse(statsResult.statsJson);
                }
                catch (const std::exception& ex)
                {
                    LogToWindowFormat("AssertDebugStats: JSON parse failed: %s", ex.what());
                    return E_FAIL;
                }
            }

            LogToWindowFormat("AssertDebugStats: parsed stats, running assertions");

            // Run each requested assertion.
            std::vector<AssertionCheck> checks;
            checks.push_back(CheckField(statsRoot, parameters, "filesToUploadCount",           "FilesToUpload"));
            checks.push_back(CheckField(statsRoot, parameters, "filesToDownloadCount",         "FilesToDownload"));
            checks.push_back(CheckField(statsRoot, parameters, "compressedFilesToUploadCount", "CompressedFilesToUpload"));
            checks.push_back(CheckField(statsRoot, parameters, "compressedFilesToDownloadCount","CompressedFilesToDownload"));
            checks.push_back(CheckField(statsRoot, parameters, "skippedFilesCount",            "SkippedFiles"));
            checks.push_back(CheckField(statsRoot, parameters, "numFilesInFinalizedManifest",  "Upload", "NumFilesInFinalizedManifest"));

            // Evaluate results
            nlohmann::json assertionsJson = nlohmann::json::array();
            bool allPassed = true;
            for (const AssertionCheck& check : checks)
            {
                if (check.expectedValue == -1 && check.actualValue == -1)
                {
                    continue; // param was not specified, skip
                }

                nlohmann::json entry;
                entry["field"] = check.fieldName;
                entry["expected"] = check.expectedValue;
                entry["actual"] = check.actualValue;
                entry["passed"] = check.passed;
                assertionsJson.push_back(entry);

                if (check.passed)
                {
                    LogToWindowFormat("AssertDebugStats: PASS %s expected=%d actual=%d",
                        check.fieldName.c_str(), check.expectedValue, check.actualValue);
                }
                else
                {
                    LogToWindowFormat("AssertDebugStats: FAIL %s expected=%d actual=%d",
                        check.fieldName.c_str(), check.expectedValue, check.actualValue);
                    allPassed = false;
                }
            }

            payload.result["assertions"] = assertionsJson;
            payload.result["statsJson"] = statsResult.statsJson;
            payload.result["allPassed"] = allPassed;

            if (!allPassed)
            {
                LogToWindowFormat("AssertDebugStats: one or more assertions failed");
                return E_FAIL;
            }

            LogToWindowFormat("AssertDebugStats: all assertions passed");
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "GetDebugStats", HandleGetDebugStats },
    { "AssertDebugStats", HandleAssertDebugStats }
});

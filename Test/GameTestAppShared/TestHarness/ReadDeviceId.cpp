#include "pch.h"

#include "ReadDeviceId.h"

#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "DeviceFileSystem.h"
#include "CommandHandlerShared.h"
#include "ResolveSaveRoot.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>

#include <playfab/gamesave/PFGameSaveFiles.h>
#include "CommandRegistry.h"

namespace
{
    namespace fs = std::filesystem;
}

ReadDeviceIdResult ExecuteReadDeviceId(DeviceGameSaveState* state)
{
    ReadDeviceIdResult result{};

    if (!state || !state->localUserHandle)
    {
        result.hr = E_POINTER;
        result.errorMessage = "Local user handle not created";
        return result;
    }

    fs::path rootPath;
    std::string resolveError;
    HRESULT hr = ResolveSaveRoot(state, rootPath, resolveError);
    if (FAILED(hr))
    {
        result.hr = hr;
        result.errorMessage = "ResolveSaveRoot failed: " + resolveError;
        return result;
    }

    auto mountGuard = DeviceFileSystemMount(state->saveFolder);

    // The in-process SDK persists the device id in <saveRoot>/cloudsync/info.json.
    const fs::path infoJsonPath = rootPath / "cloudsync" / "info.json";
    result.infoJsonPath = GetStringFromU8String(infoJsonPath.u8string());

    std::error_code existsEc;
    if (!fs::exists(infoJsonPath, existsEc) || existsEc)
    {
        // Not present — device id has not been persisted at this location.
        result.infoJsonPresent = false;
        result.hr = S_OK;
        LogToWindowFormat("ReadDeviceId: info.json not present at '%s'", result.infoJsonPath.c_str());
        return result;
    }

    std::ifstream file(infoJsonPath, std::ios::binary);
    if (!file.is_open())
    {
        result.hr = E_ACCESSDENIED;
        result.errorMessage = "Failed to open info.json at '" + result.infoJsonPath + "'";
        return result;
    }

    std::ostringstream contentStream;
    contentStream << file.rdbuf();
    file.close();
    const std::string content = contentStream.str();

    try
    {
        const nlohmann::json root = nlohmann::json::parse(content);
        if (root.contains("Data") && root["Data"].is_object() && root["Data"].contains("DeviceID"))
        {
            result.deviceId = root["Data"]["DeviceID"].get<std::string>();
        }
    }
    catch (const std::exception& ex)
    {
        result.hr = E_FAIL;
        result.errorMessage = std::string("Failed to parse info.json: ") + ex.what();
        return result;
    }

    result.infoJsonPresent = true;
    result.hr = S_OK;
    LogToWindowFormat("ReadDeviceId: deviceId='%s' (from '%s')",
        result.deviceId.c_str(), result.infoJsonPath.c_str());
    return result;
}

CommandResultPayload HandleReadDeviceId(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);

    if (!state || !state->localUserHandle)
    {
        CommandHandlerShared::MarkFailure(payload.result, E_POINTER, "Local user handle not created");
        CommandHandlerShared::SetHResult(payload.result, E_POINTER);
        return payload;
    }

    bool requirePresent = true;
    std::string parseError;
    if (parameters.is_object() && parameters.contains("requirePresent"))
    {
        if (!CommandHandlerShared::TryParseBoolParameter(parameters, "requirePresent", requirePresent, parseError))
        {
            CommandHandlerShared::MarkFailure(payload.result, E_INVALIDARG, parseError);
            CommandHandlerShared::SetHResult(payload.result, E_INVALIDARG);
            return payload;
        }
    }

    std::string expectedDeviceId;
    if (parameters.is_object() && parameters.contains("expectedDeviceId") && parameters["expectedDeviceId"].is_string())
    {
        expectedDeviceId = parameters["expectedDeviceId"].get<std::string>();
    }

    const auto start = std::chrono::steady_clock::now();
    const ReadDeviceIdResult result = ExecuteReadDeviceId(state);
    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);

    if (FAILED(result.hr))
    {
        CommandHandlerShared::MarkFailure(payload.result, result.hr, result.errorMessage);
        CommandHandlerShared::SetHResult(payload.result, result.hr);
        return payload;
    }

    payload.result["persistedDeviceId"] = result.deviceId;
    payload.result["infoJsonPresent"] = result.infoJsonPresent;
    payload.result["infoJsonPath"] = result.infoJsonPath;

    // The device id file must exist unless the caller opted out.
    if (requirePresent && (!result.infoJsonPresent || result.deviceId.empty()))
    {
        std::ostringstream oss;
        oss << "Device id not persisted: info.json "
            << (result.infoJsonPresent ? "present but DeviceID empty" : "missing")
            << " at '" << result.infoJsonPath << "'";
        CommandHandlerShared::MarkFailure(payload.result, E_FAIL, oss.str());
        CommandHandlerShared::SetHResult(payload.result, E_FAIL);
        return payload;
    }

    // If an expected id was supplied, the persisted id must match it. This is the
    // core assertion that the device id is STABLE across relaunch sessions.
    if (!expectedDeviceId.empty() && result.deviceId != expectedDeviceId)
    {
        std::ostringstream oss;
        oss << "Device id changed across sessions: expected '" << expectedDeviceId
            << "' but info.json has '" << result.deviceId << "'";
        LogToWindowFormat("ReadDeviceId: FAIL — %s", oss.str().c_str());
        CommandHandlerShared::MarkFailure(payload.result, E_FAIL, oss.str());
        CommandHandlerShared::SetHResult(payload.result, E_FAIL);
        return payload;
    }

    if (!expectedDeviceId.empty())
    {
        LogToWindowFormat("ReadDeviceId: PASS — device id stable across sessions ('%s')",
            result.deviceId.c_str());
    }

    CommandHandlerShared::MarkSuccess(payload.result);
    CommandHandlerShared::SetHResult(payload.result, S_OK);
    return payload;
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "ReadDeviceId", HandleReadDeviceId }
});

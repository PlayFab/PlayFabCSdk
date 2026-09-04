#include "pch.h"

#include "HCTraceHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"

#include <httpClient/trace.h>
#include "CommandRegistry.h"

HC_DEFINE_TRACE_AREA(TestHandler, HCTraceLevel::Verbose);

namespace
{
    HCTraceLevel TraceLevelFromString(const std::string& level)
    {
        const std::string lower = CommandHandlerShared::ToLowerCopy(level);
        if (lower == "off") return HCTraceLevel::Off;
        if (lower == "error") return HCTraceLevel::Error;
        if (lower == "warning") return HCTraceLevel::Warning;
        if (lower == "important") return HCTraceLevel::Important;
        if (lower == "information") return HCTraceLevel::Information;
        if (lower == "verbose") return HCTraceLevel::Verbose;
        return HCTraceLevel::Off;
    }

    const char* TraceLevelToString(HCTraceLevel level)
    {
        switch (level)
        {
        case HCTraceLevel::Off: return "Off";
        case HCTraceLevel::Error: return "Error";
        case HCTraceLevel::Warning: return "Warning";
        case HCTraceLevel::Important: return "Important";
        case HCTraceLevel::Information: return "Information";
        case HCTraceLevel::Verbose: return "Verbose";
        default: return "Off";
        }
    }

    void CALLBACK TraceClientCallback(
        _In_z_ const char* areaName,
        _In_ HCTraceLevel level,
        _In_ uint64_t threadId,
        _In_ uint64_t timestamp,
        _In_z_ const char* message)
    {
        UNREFERENCED_PARAMETER(level);
        UNREFERENCED_PARAMETER(threadId);
        UNREFERENCED_PARAMETER(timestamp);
        LogToWindow(std::string("[") + (areaName ? areaName : "") + "] " + (message ? message : ""));
    }
}

CommandResultPayload HandleHCTraceInit(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HCTraceInit();
            LogToWindow("HCTraceInit executed");
            return S_OK;
        });
}

CommandResultPayload HandleHCTraceCleanup(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HCTraceCleanup();
            LogToWindow("HCTraceCleanup executed");
            return S_OK;
        });
}

CommandResultPayload HandleHCSettingsSetTraceLevel(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string traceLevelStr;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "traceLevel", traceLevelStr, error))
            {
                return E_INVALIDARG;
            }

            const HCTraceLevel level = TraceLevelFromString(traceLevelStr);
            HCSettingsSetTraceLevel(level);
            LogToWindowFormat("HCSettingsSetTraceLevel set to %s", TraceLevelToString(level));
            return S_OK;
        });
}

CommandResultPayload HandleHCSettingsGetTraceLevel(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            HCTraceLevel level = HCTraceLevel::Off;
            HCSettingsGetTraceLevel(&level);
            payload.result["traceLevel"] = TraceLevelToString(level);
            LogToWindowFormat("HCSettingsGetTraceLevel: %s", TraceLevelToString(level));
            return S_OK;
        });
}

CommandResultPayload HandleHCTraceSetClientCallback(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string action = "set";
            std::string error;
            if (parameters.is_object() && parameters.contains("action"))
            {
                CommandHandlerShared::TryGetStringParameter(parameters, "action", action, error);
            }

            if (CommandHandlerShared::ToLowerCopy(action) == "clear")
            {
                HCTraceSetClientCallback(nullptr);
                LogToWindow("HCTraceSetClientCallback cleared");
            }
            else
            {
                HCTraceSetClientCallback(TraceClientCallback);
                LogToWindow("HCTraceSetClientCallback set");
            }
            return S_OK;
        });
}

CommandResultPayload HandleHCTraceSetTraceToDebugger(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            bool traceToDebugger = true;
            std::string error;
            if (!CommandHandlerShared::TryParseBoolParameter(parameters, "traceToDebugger", traceToDebugger, error))
            {
                return E_INVALIDARG;
            }

            HCTraceSetTraceToDebugger(traceToDebugger);
            LogToWindowFormat("HCTraceSetTraceToDebugger: %s", traceToDebugger ? "true" : "false");
            return S_OK;
        });
}

CommandResultPayload HandleHCTraceSetEtwEnabled(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
#if HC_PLATFORM_IS_MICROSOFT
            bool enabled = true;
            std::string error;
            if (!CommandHandlerShared::TryParseBoolParameter(parameters, "enabled", enabled, error))
            {
                return E_INVALIDARG;
            }

            HCTraceSetEtwEnabled(enabled);
            LogToWindowFormat("HCTraceSetEtwEnabled: %s", enabled ? "true" : "false");
            return S_OK;
#else
            LogToWindow("HCTraceSetEtwEnabled not supported on this platform");
            return E_NOTIMPL;
#endif
        });
}

CommandResultPayload HandleHCTraceSetPlatformCallbacks(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = HCTraceSetPlatformCallbacks(nullptr, nullptr, nullptr, nullptr);
            LogToWindowFormat("HCTraceSetPlatformCallbacks (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCTraceImplMessage(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string message;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "message", message, error))
            {
                return E_INVALIDARG;
            }

            std::string levelStr = "Verbose";
            if (parameters.is_object() && parameters.contains("level"))
            {
                CommandHandlerShared::TryGetStringParameter(parameters, "level", levelStr, error);
            }

            const HCTraceLevel level = TraceLevelFromString(levelStr);
            HCTraceImplMessage(&g_traceTestHandler, level, "%s", message.c_str());
            LogToWindowFormat("HCTraceImplMessage: [%s] %s", TraceLevelToString(level), message.c_str());
            return S_OK;
        });
}

CommandResultPayload HandleHCTraceImplMessage_v(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string message;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "message", message, error))
            {
                return E_INVALIDARG;
            }

            const HCTraceLevel level = HCTraceLevel::Verbose;
            HCTraceImplMessage(&g_traceTestHandler, level, "%s", message.c_str());
            LogToWindowFormat("HCTraceImplMessage_v: %s", message.c_str());
            return S_OK;
        });
}

CommandResultPayload HandleHCTraceImplScopeId(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const uint64_t id = HCTraceImplScopeId();
            payload.result["scopeId"] = id;
            LogToWindowFormat("HCTraceImplScopeId: %llu", static_cast<unsigned long long>(id));
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "HCSettingsGetTraceLevel", HandleHCSettingsGetTraceLevel },
    { "HCSettingsSetTraceLevel", HandleHCSettingsSetTraceLevel },
    { "HCTraceCleanup", HandleHCTraceCleanup },
    { "HCTraceImplMessage", HandleHCTraceImplMessage },
    { "HCTraceImplMessage_v", HandleHCTraceImplMessage_v },
    { "HCTraceImplScopeId", HandleHCTraceImplScopeId },
    { "HCTraceInit", HandleHCTraceInit },
    { "HCTraceSetClientCallback", HandleHCTraceSetClientCallback },
    { "HCTraceSetEtwEnabled", HandleHCTraceSetEtwEnabled },
    { "HCTraceSetPlatformCallbacks", HandleHCTraceSetPlatformCallbacks },
    { "HCTraceSetTraceToDebugger", HandleHCTraceSetTraceToDebugger }
});

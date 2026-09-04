#include "pch.h"

#include "XblGlobalHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

CommandResultPayload HandleXblMemSetFunctions(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XblMemSetFunctions(nullptr, nullptr);
            LogToWindowFormat("XblMemSetFunctions (nullptr, nullptr, hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblMemGetFunctions(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XblMemAllocFunction allocFunc = nullptr;
            XblMemFreeFunction freeFunc = nullptr;
            const HRESULT hr = XblMemGetFunctions(&allocFunc, &freeFunc);
            LogToWindowFormat("XblMemGetFunctions (customAlloc=%s, customFree=%s, hr=0x%08X)",
                allocFunc ? "true" : "false",
                freeFunc ? "true" : "false",
                static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["customAllocSet"] = (allocFunc != nullptr);
                payload.result["customFreeSet"] = (freeFunc != nullptr);
            }
            return hr;
        });
}

CommandResultPayload HandleXblGetErrorCondition(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            int64_t hrParam{};
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "hr", hrParam, error))
            {
                return E_INVALIDARG;
            }

            const XblErrorCondition condition = XblGetErrorCondition(static_cast<HRESULT>(hrParam));
            LogToWindowFormat("XblGetErrorCondition (hr=0x%08X, condition=%d)",
                static_cast<uint32_t>(hrParam), static_cast<int>(condition));
            payload.result["errorCondition"] = static_cast<int>(condition);
            return S_OK;
        });
}

CommandResultPayload HandleXblGetScid(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const char* scid = nullptr;
            const HRESULT hr = XblGetScid(&scid);
            LogToWindowFormat("XblGetScid (scid=%s, hr=0x%08X)",
                scid ? scid : "(null)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr) && scid)
            {
                payload.result["scid"] = scid;
            }
            return hr;
        });
}

CommandResultPayload HandleXblDisableAssertsForXboxLiveThrottlingInDevSandboxes(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            int64_t settingValue{};
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "setting", settingValue, error))
            {
                return E_INVALIDARG;
            }

            XblDisableAssertsForXboxLiveThrottlingInDevSandboxes(
                static_cast<XblConfigSetting>(settingValue));
            LogToWindowFormat("XblDisableAssertsForXboxLiveThrottlingInDevSandboxes (setting=%lld)",
                static_cast<long long>(settingValue));
            return S_OK;
        });
}

CommandResultPayload HandleXblSetOverrideConfiguration(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string overrideScid;
            int64_t overrideTitleId{};
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "overrideScid", overrideScid, error))
            {
                return E_INVALIDARG;
            }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "overrideTitleId", overrideTitleId, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblSetOverrideConfiguration(
                overrideScid.c_str(), static_cast<uint32_t>(overrideTitleId));
            LogToWindowFormat("XblSetOverrideConfiguration (scid=%s, titleId=%u, hr=0x%08X)",
                overrideScid.c_str(), static_cast<uint32_t>(overrideTitleId), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblSetOverrideLocale(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string overrideLocale;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "overrideLocale", overrideLocale, error))
            {
                overrideLocale = "en-US";
            }

            const HRESULT hr = XblSetOverrideLocale(overrideLocale.c_str());
            LogToWindowFormat("XblSetOverrideLocale (locale=%s, hr=0x%08X)",
                overrideLocale.c_str(), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblAddServiceCallRoutedHandler(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XblFunctionContext token = XblAddServiceCallRoutedHandler(
                [](XblServiceCallRoutedArgs args, void* /*context*/)
                {
                    LogToWindowFormat("XblServiceCallRouted (responseCount=%llu, response=%s)",
                        static_cast<unsigned long long>(args.responseCount),
                        args.fullResponseFormatted ? args.fullResponseFormatted : "");
                },
                nullptr);
            LogToWindowFormat("XblAddServiceCallRoutedHandler (token=%lld)",
                static_cast<long long>(token));
            payload.result["handlerToken"] = static_cast<int64_t>(token);
            return S_OK;
        });
}

CommandResultPayload HandleXblRemoveServiceCallRoutedHandler(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            int64_t token{};
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "token", token, error))
            {
                token = 0;
            }

            XblRemoveServiceCallRoutedHandler(static_cast<XblFunctionContext>(token));
            LogToWindowFormat("XblRemoveServiceCallRoutedHandler (token=%lld)",
                static_cast<long long>(token));
            return S_OK;
        });
}

CommandResultPayload HandleXblGetAsyncQueue(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XTaskQueueHandle queue = XblGetAsyncQueue();
            const bool hasQueue = (queue != nullptr);
            LogToWindowFormat("XblGetAsyncQueue (hasQueue=%s)", hasQueue ? "true" : "false");
            payload.result["hasQueue"] = hasQueue;
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XblAddServiceCallRoutedHandler", HandleXblAddServiceCallRoutedHandler },
    { "XblDisableAssertsForXboxLiveThrottlingInDevSandboxes", HandleXblDisableAssertsForXboxLiveThrottlingInDevSandboxes },
    { "XblGetAsyncQueue", HandleXblGetAsyncQueue },
    { "XblGetErrorCondition", HandleXblGetErrorCondition },
    { "XblGetScid", HandleXblGetScid },
    { "XblMemGetFunctions", HandleXblMemGetFunctions },
    { "XblMemSetFunctions", HandleXblMemSetFunctions },
    { "XblRemoveServiceCallRoutedHandler", HandleXblRemoveServiceCallRoutedHandler },
    { "XblSetOverrideConfiguration", HandleXblSetOverrideConfiguration },
    { "XblSetOverrideLocale", HandleXblSetOverrideLocale }
});

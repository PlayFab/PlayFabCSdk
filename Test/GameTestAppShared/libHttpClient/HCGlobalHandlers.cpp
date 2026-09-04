#include "pch.h"

#include "HCGlobalHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"

#include <httpClient/httpClient.h>
#include <httpClient/httpProvider.h>
#include "CommandRegistry.h"

void* STDAPIVCALLTYPE CustomHCMemAlloc(
    _In_ size_t size,
    _In_ HCMemoryType memoryType)
{
    UNREFERENCED_PARAMETER(memoryType);
    GetSampleGameSaveState()->hcMemAllocCount.fetch_add(1, std::memory_order_relaxed);
    return malloc(size);
}

void STDAPIVCALLTYPE CustomHCMemFree(
    _In_ _Post_invalid_ void* pointer,
    _In_ HCMemoryType memoryType)
{
    UNREFERENCED_PARAMETER(memoryType);
    GetSampleGameSaveState()->hcMemFreeCount.fetch_add(1, std::memory_order_relaxed);
    free(pointer);
}

namespace
{
    void STDAPIVCALLTYPE CallRoutedCallback(
        _In_ HCCallHandle call,
        _In_opt_ void* context)
    {
        UNREFERENCED_PARAMETER(context);
        const char* url = nullptr;
        HCHttpCallGetRequestUrl(call, &url);
        LogToWindow("HCCallRoutedHandler: call routed to " + std::string(url ? url : ""));
    }

    void STDAPIVCALLTYPE WebSocketRoutedCallback(
        _In_ HCWebsocketHandle websocket,
        _In_ bool receiving,
        _In_opt_z_ const char* message,
        _In_opt_ const uint8_t* payloadBytes,
        _In_ size_t payloadSize,
        _In_opt_ void* context)
    {
        UNREFERENCED_PARAMETER(websocket);
        UNREFERENCED_PARAMETER(payloadBytes);
        UNREFERENCED_PARAMETER(payloadSize);
        UNREFERENCED_PARAMETER(context);
        LogToWindow(std::string("HCWebSocketRoutedHandler: ") + (receiving ? "recv" : "send") + " " + (message ? message : "(binary)"));
    }
}

CommandResultPayload HandleHCMemSetFunctions(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = HCMemSetFunctions(nullptr, nullptr);
            LogToWindowFormat("HCMemSetFunctions (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCMemGetFunctions(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HCMemAllocFunction memAllocFunc = nullptr;
            HCMemFreeFunction memFreeFunc = nullptr;
            const HRESULT hr = HCMemGetFunctions(&memAllocFunc, &memFreeFunc);
            LogToWindowFormat("HCMemGetFunctions (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCInitialize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = HCInitialize(nullptr);
            LogToWindowFormat("HCInitialize (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCIsInitialized(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            bool result = HCIsInitialized();
            payload.result["isInitialized"] = result;
            LogToWindowFormat("HCIsInitialized: %s", result ? "true" : "false");
            return S_OK;
        });
}

CommandResultPayload HandleHCCleanup(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HCCleanup();
            LogToWindow("HCCleanup executed");
            return S_OK;
        });
}

CommandResultPayload HandleHCCleanupAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCall(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = HCCleanupAsync(&async);
            LogToWindowFormat("HCCleanupAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleHCGetLibVersion(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const char* version = nullptr;
            const HRESULT hr = HCGetLibVersion(&version);
            LogToWindowFormat("HCGetLibVersion (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["version"] = version ? version : "";
            return S_OK;
        });
}

CommandResultPayload HandleHCAddCallRoutedHandler(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            state->hcCallRoutedHandlerId = HCAddCallRoutedHandler(CallRoutedCallback, nullptr);
            LogToWindowFormat("HCAddCallRoutedHandler (id=%d)", state->hcCallRoutedHandlerId);
            return S_OK;
        });
}

CommandResultPayload HandleHCRemoveCallRoutedHandler(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HCRemoveCallRoutedHandler(state->hcCallRoutedHandlerId);
            LogToWindowFormat("HCRemoveCallRoutedHandler (id=%d)", state->hcCallRoutedHandlerId);
            state->hcCallRoutedHandlerId = 0;
            return S_OK;
        });
}

CommandResultPayload HandleHCAddWebSocketRoutedHandler(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            state->hcWebSocketRoutedHandlerId = HCAddWebSocketRoutedHandler(WebSocketRoutedCallback, nullptr);
            LogToWindowFormat("HCAddWebSocketRoutedHandler (id=%d)", state->hcWebSocketRoutedHandlerId);
            return S_OK;
        });
}

CommandResultPayload HandleHCRemoveWebSocketRoutedHandler(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HCRemoveWebSocketRoutedHandler(state->hcWebSocketRoutedHandlerId);
            LogToWindowFormat("HCRemoveWebSocketRoutedHandler (id=%d)", state->hcWebSocketRoutedHandlerId);
            state->hcWebSocketRoutedHandlerId = 0;
            return S_OK;
        });
}

CommandResultPayload HandleHCSetGlobalProxy(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string proxyUri;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "proxyUri", proxyUri, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCSetGlobalProxy(proxyUri.c_str());
            LogToWindowFormat("HCSetGlobalProxy (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

// Resolved at runtime rather than linked, so the app still loads against an older libHttpClient.
//
// These two entry points were added late (LHC "Add PLM suspend and request-limit features to
// WinHTTP"). A static import makes the whole app fail to start with 0xC0000139 against any earlier
// libHttpClient - including the build that ships in the 2510 GDK, which is exactly what a QFE has
// to be validated against. Nothing outside these two commands uses them, so a hard dependency
// would block every unrelated scenario for no benefit. Same approach as
// PFGameSaveFilesSetForceInprocForDebug in PFGameSaveFilesHandlers.cpp.
namespace
{

using HCSettingsSetGlobalRequestLimitFn = HRESULT(CALLBACK*)(uint32_t);
using HCSettingsGetGlobalRequestLimitFn = HRESULT(CALLBACK*)(uint32_t*);

template<typename TFn>
TFn ResolveHCExport(const char* name)
{
    HMODULE hMod = GetModuleHandleW(L"libHttpClient.dll");
    if (!hMod)
    {
        // Fall back to the suffixed name so this also works when the app is hosted alongside a
        // platform-suffixed libHttpClient build.
        hMod = GetModuleHandleW(L"libHttpClient.GDK.dll");
    }
    return hMod ? reinterpret_cast<TFn>(GetProcAddress(hMod, name)) : nullptr;
}

} // anonymous namespace

CommandResultPayload HandleHCSettingsSetGlobalRequestLimit(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            int64_t limit = 0;
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "limit", limit, error))
            {
                return E_INVALIDARG;
            }

            // Range-check before narrowing: a negative or oversized YAML value would otherwise wrap
            // to a huge cap, silently disabling the throttling the scenario is trying to assert.
            if (limit < 0 || limit > static_cast<int64_t>(UINT32_MAX))
            {
                LogToWindowFormat("HCSettingsSetGlobalRequestLimit: limit %lld out of range [0, %u]", limit, UINT32_MAX);
                return E_INVALIDARG;
            }

            auto pfn = ResolveHCExport<HCSettingsSetGlobalRequestLimitFn>("HCSettingsSetGlobalRequestLimit");
            if (!pfn)
            {
                LogToWindowFormat("HCSettingsSetGlobalRequestLimit not available in this libHttpClient build");
                return E_NOTIMPL;
            }

            const HRESULT hr = pfn(static_cast<uint32_t>(limit));
            LogToWindowFormat("HCSettingsSetGlobalRequestLimit (limit=%lld hr=0x%08X)", limit, static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCSettingsGetGlobalRequestLimit(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            auto pfn = ResolveHCExport<HCSettingsGetGlobalRequestLimitFn>("HCSettingsGetGlobalRequestLimit");
            if (!pfn)
            {
                LogToWindowFormat("HCSettingsGetGlobalRequestLimit not available in this libHttpClient build");
                return E_NOTIMPL;
            }

            uint32_t limit = 0;
            const HRESULT hr = pfn(&limit);
            LogToWindowFormat("HCSettingsGetGlobalRequestLimit (limit=%u hr=0x%08X)", limit, static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);

            payload.result["limit"] = limit;

            // Optional assertion so scenarios can pin the value without a separate compare step.
            int64_t expected = 0;
            std::string error;
            if (CommandHandlerShared::TryGetInt64Parameter(parameters, "expectedLimit", expected, error))
            {
                if (static_cast<uint32_t>(expected) != limit)
                {
                    LogToWindowFormat("HCSettingsGetGlobalRequestLimit: expected %lld but got %u", expected, limit);
                    return E_FAIL;
                }
            }
            return S_OK;
        });
}

CommandResultPayload HandleHCSetHttpCallPerformFunction(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = HCSetHttpCallPerformFunction(nullptr, nullptr);
            LogToWindowFormat("HCSetHttpCallPerformFunction (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCGetHttpCallPerformFunction(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HCCallPerformFunction performFunc = nullptr;
            void* performContext = nullptr;
            const HRESULT hr = HCGetHttpCallPerformFunction(&performFunc, &performContext);
            LogToWindowFormat("HCGetHttpCallPerformFunction (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCSetWebSocketFunctions(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = HCSetWebSocketFunctions(nullptr, nullptr, nullptr, nullptr, nullptr);
            LogToWindowFormat("HCSetWebSocketFunctions (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCGetWebSocketFunctions(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HCWebSocketConnectFunction connectFunc = nullptr;
            HCWebSocketSendMessageFunction sendFunc = nullptr;
            HCWebSocketSendBinaryMessageFunction sendBinaryFunc = nullptr;
            HCWebSocketDisconnectFunction disconnectFunc = nullptr;
            void* wsContext = nullptr;
            const HRESULT hr = HCGetWebSocketFunctions(&connectFunc, &sendFunc, &sendBinaryFunc, &disconnectFunc, &wsContext);
            LogToWindowFormat("HCGetWebSocketFunctions (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpDisableAssertsForSSLValidationInDevSandboxes(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_GDK || defined(HC_WINHTTP_WIN32_NOXASYNC)
            const HRESULT hr = HCHttpDisableAssertsForSSLValidationInDevSandboxes(HCConfigSetting::SSLValidationEnforcedInRetailSandbox);
            LogToWindowFormat("HCHttpDisableAssertsForSSLValidationInDevSandboxes (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
#else
            LogToWindow("HCHttpDisableAssertsForSSLValidationInDevSandboxes not supported on this platform");
#endif
            return S_OK;
        });
}

CommandResultPayload HandleHCMemGetAllocStats(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint64_t allocCount = state->hcMemAllocCount.load(std::memory_order_relaxed);
            uint64_t freeCount = state->hcMemFreeCount.load(std::memory_order_relaxed);
            LogToWindowFormat("HCMemGetAllocStats: allocCount=%llu, freeCount=%llu", allocCount, freeCount);
            payload.result["allocCount"] = allocCount;
            payload.result["freeCount"] = freeCount;
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "HCAddCallRoutedHandler", HandleHCAddCallRoutedHandler },
    { "HCAddWebSocketRoutedHandler", HandleHCAddWebSocketRoutedHandler },
    { "HCCleanup", HandleHCCleanup },
    { "HCCleanupAsync", HandleHCCleanupAsync },
    { "HCGetHttpCallPerformFunction", HandleHCGetHttpCallPerformFunction },
    { "HCGetLibVersion", HandleHCGetLibVersion },
    { "HCGetWebSocketFunctions", HandleHCGetWebSocketFunctions },
    { "HCHttpDisableAssertsForSSLValidationInDevSandboxes", HandleHCHttpDisableAssertsForSSLValidationInDevSandboxes },
    { "HCInitialize", HandleHCInitialize },
    { "HCIsInitialized", HandleHCIsInitialized },
    { "HCMemGetAllocStats", HandleHCMemGetAllocStats },
    { "HCMemGetFunctions", HandleHCMemGetFunctions },
    { "HCMemSetFunctions", HandleHCMemSetFunctions },
    { "HCRemoveCallRoutedHandler", HandleHCRemoveCallRoutedHandler },
    { "HCRemoveWebSocketRoutedHandler", HandleHCRemoveWebSocketRoutedHandler },
    { "HCSetGlobalProxy", HandleHCSetGlobalProxy },
    { "HCSettingsGetGlobalRequestLimit", HandleHCSettingsGetGlobalRequestLimit },
    { "HCSettingsSetGlobalRequestLimit", HandleHCSettingsSetGlobalRequestLimit },
    { "HCSetHttpCallPerformFunction", HandleHCSetHttpCallPerformFunction },
    { "HCSetWebSocketFunctions", HandleHCSetWebSocketFunctions }
});

#include "pch.h"

#include "HCWebSocketHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"

#include <httpClient/httpClient.h>
#include <httpClient/httpProvider.h>
#include "CommandRegistry.h"

#include <chrono>
#include <thread>

// Records the close event so scenarios can assert that a socket really was torn down (for example
// by a PLM suspend) rather than inferring it from a later failure.
static void CALLBACK OnWebSocketClosed(
    _In_ HCWebsocketHandle /*websocket*/,
    _In_ HCWebSocketCloseStatus closeStatus,
    _In_ void* functionContext)
{
    auto* state = static_cast<DeviceGameSaveState*>(functionContext);
    if (state == nullptr)
    {
        return;
    }

    state->hcWebSocketCloseStatus.store(static_cast<int32_t>(closeStatus), std::memory_order_relaxed);
    state->hcWebSocketCloseEventFired.store(true, std::memory_order_release);
    LogToWindowFormat("WebSocket close event (closeStatus=%d)", static_cast<int32_t>(closeStatus));
}

CommandResultPayload HandleHCWebSocketCreate(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            // Clear any close recorded for a previous socket so an assertion cannot pass on stale
            // state from an earlier block.
            state->hcWebSocketCloseEventFired.store(false, std::memory_order_relaxed);
            state->hcWebSocketCloseStatus.store(0, std::memory_order_relaxed);

            HCWebsocketHandle ws = nullptr;
            const HRESULT hr = HCWebSocketCreate(&ws, nullptr, nullptr, OnWebSocketClosed, state);
            LogToWindowFormat("HCWebSocketCreate (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            state->hcWebSocket = ws;
            return S_OK;
        });
}

CommandResultPayload HandleHCWebSocketConnectAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCall(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            std::string uri;
            std::string subProtocol;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "uri", uri, error))
            {
                return E_INVALIDARG;
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "subProtocol", subProtocol, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCWebSocketConnectAsync(uri.c_str(), subProtocol.c_str(), state->hcWebSocket, &async);
            LogToWindowFormat("HCWebSocketConnectAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleHCGetWebSocketConnectResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("HCGetWebSocketConnectResult: result is obtained via async completion");
            return S_OK;
        });
}

CommandResultPayload HandleHCWebSocketSendMessageAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCall(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            std::string message;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "message", message, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCWebSocketSendMessageAsync(state->hcWebSocket, message.c_str(), &async);
            LogToWindowFormat("HCWebSocketSendMessageAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleHCWebSocketSendBinaryMessageAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCall(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            std::string payload;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "payload", payload, error))
            {
                return E_INVALIDARG;
            }

            const auto* bytes = reinterpret_cast<const uint8_t*>(payload.data());
            const uint32_t size = static_cast<uint32_t>(payload.size());
            const HRESULT hr = HCWebSocketSendBinaryMessageAsync(state->hcWebSocket, bytes, size, &async);
            LogToWindowFormat("HCWebSocketSendBinaryMessageAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleHCGetWebSocketSendMessageResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("HCGetWebSocketSendMessageResult: result is obtained via async completion");
            return S_OK;
        });
}

CommandResultPayload HandleHCWebSocketDisconnect(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = HCWebSocketDisconnect(state->hcWebSocket);
            LogToWindowFormat("HCWebSocketDisconnect (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCWebSocketDuplicateHandle(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HCWebsocketHandle duplicatedHandle = HCWebSocketDuplicateHandle(state->hcWebSocket);
            LogToWindowFormat("HCWebSocketDuplicateHandle (handle=%p)", duplicatedHandle);
            state->hcWebSocket = duplicatedHandle;
            return S_OK;
        });
}

CommandResultPayload HandleHCWebSocketCloseHandle(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HCWebSocketCloseHandle(state->hcWebSocket);
            LogToWindow("HCWebSocketCloseHandle");
            state->hcWebSocket = nullptr;
            return S_OK;
        });
}

// Asserts that the close event fired for the current socket, waiting up to timeoutMs because the
// close is delivered asynchronously (a PLM suspend tears the connection down on another thread).
// Set expectClosed: false to assert the socket is still open.
CommandResultPayload HandleHCWebSocketAssertClosed(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            std::string parseError;

            bool expectClosed = true;
            CommandHandlerShared::TryParseBoolParameter(parameters, "expectClosed", expectClosed, parseError);

            int64_t timeoutMs = 30000;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "timeoutMs", timeoutMs, parseError);

            const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeoutMs);
            bool fired = state->hcWebSocketCloseEventFired.load(std::memory_order_acquire);
            while (expectClosed && !fired && std::chrono::steady_clock::now() < deadline)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                fired = state->hcWebSocketCloseEventFired.load(std::memory_order_acquire);
            }

            const int32_t closeStatus = state->hcWebSocketCloseStatus.load(std::memory_order_relaxed);
            payload.result["closeEventFired"] = fired;
            payload.result["closeStatus"] = closeStatus;
            LogToWindowFormat("HCWebSocketAssertClosed (fired=%d closeStatus=%d expectClosed=%d)",
                fired ? 1 : 0, closeStatus, expectClosed ? 1 : 0);

            if (fired != expectClosed)
            {
                LogToWindowFormat("HCWebSocketAssertClosed: expected closeEventFired=%d but got %d",
                    expectClosed ? 1 : 0, fired ? 1 : 0);
                return E_FAIL;
            }

            return S_OK;
        });
}

CommandResultPayload HandleHCWebSocketSetProxyUri(
    DeviceGameSaveState* state,
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

            const HRESULT hr = HCWebSocketSetProxyUri(state->hcWebSocket, proxyUri.c_str());
            LogToWindowFormat("HCWebSocketSetProxyUri (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCWebSocketSetProxyDecryptsHttps(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_WIN32 && !HC_WINHTTP_WEBSOCKETS
            bool allow = true;
            std::string error;
            if (!CommandHandlerShared::TryParseBoolParameter(parameters, "allow", allow, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCWebSocketSetProxyDecryptsHttps(state->hcWebSocket, allow);
            LogToWindowFormat("HCWebSocketSetProxyDecryptsHttps (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
#else
            LogToWindow("HCWebSocketSetProxyDecryptsHttps not supported on this platform");
            return E_NOTIMPL;
#endif
        });
}

CommandResultPayload HandleHCWebSocketSetHeader(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string headerName;
            std::string headerValue;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "headerName", headerName, error))
            {
                return E_INVALIDARG;
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "headerValue", headerValue, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCWebSocketSetHeader(state->hcWebSocket, headerName.c_str(), headerValue.c_str());
            LogToWindowFormat("HCWebSocketSetHeader (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCWebSocketSetPingInterval(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            int64_t pingIntervalSeconds = 0;
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "pingIntervalSeconds", pingIntervalSeconds, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCWebSocketSetPingInterval(state->hcWebSocket, static_cast<uint32_t>(pingIntervalSeconds));
            LogToWindowFormat("HCWebSocketSetPingInterval (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCWebSocketSetMaxReceiveBufferSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
            int64_t bufferSizeInBytes = 0;
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "bufferSizeInBytes", bufferSizeInBytes, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCWebSocketSetMaxReceiveBufferSize(state->hcWebSocket, static_cast<size_t>(bufferSizeInBytes));
            LogToWindowFormat("HCWebSocketSetMaxReceiveBufferSize (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
#else
            LogToWindow("HCWebSocketSetMaxReceiveBufferSize not supported on this platform");
            return E_NOTIMPL;
#endif
        });
}

CommandResultPayload HandleHCWebSocketSetBinaryMessageFragmentEventFunction(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
            const HRESULT hr = HCWebSocketSetBinaryMessageFragmentEventFunction(state->hcWebSocket, nullptr);
            LogToWindowFormat("HCWebSocketSetBinaryMessageFragmentEventFunction (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
#else
            LogToWindow("HCWebSocketSetBinaryMessageFragmentEventFunction not supported on this platform");
            return E_NOTIMPL;
#endif
        });
}

CommandResultPayload HandleHCWebSocketGetEventFunctions(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            void* ctx = nullptr;
            const HRESULT hr = HCWebSocketGetEventFunctions(state->hcWebSocket, nullptr, nullptr, nullptr, &ctx);
            LogToWindowFormat("HCWebSocketGetEventFunctions (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["hasContext"] = (ctx != nullptr);
            return S_OK;
        });
}

CommandResultPayload HandleHCWebSocketGetBinaryMessageFragmentEventFunction(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
            HCWebSocketBinaryMessageFragmentFunction func = nullptr;
            void* ctx = nullptr;
            const HRESULT hr = HCWebSocketGetBinaryMessageFragmentEventFunction(state->hcWebSocket, &func, &ctx);
            LogToWindowFormat("HCWebSocketGetBinaryMessageFragmentEventFunction (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
#else
            LogToWindow("HCWebSocketGetBinaryMessageFragmentEventFunction not supported on this platform");
            return E_NOTIMPL;
#endif
        });
}

CommandResultPayload HandleHCWebSocketGetProxyUri(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const char* proxyUri = nullptr;
            const HRESULT hr = HCWebSocketGetProxyUri(state->hcWebSocket, &proxyUri);
            LogToWindowFormat("HCWebSocketGetProxyUri (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["proxyUri"] = proxyUri ? proxyUri : "";
            return S_OK;
        });
}

CommandResultPayload HandleHCWebSocketGetHeader(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            std::string headerName;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "headerName", headerName, error))
            {
                return E_INVALIDARG;
            }

            const char* headerValue = nullptr;
            const HRESULT hr = HCWebSocketGetHeader(state->hcWebSocket, headerName.c_str(), &headerValue);
            LogToWindowFormat("HCWebSocketGetHeader (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["headerValue"] = headerValue ? headerValue : "";
            return S_OK;
        });
}

CommandResultPayload HandleHCWebSocketGetNumHeaders(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t numHeaders = 0;
            const HRESULT hr = HCWebSocketGetNumHeaders(state->hcWebSocket, &numHeaders);
            LogToWindowFormat("HCWebSocketGetNumHeaders (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["numHeaders"] = numHeaders;
            return S_OK;
        });
}

CommandResultPayload HandleHCWebSocketGetHeaderAtIndex(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            int64_t headerIndex = 0;
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "headerIndex", headerIndex, error))
            {
                return E_INVALIDARG;
            }

            const char* headerName = nullptr;
            const char* headerValue = nullptr;
            const HRESULT hr = HCWebSocketGetHeaderAtIndex(state->hcWebSocket, static_cast<uint32_t>(headerIndex), &headerName, &headerValue);
            LogToWindowFormat("HCWebSocketGetHeaderAtIndex (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["headerName"] = headerName ? headerName : "";
            payload.result["headerValue"] = headerValue ? headerValue : "";
            return S_OK;
        });
}

CommandResultPayload HandleHCWebSocketGetPingInterval(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t pingIntervalSeconds = 0;
            const HRESULT hr = HCWebSocketGetPingInterval(state->hcWebSocket, &pingIntervalSeconds);
            LogToWindowFormat("HCWebSocketGetPingInterval (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["pingIntervalSeconds"] = pingIntervalSeconds;
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "HCGetWebSocketConnectResult", HandleHCGetWebSocketConnectResult },
    { "HCGetWebSocketSendMessageResult", HandleHCGetWebSocketSendMessageResult },
    { "HCWebSocketAssertClosed", HandleHCWebSocketAssertClosed },
    { "HCWebSocketCloseHandle", HandleHCWebSocketCloseHandle },
    { "HCWebSocketConnectAsync", HandleHCWebSocketConnectAsync },
    { "HCWebSocketCreate", HandleHCWebSocketCreate },
    { "HCWebSocketDisconnect", HandleHCWebSocketDisconnect },
    { "HCWebSocketDuplicateHandle", HandleHCWebSocketDuplicateHandle },
    { "HCWebSocketGetBinaryMessageFragmentEventFunction", HandleHCWebSocketGetBinaryMessageFragmentEventFunction },
    { "HCWebSocketGetEventFunctions", HandleHCWebSocketGetEventFunctions },
    { "HCWebSocketGetHeader", HandleHCWebSocketGetHeader },
    { "HCWebSocketGetHeaderAtIndex", HandleHCWebSocketGetHeaderAtIndex },
    { "HCWebSocketGetNumHeaders", HandleHCWebSocketGetNumHeaders },
    { "HCWebSocketGetPingInterval", HandleHCWebSocketGetPingInterval },
    { "HCWebSocketGetProxyUri", HandleHCWebSocketGetProxyUri },
    { "HCWebSocketSendBinaryMessageAsync", HandleHCWebSocketSendBinaryMessageAsync },
    { "HCWebSocketSendMessageAsync", HandleHCWebSocketSendMessageAsync },
    { "HCWebSocketSetBinaryMessageFragmentEventFunction", HandleHCWebSocketSetBinaryMessageFragmentEventFunction },
    { "HCWebSocketSetHeader", HandleHCWebSocketSetHeader },
    { "HCWebSocketSetMaxReceiveBufferSize", HandleHCWebSocketSetMaxReceiveBufferSize },
    { "HCWebSocketSetPingInterval", HandleHCWebSocketSetPingInterval },
    { "HCWebSocketSetProxyDecryptsHttps", HandleHCWebSocketSetProxyDecryptsHttps },
    { "HCWebSocketSetProxyUri", HandleHCWebSocketSetProxyUri }
});

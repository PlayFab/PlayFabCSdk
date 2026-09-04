#include "pch.h"

#include "XblHttpCallHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

static XblHttpCallHandle s_xblHttpCall = nullptr;

// ============================================================================
// Create / Lifecycle
// ============================================================================

CommandResultPayload HandleXblHttpCallCreate(
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

            std::string method;
            std::string url;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "method", method, error))
            {
                method = "GET";
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "url", url, error))
            {
                url = "https://profile.xboxlive.com";
            }

            XblHttpCallHandle call = nullptr;
            const HRESULT hr = XblHttpCallCreate(state->xblContext, method.c_str(), url.c_str(), &call);
            LogToWindowFormat("XblHttpCallCreate (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            s_xblHttpCall = call;
            return S_OK;
        });
}

CommandResultPayload HandleXblHttpCallPerformAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCall(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_xblHttpCall);

            int64_t responseBodyType = 0;
            std::string error;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "responseBodyType", responseBodyType, error);

            const HRESULT hr = XblHttpCallPerformAsync(
                s_xblHttpCall,
                static_cast<XblHttpCallResponseBodyType>(responseBodyType),
                &async);
            LogToWindowFormat("XblHttpCallPerformAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXblHttpCallDuplicateHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_xblHttpCall);
            XblHttpCallHandle duplicatedHandle = nullptr;
            const HRESULT hr = XblHttpCallDuplicateHandle(s_xblHttpCall, &duplicatedHandle);
            LogToWindowFormat("XblHttpCallDuplicateHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            s_xblHttpCall = duplicatedHandle;
            return S_OK;
        });
}

CommandResultPayload HandleXblHttpCallCloseHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_xblHttpCall);
            XblHttpCallCloseHandle(s_xblHttpCall);
            s_xblHttpCall = nullptr;
            LogToWindowFormat("XblHttpCallCloseHandle");
            return S_OK;
        });
}

// ============================================================================
// Request Configuration
// ============================================================================

CommandResultPayload HandleXblHttpCallSetTracing(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_xblHttpCall);

            bool traceCall = true;
            std::string error;
            if (!CommandHandlerShared::TryParseBoolParameter(parameters, "traceCall", traceCall, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblHttpCallSetTracing(s_xblHttpCall, traceCall);
            LogToWindowFormat("XblHttpCallSetTracing (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleXblHttpCallGetRequestUrl(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_xblHttpCall);

            const char* url = nullptr;
            const HRESULT hr = XblHttpCallGetRequestUrl(s_xblHttpCall, &url);
            LogToWindowFormat("XblHttpCallGetRequestUrl (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["url"] = url ? url : "";
            return S_OK;
        });
}

CommandResultPayload HandleXblHttpCallRequestSetRequestBodyBytes(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_xblHttpCall);

            std::string body;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "body", body, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblHttpCallRequestSetRequestBodyBytes(s_xblHttpCall, reinterpret_cast<const uint8_t*>(body.c_str()), static_cast<uint32_t>(body.size()));
            LogToWindowFormat("XblHttpCallRequestSetRequestBodyBytes (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleXblHttpCallRequestSetRequestBodyString(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_xblHttpCall);

            std::string body;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "body", body, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblHttpCallRequestSetRequestBodyString(s_xblHttpCall, body.c_str());
            LogToWindowFormat("XblHttpCallRequestSetRequestBodyString (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleXblHttpCallRequestSetHeader(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_xblHttpCall);

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

            bool allowTracing = true;
            if (!CommandHandlerShared::TryParseBoolParameter(parameters, "allowTracing", allowTracing, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblHttpCallRequestSetHeader(s_xblHttpCall, headerName.c_str(), headerValue.c_str(), allowTracing);
            LogToWindowFormat("XblHttpCallRequestSetHeader (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleXblHttpCallRequestSetRetryAllowed(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_xblHttpCall);

            bool retryAllowed = true;
            std::string error;
            if (!CommandHandlerShared::TryParseBoolParameter(parameters, "retryAllowed", retryAllowed, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblHttpCallRequestSetRetryAllowed(s_xblHttpCall, retryAllowed);
            LogToWindowFormat("XblHttpCallRequestSetRetryAllowed (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleXblHttpCallRequestSetRetryCacheId(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_xblHttpCall);

            int64_t retryAfterCacheId = 0;
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "retryAfterCacheId", retryAfterCacheId, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblHttpCallRequestSetRetryCacheId(s_xblHttpCall, static_cast<uint32_t>(retryAfterCacheId));
            LogToWindowFormat("XblHttpCallRequestSetRetryCacheId (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleXblHttpCallRequestSetLongHttpCall(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_xblHttpCall);

            bool longHttpCall = true;
            std::string error;
            if (!CommandHandlerShared::TryParseBoolParameter(parameters, "longHttpCall", longHttpCall, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = XblHttpCallRequestSetLongHttpCall(s_xblHttpCall, longHttpCall);
            LogToWindowFormat("XblHttpCallRequestSetLongHttpCall (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

// ============================================================================
// Response APIs
// ============================================================================

CommandResultPayload HandleXblHttpCallGetResponseString(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_xblHttpCall);

            const char* responseString = nullptr;
            const HRESULT hr = XblHttpCallGetResponseString(s_xblHttpCall, &responseString);
            LogToWindowFormat("XblHttpCallGetResponseString (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["responseString"] = responseString ? responseString : "";
            return S_OK;
        });
}

CommandResultPayload HandleXblHttpCallGetResponseBodyBytesSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_xblHttpCall);

            size_t bufferSize = 0;
            const HRESULT hr = XblHttpCallGetResponseBodyBytesSize(s_xblHttpCall, &bufferSize);
            LogToWindowFormat("XblHttpCallGetResponseBodyBytesSize (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["bufferSize"] = static_cast<uint64_t>(bufferSize);
            return S_OK;
        });
}

CommandResultPayload HandleXblHttpCallGetResponseBodyBytes(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_xblHttpCall);

            size_t bufferSize = 0;
            HRESULT hr = XblHttpCallGetResponseBodyBytesSize(s_xblHttpCall, &bufferSize);
            LogToWindowFormat("XblHttpCallGetResponseBodyBytesSize (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);

            std::vector<uint8_t> buffer(bufferSize);
            size_t bufferUsed = 0;
            hr = XblHttpCallGetResponseBodyBytes(s_xblHttpCall, bufferSize, buffer.data(), &bufferUsed);
            LogToWindowFormat("XblHttpCallGetResponseBodyBytes (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);

            payload.result["bufferSize"] = static_cast<uint64_t>(bufferUsed);
            return S_OK;
        });
}

CommandResultPayload HandleXblHttpCallGetStatusCode(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_xblHttpCall);

            uint32_t statusCode = 0;
            const HRESULT hr = XblHttpCallGetStatusCode(s_xblHttpCall, &statusCode);
            LogToWindowFormat("XblHttpCallGetStatusCode (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["statusCode"] = statusCode;
            return S_OK;
        });
}

CommandResultPayload HandleXblHttpCallGetNetworkErrorCode(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_xblHttpCall);

            HRESULT networkErrorCode = S_OK;
            uint32_t platformNetworkErrorCode = 0;
            const HRESULT hr = XblHttpCallGetNetworkErrorCode(s_xblHttpCall, &networkErrorCode, &platformNetworkErrorCode);
            LogToWindowFormat("XblHttpCallGetNetworkErrorCode (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["networkErrorCode"] = static_cast<int32_t>(networkErrorCode);
            payload.result["platformNetworkErrorCode"] = platformNetworkErrorCode;
            return S_OK;
        });
}

CommandResultPayload HandleXblHttpCallGetPlatformNetworkErrorMessage(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_xblHttpCall);

            const char* platformNetworkErrorMessage = nullptr;
            const HRESULT hr = XblHttpCallGetPlatformNetworkErrorMessage(s_xblHttpCall, &platformNetworkErrorMessage);
            LogToWindowFormat("XblHttpCallGetPlatformNetworkErrorMessage (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["platformNetworkErrorMessage"] = platformNetworkErrorMessage ? platformNetworkErrorMessage : "";
            return S_OK;
        });
}

// ============================================================================
// Response Header APIs
// ============================================================================

CommandResultPayload HandleXblHttpCallGetHeader(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_xblHttpCall);

            std::string headerName;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "headerName", headerName, error))
            {
                return E_INVALIDARG;
            }

            const char* headerValue = nullptr;
            const HRESULT hr = XblHttpCallGetHeader(s_xblHttpCall, headerName.c_str(), &headerValue);
            LogToWindowFormat("XblHttpCallGetHeader (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["headerValue"] = headerValue ? headerValue : "";
            return S_OK;
        });
}

CommandResultPayload HandleXblHttpCallGetNumHeaders(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_xblHttpCall);

            uint32_t numHeaders = 0;
            const HRESULT hr = XblHttpCallGetNumHeaders(s_xblHttpCall, &numHeaders);
            LogToWindowFormat("XblHttpCallGetNumHeaders (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["numHeaders"] = numHeaders;
            return S_OK;
        });
}

CommandResultPayload HandleXblHttpCallGetHeaderAtIndex(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !s_xblHttpCall);

            int64_t headerIndex = 0;
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "headerIndex", headerIndex, error))
            {
                return E_INVALIDARG;
            }

            const char* headerName = nullptr;
            const char* headerValue = nullptr;
            const HRESULT hr = XblHttpCallGetHeaderAtIndex(s_xblHttpCall, static_cast<uint32_t>(headerIndex), &headerName, &headerValue);
            LogToWindowFormat("XblHttpCallGetHeaderAtIndex (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["headerName"] = headerName ? headerName : "";
            payload.result["headerValue"] = headerValue ? headerValue : "";
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XblHttpCallCloseHandle", HandleXblHttpCallCloseHandle },
    { "XblHttpCallCreate", HandleXblHttpCallCreate },
    { "XblHttpCallDuplicateHandle", HandleXblHttpCallDuplicateHandle },
    { "XblHttpCallGetHeader", HandleXblHttpCallGetHeader },
    { "XblHttpCallGetHeaderAtIndex", HandleXblHttpCallGetHeaderAtIndex },
    { "XblHttpCallGetNetworkErrorCode", HandleXblHttpCallGetNetworkErrorCode },
    { "XblHttpCallGetNumHeaders", HandleXblHttpCallGetNumHeaders },
    { "XblHttpCallGetPlatformNetworkErrorMessage", HandleXblHttpCallGetPlatformNetworkErrorMessage },
    { "XblHttpCallGetRequestUrl", HandleXblHttpCallGetRequestUrl },
    { "XblHttpCallGetResponseBodyBytes", HandleXblHttpCallGetResponseBodyBytes },
    { "XblHttpCallGetResponseBodyBytesSize", HandleXblHttpCallGetResponseBodyBytesSize },
    { "XblHttpCallGetResponseString", HandleXblHttpCallGetResponseString },
    { "XblHttpCallGetStatusCode", HandleXblHttpCallGetStatusCode },
    { "XblHttpCallPerformAsync", HandleXblHttpCallPerformAsync },
    { "XblHttpCallRequestSetHeader", HandleXblHttpCallRequestSetHeader },
    { "XblHttpCallRequestSetLongHttpCall", HandleXblHttpCallRequestSetLongHttpCall },
    { "XblHttpCallRequestSetRequestBodyBytes", HandleXblHttpCallRequestSetRequestBodyBytes },
    { "XblHttpCallRequestSetRequestBodyString", HandleXblHttpCallRequestSetRequestBodyString },
    { "XblHttpCallRequestSetRetryAllowed", HandleXblHttpCallRequestSetRetryAllowed },
    { "XblHttpCallRequestSetRetryCacheId", HandleXblHttpCallRequestSetRetryCacheId },
    { "XblHttpCallSetTracing", HandleXblHttpCallSetTracing }
});

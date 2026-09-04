#include "pch.h"

#include "HCMockHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"

#include <httpClient/mock.h>
#include "CommandRegistry.h"

namespace
{
    void CALLBACK MockMatchedCallback(
        _In_ HCMockCallHandle call,
        _In_ const char* method,
        _In_ const char* url,
        _In_ const uint8_t* requestBodyBytes,
        _In_ uint32_t requestBodySize,
        _In_ void* context) noexcept
    {
        UNREFERENCED_PARAMETER(call);
        UNREFERENCED_PARAMETER(requestBodyBytes);
        UNREFERENCED_PARAMETER(requestBodySize);
        UNREFERENCED_PARAMETER(context);
        LogToWindow("HCMockMatchedCallback: " + std::string(method ? method : "") + " " + std::string(url ? url : ""));
    }
}

CommandResultPayload HandleHCMockCallCreate(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HCMockCallHandle call = nullptr;
            const HRESULT hr = HCMockCallCreate(&call);
            LogToWindowFormat("HCMockCallCreate (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            state->hcMockCall = call;
            return S_OK;
        });
}

CommandResultPayload HandleHCMockCallDuplicateHandle(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HCMockCallHandle duplicatedHandle = HCMockCallDuplicateHandle(state->hcMockCall);
            LogToWindowFormat("HCMockCallDuplicateHandle (handle=%p)", duplicatedHandle);
            state->hcMockCall = duplicatedHandle;
            return S_OK;
        });
}

CommandResultPayload HandleHCMockCallCloseHandle(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HCMockCallCloseHandle(state->hcMockCall);
            LogToWindow("HCMockCallCloseHandle");
            state->hcMockCall = nullptr;
            return S_OK;
        });
}

CommandResultPayload HandleHCMockAddMock(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const char* method = nullptr;
            const char* url = nullptr;
            std::string methodStr;
            std::string urlStr;
            std::string error;

            if (CommandHandlerShared::TryGetStringParameter(parameters, "method", methodStr, error))
            {
                method = methodStr.c_str();
            }
            if (CommandHandlerShared::TryGetStringParameter(parameters, "url", urlStr, error))
            {
                url = urlStr.c_str();
            }

            const HRESULT hr = HCMockAddMock(state->hcMockCall, method, url, nullptr, 0);
            LogToWindowFormat("HCMockAddMock (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCMockRemoveMock(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = HCMockRemoveMock(state->hcMockCall);
            LogToWindowFormat("HCMockRemoveMock (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCMockClearMocks(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = HCMockClearMocks();
            LogToWindowFormat("HCMockClearMocks (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCMockSetMockMatchedCallback(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = HCMockSetMockMatchedCallback(state->hcMockCall, MockMatchedCallback, nullptr);
            LogToWindowFormat("HCMockSetMockMatchedCallback (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCMockResponseSetResponseBodyBytes(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string body;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "body", body, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCMockResponseSetResponseBodyBytes(state->hcMockCall, (uint8_t*)body.c_str(), (uint32_t)body.size());
            LogToWindowFormat("HCMockResponseSetResponseBodyBytes (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCMockResponseSetStatusCode(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            int64_t statusCode = 0;
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "statusCode", statusCode, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCMockResponseSetStatusCode(state->hcMockCall, (uint32_t)statusCode);
            LogToWindowFormat("HCMockResponseSetStatusCode (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCMockResponseSetNetworkErrorCode(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            int64_t networkErrorCode = 0;
            int64_t platformNetworkErrorCode = 0;
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "networkErrorCode", networkErrorCode, error))
            {
                return E_INVALIDARG;
            }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "platformNetworkErrorCode", platformNetworkErrorCode, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCMockResponseSetNetworkErrorCode(state->hcMockCall, (HRESULT)networkErrorCode, (uint32_t)platformNetworkErrorCode);
            LogToWindowFormat("HCMockResponseSetNetworkErrorCode (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCMockResponseSetHeader(
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

            const HRESULT hr = HCMockResponseSetHeader(state->hcMockCall, headerName.c_str(), headerValue.c_str());
            LogToWindowFormat("HCMockResponseSetHeader (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "HCMockAddMock", HandleHCMockAddMock },
    { "HCMockCallCloseHandle", HandleHCMockCallCloseHandle },
    { "HCMockCallCreate", HandleHCMockCallCreate },
    { "HCMockCallDuplicateHandle", HandleHCMockCallDuplicateHandle },
    { "HCMockClearMocks", HandleHCMockClearMocks },
    { "HCMockRemoveMock", HandleHCMockRemoveMock },
    { "HCMockResponseSetHeader", HandleHCMockResponseSetHeader },
    { "HCMockResponseSetNetworkErrorCode", HandleHCMockResponseSetNetworkErrorCode },
    { "HCMockResponseSetResponseBodyBytes", HandleHCMockResponseSetResponseBodyBytes },
    { "HCMockResponseSetStatusCode", HandleHCMockResponseSetStatusCode },
    { "HCMockSetMockMatchedCallback", HandleHCMockSetMockMatchedCallback }
});

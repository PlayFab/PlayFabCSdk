#include "pch.h"

#include "XUserHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"

#include <chrono>
#include <string>

CommandResultPayload HandleXUserAddAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);

    const auto start = std::chrono::steady_clock::now();

    // Helper lambda to attempt XUserAddAsync with given options
    auto tryAddUser = [state](XUserAddOptions options, HRESULT& outHr, HRESULT& outWaitHr, HRESULT& outResultHr) -> bool
    {
        XAsyncBlock async{};
        outHr = XUserAddAsync(options, &async);
        outWaitHr = S_OK;
        outResultHr = S_OK;

        if (FAILED(outHr))
        {
            return false;
        }

        outWaitHr = XAsyncGetStatus(&async, true);
        if (FAILED(outWaitHr))
        {
            return false;
        }

        if (state->xuser)
        {
            XUserCloseHandle(state->xuser);
            state->xuser = nullptr;
        }

        outResultHr = XUserAddResult(&async, &state->xuser);
        return SUCCEEDED(outResultHr);
    };

    HRESULT hr = S_OK;
    HRESULT waitHr = S_OK;
    HRESULT resultHr = S_OK;

    // First, try silent login
    LogToWindow("XUserAddAsync: Trying AddDefaultUserSilently...");
    bool success = tryAddUser(XUserAddOptions::AddDefaultUserSilently, hr, waitHr, resultHr);

    if (!success)
    {
        // Silent login failed, try with UI
        LogToWindowFormat("XUserAddAsync: Silent login failed (hr=0x%08X, waitHr=0x%08X, resultHr=0x%08X), trying with UI...",
            static_cast<uint32_t>(hr), static_cast<uint32_t>(waitHr), static_cast<uint32_t>(resultHr));
        success = tryAddUser(XUserAddOptions::AddDefaultUserAllowingUI, hr, waitHr, resultHr);
    }

    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);
    LogToWindowFormat("XUserAddAsync (hr=0x%08X)", static_cast<uint32_t>(hr));

    if (FAILED(hr))
    {
        CommandHandlerShared::SetHResult(payload.result, hr);
        CommandHandlerShared::MarkFailure(payload.result, hr, "XUserAddAsync failed");
        return payload;
    }

    if (FAILED(waitHr))
    {
        CommandHandlerShared::SetHResult(payload.result, waitHr);
        CommandHandlerShared::MarkFailure(payload.result, waitHr, "XUserAddAsync wait failed");
        return payload;
    }

    if (FAILED(resultHr))
    {
        CommandHandlerShared::SetHResult(payload.result, resultHr);
        CommandHandlerShared::MarkFailure(payload.result, resultHr, "XUserAddResult failed");
        return payload;
    }

    CommandHandlerShared::MarkSuccess(payload.result);
    CommandHandlerShared::SetHResult(payload.result, S_OK);
    return payload;
}

CommandResultPayload HandleXUserCloseHandle(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);

    const auto start = std::chrono::steady_clock::now();
    if (state->xuser)
    {
        XUserCloseHandle(state->xuser);
        state->xuser = nullptr;
        LogToWindow("XUserCloseHandle executed");
    }
    else
    {
        LogToWindow("XUserCloseHandle skipped (no handle)");
    }

    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);
    CommandHandlerShared::MarkSuccess(payload.result);
    CommandHandlerShared::SetHResult(payload.result, S_OK);
    return payload;
}

// Static callbacks for XUserPlatformRemoteConnect events
static void OnRemoteConnectShow(
    _In_opt_ void* context,
    _In_ uint32_t userIdentifier,
    _In_ XUserPlatformOperation operation,
    _In_z_ char const* url,
    _In_z_ char const* code,
    _In_ size_t qrCodeSize,
    _In_reads_bytes_(qrCodeSize) void const* qrCode
)
{
    (void)context;
    (void)operation;
    (void)qrCodeSize;
    (void)qrCode;
    
    LogToWindowFormat("RemoteConnectShow: userIdentifier=%u, url=%s, code=%s", 
        userIdentifier, 
        url ? url : "null", 
        code ? code : "null");
    if (url && code)
    {
        char buffer[512];
        OutputDebugStringA("=====================\n");
        sprintf_s(buffer, "URL: %s\n", url);
        OutputDebugStringA(buffer);
        sprintf_s(buffer, "Code: %s\n", code);
        OutputDebugStringA(buffer);
        OutputDebugStringA("=====================\n");
    }
}

static void OnRemoteConnectClose(
    _In_opt_ void* context,
    _In_ uint32_t userIdentifier,
    _In_ XUserPlatformOperation operation
)
{
    (void)context;
    (void)operation;
    
    LogToWindowFormat("RemoteConnectClose: userIdentifier=%u", userIdentifier);
}

// Static callback for XUserPlatformSpopPrompt events
static void OnSpopPrompt(
    _In_opt_ void* context,
    _In_ uint32_t userIdentifier,
    _In_ XUserPlatformOperation operation,
    _In_z_ char const* modernGamertag,
    _In_opt_z_ char const* modernGamertagSuffix
)
{
    auto* state = static_cast<DeviceGameSaveState*>(context);
    
    LogToWindowFormat("SpopPrompt: userIdentifier=%u, gamertag=%s%s", 
        userIdentifier,
        modernGamertag ? modernGamertag : "null",
        modernGamertagSuffix ? modernGamertagSuffix : "");
    
    // Store the operation so we can complete it later
    if (state)
    {
        state->pendingSpopOperation = operation;
    }
}

CommandResultPayload HandleXUserPlatformRemoteConnectSetEventHandlers(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);

    XUserPlatformRemoteConnectEventHandlers remoteConnect{};
    remoteConnect.context = state;
    remoteConnect.show = &OnRemoteConnectShow;
    remoteConnect.close = &OnRemoteConnectClose;

    const auto start = std::chrono::steady_clock::now();
    const HRESULT hr = XUserPlatformRemoteConnectSetEventHandlers(nullptr, &remoteConnect);
    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);

    LogToWindowFormat("XUserPlatformRemoteConnectSetEventHandlers (hr=0x%08X)", static_cast<uint32_t>(hr));
    CommandHandlerShared::SetHResult(payload.result, hr);

    if (FAILED(hr))
    {
        CommandHandlerShared::MarkFailure(payload.result, hr, "XUserPlatformRemoteConnectSetEventHandlers failed");
        return payload;
    }

    CommandHandlerShared::MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandleXUserPlatformSpopPromptSetEventHandlers(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);

    const auto start = std::chrono::steady_clock::now();
    const HRESULT hr = XUserPlatformSpopPromptSetEventHandlers(state->taskQueue, &OnSpopPrompt, state);
    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);

    LogToWindowFormat("XUserPlatformSpopPromptSetEventHandlers (hr=0x%08X)", static_cast<uint32_t>(hr));
    CommandHandlerShared::SetHResult(payload.result, hr);

    if (FAILED(hr))
    {
        CommandHandlerShared::MarkFailure(payload.result, hr, "XUserPlatformSpopPromptSetEventHandlers failed");
        return payload;
    }

    CommandHandlerShared::MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandleXUserPlatformSpopPromptComplete(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);

    if (!state->pendingSpopOperation)
    {
        CommandHandlerShared::MarkFailure(payload.result, E_POINTER, "No pending SPOP operation");
        return payload;
    }

    // Get the result parameter (default to SignInHere)
    XUserPlatformSpopOperationResult result = XUserPlatformSpopOperationResult::SignInHere;
    std::string resultStr;
    std::string error;
    if (CommandHandlerShared::TryGetStringParameter(parameters, "result", resultStr, error))
    {
        std::string lowerResult = CommandHandlerShared::ToLowerCopy(resultStr);
        if (lowerResult == "signinhere")
        {
            result = XUserPlatformSpopOperationResult::SignInHere;
        }
        else if (lowerResult == "switchaccount")
        {
            result = XUserPlatformSpopOperationResult::SwitchAccount;
        }
        else if (lowerResult == "canceled" || lowerResult == "cancel")
        {
            result = XUserPlatformSpopOperationResult::Canceled;
        }
    }

    XUserPlatformOperation op = state->pendingSpopOperation;
    state->pendingSpopOperation = nullptr;

    const auto start = std::chrono::steady_clock::now();
    const HRESULT hr = XUserPlatformSpopPromptComplete(op, result);
    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);

    LogToWindowFormat("XUserPlatformSpopPromptComplete (result=%s, hr=0x%08X)", resultStr.empty() ? "SignInHere" : resultStr.c_str(), static_cast<uint32_t>(hr));
    CommandHandlerShared::SetHResult(payload.result, hr);
    CommandHandlerShared::MarkSuccess(payload.result);
    CommandHandlerShared::SetHResult(payload.result, S_OK);
    return payload;
}

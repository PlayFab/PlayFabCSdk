#include "pch.h"

#include "XUserHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"

#include <chrono>
#include <string>
#include <thread>
#include <vector>
#include "CommandRegistry.h"

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

    // Parse userIndex parameter (default 0) — determines which XUser slot to populate
    int userIndex = ParseIndexParam(parameters, "userIndex");
    if (userIndex < 0 || userIndex >= DeviceGameSaveState::kMaxUsers)
    {
        payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);
        CommandHandlerShared::MarkFailure(payload.result, E_INVALIDARG, "userIndex out of range (0-3)");
        return payload;
    }

    // Debug: log received parameters so we can diagnose serialization issues
    LogToWindowFormat("XUserAddAsync: parameters type=%d dump=%s userIndex=%d",
        static_cast<int>(parameters.type()), parameters.dump().c_str(), userIndex);

    // Parse optional allowUi parameter (default false).
    // When true and silent sign-in fails, automatically retries with UI (account picker).
    // The UI path pumps Win32 messages so the sign-in dialog can render.
    bool allowUi = false;
    if (parameters.is_object() && parameters.contains("allowUi"))
    {
        const auto& node = parameters["allowUi"];
        if (node.is_boolean())
        {
            allowUi = node.get<bool>();
        }
        else if (node.is_string())
        {
            auto s = node.get<std::string>();
            allowUi = (s == "true" || s == "1");
        }
    }

    // Get reference to the target XUser slot
    XUserHandle& targetSlot = GetXUserHandle(state, userIndex);

    // Helper lambda: attempt XUserAddAsync with the given options.
    // Matches the shipping sample (PlayFabGameSaveSample-XboxConsole): issue the async and
    // block on XAsyncGetStatus(&async, true). A blocking wait lets the GDK drive completion
    // delivery on the calling thread regardless of the task queue's dispatch mode — a
    // wait=false poll on a ThreadPool completion queue does NOT reliably observe the result
    // and intermittently times out at 5s (E_ABORT) even with a user signed in. Silent
    // options never show UI, so the blocking wait cannot hang on user input; for the UI
    // option the controller's per-command timeout bounds the wait.
    auto tryAddUser = [state, &targetSlot](XUserAddOptions options, HRESULT& outHr, HRESULT& outWaitHr, HRESULT& outResultHr) -> bool
    {
        XAsyncBlock async{};
        async.queue = state->taskQueue;
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

        if (targetSlot)
        {
            XUserCloseHandle(targetSlot);
            targetSlot = nullptr;
        }

        outResultHr = XUserAddResult(&async, &targetSlot);
        return SUCCEEDED(outResultHr);
    };

    HRESULT hr = S_OK;
    HRESULT waitHr = S_OK;
    HRESULT resultHr = S_OK;

    constexpr long hrNoPackageIdentity = static_cast<long>(0x89245110); // E_GAMEUSER_NO_PACKAGE_IDENTITY
    constexpr long hrNoDefaultUser = static_cast<long>(0x89245106);     // E_GAMEUSER_NO_DEFAULT_USER

    // First, try silent login. Blocking wait (matches the sample) reliably resolves a
    // signed-in default user; silent never shows UI so it cannot hang.
    LogToWindow("XUserAddAsync: Trying AddDefaultUserSilently...");
    bool success = tryAddUser(XUserAddOptions::AddDefaultUserSilently, hr, waitHr, resultHr);

    // Check for E_GAMEUSER_NO_PACKAGE_IDENTITY on any of the three HRESULTs
    if (!success && (hr == hrNoPackageIdentity || waitHr == hrNoPackageIdentity || resultHr == hrNoPackageIdentity))
    {
        LogToWindow("XUserAddAsync: ERROR: E_GAMEUSER_NO_PACKAGE_IDENTITY (0x89245110). "
            "This app must be built and run as a packaged app (MSIXVC/loose layout with package identity) for XUser APIs to work. "
            "Build the GDK project with packaging enabled or use 'wdapp register' to register a loose layout.");
        payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);
        CommandHandlerShared::SetHResult(payload.result, hrNoPackageIdentity);
        CommandHandlerShared::MarkFailure(payload.result, hrNoPackageIdentity,
            "XUserAddAsync failed: E_GAMEUSER_NO_PACKAGE_IDENTITY - app must be built with package identity for XUser APIs");
        return payload;
    }

    // E_GAMEUSER_NO_DEFAULT_USER: no user signed in silently.
    // On Xbox, the XboxDialogDetector can handle the sign-in dialog, so auto-retry with UI.
    // On PC-GRTS (headless), retrying with UI causes an indefinite hang because no human
    // or dialog detector can interact with the Windows Xbox sign-in popup. Fail fast instead.
    if (!success && (hr == hrNoDefaultUser || waitHr == hrNoDefaultUser || resultHr == hrNoDefaultUser))
    {
        if (state->engineType == DeviceEngineType::Xbox)
        {
            LogToWindowFormat("XUserAddAsync: E_GAMEUSER_NO_DEFAULT_USER (0x89245106) — no default user, retrying with UI...");
            success = tryAddUser(XUserAddOptions::AddDefaultUserAllowingUI, hr, waitHr, resultHr);
        }
        else
        {
            LogToWindow("XUserAddAsync: E_GAMEUSER_NO_DEFAULT_USER (0x89245106) — no default user. "
                "Not retrying with UI on non-Xbox device (would hang in headless mode). "
                "Ensure Xbox Live is signed in via the Xbox app before running pc-grts tests.");
        }
    }

    // NOTE: Previous behavior auto-escalated to UI on silent failure. That caused
    // unattended test runs on Xbox to display the "Who are you?" account picker and
    // hang for 180s after any prior test left the console without a single canonical
    // default user (e.g., tests 70/104/105/106 cleanup left two signed-in accounts).
    // UI fallback is now opt-in via the YAML parameter `allowUi: true`. Scenarios
    // that don't opt in fail fast in ~5s with no blocking UI prompt.
    if (!success && allowUi)
    {
        // Silent login failed, retry with UI (account picker)
        LogToWindowFormat("XUserAddAsync: Silent login failed (hr=0x%08X, waitHr=0x%08X, resultHr=0x%08X), trying with UI...",
            static_cast<uint32_t>(hr), static_cast<uint32_t>(waitHr), static_cast<uint32_t>(resultHr));
        success = tryAddUser(XUserAddOptions::AddDefaultUserAllowingUI, hr, waitHr, resultHr);
    }
    else if (!success)
    {
        LogToWindowFormat("XUserAddAsync: Silent login failed (hr=0x%08X, waitHr=0x%08X, resultHr=0x%08X). Pass allowUi:true to retry with sign-in UI.",
            static_cast<uint32_t>(hr), static_cast<uint32_t>(waitHr), static_cast<uint32_t>(resultHr));
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
    int userIndex = ParseIndexParam(parameters, "userIndex");
    XUserHandle& targetSlot = GetXUserHandle(state, (userIndex >= 0 && userIndex < DeviceGameSaveState::kMaxUsers) ? userIndex : 0);

    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (targetSlot)
            {
                XUserCloseHandle(targetSlot);
                targetSlot = nullptr;
                LogToWindowFormat("XUserCloseHandle executed (userIndex=%d)", userIndex);
            }
            else
            {
                LogToWindowFormat("XUserCloseHandle skipped (no handle at userIndex=%d)", userIndex);
            }
            return S_OK;
        });
}

// Helper to send a device log message to the controller via WebSocket.
// The controller recognizes {"type":"deviceLog","message":"..."} and displays it in the CLI.
static void SendDeviceLog(DeviceGameSaveState* state, const char* format, ...)
{
    if (!state || !state->websocketClient.IsConnected())
    {
        return;
    }

    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    nlohmann::json logJson;
    logJson["type"] = "deviceLog";
    logJson["message"] = buffer;
    std::string payload = logJson.dump();
    HRESULT hr = state->websocketClient.SendText(payload);
    if (FAILED(hr))
    {
        LogToWindowFormat("Failed to send deviceLog to controller (hr=0x%08X)", static_cast<uint32_t>(hr));
    }
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

    auto* state = static_cast<DeviceGameSaveState*>(context);
    SendDeviceLog(state, "Remote Connect: Open %s and enter code: %s", url ? url : "", code ? code : "");
}

static void OnRemoteConnectClose(
    _In_opt_ void* context,
    _In_ uint32_t userIdentifier,
    _In_ XUserPlatformOperation operation
)
{
    (void)operation;
    
    LogToWindowFormat("RemoteConnectClose: userIdentifier=%u", userIdentifier);

    auto* state = static_cast<DeviceGameSaveState*>(context);
    SendDeviceLog(state, "Remote Connect: Authentication completed");
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

    SendDeviceLog(state, "SPOP Prompt: gamertag=%s%s",
        modernGamertag ? modernGamertag : "",
        modernGamertagSuffix ? modernGamertagSuffix : "");
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
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->pendingSpopOperation);

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

            const HRESULT hr = XUserPlatformSpopPromptComplete(op, result);
            LogToWindowFormat("XUserPlatformSpopPromptComplete (result=%s, hr=0x%08X)", resultStr.empty() ? "SignInHere" : resultStr.c_str(), static_cast<uint32_t>(hr));
            return S_OK;
        });
}

CommandResultPayload HandleXUserDuplicateHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            XUserHandle duplicated = nullptr;
            HRESULT hr = XUserDuplicateHandle(state->xuser, &duplicated);
            LogToWindowFormat("XUserDuplicateHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr) && duplicated)
            {
                payload.result["duplicated"] = true;
                XUserCloseHandle(duplicated);
            }
            return hr;
        });
}

CommandResultPayload HandleXUserCompare(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            int32_t compareResult = XUserCompare(state->xuser, state->xuser);
            LogToWindowFormat("XUserCompare result=%d", compareResult);
            payload.result["compareResult"] = compareResult;
            return S_OK;
        });
}

CommandResultPayload HandleXUserGetMaxUsers(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t maxUsers = 0;
            HRESULT hr = XUserGetMaxUsers(&maxUsers);
            LogToWindowFormat("XUserGetMaxUsers (hr=0x%08X, maxUsers=%u)", static_cast<uint32_t>(hr), maxUsers);
            if (SUCCEEDED(hr))
            {
                payload.result["maxUsers"] = maxUsers;
            }
            return hr;
        });
}

CommandResultPayload HandleXUserAddResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XUserAddResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXUserAddByIdWithUiAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            uint64_t userId = 0;
            HRESULT idHr = XUserGetId(state->xuser, &userId);
            RETURN_HR_IF(E_FAIL, FAILED(idHr));

            HRESULT hr = XUserAddByIdWithUiAsync(userId, &async);
            LogToWindowFormat("XUserAddByIdWithUiAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [state](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            XUserHandle newUser = nullptr;
            HRESULT hr = XUserAddByIdWithUiResult(&async, &newUser);
            LogToWindowFormat("XUserAddByIdWithUiResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr) && newUser)
            {
                if (state->xuser)
                {
                    XUserCloseHandle(state->xuser);
                }
                state->xuser = newUser;
                payload.result["userAdded"] = true;
            }
            return hr;
        });
}

CommandResultPayload HandleXUserAddByIdWithUiResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XUserAddByIdWithUiResult: called inline by Async handler");
            return S_OK;
        });
}

#if _GRDK_VER >= 0x65F41E8E /* GDK Edition 260400 */
CommandResultPayload HandleXUserIsSignOutPresent(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            bool isPresent = XUserIsSignOutPresent();
            LogToWindowFormat("XUserIsSignOutPresent: %s", isPresent ? "true" : "false");
            payload.result["isSignOutPresent"] = isPresent;
            return S_OK;
        });
}

CommandResultPayload HandleXUserSignOutAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            HRESULT hr = XUserSignOutAsync(state->xuser, &async);
            LogToWindowFormat("XUserSignOutAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XUserSignOutResult(&async);
            LogToWindowFormat("XUserSignOutResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXUserSignOutResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XUserSignOutResult: called inline by Async handler");
            return S_OK;
        });
}
#endif

CommandResultPayload HandleXUserFindUserById(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            uint64_t userId = 0;
            HRESULT idHr = XUserGetId(state->xuser, &userId);
            RETURN_HR_IF(E_FAIL, FAILED(idHr));

            XUserHandle foundUser = nullptr;
            HRESULT hr = XUserFindUserById(userId, &foundUser);
            LogToWindowFormat("XUserFindUserById (hr=0x%08X, userId=%llu)", static_cast<uint32_t>(hr), static_cast<unsigned long long>(userId));
            if (SUCCEEDED(hr) && foundUser)
            {
                payload.result["found"] = true;
                XUserCloseHandle(foundUser);
            }
            return hr;
        });
}

CommandResultPayload HandleXUserGetLocalId(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            XUserLocalId localId{};
            HRESULT hr = XUserGetLocalId(state->xuser, &localId);
            LogToWindowFormat("XUserGetLocalId (hr=0x%08X, localId=%llu)", static_cast<uint32_t>(hr), static_cast<unsigned long long>(localId.value));
            if (SUCCEEDED(hr))
            {
                payload.result["localId"] = localId.value;
            }
            return hr;
        });
}

CommandResultPayload HandleXUserFindUserByLocalId(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            XUserLocalId localId{};
            HRESULT hr = XUserGetLocalId(state->xuser, &localId);
            RETURN_HR_IF(E_FAIL, FAILED(hr));

            XUserHandle foundUser = nullptr;
            hr = XUserFindUserByLocalId(localId, &foundUser);
            LogToWindowFormat("XUserFindUserByLocalId (hr=0x%08X, localId=%llu)", static_cast<uint32_t>(hr), static_cast<unsigned long long>(localId.value));
            if (SUCCEEDED(hr) && foundUser)
            {
                payload.result["found"] = true;
                XUserCloseHandle(foundUser);
            }
            return hr;
        });
}

CommandResultPayload HandleXUserFindForDevice(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            APP_LOCAL_DEVICE_ID deviceIdValue{};
            XUserHandle foundUser = nullptr;
            HRESULT hr = XUserFindForDevice(&deviceIdValue, &foundUser);
            LogToWindowFormat("XUserFindForDevice (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr) && foundUser)
            {
                payload.result["found"] = true;
                XUserCloseHandle(foundUser);
            }
            return hr;
        });
}

CommandResultPayload HandleXUserGetIsGuest(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            bool isGuest = false;
            HRESULT hr = XUserGetIsGuest(state->xuser, &isGuest);
            LogToWindowFormat("XUserGetIsGuest (hr=0x%08X, isGuest=%s)", static_cast<uint32_t>(hr), isGuest ? "true" : "false");
            if (SUCCEEDED(hr))
            {
                payload.result["isGuest"] = isGuest;
            }
            return hr;
        });
}

CommandResultPayload HandleXUserGetState(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            XUserState userState{};
            HRESULT hr = XUserGetState(state->xuser, &userState);
            LogToWindowFormat("XUserGetState (hr=0x%08X, state=%u)", static_cast<uint32_t>(hr), static_cast<uint32_t>(userState));
            if (SUCCEEDED(hr))
            {
                payload.result["userState"] = static_cast<uint32_t>(userState);
            }
            return hr;
        });
}

CommandResultPayload HandleXUserGetGamertag(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            char gamertag[XUserGamertagComponentUniqueModernMaxBytes]{};
            size_t gamertagUsed = 0;
            HRESULT hr = XUserGetGamertag(state->xuser, XUserGamertagComponent::UniqueModern, sizeof(gamertag), gamertag, &gamertagUsed);
            LogToWindowFormat("XUserGetGamertag (hr=0x%08X, gamertag=%s)", static_cast<uint32_t>(hr), gamertag);
            if (SUCCEEDED(hr))
            {
                payload.result["gamertag"] = gamertag;
            }
            return hr;
        });
}

CommandResultPayload HandleXUserGetGamerPictureAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            HRESULT hr = XUserGetGamerPictureAsync(state->xuser, XUserGamerPictureSize::Small, &async);
            LogToWindowFormat("XUserGetGamerPictureAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            HRESULT hr = XUserGetGamerPictureResultSize(&async, &bufferSize);
            LogToWindowFormat("XUserGetGamerPictureResultSize (hr=0x%08X, size=%zu)", static_cast<uint32_t>(hr), bufferSize);
            if (SUCCEEDED(hr) && bufferSize > 0)
            {
                std::vector<uint8_t> buffer(bufferSize);
                size_t bufferUsed = 0;
                hr = XUserGetGamerPictureResult(&async, bufferSize, buffer.data(), &bufferUsed);
                LogToWindowFormat("XUserGetGamerPictureResult (hr=0x%08X, used=%zu)", static_cast<uint32_t>(hr), bufferUsed);
                if (SUCCEEDED(hr))
                {
                    payload.result["gamerPictureSize"] = bufferUsed;
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXUserGetGamerPictureResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XUserGetGamerPictureResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXUserGetGamerPictureResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XUserGetGamerPictureResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXUserGetAgeGroup(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            XUserAgeGroup ageGroup{};
            HRESULT hr = XUserGetAgeGroup(state->xuser, &ageGroup);
            LogToWindowFormat("XUserGetAgeGroup (hr=0x%08X, ageGroup=%u)", static_cast<uint32_t>(hr), static_cast<uint32_t>(ageGroup));
            if (SUCCEEDED(hr))
            {
                payload.result["ageGroup"] = static_cast<uint32_t>(ageGroup);
            }
            return hr;
        });
}

CommandResultPayload HandleXUserCheckPrivilege(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            bool hasPrivilege = false;
            XUserPrivilegeDenyReason reason{};
            HRESULT hr = XUserCheckPrivilege(state->xuser, XUserPrivilegeOptions::None, XUserPrivilege::Multiplayer, &hasPrivilege, &reason);
            LogToWindowFormat("XUserCheckPrivilege (hr=0x%08X, has=%s, reason=%u)", static_cast<uint32_t>(hr), hasPrivilege ? "true" : "false", static_cast<uint32_t>(reason));
            if (SUCCEEDED(hr))
            {
                payload.result["hasPrivilege"] = hasPrivilege;
                payload.result["denyReason"] = static_cast<uint32_t>(reason);
            }
            return hr;
        });
}

CommandResultPayload HandleXUserResolvePrivilegeWithUiAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            HRESULT hr = XUserResolvePrivilegeWithUiAsync(state->xuser, XUserPrivilegeOptions::None, XUserPrivilege::Multiplayer, &async);
            LogToWindowFormat("XUserResolvePrivilegeWithUiAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XUserResolvePrivilegeWithUiResult(&async);
            LogToWindowFormat("XUserResolvePrivilegeWithUiResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXUserResolvePrivilegeWithUiResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XUserResolvePrivilegeWithUiResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXUserGetTokenAndSignatureAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            HRESULT hr = XUserGetTokenAndSignatureAsync(state->xuser, XUserGetTokenAndSignatureOptions::None, "GET", "https://playfabapi.com", 0, nullptr, 0, nullptr, &async);
            LogToWindowFormat("XUserGetTokenAndSignatureAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            HRESULT hr = XUserGetTokenAndSignatureResultSize(&async, &bufferSize);
            LogToWindowFormat("XUserGetTokenAndSignatureResultSize (hr=0x%08X, size=%zu)", static_cast<uint32_t>(hr), bufferSize);
            if (SUCCEEDED(hr) && bufferSize > 0)
            {
                std::vector<uint8_t> buffer(bufferSize);
                XUserGetTokenAndSignatureData* data = nullptr;
                hr = XUserGetTokenAndSignatureResult(&async, bufferSize, buffer.data(), &data, nullptr);
                LogToWindowFormat("XUserGetTokenAndSignatureResult (hr=0x%08X)", static_cast<uint32_t>(hr));
                if (SUCCEEDED(hr) && data)
                {
                    payload.result["tokenSize"] = data->tokenSize;
                    payload.result["signatureSize"] = data->signatureSize;
                    if (data->token) payload.result["token"] = data->token;
                    if (data->signature) payload.result["signature"] = data->signature;
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXUserGetTokenAndSignatureResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XUserGetTokenAndSignatureResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXUserGetTokenAndSignatureResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XUserGetTokenAndSignatureResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXUserGetTokenAndSignatureUtf16Async(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            HRESULT hr = XUserGetTokenAndSignatureUtf16Async(state->xuser, XUserGetTokenAndSignatureOptions::None, L"GET", L"https://playfabapi.com", 0, nullptr, 0, nullptr, &async);
            LogToWindowFormat("XUserGetTokenAndSignatureUtf16Async (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            HRESULT hr = XUserGetTokenAndSignatureUtf16ResultSize(&async, &bufferSize);
            LogToWindowFormat("XUserGetTokenAndSignatureUtf16ResultSize (hr=0x%08X, size=%zu)", static_cast<uint32_t>(hr), bufferSize);
            if (SUCCEEDED(hr) && bufferSize > 0)
            {
                std::vector<uint8_t> buffer(bufferSize);
                XUserGetTokenAndSignatureUtf16Data* data = nullptr;
                hr = XUserGetTokenAndSignatureUtf16Result(&async, bufferSize, buffer.data(), &data, nullptr);
                LogToWindowFormat("XUserGetTokenAndSignatureUtf16Result (hr=0x%08X)", static_cast<uint32_t>(hr));
                if (SUCCEEDED(hr) && data)
                {
                    payload.result["tokenCount"] = data->tokenCount;
                    payload.result["signatureCount"] = data->signatureCount;
                }
            }
            return hr;
        });
}

CommandResultPayload HandleXUserGetTokenAndSignatureUtf16ResultSize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XUserGetTokenAndSignatureUtf16ResultSize: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXUserGetTokenAndSignatureUtf16Result(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XUserGetTokenAndSignatureUtf16Result: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXUserResolveIssueWithUiAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            HRESULT hr = XUserResolveIssueWithUiAsync(state->xuser, nullptr, &async);
            LogToWindowFormat("XUserResolveIssueWithUiAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XUserResolveIssueWithUiResult(&async);
            LogToWindowFormat("XUserResolveIssueWithUiResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXUserResolveIssueWithUiResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XUserResolveIssueWithUiResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXUserResolveIssueWithUiUtf16Async(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            HRESULT hr = XUserResolveIssueWithUiUtf16Async(state->xuser, nullptr, &async);
            LogToWindowFormat("XUserResolveIssueWithUiUtf16Async (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            HRESULT hr = XUserResolveIssueWithUiUtf16Result(&async);
            LogToWindowFormat("XUserResolveIssueWithUiUtf16Result (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXUserResolveIssueWithUiUtf16Result(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XUserResolveIssueWithUiUtf16Result: called inline by Async handler");
            return S_OK;
        });
}

// Static tokens for event registration
static XTaskQueueRegistrationToken s_changeEventToken{};
static XTaskQueueRegistrationToken s_deviceAssociationToken{};
static XTaskQueueRegistrationToken s_audioEndpointToken{};
static XUserSignOutDeferralHandle s_signOutDeferral = nullptr;

// Static callback for XUserChangeEvent
static void CALLBACK OnUserChangeEvent(
    _In_opt_ void* /*context*/,
    _In_ XUserLocalId userLocalId,
    _In_ XUserChangeEvent event)
{
    LogToWindowFormat("UserChangeEvent: localId=%llu, event=%u",
        static_cast<unsigned long long>(userLocalId.value),
        static_cast<uint32_t>(event));
}

// Static callback for XUserDeviceAssociationChanged
static void CALLBACK OnDeviceAssociationChanged(
    _In_opt_ void* /*context*/,
    _In_ const XUserDeviceAssociationChange* change)
{
    LogToWindowFormat("DeviceAssociationChanged: oldUser=%llu, newUser=%llu",
        static_cast<unsigned long long>(change->oldUser.value),
        static_cast<unsigned long long>(change->newUser.value));
}

// Static callback for XUserDefaultAudioEndpointUtf16Changed
static void CALLBACK OnDefaultAudioEndpointUtf16Changed(
    _In_opt_ void* /*context*/,
    XUserLocalId user,
    XUserDefaultAudioEndpointKind kind,
    _In_opt_z_ const wchar_t* endpointIdUtf16)
{
    LogToWindowFormat("DefaultAudioEndpointUtf16Changed: user=%llu, kind=%u, endpoint=%S",
        static_cast<unsigned long long>(user.value),
        static_cast<uint32_t>(kind),
        endpointIdUtf16 ? endpointIdUtf16 : L"null");
}

CommandResultPayload HandleXUserRegisterForChangeEvent(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            HRESULT hr = XUserRegisterForChangeEvent(state->taskQueue, nullptr, OnUserChangeEvent, &s_changeEventToken);
            LogToWindowFormat("XUserRegisterForChangeEvent (hr=0x%08X, token=%llu)", static_cast<uint32_t>(hr), static_cast<unsigned long long>(s_changeEventToken.token));
            if (SUCCEEDED(hr))
            {
                payload.result["token"] = s_changeEventToken.token;
            }
            return hr;
        });
}

CommandResultPayload HandleXUserUnregisterForChangeEvent(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            bool result = XUserUnregisterForChangeEvent(s_changeEventToken, true);
            s_changeEventToken = {};
            LogToWindowFormat("XUserUnregisterForChangeEvent (result=%s)", result ? "true" : "false");
            payload.result["unregistered"] = result;
            return S_OK;
        });
}

CommandResultPayload HandleXUserGetSignOutDeferral(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            HRESULT hr = XUserGetSignOutDeferral(&s_signOutDeferral);
            LogToWindowFormat("XUserGetSignOutDeferral (hr=0x%08X)", static_cast<uint32_t>(hr));
            payload.result["hasDeferral"] = (s_signOutDeferral != nullptr);
            return hr;
        });
}

CommandResultPayload HandleXUserCloseSignOutDeferralHandle(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (s_signOutDeferral)
            {
                XUserCloseSignOutDeferralHandle(s_signOutDeferral);
                s_signOutDeferral = nullptr;
                LogToWindow("XUserCloseSignOutDeferralHandle executed");
            }
            else
            {
                LogToWindow("XUserCloseSignOutDeferralHandle skipped (no deferral)");
            }
            return S_OK;
        });
}

CommandResultPayload HandleXUserRegisterForDeviceAssociationChanged(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            HRESULT hr = XUserRegisterForDeviceAssociationChanged(state->taskQueue, nullptr, OnDeviceAssociationChanged, &s_deviceAssociationToken);
            LogToWindowFormat("XUserRegisterForDeviceAssociationChanged (hr=0x%08X, token=%llu)", static_cast<uint32_t>(hr), static_cast<unsigned long long>(s_deviceAssociationToken.token));
            if (SUCCEEDED(hr))
            {
                payload.result["token"] = s_deviceAssociationToken.token;
            }
            return hr;
        });
}

CommandResultPayload HandleXUserUnregisterForDeviceAssociationChanged(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            bool result = XUserUnregisterForDeviceAssociationChanged(s_deviceAssociationToken, true);
            s_deviceAssociationToken = {};
            LogToWindowFormat("XUserUnregisterForDeviceAssociationChanged (result=%s)", result ? "true" : "false");
            payload.result["unregistered"] = result;
            return S_OK;
        });
}

CommandResultPayload HandleXUserFindControllerForUserWithUiAsync(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(
        state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            HRESULT hr = XUserFindControllerForUserWithUiAsync(state->xuser, &async);
            LogToWindowFormat("XUserFindControllerForUserWithUiAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            APP_LOCAL_DEVICE_ID foundDeviceId{};
            HRESULT hr = XUserFindControllerForUserWithUiResult(&async, &foundDeviceId);
            LogToWindowFormat("XUserFindControllerForUserWithUiResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                payload.result["deviceFound"] = true;
            }
            return hr;
        });
}

CommandResultPayload HandleXUserFindControllerForUserWithUiResult(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("XUserFindControllerForUserWithUiResult: called inline by Async handler");
            return S_OK;
        });
}

CommandResultPayload HandleXUserGetDefaultAudioEndpointUtf16(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            XUserLocalId localId{};
            HRESULT hr = XUserGetLocalId(state->xuser, &localId);
            RETURN_HR_IF(E_FAIL, FAILED(hr));

            wchar_t endpointId[XUserAudioEndpointMaxUtf16Count]{};
            size_t endpointIdUsed = 0;
            hr = XUserGetDefaultAudioEndpointUtf16(localId, XUserDefaultAudioEndpointKind::CommunicationRender, XUserAudioEndpointMaxUtf16Count, endpointId, &endpointIdUsed);
            LogToWindowFormat("XUserGetDefaultAudioEndpointUtf16 (hr=0x%08X, endpoint=%S)", static_cast<uint32_t>(hr), endpointId);
            if (SUCCEEDED(hr))
            {
                payload.result["endpointIdUsed"] = endpointIdUsed;
            }
            return hr;
        });
}

CommandResultPayload HandleXUserRegisterForDefaultAudioEndpointUtf16Changed(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            HRESULT hr = XUserRegisterForDefaultAudioEndpointUtf16Changed(state->taskQueue, nullptr, OnDefaultAudioEndpointUtf16Changed, &s_audioEndpointToken);
            LogToWindowFormat("XUserRegisterForDefaultAudioEndpointUtf16Changed (hr=0x%08X, token=%llu)", static_cast<uint32_t>(hr), static_cast<unsigned long long>(s_audioEndpointToken.token));
            if (SUCCEEDED(hr))
            {
                payload.result["token"] = s_audioEndpointToken.token;
            }
            return hr;
        });
}

CommandResultPayload HandleXUserUnregisterForDefaultAudioEndpointUtf16Changed(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            bool result = XUserUnregisterForDefaultAudioEndpointUtf16Changed(s_audioEndpointToken, true);
            s_audioEndpointToken = {};
            LogToWindowFormat("XUserUnregisterForDefaultAudioEndpointUtf16Changed (result=%s)", result ? "true" : "false");
            payload.result["unregistered"] = result;
            return S_OK;
        });
}

CommandResultPayload HandleXUserIsStoreUser(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            bool isStoreUser = XUserIsStoreUser(state->xuser);
            LogToWindowFormat("XUserIsStoreUser: %s", isStoreUser ? "true" : "false");
            payload.result["isStoreUser"] = isStoreUser;
            return S_OK;
        });
}

CommandResultPayload HandleXUserPlatformRemoteConnectCancelPrompt(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->pendingSpopOperation);

            HRESULT hr = XUserPlatformRemoteConnectCancelPrompt(state->pendingSpopOperation);
            LogToWindowFormat("XUserPlatformRemoteConnectCancelPrompt (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}



// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XUserAddAsync", HandleXUserAddAsync },
    { "XUserAddByIdWithUiAsync", HandleXUserAddByIdWithUiAsync },
    { "XUserAddByIdWithUiResult", HandleXUserAddByIdWithUiResult },
    { "XUserAddResult", HandleXUserAddResult },
    { "XUserCheckPrivilege", HandleXUserCheckPrivilege },
    { "XUserCloseHandle", HandleXUserCloseHandle },
    { "XUserCloseSignOutDeferralHandle", HandleXUserCloseSignOutDeferralHandle },
    { "XUserCompare", HandleXUserCompare },
    { "XUserDuplicateHandle", HandleXUserDuplicateHandle },
    { "XUserFindControllerForUserWithUiAsync", HandleXUserFindControllerForUserWithUiAsync },
    { "XUserFindControllerForUserWithUiResult", HandleXUserFindControllerForUserWithUiResult },
    { "XUserFindForDevice", HandleXUserFindForDevice },
    { "XUserFindUserById", HandleXUserFindUserById },
    { "XUserFindUserByLocalId", HandleXUserFindUserByLocalId },
    { "XUserGetAgeGroup", HandleXUserGetAgeGroup },
    { "XUserGetDefaultAudioEndpointUtf16", HandleXUserGetDefaultAudioEndpointUtf16 },
    { "XUserGetGamerPictureAsync", HandleXUserGetGamerPictureAsync },
    { "XUserGetGamerPictureResult", HandleXUserGetGamerPictureResult },
    { "XUserGetGamerPictureResultSize", HandleXUserGetGamerPictureResultSize },
    { "XUserGetGamertag", HandleXUserGetGamertag },
    { "XUserGetIsGuest", HandleXUserGetIsGuest },
    { "XUserGetLocalId", HandleXUserGetLocalId },
    { "XUserGetMaxUsers", HandleXUserGetMaxUsers },
    { "XUserGetSignOutDeferral", HandleXUserGetSignOutDeferral },
    { "XUserGetState", HandleXUserGetState },
    { "XUserGetTokenAndSignatureAsync", HandleXUserGetTokenAndSignatureAsync },
    { "XUserGetTokenAndSignatureResult", HandleXUserGetTokenAndSignatureResult },
    { "XUserGetTokenAndSignatureResultSize", HandleXUserGetTokenAndSignatureResultSize },
    { "XUserGetTokenAndSignatureUtf16Async", HandleXUserGetTokenAndSignatureUtf16Async },
    { "XUserGetTokenAndSignatureUtf16Result", HandleXUserGetTokenAndSignatureUtf16Result },
    { "XUserGetTokenAndSignatureUtf16ResultSize", HandleXUserGetTokenAndSignatureUtf16ResultSize },
    // { "XUserIsSignOutPresent", HandleXUserIsSignOutPresent }, // Only available in newest GDK
    { "XUserIsStoreUser", HandleXUserIsStoreUser },
    { "XUserPlatformRemoteConnectCancelPrompt", HandleXUserPlatformRemoteConnectCancelPrompt },
    { "XUserPlatformRemoteConnectSetEventHandlers", HandleXUserPlatformRemoteConnectSetEventHandlers },
    { "XUserPlatformSpopPromptComplete", HandleXUserPlatformSpopPromptComplete },
    { "XUserPlatformSpopPromptSetEventHandlers", HandleXUserPlatformSpopPromptSetEventHandlers },
    { "XUserRegisterForChangeEvent", HandleXUserRegisterForChangeEvent },
    { "XUserRegisterForDefaultAudioEndpointUtf16Changed", HandleXUserRegisterForDefaultAudioEndpointUtf16Changed },
    { "XUserRegisterForDeviceAssociationChanged", HandleXUserRegisterForDeviceAssociationChanged },
    { "XUserResolveIssueWithUiAsync", HandleXUserResolveIssueWithUiAsync },
    { "XUserResolveIssueWithUiResult", HandleXUserResolveIssueWithUiResult },
    { "XUserResolveIssueWithUiUtf16Async", HandleXUserResolveIssueWithUiUtf16Async },
    { "XUserResolveIssueWithUiUtf16Result", HandleXUserResolveIssueWithUiUtf16Result },
    { "XUserResolvePrivilegeWithUiAsync", HandleXUserResolvePrivilegeWithUiAsync },
    { "XUserResolvePrivilegeWithUiResult", HandleXUserResolvePrivilegeWithUiResult },
#if _GRDK_VER >= 0x65F41E8E /* GDK Edition 260400 */
    { "XUserSignOutAsync", HandleXUserSignOutAsync },
#endif
    // { "XUserSignOutResult", HandleXUserSignOutResult }, // Only available in newest GDK
    { "XUserUnregisterForChangeEvent", HandleXUserUnregisterForChangeEvent },
    { "XUserUnregisterForDefaultAudioEndpointUtf16Changed", HandleXUserUnregisterForDefaultAudioEndpointUtf16Changed },
    { "XUserUnregisterForDeviceAssociationChanged", HandleXUserUnregisterForDeviceAssociationChanged }
});

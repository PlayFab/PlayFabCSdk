#include "pch.h"

#include "XGameRuntimeHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"

#include <chrono>
#include "CommandRegistry.h"

CommandResultPayload HandleXGameRuntimeInitialize(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = XGameRuntimeInitialize();
            LogToWindowFormat("XGameRuntimeInitialize (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            state->runtimeInitialized = true;
            return S_OK;
        });
}

CommandResultPayload HandleXUserGetId(
    DeviceGameSaveState* state,
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
            const HRESULT hr = XUserGetId(state->xuser, &userId);
            LogToWindowFormat("XUserGetId (userId=0x%016llX, hr=0x%08X)", userId, static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);

            payload.result["userId"] = userId;
            return S_OK;
        });
}

//CommandResultPayload HandleXGameInviteRegisterForEvent(
//    DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    CommandResultPayload payload{};
//    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);
//
//    XTaskQueueHandle queue = state->taskQueue;
//
//    // Reset the event flag when registering
//    state->inviteEventReceived.store(false);
//    state->lastInviteUri.clear();
//
//    XTaskQueueRegistrationToken token{};
//    const auto start = std::chrono::steady_clock::now();
//    const HRESULT hr = XGameInviteRegisterForEvent(
//        queue,
//        state,
//        [](void* context, const char* inviteUri) {
//            auto* s = static_cast<DeviceGameSaveState*>(context);
//            LogToWindowFormat("XGameInvite event received (inviteUri=%s)", inviteUri ? inviteUri : "null");
//            s->lastInviteUri = inviteUri ? inviteUri : "";
//            s->inviteEventReceived.store(true);
//        },
//        &token);
//    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);
//
//    LogToWindowFormat("XGameInviteRegisterForEvent (token=0x%016llX, hr=0x%08X)", token.token, static_cast<uint32_t>(hr));
//    CommandHandlerShared::SetHResult(payload.result, hr);
//
//    if (FAILED(hr))
//    {
//        CommandHandlerShared::MarkFailure(payload.result, hr, "XGameInviteRegisterForEvent failed");
//        return payload;
//    }
//
//    state->inviteEventToken = token;
//    payload.result["token"] = token.token;
//    CommandHandlerShared::MarkSuccess(payload.result);
//    return payload;
//}

CommandResultPayload HandleXGameInviteRegisterForPendingEvent(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);

#ifdef GDK_2510_QFE
    XTaskQueueHandle queue = state->taskQueue;

    // Reset the event flag when registering
    state->pendingInviteEventReceived.store(false);
    state->lastPendingInviteUri.clear();

    XTaskQueueRegistrationToken token{};;
    const auto start = std::chrono::steady_clock::now();
    const HRESULT hr = XGameInviteRegisterForPendingEvent(
        queue,
        state,
        [](void* context, const char* inviteUri) {
            auto* s = static_cast<DeviceGameSaveState*>(context);
            LogToWindowFormat("XGameInvite pending event (inviteUri=%s)", 
                inviteUri ? inviteUri : "null");
            s->lastPendingInviteUri = inviteUri ? inviteUri : "";
            s->pendingInviteEventReceived.store(true);
        },
        &token);
    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);

    LogToWindowFormat("XGameInviteRegisterForPendingEvent (token=0x%016llX, hr=0x%08X)", token.token, static_cast<uint32_t>(hr));
    CommandHandlerShared::SetHResult(payload.result, hr);

    if (FAILED(hr))
    {
        CommandHandlerShared::MarkFailure(payload.result, hr, "XGameInviteRegisterForPendingEvent failed");
        return payload;
    }

    state->pendingInviteEventToken = token;
    payload.result["token"] = token.token;
    CommandHandlerShared::MarkSuccess(payload.result);
#else
    UNREFERENCED_PARAMETER(state);
    LogToWindow("XGameInviteRegisterForPendingEvent: Skipped - requires GDK 2510 QFE or later");
    payload.result["skipped"] = true;
    payload.result["skipReason"] = "Requires GDK 2510 QFE or later";
    CommandHandlerShared::MarkSuccess(payload.result);
    CommandHandlerShared::SetHResult(payload.result, S_OK);
#endif
    return payload;
}

CommandResultPayload HandleWaitForXGameInviteRegisterForEvent(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);

    // Get optional timeout parameter (default 60 seconds)
    int64_t timeoutSeconds = 60;
    if (parameters.is_object())
    {
        auto it = parameters.find("timeoutSeconds");
        if (it != parameters.end())
        {
            if (it->is_number())
            {
                timeoutSeconds = it->get<int64_t>();
            }
            else if (it->is_string())
            {
                try { timeoutSeconds = std::stoll(it->get<std::string>()); } catch (...) {}
            }
            if (timeoutSeconds <= 0)
            {
                timeoutSeconds = 60;
            }
        }
    }

    const auto start = std::chrono::steady_clock::now();
    const auto timeout = std::chrono::seconds(timeoutSeconds);

    LogToWindowFormat("WaitForXGameInviteRegisterForEvent: waiting up to %lld seconds...", timeoutSeconds);

    // Wait for the invite event or timeout
    while (!state->inviteEventReceived.load())
    {
        // Check for timeout
        auto elapsed = std::chrono::steady_clock::now() - start;
        if (elapsed >= timeout)
        {
            payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);
            LogToWindowFormat("WaitForXGameInviteRegisterForEvent: timed out after %lld seconds", timeoutSeconds);
            CommandHandlerShared::SetHResult(payload.result, HRESULT_FROM_WIN32(ERROR_TIMEOUT));
            CommandHandlerShared::MarkFailure(payload.result, HRESULT_FROM_WIN32(ERROR_TIMEOUT), "Timed out waiting for XGameInvite event");
            return payload;
        }

        // Dispatch task queue to process callbacks
        if (state->taskQueue)
        {
            while (XTaskQueueDispatch(state->taskQueue, XTaskQueuePort::Completion, 0))
            {
            }
        }
        Sleep(10);
    }

    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);
    LogToWindowFormat("WaitForXGameInviteRegisterForEvent: event received (inviteUri=%s)", state->lastInviteUri.c_str());
    
    payload.result["inviteUri"] = state->lastInviteUri;
    CommandHandlerShared::SetHResult(payload.result, S_OK);
    CommandHandlerShared::MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandleWaitForXGameInviteRegisterForPendingEvent(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);

    // Get optional timeout parameter (default 60 seconds)
    int64_t timeoutSeconds = 60;
    if (parameters.is_object())
    {
        auto it = parameters.find("timeoutSeconds");
        if (it != parameters.end())
        {
            if (it->is_number())
            {
                timeoutSeconds = it->get<int64_t>();
            }
            else if (it->is_string())
            {
                try { timeoutSeconds = std::stoll(it->get<std::string>()); } catch (...) {}
            }
            if (timeoutSeconds <= 0)
            {
                timeoutSeconds = 60;
            }
        }
    }

    const auto start = std::chrono::steady_clock::now();
    const auto timeout = std::chrono::seconds(timeoutSeconds);

    LogToWindowFormat("WaitForXGameInviteRegisterForPendingEvent: waiting up to %lld seconds...", timeoutSeconds);

    // Wait for the pending invite event or timeout
    while (!state->pendingInviteEventReceived.load())
    {
        // Check for timeout
        auto elapsed = std::chrono::steady_clock::now() - start;
        if (elapsed >= timeout)
        {
            payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);
            LogToWindowFormat("WaitForXGameInviteRegisterForPendingEvent: timed out after %lld seconds", timeoutSeconds);
            CommandHandlerShared::SetHResult(payload.result, HRESULT_FROM_WIN32(ERROR_TIMEOUT));
            CommandHandlerShared::MarkFailure(payload.result, HRESULT_FROM_WIN32(ERROR_TIMEOUT), "Timed out waiting for XGameInvite pending event");
            return payload;
        }

        // Dispatch task queue to process callbacks
        if (state->taskQueue)
        {
            while (XTaskQueueDispatch(state->taskQueue, XTaskQueuePort::Completion, 0))
            {
            }
        }
        Sleep(10);
    }

    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);
    LogToWindowFormat("WaitForXGameInviteRegisterForPendingEvent: event received (inviteUri=%s)", state->lastPendingInviteUri.c_str());
    
    payload.result["inviteUri"] = state->lastPendingInviteUri;
    CommandHandlerShared::SetHResult(payload.result, S_OK);
    CommandHandlerShared::MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandleXGameUiShowPlayerPickerAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCall(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            // Get optional parameters
            std::string promptDisplayText;
            std::string error;
            CommandHandlerShared::TryGetStringParameter(parameters, "promptDisplayText", promptDisplayText, error);

            uint32_t maxSelectedPlayers = 1;
            if (parameters.contains("maxSelectedPlayers"))
            {
                const auto& val = parameters["maxSelectedPlayers"];
                if (val.is_number_unsigned())
                {
                    maxSelectedPlayers = val.get<uint32_t>();
                }
                else if (val.is_number_integer())
                {
                    maxSelectedPlayers = static_cast<uint32_t>(val.get<int64_t>());
                }
                else if (val.is_string())
                {
                    try { maxSelectedPlayers = static_cast<uint32_t>(std::stoul(val.get<std::string>())); } catch (...) {}
                }
            }

            HRESULT hr = XGameUiShowPlayerPickerAsync(
                &async,
                state->xuser,
                promptDisplayText.empty() ? nullptr : promptDisplayText.c_str(),
                0,       // selectFromPlayersCount
                nullptr, // selectFromPlayers
                0,       // preselectedPlayersCount
                nullptr, // preselectedPlayers
                0,       // minSelectionCount
                maxSelectedPlayers);

            if (SUCCEEDED(hr))
            {
                hr = XAsyncGetStatus(&async, true);
            }

            uint32_t resultCount = 0;
            uint32_t resultPlayersUsed = 0;
            std::vector<uint64_t> selectedPlayers;
            if (SUCCEEDED(hr))
            {
                hr = XGameUiShowPlayerPickerResultCount(&async, &resultCount);
                if (SUCCEEDED(hr) && resultCount > 0)
                {
                    selectedPlayers.resize(resultCount);
                    hr = XGameUiShowPlayerPickerResult(&async, resultCount, selectedPlayers.data(), &resultPlayersUsed);
                }
            }

            LogToWindowFormat("XGameUiShowPlayerPickerAsync (resultCount=%u, hr=0x%08X)", resultCount, static_cast<uint32_t>(hr));

            if (SUCCEEDED(hr))
            {
                nlohmann::json xuidsArray = nlohmann::json::array();
                for (const auto& xuid : selectedPlayers)
                {
                    xuidsArray.push_back(xuid);
                }
                payload.result["selectedPlayers"] = xuidsArray;
            }
            return hr;
        });
}

//CommandResultPayload HandleXGameInviteUnregisterForEvent(
//    DeviceGameSaveState* state,
//    const std::string& commandId,
//    const std::string& command,
//    [[maybe_unused]] const nlohmann::json& parameters,
//    const std::string& deviceId)
//{
//    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
//        [&](CommandResultPayload& payload) -> HRESULT
//        {
//            const bool result = XGameInviteUnregisterForEvent(state->inviteEventToken, true);
//            LogToWindowFormat("XGameInviteUnregisterForEvent (token=0x%016llX, result=%s)",
//                state->inviteEventToken.token, result ? "true" : "false");
//            state->inviteEventToken = {};
//            payload.result["unregistered"] = result;
//            return S_OK;
//        });
//}

CommandResultPayload HandleXGameInviteUnregisterForPendingEvent(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
#ifdef GDK_2510_QFE
            const bool result = XGameInviteUnregisterForPendingEvent(state->pendingInviteEventToken, true);
            LogToWindowFormat("XGameInviteUnregisterForPendingEvent (token=0x%016llX, result=%s)",
                state->pendingInviteEventToken.token, result ? "true" : "false");
            state->pendingInviteEventToken = {};
            payload.result["unregistered"] = result;
#else
            UNREFERENCED_PARAMETER(state);
            LogToWindow("XGameInviteUnregisterForPendingEvent: Skipped - requires GDK 2510 QFE or later");
            payload.result["skipped"] = true;
            payload.result["skipReason"] = "Requires GDK 2510 QFE or later";
            payload.result["unregistered"] = false;
#endif
            return S_OK;
        });
}

CommandResultPayload HandleXGameInviteAcceptPendingInvite(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
#ifdef GDK_2510_QFE
            // Get optional inviteUri parameter
            std::string inviteUri;
            std::string error;
            CommandHandlerShared::TryGetStringParameter(parameters, "inviteUri", inviteUri, error);

            const HRESULT hr = XGameInviteAcceptPendingInvite(inviteUri.empty() ? nullptr : inviteUri.c_str());
            LogToWindowFormat("XGameInviteAcceptPendingInvite (inviteUri=%s, hr=0x%08X)",
                inviteUri.empty() ? "null" : inviteUri.c_str(), static_cast<uint32_t>(hr));
            return hr;
#else
            LogToWindow("XGameInviteAcceptPendingInvite: Skipped - requires GDK 2510 QFE or later");
            payload.result["skipped"] = true;
            payload.result["skipReason"] = "Requires GDK 2510 QFE or later";
            return S_OK;
#endif
        });
}

CommandResultPayload HandleXGameRuntimeIsFeatureAvailable(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const bool result = XGameRuntimeIsFeatureAvailable(XGameRuntimeFeature::XTaskQueue);
            LogToWindowFormat("XGameRuntimeIsFeatureAvailable (XTaskQueue=%s)", result ? "true" : "false");
            payload.result["available"] = result;
            return S_OK;
        });
}

CommandResultPayload HandleXGameRuntimeInitializeWithOptions(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XGameRuntimeOptions options{};
            options.gameConfigSource = XGameRuntimeGameConfigSource::Default;
            options.gameConfig = nullptr;
            const HRESULT hr = XGameRuntimeInitializeWithOptions(&options);
            LogToWindowFormat("XGameRuntimeInitializeWithOptions (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            state->runtimeInitialized = true;
            return S_OK;
        });
}

CommandResultPayload HandleXGameRuntimeUninitialize(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XGameRuntimeUninitialize();
            LogToWindow("XGameRuntimeUninitialize called");
            state->runtimeInitialized = false;
            return S_OK;
        });
}



// Self-registration of commands
static CommandRegistrar s_registrar({
    { "WaitForXGameInviteRegisterForEvent", HandleWaitForXGameInviteRegisterForEvent },
    { "WaitForXGameInviteRegisterForPendingEvent", HandleWaitForXGameInviteRegisterForPendingEvent },
    { "XGameInviteAcceptPendingInvite", HandleXGameInviteAcceptPendingInvite },
    //{ "XGameInviteRegisterForEvent", HandleXGameInviteRegisterForEvent },
    { "XGameInviteRegisterForPendingEvent", HandleXGameInviteRegisterForPendingEvent },
    //{ "XGameInviteUnregisterForEvent", HandleXGameInviteUnregisterForEvent },
    //{ "XGameInviteUnregisterForPendingEvent", HandleXGameInviteUnregisterForPendingEvent },
    { "XGameRuntimeInitialize", HandleXGameRuntimeInitialize },
    { "XGameRuntimeInitializeWithOptions", HandleXGameRuntimeInitializeWithOptions },
    { "XGameRuntimeIsFeatureAvailable", HandleXGameRuntimeIsFeatureAvailable },
    { "XGameRuntimeUninitialize", HandleXGameRuntimeUninitialize },
    { "XGameUiShowPlayerPickerAsync", HandleXGameUiShowPlayerPickerAsync },
    { "XUserGetId", HandleXUserGetId }
});

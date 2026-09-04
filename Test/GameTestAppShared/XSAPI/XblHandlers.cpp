#include "pch.h"

#include "XblHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"
#include "CommandRegistry.h"

CommandResultPayload HandleXblInitialize(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string scid;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "scid", scid, error))
            {
                return E_INVALIDARG;
            }

            XblInitArgs initArgs{};
            initArgs.scid = scid.c_str();
            const HRESULT hr = XblInitialize(&initArgs);
            LogToWindowFormat("XblInitialize (scid=%s, hr=0x%08X)", scid.c_str(), static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                state->xblInitialized = true;
            }
            return hr;
        });
}

CommandResultPayload HandleXblContextCreateHandle(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xuser);

            XblContextHandle context{ nullptr };
            const HRESULT hr = XblContextCreateHandle(state->xuser, &context);
            LogToWindowFormat("XblContextCreateHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
            if (SUCCEEDED(hr))
            {
                if (state->xblContext)
                {
                    XblContextCloseHandle(state->xblContext);
                }
                state->xblContext = context;
            }
            return hr;
        });
}

CommandResultPayload HandleXblCleanupAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCallWithResult(nullptr, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            if (state->xblContext)
            {
                XblContextCloseHandle(state->xblContext);
                state->xblContext = nullptr;
            }

            HRESULT hr = XblCleanupAsync(&async);
            LogToWindowFormat("XblCleanupAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        },
        [&](XAsyncBlock&, CommandResultPayload&) -> HRESULT
        {
            state->xblInitialized = false;
            return S_OK;
        });
}

CommandResultPayload HandleXblContextCloseHandle(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);
            XblContextCloseHandle(state->xblContext);
            state->xblContext = nullptr;
            LogToWindowFormat("XblContextCloseHandle");
            return S_OK;
        });
}

CommandResultPayload HandleXblMultiplayerActivitySendInvitesAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCall(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);
            auto xuids = CommandHandlerShared::GetUint64Array(parameters, "xuids");
            RETURN_HR_IF(E_INVALIDARG, xuids.empty());

            HRESULT hr = XblMultiplayerActivitySendInvitesAsync(
                state->xblContext,
                xuids.data(),
                static_cast<uint32_t>(xuids.size()),
                true,     // allowCrossPlatformJoin
                nullptr,  // connectionString
                &async);
            LogToWindowFormat("XblMultiplayerActivitySendInvitesAsync (count=%zu, hr=0x%08X)",
                xuids.size(), static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleInteractiveInviteLoop(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            RETURN_HR_IF(E_POINTER, !state->xblContext);
            auto xuids = CommandHandlerShared::GetUint64Array(parameters, "xuids");
            RETURN_HR_IF(E_INVALIDARG, xuids.empty());

            int invitesSent = 0;

            LogToWindow("========================================");
            LogToWindow("INTERACTIVE INVITE LOOP");
            LogToWindowFormat("Target XUIDs: %zu recipient(s)", xuids.size());
            LogToWindow("Press SPACE to send invite");
            LogToWindow("Press 'Q' to quit loop");
            LogToWindow("========================================");

            state->waitingForUserInput = true;
            state->userInputReceived = false;
            state->lastUserInputKey = 0;

            bool exitLoop = false;
            while (!exitLoop && !state->quit)
            {
                MSG msg;
                while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }

                if (state->userInputReceived)
                {
                    int key = state->lastUserInputKey;
                    state->userInputReceived = false;
                    state->lastUserInputKey = 0;

                    if (key == 'Q' || key == 'q')
                    {
                        LogToWindow("[InteractiveInviteLoop] 'Q' pressed - exiting loop");
                        exitLoop = true;
                    }
                    else if (key == VK_SPACE)
                    {
                        LogToWindow("[InteractiveInviteLoop] SPACE pressed - sending invite...");

                        XAsyncBlock inviteAsync{};
                        inviteAsync.queue = state->taskQueue;
                        HRESULT hr = XblMultiplayerActivitySendInvitesAsync(
                            state->xblContext, xuids.data(), static_cast<uint32_t>(xuids.size()),
                            true, nullptr, &inviteAsync);
                        if (SUCCEEDED(hr))
                        {
                            hr = XAsyncGetStatus(&inviteAsync, true);
                        }

                        if (SUCCEEDED(hr))
                        {
                            invitesSent++;
                            LogToWindowFormat("[InteractiveInviteLoop] Invite #%d sent successfully (count=%zu)",
                                invitesSent, xuids.size());
                        }
                        else
                        {
                            LogToWindowFormat("[InteractiveInviteLoop] Invite send failed (hr=0x%08X)",
                                static_cast<uint32_t>(hr));
                        }
                    }
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }

            state->waitingForUserInput = false;
            payload.result["invitesSent"] = invitesSent;

            if (state->quit)
            {
                LogToWindow("[InteractiveInviteLoop] Application quit during loop");
                return E_ABORT;
            }

            LogToWindowFormat("[InteractiveInviteLoop] Exited loop. Total invites sent: %d", invitesSent);
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "InteractiveInviteLoop", HandleInteractiveInviteLoop },
    { "XblCleanupAsync", HandleXblCleanupAsync },
    { "XblContextCloseHandle", HandleXblContextCloseHandle },
    { "XblContextCreateHandle", HandleXblContextCreateHandle },
    { "XblInitialize", HandleXblInitialize },
    { "XblMultiplayerActivitySendInvitesAsync", HandleXblMultiplayerActivitySendInvitesAsync }
});

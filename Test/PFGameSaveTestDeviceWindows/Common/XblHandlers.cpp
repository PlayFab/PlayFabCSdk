#include "pch.h"

#include "XblHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"

#include <chrono>

CommandResultPayload HandleXblInitialize(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);

    // Get scid from parameters or use default
    std::string scid;
    std::string error;
    if (!CommandHandlerShared::TryGetStringParameter(parameters, "scid", scid, error))
    {
        // scid is required
        CommandHandlerShared::MarkFailure(payload.result, E_INVALIDARG, "Missing required parameter 'scid'");
        return payload;
    }

    XblInitArgs initArgs{};
    initArgs.scid = scid.c_str();

    const auto start = std::chrono::steady_clock::now();
    const HRESULT hr = XblInitialize(&initArgs);
    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);

    LogToWindowFormat("XblInitialize (scid=%s, hr=0x%08X)", scid.c_str(), static_cast<uint32_t>(hr));
    CommandHandlerShared::SetHResult(payload.result, hr);

    if (FAILED(hr))
    {
        CommandHandlerShared::MarkFailure(payload.result, hr, "XblInitialize failed");
        return payload;
    }

    state->xblInitialized = true;
    CommandHandlerShared::MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandleXblContextCreateHandle(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);

    if (!state->xuser)
    {
        CommandHandlerShared::MarkFailure(payload.result, E_POINTER, "XUser handle not created");
        return payload;
    }

    XblContextHandle context{ nullptr };
    const auto start = std::chrono::steady_clock::now();
    const HRESULT hr = XblContextCreateHandle(state->xuser, &context);
    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);

    LogToWindowFormat("XblContextCreateHandle (hr=0x%08X)", static_cast<uint32_t>(hr));
    CommandHandlerShared::SetHResult(payload.result, hr);

    if (FAILED(hr))
    {
        CommandHandlerShared::MarkFailure(payload.result, hr, "XblContextCreateHandle failed");
        return payload;
    }

    // Close existing context if any
    if (state->xblContext)
    {
        XblContextCloseHandle(state->xblContext);
    }

    state->xblContext = context;
    CommandHandlerShared::MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandleXblCleanupAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);

    // Close XblContext if it exists
    if (state->xblContext)
    {
        XblContextCloseHandle(state->xblContext);
        state->xblContext = nullptr;
    }

    XAsyncBlock async{};
    async.queue = nullptr; // Use default queue

    const auto start = std::chrono::steady_clock::now();
    HRESULT hr = XblCleanupAsync(&async);
    if (SUCCEEDED(hr))
    {
        hr = XAsyncGetStatus(&async, true); // Wait for completion
    }
    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);

    LogToWindowFormat("XblCleanupAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
    CommandHandlerShared::SetHResult(payload.result, hr);

    if (FAILED(hr))
    {
        CommandHandlerShared::MarkFailure(payload.result, hr, "XblCleanupAsync failed");
        return payload;
    }

    state->xblInitialized = false;
    CommandHandlerShared::MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandleXblContextCloseHandle(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);

    if (!state->xblContext)
    {
        CommandHandlerShared::MarkFailure(payload.result, E_POINTER, "XblContext handle not created");
        return payload;
    }

    const auto start = std::chrono::steady_clock::now();
    XblContextCloseHandle(state->xblContext);
    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);

    state->xblContext = nullptr;

    LogToWindowFormat("XblContextCloseHandle");
    CommandHandlerShared::MarkSuccess(payload.result);
    CommandHandlerShared::SetHResult(payload.result, S_OK);
    return payload;
}

CommandResultPayload HandleXblMultiplayerActivitySendInvitesAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);

    if (!state->xblContext)
    {
        CommandHandlerShared::MarkFailure(payload.result, E_POINTER, "XblContext handle not created");
        return payload;
    }

    // Get xuids from parameters
    std::vector<uint64_t> xuids;
    if (parameters.contains("xuids") && parameters["xuids"].is_array())
    {
        for (const auto& xuid : parameters["xuids"])
        {
            if (xuid.is_number_unsigned())
            {
                xuids.push_back(xuid.get<uint64_t>());
            }
            else if (xuid.is_number_integer())
            {
                xuids.push_back(static_cast<uint64_t>(xuid.get<int64_t>()));
            }
            else if (xuid.is_string())
            {
                // Handle string xuids (from YAML parsing)
                try
                {
                    xuids.push_back(std::stoull(xuid.get<std::string>()));
                }
                catch (...)
                {
                    // Skip invalid entries
                }
            }
        }
    }

    if (xuids.empty())
    {
        CommandHandlerShared::MarkFailure(payload.result, E_INVALIDARG, "Missing required parameter 'xuids' array");
        return payload;
    }

    XAsyncBlock async{};
    async.queue = state->taskQueue;

    const auto start = std::chrono::steady_clock::now();
    HRESULT hr = XblMultiplayerActivitySendInvitesAsync(
        state->xblContext,
        xuids.data(),
        static_cast<uint32_t>(xuids.size()),
        true,  // allowCrossPlatformJoin
        nullptr,  // connectionString
        &async);
    if (SUCCEEDED(hr))
    {
        hr = XAsyncGetStatus(&async, true);
    }
    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);

    LogToWindowFormat("XblMultiplayerActivitySendInvitesAsync (count=%zu, hr=0x%08X)", xuids.size(), static_cast<uint32_t>(hr));
    CommandHandlerShared::SetHResult(payload.result, hr);

    if (FAILED(hr))
    {
        CommandHandlerShared::MarkFailure(payload.result, hr, "XblMultiplayerActivitySendInvitesAsync failed");
        return payload;
    }

    CommandHandlerShared::MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandleInteractiveInviteLoop(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CommandHandlerShared::CreateBaseResult(commandId, command, deviceId);

    if (!state->xblContext)
    {
        CommandHandlerShared::MarkFailure(payload.result, E_POINTER, "XblContext handle not created");
        return payload;
    }

    // Get xuids from parameters
    std::vector<uint64_t> xuids;
    if (parameters.contains("xuids") && parameters["xuids"].is_array())
    {
        for (const auto& xuid : parameters["xuids"])
        {
            if (xuid.is_number_unsigned())
            {
                xuids.push_back(xuid.get<uint64_t>());
            }
            else if (xuid.is_number_integer())
            {
                xuids.push_back(static_cast<uint64_t>(xuid.get<int64_t>()));
            }
            else if (xuid.is_string())
            {
                try
                {
                    xuids.push_back(std::stoull(xuid.get<std::string>()));
                }
                catch (...)
                {
                    // Skip invalid entries
                }
            }
        }
    }

    if (xuids.empty())
    {
        CommandHandlerShared::MarkFailure(payload.result, E_INVALIDARG, "Missing required parameter 'xuids' array");
        return payload;
    }

    const auto start = std::chrono::steady_clock::now();
    int invitesSent = 0;

    // Log instructions
    LogToWindow("========================================");
    LogToWindow("INTERACTIVE INVITE LOOP");
    LogToWindowFormat("Target XUIDs: %zu recipient(s)", xuids.size());
    LogToWindow("Press SPACE to send invite");
    LogToWindow("Press 'Q' to quit loop");
    LogToWindow("========================================");

    // Set up state flags to receive input from window proc
    state->waitingForUserInput = true;
    state->userInputReceived = false;
    state->lastUserInputKey = 0;

    bool exitLoop = false;
    while (!exitLoop && !state->quit)
    {
        // Process Windows messages to keep UI responsive
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Check for user input
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

                XAsyncBlock async{};
                async.queue = state->taskQueue;

                HRESULT hr = XblMultiplayerActivitySendInvitesAsync(
                    state->xblContext,
                    xuids.data(),
                    static_cast<uint32_t>(xuids.size()),
                    true,  // allowCrossPlatformJoin
                    nullptr,  // connectionString
                    &async);
                if (SUCCEEDED(hr))
                {
                    hr = XAsyncGetStatus(&async, true);
                }

                if (SUCCEEDED(hr))
                {
                    invitesSent++;
                    LogToWindowFormat("[InteractiveInviteLoop] Invite #%d sent successfully (count=%zu)", invitesSent, xuids.size());
                }
                else
                {
                    LogToWindowFormat("[InteractiveInviteLoop] Invite send failed (hr=0x%08X)", static_cast<uint32_t>(hr));
                }
            }
        }

        // Small delay to avoid burning CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    state->waitingForUserInput = false;

    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);
    payload.result["invitesSent"] = invitesSent;

    if (state->quit)
    {
        LogToWindow("[InteractiveInviteLoop] Application quit during loop");
        CommandHandlerShared::MarkFailure(payload.result, E_ABORT, "Application quit during invite loop");
    }
    else
    {
        LogToWindowFormat("[InteractiveInviteLoop] Exited loop. Total invites sent: %d", invitesSent);
        CommandHandlerShared::MarkSuccess(payload.result);
    }

    return payload;
}

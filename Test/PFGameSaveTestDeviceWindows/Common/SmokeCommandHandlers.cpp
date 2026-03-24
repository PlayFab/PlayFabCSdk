#include "pch.h"

#include "SmokeCommandHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"

#include <algorithm>
#include <chrono>
#include <limits>
#include <string>
#include <thread>

namespace
{
    using CommandHandlerShared::CreateBaseResult;
    using CommandHandlerShared::MarkFailure;
    using CommandHandlerShared::MarkSuccess;

    std::string ExtractEchoPayload(const nlohmann::json& parameters)
    {
        std::string payloadText;
        try
        {
            if (parameters.is_object() && parameters.contains("payload"))
            {
                const auto& payloadNode = parameters["payload"];
                if (payloadNode.is_string())
                {
                    payloadText = payloadNode.get<std::string>();
                }
                else
                {
                    payloadText = payloadNode.dump();
                }
            }
        }
        catch (const std::exception& ex)
        {
            payloadText = std::string("<error reading payload: ") + ex.what() + ">";
        }

        if (payloadText.empty())
        {
            payloadText = "<empty>";
        }

        return payloadText;
    }

    int ExtractDurationMs(const nlohmann::json& parameters)
    {
        if (!parameters.is_object())
        {
            return 0;
        }

        auto durationIt = parameters.find("durationMs");
        if (durationIt == parameters.end())
        {
            return 0;
        }

        try
        {
            if (durationIt->is_number_integer())
            {
                return std::max(0, durationIt->get<int>());
            }

            if (durationIt->is_number_unsigned())
            {
                auto value = durationIt->get<uint64_t>();
                if (value > static_cast<uint64_t>(std::numeric_limits<int>::max()))
                {
                    return std::numeric_limits<int>::max();
                }

                return static_cast<int>(value);
            }

            if (durationIt->is_string())
            {
                const std::string text = durationIt->get<std::string>();
                size_t index = 0;
                int parsed = std::stoi(text, &index, 10);
                if (index == text.size())
                {
                    return std::max(0, parsed);
                }
            }
        }
        catch (...)
        {
            // Ignore parsing failures and fall through to default value.
        }

        return 0;
    }
}

CommandResultPayload HandleSmokeCommandEcho(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);
    const std::string payloadText = ExtractEchoPayload(parameters);
    LogToWindow("Echo [" + deviceId + "]: " + payloadText);
    MarkSuccess(payload.result);
    payload.elapsedMs = 0;
    return payload;
}

CommandResultPayload HandleSmokeDelay(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    const int durationMs = ExtractDurationMs(parameters);
    LogToWindow("Delay [" + deviceId + "]: " + std::to_string(durationMs) + " ms");

    const auto start = std::chrono::steady_clock::now();
    if (durationMs > 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(durationMs));
    }

    payload.elapsedMs = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start).count());
    MarkSuccess(payload.result);
    return payload;
}

CommandResultPayload HandleSmokeTestFailure(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);
    LogToWindow("Command: [" + deviceId + "] " + command + " (simulated failure)");
    MarkFailure(payload.result, E_FAIL, "Simulated failure");
    payload.elapsedMs = 0;
    return payload;
}

CommandResultPayload HandleWaitForUserInput(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    const auto start = std::chrono::steady_clock::now();

    // Extract optional message parameter
    std::string message = "Press any key to continue...";
    if (parameters.is_object() && parameters.contains("message"))
    {
        const auto& messageNode = parameters["message"];
        if (messageNode.is_string())
        {
            message = messageNode.get<std::string>();
        }
    }

    // Log the save folder path and instructions
    LogToWindow("========================================");
    LogToWindow("WAITING FOR USER INPUT");
    LogToWindow("Save Folder: " + state->saveFolder);
    LogToWindow(message);
    LogToWindow("Press 'O' to open save folder in Explorer");
    LogToWindow("Press SPACE or ENTER to continue...");
    LogToWindow("========================================");

    // Set up state flags to receive input from window proc
    state->waitingForUserInput = true;
    state->userInputReceived = false;

    // Wait for keypress using a simple polling loop with message processing
    while (!state->userInputReceived && !state->quit)
    {
        // Process Windows messages to keep UI responsive
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Small delay to avoid burning CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    state->waitingForUserInput = false;

    if (state->quit)
    {
        MarkFailure(payload.result, E_ABORT, "Application quit during wait");
    }
    else
    {
        LogToWindow("User input received, continuing...");
        MarkSuccess(payload.result);
    }

    payload.elapsedMs = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start).count());
    return payload;
}

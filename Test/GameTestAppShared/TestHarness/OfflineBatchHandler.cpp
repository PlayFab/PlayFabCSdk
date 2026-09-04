// Copyright (C) Microsoft Corporation. All rights reserved.
//
// Offline batch execution — allows the controller to queue commands for execution
// while the device's network is down (xbstress network=broken). The controller
// sends BeginOfflineBatch before disabling network, the device executes the
// commands autonomously, and the controller retrieves results after re-enabling
// network and the WebSocket reconnects.

#include "pch.h"

#include "OfflineBatchHandler.h"
#include "CommandHandlerShared.h"
#include "CommandRegistry.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"

#include <chrono>
#include <mutex>
#include <thread>
#include <vector>

namespace
{
    using CommandHandlerShared::CreateBaseResult;
    using CommandHandlerShared::MarkFailure;
    using CommandHandlerShared::MarkSuccess;

    struct BatchCommand
    {
        std::string commandId;
        std::string command;
        nlohmann::json parameters;
        int timeoutSeconds{ 180 };
    };

    struct OfflineBatchState
    {
        std::mutex mutex;
        std::vector<BatchCommand> commands;
        std::vector<nlohmann::json> results;
        bool executing{ false };
        bool complete{ false };
    };

    OfflineBatchState g_offlineBatch;

    void ExecuteOfflineBatch(DeviceGameSaveState* state, int startDelayMs)
    {
        LogToWindowFormat("[OfflineBatch] Waiting %dms for network to go down...", startDelayMs);
        std::this_thread::sleep_for(std::chrono::milliseconds(startDelayMs));

        std::vector<BatchCommand> commands;
        {
            std::lock_guard<std::mutex> lock(g_offlineBatch.mutex);
            commands = g_offlineBatch.commands;
            g_offlineBatch.executing = true;
        }

        std::string deviceId;
        if (state != nullptr)
        {
            deviceId = state->inputDeviceId;
        }
        if (deviceId.empty())
        {
            deviceId = "TestDevice";
        }

        LogToWindowFormat("[OfflineBatch] Starting execution of %zu commands.", commands.size());

        std::vector<nlohmann::json> results;
        results.reserve(commands.size());

        for (size_t i = 0; i < commands.size(); ++i)
        {
            const auto& cmd = commands[i];
            LogToWindowFormat("[OfflineBatch] [%zu/%zu] Executing: %s (id=%s)",
                i + 1, commands.size(), cmd.command.c_str(), cmd.commandId.c_str());

            auto startTime = std::chrono::steady_clock::now();

            CommandResultPayload resultPayload = BuildActionResult(
                state, cmd.commandId, cmd.command, cmd.parameters, deviceId);

            auto endTime = std::chrono::steady_clock::now();
            int elapsedMs = static_cast<int>(
                std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count());

            resultPayload.result["elapsedMs"] = elapsedMs;

            // Extract status for logging
            std::string status = "unknown";
            uint32_t resultHr = 0;
            if (resultPayload.result.contains("status"))
            {
                status = resultPayload.result["status"].get<std::string>();
            }
            if (resultPayload.result.contains("hresult"))
            {
                resultHr = static_cast<uint32_t>(resultPayload.result["hresult"].get<int64_t>());
            }

            LogToWindowFormat("[OfflineBatch] [%zu/%zu] << %s >> status=%s, hr=0x%08X, %dms",
                i + 1, commands.size(), cmd.command.c_str(),
                status.c_str(), resultHr, elapsedMs);

            results.push_back(std::move(resultPayload.result));
        }

        {
            std::lock_guard<std::mutex> lock(g_offlineBatch.mutex);
            g_offlineBatch.results = std::move(results);
            g_offlineBatch.executing = false;
            g_offlineBatch.complete = true;
        }

        LogToWindowFormat("[OfflineBatch] All %zu commands complete.", commands.size());

        // Signal the main loop to force a WebSocket reconnect. During xbstress
        // network=broken, the HC websocket may not receive a TCP close frame,
        // leaving m_connected stuck on true. This flag tells the pump to
        // disconnect and reconnect now that the batch is done and network
        // should be restored shortly.
        if (state != nullptr)
        {
            state->forceWebsocketReconnect.store(true);
        }
    }
}

CommandResultPayload HandleBeginOfflineBatch(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload result{};
    result.result = CreateBaseResult(commandId, command, deviceId);

    auto commandsIt = parameters.find("commands");
    if (commandsIt == parameters.end() || !commandsIt->is_array())
    {
        MarkFailure(result.result, E_INVALIDARG, "Missing or invalid 'commands' parameter");
        return result;
    }

    int startDelayMs = 5000;
    auto delayIt = parameters.find("startDelayMs");
    if (delayIt != parameters.end() && delayIt->is_number_integer())
    {
        startDelayMs = delayIt->get<int>();
    }

    {
        std::lock_guard<std::mutex> lock(g_offlineBatch.mutex);
        g_offlineBatch.commands.clear();
        g_offlineBatch.results.clear();
        g_offlineBatch.executing = false;
        g_offlineBatch.complete = false;

        for (const auto& cmdJson : *commandsIt)
        {
            BatchCommand cmd;
            cmd.commandId = cmdJson.value("commandId", "");
            cmd.command = cmdJson.value("command", "");
            cmd.timeoutSeconds = cmdJson.value("timeoutSeconds", 180);

            // Ensure parameters is always an object (not null)
            auto paramsIt = cmdJson.find("parameters");
            if (paramsIt != cmdJson.end() && paramsIt->is_object())
            {
                cmd.parameters = *paramsIt;
            }
            else
            {
                cmd.parameters = nlohmann::json::object();
            }

            g_offlineBatch.commands.push_back(std::move(cmd));
        }
    }

    size_t batchSize = g_offlineBatch.commands.size();

    // Launch batch execution on a detached background thread.
    // The thread sleeps for startDelayMs to allow the controller to disable
    // the network, then executes all commands sequentially.
    std::thread batchThread(ExecuteOfflineBatch, state, startDelayMs);
    batchThread.detach();

    MarkSuccess(result.result);
    result.result["batchSize"] = static_cast<int>(batchSize);
    result.result["startDelayMs"] = startDelayMs;

    LogToWindowFormat("[OfflineBatch] Queued %zu commands (startDelay=%dms)", batchSize, startDelayMs);

    return result;
}

CommandResultPayload HandleGetOfflineBatchResults(
    DeviceGameSaveState* /*state*/,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& /*parameters*/,
    const std::string& deviceId)
{
    CommandResultPayload result{};
    result.result = CreateBaseResult(commandId, command, deviceId);

    std::lock_guard<std::mutex> lock(g_offlineBatch.mutex);

    if (g_offlineBatch.executing)
    {
        MarkFailure(result.result, static_cast<HRESULT>(0x80000005) /*E_PENDING*/,
            "Batch is still executing");
        result.result["batchExecuting"] = true;
        return result;
    }

    if (!g_offlineBatch.complete)
    {
        MarkFailure(result.result, static_cast<HRESULT>(0x80000005) /*E_PENDING*/,
            "No batch results available — batch may not have started");
        return result;
    }

    nlohmann::json resultsArray = nlohmann::json::array();
    for (const auto& r : g_offlineBatch.results)
    {
        resultsArray.push_back(r);
    }

    MarkSuccess(result.result);
    result.result["batchResults"] = std::move(resultsArray);
    result.result["batchSize"] = static_cast<int>(g_offlineBatch.results.size());

    // Clear state for next batch
    g_offlineBatch.commands.clear();
    g_offlineBatch.results.clear();
    g_offlineBatch.complete = false;

    LogToWindowFormat("[OfflineBatch] Returned %zu results.", g_offlineBatch.results.size());

    return result;
}

static CommandRegistrar s_offlineBatchRegistrar({
    { "BeginOfflineBatch", HandleBeginOfflineBatch },
    { "GetOfflineBatchResults", HandleGetOfflineBatchResults },
});

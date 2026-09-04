#include "pch.h"

#include "XTaskQueueHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"

#include <chrono>
#include <string>
#include <thread>
#include "CommandRegistry.h"

namespace
{
    using CommandHandlerShared::CreateBaseResult;
    using CommandHandlerShared::MarkFailure;
    using CommandHandlerShared::MarkSuccess;
    using CommandHandlerShared::SetHResult;
    using CommandHandlerShared::TryParseBoolParameter;

    bool TryParseDispatchMode(const nlohmann::json& parameters, const char* key, XTaskQueueDispatchMode& mode, std::string& error)
    {
        if (!parameters.is_object())
        {
            error = "Parameters payload is not an object";
            return false;
        }

        auto it = parameters.find(key);
        if (it == parameters.end())
        {
            return true;
        }

        if (it->is_string())
        {
            const std::string lower = CommandHandlerShared::ToLowerCopy(it->get<std::string>());
            if (lower == "threadpool")
            {
                mode = XTaskQueueDispatchMode::ThreadPool;
                return true;
            }

            if (lower == "serializedthreadpool" || lower == "serialized")
            {
                mode = XTaskQueueDispatchMode::SerializedThreadPool;
                return true;
            }

            if (lower == "manual")
            {
                mode = XTaskQueueDispatchMode::Manual;
                return true;
            }

            error = std::string("Unsupported dispatch mode value for '") + key + "'";
            return false;
        }

        error = std::string("Parameter '") + key + "' must be a string";
        return false;
    }
}

CommandResultPayload HandleXTaskQueueCreate(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XTaskQueueDispatchMode workMode = XTaskQueueDispatchMode::ThreadPool;
            XTaskQueueDispatchMode completionMode = XTaskQueueDispatchMode::ThreadPool;
            bool setAsProcessQueue = true;
            std::string error;

            if (!TryParseDispatchMode(parameters, "workMode", workMode, error))
            {
                return E_INVALIDARG;
            }

            if (!TryParseDispatchMode(parameters, "completionMode", completionMode, error))
            {
                return E_INVALIDARG;
            }

            if (!TryParseBoolParameter(parameters, "setAsProcessQueue", setAsProcessQueue, error))
            {
                return E_INVALIDARG;
            }

            XTaskQueueHandle newQueue{ nullptr };
            const HRESULT hr = XTaskQueueCreate(workMode, completionMode, &newQueue);
            LogToWindowFormat("XTaskQueueCreate (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);

            if (setAsProcessQueue)
            {
                XTaskQueueSetCurrentProcessTaskQueue(newQueue);
            }

            if (state->taskQueue)
            {
                XTaskQueueTerminate(state->taskQueue, true, nullptr, nullptr);
                XTaskQueueCloseHandle(state->taskQueue);
            }

            state->taskQueue = newQueue;
            state->taskQueueOwnedByCommand = true;
            return S_OK;
        });
}

CommandResultPayload HandleXTaskQueueCloseHandle(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (state->taskQueue)
            {
                XTaskQueueTerminate(state->taskQueue, true, nullptr, nullptr);
                XTaskQueueCloseHandle(state->taskQueue);
                state->taskQueue = nullptr;
                state->taskQueueOwnedByCommand = false;
                LogToWindow("XTaskQueueCloseHandle executed");
            }
            else
            {
                LogToWindow("XTaskQueueCloseHandle skipped (no queue)");
            }
            return S_OK;
        });
}

CommandResultPayload HandleXTaskQueueDispatch(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId){
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (!state->taskQueue)
            {
                LogToWindow("XTaskQueueDispatch: no task queue");
                return E_UNEXPECTED;
            }

            // Pump a Manual port for a bounded window. This lets a scenario deliberately
            // drive a queue for a while and then stop ("park" it), which is what a title
            // doing its own dispatching looks like. Without this, work submitted to a
            // Manual port never runs at all.
            uint32_t durationMs = 2000;
            if (parameters.is_object())
            {
                auto it = parameters.find("durationMs");
                if (it != parameters.end())
                {
                    if (it->is_number_unsigned())
                    {
                        durationMs = it->get<uint32_t>();
                    }
                    else if (it->is_string())
                    {
                        try
                        {
                            durationMs = static_cast<uint32_t>(std::stoul(it->get<std::string>()));
                        }
                        catch (const std::exception&)
                        {
                            LogToWindow("XTaskQueueDispatch: invalid durationMs, using default");
                        }
                    }
                }
            }

            XTaskQueuePort port = XTaskQueuePort::Work;
            if (parameters.is_object())
            {
                auto it = parameters.find("port");
                if (it != parameters.end() && it->is_string())
                {
                    const std::string lower = CommandHandlerShared::ToLowerCopy(it->get<std::string>());
                    if (lower == "completion")
                    {
                        port = XTaskQueuePort::Completion;
                    }
                }
            }

            // Optional hard cap on how many callbacks to dispatch. This is the deterministic
            // way to start an async operation without letting it run to completion: a purely
            // time-based window is racy (a fast HTTP request can finish inside it, which
            // silently destroys the scenario being tested). 0 means "no cap".
            uint32_t maxCallbacks = 0;
            if (parameters.is_object())
            {
                auto it = parameters.find("maxCallbacks");
                if (it != parameters.end())
                {
                    if (it->is_number_unsigned())
                    {
                        maxCallbacks = it->get<uint32_t>();
                    }
                    else if (it->is_string())
                    {
                        try
                        {
                            maxCallbacks = static_cast<uint32_t>(std::stoul(it->get<std::string>()));
                        }
                        catch (const std::exception&)
                        {
                            LogToWindow("XTaskQueueDispatch: invalid maxCallbacks, ignoring");
                        }
                    }
                }
            }

            const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(durationMs);
            uint32_t dispatched = 0;
            while (std::chrono::steady_clock::now() < deadline)
            {
                if (maxCallbacks != 0 && dispatched >= maxCallbacks)
                {
                    break;
                }

                // 0 timeout: dispatch whatever is ready, then yield briefly so we don't spin.
                if (XTaskQueueDispatch(state->taskQueue, port, 0))
                {
                    ++dispatched;
                }
                else
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            }

            LogToWindowFormat("XTaskQueueDispatch: pumped %s port for up to %u ms (%u callback(s) dispatched, cap=%u)",
                (port == XTaskQueuePort::Work) ? "Work" : "Completion", durationMs, dispatched, maxCallbacks);
            return S_OK;
        });
}

CommandResultPayload HandleArmSuspendQueueTerminate(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            bool armed = true;
            std::string error;
            if (!TryParseBoolParameter(parameters, "armed", armed, error))
            {
                return E_INVALIDARG;
            }

            if (armed && !state->taskQueue)
            {
                LogToWindow("ArmSuspendQueueTerminate: no task queue to terminate");
                return E_UNEXPECTED;
            }

            state->terminateQueueOnSuspend = armed;
            LogToWindowFormat("ArmSuspendQueueTerminate: %s - suspend handler %s perform a blocking "
                "XTaskQueueTerminate(wait=true)",
                armed ? "ARMED" : "disarmed",
                armed ? "will" : "will not");
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XTaskQueueCloseHandle", HandleXTaskQueueCloseHandle },
    { "XTaskQueueCreate", HandleXTaskQueueCreate },
    { "XTaskQueueDispatch", HandleXTaskQueueDispatch },
    { "ArmSuspendQueueTerminate", HandleArmSuspendQueueTerminate }
});

#include "pch.h"

#include "XTaskQueueHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"

#include <chrono>
#include <string>

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
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    XTaskQueueDispatchMode workMode = XTaskQueueDispatchMode::ThreadPool;
    XTaskQueueDispatchMode completionMode = XTaskQueueDispatchMode::ThreadPool;
    bool setAsProcessQueue = true;
    std::string error;

    if (!TryParseDispatchMode(parameters, "workMode", workMode, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    if (!TryParseDispatchMode(parameters, "completionMode", completionMode, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    if (!TryParseBoolParameter(parameters, "setAsProcessQueue", setAsProcessQueue, error))
    {
        MarkFailure(payload.result, E_INVALIDARG, error);
        return payload;
    }

    const auto start = std::chrono::steady_clock::now();
    XTaskQueueHandle newQueue{ nullptr };
    const HRESULT hr = XTaskQueueCreate(workMode, completionMode, &newQueue);
    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);
    SetHResult(payload.result, hr);

    LogToWindowFormat("XTaskQueueCreate (hr=0x%08X)", static_cast<uint32_t>(hr));

    if (FAILED(hr))
    {
        MarkFailure(payload.result, hr, "XTaskQueueCreate failed");
        return payload;
    }

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

    MarkSuccess(payload.result);
    SetHResult(payload.result, hr);
    return payload;
}

CommandResultPayload HandleXTaskQueueCloseHandle(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    CommandResultPayload payload{};
    payload.result = CreateBaseResult(commandId, command, deviceId);

    const auto start = std::chrono::steady_clock::now();
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

    payload.elapsedMs = CommandHandlerShared::ComputeElapsedMs(start);
    MarkSuccess(payload.result);
    SetHResult(payload.result, S_OK);
    return payload;
}

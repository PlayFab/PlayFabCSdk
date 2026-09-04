#pragma once

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdint>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

#include <nlohmann/json.hpp>

#include "DeviceCommandHandlers.h"

namespace CommandHandlerShared
{
    inline int64_t ClampToIntRange(int64_t value)
    {
        if (value < 0)
        {
            return 0;
        }

        if (value > static_cast<int64_t>(std::numeric_limits<int>::max()))
        {
            return static_cast<int64_t>(std::numeric_limits<int>::max());
        }

        return value;
    }

    inline int ComputeElapsedMs(std::chrono::steady_clock::time_point start)
    {
        const auto elapsed = std::chrono::steady_clock::now() - start;
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        return static_cast<int>(ClampToIntRange(ms));
    }

    inline void SetHResult(nlohmann::json& result, HRESULT hr)
    {
        result["hresult"] = static_cast<int32_t>(hr);
    }

    inline nlohmann::json CreateBaseResult(
        const std::string& commandId,
        const std::string& command,
        const std::string& deviceId)
    {
        nlohmann::json result = nlohmann::json::object();
        result["type"] = "actionResult";
        result["commandId"] = commandId;
        result["command"] = command;
        result["deviceId"] = deviceId;
        return result;
    }

    inline void MarkSuccess(nlohmann::json& result)
    {
        result["status"] = "succeeded";
        result["hresult"] = 0;
    }

    inline void MarkFailure(nlohmann::json& result, HRESULT hr, std::string_view errorMessage)
    {
        result["status"] = "failed";
        result["hresult"] = hr;
        if (!errorMessage.empty())
        {
            result["errorMessage"] = errorMessage;
        }
    }

    inline std::string ToLowerCopy(const std::string& input)
    {
        std::string lower = input;
        std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char ch)
        {
            return static_cast<char>(std::tolower(ch));
        });
        return lower;
    }

    inline bool TryGetInt64Parameter(
        const nlohmann::json& parameters,
        const char* key,
        int64_t& value,
        std::string& error)
    {
        if (!parameters.is_object())
        {
            error = "Parameters payload is not an object";
            return false;
        }

        auto it = parameters.find(key);
        if (it == parameters.end())
        {
            error = std::string("Missing parameter '") + key + "'";
            return false;
        }

        try
        {
            if (it->is_number_integer())
            {
                value = it->get<int64_t>();
                return true;
            }

            if (it->is_number_unsigned())
            {
                value = static_cast<int64_t>(it->get<uint64_t>());
                return true;
            }

            if (it->is_string())
            {
                const std::string text = it->get<std::string>();
                size_t index = 0;
                const long long parsed = std::stoll(text, &index, 10);
                if (index == text.size())
                {
                    value = static_cast<int64_t>(parsed);
                    return true;
                }
            }
        }
        catch (...)
        {
            // fall through to error path below
        }

        error = std::string("Parameter '") + key + "' must be an integer";
        return false;
    }

    inline bool TryGetStringParameter(
        const nlohmann::json& parameters,
        const char* key,
        std::string& value,
        std::string& error)
    {
        if (!parameters.is_object())
        {
            error = "Parameters payload is not an object";
            return false;
        }

        auto it = parameters.find(key);
        if (it == parameters.end())
        {
            error = std::string("Missing parameter '") + key + "'";
            return false;
        }

        if (!it->is_string())
        {
            error = std::string("Parameter '") + key + "' must be a string";
            return false;
        }

        value = it->get<std::string>();
        return true;
    }

    inline bool TryParseBoolParameter(
        const nlohmann::json& parameters,
        const char* key,
        bool& value,
        std::string& error)
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

        if (it->is_boolean())
        {
            value = it->get<bool>();
            return true;
        }

        if (it->is_string())
        {
            const std::string lower = ToLowerCopy(it->get<std::string>());
            if (lower == "true")
            {
                value = true;
                return true;
            }

            if (lower == "false")
            {
                value = false;
                return true;
            }

            error = std::string("Parameter '") + key + "' must be 'true' or 'false'";
            return false;
        }

        error = std::string("Parameter '") + key + "' must be boolean or string";
        return false;
    }

    inline std::vector<uint64_t> GetUint64Array(const nlohmann::json& parameters, const char* key)
    {
        std::vector<uint64_t> result;
        if (!parameters.is_object() || !parameters.contains(key) || !parameters[key].is_array())
        {
            return result;
        }
        for (const auto& item : parameters[key])
        {
            if (item.is_number_unsigned())
            {
                result.push_back(item.get<uint64_t>());
            }
            else if (item.is_number_integer())
            {
                result.push_back(static_cast<uint64_t>(item.get<int64_t>()));
            }
            else if (item.is_string())
            {
                try { result.push_back(std::stoull(item.get<std::string>())); }
                catch (...) { /* skip invalid */ }
            }
        }
        return result;
    }

    // Wrapper for synchronous command handlers.
    // The lambda receives (payload) and returns HRESULT.
    // Payload init, timing, and success/failure marking are handled automatically.
    template<typename Fn>
    inline CommandResultPayload SyncCall(
        const std::string& commandId,
        const std::string& command,
        const std::string& deviceId,
        Fn&& fn)
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        const auto start = std::chrono::steady_clock::now();
        const HRESULT hr = fn(payload);
        payload.elapsedMs = ComputeElapsedMs(start);
        SetHResult(payload.result, hr);
        if (FAILED(hr))
        {
            MarkFailure(payload.result, hr, command + " failed");
        }
        else
        {
            MarkSuccess(payload.result);
        }
        return payload;
    }

    // Wrapper for asynchronous command handlers that use XAsyncBlock.
    // The lambda receives (async, payload) and should call the async API with the
    // provided XAsyncBlock, returning the HRESULT from the initial call.
    // XAsyncGetStatus is called automatically after the lambda returns.
    template<typename Fn>
    inline CommandResultPayload AsyncCall(
        XTaskQueueHandle queue,
        const std::string& commandId,
        const std::string& command,
        const std::string& deviceId,
        Fn&& fn)
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        XAsyncBlock async{};
        async.queue = queue;
        const auto start = std::chrono::steady_clock::now();
        HRESULT hr = fn(async, payload);
        if (SUCCEEDED(hr))
        {
            hr = XAsyncGetStatus(&async, true);
        }
        payload.elapsedMs = ComputeElapsedMs(start);
        SetHResult(payload.result, hr);
        if (FAILED(hr))
        {
            MarkFailure(payload.result, hr, command + " failed");
        }
        else
        {
            MarkSuccess(payload.result);
        }
        return payload;
    }

    template<typename StartFn, typename ResultFn>
    inline CommandResultPayload AsyncCallWithResult(
        XTaskQueueHandle queue,
        const std::string& commandId,
        const std::string& command,
        const std::string& deviceId,
        StartFn&& startFn,
        ResultFn&& resultFn)
    {
        CommandResultPayload payload{};
        payload.result = CreateBaseResult(commandId, command, deviceId);
        XAsyncBlock async{};
        async.queue = queue;
        const auto start = std::chrono::steady_clock::now();
        HRESULT hr = startFn(async, payload);
        if (SUCCEEDED(hr))
        {
            hr = XAsyncGetStatus(&async, true);
        }
        if (SUCCEEDED(hr))
        {
            hr = resultFn(async, payload);
        }
        payload.elapsedMs = ComputeElapsedMs(start);
        SetHResult(payload.result, hr);
        if (FAILED(hr))
        {
            MarkFailure(payload.result, hr, command + " failed");
        }
        else
        {
            MarkSuccess(payload.result);
        }
        return payload;
    }
}

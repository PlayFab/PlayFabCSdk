#include "pch.h"

#include "HCAsyncHandlers.h"

#include "CommandHandlerShared.h"
#include "CommandRegistry.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"

#include <XAsync.h>
#include <XAsyncProvider.h>

namespace
{
    HRESULT CALLBACK NoOpWorkCallback(XAsyncBlock* async) noexcept
    {
        UNREFERENCED_PARAMETER(async);
        return S_OK;
    }

    HRESULT CALLBACK NoOpProvider(XAsyncOp op, const XAsyncProviderData* data) noexcept
    {
        UNREFERENCED_PARAMETER(op);
        UNREFERENCED_PARAMETER(data);
        return S_OK;
    }

    HRESULT CALLBACK DoWorkCompletingProvider(XAsyncOp op, const XAsyncProviderData* data) noexcept
    {
        if (op == XAsyncOp::DoWork)
        {
            XAsyncComplete(data->async, S_OK, 0);
        }
        return S_OK;
    }
}

CommandResultPayload HandleXAsyncGetStatus(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XAsyncBlock async{};
            async.queue = state->taskQueue;

            HRESULT hr = XAsyncRun(&async, NoOpWorkCallback);
            LogToWindowFormat("XAsyncRun for GetStatus (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);

            hr = XAsyncGetStatus(&async, true);
            LogToWindowFormat("XAsyncGetStatus (hr=0x%08X)", static_cast<uint32_t>(hr));
            payload.result["status"] = static_cast<int32_t>(hr);
            return S_OK;
        });
}

CommandResultPayload HandleXAsyncGetResultSize(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            XAsyncBlock async{};
            async.queue = state->taskQueue;

            HRESULT hr = XAsyncRun(&async, NoOpWorkCallback);
            LogToWindowFormat("XAsyncRun for GetResultSize (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);

            hr = XAsyncGetStatus(&async, true);
            RETURN_IF_FAILED(hr);

            size_t size = 0;
            hr = XAsyncGetResultSize(&async, &size);
            LogToWindowFormat("XAsyncGetResultSize (size=%zu, hr=0x%08X)", size, static_cast<uint32_t>(hr));
            payload.result["resultSize"] = size;
            return hr;
        });
}

CommandResultPayload HandleXAsyncCancel(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XAsyncBlock async{};
            XAsyncCancel(&async);
            LogToWindow("XAsyncCancel executed");
            return S_OK;
        });
}

CommandResultPayload HandleXAsyncRun(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XAsyncBlock async{};
            async.queue = state->taskQueue;

            const HRESULT hr = XAsyncRun(&async, NoOpWorkCallback);
            LogToWindowFormat("XAsyncRun (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);

            const HRESULT waitHr = XAsyncGetStatus(&async, true);
            LogToWindowFormat("XAsyncRun wait complete (hr=0x%08X)", static_cast<uint32_t>(waitHr));
            return waitHr;
        });
}

CommandResultPayload HandleXAsyncBegin(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XAsyncBlock async{};
            async.queue = state->taskQueue;

            const HRESULT hr = XAsyncBegin(&async, nullptr, nullptr, nullptr, NoOpProvider);
            LogToWindowFormat("XAsyncBegin (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);

            XAsyncComplete(&async, S_OK, 0);
            LogToWindow("XAsyncBegin cleanup complete");
            return S_OK;
        });
}

CommandResultPayload HandleXAsyncComplete(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XAsyncBlock async{};
            async.queue = state->taskQueue;

            const HRESULT hr = XAsyncBegin(&async, nullptr, nullptr, nullptr, NoOpProvider);
            LogToWindowFormat("XAsyncBegin for Complete (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);

            XAsyncComplete(&async, S_OK, 0);
            LogToWindow("XAsyncComplete executed");
            return S_OK;
        });
}

CommandResultPayload HandleXAsyncGetResult(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            XAsyncBlock async{};
            async.queue = state->taskQueue;

            HRESULT hr = XAsyncRun(&async, NoOpWorkCallback);
            LogToWindowFormat("XAsyncRun for GetResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);

            hr = XAsyncGetStatus(&async, true);
            RETURN_IF_FAILED(hr);

            hr = XAsyncGetResult(&async, nullptr, 0, nullptr, nullptr);
            LogToWindowFormat("XAsyncGetResult (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleXAsyncSchedule(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            uint32_t delayInMs = 0;
            if (parameters.is_object() && parameters.contains("delayInMs"))
            {
                int64_t val = 0;
                std::string error;
                if (CommandHandlerShared::TryGetInt64Parameter(parameters, "delayInMs", val, error))
                {
                    delayInMs = static_cast<uint32_t>(CommandHandlerShared::ClampToIntRange(val));
                }
            }

            XAsyncBlock async{};
            async.queue = state->taskQueue;

            HRESULT hr = XAsyncBegin(&async, nullptr, nullptr, nullptr, DoWorkCompletingProvider);
            LogToWindowFormat("XAsyncBegin for Schedule (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);

            hr = XAsyncSchedule(&async, delayInMs);
            LogToWindowFormat("XAsyncSchedule (delayInMs=%u, hr=0x%08X)", delayInMs, static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);

            hr = XAsyncGetStatus(&async, true);
            LogToWindowFormat("XAsyncSchedule wait complete (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "XAsyncBegin", HandleXAsyncBegin },
    { "XAsyncCancel", HandleXAsyncCancel },
    { "XAsyncComplete", HandleXAsyncComplete },
    { "XAsyncGetResult", HandleXAsyncGetResult },
    { "XAsyncGetResultSize", HandleXAsyncGetResultSize },
    { "XAsyncGetStatus", HandleXAsyncGetStatus },
    { "XAsyncRun", HandleXAsyncRun },
    { "XAsyncSchedule", HandleXAsyncSchedule }
});
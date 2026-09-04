#include "pch.h"

#include "HCHttpCallHandlers.h"

#include "CommandHandlerShared.h"
#include "DeviceGameSaveState.h"
#include "DeviceLogging.h"

#include <httpClient/httpClient.h>
#include <httpClient/httpProvider.h>
#include "CommandRegistry.h"

#include <chrono>
#include <memory>
#include <thread>
#include <atomic>
#include <vector>

// ============================================================================
// Concurrency burst
// ============================================================================
//
// Exists because the rest of this file models a single in-flight call (state->hcCall), which
// cannot exercise the global concurrent-request limit. The limit only queues work, it never fails
// a request, so the observable contract is: every request completes successfully no matter how far
// the burst exceeds the cap, and none are lost or left hanging.
//
// Three commands share this machinery:
//   TestHCHttpCallPerformBurst      - fire N and wait (the common case)
//   TestHCHttpCallPerformBurstStart - fire N and return immediately
//   TestHCHttpCallPerformBurstWait  - await a burst started earlier
//
// The Start/Wait split exists for one specific gap: requests that are still QUEUED when a PLM
// suspend arrives are abandoned by CloseAllConnections with E_ABORT, which is a different code
// path from the in-flight teardown. Reaching it requires being mid-burst at the suspend boundary,
// which the blocking form cannot do because it waits for everything before returning.

namespace
{
    using HttpBurstState = DeviceGameSaveState::HttpBurstState;
    using HttpBurstCallContext = DeviceGameSaveState::HttpBurstCallContext;
    using PendingHttpBurst = DeviceGameSaveState::PendingHttpBurst;

    // Fires requestCount requests at url without waiting. Ownership of every context is returned to
    // the caller; the completion callback never frees its own, so a caller that later times out can
    // still safely inspect and cancel the XAsyncBlock of a request that never completed.
    std::unique_ptr<PendingHttpBurst> StartHttpBurst(
        DeviceGameSaveState* state,
        const std::string& url,
        int64_t requestCount)
    {
        auto pending = std::make_unique<PendingHttpBurst>();
        pending->burst = std::make_shared<HttpBurstState>();
        pending->contexts.reserve(static_cast<size_t>(requestCount));

        for (int64_t i = 0; i < requestCount; i++)
        {
            HCCallHandle call{ nullptr };
            HRESULT hr = HCHttpCallCreate(&call);
            if (FAILED(hr))
            {
                LogToWindowFormat("TestHCHttpCallPerformBurst: create failed (hr=0x%08X)", static_cast<uint32_t>(hr));
                break;
            }

            // Retries would make the completion count non-deterministic under load.
            (void)HCHttpCallRequestSetRetryAllowed(call, false);
            hr = HCHttpCallRequestSetUrl(call, "GET", url.c_str());
            if (FAILED(hr))
            {
                HCHttpCallCloseHandle(call);
                break;
            }

            auto context = std::make_unique<HttpBurstCallContext>();
            context->burst = pending->burst;
            context->call = call;
            context->async = {};
            context->async.queue = state->taskQueue;
            context->async.context = context.get();
            context->async.callback = [](XAsyncBlock* async)
            {
                auto ctx = static_cast<HttpBurstCallContext*>(async->context);
                const HRESULT status = XAsyncGetStatus(async, false);
                if (SUCCEEDED(status))
                {
                    ctx->burst->succeeded++;
                }
                else
                {
                    ctx->burst->failed++;
                    uint32_t expected = 0;
                    ctx->burst->firstFailure.compare_exchange_strong(expected, static_cast<uint32_t>(status));
                }

                // Read before closing the handle. libHttpClient surfaces transport outcomes here
                // rather than through the XAsync status, so this is the only place an abandoned or
                // cancelled request is distinguishable from one that genuinely completed.
                HRESULT networkErrorCode = S_OK;
                uint32_t platformNetworkErrorCode = 0;
                if (SUCCEEDED(HCHttpCallResponseGetNetworkErrorCode(ctx->call, &networkErrorCode, &platformNetworkErrorCode)) &&
                    FAILED(networkErrorCode))
                {
                    ctx->burst->networkErrors++;
                    if (networkErrorCode == E_ABORT)
                    {
                        ctx->burst->aborted++;
                    }
                }

                HCHttpCallCloseHandle(ctx->call);
                // Mark before decrementing: callers treat outstanding==0 as "every context is done
                // being touched", so the flag must already be visible by then.
                ctx->completed.store(true, std::memory_order_release);
                ctx->burst->outstanding--;
            };

            pending->burst->outstanding++;
            hr = HCHttpCallPerformAsync(call, &context->async);
            if (FAILED(hr))
            {
                // The callback will not run, so unwind this one here.
                LogToWindowFormat("TestHCHttpCallPerformBurst: perform failed (hr=0x%08X)", static_cast<uint32_t>(hr));
                pending->burst->outstanding--;
                pending->burst->failed++;
                uint32_t expected = 0;
                pending->burst->firstFailure.compare_exchange_strong(expected, static_cast<uint32_t>(hr));
                HCHttpCallCloseHandle(call);
                continue;
            }

            pending->contexts.push_back(std::move(context));
        }

        return pending;
    }

    // Waits for a burst to drain, then reports and validates the tallies. Consumes `pending`.
    HRESULT AwaitHttpBurst(
        std::unique_ptr<PendingHttpBurst> pending,
        int64_t timeoutMs,
        bool hasExpectedSuccesses,
        int64_t expectedSuccesses,
        bool hasExpectedAbortedAtLeast,
        int64_t expectedAbortedAtLeast,
        CommandResultPayload& payload)
    {
        auto& burst = pending->burst;

        const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeoutMs);
        while (burst->outstanding.load() > 0 && std::chrono::steady_clock::now() < deadline)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        bool abandonedContexts = false;
        if (burst->outstanding.load() > 0)
        {
            // Returning now would leave callbacks queued on the task queue holding pointers into
            // contexts about to be destroyed, and the scenario's next steps are cleanup and
            // XTaskQueueCloseHandle. A crash there would bury the timeout that is the real finding,
            // so cancel the stragglers and give them a bounded window to run.
            LogToWindow("TestHCHttpCallPerformBurst: timed out, cancelling outstanding requests");
            for (auto& context : pending->contexts)
            {
                if (!context->completed.load(std::memory_order_acquire))
                {
                    XAsyncCancel(&context->async);
                }
            }

            const auto drainDeadline = std::chrono::steady_clock::now() + std::chrono::seconds(5);
            while (burst->outstanding.load() > 0 && std::chrono::steady_clock::now() < drainDeadline)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }

            if (burst->outstanding.load() > 0)
            {
                // Cancellation did not land. Freeing the contexts would hand the pending callbacks
                // dangling pointers, so leak them deliberately: this handler is already reporting
                // failure, and a leak in a failing test run is strictly better than a
                // use-after-free that masks the cause.
                abandonedContexts = true;
                for (auto& context : pending->contexts)
                {
                    (void)context.release();
                }
            }
        }

        const int32_t stillOutstanding = burst->outstanding.load();
        const int32_t succeeded = burst->succeeded.load();
        const int32_t failed = burst->failed.load();
        const int32_t aborted = burst->aborted.load();
        const int32_t networkErrors = burst->networkErrors.load();

        payload.result["succeeded"] = succeeded;
        payload.result["failed"] = failed;
        payload.result["outstanding"] = stillOutstanding;
        payload.result["aborted"] = aborted;
        payload.result["networkErrors"] = networkErrors;
        payload.result["firstFailureHr"] = burst->firstFailure.load();

        LogToWindowFormat("TestHCHttpCallPerformBurst (succeeded=%d failed=%d outstanding=%d aborted=%d networkErrors=%d)",
            succeeded, failed, stillOutstanding, aborted, networkErrors);

        if (stillOutstanding > 0)
        {
            // Queued requests were never started, or completions were lost. This is the failure
            // the concurrency cap must never cause.
            LogToWindowFormat("TestHCHttpCallPerformBurst: timed out with %d request(s) still outstanding%s",
                stillOutstanding, abandonedContexts ? " (contexts intentionally leaked)" : "");
            return E_ABORT;
        }

        if (hasExpectedSuccesses && succeeded != static_cast<int32_t>(expectedSuccesses))
        {
            LogToWindowFormat("TestHCHttpCallPerformBurst: expected %lld successes, got %d",
                expectedSuccesses, succeeded);
            return E_FAIL;
        }

        // Proves the requests took the path the scenario intended rather than simply completing.
        // Without this, "everything resolved" is equally true of a run where the code under test
        // never executed at all.
        if (hasExpectedAbortedAtLeast && aborted < static_cast<int32_t>(expectedAbortedAtLeast))
        {
            LogToWindowFormat("TestHCHttpCallPerformBurst: expected at least %lld request(s) aborted with E_ABORT, got %d",
                expectedAbortedAtLeast, aborted);
            return E_FAIL;
        }

        return S_OK;
    }

    // Shared parameter parsing for the burst commands.
    bool TryGetBurstUrlAndCount(
        const nlohmann::json& parameters,
        std::string& url,
        int64_t& requestCount)
    {
        std::string error;
        if (!CommandHandlerShared::TryGetStringParameter(parameters, "url", url, error))
        {
            return false;
        }

        if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "requestCount", requestCount, error) ||
            requestCount <= 0 || requestCount > 256)
        {
            LogToWindow("TestHCHttpCallPerformBurst: requestCount must be between 1 and 256");
            return false;
        }

        return true;
    }
}

// Fires N HTTP requests concurrently against one URL and waits for all of them.
CommandResultPayload HandleTestHCHttpCallPerformBurst(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            std::string url;
            int64_t requestCount = 0;
            if (!TryGetBurstUrlAndCount(parameters, url, requestCount))
            {
                return E_INVALIDARG;
            }

            std::string error;
            int64_t timeoutMs = 60000;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "timeoutMs", timeoutMs, error);

            int64_t expectedSuccesses = requestCount;
            (void)CommandHandlerShared::TryGetInt64Parameter(parameters, "expectedSuccesses", expectedSuccesses, error);

            payload.result["requested"] = requestCount;
            auto pending = StartHttpBurst(state, url, requestCount);
            return AwaitHttpBurst(std::move(pending), timeoutMs, true, expectedSuccesses, false, 0, payload);
        });
}

// Fires N HTTP requests and returns immediately, leaving them in flight on the device state.
//
// This is what makes the queued-request suspend path reachable: with a cap far below the burst
// size, most of these requests are still sitting in the provider's pending queue - never started -
// when the scenario suspends. Those are abandoned via E_ABORT by CloseAllConnections, which is
// separate code from the in-flight teardown and is otherwise untested.
CommandResultPayload HandleTestHCHttpCallPerformBurstStart(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            if (state->hcPendingBurst)
            {
                LogToWindow("TestHCHttpCallPerformBurstStart: a burst is already pending; call Wait first");
                return E_UNEXPECTED;
            }

            std::string url;
            int64_t requestCount = 0;
            if (!TryGetBurstUrlAndCount(parameters, url, requestCount))
            {
                return E_INVALIDARG;
            }

            state->hcPendingBurst = StartHttpBurst(state, url, requestCount);

            const int32_t outstanding = state->hcPendingBurst->burst->outstanding.load();
            payload.result["requested"] = requestCount;
            payload.result["outstanding"] = outstanding;
            LogToWindowFormat("TestHCHttpCallPerformBurstStart (requested=%lld outstanding=%d)",
                requestCount, outstanding);

            // Every request failing to even start would make the scenario vacuous rather than
            // failing it later, so surface that here.
            if (outstanding == 0)
            {
                LogToWindow("TestHCHttpCallPerformBurstStart: no requests are in flight");
                return E_FAIL;
            }

            return S_OK;
        });
}

// Awaits a burst started by TestHCHttpCallPerformBurstStart.
//
// expectedSuccesses is optional here. After a suspend some requests are legitimately cancelled, so
// scenarios usually assert only that everything RESOLVED - no request may be left hanging - rather
// than pinning an exact success count that depends on timing.
CommandResultPayload HandleTestHCHttpCallPerformBurstWait(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            if (!state->hcPendingBurst)
            {
                LogToWindow("TestHCHttpCallPerformBurstWait: no pending burst; call Start first");
                return E_UNEXPECTED;
            }

            std::string error;
            int64_t timeoutMs = 60000;
            CommandHandlerShared::TryGetInt64Parameter(parameters, "timeoutMs", timeoutMs, error);

            int64_t expectedSuccesses = 0;
            const bool hasExpectedSuccesses =
                CommandHandlerShared::TryGetInt64Parameter(parameters, "expectedSuccesses", expectedSuccesses, error) &&
                parameters.contains("expectedSuccesses");

            int64_t expectedAbortedAtLeast = 0;
            const bool hasExpectedAbortedAtLeast =
                CommandHandlerShared::TryGetInt64Parameter(parameters, "expectedAbortedAtLeast", expectedAbortedAtLeast, error) &&
                parameters.contains("expectedAbortedAtLeast");

            auto pending = std::move(state->hcPendingBurst);
            return AwaitHttpBurst(std::move(pending), timeoutMs, hasExpectedSuccesses, expectedSuccesses,
                hasExpectedAbortedAtLeast, expectedAbortedAtLeast, payload);
        });
}

// ============================================================================
// Create / Lifecycle
// ============================================================================

CommandResultPayload HandleHCHttpCallCreate(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HCCallHandle call = nullptr;
            const HRESULT hr = HCHttpCallCreate(&call);
            LogToWindowFormat("HCHttpCallCreate (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            state->hcCall = call;
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallPerformAsync(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::AsyncCall(state->taskQueue, commandId, command, deviceId,
        [&](XAsyncBlock& async, CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = HCHttpCallPerformAsync(state->hcCall, &async);
            LogToWindowFormat("HCHttpCallPerformAsync (hr=0x%08X)", static_cast<uint32_t>(hr));
            return hr;
        });
}

CommandResultPayload HandleHCHttpCallPerformStart(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            if (state->hcPendingPerform)
            {
                LogToWindow("HCHttpCallPerformStart: a perform is already in flight");
                return E_UNEXPECTED;
            }

            auto async = std::make_unique<XAsyncBlock>();
            async->queue = state->taskQueue;

            const HRESULT hr = HCHttpCallPerformAsync(state->hcCall, async.get());
            LogToWindowFormat("HCHttpCallPerformStart (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);

            // Deliberately do NOT wait here: the request stays in flight so later commands
            // (queue parking, PLM suspend) can run while it is outstanding.
            state->hcPendingPerform = std::move(async);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallPerformAssertPending(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            if (!state->hcPendingPerform)
            {
                LogToWindow("HCHttpCallPerformAssertPending: no perform in flight");
                return E_UNEXPECTED;
            }

            // Non-destructive on purpose. HCHttpCallPerformWait cannot be used as a "still
            // pending?" probe because on timeout it releases ownership of the async block, which
            // would leave a later HCHttpCallPerformWait with nothing to wait on.
            const HRESULT hr = XAsyncGetStatus(state->hcPendingPerform.get(), false);
            const bool stillPending = (hr == E_PENDING);
            payload.result["stillPending"] = stillPending;

            LogToWindowFormat("HCHttpCallPerformAssertPending: hr=0x%08X stillPending=%d",
                static_cast<uint32_t>(hr), stillPending ? 1 : 0);

            if (!stillPending)
            {
                // The request already finished, so whatever this scenario intended to observe
                // while it was outstanding cannot happen. Fail loudly rather than let the
                // scenario continue and report a pass that proves nothing.
                LogToWindow("HCHttpCallPerformAssertPending: request already completed - the scenario's precondition does not hold");
                return E_FAIL;
            }

            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallPerformWait(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            if (!state->hcPendingPerform)
            {
                LogToWindow("HCHttpCallPerformWait: no perform in flight");
                return E_UNEXPECTED;
            }

            uint32_t timeoutMs = 30000;
            if (parameters.is_object())
            {
                auto it = parameters.find("timeoutMs");
                if (it != parameters.end())
                {
                    // The controller serializes scenario parameters as strings, so a YAML
                    // `timeoutMs: 45000` arrives as "45000". Accept both forms - previously
                    // only the numeric form was honored and every caller silently fell back
                    // to the 30000 default.
                    if (it->is_number_unsigned())
                    {
                        timeoutMs = it->get<uint32_t>();
                    }
                    else if (it->is_string())
                    {
                        try
                        {
                            timeoutMs = static_cast<uint32_t>(std::stoul(it->get<std::string>()));
                        }
                        catch (const std::exception&)
                        {
                            LogToWindow("HCHttpCallPerformWait: invalid timeoutMs, using default");
                        }
                    }
                }
            }
            LogToWindowFormat("HCHttpCallPerformWait: waiting up to %u ms", timeoutMs);

            // Poll rather than block indefinitely so a request that never drains reports a
            // clean timeout instead of hanging the harness.
            const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeoutMs);
            HRESULT hr = E_PENDING;
            for (;;)
            {
                hr = XAsyncGetStatus(state->hcPendingPerform.get(), false);
                if (hr != E_PENDING)
                {
                    break;
                }
                if (std::chrono::steady_clock::now() >= deadline)
                {
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            payload.result["performCompleted"] = (hr != E_PENDING);
            LogToWindowFormat("HCHttpCallPerformWait (hr=0x%08X, timeoutMs=%u)", static_cast<uint32_t>(hr), timeoutMs);

            if (hr == E_PENDING)
            {
                // The in-flight request never progressed within the timeout. Leave the async block
                // allocated (it is still owned by libHttpClient) and fail the command.
                state->hcPendingPerform.release();
                LogToWindow("HCHttpCallPerformWait: TIMED OUT - request did not drain");
                return HRESULT_FROM_WIN32(ERROR_TIMEOUT);
            }

            state->hcPendingPerform.reset();

            // Some scenarios only care that the request *drained* (completed or was cancelled),
            // not that it succeeded -- e.g. a suspend test that deliberately points at an
            // unreachable host so a curl handle is guaranteed to still be active at the suspend
            // boundary. There, "the transfer finished one way or another" is the assertion and a
            // transport error is the expected result.
            bool allowFailedResult = false;
            std::string parseError;
            if (CommandHandlerShared::TryParseBoolParameter(parameters, "allowFailedResult", allowFailedResult, parseError)
                && allowFailedResult && FAILED(hr))
            {
                payload.result["performResult"] = static_cast<int64_t>(static_cast<uint32_t>(hr));
                LogToWindowFormat("HCHttpCallPerformWait: request drained with hr=0x%08X (allowFailedResult)",
                    static_cast<uint32_t>(hr));
                return S_OK;
            }

            return hr;
        });
}

CommandResultPayload HandleHCHttpCallDuplicateHandle(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HCCallHandle duplicatedHandle = HCHttpCallDuplicateHandle(state->hcCall);
            LogToWindowFormat("HCHttpCallDuplicateHandle (handle=%p)", duplicatedHandle);
            state->hcCall = duplicatedHandle;
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallCloseHandle(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HCHttpCallCloseHandle(state->hcCall);
            LogToWindow("HCHttpCallCloseHandle");
            state->hcCall = nullptr;
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallGetId(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const uint64_t callId = HCHttpCallGetId(state->hcCall);
            LogToWindowFormat("HCHttpCallGetId (id=%llu)", callId);
            payload.result["callId"] = callId;
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallSetTracing(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            bool traceCall = true;
            std::string error;
            if (!CommandHandlerShared::TryParseBoolParameter(parameters, "traceCall", traceCall, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallSetTracing(state->hcCall, traceCall);
            LogToWindowFormat("HCHttpCallSetTracing (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallGetRequestUrl(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const char* url = nullptr;
            const HRESULT hr = HCHttpCallGetRequestUrl(state->hcCall, &url);
            LogToWindowFormat("HCHttpCallGetRequestUrl (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["url"] = url ? url : "";
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallGetPerformCount(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t count = 0;
            const HRESULT hr = HCHttpCallGetPerformCount(state->hcCall, &count);
            LogToWindowFormat("HCHttpCallGetPerformCount (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["performCount"] = count;
            return S_OK;
        });
}

// ============================================================================
// Request Set APIs
// ============================================================================

CommandResultPayload HandleHCHttpCallRequestSetUrl(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string method;
            std::string url;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "method", method, error))
            {
                return E_INVALIDARG;
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "url", url, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallRequestSetUrl(state->hcCall, method.c_str(), url.c_str());
            LogToWindowFormat("HCHttpCallRequestSetUrl (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestSetDynamicSize(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            int64_t dynamicBodySize = 0;
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "dynamicBodySize", dynamicBodySize, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallRequestSetDynamicSize(state->hcCall, static_cast<uint64_t>(dynamicBodySize));
            LogToWindowFormat("HCHttpCallRequestSetDynamicSize (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestAddDynamicBytesWritten(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            int64_t bytesWritten = 0;
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "bytesWritten", bytesWritten, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallRequestAddDynamicBytesWritten(state->hcCall, static_cast<uint64_t>(bytesWritten));
            LogToWindowFormat("HCHttpCallRequestAddDynamicBytesWritten (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestSetRequestBodyBytes(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string body;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "body", body, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallRequestSetRequestBodyBytes(state->hcCall, (uint8_t*)body.c_str(), (uint32_t)body.size());
            LogToWindowFormat("HCHttpCallRequestSetRequestBodyBytes (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestSetRequestBodyString(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string body;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "body", body, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallRequestSetRequestBodyString(state->hcCall, body.c_str());
            LogToWindowFormat("HCHttpCallRequestSetRequestBodyString (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestEnableGzipCompression(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string level;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "level", level, error))
            {
                level = "Medium";
            }

            HCCompressionLevel compressionLevel = HCCompressionLevel::Medium;
            std::string lowerLevel = CommandHandlerShared::ToLowerCopy(level);
            if (lowerLevel == "none")
            {
                compressionLevel = HCCompressionLevel::None;
            }
            else if (lowerLevel == "low")
            {
                compressionLevel = HCCompressionLevel::Low;
            }
            else if (lowerLevel == "medium")
            {
                compressionLevel = HCCompressionLevel::Medium;
            }
            else if (lowerLevel == "high")
            {
                compressionLevel = HCCompressionLevel::High;
            }

            const HRESULT hr = HCHttpCallRequestEnableGzipCompression(state->hcCall, compressionLevel);
            LogToWindowFormat("HCHttpCallRequestEnableGzipCompression (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestSetRequestBodyReadFunction(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = HCHttpCallRequestSetRequestBodyReadFunction(state->hcCall, nullptr, 0, nullptr);
            LogToWindowFormat("HCHttpCallRequestSetRequestBodyReadFunction (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestSetProgressReportFunction(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = HCHttpCallRequestSetProgressReportFunction(state->hcCall, nullptr, true, 0, nullptr);
            LogToWindowFormat("HCHttpCallRequestSetProgressReportFunction (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestSetHeader(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string headerName;
            std::string headerValue;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "headerName", headerName, error))
            {
                return E_INVALIDARG;
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "headerValue", headerValue, error))
            {
                return E_INVALIDARG;
            }

            bool allowTracing = true;
            if (!CommandHandlerShared::TryParseBoolParameter(parameters, "allowTracing", allowTracing, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallRequestSetHeader(state->hcCall, headerName.c_str(), headerValue.c_str(), allowTracing);
            LogToWindowFormat("HCHttpCallRequestSetHeader (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestSetRetryAllowed(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            bool retryAllowed = true;
            std::string error;
            if (!CommandHandlerShared::TryParseBoolParameter(parameters, "retryAllowed", retryAllowed, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallRequestSetRetryAllowed(state->hcCall, retryAllowed);
            LogToWindowFormat("HCHttpCallRequestSetRetryAllowed (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestSetRetryCacheId(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            int64_t retryAfterCacheId = 0;
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "retryAfterCacheId", retryAfterCacheId, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallRequestSetRetryCacheId(state->hcCall, static_cast<uint32_t>(retryAfterCacheId));
            LogToWindowFormat("HCHttpCallRequestSetRetryCacheId (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestSetTimeout(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            int64_t timeoutInSeconds = 0;
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "timeoutInSeconds", timeoutInSeconds, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallRequestSetTimeout(state->hcCall, static_cast<uint32_t>(timeoutInSeconds));
            LogToWindowFormat("HCHttpCallRequestSetTimeout (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestSetRetryDelay(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            int64_t retryDelayInSeconds = 0;
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "retryDelayInSeconds", retryDelayInSeconds, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallRequestSetRetryDelay(state->hcCall, static_cast<uint32_t>(retryDelayInSeconds));
            LogToWindowFormat("HCHttpCallRequestSetRetryDelay (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestSetTimeoutWindow(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            int64_t timeoutWindowInSeconds = 0;
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "timeoutWindowInSeconds", timeoutWindowInSeconds, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallRequestSetTimeoutWindow(state->hcCall, static_cast<uint32_t>(timeoutWindowInSeconds));
            LogToWindowFormat("HCHttpCallRequestSetTimeoutWindow (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestSetMaxReceiveBufferSize(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            int64_t bufferSizeInBytes = 0;
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "bufferSizeInBytes", bufferSizeInBytes, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallRequestSetMaxReceiveBufferSize(state->hcCall, static_cast<size_t>(bufferSizeInBytes));
            LogToWindowFormat("HCHttpCallRequestSetMaxReceiveBufferSize (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestSetSSLValidation(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
#if HC_PLATFORM_IS_MICROSOFT && (HC_PLATFORM != HC_PLATFORM_UWP) && (HC_PLATFORM != HC_PLATFORM_XDK)
            bool sslValidation = true;
            std::string error;
            if (!CommandHandlerShared::TryParseBoolParameter(parameters, "sslValidation", sslValidation, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallRequestSetSSLValidation(state->hcCall, sslValidation);
            LogToWindowFormat("HCHttpCallRequestSetSSLValidation (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
#else
            LogToWindow("HCHttpCallRequestSetSSLValidation not supported on this platform");
#endif
            return S_OK;
        });
}

// ============================================================================
// Request Get APIs
// ============================================================================

CommandResultPayload HandleHCHttpCallRequestGetUrl(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const char* method = nullptr;
            const char* url = nullptr;
            const HRESULT hr = HCHttpCallRequestGetUrl(state->hcCall, &method, &url);
            LogToWindowFormat("HCHttpCallRequestGetUrl (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["method"] = method ? method : "";
            payload.result["url"] = url ? url : "";
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestGetRequestBodyBytes(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const uint8_t* body = nullptr;
            uint32_t bodySize = 0;
            const HRESULT hr = HCHttpCallRequestGetRequestBodyBytes(state->hcCall, &body, &bodySize);
            LogToWindowFormat("HCHttpCallRequestGetRequestBodyBytes (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["bodySize"] = bodySize;
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestGetRequestBodyString(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const char* body = nullptr;
            const HRESULT hr = HCHttpCallRequestGetRequestBodyString(state->hcCall, &body);
            LogToWindowFormat("HCHttpCallRequestGetRequestBodyString (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["body"] = body ? body : "";
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestGetRequestBodyReadFunction(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HCHttpCallRequestBodyReadFunction readFunc = nullptr;
            size_t bodySize = 0;
            void* context = nullptr;
            const HRESULT hr = HCHttpCallRequestGetRequestBodyReadFunction(state->hcCall, &readFunc, &bodySize, &context);
            LogToWindowFormat("HCHttpCallRequestGetRequestBodyReadFunction (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestGetDynamicBytesWritten(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint64_t dynamicBodySize = 0;
            uint64_t dynamicBodyBytesWritten = 0;
            const HRESULT hr = HCHttpCallRequestGetDynamicBytesWritten(state->hcCall, &dynamicBodySize, &dynamicBodyBytesWritten);
            LogToWindowFormat("HCHttpCallRequestGetDynamicBytesWritten (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["dynamicBodySize"] = dynamicBodySize;
            payload.result["dynamicBodyBytesWritten"] = dynamicBodyBytesWritten;
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestGetProgressReportFunction(
    [[maybe_unused]] DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            LogToWindow("HCHttpCallRequestGetProgressReportFunction: not linked in this build");
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestGetHeader(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            std::string headerName;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "headerName", headerName, error))
            {
                return E_INVALIDARG;
            }

            const char* headerValue = nullptr;
            const HRESULT hr = HCHttpCallRequestGetHeader(state->hcCall, headerName.c_str(), &headerValue);
            LogToWindowFormat("HCHttpCallRequestGetHeader (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["headerValue"] = headerValue ? headerValue : "";
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestGetHeaderAtIndex(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            int64_t headerIndex = 0;
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "headerIndex", headerIndex, error))
            {
                return E_INVALIDARG;
            }

            const char* headerName = nullptr;
            const char* headerValue = nullptr;
            const HRESULT hr = HCHttpCallRequestGetHeaderAtIndex(state->hcCall, static_cast<uint32_t>(headerIndex), &headerName, &headerValue);
            LogToWindowFormat("HCHttpCallRequestGetHeaderAtIndex (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["headerName"] = headerName ? headerName : "";
            payload.result["headerValue"] = headerValue ? headerValue : "";
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestGetNumHeaders(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t numHeaders = 0;
            const HRESULT hr = HCHttpCallRequestGetNumHeaders(state->hcCall, &numHeaders);
            LogToWindowFormat("HCHttpCallRequestGetNumHeaders (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["numHeaders"] = numHeaders;
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestGetMaxReceiveBufferSize(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSizeInBytes = 0;
            const HRESULT hr = HCHttpCallRequestGetMaxReceiveBufferSize(state->hcCall, &bufferSizeInBytes);
            LogToWindowFormat("HCHttpCallRequestGetMaxReceiveBufferSize (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["bufferSizeInBytes"] = static_cast<uint64_t>(bufferSizeInBytes);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestGetRetryAllowed(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            bool retryAllowed = false;
            const HRESULT hr = HCHttpCallRequestGetRetryAllowed(state->hcCall, &retryAllowed);
            LogToWindowFormat("HCHttpCallRequestGetRetryAllowed (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["retryAllowed"] = retryAllowed;
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestGetRetryCacheId(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t retryAfterCacheId = 0;
            const HRESULT hr = HCHttpCallRequestGetRetryCacheId(state->hcCall, &retryAfterCacheId);
            LogToWindowFormat("HCHttpCallRequestGetRetryCacheId (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["retryAfterCacheId"] = retryAfterCacheId;
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestGetTimeout(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t timeoutInSeconds = 0;
            const HRESULT hr = HCHttpCallRequestGetTimeout(state->hcCall, &timeoutInSeconds);
            LogToWindowFormat("HCHttpCallRequestGetTimeout (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["timeoutInSeconds"] = timeoutInSeconds;
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestGetRetryDelay(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t retryDelayInSeconds = 0;
            const HRESULT hr = HCHttpCallRequestGetRetryDelay(state->hcCall, &retryDelayInSeconds);
            LogToWindowFormat("HCHttpCallRequestGetRetryDelay (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["retryDelayInSeconds"] = retryDelayInSeconds;
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallRequestGetTimeoutWindow(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t timeoutWindowInSeconds = 0;
            const HRESULT hr = HCHttpCallRequestGetTimeoutWindow(state->hcCall, &timeoutWindowInSeconds);
            LogToWindowFormat("HCHttpCallRequestGetTimeoutWindow (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["timeoutWindowInSeconds"] = timeoutWindowInSeconds;
            return S_OK;
        });
}

// ============================================================================
// Response Set APIs
// ============================================================================

CommandResultPayload HandleHCHttpCallResponseSetResponseBodyWriteFunction(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = HCHttpCallResponseSetResponseBodyWriteFunction(state->hcCall, nullptr, nullptr);
            LogToWindowFormat("HCHttpCallResponseSetResponseBodyWriteFunction (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallResponseSetGzipCompressed(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            bool compress = true;
            std::string error;
            if (!CommandHandlerShared::TryParseBoolParameter(parameters, "compress", compress, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallResponseSetGzipCompressed(state->hcCall, compress);
            LogToWindowFormat("HCHttpCallResponseSetGzipCompressed (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallResponseSetDynamicSize(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            int64_t dynamicBodySize = 0;
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "dynamicBodySize", dynamicBodySize, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallResponseSetDynamicSize(state->hcCall, static_cast<uint64_t>(dynamicBodySize));
            LogToWindowFormat("HCHttpCallResponseSetDynamicSize (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallResponseAddDynamicBytesWritten(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            int64_t bytesWritten = 0;
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "bytesWritten", bytesWritten, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallResponseAddDynamicBytesWritten(state->hcCall, static_cast<uint64_t>(bytesWritten));
            LogToWindowFormat("HCHttpCallResponseAddDynamicBytesWritten (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallResponseSetResponseBodyBytes(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string body;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "body", body, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallResponseSetResponseBodyBytes(state->hcCall, (uint8_t*)body.c_str(), (uint32_t)body.size());
            LogToWindowFormat("HCHttpCallResponseSetResponseBodyBytes (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallResponseAppendResponseBodyBytes(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string body;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "body", body, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallResponseAppendResponseBodyBytes(state->hcCall, (uint8_t*)body.c_str(), (uint32_t)body.size());
            LogToWindowFormat("HCHttpCallResponseAppendResponseBodyBytes (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallResponseSetStatusCode(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            int64_t statusCode = 0;
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "statusCode", statusCode, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallResponseSetStatusCode(state->hcCall, static_cast<uint32_t>(statusCode));
            LogToWindowFormat("HCHttpCallResponseSetStatusCode (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallResponseSetNetworkErrorCode(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            int64_t networkErrorCode = 0;
            int64_t platformNetworkErrorCode = 0;
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "networkErrorCode", networkErrorCode, error))
            {
                return E_INVALIDARG;
            }
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "platformNetworkErrorCode", platformNetworkErrorCode, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallResponseSetNetworkErrorCode(state->hcCall, static_cast<HRESULT>(networkErrorCode), static_cast<uint32_t>(platformNetworkErrorCode));
            LogToWindowFormat("HCHttpCallResponseSetNetworkErrorCode (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallResponseSetPlatformNetworkErrorMessage(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string message;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "message", message, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallResponseSetPlatformNetworkErrorMessage(state->hcCall, message.c_str());
            LogToWindowFormat("HCHttpCallResponseSetPlatformNetworkErrorMessage (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallResponseSetHeader(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string headerName;
            std::string headerValue;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "headerName", headerName, error))
            {
                return E_INVALIDARG;
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "headerValue", headerValue, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallResponseSetHeader(state->hcCall, headerName.c_str(), headerValue.c_str());
            LogToWindowFormat("HCHttpCallResponseSetHeader (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallResponseSetHeaderWithLength(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            std::string headerName;
            std::string headerValue;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "headerName", headerName, error))
            {
                return E_INVALIDARG;
            }
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "headerValue", headerValue, error))
            {
                return E_INVALIDARG;
            }

            const HRESULT hr = HCHttpCallResponseSetHeaderWithLength(state->hcCall, headerName.c_str(), static_cast<uint32_t>(headerName.size()), headerValue.c_str(), static_cast<uint32_t>(headerValue.size()));
            LogToWindowFormat("HCHttpCallResponseSetHeaderWithLength (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

// ============================================================================
// Response Get APIs
// ============================================================================

CommandResultPayload HandleHCHttpCallResponseGetResponseBodyWriteFunction(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            HCHttpCallResponseBodyWriteFunction writeFunc = nullptr;
            void* context = nullptr;
            const HRESULT hr = HCHttpCallResponseGetResponseBodyWriteFunction(state->hcCall, &writeFunc, &context);
            LogToWindowFormat("HCHttpCallResponseGetResponseBodyWriteFunction (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallResponseGetDynamicBytesWritten(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint64_t dynamicBodySize = 0;
            uint64_t dynamicBodyBytesWritten = 0;
            const HRESULT hr = HCHttpCallResponseGetDynamicBytesWritten(state->hcCall, &dynamicBodySize, &dynamicBodyBytesWritten);
            LogToWindowFormat("HCHttpCallResponseGetDynamicBytesWritten (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["dynamicBodySize"] = dynamicBodySize;
            payload.result["dynamicBodyBytesWritten"] = dynamicBodyBytesWritten;
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallResponseGetResponseString(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const char* responseString = nullptr;
            const HRESULT hr = HCHttpCallResponseGetResponseString(state->hcCall, &responseString);
            LogToWindowFormat("HCHttpCallResponseGetResponseString (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["responseString"] = responseString ? responseString : "";
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallResponseGetResponseBodyBytesSize(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            const HRESULT hr = HCHttpCallResponseGetResponseBodyBytesSize(state->hcCall, &bufferSize);
            LogToWindowFormat("HCHttpCallResponseGetResponseBodyBytesSize (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["bufferSize"] = static_cast<uint64_t>(bufferSize);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallResponseGetResponseBodyBytes(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            size_t bufferSize = 0;
            HRESULT hr = HCHttpCallResponseGetResponseBodyBytesSize(state->hcCall, &bufferSize);
            LogToWindowFormat("HCHttpCallResponseGetResponseBodyBytesSize (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);

            std::vector<uint8_t> buffer(bufferSize);
            size_t bufferUsed = 0;
            hr = HCHttpCallResponseGetResponseBodyBytes(state->hcCall, bufferSize, buffer.data(), &bufferUsed);
            LogToWindowFormat("HCHttpCallResponseGetResponseBodyBytes (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);

            payload.result["responseBody"] = std::string(reinterpret_cast<const char*>(buffer.data()), bufferUsed);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallResponseGetStatusCode(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t statusCode = 0;
            const HRESULT hr = HCHttpCallResponseGetStatusCode(state->hcCall, &statusCode);
            LogToWindowFormat("HCHttpCallResponseGetStatusCode (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["statusCode"] = statusCode;

            // Optional assertions. Without one of these the command only reports the status and
            // the step passes for any outcome, including a request that never got a response at
            // all (status 0), which is how a torn-down request looks.
            std::string parseError;

            int64_t expectedStatusCode = 0;
            if (CommandHandlerShared::TryGetInt64Parameter(parameters, "expectedStatusCode", expectedStatusCode, parseError)
                && expectedStatusCode != 0)
            {
                if (statusCode != static_cast<uint32_t>(expectedStatusCode))
                {
                    LogToWindowFormat("HCHttpCallResponseGetStatusCode: expected %lld but got %u", expectedStatusCode, statusCode);
                    return E_FAIL;
                }
            }

            // Strict by default: a step that reads the status code is asserting that a response
            // actually came back. Status 0 means no HTTP response was ever received, which is what
            // a failed or torn-down transport looks like, so it fails unless the scenario opts out.
            // Scenarios that deliberately exercise a cancelled or timed-out request set
            // requireResponse: false.
            bool requireResponse = true;
            CommandHandlerShared::TryParseBoolParameter(parameters, "requireResponse", requireResponse, parseError);
            if (requireResponse && statusCode == 0)
            {
                LogToWindow("HCHttpCallResponseGetStatusCode: no HTTP response received (status 0)");
                return E_FAIL;
            }

            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallResponseGetNetworkErrorCode(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            HRESULT networkErrorCode = S_OK;
            uint32_t platformNetworkErrorCode = 0;
            const HRESULT hr = HCHttpCallResponseGetNetworkErrorCode(state->hcCall, &networkErrorCode, &platformNetworkErrorCode);
            LogToWindowFormat("HCHttpCallResponseGetNetworkErrorCode (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["networkErrorCode"] = static_cast<int32_t>(networkErrorCode);
            payload.result["platformNetworkErrorCode"] = platformNetworkErrorCode;

            // Optional assertion. libHttpClient reports transport failures here rather than through
            // the XAsync result, so a request cancelled by suspend completes with S_OK and status 0.
            // Asserting the network error is what proves the request was actually torn down instead
            // of quietly returning nothing.
            std::string parseError;
            bool expectNetworkError = false;
            if (CommandHandlerShared::TryParseBoolParameter(parameters, "expectNetworkError", expectNetworkError, parseError)
                && expectNetworkError
                && SUCCEEDED(networkErrorCode))
            {
                LogToWindowFormat("HCHttpCallResponseGetNetworkErrorCode: expected a network error but got 0x%08X",
                    static_cast<uint32_t>(networkErrorCode));
                return E_FAIL;
            }

            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallResponseGetPlatformNetworkErrorMessage(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            const char* platformNetworkErrorMessage = nullptr;
            const HRESULT hr = HCHttpCallResponseGetPlatformNetworkErrorMessage(state->hcCall, &platformNetworkErrorMessage);
            LogToWindowFormat("HCHttpCallResponseGetPlatformNetworkErrorMessage (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["platformNetworkErrorMessage"] = platformNetworkErrorMessage ? platformNetworkErrorMessage : "";
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallResponseGetHeader(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            std::string headerName;
            std::string error;
            if (!CommandHandlerShared::TryGetStringParameter(parameters, "headerName", headerName, error))
            {
                return E_INVALIDARG;
            }

            const char* headerValue = nullptr;
            const HRESULT hr = HCHttpCallResponseGetHeader(state->hcCall, headerName.c_str(), &headerValue);
            LogToWindowFormat("HCHttpCallResponseGetHeader (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["headerValue"] = headerValue ? headerValue : "";
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallResponseGetHeaderAtIndex(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            int64_t headerIndex = 0;
            std::string error;
            if (!CommandHandlerShared::TryGetInt64Parameter(parameters, "headerIndex", headerIndex, error))
            {
                return E_INVALIDARG;
            }

            const char* headerName = nullptr;
            const char* headerValue = nullptr;
            const HRESULT hr = HCHttpCallResponseGetHeaderAtIndex(state->hcCall, static_cast<uint32_t>(headerIndex), &headerName, &headerValue);
            LogToWindowFormat("HCHttpCallResponseGetHeaderAtIndex (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["headerName"] = headerName ? headerName : "";
            payload.result["headerValue"] = headerValue ? headerValue : "";
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallResponseGetNumHeaders(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            uint32_t numHeaders = 0;
            const HRESULT hr = HCHttpCallResponseGetNumHeaders(state->hcCall, &numHeaders);
            LogToWindowFormat("HCHttpCallResponseGetNumHeaders (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["numHeaders"] = numHeaders;
            return S_OK;
        });
}

// ============================================================================
// Context APIs
// ============================================================================

CommandResultPayload HandleHCHttpCallSetContext(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload&) -> HRESULT
        {
            const HRESULT hr = HCHttpCallSetContext(state->hcCall, nullptr);
            LogToWindowFormat("HCHttpCallSetContext (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            return S_OK;
        });
}

CommandResultPayload HandleHCHttpCallGetContext(
    DeviceGameSaveState* state,
    const std::string& commandId,
    const std::string& command,
    [[maybe_unused]] const nlohmann::json& parameters,
    const std::string& deviceId)
{
    return CommandHandlerShared::SyncCall(commandId, command, deviceId,
        [&](CommandResultPayload& payload) -> HRESULT
        {
            void* context = nullptr;
            const HRESULT hr = HCHttpCallGetContext(state->hcCall, &context);
            LogToWindowFormat("HCHttpCallGetContext (hr=0x%08X)", static_cast<uint32_t>(hr));
            RETURN_IF_FAILED(hr);
            payload.result["hasContext"] = (context != nullptr);
            return S_OK;
        });
}

// Self-registration of commands
static CommandRegistrar s_registrar({
    { "HCHttpCallCloseHandle", HandleHCHttpCallCloseHandle },
    { "HCHttpCallCreate", HandleHCHttpCallCreate },
    { "HCHttpCallDuplicateHandle", HandleHCHttpCallDuplicateHandle },
    { "HCHttpCallGetContext", HandleHCHttpCallGetContext },
    { "HCHttpCallGetId", HandleHCHttpCallGetId },
    { "HCHttpCallGetPerformCount", HandleHCHttpCallGetPerformCount },
    { "HCHttpCallGetRequestUrl", HandleHCHttpCallGetRequestUrl },
    { "HCHttpCallPerformAsync", HandleHCHttpCallPerformAsync },
    { "HCHttpCallPerformAssertPending", HandleHCHttpCallPerformAssertPending },
    { "TestHCHttpCallPerformBurst", HandleTestHCHttpCallPerformBurst },
    { "TestHCHttpCallPerformBurstStart", HandleTestHCHttpCallPerformBurstStart },
    { "TestHCHttpCallPerformBurstWait", HandleTestHCHttpCallPerformBurstWait },
    { "HCHttpCallPerformStart", HandleHCHttpCallPerformStart },
    { "HCHttpCallPerformWait", HandleHCHttpCallPerformWait },
    { "HCHttpCallRequestAddDynamicBytesWritten", HandleHCHttpCallRequestAddDynamicBytesWritten },
    { "HCHttpCallRequestEnableGzipCompression", HandleHCHttpCallRequestEnableGzipCompression },
    { "HCHttpCallRequestGetDynamicBytesWritten", HandleHCHttpCallRequestGetDynamicBytesWritten },
    { "HCHttpCallRequestGetHeader", HandleHCHttpCallRequestGetHeader },
    { "HCHttpCallRequestGetHeaderAtIndex", HandleHCHttpCallRequestGetHeaderAtIndex },
    { "HCHttpCallRequestGetMaxReceiveBufferSize", HandleHCHttpCallRequestGetMaxReceiveBufferSize },
    { "HCHttpCallRequestGetNumHeaders", HandleHCHttpCallRequestGetNumHeaders },
    { "HCHttpCallRequestGetProgressReportFunction", HandleHCHttpCallRequestGetProgressReportFunction },
    { "HCHttpCallRequestGetRequestBodyBytes", HandleHCHttpCallRequestGetRequestBodyBytes },
    { "HCHttpCallRequestGetRequestBodyReadFunction", HandleHCHttpCallRequestGetRequestBodyReadFunction },
    { "HCHttpCallRequestGetRequestBodyString", HandleHCHttpCallRequestGetRequestBodyString },
    { "HCHttpCallRequestGetRetryAllowed", HandleHCHttpCallRequestGetRetryAllowed },
    { "HCHttpCallRequestGetRetryCacheId", HandleHCHttpCallRequestGetRetryCacheId },
    { "HCHttpCallRequestGetRetryDelay", HandleHCHttpCallRequestGetRetryDelay },
    { "HCHttpCallRequestGetTimeout", HandleHCHttpCallRequestGetTimeout },
    { "HCHttpCallRequestGetTimeoutWindow", HandleHCHttpCallRequestGetTimeoutWindow },
    { "HCHttpCallRequestGetUrl", HandleHCHttpCallRequestGetUrl },
    { "HCHttpCallRequestSetDynamicSize", HandleHCHttpCallRequestSetDynamicSize },
    { "HCHttpCallRequestSetHeader", HandleHCHttpCallRequestSetHeader },
    { "HCHttpCallRequestSetMaxReceiveBufferSize", HandleHCHttpCallRequestSetMaxReceiveBufferSize },
    { "HCHttpCallRequestSetProgressReportFunction", HandleHCHttpCallRequestSetProgressReportFunction },
    { "HCHttpCallRequestSetRequestBodyBytes", HandleHCHttpCallRequestSetRequestBodyBytes },
    { "HCHttpCallRequestSetRequestBodyReadFunction", HandleHCHttpCallRequestSetRequestBodyReadFunction },
    { "HCHttpCallRequestSetRequestBodyString", HandleHCHttpCallRequestSetRequestBodyString },
    { "HCHttpCallRequestSetRetryAllowed", HandleHCHttpCallRequestSetRetryAllowed },
    { "HCHttpCallRequestSetRetryCacheId", HandleHCHttpCallRequestSetRetryCacheId },
    { "HCHttpCallRequestSetRetryDelay", HandleHCHttpCallRequestSetRetryDelay },
    { "HCHttpCallRequestSetSSLValidation", HandleHCHttpCallRequestSetSSLValidation },
    { "HCHttpCallRequestSetTimeout", HandleHCHttpCallRequestSetTimeout },
    { "HCHttpCallRequestSetTimeoutWindow", HandleHCHttpCallRequestSetTimeoutWindow },
    { "HCHttpCallRequestSetUrl", HandleHCHttpCallRequestSetUrl },
    { "HCHttpCallResponseAddDynamicBytesWritten", HandleHCHttpCallResponseAddDynamicBytesWritten },
    { "HCHttpCallResponseAppendResponseBodyBytes", HandleHCHttpCallResponseAppendResponseBodyBytes },
    { "HCHttpCallResponseGetDynamicBytesWritten", HandleHCHttpCallResponseGetDynamicBytesWritten },
    { "HCHttpCallResponseGetHeader", HandleHCHttpCallResponseGetHeader },
    { "HCHttpCallResponseGetHeaderAtIndex", HandleHCHttpCallResponseGetHeaderAtIndex },
    { "HCHttpCallResponseGetNetworkErrorCode", HandleHCHttpCallResponseGetNetworkErrorCode },
    { "HCHttpCallResponseGetNumHeaders", HandleHCHttpCallResponseGetNumHeaders },
    { "HCHttpCallResponseGetPlatformNetworkErrorMessage", HandleHCHttpCallResponseGetPlatformNetworkErrorMessage },
    { "HCHttpCallResponseGetResponseBodyBytes", HandleHCHttpCallResponseGetResponseBodyBytes },
    { "HCHttpCallResponseGetResponseBodyBytesSize", HandleHCHttpCallResponseGetResponseBodyBytesSize },
    { "HCHttpCallResponseGetResponseBodyWriteFunction", HandleHCHttpCallResponseGetResponseBodyWriteFunction },
    { "HCHttpCallResponseGetResponseString", HandleHCHttpCallResponseGetResponseString },
    { "HCHttpCallResponseGetStatusCode", HandleHCHttpCallResponseGetStatusCode },
    { "HCHttpCallResponseSetDynamicSize", HandleHCHttpCallResponseSetDynamicSize },
    { "HCHttpCallResponseSetGzipCompressed", HandleHCHttpCallResponseSetGzipCompressed },
    { "HCHttpCallResponseSetHeader", HandleHCHttpCallResponseSetHeader },
    { "HCHttpCallResponseSetHeaderWithLength", HandleHCHttpCallResponseSetHeaderWithLength },
    { "HCHttpCallResponseSetNetworkErrorCode", HandleHCHttpCallResponseSetNetworkErrorCode },
    { "HCHttpCallResponseSetPlatformNetworkErrorMessage", HandleHCHttpCallResponseSetPlatformNetworkErrorMessage },
    { "HCHttpCallResponseSetResponseBodyBytes", HandleHCHttpCallResponseSetResponseBodyBytes },
    { "HCHttpCallResponseSetResponseBodyWriteFunction", HandleHCHttpCallResponseSetResponseBodyWriteFunction },
    { "HCHttpCallResponseSetStatusCode", HandleHCHttpCallResponseSetStatusCode },
    { "HCHttpCallSetContext", HandleHCHttpCallSetContext },
    { "HCHttpCallSetTracing", HandleHCHttpCallSetTracing }
});

// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//
// QoS region-ping orchestrator. Discovers beacons through MultiplayerServer.ListQosServersForTitle,
// fans out per-region UDP pings on a small pool of std::thread workers, then aggregates and sorts
// the results. The orchestrator is a hand-authored counterpart to the generated MultiplayerServer.cpp.

#include "stdafx.h"
#include "QoS.h"
#include "QoSWireFormat.h"
#include "UdpSocketPAL.h"
#include "Generated/MultiplayerServer.h"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstring>
#include <thread>

using namespace PlayFab::MultiplayerServer;

namespace PlayFab
{
namespace QoS
{

PingQosServersOptions PingQosServersOptions::FromC(const PFMultiplayerServerPingQosServersOptions* c)
{
    PingQosServersOptions out{};
    if (c == nullptr)
    {
        return out;
    }
    if (c->timeoutMs > 0)
    {
        out.timeoutMs = c->timeoutMs;
    }
    if (c->pingsPerRegion > 0)
    {
        out.pingsPerRegion = c->pingsPerRegion;
    }
    out.maxConcurrentRegions = c->maxConcurrentRegions;
    out.includeAllRegions = c->includeAllRegions;
    if (c->routingPreference != nullptr)
    {
        out.routingPreference = c->routingPreference;
    }
    return out;
}

PFMultiplayerServerQosResult RegionLatency::ToC() const
{
    PFMultiplayerServerQosResult c{};
    c.region = region.empty() ? nullptr : region.data();
    c.latencyMs = latencyMs;
    c.pingsAttempted = pingsAttempted;
    c.pingsSucceeded = pingsSucceeded;
    c.errorCode = errorCode;
    return c;
}

size_t RegionLatency::RequiredBufferSize(const PFMultiplayerServerQosResult& model)
{
    size_t required = alignof(PFMultiplayerServerQosResult) + sizeof(PFMultiplayerServerQosResult);
    if (model.region)
    {
        required += std::strlen(model.region) + 1;
    }
    return required;
}

HRESULT RegionLatency::Copy(const PFMultiplayerServerQosResult& input, PFMultiplayerServerQosResult& output, ModelBuffer& buffer)
{
    output = input;
    auto regionCopy = buffer.CopyTo(input.region);
    RETURN_IF_FAILED(regionCopy.hr);
    output.region = regionCopy.ExtractPayload();
    return S_OK;
}

PingQosServersResult::PingQosServersResult(Vector<RegionLatency> rows) noexcept
    : m_rows{ std::move(rows) }
{
}

void PingQosServersResult::RefreshCStorage() const
{
    m_cRows.clear();
    m_cRowPtrs.clear();
    m_cRows.reserve(m_rows.size());
    m_cRowPtrs.reserve(m_rows.size());
    for (auto const& row : m_rows)
    {
        m_cRows.push_back(row.ToC());
    }
    for (auto const& cRow : m_cRows)
    {
        m_cRowPtrs.push_back(&cRow);
    }
    m_cModel.regionCount = static_cast<uint32_t>(m_cRows.size());
    m_cModel.regions = m_cRowPtrs.empty() ? nullptr : m_cRowPtrs.data();
}

size_t PingQosServersResult::RequiredBufferSize() const
{
    RefreshCStorage();
    size_t required = alignof(PFMultiplayerServerPingQosServersResult) + sizeof(PFMultiplayerServerPingQosServersResult);
    if (m_cModel.regionCount > 0)
    {
        required += alignof(PFMultiplayerServerQosResult const*) + sizeof(PFMultiplayerServerQosResult const*) * m_cModel.regionCount;
        for (uint32_t i = 0; i < m_cModel.regionCount; ++i)
        {
            required += RegionLatency::RequiredBufferSize(*m_cModel.regions[i]);
        }
    }
    return required;
}

Result<PFMultiplayerServerPingQosServersResult const*> PingQosServersResult::Copy(ModelBuffer& buffer) const
{
    RefreshCStorage();

    auto modelAlloc = buffer.Alloc<PFMultiplayerServerPingQosServersResult>(1);
    RETURN_IF_FAILED(modelAlloc.hr);
    PFMultiplayerServerPingQosServersResult* outModel = modelAlloc.ExtractPayload();
    *outModel = m_cModel;

    auto rowsCopy = buffer.CopyToArray<RegionLatency>(m_cModel.regions, m_cModel.regionCount);
    RETURN_IF_FAILED(rowsCopy.hr);
    outModel->regions = rowsCopy.ExtractPayload();

    return outModel;
}

namespace
{

// Shared state for an in-flight QoS operation. Lives on the heap (via SharedPtr) so the
// per-region worker threads and the completion callback can keep it alive together.
struct OrchestrationState
{
    Entity entity;
    PingQosServersOptions options;
    SharedPtr<AsyncOpContext<PingQosServersResult>> resultCtx;
    RunContext rc;

    // Inputs from ListQosServersForTitle. Indexed in parallel with results.
    Vector<String> regionNames;
    Vector<String> serverUrls;

    // Per-region accumulating results. Worker N writes index N only.
    Vector<RegionLatency> results;

    // Counter used by workers to claim the next region index.
    std::atomic<size_t> nextRegionIndex{ 0 };

    // Tracks how many worker threads are still running. Set once in DispatchWorkers before
    // any thread starts. Each worker decrements as it exits its loop. The thread that
    // decrements this to zero fires the completion. This guarantees that no worker is still
    // touching `state` (or `rc`, or any captured resource) when the user's completion runs.
    std::atomic<uint32_t> activeWorkers{ 0 };

    OrchestrationState(Entity e, PingQosServersOptions o, SharedPtr<AsyncOpContext<PingQosServersResult>> ctx, RunContext r)
        : entity{ std::move(e) }, options{ std::move(o) }, resultCtx{ std::move(ctx) }, rc{ std::move(r) }
    {
    }
};

void FireCompletion(SharedPtr<OrchestrationState> const& state)
{
    // Sort runs on the last-worker-to-exit's thread before we hand results to the queue.
    std::stable_sort(state->results.begin(), state->results.end(),
        [](RegionLatency const& a, RegionLatency const& b)
        {
            return a.latencyMs < b.latencyMs;
        });

    auto completion = [state]() mutable
    {
        state->resultCtx->Complete(Result<PingQosServersResult>{ PingQosServersResult{ std::move(state->results) } });
    };
    state->rc.TaskQueueSubmitCompletion(std::move(completion));
}

struct ValidatorContext
{
    uint64_t expectedToken;
};

bool ValidatePongCallback(void* ctx, uint8_t const* bytes, size_t size) noexcept
{
    auto* vctx = static_cast<ValidatorContext*>(ctx);
    return ValidateResponse(bytes, size, vctx->expectedToken);
}

void PingOneRegion(SharedPtr<OrchestrationState> state, size_t regionIndex) noexcept
{
    auto& result = state->results[regionIndex];
    result.region = state->regionNames[regionIndex];
    result.latencyMs = UINT32_MAX;
    result.errorCode = S_OK;

    if (state->rc.CancellationToken().IsCancelled())
    {
        result.errorCode = E_ABORT;
        return;
    }

    UniquePtr<UdpSocket> sock;
    HRESULT connectHr = UdpSocket::Connect(state->serverUrls[regionIndex].data(), kQoSBeaconPort, sock);
    if (FAILED(connectHr))
    {
        result.errorCode = connectHr;
        return;
    }

    uint64_t totalMicros = 0;
    HRESULT lastFailure = S_OK;

    for (uint32_t i = 0; i < state->options.pingsPerRegion; ++i)
    {
        if (state->rc.CancellationToken().IsCancelled())
        {
            lastFailure = E_ABORT;
            break;
        }

        result.pingsAttempted++;

        uint64_t token = MakeEchoToken();
        QoSPacket packet{};
        EncodeRequest(token, packet);

        ValidatorContext vctx{ token };
        std::chrono::microseconds rttValue{};
        HRESULT exchangeHr = sock->Exchange(
            packet.data(),
            packet.size(),
            std::chrono::milliseconds{ state->options.timeoutMs },
            &ValidatePongCallback,
            &vctx,
            state->rc.CancellationToken(),
            rttValue);

        if (SUCCEEDED(exchangeHr))
        {
            totalMicros += static_cast<uint64_t>(rttValue.count());
            result.pingsSucceeded++;
        }
        else
        {
            lastFailure = exchangeHr;
            if (exchangeHr == E_ABORT)
            {
                break;
            }
            // Otherwise: timeout / transient socket error — count as failed iteration, keep trying.
        }
    }

    if (result.pingsSucceeded > 0)
    {
        // Convert mean RTT (microseconds) to milliseconds. Clamp to UINT32_MAX-1 so we never
        // accidentally hit the "unreachable" sentinel.
        uint64_t meanMicros = totalMicros / result.pingsSucceeded;
        uint64_t meanMs = meanMicros / 1000;
        if (meanMs >= UINT32_MAX)
        {
            meanMs = UINT32_MAX - 1;
        }
        result.latencyMs = static_cast<uint32_t>(meanMs);
        result.errorCode = S_OK;
    }
    else
    {
        // Surface the last actionable error (E_ABORT if cancelled, otherwise the most recent
        // socket failure, or kUdpSocketTimeoutHr if all iterations just timed out without a
        // hard error). kUdpSocketTimeoutHr is the cross-platform equivalent of
        // HRESULT_FROM_WIN32(WAIT_TIMEOUT) -- WAIT_TIMEOUT is a Win32-only macro and is not
        // declared on POSIX platforms (see UdpSocketPAL.h).
        result.errorCode = lastFailure != S_OK ? lastFailure : kUdpSocketTimeoutHr;
        result.latencyMs = UINT32_MAX;
    }
}

void DispatchWorkers(SharedPtr<OrchestrationState> state)
{
    uint32_t maxConcurrent = state->options.maxConcurrentRegions;
    if (maxConcurrent == 0 || maxConcurrent > state->regionNames.size())
    {
        maxConcurrent = static_cast<uint32_t>(state->regionNames.size());
    }

    // Publish active worker count BEFORE starting any thread, so a fast worker that exits its
    // loop immediately doesn't see activeWorkers == 0 and fire completion before other workers
    // have started.
    state->activeWorkers.store(maxConcurrent, std::memory_order_release);

    for (uint32_t w = 0; w < maxConcurrent; ++w)
    {
        std::thread([state]()
        {
            for (;;)
            {
                size_t idx = state->nextRegionIndex.fetch_add(1, std::memory_order_relaxed);
                if (idx >= state->regionNames.size())
                {
                    break;
                }
                PingOneRegion(state, idx);
            }

            // This worker is done. If we are the last worker to exit, fire the completion.
            // Doing it here (instead of inside PingOneRegion) guarantees that no worker is
            // still inside its loop touching `state` or `state->rc` when the caller's
            // completion runs — even though SharedPtr keeps state itself alive, this lets us
            // reason about lifetime of any user-owned resources captured in RunContext.
            if (state->activeWorkers.fetch_sub(1, std::memory_order_acq_rel) == 1)
            {
                FireCompletion(state);
            }
        }).detach();
    }
}

} // anonymous namespace

AsyncOp<PingQosServersResult> QoSAPI::PingQosServers(
    Entity const& entity,
    PingQosServersOptions const& options,
    RunContext rc)
{
    ListQosServersForTitleRequest request{};
    request.SetIncludeAllRegions(options.includeAllRegions);
    // Empty string is a no-op inside the wrapper (it null-checks the underlying field),
    // so unconditionally forwarding is safe and keeps the caller's "leave unset" semantic.
    request.SetRoutingPreference(options.routingPreference);

    auto listOp = MultiplayerServerAPI::ListQosServersForTitle(entity, request, rc.Derive());

    auto resultCtx = MakeShared<AsyncOpContext<PingQosServersResult>>();

    listOp.Finally([entity, options, rc, resultCtx](Result<ListQosServersForTitleResponse> r) mutable
    {
        if (Failed(r))
        {
            resultCtx->Complete(Result<PingQosServersResult>{ r.hr, String{ r.errorMessage } });
            return;
        }

        auto response = r.ExtractPayload();
        auto const& model = response.Model();

        if (model.qosServersCount == 0)
        {
            resultCtx->Complete(Result<PingQosServersResult>{ PingQosServersResult{} });
            return;
        }

        if (model.qosServersCount > kMaxRegionCount)
        {
            // Defense-in-depth: a malformed or compromised ListQosServersForTitle response cannot
            // drive an unbounded thread / socket / memory burst. Real-world response is ~30.
            resultCtx->Complete(Result<PingQosServersResult>{ E_BOUNDS, String{ "ListQosServersForTitle returned more regions than the SDK will probe in a single call" } });
            return;
        }

        auto state = MakeShared<OrchestrationState>(entity, std::move(options), resultCtx, std::move(rc));
        state->regionNames.reserve(model.qosServersCount);
        state->serverUrls.reserve(model.qosServersCount);
        state->results.resize(model.qosServersCount);
        for (uint32_t i = 0; i < model.qosServersCount; ++i)
        {
            auto const* qs = model.qosServers[i];
            state->regionNames.emplace_back(qs && qs->region ? qs->region : "");
            state->serverUrls.emplace_back(qs && qs->serverUrl ? qs->serverUrl : "");
        }

        DispatchWorkers(std::move(state));
    });

    return AsyncOp<PingQosServersResult>{ resultCtx };
}

} // namespace QoS
} // namespace PlayFab

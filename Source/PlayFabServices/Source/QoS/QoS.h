// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <playfab/services/QoS/PFMultiplayerServerQoS.h>
#include "AsyncOp.h"
#include "BaseModel.h"
#include "RunContext.h"
#include "Generated/Types.h"

namespace PlayFab
{
namespace QoS
{

// Defaults applied when the caller passes nullptr or zero-initialized PFMultiplayerServerPingQosServersOptions.
constexpr uint32_t kDefaultTimeoutMs = 250;
constexpr uint32_t kDefaultPingsPerRegion = 3;

// Hard cap on regions returned by ListQosServersForTitle that we will probe in one call.
// Azure has ~30 public regions today; 256 is well beyond plausible future growth. A malformed
// or compromised response cannot use the SDK to drive an unbounded thread/socket/memory burst.
constexpr uint32_t kMaxRegionCount = 256;

struct PingQosServersOptions
{
    uint32_t timeoutMs{ kDefaultTimeoutMs };
    uint32_t pingsPerRegion{ kDefaultPingsPerRegion };
    // 0 means "all regions concurrently". Otherwise capped at the actual region count.
    uint32_t maxConcurrentRegions{ 0 };
    bool includeAllRegions{ false };
    // Empty string => leave unset on the upstream request; service default ("Microsoft") applies.
    // Otherwise forwarded verbatim to ListQosServersForTitle.RoutingPreference. The set of valid
    // strings is defined by the upstream RoutingType enumeration ("Microsoft" or "Internet").
    String routingPreference{};

    static PingQosServersOptions FromC(const PFMultiplayerServerPingQosServersOptions* c);
};

struct RegionLatency
{
    String region;
    uint32_t latencyMs{ UINT32_MAX };
    uint32_t pingsAttempted{ 0 };
    uint32_t pingsSucceeded{ 0 };
    HRESULT errorCode{ S_OK };

    PFMultiplayerServerQosResult ToC() const;
    static size_t RequiredBufferSize(const PFMultiplayerServerQosResult& model);
    static HRESULT Copy(const PFMultiplayerServerQosResult& input, PFMultiplayerServerQosResult& output, ModelBuffer& buffer);

    // ClientOutputModel "row" alias so CopyToArray<RegionLatency> resolves
    using ModelType = PFMultiplayerServerQosResult;
};

class PingQosServersResult : public ClientOutputModel<PFMultiplayerServerPingQosServersResult>
{
public:
    PingQosServersResult() = default;
    explicit PingQosServersResult(Vector<RegionLatency> rows) noexcept;

    Vector<RegionLatency> const& Rows() const noexcept { return m_rows; }

    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFMultiplayerServerPingQosServersResult const*> Copy(ModelBuffer& buffer) const override;

private:
    void RefreshCStorage() const;

    Vector<RegionLatency> m_rows;
    // Mutable cache for the C representation handed back through Copy(). Re-built on demand.
    mutable Vector<PFMultiplayerServerQosResult> m_cRows;
    mutable Vector<const PFMultiplayerServerQosResult*> m_cRowPtrs;
    mutable PFMultiplayerServerPingQosServersResult m_cModel{};
};

class QoSAPI
{
public:
    QoSAPI() = delete;
    QoSAPI(QoSAPI const&) = delete;
    QoSAPI& operator=(QoSAPI const&) = delete;
    ~QoSAPI() = delete;

    static AsyncOp<PingQosServersResult> PingQosServers(
        Entity const& entity,
        PingQosServersOptions const& options,
        RunContext rc);
};

} // namespace QoS
} // namespace PlayFab

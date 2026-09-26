// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//
// Hand-authored public header for the QoS region-ping feature. NOT auto-generated.
// Do not place under Include/Generated/ — the Generated tree is rewritten from an upstream
// API spec and would clobber this file. Lives under Include/playfab/services/QoS/ (its own
// subfolder) rather than directly under Include/playfab/services/, mirroring Source/QoS/.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/PFPal.h>
#include <playfab/core/PFTypes.h>
#include <playfab/services/PFTypes.h>

extern "C"
{

/// <summary>
/// Options controlling a QoS region ping operation. Pass NULL to PFMultiplayerServerPingQosServersAsync to use
/// the defaults documented below.
/// </summary>
typedef struct PFMultiplayerServerPingQosServersOptions
{
    /// <summary>
    /// Per-ping receive timeout in milliseconds. A ping that does not receive a valid echo within
    /// this window is counted as a failure. Default 250.
    /// </summary>
    uint32_t timeoutMs;

    /// <summary>
    /// Number of ping iterations per region. Reported latencyMs is the arithmetic mean of the
    /// successful iterations. Default 3.
    /// </summary>
    uint32_t pingsPerRegion;

    /// <summary>
    /// Maximum number of regions probed concurrently. One UDP socket is opened per region while
    /// it is in flight. Default 0, interpreted as "all regions concurrently".
    /// </summary>
    uint32_t maxConcurrentRegions;

    /// <summary>
    /// If true, ListQosServersForTitle is asked to return beacons for every Azure region rather
    /// than only the regions where the title has deployed builds. Default false.
    /// </summary>
    bool includeAllRegions;

    /// <summary>
    /// Routing preference forwarded to ListQosServersForTitle, which selects which beacon VIPs
    /// the service returns. Accepts the strings defined by the MultiplayerServer RoutingType
    /// enumeration: "Microsoft" (default — beacons whose Azure Public IPs are tagged with
    /// Microsoft network tier routing, traversing the Azure backbone for as much of the path
    /// as possible) or "Internet" (beacons routed over the public internet end-to-end). NULL
    /// or empty leaves the field unset and the service applies its default ("Microsoft").
    /// A title that ships data-plane VIPs with both routing types should call this API twice
    /// (once per preference) and rank regions per the data plane the player will actually use.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* routingPreference;
} PFMultiplayerServerPingQosServersOptions;

/// <summary>
/// Per-region result row. Pointers point inside the caller-supplied result buffer and are valid
/// only as long as that buffer remains valid and unmoved.
/// </summary>
typedef struct PFMultiplayerServerQosResult
{
    /// <summary>
    /// Azure region name as returned by ListQosServersForTitle (e.g. "EastUs", "FranceCentral").
    /// </summary>
    _Null_terminated_ const char* region;

    /// <summary>
    /// Mean round-trip latency across the successful pings, in milliseconds. UINT32_MAX when
    /// the region was unreachable (every ping timed out or failed validation).
    /// </summary>
    uint32_t latencyMs;

    /// <summary>
    /// Total number of pings attempted against this region.
    /// </summary>
    uint32_t pingsAttempted;

    /// <summary>
    /// Number of pings that received a validated echo within timeoutMs.
    /// </summary>
    uint32_t pingsSucceeded;

    /// <summary>
    /// S_OK if the region produced at least one validated reply. Otherwise an HRESULT describing
    /// the reason (timeout, DNS failure, socket error). Region rows are returned even when this
    /// is a failure code; the operation-level HRESULT is independent.
    /// </summary>
    HRESULT errorCode;
} PFMultiplayerServerQosResult;

/// <summary>
/// Aggregate result of PFMultiplayerServerPingQosServersAsync. Regions are sorted ascending by latencyMs;
/// unreachable regions (latencyMs == UINT32_MAX) appear at the end.
/// </summary>
typedef struct PFMultiplayerServerPingQosServersResult
{
    /// <summary>
    /// Number of entries in the regions array.
    /// </summary>
    uint32_t regionCount;

    /// <summary>
    /// Sorted region results. NULL when regionCount is 0. Array of pointers (array-of-pointers
    /// convention used by all generated PlayFab C structures).
    /// </summary>
    _Maybenull_ _Field_size_(regionCount) PFMultiplayerServerQosResult const* const* regions;
} PFMultiplayerServerPingQosServersResult;

/// <summary>
/// Discovers QoS beacons via PFMultiplayerServerListQosServersForTitleAsync and pings each one
/// over UDP/3075. Aggregates the latencies and returns the sorted list. Wall-clock duration is
/// bounded by roughly ceil(regionCount / maxConcurrentRegions) * timeoutMs * pingsPerRegion plus
/// the cost of the ListQosServersForTitle call.
/// </summary>
/// <param name="entityHandle">Authenticated PlayFab entity. The same handle accepted by
/// PFMultiplayerServerListQosServersForTitleAsync.</param>
/// <param name="options">Optional. NULL means apply defaults documented on PFMultiplayerServerPingQosServersOptions.</param>
/// <param name="async">XAsyncBlock for the async operation. Honors XAsyncCancel.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// A network where every region times out (for example, a corporate firewall blocking UDP/3075)
/// is not a failure mode: the operation completes with S_OK and every PFMultiplayerServerQosResult is
/// marked unreachable so callers can detect and surface that condition.
///
/// When the asynchronous operation is complete, call <see cref="PFMultiplayerServerPingQosServersGetResultSize"/>
/// and <see cref="PFMultiplayerServerPingQosServersGetResult"/> to retrieve the result.
/// </remarks>
PF_API PFMultiplayerServerPingQosServersAsync(
    _In_ PFEntityHandle entityHandle,
    _In_opt_ const PFMultiplayerServerPingQosServersOptions* options,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a PFMultiplayerServerPingQosServersAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>Result code for this API operation.</returns>
PF_API PFMultiplayerServerPingQosServersGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFMultiplayerServerPingQosServersAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFMultiplayerServerPingQosServersGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFMultiplayerServerPingQosServersResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

} // extern "C"

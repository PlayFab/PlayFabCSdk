// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/PFPal.h>
#include <playfab/core/PFTypes.h>
#include <playfab/services/PFTypes.h>

extern "C"
{

#pragma push_macro("IN")
#undef IN

/// <summary>
/// AzureVmSize enum.
/// </summary>
enum class PFMultiplayerServerAzureVmSize : uint32_t
{
    Standard_A1,
    Standard_A2,
    Standard_A3,
    Standard_A4,
    Standard_A1_v2,
    Standard_A2_v2,
    Standard_A4_v2,
    Standard_A8_v2,
    Standard_D1_v2,
    Standard_D2_v2,
    Standard_D3_v2,
    Standard_D4_v2,
    Standard_D5_v2,
    Standard_D2_v3,
    Standard_D4_v3,
    Standard_D8_v3,
    Standard_D16_v3,
    Standard_F1,
    Standard_F2,
    Standard_F4,
    Standard_F8,
    Standard_F16,
    Standard_F2s_v2,
    Standard_F4s_v2,
    Standard_F8s_v2,
    Standard_F16s_v2,
    Standard_D2as_v4,
    Standard_D4as_v4,
    Standard_D8as_v4,
    Standard_D16as_v4,
    Standard_D2a_v4,
    Standard_D4a_v4,
    Standard_D8a_v4,
    Standard_D16a_v4,
    Standard_D2ads_v5,
    Standard_D4ads_v5,
    Standard_D8ads_v5,
    Standard_D16ads_v5,
    Standard_E2a_v4,
    Standard_E4a_v4,
    Standard_E8a_v4,
    Standard_E16a_v4,
    Standard_E2as_v4,
    Standard_E4as_v4,
    Standard_E8as_v4,
    Standard_E16as_v4,
    Standard_D2s_v3,
    Standard_D4s_v3,
    Standard_D8s_v3,
    Standard_D16s_v3,
    Standard_DS1_v2,
    Standard_DS2_v2,
    Standard_DS3_v2,
    Standard_DS4_v2,
    Standard_DS5_v2,
    Standard_NC4as_T4_v3,
    Standard_D2d_v4,
    Standard_D4d_v4,
    Standard_D8d_v4,
    Standard_D16d_v4,
    Standard_D2ds_v4,
    Standard_D4ds_v4,
    Standard_D8ds_v4,
    Standard_D16ds_v4,
    Standard_HB120_16rs_v3,
    Standard_HB120_32rs_v3,
    Standard_HB120_64rs_v3,
    Standard_HB120_96rs_v3,
    Standard_HB120rs_v3
};

/// <summary>
/// ProtocolType enum.
/// </summary>
enum class PFMultiplayerServerProtocolType : uint32_t
{
    TCP,
    UDP
};

/// <summary>
/// PFMultiplayerServerListBuildAliasesRequest data model. Returns a list of summarized details of all
/// multiplayer server builds for a title.
/// </summary>
typedef struct PFMultiplayerServerListBuildAliasesRequest
{
    /// <summary>
    /// (Optional) The optional custom tags associated with the request (e.g. build number, external
    /// trace identifiers, etc.).
    /// </summary>
    _Maybenull_ _Field_size_(customTagsCount) struct PFStringDictionaryEntry const* customTags;

    /// <summary>
    /// Count of customTags
    /// </summary>
    uint32_t customTagsCount;

    /// <summary>
    /// (Optional) The page size for the request.
    /// </summary>
    _Maybenull_ int32_t const* pageSize;

    /// <summary>
    /// (Optional) The skip token for the paged request.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* skipToken;

} PFMultiplayerServerListBuildAliasesRequest;

/// <summary>
/// PFMultiplayerServerBuildSelectionCriterion data model.
/// </summary>
typedef struct PFMultiplayerServerBuildSelectionCriterion
{
    /// <summary>
    /// (Optional) Dictionary of build ids and their respective weights for distribution of allocation
    /// requests.
    /// </summary>
    _Maybenull_ _Field_size_(buildWeightDistributionCount) struct PFUint32DictionaryEntry const* buildWeightDistribution;

    /// <summary>
    /// Count of buildWeightDistribution
    /// </summary>
    uint32_t buildWeightDistributionCount;

} PFMultiplayerServerBuildSelectionCriterion;

/// <summary>
/// PFMultiplayerServerBuildAliasDetailsResponse data model.
/// </summary>
typedef struct PFMultiplayerServerBuildAliasDetailsResponse
{
    /// <summary>
    /// (Optional) The guid string alias Id of the alias to be created or updated.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* aliasId;

    /// <summary>
    /// (Optional) The alias name.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* aliasName;

    /// <summary>
    /// (Optional) Array of build selection criteria.
    /// </summary>
    _Maybenull_ _Field_size_(buildSelectionCriteriaCount) PFMultiplayerServerBuildSelectionCriterion const* const* buildSelectionCriteria;

    /// <summary>
    /// Count of buildSelectionCriteria
    /// </summary>
    uint32_t buildSelectionCriteriaCount;

} PFMultiplayerServerBuildAliasDetailsResponse;

/// <summary>
/// PFMultiplayerServerListBuildAliasesResponse data model.
/// </summary>
typedef struct PFMultiplayerServerListBuildAliasesResponse
{
    /// <summary>
    /// (Optional) The list of build aliases for the title.
    /// </summary>
    _Maybenull_ _Field_size_(buildAliasesCount) PFMultiplayerServerBuildAliasDetailsResponse const* const* buildAliases;

    /// <summary>
    /// Count of buildAliases
    /// </summary>
    uint32_t buildAliasesCount;

    /// <summary>
    /// The page size on the response.
    /// </summary>
    int32_t pageSize;

    /// <summary>
    /// (Optional) The skip token for the paged response.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* skipToken;

} PFMultiplayerServerListBuildAliasesResponse;

/// <summary>
/// PFMultiplayerServerListBuildSummariesRequest data model. Returns a list of summarized details of
/// all multiplayer server builds for a title.
/// </summary>
typedef struct PFMultiplayerServerListBuildSummariesRequest
{
    /// <summary>
    /// (Optional) The optional custom tags associated with the request (e.g. build number, external
    /// trace identifiers, etc.).
    /// </summary>
    _Maybenull_ _Field_size_(customTagsCount) struct PFStringDictionaryEntry const* customTags;

    /// <summary>
    /// Count of customTags
    /// </summary>
    uint32_t customTagsCount;

    /// <summary>
    /// (Optional) The page size for the request.
    /// </summary>
    _Maybenull_ int32_t const* pageSize;

    /// <summary>
    /// (Optional) The skip token for the paged request.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* skipToken;

} PFMultiplayerServerListBuildSummariesRequest;

/// <summary>
/// PFMultiplayerServerCurrentServerStats data model.
/// </summary>
typedef struct PFMultiplayerServerCurrentServerStats
{
    /// <summary>
    /// The number of active multiplayer servers.
    /// </summary>
    int32_t active;

    /// <summary>
    /// The number of multiplayer servers still downloading game resources (such as assets).
    /// </summary>
    int32_t propping;

    /// <summary>
    /// The number of standingby multiplayer servers.
    /// </summary>
    int32_t standingBy;

    /// <summary>
    /// The total number of multiplayer servers.
    /// </summary>
    int32_t total;

} PFMultiplayerServerCurrentServerStats;

/// <summary>
/// PFMultiplayerServerDynamicStandbyThreshold data model.
/// </summary>
typedef struct PFMultiplayerServerDynamicStandbyThreshold
{
    /// <summary>
    /// When the trigger threshold is reached, multiply by this value.
    /// </summary>
    double multiplier;

    /// <summary>
    /// The multiplier will be applied when the actual standby divided by target standby floor is less
    /// than this value.
    /// </summary>
    double triggerThresholdPercentage;

} PFMultiplayerServerDynamicStandbyThreshold;

/// <summary>
/// PFMultiplayerServerDynamicStandbySettings data model.
/// </summary>
typedef struct PFMultiplayerServerDynamicStandbySettings
{
    /// <summary>
    /// (Optional) List of auto standing by trigger values and corresponding standing by multiplier.
    /// Defaults to 1.5X at 50%, 3X at 25%, and 4X at 5%.
    /// </summary>
    _Maybenull_ _Field_size_(dynamicFloorMultiplierThresholdsCount) PFMultiplayerServerDynamicStandbyThreshold const* const* dynamicFloorMultiplierThresholds;

    /// <summary>
    /// Count of dynamicFloorMultiplierThresholds
    /// </summary>
    uint32_t dynamicFloorMultiplierThresholdsCount;

    /// <summary>
    /// When true, dynamic standby will be enabled.
    /// </summary>
    bool isEnabled;

    /// <summary>
    /// (Optional) The time it takes to reduce target standing by to configured floor value after an
    /// increase. Defaults to 30 minutes.
    /// </summary>
    _Maybenull_ int32_t const* rampDownSeconds;

} PFMultiplayerServerDynamicStandbySettings;

/// <summary>
/// PFMultiplayerServerSchedule data model.
/// </summary>
typedef struct PFMultiplayerServerSchedule
{
    /// <summary>
    /// (Optional) A short description about this schedule. For example, "Game launch on July 15th".
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* description;

    /// <summary>
    /// The date and time in UTC at which the schedule ends. If IsRecurringWeekly is true, this schedule
    /// will keep renewing for future weeks until disabled or removed.
    /// </summary>
    time_t endTime;

    /// <summary>
    /// Disables the schedule.
    /// </summary>
    bool isDisabled;

    /// <summary>
    /// If true, the StartTime and EndTime will get renewed every week.
    /// </summary>
    bool isRecurringWeekly;

    /// <summary>
    /// The date and time in UTC at which the schedule starts.
    /// </summary>
    time_t startTime;

    /// <summary>
    /// The standby target to maintain for the duration of the schedule.
    /// </summary>
    int32_t targetStandby;

} PFMultiplayerServerSchedule;

/// <summary>
/// PFMultiplayerServerScheduledStandbySettings data model.
/// </summary>
typedef struct PFMultiplayerServerScheduledStandbySettings
{
    /// <summary>
    /// When true, scheduled standby will be enabled.
    /// </summary>
    bool isEnabled;

    /// <summary>
    /// (Optional) A list of non-overlapping schedules.
    /// </summary>
    _Maybenull_ _Field_size_(scheduleListCount) PFMultiplayerServerSchedule const* const* scheduleList;

    /// <summary>
    /// Count of scheduleList
    /// </summary>
    uint32_t scheduleListCount;

} PFMultiplayerServerScheduledStandbySettings;

/// <summary>
/// PFMultiplayerServerBuildRegion data model.
/// </summary>
typedef struct PFMultiplayerServerBuildRegion
{
    /// <summary>
    /// (Optional) The current multiplayer server stats for the region.
    /// </summary>
    _Maybenull_ PFMultiplayerServerCurrentServerStats const* currentServerStats;

    /// <summary>
    /// (Optional) Optional settings to control dynamic adjustment of standby target.
    /// </summary>
    _Maybenull_ PFMultiplayerServerDynamicStandbySettings const* dynamicStandbySettings;

    /// <summary>
    /// Whether the game assets provided for the build have been replicated to this region.
    /// </summary>
    bool isAssetReplicationComplete;

    /// <summary>
    /// The maximum number of multiplayer servers for the region.
    /// </summary>
    int32_t maxServers;

    /// <summary>
    /// (Optional) Regional override for the number of multiplayer servers to host on a single VM of
    /// the build.
    /// </summary>
    _Maybenull_ int32_t const* multiplayerServerCountPerVm;

    /// <summary>
    /// (Optional) The build region.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* region;

    /// <summary>
    /// (Optional) Optional settings to set the standby target to specified values during the supplied
    /// schedules.
    /// </summary>
    _Maybenull_ PFMultiplayerServerScheduledStandbySettings const* scheduledStandbySettings;

    /// <summary>
    /// The target number of standby multiplayer servers for the region.
    /// </summary>
    int32_t standbyServers;

    /// <summary>
    /// (Optional) The status of multiplayer servers in the build region. Valid values are - Unknown,
    /// Initialized, Deploying, Deployed, Unhealthy, Deleting, Deleted.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* status;

    /// <summary>
    /// (Optional) Regional override for the VM size the build was created on.
    /// </summary>
    _Maybenull_ PFMultiplayerServerAzureVmSize const* vmSize;

} PFMultiplayerServerBuildRegion;

/// <summary>
/// PFMultiplayerServerBuildSummary data model.
/// </summary>
typedef struct PFMultiplayerServerBuildSummary
{
    /// <summary>
    /// (Optional) The guid string build ID of the build.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* buildId;

    /// <summary>
    /// (Optional) The build name.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* buildName;

    /// <summary>
    /// (Optional) The time the build was created in UTC.
    /// </summary>
    _Maybenull_ time_t const* creationTime;

    /// <summary>
    /// (Optional) The metadata of the build.
    /// </summary>
    _Maybenull_ _Field_size_(metadataCount) struct PFStringDictionaryEntry const* metadata;

    /// <summary>
    /// Count of metadata
    /// </summary>
    uint32_t metadataCount;

    /// <summary>
    /// (Optional) The configuration and status for each region in the build.
    /// </summary>
    _Maybenull_ _Field_size_(regionConfigurationsCount) PFMultiplayerServerBuildRegion const* const* regionConfigurations;

    /// <summary>
    /// Count of regionConfigurations
    /// </summary>
    uint32_t regionConfigurationsCount;

} PFMultiplayerServerBuildSummary;

/// <summary>
/// PFMultiplayerServerListBuildSummariesResponse data model.
/// </summary>
typedef struct PFMultiplayerServerListBuildSummariesResponse
{
    /// <summary>
    /// (Optional) The list of build summaries for a title.
    /// </summary>
    _Maybenull_ _Field_size_(buildSummariesCount) PFMultiplayerServerBuildSummary const* const* buildSummaries;

    /// <summary>
    /// Count of buildSummaries
    /// </summary>
    uint32_t buildSummariesCount;

    /// <summary>
    /// The page size on the response.
    /// </summary>
    int32_t pageSize;

    /// <summary>
    /// (Optional) The skip token for the paged response.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* skipToken;

} PFMultiplayerServerListBuildSummariesResponse;

/// <summary>
/// PFMultiplayerServerListQosServersForTitleRequest data model. Returns a list of quality of service
/// servers for a title.
/// </summary>
typedef struct PFMultiplayerServerListQosServersForTitleRequest
{
    /// <summary>
    /// (Optional) The optional custom tags associated with the request (e.g. build number, external
    /// trace identifiers, etc.).
    /// </summary>
    _Maybenull_ _Field_size_(customTagsCount) struct PFStringDictionaryEntry const* customTags;

    /// <summary>
    /// Count of customTags
    /// </summary>
    uint32_t customTagsCount;

    /// <summary>
    /// (Optional) Indicates that the response should contain Qos servers for all regions, including
    /// those where there are no builds deployed for the title.
    /// </summary>
    _Maybenull_ bool const* includeAllRegions;

} PFMultiplayerServerListQosServersForTitleRequest;

/// <summary>
/// PFMultiplayerServerQosServer data model.
/// </summary>
typedef struct PFMultiplayerServerQosServer
{
    /// <summary>
    /// (Optional) The region the QoS server is located in.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* region;

    /// <summary>
    /// (Optional) The QoS server URL.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* serverUrl;

} PFMultiplayerServerQosServer;

/// <summary>
/// PFMultiplayerServerListQosServersForTitleResponse data model.
/// </summary>
typedef struct PFMultiplayerServerListQosServersForTitleResponse
{
    /// <summary>
    /// The page size on the response.
    /// </summary>
    int32_t pageSize;

    /// <summary>
    /// (Optional) The list of QoS servers.
    /// </summary>
    _Maybenull_ _Field_size_(qosServersCount) PFMultiplayerServerQosServer const* const* qosServers;

    /// <summary>
    /// Count of qosServers
    /// </summary>
    uint32_t qosServersCount;

    /// <summary>
    /// (Optional) The skip token for the paged response.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* skipToken;

} PFMultiplayerServerListQosServersForTitleResponse;

/// <summary>
/// PFMultiplayerServerBuildAliasParams data model.
/// </summary>
typedef struct PFMultiplayerServerBuildAliasParams
{
    /// <summary>
    /// The guid string alias ID to use for the request.
    /// </summary>
    _Null_terminated_ const char* aliasId;

} PFMultiplayerServerBuildAliasParams;

/// <summary>
/// PFMultiplayerServerRequestMultiplayerServerRequest data model. Requests a multiplayer server session
/// from a particular build in any of the given preferred regions.
/// </summary>
typedef struct PFMultiplayerServerRequestMultiplayerServerRequest
{
    /// <summary>
    /// (Optional) The identifiers of the build alias to use for the request.
    /// </summary>
    _Maybenull_ PFMultiplayerServerBuildAliasParams const* buildAliasParams;

    /// <summary>
    /// (Optional) The guid string build ID of the multiplayer server to request.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* buildId;

    /// <summary>
    /// (Optional) The optional custom tags associated with the request (e.g. build number, external
    /// trace identifiers, etc.).
    /// </summary>
    _Maybenull_ _Field_size_(customTagsCount) struct PFStringDictionaryEntry const* customTags;

    /// <summary>
    /// Count of customTags
    /// </summary>
    uint32_t customTagsCount;

    /// <summary>
    /// (Optional) Initial list of players (potentially matchmade) allowed to connect to the game. This
    /// list is passed to the game server when requested (via GSDK) and can be used to validate players
    /// connecting to it.
    /// </summary>
    _Maybenull_ _Field_size_(initialPlayersCount) const char* const* initialPlayers;

    /// <summary>
    /// Count of initialPlayers
    /// </summary>
    uint32_t initialPlayersCount;

    /// <summary>
    /// The preferred regions to request a multiplayer server from. The Multiplayer Service will iterate
    /// through the regions in the specified order and allocate a server from the first one that has servers
    /// available.
    /// </summary>
    _Field_size_(preferredRegionsCount) const char* const* preferredRegions;

    /// <summary>
    /// Count of preferredRegions
    /// </summary>
    uint32_t preferredRegionsCount;

    /// <summary>
    /// (Optional) Data encoded as a string that is passed to the game server when requested. This can
    /// be used to to communicate information such as game mode or map through the request flow.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* sessionCookie;

    /// <summary>
    /// A guid string session ID created track the multiplayer server session over its life.
    /// </summary>
    _Null_terminated_ const char* sessionId;

} PFMultiplayerServerRequestMultiplayerServerRequest;

/// <summary>
/// PFMultiplayerServerConnectedPlayer data model.
/// </summary>
typedef struct PFMultiplayerServerConnectedPlayer
{
    /// <summary>
    /// (Optional) The player ID of the player connected to the multiplayer server.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* playerId;

} PFMultiplayerServerConnectedPlayer;

/// <summary>
/// PFMultiplayerServerPort data model.
/// </summary>
typedef struct PFMultiplayerServerPort
{
    /// <summary>
    /// The name for the port.
    /// </summary>
    _Null_terminated_ const char* name;

    /// <summary>
    /// The number for the port.
    /// </summary>
    int32_t num;

    /// <summary>
    /// The protocol for the port.
    /// </summary>
    PFMultiplayerServerProtocolType protocol;

} PFMultiplayerServerPort;

/// <summary>
/// PFMultiplayerServerPublicIpAddress data model.
/// </summary>
typedef struct PFMultiplayerServerPublicIpAddress
{
    /// <summary>
    /// FQDN of the public IP.
    /// </summary>
    _Null_terminated_ const char* fQDN;

    /// <summary>
    /// Server IP Address.
    /// </summary>
    _Null_terminated_ const char* ipAddress;

    /// <summary>
    /// Routing Type of the public IP.
    /// </summary>
    _Null_terminated_ const char* routingType;

} PFMultiplayerServerPublicIpAddress;

/// <summary>
/// PFMultiplayerServerRequestMultiplayerServerResponse data model.
/// </summary>
typedef struct PFMultiplayerServerRequestMultiplayerServerResponse
{
    /// <summary>
    /// (Optional) The identity of the build in which the server was allocated.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* buildId;

    /// <summary>
    /// (Optional) The connected players in the multiplayer server.
    /// </summary>
    _Maybenull_ _Field_size_(connectedPlayersCount) PFMultiplayerServerConnectedPlayer const* const* connectedPlayers;

    /// <summary>
    /// Count of connectedPlayers
    /// </summary>
    uint32_t connectedPlayersCount;

    /// <summary>
    /// (Optional) The fully qualified domain name of the virtual machine that is hosting this multiplayer
    /// server.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* fQDN;

    /// <summary>
    /// (Optional) The public IPv4 address of the virtual machine that is hosting this multiplayer server.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* iPV4Address;

    /// <summary>
    /// (Optional) The time (UTC) at which a change in the multiplayer server state was observed.
    /// </summary>
    _Maybenull_ time_t const* lastStateTransitionTime;

    /// <summary>
    /// (Optional) The ports the multiplayer server uses.
    /// </summary>
    _Maybenull_ _Field_size_(portsCount) PFMultiplayerServerPort const* const* ports;

    /// <summary>
    /// Count of ports
    /// </summary>
    uint32_t portsCount;

    /// <summary>
    /// (Optional) The list of public Ipv4 addresses associated with the server.
    /// </summary>
    _Maybenull_ _Field_size_(publicIPV4AddressesCount) PFMultiplayerServerPublicIpAddress const* const* publicIPV4Addresses;

    /// <summary>
    /// Count of publicIPV4Addresses
    /// </summary>
    uint32_t publicIPV4AddressesCount;

    /// <summary>
    /// (Optional) The region the multiplayer server is located in.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* region;

    /// <summary>
    /// (Optional) The string server ID of the multiplayer server generated by PlayFab.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* serverId;

    /// <summary>
    /// (Optional) The guid string session ID of the multiplayer server.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* sessionId;

    /// <summary>
    /// (Optional) The state of the multiplayer server.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* state;

    /// <summary>
    /// (Optional) The virtual machine ID that the multiplayer server is located on.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* vmId;

} PFMultiplayerServerRequestMultiplayerServerResponse;

#pragma pop_macro("IN")

}

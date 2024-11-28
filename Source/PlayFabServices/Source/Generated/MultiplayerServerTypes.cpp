#include "stdafx.h"
#include "MultiplayerServerTypes.h"
#include "JsonUtils.h"
#include "Types.h"

namespace PlayFab
{
namespace MultiplayerServer
{

JsonValue DeleteSecretRequest::ToJson() const
{
    return DeleteSecretRequest::ToJson(this->Model());
}

JsonValue DeleteSecretRequest::ToJson(const PFMultiplayerServerDeleteSecretRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    return output;
}

JsonValue ListBuildAliasesRequest::ToJson() const
{
    return ListBuildAliasesRequest::ToJson(this->Model());
}

JsonValue ListBuildAliasesRequest::ToJson(const PFMultiplayerServerListBuildAliasesRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "PageSize", input.pageSize);
    JsonUtils::ObjectAddMember(output, "SkipToken", input.skipToken);
    return output;
}

HRESULT BuildSelectionCriterion::FromJson(const JsonValue& input)
{
    DictionaryEntryVector<PFUint32DictionaryEntry> buildWeightDistribution{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "BuildWeightDistribution", buildWeightDistribution));
    this->SetBuildWeightDistribution(std::move(buildWeightDistribution));

    return S_OK;
}

size_t BuildSelectionCriterion::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFMultiplayerServerBuildSelectionCriterion const*> BuildSelectionCriterion::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<BuildSelectionCriterion>(&this->Model());
}

size_t BuildSelectionCriterion::RequiredBufferSize(const PFMultiplayerServerBuildSelectionCriterion& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFUint32DictionaryEntry) + sizeof(PFUint32DictionaryEntry) * model.buildWeightDistributionCount);
    for (size_t i = 0; i < model.buildWeightDistributionCount; ++i)
    {
        requiredSize += (std::strlen(model.buildWeightDistribution[i].key) + 1);
    }
    return requiredSize;
}

HRESULT BuildSelectionCriterion::Copy(const PFMultiplayerServerBuildSelectionCriterion& input, PFMultiplayerServerBuildSelectionCriterion& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToDictionary(input.buildWeightDistribution, input.buildWeightDistributionCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.buildWeightDistribution = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT BuildAliasDetailsResponse::FromJson(const JsonValue& input)
{
    String aliasId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "AliasId", aliasId));
    this->SetAliasId(std::move(aliasId));

    String aliasName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "AliasName", aliasName));
    this->SetAliasName(std::move(aliasName));

    ModelVector<BuildSelectionCriterion> buildSelectionCriteria{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<BuildSelectionCriterion>(input, "BuildSelectionCriteria", buildSelectionCriteria));
    this->SetBuildSelectionCriteria(std::move(buildSelectionCriteria));

    return S_OK;
}

size_t BuildAliasDetailsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFMultiplayerServerBuildAliasDetailsResponse const*> BuildAliasDetailsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<BuildAliasDetailsResponse>(&this->Model());
}

size_t BuildAliasDetailsResponse::RequiredBufferSize(const PFMultiplayerServerBuildAliasDetailsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.aliasId)
    {
        requiredSize += (std::strlen(model.aliasId) + 1);
    }
    if (model.aliasName)
    {
        requiredSize += (std::strlen(model.aliasName) + 1);
    }
    requiredSize += (alignof(PFMultiplayerServerBuildSelectionCriterion*) + sizeof(PFMultiplayerServerBuildSelectionCriterion*) * model.buildSelectionCriteriaCount);
    for (size_t i = 0; i < model.buildSelectionCriteriaCount; ++i)
    {
        requiredSize += BuildSelectionCriterion::RequiredBufferSize(*model.buildSelectionCriteria[i]);
    }
    return requiredSize;
}

HRESULT BuildAliasDetailsResponse::Copy(const PFMultiplayerServerBuildAliasDetailsResponse& input, PFMultiplayerServerBuildAliasDetailsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.aliasId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.aliasId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.aliasName);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.aliasName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<BuildSelectionCriterion>(input.buildSelectionCriteria, input.buildSelectionCriteriaCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.buildSelectionCriteria = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT ListBuildAliasesResponse::FromJson(const JsonValue& input)
{
    ModelVector<BuildAliasDetailsResponse> buildAliases{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<BuildAliasDetailsResponse>(input, "BuildAliases", buildAliases));
    this->SetBuildAliases(std::move(buildAliases));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PageSize", this->m_model.pageSize));

    String skipToken{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SkipToken", skipToken));
    this->SetSkipToken(std::move(skipToken));

    return S_OK;
}

size_t ListBuildAliasesResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFMultiplayerServerListBuildAliasesResponse const*> ListBuildAliasesResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ListBuildAliasesResponse>(&this->Model());
}

size_t ListBuildAliasesResponse::RequiredBufferSize(const PFMultiplayerServerListBuildAliasesResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFMultiplayerServerBuildAliasDetailsResponse*) + sizeof(PFMultiplayerServerBuildAliasDetailsResponse*) * model.buildAliasesCount);
    for (size_t i = 0; i < model.buildAliasesCount; ++i)
    {
        requiredSize += BuildAliasDetailsResponse::RequiredBufferSize(*model.buildAliases[i]);
    }
    if (model.skipToken)
    {
        requiredSize += (std::strlen(model.skipToken) + 1);
    }
    return requiredSize;
}

HRESULT ListBuildAliasesResponse::Copy(const PFMultiplayerServerListBuildAliasesResponse& input, PFMultiplayerServerListBuildAliasesResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<BuildAliasDetailsResponse>(input.buildAliases, input.buildAliasesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.buildAliases = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.skipToken);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.skipToken = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ListBuildSummariesRequest::ToJson() const
{
    return ListBuildSummariesRequest::ToJson(this->Model());
}

JsonValue ListBuildSummariesRequest::ToJson(const PFMultiplayerServerListBuildSummariesRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "PageSize", input.pageSize);
    JsonUtils::ObjectAddMember(output, "SkipToken", input.skipToken);
    return output;
}

HRESULT CurrentServerStats::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Active", this->m_model.active));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Propping", this->m_model.propping));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "StandingBy", this->m_model.standingBy));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Total", this->m_model.total));

    return S_OK;
}

size_t CurrentServerStats::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFMultiplayerServerCurrentServerStats const*> CurrentServerStats::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<CurrentServerStats>(&this->Model());
}

size_t CurrentServerStats::RequiredBufferSize(const PFMultiplayerServerCurrentServerStats& model)
{
    UNREFERENCED_PARAMETER(model); // Fixed size
    return sizeof(ModelType);
}

HRESULT CurrentServerStats::Copy(const PFMultiplayerServerCurrentServerStats& input, PFMultiplayerServerCurrentServerStats& output, ModelBuffer& buffer)
{
    output = input;
    UNREFERENCED_PARAMETER(buffer); // Fixed size
    return S_OK;
}

HRESULT DynamicStandbyThreshold::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Multiplier", this->m_model.multiplier));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TriggerThresholdPercentage", this->m_model.triggerThresholdPercentage));

    return S_OK;
}

size_t DynamicStandbyThreshold::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFMultiplayerServerDynamicStandbyThreshold const*> DynamicStandbyThreshold::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<DynamicStandbyThreshold>(&this->Model());
}

size_t DynamicStandbyThreshold::RequiredBufferSize(const PFMultiplayerServerDynamicStandbyThreshold& model)
{
    UNREFERENCED_PARAMETER(model); // Fixed size
    return sizeof(ModelType);
}

HRESULT DynamicStandbyThreshold::Copy(const PFMultiplayerServerDynamicStandbyThreshold& input, PFMultiplayerServerDynamicStandbyThreshold& output, ModelBuffer& buffer)
{
    output = input;
    UNREFERENCED_PARAMETER(buffer); // Fixed size
    return S_OK;
}

HRESULT DynamicStandbySettings::FromJson(const JsonValue& input)
{
    ModelVector<DynamicStandbyThreshold> dynamicFloorMultiplierThresholds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<DynamicStandbyThreshold>(input, "DynamicFloorMultiplierThresholds", dynamicFloorMultiplierThresholds));
    this->SetDynamicFloorMultiplierThresholds(std::move(dynamicFloorMultiplierThresholds));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IsEnabled", this->m_model.isEnabled));

    std::optional<int32_t> rampDownSeconds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "RampDownSeconds", rampDownSeconds));
    this->SetRampDownSeconds(std::move(rampDownSeconds));

    return S_OK;
}

size_t DynamicStandbySettings::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFMultiplayerServerDynamicStandbySettings const*> DynamicStandbySettings::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<DynamicStandbySettings>(&this->Model());
}

size_t DynamicStandbySettings::RequiredBufferSize(const PFMultiplayerServerDynamicStandbySettings& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFMultiplayerServerDynamicStandbyThreshold*) + sizeof(PFMultiplayerServerDynamicStandbyThreshold*) * model.dynamicFloorMultiplierThresholdsCount);
    for (size_t i = 0; i < model.dynamicFloorMultiplierThresholdsCount; ++i)
    {
        requiredSize += DynamicStandbyThreshold::RequiredBufferSize(*model.dynamicFloorMultiplierThresholds[i]);
    }
    if (model.rampDownSeconds)
    {
        requiredSize += (alignof(int32_t) + sizeof(int32_t));
    }
    return requiredSize;
}

HRESULT DynamicStandbySettings::Copy(const PFMultiplayerServerDynamicStandbySettings& input, PFMultiplayerServerDynamicStandbySettings& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<DynamicStandbyThreshold>(input.dynamicFloorMultiplierThresholds, input.dynamicFloorMultiplierThresholdsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.dynamicFloorMultiplierThresholds = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.rampDownSeconds);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.rampDownSeconds = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT Schedule::FromJson(const JsonValue& input)
{
    String description{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Description", description));
    this->SetDescription(std::move(description));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "EndTime", this->m_model.endTime));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IsDisabled", this->m_model.isDisabled));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IsRecurringWeekly", this->m_model.isRecurringWeekly));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "StartTime", this->m_model.startTime));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TargetStandby", this->m_model.targetStandby));

    return S_OK;
}

size_t Schedule::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFMultiplayerServerSchedule const*> Schedule::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<Schedule>(&this->Model());
}

size_t Schedule::RequiredBufferSize(const PFMultiplayerServerSchedule& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.description)
    {
        requiredSize += (std::strlen(model.description) + 1);
    }
    return requiredSize;
}

HRESULT Schedule::Copy(const PFMultiplayerServerSchedule& input, PFMultiplayerServerSchedule& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.description);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.description = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT ScheduledStandbySettings::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IsEnabled", this->m_model.isEnabled));

    ModelVector<Schedule> scheduleList{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<Schedule>(input, "ScheduleList", scheduleList));
    this->SetScheduleList(std::move(scheduleList));

    return S_OK;
}

size_t ScheduledStandbySettings::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFMultiplayerServerScheduledStandbySettings const*> ScheduledStandbySettings::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ScheduledStandbySettings>(&this->Model());
}

size_t ScheduledStandbySettings::RequiredBufferSize(const PFMultiplayerServerScheduledStandbySettings& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFMultiplayerServerSchedule*) + sizeof(PFMultiplayerServerSchedule*) * model.scheduleListCount);
    for (size_t i = 0; i < model.scheduleListCount; ++i)
    {
        requiredSize += Schedule::RequiredBufferSize(*model.scheduleList[i]);
    }
    return requiredSize;
}

HRESULT ScheduledStandbySettings::Copy(const PFMultiplayerServerScheduledStandbySettings& input, PFMultiplayerServerScheduledStandbySettings& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<Schedule>(input.scheduleList, input.scheduleListCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.scheduleList = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT BuildRegion::FromJson(const JsonValue& input)
{
    std::optional<CurrentServerStats> currentServerStats{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "CurrentServerStats", currentServerStats));
    if (currentServerStats)
    {
        this->SetCurrentServerStats(std::move(*currentServerStats));
    }

    std::optional<DynamicStandbySettings> dynamicStandbySettings{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DynamicStandbySettings", dynamicStandbySettings));
    if (dynamicStandbySettings)
    {
        this->SetDynamicStandbySettings(std::move(*dynamicStandbySettings));
    }

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IsAssetReplicationComplete", this->m_model.isAssetReplicationComplete));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "MaxServers", this->m_model.maxServers));

    std::optional<int32_t> multiplayerServerCountPerVm{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "MultiplayerServerCountPerVm", multiplayerServerCountPerVm));
    this->SetMultiplayerServerCountPerVm(std::move(multiplayerServerCountPerVm));

    String region{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Region", region));
    this->SetRegion(std::move(region));

    std::optional<ScheduledStandbySettings> scheduledStandbySettings{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ScheduledStandbySettings", scheduledStandbySettings));
    if (scheduledStandbySettings)
    {
        this->SetScheduledStandbySettings(std::move(*scheduledStandbySettings));
    }

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "StandbyServers", this->m_model.standbyServers));

    String status{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Status", status));
    this->SetStatus(std::move(status));

    std::optional<PFMultiplayerServerAzureVmSize> vmSize{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "VmSize", vmSize));
    this->SetVmSize(std::move(vmSize));

    return S_OK;
}

size_t BuildRegion::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFMultiplayerServerBuildRegion const*> BuildRegion::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<BuildRegion>(&this->Model());
}

size_t BuildRegion::RequiredBufferSize(const PFMultiplayerServerBuildRegion& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.currentServerStats)
    {
        requiredSize += CurrentServerStats::RequiredBufferSize(*model.currentServerStats);
    }
    if (model.dynamicStandbySettings)
    {
        requiredSize += DynamicStandbySettings::RequiredBufferSize(*model.dynamicStandbySettings);
    }
    if (model.multiplayerServerCountPerVm)
    {
        requiredSize += (alignof(int32_t) + sizeof(int32_t));
    }
    if (model.region)
    {
        requiredSize += (std::strlen(model.region) + 1);
    }
    if (model.scheduledStandbySettings)
    {
        requiredSize += ScheduledStandbySettings::RequiredBufferSize(*model.scheduledStandbySettings);
    }
    if (model.status)
    {
        requiredSize += (std::strlen(model.status) + 1);
    }
    if (model.vmSize)
    {
        requiredSize += (alignof(PFMultiplayerServerAzureVmSize) + sizeof(PFMultiplayerServerAzureVmSize));
    }
    return requiredSize;
}

HRESULT BuildRegion::Copy(const PFMultiplayerServerBuildRegion& input, PFMultiplayerServerBuildRegion& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<CurrentServerStats>(input.currentServerStats);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.currentServerStats = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<DynamicStandbySettings>(input.dynamicStandbySettings);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.dynamicStandbySettings = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.multiplayerServerCountPerVm);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.multiplayerServerCountPerVm = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.region);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.region = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<ScheduledStandbySettings>(input.scheduledStandbySettings);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.scheduledStandbySettings = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.status);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.status = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.vmSize);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.vmSize = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT BuildSummary::FromJson(const JsonValue& input)
{
    String buildId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "BuildId", buildId));
    this->SetBuildId(std::move(buildId));

    String buildName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "BuildName", buildName));
    this->SetBuildName(std::move(buildName));

    std::optional<time_t> creationTime{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "CreationTime", creationTime));
    this->SetCreationTime(std::move(creationTime));

    StringDictionaryEntryVector metadata{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Metadata", metadata));
    this->SetMetadata(std::move(metadata));

    ModelVector<BuildRegion> regionConfigurations{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<BuildRegion>(input, "RegionConfigurations", regionConfigurations));
    this->SetRegionConfigurations(std::move(regionConfigurations));

    return S_OK;
}

size_t BuildSummary::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFMultiplayerServerBuildSummary const*> BuildSummary::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<BuildSummary>(&this->Model());
}

size_t BuildSummary::RequiredBufferSize(const PFMultiplayerServerBuildSummary& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.buildId)
    {
        requiredSize += (std::strlen(model.buildId) + 1);
    }
    if (model.buildName)
    {
        requiredSize += (std::strlen(model.buildName) + 1);
    }
    if (model.creationTime)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    requiredSize += (alignof(PFStringDictionaryEntry) + sizeof(PFStringDictionaryEntry) * model.metadataCount);
    for (size_t i = 0; i < model.metadataCount; ++i)
    {
        requiredSize += (std::strlen(model.metadata[i].key) + 1);
        requiredSize += (std::strlen(model.metadata[i].value) + 1);
    }
    requiredSize += (alignof(PFMultiplayerServerBuildRegion*) + sizeof(PFMultiplayerServerBuildRegion*) * model.regionConfigurationsCount);
    for (size_t i = 0; i < model.regionConfigurationsCount; ++i)
    {
        requiredSize += BuildRegion::RequiredBufferSize(*model.regionConfigurations[i]);
    }
    return requiredSize;
}

HRESULT BuildSummary::Copy(const PFMultiplayerServerBuildSummary& input, PFMultiplayerServerBuildSummary& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.buildId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.buildId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.buildName);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.buildName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.creationTime);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.creationTime = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary(input.metadata, input.metadataCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.metadata = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<BuildRegion>(input.regionConfigurations, input.regionConfigurationsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.regionConfigurations = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT ListBuildSummariesResponse::FromJson(const JsonValue& input)
{
    ModelVector<BuildSummary> buildSummaries{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<BuildSummary>(input, "BuildSummaries", buildSummaries));
    this->SetBuildSummaries(std::move(buildSummaries));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PageSize", this->m_model.pageSize));

    String skipToken{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SkipToken", skipToken));
    this->SetSkipToken(std::move(skipToken));

    return S_OK;
}

size_t ListBuildSummariesResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFMultiplayerServerListBuildSummariesResponse const*> ListBuildSummariesResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ListBuildSummariesResponse>(&this->Model());
}

size_t ListBuildSummariesResponse::RequiredBufferSize(const PFMultiplayerServerListBuildSummariesResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFMultiplayerServerBuildSummary*) + sizeof(PFMultiplayerServerBuildSummary*) * model.buildSummariesCount);
    for (size_t i = 0; i < model.buildSummariesCount; ++i)
    {
        requiredSize += BuildSummary::RequiredBufferSize(*model.buildSummaries[i]);
    }
    if (model.skipToken)
    {
        requiredSize += (std::strlen(model.skipToken) + 1);
    }
    return requiredSize;
}

HRESULT ListBuildSummariesResponse::Copy(const PFMultiplayerServerListBuildSummariesResponse& input, PFMultiplayerServerListBuildSummariesResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<BuildSummary>(input.buildSummaries, input.buildSummariesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.buildSummaries = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.skipToken);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.skipToken = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ListQosServersForTitleRequest::ToJson() const
{
    return ListQosServersForTitleRequest::ToJson(this->Model());
}

JsonValue ListQosServersForTitleRequest::ToJson(const PFMultiplayerServerListQosServersForTitleRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "IncludeAllRegions", input.includeAllRegions);
    JsonUtils::ObjectAddMember(output, "RoutingPreference", input.routingPreference);
    return output;
}

HRESULT QosServer::FromJson(const JsonValue& input)
{
    String region{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Region", region));
    this->SetRegion(std::move(region));

    String serverUrl{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ServerUrl", serverUrl));
    this->SetServerUrl(std::move(serverUrl));

    return S_OK;
}

size_t QosServer::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFMultiplayerServerQosServer const*> QosServer::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<QosServer>(&this->Model());
}

size_t QosServer::RequiredBufferSize(const PFMultiplayerServerQosServer& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.region)
    {
        requiredSize += (std::strlen(model.region) + 1);
    }
    if (model.serverUrl)
    {
        requiredSize += (std::strlen(model.serverUrl) + 1);
    }
    return requiredSize;
}

HRESULT QosServer::Copy(const PFMultiplayerServerQosServer& input, PFMultiplayerServerQosServer& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.region);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.region = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.serverUrl);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.serverUrl = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT ListQosServersForTitleResponse::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PageSize", this->m_model.pageSize));

    ModelVector<QosServer> qosServers{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<QosServer>(input, "QosServers", qosServers));
    this->SetQosServers(std::move(qosServers));

    String skipToken{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SkipToken", skipToken));
    this->SetSkipToken(std::move(skipToken));

    return S_OK;
}

size_t ListQosServersForTitleResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFMultiplayerServerListQosServersForTitleResponse const*> ListQosServersForTitleResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ListQosServersForTitleResponse>(&this->Model());
}

size_t ListQosServersForTitleResponse::RequiredBufferSize(const PFMultiplayerServerListQosServersForTitleResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFMultiplayerServerQosServer*) + sizeof(PFMultiplayerServerQosServer*) * model.qosServersCount);
    for (size_t i = 0; i < model.qosServersCount; ++i)
    {
        requiredSize += QosServer::RequiredBufferSize(*model.qosServers[i]);
    }
    if (model.skipToken)
    {
        requiredSize += (std::strlen(model.skipToken) + 1);
    }
    return requiredSize;
}

HRESULT ListQosServersForTitleResponse::Copy(const PFMultiplayerServerListQosServersForTitleResponse& input, PFMultiplayerServerListQosServersForTitleResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<QosServer>(input.qosServers, input.qosServersCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.qosServers = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.skipToken);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.skipToken = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ListSecretSummariesRequest::ToJson() const
{
    return ListSecretSummariesRequest::ToJson(this->Model());
}

JsonValue ListSecretSummariesRequest::ToJson(const PFMultiplayerServerListSecretSummariesRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "PageSize", input.pageSize);
    JsonUtils::ObjectAddMember(output, "SkipToken", input.skipToken);
    return output;
}

HRESULT SecretSummary::FromJson(const JsonValue& input)
{
    std::optional<time_t> expirationDate{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "ExpirationDate", expirationDate));
    this->SetExpirationDate(std::move(expirationDate));

    String name{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Name", name));
    this->SetName(std::move(name));

    String version{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Version", version));
    this->SetVersion(std::move(version));

    return S_OK;
}

size_t SecretSummary::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFMultiplayerServerSecretSummary const*> SecretSummary::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<SecretSummary>(&this->Model());
}

size_t SecretSummary::RequiredBufferSize(const PFMultiplayerServerSecretSummary& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.expirationDate)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    if (model.name)
    {
        requiredSize += (std::strlen(model.name) + 1);
    }
    if (model.version)
    {
        requiredSize += (std::strlen(model.version) + 1);
    }
    return requiredSize;
}

HRESULT SecretSummary::Copy(const PFMultiplayerServerSecretSummary& input, PFMultiplayerServerSecretSummary& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.expirationDate);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.expirationDate = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.name);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.name = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.version);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.version = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT ListSecretSummariesResponse::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PageSize", this->m_model.pageSize));

    ModelVector<SecretSummary> secretSummaries{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<SecretSummary>(input, "SecretSummaries", secretSummaries));
    this->SetSecretSummaries(std::move(secretSummaries));

    String skipToken{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SkipToken", skipToken));
    this->SetSkipToken(std::move(skipToken));

    return S_OK;
}

size_t ListSecretSummariesResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFMultiplayerServerListSecretSummariesResponse const*> ListSecretSummariesResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ListSecretSummariesResponse>(&this->Model());
}

size_t ListSecretSummariesResponse::RequiredBufferSize(const PFMultiplayerServerListSecretSummariesResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFMultiplayerServerSecretSummary*) + sizeof(PFMultiplayerServerSecretSummary*) * model.secretSummariesCount);
    for (size_t i = 0; i < model.secretSummariesCount; ++i)
    {
        requiredSize += SecretSummary::RequiredBufferSize(*model.secretSummaries[i]);
    }
    if (model.skipToken)
    {
        requiredSize += (std::strlen(model.skipToken) + 1);
    }
    return requiredSize;
}

HRESULT ListSecretSummariesResponse::Copy(const PFMultiplayerServerListSecretSummariesResponse& input, PFMultiplayerServerListSecretSummariesResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<SecretSummary>(input.secretSummaries, input.secretSummariesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.secretSummaries = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.skipToken);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.skipToken = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue BuildAliasParams::ToJson() const
{
    return BuildAliasParams::ToJson(this->Model());
}

JsonValue BuildAliasParams::ToJson(const PFMultiplayerServerBuildAliasParams& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "AliasId", input.aliasId);
    return output;
}

JsonValue RequestMultiplayerServerRequest::ToJson() const
{
    return RequestMultiplayerServerRequest::ToJson(this->Model());
}

JsonValue RequestMultiplayerServerRequest::ToJson(const PFMultiplayerServerRequestMultiplayerServerRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember<BuildAliasParams>(output, "BuildAliasParams", input.buildAliasParams);
    JsonUtils::ObjectAddMember(output, "BuildId", input.buildId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMemberArray(output, "InitialPlayers", input.initialPlayers, input.initialPlayersCount);
    JsonUtils::ObjectAddMemberArray(output, "PreferredRegions", input.preferredRegions, input.preferredRegionsCount);
    JsonUtils::ObjectAddMember(output, "SessionCookie", input.sessionCookie);
    JsonUtils::ObjectAddMember(output, "SessionId", input.sessionId);
    return output;
}

HRESULT ConnectedPlayer::FromJson(const JsonValue& input)
{
    String playerId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PlayerId", playerId));
    this->SetPlayerId(std::move(playerId));

    return S_OK;
}

size_t ConnectedPlayer::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFMultiplayerServerConnectedPlayer const*> ConnectedPlayer::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ConnectedPlayer>(&this->Model());
}

size_t ConnectedPlayer::RequiredBufferSize(const PFMultiplayerServerConnectedPlayer& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.playerId)
    {
        requiredSize += (std::strlen(model.playerId) + 1);
    }
    return requiredSize;
}

HRESULT ConnectedPlayer::Copy(const PFMultiplayerServerConnectedPlayer& input, PFMultiplayerServerConnectedPlayer& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.playerId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.playerId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT Port::FromJson(const JsonValue& input)
{
    String name{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Name", name));
    this->SetName(std::move(name));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Num", this->m_model.num));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Protocol", this->m_model.protocol));

    return S_OK;
}

size_t Port::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFMultiplayerServerPort const*> Port::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<Port>(&this->Model());
}

size_t Port::RequiredBufferSize(const PFMultiplayerServerPort& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.name)
    {
        requiredSize += (std::strlen(model.name) + 1);
    }
    return requiredSize;
}

HRESULT Port::Copy(const PFMultiplayerServerPort& input, PFMultiplayerServerPort& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.name);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.name = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT PublicIpAddress::FromJson(const JsonValue& input)
{
    String fQDN{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FQDN", fQDN));
    this->SetFQDN(std::move(fQDN));

    String ipAddress{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IpAddress", ipAddress));
    this->SetIpAddress(std::move(ipAddress));

    String routingType{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "RoutingType", routingType));
    this->SetRoutingType(std::move(routingType));

    return S_OK;
}

size_t PublicIpAddress::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFMultiplayerServerPublicIpAddress const*> PublicIpAddress::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<PublicIpAddress>(&this->Model());
}

size_t PublicIpAddress::RequiredBufferSize(const PFMultiplayerServerPublicIpAddress& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.fQDN)
    {
        requiredSize += (std::strlen(model.fQDN) + 1);
    }
    if (model.ipAddress)
    {
        requiredSize += (std::strlen(model.ipAddress) + 1);
    }
    if (model.routingType)
    {
        requiredSize += (std::strlen(model.routingType) + 1);
    }
    return requiredSize;
}

HRESULT PublicIpAddress::Copy(const PFMultiplayerServerPublicIpAddress& input, PFMultiplayerServerPublicIpAddress& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.fQDN);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.fQDN = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.ipAddress);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.ipAddress = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.routingType);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.routingType = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT RequestMultiplayerServerResponse::FromJson(const JsonValue& input)
{
    String buildId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "BuildId", buildId));
    this->SetBuildId(std::move(buildId));

    ModelVector<ConnectedPlayer> connectedPlayers{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<ConnectedPlayer>(input, "ConnectedPlayers", connectedPlayers));
    this->SetConnectedPlayers(std::move(connectedPlayers));

    String fQDN{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FQDN", fQDN));
    this->SetFQDN(std::move(fQDN));

    String iPV4Address{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IPV4Address", iPV4Address));
    this->SetIPV4Address(std::move(iPV4Address));

    std::optional<time_t> lastStateTransitionTime{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "LastStateTransitionTime", lastStateTransitionTime));
    this->SetLastStateTransitionTime(std::move(lastStateTransitionTime));

    ModelVector<Port> ports{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<Port>(input, "Ports", ports));
    this->SetPorts(std::move(ports));

    ModelVector<PublicIpAddress> publicIPV4Addresses{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<PublicIpAddress>(input, "PublicIPV4Addresses", publicIPV4Addresses));
    this->SetPublicIPV4Addresses(std::move(publicIPV4Addresses));

    String region{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Region", region));
    this->SetRegion(std::move(region));

    String serverId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ServerId", serverId));
    this->SetServerId(std::move(serverId));

    String sessionId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SessionId", sessionId));
    this->SetSessionId(std::move(sessionId));

    String state{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "State", state));
    this->SetState(std::move(state));

    String vmId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "VmId", vmId));
    this->SetVmId(std::move(vmId));

    return S_OK;
}

size_t RequestMultiplayerServerResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFMultiplayerServerRequestMultiplayerServerResponse const*> RequestMultiplayerServerResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<RequestMultiplayerServerResponse>(&this->Model());
}

size_t RequestMultiplayerServerResponse::RequiredBufferSize(const PFMultiplayerServerRequestMultiplayerServerResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.buildId)
    {
        requiredSize += (std::strlen(model.buildId) + 1);
    }
    requiredSize += (alignof(PFMultiplayerServerConnectedPlayer*) + sizeof(PFMultiplayerServerConnectedPlayer*) * model.connectedPlayersCount);
    for (size_t i = 0; i < model.connectedPlayersCount; ++i)
    {
        requiredSize += ConnectedPlayer::RequiredBufferSize(*model.connectedPlayers[i]);
    }
    if (model.fQDN)
    {
        requiredSize += (std::strlen(model.fQDN) + 1);
    }
    if (model.iPV4Address)
    {
        requiredSize += (std::strlen(model.iPV4Address) + 1);
    }
    if (model.lastStateTransitionTime)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    requiredSize += (alignof(PFMultiplayerServerPort*) + sizeof(PFMultiplayerServerPort*) * model.portsCount);
    for (size_t i = 0; i < model.portsCount; ++i)
    {
        requiredSize += Port::RequiredBufferSize(*model.ports[i]);
    }
    requiredSize += (alignof(PFMultiplayerServerPublicIpAddress*) + sizeof(PFMultiplayerServerPublicIpAddress*) * model.publicIPV4AddressesCount);
    for (size_t i = 0; i < model.publicIPV4AddressesCount; ++i)
    {
        requiredSize += PublicIpAddress::RequiredBufferSize(*model.publicIPV4Addresses[i]);
    }
    if (model.region)
    {
        requiredSize += (std::strlen(model.region) + 1);
    }
    if (model.serverId)
    {
        requiredSize += (std::strlen(model.serverId) + 1);
    }
    if (model.sessionId)
    {
        requiredSize += (std::strlen(model.sessionId) + 1);
    }
    if (model.state)
    {
        requiredSize += (std::strlen(model.state) + 1);
    }
    if (model.vmId)
    {
        requiredSize += (std::strlen(model.vmId) + 1);
    }
    return requiredSize;
}

HRESULT RequestMultiplayerServerResponse::Copy(const PFMultiplayerServerRequestMultiplayerServerResponse& input, PFMultiplayerServerRequestMultiplayerServerResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.buildId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.buildId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<ConnectedPlayer>(input.connectedPlayers, input.connectedPlayersCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.connectedPlayers = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.fQDN);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.fQDN = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.iPV4Address);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.iPV4Address = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.lastStateTransitionTime);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.lastStateTransitionTime = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<Port>(input.ports, input.portsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.ports = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<PublicIpAddress>(input.publicIPV4Addresses, input.publicIPV4AddressesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.publicIPV4Addresses = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.region);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.region = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.serverId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.serverId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.sessionId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.sessionId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.state);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.state = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.vmId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.vmId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue PartyInvitationConfiguration::ToJson() const
{
    return PartyInvitationConfiguration::ToJson(this->Model());
}

JsonValue PartyInvitationConfiguration::ToJson(const PFMultiplayerServerPartyInvitationConfiguration& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberArray<EntityKey>(output, "EntityKeys", input.entityKeys, input.entityKeysCount);
    JsonUtils::ObjectAddMember(output, "Identifier", input.identifier);
    JsonUtils::ObjectAddMember(output, "Revocability", input.revocability);
    return output;
}

JsonValue PartyNetworkConfiguration::ToJson() const
{
    return PartyNetworkConfiguration::ToJson(this->Model());
}

JsonValue PartyNetworkConfiguration::ToJson(const PFMultiplayerServerPartyNetworkConfiguration& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "DirectPeerConnectivityOptions", input.directPeerConnectivityOptions);
    JsonUtils::ObjectAddMember(output, "MaxDevices", input.maxDevices);
    JsonUtils::ObjectAddMember(output, "MaxDevicesPerUser", input.maxDevicesPerUser);
    JsonUtils::ObjectAddMember(output, "MaxEndpointsPerDevice", input.maxEndpointsPerDevice);
    JsonUtils::ObjectAddMember(output, "MaxUsers", input.maxUsers);
    JsonUtils::ObjectAddMember(output, "MaxUsersPerDevice", input.maxUsersPerDevice);
    JsonUtils::ObjectAddMember<PartyInvitationConfiguration>(output, "PartyInvitationConfiguration", input.partyInvitationConfiguration);
    return output;
}

JsonValue RequestPartyServiceRequest::ToJson() const
{
    return RequestPartyServiceRequest::ToJson(this->Model());
}

JsonValue RequestPartyServiceRequest::ToJson(const PFMultiplayerServerRequestPartyServiceRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<PartyNetworkConfiguration>(output, "NetworkConfiguration", input.networkConfiguration);
    JsonUtils::ObjectAddMember(output, "PartyId", input.partyId);
    JsonUtils::ObjectAddMemberArray(output, "PreferredRegions", input.preferredRegions, input.preferredRegionsCount);
    return output;
}

HRESULT RequestPartyServiceResponse::FromJson(const JsonValue& input)
{
    String invitationId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "InvitationId", invitationId));
    this->SetInvitationId(std::move(invitationId));

    String partyId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PartyId", partyId));
    this->SetPartyId(std::move(partyId));

    String serializedNetworkDescriptor{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SerializedNetworkDescriptor", serializedNetworkDescriptor));
    this->SetSerializedNetworkDescriptor(std::move(serializedNetworkDescriptor));

    return S_OK;
}

size_t RequestPartyServiceResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFMultiplayerServerRequestPartyServiceResponse const*> RequestPartyServiceResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<RequestPartyServiceResponse>(&this->Model());
}

size_t RequestPartyServiceResponse::RequiredBufferSize(const PFMultiplayerServerRequestPartyServiceResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.invitationId)
    {
        requiredSize += (std::strlen(model.invitationId) + 1);
    }
    if (model.partyId)
    {
        requiredSize += (std::strlen(model.partyId) + 1);
    }
    if (model.serializedNetworkDescriptor)
    {
        requiredSize += (std::strlen(model.serializedNetworkDescriptor) + 1);
    }
    return requiredSize;
}

HRESULT RequestPartyServiceResponse::Copy(const PFMultiplayerServerRequestPartyServiceResponse& input, PFMultiplayerServerRequestPartyServiceResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.invitationId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.invitationId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.partyId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.partyId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.serializedNetworkDescriptor);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.serializedNetworkDescriptor = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue Secret::ToJson() const
{
    return Secret::ToJson(this->Model());
}

JsonValue Secret::ToJson(const PFMultiplayerServerSecret& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberTime(output, "ExpirationDate", input.expirationDate);
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    JsonUtils::ObjectAddMember(output, "Value", input.value);
    return output;
}

JsonValue UploadSecretRequest::ToJson() const
{
    return UploadSecretRequest::ToJson(this->Model());
}

JsonValue UploadSecretRequest::ToJson(const PFMultiplayerServerUploadSecretRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "ForceUpdate", input.forceUpdate);
    JsonUtils::ObjectAddMember<Secret>(output, "GameSecret", input.gameSecret);
    return output;
}

} // namespace MultiplayerServer

// Json serialization helpers

} // namespace PlayFab

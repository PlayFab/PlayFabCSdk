#include "stdafx.h"
#include "StatisticsTypes.h"
#include "JsonUtils.h"
#include "Types.h"

namespace PlayFab
{
namespace Statistics
{

JsonValue StatisticColumn::ToJson() const
{
    return StatisticColumn::ToJson(this->Model());
}

JsonValue StatisticColumn::ToJson(const PFStatisticsStatisticColumn& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "AggregationMethod", input.aggregationMethod);
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    return output;
}

HRESULT StatisticColumn::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "AggregationMethod", this->m_model.aggregationMethod));

    String name{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Name", name));
    this->SetName(std::move(name));

    return S_OK;
}

size_t StatisticColumn::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFStatisticsStatisticColumn const*> StatisticColumn::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<StatisticColumn>(&this->Model());
}

size_t StatisticColumn::RequiredBufferSize(const PFStatisticsStatisticColumn& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.name)
    {
        requiredSize += (std::strlen(model.name) + 1);
    }
    return requiredSize;
}

HRESULT StatisticColumn::Copy(const PFStatisticsStatisticColumn& input, PFStatisticsStatisticColumn& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.name); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.name = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue CreateStatisticDefinitionRequest::ToJson() const
{
    return CreateStatisticDefinitionRequest::ToJson(this->Model());
}

JsonValue CreateStatisticDefinitionRequest::ToJson(const PFStatisticsCreateStatisticDefinitionRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberArray<StatisticColumn>(output, "Columns", input.columns, input.columnsCount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "EntityType", input.entityType);
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    JsonUtils::ObjectAddMember<VersionConfiguration>(output, "VersionConfiguration", input.versionConfiguration);
    return output;
}

JsonValue DeleteStatisticDefinitionRequest::ToJson() const
{
    return DeleteStatisticDefinitionRequest::ToJson(this->Model());
}

JsonValue DeleteStatisticDefinitionRequest::ToJson(const PFStatisticsDeleteStatisticDefinitionRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    return output;
}

JsonValue StatisticDelete::ToJson() const
{
    return StatisticDelete::ToJson(this->Model());
}

JsonValue StatisticDelete::ToJson(const PFStatisticsStatisticDelete& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    return output;
}

JsonValue DeleteStatisticsRequest::ToJson() const
{
    return DeleteStatisticsRequest::ToJson(this->Model());
}

JsonValue DeleteStatisticsRequest::ToJson(const PFStatisticsDeleteStatisticsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMemberArray<StatisticDelete>(output, "Statistics", input.statistics, input.statisticsCount);
    return output;
}

HRESULT DeleteStatisticsResponse::FromJson(const JsonValue& input)
{
    std::optional<EntityKey> entity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Entity", entity));
    if (entity)
    {
        this->SetEntity(std::move(*entity));
    }

    return S_OK;
}

size_t DeleteStatisticsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFStatisticsDeleteStatisticsResponse const*> DeleteStatisticsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<DeleteStatisticsResponse>(&this->Model());
}

size_t DeleteStatisticsResponse::RequiredBufferSize(const PFStatisticsDeleteStatisticsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.entity)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.entity);
    }
    return requiredSize;
}

HRESULT DeleteStatisticsResponse::Copy(const PFStatisticsDeleteStatisticsResponse& input, PFStatisticsDeleteStatisticsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.entity); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.entity = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetStatisticDefinitionRequest::ToJson() const
{
    return GetStatisticDefinitionRequest::ToJson(this->Model());
}

JsonValue GetStatisticDefinitionRequest::ToJson(const PFStatisticsGetStatisticDefinitionRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    return output;
}

HRESULT GetStatisticDefinitionResponse::FromJson(const JsonValue& input)
{
    ModelVector<StatisticColumn> columns{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<StatisticColumn>(input, "Columns", columns));
    this->SetColumns(std::move(columns));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "Created", this->m_model.created));

    String entityType{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "EntityType", entityType));
    this->SetEntityType(std::move(entityType));

    std::optional<time_t> lastResetTime{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "LastResetTime", lastResetTime));
    this->SetLastResetTime(std::move(lastResetTime));

    CStringVector linkedLeaderboardNames{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "LinkedLeaderboardNames", linkedLeaderboardNames));
    this->SetLinkedLeaderboardNames(std::move(linkedLeaderboardNames));

    String name{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Name", name));
    this->SetName(std::move(name));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Version", this->m_model.version));

    std::optional<VersionConfiguration> versionConfiguration{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "VersionConfiguration", versionConfiguration));
    if (versionConfiguration)
    {
        this->SetVersionConfiguration(std::move(*versionConfiguration));
    }

    return S_OK;
}

size_t GetStatisticDefinitionResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFStatisticsGetStatisticDefinitionResponse const*> GetStatisticDefinitionResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetStatisticDefinitionResponse>(&this->Model());
}

size_t GetStatisticDefinitionResponse::RequiredBufferSize(const PFStatisticsGetStatisticDefinitionResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFStatisticsStatisticColumn*) + sizeof(PFStatisticsStatisticColumn*) * model.columnsCount);
    for (size_t i = 0; i < model.columnsCount; ++i)
    {
        requiredSize += StatisticColumn::RequiredBufferSize(*model.columns[i]);
    }
    if (model.entityType)
    {
        requiredSize += (std::strlen(model.entityType) + 1);
    }
    if (model.lastResetTime)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.linkedLeaderboardNamesCount);
    for (size_t i = 0; i < model.linkedLeaderboardNamesCount; ++i)
    {
        requiredSize += (std::strlen(model.linkedLeaderboardNames[i]) + 1);
    }
    if (model.name)
    {
        requiredSize += (std::strlen(model.name) + 1);
    }
    if (model.versionConfiguration)
    {
        requiredSize += VersionConfiguration::RequiredBufferSize(*model.versionConfiguration);
    }
    return requiredSize;
}

HRESULT GetStatisticDefinitionResponse::Copy(const PFStatisticsGetStatisticDefinitionResponse& input, PFStatisticsGetStatisticDefinitionResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<StatisticColumn>(input.columns, input.columnsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.columns = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.entityType); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.entityType = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.lastResetTime); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.lastResetTime = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.linkedLeaderboardNames, input.linkedLeaderboardNamesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.linkedLeaderboardNames = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.name); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.name = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<VersionConfiguration>(input.versionConfiguration); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.versionConfiguration = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetStatisticsRequest::ToJson() const
{
    return GetStatisticsRequest::ToJson(this->Model());
}

JsonValue GetStatisticsRequest::ToJson(const PFStatisticsGetStatisticsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    return output;
}

HRESULT StatisticColumnCollection::FromJson(const JsonValue& input)
{
    ModelVector<StatisticColumn> columns{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<StatisticColumn>(input, "Columns", columns));
    this->SetColumns(std::move(columns));

    return S_OK;
}

size_t StatisticColumnCollection::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFStatisticsStatisticColumnCollection const*> StatisticColumnCollection::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<StatisticColumnCollection>(&this->Model());
}

size_t StatisticColumnCollection::RequiredBufferSize(const PFStatisticsStatisticColumnCollection& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFStatisticsStatisticColumn*) + sizeof(PFStatisticsStatisticColumn*) * model.columnsCount);
    for (size_t i = 0; i < model.columnsCount; ++i)
    {
        requiredSize += StatisticColumn::RequiredBufferSize(*model.columns[i]);
    }
    return requiredSize;
}

HRESULT StatisticColumnCollection::Copy(const PFStatisticsStatisticColumnCollection& input, PFStatisticsStatisticColumnCollection& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<StatisticColumn>(input.columns, input.columnsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.columns = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT EntityStatisticValue::FromJson(const JsonValue& input)
{
    String metadata{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Metadata", metadata));
    this->SetMetadata(std::move(metadata));

    String name{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Name", name));
    this->SetName(std::move(name));

    CStringVector scores{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Scores", scores));
    this->SetScores(std::move(scores));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Version", this->m_model.version));

    return S_OK;
}

size_t EntityStatisticValue::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFStatisticsEntityStatisticValue const*> EntityStatisticValue::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<EntityStatisticValue>(&this->Model());
}

size_t EntityStatisticValue::RequiredBufferSize(const PFStatisticsEntityStatisticValue& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.metadata)
    {
        requiredSize += (std::strlen(model.metadata) + 1);
    }
    if (model.name)
    {
        requiredSize += (std::strlen(model.name) + 1);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.scoresCount);
    for (size_t i = 0; i < model.scoresCount; ++i)
    {
        requiredSize += (std::strlen(model.scores[i]) + 1);
    }
    return requiredSize;
}

HRESULT EntityStatisticValue::Copy(const PFStatisticsEntityStatisticValue& input, PFStatisticsEntityStatisticValue& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.metadata); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.metadata = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.name); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.name = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.scores, input.scoresCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.scores = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT GetStatisticsResponse::FromJson(const JsonValue& input)
{
    ModelDictionaryEntryVector<StatisticColumnCollection> columnDetails{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<StatisticColumnCollection>(input, "ColumnDetails", columnDetails));
    this->SetColumnDetails(std::move(columnDetails));

    std::optional<EntityKey> entity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Entity", entity));
    if (entity)
    {
        this->SetEntity(std::move(*entity));
    }

    ModelDictionaryEntryVector<EntityStatisticValue> statistics{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<EntityStatisticValue>(input, "Statistics", statistics));
    this->SetStatistics(std::move(statistics));

    return S_OK;
}

size_t GetStatisticsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFStatisticsGetStatisticsResponse const*> GetStatisticsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetStatisticsResponse>(&this->Model());
}

size_t GetStatisticsResponse::RequiredBufferSize(const PFStatisticsGetStatisticsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFStatisticsStatisticColumnCollectionDictionaryEntry) + sizeof(PFStatisticsStatisticColumnCollectionDictionaryEntry) * model.columnDetailsCount);
    for (size_t i = 0; i < model.columnDetailsCount; ++i)
    {
        requiredSize += (std::strlen(model.columnDetails[i].key) + 1);
        requiredSize += StatisticColumnCollection::RequiredBufferSize(*model.columnDetails[i].value);
    }
    if (model.entity)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.entity);
    }
    requiredSize += (alignof(PFStatisticsEntityStatisticValueDictionaryEntry) + sizeof(PFStatisticsEntityStatisticValueDictionaryEntry) * model.statisticsCount);
    for (size_t i = 0; i < model.statisticsCount; ++i)
    {
        requiredSize += (std::strlen(model.statistics[i].key) + 1);
        requiredSize += EntityStatisticValue::RequiredBufferSize(*model.statistics[i].value);
    }
    return requiredSize;
}

HRESULT GetStatisticsResponse::Copy(const PFStatisticsGetStatisticsResponse& input, PFStatisticsGetStatisticsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToDictionary<StatisticColumnCollection>(input.columnDetails, input.columnDetailsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.columnDetails = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.entity); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.entity = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary<EntityStatisticValue>(input.statistics, input.statisticsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.statistics = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetStatisticsForEntitiesRequest::ToJson() const
{
    return GetStatisticsForEntitiesRequest::ToJson(this->Model());
}

JsonValue GetStatisticsForEntitiesRequest::ToJson(const PFStatisticsGetStatisticsForEntitiesRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMemberArray<EntityKey>(output, "Entities", input.entities, input.entitiesCount);
    return output;
}

HRESULT EntityStatistics::FromJson(const JsonValue& input)
{
    std::optional<EntityKey> entityKey{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "EntityKey", entityKey));
    if (entityKey)
    {
        this->SetEntityKey(std::move(*entityKey));
    }

    ModelVector<EntityStatisticValue> statistics{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<EntityStatisticValue>(input, "Statistics", statistics));
    this->SetStatistics(std::move(statistics));

    return S_OK;
}

size_t EntityStatistics::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFStatisticsEntityStatistics const*> EntityStatistics::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<EntityStatistics>(&this->Model());
}

size_t EntityStatistics::RequiredBufferSize(const PFStatisticsEntityStatistics& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.entityKey)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.entityKey);
    }
    requiredSize += (alignof(PFStatisticsEntityStatisticValue*) + sizeof(PFStatisticsEntityStatisticValue*) * model.statisticsCount);
    for (size_t i = 0; i < model.statisticsCount; ++i)
    {
        requiredSize += EntityStatisticValue::RequiredBufferSize(*model.statistics[i]);
    }
    return requiredSize;
}

HRESULT EntityStatistics::Copy(const PFStatisticsEntityStatistics& input, PFStatisticsEntityStatistics& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.entityKey); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.entityKey = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<EntityStatisticValue>(input.statistics, input.statisticsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.statistics = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT GetStatisticsForEntitiesResponse::FromJson(const JsonValue& input)
{
    ModelDictionaryEntryVector<StatisticColumnCollection> columnDetails{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<StatisticColumnCollection>(input, "ColumnDetails", columnDetails));
    this->SetColumnDetails(std::move(columnDetails));

    ModelVector<EntityStatistics> entitiesStatistics{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<EntityStatistics>(input, "EntitiesStatistics", entitiesStatistics));
    this->SetEntitiesStatistics(std::move(entitiesStatistics));

    return S_OK;
}

size_t GetStatisticsForEntitiesResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFStatisticsGetStatisticsForEntitiesResponse const*> GetStatisticsForEntitiesResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetStatisticsForEntitiesResponse>(&this->Model());
}

size_t GetStatisticsForEntitiesResponse::RequiredBufferSize(const PFStatisticsGetStatisticsForEntitiesResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFStatisticsStatisticColumnCollectionDictionaryEntry) + sizeof(PFStatisticsStatisticColumnCollectionDictionaryEntry) * model.columnDetailsCount);
    for (size_t i = 0; i < model.columnDetailsCount; ++i)
    {
        requiredSize += (std::strlen(model.columnDetails[i].key) + 1);
        requiredSize += StatisticColumnCollection::RequiredBufferSize(*model.columnDetails[i].value);
    }
    requiredSize += (alignof(PFStatisticsEntityStatistics*) + sizeof(PFStatisticsEntityStatistics*) * model.entitiesStatisticsCount);
    for (size_t i = 0; i < model.entitiesStatisticsCount; ++i)
    {
        requiredSize += EntityStatistics::RequiredBufferSize(*model.entitiesStatistics[i]);
    }
    return requiredSize;
}

HRESULT GetStatisticsForEntitiesResponse::Copy(const PFStatisticsGetStatisticsForEntitiesResponse& input, PFStatisticsGetStatisticsForEntitiesResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToDictionary<StatisticColumnCollection>(input.columnDetails, input.columnDetailsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.columnDetails = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<EntityStatistics>(input.entitiesStatistics, input.entitiesStatisticsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.entitiesStatistics = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue IncrementStatisticVersionRequest::ToJson() const
{
    return IncrementStatisticVersionRequest::ToJson(this->Model());
}

JsonValue IncrementStatisticVersionRequest::ToJson(const PFStatisticsIncrementStatisticVersionRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    return output;
}

HRESULT IncrementStatisticVersionResponse::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Version", this->m_model.version));

    return S_OK;
}

size_t IncrementStatisticVersionResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFStatisticsIncrementStatisticVersionResponse const*> IncrementStatisticVersionResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<IncrementStatisticVersionResponse>(&this->Model());
}

size_t IncrementStatisticVersionResponse::RequiredBufferSize(const PFStatisticsIncrementStatisticVersionResponse& model)
{
    UNREFERENCED_PARAMETER(model); // Fixed size
    return sizeof(ModelType);
}

HRESULT IncrementStatisticVersionResponse::Copy(const PFStatisticsIncrementStatisticVersionResponse& input, PFStatisticsIncrementStatisticVersionResponse& output, ModelBuffer& buffer)
{
    output = input;
    UNREFERENCED_PARAMETER(buffer); // Fixed size
    return S_OK;
}

JsonValue ListStatisticDefinitionsRequest::ToJson() const
{
    return ListStatisticDefinitionsRequest::ToJson(this->Model());
}

JsonValue ListStatisticDefinitionsRequest::ToJson(const PFStatisticsListStatisticDefinitionsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    return output;
}

HRESULT StatisticDefinition::FromJson(const JsonValue& input)
{
    ModelVector<StatisticColumn> columns{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<StatisticColumn>(input, "Columns", columns));
    this->SetColumns(std::move(columns));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "Created", this->m_model.created));

    String entityType{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "EntityType", entityType));
    this->SetEntityType(std::move(entityType));

    std::optional<time_t> lastResetTime{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "LastResetTime", lastResetTime));
    this->SetLastResetTime(std::move(lastResetTime));

    CStringVector linkedLeaderboardNames{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "LinkedLeaderboardNames", linkedLeaderboardNames));
    this->SetLinkedLeaderboardNames(std::move(linkedLeaderboardNames));

    String name{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Name", name));
    this->SetName(std::move(name));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Version", this->m_model.version));

    std::optional<VersionConfiguration> versionConfiguration{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "VersionConfiguration", versionConfiguration));
    if (versionConfiguration)
    {
        this->SetVersionConfiguration(std::move(*versionConfiguration));
    }

    return S_OK;
}

size_t StatisticDefinition::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFStatisticsStatisticDefinition const*> StatisticDefinition::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<StatisticDefinition>(&this->Model());
}

size_t StatisticDefinition::RequiredBufferSize(const PFStatisticsStatisticDefinition& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFStatisticsStatisticColumn*) + sizeof(PFStatisticsStatisticColumn*) * model.columnsCount);
    for (size_t i = 0; i < model.columnsCount; ++i)
    {
        requiredSize += StatisticColumn::RequiredBufferSize(*model.columns[i]);
    }
    if (model.entityType)
    {
        requiredSize += (std::strlen(model.entityType) + 1);
    }
    if (model.lastResetTime)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.linkedLeaderboardNamesCount);
    for (size_t i = 0; i < model.linkedLeaderboardNamesCount; ++i)
    {
        requiredSize += (std::strlen(model.linkedLeaderboardNames[i]) + 1);
    }
    if (model.name)
    {
        requiredSize += (std::strlen(model.name) + 1);
    }
    if (model.versionConfiguration)
    {
        requiredSize += VersionConfiguration::RequiredBufferSize(*model.versionConfiguration);
    }
    return requiredSize;
}

HRESULT StatisticDefinition::Copy(const PFStatisticsStatisticDefinition& input, PFStatisticsStatisticDefinition& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<StatisticColumn>(input.columns, input.columnsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.columns = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.entityType); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.entityType = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.lastResetTime); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.lastResetTime = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.linkedLeaderboardNames, input.linkedLeaderboardNamesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.linkedLeaderboardNames = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.name); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.name = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<VersionConfiguration>(input.versionConfiguration); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.versionConfiguration = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT ListStatisticDefinitionsResponse::FromJson(const JsonValue& input)
{
    StringDictionaryEntryVector customTags{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "CustomTags", customTags));
    this->SetCustomTags(std::move(customTags));

    ModelVector<StatisticDefinition> statisticDefinitions{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<StatisticDefinition>(input, "StatisticDefinitions", statisticDefinitions));
    this->SetStatisticDefinitions(std::move(statisticDefinitions));

    return S_OK;
}

size_t ListStatisticDefinitionsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFStatisticsListStatisticDefinitionsResponse const*> ListStatisticDefinitionsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ListStatisticDefinitionsResponse>(&this->Model());
}

size_t ListStatisticDefinitionsResponse::RequiredBufferSize(const PFStatisticsListStatisticDefinitionsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFStringDictionaryEntry) + sizeof(PFStringDictionaryEntry) * model.customTagsCount);
    for (size_t i = 0; i < model.customTagsCount; ++i)
    {
        requiredSize += (std::strlen(model.customTags[i].key) + 1);
        requiredSize += (std::strlen(model.customTags[i].value) + 1);
    }
    requiredSize += (alignof(PFStatisticsStatisticDefinition*) + sizeof(PFStatisticsStatisticDefinition*) * model.statisticDefinitionsCount);
    for (size_t i = 0; i < model.statisticDefinitionsCount; ++i)
    {
        requiredSize += StatisticDefinition::RequiredBufferSize(*model.statisticDefinitions[i]);
    }
    return requiredSize;
}

HRESULT ListStatisticDefinitionsResponse::Copy(const PFStatisticsListStatisticDefinitionsResponse& input, PFStatisticsListStatisticDefinitionsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToDictionary(input.customTags, input.customTagsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.customTags = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<StatisticDefinition>(input.statisticDefinitions, input.statisticDefinitionsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.statisticDefinitions = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue StatisticUpdate::ToJson() const
{
    return StatisticUpdate::ToJson(this->Model());
}

JsonValue StatisticUpdate::ToJson(const PFStatisticsStatisticUpdate& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "Metadata", input.metadata);
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    JsonUtils::ObjectAddMemberArray(output, "Scores", input.scores, input.scoresCount);
    JsonUtils::ObjectAddMember(output, "Version", input.version);
    return output;
}

JsonValue UpdateStatisticsRequest::ToJson() const
{
    return UpdateStatisticsRequest::ToJson(this->Model());
}

JsonValue UpdateStatisticsRequest::ToJson(const PFStatisticsUpdateStatisticsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMemberArray<StatisticUpdate>(output, "Statistics", input.statistics, input.statisticsCount);
    return output;
}

HRESULT UpdateStatisticsResponse::FromJson(const JsonValue& input)
{
    ModelDictionaryEntryVector<StatisticColumnCollection> columnDetails{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<StatisticColumnCollection>(input, "ColumnDetails", columnDetails));
    this->SetColumnDetails(std::move(columnDetails));

    std::optional<EntityKey> entity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Entity", entity));
    if (entity)
    {
        this->SetEntity(std::move(*entity));
    }

    ModelDictionaryEntryVector<EntityStatisticValue> statistics{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<EntityStatisticValue>(input, "Statistics", statistics));
    this->SetStatistics(std::move(statistics));

    return S_OK;
}

size_t UpdateStatisticsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFStatisticsUpdateStatisticsResponse const*> UpdateStatisticsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UpdateStatisticsResponse>(&this->Model());
}

size_t UpdateStatisticsResponse::RequiredBufferSize(const PFStatisticsUpdateStatisticsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFStatisticsStatisticColumnCollectionDictionaryEntry) + sizeof(PFStatisticsStatisticColumnCollectionDictionaryEntry) * model.columnDetailsCount);
    for (size_t i = 0; i < model.columnDetailsCount; ++i)
    {
        requiredSize += (std::strlen(model.columnDetails[i].key) + 1);
        requiredSize += StatisticColumnCollection::RequiredBufferSize(*model.columnDetails[i].value);
    }
    if (model.entity)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.entity);
    }
    requiredSize += (alignof(PFStatisticsEntityStatisticValueDictionaryEntry) + sizeof(PFStatisticsEntityStatisticValueDictionaryEntry) * model.statisticsCount);
    for (size_t i = 0; i < model.statisticsCount; ++i)
    {
        requiredSize += (std::strlen(model.statistics[i].key) + 1);
        requiredSize += EntityStatisticValue::RequiredBufferSize(*model.statistics[i].value);
    }
    return requiredSize;
}

HRESULT UpdateStatisticsResponse::Copy(const PFStatisticsUpdateStatisticsResponse& input, PFStatisticsUpdateStatisticsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToDictionary<StatisticColumnCollection>(input.columnDetails, input.columnDetailsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.columnDetails = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.entity); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.entity = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary<EntityStatisticValue>(input.statistics, input.statisticsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.statistics = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

} // namespace Statistics

// Json serialization helpers

} // namespace PlayFab

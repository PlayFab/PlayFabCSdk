#include "stdafx.h"
#include "LeaderboardsTypes.h"
#include "JsonUtils.h"
#include "Types.h"

namespace PlayFab
{
namespace Leaderboards
{

JsonValue LinkedStatisticColumn::ToJson() const
{
    return LinkedStatisticColumn::ToJson(this->Model());
}

JsonValue LinkedStatisticColumn::ToJson(const PFLeaderboardsLinkedStatisticColumn& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "LinkedStatisticColumnName", input.linkedStatisticColumnName);
    JsonUtils::ObjectAddMember(output, "LinkedStatisticName", input.linkedStatisticName);
    return output;
}

HRESULT LinkedStatisticColumn::FromJson(const JsonValue& input)
{
    String linkedStatisticColumnName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "LinkedStatisticColumnName", linkedStatisticColumnName));
    this->SetLinkedStatisticColumnName(std::move(linkedStatisticColumnName));

    String linkedStatisticName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "LinkedStatisticName", linkedStatisticName));
    this->SetLinkedStatisticName(std::move(linkedStatisticName));

    return S_OK;
}

size_t LinkedStatisticColumn::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFLeaderboardsLinkedStatisticColumn const*> LinkedStatisticColumn::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<LinkedStatisticColumn>(&this->Model());
}

size_t LinkedStatisticColumn::RequiredBufferSize(const PFLeaderboardsLinkedStatisticColumn& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.linkedStatisticColumnName)
    {
        requiredSize += (std::strlen(model.linkedStatisticColumnName) + 1);
    }
    if (model.linkedStatisticName)
    {
        requiredSize += (std::strlen(model.linkedStatisticName) + 1);
    }
    return requiredSize;
}

HRESULT LinkedStatisticColumn::Copy(const PFLeaderboardsLinkedStatisticColumn& input, PFLeaderboardsLinkedStatisticColumn& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.linkedStatisticColumnName); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.linkedStatisticColumnName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.linkedStatisticName); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.linkedStatisticName = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue LeaderboardColumn::ToJson() const
{
    return LeaderboardColumn::ToJson(this->Model());
}

JsonValue LeaderboardColumn::ToJson(const PFLeaderboardsLeaderboardColumn& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember<LinkedStatisticColumn>(output, "LinkedStatisticColumn", input.linkedStatisticColumn);
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    JsonUtils::ObjectAddMember(output, "SortDirection", input.sortDirection);
    return output;
}

HRESULT LeaderboardColumn::FromJson(const JsonValue& input)
{
    std::optional<LinkedStatisticColumn> linkedStatisticColumn{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "LinkedStatisticColumn", linkedStatisticColumn));
    if (linkedStatisticColumn)
    {
        this->SetLinkedStatisticColumn(std::move(*linkedStatisticColumn));
    }

    String name{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Name", name));
    this->SetName(std::move(name));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SortDirection", this->m_model.sortDirection));

    return S_OK;
}

size_t LeaderboardColumn::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFLeaderboardsLeaderboardColumn const*> LeaderboardColumn::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<LeaderboardColumn>(&this->Model());
}

size_t LeaderboardColumn::RequiredBufferSize(const PFLeaderboardsLeaderboardColumn& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.linkedStatisticColumn)
    {
        requiredSize += LinkedStatisticColumn::RequiredBufferSize(*model.linkedStatisticColumn);
    }
    if (model.name)
    {
        requiredSize += (std::strlen(model.name) + 1);
    }
    return requiredSize;
}

HRESULT LeaderboardColumn::Copy(const PFLeaderboardsLeaderboardColumn& input, PFLeaderboardsLeaderboardColumn& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<LinkedStatisticColumn>(input.linkedStatisticColumn); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.linkedStatisticColumn = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.name); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.name = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue CreateLeaderboardDefinitionRequest::ToJson() const
{
    return CreateLeaderboardDefinitionRequest::ToJson(this->Model());
}

JsonValue CreateLeaderboardDefinitionRequest::ToJson(const PFLeaderboardsCreateLeaderboardDefinitionRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberArray<LeaderboardColumn>(output, "Columns", input.columns, input.columnsCount);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "EntityType", input.entityType);
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    JsonUtils::ObjectAddMember(output, "SizeLimit", input.sizeLimit);
    JsonUtils::ObjectAddMember<VersionConfiguration>(output, "VersionConfiguration", input.versionConfiguration);
    return output;
}

JsonValue DeleteLeaderboardDefinitionRequest::ToJson() const
{
    return DeleteLeaderboardDefinitionRequest::ToJson(this->Model());
}

JsonValue DeleteLeaderboardDefinitionRequest::ToJson(const PFLeaderboardsDeleteLeaderboardDefinitionRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    return output;
}

JsonValue DeleteLeaderboardEntriesRequest::ToJson() const
{
    return DeleteLeaderboardEntriesRequest::ToJson(this->Model());
}

JsonValue DeleteLeaderboardEntriesRequest::ToJson(const PFLeaderboardsDeleteLeaderboardEntriesRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMemberArray(output, "EntityIds", input.entityIds, input.entityIdsCount);
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    return output;
}

JsonValue GetFriendLeaderboardForEntityRequest::ToJson() const
{
    return GetFriendLeaderboardForEntityRequest::ToJson(this->Model());
}

JsonValue GetFriendLeaderboardForEntityRequest::ToJson(const PFLeaderboardsGetFriendLeaderboardForEntityRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "ExternalFriendSources", JsonUtils::ToJson(input.externalFriendSources));
    JsonUtils::ObjectAddMember(output, "LeaderboardName", input.leaderboardName);
    JsonUtils::ObjectAddMember(output, "Version", input.version);
    JsonUtils::ObjectAddMember(output, "XboxToken", input.xboxToken);
    return output;
}

HRESULT EntityLeaderboardEntry::FromJson(const JsonValue& input)
{
    String displayName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DisplayName", displayName));
    this->SetDisplayName(std::move(displayName));

    std::optional<EntityKey> entity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Entity", entity));
    if (entity)
    {
        this->SetEntity(std::move(*entity));
    }

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "LastUpdated", this->m_model.lastUpdated));

    String metadata{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Metadata", metadata));
    this->SetMetadata(std::move(metadata));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Rank", this->m_model.rank));

    CStringVector scores{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Scores", scores));
    this->SetScores(std::move(scores));

    return S_OK;
}

size_t EntityLeaderboardEntry::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFLeaderboardsEntityLeaderboardEntry const*> EntityLeaderboardEntry::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<EntityLeaderboardEntry>(&this->Model());
}

size_t EntityLeaderboardEntry::RequiredBufferSize(const PFLeaderboardsEntityLeaderboardEntry& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.displayName)
    {
        requiredSize += (std::strlen(model.displayName) + 1);
    }
    if (model.entity)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.entity);
    }
    if (model.metadata)
    {
        requiredSize += (std::strlen(model.metadata) + 1);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.scoresCount);
    for (size_t i = 0; i < model.scoresCount; ++i)
    {
        requiredSize += (std::strlen(model.scores[i]) + 1);
    }
    return requiredSize;
}

HRESULT EntityLeaderboardEntry::Copy(const PFLeaderboardsEntityLeaderboardEntry& input, PFLeaderboardsEntityLeaderboardEntry& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.displayName); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.displayName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.entity); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.entity = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.metadata); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.metadata = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.scores, input.scoresCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.scores = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT GetEntityLeaderboardResponse::FromJson(const JsonValue& input)
{
    ModelVector<LeaderboardColumn> columns{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<LeaderboardColumn>(input, "Columns", columns));
    this->SetColumns(std::move(columns));

    ModelVector<EntityLeaderboardEntry> rankings{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<EntityLeaderboardEntry>(input, "Rankings", rankings));
    this->SetRankings(std::move(rankings));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Version", this->m_model.version));

    return S_OK;
}

size_t GetEntityLeaderboardResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFLeaderboardsGetEntityLeaderboardResponse const*> GetEntityLeaderboardResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetEntityLeaderboardResponse>(&this->Model());
}

size_t GetEntityLeaderboardResponse::RequiredBufferSize(const PFLeaderboardsGetEntityLeaderboardResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFLeaderboardsLeaderboardColumn*) + sizeof(PFLeaderboardsLeaderboardColumn*) * model.columnsCount);
    for (size_t i = 0; i < model.columnsCount; ++i)
    {
        requiredSize += LeaderboardColumn::RequiredBufferSize(*model.columns[i]);
    }
    requiredSize += (alignof(PFLeaderboardsEntityLeaderboardEntry*) + sizeof(PFLeaderboardsEntityLeaderboardEntry*) * model.rankingsCount);
    for (size_t i = 0; i < model.rankingsCount; ++i)
    {
        requiredSize += EntityLeaderboardEntry::RequiredBufferSize(*model.rankings[i]);
    }
    return requiredSize;
}

HRESULT GetEntityLeaderboardResponse::Copy(const PFLeaderboardsGetEntityLeaderboardResponse& input, PFLeaderboardsGetEntityLeaderboardResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<LeaderboardColumn>(input.columns, input.columnsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.columns = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<EntityLeaderboardEntry>(input.rankings, input.rankingsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.rankings = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetEntityLeaderboardRequest::ToJson() const
{
    return GetEntityLeaderboardRequest::ToJson(this->Model());
}

JsonValue GetEntityLeaderboardRequest::ToJson(const PFLeaderboardsGetEntityLeaderboardRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "LeaderboardName", input.leaderboardName);
    JsonUtils::ObjectAddMember(output, "PageSize", input.pageSize);
    JsonUtils::ObjectAddMember(output, "StartingPosition", input.startingPosition);
    JsonUtils::ObjectAddMember(output, "Version", input.version);
    return output;
}

JsonValue GetLeaderboardAroundEntityRequest::ToJson() const
{
    return GetLeaderboardAroundEntityRequest::ToJson(this->Model());
}

JsonValue GetLeaderboardAroundEntityRequest::ToJson(const PFLeaderboardsGetLeaderboardAroundEntityRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "LeaderboardName", input.leaderboardName);
    JsonUtils::ObjectAddMember(output, "MaxSurroundingEntries", input.maxSurroundingEntries);
    JsonUtils::ObjectAddMember(output, "Version", input.version);
    return output;
}

JsonValue GetLeaderboardDefinitionRequest::ToJson() const
{
    return GetLeaderboardDefinitionRequest::ToJson(this->Model());
}

JsonValue GetLeaderboardDefinitionRequest::ToJson(const PFLeaderboardsGetLeaderboardDefinitionRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    return output;
}

HRESULT GetLeaderboardDefinitionResponse::FromJson(const JsonValue& input)
{
    ModelVector<LeaderboardColumn> columns{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<LeaderboardColumn>(input, "Columns", columns));
    this->SetColumns(std::move(columns));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "Created", this->m_model.created));

    String entityType{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "EntityType", entityType));
    this->SetEntityType(std::move(entityType));

    std::optional<time_t> lastResetTime{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "LastResetTime", lastResetTime));
    this->SetLastResetTime(std::move(lastResetTime));

    String name{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Name", name));
    this->SetName(std::move(name));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SizeLimit", this->m_model.sizeLimit));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Version", this->m_model.version));

    VersionConfiguration versionConfiguration{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "VersionConfiguration", versionConfiguration));
    this->SetVersionConfiguration(std::move(versionConfiguration));

    return S_OK;
}

size_t GetLeaderboardDefinitionResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFLeaderboardsGetLeaderboardDefinitionResponse const*> GetLeaderboardDefinitionResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetLeaderboardDefinitionResponse>(&this->Model());
}

size_t GetLeaderboardDefinitionResponse::RequiredBufferSize(const PFLeaderboardsGetLeaderboardDefinitionResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFLeaderboardsLeaderboardColumn*) + sizeof(PFLeaderboardsLeaderboardColumn*) * model.columnsCount);
    for (size_t i = 0; i < model.columnsCount; ++i)
    {
        requiredSize += LeaderboardColumn::RequiredBufferSize(*model.columns[i]);
    }
    if (model.entityType)
    {
        requiredSize += (std::strlen(model.entityType) + 1);
    }
    if (model.lastResetTime)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
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

HRESULT GetLeaderboardDefinitionResponse::Copy(const PFLeaderboardsGetLeaderboardDefinitionResponse& input, PFLeaderboardsGetLeaderboardDefinitionResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<LeaderboardColumn>(input.columns, input.columnsCount);
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

JsonValue GetLeaderboardForEntitiesRequest::ToJson() const
{
    return GetLeaderboardForEntitiesRequest::ToJson(this->Model());
}

JsonValue GetLeaderboardForEntitiesRequest::ToJson(const PFLeaderboardsGetLeaderboardForEntitiesRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMemberArray(output, "EntityIds", input.entityIds, input.entityIdsCount);
    JsonUtils::ObjectAddMember(output, "LeaderboardName", input.leaderboardName);
    JsonUtils::ObjectAddMember(output, "Version", input.version);
    return output;
}

JsonValue IncrementLeaderboardVersionRequest::ToJson() const
{
    return IncrementLeaderboardVersionRequest::ToJson(this->Model());
}

JsonValue IncrementLeaderboardVersionRequest::ToJson(const PFLeaderboardsIncrementLeaderboardVersionRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    return output;
}

HRESULT IncrementLeaderboardVersionResponse::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Version", this->m_model.version));

    return S_OK;
}

size_t IncrementLeaderboardVersionResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFLeaderboardsIncrementLeaderboardVersionResponse const*> IncrementLeaderboardVersionResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<IncrementLeaderboardVersionResponse>(&this->Model());
}

size_t IncrementLeaderboardVersionResponse::RequiredBufferSize(const PFLeaderboardsIncrementLeaderboardVersionResponse& model)
{
    UNREFERENCED_PARAMETER(model); // Fixed size
    return sizeof(ModelType);
}

HRESULT IncrementLeaderboardVersionResponse::Copy(const PFLeaderboardsIncrementLeaderboardVersionResponse& input, PFLeaderboardsIncrementLeaderboardVersionResponse& output, ModelBuffer& buffer)
{
    output = input;
    UNREFERENCED_PARAMETER(buffer); // Fixed size
    return S_OK;
}

JsonValue ListLeaderboardDefinitionsRequest::ToJson() const
{
    return ListLeaderboardDefinitionsRequest::ToJson(this->Model());
}

JsonValue ListLeaderboardDefinitionsRequest::ToJson(const PFLeaderboardsListLeaderboardDefinitionsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    return output;
}

HRESULT LeaderboardDefinition::FromJson(const JsonValue& input)
{
    ModelVector<LeaderboardColumn> columns{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<LeaderboardColumn>(input, "Columns", columns));
    this->SetColumns(std::move(columns));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "Created", this->m_model.created));

    String entityType{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "EntityType", entityType));
    this->SetEntityType(std::move(entityType));

    std::optional<time_t> lastResetTime{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "LastResetTime", lastResetTime));
    this->SetLastResetTime(std::move(lastResetTime));

    String name{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Name", name));
    this->SetName(std::move(name));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SizeLimit", this->m_model.sizeLimit));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Version", this->m_model.version));

    VersionConfiguration versionConfiguration{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "VersionConfiguration", versionConfiguration));
    this->SetVersionConfiguration(std::move(versionConfiguration));

    return S_OK;
}

size_t LeaderboardDefinition::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFLeaderboardsLeaderboardDefinition const*> LeaderboardDefinition::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<LeaderboardDefinition>(&this->Model());
}

size_t LeaderboardDefinition::RequiredBufferSize(const PFLeaderboardsLeaderboardDefinition& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFLeaderboardsLeaderboardColumn*) + sizeof(PFLeaderboardsLeaderboardColumn*) * model.columnsCount);
    for (size_t i = 0; i < model.columnsCount; ++i)
    {
        requiredSize += LeaderboardColumn::RequiredBufferSize(*model.columns[i]);
    }
    if (model.entityType)
    {
        requiredSize += (std::strlen(model.entityType) + 1);
    }
    if (model.lastResetTime)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
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

HRESULT LeaderboardDefinition::Copy(const PFLeaderboardsLeaderboardDefinition& input, PFLeaderboardsLeaderboardDefinition& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<LeaderboardColumn>(input.columns, input.columnsCount);
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

HRESULT ListLeaderboardDefinitionsResponse::FromJson(const JsonValue& input)
{
    ModelVector<LeaderboardDefinition> leaderboardDefinitions{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<LeaderboardDefinition>(input, "LeaderboardDefinitions", leaderboardDefinitions));
    this->SetLeaderboardDefinitions(std::move(leaderboardDefinitions));

    return S_OK;
}

size_t ListLeaderboardDefinitionsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFLeaderboardsListLeaderboardDefinitionsResponse const*> ListLeaderboardDefinitionsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ListLeaderboardDefinitionsResponse>(&this->Model());
}

size_t ListLeaderboardDefinitionsResponse::RequiredBufferSize(const PFLeaderboardsListLeaderboardDefinitionsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFLeaderboardsLeaderboardDefinition*) + sizeof(PFLeaderboardsLeaderboardDefinition*) * model.leaderboardDefinitionsCount);
    for (size_t i = 0; i < model.leaderboardDefinitionsCount; ++i)
    {
        requiredSize += LeaderboardDefinition::RequiredBufferSize(*model.leaderboardDefinitions[i]);
    }
    return requiredSize;
}

HRESULT ListLeaderboardDefinitionsResponse::Copy(const PFLeaderboardsListLeaderboardDefinitionsResponse& input, PFLeaderboardsListLeaderboardDefinitionsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<LeaderboardDefinition>(input.leaderboardDefinitions, input.leaderboardDefinitionsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.leaderboardDefinitions = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue UnlinkLeaderboardFromStatisticRequest::ToJson() const
{
    return UnlinkLeaderboardFromStatisticRequest::ToJson(this->Model());
}

JsonValue UnlinkLeaderboardFromStatisticRequest::ToJson(const PFLeaderboardsUnlinkLeaderboardFromStatisticRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    JsonUtils::ObjectAddMember(output, "StatisticName", input.statisticName);
    return output;
}

JsonValue LeaderboardEntryUpdate::ToJson() const
{
    return LeaderboardEntryUpdate::ToJson(this->Model());
}

JsonValue LeaderboardEntryUpdate::ToJson(const PFLeaderboardsLeaderboardEntryUpdate& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "EntityId", input.entityId);
    JsonUtils::ObjectAddMember(output, "Metadata", input.metadata);
    JsonUtils::ObjectAddMemberArray(output, "Scores", input.scores, input.scoresCount);
    return output;
}

JsonValue UpdateLeaderboardEntriesRequest::ToJson() const
{
    return UpdateLeaderboardEntriesRequest::ToJson(this->Model());
}

JsonValue UpdateLeaderboardEntriesRequest::ToJson(const PFLeaderboardsUpdateLeaderboardEntriesRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMemberArray<LeaderboardEntryUpdate>(output, "Entries", input.entries, input.entriesCount);
    JsonUtils::ObjectAddMember(output, "LeaderboardName", input.leaderboardName);
    return output;
}

} // namespace Leaderboards

// Json serialization helpers
namespace JsonUtils
{

JsonValue ToJson(PFExternalFriendSources const* input)
{
    if (input)
    {
        String separator{};
        Stringstream ss;
        if (*input == PFExternalFriendSources::None)
        {
            return JsonValue{ EnumName(PFExternalFriendSources::None), JsonUtils::allocator };
        }
        if ((*input & PFExternalFriendSources::Steam) == PFExternalFriendSources::Steam)
        {
            ss << separator << EnumName(PFExternalFriendSources::Steam);
            separator = ",";
        }
        if ((*input & PFExternalFriendSources::Facebook) == PFExternalFriendSources::Facebook)
        {
            ss << separator << EnumName(PFExternalFriendSources::Facebook);
            separator = ",";
        }
        if ((*input & PFExternalFriendSources::Xbox) == PFExternalFriendSources::Xbox)
        {
            ss << separator << EnumName(PFExternalFriendSources::Xbox);
            separator = ",";
        }
        if ((*input & PFExternalFriendSources::Psn) == PFExternalFriendSources::Psn)
        {
            ss << separator << EnumName(PFExternalFriendSources::Psn);
            separator = ",";
        }
        if ((*input & PFExternalFriendSources::All) == PFExternalFriendSources::All)
        {
            ss << separator << EnumName(PFExternalFriendSources::All);
            separator = ",";
        }
        return JsonValue{ ss.str().data(), JsonUtils::allocator };
    }
    else
    {
        return JsonValue{ rapidjson::kNullType };
    }
}

} // namespace JsonUtils

} // namespace PlayFab

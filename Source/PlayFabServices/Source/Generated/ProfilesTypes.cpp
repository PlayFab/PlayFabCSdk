#include "stdafx.h"
#include "ProfilesTypes.h"
#include "JsonUtils.h"
#include "Types.h"

namespace PlayFab
{
namespace Profiles
{

JsonValue GetEntityProfileRequest::ToJson() const
{
    return GetEntityProfileRequest::ToJson(this->Model());
}

JsonValue GetEntityProfileRequest::ToJson(const PFProfilesGetEntityProfileRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "DataAsObject", input.dataAsObject);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    return output;
}

HRESULT EntityProfileFileMetadata::FromJson(const JsonValue& input)
{
    String checksum{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Checksum", checksum));
    this->SetChecksum(std::move(checksum));

    String fileName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FileName", fileName));
    this->SetFileName(std::move(fileName));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "LastModified", this->m_model.lastModified));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Size", this->m_model.size));

    return S_OK;
}

size_t EntityProfileFileMetadata::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFProfilesEntityProfileFileMetadata const*> EntityProfileFileMetadata::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<EntityProfileFileMetadata>(&this->Model());
}

size_t EntityProfileFileMetadata::RequiredBufferSize(const PFProfilesEntityProfileFileMetadata& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.checksum)
    {
        requiredSize += (std::strlen(model.checksum) + 1);
    }
    if (model.fileName)
    {
        requiredSize += (std::strlen(model.fileName) + 1);
    }
    return requiredSize;
}

HRESULT EntityProfileFileMetadata::Copy(const PFProfilesEntityProfileFileMetadata& input, PFProfilesEntityProfileFileMetadata& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.checksum); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.checksum = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.fileName); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.fileName = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT EntityDataObject::FromJson(const JsonValue& input)
{
    JsonObject dataObject{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DataObject", dataObject));
    this->SetDataObject(std::move(dataObject));

    String escapedDataObject{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "EscapedDataObject", escapedDataObject));
    this->SetEscapedDataObject(std::move(escapedDataObject));

    String objectName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ObjectName", objectName));
    this->SetObjectName(std::move(objectName));

    return S_OK;
}

size_t EntityDataObject::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFProfilesEntityDataObject const*> EntityDataObject::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<EntityDataObject>(&this->Model());
}

size_t EntityDataObject::RequiredBufferSize(const PFProfilesEntityDataObject& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.dataObject.stringValue)
    {
    requiredSize += (std::strlen(model.dataObject.stringValue) + 1);
    }
    if (model.escapedDataObject)
    {
        requiredSize += (std::strlen(model.escapedDataObject) + 1);
    }
    if (model.objectName)
    {
        requiredSize += (std::strlen(model.objectName) + 1);
    }
    return requiredSize;
}

HRESULT EntityDataObject::Copy(const PFProfilesEntityDataObject& input, PFProfilesEntityDataObject& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.dataObject.stringValue);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.dataObject.stringValue = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.escapedDataObject); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.escapedDataObject = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.objectName); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.objectName = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue EntityPermissionStatement::ToJson() const
{
    return EntityPermissionStatement::ToJson(this->Model());
}

JsonValue EntityPermissionStatement::ToJson(const PFProfilesEntityPermissionStatement& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "Action", input.action);
    JsonUtils::ObjectAddMember(output, "Comment", input.comment);
    JsonUtils::ObjectAddMember(output, "Condition", input.condition);
    JsonUtils::ObjectAddMember(output, "Effect", input.effect);
    JsonUtils::ObjectAddMember(output, "Principal", input.principal);
    JsonUtils::ObjectAddMember(output, "Resource", input.resource);
    return output;
}

HRESULT EntityPermissionStatement::FromJson(const JsonValue& input)
{
    String action{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Action", action));
    this->SetAction(std::move(action));

    String comment{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Comment", comment));
    this->SetComment(std::move(comment));

    JsonObject condition{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Condition", condition));
    this->SetCondition(std::move(condition));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Effect", this->m_model.effect));

    JsonObject principal{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Principal", principal));
    this->SetPrincipal(std::move(principal));

    String resource{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Resource", resource));
    this->SetResource(std::move(resource));

    return S_OK;
}

size_t EntityPermissionStatement::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFProfilesEntityPermissionStatement const*> EntityPermissionStatement::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<EntityPermissionStatement>(&this->Model());
}

size_t EntityPermissionStatement::RequiredBufferSize(const PFProfilesEntityPermissionStatement& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.action)
    {
        requiredSize += (std::strlen(model.action) + 1);
    }
    if (model.comment)
    {
        requiredSize += (std::strlen(model.comment) + 1);
    }
    if (model.condition.stringValue)
    {
    requiredSize += (std::strlen(model.condition.stringValue) + 1);
    }
    if (model.principal.stringValue)
    {
    requiredSize += (std::strlen(model.principal.stringValue) + 1);
    }
    if (model.resource)
    {
        requiredSize += (std::strlen(model.resource) + 1);
    }
    return requiredSize;
}

HRESULT EntityPermissionStatement::Copy(const PFProfilesEntityPermissionStatement& input, PFProfilesEntityPermissionStatement& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.action); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.action = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.comment); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.comment = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.condition.stringValue);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.condition.stringValue = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.principal.stringValue);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.principal.stringValue = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.resource); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.resource = propCopyResult.ExtractPayload();
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

Result<PFEntityStatisticValue const*> EntityStatisticValue::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<EntityStatisticValue>(&this->Model());
}

size_t EntityStatisticValue::RequiredBufferSize(const PFEntityStatisticValue& model)
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

HRESULT EntityStatisticValue::Copy(const PFEntityStatisticValue& input, PFEntityStatisticValue& output, ModelBuffer& buffer)
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

HRESULT EntityProfileBody::FromJson(const JsonValue& input)
{
    String avatarUrl{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "AvatarUrl", avatarUrl));
    this->SetAvatarUrl(std::move(avatarUrl));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "Created", this->m_model.created));

    String displayName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DisplayName", displayName));
    this->SetDisplayName(std::move(displayName));

    std::optional<EntityKey> entity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Entity", entity));
    if (entity)
    {
        this->SetEntity(std::move(*entity));
    }

    String entityChain{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "EntityChain", entityChain));
    this->SetEntityChain(std::move(entityChain));

    CStringVector experimentVariants{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ExperimentVariants", experimentVariants));
    this->SetExperimentVariants(std::move(experimentVariants));

    ModelDictionaryEntryVector<EntityProfileFileMetadata> files{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<EntityProfileFileMetadata>(input, "Files", files));
    this->SetFiles(std::move(files));

    String language{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Language", language));
    this->SetLanguage(std::move(language));

    std::optional<EntityLineage> lineage{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Lineage", lineage));
    if (lineage)
    {
        this->SetLineage(std::move(*lineage));
    }

    ModelDictionaryEntryVector<EntityDataObject> objects{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<EntityDataObject>(input, "Objects", objects));
    this->SetObjects(std::move(objects));

    ModelVector<EntityPermissionStatement> permissions{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<EntityPermissionStatement>(input, "Permissions", permissions));
    this->SetPermissions(std::move(permissions));

    ModelDictionaryEntryVector<EntityStatisticValue> statistics{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<EntityStatisticValue>(input, "Statistics", statistics));
    this->SetStatistics(std::move(statistics));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "VersionNumber", this->m_model.versionNumber));

    return S_OK;
}

size_t EntityProfileBody::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFProfilesEntityProfileBody const*> EntityProfileBody::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<EntityProfileBody>(&this->Model());
}

size_t EntityProfileBody::RequiredBufferSize(const PFProfilesEntityProfileBody& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.avatarUrl)
    {
        requiredSize += (std::strlen(model.avatarUrl) + 1);
    }
    if (model.displayName)
    {
        requiredSize += (std::strlen(model.displayName) + 1);
    }
    if (model.entity)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.entity);
    }
    if (model.entityChain)
    {
        requiredSize += (std::strlen(model.entityChain) + 1);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.experimentVariantsCount);
    for (size_t i = 0; i < model.experimentVariantsCount; ++i)
    {
        requiredSize += (std::strlen(model.experimentVariants[i]) + 1);
    }
    requiredSize += (alignof(PFProfilesEntityProfileFileMetadataDictionaryEntry) + sizeof(PFProfilesEntityProfileFileMetadataDictionaryEntry) * model.filesCount);
    for (size_t i = 0; i < model.filesCount; ++i)
    {
        requiredSize += (std::strlen(model.files[i].key) + 1);
        requiredSize += EntityProfileFileMetadata::RequiredBufferSize(*model.files[i].value);
    }
    if (model.language)
    {
        requiredSize += (std::strlen(model.language) + 1);
    }
    if (model.lineage)
    {
        requiredSize += EntityLineage::RequiredBufferSize(*model.lineage);
    }
    requiredSize += (alignof(PFProfilesEntityDataObjectDictionaryEntry) + sizeof(PFProfilesEntityDataObjectDictionaryEntry) * model.objectsCount);
    for (size_t i = 0; i < model.objectsCount; ++i)
    {
        requiredSize += (std::strlen(model.objects[i].key) + 1);
        requiredSize += EntityDataObject::RequiredBufferSize(*model.objects[i].value);
    }
    requiredSize += (alignof(PFProfilesEntityPermissionStatement*) + sizeof(PFProfilesEntityPermissionStatement*) * model.permissionsCount);
    for (size_t i = 0; i < model.permissionsCount; ++i)
    {
        requiredSize += EntityPermissionStatement::RequiredBufferSize(*model.permissions[i]);
    }
    requiredSize += (alignof(PFEntityStatisticValueDictionaryEntry) + sizeof(PFEntityStatisticValueDictionaryEntry) * model.statisticsCount);
    for (size_t i = 0; i < model.statisticsCount; ++i)
    {
        requiredSize += (std::strlen(model.statistics[i].key) + 1);
        requiredSize += EntityStatisticValue::RequiredBufferSize(*model.statistics[i].value);
    }
    return requiredSize;
}

HRESULT EntityProfileBody::Copy(const PFProfilesEntityProfileBody& input, PFProfilesEntityProfileBody& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.avatarUrl); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.avatarUrl = propCopyResult.ExtractPayload();
    }
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
        auto propCopyResult = buffer.CopyTo(input.entityChain); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.entityChain = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.experimentVariants, input.experimentVariantsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.experimentVariants = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary<EntityProfileFileMetadata>(input.files, input.filesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.files = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.language); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.language = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<EntityLineage>(input.lineage); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.lineage = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary<EntityDataObject>(input.objects, input.objectsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.objects = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<EntityPermissionStatement>(input.permissions, input.permissionsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.permissions = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary<EntityStatisticValue>(input.statistics, input.statisticsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.statistics = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT GetEntityProfileResponse::FromJson(const JsonValue& input)
{
    std::optional<EntityProfileBody> profile{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Profile", profile));
    if (profile)
    {
        this->SetProfile(std::move(*profile));
    }

    return S_OK;
}

size_t GetEntityProfileResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFProfilesGetEntityProfileResponse const*> GetEntityProfileResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetEntityProfileResponse>(&this->Model());
}

size_t GetEntityProfileResponse::RequiredBufferSize(const PFProfilesGetEntityProfileResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.profile)
    {
        requiredSize += EntityProfileBody::RequiredBufferSize(*model.profile);
    }
    return requiredSize;
}

HRESULT GetEntityProfileResponse::Copy(const PFProfilesGetEntityProfileResponse& input, PFProfilesGetEntityProfileResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<EntityProfileBody>(input.profile); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.profile = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetEntityProfilesRequest::ToJson() const
{
    return GetEntityProfilesRequest::ToJson(this->Model());
}

JsonValue GetEntityProfilesRequest::ToJson(const PFProfilesGetEntityProfilesRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "DataAsObject", input.dataAsObject);
    JsonUtils::ObjectAddMemberArray<EntityKey>(output, "Entities", input.entities, input.entitiesCount);
    return output;
}

HRESULT GetEntityProfilesResponse::FromJson(const JsonValue& input)
{
    ModelVector<EntityProfileBody> profiles{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<EntityProfileBody>(input, "Profiles", profiles));
    this->SetProfiles(std::move(profiles));

    return S_OK;
}

size_t GetEntityProfilesResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFProfilesGetEntityProfilesResponse const*> GetEntityProfilesResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetEntityProfilesResponse>(&this->Model());
}

size_t GetEntityProfilesResponse::RequiredBufferSize(const PFProfilesGetEntityProfilesResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFProfilesEntityProfileBody*) + sizeof(PFProfilesEntityProfileBody*) * model.profilesCount);
    for (size_t i = 0; i < model.profilesCount; ++i)
    {
        requiredSize += EntityProfileBody::RequiredBufferSize(*model.profiles[i]);
    }
    return requiredSize;
}

HRESULT GetEntityProfilesResponse::Copy(const PFProfilesGetEntityProfilesResponse& input, PFProfilesGetEntityProfilesResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<EntityProfileBody>(input.profiles, input.profilesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.profiles = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetTitlePlayersFromMasterPlayerAccountIdsRequest::ToJson() const
{
    return GetTitlePlayersFromMasterPlayerAccountIdsRequest::ToJson(this->Model());
}

JsonValue GetTitlePlayersFromMasterPlayerAccountIdsRequest::ToJson(const PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMemberArray(output, "MasterPlayerAccountIds", input.masterPlayerAccountIds, input.masterPlayerAccountIdsCount);
    JsonUtils::ObjectAddMember(output, "TitleId", input.titleId);
    return output;
}

HRESULT GetTitlePlayersFromMasterPlayerAccountIdsResponse::FromJson(const JsonValue& input)
{
    String titleId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TitleId", titleId));
    this->SetTitleId(std::move(titleId));

    ModelDictionaryEntryVector<EntityKey> titlePlayerAccounts{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<EntityKey>(input, "TitlePlayerAccounts", titlePlayerAccounts));
    this->SetTitlePlayerAccounts(std::move(titlePlayerAccounts));

    return S_OK;
}

size_t GetTitlePlayersFromMasterPlayerAccountIdsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsResponse const*> GetTitlePlayersFromMasterPlayerAccountIdsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetTitlePlayersFromMasterPlayerAccountIdsResponse>(&this->Model());
}

size_t GetTitlePlayersFromMasterPlayerAccountIdsResponse::RequiredBufferSize(const PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.titleId)
    {
        requiredSize += (std::strlen(model.titleId) + 1);
    }
    requiredSize += (alignof(PFEntityKeyDictionaryEntry) + sizeof(PFEntityKeyDictionaryEntry) * model.titlePlayerAccountsCount);
    for (size_t i = 0; i < model.titlePlayerAccountsCount; ++i)
    {
        requiredSize += (std::strlen(model.titlePlayerAccounts[i].key) + 1);
        requiredSize += EntityKey::RequiredBufferSize(*model.titlePlayerAccounts[i].value);
    }
    return requiredSize;
}

HRESULT GetTitlePlayersFromMasterPlayerAccountIdsResponse::Copy(const PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsResponse& input, PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.titleId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.titleId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary<EntityKey>(input.titlePlayerAccounts, input.titlePlayerAccountsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.titlePlayerAccounts = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue SetProfileLanguageRequest::ToJson() const
{
    return SetProfileLanguageRequest::ToJson(this->Model());
}

JsonValue SetProfileLanguageRequest::ToJson(const PFProfilesSetProfileLanguageRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "ExpectedVersion", input.expectedVersion);
    JsonUtils::ObjectAddMember(output, "Language", input.language);
    return output;
}

HRESULT SetProfileLanguageResponse::FromJson(const JsonValue& input)
{
    std::optional<PFOperationTypes> operationResult{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "OperationResult", operationResult));
    this->SetOperationResult(std::move(operationResult));

    std::optional<int32_t> versionNumber{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "VersionNumber", versionNumber));
    this->SetVersionNumber(std::move(versionNumber));

    return S_OK;
}

size_t SetProfileLanguageResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFProfilesSetProfileLanguageResponse const*> SetProfileLanguageResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<SetProfileLanguageResponse>(&this->Model());
}

size_t SetProfileLanguageResponse::RequiredBufferSize(const PFProfilesSetProfileLanguageResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.operationResult)
    {
        requiredSize += (alignof(PFOperationTypes) + sizeof(PFOperationTypes));
    }
    if (model.versionNumber)
    {
        requiredSize += (alignof(int32_t) + sizeof(int32_t));
    }
    return requiredSize;
}

HRESULT SetProfileLanguageResponse::Copy(const PFProfilesSetProfileLanguageResponse& input, PFProfilesSetProfileLanguageResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.operationResult); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.operationResult = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.versionNumber); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.versionNumber = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue SetEntityProfilePolicyRequest::ToJson() const
{
    return SetEntityProfilePolicyRequest::ToJson(this->Model());
}

JsonValue SetEntityProfilePolicyRequest::ToJson(const PFProfilesSetEntityProfilePolicyRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMemberArray<EntityPermissionStatement>(output, "Statements", input.statements, input.statementsCount);
    return output;
}

HRESULT SetEntityProfilePolicyResponse::FromJson(const JsonValue& input)
{
    ModelVector<EntityPermissionStatement> permissions{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<EntityPermissionStatement>(input, "Permissions", permissions));
    this->SetPermissions(std::move(permissions));

    return S_OK;
}

size_t SetEntityProfilePolicyResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFProfilesSetEntityProfilePolicyResponse const*> SetEntityProfilePolicyResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<SetEntityProfilePolicyResponse>(&this->Model());
}

size_t SetEntityProfilePolicyResponse::RequiredBufferSize(const PFProfilesSetEntityProfilePolicyResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFProfilesEntityPermissionStatement*) + sizeof(PFProfilesEntityPermissionStatement*) * model.permissionsCount);
    for (size_t i = 0; i < model.permissionsCount; ++i)
    {
        requiredSize += EntityPermissionStatement::RequiredBufferSize(*model.permissions[i]);
    }
    return requiredSize;
}

HRESULT SetEntityProfilePolicyResponse::Copy(const PFProfilesSetEntityProfilePolicyResponse& input, PFProfilesSetEntityProfilePolicyResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<EntityPermissionStatement>(input.permissions, input.permissionsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.permissions = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

} // namespace Profiles

// Json serialization helpers

} // namespace PlayFab

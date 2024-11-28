#include "stdafx.h"
#include "DataTypes.h"
#include "JsonUtils.h"
#include "Types.h"

namespace PlayFab
{
namespace Data
{

JsonValue AbortFileUploadsRequest::ToJson() const
{
    return AbortFileUploadsRequest::ToJson(this->Model());
}

JsonValue AbortFileUploadsRequest::ToJson(const PFDataAbortFileUploadsRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMemberArray(output, "FileNames", input.fileNames, input.fileNamesCount);
    JsonUtils::ObjectAddMember(output, "ProfileVersion", input.profileVersion);
    return output;
}

HRESULT AbortFileUploadsResponse::FromJson(const JsonValue& input)
{
    std::optional<EntityKey> entity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Entity", entity));
    if (entity)
    {
        this->SetEntity(std::move(*entity));
    }

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ProfileVersion", this->m_model.profileVersion));

    return S_OK;
}

size_t AbortFileUploadsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFDataAbortFileUploadsResponse const*> AbortFileUploadsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<AbortFileUploadsResponse>(&this->Model());
}

size_t AbortFileUploadsResponse::RequiredBufferSize(const PFDataAbortFileUploadsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.entity)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.entity);
    }
    return requiredSize;
}

HRESULT AbortFileUploadsResponse::Copy(const PFDataAbortFileUploadsResponse& input, PFDataAbortFileUploadsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.entity);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.entity = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue DeleteFilesRequest::ToJson() const
{
    return DeleteFilesRequest::ToJson(this->Model());
}

JsonValue DeleteFilesRequest::ToJson(const PFDataDeleteFilesRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMemberArray(output, "FileNames", input.fileNames, input.fileNamesCount);
    JsonUtils::ObjectAddMember(output, "ProfileVersion", input.profileVersion);
    return output;
}

HRESULT DeleteFilesResponse::FromJson(const JsonValue& input)
{
    std::optional<EntityKey> entity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Entity", entity));
    if (entity)
    {
        this->SetEntity(std::move(*entity));
    }

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ProfileVersion", this->m_model.profileVersion));

    return S_OK;
}

size_t DeleteFilesResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFDataDeleteFilesResponse const*> DeleteFilesResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<DeleteFilesResponse>(&this->Model());
}

size_t DeleteFilesResponse::RequiredBufferSize(const PFDataDeleteFilesResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.entity)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.entity);
    }
    return requiredSize;
}

HRESULT DeleteFilesResponse::Copy(const PFDataDeleteFilesResponse& input, PFDataDeleteFilesResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.entity);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.entity = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue FinalizeFileUploadsRequest::ToJson() const
{
    return FinalizeFileUploadsRequest::ToJson(this->Model());
}

JsonValue FinalizeFileUploadsRequest::ToJson(const PFDataFinalizeFileUploadsRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMemberArray(output, "FileNames", input.fileNames, input.fileNamesCount);
    JsonUtils::ObjectAddMember(output, "ProfileVersion", input.profileVersion);
    return output;
}

HRESULT GetFileMetadata::FromJson(const JsonValue& input)
{
    String checksum{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Checksum", checksum));
    this->SetChecksum(std::move(checksum));

    String downloadUrl{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DownloadUrl", downloadUrl));
    this->SetDownloadUrl(std::move(downloadUrl));

    String fileName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FileName", fileName));
    this->SetFileName(std::move(fileName));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "LastModified", this->m_model.lastModified));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Size", this->m_model.size));

    return S_OK;
}

size_t GetFileMetadata::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFDataGetFileMetadata const*> GetFileMetadata::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetFileMetadata>(&this->Model());
}

size_t GetFileMetadata::RequiredBufferSize(const PFDataGetFileMetadata& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.checksum)
    {
        requiredSize += (std::strlen(model.checksum) + 1);
    }
    if (model.downloadUrl)
    {
        requiredSize += (std::strlen(model.downloadUrl) + 1);
    }
    if (model.fileName)
    {
        requiredSize += (std::strlen(model.fileName) + 1);
    }
    return requiredSize;
}

HRESULT GetFileMetadata::Copy(const PFDataGetFileMetadata& input, PFDataGetFileMetadata& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.checksum);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.checksum = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.downloadUrl);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.downloadUrl = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.fileName);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.fileName = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT FinalizeFileUploadsResponse::FromJson(const JsonValue& input)
{
    std::optional<EntityKey> entity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Entity", entity));
    if (entity)
    {
        this->SetEntity(std::move(*entity));
    }

    ModelDictionaryEntryVector<GetFileMetadata> metadata{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<GetFileMetadata>(input, "Metadata", metadata));
    this->SetMetadata(std::move(metadata));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ProfileVersion", this->m_model.profileVersion));

    return S_OK;
}

size_t FinalizeFileUploadsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFDataFinalizeFileUploadsResponse const*> FinalizeFileUploadsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<FinalizeFileUploadsResponse>(&this->Model());
}

size_t FinalizeFileUploadsResponse::RequiredBufferSize(const PFDataFinalizeFileUploadsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.entity)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.entity);
    }
    requiredSize += (alignof(PFDataGetFileMetadataDictionaryEntry) + sizeof(PFDataGetFileMetadataDictionaryEntry) * model.metadataCount);
    for (size_t i = 0; i < model.metadataCount; ++i)
    {
        requiredSize += (std::strlen(model.metadata[i].key) + 1);
        requiredSize += GetFileMetadata::RequiredBufferSize(*model.metadata[i].value);
    }
    return requiredSize;
}

HRESULT FinalizeFileUploadsResponse::Copy(const PFDataFinalizeFileUploadsResponse& input, PFDataFinalizeFileUploadsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.entity);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.entity = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary<GetFileMetadata>(input.metadata, input.metadataCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.metadata = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetFilesRequest::ToJson() const
{
    return GetFilesRequest::ToJson(this->Model());
}

JsonValue GetFilesRequest::ToJson(const PFDataGetFilesRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    return output;
}

HRESULT GetFilesResponse::FromJson(const JsonValue& input)
{
    std::optional<EntityKey> entity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Entity", entity));
    if (entity)
    {
        this->SetEntity(std::move(*entity));
    }

    ModelDictionaryEntryVector<GetFileMetadata> metadata{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<GetFileMetadata>(input, "Metadata", metadata));
    this->SetMetadata(std::move(metadata));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ProfileVersion", this->m_model.profileVersion));

    return S_OK;
}

size_t GetFilesResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFDataGetFilesResponse const*> GetFilesResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetFilesResponse>(&this->Model());
}

size_t GetFilesResponse::RequiredBufferSize(const PFDataGetFilesResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.entity)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.entity);
    }
    requiredSize += (alignof(PFDataGetFileMetadataDictionaryEntry) + sizeof(PFDataGetFileMetadataDictionaryEntry) * model.metadataCount);
    for (size_t i = 0; i < model.metadataCount; ++i)
    {
        requiredSize += (std::strlen(model.metadata[i].key) + 1);
        requiredSize += GetFileMetadata::RequiredBufferSize(*model.metadata[i].value);
    }
    return requiredSize;
}

HRESULT GetFilesResponse::Copy(const PFDataGetFilesResponse& input, PFDataGetFilesResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.entity);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.entity = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary<GetFileMetadata>(input.metadata, input.metadataCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.metadata = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetObjectsRequest::ToJson() const
{
    return GetObjectsRequest::ToJson(this->Model());
}

JsonValue GetObjectsRequest::ToJson(const PFDataGetObjectsRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "EscapeObject", input.escapeObject);
    return output;
}

HRESULT ObjectResult::FromJson(const JsonValue& input)
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

size_t ObjectResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFDataObjectResult const*> ObjectResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ObjectResult>(&this->Model());
}

size_t ObjectResult::RequiredBufferSize(const PFDataObjectResult& model)
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

HRESULT ObjectResult::Copy(const PFDataObjectResult& input, PFDataObjectResult& output, ModelBuffer& buffer)
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

HRESULT GetObjectsResponse::FromJson(const JsonValue& input)
{
    std::optional<EntityKey> entity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Entity", entity));
    if (entity)
    {
        this->SetEntity(std::move(*entity));
    }

    ModelDictionaryEntryVector<ObjectResult> objects{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<ObjectResult>(input, "Objects", objects));
    this->SetObjects(std::move(objects));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ProfileVersion", this->m_model.profileVersion));

    return S_OK;
}

size_t GetObjectsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFDataGetObjectsResponse const*> GetObjectsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetObjectsResponse>(&this->Model());
}

size_t GetObjectsResponse::RequiredBufferSize(const PFDataGetObjectsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.entity)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.entity);
    }
    requiredSize += (alignof(PFDataObjectResultDictionaryEntry) + sizeof(PFDataObjectResultDictionaryEntry) * model.objectsCount);
    for (size_t i = 0; i < model.objectsCount; ++i)
    {
        requiredSize += (std::strlen(model.objects[i].key) + 1);
        requiredSize += ObjectResult::RequiredBufferSize(*model.objects[i].value);
    }
    return requiredSize;
}

HRESULT GetObjectsResponse::Copy(const PFDataGetObjectsResponse& input, PFDataGetObjectsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.entity);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.entity = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary<ObjectResult>(input.objects, input.objectsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.objects = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue InitiateFileUploadsRequest::ToJson() const
{
    return InitiateFileUploadsRequest::ToJson(this->Model());
}

JsonValue InitiateFileUploadsRequest::ToJson(const PFDataInitiateFileUploadsRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMemberArray(output, "FileNames", input.fileNames, input.fileNamesCount);
    JsonUtils::ObjectAddMember(output, "ProfileVersion", input.profileVersion);
    return output;
}

HRESULT InitiateFileUploadMetadata::FromJson(const JsonValue& input)
{
    String fileName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FileName", fileName));
    this->SetFileName(std::move(fileName));

    String uploadUrl{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "UploadUrl", uploadUrl));
    this->SetUploadUrl(std::move(uploadUrl));

    return S_OK;
}

size_t InitiateFileUploadMetadata::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFDataInitiateFileUploadMetadata const*> InitiateFileUploadMetadata::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<InitiateFileUploadMetadata>(&this->Model());
}

size_t InitiateFileUploadMetadata::RequiredBufferSize(const PFDataInitiateFileUploadMetadata& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.fileName)
    {
        requiredSize += (std::strlen(model.fileName) + 1);
    }
    if (model.uploadUrl)
    {
        requiredSize += (std::strlen(model.uploadUrl) + 1);
    }
    return requiredSize;
}

HRESULT InitiateFileUploadMetadata::Copy(const PFDataInitiateFileUploadMetadata& input, PFDataInitiateFileUploadMetadata& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.fileName);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.fileName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.uploadUrl);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.uploadUrl = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT InitiateFileUploadsResponse::FromJson(const JsonValue& input)
{
    std::optional<EntityKey> entity{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Entity", entity));
    if (entity)
    {
        this->SetEntity(std::move(*entity));
    }

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ProfileVersion", this->m_model.profileVersion));

    ModelVector<InitiateFileUploadMetadata> uploadDetails{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<InitiateFileUploadMetadata>(input, "UploadDetails", uploadDetails));
    this->SetUploadDetails(std::move(uploadDetails));

    return S_OK;
}

size_t InitiateFileUploadsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFDataInitiateFileUploadsResponse const*> InitiateFileUploadsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<InitiateFileUploadsResponse>(&this->Model());
}

size_t InitiateFileUploadsResponse::RequiredBufferSize(const PFDataInitiateFileUploadsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.entity)
    {
        requiredSize += EntityKey::RequiredBufferSize(*model.entity);
    }
    requiredSize += (alignof(PFDataInitiateFileUploadMetadata*) + sizeof(PFDataInitiateFileUploadMetadata*) * model.uploadDetailsCount);
    for (size_t i = 0; i < model.uploadDetailsCount; ++i)
    {
        requiredSize += InitiateFileUploadMetadata::RequiredBufferSize(*model.uploadDetails[i]);
    }
    return requiredSize;
}

HRESULT InitiateFileUploadsResponse::Copy(const PFDataInitiateFileUploadsResponse& input, PFDataInitiateFileUploadsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<EntityKey>(input.entity);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.entity = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<InitiateFileUploadMetadata>(input.uploadDetails, input.uploadDetailsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.uploadDetails = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue SetObject::ToJson() const
{
    return SetObject::ToJson(this->Model());
}

JsonValue SetObject::ToJson(const PFDataSetObject& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "DataObject", input.dataObject);
    JsonUtils::ObjectAddMember(output, "DeleteObject", input.deleteObject);
    JsonUtils::ObjectAddMember(output, "EscapedDataObject", input.escapedDataObject);
    JsonUtils::ObjectAddMember(output, "ObjectName", input.objectName);
    return output;
}

JsonValue SetObjectsRequest::ToJson() const
{
    return SetObjectsRequest::ToJson(this->Model());
}

JsonValue SetObjectsRequest::ToJson(const PFDataSetObjectsRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "ExpectedProfileVersion", input.expectedProfileVersion);
    JsonUtils::ObjectAddMemberArray<SetObject>(output, "Objects", input.objects, input.objectsCount);
    return output;
}

HRESULT SetObjectInfo::FromJson(const JsonValue& input)
{
    String objectName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ObjectName", objectName));
    this->SetObjectName(std::move(objectName));

    String operationReason{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "OperationReason", operationReason));
    this->SetOperationReason(std::move(operationReason));

    std::optional<PFOperationTypes> setResult{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SetResult", setResult));
    this->SetSetResult(std::move(setResult));

    return S_OK;
}

size_t SetObjectInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFDataSetObjectInfo const*> SetObjectInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<SetObjectInfo>(&this->Model());
}

size_t SetObjectInfo::RequiredBufferSize(const PFDataSetObjectInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.objectName)
    {
        requiredSize += (std::strlen(model.objectName) + 1);
    }
    if (model.operationReason)
    {
        requiredSize += (std::strlen(model.operationReason) + 1);
    }
    if (model.setResult)
    {
        requiredSize += (alignof(PFOperationTypes) + sizeof(PFOperationTypes));
    }
    return requiredSize;
}

HRESULT SetObjectInfo::Copy(const PFDataSetObjectInfo& input, PFDataSetObjectInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.objectName);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.objectName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.operationReason);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.operationReason = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.setResult);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.setResult = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT SetObjectsResponse::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ProfileVersion", this->m_model.profileVersion));

    ModelVector<SetObjectInfo> setResults{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<SetObjectInfo>(input, "SetResults", setResults));
    this->SetSetResults(std::move(setResults));

    return S_OK;
}

size_t SetObjectsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFDataSetObjectsResponse const*> SetObjectsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<SetObjectsResponse>(&this->Model());
}

size_t SetObjectsResponse::RequiredBufferSize(const PFDataSetObjectsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFDataSetObjectInfo*) + sizeof(PFDataSetObjectInfo*) * model.setResultsCount);
    for (size_t i = 0; i < model.setResultsCount; ++i)
    {
        requiredSize += SetObjectInfo::RequiredBufferSize(*model.setResults[i]);
    }
    return requiredSize;
}

HRESULT SetObjectsResponse::Copy(const PFDataSetObjectsResponse& input, PFDataSetObjectsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<SetObjectInfo>(input.setResults, input.setResultsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.setResults = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

} // namespace Data

// Json serialization helpers

} // namespace PlayFab

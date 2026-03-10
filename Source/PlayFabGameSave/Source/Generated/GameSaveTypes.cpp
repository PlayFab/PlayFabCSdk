#include "stdafx.h"
#include "GameSaveTypes.h"
#include "JsonUtils.h"
#include "Types.h"

namespace PlayFab
{
namespace GameSaveWrapper
{

JsonValue DeleteManifestRequest::ToJson() const
{
    return DeleteManifestRequest::ToJson(this->Model());
}

JsonValue DeleteManifestRequest::ToJson(const PFGameSaveDeleteManifestRequest& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "Version", input.version);
    return output;
}

JsonValue FinalizeManifestConflict::ToJson() const
{
    return FinalizeManifestConflict::ToJson(this->Model());
}

JsonValue FinalizeManifestConflict::ToJson(const PFGameSaveFinalizeManifestConflict& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMember(output, "ConflictingVersion", input.conflictingVersion);
    JsonUtils::ObjectAddMember(output, "IsWinner", input.isWinner);
    return output;
}

JsonValue FinalizedFileDetails::ToJson() const
{
    return FinalizedFileDetails::ToJson(this->Model());
}

JsonValue FinalizedFileDetails::ToJson(const PFGameSaveFinalizedFileDetails& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMember(output, "FileName", input.fileName);
    JsonUtils::ObjectAddMember(output, "FileSizeBytes", input.fileSizeBytes);
    return output;
}

JsonValue FinalizeManifestRequest::ToJson() const
{
    return FinalizeManifestRequest::ToJson(this->Model());
}

JsonValue FinalizeManifestRequest::ToJson(const PFGameSaveFinalizeManifestRequest& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMember<FinalizeManifestConflict>(output, "Conflict", input.conflict);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMemberArray<FinalizedFileDetails>(output, "FilesToFinalize", input.filesToFinalize, input.filesToFinalizeCount);
    JsonUtils::ObjectAddMember(output, "Force", input.force);
    JsonUtils::ObjectAddMember(output, "ManifestDescription", input.manifestDescription);
    JsonUtils::ObjectAddMember(output, "MarkBaseAsKnownGood", input.markBaseAsKnownGood);
    JsonUtils::ObjectAddMember(output, "Version", input.version);
    return output;
}

HRESULT ManifestConflict::FromJson(const JsonValue& input)
{
    String conflictingVersion{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ConflictingVersion", conflictingVersion));
    this->SetConflictingVersion(std::move(conflictingVersion));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "ConflictResolutionTimestamp", this->m_model.conflictResolutionTimestamp));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IsWinner", this->m_model.isWinner));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "RetainedUntilTimestamp", this->m_model.retainedUntilTimestamp));

    return S_OK;
}

size_t ManifestConflict::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGameSaveManifestConflict const*> ManifestConflict::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ManifestConflict>(&this->Model());
}

size_t ManifestConflict::RequiredBufferSize(const PFGameSaveManifestConflict& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.conflictingVersion)
    {
        requiredSize += (std::strlen(model.conflictingVersion) + 1);
    }
    return requiredSize;
}

HRESULT ManifestConflict::Copy(const PFGameSaveManifestConflict& input, PFGameSaveManifestConflict& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.conflictingVersion);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.conflictingVersion = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT FileMetadata::FromJson(const JsonValue& input)
{
    String fileName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FileName", fileName));
    this->SetFileName(std::move(fileName));

    return S_OK;
}

size_t FileMetadata::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGameSaveFileMetadata const*> FileMetadata::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<FileMetadata>(&this->Model());
}

size_t FileMetadata::RequiredBufferSize(const PFGameSaveFileMetadata& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.fileName)
    {
        requiredSize += (std::strlen(model.fileName) + 1);
    }
    return requiredSize;
}

HRESULT FileMetadata::Copy(const PFGameSaveFileMetadata& input, PFGameSaveFileMetadata& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.fileName);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.fileName = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ManifestMetadata::ToJson() const
{
    return ManifestMetadata::ToJson(this->Model());
}

JsonValue ManifestMetadata::ToJson(const PFGameSaveManifestMetadata& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMember(output, "DeviceId", input.deviceId);
    JsonUtils::ObjectAddMember(output, "DeviceName", input.deviceName);
    JsonUtils::ObjectAddMember(output, "DeviceType", input.deviceType);
    JsonUtils::ObjectAddMember(output, "DeviceVersion", input.deviceVersion);
    return output;
}

HRESULT ManifestMetadata::FromJson(const JsonValue& input)
{
    String deviceId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DeviceId", deviceId));
    this->SetDeviceId(std::move(deviceId));

    String deviceName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DeviceName", deviceName));
    this->SetDeviceName(std::move(deviceName));

    String deviceType{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DeviceType", deviceType));
    this->SetDeviceType(std::move(deviceType));

    String deviceVersion{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DeviceVersion", deviceVersion));
    this->SetDeviceVersion(std::move(deviceVersion));

    return S_OK;
}

size_t ManifestMetadata::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGameSaveManifestMetadata const*> ManifestMetadata::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ManifestMetadata>(&this->Model());
}

size_t ManifestMetadata::RequiredBufferSize(const PFGameSaveManifestMetadata& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.deviceId)
    {
        requiredSize += (std::strlen(model.deviceId) + 1);
    }
    if (model.deviceName)
    {
        requiredSize += (std::strlen(model.deviceName) + 1);
    }
    if (model.deviceType)
    {
        requiredSize += (std::strlen(model.deviceType) + 1);
    }
    if (model.deviceVersion)
    {
        requiredSize += (std::strlen(model.deviceVersion) + 1);
    }
    return requiredSize;
}

HRESULT ManifestMetadata::Copy(const PFGameSaveManifestMetadata& input, PFGameSaveManifestMetadata& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.deviceId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.deviceId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.deviceName);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.deviceName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.deviceType);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.deviceType = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.deviceVersion);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.deviceVersion = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue UploadProgress::ToJson() const
{
    return UploadProgress::ToJson(this->Model());
}

JsonValue UploadProgress::ToJson(const PFGameSaveUploadProgress& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMember(output, "TotalBytes", input.totalBytes);
    JsonUtils::ObjectAddMember(output, "UploadedBytes", input.uploadedBytes);
    return output;
}

HRESULT UploadProgress::FromJson(const JsonValue& input)
{
    String totalBytes{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TotalBytes", totalBytes));
    this->SetTotalBytes(std::move(totalBytes));

    String uploadedBytes{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "UploadedBytes", uploadedBytes));
    this->SetUploadedBytes(std::move(uploadedBytes));

    return S_OK;
}

size_t UploadProgress::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGameSaveUploadProgress const*> UploadProgress::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UploadProgress>(&this->Model());
}

size_t UploadProgress::RequiredBufferSize(const PFGameSaveUploadProgress& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.totalBytes)
    {
        requiredSize += (std::strlen(model.totalBytes) + 1);
    }
    if (model.uploadedBytes)
    {
        requiredSize += (std::strlen(model.uploadedBytes) + 1);
    }
    return requiredSize;
}

HRESULT UploadProgress::Copy(const PFGameSaveUploadProgress& input, PFGameSaveUploadProgress& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.totalBytes);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.totalBytes = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.uploadedBytes);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.uploadedBytes = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT Manifest::FromJson(const JsonValue& input)
{
    String baseVersion{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "BaseVersion", baseVersion));
    this->SetBaseVersion(std::move(baseVersion));

    std::optional<ManifestConflict> conflict{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Conflict", conflict));
    if (conflict)
    {
        this->SetConflict(std::move(*conflict));
    }

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "CreationTimestamp", this->m_model.creationTimestamp));

    ModelVector<FileMetadata> files{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<FileMetadata>(input, "Files", files));
    this->SetFiles(std::move(files));

    std::optional<time_t> finalizationTimestamp{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "FinalizationTimestamp", finalizationTimestamp));
    this->SetFinalizationTimestamp(std::move(finalizationTimestamp));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GeneratedByRollback", this->m_model.generatedByRollback));

    std::optional<bool> isKnownGood{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IsKnownGood", isKnownGood));
    this->SetIsKnownGood(std::move(isKnownGood));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "LastUpdateTimestamp", this->m_model.lastUpdateTimestamp));

    String manifestDescription{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ManifestDescription", manifestDescription));
    this->SetManifestDescription(std::move(manifestDescription));

    std::optional<ManifestMetadata> metadata{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Metadata", metadata));
    if (metadata)
    {
        this->SetMetadata(std::move(*metadata));
    }

    String rollbackReason{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "RollbackReason", rollbackReason));
    this->SetRollbackReason(std::move(rollbackReason));

    String status{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Status", status));
    this->SetStatus(std::move(status));

    std::optional<UploadProgress> uploadProgress{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "UploadProgress", uploadProgress));
    if (uploadProgress)
    {
        this->SetUploadProgress(std::move(*uploadProgress));
    }

    String version{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Version", version));
    this->SetVersion(std::move(version));

    return S_OK;
}

size_t Manifest::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGameSaveManifest const*> Manifest::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<Manifest>(&this->Model());
}

size_t Manifest::RequiredBufferSize(const PFGameSaveManifest& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.baseVersion)
    {
        requiredSize += (std::strlen(model.baseVersion) + 1);
    }
    if (model.conflict)
    {
        requiredSize += ManifestConflict::RequiredBufferSize(*model.conflict);
    }
    requiredSize += (alignof(PFGameSaveFileMetadata*) + sizeof(PFGameSaveFileMetadata*) * model.filesCount);
    for (size_t i = 0; i < model.filesCount; ++i)
    {
        requiredSize += FileMetadata::RequiredBufferSize(*model.files[i]);
    }
    if (model.finalizationTimestamp)
    {
        requiredSize += (alignof(time_t) + sizeof(time_t));
    }
    if (model.isKnownGood)
    {
        requiredSize += (alignof(bool) + sizeof(bool));
    }
    if (model.manifestDescription)
    {
        requiredSize += (std::strlen(model.manifestDescription) + 1);
    }
    if (model.metadata)
    {
        requiredSize += ManifestMetadata::RequiredBufferSize(*model.metadata);
    }
    if (model.rollbackReason)
    {
        requiredSize += (std::strlen(model.rollbackReason) + 1);
    }
    if (model.status)
    {
        requiredSize += (std::strlen(model.status) + 1);
    }
    if (model.uploadProgress)
    {
        requiredSize += UploadProgress::RequiredBufferSize(*model.uploadProgress);
    }
    if (model.version)
    {
        requiredSize += (std::strlen(model.version) + 1);
    }
    return requiredSize;
}

HRESULT Manifest::Copy(const PFGameSaveManifest& input, PFGameSaveManifest& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.baseVersion);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.baseVersion = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<ManifestConflict>(input.conflict);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.conflict = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<FileMetadata>(input.files, input.filesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.files = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.finalizationTimestamp);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.finalizationTimestamp = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.isKnownGood);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.isKnownGood = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.manifestDescription);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.manifestDescription = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<ManifestMetadata>(input.metadata);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.metadata = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.rollbackReason);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.rollbackReason = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.status);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.status = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UploadProgress>(input.uploadProgress);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.uploadProgress = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.version);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.version = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT FinalizeManifestResponse::FromJson(const JsonValue& input)
{
    std::optional<Manifest> manifest{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Manifest", manifest));
    if (manifest)
    {
        this->SetManifest(std::move(*manifest));
    }

    return S_OK;
}

size_t FinalizeManifestResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGameSaveFinalizeManifestResponse const*> FinalizeManifestResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<FinalizeManifestResponse>(&this->Model());
}

size_t FinalizeManifestResponse::RequiredBufferSize(const PFGameSaveFinalizeManifestResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.manifest)
    {
        requiredSize += Manifest::RequiredBufferSize(*model.manifest);
    }
    return requiredSize;
}

HRESULT FinalizeManifestResponse::Copy(const PFGameSaveFinalizeManifestResponse& input, PFGameSaveFinalizeManifestResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<Manifest>(input.manifest);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.manifest = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetConfigForTitleRequest::ToJson() const
{
    return GetConfigForTitleRequest::ToJson(this->Model());
}

JsonValue GetConfigForTitleRequest::ToJson(const PFGameSaveGetConfigForTitleRequest& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    return output;
}

HRESULT GetConfigForTitleResponse::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DisableClientRollbackToLastConflictLoser", this->m_model.disableClientRollbackToLastConflictLoser));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DisableClientRollbackToLastKnownGood", this->m_model.disableClientRollbackToLastKnownGood));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DisableUnsignaledOutOfProcessUpload", this->m_model.disableUnsignaledOutOfProcessUpload));

    String onboardingStatus{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "OnboardingStatus", onboardingStatus));
    this->SetOnboardingStatus(std::move(onboardingStatus));

    String perPlayerQuotaBytes{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PerPlayerQuotaBytes", perPlayerQuotaBytes));
    this->SetPerPlayerQuotaBytes(std::move(perPlayerQuotaBytes));

    return S_OK;
}

size_t GetConfigForTitleResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGameSaveGetConfigForTitleResponse const*> GetConfigForTitleResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetConfigForTitleResponse>(&this->Model());
}

size_t GetConfigForTitleResponse::RequiredBufferSize(const PFGameSaveGetConfigForTitleResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.onboardingStatus)
    {
        requiredSize += (std::strlen(model.onboardingStatus) + 1);
    }
    if (model.perPlayerQuotaBytes)
    {
        requiredSize += (std::strlen(model.perPlayerQuotaBytes) + 1);
    }
    return requiredSize;
}

HRESULT GetConfigForTitleResponse::Copy(const PFGameSaveGetConfigForTitleResponse& input, PFGameSaveGetConfigForTitleResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.onboardingStatus);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.onboardingStatus = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.perPlayerQuotaBytes);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.perPlayerQuotaBytes = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetManifestDownloadDetailsRequest::ToJson() const
{
    return GetManifestDownloadDetailsRequest::ToJson(this->Model());
}

JsonValue GetManifestDownloadDetailsRequest::ToJson(const PFGameSaveGetManifestDownloadDetailsRequest& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "Version", input.version);
    return output;
}

HRESULT FileDownloadDetails::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "AccessTokenExpirationTime", this->m_model.accessTokenExpirationTime));

    String downloadUrl{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DownloadUrl", downloadUrl));
    this->SetDownloadUrl(std::move(downloadUrl));

    String fileName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FileName", fileName));
    this->SetFileName(std::move(fileName));

    return S_OK;
}

size_t FileDownloadDetails::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGameSaveFileDownloadDetails const*> FileDownloadDetails::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<FileDownloadDetails>(&this->Model());
}

size_t FileDownloadDetails::RequiredBufferSize(const PFGameSaveFileDownloadDetails& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
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

HRESULT FileDownloadDetails::Copy(const PFGameSaveFileDownloadDetails& input, PFGameSaveFileDownloadDetails& output, ModelBuffer& buffer)
{
    output = input;
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

HRESULT GetManifestDownloadDetailsResponse::FromJson(const JsonValue& input)
{
    ModelVector<FileDownloadDetails> files{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<FileDownloadDetails>(input, "Files", files));
    this->SetFiles(std::move(files));

    return S_OK;
}

size_t GetManifestDownloadDetailsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGameSaveGetManifestDownloadDetailsResponse const*> GetManifestDownloadDetailsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetManifestDownloadDetailsResponse>(&this->Model());
}

size_t GetManifestDownloadDetailsResponse::RequiredBufferSize(const PFGameSaveGetManifestDownloadDetailsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFGameSaveFileDownloadDetails*) + sizeof(PFGameSaveFileDownloadDetails*) * model.filesCount);
    for (size_t i = 0; i < model.filesCount; ++i)
    {
        requiredSize += FileDownloadDetails::RequiredBufferSize(*model.files[i]);
    }
    return requiredSize;
}

HRESULT GetManifestDownloadDetailsResponse::Copy(const PFGameSaveGetManifestDownloadDetailsResponse& input, PFGameSaveGetManifestDownloadDetailsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<FileDownloadDetails>(input.files, input.filesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.files = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetQuotaForPlayerRequest::ToJson() const
{
    return GetQuotaForPlayerRequest::ToJson(this->Model());
}

JsonValue GetQuotaForPlayerRequest::ToJson(const PFGameSaveGetQuotaForPlayerRequest& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    return output;
}

HRESULT GetQuotaForPlayerResponse::FromJson(const JsonValue& input)
{
    String availableBytes{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "AvailableBytes", availableBytes));
    this->SetAvailableBytes(std::move(availableBytes));

    String totalBytes{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TotalBytes", totalBytes));
    this->SetTotalBytes(std::move(totalBytes));

    return S_OK;
}

size_t GetQuotaForPlayerResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGameSaveGetQuotaForPlayerResponse const*> GetQuotaForPlayerResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetQuotaForPlayerResponse>(&this->Model());
}

size_t GetQuotaForPlayerResponse::RequiredBufferSize(const PFGameSaveGetQuotaForPlayerResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.availableBytes)
    {
        requiredSize += (std::strlen(model.availableBytes) + 1);
    }
    if (model.totalBytes)
    {
        requiredSize += (std::strlen(model.totalBytes) + 1);
    }
    return requiredSize;
}

HRESULT GetQuotaForPlayerResponse::Copy(const PFGameSaveGetQuotaForPlayerResponse& input, PFGameSaveGetQuotaForPlayerResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.availableBytes);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.availableBytes = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.totalBytes);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.totalBytes = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue InitializeManifestRequest::ToJson() const
{
    return InitializeManifestRequest::ToJson(this->Model());
}

JsonValue InitializeManifestRequest::ToJson(const PFGameSaveInitializeManifestRequest& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMember(output, "BaseVersion", input.baseVersion);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember<ManifestMetadata>(output, "Metadata", input.metadata);
    JsonUtils::ObjectAddMember(output, "Version", input.version);
    return output;
}

HRESULT InitializeManifestResponse::FromJson(const JsonValue& input)
{
    std::optional<Manifest> manifest{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Manifest", manifest));
    if (manifest)
    {
        this->SetManifest(std::move(*manifest));
    }

    return S_OK;
}

size_t InitializeManifestResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGameSaveInitializeManifestResponse const*> InitializeManifestResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<InitializeManifestResponse>(&this->Model());
}

size_t InitializeManifestResponse::RequiredBufferSize(const PFGameSaveInitializeManifestResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.manifest)
    {
        requiredSize += Manifest::RequiredBufferSize(*model.manifest);
    }
    return requiredSize;
}

HRESULT InitializeManifestResponse::Copy(const PFGameSaveInitializeManifestResponse& input, PFGameSaveInitializeManifestResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<Manifest>(input.manifest);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.manifest = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue FileToUpload::ToJson() const
{
    return FileToUpload::ToJson(this->Model());
}

JsonValue FileToUpload::ToJson(const PFGameSaveFileToUpload& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMember(output, "FileName", input.fileName);
    return output;
}

JsonValue InitiateUploadRequest::ToJson() const
{
    return InitiateUploadRequest::ToJson(this->Model());
}

JsonValue InitiateUploadRequest::ToJson(const PFGameSaveInitiateUploadRequest& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMemberArray<FileToUpload>(output, "Files", input.files, input.filesCount);
    JsonUtils::ObjectAddMember(output, "Version", input.version);
    return output;
}

HRESULT AllocatedFile::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "AccessTokenExpirationTime", this->m_model.accessTokenExpirationTime));

    String fileName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FileName", fileName));
    this->SetFileName(std::move(fileName));

    String uploadUrl{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "UploadUrl", uploadUrl));
    this->SetUploadUrl(std::move(uploadUrl));

    return S_OK;
}

size_t AllocatedFile::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGameSaveAllocatedFile const*> AllocatedFile::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<AllocatedFile>(&this->Model());
}

size_t AllocatedFile::RequiredBufferSize(const PFGameSaveAllocatedFile& model)
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

HRESULT AllocatedFile::Copy(const PFGameSaveAllocatedFile& input, PFGameSaveAllocatedFile& output, ModelBuffer& buffer)
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

HRESULT InitiateUploadResponse::FromJson(const JsonValue& input)
{
    ModelVector<AllocatedFile> files{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<AllocatedFile>(input, "Files", files));
    this->SetFiles(std::move(files));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "RecommendedChunkSizeBytes", this->m_model.recommendedChunkSizeBytes));

    return S_OK;
}

size_t InitiateUploadResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGameSaveInitiateUploadResponse const*> InitiateUploadResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<InitiateUploadResponse>(&this->Model());
}

size_t InitiateUploadResponse::RequiredBufferSize(const PFGameSaveInitiateUploadResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFGameSaveAllocatedFile*) + sizeof(PFGameSaveAllocatedFile*) * model.filesCount);
    for (size_t i = 0; i < model.filesCount; ++i)
    {
        requiredSize += AllocatedFile::RequiredBufferSize(*model.files[i]);
    }
    return requiredSize;
}

HRESULT InitiateUploadResponse::Copy(const PFGameSaveInitiateUploadResponse& input, PFGameSaveInitiateUploadResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<AllocatedFile>(input.files, input.filesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.files = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ListManifestsRequest::ToJson() const
{
    return ListManifestsRequest::ToJson(this->Model());
}

JsonValue ListManifestsRequest::ToJson(const PFGameSaveListManifestsRequest& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "IncludeUnavailable", input.includeUnavailable);
    return output;
}

HRESULT TitleConfiguration::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DisableClientRollbackToLastConflictLoser", this->m_model.disableClientRollbackToLastConflictLoser));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DisableClientRollbackToLastKnownGood", this->m_model.disableClientRollbackToLastKnownGood));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DisableUnsignaledOutOfProcessUpload", this->m_model.disableUnsignaledOutOfProcessUpload));

    String perPlayerQuotaBytes{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PerPlayerQuotaBytes", perPlayerQuotaBytes));
    this->SetPerPlayerQuotaBytes(std::move(perPlayerQuotaBytes));

    return S_OK;
}

size_t TitleConfiguration::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGameSaveTitleConfiguration const*> TitleConfiguration::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<TitleConfiguration>(&this->Model());
}

size_t TitleConfiguration::RequiredBufferSize(const PFGameSaveTitleConfiguration& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.perPlayerQuotaBytes)
    {
        requiredSize += (std::strlen(model.perPlayerQuotaBytes) + 1);
    }
    return requiredSize;
}

HRESULT TitleConfiguration::Copy(const PFGameSaveTitleConfiguration& input, PFGameSaveTitleConfiguration& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.perPlayerQuotaBytes);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.perPlayerQuotaBytes = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT ListManifestsResponse::FromJson(const JsonValue& input)
{
    ModelVector<Manifest> manifests{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<Manifest>(input, "Manifests", manifests));
    this->SetManifests(std::move(manifests));

    String nextAvailableVersion{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "NextAvailableVersion", nextAvailableVersion));
    this->SetNextAvailableVersion(std::move(nextAvailableVersion));

    std::optional<TitleConfiguration> titleConfiguration{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TitleConfiguration", titleConfiguration));
    if (titleConfiguration)
    {
        this->SetTitleConfiguration(std::move(*titleConfiguration));
    }

    return S_OK;
}

size_t ListManifestsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGameSaveListManifestsResponse const*> ListManifestsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ListManifestsResponse>(&this->Model());
}

size_t ListManifestsResponse::RequiredBufferSize(const PFGameSaveListManifestsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFGameSaveManifest*) + sizeof(PFGameSaveManifest*) * model.manifestsCount);
    for (size_t i = 0; i < model.manifestsCount; ++i)
    {
        requiredSize += Manifest::RequiredBufferSize(*model.manifests[i]);
    }
    if (model.nextAvailableVersion)
    {
        requiredSize += (std::strlen(model.nextAvailableVersion) + 1);
    }
    if (model.titleConfiguration)
    {
        requiredSize += TitleConfiguration::RequiredBufferSize(*model.titleConfiguration);
    }
    return requiredSize;
}

HRESULT ListManifestsResponse::Copy(const PFGameSaveListManifestsResponse& input, PFGameSaveListManifestsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<Manifest>(input.manifests, input.manifestsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.manifests = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.nextAvailableVersion);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.nextAvailableVersion = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<TitleConfiguration>(input.titleConfiguration);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.titleConfiguration = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue RequestOnboardingRequest::ToJson() const
{
    return RequestOnboardingRequest::ToJson(this->Model());
}

JsonValue RequestOnboardingRequest::ToJson(const PFGameSaveRequestOnboardingRequest& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    return output;
}

JsonValue RollbackToManifestRequest::ToJson() const
{
    return RollbackToManifestRequest::ToJson(this->Model());
}

JsonValue RollbackToManifestRequest::ToJson(const PFGameSaveRollbackToManifestRequest& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "RollbackReason", input.rollbackReason);
    JsonUtils::ObjectAddMember(output, "RollbackVersion", input.rollbackVersion);
    return output;
}

HRESULT RollbackToManifestResponse::FromJson(const JsonValue& input)
{
    std::optional<Manifest> manifest{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Manifest", manifest));
    if (manifest)
    {
        this->SetManifest(std::move(*manifest));
    }

    return S_OK;
}

size_t RollbackToManifestResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGameSaveRollbackToManifestResponse const*> RollbackToManifestResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<RollbackToManifestResponse>(&this->Model());
}

size_t RollbackToManifestResponse::RequiredBufferSize(const PFGameSaveRollbackToManifestResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.manifest)
    {
        requiredSize += Manifest::RequiredBufferSize(*model.manifest);
    }
    return requiredSize;
}

HRESULT RollbackToManifestResponse::Copy(const PFGameSaveRollbackToManifestResponse& input, PFGameSaveRollbackToManifestResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<Manifest>(input.manifest);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.manifest = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue UpdateConfigForTitleRequest::ToJson() const
{
    return UpdateConfigForTitleRequest::ToJson(this->Model());
}

JsonValue UpdateConfigForTitleRequest::ToJson(const PFGameSaveUpdateConfigForTitleRequest& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "DisableClientRollbackToLastConflictLoser", input.disableClientRollbackToLastConflictLoser);
    JsonUtils::ObjectAddMember(output, "DisableClientRollbackToLastKnownGood", input.disableClientRollbackToLastKnownGood);
    JsonUtils::ObjectAddMember(output, "DisableUnsignaledOutOfProcessUpload", input.disableUnsignaledOutOfProcessUpload);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    return output;
}

HRESULT UpdateConfigForTitleResponse::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DisableClientRollbackToLastConflictLoser", this->m_model.disableClientRollbackToLastConflictLoser));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DisableClientRollbackToLastKnownGood", this->m_model.disableClientRollbackToLastKnownGood));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DisableUnsignaledOutOfProcessUpload", this->m_model.disableUnsignaledOutOfProcessUpload));

    String onboardingStatus{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "OnboardingStatus", onboardingStatus));
    this->SetOnboardingStatus(std::move(onboardingStatus));

    String perPlayerQuotaBytes{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PerPlayerQuotaBytes", perPlayerQuotaBytes));
    this->SetPerPlayerQuotaBytes(std::move(perPlayerQuotaBytes));

    return S_OK;
}

size_t UpdateConfigForTitleResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGameSaveUpdateConfigForTitleResponse const*> UpdateConfigForTitleResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UpdateConfigForTitleResponse>(&this->Model());
}

size_t UpdateConfigForTitleResponse::RequiredBufferSize(const PFGameSaveUpdateConfigForTitleResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.onboardingStatus)
    {
        requiredSize += (std::strlen(model.onboardingStatus) + 1);
    }
    if (model.perPlayerQuotaBytes)
    {
        requiredSize += (std::strlen(model.perPlayerQuotaBytes) + 1);
    }
    return requiredSize;
}

HRESULT UpdateConfigForTitleResponse::Copy(const PFGameSaveUpdateConfigForTitleResponse& input, PFGameSaveUpdateConfigForTitleResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.onboardingStatus);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.onboardingStatus = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.perPlayerQuotaBytes);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.perPlayerQuotaBytes = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue UpdateManifestRequest::ToJson() const
{
    return UpdateManifestRequest::ToJson(this->Model());
}

JsonValue UpdateManifestRequest::ToJson(const PFGameSaveUpdateManifestRequest& input)
{
    JsonValue output = JsonValue::object();
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "ManifestDescription", input.manifestDescription);
    JsonUtils::ObjectAddMember(output, "MarkAsKnownGood", input.markAsKnownGood);
    JsonUtils::ObjectAddMember<UploadProgress>(output, "UploadProgress", input.uploadProgress);
    JsonUtils::ObjectAddMember(output, "Version", input.version);
    return output;
}

HRESULT UpdateManifestResponse::FromJson(const JsonValue& input)
{
    std::optional<Manifest> manifest{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Manifest", manifest));
    if (manifest)
    {
        this->SetManifest(std::move(*manifest));
    }

    return S_OK;
}

size_t UpdateManifestResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFGameSaveUpdateManifestResponse const*> UpdateManifestResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UpdateManifestResponse>(&this->Model());
}

size_t UpdateManifestResponse::RequiredBufferSize(const PFGameSaveUpdateManifestResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.manifest)
    {
        requiredSize += Manifest::RequiredBufferSize(*model.manifest);
    }
    return requiredSize;
}

HRESULT UpdateManifestResponse::Copy(const PFGameSaveUpdateManifestResponse& input, PFGameSaveUpdateManifestResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<Manifest>(input.manifest);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.manifest = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

} // namespace GameSave
} // namespace PlayFab

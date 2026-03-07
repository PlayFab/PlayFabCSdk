#pragma once

#include "GameSaveTypeWrappers.h"
#include "Wrappers/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace GameSaveWrapper
{

// GameSave Classes
class DeleteManifestRequest : public Wrappers::PFGameSaveDeleteManifestRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveDeleteManifestRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGameSaveDeleteManifestRequest& input);
};

class FinalizeManifestConflict : public Wrappers::PFGameSaveFinalizeManifestConflictWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveFinalizeManifestConflictWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGameSaveFinalizeManifestConflict& input);
};

class FinalizedFileDetails : public Wrappers::PFGameSaveFinalizedFileDetailsWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveFinalizedFileDetailsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGameSaveFinalizedFileDetails& input);
};

class FinalizeManifestRequest : public Wrappers::PFGameSaveFinalizeManifestRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveFinalizeManifestRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGameSaveFinalizeManifestRequest& input);
};

class ManifestConflict : public Wrappers::PFGameSaveManifestConflictWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGameSaveManifestConflict>
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveManifestConflictWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGameSaveManifestConflict const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGameSaveManifestConflict& model);
    static HRESULT Copy(const PFGameSaveManifestConflict& input, PFGameSaveManifestConflict& output, ModelBuffer& buffer);
};

class FileMetadata : public Wrappers::PFGameSaveFileMetadataWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGameSaveFileMetadata>
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveFileMetadataWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGameSaveFileMetadata const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGameSaveFileMetadata& model);
    static HRESULT Copy(const PFGameSaveFileMetadata& input, PFGameSaveFileMetadata& output, ModelBuffer& buffer);
};

class ManifestMetadata : public Wrappers::PFGameSaveManifestMetadataWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFGameSaveManifestMetadata>
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveManifestMetadataWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGameSaveManifestMetadata& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGameSaveManifestMetadata const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGameSaveManifestMetadata& model);
    static HRESULT Copy(const PFGameSaveManifestMetadata& input, PFGameSaveManifestMetadata& output, ModelBuffer& buffer);
};

class UploadProgress : public Wrappers::PFGameSaveUploadProgressWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFGameSaveUploadProgress>
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveUploadProgressWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGameSaveUploadProgress& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGameSaveUploadProgress const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGameSaveUploadProgress& model);
    static HRESULT Copy(const PFGameSaveUploadProgress& input, PFGameSaveUploadProgress& output, ModelBuffer& buffer);
};

class Manifest : public Wrappers::PFGameSaveManifestWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGameSaveManifest>
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveManifestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGameSaveManifest const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGameSaveManifest& model);
    static HRESULT Copy(const PFGameSaveManifest& input, PFGameSaveManifest& output, ModelBuffer& buffer);
};

class FinalizeManifestResponse : public Wrappers::PFGameSaveFinalizeManifestResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGameSaveFinalizeManifestResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveFinalizeManifestResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGameSaveFinalizeManifestResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGameSaveFinalizeManifestResponse& model);
    static HRESULT Copy(const PFGameSaveFinalizeManifestResponse& input, PFGameSaveFinalizeManifestResponse& output, ModelBuffer& buffer);
};

class GetConfigForTitleRequest : public Wrappers::PFGameSaveGetConfigForTitleRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveGetConfigForTitleRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGameSaveGetConfigForTitleRequest& input);
};

class GetConfigForTitleResponse : public Wrappers::PFGameSaveGetConfigForTitleResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGameSaveGetConfigForTitleResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveGetConfigForTitleResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGameSaveGetConfigForTitleResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGameSaveGetConfigForTitleResponse& model);
    static HRESULT Copy(const PFGameSaveGetConfigForTitleResponse& input, PFGameSaveGetConfigForTitleResponse& output, ModelBuffer& buffer);
};

class GetManifestDownloadDetailsRequest : public Wrappers::PFGameSaveGetManifestDownloadDetailsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveGetManifestDownloadDetailsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGameSaveGetManifestDownloadDetailsRequest& input);
};

class FileDownloadDetails : public Wrappers::PFGameSaveFileDownloadDetailsWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGameSaveFileDownloadDetails>
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveFileDownloadDetailsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGameSaveFileDownloadDetails const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGameSaveFileDownloadDetails& model);
    static HRESULT Copy(const PFGameSaveFileDownloadDetails& input, PFGameSaveFileDownloadDetails& output, ModelBuffer& buffer);
};

class GetManifestDownloadDetailsResponse : public Wrappers::PFGameSaveGetManifestDownloadDetailsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGameSaveGetManifestDownloadDetailsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveGetManifestDownloadDetailsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGameSaveGetManifestDownloadDetailsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGameSaveGetManifestDownloadDetailsResponse& model);
    static HRESULT Copy(const PFGameSaveGetManifestDownloadDetailsResponse& input, PFGameSaveGetManifestDownloadDetailsResponse& output, ModelBuffer& buffer);
};

class GetQuotaForPlayerRequest : public Wrappers::PFGameSaveGetQuotaForPlayerRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveGetQuotaForPlayerRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGameSaveGetQuotaForPlayerRequest& input);
};

class GetQuotaForPlayerResponse : public Wrappers::PFGameSaveGetQuotaForPlayerResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGameSaveGetQuotaForPlayerResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveGetQuotaForPlayerResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGameSaveGetQuotaForPlayerResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGameSaveGetQuotaForPlayerResponse& model);
    static HRESULT Copy(const PFGameSaveGetQuotaForPlayerResponse& input, PFGameSaveGetQuotaForPlayerResponse& output, ModelBuffer& buffer);
};

class InitializeManifestRequest : public Wrappers::PFGameSaveInitializeManifestRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveInitializeManifestRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGameSaveInitializeManifestRequest& input);
};

class InitializeManifestResponse : public Wrappers::PFGameSaveInitializeManifestResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGameSaveInitializeManifestResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveInitializeManifestResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGameSaveInitializeManifestResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGameSaveInitializeManifestResponse& model);
    static HRESULT Copy(const PFGameSaveInitializeManifestResponse& input, PFGameSaveInitializeManifestResponse& output, ModelBuffer& buffer);
};

class FileToUpload : public Wrappers::PFGameSaveFileToUploadWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveFileToUploadWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGameSaveFileToUpload& input);
};

class InitiateUploadRequest : public Wrappers::PFGameSaveInitiateUploadRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveInitiateUploadRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGameSaveInitiateUploadRequest& input);
};

class AllocatedFile : public Wrappers::PFGameSaveAllocatedFileWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGameSaveAllocatedFile>
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveAllocatedFileWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGameSaveAllocatedFile const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGameSaveAllocatedFile& model);
    static HRESULT Copy(const PFGameSaveAllocatedFile& input, PFGameSaveAllocatedFile& output, ModelBuffer& buffer);
};

class InitiateUploadResponse : public Wrappers::PFGameSaveInitiateUploadResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGameSaveInitiateUploadResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveInitiateUploadResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGameSaveInitiateUploadResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGameSaveInitiateUploadResponse& model);
    static HRESULT Copy(const PFGameSaveInitiateUploadResponse& input, PFGameSaveInitiateUploadResponse& output, ModelBuffer& buffer);
};

class ListManifestsRequest : public Wrappers::PFGameSaveListManifestsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveListManifestsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGameSaveListManifestsRequest& input);
};

class TitleConfiguration : public Wrappers::PFGameSaveTitleConfigurationWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGameSaveTitleConfiguration>
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveTitleConfigurationWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGameSaveTitleConfiguration const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGameSaveTitleConfiguration& model);
    static HRESULT Copy(const PFGameSaveTitleConfiguration& input, PFGameSaveTitleConfiguration& output, ModelBuffer& buffer);
};

class ListManifestsResponse : public Wrappers::PFGameSaveListManifestsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGameSaveListManifestsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveListManifestsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGameSaveListManifestsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGameSaveListManifestsResponse& model);
    static HRESULT Copy(const PFGameSaveListManifestsResponse& input, PFGameSaveListManifestsResponse& output, ModelBuffer& buffer);
};

class RequestOnboardingRequest : public Wrappers::PFGameSaveRequestOnboardingRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveRequestOnboardingRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGameSaveRequestOnboardingRequest& input);
};

class RollbackToManifestRequest : public Wrappers::PFGameSaveRollbackToManifestRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveRollbackToManifestRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGameSaveRollbackToManifestRequest& input);
};

class RollbackToManifestResponse : public Wrappers::PFGameSaveRollbackToManifestResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGameSaveRollbackToManifestResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveRollbackToManifestResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGameSaveRollbackToManifestResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGameSaveRollbackToManifestResponse& model);
    static HRESULT Copy(const PFGameSaveRollbackToManifestResponse& input, PFGameSaveRollbackToManifestResponse& output, ModelBuffer& buffer);
};

class UpdateConfigForTitleRequest : public Wrappers::PFGameSaveUpdateConfigForTitleRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveUpdateConfigForTitleRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGameSaveUpdateConfigForTitleRequest& input);
};

class UpdateConfigForTitleResponse : public Wrappers::PFGameSaveUpdateConfigForTitleResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGameSaveUpdateConfigForTitleResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveUpdateConfigForTitleResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGameSaveUpdateConfigForTitleResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGameSaveUpdateConfigForTitleResponse& model);
    static HRESULT Copy(const PFGameSaveUpdateConfigForTitleResponse& input, PFGameSaveUpdateConfigForTitleResponse& output, ModelBuffer& buffer);
};

class UpdateManifestRequest : public Wrappers::PFGameSaveUpdateManifestRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveUpdateManifestRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFGameSaveUpdateManifestRequest& input);
};

class UpdateManifestResponse : public Wrappers::PFGameSaveUpdateManifestResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFGameSaveUpdateManifestResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFGameSaveUpdateManifestResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFGameSaveUpdateManifestResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFGameSaveUpdateManifestResponse& model);
    static HRESULT Copy(const PFGameSaveUpdateManifestResponse& input, PFGameSaveUpdateManifestResponse& output, ModelBuffer& buffer);
};

} // namespace GameSave

// EnumRange definitions used for Enum (de)serialization
} // namespace PlayFab

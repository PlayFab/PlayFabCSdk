#pragma once

#include <playfab/services/cpp/ProfilesTypeWrappers.h>
#include "Generated/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace Profiles
{

// Profiles Classes
class GetEntityProfileRequest : public Wrappers::PFProfilesGetEntityProfileRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFProfilesGetEntityProfileRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFProfilesGetEntityProfileRequest& input);
};

class EntityProfileFileMetadata : public Wrappers::PFProfilesEntityProfileFileMetadataWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFProfilesEntityProfileFileMetadata>
{
public:
    using ModelWrapperType = typename Wrappers::PFProfilesEntityProfileFileMetadataWrapper<Allocator>;
    using ModelWrapperType::ModelType;
    using DictionaryEntryType = ModelWrapperType::DictionaryEntryType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFProfilesEntityProfileFileMetadata const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFProfilesEntityProfileFileMetadata& model);
    static HRESULT Copy(const PFProfilesEntityProfileFileMetadata& input, PFProfilesEntityProfileFileMetadata& output, ModelBuffer& buffer);
};

class EntityDataObject : public Wrappers::PFProfilesEntityDataObjectWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFProfilesEntityDataObject>
{
public:
    using ModelWrapperType = typename Wrappers::PFProfilesEntityDataObjectWrapper<Allocator>;
    using ModelWrapperType::ModelType;
    using DictionaryEntryType = ModelWrapperType::DictionaryEntryType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFProfilesEntityDataObject const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFProfilesEntityDataObject& model);
    static HRESULT Copy(const PFProfilesEntityDataObject& input, PFProfilesEntityDataObject& output, ModelBuffer& buffer);
};

class EntityPermissionStatement : public Wrappers::PFProfilesEntityPermissionStatementWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFProfilesEntityPermissionStatement>
{
public:
    using ModelWrapperType = typename Wrappers::PFProfilesEntityPermissionStatementWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFProfilesEntityPermissionStatement& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFProfilesEntityPermissionStatement const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFProfilesEntityPermissionStatement& model);
    static HRESULT Copy(const PFProfilesEntityPermissionStatement& input, PFProfilesEntityPermissionStatement& output, ModelBuffer& buffer);
};

class EntityStatisticValue : public Wrappers::PFProfilesEntityStatisticValueWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFProfilesEntityStatisticValue>
{
public:
    using ModelWrapperType = typename Wrappers::PFProfilesEntityStatisticValueWrapper<Allocator>;
    using ModelWrapperType::ModelType;
    using DictionaryEntryType = ModelWrapperType::DictionaryEntryType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFProfilesEntityStatisticValue const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFProfilesEntityStatisticValue& model);
    static HRESULT Copy(const PFProfilesEntityStatisticValue& input, PFProfilesEntityStatisticValue& output, ModelBuffer& buffer);
};

class EntityProfileBody : public Wrappers::PFProfilesEntityProfileBodyWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFProfilesEntityProfileBody>
{
public:
    using ModelWrapperType = typename Wrappers::PFProfilesEntityProfileBodyWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFProfilesEntityProfileBody const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFProfilesEntityProfileBody& model);
    static HRESULT Copy(const PFProfilesEntityProfileBody& input, PFProfilesEntityProfileBody& output, ModelBuffer& buffer);
};

class GetEntityProfileResponse : public Wrappers::PFProfilesGetEntityProfileResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFProfilesGetEntityProfileResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFProfilesGetEntityProfileResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFProfilesGetEntityProfileResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFProfilesGetEntityProfileResponse& model);
    static HRESULT Copy(const PFProfilesGetEntityProfileResponse& input, PFProfilesGetEntityProfileResponse& output, ModelBuffer& buffer);
};

class GetEntityProfilesRequest : public Wrappers::PFProfilesGetEntityProfilesRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFProfilesGetEntityProfilesRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFProfilesGetEntityProfilesRequest& input);
};

class GetEntityProfilesResponse : public Wrappers::PFProfilesGetEntityProfilesResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFProfilesGetEntityProfilesResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFProfilesGetEntityProfilesResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFProfilesGetEntityProfilesResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFProfilesGetEntityProfilesResponse& model);
    static HRESULT Copy(const PFProfilesGetEntityProfilesResponse& input, PFProfilesGetEntityProfilesResponse& output, ModelBuffer& buffer);
};

class GetTitlePlayersFromMasterPlayerAccountIdsRequest : public Wrappers::PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsRequest& input);
};

class GetTitlePlayersFromMasterPlayerAccountIdsResponse : public Wrappers::PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsResponse& model);
    static HRESULT Copy(const PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsResponse& input, PFProfilesGetTitlePlayersFromMasterPlayerAccountIdsResponse& output, ModelBuffer& buffer);
};

class SetProfileLanguageRequest : public Wrappers::PFProfilesSetProfileLanguageRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFProfilesSetProfileLanguageRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFProfilesSetProfileLanguageRequest& input);
};

class SetProfileLanguageResponse : public Wrappers::PFProfilesSetProfileLanguageResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFProfilesSetProfileLanguageResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFProfilesSetProfileLanguageResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFProfilesSetProfileLanguageResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFProfilesSetProfileLanguageResponse& model);
    static HRESULT Copy(const PFProfilesSetProfileLanguageResponse& input, PFProfilesSetProfileLanguageResponse& output, ModelBuffer& buffer);
};

class SetEntityProfilePolicyRequest : public Wrappers::PFProfilesSetEntityProfilePolicyRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFProfilesSetEntityProfilePolicyRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFProfilesSetEntityProfilePolicyRequest& input);
};

class SetEntityProfilePolicyResponse : public Wrappers::PFProfilesSetEntityProfilePolicyResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFProfilesSetEntityProfilePolicyResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFProfilesSetEntityProfilePolicyResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFProfilesSetEntityProfilePolicyResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFProfilesSetEntityProfilePolicyResponse& model);
    static HRESULT Copy(const PFProfilesSetEntityProfilePolicyResponse& input, PFProfilesSetEntityProfilePolicyResponse& output, ModelBuffer& buffer);
};

} // namespace Profiles
// EnumRange definitions used for Enum (de)serialization
} // namespace PlayFab

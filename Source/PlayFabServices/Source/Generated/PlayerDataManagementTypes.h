#pragma once

#include <playfab/services/cpp/PlayerDataManagementTypeWrappers.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "Generated/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace PlayerDataManagement
{

// PlayerDataManagement Classes
class ClientDeletePlayerCustomPropertiesRequest : public Wrappers::PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequest& input);
};

class DeletedPropertyDetails : public Wrappers::PFPlayerDataManagementDeletedPropertyDetailsWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFPlayerDataManagementDeletedPropertyDetails>
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementDeletedPropertyDetailsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFPlayerDataManagementDeletedPropertyDetails const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFPlayerDataManagementDeletedPropertyDetails& model);
    static HRESULT Copy(const PFPlayerDataManagementDeletedPropertyDetails& input, PFPlayerDataManagementDeletedPropertyDetails& output, ModelBuffer& buffer);
};

class ClientDeletePlayerCustomPropertiesResult : public Wrappers::PFPlayerDataManagementClientDeletePlayerCustomPropertiesResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFPlayerDataManagementClientDeletePlayerCustomPropertiesResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementClientDeletePlayerCustomPropertiesResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFPlayerDataManagementClientDeletePlayerCustomPropertiesResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFPlayerDataManagementClientDeletePlayerCustomPropertiesResult& model);
    static HRESULT Copy(const PFPlayerDataManagementClientDeletePlayerCustomPropertiesResult& input, PFPlayerDataManagementClientDeletePlayerCustomPropertiesResult& output, ModelBuffer& buffer);
};

class ClientGetPlayerCustomPropertyRequest : public Wrappers::PFPlayerDataManagementClientGetPlayerCustomPropertyRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementClientGetPlayerCustomPropertyRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPlayerDataManagementClientGetPlayerCustomPropertyRequest& input);
};

class CustomPropertyDetails : public Wrappers::PFPlayerDataManagementCustomPropertyDetailsWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFPlayerDataManagementCustomPropertyDetails>
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementCustomPropertyDetailsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFPlayerDataManagementCustomPropertyDetails const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFPlayerDataManagementCustomPropertyDetails& model);
    static HRESULT Copy(const PFPlayerDataManagementCustomPropertyDetails& input, PFPlayerDataManagementCustomPropertyDetails& output, ModelBuffer& buffer);
};

class ClientGetPlayerCustomPropertyResult : public Wrappers::PFPlayerDataManagementClientGetPlayerCustomPropertyResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFPlayerDataManagementClientGetPlayerCustomPropertyResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementClientGetPlayerCustomPropertyResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFPlayerDataManagementClientGetPlayerCustomPropertyResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFPlayerDataManagementClientGetPlayerCustomPropertyResult& model);
    static HRESULT Copy(const PFPlayerDataManagementClientGetPlayerCustomPropertyResult& input, PFPlayerDataManagementClientGetPlayerCustomPropertyResult& output, ModelBuffer& buffer);
};

class GetUserDataRequest : public Wrappers::PFPlayerDataManagementGetUserDataRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementGetUserDataRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPlayerDataManagementGetUserDataRequest& input);
};

class ClientGetUserDataResult : public Wrappers::PFPlayerDataManagementClientGetUserDataResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFPlayerDataManagementClientGetUserDataResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementClientGetUserDataResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFPlayerDataManagementClientGetUserDataResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFPlayerDataManagementClientGetUserDataResult& model);
    static HRESULT Copy(const PFPlayerDataManagementClientGetUserDataResult& input, PFPlayerDataManagementClientGetUserDataResult& output, ModelBuffer& buffer);
};

class ClientListPlayerCustomPropertiesResult : public Wrappers::PFPlayerDataManagementClientListPlayerCustomPropertiesResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFPlayerDataManagementClientListPlayerCustomPropertiesResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementClientListPlayerCustomPropertiesResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFPlayerDataManagementClientListPlayerCustomPropertiesResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFPlayerDataManagementClientListPlayerCustomPropertiesResult& model);
    static HRESULT Copy(const PFPlayerDataManagementClientListPlayerCustomPropertiesResult& input, PFPlayerDataManagementClientListPlayerCustomPropertiesResult& output, ModelBuffer& buffer);
};

class UpdateProperty : public Wrappers::PFPlayerDataManagementUpdatePropertyWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementUpdatePropertyWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPlayerDataManagementUpdateProperty& input);
};

class ClientUpdatePlayerCustomPropertiesRequest : public Wrappers::PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequest& input);
};

class ClientUpdatePlayerCustomPropertiesResult : public Wrappers::PFPlayerDataManagementClientUpdatePlayerCustomPropertiesResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFPlayerDataManagementClientUpdatePlayerCustomPropertiesResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementClientUpdatePlayerCustomPropertiesResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFPlayerDataManagementClientUpdatePlayerCustomPropertiesResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFPlayerDataManagementClientUpdatePlayerCustomPropertiesResult& model);
    static HRESULT Copy(const PFPlayerDataManagementClientUpdatePlayerCustomPropertiesResult& input, PFPlayerDataManagementClientUpdatePlayerCustomPropertiesResult& output, ModelBuffer& buffer);
};

class ClientUpdateUserDataRequest : public Wrappers::PFPlayerDataManagementClientUpdateUserDataRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementClientUpdateUserDataRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPlayerDataManagementClientUpdateUserDataRequest& input);
};

class UpdateUserDataResult : public Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFPlayerDataManagementUpdateUserDataResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFPlayerDataManagementUpdateUserDataResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFPlayerDataManagementUpdateUserDataResult& model);
    static HRESULT Copy(const PFPlayerDataManagementUpdateUserDataResult& input, PFPlayerDataManagementUpdateUserDataResult& output, ModelBuffer& buffer);
};

class ServerDeletePlayerCustomPropertiesRequest : public Wrappers::PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequest& input);
};

class ServerDeletePlayerCustomPropertiesResult : public Wrappers::PFPlayerDataManagementServerDeletePlayerCustomPropertiesResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFPlayerDataManagementServerDeletePlayerCustomPropertiesResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementServerDeletePlayerCustomPropertiesResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFPlayerDataManagementServerDeletePlayerCustomPropertiesResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFPlayerDataManagementServerDeletePlayerCustomPropertiesResult& model);
    static HRESULT Copy(const PFPlayerDataManagementServerDeletePlayerCustomPropertiesResult& input, PFPlayerDataManagementServerDeletePlayerCustomPropertiesResult& output, ModelBuffer& buffer);
};

class ServerGetPlayerCustomPropertyRequest : public Wrappers::PFPlayerDataManagementServerGetPlayerCustomPropertyRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementServerGetPlayerCustomPropertyRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPlayerDataManagementServerGetPlayerCustomPropertyRequest& input);
};

class ServerGetPlayerCustomPropertyResult : public Wrappers::PFPlayerDataManagementServerGetPlayerCustomPropertyResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFPlayerDataManagementServerGetPlayerCustomPropertyResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementServerGetPlayerCustomPropertyResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFPlayerDataManagementServerGetPlayerCustomPropertyResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFPlayerDataManagementServerGetPlayerCustomPropertyResult& model);
    static HRESULT Copy(const PFPlayerDataManagementServerGetPlayerCustomPropertyResult& input, PFPlayerDataManagementServerGetPlayerCustomPropertyResult& output, ModelBuffer& buffer);
};

class ServerGetUserDataResult : public Wrappers::PFPlayerDataManagementServerGetUserDataResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFPlayerDataManagementServerGetUserDataResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementServerGetUserDataResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFPlayerDataManagementServerGetUserDataResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFPlayerDataManagementServerGetUserDataResult& model);
    static HRESULT Copy(const PFPlayerDataManagementServerGetUserDataResult& input, PFPlayerDataManagementServerGetUserDataResult& output, ModelBuffer& buffer);
};

class ListPlayerCustomPropertiesRequest : public Wrappers::PFPlayerDataManagementListPlayerCustomPropertiesRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementListPlayerCustomPropertiesRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPlayerDataManagementListPlayerCustomPropertiesRequest& input);
};

class ServerListPlayerCustomPropertiesResult : public Wrappers::PFPlayerDataManagementServerListPlayerCustomPropertiesResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFPlayerDataManagementServerListPlayerCustomPropertiesResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementServerListPlayerCustomPropertiesResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFPlayerDataManagementServerListPlayerCustomPropertiesResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFPlayerDataManagementServerListPlayerCustomPropertiesResult& model);
    static HRESULT Copy(const PFPlayerDataManagementServerListPlayerCustomPropertiesResult& input, PFPlayerDataManagementServerListPlayerCustomPropertiesResult& output, ModelBuffer& buffer);
};

class ServerUpdatePlayerCustomPropertiesRequest : public Wrappers::PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequest& input);
};

class ServerUpdatePlayerCustomPropertiesResult : public Wrappers::PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResult& model);
    static HRESULT Copy(const PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResult& input, PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResult& output, ModelBuffer& buffer);
};

class ServerUpdateUserDataRequest : public Wrappers::PFPlayerDataManagementServerUpdateUserDataRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementServerUpdateUserDataRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPlayerDataManagementServerUpdateUserDataRequest& input);
};

class UpdateUserInternalDataRequest : public Wrappers::PFPlayerDataManagementUpdateUserInternalDataRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayerDataManagementUpdateUserInternalDataRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPlayerDataManagementUpdateUserInternalDataRequest& input);
};

} // namespace PlayerDataManagement

// Json serialization helpers

// EnumRange definitions used for Enum (de)serialization

} // namespace PlayFab

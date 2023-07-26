#pragma once

#include <playfab/services/cpp/PlayerDataManagementTypeWrappers.h>
#include "Generated/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace PlayerDataManagement
{

// PlayerDataManagement Classes
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
// EnumRange definitions used for Enum (de)serialization
} // namespace PlayFab

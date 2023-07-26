#pragma once

#include <playfab/services/cpp/TitleDataManagementTypeWrappers.h>
#include "Generated/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace TitleDataManagement
{

// TitleDataManagement Classes
class GetPublisherDataRequest : public Wrappers::PFTitleDataManagementGetPublisherDataRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFTitleDataManagementGetPublisherDataRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFTitleDataManagementGetPublisherDataRequest& input);
};

class GetPublisherDataResult : public Wrappers::PFTitleDataManagementGetPublisherDataResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFTitleDataManagementGetPublisherDataResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFTitleDataManagementGetPublisherDataResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFTitleDataManagementGetPublisherDataResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFTitleDataManagementGetPublisherDataResult& model);
    static HRESULT Copy(const PFTitleDataManagementGetPublisherDataResult& input, PFTitleDataManagementGetPublisherDataResult& output, ModelBuffer& buffer);
};

class GetTimeResult : public Wrappers::PFTitleDataManagementGetTimeResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFTitleDataManagementGetTimeResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFTitleDataManagementGetTimeResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFTitleDataManagementGetTimeResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFTitleDataManagementGetTimeResult& model);
    static HRESULT Copy(const PFTitleDataManagementGetTimeResult& input, PFTitleDataManagementGetTimeResult& output, ModelBuffer& buffer);
};

class GetTitleDataRequest : public Wrappers::PFTitleDataManagementGetTitleDataRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFTitleDataManagementGetTitleDataRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFTitleDataManagementGetTitleDataRequest& input);
};

class GetTitleDataResult : public Wrappers::PFTitleDataManagementGetTitleDataResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFTitleDataManagementGetTitleDataResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFTitleDataManagementGetTitleDataResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFTitleDataManagementGetTitleDataResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFTitleDataManagementGetTitleDataResult& model);
    static HRESULT Copy(const PFTitleDataManagementGetTitleDataResult& input, PFTitleDataManagementGetTitleDataResult& output, ModelBuffer& buffer);
};

class GetTitleNewsRequest : public Wrappers::PFTitleDataManagementGetTitleNewsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFTitleDataManagementGetTitleNewsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFTitleDataManagementGetTitleNewsRequest& input);
};

class TitleNewsItem : public Wrappers::PFTitleDataManagementTitleNewsItemWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFTitleDataManagementTitleNewsItem>
{
public:
    using ModelWrapperType = typename Wrappers::PFTitleDataManagementTitleNewsItemWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFTitleDataManagementTitleNewsItem const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFTitleDataManagementTitleNewsItem& model);
    static HRESULT Copy(const PFTitleDataManagementTitleNewsItem& input, PFTitleDataManagementTitleNewsItem& output, ModelBuffer& buffer);
};

class GetTitleNewsResult : public Wrappers::PFTitleDataManagementGetTitleNewsResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFTitleDataManagementGetTitleNewsResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFTitleDataManagementGetTitleNewsResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFTitleDataManagementGetTitleNewsResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFTitleDataManagementGetTitleNewsResult& model);
    static HRESULT Copy(const PFTitleDataManagementGetTitleNewsResult& input, PFTitleDataManagementGetTitleNewsResult& output, ModelBuffer& buffer);
};

class SetPublisherDataRequest : public Wrappers::PFTitleDataManagementSetPublisherDataRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFTitleDataManagementSetPublisherDataRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFTitleDataManagementSetPublisherDataRequest& input);
};

class SetTitleDataRequest : public Wrappers::PFTitleDataManagementSetTitleDataRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFTitleDataManagementSetTitleDataRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFTitleDataManagementSetTitleDataRequest& input);
};

} // namespace TitleDataManagement
// EnumRange definitions used for Enum (de)serialization
} // namespace PlayFab

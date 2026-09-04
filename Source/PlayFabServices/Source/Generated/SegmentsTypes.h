#pragma once

#include <playfab/services/cpp/SegmentsTypeWrappers.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "Generated/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace Segments
{

// Segments Classes
class GetSegmentResult : public Wrappers::PFSegmentsGetSegmentResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFSegmentsGetSegmentResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsGetSegmentResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFSegmentsGetSegmentResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFSegmentsGetSegmentResult& model);
    static HRESULT Copy(const PFSegmentsGetSegmentResult& input, PFSegmentsGetSegmentResult& output, ModelBuffer& buffer);
};

class GetPlayerSegmentsResult : public Wrappers::PFSegmentsGetPlayerSegmentsResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFSegmentsGetPlayerSegmentsResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsGetPlayerSegmentsResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFSegmentsGetPlayerSegmentsResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFSegmentsGetPlayerSegmentsResult& model);
    static HRESULT Copy(const PFSegmentsGetPlayerSegmentsResult& input, PFSegmentsGetPlayerSegmentsResult& output, ModelBuffer& buffer);
};

class GetPlayerTagsRequest : public Wrappers::PFSegmentsGetPlayerTagsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsGetPlayerTagsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFSegmentsGetPlayerTagsRequest& input);
};

class GetPlayerTagsResult : public Wrappers::PFSegmentsGetPlayerTagsResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFSegmentsGetPlayerTagsResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsGetPlayerTagsResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFSegmentsGetPlayerTagsResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFSegmentsGetPlayerTagsResult& model);
    static HRESULT Copy(const PFSegmentsGetPlayerTagsResult& input, PFSegmentsGetPlayerTagsResult& output, ModelBuffer& buffer);
};

class AddPlayerTagRequest : public Wrappers::PFSegmentsAddPlayerTagRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsAddPlayerTagRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFSegmentsAddPlayerTagRequest& input);
};

class GetAllSegmentsResult : public Wrappers::PFSegmentsGetAllSegmentsResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFSegmentsGetAllSegmentsResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsGetAllSegmentsResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFSegmentsGetAllSegmentsResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFSegmentsGetAllSegmentsResult& model);
    static HRESULT Copy(const PFSegmentsGetAllSegmentsResult& input, PFSegmentsGetAllSegmentsResult& output, ModelBuffer& buffer);
};

class GetPlayersSegmentsRequest : public Wrappers::PFSegmentsGetPlayersSegmentsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsGetPlayersSegmentsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFSegmentsGetPlayersSegmentsRequest& input);
};

class RemovePlayerTagRequest : public Wrappers::PFSegmentsRemovePlayerTagRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFSegmentsRemovePlayerTagRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFSegmentsRemovePlayerTagRequest& input);
};

} // namespace Segments

// Json serialization helpers

// EnumRange definitions used for Enum (de)serialization

} // namespace PlayFab

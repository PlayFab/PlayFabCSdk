#pragma once

#include <playfab/services/cpp/PlayStreamTypeWrappers.h>
#include <playfab/services/cpp/TypeWrappers.h>
#include "Generated/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace PlayStream
{

// PlayStream Classes
class ExportPlayersInSegmentRequest : public Wrappers::PFPlayStreamExportPlayersInSegmentRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayStreamExportPlayersInSegmentRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPlayStreamExportPlayersInSegmentRequest& input);
};

class ExportPlayersInSegmentResult : public Wrappers::PFPlayStreamExportPlayersInSegmentResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFPlayStreamExportPlayersInSegmentResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayStreamExportPlayersInSegmentResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFPlayStreamExportPlayersInSegmentResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFPlayStreamExportPlayersInSegmentResult& model);
    static HRESULT Copy(const PFPlayStreamExportPlayersInSegmentResult& input, PFPlayStreamExportPlayersInSegmentResult& output, ModelBuffer& buffer);
};

class GetPlayersInSegmentExportRequest : public Wrappers::PFPlayStreamGetPlayersInSegmentExportRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayStreamGetPlayersInSegmentExportRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPlayStreamGetPlayersInSegmentExportRequest& input);
};

class GetPlayersInSegmentExportResponse : public Wrappers::PFPlayStreamGetPlayersInSegmentExportResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFPlayStreamGetPlayersInSegmentExportResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayStreamGetPlayersInSegmentExportResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFPlayStreamGetPlayersInSegmentExportResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFPlayStreamGetPlayersInSegmentExportResponse& model);
    static HRESULT Copy(const PFPlayStreamGetPlayersInSegmentExportResponse& input, PFPlayStreamGetPlayersInSegmentExportResponse& output, ModelBuffer& buffer);
};

class GetSegmentPlayerCountRequest : public Wrappers::PFPlayStreamGetSegmentPlayerCountRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayStreamGetSegmentPlayerCountRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFPlayStreamGetSegmentPlayerCountRequest& input);
};

class GetSegmentPlayerCountResult : public Wrappers::PFPlayStreamGetSegmentPlayerCountResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFPlayStreamGetSegmentPlayerCountResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFPlayStreamGetSegmentPlayerCountResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFPlayStreamGetSegmentPlayerCountResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFPlayStreamGetSegmentPlayerCountResult& model);
    static HRESULT Copy(const PFPlayStreamGetSegmentPlayerCountResult& input, PFPlayStreamGetSegmentPlayerCountResult& output, ModelBuffer& buffer);
};

} // namespace PlayStream

// Json serialization helpers

// EnumRange definitions used for Enum (de)serialization

} // namespace PlayFab

#pragma once

#include <playfab/core/cpp/EventsTypeWrappers.h>
#include "BaseModel.h"

namespace PlayFab
{
namespace Events
{

// Events Classes
class EventContents : public Wrappers::PFEventsEventContentsWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsEventContentsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFEventsEventContents& input);
};

class WriteEventsRequest : public Wrappers::PFEventsWriteEventsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsWriteEventsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFEventsWriteEventsRequest& input);
};

class WriteEventsResponse : public Wrappers::PFEventsWriteEventsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFEventsWriteEventsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsWriteEventsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFEventsWriteEventsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFEventsWriteEventsResponse& model);
    static HRESULT Copy(const PFEventsWriteEventsResponse& input, PFEventsWriteEventsResponse& output, ModelBuffer& buffer);
};

} // namespace Events

// EnumRange definitions used for Enum (de)serialization
} // namespace PlayFab

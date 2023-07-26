#pragma once

#include <playfab/core/cpp/EventsTypeWrappers.h>
#include "BaseModel.h"

namespace PlayFab
{
namespace Events
{

// Events Classes
class CreateTelemetryKeyRequest : public Wrappers::PFEventsCreateTelemetryKeyRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsCreateTelemetryKeyRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFEventsCreateTelemetryKeyRequest& input);
};

class TelemetryKeyDetails : public Wrappers::PFEventsTelemetryKeyDetailsWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFEventsTelemetryKeyDetails>
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsTelemetryKeyDetailsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFEventsTelemetryKeyDetails const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFEventsTelemetryKeyDetails& model);
    static HRESULT Copy(const PFEventsTelemetryKeyDetails& input, PFEventsTelemetryKeyDetails& output, ModelBuffer& buffer);
};

class CreateTelemetryKeyResponse : public Wrappers::PFEventsCreateTelemetryKeyResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFEventsCreateTelemetryKeyResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsCreateTelemetryKeyResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFEventsCreateTelemetryKeyResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFEventsCreateTelemetryKeyResponse& model);
    static HRESULT Copy(const PFEventsCreateTelemetryKeyResponse& input, PFEventsCreateTelemetryKeyResponse& output, ModelBuffer& buffer);
};

class DeleteTelemetryKeyRequest : public Wrappers::PFEventsDeleteTelemetryKeyRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsDeleteTelemetryKeyRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFEventsDeleteTelemetryKeyRequest& input);
};

class DeleteTelemetryKeyResponse : public Wrappers::PFEventsDeleteTelemetryKeyResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFEventsDeleteTelemetryKeyResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsDeleteTelemetryKeyResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFEventsDeleteTelemetryKeyResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFEventsDeleteTelemetryKeyResponse& model);
    static HRESULT Copy(const PFEventsDeleteTelemetryKeyResponse& input, PFEventsDeleteTelemetryKeyResponse& output, ModelBuffer& buffer);
};

class GetTelemetryKeyRequest : public Wrappers::PFEventsGetTelemetryKeyRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsGetTelemetryKeyRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFEventsGetTelemetryKeyRequest& input);
};

class GetTelemetryKeyResponse : public Wrappers::PFEventsGetTelemetryKeyResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFEventsGetTelemetryKeyResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsGetTelemetryKeyResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFEventsGetTelemetryKeyResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFEventsGetTelemetryKeyResponse& model);
    static HRESULT Copy(const PFEventsGetTelemetryKeyResponse& input, PFEventsGetTelemetryKeyResponse& output, ModelBuffer& buffer);
};

class ListTelemetryKeysRequest : public Wrappers::PFEventsListTelemetryKeysRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsListTelemetryKeysRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFEventsListTelemetryKeysRequest& input);
};

class ListTelemetryKeysResponse : public Wrappers::PFEventsListTelemetryKeysResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFEventsListTelemetryKeysResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsListTelemetryKeysResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFEventsListTelemetryKeysResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFEventsListTelemetryKeysResponse& model);
    static HRESULT Copy(const PFEventsListTelemetryKeysResponse& input, PFEventsListTelemetryKeysResponse& output, ModelBuffer& buffer);
};

class SetTelemetryKeyActiveRequest : public Wrappers::PFEventsSetTelemetryKeyActiveRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsSetTelemetryKeyActiveRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFEventsSetTelemetryKeyActiveRequest& input);
};

class SetTelemetryKeyActiveResponse : public Wrappers::PFEventsSetTelemetryKeyActiveResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFEventsSetTelemetryKeyActiveResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsSetTelemetryKeyActiveResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFEventsSetTelemetryKeyActiveResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFEventsSetTelemetryKeyActiveResponse& model);
    static HRESULT Copy(const PFEventsSetTelemetryKeyActiveResponse& input, PFEventsSetTelemetryKeyActiveResponse& output, ModelBuffer& buffer);
};

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

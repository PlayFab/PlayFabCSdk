#pragma once

#include <playfab/core/cpp/EventsTypeWrappers.h>
#include "BaseModel.h"

namespace PlayFab
{
namespace Events
{

// Events Classes
class DeleteDataConnectionRequest : public Wrappers::PFEventsDeleteDataConnectionRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsDeleteDataConnectionRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFEventsDeleteDataConnectionRequest& input);
};

class DeleteDataConnectionResponse : public Wrappers::PFEventsDeleteDataConnectionResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFEventsDeleteDataConnectionResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsDeleteDataConnectionResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFEventsDeleteDataConnectionResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFEventsDeleteDataConnectionResponse& model);
    static HRESULT Copy(const PFEventsDeleteDataConnectionResponse& input, PFEventsDeleteDataConnectionResponse& output, ModelBuffer& buffer);
};

class GetDataConnectionRequest : public Wrappers::PFEventsGetDataConnectionRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsGetDataConnectionRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFEventsGetDataConnectionRequest& input);
};

class DataConnectionAzureBlobSettings : public Wrappers::PFEventsDataConnectionAzureBlobSettingsWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFEventsDataConnectionAzureBlobSettings>
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsDataConnectionAzureBlobSettingsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFEventsDataConnectionAzureBlobSettings& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFEventsDataConnectionAzureBlobSettings const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFEventsDataConnectionAzureBlobSettings& model);
    static HRESULT Copy(const PFEventsDataConnectionAzureBlobSettings& input, PFEventsDataConnectionAzureBlobSettings& output, ModelBuffer& buffer);
};

class DataConnectionAzureDataExplorerSettings : public Wrappers::PFEventsDataConnectionAzureDataExplorerSettingsWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFEventsDataConnectionAzureDataExplorerSettings>
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsDataConnectionAzureDataExplorerSettingsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFEventsDataConnectionAzureDataExplorerSettings& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFEventsDataConnectionAzureDataExplorerSettings const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFEventsDataConnectionAzureDataExplorerSettings& model);
    static HRESULT Copy(const PFEventsDataConnectionAzureDataExplorerSettings& input, PFEventsDataConnectionAzureDataExplorerSettings& output, ModelBuffer& buffer);
};

class DataConnectionFabricKQLSettings : public Wrappers::PFEventsDataConnectionFabricKQLSettingsWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFEventsDataConnectionFabricKQLSettings>
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsDataConnectionFabricKQLSettingsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFEventsDataConnectionFabricKQLSettings& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFEventsDataConnectionFabricKQLSettings const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFEventsDataConnectionFabricKQLSettings& model);
    static HRESULT Copy(const PFEventsDataConnectionFabricKQLSettings& input, PFEventsDataConnectionFabricKQLSettings& output, ModelBuffer& buffer);
};

class DataConnectionSettings : public Wrappers::PFEventsDataConnectionSettingsWrapper<Allocator>, public InputModel, public ServiceOutputModel, public ClientOutputModel<PFEventsDataConnectionSettings>
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsDataConnectionSettingsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFEventsDataConnectionSettings& input);
    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFEventsDataConnectionSettings const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFEventsDataConnectionSettings& model);
    static HRESULT Copy(const PFEventsDataConnectionSettings& input, PFEventsDataConnectionSettings& output, ModelBuffer& buffer);
};

class DataConnectionStatusDetails : public Wrappers::PFEventsDataConnectionStatusDetailsWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFEventsDataConnectionStatusDetails>
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsDataConnectionStatusDetailsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFEventsDataConnectionStatusDetails const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFEventsDataConnectionStatusDetails& model);
    static HRESULT Copy(const PFEventsDataConnectionStatusDetails& input, PFEventsDataConnectionStatusDetails& output, ModelBuffer& buffer);
};

class DataConnectionDetails : public Wrappers::PFEventsDataConnectionDetailsWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFEventsDataConnectionDetails>
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsDataConnectionDetailsWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFEventsDataConnectionDetails const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFEventsDataConnectionDetails& model);
    static HRESULT Copy(const PFEventsDataConnectionDetails& input, PFEventsDataConnectionDetails& output, ModelBuffer& buffer);
};

class GetDataConnectionResponse : public Wrappers::PFEventsGetDataConnectionResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFEventsGetDataConnectionResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsGetDataConnectionResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFEventsGetDataConnectionResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFEventsGetDataConnectionResponse& model);
    static HRESULT Copy(const PFEventsGetDataConnectionResponse& input, PFEventsGetDataConnectionResponse& output, ModelBuffer& buffer);
};

class ListDataConnectionsRequest : public Wrappers::PFEventsListDataConnectionsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsListDataConnectionsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFEventsListDataConnectionsRequest& input);
};

class ListDataConnectionsResponse : public Wrappers::PFEventsListDataConnectionsResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFEventsListDataConnectionsResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsListDataConnectionsResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFEventsListDataConnectionsResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFEventsListDataConnectionsResponse& model);
    static HRESULT Copy(const PFEventsListDataConnectionsResponse& input, PFEventsListDataConnectionsResponse& output, ModelBuffer& buffer);
};

class SetDataConnectionRequest : public Wrappers::PFEventsSetDataConnectionRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsSetDataConnectionRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFEventsSetDataConnectionRequest& input);
};

class SetDataConnectionResponse : public Wrappers::PFEventsSetDataConnectionResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFEventsSetDataConnectionResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsSetDataConnectionResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFEventsSetDataConnectionResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFEventsSetDataConnectionResponse& model);
    static HRESULT Copy(const PFEventsSetDataConnectionResponse& input, PFEventsSetDataConnectionResponse& output, ModelBuffer& buffer);
};

class SetDataConnectionActiveRequest : public Wrappers::PFEventsSetDataConnectionActiveRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsSetDataConnectionActiveRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFEventsSetDataConnectionActiveRequest& input);
};

class SetDataConnectionActiveResponse : public Wrappers::PFEventsSetDataConnectionActiveResponseWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFEventsSetDataConnectionActiveResponse>
{
public:
    using ModelWrapperType = typename Wrappers::PFEventsSetDataConnectionActiveResponseWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFEventsSetDataConnectionActiveResponse const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFEventsSetDataConnectionActiveResponse& model);
    static HRESULT Copy(const PFEventsSetDataConnectionActiveResponse& input, PFEventsSetDataConnectionActiveResponse& output, ModelBuffer& buffer);
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
template<typename T> struct EnumRange;

template<> struct EnumRange<PFEventsDataConnectionErrorState>
{
    static constexpr PFEventsDataConnectionErrorState maxValue = PFEventsDataConnectionErrorState::Error;
};

template<> struct EnumRange<PFEventsDataConnectionType>
{
    static constexpr PFEventsDataConnectionType maxValue = PFEventsDataConnectionType::FabricKQL;
};

} // namespace PlayFab

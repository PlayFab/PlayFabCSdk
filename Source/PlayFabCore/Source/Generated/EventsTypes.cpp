#include "stdafx.h"
#include "EventsTypes.h"
#include "JsonUtils.h"

namespace PlayFab
{
namespace Events
{

JsonValue CreateTelemetryKeyRequest::ToJson() const
{
    return CreateTelemetryKeyRequest::ToJson(this->Model());
}

JsonValue CreateTelemetryKeyRequest::ToJson(const PFEventsCreateTelemetryKeyRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "KeyName", input.keyName);
    return output;
}

HRESULT TelemetryKeyDetails::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "CreateTime", this->m_model.createTime));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "IsActive", this->m_model.isActive));

    String keyValue{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "KeyValue", keyValue));
    this->SetKeyValue(std::move(keyValue));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMemberTime(input, "LastUpdateTime", this->m_model.lastUpdateTime));

    String name{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Name", name));
    this->SetName(std::move(name));

    return S_OK;
}

size_t TelemetryKeyDetails::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFEventsTelemetryKeyDetails const*> TelemetryKeyDetails::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<TelemetryKeyDetails>(&this->Model());
}

size_t TelemetryKeyDetails::RequiredBufferSize(const PFEventsTelemetryKeyDetails& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.keyValue)
    {
        requiredSize += (std::strlen(model.keyValue) + 1);
    }
    if (model.name)
    {
        requiredSize += (std::strlen(model.name) + 1);
    }
    return requiredSize;
}

HRESULT TelemetryKeyDetails::Copy(const PFEventsTelemetryKeyDetails& input, PFEventsTelemetryKeyDetails& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.keyValue); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.keyValue = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.name); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.name = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT CreateTelemetryKeyResponse::FromJson(const JsonValue& input)
{
    std::optional<TelemetryKeyDetails> newKeyDetails{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "NewKeyDetails", newKeyDetails));
    if (newKeyDetails)
    {
        this->SetNewKeyDetails(std::move(*newKeyDetails));
    }

    return S_OK;
}

size_t CreateTelemetryKeyResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFEventsCreateTelemetryKeyResponse const*> CreateTelemetryKeyResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<CreateTelemetryKeyResponse>(&this->Model());
}

size_t CreateTelemetryKeyResponse::RequiredBufferSize(const PFEventsCreateTelemetryKeyResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.newKeyDetails)
    {
        requiredSize += TelemetryKeyDetails::RequiredBufferSize(*model.newKeyDetails);
    }
    return requiredSize;
}

HRESULT CreateTelemetryKeyResponse::Copy(const PFEventsCreateTelemetryKeyResponse& input, PFEventsCreateTelemetryKeyResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<TelemetryKeyDetails>(input.newKeyDetails); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.newKeyDetails = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue DeleteTelemetryKeyRequest::ToJson() const
{
    return DeleteTelemetryKeyRequest::ToJson(this->Model());
}

JsonValue DeleteTelemetryKeyRequest::ToJson(const PFEventsDeleteTelemetryKeyRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "KeyName", input.keyName);
    return output;
}

HRESULT DeleteTelemetryKeyResponse::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "WasKeyDeleted", this->m_model.wasKeyDeleted));

    return S_OK;
}

size_t DeleteTelemetryKeyResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFEventsDeleteTelemetryKeyResponse const*> DeleteTelemetryKeyResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<DeleteTelemetryKeyResponse>(&this->Model());
}

size_t DeleteTelemetryKeyResponse::RequiredBufferSize(const PFEventsDeleteTelemetryKeyResponse& model)
{
    UNREFERENCED_PARAMETER(model); // Fixed size
    return sizeof(ModelType);
}

HRESULT DeleteTelemetryKeyResponse::Copy(const PFEventsDeleteTelemetryKeyResponse& input, PFEventsDeleteTelemetryKeyResponse& output, ModelBuffer& buffer)
{
    output = input;
    UNREFERENCED_PARAMETER(buffer); // Fixed size
    return S_OK;
}

JsonValue GetTelemetryKeyRequest::ToJson() const
{
    return GetTelemetryKeyRequest::ToJson(this->Model());
}

JsonValue GetTelemetryKeyRequest::ToJson(const PFEventsGetTelemetryKeyRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "KeyName", input.keyName);
    return output;
}

HRESULT GetTelemetryKeyResponse::FromJson(const JsonValue& input)
{
    std::optional<TelemetryKeyDetails> keyDetails{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "KeyDetails", keyDetails));
    if (keyDetails)
    {
        this->SetKeyDetails(std::move(*keyDetails));
    }

    return S_OK;
}

size_t GetTelemetryKeyResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFEventsGetTelemetryKeyResponse const*> GetTelemetryKeyResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetTelemetryKeyResponse>(&this->Model());
}

size_t GetTelemetryKeyResponse::RequiredBufferSize(const PFEventsGetTelemetryKeyResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.keyDetails)
    {
        requiredSize += TelemetryKeyDetails::RequiredBufferSize(*model.keyDetails);
    }
    return requiredSize;
}

HRESULT GetTelemetryKeyResponse::Copy(const PFEventsGetTelemetryKeyResponse& input, PFEventsGetTelemetryKeyResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<TelemetryKeyDetails>(input.keyDetails); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.keyDetails = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ListTelemetryKeysRequest::ToJson() const
{
    return ListTelemetryKeysRequest::ToJson(this->Model());
}

JsonValue ListTelemetryKeysRequest::ToJson(const PFEventsListTelemetryKeysRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    return output;
}

HRESULT ListTelemetryKeysResponse::FromJson(const JsonValue& input)
{
    ModelVector<TelemetryKeyDetails> keyDetails{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<TelemetryKeyDetails>(input, "KeyDetails", keyDetails));
    this->SetKeyDetails(std::move(keyDetails));

    return S_OK;
}

size_t ListTelemetryKeysResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFEventsListTelemetryKeysResponse const*> ListTelemetryKeysResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ListTelemetryKeysResponse>(&this->Model());
}

size_t ListTelemetryKeysResponse::RequiredBufferSize(const PFEventsListTelemetryKeysResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFEventsTelemetryKeyDetails*) + sizeof(PFEventsTelemetryKeyDetails*) * model.keyDetailsCount);
    for (size_t i = 0; i < model.keyDetailsCount; ++i)
    {
        requiredSize += TelemetryKeyDetails::RequiredBufferSize(*model.keyDetails[i]);
    }
    return requiredSize;
}

HRESULT ListTelemetryKeysResponse::Copy(const PFEventsListTelemetryKeysResponse& input, PFEventsListTelemetryKeysResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<TelemetryKeyDetails>(input.keyDetails, input.keyDetailsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.keyDetails = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue SetTelemetryKeyActiveRequest::ToJson() const
{
    return SetTelemetryKeyActiveRequest::ToJson(this->Model());
}

JsonValue SetTelemetryKeyActiveRequest::ToJson(const PFEventsSetTelemetryKeyActiveRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "Active", input.active);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "KeyName", input.keyName);
    return output;
}

HRESULT SetTelemetryKeyActiveResponse::FromJson(const JsonValue& input)
{
    std::optional<TelemetryKeyDetails> keyDetails{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "KeyDetails", keyDetails));
    if (keyDetails)
    {
        this->SetKeyDetails(std::move(*keyDetails));
    }

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "WasKeyUpdated", this->m_model.wasKeyUpdated));

    return S_OK;
}

size_t SetTelemetryKeyActiveResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFEventsSetTelemetryKeyActiveResponse const*> SetTelemetryKeyActiveResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<SetTelemetryKeyActiveResponse>(&this->Model());
}

size_t SetTelemetryKeyActiveResponse::RequiredBufferSize(const PFEventsSetTelemetryKeyActiveResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.keyDetails)
    {
        requiredSize += TelemetryKeyDetails::RequiredBufferSize(*model.keyDetails);
    }
    return requiredSize;
}

HRESULT SetTelemetryKeyActiveResponse::Copy(const PFEventsSetTelemetryKeyActiveResponse& input, PFEventsSetTelemetryKeyActiveResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<TelemetryKeyDetails>(input.keyDetails); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.keyDetails = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue EventContents::ToJson() const
{
    return EventContents::ToJson(this->Model());
}

JsonValue EventContents::ToJson(const PFEventsEventContents& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "EventNamespace", input.eventNamespace);
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    JsonUtils::ObjectAddMember(output, "OriginalId", input.originalId);
    JsonUtils::ObjectAddMemberTime(output, "OriginalTimestamp", input.originalTimestamp);
    JsonUtils::ObjectAddMember(output, "Payload", input.payload);
    JsonUtils::ObjectAddMember(output, "PayloadJSON", input.payloadJSON);
    return output;
}

JsonValue WriteEventsRequest::ToJson() const
{
    return WriteEventsRequest::ToJson(this->Model());
}

JsonValue WriteEventsRequest::ToJson(const PFEventsWriteEventsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMemberArray<EventContents>(output, "Events", input.events, input.eventsCount);
    return output;
}

HRESULT WriteEventsResponse::FromJson(const JsonValue& input)
{
    CStringVector assignedEventIds{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "AssignedEventIds", assignedEventIds));
    this->SetAssignedEventIds(std::move(assignedEventIds));

    return S_OK;
}

size_t WriteEventsResponse::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFEventsWriteEventsResponse const*> WriteEventsResponse::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<WriteEventsResponse>(&this->Model());
}

size_t WriteEventsResponse::RequiredBufferSize(const PFEventsWriteEventsResponse& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(char*) + sizeof(char*) * model.assignedEventIdsCount);
    for (size_t i = 0; i < model.assignedEventIdsCount; ++i)
    {
        requiredSize += (std::strlen(model.assignedEventIds[i]) + 1);
    }
    return requiredSize;
}

HRESULT WriteEventsResponse::Copy(const PFEventsWriteEventsResponse& input, PFEventsWriteEventsResponse& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray(input.assignedEventIds, input.assignedEventIdsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.assignedEventIds = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

} // namespace Events
} // namespace PlayFab

#include "stdafx.h"
#include "PlayerDataManagementTypes.h"
#include "JsonUtils.h"
#include "Types.h"

namespace PlayFab
{
namespace PlayerDataManagement
{

JsonValue ClientDeletePlayerCustomPropertiesRequest::ToJson() const
{
    return ClientDeletePlayerCustomPropertiesRequest::ToJson(this->Model());
}

JsonValue ClientDeletePlayerCustomPropertiesRequest::ToJson(const PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "ExpectedPropertiesVersion", input.expectedPropertiesVersion);
    JsonUtils::ObjectAddMemberArray(output, "PropertyNames", input.propertyNames, input.propertyNamesCount);
    return output;
}

HRESULT DeletedPropertyDetails::FromJson(const JsonValue& input)
{
    String name{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Name", name));
    this->SetName(std::move(name));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "WasDeleted", this->m_model.wasDeleted));

    return S_OK;
}

size_t DeletedPropertyDetails::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFPlayerDataManagementDeletedPropertyDetails const*> DeletedPropertyDetails::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<DeletedPropertyDetails>(&this->Model());
}

size_t DeletedPropertyDetails::RequiredBufferSize(const PFPlayerDataManagementDeletedPropertyDetails& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.name)
    {
        requiredSize += (std::strlen(model.name) + 1);
    }
    return requiredSize;
}

HRESULT DeletedPropertyDetails::Copy(const PFPlayerDataManagementDeletedPropertyDetails& input, PFPlayerDataManagementDeletedPropertyDetails& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.name);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.name = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT ClientDeletePlayerCustomPropertiesResult::FromJson(const JsonValue& input)
{
    ModelVector<DeletedPropertyDetails> deletedProperties{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<DeletedPropertyDetails>(input, "DeletedProperties", deletedProperties));
    this->SetDeletedProperties(std::move(deletedProperties));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PropertiesVersion", this->m_model.propertiesVersion));

    return S_OK;
}

size_t ClientDeletePlayerCustomPropertiesResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFPlayerDataManagementClientDeletePlayerCustomPropertiesResult const*> ClientDeletePlayerCustomPropertiesResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ClientDeletePlayerCustomPropertiesResult>(&this->Model());
}

size_t ClientDeletePlayerCustomPropertiesResult::RequiredBufferSize(const PFPlayerDataManagementClientDeletePlayerCustomPropertiesResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFPlayerDataManagementDeletedPropertyDetails*) + sizeof(PFPlayerDataManagementDeletedPropertyDetails*) * model.deletedPropertiesCount);
    for (size_t i = 0; i < model.deletedPropertiesCount; ++i)
    {
        requiredSize += DeletedPropertyDetails::RequiredBufferSize(*model.deletedProperties[i]);
    }
    return requiredSize;
}

HRESULT ClientDeletePlayerCustomPropertiesResult::Copy(const PFPlayerDataManagementClientDeletePlayerCustomPropertiesResult& input, PFPlayerDataManagementClientDeletePlayerCustomPropertiesResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<DeletedPropertyDetails>(input.deletedProperties, input.deletedPropertiesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.deletedProperties = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ClientGetPlayerCustomPropertyRequest::ToJson() const
{
    return ClientGetPlayerCustomPropertyRequest::ToJson(this->Model());
}

JsonValue ClientGetPlayerCustomPropertyRequest::ToJson(const PFPlayerDataManagementClientGetPlayerCustomPropertyRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "PropertyName", input.propertyName);
    return output;
}

HRESULT CustomPropertyDetails::FromJson(const JsonValue& input)
{
    String name{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Name", name));
    this->SetName(std::move(name));

    JsonObject value{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Value", value));
    this->SetValue(std::move(value));

    return S_OK;
}

size_t CustomPropertyDetails::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFPlayerDataManagementCustomPropertyDetails const*> CustomPropertyDetails::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<CustomPropertyDetails>(&this->Model());
}

size_t CustomPropertyDetails::RequiredBufferSize(const PFPlayerDataManagementCustomPropertyDetails& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.name)
    {
        requiredSize += (std::strlen(model.name) + 1);
    }
    if (model.value.stringValue)
    {
    requiredSize += (std::strlen(model.value.stringValue) + 1);
    }
    return requiredSize;
}

HRESULT CustomPropertyDetails::Copy(const PFPlayerDataManagementCustomPropertyDetails& input, PFPlayerDataManagementCustomPropertyDetails& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.name);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.name = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.value.stringValue);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.value.stringValue = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT ClientGetPlayerCustomPropertyResult::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PropertiesVersion", this->m_model.propertiesVersion));

    std::optional<CustomPropertyDetails> property{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Property", property));
    if (property)
    {
        this->SetProperty(std::move(*property));
    }

    return S_OK;
}

size_t ClientGetPlayerCustomPropertyResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFPlayerDataManagementClientGetPlayerCustomPropertyResult const*> ClientGetPlayerCustomPropertyResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ClientGetPlayerCustomPropertyResult>(&this->Model());
}

size_t ClientGetPlayerCustomPropertyResult::RequiredBufferSize(const PFPlayerDataManagementClientGetPlayerCustomPropertyResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.property)
    {
        requiredSize += CustomPropertyDetails::RequiredBufferSize(*model.property);
    }
    return requiredSize;
}

HRESULT ClientGetPlayerCustomPropertyResult::Copy(const PFPlayerDataManagementClientGetPlayerCustomPropertyResult& input, PFPlayerDataManagementClientGetPlayerCustomPropertyResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<CustomPropertyDetails>(input.property);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.property = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetUserDataRequest::ToJson() const
{
    return GetUserDataRequest::ToJson(this->Model());
}

JsonValue GetUserDataRequest::ToJson(const PFPlayerDataManagementGetUserDataRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "IfChangedFromDataVersion", input.ifChangedFromDataVersion);
    JsonUtils::ObjectAddMemberArray(output, "Keys", input.keys, input.keysCount);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    return output;
}

HRESULT ClientGetUserDataResult::FromJson(const JsonValue& input)
{
    ModelDictionaryEntryVector<UserDataRecord> data{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<UserDataRecord>(input, "Data", data));
    this->SetData(std::move(data));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DataVersion", this->m_model.dataVersion));

    return S_OK;
}

size_t ClientGetUserDataResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFPlayerDataManagementClientGetUserDataResult const*> ClientGetUserDataResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ClientGetUserDataResult>(&this->Model());
}

size_t ClientGetUserDataResult::RequiredBufferSize(const PFPlayerDataManagementClientGetUserDataResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFUserDataRecordDictionaryEntry) + sizeof(PFUserDataRecordDictionaryEntry) * model.dataCount);
    for (size_t i = 0; i < model.dataCount; ++i)
    {
        requiredSize += (std::strlen(model.data[i].key) + 1);
        requiredSize += UserDataRecord::RequiredBufferSize(*model.data[i].value);
    }
    return requiredSize;
}

HRESULT ClientGetUserDataResult::Copy(const PFPlayerDataManagementClientGetUserDataResult& input, PFPlayerDataManagementClientGetUserDataResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToDictionary<UserDataRecord>(input.data, input.dataCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.data = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT ClientListPlayerCustomPropertiesResult::FromJson(const JsonValue& input)
{
    ModelVector<CustomPropertyDetails> properties{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<CustomPropertyDetails>(input, "Properties", properties));
    this->SetProperties(std::move(properties));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PropertiesVersion", this->m_model.propertiesVersion));

    return S_OK;
}

size_t ClientListPlayerCustomPropertiesResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFPlayerDataManagementClientListPlayerCustomPropertiesResult const*> ClientListPlayerCustomPropertiesResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ClientListPlayerCustomPropertiesResult>(&this->Model());
}

size_t ClientListPlayerCustomPropertiesResult::RequiredBufferSize(const PFPlayerDataManagementClientListPlayerCustomPropertiesResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFPlayerDataManagementCustomPropertyDetails*) + sizeof(PFPlayerDataManagementCustomPropertyDetails*) * model.propertiesCount);
    for (size_t i = 0; i < model.propertiesCount; ++i)
    {
        requiredSize += CustomPropertyDetails::RequiredBufferSize(*model.properties[i]);
    }
    return requiredSize;
}

HRESULT ClientListPlayerCustomPropertiesResult::Copy(const PFPlayerDataManagementClientListPlayerCustomPropertiesResult& input, PFPlayerDataManagementClientListPlayerCustomPropertiesResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<CustomPropertyDetails>(input.properties, input.propertiesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.properties = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue UpdateProperty::ToJson() const
{
    return UpdateProperty::ToJson(this->Model());
}

JsonValue UpdateProperty::ToJson(const PFPlayerDataManagementUpdateProperty& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "Name", input.name);
    JsonUtils::ObjectAddMember(output, "Value", input.value);
    return output;
}

JsonValue ClientUpdatePlayerCustomPropertiesRequest::ToJson() const
{
    return ClientUpdatePlayerCustomPropertiesRequest::ToJson(this->Model());
}

JsonValue ClientUpdatePlayerCustomPropertiesRequest::ToJson(const PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "ExpectedPropertiesVersion", input.expectedPropertiesVersion);
    JsonUtils::ObjectAddMemberArray<UpdateProperty>(output, "Properties", input.properties, input.propertiesCount);
    return output;
}

HRESULT ClientUpdatePlayerCustomPropertiesResult::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PropertiesVersion", this->m_model.propertiesVersion));

    return S_OK;
}

size_t ClientUpdatePlayerCustomPropertiesResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFPlayerDataManagementClientUpdatePlayerCustomPropertiesResult const*> ClientUpdatePlayerCustomPropertiesResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ClientUpdatePlayerCustomPropertiesResult>(&this->Model());
}

size_t ClientUpdatePlayerCustomPropertiesResult::RequiredBufferSize(const PFPlayerDataManagementClientUpdatePlayerCustomPropertiesResult& model)
{
    UNREFERENCED_PARAMETER(model); // Fixed size
    return sizeof(ModelType);
}

HRESULT ClientUpdatePlayerCustomPropertiesResult::Copy(const PFPlayerDataManagementClientUpdatePlayerCustomPropertiesResult& input, PFPlayerDataManagementClientUpdatePlayerCustomPropertiesResult& output, ModelBuffer& buffer)
{
    output = input;
    UNREFERENCED_PARAMETER(buffer); // Fixed size
    return S_OK;
}

JsonValue ClientUpdateUserDataRequest::ToJson() const
{
    return ClientUpdateUserDataRequest::ToJson(this->Model());
}

JsonValue ClientUpdateUserDataRequest::ToJson(const PFPlayerDataManagementClientUpdateUserDataRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMemberDictionary(output, "Data", input.data, input.dataCount);
    JsonUtils::ObjectAddMemberArray(output, "KeysToRemove", input.keysToRemove, input.keysToRemoveCount);
    JsonUtils::ObjectAddMember(output, "Permission", input.permission);
    return output;
}

HRESULT UpdateUserDataResult::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DataVersion", this->m_model.dataVersion));

    return S_OK;
}

size_t UpdateUserDataResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFPlayerDataManagementUpdateUserDataResult const*> UpdateUserDataResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UpdateUserDataResult>(&this->Model());
}

size_t UpdateUserDataResult::RequiredBufferSize(const PFPlayerDataManagementUpdateUserDataResult& model)
{
    UNREFERENCED_PARAMETER(model); // Fixed size
    return sizeof(ModelType);
}

HRESULT UpdateUserDataResult::Copy(const PFPlayerDataManagementUpdateUserDataResult& input, PFPlayerDataManagementUpdateUserDataResult& output, ModelBuffer& buffer)
{
    output = input;
    UNREFERENCED_PARAMETER(buffer); // Fixed size
    return S_OK;
}

JsonValue ServerDeletePlayerCustomPropertiesRequest::ToJson() const
{
    return ServerDeletePlayerCustomPropertiesRequest::ToJson(this->Model());
}

JsonValue ServerDeletePlayerCustomPropertiesRequest::ToJson(const PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "ExpectedPropertiesVersion", input.expectedPropertiesVersion);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    JsonUtils::ObjectAddMemberArray(output, "PropertyNames", input.propertyNames, input.propertyNamesCount);
    return output;
}

HRESULT ServerDeletePlayerCustomPropertiesResult::FromJson(const JsonValue& input)
{
    ModelVector<DeletedPropertyDetails> deletedProperties{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<DeletedPropertyDetails>(input, "DeletedProperties", deletedProperties));
    this->SetDeletedProperties(std::move(deletedProperties));

    String playFabId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PlayFabId", playFabId));
    this->SetPlayFabId(std::move(playFabId));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PropertiesVersion", this->m_model.propertiesVersion));

    return S_OK;
}

size_t ServerDeletePlayerCustomPropertiesResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFPlayerDataManagementServerDeletePlayerCustomPropertiesResult const*> ServerDeletePlayerCustomPropertiesResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ServerDeletePlayerCustomPropertiesResult>(&this->Model());
}

size_t ServerDeletePlayerCustomPropertiesResult::RequiredBufferSize(const PFPlayerDataManagementServerDeletePlayerCustomPropertiesResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFPlayerDataManagementDeletedPropertyDetails*) + sizeof(PFPlayerDataManagementDeletedPropertyDetails*) * model.deletedPropertiesCount);
    for (size_t i = 0; i < model.deletedPropertiesCount; ++i)
    {
        requiredSize += DeletedPropertyDetails::RequiredBufferSize(*model.deletedProperties[i]);
    }
    if (model.playFabId)
    {
        requiredSize += (std::strlen(model.playFabId) + 1);
    }
    return requiredSize;
}

HRESULT ServerDeletePlayerCustomPropertiesResult::Copy(const PFPlayerDataManagementServerDeletePlayerCustomPropertiesResult& input, PFPlayerDataManagementServerDeletePlayerCustomPropertiesResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<DeletedPropertyDetails>(input.deletedProperties, input.deletedPropertiesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.deletedProperties = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.playFabId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.playFabId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ServerGetPlayerCustomPropertyRequest::ToJson() const
{
    return ServerGetPlayerCustomPropertyRequest::ToJson(this->Model());
}

JsonValue ServerGetPlayerCustomPropertyRequest::ToJson(const PFPlayerDataManagementServerGetPlayerCustomPropertyRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    JsonUtils::ObjectAddMember(output, "PropertyName", input.propertyName);
    return output;
}

HRESULT ServerGetPlayerCustomPropertyResult::FromJson(const JsonValue& input)
{
    String playFabId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PlayFabId", playFabId));
    this->SetPlayFabId(std::move(playFabId));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PropertiesVersion", this->m_model.propertiesVersion));

    std::optional<CustomPropertyDetails> property{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Property", property));
    if (property)
    {
        this->SetProperty(std::move(*property));
    }

    return S_OK;
}

size_t ServerGetPlayerCustomPropertyResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFPlayerDataManagementServerGetPlayerCustomPropertyResult const*> ServerGetPlayerCustomPropertyResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ServerGetPlayerCustomPropertyResult>(&this->Model());
}

size_t ServerGetPlayerCustomPropertyResult::RequiredBufferSize(const PFPlayerDataManagementServerGetPlayerCustomPropertyResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.playFabId)
    {
        requiredSize += (std::strlen(model.playFabId) + 1);
    }
    if (model.property)
    {
        requiredSize += CustomPropertyDetails::RequiredBufferSize(*model.property);
    }
    return requiredSize;
}

HRESULT ServerGetPlayerCustomPropertyResult::Copy(const PFPlayerDataManagementServerGetPlayerCustomPropertyResult& input, PFPlayerDataManagementServerGetPlayerCustomPropertyResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.playFabId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.playFabId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<CustomPropertyDetails>(input.property);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.property = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT ServerGetUserDataResult::FromJson(const JsonValue& input)
{
    ModelDictionaryEntryVector<UserDataRecord> data{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<UserDataRecord>(input, "Data", data));
    this->SetData(std::move(data));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DataVersion", this->m_model.dataVersion));

    String playFabId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PlayFabId", playFabId));
    this->SetPlayFabId(std::move(playFabId));

    return S_OK;
}

size_t ServerGetUserDataResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFPlayerDataManagementServerGetUserDataResult const*> ServerGetUserDataResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ServerGetUserDataResult>(&this->Model());
}

size_t ServerGetUserDataResult::RequiredBufferSize(const PFPlayerDataManagementServerGetUserDataResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFUserDataRecordDictionaryEntry) + sizeof(PFUserDataRecordDictionaryEntry) * model.dataCount);
    for (size_t i = 0; i < model.dataCount; ++i)
    {
        requiredSize += (std::strlen(model.data[i].key) + 1);
        requiredSize += UserDataRecord::RequiredBufferSize(*model.data[i].value);
    }
    if (model.playFabId)
    {
        requiredSize += (std::strlen(model.playFabId) + 1);
    }
    return requiredSize;
}

HRESULT ServerGetUserDataResult::Copy(const PFPlayerDataManagementServerGetUserDataResult& input, PFPlayerDataManagementServerGetUserDataResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToDictionary<UserDataRecord>(input.data, input.dataCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.data = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.playFabId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.playFabId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ListPlayerCustomPropertiesRequest::ToJson() const
{
    return ListPlayerCustomPropertiesRequest::ToJson(this->Model());
}

JsonValue ListPlayerCustomPropertiesRequest::ToJson(const PFPlayerDataManagementListPlayerCustomPropertiesRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    return output;
}

HRESULT ServerListPlayerCustomPropertiesResult::FromJson(const JsonValue& input)
{
    String playFabId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PlayFabId", playFabId));
    this->SetPlayFabId(std::move(playFabId));

    ModelVector<CustomPropertyDetails> properties{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<CustomPropertyDetails>(input, "Properties", properties));
    this->SetProperties(std::move(properties));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PropertiesVersion", this->m_model.propertiesVersion));

    return S_OK;
}

size_t ServerListPlayerCustomPropertiesResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFPlayerDataManagementServerListPlayerCustomPropertiesResult const*> ServerListPlayerCustomPropertiesResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ServerListPlayerCustomPropertiesResult>(&this->Model());
}

size_t ServerListPlayerCustomPropertiesResult::RequiredBufferSize(const PFPlayerDataManagementServerListPlayerCustomPropertiesResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.playFabId)
    {
        requiredSize += (std::strlen(model.playFabId) + 1);
    }
    requiredSize += (alignof(PFPlayerDataManagementCustomPropertyDetails*) + sizeof(PFPlayerDataManagementCustomPropertyDetails*) * model.propertiesCount);
    for (size_t i = 0; i < model.propertiesCount; ++i)
    {
        requiredSize += CustomPropertyDetails::RequiredBufferSize(*model.properties[i]);
    }
    return requiredSize;
}

HRESULT ServerListPlayerCustomPropertiesResult::Copy(const PFPlayerDataManagementServerListPlayerCustomPropertiesResult& input, PFPlayerDataManagementServerListPlayerCustomPropertiesResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.playFabId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.playFabId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<CustomPropertyDetails>(input.properties, input.propertiesCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.properties = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ServerUpdatePlayerCustomPropertiesRequest::ToJson() const
{
    return ServerUpdatePlayerCustomPropertiesRequest::ToJson(this->Model());
}

JsonValue ServerUpdatePlayerCustomPropertiesRequest::ToJson(const PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "ExpectedPropertiesVersion", input.expectedPropertiesVersion);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    JsonUtils::ObjectAddMemberArray<UpdateProperty>(output, "Properties", input.properties, input.propertiesCount);
    return output;
}

HRESULT ServerUpdatePlayerCustomPropertiesResult::FromJson(const JsonValue& input)
{
    String playFabId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PlayFabId", playFabId));
    this->SetPlayFabId(std::move(playFabId));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PropertiesVersion", this->m_model.propertiesVersion));

    return S_OK;
}

size_t ServerUpdatePlayerCustomPropertiesResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResult const*> ServerUpdatePlayerCustomPropertiesResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ServerUpdatePlayerCustomPropertiesResult>(&this->Model());
}

size_t ServerUpdatePlayerCustomPropertiesResult::RequiredBufferSize(const PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.playFabId)
    {
        requiredSize += (std::strlen(model.playFabId) + 1);
    }
    return requiredSize;
}

HRESULT ServerUpdatePlayerCustomPropertiesResult::Copy(const PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResult& input, PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.playFabId);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.playFabId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ServerUpdateUserDataRequest::ToJson() const
{
    return ServerUpdateUserDataRequest::ToJson(this->Model());
}

JsonValue ServerUpdateUserDataRequest::ToJson(const PFPlayerDataManagementServerUpdateUserDataRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMemberDictionary(output, "Data", input.data, input.dataCount);
    JsonUtils::ObjectAddMemberArray(output, "KeysToRemove", input.keysToRemove, input.keysToRemoveCount);
    JsonUtils::ObjectAddMember(output, "Permission", input.permission);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    return output;
}

JsonValue UpdateUserInternalDataRequest::ToJson() const
{
    return UpdateUserInternalDataRequest::ToJson(this->Model());
}

JsonValue UpdateUserInternalDataRequest::ToJson(const PFPlayerDataManagementUpdateUserInternalDataRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMemberDictionary(output, "Data", input.data, input.dataCount);
    JsonUtils::ObjectAddMemberArray(output, "KeysToRemove", input.keysToRemove, input.keysToRemoveCount);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    return output;
}

} // namespace PlayerDataManagement

// Json serialization helpers

} // namespace PlayFab

#include "stdafx.h"
#include "PlayerDataManagementTypes.h"
#include "JsonUtils.h"
#include "Types.h"

namespace PlayFab
{
namespace PlayerDataManagement
{

JsonValue GetUserDataRequest::ToJson() const
{
    return GetUserDataRequest::ToJson(this->Model());
}

JsonValue GetUserDataRequest::ToJson(const PFPlayerDataManagementGetUserDataRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
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

JsonValue ClientUpdateUserDataRequest::ToJson() const
{
    return ClientUpdateUserDataRequest::ToJson(this->Model());
}

JsonValue ClientUpdateUserDataRequest::ToJson(const PFPlayerDataManagementClientUpdateUserDataRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
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

JsonValue ServerUpdateUserDataRequest::ToJson() const
{
    return ServerUpdateUserDataRequest::ToJson(this->Model());
}

JsonValue ServerUpdateUserDataRequest::ToJson(const PFPlayerDataManagementServerUpdateUserDataRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
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
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMemberDictionary(output, "Data", input.data, input.dataCount);
    JsonUtils::ObjectAddMemberArray(output, "KeysToRemove", input.keysToRemove, input.keysToRemoveCount);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    return output;
}

} // namespace PlayerDataManagement
} // namespace PlayFab

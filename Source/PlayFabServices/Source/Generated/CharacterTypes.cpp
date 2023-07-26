#include "stdafx.h"
#include "CharacterTypes.h"
#include "JsonUtils.h"
#include "Types.h"

namespace PlayFab
{
namespace Character
{

JsonValue ListUsersCharactersRequest::ToJson() const
{
    return ListUsersCharactersRequest::ToJson(this->Model());
}

JsonValue ListUsersCharactersRequest::ToJson(const PFCharacterListUsersCharactersRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    return output;
}

HRESULT ListUsersCharactersResult::FromJson(const JsonValue& input)
{
    ModelVector<CharacterResult> characters{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<CharacterResult>(input, "Characters", characters));
    this->SetCharacters(std::move(characters));

    return S_OK;
}

size_t ListUsersCharactersResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCharacterListUsersCharactersResult const*> ListUsersCharactersResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ListUsersCharactersResult>(&this->Model());
}

size_t ListUsersCharactersResult::RequiredBufferSize(const PFCharacterListUsersCharactersResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFCharacterResult*) + sizeof(PFCharacterResult*) * model.charactersCount);
    for (size_t i = 0; i < model.charactersCount; ++i)
    {
        requiredSize += CharacterResult::RequiredBufferSize(*model.characters[i]);
    }
    return requiredSize;
}

HRESULT ListUsersCharactersResult::Copy(const PFCharacterListUsersCharactersResult& input, PFCharacterListUsersCharactersResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<CharacterResult>(input.characters, input.charactersCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.characters = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue GetCharacterDataRequest::ToJson() const
{
    return GetCharacterDataRequest::ToJson(this->Model());
}

JsonValue GetCharacterDataRequest::ToJson(const PFCharacterGetCharacterDataRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CharacterId", input.characterId);
    JsonUtils::ObjectAddMember(output, "IfChangedFromDataVersion", input.ifChangedFromDataVersion);
    JsonUtils::ObjectAddMemberArray(output, "Keys", input.keys, input.keysCount);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    return output;
}

HRESULT ClientGetCharacterDataResult::FromJson(const JsonValue& input)
{
    String characterId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "CharacterId", characterId));
    this->SetCharacterId(std::move(characterId));

    ModelDictionaryEntryVector<UserDataRecord> data{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<UserDataRecord>(input, "Data", data));
    this->SetData(std::move(data));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DataVersion", this->m_model.dataVersion));

    return S_OK;
}

size_t ClientGetCharacterDataResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCharacterClientGetCharacterDataResult const*> ClientGetCharacterDataResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ClientGetCharacterDataResult>(&this->Model());
}

size_t ClientGetCharacterDataResult::RequiredBufferSize(const PFCharacterClientGetCharacterDataResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.characterId)
    {
        requiredSize += (std::strlen(model.characterId) + 1);
    }
    requiredSize += (alignof(PFUserDataRecordDictionaryEntry) + sizeof(PFUserDataRecordDictionaryEntry) * model.dataCount);
    for (size_t i = 0; i < model.dataCount; ++i)
    {
        requiredSize += (std::strlen(model.data[i].key) + 1);
        requiredSize += UserDataRecord::RequiredBufferSize(*model.data[i].value);
    }
    return requiredSize;
}

HRESULT ClientGetCharacterDataResult::Copy(const PFCharacterClientGetCharacterDataResult& input, PFCharacterClientGetCharacterDataResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.characterId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.characterId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToDictionary<UserDataRecord>(input.data, input.dataCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.data = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ClientGrantCharacterToUserRequest::ToJson() const
{
    return ClientGrantCharacterToUserRequest::ToJson(this->Model());
}

JsonValue ClientGrantCharacterToUserRequest::ToJson(const PFCharacterClientGrantCharacterToUserRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CatalogVersion", input.catalogVersion);
    JsonUtils::ObjectAddMember(output, "CharacterName", input.characterName);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "ItemId", input.itemId);
    return output;
}

HRESULT ClientGrantCharacterToUserResult::FromJson(const JsonValue& input)
{
    String characterId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "CharacterId", characterId));
    this->SetCharacterId(std::move(characterId));

    String characterType{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "CharacterType", characterType));
    this->SetCharacterType(std::move(characterType));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Result", this->m_model.result));

    return S_OK;
}

size_t ClientGrantCharacterToUserResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCharacterClientGrantCharacterToUserResult const*> ClientGrantCharacterToUserResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ClientGrantCharacterToUserResult>(&this->Model());
}

size_t ClientGrantCharacterToUserResult::RequiredBufferSize(const PFCharacterClientGrantCharacterToUserResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.characterId)
    {
        requiredSize += (std::strlen(model.characterId) + 1);
    }
    if (model.characterType)
    {
        requiredSize += (std::strlen(model.characterType) + 1);
    }
    return requiredSize;
}

HRESULT ClientGrantCharacterToUserResult::Copy(const PFCharacterClientGrantCharacterToUserResult& input, PFCharacterClientGrantCharacterToUserResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.characterId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.characterId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.characterType); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.characterType = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ClientUpdateCharacterDataRequest::ToJson() const
{
    return ClientUpdateCharacterDataRequest::ToJson(this->Model());
}

JsonValue ClientUpdateCharacterDataRequest::ToJson(const PFCharacterClientUpdateCharacterDataRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CharacterId", input.characterId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMemberDictionary(output, "Data", input.data, input.dataCount);
    JsonUtils::ObjectAddMemberArray(output, "KeysToRemove", input.keysToRemove, input.keysToRemoveCount);
    JsonUtils::ObjectAddMember(output, "Permission", input.permission);
    return output;
}

HRESULT UpdateCharacterDataResult::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DataVersion", this->m_model.dataVersion));

    return S_OK;
}

size_t UpdateCharacterDataResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCharacterUpdateCharacterDataResult const*> UpdateCharacterDataResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<UpdateCharacterDataResult>(&this->Model());
}

size_t UpdateCharacterDataResult::RequiredBufferSize(const PFCharacterUpdateCharacterDataResult& model)
{
    UNREFERENCED_PARAMETER(model); // Fixed size
    return sizeof(ModelType);
}

HRESULT UpdateCharacterDataResult::Copy(const PFCharacterUpdateCharacterDataResult& input, PFCharacterUpdateCharacterDataResult& output, ModelBuffer& buffer)
{
    output = input;
    UNREFERENCED_PARAMETER(buffer); // Fixed size
    return S_OK;
}

JsonValue DeleteCharacterFromUserRequest::ToJson() const
{
    return DeleteCharacterFromUserRequest::ToJson(this->Model());
}

JsonValue DeleteCharacterFromUserRequest::ToJson(const PFCharacterDeleteCharacterFromUserRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CharacterId", input.characterId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    JsonUtils::ObjectAddMember(output, "SaveCharacterInventory", input.saveCharacterInventory);
    return output;
}

HRESULT ServerGetCharacterDataResult::FromJson(const JsonValue& input)
{
    String characterId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "CharacterId", characterId));
    this->SetCharacterId(std::move(characterId));

    ModelDictionaryEntryVector<UserDataRecord> data{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<UserDataRecord>(input, "Data", data));
    this->SetData(std::move(data));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "DataVersion", this->m_model.dataVersion));

    String playFabId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PlayFabId", playFabId));
    this->SetPlayFabId(std::move(playFabId));

    return S_OK;
}

size_t ServerGetCharacterDataResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCharacterServerGetCharacterDataResult const*> ServerGetCharacterDataResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ServerGetCharacterDataResult>(&this->Model());
}

size_t ServerGetCharacterDataResult::RequiredBufferSize(const PFCharacterServerGetCharacterDataResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.characterId)
    {
        requiredSize += (std::strlen(model.characterId) + 1);
    }
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

HRESULT ServerGetCharacterDataResult::Copy(const PFCharacterServerGetCharacterDataResult& input, PFCharacterServerGetCharacterDataResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.characterId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.characterId = propCopyResult.ExtractPayload();
    }
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

JsonValue ServerGrantCharacterToUserRequest::ToJson() const
{
    return ServerGrantCharacterToUserRequest::ToJson(this->Model());
}

JsonValue ServerGrantCharacterToUserRequest::ToJson(const PFCharacterServerGrantCharacterToUserRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CharacterName", input.characterName);
    JsonUtils::ObjectAddMember(output, "CharacterType", input.characterType);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    return output;
}

HRESULT ServerGrantCharacterToUserResult::FromJson(const JsonValue& input)
{
    String characterId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "CharacterId", characterId));
    this->SetCharacterId(std::move(characterId));

    return S_OK;
}

size_t ServerGrantCharacterToUserResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCharacterServerGrantCharacterToUserResult const*> ServerGrantCharacterToUserResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ServerGrantCharacterToUserResult>(&this->Model());
}

size_t ServerGrantCharacterToUserResult::RequiredBufferSize(const PFCharacterServerGrantCharacterToUserResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.characterId)
    {
        requiredSize += (std::strlen(model.characterId) + 1);
    }
    return requiredSize;
}

HRESULT ServerGrantCharacterToUserResult::Copy(const PFCharacterServerGrantCharacterToUserResult& input, PFCharacterServerGrantCharacterToUserResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.characterId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.characterId = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ServerUpdateCharacterDataRequest::ToJson() const
{
    return ServerUpdateCharacterDataRequest::ToJson(this->Model());
}

JsonValue ServerUpdateCharacterDataRequest::ToJson(const PFCharacterServerUpdateCharacterDataRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "CharacterId", input.characterId);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMemberDictionary(output, "Data", input.data, input.dataCount);
    JsonUtils::ObjectAddMemberArray(output, "KeysToRemove", input.keysToRemove, input.keysToRemoveCount);
    JsonUtils::ObjectAddMember(output, "Permission", input.permission);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    return output;
}

} // namespace Character
} // namespace PlayFab

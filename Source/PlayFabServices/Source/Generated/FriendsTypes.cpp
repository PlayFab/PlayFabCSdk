#include "stdafx.h"
#include "FriendsTypes.h"
#include "JsonUtils.h"
#include "Types.h"

namespace PlayFab
{
namespace Friends
{

JsonValue ClientAddFriendRequest::ToJson() const
{
    return ClientAddFriendRequest::ToJson(this->Model());
}

JsonValue ClientAddFriendRequest::ToJson(const PFFriendsClientAddFriendRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "FriendEmail", input.friendEmail);
    JsonUtils::ObjectAddMember(output, "FriendPlayFabId", input.friendPlayFabId);
    JsonUtils::ObjectAddMember(output, "FriendTitleDisplayName", input.friendTitleDisplayName);
    JsonUtils::ObjectAddMember(output, "FriendUsername", input.friendUsername);
    return output;
}

HRESULT AddFriendResult::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Created", this->m_model.created));

    return S_OK;
}

size_t AddFriendResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFFriendsAddFriendResult const*> AddFriendResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<AddFriendResult>(&this->Model());
}

size_t AddFriendResult::RequiredBufferSize(const PFFriendsAddFriendResult& model)
{
    UNREFERENCED_PARAMETER(model); // Fixed size
    return sizeof(ModelType);
}

HRESULT AddFriendResult::Copy(const PFFriendsAddFriendResult& input, PFFriendsAddFriendResult& output, ModelBuffer& buffer)
{
    output = input;
    UNREFERENCED_PARAMETER(buffer); // Fixed size
    return S_OK;
}

JsonValue ClientGetFriendsListRequest::ToJson() const
{
    return ClientGetFriendsListRequest::ToJson(this->Model());
}

JsonValue ClientGetFriendsListRequest::ToJson(const PFFriendsClientGetFriendsListRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "ExternalPlatformFriends", input.externalPlatformFriends);
    JsonUtils::ObjectAddMember<PlayerProfileViewConstraints>(output, "ProfileConstraints", input.profileConstraints);
#if HC_PLATFORM != HC_PLATFORM_GDK
    JsonUtils::ObjectAddMember(output, "XboxToken", input.xboxToken);
#endif
    return output;
}

HRESULT FriendInfo::FromJson(const JsonValue& input)
{
    std::optional<UserFacebookInfo> facebookInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FacebookInfo", facebookInfo));
    if (facebookInfo)
    {
        this->SetFacebookInfo(std::move(*facebookInfo));
    }

    String friendPlayFabId{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FriendPlayFabId", friendPlayFabId));
    this->SetFriendPlayFabId(std::move(friendPlayFabId));

    std::optional<UserGameCenterInfo> gameCenterInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "GameCenterInfo", gameCenterInfo));
    if (gameCenterInfo)
    {
        this->SetGameCenterInfo(std::move(*gameCenterInfo));
    }

    std::optional<PlayerProfileModel> profile{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Profile", profile));
    if (profile)
    {
        this->SetProfile(std::move(*profile));
    }

    std::optional<UserPsnInfo> PSNInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "PSNInfo", PSNInfo));
    if (PSNInfo)
    {
        this->SetPSNInfo(std::move(*PSNInfo));
    }

    std::optional<UserSteamInfo> steamInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "SteamInfo", steamInfo));
    if (steamInfo)
    {
        this->SetSteamInfo(std::move(*steamInfo));
    }

    CStringVector tags{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Tags", tags));
    this->SetTags(std::move(tags));

    String titleDisplayName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "TitleDisplayName", titleDisplayName));
    this->SetTitleDisplayName(std::move(titleDisplayName));

    String username{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Username", username));
    this->SetUsername(std::move(username));

    std::optional<UserXboxInfo> xboxInfo{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "XboxInfo", xboxInfo));
    if (xboxInfo)
    {
        this->SetXboxInfo(std::move(*xboxInfo));
    }

    return S_OK;
}

size_t FriendInfo::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFFriendsFriendInfo const*> FriendInfo::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<FriendInfo>(&this->Model());
}

size_t FriendInfo::RequiredBufferSize(const PFFriendsFriendInfo& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.facebookInfo)
    {
        requiredSize += UserFacebookInfo::RequiredBufferSize(*model.facebookInfo);
    }
    if (model.friendPlayFabId)
    {
        requiredSize += (std::strlen(model.friendPlayFabId) + 1);
    }
    if (model.gameCenterInfo)
    {
        requiredSize += UserGameCenterInfo::RequiredBufferSize(*model.gameCenterInfo);
    }
    if (model.profile)
    {
        requiredSize += PlayerProfileModel::RequiredBufferSize(*model.profile);
    }
    if (model.PSNInfo)
    {
        requiredSize += UserPsnInfo::RequiredBufferSize(*model.PSNInfo);
    }
    if (model.steamInfo)
    {
        requiredSize += UserSteamInfo::RequiredBufferSize(*model.steamInfo);
    }
    requiredSize += (alignof(char*) + sizeof(char*) * model.tagsCount);
    for (size_t i = 0; i < model.tagsCount; ++i)
    {
        requiredSize += (std::strlen(model.tags[i]) + 1);
    }
    if (model.titleDisplayName)
    {
        requiredSize += (std::strlen(model.titleDisplayName) + 1);
    }
    if (model.username)
    {
        requiredSize += (std::strlen(model.username) + 1);
    }
    if (model.xboxInfo)
    {
        requiredSize += UserXboxInfo::RequiredBufferSize(*model.xboxInfo);
    }
    return requiredSize;
}

HRESULT FriendInfo::Copy(const PFFriendsFriendInfo& input, PFFriendsFriendInfo& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<UserFacebookInfo>(input.facebookInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.facebookInfo = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.friendPlayFabId); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.friendPlayFabId = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserGameCenterInfo>(input.gameCenterInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.gameCenterInfo = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<PlayerProfileModel>(input.profile); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.profile = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserPsnInfo>(input.PSNInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.PSNInfo = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserSteamInfo>(input.steamInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.steamInfo = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray(input.tags, input.tagsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.tags = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.titleDisplayName); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.titleDisplayName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.username); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.username = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo<UserXboxInfo>(input.xboxInfo); 
        RETURN_IF_FAILED(propCopyResult.hr);
        output.xboxInfo = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT GetFriendsListResult::FromJson(const JsonValue& input)
{
    ModelVector<FriendInfo> friends{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<FriendInfo>(input, "Friends", friends));
    this->SetFriends(std::move(friends));

    return S_OK;
}

size_t GetFriendsListResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFFriendsGetFriendsListResult const*> GetFriendsListResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<GetFriendsListResult>(&this->Model());
}

size_t GetFriendsListResult::RequiredBufferSize(const PFFriendsGetFriendsListResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFFriendsFriendInfo*) + sizeof(PFFriendsFriendInfo*) * model.friendsCount);
    for (size_t i = 0; i < model.friendsCount; ++i)
    {
        requiredSize += FriendInfo::RequiredBufferSize(*model.friends[i]);
    }
    return requiredSize;
}

HRESULT GetFriendsListResult::Copy(const PFFriendsGetFriendsListResult& input, PFFriendsGetFriendsListResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<FriendInfo>(input.friends, input.friendsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.friends = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ClientRemoveFriendRequest::ToJson() const
{
    return ClientRemoveFriendRequest::ToJson(this->Model());
}

JsonValue ClientRemoveFriendRequest::ToJson(const PFFriendsClientRemoveFriendRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "FriendPlayFabId", input.friendPlayFabId);
    return output;
}

JsonValue ClientSetFriendTagsRequest::ToJson() const
{
    return ClientSetFriendTagsRequest::ToJson(this->Model());
}

JsonValue ClientSetFriendTagsRequest::ToJson(const PFFriendsClientSetFriendTagsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "FriendPlayFabId", input.friendPlayFabId);
    JsonUtils::ObjectAddMemberArray(output, "Tags", input.tags, input.tagsCount);
    return output;
}

JsonValue ServerAddFriendRequest::ToJson() const
{
    return ServerAddFriendRequest::ToJson(this->Model());
}

JsonValue ServerAddFriendRequest::ToJson(const PFFriendsServerAddFriendRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "FriendEmail", input.friendEmail);
    JsonUtils::ObjectAddMember(output, "FriendPlayFabId", input.friendPlayFabId);
    JsonUtils::ObjectAddMember(output, "FriendTitleDisplayName", input.friendTitleDisplayName);
    JsonUtils::ObjectAddMember(output, "FriendUsername", input.friendUsername);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    return output;
}

JsonValue ServerGetFriendsListRequest::ToJson() const
{
    return ServerGetFriendsListRequest::ToJson(this->Model());
}

JsonValue ServerGetFriendsListRequest::ToJson(const PFFriendsServerGetFriendsListRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "ExternalPlatformFriends", input.externalPlatformFriends);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    JsonUtils::ObjectAddMember<PlayerProfileViewConstraints>(output, "ProfileConstraints", input.profileConstraints);
    JsonUtils::ObjectAddMember(output, "XboxToken", input.xboxToken);
    return output;
}

JsonValue ServerRemoveFriendRequest::ToJson() const
{
    return ServerRemoveFriendRequest::ToJson(this->Model());
}

JsonValue ServerRemoveFriendRequest::ToJson(const PFFriendsServerRemoveFriendRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "FriendPlayFabId", input.friendPlayFabId);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    return output;
}

JsonValue ServerSetFriendTagsRequest::ToJson() const
{
    return ServerSetFriendTagsRequest::ToJson(this->Model());
}

JsonValue ServerSetFriendTagsRequest::ToJson(const PFFriendsServerSetFriendTagsRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(output, "FriendPlayFabId", input.friendPlayFabId);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    JsonUtils::ObjectAddMemberArray(output, "Tags", input.tags, input.tagsCount);
    return output;
}

} // namespace Friends
} // namespace PlayFab

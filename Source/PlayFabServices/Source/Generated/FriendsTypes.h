#pragma once

#include <playfab/services/cpp/FriendsTypeWrappers.h>
#include "Generated/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace Friends
{

// Friends Classes
class ClientAddFriendRequest : public Wrappers::PFFriendsClientAddFriendRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFFriendsClientAddFriendRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFFriendsClientAddFriendRequest& input);
};

class AddFriendResult : public Wrappers::PFFriendsAddFriendResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFFriendsAddFriendResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFFriendsAddFriendResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFFriendsAddFriendResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFFriendsAddFriendResult& model);
    static HRESULT Copy(const PFFriendsAddFriendResult& input, PFFriendsAddFriendResult& output, ModelBuffer& buffer);
};

class ClientGetFriendsListRequest : public Wrappers::PFFriendsClientGetFriendsListRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFFriendsClientGetFriendsListRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFFriendsClientGetFriendsListRequest& input);
};

class FriendInfo : public Wrappers::PFFriendsFriendInfoWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFFriendsFriendInfo>
{
public:
    using ModelWrapperType = typename Wrappers::PFFriendsFriendInfoWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFFriendsFriendInfo const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFFriendsFriendInfo& model);
    static HRESULT Copy(const PFFriendsFriendInfo& input, PFFriendsFriendInfo& output, ModelBuffer& buffer);
};

class GetFriendsListResult : public Wrappers::PFFriendsGetFriendsListResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFFriendsGetFriendsListResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFFriendsGetFriendsListResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFFriendsGetFriendsListResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFFriendsGetFriendsListResult& model);
    static HRESULT Copy(const PFFriendsGetFriendsListResult& input, PFFriendsGetFriendsListResult& output, ModelBuffer& buffer);
};

class ClientRemoveFriendRequest : public Wrappers::PFFriendsClientRemoveFriendRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFFriendsClientRemoveFriendRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFFriendsClientRemoveFriendRequest& input);
};

class ClientSetFriendTagsRequest : public Wrappers::PFFriendsClientSetFriendTagsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFFriendsClientSetFriendTagsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFFriendsClientSetFriendTagsRequest& input);
};

class ServerAddFriendRequest : public Wrappers::PFFriendsServerAddFriendRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFFriendsServerAddFriendRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFFriendsServerAddFriendRequest& input);
};

class ServerGetFriendsListRequest : public Wrappers::PFFriendsServerGetFriendsListRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFFriendsServerGetFriendsListRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFFriendsServerGetFriendsListRequest& input);
};

class ServerRemoveFriendRequest : public Wrappers::PFFriendsServerRemoveFriendRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFFriendsServerRemoveFriendRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFFriendsServerRemoveFriendRequest& input);
};

class ServerSetFriendTagsRequest : public Wrappers::PFFriendsServerSetFriendTagsRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFFriendsServerSetFriendTagsRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFFriendsServerSetFriendTagsRequest& input);
};

} // namespace Friends

// EnumRange definitions used for Enum (de)serialization
template<typename T> struct EnumRange;

template<> struct EnumRange<PFFriendsExternalFriendSources>
{
    static constexpr PFFriendsExternalFriendSources maxValue = PFFriendsExternalFriendSources::All;
};

} // namespace PlayFab

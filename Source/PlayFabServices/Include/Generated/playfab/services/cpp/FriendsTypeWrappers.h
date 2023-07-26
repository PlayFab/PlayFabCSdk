// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFFriendsTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>
#if HC_PLATFORM == HC_PLATFORM_GDK
#include <playfab/core/cpp/AuthenticationTypeWrappers.h>
#endif

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFFriendsClientAddFriendRequestWrapper : public ModelWrapper<PFFriendsClientAddFriendRequest, Alloc>
{
public:
    using ModelType = PFFriendsClientAddFriendRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFFriendsClientAddFriendRequestWrapper() = default;

    PFFriendsClientAddFriendRequestWrapper(const PFFriendsClientAddFriendRequest& model) :
        ModelWrapper<PFFriendsClientAddFriendRequest, Alloc>{ model },
        m_friendEmail{ SafeString(model.friendEmail) },
        m_friendPlayFabId{ SafeString(model.friendPlayFabId) },
        m_friendTitleDisplayName{ SafeString(model.friendTitleDisplayName) },
        m_friendUsername{ SafeString(model.friendUsername) }
    {
        SetModelPointers();
    }

    PFFriendsClientAddFriendRequestWrapper(const PFFriendsClientAddFriendRequestWrapper& src) :
        PFFriendsClientAddFriendRequestWrapper{ src.Model() }
    {
    }

    PFFriendsClientAddFriendRequestWrapper(PFFriendsClientAddFriendRequestWrapper&& src) :
        PFFriendsClientAddFriendRequestWrapper{}
    {
        swap(*this, src);
    }

    PFFriendsClientAddFriendRequestWrapper& operator=(PFFriendsClientAddFriendRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFFriendsClientAddFriendRequestWrapper() = default;

    friend void swap(PFFriendsClientAddFriendRequestWrapper& lhs, PFFriendsClientAddFriendRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_friendEmail, rhs.m_friendEmail);
        swap(lhs.m_friendPlayFabId, rhs.m_friendPlayFabId);
        swap(lhs.m_friendTitleDisplayName, rhs.m_friendTitleDisplayName);
        swap(lhs.m_friendUsername, rhs.m_friendUsername);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFriendEmail(String value)
    {
        m_friendEmail = std::move(value);
        this->m_model.friendEmail =  m_friendEmail.empty() ? nullptr : m_friendEmail.data();
    }

    void SetFriendPlayFabId(String value)
    {
        m_friendPlayFabId = std::move(value);
        this->m_model.friendPlayFabId =  m_friendPlayFabId.empty() ? nullptr : m_friendPlayFabId.data();
    }

    void SetFriendTitleDisplayName(String value)
    {
        m_friendTitleDisplayName = std::move(value);
        this->m_model.friendTitleDisplayName =  m_friendTitleDisplayName.empty() ? nullptr : m_friendTitleDisplayName.data();
    }

    void SetFriendUsername(String value)
    {
        m_friendUsername = std::move(value);
        this->m_model.friendUsername =  m_friendUsername.empty() ? nullptr : m_friendUsername.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.friendEmail = m_friendEmail.empty() ? nullptr : m_friendEmail.data();
        this->m_model.friendPlayFabId = m_friendPlayFabId.empty() ? nullptr : m_friendPlayFabId.data();
        this->m_model.friendTitleDisplayName = m_friendTitleDisplayName.empty() ? nullptr : m_friendTitleDisplayName.data();
        this->m_model.friendUsername = m_friendUsername.empty() ? nullptr : m_friendUsername.data();
    }

    String m_friendEmail;
    String m_friendPlayFabId;
    String m_friendTitleDisplayName;
    String m_friendUsername;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFFriendsAddFriendResultWrapper : public ModelWrapper<PFFriendsAddFriendResult, Alloc>
{
public:
    using ModelType = PFFriendsAddFriendResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    using ModelWrapper<PFFriendsAddFriendResult, Alloc>::ModelWrapper;

    void SetCreated(bool value)
    {
        this->m_model.created = value;
    }

private:
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFFriendsClientGetFriendsListRequestWrapper : public ModelWrapper<PFFriendsClientGetFriendsListRequest, Alloc>
{
public:
    using ModelType = PFFriendsClientGetFriendsListRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

#if HC_PLATFORM == HC_PLATFORM_GDK
    PFFriendsClientGetFriendsListRequestWrapper(XUser user)
        : m_user{ std::move(user) }
    {
        SetModelPointers();
    }
#else
    PFFriendsClientGetFriendsListRequestWrapper() = default;
#endif

    PFFriendsClientGetFriendsListRequestWrapper(const PFFriendsClientGetFriendsListRequest& model) :
        ModelWrapper<PFFriendsClientGetFriendsListRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_externalPlatformFriends{ model.externalPlatformFriends ? std::optional<PFFriendsExternalFriendSources>{ *model.externalPlatformFriends } : std::nullopt },
        m_profileConstraints{ model.profileConstraints ? std::optional<PFPlayerProfileViewConstraintsWrapper<Alloc>>{ *model.profileConstraints } : std::nullopt },
#if HC_PLATFORM == HC_PLATFORM_GDK
        m_user{ XUser::Duplicate(model.user) }
#else
        m_xboxToken{ SafeString(model.xboxToken) }
#endif
    {
        SetModelPointers();
    }

    PFFriendsClientGetFriendsListRequestWrapper(const PFFriendsClientGetFriendsListRequestWrapper& src) :
        PFFriendsClientGetFriendsListRequestWrapper{ src.Model() }
    {
    }

    PFFriendsClientGetFriendsListRequestWrapper(PFFriendsClientGetFriendsListRequestWrapper&& src) :
#if HC_PLATFORM == HC_PLATFORM_GDK
        PFFriendsClientGetFriendsListRequestWrapper{ XUser::Wrap(nullptr) }
#else
        PFFriendsClientGetFriendsListRequestWrapper{}
#endif
    {
        swap(*this, src);
    }

    PFFriendsClientGetFriendsListRequestWrapper& operator=(PFFriendsClientGetFriendsListRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFFriendsClientGetFriendsListRequestWrapper() = default;

    friend void swap(PFFriendsClientGetFriendsListRequestWrapper& lhs, PFFriendsClientGetFriendsListRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_externalPlatformFriends, rhs.m_externalPlatformFriends);
        swap(lhs.m_profileConstraints, rhs.m_profileConstraints);
#if HC_PLATFORM == HC_PLATFORM_GDK
        swap(lhs.m_user, rhs.m_user);
#endif
        swap(lhs.m_xboxToken, rhs.m_xboxToken);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetExternalPlatformFriends(std::optional<PFFriendsExternalFriendSources> value)
    {
        m_externalPlatformFriends = std::move(value);
        this->m_model.externalPlatformFriends = m_externalPlatformFriends ? m_externalPlatformFriends.operator->() : nullptr;
    }

    void SetProfileConstraints(std::optional<PFPlayerProfileViewConstraintsWrapper<Alloc>> value)
    {
        m_profileConstraints = std::move(value);
        this->m_model.profileConstraints = m_profileConstraints ? &m_profileConstraints->Model() : nullptr;
    }

#if HC_PLATFORM == HC_PLATFORM_GDK
    void SetUser(XUser value)
    {
        m_user = std::move(value);
        this->m_model.user = m_user.Handle();
    }
#else
    void SetXboxToken(String value)
    {
        m_xboxToken = std::move(value);
        this->m_model.xboxToken =  m_xboxToken.empty() ? nullptr : m_xboxToken.data();
    }
#endif

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.externalPlatformFriends = m_externalPlatformFriends ? m_externalPlatformFriends.operator->() : nullptr;
        this->m_model.profileConstraints = m_profileConstraints ?  &m_profileConstraints->Model() : nullptr;
#if HC_PLATFORM == HC_PLATFORM_GDK
        this->m_model.user = m_user.Handle();
#else
        this->m_model.xboxToken = m_xboxToken.empty() ? nullptr : m_xboxToken.data();
#endif
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFFriendsExternalFriendSources> m_externalPlatformFriends;
    std::optional<PFPlayerProfileViewConstraintsWrapper<Alloc>> m_profileConstraints;
#if HC_PLATFORM == HC_PLATFORM_GDK
    XUser m_user;
#endif
    String m_xboxToken;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFFriendsFriendInfoWrapper : public ModelWrapper<PFFriendsFriendInfo, Alloc>
{
public:
    using ModelType = PFFriendsFriendInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFFriendsFriendInfoWrapper() = default;

    PFFriendsFriendInfoWrapper(const PFFriendsFriendInfo& model) :
        ModelWrapper<PFFriendsFriendInfo, Alloc>{ model },
        m_facebookInfo{ model.facebookInfo ? std::optional<PFUserFacebookInfoWrapper<Alloc>>{ *model.facebookInfo } : std::nullopt },
        m_friendPlayFabId{ SafeString(model.friendPlayFabId) },
        m_gameCenterInfo{ model.gameCenterInfo ? std::optional<PFUserGameCenterInfoWrapper<Alloc>>{ *model.gameCenterInfo } : std::nullopt },
        m_profile{ model.profile ? std::optional<PFPlayerProfileModelWrapper<Alloc>>{ *model.profile } : std::nullopt },
        m_PSNInfo{ model.PSNInfo ? std::optional<PFUserPsnInfoWrapper<Alloc>>{ *model.PSNInfo } : std::nullopt },
        m_steamInfo{ model.steamInfo ? std::optional<PFUserSteamInfoWrapper<Alloc>>{ *model.steamInfo } : std::nullopt },
        m_tags{ model.tags, model.tags + model.tagsCount },
        m_titleDisplayName{ SafeString(model.titleDisplayName) },
        m_username{ SafeString(model.username) },
        m_xboxInfo{ model.xboxInfo ? std::optional<PFUserXboxInfoWrapper<Alloc>>{ *model.xboxInfo } : std::nullopt }
    {
        SetModelPointers();
    }

    PFFriendsFriendInfoWrapper(const PFFriendsFriendInfoWrapper& src) :
        PFFriendsFriendInfoWrapper{ src.Model() }
    {
    }

    PFFriendsFriendInfoWrapper(PFFriendsFriendInfoWrapper&& src) :
        PFFriendsFriendInfoWrapper{}
    {
        swap(*this, src);
    }

    PFFriendsFriendInfoWrapper& operator=(PFFriendsFriendInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFFriendsFriendInfoWrapper() = default;

    friend void swap(PFFriendsFriendInfoWrapper& lhs, PFFriendsFriendInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_facebookInfo, rhs.m_facebookInfo);
        swap(lhs.m_friendPlayFabId, rhs.m_friendPlayFabId);
        swap(lhs.m_gameCenterInfo, rhs.m_gameCenterInfo);
        swap(lhs.m_profile, rhs.m_profile);
        swap(lhs.m_PSNInfo, rhs.m_PSNInfo);
        swap(lhs.m_steamInfo, rhs.m_steamInfo);
        swap(lhs.m_tags, rhs.m_tags);
        swap(lhs.m_titleDisplayName, rhs.m_titleDisplayName);
        swap(lhs.m_username, rhs.m_username);
        swap(lhs.m_xboxInfo, rhs.m_xboxInfo);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFacebookInfo(std::optional<PFUserFacebookInfoWrapper<Alloc>> value)
    {
        m_facebookInfo = std::move(value);
        this->m_model.facebookInfo = m_facebookInfo ? &m_facebookInfo->Model() : nullptr;
    }

    void SetFriendPlayFabId(String value)
    {
        m_friendPlayFabId = std::move(value);
        this->m_model.friendPlayFabId =  m_friendPlayFabId.empty() ? nullptr : m_friendPlayFabId.data();
    }

    void SetGameCenterInfo(std::optional<PFUserGameCenterInfoWrapper<Alloc>> value)
    {
        m_gameCenterInfo = std::move(value);
        this->m_model.gameCenterInfo = m_gameCenterInfo ? &m_gameCenterInfo->Model() : nullptr;
    }

    void SetProfile(std::optional<PFPlayerProfileModelWrapper<Alloc>> value)
    {
        m_profile = std::move(value);
        this->m_model.profile = m_profile ? &m_profile->Model() : nullptr;
    }

    void SetPSNInfo(std::optional<PFUserPsnInfoWrapper<Alloc>> value)
    {
        m_PSNInfo = std::move(value);
        this->m_model.PSNInfo = m_PSNInfo ? &m_PSNInfo->Model() : nullptr;
    }

    void SetSteamInfo(std::optional<PFUserSteamInfoWrapper<Alloc>> value)
    {
        m_steamInfo = std::move(value);
        this->m_model.steamInfo = m_steamInfo ? &m_steamInfo->Model() : nullptr;
    }

    void SetTags(CStringVector<Alloc> value)
    {
        m_tags = std::move(value);
        this->m_model.tags =  m_tags.empty() ? nullptr : m_tags.data();
        this->m_model.tagsCount =  static_cast<uint32_t>(m_tags.size());
    }

    void SetTitleDisplayName(String value)
    {
        m_titleDisplayName = std::move(value);
        this->m_model.titleDisplayName =  m_titleDisplayName.empty() ? nullptr : m_titleDisplayName.data();
    }

    void SetUsername(String value)
    {
        m_username = std::move(value);
        this->m_model.username =  m_username.empty() ? nullptr : m_username.data();
    }

    void SetXboxInfo(std::optional<PFUserXboxInfoWrapper<Alloc>> value)
    {
        m_xboxInfo = std::move(value);
        this->m_model.xboxInfo = m_xboxInfo ? &m_xboxInfo->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.facebookInfo = m_facebookInfo ?  &m_facebookInfo->Model() : nullptr;
        this->m_model.friendPlayFabId = m_friendPlayFabId.empty() ? nullptr : m_friendPlayFabId.data();
        this->m_model.gameCenterInfo = m_gameCenterInfo ?  &m_gameCenterInfo->Model() : nullptr;
        this->m_model.profile = m_profile ?  &m_profile->Model() : nullptr;
        this->m_model.PSNInfo = m_PSNInfo ?  &m_PSNInfo->Model() : nullptr;
        this->m_model.steamInfo = m_steamInfo ?  &m_steamInfo->Model() : nullptr;
        this->m_model.tags = m_tags.empty() ? nullptr : m_tags.data();
        this->m_model.titleDisplayName = m_titleDisplayName.empty() ? nullptr : m_titleDisplayName.data();
        this->m_model.username = m_username.empty() ? nullptr : m_username.data();
        this->m_model.xboxInfo = m_xboxInfo ?  &m_xboxInfo->Model() : nullptr;
    }

    std::optional<PFUserFacebookInfoWrapper<Alloc>> m_facebookInfo;
    String m_friendPlayFabId;
    std::optional<PFUserGameCenterInfoWrapper<Alloc>> m_gameCenterInfo;
    std::optional<PFPlayerProfileModelWrapper<Alloc>> m_profile;
    std::optional<PFUserPsnInfoWrapper<Alloc>> m_PSNInfo;
    std::optional<PFUserSteamInfoWrapper<Alloc>> m_steamInfo;
    CStringVector<Alloc> m_tags;
    String m_titleDisplayName;
    String m_username;
    std::optional<PFUserXboxInfoWrapper<Alloc>> m_xboxInfo;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFFriendsGetFriendsListResultWrapper : public ModelWrapper<PFFriendsGetFriendsListResult, Alloc>
{
public:
    using ModelType = PFFriendsGetFriendsListResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFFriendsGetFriendsListResultWrapper() = default;

    PFFriendsGetFriendsListResultWrapper(const PFFriendsGetFriendsListResult& model) :
        ModelWrapper<PFFriendsGetFriendsListResult, Alloc>{ model },
        m_friends{ model.friends, model.friends + model.friendsCount }
    {
        SetModelPointers();
    }

    PFFriendsGetFriendsListResultWrapper(const PFFriendsGetFriendsListResultWrapper& src) :
        PFFriendsGetFriendsListResultWrapper{ src.Model() }
    {
    }

    PFFriendsGetFriendsListResultWrapper(PFFriendsGetFriendsListResultWrapper&& src) :
        PFFriendsGetFriendsListResultWrapper{}
    {
        swap(*this, src);
    }

    PFFriendsGetFriendsListResultWrapper& operator=(PFFriendsGetFriendsListResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFFriendsGetFriendsListResultWrapper() = default;

    friend void swap(PFFriendsGetFriendsListResultWrapper& lhs, PFFriendsGetFriendsListResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_friends, rhs.m_friends);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFriends(ModelVector<PFFriendsFriendInfoWrapper<Alloc>, Alloc> value)
    {
        m_friends = std::move(value);
        this->m_model.friends =  m_friends.empty() ? nullptr : m_friends.data();
        this->m_model.friendsCount =  static_cast<uint32_t>(m_friends.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.friends = m_friends.empty() ? nullptr : m_friends.data();
    }

    ModelVector<PFFriendsFriendInfoWrapper<Alloc>, Alloc> m_friends;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFFriendsClientRemoveFriendRequestWrapper : public ModelWrapper<PFFriendsClientRemoveFriendRequest, Alloc>
{
public:
    using ModelType = PFFriendsClientRemoveFriendRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFFriendsClientRemoveFriendRequestWrapper() = default;

    PFFriendsClientRemoveFriendRequestWrapper(const PFFriendsClientRemoveFriendRequest& model) :
        ModelWrapper<PFFriendsClientRemoveFriendRequest, Alloc>{ model },
        m_friendPlayFabId{ SafeString(model.friendPlayFabId) }
    {
        SetModelPointers();
    }

    PFFriendsClientRemoveFriendRequestWrapper(const PFFriendsClientRemoveFriendRequestWrapper& src) :
        PFFriendsClientRemoveFriendRequestWrapper{ src.Model() }
    {
    }

    PFFriendsClientRemoveFriendRequestWrapper(PFFriendsClientRemoveFriendRequestWrapper&& src) :
        PFFriendsClientRemoveFriendRequestWrapper{}
    {
        swap(*this, src);
    }

    PFFriendsClientRemoveFriendRequestWrapper& operator=(PFFriendsClientRemoveFriendRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFFriendsClientRemoveFriendRequestWrapper() = default;

    friend void swap(PFFriendsClientRemoveFriendRequestWrapper& lhs, PFFriendsClientRemoveFriendRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_friendPlayFabId, rhs.m_friendPlayFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFriendPlayFabId(String value)
    {
        m_friendPlayFabId = std::move(value);
        this->m_model.friendPlayFabId =  m_friendPlayFabId.empty() ? nullptr : m_friendPlayFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.friendPlayFabId = m_friendPlayFabId.empty() ? nullptr : m_friendPlayFabId.data();
    }

    String m_friendPlayFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFFriendsClientSetFriendTagsRequestWrapper : public ModelWrapper<PFFriendsClientSetFriendTagsRequest, Alloc>
{
public:
    using ModelType = PFFriendsClientSetFriendTagsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFFriendsClientSetFriendTagsRequestWrapper() = default;

    PFFriendsClientSetFriendTagsRequestWrapper(const PFFriendsClientSetFriendTagsRequest& model) :
        ModelWrapper<PFFriendsClientSetFriendTagsRequest, Alloc>{ model },
        m_friendPlayFabId{ SafeString(model.friendPlayFabId) },
        m_tags{ model.tags, model.tags + model.tagsCount }
    {
        SetModelPointers();
    }

    PFFriendsClientSetFriendTagsRequestWrapper(const PFFriendsClientSetFriendTagsRequestWrapper& src) :
        PFFriendsClientSetFriendTagsRequestWrapper{ src.Model() }
    {
    }

    PFFriendsClientSetFriendTagsRequestWrapper(PFFriendsClientSetFriendTagsRequestWrapper&& src) :
        PFFriendsClientSetFriendTagsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFFriendsClientSetFriendTagsRequestWrapper& operator=(PFFriendsClientSetFriendTagsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFFriendsClientSetFriendTagsRequestWrapper() = default;

    friend void swap(PFFriendsClientSetFriendTagsRequestWrapper& lhs, PFFriendsClientSetFriendTagsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_friendPlayFabId, rhs.m_friendPlayFabId);
        swap(lhs.m_tags, rhs.m_tags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFriendPlayFabId(String value)
    {
        m_friendPlayFabId = std::move(value);
        this->m_model.friendPlayFabId =  m_friendPlayFabId.empty() ? nullptr : m_friendPlayFabId.data();
    }

    void SetTags(CStringVector<Alloc> value)
    {
        m_tags = std::move(value);
        this->m_model.tags =  m_tags.empty() ? nullptr : m_tags.data();
        this->m_model.tagsCount =  static_cast<uint32_t>(m_tags.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.friendPlayFabId = m_friendPlayFabId.empty() ? nullptr : m_friendPlayFabId.data();
        this->m_model.tags = m_tags.empty() ? nullptr : m_tags.data();
    }

    String m_friendPlayFabId;
    CStringVector<Alloc> m_tags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFFriendsServerAddFriendRequestWrapper : public ModelWrapper<PFFriendsServerAddFriendRequest, Alloc>
{
public:
    using ModelType = PFFriendsServerAddFriendRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFFriendsServerAddFriendRequestWrapper() = default;

    PFFriendsServerAddFriendRequestWrapper(const PFFriendsServerAddFriendRequest& model) :
        ModelWrapper<PFFriendsServerAddFriendRequest, Alloc>{ model },
        m_friendEmail{ SafeString(model.friendEmail) },
        m_friendPlayFabId{ SafeString(model.friendPlayFabId) },
        m_friendTitleDisplayName{ SafeString(model.friendTitleDisplayName) },
        m_friendUsername{ SafeString(model.friendUsername) },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFFriendsServerAddFriendRequestWrapper(const PFFriendsServerAddFriendRequestWrapper& src) :
        PFFriendsServerAddFriendRequestWrapper{ src.Model() }
    {
    }

    PFFriendsServerAddFriendRequestWrapper(PFFriendsServerAddFriendRequestWrapper&& src) :
        PFFriendsServerAddFriendRequestWrapper{}
    {
        swap(*this, src);
    }

    PFFriendsServerAddFriendRequestWrapper& operator=(PFFriendsServerAddFriendRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFFriendsServerAddFriendRequestWrapper() = default;

    friend void swap(PFFriendsServerAddFriendRequestWrapper& lhs, PFFriendsServerAddFriendRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_friendEmail, rhs.m_friendEmail);
        swap(lhs.m_friendPlayFabId, rhs.m_friendPlayFabId);
        swap(lhs.m_friendTitleDisplayName, rhs.m_friendTitleDisplayName);
        swap(lhs.m_friendUsername, rhs.m_friendUsername);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFriendEmail(String value)
    {
        m_friendEmail = std::move(value);
        this->m_model.friendEmail =  m_friendEmail.empty() ? nullptr : m_friendEmail.data();
    }

    void SetFriendPlayFabId(String value)
    {
        m_friendPlayFabId = std::move(value);
        this->m_model.friendPlayFabId =  m_friendPlayFabId.empty() ? nullptr : m_friendPlayFabId.data();
    }

    void SetFriendTitleDisplayName(String value)
    {
        m_friendTitleDisplayName = std::move(value);
        this->m_model.friendTitleDisplayName =  m_friendTitleDisplayName.empty() ? nullptr : m_friendTitleDisplayName.data();
    }

    void SetFriendUsername(String value)
    {
        m_friendUsername = std::move(value);
        this->m_model.friendUsername =  m_friendUsername.empty() ? nullptr : m_friendUsername.data();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.friendEmail = m_friendEmail.empty() ? nullptr : m_friendEmail.data();
        this->m_model.friendPlayFabId = m_friendPlayFabId.empty() ? nullptr : m_friendPlayFabId.data();
        this->m_model.friendTitleDisplayName = m_friendTitleDisplayName.empty() ? nullptr : m_friendTitleDisplayName.data();
        this->m_model.friendUsername = m_friendUsername.empty() ? nullptr : m_friendUsername.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String m_friendEmail;
    String m_friendPlayFabId;
    String m_friendTitleDisplayName;
    String m_friendUsername;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFFriendsServerGetFriendsListRequestWrapper : public ModelWrapper<PFFriendsServerGetFriendsListRequest, Alloc>
{
public:
    using ModelType = PFFriendsServerGetFriendsListRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFFriendsServerGetFriendsListRequestWrapper() = default;

    PFFriendsServerGetFriendsListRequestWrapper(const PFFriendsServerGetFriendsListRequest& model) :
        ModelWrapper<PFFriendsServerGetFriendsListRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_externalPlatformFriends{ model.externalPlatformFriends ? std::optional<PFFriendsExternalFriendSources>{ *model.externalPlatformFriends } : std::nullopt },
        m_playFabId{ SafeString(model.playFabId) },
        m_profileConstraints{ model.profileConstraints ? std::optional<PFPlayerProfileViewConstraintsWrapper<Alloc>>{ *model.profileConstraints } : std::nullopt },
        m_xboxToken{ SafeString(model.xboxToken) }
    {
        SetModelPointers();
    }

    PFFriendsServerGetFriendsListRequestWrapper(const PFFriendsServerGetFriendsListRequestWrapper& src) :
        PFFriendsServerGetFriendsListRequestWrapper{ src.Model() }
    {
    }

    PFFriendsServerGetFriendsListRequestWrapper(PFFriendsServerGetFriendsListRequestWrapper&& src) :
        PFFriendsServerGetFriendsListRequestWrapper{}
    {
        swap(*this, src);
    }

    PFFriendsServerGetFriendsListRequestWrapper& operator=(PFFriendsServerGetFriendsListRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFFriendsServerGetFriendsListRequestWrapper() = default;

    friend void swap(PFFriendsServerGetFriendsListRequestWrapper& lhs, PFFriendsServerGetFriendsListRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_externalPlatformFriends, rhs.m_externalPlatformFriends);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_profileConstraints, rhs.m_profileConstraints);
        swap(lhs.m_xboxToken, rhs.m_xboxToken);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetExternalPlatformFriends(std::optional<PFFriendsExternalFriendSources> value)
    {
        m_externalPlatformFriends = std::move(value);
        this->m_model.externalPlatformFriends = m_externalPlatformFriends ? m_externalPlatformFriends.operator->() : nullptr;
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetProfileConstraints(std::optional<PFPlayerProfileViewConstraintsWrapper<Alloc>> value)
    {
        m_profileConstraints = std::move(value);
        this->m_model.profileConstraints = m_profileConstraints ? &m_profileConstraints->Model() : nullptr;
    }

    void SetXboxToken(String value)
    {
        m_xboxToken = std::move(value);
        this->m_model.xboxToken =  m_xboxToken.empty() ? nullptr : m_xboxToken.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.externalPlatformFriends = m_externalPlatformFriends ? m_externalPlatformFriends.operator->() : nullptr;
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.profileConstraints = m_profileConstraints ?  &m_profileConstraints->Model() : nullptr;
        this->m_model.xboxToken = m_xboxToken.empty() ? nullptr : m_xboxToken.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFFriendsExternalFriendSources> m_externalPlatformFriends;
    String m_playFabId;
    std::optional<PFPlayerProfileViewConstraintsWrapper<Alloc>> m_profileConstraints;
    String m_xboxToken;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFFriendsServerRemoveFriendRequestWrapper : public ModelWrapper<PFFriendsServerRemoveFriendRequest, Alloc>
{
public:
    using ModelType = PFFriendsServerRemoveFriendRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFFriendsServerRemoveFriendRequestWrapper() = default;

    PFFriendsServerRemoveFriendRequestWrapper(const PFFriendsServerRemoveFriendRequest& model) :
        ModelWrapper<PFFriendsServerRemoveFriendRequest, Alloc>{ model },
        m_friendPlayFabId{ SafeString(model.friendPlayFabId) },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFFriendsServerRemoveFriendRequestWrapper(const PFFriendsServerRemoveFriendRequestWrapper& src) :
        PFFriendsServerRemoveFriendRequestWrapper{ src.Model() }
    {
    }

    PFFriendsServerRemoveFriendRequestWrapper(PFFriendsServerRemoveFriendRequestWrapper&& src) :
        PFFriendsServerRemoveFriendRequestWrapper{}
    {
        swap(*this, src);
    }

    PFFriendsServerRemoveFriendRequestWrapper& operator=(PFFriendsServerRemoveFriendRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFFriendsServerRemoveFriendRequestWrapper() = default;

    friend void swap(PFFriendsServerRemoveFriendRequestWrapper& lhs, PFFriendsServerRemoveFriendRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_friendPlayFabId, rhs.m_friendPlayFabId);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFriendPlayFabId(String value)
    {
        m_friendPlayFabId = std::move(value);
        this->m_model.friendPlayFabId =  m_friendPlayFabId.empty() ? nullptr : m_friendPlayFabId.data();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.friendPlayFabId = m_friendPlayFabId.empty() ? nullptr : m_friendPlayFabId.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String m_friendPlayFabId;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFFriendsServerSetFriendTagsRequestWrapper : public ModelWrapper<PFFriendsServerSetFriendTagsRequest, Alloc>
{
public:
    using ModelType = PFFriendsServerSetFriendTagsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFFriendsServerSetFriendTagsRequestWrapper() = default;

    PFFriendsServerSetFriendTagsRequestWrapper(const PFFriendsServerSetFriendTagsRequest& model) :
        ModelWrapper<PFFriendsServerSetFriendTagsRequest, Alloc>{ model },
        m_friendPlayFabId{ SafeString(model.friendPlayFabId) },
        m_playFabId{ SafeString(model.playFabId) },
        m_tags{ model.tags, model.tags + model.tagsCount }
    {
        SetModelPointers();
    }

    PFFriendsServerSetFriendTagsRequestWrapper(const PFFriendsServerSetFriendTagsRequestWrapper& src) :
        PFFriendsServerSetFriendTagsRequestWrapper{ src.Model() }
    {
    }

    PFFriendsServerSetFriendTagsRequestWrapper(PFFriendsServerSetFriendTagsRequestWrapper&& src) :
        PFFriendsServerSetFriendTagsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFFriendsServerSetFriendTagsRequestWrapper& operator=(PFFriendsServerSetFriendTagsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFFriendsServerSetFriendTagsRequestWrapper() = default;

    friend void swap(PFFriendsServerSetFriendTagsRequestWrapper& lhs, PFFriendsServerSetFriendTagsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_friendPlayFabId, rhs.m_friendPlayFabId);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_tags, rhs.m_tags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFriendPlayFabId(String value)
    {
        m_friendPlayFabId = std::move(value);
        this->m_model.friendPlayFabId =  m_friendPlayFabId.empty() ? nullptr : m_friendPlayFabId.data();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetTags(CStringVector<Alloc> value)
    {
        m_tags = std::move(value);
        this->m_model.tags =  m_tags.empty() ? nullptr : m_tags.data();
        this->m_model.tagsCount =  static_cast<uint32_t>(m_tags.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.friendPlayFabId = m_friendPlayFabId.empty() ? nullptr : m_friendPlayFabId.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.tags = m_tags.empty() ? nullptr : m_tags.data();
    }

    String m_friendPlayFabId;
    String m_playFabId;
    CStringVector<Alloc> m_tags;
};

} // namespace Wrappers
} // namespace PlayFab

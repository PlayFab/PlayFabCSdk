// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFAccountManagementTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>
#if HC_PLATFORM == HC_PLATFORM_GDK
#include <playfab/core/cpp/AuthenticationTypeWrappers.h>
#endif

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementAddOrUpdateContactEmailRequestWrapper : public ModelWrapper<PFAccountManagementAddOrUpdateContactEmailRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementAddOrUpdateContactEmailRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementAddOrUpdateContactEmailRequestWrapper() = default;

    PFAccountManagementAddOrUpdateContactEmailRequestWrapper(const PFAccountManagementAddOrUpdateContactEmailRequest& model) :
        ModelWrapper<PFAccountManagementAddOrUpdateContactEmailRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_emailAddress{ SafeString(model.emailAddress) }
    {
        SetModelPointers();
    }

    PFAccountManagementAddOrUpdateContactEmailRequestWrapper(const PFAccountManagementAddOrUpdateContactEmailRequestWrapper& src) :
        PFAccountManagementAddOrUpdateContactEmailRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementAddOrUpdateContactEmailRequestWrapper(PFAccountManagementAddOrUpdateContactEmailRequestWrapper&& src) :
        PFAccountManagementAddOrUpdateContactEmailRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementAddOrUpdateContactEmailRequestWrapper& operator=(PFAccountManagementAddOrUpdateContactEmailRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementAddOrUpdateContactEmailRequestWrapper() = default;

    friend void swap(PFAccountManagementAddOrUpdateContactEmailRequestWrapper& lhs, PFAccountManagementAddOrUpdateContactEmailRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_emailAddress, rhs.m_emailAddress);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEmailAddress(String value)
    {
        m_emailAddress = std::move(value);
        this->m_model.emailAddress =  m_emailAddress.empty() ? nullptr : m_emailAddress.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.emailAddress = m_emailAddress.empty() ? nullptr : m_emailAddress.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_emailAddress;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementAddUsernamePasswordRequestWrapper : public ModelWrapper<PFAccountManagementAddUsernamePasswordRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementAddUsernamePasswordRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementAddUsernamePasswordRequestWrapper() = default;

    PFAccountManagementAddUsernamePasswordRequestWrapper(const PFAccountManagementAddUsernamePasswordRequest& model) :
        ModelWrapper<PFAccountManagementAddUsernamePasswordRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_email{ SafeString(model.email) },
        m_password{ SafeString(model.password) },
        m_username{ SafeString(model.username) }
    {
        SetModelPointers();
    }

    PFAccountManagementAddUsernamePasswordRequestWrapper(const PFAccountManagementAddUsernamePasswordRequestWrapper& src) :
        PFAccountManagementAddUsernamePasswordRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementAddUsernamePasswordRequestWrapper(PFAccountManagementAddUsernamePasswordRequestWrapper&& src) :
        PFAccountManagementAddUsernamePasswordRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementAddUsernamePasswordRequestWrapper& operator=(PFAccountManagementAddUsernamePasswordRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementAddUsernamePasswordRequestWrapper() = default;

    friend void swap(PFAccountManagementAddUsernamePasswordRequestWrapper& lhs, PFAccountManagementAddUsernamePasswordRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_email, rhs.m_email);
        swap(lhs.m_password, rhs.m_password);
        swap(lhs.m_username, rhs.m_username);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEmail(String value)
    {
        m_email = std::move(value);
        this->m_model.email =  m_email.empty() ? nullptr : m_email.data();
    }

    void SetPassword(String value)
    {
        m_password = std::move(value);
        this->m_model.password =  m_password.empty() ? nullptr : m_password.data();
    }

    void SetUsername(String value)
    {
        m_username = std::move(value);
        this->m_model.username =  m_username.empty() ? nullptr : m_username.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.email = m_email.empty() ? nullptr : m_email.data();
        this->m_model.password = m_password.empty() ? nullptr : m_password.data();
        this->m_model.username = m_username.empty() ? nullptr : m_username.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_email;
    String m_password;
    String m_username;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementAddUsernamePasswordResultWrapper : public ModelWrapper<PFAccountManagementAddUsernamePasswordResult, Alloc>
{
public:
    using ModelType = PFAccountManagementAddUsernamePasswordResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementAddUsernamePasswordResultWrapper() = default;

    PFAccountManagementAddUsernamePasswordResultWrapper(const PFAccountManagementAddUsernamePasswordResult& model) :
        ModelWrapper<PFAccountManagementAddUsernamePasswordResult, Alloc>{ model },
        m_username{ SafeString(model.username) }
    {
        SetModelPointers();
    }

    PFAccountManagementAddUsernamePasswordResultWrapper(const PFAccountManagementAddUsernamePasswordResultWrapper& src) :
        PFAccountManagementAddUsernamePasswordResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementAddUsernamePasswordResultWrapper(PFAccountManagementAddUsernamePasswordResultWrapper&& src) :
        PFAccountManagementAddUsernamePasswordResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementAddUsernamePasswordResultWrapper& operator=(PFAccountManagementAddUsernamePasswordResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementAddUsernamePasswordResultWrapper() = default;

    friend void swap(PFAccountManagementAddUsernamePasswordResultWrapper& lhs, PFAccountManagementAddUsernamePasswordResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_username, rhs.m_username);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetUsername(String value)
    {
        m_username = std::move(value);
        this->m_model.username =  m_username.empty() ? nullptr : m_username.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.username = m_username.empty() ? nullptr : m_username.data();
    }

    String m_username;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetAccountInfoRequestWrapper : public ModelWrapper<PFAccountManagementGetAccountInfoRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementGetAccountInfoRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetAccountInfoRequestWrapper() = default;

    PFAccountManagementGetAccountInfoRequestWrapper(const PFAccountManagementGetAccountInfoRequest& model) :
        ModelWrapper<PFAccountManagementGetAccountInfoRequest, Alloc>{ model },
        m_email{ SafeString(model.email) },
        m_playFabId{ SafeString(model.playFabId) },
        m_titleDisplayName{ SafeString(model.titleDisplayName) },
        m_username{ SafeString(model.username) }
    {
        SetModelPointers();
    }

    PFAccountManagementGetAccountInfoRequestWrapper(const PFAccountManagementGetAccountInfoRequestWrapper& src) :
        PFAccountManagementGetAccountInfoRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetAccountInfoRequestWrapper(PFAccountManagementGetAccountInfoRequestWrapper&& src) :
        PFAccountManagementGetAccountInfoRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetAccountInfoRequestWrapper& operator=(PFAccountManagementGetAccountInfoRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetAccountInfoRequestWrapper() = default;

    friend void swap(PFAccountManagementGetAccountInfoRequestWrapper& lhs, PFAccountManagementGetAccountInfoRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_email, rhs.m_email);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_titleDisplayName, rhs.m_titleDisplayName);
        swap(lhs.m_username, rhs.m_username);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetEmail(String value)
    {
        m_email = std::move(value);
        this->m_model.email =  m_email.empty() ? nullptr : m_email.data();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
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

private:
    void SetModelPointers()
    {
        this->m_model.email = m_email.empty() ? nullptr : m_email.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.titleDisplayName = m_titleDisplayName.empty() ? nullptr : m_titleDisplayName.data();
        this->m_model.username = m_username.empty() ? nullptr : m_username.data();
    }

    String m_email;
    String m_playFabId;
    String m_titleDisplayName;
    String m_username;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetAccountInfoResultWrapper : public ModelWrapper<PFAccountManagementGetAccountInfoResult, Alloc>
{
public:
    using ModelType = PFAccountManagementGetAccountInfoResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetAccountInfoResultWrapper() = default;

    PFAccountManagementGetAccountInfoResultWrapper(const PFAccountManagementGetAccountInfoResult& model) :
        ModelWrapper<PFAccountManagementGetAccountInfoResult, Alloc>{ model },
        m_accountInfo{ model.accountInfo ? std::optional<PFUserAccountInfoWrapper<Alloc>>{ *model.accountInfo } : std::nullopt }
    {
        SetModelPointers();
    }

    PFAccountManagementGetAccountInfoResultWrapper(const PFAccountManagementGetAccountInfoResultWrapper& src) :
        PFAccountManagementGetAccountInfoResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetAccountInfoResultWrapper(PFAccountManagementGetAccountInfoResultWrapper&& src) :
        PFAccountManagementGetAccountInfoResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetAccountInfoResultWrapper& operator=(PFAccountManagementGetAccountInfoResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetAccountInfoResultWrapper() = default;

    friend void swap(PFAccountManagementGetAccountInfoResultWrapper& lhs, PFAccountManagementGetAccountInfoResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_accountInfo, rhs.m_accountInfo);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAccountInfo(std::optional<PFUserAccountInfoWrapper<Alloc>> value)
    {
        m_accountInfo = std::move(value);
        this->m_model.accountInfo = m_accountInfo ? &m_accountInfo->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.accountInfo = m_accountInfo ?  &m_accountInfo->Model() : nullptr;
    }

    std::optional<PFUserAccountInfoWrapper<Alloc>> m_accountInfo;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayerCombinedInfoRequestWrapper : public ModelWrapper<PFAccountManagementGetPlayerCombinedInfoRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayerCombinedInfoRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayerCombinedInfoRequestWrapper() = default;

    PFAccountManagementGetPlayerCombinedInfoRequestWrapper(const PFAccountManagementGetPlayerCombinedInfoRequest& model) :
        ModelWrapper<PFAccountManagementGetPlayerCombinedInfoRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_infoRequestParameters{ model.infoRequestParameters ? *model.infoRequestParameters : decltype(*model.infoRequestParameters){} },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayerCombinedInfoRequestWrapper(const PFAccountManagementGetPlayerCombinedInfoRequestWrapper& src) :
        PFAccountManagementGetPlayerCombinedInfoRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayerCombinedInfoRequestWrapper(PFAccountManagementGetPlayerCombinedInfoRequestWrapper&& src) :
        PFAccountManagementGetPlayerCombinedInfoRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayerCombinedInfoRequestWrapper& operator=(PFAccountManagementGetPlayerCombinedInfoRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayerCombinedInfoRequestWrapper() = default;

    friend void swap(PFAccountManagementGetPlayerCombinedInfoRequestWrapper& lhs, PFAccountManagementGetPlayerCombinedInfoRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetInfoRequestParameters(PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = &m_infoRequestParameters.Model();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.infoRequestParameters = &m_infoRequestParameters.Model();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc> m_infoRequestParameters;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayerCombinedInfoResultWrapper : public ModelWrapper<PFAccountManagementGetPlayerCombinedInfoResult, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayerCombinedInfoResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayerCombinedInfoResultWrapper() = default;

    PFAccountManagementGetPlayerCombinedInfoResultWrapper(const PFAccountManagementGetPlayerCombinedInfoResult& model) :
        ModelWrapper<PFAccountManagementGetPlayerCombinedInfoResult, Alloc>{ model },
        m_infoResultPayload{ model.infoResultPayload ? std::optional<PFGetPlayerCombinedInfoResultPayloadWrapper<Alloc>>{ *model.infoResultPayload } : std::nullopt },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayerCombinedInfoResultWrapper(const PFAccountManagementGetPlayerCombinedInfoResultWrapper& src) :
        PFAccountManagementGetPlayerCombinedInfoResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayerCombinedInfoResultWrapper(PFAccountManagementGetPlayerCombinedInfoResultWrapper&& src) :
        PFAccountManagementGetPlayerCombinedInfoResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayerCombinedInfoResultWrapper& operator=(PFAccountManagementGetPlayerCombinedInfoResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayerCombinedInfoResultWrapper() = default;

    friend void swap(PFAccountManagementGetPlayerCombinedInfoResultWrapper& lhs, PFAccountManagementGetPlayerCombinedInfoResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_infoResultPayload, rhs.m_infoResultPayload);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetInfoResultPayload(std::optional<PFGetPlayerCombinedInfoResultPayloadWrapper<Alloc>> value)
    {
        m_infoResultPayload = std::move(value);
        this->m_model.infoResultPayload = m_infoResultPayload ? &m_infoResultPayload->Model() : nullptr;
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.infoResultPayload = m_infoResultPayload ?  &m_infoResultPayload->Model() : nullptr;
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    std::optional<PFGetPlayerCombinedInfoResultPayloadWrapper<Alloc>> m_infoResultPayload;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayerProfileRequestWrapper : public ModelWrapper<PFAccountManagementGetPlayerProfileRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayerProfileRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayerProfileRequestWrapper() = default;

    PFAccountManagementGetPlayerProfileRequestWrapper(const PFAccountManagementGetPlayerProfileRequest& model) :
        ModelWrapper<PFAccountManagementGetPlayerProfileRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_playFabId{ SafeString(model.playFabId) },
        m_profileConstraints{ model.profileConstraints ? std::optional<PFPlayerProfileViewConstraintsWrapper<Alloc>>{ *model.profileConstraints } : std::nullopt }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayerProfileRequestWrapper(const PFAccountManagementGetPlayerProfileRequestWrapper& src) :
        PFAccountManagementGetPlayerProfileRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayerProfileRequestWrapper(PFAccountManagementGetPlayerProfileRequestWrapper&& src) :
        PFAccountManagementGetPlayerProfileRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayerProfileRequestWrapper& operator=(PFAccountManagementGetPlayerProfileRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayerProfileRequestWrapper() = default;

    friend void swap(PFAccountManagementGetPlayerProfileRequestWrapper& lhs, PFAccountManagementGetPlayerProfileRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_profileConstraints, rhs.m_profileConstraints);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
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

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.profileConstraints = m_profileConstraints ?  &m_profileConstraints->Model() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_playFabId;
    std::optional<PFPlayerProfileViewConstraintsWrapper<Alloc>> m_profileConstraints;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayerProfileResultWrapper : public ModelWrapper<PFAccountManagementGetPlayerProfileResult, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayerProfileResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayerProfileResultWrapper() = default;

    PFAccountManagementGetPlayerProfileResultWrapper(const PFAccountManagementGetPlayerProfileResult& model) :
        ModelWrapper<PFAccountManagementGetPlayerProfileResult, Alloc>{ model },
        m_playerProfile{ model.playerProfile ? std::optional<PFPlayerProfileModelWrapper<Alloc>>{ *model.playerProfile } : std::nullopt }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayerProfileResultWrapper(const PFAccountManagementGetPlayerProfileResultWrapper& src) :
        PFAccountManagementGetPlayerProfileResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayerProfileResultWrapper(PFAccountManagementGetPlayerProfileResultWrapper&& src) :
        PFAccountManagementGetPlayerProfileResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayerProfileResultWrapper& operator=(PFAccountManagementGetPlayerProfileResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayerProfileResultWrapper() = default;

    friend void swap(PFAccountManagementGetPlayerProfileResultWrapper& lhs, PFAccountManagementGetPlayerProfileResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_playerProfile, rhs.m_playerProfile);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetPlayerProfile(std::optional<PFPlayerProfileModelWrapper<Alloc>> value)
    {
        m_playerProfile = std::move(value);
        this->m_model.playerProfile = m_playerProfile ? &m_playerProfile->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.playerProfile = m_playerProfile ?  &m_playerProfile->Model() : nullptr;
    }

    std::optional<PFPlayerProfileModelWrapper<Alloc>> m_playerProfile;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromFacebookIDsRequestWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromFacebookIDsRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromFacebookIDsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromFacebookIDsRequestWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromFacebookIDsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromFacebookIDsRequest& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromFacebookIDsRequest, Alloc>{ model },
        m_facebookIDs{ model.facebookIDs, model.facebookIDs + model.facebookIDsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromFacebookIDsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromFacebookIDsRequestWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromFacebookIDsRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromFacebookIDsRequestWrapper(PFAccountManagementGetPlayFabIDsFromFacebookIDsRequestWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromFacebookIDsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromFacebookIDsRequestWrapper& operator=(PFAccountManagementGetPlayFabIDsFromFacebookIDsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromFacebookIDsRequestWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromFacebookIDsRequestWrapper& lhs, PFAccountManagementGetPlayFabIDsFromFacebookIDsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_facebookIDs, rhs.m_facebookIDs);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFacebookIDs(CStringVector<Alloc> value)
    {
        m_facebookIDs = std::move(value);
        this->m_model.facebookIDs =  m_facebookIDs.empty() ? nullptr : m_facebookIDs.data();
        this->m_model.facebookIDsCount =  static_cast<uint32_t>(m_facebookIDs.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.facebookIDs = m_facebookIDs.empty() ? nullptr : m_facebookIDs.data();
    }

    CStringVector<Alloc> m_facebookIDs;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementFacebookPlayFabIdPairWrapper : public ModelWrapper<PFAccountManagementFacebookPlayFabIdPair, Alloc>
{
public:
    using ModelType = PFAccountManagementFacebookPlayFabIdPair;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementFacebookPlayFabIdPairWrapper() = default;

    PFAccountManagementFacebookPlayFabIdPairWrapper(const PFAccountManagementFacebookPlayFabIdPair& model) :
        ModelWrapper<PFAccountManagementFacebookPlayFabIdPair, Alloc>{ model },
        m_facebookId{ SafeString(model.facebookId) },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFAccountManagementFacebookPlayFabIdPairWrapper(const PFAccountManagementFacebookPlayFabIdPairWrapper& src) :
        PFAccountManagementFacebookPlayFabIdPairWrapper{ src.Model() }
    {
    }

    PFAccountManagementFacebookPlayFabIdPairWrapper(PFAccountManagementFacebookPlayFabIdPairWrapper&& src) :
        PFAccountManagementFacebookPlayFabIdPairWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementFacebookPlayFabIdPairWrapper& operator=(PFAccountManagementFacebookPlayFabIdPairWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementFacebookPlayFabIdPairWrapper() = default;

    friend void swap(PFAccountManagementFacebookPlayFabIdPairWrapper& lhs, PFAccountManagementFacebookPlayFabIdPairWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_facebookId, rhs.m_facebookId);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFacebookId(String value)
    {
        m_facebookId = std::move(value);
        this->m_model.facebookId =  m_facebookId.empty() ? nullptr : m_facebookId.data();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.facebookId = m_facebookId.empty() ? nullptr : m_facebookId.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String m_facebookId;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromFacebookIDsResultWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromFacebookIDsResult, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromFacebookIDsResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromFacebookIDsResultWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromFacebookIDsResultWrapper(const PFAccountManagementGetPlayFabIDsFromFacebookIDsResult& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromFacebookIDsResult, Alloc>{ model },
        m_data{ model.data, model.data + model.dataCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromFacebookIDsResultWrapper(const PFAccountManagementGetPlayFabIDsFromFacebookIDsResultWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromFacebookIDsResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromFacebookIDsResultWrapper(PFAccountManagementGetPlayFabIDsFromFacebookIDsResultWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromFacebookIDsResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromFacebookIDsResultWrapper& operator=(PFAccountManagementGetPlayFabIDsFromFacebookIDsResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromFacebookIDsResultWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromFacebookIDsResultWrapper& lhs, PFAccountManagementGetPlayFabIDsFromFacebookIDsResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_data, rhs.m_data);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetData(ModelVector<PFAccountManagementFacebookPlayFabIdPairWrapper<Alloc>, Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
    }

    ModelVector<PFAccountManagementFacebookPlayFabIdPairWrapper<Alloc>, Alloc> m_data;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequestWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequestWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequest& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequest, Alloc>{ model },
        m_facebookInstantGamesIds{ model.facebookInstantGamesIds, model.facebookInstantGamesIds + model.facebookInstantGamesIdsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequestWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequestWrapper(PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequestWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequestWrapper& operator=(PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequestWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequestWrapper& lhs, PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_facebookInstantGamesIds, rhs.m_facebookInstantGamesIds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFacebookInstantGamesIds(CStringVector<Alloc> value)
    {
        m_facebookInstantGamesIds = std::move(value);
        this->m_model.facebookInstantGamesIds =  m_facebookInstantGamesIds.empty() ? nullptr : m_facebookInstantGamesIds.data();
        this->m_model.facebookInstantGamesIdsCount =  static_cast<uint32_t>(m_facebookInstantGamesIds.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.facebookInstantGamesIds = m_facebookInstantGamesIds.empty() ? nullptr : m_facebookInstantGamesIds.data();
    }

    CStringVector<Alloc> m_facebookInstantGamesIds;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementFacebookInstantGamesPlayFabIdPairWrapper : public ModelWrapper<PFAccountManagementFacebookInstantGamesPlayFabIdPair, Alloc>
{
public:
    using ModelType = PFAccountManagementFacebookInstantGamesPlayFabIdPair;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementFacebookInstantGamesPlayFabIdPairWrapper() = default;

    PFAccountManagementFacebookInstantGamesPlayFabIdPairWrapper(const PFAccountManagementFacebookInstantGamesPlayFabIdPair& model) :
        ModelWrapper<PFAccountManagementFacebookInstantGamesPlayFabIdPair, Alloc>{ model },
        m_facebookInstantGamesId{ SafeString(model.facebookInstantGamesId) },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFAccountManagementFacebookInstantGamesPlayFabIdPairWrapper(const PFAccountManagementFacebookInstantGamesPlayFabIdPairWrapper& src) :
        PFAccountManagementFacebookInstantGamesPlayFabIdPairWrapper{ src.Model() }
    {
    }

    PFAccountManagementFacebookInstantGamesPlayFabIdPairWrapper(PFAccountManagementFacebookInstantGamesPlayFabIdPairWrapper&& src) :
        PFAccountManagementFacebookInstantGamesPlayFabIdPairWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementFacebookInstantGamesPlayFabIdPairWrapper& operator=(PFAccountManagementFacebookInstantGamesPlayFabIdPairWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementFacebookInstantGamesPlayFabIdPairWrapper() = default;

    friend void swap(PFAccountManagementFacebookInstantGamesPlayFabIdPairWrapper& lhs, PFAccountManagementFacebookInstantGamesPlayFabIdPairWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_facebookInstantGamesId, rhs.m_facebookInstantGamesId);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFacebookInstantGamesId(String value)
    {
        m_facebookInstantGamesId = std::move(value);
        this->m_model.facebookInstantGamesId =  m_facebookInstantGamesId.empty() ? nullptr : m_facebookInstantGamesId.data();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.facebookInstantGamesId = m_facebookInstantGamesId.empty() ? nullptr : m_facebookInstantGamesId.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String m_facebookInstantGamesId;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResultWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResult, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResultWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResultWrapper(const PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResult& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResult, Alloc>{ model },
        m_data{ model.data, model.data + model.dataCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResultWrapper(const PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResultWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResultWrapper(PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResultWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResultWrapper& operator=(PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResultWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResultWrapper& lhs, PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_data, rhs.m_data);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetData(ModelVector<PFAccountManagementFacebookInstantGamesPlayFabIdPairWrapper<Alloc>, Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
    }

    ModelVector<PFAccountManagementFacebookInstantGamesPlayFabIdPairWrapper<Alloc>, Alloc> m_data;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequestWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequestWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequest& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequest, Alloc>{ model },
        m_gameCenterIDs{ model.gameCenterIDs, model.gameCenterIDs + model.gameCenterIDsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequestWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequestWrapper(PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequestWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequestWrapper& operator=(PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequestWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequestWrapper& lhs, PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_gameCenterIDs, rhs.m_gameCenterIDs);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetGameCenterIDs(CStringVector<Alloc> value)
    {
        m_gameCenterIDs = std::move(value);
        this->m_model.gameCenterIDs =  m_gameCenterIDs.empty() ? nullptr : m_gameCenterIDs.data();
        this->m_model.gameCenterIDsCount =  static_cast<uint32_t>(m_gameCenterIDs.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.gameCenterIDs = m_gameCenterIDs.empty() ? nullptr : m_gameCenterIDs.data();
    }

    CStringVector<Alloc> m_gameCenterIDs;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGameCenterPlayFabIdPairWrapper : public ModelWrapper<PFAccountManagementGameCenterPlayFabIdPair, Alloc>
{
public:
    using ModelType = PFAccountManagementGameCenterPlayFabIdPair;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGameCenterPlayFabIdPairWrapper() = default;

    PFAccountManagementGameCenterPlayFabIdPairWrapper(const PFAccountManagementGameCenterPlayFabIdPair& model) :
        ModelWrapper<PFAccountManagementGameCenterPlayFabIdPair, Alloc>{ model },
        m_gameCenterId{ SafeString(model.gameCenterId) },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFAccountManagementGameCenterPlayFabIdPairWrapper(const PFAccountManagementGameCenterPlayFabIdPairWrapper& src) :
        PFAccountManagementGameCenterPlayFabIdPairWrapper{ src.Model() }
    {
    }

    PFAccountManagementGameCenterPlayFabIdPairWrapper(PFAccountManagementGameCenterPlayFabIdPairWrapper&& src) :
        PFAccountManagementGameCenterPlayFabIdPairWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGameCenterPlayFabIdPairWrapper& operator=(PFAccountManagementGameCenterPlayFabIdPairWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGameCenterPlayFabIdPairWrapper() = default;

    friend void swap(PFAccountManagementGameCenterPlayFabIdPairWrapper& lhs, PFAccountManagementGameCenterPlayFabIdPairWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_gameCenterId, rhs.m_gameCenterId);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetGameCenterId(String value)
    {
        m_gameCenterId = std::move(value);
        this->m_model.gameCenterId =  m_gameCenterId.empty() ? nullptr : m_gameCenterId.data();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.gameCenterId = m_gameCenterId.empty() ? nullptr : m_gameCenterId.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String m_gameCenterId;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromGameCenterIDsResultWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromGameCenterIDsResult, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromGameCenterIDsResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromGameCenterIDsResultWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromGameCenterIDsResultWrapper(const PFAccountManagementGetPlayFabIDsFromGameCenterIDsResult& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromGameCenterIDsResult, Alloc>{ model },
        m_data{ model.data, model.data + model.dataCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromGameCenterIDsResultWrapper(const PFAccountManagementGetPlayFabIDsFromGameCenterIDsResultWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromGameCenterIDsResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromGameCenterIDsResultWrapper(PFAccountManagementGetPlayFabIDsFromGameCenterIDsResultWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromGameCenterIDsResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromGameCenterIDsResultWrapper& operator=(PFAccountManagementGetPlayFabIDsFromGameCenterIDsResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromGameCenterIDsResultWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromGameCenterIDsResultWrapper& lhs, PFAccountManagementGetPlayFabIDsFromGameCenterIDsResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_data, rhs.m_data);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetData(ModelVector<PFAccountManagementGameCenterPlayFabIdPairWrapper<Alloc>, Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
    }

    ModelVector<PFAccountManagementGameCenterPlayFabIdPairWrapper<Alloc>, Alloc> m_data;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromGoogleIDsRequestWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromGoogleIDsRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromGoogleIDsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromGoogleIDsRequestWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromGoogleIDsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromGoogleIDsRequest& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromGoogleIDsRequest, Alloc>{ model },
        m_googleIDs{ model.googleIDs, model.googleIDs + model.googleIDsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromGoogleIDsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromGoogleIDsRequestWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromGoogleIDsRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromGoogleIDsRequestWrapper(PFAccountManagementGetPlayFabIDsFromGoogleIDsRequestWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromGoogleIDsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromGoogleIDsRequestWrapper& operator=(PFAccountManagementGetPlayFabIDsFromGoogleIDsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromGoogleIDsRequestWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromGoogleIDsRequestWrapper& lhs, PFAccountManagementGetPlayFabIDsFromGoogleIDsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_googleIDs, rhs.m_googleIDs);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetGoogleIDs(CStringVector<Alloc> value)
    {
        m_googleIDs = std::move(value);
        this->m_model.googleIDs =  m_googleIDs.empty() ? nullptr : m_googleIDs.data();
        this->m_model.googleIDsCount =  static_cast<uint32_t>(m_googleIDs.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.googleIDs = m_googleIDs.empty() ? nullptr : m_googleIDs.data();
    }

    CStringVector<Alloc> m_googleIDs;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGooglePlayFabIdPairWrapper : public ModelWrapper<PFAccountManagementGooglePlayFabIdPair, Alloc>
{
public:
    using ModelType = PFAccountManagementGooglePlayFabIdPair;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGooglePlayFabIdPairWrapper() = default;

    PFAccountManagementGooglePlayFabIdPairWrapper(const PFAccountManagementGooglePlayFabIdPair& model) :
        ModelWrapper<PFAccountManagementGooglePlayFabIdPair, Alloc>{ model },
        m_googleId{ SafeString(model.googleId) },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFAccountManagementGooglePlayFabIdPairWrapper(const PFAccountManagementGooglePlayFabIdPairWrapper& src) :
        PFAccountManagementGooglePlayFabIdPairWrapper{ src.Model() }
    {
    }

    PFAccountManagementGooglePlayFabIdPairWrapper(PFAccountManagementGooglePlayFabIdPairWrapper&& src) :
        PFAccountManagementGooglePlayFabIdPairWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGooglePlayFabIdPairWrapper& operator=(PFAccountManagementGooglePlayFabIdPairWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGooglePlayFabIdPairWrapper() = default;

    friend void swap(PFAccountManagementGooglePlayFabIdPairWrapper& lhs, PFAccountManagementGooglePlayFabIdPairWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_googleId, rhs.m_googleId);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetGoogleId(String value)
    {
        m_googleId = std::move(value);
        this->m_model.googleId =  m_googleId.empty() ? nullptr : m_googleId.data();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.googleId = m_googleId.empty() ? nullptr : m_googleId.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String m_googleId;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromGoogleIDsResultWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromGoogleIDsResult, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromGoogleIDsResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromGoogleIDsResultWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromGoogleIDsResultWrapper(const PFAccountManagementGetPlayFabIDsFromGoogleIDsResult& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromGoogleIDsResult, Alloc>{ model },
        m_data{ model.data, model.data + model.dataCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromGoogleIDsResultWrapper(const PFAccountManagementGetPlayFabIDsFromGoogleIDsResultWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromGoogleIDsResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromGoogleIDsResultWrapper(PFAccountManagementGetPlayFabIDsFromGoogleIDsResultWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromGoogleIDsResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromGoogleIDsResultWrapper& operator=(PFAccountManagementGetPlayFabIDsFromGoogleIDsResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromGoogleIDsResultWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromGoogleIDsResultWrapper& lhs, PFAccountManagementGetPlayFabIDsFromGoogleIDsResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_data, rhs.m_data);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetData(ModelVector<PFAccountManagementGooglePlayFabIdPairWrapper<Alloc>, Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
    }

    ModelVector<PFAccountManagementGooglePlayFabIdPairWrapper<Alloc>, Alloc> m_data;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequestWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequestWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequest& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequest, Alloc>{ model },
        m_googlePlayGamesPlayerIDs{ model.googlePlayGamesPlayerIDs, model.googlePlayGamesPlayerIDs + model.googlePlayGamesPlayerIDsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequestWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequestWrapper(PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequestWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequestWrapper& operator=(PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequestWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequestWrapper& lhs, PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_googlePlayGamesPlayerIDs, rhs.m_googlePlayGamesPlayerIDs);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetGooglePlayGamesPlayerIDs(CStringVector<Alloc> value)
    {
        m_googlePlayGamesPlayerIDs = std::move(value);
        this->m_model.googlePlayGamesPlayerIDs =  m_googlePlayGamesPlayerIDs.empty() ? nullptr : m_googlePlayGamesPlayerIDs.data();
        this->m_model.googlePlayGamesPlayerIDsCount =  static_cast<uint32_t>(m_googlePlayGamesPlayerIDs.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.googlePlayGamesPlayerIDs = m_googlePlayGamesPlayerIDs.empty() ? nullptr : m_googlePlayGamesPlayerIDs.data();
    }

    CStringVector<Alloc> m_googlePlayGamesPlayerIDs;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGooglePlayGamesPlayFabIdPairWrapper : public ModelWrapper<PFAccountManagementGooglePlayGamesPlayFabIdPair, Alloc>
{
public:
    using ModelType = PFAccountManagementGooglePlayGamesPlayFabIdPair;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGooglePlayGamesPlayFabIdPairWrapper() = default;

    PFAccountManagementGooglePlayGamesPlayFabIdPairWrapper(const PFAccountManagementGooglePlayGamesPlayFabIdPair& model) :
        ModelWrapper<PFAccountManagementGooglePlayGamesPlayFabIdPair, Alloc>{ model },
        m_googlePlayGamesPlayerId{ SafeString(model.googlePlayGamesPlayerId) },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFAccountManagementGooglePlayGamesPlayFabIdPairWrapper(const PFAccountManagementGooglePlayGamesPlayFabIdPairWrapper& src) :
        PFAccountManagementGooglePlayGamesPlayFabIdPairWrapper{ src.Model() }
    {
    }

    PFAccountManagementGooglePlayGamesPlayFabIdPairWrapper(PFAccountManagementGooglePlayGamesPlayFabIdPairWrapper&& src) :
        PFAccountManagementGooglePlayGamesPlayFabIdPairWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGooglePlayGamesPlayFabIdPairWrapper& operator=(PFAccountManagementGooglePlayGamesPlayFabIdPairWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGooglePlayGamesPlayFabIdPairWrapper() = default;

    friend void swap(PFAccountManagementGooglePlayGamesPlayFabIdPairWrapper& lhs, PFAccountManagementGooglePlayGamesPlayFabIdPairWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_googlePlayGamesPlayerId, rhs.m_googlePlayGamesPlayerId);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetGooglePlayGamesPlayerId(String value)
    {
        m_googlePlayGamesPlayerId = std::move(value);
        this->m_model.googlePlayGamesPlayerId =  m_googlePlayGamesPlayerId.empty() ? nullptr : m_googlePlayGamesPlayerId.data();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.googlePlayGamesPlayerId = m_googlePlayGamesPlayerId.empty() ? nullptr : m_googlePlayGamesPlayerId.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String m_googlePlayGamesPlayerId;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResultWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResult, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResultWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResultWrapper(const PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResult& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResult, Alloc>{ model },
        m_data{ model.data, model.data + model.dataCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResultWrapper(const PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResultWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResultWrapper(PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResultWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResultWrapper& operator=(PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResultWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResultWrapper& lhs, PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_data, rhs.m_data);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetData(ModelVector<PFAccountManagementGooglePlayGamesPlayFabIdPairWrapper<Alloc>, Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
    }

    ModelVector<PFAccountManagementGooglePlayGamesPlayFabIdPairWrapper<Alloc>, Alloc> m_data;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromKongregateIDsRequestWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromKongregateIDsRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromKongregateIDsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromKongregateIDsRequestWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromKongregateIDsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromKongregateIDsRequest& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromKongregateIDsRequest, Alloc>{ model },
        m_kongregateIDs{ model.kongregateIDs, model.kongregateIDs + model.kongregateIDsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromKongregateIDsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromKongregateIDsRequestWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromKongregateIDsRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromKongregateIDsRequestWrapper(PFAccountManagementGetPlayFabIDsFromKongregateIDsRequestWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromKongregateIDsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromKongregateIDsRequestWrapper& operator=(PFAccountManagementGetPlayFabIDsFromKongregateIDsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromKongregateIDsRequestWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromKongregateIDsRequestWrapper& lhs, PFAccountManagementGetPlayFabIDsFromKongregateIDsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_kongregateIDs, rhs.m_kongregateIDs);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetKongregateIDs(CStringVector<Alloc> value)
    {
        m_kongregateIDs = std::move(value);
        this->m_model.kongregateIDs =  m_kongregateIDs.empty() ? nullptr : m_kongregateIDs.data();
        this->m_model.kongregateIDsCount =  static_cast<uint32_t>(m_kongregateIDs.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.kongregateIDs = m_kongregateIDs.empty() ? nullptr : m_kongregateIDs.data();
    }

    CStringVector<Alloc> m_kongregateIDs;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementKongregatePlayFabIdPairWrapper : public ModelWrapper<PFAccountManagementKongregatePlayFabIdPair, Alloc>
{
public:
    using ModelType = PFAccountManagementKongregatePlayFabIdPair;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementKongregatePlayFabIdPairWrapper() = default;

    PFAccountManagementKongregatePlayFabIdPairWrapper(const PFAccountManagementKongregatePlayFabIdPair& model) :
        ModelWrapper<PFAccountManagementKongregatePlayFabIdPair, Alloc>{ model },
        m_kongregateId{ SafeString(model.kongregateId) },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFAccountManagementKongregatePlayFabIdPairWrapper(const PFAccountManagementKongregatePlayFabIdPairWrapper& src) :
        PFAccountManagementKongregatePlayFabIdPairWrapper{ src.Model() }
    {
    }

    PFAccountManagementKongregatePlayFabIdPairWrapper(PFAccountManagementKongregatePlayFabIdPairWrapper&& src) :
        PFAccountManagementKongregatePlayFabIdPairWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementKongregatePlayFabIdPairWrapper& operator=(PFAccountManagementKongregatePlayFabIdPairWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementKongregatePlayFabIdPairWrapper() = default;

    friend void swap(PFAccountManagementKongregatePlayFabIdPairWrapper& lhs, PFAccountManagementKongregatePlayFabIdPairWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_kongregateId, rhs.m_kongregateId);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetKongregateId(String value)
    {
        m_kongregateId = std::move(value);
        this->m_model.kongregateId =  m_kongregateId.empty() ? nullptr : m_kongregateId.data();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.kongregateId = m_kongregateId.empty() ? nullptr : m_kongregateId.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String m_kongregateId;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromKongregateIDsResultWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromKongregateIDsResult, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromKongregateIDsResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromKongregateIDsResultWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromKongregateIDsResultWrapper(const PFAccountManagementGetPlayFabIDsFromKongregateIDsResult& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromKongregateIDsResult, Alloc>{ model },
        m_data{ model.data, model.data + model.dataCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromKongregateIDsResultWrapper(const PFAccountManagementGetPlayFabIDsFromKongregateIDsResultWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromKongregateIDsResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromKongregateIDsResultWrapper(PFAccountManagementGetPlayFabIDsFromKongregateIDsResultWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromKongregateIDsResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromKongregateIDsResultWrapper& operator=(PFAccountManagementGetPlayFabIDsFromKongregateIDsResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromKongregateIDsResultWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromKongregateIDsResultWrapper& lhs, PFAccountManagementGetPlayFabIDsFromKongregateIDsResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_data, rhs.m_data);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetData(ModelVector<PFAccountManagementKongregatePlayFabIdPairWrapper<Alloc>, Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
    }

    ModelVector<PFAccountManagementKongregatePlayFabIdPairWrapper<Alloc>, Alloc> m_data;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequestWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequestWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequest& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequest, Alloc>{ model },
        m_nintendoAccountIds{ model.nintendoAccountIds, model.nintendoAccountIds + model.nintendoAccountIdsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequestWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequestWrapper(PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequestWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequestWrapper& operator=(PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequestWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequestWrapper& lhs, PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_nintendoAccountIds, rhs.m_nintendoAccountIds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetNintendoAccountIds(CStringVector<Alloc> value)
    {
        m_nintendoAccountIds = std::move(value);
        this->m_model.nintendoAccountIds =  m_nintendoAccountIds.empty() ? nullptr : m_nintendoAccountIds.data();
        this->m_model.nintendoAccountIdsCount =  static_cast<uint32_t>(m_nintendoAccountIds.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.nintendoAccountIds = m_nintendoAccountIds.empty() ? nullptr : m_nintendoAccountIds.data();
    }

    CStringVector<Alloc> m_nintendoAccountIds;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementNintendoServiceAccountPlayFabIdPairWrapper : public ModelWrapper<PFAccountManagementNintendoServiceAccountPlayFabIdPair, Alloc>
{
public:
    using ModelType = PFAccountManagementNintendoServiceAccountPlayFabIdPair;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementNintendoServiceAccountPlayFabIdPairWrapper() = default;

    PFAccountManagementNintendoServiceAccountPlayFabIdPairWrapper(const PFAccountManagementNintendoServiceAccountPlayFabIdPair& model) :
        ModelWrapper<PFAccountManagementNintendoServiceAccountPlayFabIdPair, Alloc>{ model },
        m_nintendoServiceAccountId{ SafeString(model.nintendoServiceAccountId) },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFAccountManagementNintendoServiceAccountPlayFabIdPairWrapper(const PFAccountManagementNintendoServiceAccountPlayFabIdPairWrapper& src) :
        PFAccountManagementNintendoServiceAccountPlayFabIdPairWrapper{ src.Model() }
    {
    }

    PFAccountManagementNintendoServiceAccountPlayFabIdPairWrapper(PFAccountManagementNintendoServiceAccountPlayFabIdPairWrapper&& src) :
        PFAccountManagementNintendoServiceAccountPlayFabIdPairWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementNintendoServiceAccountPlayFabIdPairWrapper& operator=(PFAccountManagementNintendoServiceAccountPlayFabIdPairWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementNintendoServiceAccountPlayFabIdPairWrapper() = default;

    friend void swap(PFAccountManagementNintendoServiceAccountPlayFabIdPairWrapper& lhs, PFAccountManagementNintendoServiceAccountPlayFabIdPairWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_nintendoServiceAccountId, rhs.m_nintendoServiceAccountId);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetNintendoServiceAccountId(String value)
    {
        m_nintendoServiceAccountId = std::move(value);
        this->m_model.nintendoServiceAccountId =  m_nintendoServiceAccountId.empty() ? nullptr : m_nintendoServiceAccountId.data();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.nintendoServiceAccountId = m_nintendoServiceAccountId.empty() ? nullptr : m_nintendoServiceAccountId.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String m_nintendoServiceAccountId;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResultWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResult, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResultWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResultWrapper(const PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResult& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResult, Alloc>{ model },
        m_data{ model.data, model.data + model.dataCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResultWrapper(const PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResultWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResultWrapper(PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResultWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResultWrapper& operator=(PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResultWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResultWrapper& lhs, PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_data, rhs.m_data);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetData(ModelVector<PFAccountManagementNintendoServiceAccountPlayFabIdPairWrapper<Alloc>, Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
    }

    ModelVector<PFAccountManagementNintendoServiceAccountPlayFabIdPairWrapper<Alloc>, Alloc> m_data;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequestWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequestWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequest& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequest, Alloc>{ model },
        m_nintendoSwitchDeviceIds{ model.nintendoSwitchDeviceIds, model.nintendoSwitchDeviceIds + model.nintendoSwitchDeviceIdsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequestWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequestWrapper(PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequestWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequestWrapper& operator=(PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequestWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequestWrapper& lhs, PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_nintendoSwitchDeviceIds, rhs.m_nintendoSwitchDeviceIds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetNintendoSwitchDeviceIds(CStringVector<Alloc> value)
    {
        m_nintendoSwitchDeviceIds = std::move(value);
        this->m_model.nintendoSwitchDeviceIds =  m_nintendoSwitchDeviceIds.empty() ? nullptr : m_nintendoSwitchDeviceIds.data();
        this->m_model.nintendoSwitchDeviceIdsCount =  static_cast<uint32_t>(m_nintendoSwitchDeviceIds.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.nintendoSwitchDeviceIds = m_nintendoSwitchDeviceIds.empty() ? nullptr : m_nintendoSwitchDeviceIds.data();
    }

    CStringVector<Alloc> m_nintendoSwitchDeviceIds;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementNintendoSwitchPlayFabIdPairWrapper : public ModelWrapper<PFAccountManagementNintendoSwitchPlayFabIdPair, Alloc>
{
public:
    using ModelType = PFAccountManagementNintendoSwitchPlayFabIdPair;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementNintendoSwitchPlayFabIdPairWrapper() = default;

    PFAccountManagementNintendoSwitchPlayFabIdPairWrapper(const PFAccountManagementNintendoSwitchPlayFabIdPair& model) :
        ModelWrapper<PFAccountManagementNintendoSwitchPlayFabIdPair, Alloc>{ model },
        m_nintendoSwitchDeviceId{ SafeString(model.nintendoSwitchDeviceId) },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFAccountManagementNintendoSwitchPlayFabIdPairWrapper(const PFAccountManagementNintendoSwitchPlayFabIdPairWrapper& src) :
        PFAccountManagementNintendoSwitchPlayFabIdPairWrapper{ src.Model() }
    {
    }

    PFAccountManagementNintendoSwitchPlayFabIdPairWrapper(PFAccountManagementNintendoSwitchPlayFabIdPairWrapper&& src) :
        PFAccountManagementNintendoSwitchPlayFabIdPairWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementNintendoSwitchPlayFabIdPairWrapper& operator=(PFAccountManagementNintendoSwitchPlayFabIdPairWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementNintendoSwitchPlayFabIdPairWrapper() = default;

    friend void swap(PFAccountManagementNintendoSwitchPlayFabIdPairWrapper& lhs, PFAccountManagementNintendoSwitchPlayFabIdPairWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_nintendoSwitchDeviceId, rhs.m_nintendoSwitchDeviceId);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetNintendoSwitchDeviceId(String value)
    {
        m_nintendoSwitchDeviceId = std::move(value);
        this->m_model.nintendoSwitchDeviceId =  m_nintendoSwitchDeviceId.empty() ? nullptr : m_nintendoSwitchDeviceId.data();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.nintendoSwitchDeviceId = m_nintendoSwitchDeviceId.empty() ? nullptr : m_nintendoSwitchDeviceId.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String m_nintendoSwitchDeviceId;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResultWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResult, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResultWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResultWrapper(const PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResult& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResult, Alloc>{ model },
        m_data{ model.data, model.data + model.dataCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResultWrapper(const PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResultWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResultWrapper(PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResultWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResultWrapper& operator=(PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResultWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResultWrapper& lhs, PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_data, rhs.m_data);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetData(ModelVector<PFAccountManagementNintendoSwitchPlayFabIdPairWrapper<Alloc>, Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
    }

    ModelVector<PFAccountManagementNintendoSwitchPlayFabIdPairWrapper<Alloc>, Alloc> m_data;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequestWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequestWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequest& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequest, Alloc>{ model },
        m_issuerId{ model.issuerId ? std::optional<int32_t>{ *model.issuerId } : std::nullopt },
        m_PSNAccountIDs{ model.PSNAccountIDs, model.PSNAccountIDs + model.PSNAccountIDsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequestWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequestWrapper(PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequestWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequestWrapper& operator=(PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequestWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequestWrapper& lhs, PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_issuerId, rhs.m_issuerId);
        swap(lhs.m_PSNAccountIDs, rhs.m_PSNAccountIDs);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetIssuerId(std::optional<int32_t> value)
    {
        m_issuerId = std::move(value);
        this->m_model.issuerId = m_issuerId ? m_issuerId.operator->() : nullptr;
    }

    void SetPSNAccountIDs(CStringVector<Alloc> value)
    {
        m_PSNAccountIDs = std::move(value);
        this->m_model.PSNAccountIDs =  m_PSNAccountIDs.empty() ? nullptr : m_PSNAccountIDs.data();
        this->m_model.PSNAccountIDsCount =  static_cast<uint32_t>(m_PSNAccountIDs.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.issuerId = m_issuerId ? m_issuerId.operator->() : nullptr;
        this->m_model.PSNAccountIDs = m_PSNAccountIDs.empty() ? nullptr : m_PSNAccountIDs.data();
    }

    std::optional<int32_t> m_issuerId;
    CStringVector<Alloc> m_PSNAccountIDs;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementPSNAccountPlayFabIdPairWrapper : public ModelWrapper<PFAccountManagementPSNAccountPlayFabIdPair, Alloc>
{
public:
    using ModelType = PFAccountManagementPSNAccountPlayFabIdPair;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementPSNAccountPlayFabIdPairWrapper() = default;

    PFAccountManagementPSNAccountPlayFabIdPairWrapper(const PFAccountManagementPSNAccountPlayFabIdPair& model) :
        ModelWrapper<PFAccountManagementPSNAccountPlayFabIdPair, Alloc>{ model },
        m_playFabId{ SafeString(model.playFabId) },
        m_PSNAccountId{ SafeString(model.PSNAccountId) }
    {
        SetModelPointers();
    }

    PFAccountManagementPSNAccountPlayFabIdPairWrapper(const PFAccountManagementPSNAccountPlayFabIdPairWrapper& src) :
        PFAccountManagementPSNAccountPlayFabIdPairWrapper{ src.Model() }
    {
    }

    PFAccountManagementPSNAccountPlayFabIdPairWrapper(PFAccountManagementPSNAccountPlayFabIdPairWrapper&& src) :
        PFAccountManagementPSNAccountPlayFabIdPairWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementPSNAccountPlayFabIdPairWrapper& operator=(PFAccountManagementPSNAccountPlayFabIdPairWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementPSNAccountPlayFabIdPairWrapper() = default;

    friend void swap(PFAccountManagementPSNAccountPlayFabIdPairWrapper& lhs, PFAccountManagementPSNAccountPlayFabIdPairWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_PSNAccountId, rhs.m_PSNAccountId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetPSNAccountId(String value)
    {
        m_PSNAccountId = std::move(value);
        this->m_model.PSNAccountId =  m_PSNAccountId.empty() ? nullptr : m_PSNAccountId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.PSNAccountId = m_PSNAccountId.empty() ? nullptr : m_PSNAccountId.data();
    }

    String m_playFabId;
    String m_PSNAccountId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResultWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResult, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResultWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResultWrapper(const PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResult& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResult, Alloc>{ model },
        m_data{ model.data, model.data + model.dataCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResultWrapper(const PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResultWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResultWrapper(PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResultWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResultWrapper& operator=(PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResultWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResultWrapper& lhs, PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_data, rhs.m_data);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetData(ModelVector<PFAccountManagementPSNAccountPlayFabIdPairWrapper<Alloc>, Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
    }

    ModelVector<PFAccountManagementPSNAccountPlayFabIdPairWrapper<Alloc>, Alloc> m_data;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequestWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequestWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequest& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequest, Alloc>{ model },
        m_issuerId{ model.issuerId ? std::optional<int32_t>{ *model.issuerId } : std::nullopt },
        m_PSNOnlineIDs{ model.PSNOnlineIDs, model.PSNOnlineIDs + model.PSNOnlineIDsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequestWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequestWrapper(PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequestWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequestWrapper& operator=(PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequestWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequestWrapper& lhs, PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_issuerId, rhs.m_issuerId);
        swap(lhs.m_PSNOnlineIDs, rhs.m_PSNOnlineIDs);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetIssuerId(std::optional<int32_t> value)
    {
        m_issuerId = std::move(value);
        this->m_model.issuerId = m_issuerId ? m_issuerId.operator->() : nullptr;
    }

    void SetPSNOnlineIDs(CStringVector<Alloc> value)
    {
        m_PSNOnlineIDs = std::move(value);
        this->m_model.PSNOnlineIDs =  m_PSNOnlineIDs.empty() ? nullptr : m_PSNOnlineIDs.data();
        this->m_model.PSNOnlineIDsCount =  static_cast<uint32_t>(m_PSNOnlineIDs.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.issuerId = m_issuerId ? m_issuerId.operator->() : nullptr;
        this->m_model.PSNOnlineIDs = m_PSNOnlineIDs.empty() ? nullptr : m_PSNOnlineIDs.data();
    }

    std::optional<int32_t> m_issuerId;
    CStringVector<Alloc> m_PSNOnlineIDs;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementPSNOnlinePlayFabIdPairWrapper : public ModelWrapper<PFAccountManagementPSNOnlinePlayFabIdPair, Alloc>
{
public:
    using ModelType = PFAccountManagementPSNOnlinePlayFabIdPair;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementPSNOnlinePlayFabIdPairWrapper() = default;

    PFAccountManagementPSNOnlinePlayFabIdPairWrapper(const PFAccountManagementPSNOnlinePlayFabIdPair& model) :
        ModelWrapper<PFAccountManagementPSNOnlinePlayFabIdPair, Alloc>{ model },
        m_playFabId{ SafeString(model.playFabId) },
        m_PSNOnlineId{ SafeString(model.PSNOnlineId) }
    {
        SetModelPointers();
    }

    PFAccountManagementPSNOnlinePlayFabIdPairWrapper(const PFAccountManagementPSNOnlinePlayFabIdPairWrapper& src) :
        PFAccountManagementPSNOnlinePlayFabIdPairWrapper{ src.Model() }
    {
    }

    PFAccountManagementPSNOnlinePlayFabIdPairWrapper(PFAccountManagementPSNOnlinePlayFabIdPairWrapper&& src) :
        PFAccountManagementPSNOnlinePlayFabIdPairWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementPSNOnlinePlayFabIdPairWrapper& operator=(PFAccountManagementPSNOnlinePlayFabIdPairWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementPSNOnlinePlayFabIdPairWrapper() = default;

    friend void swap(PFAccountManagementPSNOnlinePlayFabIdPairWrapper& lhs, PFAccountManagementPSNOnlinePlayFabIdPairWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_PSNOnlineId, rhs.m_PSNOnlineId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetPSNOnlineId(String value)
    {
        m_PSNOnlineId = std::move(value);
        this->m_model.PSNOnlineId =  m_PSNOnlineId.empty() ? nullptr : m_PSNOnlineId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.PSNOnlineId = m_PSNOnlineId.empty() ? nullptr : m_PSNOnlineId.data();
    }

    String m_playFabId;
    String m_PSNOnlineId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResultWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResult, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResultWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResultWrapper(const PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResult& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResult, Alloc>{ model },
        m_data{ model.data, model.data + model.dataCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResultWrapper(const PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResultWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResultWrapper(PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResultWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResultWrapper& operator=(PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResultWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResultWrapper& lhs, PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_data, rhs.m_data);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetData(ModelVector<PFAccountManagementPSNOnlinePlayFabIdPairWrapper<Alloc>, Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
    }

    ModelVector<PFAccountManagementPSNOnlinePlayFabIdPairWrapper<Alloc>, Alloc> m_data;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromSteamIDsRequestWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromSteamIDsRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromSteamIDsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromSteamIDsRequestWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromSteamIDsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromSteamIDsRequest& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromSteamIDsRequest, Alloc>{ model },
        m_steamStringIDs{ model.steamStringIDs, model.steamStringIDs + model.steamStringIDsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromSteamIDsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromSteamIDsRequestWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromSteamIDsRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromSteamIDsRequestWrapper(PFAccountManagementGetPlayFabIDsFromSteamIDsRequestWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromSteamIDsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromSteamIDsRequestWrapper& operator=(PFAccountManagementGetPlayFabIDsFromSteamIDsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromSteamIDsRequestWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromSteamIDsRequestWrapper& lhs, PFAccountManagementGetPlayFabIDsFromSteamIDsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_steamStringIDs, rhs.m_steamStringIDs);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetSteamStringIDs(CStringVector<Alloc> value)
    {
        m_steamStringIDs = std::move(value);
        this->m_model.steamStringIDs =  m_steamStringIDs.empty() ? nullptr : m_steamStringIDs.data();
        this->m_model.steamStringIDsCount =  static_cast<uint32_t>(m_steamStringIDs.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.steamStringIDs = m_steamStringIDs.empty() ? nullptr : m_steamStringIDs.data();
    }

    CStringVector<Alloc> m_steamStringIDs;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementSteamPlayFabIdPairWrapper : public ModelWrapper<PFAccountManagementSteamPlayFabIdPair, Alloc>
{
public:
    using ModelType = PFAccountManagementSteamPlayFabIdPair;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementSteamPlayFabIdPairWrapper() = default;

    PFAccountManagementSteamPlayFabIdPairWrapper(const PFAccountManagementSteamPlayFabIdPair& model) :
        ModelWrapper<PFAccountManagementSteamPlayFabIdPair, Alloc>{ model },
        m_playFabId{ SafeString(model.playFabId) },
        m_steamStringId{ SafeString(model.steamStringId) }
    {
        SetModelPointers();
    }

    PFAccountManagementSteamPlayFabIdPairWrapper(const PFAccountManagementSteamPlayFabIdPairWrapper& src) :
        PFAccountManagementSteamPlayFabIdPairWrapper{ src.Model() }
    {
    }

    PFAccountManagementSteamPlayFabIdPairWrapper(PFAccountManagementSteamPlayFabIdPairWrapper&& src) :
        PFAccountManagementSteamPlayFabIdPairWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementSteamPlayFabIdPairWrapper& operator=(PFAccountManagementSteamPlayFabIdPairWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementSteamPlayFabIdPairWrapper() = default;

    friend void swap(PFAccountManagementSteamPlayFabIdPairWrapper& lhs, PFAccountManagementSteamPlayFabIdPairWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_steamStringId, rhs.m_steamStringId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetSteamStringId(String value)
    {
        m_steamStringId = std::move(value);
        this->m_model.steamStringId =  m_steamStringId.empty() ? nullptr : m_steamStringId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.steamStringId = m_steamStringId.empty() ? nullptr : m_steamStringId.data();
    }

    String m_playFabId;
    String m_steamStringId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromSteamIDsResultWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromSteamIDsResult, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromSteamIDsResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromSteamIDsResultWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromSteamIDsResultWrapper(const PFAccountManagementGetPlayFabIDsFromSteamIDsResult& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromSteamIDsResult, Alloc>{ model },
        m_data{ model.data, model.data + model.dataCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromSteamIDsResultWrapper(const PFAccountManagementGetPlayFabIDsFromSteamIDsResultWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromSteamIDsResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromSteamIDsResultWrapper(PFAccountManagementGetPlayFabIDsFromSteamIDsResultWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromSteamIDsResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromSteamIDsResultWrapper& operator=(PFAccountManagementGetPlayFabIDsFromSteamIDsResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromSteamIDsResultWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromSteamIDsResultWrapper& lhs, PFAccountManagementGetPlayFabIDsFromSteamIDsResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_data, rhs.m_data);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetData(ModelVector<PFAccountManagementSteamPlayFabIdPairWrapper<Alloc>, Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
    }

    ModelVector<PFAccountManagementSteamPlayFabIdPairWrapper<Alloc>, Alloc> m_data;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromTwitchIDsRequestWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromTwitchIDsRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromTwitchIDsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromTwitchIDsRequestWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromTwitchIDsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromTwitchIDsRequest& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromTwitchIDsRequest, Alloc>{ model },
        m_twitchIds{ model.twitchIds, model.twitchIds + model.twitchIdsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromTwitchIDsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromTwitchIDsRequestWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromTwitchIDsRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromTwitchIDsRequestWrapper(PFAccountManagementGetPlayFabIDsFromTwitchIDsRequestWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromTwitchIDsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromTwitchIDsRequestWrapper& operator=(PFAccountManagementGetPlayFabIDsFromTwitchIDsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromTwitchIDsRequestWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromTwitchIDsRequestWrapper& lhs, PFAccountManagementGetPlayFabIDsFromTwitchIDsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_twitchIds, rhs.m_twitchIds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetTwitchIds(CStringVector<Alloc> value)
    {
        m_twitchIds = std::move(value);
        this->m_model.twitchIds =  m_twitchIds.empty() ? nullptr : m_twitchIds.data();
        this->m_model.twitchIdsCount =  static_cast<uint32_t>(m_twitchIds.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.twitchIds = m_twitchIds.empty() ? nullptr : m_twitchIds.data();
    }

    CStringVector<Alloc> m_twitchIds;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementTwitchPlayFabIdPairWrapper : public ModelWrapper<PFAccountManagementTwitchPlayFabIdPair, Alloc>
{
public:
    using ModelType = PFAccountManagementTwitchPlayFabIdPair;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementTwitchPlayFabIdPairWrapper() = default;

    PFAccountManagementTwitchPlayFabIdPairWrapper(const PFAccountManagementTwitchPlayFabIdPair& model) :
        ModelWrapper<PFAccountManagementTwitchPlayFabIdPair, Alloc>{ model },
        m_playFabId{ SafeString(model.playFabId) },
        m_twitchId{ SafeString(model.twitchId) }
    {
        SetModelPointers();
    }

    PFAccountManagementTwitchPlayFabIdPairWrapper(const PFAccountManagementTwitchPlayFabIdPairWrapper& src) :
        PFAccountManagementTwitchPlayFabIdPairWrapper{ src.Model() }
    {
    }

    PFAccountManagementTwitchPlayFabIdPairWrapper(PFAccountManagementTwitchPlayFabIdPairWrapper&& src) :
        PFAccountManagementTwitchPlayFabIdPairWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementTwitchPlayFabIdPairWrapper& operator=(PFAccountManagementTwitchPlayFabIdPairWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementTwitchPlayFabIdPairWrapper() = default;

    friend void swap(PFAccountManagementTwitchPlayFabIdPairWrapper& lhs, PFAccountManagementTwitchPlayFabIdPairWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_twitchId, rhs.m_twitchId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetTwitchId(String value)
    {
        m_twitchId = std::move(value);
        this->m_model.twitchId =  m_twitchId.empty() ? nullptr : m_twitchId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.twitchId = m_twitchId.empty() ? nullptr : m_twitchId.data();
    }

    String m_playFabId;
    String m_twitchId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromTwitchIDsResultWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromTwitchIDsResult, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromTwitchIDsResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromTwitchIDsResultWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromTwitchIDsResultWrapper(const PFAccountManagementGetPlayFabIDsFromTwitchIDsResult& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromTwitchIDsResult, Alloc>{ model },
        m_data{ model.data, model.data + model.dataCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromTwitchIDsResultWrapper(const PFAccountManagementGetPlayFabIDsFromTwitchIDsResultWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromTwitchIDsResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromTwitchIDsResultWrapper(PFAccountManagementGetPlayFabIDsFromTwitchIDsResultWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromTwitchIDsResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromTwitchIDsResultWrapper& operator=(PFAccountManagementGetPlayFabIDsFromTwitchIDsResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromTwitchIDsResultWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromTwitchIDsResultWrapper& lhs, PFAccountManagementGetPlayFabIDsFromTwitchIDsResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_data, rhs.m_data);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetData(ModelVector<PFAccountManagementTwitchPlayFabIdPairWrapper<Alloc>, Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
    }

    ModelVector<PFAccountManagementTwitchPlayFabIdPairWrapper<Alloc>, Alloc> m_data;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequestWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequestWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequest& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequest, Alloc>{ model },
        m_sandbox{ SafeString(model.sandbox) },
        m_xboxLiveAccountIDs{ model.xboxLiveAccountIDs, model.xboxLiveAccountIDs + model.xboxLiveAccountIDsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequestWrapper(const PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequestWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequestWrapper(PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequestWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequestWrapper& operator=(PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequestWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequestWrapper& lhs, PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_sandbox, rhs.m_sandbox);
        swap(lhs.m_xboxLiveAccountIDs, rhs.m_xboxLiveAccountIDs);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetSandbox(String value)
    {
        m_sandbox = std::move(value);
        this->m_model.sandbox =  m_sandbox.empty() ? nullptr : m_sandbox.data();
    }

    void SetXboxLiveAccountIDs(CStringVector<Alloc> value)
    {
        m_xboxLiveAccountIDs = std::move(value);
        this->m_model.xboxLiveAccountIDs =  m_xboxLiveAccountIDs.empty() ? nullptr : m_xboxLiveAccountIDs.data();
        this->m_model.xboxLiveAccountIDsCount =  static_cast<uint32_t>(m_xboxLiveAccountIDs.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.sandbox = m_sandbox.empty() ? nullptr : m_sandbox.data();
        this->m_model.xboxLiveAccountIDs = m_xboxLiveAccountIDs.empty() ? nullptr : m_xboxLiveAccountIDs.data();
    }

    String m_sandbox;
    CStringVector<Alloc> m_xboxLiveAccountIDs;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementXboxLiveAccountPlayFabIdPairWrapper : public ModelWrapper<PFAccountManagementXboxLiveAccountPlayFabIdPair, Alloc>
{
public:
    using ModelType = PFAccountManagementXboxLiveAccountPlayFabIdPair;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementXboxLiveAccountPlayFabIdPairWrapper() = default;

    PFAccountManagementXboxLiveAccountPlayFabIdPairWrapper(const PFAccountManagementXboxLiveAccountPlayFabIdPair& model) :
        ModelWrapper<PFAccountManagementXboxLiveAccountPlayFabIdPair, Alloc>{ model },
        m_playFabId{ SafeString(model.playFabId) },
        m_xboxLiveAccountId{ SafeString(model.xboxLiveAccountId) }
    {
        SetModelPointers();
    }

    PFAccountManagementXboxLiveAccountPlayFabIdPairWrapper(const PFAccountManagementXboxLiveAccountPlayFabIdPairWrapper& src) :
        PFAccountManagementXboxLiveAccountPlayFabIdPairWrapper{ src.Model() }
    {
    }

    PFAccountManagementXboxLiveAccountPlayFabIdPairWrapper(PFAccountManagementXboxLiveAccountPlayFabIdPairWrapper&& src) :
        PFAccountManagementXboxLiveAccountPlayFabIdPairWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementXboxLiveAccountPlayFabIdPairWrapper& operator=(PFAccountManagementXboxLiveAccountPlayFabIdPairWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementXboxLiveAccountPlayFabIdPairWrapper() = default;

    friend void swap(PFAccountManagementXboxLiveAccountPlayFabIdPairWrapper& lhs, PFAccountManagementXboxLiveAccountPlayFabIdPairWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_xboxLiveAccountId, rhs.m_xboxLiveAccountId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetXboxLiveAccountId(String value)
    {
        m_xboxLiveAccountId = std::move(value);
        this->m_model.xboxLiveAccountId =  m_xboxLiveAccountId.empty() ? nullptr : m_xboxLiveAccountId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.xboxLiveAccountId = m_xboxLiveAccountId.empty() ? nullptr : m_xboxLiveAccountId.data();
    }

    String m_playFabId;
    String m_xboxLiveAccountId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResultWrapper : public ModelWrapper<PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResult, Alloc>
{
public:
    using ModelType = PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResultWrapper() = default;

    PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResultWrapper(const PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResult& model) :
        ModelWrapper<PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResult, Alloc>{ model },
        m_data{ model.data, model.data + model.dataCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResultWrapper(const PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResultWrapper& src) :
        PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResultWrapper(PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResultWrapper&& src) :
        PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResultWrapper& operator=(PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResultWrapper() = default;

    friend void swap(PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResultWrapper& lhs, PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_data, rhs.m_data);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetData(ModelVector<PFAccountManagementXboxLiveAccountPlayFabIdPairWrapper<Alloc>, Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
    }

    ModelVector<PFAccountManagementXboxLiveAccountPlayFabIdPairWrapper<Alloc>, Alloc> m_data;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementLinkAndroidDeviceIDRequestWrapper : public ModelWrapper<PFAccountManagementLinkAndroidDeviceIDRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementLinkAndroidDeviceIDRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementLinkAndroidDeviceIDRequestWrapper() = default;

    PFAccountManagementLinkAndroidDeviceIDRequestWrapper(const PFAccountManagementLinkAndroidDeviceIDRequest& model) :
        ModelWrapper<PFAccountManagementLinkAndroidDeviceIDRequest, Alloc>{ model },
        m_androidDevice{ SafeString(model.androidDevice) },
        m_androidDeviceId{ SafeString(model.androidDeviceId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt },
        m_OS{ SafeString(model.OS) }
    {
        SetModelPointers();
    }

    PFAccountManagementLinkAndroidDeviceIDRequestWrapper(const PFAccountManagementLinkAndroidDeviceIDRequestWrapper& src) :
        PFAccountManagementLinkAndroidDeviceIDRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementLinkAndroidDeviceIDRequestWrapper(PFAccountManagementLinkAndroidDeviceIDRequestWrapper&& src) :
        PFAccountManagementLinkAndroidDeviceIDRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementLinkAndroidDeviceIDRequestWrapper& operator=(PFAccountManagementLinkAndroidDeviceIDRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementLinkAndroidDeviceIDRequestWrapper() = default;

    friend void swap(PFAccountManagementLinkAndroidDeviceIDRequestWrapper& lhs, PFAccountManagementLinkAndroidDeviceIDRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_androidDevice, rhs.m_androidDevice);
        swap(lhs.m_androidDeviceId, rhs.m_androidDeviceId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        swap(lhs.m_OS, rhs.m_OS);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAndroidDevice(String value)
    {
        m_androidDevice = std::move(value);
        this->m_model.androidDevice =  m_androidDevice.empty() ? nullptr : m_androidDevice.data();
    }

    void SetAndroidDeviceId(String value)
    {
        m_androidDeviceId = std::move(value);
        this->m_model.androidDeviceId =  m_androidDeviceId.empty() ? nullptr : m_androidDeviceId.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    void SetOS(String value)
    {
        m_OS = std::move(value);
        this->m_model.OS =  m_OS.empty() ? nullptr : m_OS.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.androidDevice = m_androidDevice.empty() ? nullptr : m_androidDevice.data();
        this->m_model.androidDeviceId = m_androidDeviceId.empty() ? nullptr : m_androidDeviceId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
        this->m_model.OS = m_OS.empty() ? nullptr : m_OS.data();
    }

    String m_androidDevice;
    String m_androidDeviceId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceLink;
    String m_OS;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementLinkAppleRequestWrapper : public ModelWrapper<PFAccountManagementLinkAppleRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementLinkAppleRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementLinkAppleRequestWrapper() = default;

    PFAccountManagementLinkAppleRequestWrapper(const PFAccountManagementLinkAppleRequest& model) :
        ModelWrapper<PFAccountManagementLinkAppleRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt },
        m_identityToken{ SafeString(model.identityToken) }
    {
        SetModelPointers();
    }

    PFAccountManagementLinkAppleRequestWrapper(const PFAccountManagementLinkAppleRequestWrapper& src) :
        PFAccountManagementLinkAppleRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementLinkAppleRequestWrapper(PFAccountManagementLinkAppleRequestWrapper&& src) :
        PFAccountManagementLinkAppleRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementLinkAppleRequestWrapper& operator=(PFAccountManagementLinkAppleRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementLinkAppleRequestWrapper() = default;

    friend void swap(PFAccountManagementLinkAppleRequestWrapper& lhs, PFAccountManagementLinkAppleRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        swap(lhs.m_identityToken, rhs.m_identityToken);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    void SetIdentityToken(String value)
    {
        m_identityToken = std::move(value);
        this->m_model.identityToken =  m_identityToken.empty() ? nullptr : m_identityToken.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
        this->m_model.identityToken = m_identityToken.empty() ? nullptr : m_identityToken.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceLink;
    String m_identityToken;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementLinkCustomIDRequestWrapper : public ModelWrapper<PFAccountManagementLinkCustomIDRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementLinkCustomIDRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementLinkCustomIDRequestWrapper() = default;

    PFAccountManagementLinkCustomIDRequestWrapper(const PFAccountManagementLinkCustomIDRequest& model) :
        ModelWrapper<PFAccountManagementLinkCustomIDRequest, Alloc>{ model },
        m_customId{ SafeString(model.customId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt }
    {
        SetModelPointers();
    }

    PFAccountManagementLinkCustomIDRequestWrapper(const PFAccountManagementLinkCustomIDRequestWrapper& src) :
        PFAccountManagementLinkCustomIDRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementLinkCustomIDRequestWrapper(PFAccountManagementLinkCustomIDRequestWrapper&& src) :
        PFAccountManagementLinkCustomIDRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementLinkCustomIDRequestWrapper& operator=(PFAccountManagementLinkCustomIDRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementLinkCustomIDRequestWrapper() = default;

    friend void swap(PFAccountManagementLinkCustomIDRequestWrapper& lhs, PFAccountManagementLinkCustomIDRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customId, rhs.m_customId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomId(String value)
    {
        m_customId = std::move(value);
        this->m_model.customId =  m_customId.empty() ? nullptr : m_customId.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customId = m_customId.empty() ? nullptr : m_customId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    String m_customId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceLink;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementLinkFacebookAccountRequestWrapper : public ModelWrapper<PFAccountManagementLinkFacebookAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementLinkFacebookAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementLinkFacebookAccountRequestWrapper() = default;

    PFAccountManagementLinkFacebookAccountRequestWrapper(const PFAccountManagementLinkFacebookAccountRequest& model) :
        ModelWrapper<PFAccountManagementLinkFacebookAccountRequest, Alloc>{ model },
        m_accessToken{ SafeString(model.accessToken) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt }
    {
        SetModelPointers();
    }

    PFAccountManagementLinkFacebookAccountRequestWrapper(const PFAccountManagementLinkFacebookAccountRequestWrapper& src) :
        PFAccountManagementLinkFacebookAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementLinkFacebookAccountRequestWrapper(PFAccountManagementLinkFacebookAccountRequestWrapper&& src) :
        PFAccountManagementLinkFacebookAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementLinkFacebookAccountRequestWrapper& operator=(PFAccountManagementLinkFacebookAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementLinkFacebookAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementLinkFacebookAccountRequestWrapper& lhs, PFAccountManagementLinkFacebookAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_accessToken, rhs.m_accessToken);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAccessToken(String value)
    {
        m_accessToken = std::move(value);
        this->m_model.accessToken =  m_accessToken.empty() ? nullptr : m_accessToken.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.accessToken = m_accessToken.empty() ? nullptr : m_accessToken.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    String m_accessToken;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceLink;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementLinkFacebookInstantGamesIdRequestWrapper : public ModelWrapper<PFAccountManagementLinkFacebookInstantGamesIdRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementLinkFacebookInstantGamesIdRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementLinkFacebookInstantGamesIdRequestWrapper() = default;

    PFAccountManagementLinkFacebookInstantGamesIdRequestWrapper(const PFAccountManagementLinkFacebookInstantGamesIdRequest& model) :
        ModelWrapper<PFAccountManagementLinkFacebookInstantGamesIdRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_facebookInstantGamesSignature{ SafeString(model.facebookInstantGamesSignature) },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt }
    {
        SetModelPointers();
    }

    PFAccountManagementLinkFacebookInstantGamesIdRequestWrapper(const PFAccountManagementLinkFacebookInstantGamesIdRequestWrapper& src) :
        PFAccountManagementLinkFacebookInstantGamesIdRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementLinkFacebookInstantGamesIdRequestWrapper(PFAccountManagementLinkFacebookInstantGamesIdRequestWrapper&& src) :
        PFAccountManagementLinkFacebookInstantGamesIdRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementLinkFacebookInstantGamesIdRequestWrapper& operator=(PFAccountManagementLinkFacebookInstantGamesIdRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementLinkFacebookInstantGamesIdRequestWrapper() = default;

    friend void swap(PFAccountManagementLinkFacebookInstantGamesIdRequestWrapper& lhs, PFAccountManagementLinkFacebookInstantGamesIdRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_facebookInstantGamesSignature, rhs.m_facebookInstantGamesSignature);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetFacebookInstantGamesSignature(String value)
    {
        m_facebookInstantGamesSignature = std::move(value);
        this->m_model.facebookInstantGamesSignature =  m_facebookInstantGamesSignature.empty() ? nullptr : m_facebookInstantGamesSignature.data();
    }

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.facebookInstantGamesSignature = m_facebookInstantGamesSignature.empty() ? nullptr : m_facebookInstantGamesSignature.data();
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_facebookInstantGamesSignature;
    std::optional<bool> m_forceLink;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementLinkGameCenterAccountRequestWrapper : public ModelWrapper<PFAccountManagementLinkGameCenterAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementLinkGameCenterAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementLinkGameCenterAccountRequestWrapper() = default;

    PFAccountManagementLinkGameCenterAccountRequestWrapper(const PFAccountManagementLinkGameCenterAccountRequest& model) :
        ModelWrapper<PFAccountManagementLinkGameCenterAccountRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt },
        m_gameCenterId{ SafeString(model.gameCenterId) },
        m_publicKeyUrl{ SafeString(model.publicKeyUrl) },
        m_salt{ SafeString(model.salt) },
        m_signature{ SafeString(model.signature) },
        m_timestamp{ SafeString(model.timestamp) }
    {
        SetModelPointers();
    }

    PFAccountManagementLinkGameCenterAccountRequestWrapper(const PFAccountManagementLinkGameCenterAccountRequestWrapper& src) :
        PFAccountManagementLinkGameCenterAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementLinkGameCenterAccountRequestWrapper(PFAccountManagementLinkGameCenterAccountRequestWrapper&& src) :
        PFAccountManagementLinkGameCenterAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementLinkGameCenterAccountRequestWrapper& operator=(PFAccountManagementLinkGameCenterAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementLinkGameCenterAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementLinkGameCenterAccountRequestWrapper& lhs, PFAccountManagementLinkGameCenterAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        swap(lhs.m_gameCenterId, rhs.m_gameCenterId);
        swap(lhs.m_publicKeyUrl, rhs.m_publicKeyUrl);
        swap(lhs.m_salt, rhs.m_salt);
        swap(lhs.m_signature, rhs.m_signature);
        swap(lhs.m_timestamp, rhs.m_timestamp);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    void SetGameCenterId(String value)
    {
        m_gameCenterId = std::move(value);
        this->m_model.gameCenterId =  m_gameCenterId.empty() ? nullptr : m_gameCenterId.data();
    }

    void SetPublicKeyUrl(String value)
    {
        m_publicKeyUrl = std::move(value);
        this->m_model.publicKeyUrl =  m_publicKeyUrl.empty() ? nullptr : m_publicKeyUrl.data();
    }

    void SetSalt(String value)
    {
        m_salt = std::move(value);
        this->m_model.salt =  m_salt.empty() ? nullptr : m_salt.data();
    }

    void SetSignature(String value)
    {
        m_signature = std::move(value);
        this->m_model.signature =  m_signature.empty() ? nullptr : m_signature.data();
    }

    void SetTimestamp(String value)
    {
        m_timestamp = std::move(value);
        this->m_model.timestamp =  m_timestamp.empty() ? nullptr : m_timestamp.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
        this->m_model.gameCenterId = m_gameCenterId.empty() ? nullptr : m_gameCenterId.data();
        this->m_model.publicKeyUrl = m_publicKeyUrl.empty() ? nullptr : m_publicKeyUrl.data();
        this->m_model.salt = m_salt.empty() ? nullptr : m_salt.data();
        this->m_model.signature = m_signature.empty() ? nullptr : m_signature.data();
        this->m_model.timestamp = m_timestamp.empty() ? nullptr : m_timestamp.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceLink;
    String m_gameCenterId;
    String m_publicKeyUrl;
    String m_salt;
    String m_signature;
    String m_timestamp;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementLinkGoogleAccountRequestWrapper : public ModelWrapper<PFAccountManagementLinkGoogleAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementLinkGoogleAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementLinkGoogleAccountRequestWrapper() = default;

    PFAccountManagementLinkGoogleAccountRequestWrapper(const PFAccountManagementLinkGoogleAccountRequest& model) :
        ModelWrapper<PFAccountManagementLinkGoogleAccountRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt },
        m_serverAuthCode{ SafeString(model.serverAuthCode) }
    {
        SetModelPointers();
    }

    PFAccountManagementLinkGoogleAccountRequestWrapper(const PFAccountManagementLinkGoogleAccountRequestWrapper& src) :
        PFAccountManagementLinkGoogleAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementLinkGoogleAccountRequestWrapper(PFAccountManagementLinkGoogleAccountRequestWrapper&& src) :
        PFAccountManagementLinkGoogleAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementLinkGoogleAccountRequestWrapper& operator=(PFAccountManagementLinkGoogleAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementLinkGoogleAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementLinkGoogleAccountRequestWrapper& lhs, PFAccountManagementLinkGoogleAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        swap(lhs.m_serverAuthCode, rhs.m_serverAuthCode);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    void SetServerAuthCode(String value)
    {
        m_serverAuthCode = std::move(value);
        this->m_model.serverAuthCode =  m_serverAuthCode.empty() ? nullptr : m_serverAuthCode.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
        this->m_model.serverAuthCode = m_serverAuthCode.empty() ? nullptr : m_serverAuthCode.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceLink;
    String m_serverAuthCode;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementLinkGooglePlayGamesServicesAccountRequestWrapper : public ModelWrapper<PFAccountManagementLinkGooglePlayGamesServicesAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementLinkGooglePlayGamesServicesAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementLinkGooglePlayGamesServicesAccountRequestWrapper() = default;

    PFAccountManagementLinkGooglePlayGamesServicesAccountRequestWrapper(const PFAccountManagementLinkGooglePlayGamesServicesAccountRequest& model) :
        ModelWrapper<PFAccountManagementLinkGooglePlayGamesServicesAccountRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt },
        m_serverAuthCode{ SafeString(model.serverAuthCode) }
    {
        SetModelPointers();
    }

    PFAccountManagementLinkGooglePlayGamesServicesAccountRequestWrapper(const PFAccountManagementLinkGooglePlayGamesServicesAccountRequestWrapper& src) :
        PFAccountManagementLinkGooglePlayGamesServicesAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementLinkGooglePlayGamesServicesAccountRequestWrapper(PFAccountManagementLinkGooglePlayGamesServicesAccountRequestWrapper&& src) :
        PFAccountManagementLinkGooglePlayGamesServicesAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementLinkGooglePlayGamesServicesAccountRequestWrapper& operator=(PFAccountManagementLinkGooglePlayGamesServicesAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementLinkGooglePlayGamesServicesAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementLinkGooglePlayGamesServicesAccountRequestWrapper& lhs, PFAccountManagementLinkGooglePlayGamesServicesAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        swap(lhs.m_serverAuthCode, rhs.m_serverAuthCode);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    void SetServerAuthCode(String value)
    {
        m_serverAuthCode = std::move(value);
        this->m_model.serverAuthCode =  m_serverAuthCode.empty() ? nullptr : m_serverAuthCode.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
        this->m_model.serverAuthCode = m_serverAuthCode.empty() ? nullptr : m_serverAuthCode.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceLink;
    String m_serverAuthCode;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementLinkIOSDeviceIDRequestWrapper : public ModelWrapper<PFAccountManagementLinkIOSDeviceIDRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementLinkIOSDeviceIDRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementLinkIOSDeviceIDRequestWrapper() = default;

    PFAccountManagementLinkIOSDeviceIDRequestWrapper(const PFAccountManagementLinkIOSDeviceIDRequest& model) :
        ModelWrapper<PFAccountManagementLinkIOSDeviceIDRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_deviceId{ SafeString(model.deviceId) },
        m_deviceModel{ SafeString(model.deviceModel) },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt },
        m_OS{ SafeString(model.OS) }
    {
        SetModelPointers();
    }

    PFAccountManagementLinkIOSDeviceIDRequestWrapper(const PFAccountManagementLinkIOSDeviceIDRequestWrapper& src) :
        PFAccountManagementLinkIOSDeviceIDRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementLinkIOSDeviceIDRequestWrapper(PFAccountManagementLinkIOSDeviceIDRequestWrapper&& src) :
        PFAccountManagementLinkIOSDeviceIDRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementLinkIOSDeviceIDRequestWrapper& operator=(PFAccountManagementLinkIOSDeviceIDRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementLinkIOSDeviceIDRequestWrapper() = default;

    friend void swap(PFAccountManagementLinkIOSDeviceIDRequestWrapper& lhs, PFAccountManagementLinkIOSDeviceIDRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_deviceId, rhs.m_deviceId);
        swap(lhs.m_deviceModel, rhs.m_deviceModel);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        swap(lhs.m_OS, rhs.m_OS);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetDeviceId(String value)
    {
        m_deviceId = std::move(value);
        this->m_model.deviceId =  m_deviceId.empty() ? nullptr : m_deviceId.data();
    }

    void SetDeviceModel(String value)
    {
        m_deviceModel = std::move(value);
        this->m_model.deviceModel =  m_deviceModel.empty() ? nullptr : m_deviceModel.data();
    }

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    void SetOS(String value)
    {
        m_OS = std::move(value);
        this->m_model.OS =  m_OS.empty() ? nullptr : m_OS.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.deviceId = m_deviceId.empty() ? nullptr : m_deviceId.data();
        this->m_model.deviceModel = m_deviceModel.empty() ? nullptr : m_deviceModel.data();
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
        this->m_model.OS = m_OS.empty() ? nullptr : m_OS.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_deviceId;
    String m_deviceModel;
    std::optional<bool> m_forceLink;
    String m_OS;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementLinkKongregateAccountRequestWrapper : public ModelWrapper<PFAccountManagementLinkKongregateAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementLinkKongregateAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementLinkKongregateAccountRequestWrapper() = default;

    PFAccountManagementLinkKongregateAccountRequestWrapper(const PFAccountManagementLinkKongregateAccountRequest& model) :
        ModelWrapper<PFAccountManagementLinkKongregateAccountRequest, Alloc>{ model },
        m_authTicket{ SafeString(model.authTicket) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt },
        m_kongregateId{ SafeString(model.kongregateId) }
    {
        SetModelPointers();
    }

    PFAccountManagementLinkKongregateAccountRequestWrapper(const PFAccountManagementLinkKongregateAccountRequestWrapper& src) :
        PFAccountManagementLinkKongregateAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementLinkKongregateAccountRequestWrapper(PFAccountManagementLinkKongregateAccountRequestWrapper&& src) :
        PFAccountManagementLinkKongregateAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementLinkKongregateAccountRequestWrapper& operator=(PFAccountManagementLinkKongregateAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementLinkKongregateAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementLinkKongregateAccountRequestWrapper& lhs, PFAccountManagementLinkKongregateAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_authTicket, rhs.m_authTicket);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        swap(lhs.m_kongregateId, rhs.m_kongregateId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAuthTicket(String value)
    {
        m_authTicket = std::move(value);
        this->m_model.authTicket =  m_authTicket.empty() ? nullptr : m_authTicket.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    void SetKongregateId(String value)
    {
        m_kongregateId = std::move(value);
        this->m_model.kongregateId =  m_kongregateId.empty() ? nullptr : m_kongregateId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.authTicket = m_authTicket.empty() ? nullptr : m_authTicket.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
        this->m_model.kongregateId = m_kongregateId.empty() ? nullptr : m_kongregateId.data();
    }

    String m_authTicket;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceLink;
    String m_kongregateId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementClientLinkNintendoServiceAccountRequestWrapper : public ModelWrapper<PFAccountManagementClientLinkNintendoServiceAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementClientLinkNintendoServiceAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementClientLinkNintendoServiceAccountRequestWrapper() = default;

    PFAccountManagementClientLinkNintendoServiceAccountRequestWrapper(const PFAccountManagementClientLinkNintendoServiceAccountRequest& model) :
        ModelWrapper<PFAccountManagementClientLinkNintendoServiceAccountRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt },
        m_identityToken{ SafeString(model.identityToken) }
    {
        SetModelPointers();
    }

    PFAccountManagementClientLinkNintendoServiceAccountRequestWrapper(const PFAccountManagementClientLinkNintendoServiceAccountRequestWrapper& src) :
        PFAccountManagementClientLinkNintendoServiceAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementClientLinkNintendoServiceAccountRequestWrapper(PFAccountManagementClientLinkNintendoServiceAccountRequestWrapper&& src) :
        PFAccountManagementClientLinkNintendoServiceAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementClientLinkNintendoServiceAccountRequestWrapper& operator=(PFAccountManagementClientLinkNintendoServiceAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementClientLinkNintendoServiceAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementClientLinkNintendoServiceAccountRequestWrapper& lhs, PFAccountManagementClientLinkNintendoServiceAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        swap(lhs.m_identityToken, rhs.m_identityToken);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    void SetIdentityToken(String value)
    {
        m_identityToken = std::move(value);
        this->m_model.identityToken =  m_identityToken.empty() ? nullptr : m_identityToken.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
        this->m_model.identityToken = m_identityToken.empty() ? nullptr : m_identityToken.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceLink;
    String m_identityToken;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementClientLinkNintendoSwitchDeviceIdRequestWrapper : public ModelWrapper<PFAccountManagementClientLinkNintendoSwitchDeviceIdRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementClientLinkNintendoSwitchDeviceIdRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementClientLinkNintendoSwitchDeviceIdRequestWrapper() = default;

    PFAccountManagementClientLinkNintendoSwitchDeviceIdRequestWrapper(const PFAccountManagementClientLinkNintendoSwitchDeviceIdRequest& model) :
        ModelWrapper<PFAccountManagementClientLinkNintendoSwitchDeviceIdRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt },
        m_nintendoSwitchDeviceId{ SafeString(model.nintendoSwitchDeviceId) }
    {
        SetModelPointers();
    }

    PFAccountManagementClientLinkNintendoSwitchDeviceIdRequestWrapper(const PFAccountManagementClientLinkNintendoSwitchDeviceIdRequestWrapper& src) :
        PFAccountManagementClientLinkNintendoSwitchDeviceIdRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementClientLinkNintendoSwitchDeviceIdRequestWrapper(PFAccountManagementClientLinkNintendoSwitchDeviceIdRequestWrapper&& src) :
        PFAccountManagementClientLinkNintendoSwitchDeviceIdRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementClientLinkNintendoSwitchDeviceIdRequestWrapper& operator=(PFAccountManagementClientLinkNintendoSwitchDeviceIdRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementClientLinkNintendoSwitchDeviceIdRequestWrapper() = default;

    friend void swap(PFAccountManagementClientLinkNintendoSwitchDeviceIdRequestWrapper& lhs, PFAccountManagementClientLinkNintendoSwitchDeviceIdRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        swap(lhs.m_nintendoSwitchDeviceId, rhs.m_nintendoSwitchDeviceId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    void SetNintendoSwitchDeviceId(String value)
    {
        m_nintendoSwitchDeviceId = std::move(value);
        this->m_model.nintendoSwitchDeviceId =  m_nintendoSwitchDeviceId.empty() ? nullptr : m_nintendoSwitchDeviceId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
        this->m_model.nintendoSwitchDeviceId = m_nintendoSwitchDeviceId.empty() ? nullptr : m_nintendoSwitchDeviceId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceLink;
    String m_nintendoSwitchDeviceId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementLinkOpenIdConnectRequestWrapper : public ModelWrapper<PFAccountManagementLinkOpenIdConnectRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementLinkOpenIdConnectRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementLinkOpenIdConnectRequestWrapper() = default;

    PFAccountManagementLinkOpenIdConnectRequestWrapper(const PFAccountManagementLinkOpenIdConnectRequest& model) :
        ModelWrapper<PFAccountManagementLinkOpenIdConnectRequest, Alloc>{ model },
        m_connectionId{ SafeString(model.connectionId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt },
        m_idToken{ SafeString(model.idToken) }
    {
        SetModelPointers();
    }

    PFAccountManagementLinkOpenIdConnectRequestWrapper(const PFAccountManagementLinkOpenIdConnectRequestWrapper& src) :
        PFAccountManagementLinkOpenIdConnectRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementLinkOpenIdConnectRequestWrapper(PFAccountManagementLinkOpenIdConnectRequestWrapper&& src) :
        PFAccountManagementLinkOpenIdConnectRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementLinkOpenIdConnectRequestWrapper& operator=(PFAccountManagementLinkOpenIdConnectRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementLinkOpenIdConnectRequestWrapper() = default;

    friend void swap(PFAccountManagementLinkOpenIdConnectRequestWrapper& lhs, PFAccountManagementLinkOpenIdConnectRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_connectionId, rhs.m_connectionId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        swap(lhs.m_idToken, rhs.m_idToken);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetConnectionId(String value)
    {
        m_connectionId = std::move(value);
        this->m_model.connectionId =  m_connectionId.empty() ? nullptr : m_connectionId.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    void SetIdToken(String value)
    {
        m_idToken = std::move(value);
        this->m_model.idToken =  m_idToken.empty() ? nullptr : m_idToken.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.connectionId = m_connectionId.empty() ? nullptr : m_connectionId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
        this->m_model.idToken = m_idToken.empty() ? nullptr : m_idToken.data();
    }

    String m_connectionId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceLink;
    String m_idToken;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementClientLinkPSNAccountRequestWrapper : public ModelWrapper<PFAccountManagementClientLinkPSNAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementClientLinkPSNAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementClientLinkPSNAccountRequestWrapper() = default;

    PFAccountManagementClientLinkPSNAccountRequestWrapper(const PFAccountManagementClientLinkPSNAccountRequest& model) :
        ModelWrapper<PFAccountManagementClientLinkPSNAccountRequest, Alloc>{ model },
        m_authCode{ SafeString(model.authCode) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt },
        m_issuerId{ model.issuerId ? std::optional<int32_t>{ *model.issuerId } : std::nullopt },
        m_redirectUri{ SafeString(model.redirectUri) }
    {
        SetModelPointers();
    }

    PFAccountManagementClientLinkPSNAccountRequestWrapper(const PFAccountManagementClientLinkPSNAccountRequestWrapper& src) :
        PFAccountManagementClientLinkPSNAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementClientLinkPSNAccountRequestWrapper(PFAccountManagementClientLinkPSNAccountRequestWrapper&& src) :
        PFAccountManagementClientLinkPSNAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementClientLinkPSNAccountRequestWrapper& operator=(PFAccountManagementClientLinkPSNAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementClientLinkPSNAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementClientLinkPSNAccountRequestWrapper& lhs, PFAccountManagementClientLinkPSNAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_authCode, rhs.m_authCode);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        swap(lhs.m_issuerId, rhs.m_issuerId);
        swap(lhs.m_redirectUri, rhs.m_redirectUri);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAuthCode(String value)
    {
        m_authCode = std::move(value);
        this->m_model.authCode =  m_authCode.empty() ? nullptr : m_authCode.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    void SetIssuerId(std::optional<int32_t> value)
    {
        m_issuerId = std::move(value);
        this->m_model.issuerId = m_issuerId ? m_issuerId.operator->() : nullptr;
    }

    void SetRedirectUri(String value)
    {
        m_redirectUri = std::move(value);
        this->m_model.redirectUri =  m_redirectUri.empty() ? nullptr : m_redirectUri.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.authCode = m_authCode.empty() ? nullptr : m_authCode.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
        this->m_model.issuerId = m_issuerId ? m_issuerId.operator->() : nullptr;
        this->m_model.redirectUri = m_redirectUri.empty() ? nullptr : m_redirectUri.data();
    }

    String m_authCode;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceLink;
    std::optional<int32_t> m_issuerId;
    String m_redirectUri;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementLinkSteamAccountRequestWrapper : public ModelWrapper<PFAccountManagementLinkSteamAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementLinkSteamAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementLinkSteamAccountRequestWrapper() = default;

    PFAccountManagementLinkSteamAccountRequestWrapper(const PFAccountManagementLinkSteamAccountRequest& model) :
        ModelWrapper<PFAccountManagementLinkSteamAccountRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt },
        m_steamTicket{ SafeString(model.steamTicket) },
        m_ticketIsServiceSpecific{ model.ticketIsServiceSpecific ? std::optional<bool>{ *model.ticketIsServiceSpecific } : std::nullopt }
    {
        SetModelPointers();
    }

    PFAccountManagementLinkSteamAccountRequestWrapper(const PFAccountManagementLinkSteamAccountRequestWrapper& src) :
        PFAccountManagementLinkSteamAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementLinkSteamAccountRequestWrapper(PFAccountManagementLinkSteamAccountRequestWrapper&& src) :
        PFAccountManagementLinkSteamAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementLinkSteamAccountRequestWrapper& operator=(PFAccountManagementLinkSteamAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementLinkSteamAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementLinkSteamAccountRequestWrapper& lhs, PFAccountManagementLinkSteamAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        swap(lhs.m_steamTicket, rhs.m_steamTicket);
        swap(lhs.m_ticketIsServiceSpecific, rhs.m_ticketIsServiceSpecific);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    void SetSteamTicket(String value)
    {
        m_steamTicket = std::move(value);
        this->m_model.steamTicket =  m_steamTicket.empty() ? nullptr : m_steamTicket.data();
    }

    void SetTicketIsServiceSpecific(std::optional<bool> value)
    {
        m_ticketIsServiceSpecific = std::move(value);
        this->m_model.ticketIsServiceSpecific = m_ticketIsServiceSpecific ? m_ticketIsServiceSpecific.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
        this->m_model.steamTicket = m_steamTicket.empty() ? nullptr : m_steamTicket.data();
        this->m_model.ticketIsServiceSpecific = m_ticketIsServiceSpecific ? m_ticketIsServiceSpecific.operator->() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceLink;
    String m_steamTicket;
    std::optional<bool> m_ticketIsServiceSpecific;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementLinkTwitchAccountRequestWrapper : public ModelWrapper<PFAccountManagementLinkTwitchAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementLinkTwitchAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementLinkTwitchAccountRequestWrapper() = default;

    PFAccountManagementLinkTwitchAccountRequestWrapper(const PFAccountManagementLinkTwitchAccountRequest& model) :
        ModelWrapper<PFAccountManagementLinkTwitchAccountRequest, Alloc>{ model },
        m_accessToken{ SafeString(model.accessToken) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt }
    {
        SetModelPointers();
    }

    PFAccountManagementLinkTwitchAccountRequestWrapper(const PFAccountManagementLinkTwitchAccountRequestWrapper& src) :
        PFAccountManagementLinkTwitchAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementLinkTwitchAccountRequestWrapper(PFAccountManagementLinkTwitchAccountRequestWrapper&& src) :
        PFAccountManagementLinkTwitchAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementLinkTwitchAccountRequestWrapper& operator=(PFAccountManagementLinkTwitchAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementLinkTwitchAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementLinkTwitchAccountRequestWrapper& lhs, PFAccountManagementLinkTwitchAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_accessToken, rhs.m_accessToken);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAccessToken(String value)
    {
        m_accessToken = std::move(value);
        this->m_model.accessToken =  m_accessToken.empty() ? nullptr : m_accessToken.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.accessToken = m_accessToken.empty() ? nullptr : m_accessToken.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    String m_accessToken;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceLink;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementClientLinkXboxAccountRequestWrapper : public ModelWrapper<PFAccountManagementClientLinkXboxAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementClientLinkXboxAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

#if HC_PLATFORM == HC_PLATFORM_GDK
    PFAccountManagementClientLinkXboxAccountRequestWrapper(XUser user)
        : m_user{ std::move(user) }
    {
        SetModelPointers();
    }
#else
    PFAccountManagementClientLinkXboxAccountRequestWrapper() = default;
#endif

    PFAccountManagementClientLinkXboxAccountRequestWrapper(const PFAccountManagementClientLinkXboxAccountRequest& model) :
        ModelWrapper<PFAccountManagementClientLinkXboxAccountRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt },
#if HC_PLATFORM == HC_PLATFORM_GDK
        m_user{ XUser::Duplicate(model.user) }
#else
        m_xboxToken{ SafeString(model.xboxToken) }
#endif
    {
        SetModelPointers();
    }

    PFAccountManagementClientLinkXboxAccountRequestWrapper(const PFAccountManagementClientLinkXboxAccountRequestWrapper& src) :
        PFAccountManagementClientLinkXboxAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementClientLinkXboxAccountRequestWrapper(PFAccountManagementClientLinkXboxAccountRequestWrapper&& src) :
#if HC_PLATFORM == HC_PLATFORM_GDK
        PFAccountManagementClientLinkXboxAccountRequestWrapper{ XUser::Wrap(nullptr) }
#else
        PFAccountManagementClientLinkXboxAccountRequestWrapper{}
#endif
    {
        swap(*this, src);
    }

    PFAccountManagementClientLinkXboxAccountRequestWrapper& operator=(PFAccountManagementClientLinkXboxAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementClientLinkXboxAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementClientLinkXboxAccountRequestWrapper& lhs, PFAccountManagementClientLinkXboxAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceLink, rhs.m_forceLink);
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

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
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
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
#if HC_PLATFORM == HC_PLATFORM_GDK
        this->m_model.user = m_user.Handle();
#else
        this->m_model.xboxToken = m_xboxToken.empty() ? nullptr : m_xboxToken.data();
#endif
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceLink;
#if HC_PLATFORM == HC_PLATFORM_GDK
    XUser m_user;
#endif
    String m_xboxToken;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementRemoveContactEmailRequestWrapper : public ModelWrapper<PFAccountManagementRemoveContactEmailRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementRemoveContactEmailRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementRemoveContactEmailRequestWrapper() = default;

    PFAccountManagementRemoveContactEmailRequestWrapper(const PFAccountManagementRemoveContactEmailRequest& model) :
        ModelWrapper<PFAccountManagementRemoveContactEmailRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementRemoveContactEmailRequestWrapper(const PFAccountManagementRemoveContactEmailRequestWrapper& src) :
        PFAccountManagementRemoveContactEmailRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementRemoveContactEmailRequestWrapper(PFAccountManagementRemoveContactEmailRequestWrapper&& src) :
        PFAccountManagementRemoveContactEmailRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementRemoveContactEmailRequestWrapper& operator=(PFAccountManagementRemoveContactEmailRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementRemoveContactEmailRequestWrapper() = default;

    friend void swap(PFAccountManagementRemoveContactEmailRequestWrapper& lhs, PFAccountManagementRemoveContactEmailRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementReportPlayerClientRequestWrapper : public ModelWrapper<PFAccountManagementReportPlayerClientRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementReportPlayerClientRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementReportPlayerClientRequestWrapper() = default;

    PFAccountManagementReportPlayerClientRequestWrapper(const PFAccountManagementReportPlayerClientRequest& model) :
        ModelWrapper<PFAccountManagementReportPlayerClientRequest, Alloc>{ model },
        m_comment{ SafeString(model.comment) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_reporteeId{ SafeString(model.reporteeId) }
    {
        SetModelPointers();
    }

    PFAccountManagementReportPlayerClientRequestWrapper(const PFAccountManagementReportPlayerClientRequestWrapper& src) :
        PFAccountManagementReportPlayerClientRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementReportPlayerClientRequestWrapper(PFAccountManagementReportPlayerClientRequestWrapper&& src) :
        PFAccountManagementReportPlayerClientRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementReportPlayerClientRequestWrapper& operator=(PFAccountManagementReportPlayerClientRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementReportPlayerClientRequestWrapper() = default;

    friend void swap(PFAccountManagementReportPlayerClientRequestWrapper& lhs, PFAccountManagementReportPlayerClientRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_comment, rhs.m_comment);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_reporteeId, rhs.m_reporteeId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetComment(String value)
    {
        m_comment = std::move(value);
        this->m_model.comment =  m_comment.empty() ? nullptr : m_comment.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetReporteeId(String value)
    {
        m_reporteeId = std::move(value);
        this->m_model.reporteeId =  m_reporteeId.empty() ? nullptr : m_reporteeId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.comment = m_comment.empty() ? nullptr : m_comment.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.reporteeId = m_reporteeId.empty() ? nullptr : m_reporteeId.data();
    }

    String m_comment;
    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_reporteeId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementReportPlayerClientResultWrapper : public ModelWrapper<PFAccountManagementReportPlayerClientResult, Alloc>
{
public:
    using ModelType = PFAccountManagementReportPlayerClientResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    using ModelWrapper<PFAccountManagementReportPlayerClientResult, Alloc>::ModelWrapper;

    void SetSubmissionsRemaining(int32_t value)
    {
        this->m_model.submissionsRemaining = value;
    }

private:
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementSendAccountRecoveryEmailRequestWrapper : public ModelWrapper<PFAccountManagementSendAccountRecoveryEmailRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementSendAccountRecoveryEmailRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementSendAccountRecoveryEmailRequestWrapper() = default;

    PFAccountManagementSendAccountRecoveryEmailRequestWrapper(const PFAccountManagementSendAccountRecoveryEmailRequest& model) :
        ModelWrapper<PFAccountManagementSendAccountRecoveryEmailRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_email{ SafeString(model.email) },
        m_emailTemplateId{ SafeString(model.emailTemplateId) },
        m_titleId{ SafeString(model.titleId) }
    {
        SetModelPointers();
    }

    PFAccountManagementSendAccountRecoveryEmailRequestWrapper(const PFAccountManagementSendAccountRecoveryEmailRequestWrapper& src) :
        PFAccountManagementSendAccountRecoveryEmailRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementSendAccountRecoveryEmailRequestWrapper(PFAccountManagementSendAccountRecoveryEmailRequestWrapper&& src) :
        PFAccountManagementSendAccountRecoveryEmailRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementSendAccountRecoveryEmailRequestWrapper& operator=(PFAccountManagementSendAccountRecoveryEmailRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementSendAccountRecoveryEmailRequestWrapper() = default;

    friend void swap(PFAccountManagementSendAccountRecoveryEmailRequestWrapper& lhs, PFAccountManagementSendAccountRecoveryEmailRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_email, rhs.m_email);
        swap(lhs.m_emailTemplateId, rhs.m_emailTemplateId);
        swap(lhs.m_titleId, rhs.m_titleId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEmail(String value)
    {
        m_email = std::move(value);
        this->m_model.email =  m_email.empty() ? nullptr : m_email.data();
    }

    void SetEmailTemplateId(String value)
    {
        m_emailTemplateId = std::move(value);
        this->m_model.emailTemplateId =  m_emailTemplateId.empty() ? nullptr : m_emailTemplateId.data();
    }

    void SetTitleId(String value)
    {
        m_titleId = std::move(value);
        this->m_model.titleId =  m_titleId.empty() ? nullptr : m_titleId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.email = m_email.empty() ? nullptr : m_email.data();
        this->m_model.emailTemplateId = m_emailTemplateId.empty() ? nullptr : m_emailTemplateId.data();
        this->m_model.titleId = m_titleId.empty() ? nullptr : m_titleId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_email;
    String m_emailTemplateId;
    String m_titleId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementUnlinkAndroidDeviceIDRequestWrapper : public ModelWrapper<PFAccountManagementUnlinkAndroidDeviceIDRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementUnlinkAndroidDeviceIDRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementUnlinkAndroidDeviceIDRequestWrapper() = default;

    PFAccountManagementUnlinkAndroidDeviceIDRequestWrapper(const PFAccountManagementUnlinkAndroidDeviceIDRequest& model) :
        ModelWrapper<PFAccountManagementUnlinkAndroidDeviceIDRequest, Alloc>{ model },
        m_androidDeviceId{ SafeString(model.androidDeviceId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementUnlinkAndroidDeviceIDRequestWrapper(const PFAccountManagementUnlinkAndroidDeviceIDRequestWrapper& src) :
        PFAccountManagementUnlinkAndroidDeviceIDRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementUnlinkAndroidDeviceIDRequestWrapper(PFAccountManagementUnlinkAndroidDeviceIDRequestWrapper&& src) :
        PFAccountManagementUnlinkAndroidDeviceIDRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementUnlinkAndroidDeviceIDRequestWrapper& operator=(PFAccountManagementUnlinkAndroidDeviceIDRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementUnlinkAndroidDeviceIDRequestWrapper() = default;

    friend void swap(PFAccountManagementUnlinkAndroidDeviceIDRequestWrapper& lhs, PFAccountManagementUnlinkAndroidDeviceIDRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_androidDeviceId, rhs.m_androidDeviceId);
        swap(lhs.m_customTags, rhs.m_customTags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAndroidDeviceId(String value)
    {
        m_androidDeviceId = std::move(value);
        this->m_model.androidDeviceId =  m_androidDeviceId.empty() ? nullptr : m_androidDeviceId.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.androidDeviceId = m_androidDeviceId.empty() ? nullptr : m_androidDeviceId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
    }

    String m_androidDeviceId;
    StringDictionaryEntryVector<Alloc> m_customTags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementUnlinkAppleRequestWrapper : public ModelWrapper<PFAccountManagementUnlinkAppleRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementUnlinkAppleRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementUnlinkAppleRequestWrapper() = default;

    PFAccountManagementUnlinkAppleRequestWrapper(const PFAccountManagementUnlinkAppleRequest& model) :
        ModelWrapper<PFAccountManagementUnlinkAppleRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementUnlinkAppleRequestWrapper(const PFAccountManagementUnlinkAppleRequestWrapper& src) :
        PFAccountManagementUnlinkAppleRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementUnlinkAppleRequestWrapper(PFAccountManagementUnlinkAppleRequestWrapper&& src) :
        PFAccountManagementUnlinkAppleRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementUnlinkAppleRequestWrapper& operator=(PFAccountManagementUnlinkAppleRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementUnlinkAppleRequestWrapper() = default;

    friend void swap(PFAccountManagementUnlinkAppleRequestWrapper& lhs, PFAccountManagementUnlinkAppleRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementUnlinkCustomIDRequestWrapper : public ModelWrapper<PFAccountManagementUnlinkCustomIDRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementUnlinkCustomIDRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementUnlinkCustomIDRequestWrapper() = default;

    PFAccountManagementUnlinkCustomIDRequestWrapper(const PFAccountManagementUnlinkCustomIDRequest& model) :
        ModelWrapper<PFAccountManagementUnlinkCustomIDRequest, Alloc>{ model },
        m_customId{ SafeString(model.customId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementUnlinkCustomIDRequestWrapper(const PFAccountManagementUnlinkCustomIDRequestWrapper& src) :
        PFAccountManagementUnlinkCustomIDRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementUnlinkCustomIDRequestWrapper(PFAccountManagementUnlinkCustomIDRequestWrapper&& src) :
        PFAccountManagementUnlinkCustomIDRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementUnlinkCustomIDRequestWrapper& operator=(PFAccountManagementUnlinkCustomIDRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementUnlinkCustomIDRequestWrapper() = default;

    friend void swap(PFAccountManagementUnlinkCustomIDRequestWrapper& lhs, PFAccountManagementUnlinkCustomIDRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customId, rhs.m_customId);
        swap(lhs.m_customTags, rhs.m_customTags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomId(String value)
    {
        m_customId = std::move(value);
        this->m_model.customId =  m_customId.empty() ? nullptr : m_customId.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customId = m_customId.empty() ? nullptr : m_customId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
    }

    String m_customId;
    StringDictionaryEntryVector<Alloc> m_customTags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementUnlinkFacebookAccountRequestWrapper : public ModelWrapper<PFAccountManagementUnlinkFacebookAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementUnlinkFacebookAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementUnlinkFacebookAccountRequestWrapper() = default;

    PFAccountManagementUnlinkFacebookAccountRequestWrapper(const PFAccountManagementUnlinkFacebookAccountRequest& model) :
        ModelWrapper<PFAccountManagementUnlinkFacebookAccountRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementUnlinkFacebookAccountRequestWrapper(const PFAccountManagementUnlinkFacebookAccountRequestWrapper& src) :
        PFAccountManagementUnlinkFacebookAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementUnlinkFacebookAccountRequestWrapper(PFAccountManagementUnlinkFacebookAccountRequestWrapper&& src) :
        PFAccountManagementUnlinkFacebookAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementUnlinkFacebookAccountRequestWrapper& operator=(PFAccountManagementUnlinkFacebookAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementUnlinkFacebookAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementUnlinkFacebookAccountRequestWrapper& lhs, PFAccountManagementUnlinkFacebookAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementUnlinkFacebookInstantGamesIdRequestWrapper : public ModelWrapper<PFAccountManagementUnlinkFacebookInstantGamesIdRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementUnlinkFacebookInstantGamesIdRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementUnlinkFacebookInstantGamesIdRequestWrapper() = default;

    PFAccountManagementUnlinkFacebookInstantGamesIdRequestWrapper(const PFAccountManagementUnlinkFacebookInstantGamesIdRequest& model) :
        ModelWrapper<PFAccountManagementUnlinkFacebookInstantGamesIdRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_facebookInstantGamesId{ SafeString(model.facebookInstantGamesId) }
    {
        SetModelPointers();
    }

    PFAccountManagementUnlinkFacebookInstantGamesIdRequestWrapper(const PFAccountManagementUnlinkFacebookInstantGamesIdRequestWrapper& src) :
        PFAccountManagementUnlinkFacebookInstantGamesIdRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementUnlinkFacebookInstantGamesIdRequestWrapper(PFAccountManagementUnlinkFacebookInstantGamesIdRequestWrapper&& src) :
        PFAccountManagementUnlinkFacebookInstantGamesIdRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementUnlinkFacebookInstantGamesIdRequestWrapper& operator=(PFAccountManagementUnlinkFacebookInstantGamesIdRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementUnlinkFacebookInstantGamesIdRequestWrapper() = default;

    friend void swap(PFAccountManagementUnlinkFacebookInstantGamesIdRequestWrapper& lhs, PFAccountManagementUnlinkFacebookInstantGamesIdRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_facebookInstantGamesId, rhs.m_facebookInstantGamesId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetFacebookInstantGamesId(String value)
    {
        m_facebookInstantGamesId = std::move(value);
        this->m_model.facebookInstantGamesId =  m_facebookInstantGamesId.empty() ? nullptr : m_facebookInstantGamesId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.facebookInstantGamesId = m_facebookInstantGamesId.empty() ? nullptr : m_facebookInstantGamesId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_facebookInstantGamesId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementUnlinkGameCenterAccountRequestWrapper : public ModelWrapper<PFAccountManagementUnlinkGameCenterAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementUnlinkGameCenterAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementUnlinkGameCenterAccountRequestWrapper() = default;

    PFAccountManagementUnlinkGameCenterAccountRequestWrapper(const PFAccountManagementUnlinkGameCenterAccountRequest& model) :
        ModelWrapper<PFAccountManagementUnlinkGameCenterAccountRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementUnlinkGameCenterAccountRequestWrapper(const PFAccountManagementUnlinkGameCenterAccountRequestWrapper& src) :
        PFAccountManagementUnlinkGameCenterAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementUnlinkGameCenterAccountRequestWrapper(PFAccountManagementUnlinkGameCenterAccountRequestWrapper&& src) :
        PFAccountManagementUnlinkGameCenterAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementUnlinkGameCenterAccountRequestWrapper& operator=(PFAccountManagementUnlinkGameCenterAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementUnlinkGameCenterAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementUnlinkGameCenterAccountRequestWrapper& lhs, PFAccountManagementUnlinkGameCenterAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementUnlinkGoogleAccountRequestWrapper : public ModelWrapper<PFAccountManagementUnlinkGoogleAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementUnlinkGoogleAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementUnlinkGoogleAccountRequestWrapper() = default;

    PFAccountManagementUnlinkGoogleAccountRequestWrapper(const PFAccountManagementUnlinkGoogleAccountRequest& model) :
        ModelWrapper<PFAccountManagementUnlinkGoogleAccountRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementUnlinkGoogleAccountRequestWrapper(const PFAccountManagementUnlinkGoogleAccountRequestWrapper& src) :
        PFAccountManagementUnlinkGoogleAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementUnlinkGoogleAccountRequestWrapper(PFAccountManagementUnlinkGoogleAccountRequestWrapper&& src) :
        PFAccountManagementUnlinkGoogleAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementUnlinkGoogleAccountRequestWrapper& operator=(PFAccountManagementUnlinkGoogleAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementUnlinkGoogleAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementUnlinkGoogleAccountRequestWrapper& lhs, PFAccountManagementUnlinkGoogleAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementUnlinkGooglePlayGamesServicesAccountRequestWrapper : public ModelWrapper<PFAccountManagementUnlinkGooglePlayGamesServicesAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementUnlinkGooglePlayGamesServicesAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementUnlinkGooglePlayGamesServicesAccountRequestWrapper() = default;

    PFAccountManagementUnlinkGooglePlayGamesServicesAccountRequestWrapper(const PFAccountManagementUnlinkGooglePlayGamesServicesAccountRequest& model) :
        ModelWrapper<PFAccountManagementUnlinkGooglePlayGamesServicesAccountRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementUnlinkGooglePlayGamesServicesAccountRequestWrapper(const PFAccountManagementUnlinkGooglePlayGamesServicesAccountRequestWrapper& src) :
        PFAccountManagementUnlinkGooglePlayGamesServicesAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementUnlinkGooglePlayGamesServicesAccountRequestWrapper(PFAccountManagementUnlinkGooglePlayGamesServicesAccountRequestWrapper&& src) :
        PFAccountManagementUnlinkGooglePlayGamesServicesAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementUnlinkGooglePlayGamesServicesAccountRequestWrapper& operator=(PFAccountManagementUnlinkGooglePlayGamesServicesAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementUnlinkGooglePlayGamesServicesAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementUnlinkGooglePlayGamesServicesAccountRequestWrapper& lhs, PFAccountManagementUnlinkGooglePlayGamesServicesAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementUnlinkIOSDeviceIDRequestWrapper : public ModelWrapper<PFAccountManagementUnlinkIOSDeviceIDRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementUnlinkIOSDeviceIDRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementUnlinkIOSDeviceIDRequestWrapper() = default;

    PFAccountManagementUnlinkIOSDeviceIDRequestWrapper(const PFAccountManagementUnlinkIOSDeviceIDRequest& model) :
        ModelWrapper<PFAccountManagementUnlinkIOSDeviceIDRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_deviceId{ SafeString(model.deviceId) }
    {
        SetModelPointers();
    }

    PFAccountManagementUnlinkIOSDeviceIDRequestWrapper(const PFAccountManagementUnlinkIOSDeviceIDRequestWrapper& src) :
        PFAccountManagementUnlinkIOSDeviceIDRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementUnlinkIOSDeviceIDRequestWrapper(PFAccountManagementUnlinkIOSDeviceIDRequestWrapper&& src) :
        PFAccountManagementUnlinkIOSDeviceIDRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementUnlinkIOSDeviceIDRequestWrapper& operator=(PFAccountManagementUnlinkIOSDeviceIDRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementUnlinkIOSDeviceIDRequestWrapper() = default;

    friend void swap(PFAccountManagementUnlinkIOSDeviceIDRequestWrapper& lhs, PFAccountManagementUnlinkIOSDeviceIDRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_deviceId, rhs.m_deviceId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetDeviceId(String value)
    {
        m_deviceId = std::move(value);
        this->m_model.deviceId =  m_deviceId.empty() ? nullptr : m_deviceId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.deviceId = m_deviceId.empty() ? nullptr : m_deviceId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_deviceId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementUnlinkKongregateAccountRequestWrapper : public ModelWrapper<PFAccountManagementUnlinkKongregateAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementUnlinkKongregateAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementUnlinkKongregateAccountRequestWrapper() = default;

    PFAccountManagementUnlinkKongregateAccountRequestWrapper(const PFAccountManagementUnlinkKongregateAccountRequest& model) :
        ModelWrapper<PFAccountManagementUnlinkKongregateAccountRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementUnlinkKongregateAccountRequestWrapper(const PFAccountManagementUnlinkKongregateAccountRequestWrapper& src) :
        PFAccountManagementUnlinkKongregateAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementUnlinkKongregateAccountRequestWrapper(PFAccountManagementUnlinkKongregateAccountRequestWrapper&& src) :
        PFAccountManagementUnlinkKongregateAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementUnlinkKongregateAccountRequestWrapper& operator=(PFAccountManagementUnlinkKongregateAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementUnlinkKongregateAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementUnlinkKongregateAccountRequestWrapper& lhs, PFAccountManagementUnlinkKongregateAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementClientUnlinkNintendoServiceAccountRequestWrapper : public ModelWrapper<PFAccountManagementClientUnlinkNintendoServiceAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementClientUnlinkNintendoServiceAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementClientUnlinkNintendoServiceAccountRequestWrapper() = default;

    PFAccountManagementClientUnlinkNintendoServiceAccountRequestWrapper(const PFAccountManagementClientUnlinkNintendoServiceAccountRequest& model) :
        ModelWrapper<PFAccountManagementClientUnlinkNintendoServiceAccountRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementClientUnlinkNintendoServiceAccountRequestWrapper(const PFAccountManagementClientUnlinkNintendoServiceAccountRequestWrapper& src) :
        PFAccountManagementClientUnlinkNintendoServiceAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementClientUnlinkNintendoServiceAccountRequestWrapper(PFAccountManagementClientUnlinkNintendoServiceAccountRequestWrapper&& src) :
        PFAccountManagementClientUnlinkNintendoServiceAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementClientUnlinkNintendoServiceAccountRequestWrapper& operator=(PFAccountManagementClientUnlinkNintendoServiceAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementClientUnlinkNintendoServiceAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementClientUnlinkNintendoServiceAccountRequestWrapper& lhs, PFAccountManagementClientUnlinkNintendoServiceAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementClientUnlinkNintendoSwitchDeviceIdRequestWrapper : public ModelWrapper<PFAccountManagementClientUnlinkNintendoSwitchDeviceIdRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementClientUnlinkNintendoSwitchDeviceIdRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementClientUnlinkNintendoSwitchDeviceIdRequestWrapper() = default;

    PFAccountManagementClientUnlinkNintendoSwitchDeviceIdRequestWrapper(const PFAccountManagementClientUnlinkNintendoSwitchDeviceIdRequest& model) :
        ModelWrapper<PFAccountManagementClientUnlinkNintendoSwitchDeviceIdRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_nintendoSwitchDeviceId{ SafeString(model.nintendoSwitchDeviceId) }
    {
        SetModelPointers();
    }

    PFAccountManagementClientUnlinkNintendoSwitchDeviceIdRequestWrapper(const PFAccountManagementClientUnlinkNintendoSwitchDeviceIdRequestWrapper& src) :
        PFAccountManagementClientUnlinkNintendoSwitchDeviceIdRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementClientUnlinkNintendoSwitchDeviceIdRequestWrapper(PFAccountManagementClientUnlinkNintendoSwitchDeviceIdRequestWrapper&& src) :
        PFAccountManagementClientUnlinkNintendoSwitchDeviceIdRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementClientUnlinkNintendoSwitchDeviceIdRequestWrapper& operator=(PFAccountManagementClientUnlinkNintendoSwitchDeviceIdRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementClientUnlinkNintendoSwitchDeviceIdRequestWrapper() = default;

    friend void swap(PFAccountManagementClientUnlinkNintendoSwitchDeviceIdRequestWrapper& lhs, PFAccountManagementClientUnlinkNintendoSwitchDeviceIdRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_nintendoSwitchDeviceId, rhs.m_nintendoSwitchDeviceId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetNintendoSwitchDeviceId(String value)
    {
        m_nintendoSwitchDeviceId = std::move(value);
        this->m_model.nintendoSwitchDeviceId =  m_nintendoSwitchDeviceId.empty() ? nullptr : m_nintendoSwitchDeviceId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.nintendoSwitchDeviceId = m_nintendoSwitchDeviceId.empty() ? nullptr : m_nintendoSwitchDeviceId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_nintendoSwitchDeviceId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementUnlinkOpenIdConnectRequestWrapper : public ModelWrapper<PFAccountManagementUnlinkOpenIdConnectRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementUnlinkOpenIdConnectRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementUnlinkOpenIdConnectRequestWrapper() = default;

    PFAccountManagementUnlinkOpenIdConnectRequestWrapper(const PFAccountManagementUnlinkOpenIdConnectRequest& model) :
        ModelWrapper<PFAccountManagementUnlinkOpenIdConnectRequest, Alloc>{ model },
        m_connectionId{ SafeString(model.connectionId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementUnlinkOpenIdConnectRequestWrapper(const PFAccountManagementUnlinkOpenIdConnectRequestWrapper& src) :
        PFAccountManagementUnlinkOpenIdConnectRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementUnlinkOpenIdConnectRequestWrapper(PFAccountManagementUnlinkOpenIdConnectRequestWrapper&& src) :
        PFAccountManagementUnlinkOpenIdConnectRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementUnlinkOpenIdConnectRequestWrapper& operator=(PFAccountManagementUnlinkOpenIdConnectRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementUnlinkOpenIdConnectRequestWrapper() = default;

    friend void swap(PFAccountManagementUnlinkOpenIdConnectRequestWrapper& lhs, PFAccountManagementUnlinkOpenIdConnectRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_connectionId, rhs.m_connectionId);
        swap(lhs.m_customTags, rhs.m_customTags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetConnectionId(String value)
    {
        m_connectionId = std::move(value);
        this->m_model.connectionId =  m_connectionId.empty() ? nullptr : m_connectionId.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.connectionId = m_connectionId.empty() ? nullptr : m_connectionId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
    }

    String m_connectionId;
    StringDictionaryEntryVector<Alloc> m_customTags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementClientUnlinkPSNAccountRequestWrapper : public ModelWrapper<PFAccountManagementClientUnlinkPSNAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementClientUnlinkPSNAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementClientUnlinkPSNAccountRequestWrapper() = default;

    PFAccountManagementClientUnlinkPSNAccountRequestWrapper(const PFAccountManagementClientUnlinkPSNAccountRequest& model) :
        ModelWrapper<PFAccountManagementClientUnlinkPSNAccountRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementClientUnlinkPSNAccountRequestWrapper(const PFAccountManagementClientUnlinkPSNAccountRequestWrapper& src) :
        PFAccountManagementClientUnlinkPSNAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementClientUnlinkPSNAccountRequestWrapper(PFAccountManagementClientUnlinkPSNAccountRequestWrapper&& src) :
        PFAccountManagementClientUnlinkPSNAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementClientUnlinkPSNAccountRequestWrapper& operator=(PFAccountManagementClientUnlinkPSNAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementClientUnlinkPSNAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementClientUnlinkPSNAccountRequestWrapper& lhs, PFAccountManagementClientUnlinkPSNAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementUnlinkSteamAccountRequestWrapper : public ModelWrapper<PFAccountManagementUnlinkSteamAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementUnlinkSteamAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementUnlinkSteamAccountRequestWrapper() = default;

    PFAccountManagementUnlinkSteamAccountRequestWrapper(const PFAccountManagementUnlinkSteamAccountRequest& model) :
        ModelWrapper<PFAccountManagementUnlinkSteamAccountRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementUnlinkSteamAccountRequestWrapper(const PFAccountManagementUnlinkSteamAccountRequestWrapper& src) :
        PFAccountManagementUnlinkSteamAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementUnlinkSteamAccountRequestWrapper(PFAccountManagementUnlinkSteamAccountRequestWrapper&& src) :
        PFAccountManagementUnlinkSteamAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementUnlinkSteamAccountRequestWrapper& operator=(PFAccountManagementUnlinkSteamAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementUnlinkSteamAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementUnlinkSteamAccountRequestWrapper& lhs, PFAccountManagementUnlinkSteamAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementUnlinkTwitchAccountRequestWrapper : public ModelWrapper<PFAccountManagementUnlinkTwitchAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementUnlinkTwitchAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementUnlinkTwitchAccountRequestWrapper() = default;

    PFAccountManagementUnlinkTwitchAccountRequestWrapper(const PFAccountManagementUnlinkTwitchAccountRequest& model) :
        ModelWrapper<PFAccountManagementUnlinkTwitchAccountRequest, Alloc>{ model },
        m_accessToken{ SafeString(model.accessToken) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementUnlinkTwitchAccountRequestWrapper(const PFAccountManagementUnlinkTwitchAccountRequestWrapper& src) :
        PFAccountManagementUnlinkTwitchAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementUnlinkTwitchAccountRequestWrapper(PFAccountManagementUnlinkTwitchAccountRequestWrapper&& src) :
        PFAccountManagementUnlinkTwitchAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementUnlinkTwitchAccountRequestWrapper& operator=(PFAccountManagementUnlinkTwitchAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementUnlinkTwitchAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementUnlinkTwitchAccountRequestWrapper& lhs, PFAccountManagementUnlinkTwitchAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_accessToken, rhs.m_accessToken);
        swap(lhs.m_customTags, rhs.m_customTags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAccessToken(String value)
    {
        m_accessToken = std::move(value);
        this->m_model.accessToken =  m_accessToken.empty() ? nullptr : m_accessToken.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.accessToken = m_accessToken.empty() ? nullptr : m_accessToken.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
    }

    String m_accessToken;
    StringDictionaryEntryVector<Alloc> m_customTags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementClientUnlinkXboxAccountRequestWrapper : public ModelWrapper<PFAccountManagementClientUnlinkXboxAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementClientUnlinkXboxAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementClientUnlinkXboxAccountRequestWrapper() = default;

    PFAccountManagementClientUnlinkXboxAccountRequestWrapper(const PFAccountManagementClientUnlinkXboxAccountRequest& model) :
        ModelWrapper<PFAccountManagementClientUnlinkXboxAccountRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementClientUnlinkXboxAccountRequestWrapper(const PFAccountManagementClientUnlinkXboxAccountRequestWrapper& src) :
        PFAccountManagementClientUnlinkXboxAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementClientUnlinkXboxAccountRequestWrapper(PFAccountManagementClientUnlinkXboxAccountRequestWrapper&& src) :
        PFAccountManagementClientUnlinkXboxAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementClientUnlinkXboxAccountRequestWrapper& operator=(PFAccountManagementClientUnlinkXboxAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementClientUnlinkXboxAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementClientUnlinkXboxAccountRequestWrapper& lhs, PFAccountManagementClientUnlinkXboxAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementClientUpdateAvatarUrlRequestWrapper : public ModelWrapper<PFAccountManagementClientUpdateAvatarUrlRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementClientUpdateAvatarUrlRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementClientUpdateAvatarUrlRequestWrapper() = default;

    PFAccountManagementClientUpdateAvatarUrlRequestWrapper(const PFAccountManagementClientUpdateAvatarUrlRequest& model) :
        ModelWrapper<PFAccountManagementClientUpdateAvatarUrlRequest, Alloc>{ model },
        m_imageUrl{ SafeString(model.imageUrl) }
    {
        SetModelPointers();
    }

    PFAccountManagementClientUpdateAvatarUrlRequestWrapper(const PFAccountManagementClientUpdateAvatarUrlRequestWrapper& src) :
        PFAccountManagementClientUpdateAvatarUrlRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementClientUpdateAvatarUrlRequestWrapper(PFAccountManagementClientUpdateAvatarUrlRequestWrapper&& src) :
        PFAccountManagementClientUpdateAvatarUrlRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementClientUpdateAvatarUrlRequestWrapper& operator=(PFAccountManagementClientUpdateAvatarUrlRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementClientUpdateAvatarUrlRequestWrapper() = default;

    friend void swap(PFAccountManagementClientUpdateAvatarUrlRequestWrapper& lhs, PFAccountManagementClientUpdateAvatarUrlRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_imageUrl, rhs.m_imageUrl);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetImageUrl(String value)
    {
        m_imageUrl = std::move(value);
        this->m_model.imageUrl =  m_imageUrl.empty() ? nullptr : m_imageUrl.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.imageUrl = m_imageUrl.empty() ? nullptr : m_imageUrl.data();
    }

    String m_imageUrl;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementUpdateUserTitleDisplayNameRequestWrapper : public ModelWrapper<PFAccountManagementUpdateUserTitleDisplayNameRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementUpdateUserTitleDisplayNameRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementUpdateUserTitleDisplayNameRequestWrapper() = default;

    PFAccountManagementUpdateUserTitleDisplayNameRequestWrapper(const PFAccountManagementUpdateUserTitleDisplayNameRequest& model) :
        ModelWrapper<PFAccountManagementUpdateUserTitleDisplayNameRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_displayName{ SafeString(model.displayName) }
    {
        SetModelPointers();
    }

    PFAccountManagementUpdateUserTitleDisplayNameRequestWrapper(const PFAccountManagementUpdateUserTitleDisplayNameRequestWrapper& src) :
        PFAccountManagementUpdateUserTitleDisplayNameRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementUpdateUserTitleDisplayNameRequestWrapper(PFAccountManagementUpdateUserTitleDisplayNameRequestWrapper&& src) :
        PFAccountManagementUpdateUserTitleDisplayNameRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementUpdateUserTitleDisplayNameRequestWrapper& operator=(PFAccountManagementUpdateUserTitleDisplayNameRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementUpdateUserTitleDisplayNameRequestWrapper() = default;

    friend void swap(PFAccountManagementUpdateUserTitleDisplayNameRequestWrapper& lhs, PFAccountManagementUpdateUserTitleDisplayNameRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_displayName, rhs.m_displayName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetDisplayName(String value)
    {
        m_displayName = std::move(value);
        this->m_model.displayName =  m_displayName.empty() ? nullptr : m_displayName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.displayName = m_displayName.empty() ? nullptr : m_displayName.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_displayName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementUpdateUserTitleDisplayNameResultWrapper : public ModelWrapper<PFAccountManagementUpdateUserTitleDisplayNameResult, Alloc>
{
public:
    using ModelType = PFAccountManagementUpdateUserTitleDisplayNameResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementUpdateUserTitleDisplayNameResultWrapper() = default;

    PFAccountManagementUpdateUserTitleDisplayNameResultWrapper(const PFAccountManagementUpdateUserTitleDisplayNameResult& model) :
        ModelWrapper<PFAccountManagementUpdateUserTitleDisplayNameResult, Alloc>{ model },
        m_displayName{ SafeString(model.displayName) }
    {
        SetModelPointers();
    }

    PFAccountManagementUpdateUserTitleDisplayNameResultWrapper(const PFAccountManagementUpdateUserTitleDisplayNameResultWrapper& src) :
        PFAccountManagementUpdateUserTitleDisplayNameResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementUpdateUserTitleDisplayNameResultWrapper(PFAccountManagementUpdateUserTitleDisplayNameResultWrapper&& src) :
        PFAccountManagementUpdateUserTitleDisplayNameResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementUpdateUserTitleDisplayNameResultWrapper& operator=(PFAccountManagementUpdateUserTitleDisplayNameResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementUpdateUserTitleDisplayNameResultWrapper() = default;

    friend void swap(PFAccountManagementUpdateUserTitleDisplayNameResultWrapper& lhs, PFAccountManagementUpdateUserTitleDisplayNameResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_displayName, rhs.m_displayName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetDisplayName(String value)
    {
        m_displayName = std::move(value);
        this->m_model.displayName =  m_displayName.empty() ? nullptr : m_displayName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.displayName = m_displayName.empty() ? nullptr : m_displayName.data();
    }

    String m_displayName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementBanRequestWrapper : public ModelWrapper<PFAccountManagementBanRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementBanRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementBanRequestWrapper() = default;

    PFAccountManagementBanRequestWrapper(const PFAccountManagementBanRequest& model) :
        ModelWrapper<PFAccountManagementBanRequest, Alloc>{ model },
        m_durationInHours{ model.durationInHours ? std::optional<uint32_t>{ *model.durationInHours } : std::nullopt },
        m_IPAddress{ SafeString(model.IPAddress) },
        m_playFabId{ SafeString(model.playFabId) },
        m_reason{ SafeString(model.reason) }
    {
        SetModelPointers();
    }

    PFAccountManagementBanRequestWrapper(const PFAccountManagementBanRequestWrapper& src) :
        PFAccountManagementBanRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementBanRequestWrapper(PFAccountManagementBanRequestWrapper&& src) :
        PFAccountManagementBanRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementBanRequestWrapper& operator=(PFAccountManagementBanRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementBanRequestWrapper() = default;

    friend void swap(PFAccountManagementBanRequestWrapper& lhs, PFAccountManagementBanRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_durationInHours, rhs.m_durationInHours);
        swap(lhs.m_IPAddress, rhs.m_IPAddress);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_reason, rhs.m_reason);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetDurationInHours(std::optional<uint32_t> value)
    {
        m_durationInHours = std::move(value);
        this->m_model.durationInHours = m_durationInHours ? m_durationInHours.operator->() : nullptr;
    }

    void SetIPAddress(String value)
    {
        m_IPAddress = std::move(value);
        this->m_model.IPAddress =  m_IPAddress.empty() ? nullptr : m_IPAddress.data();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetReason(String value)
    {
        m_reason = std::move(value);
        this->m_model.reason =  m_reason.empty() ? nullptr : m_reason.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.durationInHours = m_durationInHours ? m_durationInHours.operator->() : nullptr;
        this->m_model.IPAddress = m_IPAddress.empty() ? nullptr : m_IPAddress.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.reason = m_reason.empty() ? nullptr : m_reason.data();
    }

    std::optional<uint32_t> m_durationInHours;
    String m_IPAddress;
    String m_playFabId;
    String m_reason;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementBanUsersRequestWrapper : public ModelWrapper<PFAccountManagementBanUsersRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementBanUsersRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementBanUsersRequestWrapper() = default;

    PFAccountManagementBanUsersRequestWrapper(const PFAccountManagementBanUsersRequest& model) :
        ModelWrapper<PFAccountManagementBanUsersRequest, Alloc>{ model },
        m_bans{ model.bans, model.bans + model.bansCount },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementBanUsersRequestWrapper(const PFAccountManagementBanUsersRequestWrapper& src) :
        PFAccountManagementBanUsersRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementBanUsersRequestWrapper(PFAccountManagementBanUsersRequestWrapper&& src) :
        PFAccountManagementBanUsersRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementBanUsersRequestWrapper& operator=(PFAccountManagementBanUsersRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementBanUsersRequestWrapper() = default;

    friend void swap(PFAccountManagementBanUsersRequestWrapper& lhs, PFAccountManagementBanUsersRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_bans, rhs.m_bans);
        swap(lhs.m_customTags, rhs.m_customTags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetBans(ModelVector<PFAccountManagementBanRequestWrapper<Alloc>, Alloc> value)
    {
        m_bans = std::move(value);
        this->m_model.bans =  m_bans.empty() ? nullptr : m_bans.data();
        this->m_model.bansCount =  static_cast<uint32_t>(m_bans.size());
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.bans = m_bans.empty() ? nullptr : m_bans.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
    }

    ModelVector<PFAccountManagementBanRequestWrapper<Alloc>, Alloc> m_bans;
    StringDictionaryEntryVector<Alloc> m_customTags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementBanInfoWrapper : public ModelWrapper<PFAccountManagementBanInfo, Alloc>
{
public:
    using ModelType = PFAccountManagementBanInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementBanInfoWrapper() = default;

    PFAccountManagementBanInfoWrapper(const PFAccountManagementBanInfo& model) :
        ModelWrapper<PFAccountManagementBanInfo, Alloc>{ model },
        m_banId{ SafeString(model.banId) },
        m_created{ model.created ? std::optional<time_t>{ *model.created } : std::nullopt },
        m_expires{ model.expires ? std::optional<time_t>{ *model.expires } : std::nullopt },
        m_IPAddress{ SafeString(model.IPAddress) },
        m_playFabId{ SafeString(model.playFabId) },
        m_reason{ SafeString(model.reason) }
    {
        SetModelPointers();
    }

    PFAccountManagementBanInfoWrapper(const PFAccountManagementBanInfoWrapper& src) :
        PFAccountManagementBanInfoWrapper{ src.Model() }
    {
    }

    PFAccountManagementBanInfoWrapper(PFAccountManagementBanInfoWrapper&& src) :
        PFAccountManagementBanInfoWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementBanInfoWrapper& operator=(PFAccountManagementBanInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementBanInfoWrapper() = default;

    friend void swap(PFAccountManagementBanInfoWrapper& lhs, PFAccountManagementBanInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_banId, rhs.m_banId);
        swap(lhs.m_created, rhs.m_created);
        swap(lhs.m_expires, rhs.m_expires);
        swap(lhs.m_IPAddress, rhs.m_IPAddress);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_reason, rhs.m_reason);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetActive(bool value)
    {
        this->m_model.active = value;
    }

    void SetBanId(String value)
    {
        m_banId = std::move(value);
        this->m_model.banId =  m_banId.empty() ? nullptr : m_banId.data();
    }

    void SetCreated(std::optional<time_t> value)
    {
        m_created = std::move(value);
        this->m_model.created = m_created ? m_created.operator->() : nullptr;
    }

    void SetExpires(std::optional<time_t> value)
    {
        m_expires = std::move(value);
        this->m_model.expires = m_expires ? m_expires.operator->() : nullptr;
    }

    void SetIPAddress(String value)
    {
        m_IPAddress = std::move(value);
        this->m_model.IPAddress =  m_IPAddress.empty() ? nullptr : m_IPAddress.data();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetReason(String value)
    {
        m_reason = std::move(value);
        this->m_model.reason =  m_reason.empty() ? nullptr : m_reason.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.banId = m_banId.empty() ? nullptr : m_banId.data();
        this->m_model.created = m_created ? m_created.operator->() : nullptr;
        this->m_model.expires = m_expires ? m_expires.operator->() : nullptr;
        this->m_model.IPAddress = m_IPAddress.empty() ? nullptr : m_IPAddress.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.reason = m_reason.empty() ? nullptr : m_reason.data();
    }

    String m_banId;
    std::optional<time_t> m_created;
    std::optional<time_t> m_expires;
    String m_IPAddress;
    String m_playFabId;
    String m_reason;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementBanUsersResultWrapper : public ModelWrapper<PFAccountManagementBanUsersResult, Alloc>
{
public:
    using ModelType = PFAccountManagementBanUsersResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementBanUsersResultWrapper() = default;

    PFAccountManagementBanUsersResultWrapper(const PFAccountManagementBanUsersResult& model) :
        ModelWrapper<PFAccountManagementBanUsersResult, Alloc>{ model },
        m_banData{ model.banData, model.banData + model.banDataCount }
    {
        SetModelPointers();
    }

    PFAccountManagementBanUsersResultWrapper(const PFAccountManagementBanUsersResultWrapper& src) :
        PFAccountManagementBanUsersResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementBanUsersResultWrapper(PFAccountManagementBanUsersResultWrapper&& src) :
        PFAccountManagementBanUsersResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementBanUsersResultWrapper& operator=(PFAccountManagementBanUsersResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementBanUsersResultWrapper() = default;

    friend void swap(PFAccountManagementBanUsersResultWrapper& lhs, PFAccountManagementBanUsersResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_banData, rhs.m_banData);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetBanData(ModelVector<PFAccountManagementBanInfoWrapper<Alloc>, Alloc> value)
    {
        m_banData = std::move(value);
        this->m_model.banData =  m_banData.empty() ? nullptr : m_banData.data();
        this->m_model.banDataCount =  static_cast<uint32_t>(m_banData.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.banData = m_banData.empty() ? nullptr : m_banData.data();
    }

    ModelVector<PFAccountManagementBanInfoWrapper<Alloc>, Alloc> m_banData;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementDeletePlayerRequestWrapper : public ModelWrapper<PFAccountManagementDeletePlayerRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementDeletePlayerRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementDeletePlayerRequestWrapper() = default;

    PFAccountManagementDeletePlayerRequestWrapper(const PFAccountManagementDeletePlayerRequest& model) :
        ModelWrapper<PFAccountManagementDeletePlayerRequest, Alloc>{ model },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFAccountManagementDeletePlayerRequestWrapper(const PFAccountManagementDeletePlayerRequestWrapper& src) :
        PFAccountManagementDeletePlayerRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementDeletePlayerRequestWrapper(PFAccountManagementDeletePlayerRequestWrapper&& src) :
        PFAccountManagementDeletePlayerRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementDeletePlayerRequestWrapper& operator=(PFAccountManagementDeletePlayerRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementDeletePlayerRequestWrapper() = default;

    friend void swap(PFAccountManagementDeletePlayerRequestWrapper& lhs, PFAccountManagementDeletePlayerRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequestWrapper : public ModelWrapper<PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequestWrapper() = default;

    PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequestWrapper(const PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequest& model) :
        ModelWrapper<PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequest, Alloc>{ model },
        m_playFabIDs{ model.playFabIDs, model.playFabIDs + model.playFabIDsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequestWrapper(const PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequestWrapper& src) :
        PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequestWrapper(PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequestWrapper&& src) :
        PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequestWrapper& operator=(PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequestWrapper() = default;

    friend void swap(PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequestWrapper& lhs, PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_playFabIDs, rhs.m_playFabIDs);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetPlayFabIDs(CStringVector<Alloc> value)
    {
        m_playFabIDs = std::move(value);
        this->m_model.playFabIDs =  m_playFabIDs.empty() ? nullptr : m_playFabIDs.data();
        this->m_model.playFabIDsCount =  static_cast<uint32_t>(m_playFabIDs.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.playFabIDs = m_playFabIDs.empty() ? nullptr : m_playFabIDs.data();
    }

    CStringVector<Alloc> m_playFabIDs;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementServerCustomIDPlayFabIDPairWrapper : public ModelWrapper<PFAccountManagementServerCustomIDPlayFabIDPair, Alloc>
{
public:
    using ModelType = PFAccountManagementServerCustomIDPlayFabIDPair;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementServerCustomIDPlayFabIDPairWrapper() = default;

    PFAccountManagementServerCustomIDPlayFabIDPairWrapper(const PFAccountManagementServerCustomIDPlayFabIDPair& model) :
        ModelWrapper<PFAccountManagementServerCustomIDPlayFabIDPair, Alloc>{ model },
        m_playFabId{ SafeString(model.playFabId) },
        m_serverCustomId{ SafeString(model.serverCustomId) }
    {
        SetModelPointers();
    }

    PFAccountManagementServerCustomIDPlayFabIDPairWrapper(const PFAccountManagementServerCustomIDPlayFabIDPairWrapper& src) :
        PFAccountManagementServerCustomIDPlayFabIDPairWrapper{ src.Model() }
    {
    }

    PFAccountManagementServerCustomIDPlayFabIDPairWrapper(PFAccountManagementServerCustomIDPlayFabIDPairWrapper&& src) :
        PFAccountManagementServerCustomIDPlayFabIDPairWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementServerCustomIDPlayFabIDPairWrapper& operator=(PFAccountManagementServerCustomIDPlayFabIDPairWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementServerCustomIDPlayFabIDPairWrapper() = default;

    friend void swap(PFAccountManagementServerCustomIDPlayFabIDPairWrapper& lhs, PFAccountManagementServerCustomIDPlayFabIDPairWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_serverCustomId, rhs.m_serverCustomId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetServerCustomId(String value)
    {
        m_serverCustomId = std::move(value);
        this->m_model.serverCustomId =  m_serverCustomId.empty() ? nullptr : m_serverCustomId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.serverCustomId = m_serverCustomId.empty() ? nullptr : m_serverCustomId.data();
    }

    String m_playFabId;
    String m_serverCustomId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetServerCustomIDsFromPlayFabIDsResultWrapper : public ModelWrapper<PFAccountManagementGetServerCustomIDsFromPlayFabIDsResult, Alloc>
{
public:
    using ModelType = PFAccountManagementGetServerCustomIDsFromPlayFabIDsResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetServerCustomIDsFromPlayFabIDsResultWrapper() = default;

    PFAccountManagementGetServerCustomIDsFromPlayFabIDsResultWrapper(const PFAccountManagementGetServerCustomIDsFromPlayFabIDsResult& model) :
        ModelWrapper<PFAccountManagementGetServerCustomIDsFromPlayFabIDsResult, Alloc>{ model },
        m_data{ model.data, model.data + model.dataCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetServerCustomIDsFromPlayFabIDsResultWrapper(const PFAccountManagementGetServerCustomIDsFromPlayFabIDsResultWrapper& src) :
        PFAccountManagementGetServerCustomIDsFromPlayFabIDsResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetServerCustomIDsFromPlayFabIDsResultWrapper(PFAccountManagementGetServerCustomIDsFromPlayFabIDsResultWrapper&& src) :
        PFAccountManagementGetServerCustomIDsFromPlayFabIDsResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetServerCustomIDsFromPlayFabIDsResultWrapper& operator=(PFAccountManagementGetServerCustomIDsFromPlayFabIDsResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetServerCustomIDsFromPlayFabIDsResultWrapper() = default;

    friend void swap(PFAccountManagementGetServerCustomIDsFromPlayFabIDsResultWrapper& lhs, PFAccountManagementGetServerCustomIDsFromPlayFabIDsResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_data, rhs.m_data);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetData(ModelVector<PFAccountManagementServerCustomIDPlayFabIDPairWrapper<Alloc>, Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
    }

    ModelVector<PFAccountManagementServerCustomIDPlayFabIDPairWrapper<Alloc>, Alloc> m_data;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetUserAccountInfoRequestWrapper : public ModelWrapper<PFAccountManagementGetUserAccountInfoRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementGetUserAccountInfoRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetUserAccountInfoRequestWrapper() = default;

    PFAccountManagementGetUserAccountInfoRequestWrapper(const PFAccountManagementGetUserAccountInfoRequest& model) :
        ModelWrapper<PFAccountManagementGetUserAccountInfoRequest, Alloc>{ model },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFAccountManagementGetUserAccountInfoRequestWrapper(const PFAccountManagementGetUserAccountInfoRequestWrapper& src) :
        PFAccountManagementGetUserAccountInfoRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetUserAccountInfoRequestWrapper(PFAccountManagementGetUserAccountInfoRequestWrapper&& src) :
        PFAccountManagementGetUserAccountInfoRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetUserAccountInfoRequestWrapper& operator=(PFAccountManagementGetUserAccountInfoRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetUserAccountInfoRequestWrapper() = default;

    friend void swap(PFAccountManagementGetUserAccountInfoRequestWrapper& lhs, PFAccountManagementGetUserAccountInfoRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetUserAccountInfoResultWrapper : public ModelWrapper<PFAccountManagementGetUserAccountInfoResult, Alloc>
{
public:
    using ModelType = PFAccountManagementGetUserAccountInfoResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetUserAccountInfoResultWrapper() = default;

    PFAccountManagementGetUserAccountInfoResultWrapper(const PFAccountManagementGetUserAccountInfoResult& model) :
        ModelWrapper<PFAccountManagementGetUserAccountInfoResult, Alloc>{ model },
        m_userInfo{ model.userInfo ? std::optional<PFUserAccountInfoWrapper<Alloc>>{ *model.userInfo } : std::nullopt }
    {
        SetModelPointers();
    }

    PFAccountManagementGetUserAccountInfoResultWrapper(const PFAccountManagementGetUserAccountInfoResultWrapper& src) :
        PFAccountManagementGetUserAccountInfoResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetUserAccountInfoResultWrapper(PFAccountManagementGetUserAccountInfoResultWrapper&& src) :
        PFAccountManagementGetUserAccountInfoResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetUserAccountInfoResultWrapper& operator=(PFAccountManagementGetUserAccountInfoResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetUserAccountInfoResultWrapper() = default;

    friend void swap(PFAccountManagementGetUserAccountInfoResultWrapper& lhs, PFAccountManagementGetUserAccountInfoResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_userInfo, rhs.m_userInfo);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetUserInfo(std::optional<PFUserAccountInfoWrapper<Alloc>> value)
    {
        m_userInfo = std::move(value);
        this->m_model.userInfo = m_userInfo ? &m_userInfo->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.userInfo = m_userInfo ?  &m_userInfo->Model() : nullptr;
    }

    std::optional<PFUserAccountInfoWrapper<Alloc>> m_userInfo;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetUserBansRequestWrapper : public ModelWrapper<PFAccountManagementGetUserBansRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementGetUserBansRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetUserBansRequestWrapper() = default;

    PFAccountManagementGetUserBansRequestWrapper(const PFAccountManagementGetUserBansRequest& model) :
        ModelWrapper<PFAccountManagementGetUserBansRequest, Alloc>{ model },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFAccountManagementGetUserBansRequestWrapper(const PFAccountManagementGetUserBansRequestWrapper& src) :
        PFAccountManagementGetUserBansRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetUserBansRequestWrapper(PFAccountManagementGetUserBansRequestWrapper&& src) :
        PFAccountManagementGetUserBansRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetUserBansRequestWrapper& operator=(PFAccountManagementGetUserBansRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetUserBansRequestWrapper() = default;

    friend void swap(PFAccountManagementGetUserBansRequestWrapper& lhs, PFAccountManagementGetUserBansRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetUserBansResultWrapper : public ModelWrapper<PFAccountManagementGetUserBansResult, Alloc>
{
public:
    using ModelType = PFAccountManagementGetUserBansResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetUserBansResultWrapper() = default;

    PFAccountManagementGetUserBansResultWrapper(const PFAccountManagementGetUserBansResult& model) :
        ModelWrapper<PFAccountManagementGetUserBansResult, Alloc>{ model },
        m_banData{ model.banData, model.banData + model.banDataCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetUserBansResultWrapper(const PFAccountManagementGetUserBansResultWrapper& src) :
        PFAccountManagementGetUserBansResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetUserBansResultWrapper(PFAccountManagementGetUserBansResultWrapper&& src) :
        PFAccountManagementGetUserBansResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetUserBansResultWrapper& operator=(PFAccountManagementGetUserBansResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetUserBansResultWrapper() = default;

    friend void swap(PFAccountManagementGetUserBansResultWrapper& lhs, PFAccountManagementGetUserBansResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_banData, rhs.m_banData);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetBanData(ModelVector<PFAccountManagementBanInfoWrapper<Alloc>, Alloc> value)
    {
        m_banData = std::move(value);
        this->m_model.banData =  m_banData.empty() ? nullptr : m_banData.data();
        this->m_model.banDataCount =  static_cast<uint32_t>(m_banData.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.banData = m_banData.empty() ? nullptr : m_banData.data();
    }

    ModelVector<PFAccountManagementBanInfoWrapper<Alloc>, Alloc> m_banData;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementServerLinkNintendoServiceAccountRequestWrapper : public ModelWrapper<PFAccountManagementServerLinkNintendoServiceAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementServerLinkNintendoServiceAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementServerLinkNintendoServiceAccountRequestWrapper() = default;

    PFAccountManagementServerLinkNintendoServiceAccountRequestWrapper(const PFAccountManagementServerLinkNintendoServiceAccountRequest& model) :
        ModelWrapper<PFAccountManagementServerLinkNintendoServiceAccountRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt },
        m_identityToken{ SafeString(model.identityToken) },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFAccountManagementServerLinkNintendoServiceAccountRequestWrapper(const PFAccountManagementServerLinkNintendoServiceAccountRequestWrapper& src) :
        PFAccountManagementServerLinkNintendoServiceAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementServerLinkNintendoServiceAccountRequestWrapper(PFAccountManagementServerLinkNintendoServiceAccountRequestWrapper&& src) :
        PFAccountManagementServerLinkNintendoServiceAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementServerLinkNintendoServiceAccountRequestWrapper& operator=(PFAccountManagementServerLinkNintendoServiceAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementServerLinkNintendoServiceAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementServerLinkNintendoServiceAccountRequestWrapper& lhs, PFAccountManagementServerLinkNintendoServiceAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        swap(lhs.m_identityToken, rhs.m_identityToken);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    void SetIdentityToken(String value)
    {
        m_identityToken = std::move(value);
        this->m_model.identityToken =  m_identityToken.empty() ? nullptr : m_identityToken.data();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
        this->m_model.identityToken = m_identityToken.empty() ? nullptr : m_identityToken.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceLink;
    String m_identityToken;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementLinkNintendoServiceAccountSubjectRequestWrapper : public ModelWrapper<PFAccountManagementLinkNintendoServiceAccountSubjectRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementLinkNintendoServiceAccountSubjectRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementLinkNintendoServiceAccountSubjectRequestWrapper() = default;

    PFAccountManagementLinkNintendoServiceAccountSubjectRequestWrapper(const PFAccountManagementLinkNintendoServiceAccountSubjectRequest& model) :
        ModelWrapper<PFAccountManagementLinkNintendoServiceAccountSubjectRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt },
        m_playFabId{ SafeString(model.playFabId) },
        m_subject{ SafeString(model.subject) }
    {
        SetModelPointers();
    }

    PFAccountManagementLinkNintendoServiceAccountSubjectRequestWrapper(const PFAccountManagementLinkNintendoServiceAccountSubjectRequestWrapper& src) :
        PFAccountManagementLinkNintendoServiceAccountSubjectRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementLinkNintendoServiceAccountSubjectRequestWrapper(PFAccountManagementLinkNintendoServiceAccountSubjectRequestWrapper&& src) :
        PFAccountManagementLinkNintendoServiceAccountSubjectRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementLinkNintendoServiceAccountSubjectRequestWrapper& operator=(PFAccountManagementLinkNintendoServiceAccountSubjectRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementLinkNintendoServiceAccountSubjectRequestWrapper() = default;

    friend void swap(PFAccountManagementLinkNintendoServiceAccountSubjectRequestWrapper& lhs, PFAccountManagementLinkNintendoServiceAccountSubjectRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_subject, rhs.m_subject);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetSubject(String value)
    {
        m_subject = std::move(value);
        this->m_model.subject =  m_subject.empty() ? nullptr : m_subject.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.subject = m_subject.empty() ? nullptr : m_subject.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceLink;
    String m_playFabId;
    String m_subject;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementServerLinkNintendoSwitchDeviceIdRequestWrapper : public ModelWrapper<PFAccountManagementServerLinkNintendoSwitchDeviceIdRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementServerLinkNintendoSwitchDeviceIdRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementServerLinkNintendoSwitchDeviceIdRequestWrapper() = default;

    PFAccountManagementServerLinkNintendoSwitchDeviceIdRequestWrapper(const PFAccountManagementServerLinkNintendoSwitchDeviceIdRequest& model) :
        ModelWrapper<PFAccountManagementServerLinkNintendoSwitchDeviceIdRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt },
        m_nintendoSwitchDeviceId{ SafeString(model.nintendoSwitchDeviceId) },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFAccountManagementServerLinkNintendoSwitchDeviceIdRequestWrapper(const PFAccountManagementServerLinkNintendoSwitchDeviceIdRequestWrapper& src) :
        PFAccountManagementServerLinkNintendoSwitchDeviceIdRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementServerLinkNintendoSwitchDeviceIdRequestWrapper(PFAccountManagementServerLinkNintendoSwitchDeviceIdRequestWrapper&& src) :
        PFAccountManagementServerLinkNintendoSwitchDeviceIdRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementServerLinkNintendoSwitchDeviceIdRequestWrapper& operator=(PFAccountManagementServerLinkNintendoSwitchDeviceIdRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementServerLinkNintendoSwitchDeviceIdRequestWrapper() = default;

    friend void swap(PFAccountManagementServerLinkNintendoSwitchDeviceIdRequestWrapper& lhs, PFAccountManagementServerLinkNintendoSwitchDeviceIdRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        swap(lhs.m_nintendoSwitchDeviceId, rhs.m_nintendoSwitchDeviceId);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    void SetNintendoSwitchDeviceId(String value)
    {
        m_nintendoSwitchDeviceId = std::move(value);
        this->m_model.nintendoSwitchDeviceId =  m_nintendoSwitchDeviceId.empty() ? nullptr : m_nintendoSwitchDeviceId.data();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
        this->m_model.nintendoSwitchDeviceId = m_nintendoSwitchDeviceId.empty() ? nullptr : m_nintendoSwitchDeviceId.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceLink;
    String m_nintendoSwitchDeviceId;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementServerLinkPSNAccountRequestWrapper : public ModelWrapper<PFAccountManagementServerLinkPSNAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementServerLinkPSNAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementServerLinkPSNAccountRequestWrapper() = default;

    PFAccountManagementServerLinkPSNAccountRequestWrapper(const PFAccountManagementServerLinkPSNAccountRequest& model) :
        ModelWrapper<PFAccountManagementServerLinkPSNAccountRequest, Alloc>{ model },
        m_authCode{ SafeString(model.authCode) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt },
        m_issuerId{ model.issuerId ? std::optional<int32_t>{ *model.issuerId } : std::nullopt },
        m_playFabId{ SafeString(model.playFabId) },
        m_redirectUri{ SafeString(model.redirectUri) }
    {
        SetModelPointers();
    }

    PFAccountManagementServerLinkPSNAccountRequestWrapper(const PFAccountManagementServerLinkPSNAccountRequestWrapper& src) :
        PFAccountManagementServerLinkPSNAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementServerLinkPSNAccountRequestWrapper(PFAccountManagementServerLinkPSNAccountRequestWrapper&& src) :
        PFAccountManagementServerLinkPSNAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementServerLinkPSNAccountRequestWrapper& operator=(PFAccountManagementServerLinkPSNAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementServerLinkPSNAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementServerLinkPSNAccountRequestWrapper& lhs, PFAccountManagementServerLinkPSNAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_authCode, rhs.m_authCode);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        swap(lhs.m_issuerId, rhs.m_issuerId);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_redirectUri, rhs.m_redirectUri);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAuthCode(String value)
    {
        m_authCode = std::move(value);
        this->m_model.authCode =  m_authCode.empty() ? nullptr : m_authCode.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    void SetIssuerId(std::optional<int32_t> value)
    {
        m_issuerId = std::move(value);
        this->m_model.issuerId = m_issuerId ? m_issuerId.operator->() : nullptr;
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetRedirectUri(String value)
    {
        m_redirectUri = std::move(value);
        this->m_model.redirectUri =  m_redirectUri.empty() ? nullptr : m_redirectUri.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.authCode = m_authCode.empty() ? nullptr : m_authCode.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
        this->m_model.issuerId = m_issuerId ? m_issuerId.operator->() : nullptr;
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.redirectUri = m_redirectUri.empty() ? nullptr : m_redirectUri.data();
    }

    String m_authCode;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceLink;
    std::optional<int32_t> m_issuerId;
    String m_playFabId;
    String m_redirectUri;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementLinkPSNIdRequestWrapper : public ModelWrapper<PFAccountManagementLinkPSNIdRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementLinkPSNIdRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementLinkPSNIdRequestWrapper() = default;

    PFAccountManagementLinkPSNIdRequestWrapper(const PFAccountManagementLinkPSNIdRequest& model) :
        ModelWrapper<PFAccountManagementLinkPSNIdRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt },
        m_issuerId{ model.issuerId ? std::optional<int32_t>{ *model.issuerId } : std::nullopt },
        m_playFabId{ SafeString(model.playFabId) },
        m_PSNUserId{ SafeString(model.PSNUserId) }
    {
        SetModelPointers();
    }

    PFAccountManagementLinkPSNIdRequestWrapper(const PFAccountManagementLinkPSNIdRequestWrapper& src) :
        PFAccountManagementLinkPSNIdRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementLinkPSNIdRequestWrapper(PFAccountManagementLinkPSNIdRequestWrapper&& src) :
        PFAccountManagementLinkPSNIdRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementLinkPSNIdRequestWrapper& operator=(PFAccountManagementLinkPSNIdRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementLinkPSNIdRequestWrapper() = default;

    friend void swap(PFAccountManagementLinkPSNIdRequestWrapper& lhs, PFAccountManagementLinkPSNIdRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        swap(lhs.m_issuerId, rhs.m_issuerId);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_PSNUserId, rhs.m_PSNUserId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    void SetIssuerId(std::optional<int32_t> value)
    {
        m_issuerId = std::move(value);
        this->m_model.issuerId = m_issuerId ? m_issuerId.operator->() : nullptr;
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetPSNUserId(String value)
    {
        m_PSNUserId = std::move(value);
        this->m_model.PSNUserId =  m_PSNUserId.empty() ? nullptr : m_PSNUserId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
        this->m_model.issuerId = m_issuerId ? m_issuerId.operator->() : nullptr;
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.PSNUserId = m_PSNUserId.empty() ? nullptr : m_PSNUserId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceLink;
    std::optional<int32_t> m_issuerId;
    String m_playFabId;
    String m_PSNUserId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementLinkServerCustomIdRequestWrapper : public ModelWrapper<PFAccountManagementLinkServerCustomIdRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementLinkServerCustomIdRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementLinkServerCustomIdRequestWrapper() = default;

    PFAccountManagementLinkServerCustomIdRequestWrapper(const PFAccountManagementLinkServerCustomIdRequest& model) :
        ModelWrapper<PFAccountManagementLinkServerCustomIdRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt },
        m_playFabId{ SafeString(model.playFabId) },
        m_serverCustomId{ SafeString(model.serverCustomId) }
    {
        SetModelPointers();
    }

    PFAccountManagementLinkServerCustomIdRequestWrapper(const PFAccountManagementLinkServerCustomIdRequestWrapper& src) :
        PFAccountManagementLinkServerCustomIdRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementLinkServerCustomIdRequestWrapper(PFAccountManagementLinkServerCustomIdRequestWrapper&& src) :
        PFAccountManagementLinkServerCustomIdRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementLinkServerCustomIdRequestWrapper& operator=(PFAccountManagementLinkServerCustomIdRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementLinkServerCustomIdRequestWrapper() = default;

    friend void swap(PFAccountManagementLinkServerCustomIdRequestWrapper& lhs, PFAccountManagementLinkServerCustomIdRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_serverCustomId, rhs.m_serverCustomId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetServerCustomId(String value)
    {
        m_serverCustomId = std::move(value);
        this->m_model.serverCustomId =  m_serverCustomId.empty() ? nullptr : m_serverCustomId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.serverCustomId = m_serverCustomId.empty() ? nullptr : m_serverCustomId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceLink;
    String m_playFabId;
    String m_serverCustomId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementLinkSteamIdRequestWrapper : public ModelWrapper<PFAccountManagementLinkSteamIdRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementLinkSteamIdRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementLinkSteamIdRequestWrapper() = default;

    PFAccountManagementLinkSteamIdRequestWrapper(const PFAccountManagementLinkSteamIdRequest& model) :
        ModelWrapper<PFAccountManagementLinkSteamIdRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt },
        m_playFabId{ SafeString(model.playFabId) },
        m_steamId{ SafeString(model.steamId) }
    {
        SetModelPointers();
    }

    PFAccountManagementLinkSteamIdRequestWrapper(const PFAccountManagementLinkSteamIdRequestWrapper& src) :
        PFAccountManagementLinkSteamIdRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementLinkSteamIdRequestWrapper(PFAccountManagementLinkSteamIdRequestWrapper&& src) :
        PFAccountManagementLinkSteamIdRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementLinkSteamIdRequestWrapper& operator=(PFAccountManagementLinkSteamIdRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementLinkSteamIdRequestWrapper() = default;

    friend void swap(PFAccountManagementLinkSteamIdRequestWrapper& lhs, PFAccountManagementLinkSteamIdRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_steamId, rhs.m_steamId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetSteamId(String value)
    {
        m_steamId = std::move(value);
        this->m_model.steamId =  m_steamId.empty() ? nullptr : m_steamId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.steamId = m_steamId.empty() ? nullptr : m_steamId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceLink;
    String m_playFabId;
    String m_steamId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementServerLinkXboxAccountRequestWrapper : public ModelWrapper<PFAccountManagementServerLinkXboxAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementServerLinkXboxAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementServerLinkXboxAccountRequestWrapper() = default;

    PFAccountManagementServerLinkXboxAccountRequestWrapper(const PFAccountManagementServerLinkXboxAccountRequest& model) :
        ModelWrapper<PFAccountManagementServerLinkXboxAccountRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceLink{ model.forceLink ? std::optional<bool>{ *model.forceLink } : std::nullopt },
        m_playFabId{ SafeString(model.playFabId) },
        m_xboxToken{ SafeString(model.xboxToken) }
    {
        SetModelPointers();
    }

    PFAccountManagementServerLinkXboxAccountRequestWrapper(const PFAccountManagementServerLinkXboxAccountRequestWrapper& src) :
        PFAccountManagementServerLinkXboxAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementServerLinkXboxAccountRequestWrapper(PFAccountManagementServerLinkXboxAccountRequestWrapper&& src) :
        PFAccountManagementServerLinkXboxAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementServerLinkXboxAccountRequestWrapper& operator=(PFAccountManagementServerLinkXboxAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementServerLinkXboxAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementServerLinkXboxAccountRequestWrapper& lhs, PFAccountManagementServerLinkXboxAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceLink, rhs.m_forceLink);
        swap(lhs.m_playFabId, rhs.m_playFabId);
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

    void SetForceLink(std::optional<bool> value)
    {
        m_forceLink = std::move(value);
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
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
        this->m_model.forceLink = m_forceLink ? m_forceLink.operator->() : nullptr;
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.xboxToken = m_xboxToken.empty() ? nullptr : m_xboxToken.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceLink;
    String m_playFabId;
    String m_xboxToken;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementRevokeAllBansForUserRequestWrapper : public ModelWrapper<PFAccountManagementRevokeAllBansForUserRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementRevokeAllBansForUserRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementRevokeAllBansForUserRequestWrapper() = default;

    PFAccountManagementRevokeAllBansForUserRequestWrapper(const PFAccountManagementRevokeAllBansForUserRequest& model) :
        ModelWrapper<PFAccountManagementRevokeAllBansForUserRequest, Alloc>{ model },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFAccountManagementRevokeAllBansForUserRequestWrapper(const PFAccountManagementRevokeAllBansForUserRequestWrapper& src) :
        PFAccountManagementRevokeAllBansForUserRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementRevokeAllBansForUserRequestWrapper(PFAccountManagementRevokeAllBansForUserRequestWrapper&& src) :
        PFAccountManagementRevokeAllBansForUserRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementRevokeAllBansForUserRequestWrapper& operator=(PFAccountManagementRevokeAllBansForUserRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementRevokeAllBansForUserRequestWrapper() = default;

    friend void swap(PFAccountManagementRevokeAllBansForUserRequestWrapper& lhs, PFAccountManagementRevokeAllBansForUserRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementRevokeAllBansForUserResultWrapper : public ModelWrapper<PFAccountManagementRevokeAllBansForUserResult, Alloc>
{
public:
    using ModelType = PFAccountManagementRevokeAllBansForUserResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementRevokeAllBansForUserResultWrapper() = default;

    PFAccountManagementRevokeAllBansForUserResultWrapper(const PFAccountManagementRevokeAllBansForUserResult& model) :
        ModelWrapper<PFAccountManagementRevokeAllBansForUserResult, Alloc>{ model },
        m_banData{ model.banData, model.banData + model.banDataCount }
    {
        SetModelPointers();
    }

    PFAccountManagementRevokeAllBansForUserResultWrapper(const PFAccountManagementRevokeAllBansForUserResultWrapper& src) :
        PFAccountManagementRevokeAllBansForUserResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementRevokeAllBansForUserResultWrapper(PFAccountManagementRevokeAllBansForUserResultWrapper&& src) :
        PFAccountManagementRevokeAllBansForUserResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementRevokeAllBansForUserResultWrapper& operator=(PFAccountManagementRevokeAllBansForUserResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementRevokeAllBansForUserResultWrapper() = default;

    friend void swap(PFAccountManagementRevokeAllBansForUserResultWrapper& lhs, PFAccountManagementRevokeAllBansForUserResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_banData, rhs.m_banData);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetBanData(ModelVector<PFAccountManagementBanInfoWrapper<Alloc>, Alloc> value)
    {
        m_banData = std::move(value);
        this->m_model.banData =  m_banData.empty() ? nullptr : m_banData.data();
        this->m_model.banDataCount =  static_cast<uint32_t>(m_banData.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.banData = m_banData.empty() ? nullptr : m_banData.data();
    }

    ModelVector<PFAccountManagementBanInfoWrapper<Alloc>, Alloc> m_banData;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementRevokeBansRequestWrapper : public ModelWrapper<PFAccountManagementRevokeBansRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementRevokeBansRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementRevokeBansRequestWrapper() = default;

    PFAccountManagementRevokeBansRequestWrapper(const PFAccountManagementRevokeBansRequest& model) :
        ModelWrapper<PFAccountManagementRevokeBansRequest, Alloc>{ model },
        m_banIds{ model.banIds, model.banIds + model.banIdsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementRevokeBansRequestWrapper(const PFAccountManagementRevokeBansRequestWrapper& src) :
        PFAccountManagementRevokeBansRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementRevokeBansRequestWrapper(PFAccountManagementRevokeBansRequestWrapper&& src) :
        PFAccountManagementRevokeBansRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementRevokeBansRequestWrapper& operator=(PFAccountManagementRevokeBansRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementRevokeBansRequestWrapper() = default;

    friend void swap(PFAccountManagementRevokeBansRequestWrapper& lhs, PFAccountManagementRevokeBansRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_banIds, rhs.m_banIds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetBanIds(CStringVector<Alloc> value)
    {
        m_banIds = std::move(value);
        this->m_model.banIds =  m_banIds.empty() ? nullptr : m_banIds.data();
        this->m_model.banIdsCount =  static_cast<uint32_t>(m_banIds.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.banIds = m_banIds.empty() ? nullptr : m_banIds.data();
    }

    CStringVector<Alloc> m_banIds;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementRevokeBansResultWrapper : public ModelWrapper<PFAccountManagementRevokeBansResult, Alloc>
{
public:
    using ModelType = PFAccountManagementRevokeBansResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementRevokeBansResultWrapper() = default;

    PFAccountManagementRevokeBansResultWrapper(const PFAccountManagementRevokeBansResult& model) :
        ModelWrapper<PFAccountManagementRevokeBansResult, Alloc>{ model },
        m_banData{ model.banData, model.banData + model.banDataCount }
    {
        SetModelPointers();
    }

    PFAccountManagementRevokeBansResultWrapper(const PFAccountManagementRevokeBansResultWrapper& src) :
        PFAccountManagementRevokeBansResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementRevokeBansResultWrapper(PFAccountManagementRevokeBansResultWrapper&& src) :
        PFAccountManagementRevokeBansResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementRevokeBansResultWrapper& operator=(PFAccountManagementRevokeBansResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementRevokeBansResultWrapper() = default;

    friend void swap(PFAccountManagementRevokeBansResultWrapper& lhs, PFAccountManagementRevokeBansResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_banData, rhs.m_banData);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetBanData(ModelVector<PFAccountManagementBanInfoWrapper<Alloc>, Alloc> value)
    {
        m_banData = std::move(value);
        this->m_model.banData =  m_banData.empty() ? nullptr : m_banData.data();
        this->m_model.banDataCount =  static_cast<uint32_t>(m_banData.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.banData = m_banData.empty() ? nullptr : m_banData.data();
    }

    ModelVector<PFAccountManagementBanInfoWrapper<Alloc>, Alloc> m_banData;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementSendCustomAccountRecoveryEmailRequestWrapper : public ModelWrapper<PFAccountManagementSendCustomAccountRecoveryEmailRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementSendCustomAccountRecoveryEmailRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementSendCustomAccountRecoveryEmailRequestWrapper() = default;

    PFAccountManagementSendCustomAccountRecoveryEmailRequestWrapper(const PFAccountManagementSendCustomAccountRecoveryEmailRequest& model) :
        ModelWrapper<PFAccountManagementSendCustomAccountRecoveryEmailRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_email{ SafeString(model.email) },
        m_emailTemplateId{ SafeString(model.emailTemplateId) },
        m_username{ SafeString(model.username) }
    {
        SetModelPointers();
    }

    PFAccountManagementSendCustomAccountRecoveryEmailRequestWrapper(const PFAccountManagementSendCustomAccountRecoveryEmailRequestWrapper& src) :
        PFAccountManagementSendCustomAccountRecoveryEmailRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementSendCustomAccountRecoveryEmailRequestWrapper(PFAccountManagementSendCustomAccountRecoveryEmailRequestWrapper&& src) :
        PFAccountManagementSendCustomAccountRecoveryEmailRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementSendCustomAccountRecoveryEmailRequestWrapper& operator=(PFAccountManagementSendCustomAccountRecoveryEmailRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementSendCustomAccountRecoveryEmailRequestWrapper() = default;

    friend void swap(PFAccountManagementSendCustomAccountRecoveryEmailRequestWrapper& lhs, PFAccountManagementSendCustomAccountRecoveryEmailRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_email, rhs.m_email);
        swap(lhs.m_emailTemplateId, rhs.m_emailTemplateId);
        swap(lhs.m_username, rhs.m_username);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEmail(String value)
    {
        m_email = std::move(value);
        this->m_model.email =  m_email.empty() ? nullptr : m_email.data();
    }

    void SetEmailTemplateId(String value)
    {
        m_emailTemplateId = std::move(value);
        this->m_model.emailTemplateId =  m_emailTemplateId.empty() ? nullptr : m_emailTemplateId.data();
    }

    void SetUsername(String value)
    {
        m_username = std::move(value);
        this->m_model.username =  m_username.empty() ? nullptr : m_username.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.email = m_email.empty() ? nullptr : m_email.data();
        this->m_model.emailTemplateId = m_emailTemplateId.empty() ? nullptr : m_emailTemplateId.data();
        this->m_model.username = m_username.empty() ? nullptr : m_username.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_email;
    String m_emailTemplateId;
    String m_username;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementSendEmailFromTemplateRequestWrapper : public ModelWrapper<PFAccountManagementSendEmailFromTemplateRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementSendEmailFromTemplateRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementSendEmailFromTemplateRequestWrapper() = default;

    PFAccountManagementSendEmailFromTemplateRequestWrapper(const PFAccountManagementSendEmailFromTemplateRequest& model) :
        ModelWrapper<PFAccountManagementSendEmailFromTemplateRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_emailTemplateId{ SafeString(model.emailTemplateId) },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFAccountManagementSendEmailFromTemplateRequestWrapper(const PFAccountManagementSendEmailFromTemplateRequestWrapper& src) :
        PFAccountManagementSendEmailFromTemplateRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementSendEmailFromTemplateRequestWrapper(PFAccountManagementSendEmailFromTemplateRequestWrapper&& src) :
        PFAccountManagementSendEmailFromTemplateRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementSendEmailFromTemplateRequestWrapper& operator=(PFAccountManagementSendEmailFromTemplateRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementSendEmailFromTemplateRequestWrapper() = default;

    friend void swap(PFAccountManagementSendEmailFromTemplateRequestWrapper& lhs, PFAccountManagementSendEmailFromTemplateRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_emailTemplateId, rhs.m_emailTemplateId);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEmailTemplateId(String value)
    {
        m_emailTemplateId = std::move(value);
        this->m_model.emailTemplateId =  m_emailTemplateId.empty() ? nullptr : m_emailTemplateId.data();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.emailTemplateId = m_emailTemplateId.empty() ? nullptr : m_emailTemplateId.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_emailTemplateId;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementServerUnlinkNintendoServiceAccountRequestWrapper : public ModelWrapper<PFAccountManagementServerUnlinkNintendoServiceAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementServerUnlinkNintendoServiceAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementServerUnlinkNintendoServiceAccountRequestWrapper() = default;

    PFAccountManagementServerUnlinkNintendoServiceAccountRequestWrapper(const PFAccountManagementServerUnlinkNintendoServiceAccountRequest& model) :
        ModelWrapper<PFAccountManagementServerUnlinkNintendoServiceAccountRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFAccountManagementServerUnlinkNintendoServiceAccountRequestWrapper(const PFAccountManagementServerUnlinkNintendoServiceAccountRequestWrapper& src) :
        PFAccountManagementServerUnlinkNintendoServiceAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementServerUnlinkNintendoServiceAccountRequestWrapper(PFAccountManagementServerUnlinkNintendoServiceAccountRequestWrapper&& src) :
        PFAccountManagementServerUnlinkNintendoServiceAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementServerUnlinkNintendoServiceAccountRequestWrapper& operator=(PFAccountManagementServerUnlinkNintendoServiceAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementServerUnlinkNintendoServiceAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementServerUnlinkNintendoServiceAccountRequestWrapper& lhs, PFAccountManagementServerUnlinkNintendoServiceAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequestWrapper : public ModelWrapper<PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequestWrapper() = default;

    PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequestWrapper(const PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequest& model) :
        ModelWrapper<PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_nintendoSwitchDeviceId{ SafeString(model.nintendoSwitchDeviceId) },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequestWrapper(const PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequestWrapper& src) :
        PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequestWrapper(PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequestWrapper&& src) :
        PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequestWrapper& operator=(PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequestWrapper() = default;

    friend void swap(PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequestWrapper& lhs, PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_nintendoSwitchDeviceId, rhs.m_nintendoSwitchDeviceId);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetNintendoSwitchDeviceId(String value)
    {
        m_nintendoSwitchDeviceId = std::move(value);
        this->m_model.nintendoSwitchDeviceId =  m_nintendoSwitchDeviceId.empty() ? nullptr : m_nintendoSwitchDeviceId.data();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.nintendoSwitchDeviceId = m_nintendoSwitchDeviceId.empty() ? nullptr : m_nintendoSwitchDeviceId.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_nintendoSwitchDeviceId;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementServerUnlinkPSNAccountRequestWrapper : public ModelWrapper<PFAccountManagementServerUnlinkPSNAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementServerUnlinkPSNAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementServerUnlinkPSNAccountRequestWrapper() = default;

    PFAccountManagementServerUnlinkPSNAccountRequestWrapper(const PFAccountManagementServerUnlinkPSNAccountRequest& model) :
        ModelWrapper<PFAccountManagementServerUnlinkPSNAccountRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFAccountManagementServerUnlinkPSNAccountRequestWrapper(const PFAccountManagementServerUnlinkPSNAccountRequestWrapper& src) :
        PFAccountManagementServerUnlinkPSNAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementServerUnlinkPSNAccountRequestWrapper(PFAccountManagementServerUnlinkPSNAccountRequestWrapper&& src) :
        PFAccountManagementServerUnlinkPSNAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementServerUnlinkPSNAccountRequestWrapper& operator=(PFAccountManagementServerUnlinkPSNAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementServerUnlinkPSNAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementServerUnlinkPSNAccountRequestWrapper& lhs, PFAccountManagementServerUnlinkPSNAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementUnlinkServerCustomIdRequestWrapper : public ModelWrapper<PFAccountManagementUnlinkServerCustomIdRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementUnlinkServerCustomIdRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementUnlinkServerCustomIdRequestWrapper() = default;

    PFAccountManagementUnlinkServerCustomIdRequestWrapper(const PFAccountManagementUnlinkServerCustomIdRequest& model) :
        ModelWrapper<PFAccountManagementUnlinkServerCustomIdRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_playFabId{ SafeString(model.playFabId) },
        m_serverCustomId{ SafeString(model.serverCustomId) }
    {
        SetModelPointers();
    }

    PFAccountManagementUnlinkServerCustomIdRequestWrapper(const PFAccountManagementUnlinkServerCustomIdRequestWrapper& src) :
        PFAccountManagementUnlinkServerCustomIdRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementUnlinkServerCustomIdRequestWrapper(PFAccountManagementUnlinkServerCustomIdRequestWrapper&& src) :
        PFAccountManagementUnlinkServerCustomIdRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementUnlinkServerCustomIdRequestWrapper& operator=(PFAccountManagementUnlinkServerCustomIdRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementUnlinkServerCustomIdRequestWrapper() = default;

    friend void swap(PFAccountManagementUnlinkServerCustomIdRequestWrapper& lhs, PFAccountManagementUnlinkServerCustomIdRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_serverCustomId, rhs.m_serverCustomId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetServerCustomId(String value)
    {
        m_serverCustomId = std::move(value);
        this->m_model.serverCustomId =  m_serverCustomId.empty() ? nullptr : m_serverCustomId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.serverCustomId = m_serverCustomId.empty() ? nullptr : m_serverCustomId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_playFabId;
    String m_serverCustomId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementUnlinkSteamIdRequestWrapper : public ModelWrapper<PFAccountManagementUnlinkSteamIdRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementUnlinkSteamIdRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementUnlinkSteamIdRequestWrapper() = default;

    PFAccountManagementUnlinkSteamIdRequestWrapper(const PFAccountManagementUnlinkSteamIdRequest& model) :
        ModelWrapper<PFAccountManagementUnlinkSteamIdRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFAccountManagementUnlinkSteamIdRequestWrapper(const PFAccountManagementUnlinkSteamIdRequestWrapper& src) :
        PFAccountManagementUnlinkSteamIdRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementUnlinkSteamIdRequestWrapper(PFAccountManagementUnlinkSteamIdRequestWrapper&& src) :
        PFAccountManagementUnlinkSteamIdRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementUnlinkSteamIdRequestWrapper& operator=(PFAccountManagementUnlinkSteamIdRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementUnlinkSteamIdRequestWrapper() = default;

    friend void swap(PFAccountManagementUnlinkSteamIdRequestWrapper& lhs, PFAccountManagementUnlinkSteamIdRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementServerUnlinkXboxAccountRequestWrapper : public ModelWrapper<PFAccountManagementServerUnlinkXboxAccountRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementServerUnlinkXboxAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementServerUnlinkXboxAccountRequestWrapper() = default;

    PFAccountManagementServerUnlinkXboxAccountRequestWrapper(const PFAccountManagementServerUnlinkXboxAccountRequest& model) :
        ModelWrapper<PFAccountManagementServerUnlinkXboxAccountRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFAccountManagementServerUnlinkXboxAccountRequestWrapper(const PFAccountManagementServerUnlinkXboxAccountRequestWrapper& src) :
        PFAccountManagementServerUnlinkXboxAccountRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementServerUnlinkXboxAccountRequestWrapper(PFAccountManagementServerUnlinkXboxAccountRequestWrapper&& src) :
        PFAccountManagementServerUnlinkXboxAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementServerUnlinkXboxAccountRequestWrapper& operator=(PFAccountManagementServerUnlinkXboxAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementServerUnlinkXboxAccountRequestWrapper() = default;

    friend void swap(PFAccountManagementServerUnlinkXboxAccountRequestWrapper& lhs, PFAccountManagementServerUnlinkXboxAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementServerUpdateAvatarUrlRequestWrapper : public ModelWrapper<PFAccountManagementServerUpdateAvatarUrlRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementServerUpdateAvatarUrlRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementServerUpdateAvatarUrlRequestWrapper() = default;

    PFAccountManagementServerUpdateAvatarUrlRequestWrapper(const PFAccountManagementServerUpdateAvatarUrlRequest& model) :
        ModelWrapper<PFAccountManagementServerUpdateAvatarUrlRequest, Alloc>{ model },
        m_imageUrl{ SafeString(model.imageUrl) },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFAccountManagementServerUpdateAvatarUrlRequestWrapper(const PFAccountManagementServerUpdateAvatarUrlRequestWrapper& src) :
        PFAccountManagementServerUpdateAvatarUrlRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementServerUpdateAvatarUrlRequestWrapper(PFAccountManagementServerUpdateAvatarUrlRequestWrapper&& src) :
        PFAccountManagementServerUpdateAvatarUrlRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementServerUpdateAvatarUrlRequestWrapper& operator=(PFAccountManagementServerUpdateAvatarUrlRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementServerUpdateAvatarUrlRequestWrapper() = default;

    friend void swap(PFAccountManagementServerUpdateAvatarUrlRequestWrapper& lhs, PFAccountManagementServerUpdateAvatarUrlRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_imageUrl, rhs.m_imageUrl);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetImageUrl(String value)
    {
        m_imageUrl = std::move(value);
        this->m_model.imageUrl =  m_imageUrl.empty() ? nullptr : m_imageUrl.data();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.imageUrl = m_imageUrl.empty() ? nullptr : m_imageUrl.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String m_imageUrl;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementUpdateBanRequestWrapper : public ModelWrapper<PFAccountManagementUpdateBanRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementUpdateBanRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementUpdateBanRequestWrapper() = default;

    PFAccountManagementUpdateBanRequestWrapper(const PFAccountManagementUpdateBanRequest& model) :
        ModelWrapper<PFAccountManagementUpdateBanRequest, Alloc>{ model },
        m_active{ model.active ? std::optional<bool>{ *model.active } : std::nullopt },
        m_banId{ SafeString(model.banId) },
        m_expires{ model.expires ? std::optional<time_t>{ *model.expires } : std::nullopt },
        m_IPAddress{ SafeString(model.IPAddress) },
        m_permanent{ model.permanent ? std::optional<bool>{ *model.permanent } : std::nullopt },
        m_reason{ SafeString(model.reason) }
    {
        SetModelPointers();
    }

    PFAccountManagementUpdateBanRequestWrapper(const PFAccountManagementUpdateBanRequestWrapper& src) :
        PFAccountManagementUpdateBanRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementUpdateBanRequestWrapper(PFAccountManagementUpdateBanRequestWrapper&& src) :
        PFAccountManagementUpdateBanRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementUpdateBanRequestWrapper& operator=(PFAccountManagementUpdateBanRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementUpdateBanRequestWrapper() = default;

    friend void swap(PFAccountManagementUpdateBanRequestWrapper& lhs, PFAccountManagementUpdateBanRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_active, rhs.m_active);
        swap(lhs.m_banId, rhs.m_banId);
        swap(lhs.m_expires, rhs.m_expires);
        swap(lhs.m_IPAddress, rhs.m_IPAddress);
        swap(lhs.m_permanent, rhs.m_permanent);
        swap(lhs.m_reason, rhs.m_reason);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetActive(std::optional<bool> value)
    {
        m_active = std::move(value);
        this->m_model.active = m_active ? m_active.operator->() : nullptr;
    }

    void SetBanId(String value)
    {
        m_banId = std::move(value);
        this->m_model.banId =  m_banId.empty() ? nullptr : m_banId.data();
    }

    void SetExpires(std::optional<time_t> value)
    {
        m_expires = std::move(value);
        this->m_model.expires = m_expires ? m_expires.operator->() : nullptr;
    }

    void SetIPAddress(String value)
    {
        m_IPAddress = std::move(value);
        this->m_model.IPAddress =  m_IPAddress.empty() ? nullptr : m_IPAddress.data();
    }

    void SetPermanent(std::optional<bool> value)
    {
        m_permanent = std::move(value);
        this->m_model.permanent = m_permanent ? m_permanent.operator->() : nullptr;
    }

    void SetReason(String value)
    {
        m_reason = std::move(value);
        this->m_model.reason =  m_reason.empty() ? nullptr : m_reason.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.active = m_active ? m_active.operator->() : nullptr;
        this->m_model.banId = m_banId.empty() ? nullptr : m_banId.data();
        this->m_model.expires = m_expires ? m_expires.operator->() : nullptr;
        this->m_model.IPAddress = m_IPAddress.empty() ? nullptr : m_IPAddress.data();
        this->m_model.permanent = m_permanent ? m_permanent.operator->() : nullptr;
        this->m_model.reason = m_reason.empty() ? nullptr : m_reason.data();
    }

    std::optional<bool> m_active;
    String m_banId;
    std::optional<time_t> m_expires;
    String m_IPAddress;
    std::optional<bool> m_permanent;
    String m_reason;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementUpdateBansRequestWrapper : public ModelWrapper<PFAccountManagementUpdateBansRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementUpdateBansRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementUpdateBansRequestWrapper() = default;

    PFAccountManagementUpdateBansRequestWrapper(const PFAccountManagementUpdateBansRequest& model) :
        ModelWrapper<PFAccountManagementUpdateBansRequest, Alloc>{ model },
        m_bans{ model.bans, model.bans + model.bansCount }
    {
        SetModelPointers();
    }

    PFAccountManagementUpdateBansRequestWrapper(const PFAccountManagementUpdateBansRequestWrapper& src) :
        PFAccountManagementUpdateBansRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementUpdateBansRequestWrapper(PFAccountManagementUpdateBansRequestWrapper&& src) :
        PFAccountManagementUpdateBansRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementUpdateBansRequestWrapper& operator=(PFAccountManagementUpdateBansRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementUpdateBansRequestWrapper() = default;

    friend void swap(PFAccountManagementUpdateBansRequestWrapper& lhs, PFAccountManagementUpdateBansRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_bans, rhs.m_bans);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetBans(ModelVector<PFAccountManagementUpdateBanRequestWrapper<Alloc>, Alloc> value)
    {
        m_bans = std::move(value);
        this->m_model.bans =  m_bans.empty() ? nullptr : m_bans.data();
        this->m_model.bansCount =  static_cast<uint32_t>(m_bans.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.bans = m_bans.empty() ? nullptr : m_bans.data();
    }

    ModelVector<PFAccountManagementUpdateBanRequestWrapper<Alloc>, Alloc> m_bans;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementUpdateBansResultWrapper : public ModelWrapper<PFAccountManagementUpdateBansResult, Alloc>
{
public:
    using ModelType = PFAccountManagementUpdateBansResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementUpdateBansResultWrapper() = default;

    PFAccountManagementUpdateBansResultWrapper(const PFAccountManagementUpdateBansResult& model) :
        ModelWrapper<PFAccountManagementUpdateBansResult, Alloc>{ model },
        m_banData{ model.banData, model.banData + model.banDataCount }
    {
        SetModelPointers();
    }

    PFAccountManagementUpdateBansResultWrapper(const PFAccountManagementUpdateBansResultWrapper& src) :
        PFAccountManagementUpdateBansResultWrapper{ src.Model() }
    {
    }

    PFAccountManagementUpdateBansResultWrapper(PFAccountManagementUpdateBansResultWrapper&& src) :
        PFAccountManagementUpdateBansResultWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementUpdateBansResultWrapper& operator=(PFAccountManagementUpdateBansResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementUpdateBansResultWrapper() = default;

    friend void swap(PFAccountManagementUpdateBansResultWrapper& lhs, PFAccountManagementUpdateBansResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_banData, rhs.m_banData);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetBanData(ModelVector<PFAccountManagementBanInfoWrapper<Alloc>, Alloc> value)
    {
        m_banData = std::move(value);
        this->m_model.banData =  m_banData.empty() ? nullptr : m_banData.data();
        this->m_model.banDataCount =  static_cast<uint32_t>(m_banData.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.banData = m_banData.empty() ? nullptr : m_banData.data();
    }

    ModelVector<PFAccountManagementBanInfoWrapper<Alloc>, Alloc> m_banData;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequestWrapper : public ModelWrapper<PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequest, Alloc>
{
public:
    using ModelType = PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequestWrapper() = default;

    PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequestWrapper(const PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequest& model) :
        ModelWrapper<PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_sandbox{ SafeString(model.sandbox) },
        m_titleId{ SafeString(model.titleId) },
        m_xboxLiveIds{ model.xboxLiveIds, model.xboxLiveIds + model.xboxLiveIdsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequestWrapper(const PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequestWrapper& src) :
        PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequestWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequestWrapper(PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequestWrapper&& src) :
        PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequestWrapper& operator=(PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequestWrapper() = default;

    friend void swap(PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequestWrapper& lhs, PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_sandbox, rhs.m_sandbox);
        swap(lhs.m_titleId, rhs.m_titleId);
        swap(lhs.m_xboxLiveIds, rhs.m_xboxLiveIds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetSandbox(String value)
    {
        m_sandbox = std::move(value);
        this->m_model.sandbox =  m_sandbox.empty() ? nullptr : m_sandbox.data();
    }

    void SetTitleId(String value)
    {
        m_titleId = std::move(value);
        this->m_model.titleId =  m_titleId.empty() ? nullptr : m_titleId.data();
    }

    void SetXboxLiveIds(CStringVector<Alloc> value)
    {
        m_xboxLiveIds = std::move(value);
        this->m_model.xboxLiveIds =  m_xboxLiveIds.empty() ? nullptr : m_xboxLiveIds.data();
        this->m_model.xboxLiveIdsCount =  static_cast<uint32_t>(m_xboxLiveIds.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.sandbox = m_sandbox.empty() ? nullptr : m_sandbox.data();
        this->m_model.titleId = m_titleId.empty() ? nullptr : m_titleId.data();
        this->m_model.xboxLiveIds = m_xboxLiveIds.empty() ? nullptr : m_xboxLiveIds.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_sandbox;
    String m_titleId;
    CStringVector<Alloc> m_xboxLiveIds;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAccountManagementGetTitlePlayersFromProviderIDsResponseWrapper : public ModelWrapper<PFAccountManagementGetTitlePlayersFromProviderIDsResponse, Alloc>
{
public:
    using ModelType = PFAccountManagementGetTitlePlayersFromProviderIDsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAccountManagementGetTitlePlayersFromProviderIDsResponseWrapper() = default;

    PFAccountManagementGetTitlePlayersFromProviderIDsResponseWrapper(const PFAccountManagementGetTitlePlayersFromProviderIDsResponse& model) :
        ModelWrapper<PFAccountManagementGetTitlePlayersFromProviderIDsResponse, Alloc>{ model },
        m_titlePlayerAccounts{ model.titlePlayerAccounts, model.titlePlayerAccounts + model.titlePlayerAccountsCount }
    {
        SetModelPointers();
    }

    PFAccountManagementGetTitlePlayersFromProviderIDsResponseWrapper(const PFAccountManagementGetTitlePlayersFromProviderIDsResponseWrapper& src) :
        PFAccountManagementGetTitlePlayersFromProviderIDsResponseWrapper{ src.Model() }
    {
    }

    PFAccountManagementGetTitlePlayersFromProviderIDsResponseWrapper(PFAccountManagementGetTitlePlayersFromProviderIDsResponseWrapper&& src) :
        PFAccountManagementGetTitlePlayersFromProviderIDsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFAccountManagementGetTitlePlayersFromProviderIDsResponseWrapper& operator=(PFAccountManagementGetTitlePlayersFromProviderIDsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAccountManagementGetTitlePlayersFromProviderIDsResponseWrapper() = default;

    friend void swap(PFAccountManagementGetTitlePlayersFromProviderIDsResponseWrapper& lhs, PFAccountManagementGetTitlePlayersFromProviderIDsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_titlePlayerAccounts, rhs.m_titlePlayerAccounts);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetTitlePlayerAccounts(ModelDictionaryEntryVector<PFEntityLineageWrapper<Alloc>, Alloc> value)
    {
        m_titlePlayerAccounts = std::move(value);
        this->m_model.titlePlayerAccounts =  m_titlePlayerAccounts.empty() ? nullptr : m_titlePlayerAccounts.data();
        this->m_model.titlePlayerAccountsCount =  static_cast<uint32_t>(m_titlePlayerAccounts.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.titlePlayerAccounts = m_titlePlayerAccounts.empty() ? nullptr : m_titlePlayerAccounts.data();
    }

    ModelDictionaryEntryVector<PFEntityLineageWrapper<Alloc>, Alloc> m_titlePlayerAccounts;
};

} // namespace Wrappers
} // namespace PlayFab

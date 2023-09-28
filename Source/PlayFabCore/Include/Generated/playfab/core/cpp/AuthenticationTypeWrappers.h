// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/cpp/TypeWrappers.h>
#include <playfab/core/PFAuthenticationTypes.h>

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithAndroidDeviceIDRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithAndroidDeviceIDRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithAndroidDeviceIDRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithAndroidDeviceIDRequestWrapper() = default;

    PFAuthenticationLoginWithAndroidDeviceIDRequestWrapper(const PFAuthenticationLoginWithAndroidDeviceIDRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithAndroidDeviceIDRequest, Alloc>{ model },
        m_androidDevice{ SafeString(model.androidDevice) },
        m_androidDeviceId{ SafeString(model.androidDeviceId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_OS{ SafeString(model.OS) },
        m_playerSecret{ SafeString(model.playerSecret) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithAndroidDeviceIDRequestWrapper(const PFAuthenticationLoginWithAndroidDeviceIDRequestWrapper& src) :
        PFAuthenticationLoginWithAndroidDeviceIDRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithAndroidDeviceIDRequestWrapper(PFAuthenticationLoginWithAndroidDeviceIDRequestWrapper&& src) :
        PFAuthenticationLoginWithAndroidDeviceIDRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithAndroidDeviceIDRequestWrapper& operator=(PFAuthenticationLoginWithAndroidDeviceIDRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithAndroidDeviceIDRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithAndroidDeviceIDRequestWrapper& lhs, PFAuthenticationLoginWithAndroidDeviceIDRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_androidDevice, rhs.m_androidDevice);
        swap(lhs.m_androidDeviceId, rhs.m_androidDeviceId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_OS, rhs.m_OS);
        swap(lhs.m_playerSecret, rhs.m_playerSecret);
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

    void SetCreateAccount(bool value)
    {
        this->m_model.createAccount = value;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
    }

    void SetOS(String value)
    {
        m_OS = std::move(value);
        this->m_model.OS =  m_OS.empty() ? nullptr : m_OS.data();
    }

    void SetPlayerSecret(String value)
    {
        m_playerSecret = std::move(value);
        this->m_model.playerSecret =  m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.androidDevice = m_androidDevice.empty() ? nullptr : m_androidDevice.data();
        this->m_model.androidDeviceId = m_androidDeviceId.empty() ? nullptr : m_androidDeviceId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.OS = m_OS.empty() ? nullptr : m_OS.data();
        this->m_model.playerSecret = m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

    String m_androidDevice;
    String m_androidDeviceId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_OS;
    String m_playerSecret;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationUserSettingsWrapper : public ModelWrapper<PFAuthenticationUserSettings, Alloc>
{
public:
    using ModelType = PFAuthenticationUserSettings;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    using ModelWrapper<PFAuthenticationUserSettings, Alloc>::ModelWrapper;

    void SetGatherDeviceInfo(bool value)
    {
        this->m_model.gatherDeviceInfo = value;
    }

    void SetGatherFocusInfo(bool value)
    {
        this->m_model.gatherFocusInfo = value;
    }

    void SetNeedsAttribution(bool value)
    {
        this->m_model.needsAttribution = value;
    }

private:
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginResultWrapper : public ModelWrapper<PFAuthenticationLoginResult, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginResultWrapper() = default;

    PFAuthenticationLoginResultWrapper(const PFAuthenticationLoginResult& model) :
        ModelWrapper<PFAuthenticationLoginResult, Alloc>{ model },
        m_infoResultPayload{ model.infoResultPayload ? std::optional<PFGetPlayerCombinedInfoResultPayloadWrapper<Alloc>>{ *model.infoResultPayload } : std::nullopt },
        m_lastLoginTime{ model.lastLoginTime ? std::optional<time_t>{ *model.lastLoginTime } : std::nullopt },
        m_playFabId{ SafeString(model.playFabId) },
        m_settingsForUser{ model.settingsForUser ? std::optional<PFAuthenticationUserSettingsWrapper<Alloc>>{ *model.settingsForUser } : std::nullopt },
        m_treatmentAssignment{ model.treatmentAssignment ? std::optional<PFTreatmentAssignmentWrapper<Alloc>>{ *model.treatmentAssignment } : std::nullopt }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginResultWrapper(const PFAuthenticationLoginResultWrapper& src) :
        PFAuthenticationLoginResultWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginResultWrapper(PFAuthenticationLoginResultWrapper&& src) :
        PFAuthenticationLoginResultWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationLoginResultWrapper& operator=(PFAuthenticationLoginResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginResultWrapper() = default;

    friend void swap(PFAuthenticationLoginResultWrapper& lhs, PFAuthenticationLoginResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_infoResultPayload, rhs.m_infoResultPayload);
        swap(lhs.m_lastLoginTime, rhs.m_lastLoginTime);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_settingsForUser, rhs.m_settingsForUser);
        swap(lhs.m_treatmentAssignment, rhs.m_treatmentAssignment);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetInfoResultPayload(std::optional<PFGetPlayerCombinedInfoResultPayloadWrapper<Alloc>> value)
    {
        m_infoResultPayload = std::move(value);
        this->m_model.infoResultPayload = m_infoResultPayload ? &m_infoResultPayload->Model() : nullptr;
    }

    void SetLastLoginTime(std::optional<time_t> value)
    {
        m_lastLoginTime = std::move(value);
        this->m_model.lastLoginTime = m_lastLoginTime ? m_lastLoginTime.operator->() : nullptr;
    }

    void SetNewlyCreated(bool value)
    {
        this->m_model.newlyCreated = value;
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetSettingsForUser(std::optional<PFAuthenticationUserSettingsWrapper<Alloc>> value)
    {
        m_settingsForUser = std::move(value);
        this->m_model.settingsForUser = m_settingsForUser ? &m_settingsForUser->Model() : nullptr;
    }

    void SetTreatmentAssignment(std::optional<PFTreatmentAssignmentWrapper<Alloc>> value)
    {
        m_treatmentAssignment = std::move(value);
        this->m_model.treatmentAssignment = m_treatmentAssignment ? &m_treatmentAssignment->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.infoResultPayload = m_infoResultPayload ?  &m_infoResultPayload->Model() : nullptr;
        this->m_model.lastLoginTime = m_lastLoginTime ? m_lastLoginTime.operator->() : nullptr;
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.settingsForUser = m_settingsForUser ?  &m_settingsForUser->Model() : nullptr;
        this->m_model.treatmentAssignment = m_treatmentAssignment ?  &m_treatmentAssignment->Model() : nullptr;
    }

    std::optional<PFGetPlayerCombinedInfoResultPayloadWrapper<Alloc>> m_infoResultPayload;
    std::optional<time_t> m_lastLoginTime;
    String m_playFabId;
    std::optional<PFAuthenticationUserSettingsWrapper<Alloc>> m_settingsForUser;
    std::optional<PFTreatmentAssignmentWrapper<Alloc>> m_treatmentAssignment;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithAppleRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithAppleRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithAppleRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithAppleRequestWrapper() = default;

    PFAuthenticationLoginWithAppleRequestWrapper(const PFAuthenticationLoginWithAppleRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithAppleRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_identityToken{ SafeString(model.identityToken) },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_playerSecret{ SafeString(model.playerSecret) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithAppleRequestWrapper(const PFAuthenticationLoginWithAppleRequestWrapper& src) :
        PFAuthenticationLoginWithAppleRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithAppleRequestWrapper(PFAuthenticationLoginWithAppleRequestWrapper&& src) :
        PFAuthenticationLoginWithAppleRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithAppleRequestWrapper& operator=(PFAuthenticationLoginWithAppleRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithAppleRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithAppleRequestWrapper& lhs, PFAuthenticationLoginWithAppleRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_identityToken, rhs.m_identityToken);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_playerSecret, rhs.m_playerSecret);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCreateAccount(bool value)
    {
        this->m_model.createAccount = value;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetIdentityToken(String value)
    {
        m_identityToken = std::move(value);
        this->m_model.identityToken =  m_identityToken.empty() ? nullptr : m_identityToken.data();
    }

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
    }

    void SetPlayerSecret(String value)
    {
        m_playerSecret = std::move(value);
        this->m_model.playerSecret =  m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.identityToken = m_identityToken.empty() ? nullptr : m_identityToken.data();
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.playerSecret = m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_identityToken;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_playerSecret;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithCustomIDRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithCustomIDRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithCustomIDRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithCustomIDRequestWrapper() = default;

    PFAuthenticationLoginWithCustomIDRequestWrapper(const PFAuthenticationLoginWithCustomIDRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithCustomIDRequest, Alloc>{ model },
        m_customId{ SafeString(model.customId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_playerSecret{ SafeString(model.playerSecret) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithCustomIDRequestWrapper(const PFAuthenticationLoginWithCustomIDRequestWrapper& src) :
        PFAuthenticationLoginWithCustomIDRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithCustomIDRequestWrapper(PFAuthenticationLoginWithCustomIDRequestWrapper&& src) :
        PFAuthenticationLoginWithCustomIDRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithCustomIDRequestWrapper& operator=(PFAuthenticationLoginWithCustomIDRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithCustomIDRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithCustomIDRequestWrapper& lhs, PFAuthenticationLoginWithCustomIDRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customId, rhs.m_customId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_playerSecret, rhs.m_playerSecret);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCreateAccount(bool value)
    {
        this->m_model.createAccount = value;
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

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
    }

    void SetPlayerSecret(String value)
    {
        m_playerSecret = std::move(value);
        this->m_model.playerSecret =  m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customId = m_customId.empty() ? nullptr : m_customId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.playerSecret = m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

    String m_customId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_playerSecret;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithEmailAddressRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithEmailAddressRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithEmailAddressRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithEmailAddressRequestWrapper() = default;

    PFAuthenticationLoginWithEmailAddressRequestWrapper(const PFAuthenticationLoginWithEmailAddressRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithEmailAddressRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_email{ SafeString(model.email) },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_password{ SafeString(model.password) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithEmailAddressRequestWrapper(const PFAuthenticationLoginWithEmailAddressRequestWrapper& src) :
        PFAuthenticationLoginWithEmailAddressRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithEmailAddressRequestWrapper(PFAuthenticationLoginWithEmailAddressRequestWrapper&& src) :
        PFAuthenticationLoginWithEmailAddressRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithEmailAddressRequestWrapper& operator=(PFAuthenticationLoginWithEmailAddressRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithEmailAddressRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithEmailAddressRequestWrapper& lhs, PFAuthenticationLoginWithEmailAddressRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_email, rhs.m_email);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_password, rhs.m_password);
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

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
    }

    void SetPassword(String value)
    {
        m_password = std::move(value);
        this->m_model.password =  m_password.empty() ? nullptr : m_password.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.email = m_email.empty() ? nullptr : m_email.data();
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.password = m_password.empty() ? nullptr : m_password.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_email;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_password;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithFacebookRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithFacebookRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithFacebookRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithFacebookRequestWrapper() = default;

    PFAuthenticationLoginWithFacebookRequestWrapper(const PFAuthenticationLoginWithFacebookRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithFacebookRequest, Alloc>{ model },
        m_accessToken{ SafeString(model.accessToken) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_playerSecret{ SafeString(model.playerSecret) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithFacebookRequestWrapper(const PFAuthenticationLoginWithFacebookRequestWrapper& src) :
        PFAuthenticationLoginWithFacebookRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithFacebookRequestWrapper(PFAuthenticationLoginWithFacebookRequestWrapper&& src) :
        PFAuthenticationLoginWithFacebookRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithFacebookRequestWrapper& operator=(PFAuthenticationLoginWithFacebookRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithFacebookRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithFacebookRequestWrapper& lhs, PFAuthenticationLoginWithFacebookRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_accessToken, rhs.m_accessToken);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_playerSecret, rhs.m_playerSecret);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAccessToken(String value)
    {
        m_accessToken = std::move(value);
        this->m_model.accessToken =  m_accessToken.empty() ? nullptr : m_accessToken.data();
    }

    void SetCreateAccount(bool value)
    {
        this->m_model.createAccount = value;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
    }

    void SetPlayerSecret(String value)
    {
        m_playerSecret = std::move(value);
        this->m_model.playerSecret =  m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.accessToken = m_accessToken.empty() ? nullptr : m_accessToken.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.playerSecret = m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

    String m_accessToken;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_playerSecret;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithFacebookInstantGamesIdRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithFacebookInstantGamesIdRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithFacebookInstantGamesIdRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithFacebookInstantGamesIdRequestWrapper() = default;

    PFAuthenticationLoginWithFacebookInstantGamesIdRequestWrapper(const PFAuthenticationLoginWithFacebookInstantGamesIdRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithFacebookInstantGamesIdRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_facebookInstantGamesSignature{ SafeString(model.facebookInstantGamesSignature) },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_playerSecret{ SafeString(model.playerSecret) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithFacebookInstantGamesIdRequestWrapper(const PFAuthenticationLoginWithFacebookInstantGamesIdRequestWrapper& src) :
        PFAuthenticationLoginWithFacebookInstantGamesIdRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithFacebookInstantGamesIdRequestWrapper(PFAuthenticationLoginWithFacebookInstantGamesIdRequestWrapper&& src) :
        PFAuthenticationLoginWithFacebookInstantGamesIdRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithFacebookInstantGamesIdRequestWrapper& operator=(PFAuthenticationLoginWithFacebookInstantGamesIdRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithFacebookInstantGamesIdRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithFacebookInstantGamesIdRequestWrapper& lhs, PFAuthenticationLoginWithFacebookInstantGamesIdRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_facebookInstantGamesSignature, rhs.m_facebookInstantGamesSignature);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_playerSecret, rhs.m_playerSecret);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCreateAccount(bool value)
    {
        this->m_model.createAccount = value;
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

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
    }

    void SetPlayerSecret(String value)
    {
        m_playerSecret = std::move(value);
        this->m_model.playerSecret =  m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.facebookInstantGamesSignature = m_facebookInstantGamesSignature.empty() ? nullptr : m_facebookInstantGamesSignature.data();
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.playerSecret = m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_facebookInstantGamesSignature;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_playerSecret;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithGameCenterRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithGameCenterRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithGameCenterRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithGameCenterRequestWrapper() = default;

    PFAuthenticationLoginWithGameCenterRequestWrapper(const PFAuthenticationLoginWithGameCenterRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithGameCenterRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_playerId{ SafeString(model.playerId) },
        m_playerSecret{ SafeString(model.playerSecret) },
        m_publicKeyUrl{ SafeString(model.publicKeyUrl) },
        m_salt{ SafeString(model.salt) },
        m_signature{ SafeString(model.signature) },
        m_timestamp{ SafeString(model.timestamp) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithGameCenterRequestWrapper(const PFAuthenticationLoginWithGameCenterRequestWrapper& src) :
        PFAuthenticationLoginWithGameCenterRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithGameCenterRequestWrapper(PFAuthenticationLoginWithGameCenterRequestWrapper&& src) :
        PFAuthenticationLoginWithGameCenterRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithGameCenterRequestWrapper& operator=(PFAuthenticationLoginWithGameCenterRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithGameCenterRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithGameCenterRequestWrapper& lhs, PFAuthenticationLoginWithGameCenterRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_playerId, rhs.m_playerId);
        swap(lhs.m_playerSecret, rhs.m_playerSecret);
        swap(lhs.m_publicKeyUrl, rhs.m_publicKeyUrl);
        swap(lhs.m_salt, rhs.m_salt);
        swap(lhs.m_signature, rhs.m_signature);
        swap(lhs.m_timestamp, rhs.m_timestamp);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCreateAccount(bool value)
    {
        this->m_model.createAccount = value;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
    }

    void SetPlayerId(String value)
    {
        m_playerId = std::move(value);
        this->m_model.playerId =  m_playerId.empty() ? nullptr : m_playerId.data();
    }

    void SetPlayerSecret(String value)
    {
        m_playerSecret = std::move(value);
        this->m_model.playerSecret =  m_playerSecret.empty() ? nullptr : m_playerSecret.data();
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
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.playerId = m_playerId.empty() ? nullptr : m_playerId.data();
        this->m_model.playerSecret = m_playerSecret.empty() ? nullptr : m_playerSecret.data();
        this->m_model.publicKeyUrl = m_publicKeyUrl.empty() ? nullptr : m_publicKeyUrl.data();
        this->m_model.salt = m_salt.empty() ? nullptr : m_salt.data();
        this->m_model.signature = m_signature.empty() ? nullptr : m_signature.data();
        this->m_model.timestamp = m_timestamp.empty() ? nullptr : m_timestamp.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_playerId;
    String m_playerSecret;
    String m_publicKeyUrl;
    String m_salt;
    String m_signature;
    String m_timestamp;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithGoogleAccountRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithGoogleAccountRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithGoogleAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithGoogleAccountRequestWrapper() = default;

    PFAuthenticationLoginWithGoogleAccountRequestWrapper(const PFAuthenticationLoginWithGoogleAccountRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithGoogleAccountRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_playerSecret{ SafeString(model.playerSecret) },
        m_serverAuthCode{ SafeString(model.serverAuthCode) },
        m_setEmail{ model.setEmail ? std::optional<bool>{ *model.setEmail } : std::nullopt }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithGoogleAccountRequestWrapper(const PFAuthenticationLoginWithGoogleAccountRequestWrapper& src) :
        PFAuthenticationLoginWithGoogleAccountRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithGoogleAccountRequestWrapper(PFAuthenticationLoginWithGoogleAccountRequestWrapper&& src) :
        PFAuthenticationLoginWithGoogleAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithGoogleAccountRequestWrapper& operator=(PFAuthenticationLoginWithGoogleAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithGoogleAccountRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithGoogleAccountRequestWrapper& lhs, PFAuthenticationLoginWithGoogleAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_playerSecret, rhs.m_playerSecret);
        swap(lhs.m_serverAuthCode, rhs.m_serverAuthCode);
        swap(lhs.m_setEmail, rhs.m_setEmail);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCreateAccount(bool value)
    {
        this->m_model.createAccount = value;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
    }

    void SetPlayerSecret(String value)
    {
        m_playerSecret = std::move(value);
        this->m_model.playerSecret =  m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

    void SetServerAuthCode(String value)
    {
        m_serverAuthCode = std::move(value);
        this->m_model.serverAuthCode =  m_serverAuthCode.empty() ? nullptr : m_serverAuthCode.data();
    }

    void SetSetEmail(std::optional<bool> value)
    {
        m_setEmail = std::move(value);
        this->m_model.setEmail = m_setEmail ? m_setEmail.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.playerSecret = m_playerSecret.empty() ? nullptr : m_playerSecret.data();
        this->m_model.serverAuthCode = m_serverAuthCode.empty() ? nullptr : m_serverAuthCode.data();
        this->m_model.setEmail = m_setEmail ? m_setEmail.operator->() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_playerSecret;
    String m_serverAuthCode;
    std::optional<bool> m_setEmail;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithGooglePlayGamesServicesRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithGooglePlayGamesServicesRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithGooglePlayGamesServicesRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithGooglePlayGamesServicesRequestWrapper() = default;

    PFAuthenticationLoginWithGooglePlayGamesServicesRequestWrapper(const PFAuthenticationLoginWithGooglePlayGamesServicesRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithGooglePlayGamesServicesRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_playerSecret{ SafeString(model.playerSecret) },
        m_serverAuthCode{ SafeString(model.serverAuthCode) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithGooglePlayGamesServicesRequestWrapper(const PFAuthenticationLoginWithGooglePlayGamesServicesRequestWrapper& src) :
        PFAuthenticationLoginWithGooglePlayGamesServicesRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithGooglePlayGamesServicesRequestWrapper(PFAuthenticationLoginWithGooglePlayGamesServicesRequestWrapper&& src) :
        PFAuthenticationLoginWithGooglePlayGamesServicesRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithGooglePlayGamesServicesRequestWrapper& operator=(PFAuthenticationLoginWithGooglePlayGamesServicesRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithGooglePlayGamesServicesRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithGooglePlayGamesServicesRequestWrapper& lhs, PFAuthenticationLoginWithGooglePlayGamesServicesRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_playerSecret, rhs.m_playerSecret);
        swap(lhs.m_serverAuthCode, rhs.m_serverAuthCode);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCreateAccount(bool value)
    {
        this->m_model.createAccount = value;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
    }

    void SetPlayerSecret(String value)
    {
        m_playerSecret = std::move(value);
        this->m_model.playerSecret =  m_playerSecret.empty() ? nullptr : m_playerSecret.data();
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
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.playerSecret = m_playerSecret.empty() ? nullptr : m_playerSecret.data();
        this->m_model.serverAuthCode = m_serverAuthCode.empty() ? nullptr : m_serverAuthCode.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_playerSecret;
    String m_serverAuthCode;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithIOSDeviceIDRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithIOSDeviceIDRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithIOSDeviceIDRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithIOSDeviceIDRequestWrapper() = default;

    PFAuthenticationLoginWithIOSDeviceIDRequestWrapper(const PFAuthenticationLoginWithIOSDeviceIDRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithIOSDeviceIDRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_deviceId{ SafeString(model.deviceId) },
        m_deviceModel{ SafeString(model.deviceModel) },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_OS{ SafeString(model.OS) },
        m_playerSecret{ SafeString(model.playerSecret) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithIOSDeviceIDRequestWrapper(const PFAuthenticationLoginWithIOSDeviceIDRequestWrapper& src) :
        PFAuthenticationLoginWithIOSDeviceIDRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithIOSDeviceIDRequestWrapper(PFAuthenticationLoginWithIOSDeviceIDRequestWrapper&& src) :
        PFAuthenticationLoginWithIOSDeviceIDRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithIOSDeviceIDRequestWrapper& operator=(PFAuthenticationLoginWithIOSDeviceIDRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithIOSDeviceIDRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithIOSDeviceIDRequestWrapper& lhs, PFAuthenticationLoginWithIOSDeviceIDRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_deviceId, rhs.m_deviceId);
        swap(lhs.m_deviceModel, rhs.m_deviceModel);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_OS, rhs.m_OS);
        swap(lhs.m_playerSecret, rhs.m_playerSecret);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCreateAccount(bool value)
    {
        this->m_model.createAccount = value;
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

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
    }

    void SetOS(String value)
    {
        m_OS = std::move(value);
        this->m_model.OS =  m_OS.empty() ? nullptr : m_OS.data();
    }

    void SetPlayerSecret(String value)
    {
        m_playerSecret = std::move(value);
        this->m_model.playerSecret =  m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.deviceId = m_deviceId.empty() ? nullptr : m_deviceId.data();
        this->m_model.deviceModel = m_deviceModel.empty() ? nullptr : m_deviceModel.data();
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.OS = m_OS.empty() ? nullptr : m_OS.data();
        this->m_model.playerSecret = m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_deviceId;
    String m_deviceModel;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_OS;
    String m_playerSecret;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithKongregateRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithKongregateRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithKongregateRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithKongregateRequestWrapper() = default;

    PFAuthenticationLoginWithKongregateRequestWrapper(const PFAuthenticationLoginWithKongregateRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithKongregateRequest, Alloc>{ model },
        m_authTicket{ SafeString(model.authTicket) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_kongregateId{ SafeString(model.kongregateId) },
        m_playerSecret{ SafeString(model.playerSecret) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithKongregateRequestWrapper(const PFAuthenticationLoginWithKongregateRequestWrapper& src) :
        PFAuthenticationLoginWithKongregateRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithKongregateRequestWrapper(PFAuthenticationLoginWithKongregateRequestWrapper&& src) :
        PFAuthenticationLoginWithKongregateRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithKongregateRequestWrapper& operator=(PFAuthenticationLoginWithKongregateRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithKongregateRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithKongregateRequestWrapper& lhs, PFAuthenticationLoginWithKongregateRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_authTicket, rhs.m_authTicket);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_kongregateId, rhs.m_kongregateId);
        swap(lhs.m_playerSecret, rhs.m_playerSecret);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAuthTicket(String value)
    {
        m_authTicket = std::move(value);
        this->m_model.authTicket =  m_authTicket.empty() ? nullptr : m_authTicket.data();
    }

    void SetCreateAccount(bool value)
    {
        this->m_model.createAccount = value;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
    }

    void SetKongregateId(String value)
    {
        m_kongregateId = std::move(value);
        this->m_model.kongregateId =  m_kongregateId.empty() ? nullptr : m_kongregateId.data();
    }

    void SetPlayerSecret(String value)
    {
        m_playerSecret = std::move(value);
        this->m_model.playerSecret =  m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.authTicket = m_authTicket.empty() ? nullptr : m_authTicket.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.kongregateId = m_kongregateId.empty() ? nullptr : m_kongregateId.data();
        this->m_model.playerSecret = m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

    String m_authTicket;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_kongregateId;
    String m_playerSecret;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithNintendoServiceAccountRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithNintendoServiceAccountRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithNintendoServiceAccountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithNintendoServiceAccountRequestWrapper() = default;

    PFAuthenticationLoginWithNintendoServiceAccountRequestWrapper(const PFAuthenticationLoginWithNintendoServiceAccountRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithNintendoServiceAccountRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_identityToken{ SafeString(model.identityToken) },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_playerSecret{ SafeString(model.playerSecret) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithNintendoServiceAccountRequestWrapper(const PFAuthenticationLoginWithNintendoServiceAccountRequestWrapper& src) :
        PFAuthenticationLoginWithNintendoServiceAccountRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithNintendoServiceAccountRequestWrapper(PFAuthenticationLoginWithNintendoServiceAccountRequestWrapper&& src) :
        PFAuthenticationLoginWithNintendoServiceAccountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithNintendoServiceAccountRequestWrapper& operator=(PFAuthenticationLoginWithNintendoServiceAccountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithNintendoServiceAccountRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithNintendoServiceAccountRequestWrapper& lhs, PFAuthenticationLoginWithNintendoServiceAccountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_identityToken, rhs.m_identityToken);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_playerSecret, rhs.m_playerSecret);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCreateAccount(bool value)
    {
        this->m_model.createAccount = value;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetIdentityToken(String value)
    {
        m_identityToken = std::move(value);
        this->m_model.identityToken =  m_identityToken.empty() ? nullptr : m_identityToken.data();
    }

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
    }

    void SetPlayerSecret(String value)
    {
        m_playerSecret = std::move(value);
        this->m_model.playerSecret =  m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.identityToken = m_identityToken.empty() ? nullptr : m_identityToken.data();
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.playerSecret = m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_identityToken;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_playerSecret;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithNintendoSwitchDeviceIdRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithNintendoSwitchDeviceIdRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithNintendoSwitchDeviceIdRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithNintendoSwitchDeviceIdRequestWrapper() = default;

    PFAuthenticationLoginWithNintendoSwitchDeviceIdRequestWrapper(const PFAuthenticationLoginWithNintendoSwitchDeviceIdRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithNintendoSwitchDeviceIdRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_nintendoSwitchDeviceId{ SafeString(model.nintendoSwitchDeviceId) },
        m_playerSecret{ SafeString(model.playerSecret) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithNintendoSwitchDeviceIdRequestWrapper(const PFAuthenticationLoginWithNintendoSwitchDeviceIdRequestWrapper& src) :
        PFAuthenticationLoginWithNintendoSwitchDeviceIdRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithNintendoSwitchDeviceIdRequestWrapper(PFAuthenticationLoginWithNintendoSwitchDeviceIdRequestWrapper&& src) :
        PFAuthenticationLoginWithNintendoSwitchDeviceIdRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithNintendoSwitchDeviceIdRequestWrapper& operator=(PFAuthenticationLoginWithNintendoSwitchDeviceIdRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithNintendoSwitchDeviceIdRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithNintendoSwitchDeviceIdRequestWrapper& lhs, PFAuthenticationLoginWithNintendoSwitchDeviceIdRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_nintendoSwitchDeviceId, rhs.m_nintendoSwitchDeviceId);
        swap(lhs.m_playerSecret, rhs.m_playerSecret);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCreateAccount(bool value)
    {
        this->m_model.createAccount = value;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
    }

    void SetNintendoSwitchDeviceId(String value)
    {
        m_nintendoSwitchDeviceId = std::move(value);
        this->m_model.nintendoSwitchDeviceId =  m_nintendoSwitchDeviceId.empty() ? nullptr : m_nintendoSwitchDeviceId.data();
    }

    void SetPlayerSecret(String value)
    {
        m_playerSecret = std::move(value);
        this->m_model.playerSecret =  m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.nintendoSwitchDeviceId = m_nintendoSwitchDeviceId.empty() ? nullptr : m_nintendoSwitchDeviceId.data();
        this->m_model.playerSecret = m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_nintendoSwitchDeviceId;
    String m_playerSecret;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithOpenIdConnectRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithOpenIdConnectRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithOpenIdConnectRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithOpenIdConnectRequestWrapper() = default;

    PFAuthenticationLoginWithOpenIdConnectRequestWrapper(const PFAuthenticationLoginWithOpenIdConnectRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithOpenIdConnectRequest, Alloc>{ model },
        m_connectionId{ SafeString(model.connectionId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_idToken{ SafeString(model.idToken) },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_playerSecret{ SafeString(model.playerSecret) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithOpenIdConnectRequestWrapper(const PFAuthenticationLoginWithOpenIdConnectRequestWrapper& src) :
        PFAuthenticationLoginWithOpenIdConnectRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithOpenIdConnectRequestWrapper(PFAuthenticationLoginWithOpenIdConnectRequestWrapper&& src) :
        PFAuthenticationLoginWithOpenIdConnectRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithOpenIdConnectRequestWrapper& operator=(PFAuthenticationLoginWithOpenIdConnectRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithOpenIdConnectRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithOpenIdConnectRequestWrapper& lhs, PFAuthenticationLoginWithOpenIdConnectRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_connectionId, rhs.m_connectionId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_idToken, rhs.m_idToken);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_playerSecret, rhs.m_playerSecret);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetConnectionId(String value)
    {
        m_connectionId = std::move(value);
        this->m_model.connectionId =  m_connectionId.empty() ? nullptr : m_connectionId.data();
    }

    void SetCreateAccount(bool value)
    {
        this->m_model.createAccount = value;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetIdToken(String value)
    {
        m_idToken = std::move(value);
        this->m_model.idToken =  m_idToken.empty() ? nullptr : m_idToken.data();
    }

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
    }

    void SetPlayerSecret(String value)
    {
        m_playerSecret = std::move(value);
        this->m_model.playerSecret =  m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.connectionId = m_connectionId.empty() ? nullptr : m_connectionId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.idToken = m_idToken.empty() ? nullptr : m_idToken.data();
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.playerSecret = m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

    String m_connectionId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_idToken;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_playerSecret;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithPlayFabRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithPlayFabRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithPlayFabRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithPlayFabRequestWrapper() = default;

    PFAuthenticationLoginWithPlayFabRequestWrapper(const PFAuthenticationLoginWithPlayFabRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithPlayFabRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_password{ SafeString(model.password) },
        m_username{ SafeString(model.username) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithPlayFabRequestWrapper(const PFAuthenticationLoginWithPlayFabRequestWrapper& src) :
        PFAuthenticationLoginWithPlayFabRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithPlayFabRequestWrapper(PFAuthenticationLoginWithPlayFabRequestWrapper&& src) :
        PFAuthenticationLoginWithPlayFabRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithPlayFabRequestWrapper& operator=(PFAuthenticationLoginWithPlayFabRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithPlayFabRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithPlayFabRequestWrapper& lhs, PFAuthenticationLoginWithPlayFabRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
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

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
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
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.password = m_password.empty() ? nullptr : m_password.data();
        this->m_model.username = m_username.empty() ? nullptr : m_username.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_password;
    String m_username;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithPSNRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithPSNRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithPSNRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithPSNRequestWrapper() = default;

    PFAuthenticationLoginWithPSNRequestWrapper(const PFAuthenticationLoginWithPSNRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithPSNRequest, Alloc>{ model },
        m_authCode{ SafeString(model.authCode) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_issuerId{ model.issuerId ? std::optional<int32_t>{ *model.issuerId } : std::nullopt },
        m_playerSecret{ SafeString(model.playerSecret) },
        m_redirectUri{ SafeString(model.redirectUri) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithPSNRequestWrapper(const PFAuthenticationLoginWithPSNRequestWrapper& src) :
        PFAuthenticationLoginWithPSNRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithPSNRequestWrapper(PFAuthenticationLoginWithPSNRequestWrapper&& src) :
        PFAuthenticationLoginWithPSNRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithPSNRequestWrapper& operator=(PFAuthenticationLoginWithPSNRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithPSNRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithPSNRequestWrapper& lhs, PFAuthenticationLoginWithPSNRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_authCode, rhs.m_authCode);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_issuerId, rhs.m_issuerId);
        swap(lhs.m_playerSecret, rhs.m_playerSecret);
        swap(lhs.m_redirectUri, rhs.m_redirectUri);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAuthCode(String value)
    {
        m_authCode = std::move(value);
        this->m_model.authCode =  m_authCode.empty() ? nullptr : m_authCode.data();
    }

    void SetCreateAccount(bool value)
    {
        this->m_model.createAccount = value;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
    }

    void SetIssuerId(std::optional<int32_t> value)
    {
        m_issuerId = std::move(value);
        this->m_model.issuerId = m_issuerId ? m_issuerId.operator->() : nullptr;
    }

    void SetPlayerSecret(String value)
    {
        m_playerSecret = std::move(value);
        this->m_model.playerSecret =  m_playerSecret.empty() ? nullptr : m_playerSecret.data();
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
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.issuerId = m_issuerId ? m_issuerId.operator->() : nullptr;
        this->m_model.playerSecret = m_playerSecret.empty() ? nullptr : m_playerSecret.data();
        this->m_model.redirectUri = m_redirectUri.empty() ? nullptr : m_redirectUri.data();
    }

    String m_authCode;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    std::optional<int32_t> m_issuerId;
    String m_playerSecret;
    String m_redirectUri;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithSteamRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithSteamRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithSteamRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithSteamRequestWrapper() = default;

    PFAuthenticationLoginWithSteamRequestWrapper(const PFAuthenticationLoginWithSteamRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithSteamRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_playerSecret{ SafeString(model.playerSecret) },
        m_steamTicket{ SafeString(model.steamTicket) },
        m_ticketIsServiceSpecific{ model.ticketIsServiceSpecific ? std::optional<bool>{ *model.ticketIsServiceSpecific } : std::nullopt }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithSteamRequestWrapper(const PFAuthenticationLoginWithSteamRequestWrapper& src) :
        PFAuthenticationLoginWithSteamRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithSteamRequestWrapper(PFAuthenticationLoginWithSteamRequestWrapper&& src) :
        PFAuthenticationLoginWithSteamRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithSteamRequestWrapper& operator=(PFAuthenticationLoginWithSteamRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithSteamRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithSteamRequestWrapper& lhs, PFAuthenticationLoginWithSteamRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_playerSecret, rhs.m_playerSecret);
        swap(lhs.m_steamTicket, rhs.m_steamTicket);
        swap(lhs.m_ticketIsServiceSpecific, rhs.m_ticketIsServiceSpecific);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCreateAccount(bool value)
    {
        this->m_model.createAccount = value;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
    }

    void SetPlayerSecret(String value)
    {
        m_playerSecret = std::move(value);
        this->m_model.playerSecret =  m_playerSecret.empty() ? nullptr : m_playerSecret.data();
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
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.playerSecret = m_playerSecret.empty() ? nullptr : m_playerSecret.data();
        this->m_model.steamTicket = m_steamTicket.empty() ? nullptr : m_steamTicket.data();
        this->m_model.ticketIsServiceSpecific = m_ticketIsServiceSpecific ? m_ticketIsServiceSpecific.operator->() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_playerSecret;
    String m_steamTicket;
    std::optional<bool> m_ticketIsServiceSpecific;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithTwitchRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithTwitchRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithTwitchRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithTwitchRequestWrapper() = default;

    PFAuthenticationLoginWithTwitchRequestWrapper(const PFAuthenticationLoginWithTwitchRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithTwitchRequest, Alloc>{ model },
        m_accessToken{ SafeString(model.accessToken) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_playerSecret{ SafeString(model.playerSecret) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithTwitchRequestWrapper(const PFAuthenticationLoginWithTwitchRequestWrapper& src) :
        PFAuthenticationLoginWithTwitchRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithTwitchRequestWrapper(PFAuthenticationLoginWithTwitchRequestWrapper&& src) :
        PFAuthenticationLoginWithTwitchRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithTwitchRequestWrapper& operator=(PFAuthenticationLoginWithTwitchRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithTwitchRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithTwitchRequestWrapper& lhs, PFAuthenticationLoginWithTwitchRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_accessToken, rhs.m_accessToken);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_playerSecret, rhs.m_playerSecret);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAccessToken(String value)
    {
        m_accessToken = std::move(value);
        this->m_model.accessToken =  m_accessToken.empty() ? nullptr : m_accessToken.data();
    }

    void SetCreateAccount(bool value)
    {
        this->m_model.createAccount = value;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
    }

    void SetPlayerSecret(String value)
    {
        m_playerSecret = std::move(value);
        this->m_model.playerSecret =  m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.accessToken = m_accessToken.empty() ? nullptr : m_accessToken.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.playerSecret = m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

    String m_accessToken;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_playerSecret;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithXboxRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithXboxRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithXboxRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithXboxRequestWrapper() = default;

    PFAuthenticationLoginWithXboxRequestWrapper(const PFAuthenticationLoginWithXboxRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithXboxRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_playerSecret{ SafeString(model.playerSecret) },
        m_xboxToken{ SafeString(model.xboxToken) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithXboxRequestWrapper(const PFAuthenticationLoginWithXboxRequestWrapper& src) :
        PFAuthenticationLoginWithXboxRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithXboxRequestWrapper(PFAuthenticationLoginWithXboxRequestWrapper&& src) :
        PFAuthenticationLoginWithXboxRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithXboxRequestWrapper& operator=(PFAuthenticationLoginWithXboxRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithXboxRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithXboxRequestWrapper& lhs, PFAuthenticationLoginWithXboxRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_playerSecret, rhs.m_playerSecret);
        swap(lhs.m_xboxToken, rhs.m_xboxToken);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCreateAccount(bool value)
    {
        this->m_model.createAccount = value;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
    }

    void SetPlayerSecret(String value)
    {
        m_playerSecret = std::move(value);
        this->m_model.playerSecret =  m_playerSecret.empty() ? nullptr : m_playerSecret.data();
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
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.playerSecret = m_playerSecret.empty() ? nullptr : m_playerSecret.data();
        this->m_model.xboxToken = m_xboxToken.empty() ? nullptr : m_xboxToken.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_playerSecret;
    String m_xboxToken;
};

#if HC_PLATFORM == HC_PLATFORM_GDK
class XUser
{
public:
    static XUser Wrap(XUserHandle handle)
    {
        return XUser{ handle };
    }

    static XUser Duplicate(XUserHandle handle)
    {
        XUserHandle duplicatedHandle;
        THROW_IF_FAILED(XUserDuplicateHandle(handle, &duplicatedHandle));
        return XUser{ duplicatedHandle };
    }

    XUser(XUser const& other)
    {
        THROW_IF_FAILED(XUserDuplicateHandle(other.m_handle, &m_handle));
    }

    XUser(XUser&& other)
    {
        std::swap(other.m_handle, m_handle);
    }

    XUser& operator=(XUser other)
    {
        std::swap(m_handle, other.m_handle);
        return *this;
    }

    ~XUser()
    {
        if (m_handle)
        {
            XUserCloseHandle(m_handle);
        }
    }

    XUserHandle Handle() const noexcept
    {
        return m_handle;
    }

private:
    XUser(XUserHandle handle) : m_handle{ handle }
    {
    }

    XUserHandle m_handle{ nullptr };
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithXUserRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithXUserRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithXUserRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithXUserRequestWrapper(XUser user)
        : m_user{ std::move(user) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithXUserRequestWrapper(const PFAuthenticationLoginWithXUserRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithXUserRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_playerSecret{ SafeString(model.playerSecret) },
        m_user{ XUser::Duplicate(model.user) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithXUserRequestWrapper(const PFAuthenticationLoginWithXUserRequestWrapper& src) :
        PFAuthenticationLoginWithXUserRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithXUserRequestWrapper(PFAuthenticationLoginWithXUserRequestWrapper&& src) :
        m_user{ XUser::Wrap(nullptr) }
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithXUserRequestWrapper& operator=(PFAuthenticationLoginWithXUserRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithXUserRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithXUserRequestWrapper& lhs, PFAuthenticationLoginWithXUserRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_playerSecret, rhs.m_playerSecret);
        swap(lhs.m_user, rhs.m_user);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCreateAccount(bool value)
    {
        this->m_model.createAccount = value;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
    }

    void SetPlayerSecret(String value)
    {
        m_playerSecret = std::move(value);
        this->m_model.playerSecret =  m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

    void SetUser(XUser value)
    {
        m_user = std::move(value);
        this->m_model.user = m_user.Handle();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.playerSecret = m_playerSecret.empty() ? nullptr : m_playerSecret.data();
        this->m_model.user = m_user.Handle();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_playerSecret;
    XUser m_user;
};
#endif

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationRegisterPlayFabUserRequestWrapper : public ModelWrapper<PFAuthenticationRegisterPlayFabUserRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationRegisterPlayFabUserRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationRegisterPlayFabUserRequestWrapper() = default;

    PFAuthenticationRegisterPlayFabUserRequestWrapper(const PFAuthenticationRegisterPlayFabUserRequest& model) :
        ModelWrapper<PFAuthenticationRegisterPlayFabUserRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_displayName{ SafeString(model.displayName) },
        m_email{ SafeString(model.email) },
        m_encryptedRequest{ SafeString(model.encryptedRequest) },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_password{ SafeString(model.password) },
        m_playerSecret{ SafeString(model.playerSecret) },
        m_requireBothUsernameAndEmail{ model.requireBothUsernameAndEmail ? std::optional<bool>{ *model.requireBothUsernameAndEmail } : std::nullopt },
        m_titleId{ SafeString(model.titleId) },
        m_username{ SafeString(model.username) }
    {
        SetModelPointers();
    }

    PFAuthenticationRegisterPlayFabUserRequestWrapper(const PFAuthenticationRegisterPlayFabUserRequestWrapper& src) :
        PFAuthenticationRegisterPlayFabUserRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationRegisterPlayFabUserRequestWrapper(PFAuthenticationRegisterPlayFabUserRequestWrapper&& src) :
        PFAuthenticationRegisterPlayFabUserRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationRegisterPlayFabUserRequestWrapper& operator=(PFAuthenticationRegisterPlayFabUserRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationRegisterPlayFabUserRequestWrapper() = default;

    friend void swap(PFAuthenticationRegisterPlayFabUserRequestWrapper& lhs, PFAuthenticationRegisterPlayFabUserRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_displayName, rhs.m_displayName);
        swap(lhs.m_email, rhs.m_email);
        swap(lhs.m_encryptedRequest, rhs.m_encryptedRequest);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_password, rhs.m_password);
        swap(lhs.m_playerSecret, rhs.m_playerSecret);
        swap(lhs.m_requireBothUsernameAndEmail, rhs.m_requireBothUsernameAndEmail);
        swap(lhs.m_titleId, rhs.m_titleId);
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

    void SetDisplayName(String value)
    {
        m_displayName = std::move(value);
        this->m_model.displayName =  m_displayName.empty() ? nullptr : m_displayName.data();
    }

    void SetEmail(String value)
    {
        m_email = std::move(value);
        this->m_model.email =  m_email.empty() ? nullptr : m_email.data();
    }

    void SetEncryptedRequest(String value)
    {
        m_encryptedRequest = std::move(value);
        this->m_model.encryptedRequest =  m_encryptedRequest.empty() ? nullptr : m_encryptedRequest.data();
    }

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
    }

    void SetPassword(String value)
    {
        m_password = std::move(value);
        this->m_model.password =  m_password.empty() ? nullptr : m_password.data();
    }

    void SetPlayerSecret(String value)
    {
        m_playerSecret = std::move(value);
        this->m_model.playerSecret =  m_playerSecret.empty() ? nullptr : m_playerSecret.data();
    }

    void SetRequireBothUsernameAndEmail(std::optional<bool> value)
    {
        m_requireBothUsernameAndEmail = std::move(value);
        this->m_model.requireBothUsernameAndEmail = m_requireBothUsernameAndEmail ? m_requireBothUsernameAndEmail.operator->() : nullptr;
    }

    void SetTitleId(String value)
    {
        m_titleId = std::move(value);
        this->m_model.titleId =  m_titleId.empty() ? nullptr : m_titleId.data();
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
        this->m_model.displayName = m_displayName.empty() ? nullptr : m_displayName.data();
        this->m_model.email = m_email.empty() ? nullptr : m_email.data();
        this->m_model.encryptedRequest = m_encryptedRequest.empty() ? nullptr : m_encryptedRequest.data();
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.password = m_password.empty() ? nullptr : m_password.data();
        this->m_model.playerSecret = m_playerSecret.empty() ? nullptr : m_playerSecret.data();
        this->m_model.requireBothUsernameAndEmail = m_requireBothUsernameAndEmail ? m_requireBothUsernameAndEmail.operator->() : nullptr;
        this->m_model.titleId = m_titleId.empty() ? nullptr : m_titleId.data();
        this->m_model.username = m_username.empty() ? nullptr : m_username.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_displayName;
    String m_email;
    String m_encryptedRequest;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_password;
    String m_playerSecret;
    std::optional<bool> m_requireBothUsernameAndEmail;
    String m_titleId;
    String m_username;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationRegisterPlayFabUserResultWrapper : public ModelWrapper<PFAuthenticationRegisterPlayFabUserResult, Alloc>
{
public:
    using ModelType = PFAuthenticationRegisterPlayFabUserResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationRegisterPlayFabUserResultWrapper() = default;

    PFAuthenticationRegisterPlayFabUserResultWrapper(const PFAuthenticationRegisterPlayFabUserResult& model) :
        ModelWrapper<PFAuthenticationRegisterPlayFabUserResult, Alloc>{ model },
        m_playFabId{ SafeString(model.playFabId) },
        m_settingsForUser{ model.settingsForUser ? std::optional<PFAuthenticationUserSettingsWrapper<Alloc>>{ *model.settingsForUser } : std::nullopt },
        m_username{ SafeString(model.username) }
    {
        SetModelPointers();
    }

    PFAuthenticationRegisterPlayFabUserResultWrapper(const PFAuthenticationRegisterPlayFabUserResultWrapper& src) :
        PFAuthenticationRegisterPlayFabUserResultWrapper{ src.Model() }
    {
    }

    PFAuthenticationRegisterPlayFabUserResultWrapper(PFAuthenticationRegisterPlayFabUserResultWrapper&& src) :
        PFAuthenticationRegisterPlayFabUserResultWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationRegisterPlayFabUserResultWrapper& operator=(PFAuthenticationRegisterPlayFabUserResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationRegisterPlayFabUserResultWrapper() = default;

    friend void swap(PFAuthenticationRegisterPlayFabUserResultWrapper& lhs, PFAuthenticationRegisterPlayFabUserResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_settingsForUser, rhs.m_settingsForUser);
        swap(lhs.m_username, rhs.m_username);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetSettingsForUser(std::optional<PFAuthenticationUserSettingsWrapper<Alloc>> value)
    {
        m_settingsForUser = std::move(value);
        this->m_model.settingsForUser = m_settingsForUser ? &m_settingsForUser->Model() : nullptr;
    }

    void SetUsername(String value)
    {
        m_username = std::move(value);
        this->m_model.username =  m_username.empty() ? nullptr : m_username.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.settingsForUser = m_settingsForUser ?  &m_settingsForUser->Model() : nullptr;
        this->m_model.username = m_username.empty() ? nullptr : m_username.data();
    }

    String m_playFabId;
    std::optional<PFAuthenticationUserSettingsWrapper<Alloc>> m_settingsForUser;
    String m_username;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithServerCustomIdRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithServerCustomIdRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithServerCustomIdRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithServerCustomIdRequestWrapper() = default;

    PFAuthenticationLoginWithServerCustomIdRequestWrapper(const PFAuthenticationLoginWithServerCustomIdRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithServerCustomIdRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_playerSecret{ SafeString(model.playerSecret) },
        m_serverCustomId{ SafeString(model.serverCustomId) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithServerCustomIdRequestWrapper(const PFAuthenticationLoginWithServerCustomIdRequestWrapper& src) :
        PFAuthenticationLoginWithServerCustomIdRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithServerCustomIdRequestWrapper(PFAuthenticationLoginWithServerCustomIdRequestWrapper&& src) :
        PFAuthenticationLoginWithServerCustomIdRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithServerCustomIdRequestWrapper& operator=(PFAuthenticationLoginWithServerCustomIdRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithServerCustomIdRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithServerCustomIdRequestWrapper& lhs, PFAuthenticationLoginWithServerCustomIdRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_playerSecret, rhs.m_playerSecret);
        swap(lhs.m_serverCustomId, rhs.m_serverCustomId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCreateAccount(bool value)
    {
        this->m_model.createAccount = value;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
    }

    void SetPlayerSecret(String value)
    {
        m_playerSecret = std::move(value);
        this->m_model.playerSecret =  m_playerSecret.empty() ? nullptr : m_playerSecret.data();
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
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.playerSecret = m_playerSecret.empty() ? nullptr : m_playerSecret.data();
        this->m_model.serverCustomId = m_serverCustomId.empty() ? nullptr : m_serverCustomId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_playerSecret;
    String m_serverCustomId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithSteamIdRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithSteamIdRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithSteamIdRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithSteamIdRequestWrapper() = default;

    PFAuthenticationLoginWithSteamIdRequestWrapper(const PFAuthenticationLoginWithSteamIdRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithSteamIdRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_steamId{ SafeString(model.steamId) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithSteamIdRequestWrapper(const PFAuthenticationLoginWithSteamIdRequestWrapper& src) :
        PFAuthenticationLoginWithSteamIdRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithSteamIdRequestWrapper(PFAuthenticationLoginWithSteamIdRequestWrapper&& src) :
        PFAuthenticationLoginWithSteamIdRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithSteamIdRequestWrapper& operator=(PFAuthenticationLoginWithSteamIdRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithSteamIdRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithSteamIdRequestWrapper& lhs, PFAuthenticationLoginWithSteamIdRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_steamId, rhs.m_steamId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCreateAccount(bool value)
    {
        this->m_model.createAccount = value;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
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
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.steamId = m_steamId.empty() ? nullptr : m_steamId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_steamId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationServerLoginWithXboxRequestWrapper : public ModelWrapper<PFAuthenticationServerLoginWithXboxRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationServerLoginWithXboxRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationServerLoginWithXboxRequestWrapper() = default;

    PFAuthenticationServerLoginWithXboxRequestWrapper(const PFAuthenticationServerLoginWithXboxRequest& model) :
        ModelWrapper<PFAuthenticationServerLoginWithXboxRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_xboxToken{ SafeString(model.xboxToken) }
    {
        SetModelPointers();
    }

    PFAuthenticationServerLoginWithXboxRequestWrapper(const PFAuthenticationServerLoginWithXboxRequestWrapper& src) :
        PFAuthenticationServerLoginWithXboxRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationServerLoginWithXboxRequestWrapper(PFAuthenticationServerLoginWithXboxRequestWrapper&& src) :
        PFAuthenticationServerLoginWithXboxRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationServerLoginWithXboxRequestWrapper& operator=(PFAuthenticationServerLoginWithXboxRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationServerLoginWithXboxRequestWrapper() = default;

    friend void swap(PFAuthenticationServerLoginWithXboxRequestWrapper& lhs, PFAuthenticationServerLoginWithXboxRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_xboxToken, rhs.m_xboxToken);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCreateAccount(bool value)
    {
        this->m_model.createAccount = value;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
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
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.xboxToken = m_xboxToken.empty() ? nullptr : m_xboxToken.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_xboxToken;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationLoginWithXboxIdRequestWrapper : public ModelWrapper<PFAuthenticationLoginWithXboxIdRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationLoginWithXboxIdRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationLoginWithXboxIdRequestWrapper() = default;

    PFAuthenticationLoginWithXboxIdRequestWrapper(const PFAuthenticationLoginWithXboxIdRequest& model) :
        ModelWrapper<PFAuthenticationLoginWithXboxIdRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_infoRequestParameters{ model.infoRequestParameters ? std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>>{ *model.infoRequestParameters } : std::nullopt },
        m_sandbox{ SafeString(model.sandbox) },
        m_xboxId{ SafeString(model.xboxId) }
    {
        SetModelPointers();
    }

    PFAuthenticationLoginWithXboxIdRequestWrapper(const PFAuthenticationLoginWithXboxIdRequestWrapper& src) :
        PFAuthenticationLoginWithXboxIdRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationLoginWithXboxIdRequestWrapper(PFAuthenticationLoginWithXboxIdRequestWrapper&& src) :
        PFAuthenticationLoginWithXboxIdRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationLoginWithXboxIdRequestWrapper& operator=(PFAuthenticationLoginWithXboxIdRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationLoginWithXboxIdRequestWrapper() = default;

    friend void swap(PFAuthenticationLoginWithXboxIdRequestWrapper& lhs, PFAuthenticationLoginWithXboxIdRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_infoRequestParameters, rhs.m_infoRequestParameters);
        swap(lhs.m_sandbox, rhs.m_sandbox);
        swap(lhs.m_xboxId, rhs.m_xboxId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCreateAccount(bool value)
    {
        this->m_model.createAccount = value;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetInfoRequestParameters(std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> value)
    {
        m_infoRequestParameters = std::move(value);
        this->m_model.infoRequestParameters = m_infoRequestParameters ? &m_infoRequestParameters->Model() : nullptr;
    }

    void SetSandbox(String value)
    {
        m_sandbox = std::move(value);
        this->m_model.sandbox =  m_sandbox.empty() ? nullptr : m_sandbox.data();
    }

    void SetXboxId(String value)
    {
        m_xboxId = std::move(value);
        this->m_model.xboxId =  m_xboxId.empty() ? nullptr : m_xboxId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.infoRequestParameters = m_infoRequestParameters ?  &m_infoRequestParameters->Model() : nullptr;
        this->m_model.sandbox = m_sandbox.empty() ? nullptr : m_sandbox.data();
        this->m_model.xboxId = m_xboxId.empty() ? nullptr : m_xboxId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFGetPlayerCombinedInfoRequestParamsWrapper<Alloc>> m_infoRequestParameters;
    String m_sandbox;
    String m_xboxId;
};

#if HC_PLATFORM != HC_PLATFORM_GDK
template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationAuthenticateCustomIdRequestWrapper : public ModelWrapper<PFAuthenticationAuthenticateCustomIdRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationAuthenticateCustomIdRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationAuthenticateCustomIdRequestWrapper() = default;

    PFAuthenticationAuthenticateCustomIdRequestWrapper(const PFAuthenticationAuthenticateCustomIdRequest& model) :
        ModelWrapper<PFAuthenticationAuthenticateCustomIdRequest, Alloc>{ model },
        m_customId{ SafeString(model.customId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount }
    {
        SetModelPointers();
    }

    PFAuthenticationAuthenticateCustomIdRequestWrapper(const PFAuthenticationAuthenticateCustomIdRequestWrapper& src) :
        PFAuthenticationAuthenticateCustomIdRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationAuthenticateCustomIdRequestWrapper(PFAuthenticationAuthenticateCustomIdRequestWrapper&& src) :
        PFAuthenticationAuthenticateCustomIdRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationAuthenticateCustomIdRequestWrapper& operator=(PFAuthenticationAuthenticateCustomIdRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationAuthenticateCustomIdRequestWrapper() = default;

    friend void swap(PFAuthenticationAuthenticateCustomIdRequestWrapper& lhs, PFAuthenticationAuthenticateCustomIdRequestWrapper& rhs)
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
#endif

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationEntityTokenResponseWrapper : public ModelWrapper<PFAuthenticationEntityTokenResponse, Alloc>
{
public:
    using ModelType = PFAuthenticationEntityTokenResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationEntityTokenResponseWrapper() = default;

    PFAuthenticationEntityTokenResponseWrapper(const PFAuthenticationEntityTokenResponse& model) :
        ModelWrapper<PFAuthenticationEntityTokenResponse, Alloc>{ model },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_entityToken{ SafeString(model.entityToken) },
        m_tokenExpiration{ model.tokenExpiration ? std::optional<time_t>{ *model.tokenExpiration } : std::nullopt }
    {
        SetModelPointers();
    }

    PFAuthenticationEntityTokenResponseWrapper(const PFAuthenticationEntityTokenResponseWrapper& src) :
        PFAuthenticationEntityTokenResponseWrapper{ src.Model() }
    {
    }

    PFAuthenticationEntityTokenResponseWrapper(PFAuthenticationEntityTokenResponseWrapper&& src) :
        PFAuthenticationEntityTokenResponseWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationEntityTokenResponseWrapper& operator=(PFAuthenticationEntityTokenResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationEntityTokenResponseWrapper() = default;

    friend void swap(PFAuthenticationEntityTokenResponseWrapper& lhs, PFAuthenticationEntityTokenResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_entityToken, rhs.m_entityToken);
        swap(lhs.m_tokenExpiration, rhs.m_tokenExpiration);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    void SetEntityToken(String value)
    {
        m_entityToken = std::move(value);
        this->m_model.entityToken =  m_entityToken.empty() ? nullptr : m_entityToken.data();
    }

    void SetTokenExpiration(std::optional<time_t> value)
    {
        m_tokenExpiration = std::move(value);
        this->m_model.tokenExpiration = m_tokenExpiration ? m_tokenExpiration.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.entityToken = m_entityToken.empty() ? nullptr : m_entityToken.data();
        this->m_model.tokenExpiration = m_tokenExpiration ? m_tokenExpiration.operator->() : nullptr;
    }

    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_entityToken;
    std::optional<time_t> m_tokenExpiration;
};

#if HC_PLATFORM != HC_PLATFORM_GDK
template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationAuthenticateCustomIdResultWrapper : public ModelWrapper<PFAuthenticationAuthenticateCustomIdResult, Alloc>
{
public:
    using ModelType = PFAuthenticationAuthenticateCustomIdResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationAuthenticateCustomIdResultWrapper() = default;

    PFAuthenticationAuthenticateCustomIdResultWrapper(const PFAuthenticationAuthenticateCustomIdResult& model) :
        ModelWrapper<PFAuthenticationAuthenticateCustomIdResult, Alloc>{ model },
        m_entityToken{ model.entityToken ? std::optional<PFAuthenticationEntityTokenResponseWrapper<Alloc>>{ *model.entityToken } : std::nullopt }
    {
        SetModelPointers();
    }

    PFAuthenticationAuthenticateCustomIdResultWrapper(const PFAuthenticationAuthenticateCustomIdResultWrapper& src) :
        PFAuthenticationAuthenticateCustomIdResultWrapper{ src.Model() }
    {
    }

    PFAuthenticationAuthenticateCustomIdResultWrapper(PFAuthenticationAuthenticateCustomIdResultWrapper&& src) :
        PFAuthenticationAuthenticateCustomIdResultWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationAuthenticateCustomIdResultWrapper& operator=(PFAuthenticationAuthenticateCustomIdResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationAuthenticateCustomIdResultWrapper() = default;

    friend void swap(PFAuthenticationAuthenticateCustomIdResultWrapper& lhs, PFAuthenticationAuthenticateCustomIdResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_entityToken, rhs.m_entityToken);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetEntityToken(std::optional<PFAuthenticationEntityTokenResponseWrapper<Alloc>> value)
    {
        m_entityToken = std::move(value);
        this->m_model.entityToken = m_entityToken ? &m_entityToken->Model() : nullptr;
    }

    void SetNewlyCreated(bool value)
    {
        this->m_model.newlyCreated = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.entityToken = m_entityToken ?  &m_entityToken->Model() : nullptr;
    }

    std::optional<PFAuthenticationEntityTokenResponseWrapper<Alloc>> m_entityToken;
};
#endif

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationDeleteRequestWrapper : public ModelWrapper<PFAuthenticationDeleteRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationDeleteRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationDeleteRequestWrapper() = default;

    PFAuthenticationDeleteRequestWrapper(const PFAuthenticationDeleteRequest& model) :
        ModelWrapper<PFAuthenticationDeleteRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? *model.entity : decltype(*model.entity){} }
    {
        SetModelPointers();
    }

    PFAuthenticationDeleteRequestWrapper(const PFAuthenticationDeleteRequestWrapper& src) :
        PFAuthenticationDeleteRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationDeleteRequestWrapper(PFAuthenticationDeleteRequestWrapper&& src) :
        PFAuthenticationDeleteRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationDeleteRequestWrapper& operator=(PFAuthenticationDeleteRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationDeleteRequestWrapper() = default;

    friend void swap(PFAuthenticationDeleteRequestWrapper& lhs, PFAuthenticationDeleteRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntity(PFEntityKeyWrapper<Alloc> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = &m_entity.Model();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = &m_entity.Model();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFEntityKeyWrapper<Alloc> m_entity;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationGetEntityRequestWrapper : public ModelWrapper<PFAuthenticationGetEntityRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationGetEntityRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationGetEntityRequestWrapper() = default;

    PFAuthenticationGetEntityRequestWrapper(const PFAuthenticationGetEntityRequest& model) :
        ModelWrapper<PFAuthenticationGetEntityRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt }
    {
        SetModelPointers();
    }

    PFAuthenticationGetEntityRequestWrapper(const PFAuthenticationGetEntityRequestWrapper& src) :
        PFAuthenticationGetEntityRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationGetEntityRequestWrapper(PFAuthenticationGetEntityRequestWrapper&& src) :
        PFAuthenticationGetEntityRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationGetEntityRequestWrapper& operator=(PFAuthenticationGetEntityRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationGetEntityRequestWrapper() = default;

    friend void swap(PFAuthenticationGetEntityRequestWrapper& lhs, PFAuthenticationGetEntityRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationValidateEntityTokenRequestWrapper : public ModelWrapper<PFAuthenticationValidateEntityTokenRequest, Alloc>
{
public:
    using ModelType = PFAuthenticationValidateEntityTokenRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationValidateEntityTokenRequestWrapper() = default;

    PFAuthenticationValidateEntityTokenRequestWrapper(const PFAuthenticationValidateEntityTokenRequest& model) :
        ModelWrapper<PFAuthenticationValidateEntityTokenRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entityToken{ SafeString(model.entityToken) }
    {
        SetModelPointers();
    }

    PFAuthenticationValidateEntityTokenRequestWrapper(const PFAuthenticationValidateEntityTokenRequestWrapper& src) :
        PFAuthenticationValidateEntityTokenRequestWrapper{ src.Model() }
    {
    }

    PFAuthenticationValidateEntityTokenRequestWrapper(PFAuthenticationValidateEntityTokenRequestWrapper&& src) :
        PFAuthenticationValidateEntityTokenRequestWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationValidateEntityTokenRequestWrapper& operator=(PFAuthenticationValidateEntityTokenRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationValidateEntityTokenRequestWrapper() = default;

    friend void swap(PFAuthenticationValidateEntityTokenRequestWrapper& lhs, PFAuthenticationValidateEntityTokenRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entityToken, rhs.m_entityToken);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntityToken(String value)
    {
        m_entityToken = std::move(value);
        this->m_model.entityToken =  m_entityToken.empty() ? nullptr : m_entityToken.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entityToken = m_entityToken.empty() ? nullptr : m_entityToken.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_entityToken;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAuthenticationValidateEntityTokenResponseWrapper : public ModelWrapper<PFAuthenticationValidateEntityTokenResponse, Alloc>
{
public:
    using ModelType = PFAuthenticationValidateEntityTokenResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAuthenticationValidateEntityTokenResponseWrapper() = default;

    PFAuthenticationValidateEntityTokenResponseWrapper(const PFAuthenticationValidateEntityTokenResponse& model) :
        ModelWrapper<PFAuthenticationValidateEntityTokenResponse, Alloc>{ model },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_identifiedDeviceType{ model.identifiedDeviceType ? std::optional<PFAuthenticationIdentifiedDeviceType>{ *model.identifiedDeviceType } : std::nullopt },
        m_identityProvider{ model.identityProvider ? std::optional<PFLoginIdentityProvider>{ *model.identityProvider } : std::nullopt },
        m_identityProviderIssuedId{ SafeString(model.identityProviderIssuedId) },
        m_lineage{ model.lineage ? std::optional<PFEntityLineageWrapper<Alloc>>{ *model.lineage } : std::nullopt }
    {
        SetModelPointers();
    }

    PFAuthenticationValidateEntityTokenResponseWrapper(const PFAuthenticationValidateEntityTokenResponseWrapper& src) :
        PFAuthenticationValidateEntityTokenResponseWrapper{ src.Model() }
    {
    }

    PFAuthenticationValidateEntityTokenResponseWrapper(PFAuthenticationValidateEntityTokenResponseWrapper&& src) :
        PFAuthenticationValidateEntityTokenResponseWrapper{}
    {
        swap(*this, src);
    }

    PFAuthenticationValidateEntityTokenResponseWrapper& operator=(PFAuthenticationValidateEntityTokenResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAuthenticationValidateEntityTokenResponseWrapper() = default;

    friend void swap(PFAuthenticationValidateEntityTokenResponseWrapper& lhs, PFAuthenticationValidateEntityTokenResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_identifiedDeviceType, rhs.m_identifiedDeviceType);
        swap(lhs.m_identityProvider, rhs.m_identityProvider);
        swap(lhs.m_identityProviderIssuedId, rhs.m_identityProviderIssuedId);
        swap(lhs.m_lineage, rhs.m_lineage);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    void SetIdentifiedDeviceType(std::optional<PFAuthenticationIdentifiedDeviceType> value)
    {
        m_identifiedDeviceType = std::move(value);
        this->m_model.identifiedDeviceType = m_identifiedDeviceType ? m_identifiedDeviceType.operator->() : nullptr;
    }

    void SetIdentityProvider(std::optional<PFLoginIdentityProvider> value)
    {
        m_identityProvider = std::move(value);
        this->m_model.identityProvider = m_identityProvider ? m_identityProvider.operator->() : nullptr;
    }

    void SetIdentityProviderIssuedId(String value)
    {
        m_identityProviderIssuedId = std::move(value);
        this->m_model.identityProviderIssuedId =  m_identityProviderIssuedId.empty() ? nullptr : m_identityProviderIssuedId.data();
    }

    void SetLineage(std::optional<PFEntityLineageWrapper<Alloc>> value)
    {
        m_lineage = std::move(value);
        this->m_model.lineage = m_lineage ? &m_lineage->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.identifiedDeviceType = m_identifiedDeviceType ? m_identifiedDeviceType.operator->() : nullptr;
        this->m_model.identityProvider = m_identityProvider ? m_identityProvider.operator->() : nullptr;
        this->m_model.identityProviderIssuedId = m_identityProviderIssuedId.empty() ? nullptr : m_identityProviderIssuedId.data();
        this->m_model.lineage = m_lineage ?  &m_lineage->Model() : nullptr;
    }

    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    std::optional<PFAuthenticationIdentifiedDeviceType> m_identifiedDeviceType;
    std::optional<PFLoginIdentityProvider> m_identityProvider;
    String m_identityProviderIssuedId;
    std::optional<PFEntityLineageWrapper<Alloc>> m_lineage;
};

} // namespace Wrappers
} // namespace PlayFab

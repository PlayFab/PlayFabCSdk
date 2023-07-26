// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFPlatformSpecificTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequestWrapper : public ModelWrapper<PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequest, Alloc>
{
public:
    using ModelType = PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequestWrapper() = default;

    PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequestWrapper(const PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequest& model) :
        ModelWrapper<PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequest, Alloc>{ model },
        m_confirmationMessage{ SafeString(model.confirmationMessage) },
        m_deviceToken{ SafeString(model.deviceToken) },
        m_sendPushNotificationConfirmation{ model.sendPushNotificationConfirmation ? std::optional<bool>{ *model.sendPushNotificationConfirmation } : std::nullopt }
    {
        SetModelPointers();
    }

    PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequestWrapper(const PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequestWrapper& src) :
        PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequestWrapper{ src.Model() }
    {
    }

    PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequestWrapper(PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequestWrapper&& src) :
        PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequestWrapper{}
    {
        swap(*this, src);
    }

    PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequestWrapper& operator=(PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequestWrapper() = default;

    friend void swap(PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequestWrapper& lhs, PFPlatformSpecificAndroidDevicePushNotificationRegistrationRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_confirmationMessage, rhs.m_confirmationMessage);
        swap(lhs.m_deviceToken, rhs.m_deviceToken);
        swap(lhs.m_sendPushNotificationConfirmation, rhs.m_sendPushNotificationConfirmation);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetConfirmationMessage(String value)
    {
        m_confirmationMessage = std::move(value);
        this->m_model.confirmationMessage =  m_confirmationMessage.empty() ? nullptr : m_confirmationMessage.data();
    }

    void SetDeviceToken(String value)
    {
        m_deviceToken = std::move(value);
        this->m_model.deviceToken =  m_deviceToken.empty() ? nullptr : m_deviceToken.data();
    }

    void SetSendPushNotificationConfirmation(std::optional<bool> value)
    {
        m_sendPushNotificationConfirmation = std::move(value);
        this->m_model.sendPushNotificationConfirmation = m_sendPushNotificationConfirmation ? m_sendPushNotificationConfirmation.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.confirmationMessage = m_confirmationMessage.empty() ? nullptr : m_confirmationMessage.data();
        this->m_model.deviceToken = m_deviceToken.empty() ? nullptr : m_deviceToken.data();
        this->m_model.sendPushNotificationConfirmation = m_sendPushNotificationConfirmation ? m_sendPushNotificationConfirmation.operator->() : nullptr;
    }

    String m_confirmationMessage;
    String m_deviceToken;
    std::optional<bool> m_sendPushNotificationConfirmation;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlatformSpecificRefreshPSNAuthTokenRequestWrapper : public ModelWrapper<PFPlatformSpecificRefreshPSNAuthTokenRequest, Alloc>
{
public:
    using ModelType = PFPlatformSpecificRefreshPSNAuthTokenRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlatformSpecificRefreshPSNAuthTokenRequestWrapper() = default;

    PFPlatformSpecificRefreshPSNAuthTokenRequestWrapper(const PFPlatformSpecificRefreshPSNAuthTokenRequest& model) :
        ModelWrapper<PFPlatformSpecificRefreshPSNAuthTokenRequest, Alloc>{ model },
        m_authCode{ SafeString(model.authCode) },
        m_issuerId{ model.issuerId ? std::optional<int32_t>{ *model.issuerId } : std::nullopt },
        m_redirectUri{ SafeString(model.redirectUri) }
    {
        SetModelPointers();
    }

    PFPlatformSpecificRefreshPSNAuthTokenRequestWrapper(const PFPlatformSpecificRefreshPSNAuthTokenRequestWrapper& src) :
        PFPlatformSpecificRefreshPSNAuthTokenRequestWrapper{ src.Model() }
    {
    }

    PFPlatformSpecificRefreshPSNAuthTokenRequestWrapper(PFPlatformSpecificRefreshPSNAuthTokenRequestWrapper&& src) :
        PFPlatformSpecificRefreshPSNAuthTokenRequestWrapper{}
    {
        swap(*this, src);
    }

    PFPlatformSpecificRefreshPSNAuthTokenRequestWrapper& operator=(PFPlatformSpecificRefreshPSNAuthTokenRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlatformSpecificRefreshPSNAuthTokenRequestWrapper() = default;

    friend void swap(PFPlatformSpecificRefreshPSNAuthTokenRequestWrapper& lhs, PFPlatformSpecificRefreshPSNAuthTokenRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_authCode, rhs.m_authCode);
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
        this->m_model.issuerId = m_issuerId ? m_issuerId.operator->() : nullptr;
        this->m_model.redirectUri = m_redirectUri.empty() ? nullptr : m_redirectUri.data();
    }

    String m_authCode;
    std::optional<int32_t> m_issuerId;
    String m_redirectUri;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlatformSpecificRegisterForIOSPushNotificationRequestWrapper : public ModelWrapper<PFPlatformSpecificRegisterForIOSPushNotificationRequest, Alloc>
{
public:
    using ModelType = PFPlatformSpecificRegisterForIOSPushNotificationRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlatformSpecificRegisterForIOSPushNotificationRequestWrapper() = default;

    PFPlatformSpecificRegisterForIOSPushNotificationRequestWrapper(const PFPlatformSpecificRegisterForIOSPushNotificationRequest& model) :
        ModelWrapper<PFPlatformSpecificRegisterForIOSPushNotificationRequest, Alloc>{ model },
        m_confirmationMessage{ SafeString(model.confirmationMessage) },
        m_deviceToken{ SafeString(model.deviceToken) },
        m_sendPushNotificationConfirmation{ model.sendPushNotificationConfirmation ? std::optional<bool>{ *model.sendPushNotificationConfirmation } : std::nullopt }
    {
        SetModelPointers();
    }

    PFPlatformSpecificRegisterForIOSPushNotificationRequestWrapper(const PFPlatformSpecificRegisterForIOSPushNotificationRequestWrapper& src) :
        PFPlatformSpecificRegisterForIOSPushNotificationRequestWrapper{ src.Model() }
    {
    }

    PFPlatformSpecificRegisterForIOSPushNotificationRequestWrapper(PFPlatformSpecificRegisterForIOSPushNotificationRequestWrapper&& src) :
        PFPlatformSpecificRegisterForIOSPushNotificationRequestWrapper{}
    {
        swap(*this, src);
    }

    PFPlatformSpecificRegisterForIOSPushNotificationRequestWrapper& operator=(PFPlatformSpecificRegisterForIOSPushNotificationRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlatformSpecificRegisterForIOSPushNotificationRequestWrapper() = default;

    friend void swap(PFPlatformSpecificRegisterForIOSPushNotificationRequestWrapper& lhs, PFPlatformSpecificRegisterForIOSPushNotificationRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_confirmationMessage, rhs.m_confirmationMessage);
        swap(lhs.m_deviceToken, rhs.m_deviceToken);
        swap(lhs.m_sendPushNotificationConfirmation, rhs.m_sendPushNotificationConfirmation);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetConfirmationMessage(String value)
    {
        m_confirmationMessage = std::move(value);
        this->m_model.confirmationMessage =  m_confirmationMessage.empty() ? nullptr : m_confirmationMessage.data();
    }

    void SetDeviceToken(String value)
    {
        m_deviceToken = std::move(value);
        this->m_model.deviceToken =  m_deviceToken.empty() ? nullptr : m_deviceToken.data();
    }

    void SetSendPushNotificationConfirmation(std::optional<bool> value)
    {
        m_sendPushNotificationConfirmation = std::move(value);
        this->m_model.sendPushNotificationConfirmation = m_sendPushNotificationConfirmation ? m_sendPushNotificationConfirmation.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.confirmationMessage = m_confirmationMessage.empty() ? nullptr : m_confirmationMessage.data();
        this->m_model.deviceToken = m_deviceToken.empty() ? nullptr : m_deviceToken.data();
        this->m_model.sendPushNotificationConfirmation = m_sendPushNotificationConfirmation ? m_sendPushNotificationConfirmation.operator->() : nullptr;
    }

    String m_confirmationMessage;
    String m_deviceToken;
    std::optional<bool> m_sendPushNotificationConfirmation;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlatformSpecificAwardSteamAchievementItemWrapper : public ModelWrapper<PFPlatformSpecificAwardSteamAchievementItem, Alloc>
{
public:
    using ModelType = PFPlatformSpecificAwardSteamAchievementItem;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlatformSpecificAwardSteamAchievementItemWrapper() = default;

    PFPlatformSpecificAwardSteamAchievementItemWrapper(const PFPlatformSpecificAwardSteamAchievementItem& model) :
        ModelWrapper<PFPlatformSpecificAwardSteamAchievementItem, Alloc>{ model },
        m_achievementName{ SafeString(model.achievementName) },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFPlatformSpecificAwardSteamAchievementItemWrapper(const PFPlatformSpecificAwardSteamAchievementItemWrapper& src) :
        PFPlatformSpecificAwardSteamAchievementItemWrapper{ src.Model() }
    {
    }

    PFPlatformSpecificAwardSteamAchievementItemWrapper(PFPlatformSpecificAwardSteamAchievementItemWrapper&& src) :
        PFPlatformSpecificAwardSteamAchievementItemWrapper{}
    {
        swap(*this, src);
    }

    PFPlatformSpecificAwardSteamAchievementItemWrapper& operator=(PFPlatformSpecificAwardSteamAchievementItemWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlatformSpecificAwardSteamAchievementItemWrapper() = default;

    friend void swap(PFPlatformSpecificAwardSteamAchievementItemWrapper& lhs, PFPlatformSpecificAwardSteamAchievementItemWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_achievementName, rhs.m_achievementName);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAchievementName(String value)
    {
        m_achievementName = std::move(value);
        this->m_model.achievementName =  m_achievementName.empty() ? nullptr : m_achievementName.data();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetResult(bool value)
    {
        this->m_model.result = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.achievementName = m_achievementName.empty() ? nullptr : m_achievementName.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String m_achievementName;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlatformSpecificAwardSteamAchievementRequestWrapper : public ModelWrapper<PFPlatformSpecificAwardSteamAchievementRequest, Alloc>
{
public:
    using ModelType = PFPlatformSpecificAwardSteamAchievementRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlatformSpecificAwardSteamAchievementRequestWrapper() = default;

    PFPlatformSpecificAwardSteamAchievementRequestWrapper(const PFPlatformSpecificAwardSteamAchievementRequest& model) :
        ModelWrapper<PFPlatformSpecificAwardSteamAchievementRequest, Alloc>{ model },
        m_achievements{ model.achievements, model.achievements + model.achievementsCount }
    {
        SetModelPointers();
    }

    PFPlatformSpecificAwardSteamAchievementRequestWrapper(const PFPlatformSpecificAwardSteamAchievementRequestWrapper& src) :
        PFPlatformSpecificAwardSteamAchievementRequestWrapper{ src.Model() }
    {
    }

    PFPlatformSpecificAwardSteamAchievementRequestWrapper(PFPlatformSpecificAwardSteamAchievementRequestWrapper&& src) :
        PFPlatformSpecificAwardSteamAchievementRequestWrapper{}
    {
        swap(*this, src);
    }

    PFPlatformSpecificAwardSteamAchievementRequestWrapper& operator=(PFPlatformSpecificAwardSteamAchievementRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlatformSpecificAwardSteamAchievementRequestWrapper() = default;

    friend void swap(PFPlatformSpecificAwardSteamAchievementRequestWrapper& lhs, PFPlatformSpecificAwardSteamAchievementRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_achievements, rhs.m_achievements);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAchievements(ModelVector<PFPlatformSpecificAwardSteamAchievementItemWrapper<Alloc>, Alloc> value)
    {
        m_achievements = std::move(value);
        this->m_model.achievements =  m_achievements.empty() ? nullptr : m_achievements.data();
        this->m_model.achievementsCount =  static_cast<uint32_t>(m_achievements.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.achievements = m_achievements.empty() ? nullptr : m_achievements.data();
    }

    ModelVector<PFPlatformSpecificAwardSteamAchievementItemWrapper<Alloc>, Alloc> m_achievements;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlatformSpecificAwardSteamAchievementResultWrapper : public ModelWrapper<PFPlatformSpecificAwardSteamAchievementResult, Alloc>
{
public:
    using ModelType = PFPlatformSpecificAwardSteamAchievementResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlatformSpecificAwardSteamAchievementResultWrapper() = default;

    PFPlatformSpecificAwardSteamAchievementResultWrapper(const PFPlatformSpecificAwardSteamAchievementResult& model) :
        ModelWrapper<PFPlatformSpecificAwardSteamAchievementResult, Alloc>{ model },
        m_achievementResults{ model.achievementResults, model.achievementResults + model.achievementResultsCount }
    {
        SetModelPointers();
    }

    PFPlatformSpecificAwardSteamAchievementResultWrapper(const PFPlatformSpecificAwardSteamAchievementResultWrapper& src) :
        PFPlatformSpecificAwardSteamAchievementResultWrapper{ src.Model() }
    {
    }

    PFPlatformSpecificAwardSteamAchievementResultWrapper(PFPlatformSpecificAwardSteamAchievementResultWrapper&& src) :
        PFPlatformSpecificAwardSteamAchievementResultWrapper{}
    {
        swap(*this, src);
    }

    PFPlatformSpecificAwardSteamAchievementResultWrapper& operator=(PFPlatformSpecificAwardSteamAchievementResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlatformSpecificAwardSteamAchievementResultWrapper() = default;

    friend void swap(PFPlatformSpecificAwardSteamAchievementResultWrapper& lhs, PFPlatformSpecificAwardSteamAchievementResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_achievementResults, rhs.m_achievementResults);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAchievementResults(ModelVector<PFPlatformSpecificAwardSteamAchievementItemWrapper<Alloc>, Alloc> value)
    {
        m_achievementResults = std::move(value);
        this->m_model.achievementResults =  m_achievementResults.empty() ? nullptr : m_achievementResults.data();
        this->m_model.achievementResultsCount =  static_cast<uint32_t>(m_achievementResults.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.achievementResults = m_achievementResults.empty() ? nullptr : m_achievementResults.data();
    }

    ModelVector<PFPlatformSpecificAwardSteamAchievementItemWrapper<Alloc>, Alloc> m_achievementResults;
};

} // namespace Wrappers
} // namespace PlayFab

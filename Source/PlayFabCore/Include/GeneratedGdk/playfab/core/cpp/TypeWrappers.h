// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/cpp/TypeWrapperHelpers.h>
#include <playfab/core/cpp/PlayFabException.h>
#include <playfab/core/PFTypes.h>

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFItemInstanceWrapper : public ModelWrapper<PFItemInstance, Alloc>
{
public:
    using ModelType = PFItemInstance;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFItemInstanceWrapper() = default;

    PFItemInstanceWrapper(const PFItemInstance& model) :
        ModelWrapper<PFItemInstance, Alloc>{ model },
        m_annotation{ SafeString(model.annotation) },
        m_bundleContents{ model.bundleContents, model.bundleContents + model.bundleContentsCount },
        m_bundleParent{ SafeString(model.bundleParent) },
        m_catalogVersion{ SafeString(model.catalogVersion) },
        m_customData{ model.customData, model.customData + model.customDataCount },
        m_displayName{ SafeString(model.displayName) },
        m_expiration{ model.expiration ? std::optional<time_t>{ *model.expiration } : std::nullopt },
        m_itemClass{ SafeString(model.itemClass) },
        m_itemId{ SafeString(model.itemId) },
        m_itemInstanceId{ SafeString(model.itemInstanceId) },
        m_purchaseDate{ model.purchaseDate ? std::optional<time_t>{ *model.purchaseDate } : std::nullopt },
        m_remainingUses{ model.remainingUses ? std::optional<int32_t>{ *model.remainingUses } : std::nullopt },
        m_unitCurrency{ SafeString(model.unitCurrency) },
        m_usesIncrementedBy{ model.usesIncrementedBy ? std::optional<int32_t>{ *model.usesIncrementedBy } : std::nullopt }
    {
        SetModelPointers();
    }

    PFItemInstanceWrapper(const PFItemInstanceWrapper& src) :
        PFItemInstanceWrapper{ src.Model() }
    {
    }

    PFItemInstanceWrapper(PFItemInstanceWrapper&& src) :
        PFItemInstanceWrapper{}
    {
        swap(*this, src);
    }

    PFItemInstanceWrapper& operator=(PFItemInstanceWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFItemInstanceWrapper() = default;

    friend void swap(PFItemInstanceWrapper& lhs, PFItemInstanceWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_annotation, rhs.m_annotation);
        swap(lhs.m_bundleContents, rhs.m_bundleContents);
        swap(lhs.m_bundleParent, rhs.m_bundleParent);
        swap(lhs.m_catalogVersion, rhs.m_catalogVersion);
        swap(lhs.m_customData, rhs.m_customData);
        swap(lhs.m_displayName, rhs.m_displayName);
        swap(lhs.m_expiration, rhs.m_expiration);
        swap(lhs.m_itemClass, rhs.m_itemClass);
        swap(lhs.m_itemId, rhs.m_itemId);
        swap(lhs.m_itemInstanceId, rhs.m_itemInstanceId);
        swap(lhs.m_purchaseDate, rhs.m_purchaseDate);
        swap(lhs.m_remainingUses, rhs.m_remainingUses);
        swap(lhs.m_unitCurrency, rhs.m_unitCurrency);
        swap(lhs.m_usesIncrementedBy, rhs.m_usesIncrementedBy);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAnnotation(String value)
    {
        m_annotation = std::move(value);
        this->m_model.annotation =  m_annotation.empty() ? nullptr : m_annotation.data();
    }

    void SetBundleContents(CStringVector<Alloc> value)
    {
        m_bundleContents = std::move(value);
        this->m_model.bundleContents =  m_bundleContents.empty() ? nullptr : m_bundleContents.data();
        this->m_model.bundleContentsCount =  static_cast<uint32_t>(m_bundleContents.size());
    }

    void SetBundleParent(String value)
    {
        m_bundleParent = std::move(value);
        this->m_model.bundleParent =  m_bundleParent.empty() ? nullptr : m_bundleParent.data();
    }

    void SetCatalogVersion(String value)
    {
        m_catalogVersion = std::move(value);
        this->m_model.catalogVersion =  m_catalogVersion.empty() ? nullptr : m_catalogVersion.data();
    }

    void SetCustomData(StringDictionaryEntryVector<Alloc> value)
    {
        m_customData = std::move(value);
        this->m_model.customData =  m_customData.empty() ? nullptr : m_customData.data();
        this->m_model.customDataCount =  static_cast<uint32_t>(m_customData.size());
    }

    void SetDisplayName(String value)
    {
        m_displayName = std::move(value);
        this->m_model.displayName =  m_displayName.empty() ? nullptr : m_displayName.data();
    }

    void SetExpiration(std::optional<time_t> value)
    {
        m_expiration = std::move(value);
        this->m_model.expiration = m_expiration ? m_expiration.operator->() : nullptr;
    }

    void SetItemClass(String value)
    {
        m_itemClass = std::move(value);
        this->m_model.itemClass =  m_itemClass.empty() ? nullptr : m_itemClass.data();
    }

    void SetItemId(String value)
    {
        m_itemId = std::move(value);
        this->m_model.itemId =  m_itemId.empty() ? nullptr : m_itemId.data();
    }

    void SetItemInstanceId(String value)
    {
        m_itemInstanceId = std::move(value);
        this->m_model.itemInstanceId =  m_itemInstanceId.empty() ? nullptr : m_itemInstanceId.data();
    }

    void SetPurchaseDate(std::optional<time_t> value)
    {
        m_purchaseDate = std::move(value);
        this->m_model.purchaseDate = m_purchaseDate ? m_purchaseDate.operator->() : nullptr;
    }

    void SetRemainingUses(std::optional<int32_t> value)
    {
        m_remainingUses = std::move(value);
        this->m_model.remainingUses = m_remainingUses ? m_remainingUses.operator->() : nullptr;
    }

    void SetUnitCurrency(String value)
    {
        m_unitCurrency = std::move(value);
        this->m_model.unitCurrency =  m_unitCurrency.empty() ? nullptr : m_unitCurrency.data();
    }

    void SetUnitPrice(uint32_t value)
    {
        this->m_model.unitPrice = value;
    }

    void SetUsesIncrementedBy(std::optional<int32_t> value)
    {
        m_usesIncrementedBy = std::move(value);
        this->m_model.usesIncrementedBy = m_usesIncrementedBy ? m_usesIncrementedBy.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.annotation = m_annotation.empty() ? nullptr : m_annotation.data();
        this->m_model.bundleContents = m_bundleContents.empty() ? nullptr : m_bundleContents.data();
        this->m_model.bundleParent = m_bundleParent.empty() ? nullptr : m_bundleParent.data();
        this->m_model.catalogVersion = m_catalogVersion.empty() ? nullptr : m_catalogVersion.data();
        this->m_model.customData = m_customData.empty() ? nullptr : m_customData.data();
        this->m_model.displayName = m_displayName.empty() ? nullptr : m_displayName.data();
        this->m_model.expiration = m_expiration ? m_expiration.operator->() : nullptr;
        this->m_model.itemClass = m_itemClass.empty() ? nullptr : m_itemClass.data();
        this->m_model.itemId = m_itemId.empty() ? nullptr : m_itemId.data();
        this->m_model.itemInstanceId = m_itemInstanceId.empty() ? nullptr : m_itemInstanceId.data();
        this->m_model.purchaseDate = m_purchaseDate ? m_purchaseDate.operator->() : nullptr;
        this->m_model.remainingUses = m_remainingUses ? m_remainingUses.operator->() : nullptr;
        this->m_model.unitCurrency = m_unitCurrency.empty() ? nullptr : m_unitCurrency.data();
        this->m_model.usesIncrementedBy = m_usesIncrementedBy ? m_usesIncrementedBy.operator->() : nullptr;
    }

    String m_annotation;
    CStringVector<Alloc> m_bundleContents;
    String m_bundleParent;
    String m_catalogVersion;
    StringDictionaryEntryVector<Alloc> m_customData;
    String m_displayName;
    std::optional<time_t> m_expiration;
    String m_itemClass;
    String m_itemId;
    String m_itemInstanceId;
    std::optional<time_t> m_purchaseDate;
    std::optional<int32_t> m_remainingUses;
    String m_unitCurrency;
    std::optional<int32_t> m_usesIncrementedBy;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFUserAndroidDeviceInfoWrapper : public ModelWrapper<PFUserAndroidDeviceInfo, Alloc>
{
public:
    using ModelType = PFUserAndroidDeviceInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFUserAndroidDeviceInfoWrapper() = default;

    PFUserAndroidDeviceInfoWrapper(const PFUserAndroidDeviceInfo& model) :
        ModelWrapper<PFUserAndroidDeviceInfo, Alloc>{ model },
        m_androidDeviceId{ SafeString(model.androidDeviceId) }
    {
        SetModelPointers();
    }

    PFUserAndroidDeviceInfoWrapper(const PFUserAndroidDeviceInfoWrapper& src) :
        PFUserAndroidDeviceInfoWrapper{ src.Model() }
    {
    }

    PFUserAndroidDeviceInfoWrapper(PFUserAndroidDeviceInfoWrapper&& src) :
        PFUserAndroidDeviceInfoWrapper{}
    {
        swap(*this, src);
    }

    PFUserAndroidDeviceInfoWrapper& operator=(PFUserAndroidDeviceInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFUserAndroidDeviceInfoWrapper() = default;

    friend void swap(PFUserAndroidDeviceInfoWrapper& lhs, PFUserAndroidDeviceInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_androidDeviceId, rhs.m_androidDeviceId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAndroidDeviceId(String value)
    {
        m_androidDeviceId = std::move(value);
        this->m_model.androidDeviceId =  m_androidDeviceId.empty() ? nullptr : m_androidDeviceId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.androidDeviceId = m_androidDeviceId.empty() ? nullptr : m_androidDeviceId.data();
    }

    String m_androidDeviceId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFUserAppleIdInfoWrapper : public ModelWrapper<PFUserAppleIdInfo, Alloc>
{
public:
    using ModelType = PFUserAppleIdInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFUserAppleIdInfoWrapper() = default;

    PFUserAppleIdInfoWrapper(const PFUserAppleIdInfo& model) :
        ModelWrapper<PFUserAppleIdInfo, Alloc>{ model },
        m_appleSubjectId{ SafeString(model.appleSubjectId) }
    {
        SetModelPointers();
    }

    PFUserAppleIdInfoWrapper(const PFUserAppleIdInfoWrapper& src) :
        PFUserAppleIdInfoWrapper{ src.Model() }
    {
    }

    PFUserAppleIdInfoWrapper(PFUserAppleIdInfoWrapper&& src) :
        PFUserAppleIdInfoWrapper{}
    {
        swap(*this, src);
    }

    PFUserAppleIdInfoWrapper& operator=(PFUserAppleIdInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFUserAppleIdInfoWrapper() = default;

    friend void swap(PFUserAppleIdInfoWrapper& lhs, PFUserAppleIdInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_appleSubjectId, rhs.m_appleSubjectId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAppleSubjectId(String value)
    {
        m_appleSubjectId = std::move(value);
        this->m_model.appleSubjectId =  m_appleSubjectId.empty() ? nullptr : m_appleSubjectId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.appleSubjectId = m_appleSubjectId.empty() ? nullptr : m_appleSubjectId.data();
    }

    String m_appleSubjectId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFUserCustomIdInfoWrapper : public ModelWrapper<PFUserCustomIdInfo, Alloc>
{
public:
    using ModelType = PFUserCustomIdInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFUserCustomIdInfoWrapper() = default;

    PFUserCustomIdInfoWrapper(const PFUserCustomIdInfo& model) :
        ModelWrapper<PFUserCustomIdInfo, Alloc>{ model },
        m_customId{ SafeString(model.customId) }
    {
        SetModelPointers();
    }

    PFUserCustomIdInfoWrapper(const PFUserCustomIdInfoWrapper& src) :
        PFUserCustomIdInfoWrapper{ src.Model() }
    {
    }

    PFUserCustomIdInfoWrapper(PFUserCustomIdInfoWrapper&& src) :
        PFUserCustomIdInfoWrapper{}
    {
        swap(*this, src);
    }

    PFUserCustomIdInfoWrapper& operator=(PFUserCustomIdInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFUserCustomIdInfoWrapper() = default;

    friend void swap(PFUserCustomIdInfoWrapper& lhs, PFUserCustomIdInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customId, rhs.m_customId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomId(String value)
    {
        m_customId = std::move(value);
        this->m_model.customId =  m_customId.empty() ? nullptr : m_customId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customId = m_customId.empty() ? nullptr : m_customId.data();
    }

    String m_customId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFUserFacebookInfoWrapper : public ModelWrapper<PFUserFacebookInfo, Alloc>
{
public:
    using ModelType = PFUserFacebookInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFUserFacebookInfoWrapper() = default;

    PFUserFacebookInfoWrapper(const PFUserFacebookInfo& model) :
        ModelWrapper<PFUserFacebookInfo, Alloc>{ model },
        m_facebookId{ SafeString(model.facebookId) },
        m_fullName{ SafeString(model.fullName) }
    {
        SetModelPointers();
    }

    PFUserFacebookInfoWrapper(const PFUserFacebookInfoWrapper& src) :
        PFUserFacebookInfoWrapper{ src.Model() }
    {
    }

    PFUserFacebookInfoWrapper(PFUserFacebookInfoWrapper&& src) :
        PFUserFacebookInfoWrapper{}
    {
        swap(*this, src);
    }

    PFUserFacebookInfoWrapper& operator=(PFUserFacebookInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFUserFacebookInfoWrapper() = default;

    friend void swap(PFUserFacebookInfoWrapper& lhs, PFUserFacebookInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_facebookId, rhs.m_facebookId);
        swap(lhs.m_fullName, rhs.m_fullName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFacebookId(String value)
    {
        m_facebookId = std::move(value);
        this->m_model.facebookId =  m_facebookId.empty() ? nullptr : m_facebookId.data();
    }

    void SetFullName(String value)
    {
        m_fullName = std::move(value);
        this->m_model.fullName =  m_fullName.empty() ? nullptr : m_fullName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.facebookId = m_facebookId.empty() ? nullptr : m_facebookId.data();
        this->m_model.fullName = m_fullName.empty() ? nullptr : m_fullName.data();
    }

    String m_facebookId;
    String m_fullName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFUserFacebookInstantGamesIdInfoWrapper : public ModelWrapper<PFUserFacebookInstantGamesIdInfo, Alloc>
{
public:
    using ModelType = PFUserFacebookInstantGamesIdInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFUserFacebookInstantGamesIdInfoWrapper() = default;

    PFUserFacebookInstantGamesIdInfoWrapper(const PFUserFacebookInstantGamesIdInfo& model) :
        ModelWrapper<PFUserFacebookInstantGamesIdInfo, Alloc>{ model },
        m_facebookInstantGamesId{ SafeString(model.facebookInstantGamesId) }
    {
        SetModelPointers();
    }

    PFUserFacebookInstantGamesIdInfoWrapper(const PFUserFacebookInstantGamesIdInfoWrapper& src) :
        PFUserFacebookInstantGamesIdInfoWrapper{ src.Model() }
    {
    }

    PFUserFacebookInstantGamesIdInfoWrapper(PFUserFacebookInstantGamesIdInfoWrapper&& src) :
        PFUserFacebookInstantGamesIdInfoWrapper{}
    {
        swap(*this, src);
    }

    PFUserFacebookInstantGamesIdInfoWrapper& operator=(PFUserFacebookInstantGamesIdInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFUserFacebookInstantGamesIdInfoWrapper() = default;

    friend void swap(PFUserFacebookInstantGamesIdInfoWrapper& lhs, PFUserFacebookInstantGamesIdInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_facebookInstantGamesId, rhs.m_facebookInstantGamesId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFacebookInstantGamesId(String value)
    {
        m_facebookInstantGamesId = std::move(value);
        this->m_model.facebookInstantGamesId =  m_facebookInstantGamesId.empty() ? nullptr : m_facebookInstantGamesId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.facebookInstantGamesId = m_facebookInstantGamesId.empty() ? nullptr : m_facebookInstantGamesId.data();
    }

    String m_facebookInstantGamesId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFUserGameCenterInfoWrapper : public ModelWrapper<PFUserGameCenterInfo, Alloc>
{
public:
    using ModelType = PFUserGameCenterInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFUserGameCenterInfoWrapper() = default;

    PFUserGameCenterInfoWrapper(const PFUserGameCenterInfo& model) :
        ModelWrapper<PFUserGameCenterInfo, Alloc>{ model },
        m_gameCenterId{ SafeString(model.gameCenterId) }
    {
        SetModelPointers();
    }

    PFUserGameCenterInfoWrapper(const PFUserGameCenterInfoWrapper& src) :
        PFUserGameCenterInfoWrapper{ src.Model() }
    {
    }

    PFUserGameCenterInfoWrapper(PFUserGameCenterInfoWrapper&& src) :
        PFUserGameCenterInfoWrapper{}
    {
        swap(*this, src);
    }

    PFUserGameCenterInfoWrapper& operator=(PFUserGameCenterInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFUserGameCenterInfoWrapper() = default;

    friend void swap(PFUserGameCenterInfoWrapper& lhs, PFUserGameCenterInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_gameCenterId, rhs.m_gameCenterId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetGameCenterId(String value)
    {
        m_gameCenterId = std::move(value);
        this->m_model.gameCenterId =  m_gameCenterId.empty() ? nullptr : m_gameCenterId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.gameCenterId = m_gameCenterId.empty() ? nullptr : m_gameCenterId.data();
    }

    String m_gameCenterId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFUserGoogleInfoWrapper : public ModelWrapper<PFUserGoogleInfo, Alloc>
{
public:
    using ModelType = PFUserGoogleInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFUserGoogleInfoWrapper() = default;

    PFUserGoogleInfoWrapper(const PFUserGoogleInfo& model) :
        ModelWrapper<PFUserGoogleInfo, Alloc>{ model },
        m_googleEmail{ SafeString(model.googleEmail) },
        m_googleGender{ SafeString(model.googleGender) },
        m_googleId{ SafeString(model.googleId) },
        m_googleLocale{ SafeString(model.googleLocale) },
        m_googleName{ SafeString(model.googleName) }
    {
        SetModelPointers();
    }

    PFUserGoogleInfoWrapper(const PFUserGoogleInfoWrapper& src) :
        PFUserGoogleInfoWrapper{ src.Model() }
    {
    }

    PFUserGoogleInfoWrapper(PFUserGoogleInfoWrapper&& src) :
        PFUserGoogleInfoWrapper{}
    {
        swap(*this, src);
    }

    PFUserGoogleInfoWrapper& operator=(PFUserGoogleInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFUserGoogleInfoWrapper() = default;

    friend void swap(PFUserGoogleInfoWrapper& lhs, PFUserGoogleInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_googleEmail, rhs.m_googleEmail);
        swap(lhs.m_googleGender, rhs.m_googleGender);
        swap(lhs.m_googleId, rhs.m_googleId);
        swap(lhs.m_googleLocale, rhs.m_googleLocale);
        swap(lhs.m_googleName, rhs.m_googleName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetGoogleEmail(String value)
    {
        m_googleEmail = std::move(value);
        this->m_model.googleEmail =  m_googleEmail.empty() ? nullptr : m_googleEmail.data();
    }

    void SetGoogleGender(String value)
    {
        m_googleGender = std::move(value);
        this->m_model.googleGender =  m_googleGender.empty() ? nullptr : m_googleGender.data();
    }

    void SetGoogleId(String value)
    {
        m_googleId = std::move(value);
        this->m_model.googleId =  m_googleId.empty() ? nullptr : m_googleId.data();
    }

    void SetGoogleLocale(String value)
    {
        m_googleLocale = std::move(value);
        this->m_model.googleLocale =  m_googleLocale.empty() ? nullptr : m_googleLocale.data();
    }

    void SetGoogleName(String value)
    {
        m_googleName = std::move(value);
        this->m_model.googleName =  m_googleName.empty() ? nullptr : m_googleName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.googleEmail = m_googleEmail.empty() ? nullptr : m_googleEmail.data();
        this->m_model.googleGender = m_googleGender.empty() ? nullptr : m_googleGender.data();
        this->m_model.googleId = m_googleId.empty() ? nullptr : m_googleId.data();
        this->m_model.googleLocale = m_googleLocale.empty() ? nullptr : m_googleLocale.data();
        this->m_model.googleName = m_googleName.empty() ? nullptr : m_googleName.data();
    }

    String m_googleEmail;
    String m_googleGender;
    String m_googleId;
    String m_googleLocale;
    String m_googleName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFUserGooglePlayGamesInfoWrapper : public ModelWrapper<PFUserGooglePlayGamesInfo, Alloc>
{
public:
    using ModelType = PFUserGooglePlayGamesInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFUserGooglePlayGamesInfoWrapper() = default;

    PFUserGooglePlayGamesInfoWrapper(const PFUserGooglePlayGamesInfo& model) :
        ModelWrapper<PFUserGooglePlayGamesInfo, Alloc>{ model },
        m_googlePlayGamesPlayerAvatarImageUrl{ SafeString(model.googlePlayGamesPlayerAvatarImageUrl) },
        m_googlePlayGamesPlayerDisplayName{ SafeString(model.googlePlayGamesPlayerDisplayName) },
        m_googlePlayGamesPlayerId{ SafeString(model.googlePlayGamesPlayerId) }
    {
        SetModelPointers();
    }

    PFUserGooglePlayGamesInfoWrapper(const PFUserGooglePlayGamesInfoWrapper& src) :
        PFUserGooglePlayGamesInfoWrapper{ src.Model() }
    {
    }

    PFUserGooglePlayGamesInfoWrapper(PFUserGooglePlayGamesInfoWrapper&& src) :
        PFUserGooglePlayGamesInfoWrapper{}
    {
        swap(*this, src);
    }

    PFUserGooglePlayGamesInfoWrapper& operator=(PFUserGooglePlayGamesInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFUserGooglePlayGamesInfoWrapper() = default;

    friend void swap(PFUserGooglePlayGamesInfoWrapper& lhs, PFUserGooglePlayGamesInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_googlePlayGamesPlayerAvatarImageUrl, rhs.m_googlePlayGamesPlayerAvatarImageUrl);
        swap(lhs.m_googlePlayGamesPlayerDisplayName, rhs.m_googlePlayGamesPlayerDisplayName);
        swap(lhs.m_googlePlayGamesPlayerId, rhs.m_googlePlayGamesPlayerId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetGooglePlayGamesPlayerAvatarImageUrl(String value)
    {
        m_googlePlayGamesPlayerAvatarImageUrl = std::move(value);
        this->m_model.googlePlayGamesPlayerAvatarImageUrl =  m_googlePlayGamesPlayerAvatarImageUrl.empty() ? nullptr : m_googlePlayGamesPlayerAvatarImageUrl.data();
    }

    void SetGooglePlayGamesPlayerDisplayName(String value)
    {
        m_googlePlayGamesPlayerDisplayName = std::move(value);
        this->m_model.googlePlayGamesPlayerDisplayName =  m_googlePlayGamesPlayerDisplayName.empty() ? nullptr : m_googlePlayGamesPlayerDisplayName.data();
    }

    void SetGooglePlayGamesPlayerId(String value)
    {
        m_googlePlayGamesPlayerId = std::move(value);
        this->m_model.googlePlayGamesPlayerId =  m_googlePlayGamesPlayerId.empty() ? nullptr : m_googlePlayGamesPlayerId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.googlePlayGamesPlayerAvatarImageUrl = m_googlePlayGamesPlayerAvatarImageUrl.empty() ? nullptr : m_googlePlayGamesPlayerAvatarImageUrl.data();
        this->m_model.googlePlayGamesPlayerDisplayName = m_googlePlayGamesPlayerDisplayName.empty() ? nullptr : m_googlePlayGamesPlayerDisplayName.data();
        this->m_model.googlePlayGamesPlayerId = m_googlePlayGamesPlayerId.empty() ? nullptr : m_googlePlayGamesPlayerId.data();
    }

    String m_googlePlayGamesPlayerAvatarImageUrl;
    String m_googlePlayGamesPlayerDisplayName;
    String m_googlePlayGamesPlayerId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFUserIosDeviceInfoWrapper : public ModelWrapper<PFUserIosDeviceInfo, Alloc>
{
public:
    using ModelType = PFUserIosDeviceInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFUserIosDeviceInfoWrapper() = default;

    PFUserIosDeviceInfoWrapper(const PFUserIosDeviceInfo& model) :
        ModelWrapper<PFUserIosDeviceInfo, Alloc>{ model },
        m_iosDeviceId{ SafeString(model.iosDeviceId) }
    {
        SetModelPointers();
    }

    PFUserIosDeviceInfoWrapper(const PFUserIosDeviceInfoWrapper& src) :
        PFUserIosDeviceInfoWrapper{ src.Model() }
    {
    }

    PFUserIosDeviceInfoWrapper(PFUserIosDeviceInfoWrapper&& src) :
        PFUserIosDeviceInfoWrapper{}
    {
        swap(*this, src);
    }

    PFUserIosDeviceInfoWrapper& operator=(PFUserIosDeviceInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFUserIosDeviceInfoWrapper() = default;

    friend void swap(PFUserIosDeviceInfoWrapper& lhs, PFUserIosDeviceInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_iosDeviceId, rhs.m_iosDeviceId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetIosDeviceId(String value)
    {
        m_iosDeviceId = std::move(value);
        this->m_model.iosDeviceId =  m_iosDeviceId.empty() ? nullptr : m_iosDeviceId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.iosDeviceId = m_iosDeviceId.empty() ? nullptr : m_iosDeviceId.data();
    }

    String m_iosDeviceId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFUserKongregateInfoWrapper : public ModelWrapper<PFUserKongregateInfo, Alloc>
{
public:
    using ModelType = PFUserKongregateInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFUserKongregateInfoWrapper() = default;

    PFUserKongregateInfoWrapper(const PFUserKongregateInfo& model) :
        ModelWrapper<PFUserKongregateInfo, Alloc>{ model },
        m_kongregateId{ SafeString(model.kongregateId) },
        m_kongregateName{ SafeString(model.kongregateName) }
    {
        SetModelPointers();
    }

    PFUserKongregateInfoWrapper(const PFUserKongregateInfoWrapper& src) :
        PFUserKongregateInfoWrapper{ src.Model() }
    {
    }

    PFUserKongregateInfoWrapper(PFUserKongregateInfoWrapper&& src) :
        PFUserKongregateInfoWrapper{}
    {
        swap(*this, src);
    }

    PFUserKongregateInfoWrapper& operator=(PFUserKongregateInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFUserKongregateInfoWrapper() = default;

    friend void swap(PFUserKongregateInfoWrapper& lhs, PFUserKongregateInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_kongregateId, rhs.m_kongregateId);
        swap(lhs.m_kongregateName, rhs.m_kongregateName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetKongregateId(String value)
    {
        m_kongregateId = std::move(value);
        this->m_model.kongregateId =  m_kongregateId.empty() ? nullptr : m_kongregateId.data();
    }

    void SetKongregateName(String value)
    {
        m_kongregateName = std::move(value);
        this->m_model.kongregateName =  m_kongregateName.empty() ? nullptr : m_kongregateName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.kongregateId = m_kongregateId.empty() ? nullptr : m_kongregateId.data();
        this->m_model.kongregateName = m_kongregateName.empty() ? nullptr : m_kongregateName.data();
    }

    String m_kongregateId;
    String m_kongregateName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFUserNintendoSwitchAccountIdInfoWrapper : public ModelWrapper<PFUserNintendoSwitchAccountIdInfo, Alloc>
{
public:
    using ModelType = PFUserNintendoSwitchAccountIdInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFUserNintendoSwitchAccountIdInfoWrapper() = default;

    PFUserNintendoSwitchAccountIdInfoWrapper(const PFUserNintendoSwitchAccountIdInfo& model) :
        ModelWrapper<PFUserNintendoSwitchAccountIdInfo, Alloc>{ model },
        m_nintendoSwitchAccountSubjectId{ SafeString(model.nintendoSwitchAccountSubjectId) }
    {
        SetModelPointers();
    }

    PFUserNintendoSwitchAccountIdInfoWrapper(const PFUserNintendoSwitchAccountIdInfoWrapper& src) :
        PFUserNintendoSwitchAccountIdInfoWrapper{ src.Model() }
    {
    }

    PFUserNintendoSwitchAccountIdInfoWrapper(PFUserNintendoSwitchAccountIdInfoWrapper&& src) :
        PFUserNintendoSwitchAccountIdInfoWrapper{}
    {
        swap(*this, src);
    }

    PFUserNintendoSwitchAccountIdInfoWrapper& operator=(PFUserNintendoSwitchAccountIdInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFUserNintendoSwitchAccountIdInfoWrapper() = default;

    friend void swap(PFUserNintendoSwitchAccountIdInfoWrapper& lhs, PFUserNintendoSwitchAccountIdInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_nintendoSwitchAccountSubjectId, rhs.m_nintendoSwitchAccountSubjectId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetNintendoSwitchAccountSubjectId(String value)
    {
        m_nintendoSwitchAccountSubjectId = std::move(value);
        this->m_model.nintendoSwitchAccountSubjectId =  m_nintendoSwitchAccountSubjectId.empty() ? nullptr : m_nintendoSwitchAccountSubjectId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.nintendoSwitchAccountSubjectId = m_nintendoSwitchAccountSubjectId.empty() ? nullptr : m_nintendoSwitchAccountSubjectId.data();
    }

    String m_nintendoSwitchAccountSubjectId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFUserNintendoSwitchDeviceIdInfoWrapper : public ModelWrapper<PFUserNintendoSwitchDeviceIdInfo, Alloc>
{
public:
    using ModelType = PFUserNintendoSwitchDeviceIdInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFUserNintendoSwitchDeviceIdInfoWrapper() = default;

    PFUserNintendoSwitchDeviceIdInfoWrapper(const PFUserNintendoSwitchDeviceIdInfo& model) :
        ModelWrapper<PFUserNintendoSwitchDeviceIdInfo, Alloc>{ model },
        m_nintendoSwitchDeviceId{ SafeString(model.nintendoSwitchDeviceId) }
    {
        SetModelPointers();
    }

    PFUserNintendoSwitchDeviceIdInfoWrapper(const PFUserNintendoSwitchDeviceIdInfoWrapper& src) :
        PFUserNintendoSwitchDeviceIdInfoWrapper{ src.Model() }
    {
    }

    PFUserNintendoSwitchDeviceIdInfoWrapper(PFUserNintendoSwitchDeviceIdInfoWrapper&& src) :
        PFUserNintendoSwitchDeviceIdInfoWrapper{}
    {
        swap(*this, src);
    }

    PFUserNintendoSwitchDeviceIdInfoWrapper& operator=(PFUserNintendoSwitchDeviceIdInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFUserNintendoSwitchDeviceIdInfoWrapper() = default;

    friend void swap(PFUserNintendoSwitchDeviceIdInfoWrapper& lhs, PFUserNintendoSwitchDeviceIdInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_nintendoSwitchDeviceId, rhs.m_nintendoSwitchDeviceId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetNintendoSwitchDeviceId(String value)
    {
        m_nintendoSwitchDeviceId = std::move(value);
        this->m_model.nintendoSwitchDeviceId =  m_nintendoSwitchDeviceId.empty() ? nullptr : m_nintendoSwitchDeviceId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.nintendoSwitchDeviceId = m_nintendoSwitchDeviceId.empty() ? nullptr : m_nintendoSwitchDeviceId.data();
    }

    String m_nintendoSwitchDeviceId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFUserOpenIdInfoWrapper : public ModelWrapper<PFUserOpenIdInfo, Alloc>
{
public:
    using ModelType = PFUserOpenIdInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFUserOpenIdInfoWrapper() = default;

    PFUserOpenIdInfoWrapper(const PFUserOpenIdInfo& model) :
        ModelWrapper<PFUserOpenIdInfo, Alloc>{ model },
        m_connectionId{ SafeString(model.connectionId) },
        m_issuer{ SafeString(model.issuer) },
        m_subject{ SafeString(model.subject) }
    {
        SetModelPointers();
    }

    PFUserOpenIdInfoWrapper(const PFUserOpenIdInfoWrapper& src) :
        PFUserOpenIdInfoWrapper{ src.Model() }
    {
    }

    PFUserOpenIdInfoWrapper(PFUserOpenIdInfoWrapper&& src) :
        PFUserOpenIdInfoWrapper{}
    {
        swap(*this, src);
    }

    PFUserOpenIdInfoWrapper& operator=(PFUserOpenIdInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFUserOpenIdInfoWrapper() = default;

    friend void swap(PFUserOpenIdInfoWrapper& lhs, PFUserOpenIdInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_connectionId, rhs.m_connectionId);
        swap(lhs.m_issuer, rhs.m_issuer);
        swap(lhs.m_subject, rhs.m_subject);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetConnectionId(String value)
    {
        m_connectionId = std::move(value);
        this->m_model.connectionId =  m_connectionId.empty() ? nullptr : m_connectionId.data();
    }

    void SetIssuer(String value)
    {
        m_issuer = std::move(value);
        this->m_model.issuer =  m_issuer.empty() ? nullptr : m_issuer.data();
    }

    void SetSubject(String value)
    {
        m_subject = std::move(value);
        this->m_model.subject =  m_subject.empty() ? nullptr : m_subject.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.connectionId = m_connectionId.empty() ? nullptr : m_connectionId.data();
        this->m_model.issuer = m_issuer.empty() ? nullptr : m_issuer.data();
        this->m_model.subject = m_subject.empty() ? nullptr : m_subject.data();
    }

    String m_connectionId;
    String m_issuer;
    String m_subject;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFUserPrivateAccountInfoWrapper : public ModelWrapper<PFUserPrivateAccountInfo, Alloc>
{
public:
    using ModelType = PFUserPrivateAccountInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFUserPrivateAccountInfoWrapper() = default;

    PFUserPrivateAccountInfoWrapper(const PFUserPrivateAccountInfo& model) :
        ModelWrapper<PFUserPrivateAccountInfo, Alloc>{ model },
        m_email{ SafeString(model.email) }
    {
        SetModelPointers();
    }

    PFUserPrivateAccountInfoWrapper(const PFUserPrivateAccountInfoWrapper& src) :
        PFUserPrivateAccountInfoWrapper{ src.Model() }
    {
    }

    PFUserPrivateAccountInfoWrapper(PFUserPrivateAccountInfoWrapper&& src) :
        PFUserPrivateAccountInfoWrapper{}
    {
        swap(*this, src);
    }

    PFUserPrivateAccountInfoWrapper& operator=(PFUserPrivateAccountInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFUserPrivateAccountInfoWrapper() = default;

    friend void swap(PFUserPrivateAccountInfoWrapper& lhs, PFUserPrivateAccountInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_email, rhs.m_email);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetEmail(String value)
    {
        m_email = std::move(value);
        this->m_model.email =  m_email.empty() ? nullptr : m_email.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.email = m_email.empty() ? nullptr : m_email.data();
    }

    String m_email;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFUserPsnInfoWrapper : public ModelWrapper<PFUserPsnInfo, Alloc>
{
public:
    using ModelType = PFUserPsnInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFUserPsnInfoWrapper() = default;

    PFUserPsnInfoWrapper(const PFUserPsnInfo& model) :
        ModelWrapper<PFUserPsnInfo, Alloc>{ model },
        m_psnAccountId{ SafeString(model.psnAccountId) },
        m_psnOnlineId{ SafeString(model.psnOnlineId) }
    {
        SetModelPointers();
    }

    PFUserPsnInfoWrapper(const PFUserPsnInfoWrapper& src) :
        PFUserPsnInfoWrapper{ src.Model() }
    {
    }

    PFUserPsnInfoWrapper(PFUserPsnInfoWrapper&& src) :
        PFUserPsnInfoWrapper{}
    {
        swap(*this, src);
    }

    PFUserPsnInfoWrapper& operator=(PFUserPsnInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFUserPsnInfoWrapper() = default;

    friend void swap(PFUserPsnInfoWrapper& lhs, PFUserPsnInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_psnAccountId, rhs.m_psnAccountId);
        swap(lhs.m_psnOnlineId, rhs.m_psnOnlineId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetPsnAccountId(String value)
    {
        m_psnAccountId = std::move(value);
        this->m_model.psnAccountId =  m_psnAccountId.empty() ? nullptr : m_psnAccountId.data();
    }

    void SetPsnOnlineId(String value)
    {
        m_psnOnlineId = std::move(value);
        this->m_model.psnOnlineId =  m_psnOnlineId.empty() ? nullptr : m_psnOnlineId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.psnAccountId = m_psnAccountId.empty() ? nullptr : m_psnAccountId.data();
        this->m_model.psnOnlineId = m_psnOnlineId.empty() ? nullptr : m_psnOnlineId.data();
    }

    String m_psnAccountId;
    String m_psnOnlineId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFUserServerCustomIdInfoWrapper : public ModelWrapper<PFUserServerCustomIdInfo, Alloc>
{
public:
    using ModelType = PFUserServerCustomIdInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFUserServerCustomIdInfoWrapper() = default;

    PFUserServerCustomIdInfoWrapper(const PFUserServerCustomIdInfo& model) :
        ModelWrapper<PFUserServerCustomIdInfo, Alloc>{ model },
        m_customId{ SafeString(model.customId) }
    {
        SetModelPointers();
    }

    PFUserServerCustomIdInfoWrapper(const PFUserServerCustomIdInfoWrapper& src) :
        PFUserServerCustomIdInfoWrapper{ src.Model() }
    {
    }

    PFUserServerCustomIdInfoWrapper(PFUserServerCustomIdInfoWrapper&& src) :
        PFUserServerCustomIdInfoWrapper{}
    {
        swap(*this, src);
    }

    PFUserServerCustomIdInfoWrapper& operator=(PFUserServerCustomIdInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFUserServerCustomIdInfoWrapper() = default;

    friend void swap(PFUserServerCustomIdInfoWrapper& lhs, PFUserServerCustomIdInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customId, rhs.m_customId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomId(String value)
    {
        m_customId = std::move(value);
        this->m_model.customId =  m_customId.empty() ? nullptr : m_customId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customId = m_customId.empty() ? nullptr : m_customId.data();
    }

    String m_customId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFUserSteamInfoWrapper : public ModelWrapper<PFUserSteamInfo, Alloc>
{
public:
    using ModelType = PFUserSteamInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFUserSteamInfoWrapper() = default;

    PFUserSteamInfoWrapper(const PFUserSteamInfo& model) :
        ModelWrapper<PFUserSteamInfo, Alloc>{ model },
        m_steamActivationStatus{ model.steamActivationStatus ? std::optional<PFTitleActivationStatus>{ *model.steamActivationStatus } : std::nullopt },
        m_steamCountry{ SafeString(model.steamCountry) },
        m_steamCurrency{ model.steamCurrency ? std::optional<PFCurrency>{ *model.steamCurrency } : std::nullopt },
        m_steamId{ SafeString(model.steamId) },
        m_steamName{ SafeString(model.steamName) }
    {
        SetModelPointers();
    }

    PFUserSteamInfoWrapper(const PFUserSteamInfoWrapper& src) :
        PFUserSteamInfoWrapper{ src.Model() }
    {
    }

    PFUserSteamInfoWrapper(PFUserSteamInfoWrapper&& src) :
        PFUserSteamInfoWrapper{}
    {
        swap(*this, src);
    }

    PFUserSteamInfoWrapper& operator=(PFUserSteamInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFUserSteamInfoWrapper() = default;

    friend void swap(PFUserSteamInfoWrapper& lhs, PFUserSteamInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_steamActivationStatus, rhs.m_steamActivationStatus);
        swap(lhs.m_steamCountry, rhs.m_steamCountry);
        swap(lhs.m_steamCurrency, rhs.m_steamCurrency);
        swap(lhs.m_steamId, rhs.m_steamId);
        swap(lhs.m_steamName, rhs.m_steamName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetSteamActivationStatus(std::optional<PFTitleActivationStatus> value)
    {
        m_steamActivationStatus = std::move(value);
        this->m_model.steamActivationStatus = m_steamActivationStatus ? m_steamActivationStatus.operator->() : nullptr;
    }

    void SetSteamCountry(String value)
    {
        m_steamCountry = std::move(value);
        this->m_model.steamCountry =  m_steamCountry.empty() ? nullptr : m_steamCountry.data();
    }

    void SetSteamCurrency(std::optional<PFCurrency> value)
    {
        m_steamCurrency = std::move(value);
        this->m_model.steamCurrency = m_steamCurrency ? m_steamCurrency.operator->() : nullptr;
    }

    void SetSteamId(String value)
    {
        m_steamId = std::move(value);
        this->m_model.steamId =  m_steamId.empty() ? nullptr : m_steamId.data();
    }

    void SetSteamName(String value)
    {
        m_steamName = std::move(value);
        this->m_model.steamName =  m_steamName.empty() ? nullptr : m_steamName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.steamActivationStatus = m_steamActivationStatus ? m_steamActivationStatus.operator->() : nullptr;
        this->m_model.steamCountry = m_steamCountry.empty() ? nullptr : m_steamCountry.data();
        this->m_model.steamCurrency = m_steamCurrency ? m_steamCurrency.operator->() : nullptr;
        this->m_model.steamId = m_steamId.empty() ? nullptr : m_steamId.data();
        this->m_model.steamName = m_steamName.empty() ? nullptr : m_steamName.data();
    }

    std::optional<PFTitleActivationStatus> m_steamActivationStatus;
    String m_steamCountry;
    std::optional<PFCurrency> m_steamCurrency;
    String m_steamId;
    String m_steamName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEntityKeyWrapper : public ModelWrapper<PFEntityKey, Alloc>
{
public:
    using ModelType = PFEntityKey;
    using DictionaryEntryType = PFEntityKeyDictionaryEntry;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEntityKeyWrapper() = default;

    PFEntityKeyWrapper(const PFEntityKey& model) :
        ModelWrapper<PFEntityKey, Alloc>{ model },
        m_id{ SafeString(model.id) },
        m_type{ SafeString(model.type) }
    {
        SetModelPointers();
    }

    PFEntityKeyWrapper(const PFEntityKeyWrapper& src) :
        PFEntityKeyWrapper{ src.Model() }
    {
    }

    PFEntityKeyWrapper(PFEntityKeyWrapper&& src) :
        PFEntityKeyWrapper{}
    {
        swap(*this, src);
    }

    PFEntityKeyWrapper& operator=(PFEntityKeyWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEntityKeyWrapper() = default;

    friend void swap(PFEntityKeyWrapper& lhs, PFEntityKeyWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_id, rhs.m_id);
        swap(lhs.m_type, rhs.m_type);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

    void SetType(String value)
    {
        m_type = std::move(value);
        this->m_model.type =  m_type.empty() ? nullptr : m_type.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
        this->m_model.type = m_type.empty() ? nullptr : m_type.data();
    }

    String m_id;
    String m_type;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFUserTitleInfoWrapper : public ModelWrapper<PFUserTitleInfo, Alloc>
{
public:
    using ModelType = PFUserTitleInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFUserTitleInfoWrapper() = default;

    PFUserTitleInfoWrapper(const PFUserTitleInfo& model) :
        ModelWrapper<PFUserTitleInfo, Alloc>{ model },
        m_avatarUrl{ SafeString(model.avatarUrl) },
        m_displayName{ SafeString(model.displayName) },
        m_firstLogin{ model.firstLogin ? std::optional<time_t>{ *model.firstLogin } : std::nullopt },
        m_isBanned{ model.isBanned ? std::optional<bool>{ *model.isBanned } : std::nullopt },
        m_lastLogin{ model.lastLogin ? std::optional<time_t>{ *model.lastLogin } : std::nullopt },
        m_origination{ model.origination ? std::optional<PFUserOrigination>{ *model.origination } : std::nullopt },
        m_titlePlayerAccount{ model.titlePlayerAccount ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.titlePlayerAccount } : std::nullopt }
    {
        SetModelPointers();
    }

    PFUserTitleInfoWrapper(const PFUserTitleInfoWrapper& src) :
        PFUserTitleInfoWrapper{ src.Model() }
    {
    }

    PFUserTitleInfoWrapper(PFUserTitleInfoWrapper&& src) :
        PFUserTitleInfoWrapper{}
    {
        swap(*this, src);
    }

    PFUserTitleInfoWrapper& operator=(PFUserTitleInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFUserTitleInfoWrapper() = default;

    friend void swap(PFUserTitleInfoWrapper& lhs, PFUserTitleInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_avatarUrl, rhs.m_avatarUrl);
        swap(lhs.m_displayName, rhs.m_displayName);
        swap(lhs.m_firstLogin, rhs.m_firstLogin);
        swap(lhs.m_isBanned, rhs.m_isBanned);
        swap(lhs.m_lastLogin, rhs.m_lastLogin);
        swap(lhs.m_origination, rhs.m_origination);
        swap(lhs.m_titlePlayerAccount, rhs.m_titlePlayerAccount);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAvatarUrl(String value)
    {
        m_avatarUrl = std::move(value);
        this->m_model.avatarUrl =  m_avatarUrl.empty() ? nullptr : m_avatarUrl.data();
    }

    void SetCreated(time_t value)
    {
        this->m_model.created = value;
    }

    void SetDisplayName(String value)
    {
        m_displayName = std::move(value);
        this->m_model.displayName =  m_displayName.empty() ? nullptr : m_displayName.data();
    }

    void SetFirstLogin(std::optional<time_t> value)
    {
        m_firstLogin = std::move(value);
        this->m_model.firstLogin = m_firstLogin ? m_firstLogin.operator->() : nullptr;
    }

    void SetisBanned(std::optional<bool> value)
    {
        m_isBanned = std::move(value);
        this->m_model.isBanned = m_isBanned ? m_isBanned.operator->() : nullptr;
    }

    void SetLastLogin(std::optional<time_t> value)
    {
        m_lastLogin = std::move(value);
        this->m_model.lastLogin = m_lastLogin ? m_lastLogin.operator->() : nullptr;
    }

    void SetOrigination(std::optional<PFUserOrigination> value)
    {
        m_origination = std::move(value);
        this->m_model.origination = m_origination ? m_origination.operator->() : nullptr;
    }

    void SetTitlePlayerAccount(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_titlePlayerAccount = std::move(value);
        this->m_model.titlePlayerAccount = m_titlePlayerAccount ? &m_titlePlayerAccount->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.avatarUrl = m_avatarUrl.empty() ? nullptr : m_avatarUrl.data();
        this->m_model.displayName = m_displayName.empty() ? nullptr : m_displayName.data();
        this->m_model.firstLogin = m_firstLogin ? m_firstLogin.operator->() : nullptr;
        this->m_model.isBanned = m_isBanned ? m_isBanned.operator->() : nullptr;
        this->m_model.lastLogin = m_lastLogin ? m_lastLogin.operator->() : nullptr;
        this->m_model.origination = m_origination ? m_origination.operator->() : nullptr;
        this->m_model.titlePlayerAccount = m_titlePlayerAccount ?  &m_titlePlayerAccount->Model() : nullptr;
    }

    String m_avatarUrl;
    String m_displayName;
    std::optional<time_t> m_firstLogin;
    std::optional<bool> m_isBanned;
    std::optional<time_t> m_lastLogin;
    std::optional<PFUserOrigination> m_origination;
    std::optional<PFEntityKeyWrapper<Alloc>> m_titlePlayerAccount;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFUserTwitchInfoWrapper : public ModelWrapper<PFUserTwitchInfo, Alloc>
{
public:
    using ModelType = PFUserTwitchInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFUserTwitchInfoWrapper() = default;

    PFUserTwitchInfoWrapper(const PFUserTwitchInfo& model) :
        ModelWrapper<PFUserTwitchInfo, Alloc>{ model },
        m_twitchId{ SafeString(model.twitchId) },
        m_twitchUserName{ SafeString(model.twitchUserName) }
    {
        SetModelPointers();
    }

    PFUserTwitchInfoWrapper(const PFUserTwitchInfoWrapper& src) :
        PFUserTwitchInfoWrapper{ src.Model() }
    {
    }

    PFUserTwitchInfoWrapper(PFUserTwitchInfoWrapper&& src) :
        PFUserTwitchInfoWrapper{}
    {
        swap(*this, src);
    }

    PFUserTwitchInfoWrapper& operator=(PFUserTwitchInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFUserTwitchInfoWrapper() = default;

    friend void swap(PFUserTwitchInfoWrapper& lhs, PFUserTwitchInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_twitchId, rhs.m_twitchId);
        swap(lhs.m_twitchUserName, rhs.m_twitchUserName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetTwitchId(String value)
    {
        m_twitchId = std::move(value);
        this->m_model.twitchId =  m_twitchId.empty() ? nullptr : m_twitchId.data();
    }

    void SetTwitchUserName(String value)
    {
        m_twitchUserName = std::move(value);
        this->m_model.twitchUserName =  m_twitchUserName.empty() ? nullptr : m_twitchUserName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.twitchId = m_twitchId.empty() ? nullptr : m_twitchId.data();
        this->m_model.twitchUserName = m_twitchUserName.empty() ? nullptr : m_twitchUserName.data();
    }

    String m_twitchId;
    String m_twitchUserName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFUserXboxInfoWrapper : public ModelWrapper<PFUserXboxInfo, Alloc>
{
public:
    using ModelType = PFUserXboxInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFUserXboxInfoWrapper() = default;

    PFUserXboxInfoWrapper(const PFUserXboxInfo& model) :
        ModelWrapper<PFUserXboxInfo, Alloc>{ model },
        m_xboxUserId{ SafeString(model.xboxUserId) },
        m_xboxUserSandbox{ SafeString(model.xboxUserSandbox) }
    {
        SetModelPointers();
    }

    PFUserXboxInfoWrapper(const PFUserXboxInfoWrapper& src) :
        PFUserXboxInfoWrapper{ src.Model() }
    {
    }

    PFUserXboxInfoWrapper(PFUserXboxInfoWrapper&& src) :
        PFUserXboxInfoWrapper{}
    {
        swap(*this, src);
    }

    PFUserXboxInfoWrapper& operator=(PFUserXboxInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFUserXboxInfoWrapper() = default;

    friend void swap(PFUserXboxInfoWrapper& lhs, PFUserXboxInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_xboxUserId, rhs.m_xboxUserId);
        swap(lhs.m_xboxUserSandbox, rhs.m_xboxUserSandbox);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetXboxUserId(String value)
    {
        m_xboxUserId = std::move(value);
        this->m_model.xboxUserId =  m_xboxUserId.empty() ? nullptr : m_xboxUserId.data();
    }

    void SetXboxUserSandbox(String value)
    {
        m_xboxUserSandbox = std::move(value);
        this->m_model.xboxUserSandbox =  m_xboxUserSandbox.empty() ? nullptr : m_xboxUserSandbox.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.xboxUserId = m_xboxUserId.empty() ? nullptr : m_xboxUserId.data();
        this->m_model.xboxUserSandbox = m_xboxUserSandbox.empty() ? nullptr : m_xboxUserSandbox.data();
    }

    String m_xboxUserId;
    String m_xboxUserSandbox;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFUserAccountInfoWrapper : public ModelWrapper<PFUserAccountInfo, Alloc>
{
public:
    using ModelType = PFUserAccountInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFUserAccountInfoWrapper() = default;

    PFUserAccountInfoWrapper(const PFUserAccountInfo& model) :
        ModelWrapper<PFUserAccountInfo, Alloc>{ model },
        m_androidDeviceInfo{ model.androidDeviceInfo ? std::optional<PFUserAndroidDeviceInfoWrapper<Alloc>>{ *model.androidDeviceInfo } : std::nullopt },
        m_appleAccountInfo{ model.appleAccountInfo ? std::optional<PFUserAppleIdInfoWrapper<Alloc>>{ *model.appleAccountInfo } : std::nullopt },
        m_customIdInfo{ model.customIdInfo ? std::optional<PFUserCustomIdInfoWrapper<Alloc>>{ *model.customIdInfo } : std::nullopt },
        m_facebookInfo{ model.facebookInfo ? std::optional<PFUserFacebookInfoWrapper<Alloc>>{ *model.facebookInfo } : std::nullopt },
        m_facebookInstantGamesIdInfo{ model.facebookInstantGamesIdInfo ? std::optional<PFUserFacebookInstantGamesIdInfoWrapper<Alloc>>{ *model.facebookInstantGamesIdInfo } : std::nullopt },
        m_gameCenterInfo{ model.gameCenterInfo ? std::optional<PFUserGameCenterInfoWrapper<Alloc>>{ *model.gameCenterInfo } : std::nullopt },
        m_googleInfo{ model.googleInfo ? std::optional<PFUserGoogleInfoWrapper<Alloc>>{ *model.googleInfo } : std::nullopt },
        m_googlePlayGamesInfo{ model.googlePlayGamesInfo ? std::optional<PFUserGooglePlayGamesInfoWrapper<Alloc>>{ *model.googlePlayGamesInfo } : std::nullopt },
        m_iosDeviceInfo{ model.iosDeviceInfo ? std::optional<PFUserIosDeviceInfoWrapper<Alloc>>{ *model.iosDeviceInfo } : std::nullopt },
        m_kongregateInfo{ model.kongregateInfo ? std::optional<PFUserKongregateInfoWrapper<Alloc>>{ *model.kongregateInfo } : std::nullopt },
        m_nintendoSwitchAccountInfo{ model.nintendoSwitchAccountInfo ? std::optional<PFUserNintendoSwitchAccountIdInfoWrapper<Alloc>>{ *model.nintendoSwitchAccountInfo } : std::nullopt },
        m_nintendoSwitchDeviceIdInfo{ model.nintendoSwitchDeviceIdInfo ? std::optional<PFUserNintendoSwitchDeviceIdInfoWrapper<Alloc>>{ *model.nintendoSwitchDeviceIdInfo } : std::nullopt },
        m_openIdInfo{ model.openIdInfo, model.openIdInfo + model.openIdInfoCount },
        m_playFabId{ SafeString(model.playFabId) },
        m_privateInfo{ model.privateInfo ? std::optional<PFUserPrivateAccountInfoWrapper<Alloc>>{ *model.privateInfo } : std::nullopt },
        m_psnInfo{ model.psnInfo ? std::optional<PFUserPsnInfoWrapper<Alloc>>{ *model.psnInfo } : std::nullopt },
        m_serverCustomIdInfo{ model.serverCustomIdInfo ? std::optional<PFUserServerCustomIdInfoWrapper<Alloc>>{ *model.serverCustomIdInfo } : std::nullopt },
        m_steamInfo{ model.steamInfo ? std::optional<PFUserSteamInfoWrapper<Alloc>>{ *model.steamInfo } : std::nullopt },
        m_titleInfo{ model.titleInfo ? std::optional<PFUserTitleInfoWrapper<Alloc>>{ *model.titleInfo } : std::nullopt },
        m_twitchInfo{ model.twitchInfo ? std::optional<PFUserTwitchInfoWrapper<Alloc>>{ *model.twitchInfo } : std::nullopt },
        m_username{ SafeString(model.username) },
        m_xboxInfo{ model.xboxInfo ? std::optional<PFUserXboxInfoWrapper<Alloc>>{ *model.xboxInfo } : std::nullopt }
    {
        SetModelPointers();
    }

    PFUserAccountInfoWrapper(const PFUserAccountInfoWrapper& src) :
        PFUserAccountInfoWrapper{ src.Model() }
    {
    }

    PFUserAccountInfoWrapper(PFUserAccountInfoWrapper&& src) :
        PFUserAccountInfoWrapper{}
    {
        swap(*this, src);
    }

    PFUserAccountInfoWrapper& operator=(PFUserAccountInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFUserAccountInfoWrapper() = default;

    friend void swap(PFUserAccountInfoWrapper& lhs, PFUserAccountInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_androidDeviceInfo, rhs.m_androidDeviceInfo);
        swap(lhs.m_appleAccountInfo, rhs.m_appleAccountInfo);
        swap(lhs.m_customIdInfo, rhs.m_customIdInfo);
        swap(lhs.m_facebookInfo, rhs.m_facebookInfo);
        swap(lhs.m_facebookInstantGamesIdInfo, rhs.m_facebookInstantGamesIdInfo);
        swap(lhs.m_gameCenterInfo, rhs.m_gameCenterInfo);
        swap(lhs.m_googleInfo, rhs.m_googleInfo);
        swap(lhs.m_googlePlayGamesInfo, rhs.m_googlePlayGamesInfo);
        swap(lhs.m_iosDeviceInfo, rhs.m_iosDeviceInfo);
        swap(lhs.m_kongregateInfo, rhs.m_kongregateInfo);
        swap(lhs.m_nintendoSwitchAccountInfo, rhs.m_nintendoSwitchAccountInfo);
        swap(lhs.m_nintendoSwitchDeviceIdInfo, rhs.m_nintendoSwitchDeviceIdInfo);
        swap(lhs.m_openIdInfo, rhs.m_openIdInfo);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_privateInfo, rhs.m_privateInfo);
        swap(lhs.m_psnInfo, rhs.m_psnInfo);
        swap(lhs.m_serverCustomIdInfo, rhs.m_serverCustomIdInfo);
        swap(lhs.m_steamInfo, rhs.m_steamInfo);
        swap(lhs.m_titleInfo, rhs.m_titleInfo);
        swap(lhs.m_twitchInfo, rhs.m_twitchInfo);
        swap(lhs.m_username, rhs.m_username);
        swap(lhs.m_xboxInfo, rhs.m_xboxInfo);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAndroidDeviceInfo(std::optional<PFUserAndroidDeviceInfoWrapper<Alloc>> value)
    {
        m_androidDeviceInfo = std::move(value);
        this->m_model.androidDeviceInfo = m_androidDeviceInfo ? &m_androidDeviceInfo->Model() : nullptr;
    }

    void SetAppleAccountInfo(std::optional<PFUserAppleIdInfoWrapper<Alloc>> value)
    {
        m_appleAccountInfo = std::move(value);
        this->m_model.appleAccountInfo = m_appleAccountInfo ? &m_appleAccountInfo->Model() : nullptr;
    }

    void SetCreated(time_t value)
    {
        this->m_model.created = value;
    }

    void SetCustomIdInfo(std::optional<PFUserCustomIdInfoWrapper<Alloc>> value)
    {
        m_customIdInfo = std::move(value);
        this->m_model.customIdInfo = m_customIdInfo ? &m_customIdInfo->Model() : nullptr;
    }

    void SetFacebookInfo(std::optional<PFUserFacebookInfoWrapper<Alloc>> value)
    {
        m_facebookInfo = std::move(value);
        this->m_model.facebookInfo = m_facebookInfo ? &m_facebookInfo->Model() : nullptr;
    }

    void SetFacebookInstantGamesIdInfo(std::optional<PFUserFacebookInstantGamesIdInfoWrapper<Alloc>> value)
    {
        m_facebookInstantGamesIdInfo = std::move(value);
        this->m_model.facebookInstantGamesIdInfo = m_facebookInstantGamesIdInfo ? &m_facebookInstantGamesIdInfo->Model() : nullptr;
    }

    void SetGameCenterInfo(std::optional<PFUserGameCenterInfoWrapper<Alloc>> value)
    {
        m_gameCenterInfo = std::move(value);
        this->m_model.gameCenterInfo = m_gameCenterInfo ? &m_gameCenterInfo->Model() : nullptr;
    }

    void SetGoogleInfo(std::optional<PFUserGoogleInfoWrapper<Alloc>> value)
    {
        m_googleInfo = std::move(value);
        this->m_model.googleInfo = m_googleInfo ? &m_googleInfo->Model() : nullptr;
    }

    void SetGooglePlayGamesInfo(std::optional<PFUserGooglePlayGamesInfoWrapper<Alloc>> value)
    {
        m_googlePlayGamesInfo = std::move(value);
        this->m_model.googlePlayGamesInfo = m_googlePlayGamesInfo ? &m_googlePlayGamesInfo->Model() : nullptr;
    }

    void SetIosDeviceInfo(std::optional<PFUserIosDeviceInfoWrapper<Alloc>> value)
    {
        m_iosDeviceInfo = std::move(value);
        this->m_model.iosDeviceInfo = m_iosDeviceInfo ? &m_iosDeviceInfo->Model() : nullptr;
    }

    void SetKongregateInfo(std::optional<PFUserKongregateInfoWrapper<Alloc>> value)
    {
        m_kongregateInfo = std::move(value);
        this->m_model.kongregateInfo = m_kongregateInfo ? &m_kongregateInfo->Model() : nullptr;
    }

    void SetNintendoSwitchAccountInfo(std::optional<PFUserNintendoSwitchAccountIdInfoWrapper<Alloc>> value)
    {
        m_nintendoSwitchAccountInfo = std::move(value);
        this->m_model.nintendoSwitchAccountInfo = m_nintendoSwitchAccountInfo ? &m_nintendoSwitchAccountInfo->Model() : nullptr;
    }

    void SetNintendoSwitchDeviceIdInfo(std::optional<PFUserNintendoSwitchDeviceIdInfoWrapper<Alloc>> value)
    {
        m_nintendoSwitchDeviceIdInfo = std::move(value);
        this->m_model.nintendoSwitchDeviceIdInfo = m_nintendoSwitchDeviceIdInfo ? &m_nintendoSwitchDeviceIdInfo->Model() : nullptr;
    }

    void SetOpenIdInfo(ModelVector<PFUserOpenIdInfoWrapper<Alloc>, Alloc> value)
    {
        m_openIdInfo = std::move(value);
        this->m_model.openIdInfo =  m_openIdInfo.empty() ? nullptr : m_openIdInfo.data();
        this->m_model.openIdInfoCount =  static_cast<uint32_t>(m_openIdInfo.size());
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetPrivateInfo(std::optional<PFUserPrivateAccountInfoWrapper<Alloc>> value)
    {
        m_privateInfo = std::move(value);
        this->m_model.privateInfo = m_privateInfo ? &m_privateInfo->Model() : nullptr;
    }

    void SetPsnInfo(std::optional<PFUserPsnInfoWrapper<Alloc>> value)
    {
        m_psnInfo = std::move(value);
        this->m_model.psnInfo = m_psnInfo ? &m_psnInfo->Model() : nullptr;
    }

    void SetServerCustomIdInfo(std::optional<PFUserServerCustomIdInfoWrapper<Alloc>> value)
    {
        m_serverCustomIdInfo = std::move(value);
        this->m_model.serverCustomIdInfo = m_serverCustomIdInfo ? &m_serverCustomIdInfo->Model() : nullptr;
    }

    void SetSteamInfo(std::optional<PFUserSteamInfoWrapper<Alloc>> value)
    {
        m_steamInfo = std::move(value);
        this->m_model.steamInfo = m_steamInfo ? &m_steamInfo->Model() : nullptr;
    }

    void SetTitleInfo(std::optional<PFUserTitleInfoWrapper<Alloc>> value)
    {
        m_titleInfo = std::move(value);
        this->m_model.titleInfo = m_titleInfo ? &m_titleInfo->Model() : nullptr;
    }

    void SetTwitchInfo(std::optional<PFUserTwitchInfoWrapper<Alloc>> value)
    {
        m_twitchInfo = std::move(value);
        this->m_model.twitchInfo = m_twitchInfo ? &m_twitchInfo->Model() : nullptr;
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
        this->m_model.androidDeviceInfo = m_androidDeviceInfo ?  &m_androidDeviceInfo->Model() : nullptr;
        this->m_model.appleAccountInfo = m_appleAccountInfo ?  &m_appleAccountInfo->Model() : nullptr;
        this->m_model.customIdInfo = m_customIdInfo ?  &m_customIdInfo->Model() : nullptr;
        this->m_model.facebookInfo = m_facebookInfo ?  &m_facebookInfo->Model() : nullptr;
        this->m_model.facebookInstantGamesIdInfo = m_facebookInstantGamesIdInfo ?  &m_facebookInstantGamesIdInfo->Model() : nullptr;
        this->m_model.gameCenterInfo = m_gameCenterInfo ?  &m_gameCenterInfo->Model() : nullptr;
        this->m_model.googleInfo = m_googleInfo ?  &m_googleInfo->Model() : nullptr;
        this->m_model.googlePlayGamesInfo = m_googlePlayGamesInfo ?  &m_googlePlayGamesInfo->Model() : nullptr;
        this->m_model.iosDeviceInfo = m_iosDeviceInfo ?  &m_iosDeviceInfo->Model() : nullptr;
        this->m_model.kongregateInfo = m_kongregateInfo ?  &m_kongregateInfo->Model() : nullptr;
        this->m_model.nintendoSwitchAccountInfo = m_nintendoSwitchAccountInfo ?  &m_nintendoSwitchAccountInfo->Model() : nullptr;
        this->m_model.nintendoSwitchDeviceIdInfo = m_nintendoSwitchDeviceIdInfo ?  &m_nintendoSwitchDeviceIdInfo->Model() : nullptr;
        this->m_model.openIdInfo = m_openIdInfo.empty() ? nullptr : m_openIdInfo.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.privateInfo = m_privateInfo ?  &m_privateInfo->Model() : nullptr;
        this->m_model.psnInfo = m_psnInfo ?  &m_psnInfo->Model() : nullptr;
        this->m_model.serverCustomIdInfo = m_serverCustomIdInfo ?  &m_serverCustomIdInfo->Model() : nullptr;
        this->m_model.steamInfo = m_steamInfo ?  &m_steamInfo->Model() : nullptr;
        this->m_model.titleInfo = m_titleInfo ?  &m_titleInfo->Model() : nullptr;
        this->m_model.twitchInfo = m_twitchInfo ?  &m_twitchInfo->Model() : nullptr;
        this->m_model.username = m_username.empty() ? nullptr : m_username.data();
        this->m_model.xboxInfo = m_xboxInfo ?  &m_xboxInfo->Model() : nullptr;
    }

    std::optional<PFUserAndroidDeviceInfoWrapper<Alloc>> m_androidDeviceInfo;
    std::optional<PFUserAppleIdInfoWrapper<Alloc>> m_appleAccountInfo;
    std::optional<PFUserCustomIdInfoWrapper<Alloc>> m_customIdInfo;
    std::optional<PFUserFacebookInfoWrapper<Alloc>> m_facebookInfo;
    std::optional<PFUserFacebookInstantGamesIdInfoWrapper<Alloc>> m_facebookInstantGamesIdInfo;
    std::optional<PFUserGameCenterInfoWrapper<Alloc>> m_gameCenterInfo;
    std::optional<PFUserGoogleInfoWrapper<Alloc>> m_googleInfo;
    std::optional<PFUserGooglePlayGamesInfoWrapper<Alloc>> m_googlePlayGamesInfo;
    std::optional<PFUserIosDeviceInfoWrapper<Alloc>> m_iosDeviceInfo;
    std::optional<PFUserKongregateInfoWrapper<Alloc>> m_kongregateInfo;
    std::optional<PFUserNintendoSwitchAccountIdInfoWrapper<Alloc>> m_nintendoSwitchAccountInfo;
    std::optional<PFUserNintendoSwitchDeviceIdInfoWrapper<Alloc>> m_nintendoSwitchDeviceIdInfo;
    ModelVector<PFUserOpenIdInfoWrapper<Alloc>, Alloc> m_openIdInfo;
    String m_playFabId;
    std::optional<PFUserPrivateAccountInfoWrapper<Alloc>> m_privateInfo;
    std::optional<PFUserPsnInfoWrapper<Alloc>> m_psnInfo;
    std::optional<PFUserServerCustomIdInfoWrapper<Alloc>> m_serverCustomIdInfo;
    std::optional<PFUserSteamInfoWrapper<Alloc>> m_steamInfo;
    std::optional<PFUserTitleInfoWrapper<Alloc>> m_titleInfo;
    std::optional<PFUserTwitchInfoWrapper<Alloc>> m_twitchInfo;
    String m_username;
    std::optional<PFUserXboxInfoWrapper<Alloc>> m_xboxInfo;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCharacterResultWrapper : public ModelWrapper<PFCharacterResult, Alloc>
{
public:
    using ModelType = PFCharacterResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCharacterResultWrapper() = default;

    PFCharacterResultWrapper(const PFCharacterResult& model) :
        ModelWrapper<PFCharacterResult, Alloc>{ model },
        m_characterId{ SafeString(model.characterId) },
        m_characterName{ SafeString(model.characterName) },
        m_characterType{ SafeString(model.characterType) }
    {
        SetModelPointers();
    }

    PFCharacterResultWrapper(const PFCharacterResultWrapper& src) :
        PFCharacterResultWrapper{ src.Model() }
    {
    }

    PFCharacterResultWrapper(PFCharacterResultWrapper&& src) :
        PFCharacterResultWrapper{}
    {
        swap(*this, src);
    }

    PFCharacterResultWrapper& operator=(PFCharacterResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCharacterResultWrapper() = default;

    friend void swap(PFCharacterResultWrapper& lhs, PFCharacterResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_characterId, rhs.m_characterId);
        swap(lhs.m_characterName, rhs.m_characterName);
        swap(lhs.m_characterType, rhs.m_characterType);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCharacterId(String value)
    {
        m_characterId = std::move(value);
        this->m_model.characterId =  m_characterId.empty() ? nullptr : m_characterId.data();
    }

    void SetCharacterName(String value)
    {
        m_characterName = std::move(value);
        this->m_model.characterName =  m_characterName.empty() ? nullptr : m_characterName.data();
    }

    void SetCharacterType(String value)
    {
        m_characterType = std::move(value);
        this->m_model.characterType =  m_characterType.empty() ? nullptr : m_characterType.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.characterId = m_characterId.empty() ? nullptr : m_characterId.data();
        this->m_model.characterName = m_characterName.empty() ? nullptr : m_characterName.data();
        this->m_model.characterType = m_characterType.empty() ? nullptr : m_characterType.data();
    }

    String m_characterId;
    String m_characterName;
    String m_characterType;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFUserDataRecordWrapper : public ModelWrapper<PFUserDataRecord, Alloc>
{
public:
    using ModelType = PFUserDataRecord;
    using DictionaryEntryType = PFUserDataRecordDictionaryEntry;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFUserDataRecordWrapper() = default;

    PFUserDataRecordWrapper(const PFUserDataRecord& model) :
        ModelWrapper<PFUserDataRecord, Alloc>{ model },
        m_permission{ model.permission ? std::optional<PFUserDataPermission>{ *model.permission } : std::nullopt },
        m_value{ SafeString(model.value) }
    {
        SetModelPointers();
    }

    PFUserDataRecordWrapper(const PFUserDataRecordWrapper& src) :
        PFUserDataRecordWrapper{ src.Model() }
    {
    }

    PFUserDataRecordWrapper(PFUserDataRecordWrapper&& src) :
        PFUserDataRecordWrapper{}
    {
        swap(*this, src);
    }

    PFUserDataRecordWrapper& operator=(PFUserDataRecordWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFUserDataRecordWrapper() = default;

    friend void swap(PFUserDataRecordWrapper& lhs, PFUserDataRecordWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_permission, rhs.m_permission);
        swap(lhs.m_value, rhs.m_value);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetLastUpdated(time_t value)
    {
        this->m_model.lastUpdated = value;
    }

    void SetPermission(std::optional<PFUserDataPermission> value)
    {
        m_permission = std::move(value);
        this->m_model.permission = m_permission ? m_permission.operator->() : nullptr;
    }

    void SetValue(String value)
    {
        m_value = std::move(value);
        this->m_model.value =  m_value.empty() ? nullptr : m_value.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.permission = m_permission ? m_permission.operator->() : nullptr;
        this->m_model.value = m_value.empty() ? nullptr : m_value.data();
    }

    std::optional<PFUserDataPermission> m_permission;
    String m_value;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFVirtualCurrencyRechargeTimeWrapper : public ModelWrapper<PFVirtualCurrencyRechargeTime, Alloc>
{
public:
    using ModelType = PFVirtualCurrencyRechargeTime;
    using DictionaryEntryType = PFVirtualCurrencyRechargeTimeDictionaryEntry;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    using ModelWrapper<PFVirtualCurrencyRechargeTime, Alloc>::ModelWrapper;

    void SetRechargeMax(int32_t value)
    {
        this->m_model.rechargeMax = value;
    }

    void SetRechargeTime(time_t value)
    {
        this->m_model.rechargeTime = value;
    }

    void SetSecondsToRecharge(int32_t value)
    {
        this->m_model.secondsToRecharge = value;
    }

private:
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayerProfileViewConstraintsWrapper : public ModelWrapper<PFPlayerProfileViewConstraints, Alloc>
{
public:
    using ModelType = PFPlayerProfileViewConstraints;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    using ModelWrapper<PFPlayerProfileViewConstraints, Alloc>::ModelWrapper;

    void SetShowAvatarUrl(bool value)
    {
        this->m_model.showAvatarUrl = value;
    }

    void SetShowBannedUntil(bool value)
    {
        this->m_model.showBannedUntil = value;
    }

    void SetShowCampaignAttributions(bool value)
    {
        this->m_model.showCampaignAttributions = value;
    }

    void SetShowContactEmailAddresses(bool value)
    {
        this->m_model.showContactEmailAddresses = value;
    }

    void SetShowCreated(bool value)
    {
        this->m_model.showCreated = value;
    }

    void SetShowDisplayName(bool value)
    {
        this->m_model.showDisplayName = value;
    }

    void SetShowExperimentVariants(bool value)
    {
        this->m_model.showExperimentVariants = value;
    }

    void SetShowLastLogin(bool value)
    {
        this->m_model.showLastLogin = value;
    }

    void SetShowLinkedAccounts(bool value)
    {
        this->m_model.showLinkedAccounts = value;
    }

    void SetShowLocations(bool value)
    {
        this->m_model.showLocations = value;
    }

    void SetShowMemberships(bool value)
    {
        this->m_model.showMemberships = value;
    }

    void SetShowOrigination(bool value)
    {
        this->m_model.showOrigination = value;
    }

    void SetShowPushNotificationRegistrations(bool value)
    {
        this->m_model.showPushNotificationRegistrations = value;
    }

    void SetShowStatistics(bool value)
    {
        this->m_model.showStatistics = value;
    }

    void SetShowTags(bool value)
    {
        this->m_model.showTags = value;
    }

    void SetShowTotalValueToDateInUsd(bool value)
    {
        this->m_model.showTotalValueToDateInUsd = value;
    }

    void SetShowValuesToDate(bool value)
    {
        this->m_model.showValuesToDate = value;
    }

private:
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFAdCampaignAttributionModelWrapper : public ModelWrapper<PFAdCampaignAttributionModel, Alloc>
{
public:
    using ModelType = PFAdCampaignAttributionModel;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFAdCampaignAttributionModelWrapper() = default;

    PFAdCampaignAttributionModelWrapper(const PFAdCampaignAttributionModel& model) :
        ModelWrapper<PFAdCampaignAttributionModel, Alloc>{ model },
        m_campaignId{ SafeString(model.campaignId) },
        m_platform{ SafeString(model.platform) }
    {
        SetModelPointers();
    }

    PFAdCampaignAttributionModelWrapper(const PFAdCampaignAttributionModelWrapper& src) :
        PFAdCampaignAttributionModelWrapper{ src.Model() }
    {
    }

    PFAdCampaignAttributionModelWrapper(PFAdCampaignAttributionModelWrapper&& src) :
        PFAdCampaignAttributionModelWrapper{}
    {
        swap(*this, src);
    }

    PFAdCampaignAttributionModelWrapper& operator=(PFAdCampaignAttributionModelWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFAdCampaignAttributionModelWrapper() = default;

    friend void swap(PFAdCampaignAttributionModelWrapper& lhs, PFAdCampaignAttributionModelWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_campaignId, rhs.m_campaignId);
        swap(lhs.m_platform, rhs.m_platform);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAttributedAt(time_t value)
    {
        this->m_model.attributedAt = value;
    }

    void SetCampaignId(String value)
    {
        m_campaignId = std::move(value);
        this->m_model.campaignId =  m_campaignId.empty() ? nullptr : m_campaignId.data();
    }

    void SetPlatform(String value)
    {
        m_platform = std::move(value);
        this->m_model.platform =  m_platform.empty() ? nullptr : m_platform.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.campaignId = m_campaignId.empty() ? nullptr : m_campaignId.data();
        this->m_model.platform = m_platform.empty() ? nullptr : m_platform.data();
    }

    String m_campaignId;
    String m_platform;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFContactEmailInfoModelWrapper : public ModelWrapper<PFContactEmailInfoModel, Alloc>
{
public:
    using ModelType = PFContactEmailInfoModel;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFContactEmailInfoModelWrapper() = default;

    PFContactEmailInfoModelWrapper(const PFContactEmailInfoModel& model) :
        ModelWrapper<PFContactEmailInfoModel, Alloc>{ model },
        m_emailAddress{ SafeString(model.emailAddress) },
        m_name{ SafeString(model.name) },
        m_verificationStatus{ model.verificationStatus ? std::optional<PFEmailVerificationStatus>{ *model.verificationStatus } : std::nullopt }
    {
        SetModelPointers();
    }

    PFContactEmailInfoModelWrapper(const PFContactEmailInfoModelWrapper& src) :
        PFContactEmailInfoModelWrapper{ src.Model() }
    {
    }

    PFContactEmailInfoModelWrapper(PFContactEmailInfoModelWrapper&& src) :
        PFContactEmailInfoModelWrapper{}
    {
        swap(*this, src);
    }

    PFContactEmailInfoModelWrapper& operator=(PFContactEmailInfoModelWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFContactEmailInfoModelWrapper() = default;

    friend void swap(PFContactEmailInfoModelWrapper& lhs, PFContactEmailInfoModelWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_emailAddress, rhs.m_emailAddress);
        swap(lhs.m_name, rhs.m_name);
        swap(lhs.m_verificationStatus, rhs.m_verificationStatus);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetEmailAddress(String value)
    {
        m_emailAddress = std::move(value);
        this->m_model.emailAddress =  m_emailAddress.empty() ? nullptr : m_emailAddress.data();
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

    void SetVerificationStatus(std::optional<PFEmailVerificationStatus> value)
    {
        m_verificationStatus = std::move(value);
        this->m_model.verificationStatus = m_verificationStatus ? m_verificationStatus.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.emailAddress = m_emailAddress.empty() ? nullptr : m_emailAddress.data();
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
        this->m_model.verificationStatus = m_verificationStatus ? m_verificationStatus.operator->() : nullptr;
    }

    String m_emailAddress;
    String m_name;
    std::optional<PFEmailVerificationStatus> m_verificationStatus;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFLinkedPlatformAccountModelWrapper : public ModelWrapper<PFLinkedPlatformAccountModel, Alloc>
{
public:
    using ModelType = PFLinkedPlatformAccountModel;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLinkedPlatformAccountModelWrapper() = default;

    PFLinkedPlatformAccountModelWrapper(const PFLinkedPlatformAccountModel& model) :
        ModelWrapper<PFLinkedPlatformAccountModel, Alloc>{ model },
        m_email{ SafeString(model.email) },
        m_platform{ model.platform ? std::optional<PFLoginIdentityProvider>{ *model.platform } : std::nullopt },
        m_platformUserId{ SafeString(model.platformUserId) },
        m_username{ SafeString(model.username) }
    {
        SetModelPointers();
    }

    PFLinkedPlatformAccountModelWrapper(const PFLinkedPlatformAccountModelWrapper& src) :
        PFLinkedPlatformAccountModelWrapper{ src.Model() }
    {
    }

    PFLinkedPlatformAccountModelWrapper(PFLinkedPlatformAccountModelWrapper&& src) :
        PFLinkedPlatformAccountModelWrapper{}
    {
        swap(*this, src);
    }

    PFLinkedPlatformAccountModelWrapper& operator=(PFLinkedPlatformAccountModelWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLinkedPlatformAccountModelWrapper() = default;

    friend void swap(PFLinkedPlatformAccountModelWrapper& lhs, PFLinkedPlatformAccountModelWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_email, rhs.m_email);
        swap(lhs.m_platform, rhs.m_platform);
        swap(lhs.m_platformUserId, rhs.m_platformUserId);
        swap(lhs.m_username, rhs.m_username);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetEmail(String value)
    {
        m_email = std::move(value);
        this->m_model.email =  m_email.empty() ? nullptr : m_email.data();
    }

    void SetPlatform(std::optional<PFLoginIdentityProvider> value)
    {
        m_platform = std::move(value);
        this->m_model.platform = m_platform ? m_platform.operator->() : nullptr;
    }

    void SetPlatformUserId(String value)
    {
        m_platformUserId = std::move(value);
        this->m_model.platformUserId =  m_platformUserId.empty() ? nullptr : m_platformUserId.data();
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
        this->m_model.platform = m_platform ? m_platform.operator->() : nullptr;
        this->m_model.platformUserId = m_platformUserId.empty() ? nullptr : m_platformUserId.data();
        this->m_model.username = m_username.empty() ? nullptr : m_username.data();
    }

    String m_email;
    std::optional<PFLoginIdentityProvider> m_platform;
    String m_platformUserId;
    String m_username;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFLocationModelWrapper : public ModelWrapper<PFLocationModel, Alloc>
{
public:
    using ModelType = PFLocationModel;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLocationModelWrapper() = default;

    PFLocationModelWrapper(const PFLocationModel& model) :
        ModelWrapper<PFLocationModel, Alloc>{ model },
        m_city{ SafeString(model.city) },
        m_continentCode{ model.continentCode ? std::optional<PFContinentCode>{ *model.continentCode } : std::nullopt },
        m_countryCode{ model.countryCode ? std::optional<PFCountryCode>{ *model.countryCode } : std::nullopt },
        m_latitude{ model.latitude ? std::optional<double>{ *model.latitude } : std::nullopt },
        m_longitude{ model.longitude ? std::optional<double>{ *model.longitude } : std::nullopt }
    {
        SetModelPointers();
    }

    PFLocationModelWrapper(const PFLocationModelWrapper& src) :
        PFLocationModelWrapper{ src.Model() }
    {
    }

    PFLocationModelWrapper(PFLocationModelWrapper&& src) :
        PFLocationModelWrapper{}
    {
        swap(*this, src);
    }

    PFLocationModelWrapper& operator=(PFLocationModelWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLocationModelWrapper() = default;

    friend void swap(PFLocationModelWrapper& lhs, PFLocationModelWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_city, rhs.m_city);
        swap(lhs.m_continentCode, rhs.m_continentCode);
        swap(lhs.m_countryCode, rhs.m_countryCode);
        swap(lhs.m_latitude, rhs.m_latitude);
        swap(lhs.m_longitude, rhs.m_longitude);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCity(String value)
    {
        m_city = std::move(value);
        this->m_model.city =  m_city.empty() ? nullptr : m_city.data();
    }

    void SetContinentCode(std::optional<PFContinentCode> value)
    {
        m_continentCode = std::move(value);
        this->m_model.continentCode = m_continentCode ? m_continentCode.operator->() : nullptr;
    }

    void SetCountryCode(std::optional<PFCountryCode> value)
    {
        m_countryCode = std::move(value);
        this->m_model.countryCode = m_countryCode ? m_countryCode.operator->() : nullptr;
    }

    void SetLatitude(std::optional<double> value)
    {
        m_latitude = std::move(value);
        this->m_model.latitude = m_latitude ? m_latitude.operator->() : nullptr;
    }

    void SetLongitude(std::optional<double> value)
    {
        m_longitude = std::move(value);
        this->m_model.longitude = m_longitude ? m_longitude.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.city = m_city.empty() ? nullptr : m_city.data();
        this->m_model.continentCode = m_continentCode ? m_continentCode.operator->() : nullptr;
        this->m_model.countryCode = m_countryCode ? m_countryCode.operator->() : nullptr;
        this->m_model.latitude = m_latitude ? m_latitude.operator->() : nullptr;
        this->m_model.longitude = m_longitude ? m_longitude.operator->() : nullptr;
    }

    String m_city;
    std::optional<PFContinentCode> m_continentCode;
    std::optional<PFCountryCode> m_countryCode;
    std::optional<double> m_latitude;
    std::optional<double> m_longitude;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFSubscriptionModelWrapper : public ModelWrapper<PFSubscriptionModel, Alloc>
{
public:
    using ModelType = PFSubscriptionModel;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFSubscriptionModelWrapper() = default;

    PFSubscriptionModelWrapper(const PFSubscriptionModel& model) :
        ModelWrapper<PFSubscriptionModel, Alloc>{ model },
        m_status{ model.status ? std::optional<PFSubscriptionProviderStatus>{ *model.status } : std::nullopt },
        m_subscriptionId{ SafeString(model.subscriptionId) },
        m_subscriptionItemId{ SafeString(model.subscriptionItemId) },
        m_subscriptionProvider{ SafeString(model.subscriptionProvider) }
    {
        SetModelPointers();
    }

    PFSubscriptionModelWrapper(const PFSubscriptionModelWrapper& src) :
        PFSubscriptionModelWrapper{ src.Model() }
    {
    }

    PFSubscriptionModelWrapper(PFSubscriptionModelWrapper&& src) :
        PFSubscriptionModelWrapper{}
    {
        swap(*this, src);
    }

    PFSubscriptionModelWrapper& operator=(PFSubscriptionModelWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFSubscriptionModelWrapper() = default;

    friend void swap(PFSubscriptionModelWrapper& lhs, PFSubscriptionModelWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_status, rhs.m_status);
        swap(lhs.m_subscriptionId, rhs.m_subscriptionId);
        swap(lhs.m_subscriptionItemId, rhs.m_subscriptionItemId);
        swap(lhs.m_subscriptionProvider, rhs.m_subscriptionProvider);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetExpiration(time_t value)
    {
        this->m_model.expiration = value;
    }

    void SetInitialSubscriptionTime(time_t value)
    {
        this->m_model.initialSubscriptionTime = value;
    }

    void SetIsActive(bool value)
    {
        this->m_model.isActive = value;
    }

    void SetStatus(std::optional<PFSubscriptionProviderStatus> value)
    {
        m_status = std::move(value);
        this->m_model.status = m_status ? m_status.operator->() : nullptr;
    }

    void SetSubscriptionId(String value)
    {
        m_subscriptionId = std::move(value);
        this->m_model.subscriptionId =  m_subscriptionId.empty() ? nullptr : m_subscriptionId.data();
    }

    void SetSubscriptionItemId(String value)
    {
        m_subscriptionItemId = std::move(value);
        this->m_model.subscriptionItemId =  m_subscriptionItemId.empty() ? nullptr : m_subscriptionItemId.data();
    }

    void SetSubscriptionProvider(String value)
    {
        m_subscriptionProvider = std::move(value);
        this->m_model.subscriptionProvider =  m_subscriptionProvider.empty() ? nullptr : m_subscriptionProvider.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.status = m_status ? m_status.operator->() : nullptr;
        this->m_model.subscriptionId = m_subscriptionId.empty() ? nullptr : m_subscriptionId.data();
        this->m_model.subscriptionItemId = m_subscriptionItemId.empty() ? nullptr : m_subscriptionItemId.data();
        this->m_model.subscriptionProvider = m_subscriptionProvider.empty() ? nullptr : m_subscriptionProvider.data();
    }

    std::optional<PFSubscriptionProviderStatus> m_status;
    String m_subscriptionId;
    String m_subscriptionItemId;
    String m_subscriptionProvider;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMembershipModelWrapper : public ModelWrapper<PFMembershipModel, Alloc>
{
public:
    using ModelType = PFMembershipModel;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMembershipModelWrapper() = default;

    PFMembershipModelWrapper(const PFMembershipModel& model) :
        ModelWrapper<PFMembershipModel, Alloc>{ model },
        m_membershipId{ SafeString(model.membershipId) },
        m_overrideExpiration{ model.overrideExpiration ? std::optional<time_t>{ *model.overrideExpiration } : std::nullopt },
        m_subscriptions{ model.subscriptions, model.subscriptions + model.subscriptionsCount }
    {
        SetModelPointers();
    }

    PFMembershipModelWrapper(const PFMembershipModelWrapper& src) :
        PFMembershipModelWrapper{ src.Model() }
    {
    }

    PFMembershipModelWrapper(PFMembershipModelWrapper&& src) :
        PFMembershipModelWrapper{}
    {
        swap(*this, src);
    }

    PFMembershipModelWrapper& operator=(PFMembershipModelWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMembershipModelWrapper() = default;

    friend void swap(PFMembershipModelWrapper& lhs, PFMembershipModelWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_membershipId, rhs.m_membershipId);
        swap(lhs.m_overrideExpiration, rhs.m_overrideExpiration);
        swap(lhs.m_subscriptions, rhs.m_subscriptions);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetIsActive(bool value)
    {
        this->m_model.isActive = value;
    }

    void SetMembershipExpiration(time_t value)
    {
        this->m_model.membershipExpiration = value;
    }

    void SetMembershipId(String value)
    {
        m_membershipId = std::move(value);
        this->m_model.membershipId =  m_membershipId.empty() ? nullptr : m_membershipId.data();
    }

    void SetOverrideExpiration(std::optional<time_t> value)
    {
        m_overrideExpiration = std::move(value);
        this->m_model.overrideExpiration = m_overrideExpiration ? m_overrideExpiration.operator->() : nullptr;
    }

    void SetSubscriptions(ModelVector<PFSubscriptionModelWrapper<Alloc>, Alloc> value)
    {
        m_subscriptions = std::move(value);
        this->m_model.subscriptions =  m_subscriptions.empty() ? nullptr : m_subscriptions.data();
        this->m_model.subscriptionsCount =  static_cast<uint32_t>(m_subscriptions.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.membershipId = m_membershipId.empty() ? nullptr : m_membershipId.data();
        this->m_model.overrideExpiration = m_overrideExpiration ? m_overrideExpiration.operator->() : nullptr;
        this->m_model.subscriptions = m_subscriptions.empty() ? nullptr : m_subscriptions.data();
    }

    String m_membershipId;
    std::optional<time_t> m_overrideExpiration;
    ModelVector<PFSubscriptionModelWrapper<Alloc>, Alloc> m_subscriptions;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPushNotificationRegistrationModelWrapper : public ModelWrapper<PFPushNotificationRegistrationModel, Alloc>
{
public:
    using ModelType = PFPushNotificationRegistrationModel;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPushNotificationRegistrationModelWrapper() = default;

    PFPushNotificationRegistrationModelWrapper(const PFPushNotificationRegistrationModel& model) :
        ModelWrapper<PFPushNotificationRegistrationModel, Alloc>{ model },
        m_notificationEndpointARN{ SafeString(model.notificationEndpointARN) },
        m_platform{ model.platform ? std::optional<PFPushNotificationPlatform>{ *model.platform } : std::nullopt }
    {
        SetModelPointers();
    }

    PFPushNotificationRegistrationModelWrapper(const PFPushNotificationRegistrationModelWrapper& src) :
        PFPushNotificationRegistrationModelWrapper{ src.Model() }
    {
    }

    PFPushNotificationRegistrationModelWrapper(PFPushNotificationRegistrationModelWrapper&& src) :
        PFPushNotificationRegistrationModelWrapper{}
    {
        swap(*this, src);
    }

    PFPushNotificationRegistrationModelWrapper& operator=(PFPushNotificationRegistrationModelWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPushNotificationRegistrationModelWrapper() = default;

    friend void swap(PFPushNotificationRegistrationModelWrapper& lhs, PFPushNotificationRegistrationModelWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_notificationEndpointARN, rhs.m_notificationEndpointARN);
        swap(lhs.m_platform, rhs.m_platform);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetNotificationEndpointARN(String value)
    {
        m_notificationEndpointARN = std::move(value);
        this->m_model.notificationEndpointARN =  m_notificationEndpointARN.empty() ? nullptr : m_notificationEndpointARN.data();
    }

    void SetPlatform(std::optional<PFPushNotificationPlatform> value)
    {
        m_platform = std::move(value);
        this->m_model.platform = m_platform ? m_platform.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.notificationEndpointARN = m_notificationEndpointARN.empty() ? nullptr : m_notificationEndpointARN.data();
        this->m_model.platform = m_platform ? m_platform.operator->() : nullptr;
    }

    String m_notificationEndpointARN;
    std::optional<PFPushNotificationPlatform> m_platform;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticModelWrapper : public ModelWrapper<PFStatisticModel, Alloc>
{
public:
    using ModelType = PFStatisticModel;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticModelWrapper() = default;

    PFStatisticModelWrapper(const PFStatisticModel& model) :
        ModelWrapper<PFStatisticModel, Alloc>{ model },
        m_name{ SafeString(model.name) }
    {
        SetModelPointers();
    }

    PFStatisticModelWrapper(const PFStatisticModelWrapper& src) :
        PFStatisticModelWrapper{ src.Model() }
    {
    }

    PFStatisticModelWrapper(PFStatisticModelWrapper&& src) :
        PFStatisticModelWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticModelWrapper& operator=(PFStatisticModelWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticModelWrapper() = default;

    friend void swap(PFStatisticModelWrapper& lhs, PFStatisticModelWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_name, rhs.m_name);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

    void SetValue(int32_t value)
    {
        this->m_model.value = value;
    }

    void SetVersion(int32_t value)
    {
        this->m_model.version = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
    }

    String m_name;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFTagModelWrapper : public ModelWrapper<PFTagModel, Alloc>
{
public:
    using ModelType = PFTagModel;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFTagModelWrapper() = default;

    PFTagModelWrapper(const PFTagModel& model) :
        ModelWrapper<PFTagModel, Alloc>{ model },
        m_tagValue{ SafeString(model.tagValue) }
    {
        SetModelPointers();
    }

    PFTagModelWrapper(const PFTagModelWrapper& src) :
        PFTagModelWrapper{ src.Model() }
    {
    }

    PFTagModelWrapper(PFTagModelWrapper&& src) :
        PFTagModelWrapper{}
    {
        swap(*this, src);
    }

    PFTagModelWrapper& operator=(PFTagModelWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFTagModelWrapper() = default;

    friend void swap(PFTagModelWrapper& lhs, PFTagModelWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_tagValue, rhs.m_tagValue);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetTagValue(String value)
    {
        m_tagValue = std::move(value);
        this->m_model.tagValue =  m_tagValue.empty() ? nullptr : m_tagValue.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.tagValue = m_tagValue.empty() ? nullptr : m_tagValue.data();
    }

    String m_tagValue;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFValueToDateModelWrapper : public ModelWrapper<PFValueToDateModel, Alloc>
{
public:
    using ModelType = PFValueToDateModel;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFValueToDateModelWrapper() = default;

    PFValueToDateModelWrapper(const PFValueToDateModel& model) :
        ModelWrapper<PFValueToDateModel, Alloc>{ model },
        m_currency{ SafeString(model.currency) },
        m_totalValueAsDecimal{ SafeString(model.totalValueAsDecimal) }
    {
        SetModelPointers();
    }

    PFValueToDateModelWrapper(const PFValueToDateModelWrapper& src) :
        PFValueToDateModelWrapper{ src.Model() }
    {
    }

    PFValueToDateModelWrapper(PFValueToDateModelWrapper&& src) :
        PFValueToDateModelWrapper{}
    {
        swap(*this, src);
    }

    PFValueToDateModelWrapper& operator=(PFValueToDateModelWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFValueToDateModelWrapper() = default;

    friend void swap(PFValueToDateModelWrapper& lhs, PFValueToDateModelWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_currency, rhs.m_currency);
        swap(lhs.m_totalValueAsDecimal, rhs.m_totalValueAsDecimal);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCurrency(String value)
    {
        m_currency = std::move(value);
        this->m_model.currency =  m_currency.empty() ? nullptr : m_currency.data();
    }

    void SetTotalValue(uint32_t value)
    {
        this->m_model.totalValue = value;
    }

    void SetTotalValueAsDecimal(String value)
    {
        m_totalValueAsDecimal = std::move(value);
        this->m_model.totalValueAsDecimal =  m_totalValueAsDecimal.empty() ? nullptr : m_totalValueAsDecimal.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.currency = m_currency.empty() ? nullptr : m_currency.data();
        this->m_model.totalValueAsDecimal = m_totalValueAsDecimal.empty() ? nullptr : m_totalValueAsDecimal.data();
    }

    String m_currency;
    String m_totalValueAsDecimal;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayerProfileModelWrapper : public ModelWrapper<PFPlayerProfileModel, Alloc>
{
public:
    using ModelType = PFPlayerProfileModel;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerProfileModelWrapper() = default;

    PFPlayerProfileModelWrapper(const PFPlayerProfileModel& model) :
        ModelWrapper<PFPlayerProfileModel, Alloc>{ model },
        m_adCampaignAttributions{ model.adCampaignAttributions, model.adCampaignAttributions + model.adCampaignAttributionsCount },
        m_avatarUrl{ SafeString(model.avatarUrl) },
        m_bannedUntil{ model.bannedUntil ? std::optional<time_t>{ *model.bannedUntil } : std::nullopt },
        m_contactEmailAddresses{ model.contactEmailAddresses, model.contactEmailAddresses + model.contactEmailAddressesCount },
        m_created{ model.created ? std::optional<time_t>{ *model.created } : std::nullopt },
        m_displayName{ SafeString(model.displayName) },
        m_experimentVariants{ model.experimentVariants, model.experimentVariants + model.experimentVariantsCount },
        m_lastLogin{ model.lastLogin ? std::optional<time_t>{ *model.lastLogin } : std::nullopt },
        m_linkedAccounts{ model.linkedAccounts, model.linkedAccounts + model.linkedAccountsCount },
        m_locations{ model.locations, model.locations + model.locationsCount },
        m_memberships{ model.memberships, model.memberships + model.membershipsCount },
        m_origination{ model.origination ? std::optional<PFLoginIdentityProvider>{ *model.origination } : std::nullopt },
        m_playerId{ SafeString(model.playerId) },
        m_publisherId{ SafeString(model.publisherId) },
        m_pushNotificationRegistrations{ model.pushNotificationRegistrations, model.pushNotificationRegistrations + model.pushNotificationRegistrationsCount },
        m_statistics{ model.statistics, model.statistics + model.statisticsCount },
        m_tags{ model.tags, model.tags + model.tagsCount },
        m_titleId{ SafeString(model.titleId) },
        m_totalValueToDateInUSD{ model.totalValueToDateInUSD ? std::optional<uint32_t>{ *model.totalValueToDateInUSD } : std::nullopt },
        m_valuesToDate{ model.valuesToDate, model.valuesToDate + model.valuesToDateCount }
    {
        SetModelPointers();
    }

    PFPlayerProfileModelWrapper(const PFPlayerProfileModelWrapper& src) :
        PFPlayerProfileModelWrapper{ src.Model() }
    {
    }

    PFPlayerProfileModelWrapper(PFPlayerProfileModelWrapper&& src) :
        PFPlayerProfileModelWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerProfileModelWrapper& operator=(PFPlayerProfileModelWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerProfileModelWrapper() = default;

    friend void swap(PFPlayerProfileModelWrapper& lhs, PFPlayerProfileModelWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_adCampaignAttributions, rhs.m_adCampaignAttributions);
        swap(lhs.m_avatarUrl, rhs.m_avatarUrl);
        swap(lhs.m_bannedUntil, rhs.m_bannedUntil);
        swap(lhs.m_contactEmailAddresses, rhs.m_contactEmailAddresses);
        swap(lhs.m_created, rhs.m_created);
        swap(lhs.m_displayName, rhs.m_displayName);
        swap(lhs.m_experimentVariants, rhs.m_experimentVariants);
        swap(lhs.m_lastLogin, rhs.m_lastLogin);
        swap(lhs.m_linkedAccounts, rhs.m_linkedAccounts);
        swap(lhs.m_locations, rhs.m_locations);
        swap(lhs.m_memberships, rhs.m_memberships);
        swap(lhs.m_origination, rhs.m_origination);
        swap(lhs.m_playerId, rhs.m_playerId);
        swap(lhs.m_publisherId, rhs.m_publisherId);
        swap(lhs.m_pushNotificationRegistrations, rhs.m_pushNotificationRegistrations);
        swap(lhs.m_statistics, rhs.m_statistics);
        swap(lhs.m_tags, rhs.m_tags);
        swap(lhs.m_titleId, rhs.m_titleId);
        swap(lhs.m_totalValueToDateInUSD, rhs.m_totalValueToDateInUSD);
        swap(lhs.m_valuesToDate, rhs.m_valuesToDate);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAdCampaignAttributions(ModelVector<PFAdCampaignAttributionModelWrapper<Alloc>, Alloc> value)
    {
        m_adCampaignAttributions = std::move(value);
        this->m_model.adCampaignAttributions =  m_adCampaignAttributions.empty() ? nullptr : m_adCampaignAttributions.data();
        this->m_model.adCampaignAttributionsCount =  static_cast<uint32_t>(m_adCampaignAttributions.size());
    }

    void SetAvatarUrl(String value)
    {
        m_avatarUrl = std::move(value);
        this->m_model.avatarUrl =  m_avatarUrl.empty() ? nullptr : m_avatarUrl.data();
    }

    void SetBannedUntil(std::optional<time_t> value)
    {
        m_bannedUntil = std::move(value);
        this->m_model.bannedUntil = m_bannedUntil ? m_bannedUntil.operator->() : nullptr;
    }

    void SetContactEmailAddresses(ModelVector<PFContactEmailInfoModelWrapper<Alloc>, Alloc> value)
    {
        m_contactEmailAddresses = std::move(value);
        this->m_model.contactEmailAddresses =  m_contactEmailAddresses.empty() ? nullptr : m_contactEmailAddresses.data();
        this->m_model.contactEmailAddressesCount =  static_cast<uint32_t>(m_contactEmailAddresses.size());
    }

    void SetCreated(std::optional<time_t> value)
    {
        m_created = std::move(value);
        this->m_model.created = m_created ? m_created.operator->() : nullptr;
    }

    void SetDisplayName(String value)
    {
        m_displayName = std::move(value);
        this->m_model.displayName =  m_displayName.empty() ? nullptr : m_displayName.data();
    }

    void SetExperimentVariants(CStringVector<Alloc> value)
    {
        m_experimentVariants = std::move(value);
        this->m_model.experimentVariants =  m_experimentVariants.empty() ? nullptr : m_experimentVariants.data();
        this->m_model.experimentVariantsCount =  static_cast<uint32_t>(m_experimentVariants.size());
    }

    void SetLastLogin(std::optional<time_t> value)
    {
        m_lastLogin = std::move(value);
        this->m_model.lastLogin = m_lastLogin ? m_lastLogin.operator->() : nullptr;
    }

    void SetLinkedAccounts(ModelVector<PFLinkedPlatformAccountModelWrapper<Alloc>, Alloc> value)
    {
        m_linkedAccounts = std::move(value);
        this->m_model.linkedAccounts =  m_linkedAccounts.empty() ? nullptr : m_linkedAccounts.data();
        this->m_model.linkedAccountsCount =  static_cast<uint32_t>(m_linkedAccounts.size());
    }

    void SetLocations(ModelVector<PFLocationModelWrapper<Alloc>, Alloc> value)
    {
        m_locations = std::move(value);
        this->m_model.locations =  m_locations.empty() ? nullptr : m_locations.data();
        this->m_model.locationsCount =  static_cast<uint32_t>(m_locations.size());
    }

    void SetMemberships(ModelVector<PFMembershipModelWrapper<Alloc>, Alloc> value)
    {
        m_memberships = std::move(value);
        this->m_model.memberships =  m_memberships.empty() ? nullptr : m_memberships.data();
        this->m_model.membershipsCount =  static_cast<uint32_t>(m_memberships.size());
    }

    void SetOrigination(std::optional<PFLoginIdentityProvider> value)
    {
        m_origination = std::move(value);
        this->m_model.origination = m_origination ? m_origination.operator->() : nullptr;
    }

    void SetPlayerId(String value)
    {
        m_playerId = std::move(value);
        this->m_model.playerId =  m_playerId.empty() ? nullptr : m_playerId.data();
    }

    void SetPublisherId(String value)
    {
        m_publisherId = std::move(value);
        this->m_model.publisherId =  m_publisherId.empty() ? nullptr : m_publisherId.data();
    }

    void SetPushNotificationRegistrations(ModelVector<PFPushNotificationRegistrationModelWrapper<Alloc>, Alloc> value)
    {
        m_pushNotificationRegistrations = std::move(value);
        this->m_model.pushNotificationRegistrations =  m_pushNotificationRegistrations.empty() ? nullptr : m_pushNotificationRegistrations.data();
        this->m_model.pushNotificationRegistrationsCount =  static_cast<uint32_t>(m_pushNotificationRegistrations.size());
    }

    void SetStatistics(ModelVector<PFStatisticModelWrapper<Alloc>, Alloc> value)
    {
        m_statistics = std::move(value);
        this->m_model.statistics =  m_statistics.empty() ? nullptr : m_statistics.data();
        this->m_model.statisticsCount =  static_cast<uint32_t>(m_statistics.size());
    }

    void SetTags(ModelVector<PFTagModelWrapper<Alloc>, Alloc> value)
    {
        m_tags = std::move(value);
        this->m_model.tags =  m_tags.empty() ? nullptr : m_tags.data();
        this->m_model.tagsCount =  static_cast<uint32_t>(m_tags.size());
    }

    void SetTitleId(String value)
    {
        m_titleId = std::move(value);
        this->m_model.titleId =  m_titleId.empty() ? nullptr : m_titleId.data();
    }

    void SetTotalValueToDateInUSD(std::optional<uint32_t> value)
    {
        m_totalValueToDateInUSD = std::move(value);
        this->m_model.totalValueToDateInUSD = m_totalValueToDateInUSD ? m_totalValueToDateInUSD.operator->() : nullptr;
    }

    void SetValuesToDate(ModelVector<PFValueToDateModelWrapper<Alloc>, Alloc> value)
    {
        m_valuesToDate = std::move(value);
        this->m_model.valuesToDate =  m_valuesToDate.empty() ? nullptr : m_valuesToDate.data();
        this->m_model.valuesToDateCount =  static_cast<uint32_t>(m_valuesToDate.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.adCampaignAttributions = m_adCampaignAttributions.empty() ? nullptr : m_adCampaignAttributions.data();
        this->m_model.avatarUrl = m_avatarUrl.empty() ? nullptr : m_avatarUrl.data();
        this->m_model.bannedUntil = m_bannedUntil ? m_bannedUntil.operator->() : nullptr;
        this->m_model.contactEmailAddresses = m_contactEmailAddresses.empty() ? nullptr : m_contactEmailAddresses.data();
        this->m_model.created = m_created ? m_created.operator->() : nullptr;
        this->m_model.displayName = m_displayName.empty() ? nullptr : m_displayName.data();
        this->m_model.experimentVariants = m_experimentVariants.empty() ? nullptr : m_experimentVariants.data();
        this->m_model.lastLogin = m_lastLogin ? m_lastLogin.operator->() : nullptr;
        this->m_model.linkedAccounts = m_linkedAccounts.empty() ? nullptr : m_linkedAccounts.data();
        this->m_model.locations = m_locations.empty() ? nullptr : m_locations.data();
        this->m_model.memberships = m_memberships.empty() ? nullptr : m_memberships.data();
        this->m_model.origination = m_origination ? m_origination.operator->() : nullptr;
        this->m_model.playerId = m_playerId.empty() ? nullptr : m_playerId.data();
        this->m_model.publisherId = m_publisherId.empty() ? nullptr : m_publisherId.data();
        this->m_model.pushNotificationRegistrations = m_pushNotificationRegistrations.empty() ? nullptr : m_pushNotificationRegistrations.data();
        this->m_model.statistics = m_statistics.empty() ? nullptr : m_statistics.data();
        this->m_model.tags = m_tags.empty() ? nullptr : m_tags.data();
        this->m_model.titleId = m_titleId.empty() ? nullptr : m_titleId.data();
        this->m_model.totalValueToDateInUSD = m_totalValueToDateInUSD ? m_totalValueToDateInUSD.operator->() : nullptr;
        this->m_model.valuesToDate = m_valuesToDate.empty() ? nullptr : m_valuesToDate.data();
    }

    ModelVector<PFAdCampaignAttributionModelWrapper<Alloc>, Alloc> m_adCampaignAttributions;
    String m_avatarUrl;
    std::optional<time_t> m_bannedUntil;
    ModelVector<PFContactEmailInfoModelWrapper<Alloc>, Alloc> m_contactEmailAddresses;
    std::optional<time_t> m_created;
    String m_displayName;
    CStringVector<Alloc> m_experimentVariants;
    std::optional<time_t> m_lastLogin;
    ModelVector<PFLinkedPlatformAccountModelWrapper<Alloc>, Alloc> m_linkedAccounts;
    ModelVector<PFLocationModelWrapper<Alloc>, Alloc> m_locations;
    ModelVector<PFMembershipModelWrapper<Alloc>, Alloc> m_memberships;
    std::optional<PFLoginIdentityProvider> m_origination;
    String m_playerId;
    String m_publisherId;
    ModelVector<PFPushNotificationRegistrationModelWrapper<Alloc>, Alloc> m_pushNotificationRegistrations;
    ModelVector<PFStatisticModelWrapper<Alloc>, Alloc> m_statistics;
    ModelVector<PFTagModelWrapper<Alloc>, Alloc> m_tags;
    String m_titleId;
    std::optional<uint32_t> m_totalValueToDateInUSD;
    ModelVector<PFValueToDateModelWrapper<Alloc>, Alloc> m_valuesToDate;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGetPlayerCombinedInfoRequestParamsWrapper : public ModelWrapper<PFGetPlayerCombinedInfoRequestParams, Alloc>
{
public:
    using ModelType = PFGetPlayerCombinedInfoRequestParams;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGetPlayerCombinedInfoRequestParamsWrapper() = default;

    PFGetPlayerCombinedInfoRequestParamsWrapper(const PFGetPlayerCombinedInfoRequestParams& model) :
        ModelWrapper<PFGetPlayerCombinedInfoRequestParams, Alloc>{ model },
        m_playerStatisticNames{ model.playerStatisticNames, model.playerStatisticNames + model.playerStatisticNamesCount },
        m_profileConstraints{ model.profileConstraints ? std::optional<PFPlayerProfileViewConstraintsWrapper<Alloc>>{ *model.profileConstraints } : std::nullopt },
        m_titleDataKeys{ model.titleDataKeys, model.titleDataKeys + model.titleDataKeysCount },
        m_userDataKeys{ model.userDataKeys, model.userDataKeys + model.userDataKeysCount },
        m_userReadOnlyDataKeys{ model.userReadOnlyDataKeys, model.userReadOnlyDataKeys + model.userReadOnlyDataKeysCount }
    {
        SetModelPointers();
    }

    PFGetPlayerCombinedInfoRequestParamsWrapper(const PFGetPlayerCombinedInfoRequestParamsWrapper& src) :
        PFGetPlayerCombinedInfoRequestParamsWrapper{ src.Model() }
    {
    }

    PFGetPlayerCombinedInfoRequestParamsWrapper(PFGetPlayerCombinedInfoRequestParamsWrapper&& src) :
        PFGetPlayerCombinedInfoRequestParamsWrapper{}
    {
        swap(*this, src);
    }

    PFGetPlayerCombinedInfoRequestParamsWrapper& operator=(PFGetPlayerCombinedInfoRequestParamsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGetPlayerCombinedInfoRequestParamsWrapper() = default;

    friend void swap(PFGetPlayerCombinedInfoRequestParamsWrapper& lhs, PFGetPlayerCombinedInfoRequestParamsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_playerStatisticNames, rhs.m_playerStatisticNames);
        swap(lhs.m_profileConstraints, rhs.m_profileConstraints);
        swap(lhs.m_titleDataKeys, rhs.m_titleDataKeys);
        swap(lhs.m_userDataKeys, rhs.m_userDataKeys);
        swap(lhs.m_userReadOnlyDataKeys, rhs.m_userReadOnlyDataKeys);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetGetCharacterInventories(bool value)
    {
        this->m_model.getCharacterInventories = value;
    }

    void SetGetCharacterList(bool value)
    {
        this->m_model.getCharacterList = value;
    }

    void SetGetPlayerProfile(bool value)
    {
        this->m_model.getPlayerProfile = value;
    }

    void SetGetPlayerStatistics(bool value)
    {
        this->m_model.getPlayerStatistics = value;
    }

    void SetGetTitleData(bool value)
    {
        this->m_model.getTitleData = value;
    }

    void SetGetUserAccountInfo(bool value)
    {
        this->m_model.getUserAccountInfo = value;
    }

    void SetGetUserData(bool value)
    {
        this->m_model.getUserData = value;
    }

    void SetGetUserInventory(bool value)
    {
        this->m_model.getUserInventory = value;
    }

    void SetGetUserReadOnlyData(bool value)
    {
        this->m_model.getUserReadOnlyData = value;
    }

    void SetGetUserVirtualCurrency(bool value)
    {
        this->m_model.getUserVirtualCurrency = value;
    }

    void SetPlayerStatisticNames(CStringVector<Alloc> value)
    {
        m_playerStatisticNames = std::move(value);
        this->m_model.playerStatisticNames =  m_playerStatisticNames.empty() ? nullptr : m_playerStatisticNames.data();
        this->m_model.playerStatisticNamesCount =  static_cast<uint32_t>(m_playerStatisticNames.size());
    }

    void SetProfileConstraints(std::optional<PFPlayerProfileViewConstraintsWrapper<Alloc>> value)
    {
        m_profileConstraints = std::move(value);
        this->m_model.profileConstraints = m_profileConstraints ? &m_profileConstraints->Model() : nullptr;
    }

    void SetTitleDataKeys(CStringVector<Alloc> value)
    {
        m_titleDataKeys = std::move(value);
        this->m_model.titleDataKeys =  m_titleDataKeys.empty() ? nullptr : m_titleDataKeys.data();
        this->m_model.titleDataKeysCount =  static_cast<uint32_t>(m_titleDataKeys.size());
    }

    void SetUserDataKeys(CStringVector<Alloc> value)
    {
        m_userDataKeys = std::move(value);
        this->m_model.userDataKeys =  m_userDataKeys.empty() ? nullptr : m_userDataKeys.data();
        this->m_model.userDataKeysCount =  static_cast<uint32_t>(m_userDataKeys.size());
    }

    void SetUserReadOnlyDataKeys(CStringVector<Alloc> value)
    {
        m_userReadOnlyDataKeys = std::move(value);
        this->m_model.userReadOnlyDataKeys =  m_userReadOnlyDataKeys.empty() ? nullptr : m_userReadOnlyDataKeys.data();
        this->m_model.userReadOnlyDataKeysCount =  static_cast<uint32_t>(m_userReadOnlyDataKeys.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.playerStatisticNames = m_playerStatisticNames.empty() ? nullptr : m_playerStatisticNames.data();
        this->m_model.profileConstraints = m_profileConstraints ?  &m_profileConstraints->Model() : nullptr;
        this->m_model.titleDataKeys = m_titleDataKeys.empty() ? nullptr : m_titleDataKeys.data();
        this->m_model.userDataKeys = m_userDataKeys.empty() ? nullptr : m_userDataKeys.data();
        this->m_model.userReadOnlyDataKeys = m_userReadOnlyDataKeys.empty() ? nullptr : m_userReadOnlyDataKeys.data();
    }

    CStringVector<Alloc> m_playerStatisticNames;
    std::optional<PFPlayerProfileViewConstraintsWrapper<Alloc>> m_profileConstraints;
    CStringVector<Alloc> m_titleDataKeys;
    CStringVector<Alloc> m_userDataKeys;
    CStringVector<Alloc> m_userReadOnlyDataKeys;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCharacterInventoryWrapper : public ModelWrapper<PFCharacterInventory, Alloc>
{
public:
    using ModelType = PFCharacterInventory;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCharacterInventoryWrapper() = default;

    PFCharacterInventoryWrapper(const PFCharacterInventory& model) :
        ModelWrapper<PFCharacterInventory, Alloc>{ model },
        m_characterId{ SafeString(model.characterId) },
        m_inventory{ model.inventory, model.inventory + model.inventoryCount }
    {
        SetModelPointers();
    }

    PFCharacterInventoryWrapper(const PFCharacterInventoryWrapper& src) :
        PFCharacterInventoryWrapper{ src.Model() }
    {
    }

    PFCharacterInventoryWrapper(PFCharacterInventoryWrapper&& src) :
        PFCharacterInventoryWrapper{}
    {
        swap(*this, src);
    }

    PFCharacterInventoryWrapper& operator=(PFCharacterInventoryWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCharacterInventoryWrapper() = default;

    friend void swap(PFCharacterInventoryWrapper& lhs, PFCharacterInventoryWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_characterId, rhs.m_characterId);
        swap(lhs.m_inventory, rhs.m_inventory);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCharacterId(String value)
    {
        m_characterId = std::move(value);
        this->m_model.characterId =  m_characterId.empty() ? nullptr : m_characterId.data();
    }

    void SetInventory(ModelVector<PFItemInstanceWrapper<Alloc>, Alloc> value)
    {
        m_inventory = std::move(value);
        this->m_model.inventory =  m_inventory.empty() ? nullptr : m_inventory.data();
        this->m_model.inventoryCount =  static_cast<uint32_t>(m_inventory.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.characterId = m_characterId.empty() ? nullptr : m_characterId.data();
        this->m_model.inventory = m_inventory.empty() ? nullptr : m_inventory.data();
    }

    String m_characterId;
    ModelVector<PFItemInstanceWrapper<Alloc>, Alloc> m_inventory;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticValueWrapper : public ModelWrapper<PFStatisticValue, Alloc>
{
public:
    using ModelType = PFStatisticValue;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticValueWrapper() = default;

    PFStatisticValueWrapper(const PFStatisticValue& model) :
        ModelWrapper<PFStatisticValue, Alloc>{ model },
        m_statisticName{ SafeString(model.statisticName) }
    {
        SetModelPointers();
    }

    PFStatisticValueWrapper(const PFStatisticValueWrapper& src) :
        PFStatisticValueWrapper{ src.Model() }
    {
    }

    PFStatisticValueWrapper(PFStatisticValueWrapper&& src) :
        PFStatisticValueWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticValueWrapper& operator=(PFStatisticValueWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticValueWrapper() = default;

    friend void swap(PFStatisticValueWrapper& lhs, PFStatisticValueWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_statisticName, rhs.m_statisticName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetStatisticName(String value)
    {
        m_statisticName = std::move(value);
        this->m_model.statisticName =  m_statisticName.empty() ? nullptr : m_statisticName.data();
    }

    void SetValue(int32_t value)
    {
        this->m_model.value = value;
    }

    void SetVersion(uint32_t value)
    {
        this->m_model.version = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.statisticName = m_statisticName.empty() ? nullptr : m_statisticName.data();
    }

    String m_statisticName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFGetPlayerCombinedInfoResultPayloadWrapper : public ModelWrapper<PFGetPlayerCombinedInfoResultPayload, Alloc>
{
public:
    using ModelType = PFGetPlayerCombinedInfoResultPayload;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFGetPlayerCombinedInfoResultPayloadWrapper() = default;

    PFGetPlayerCombinedInfoResultPayloadWrapper(const PFGetPlayerCombinedInfoResultPayload& model) :
        ModelWrapper<PFGetPlayerCombinedInfoResultPayload, Alloc>{ model },
        m_accountInfo{ model.accountInfo ? std::optional<PFUserAccountInfoWrapper<Alloc>>{ *model.accountInfo } : std::nullopt },
        m_characterInventories{ model.characterInventories, model.characterInventories + model.characterInventoriesCount },
        m_characterList{ model.characterList, model.characterList + model.characterListCount },
        m_playerProfile{ model.playerProfile ? std::optional<PFPlayerProfileModelWrapper<Alloc>>{ *model.playerProfile } : std::nullopt },
        m_playerStatistics{ model.playerStatistics, model.playerStatistics + model.playerStatisticsCount },
        m_titleData{ model.titleData, model.titleData + model.titleDataCount },
        m_userData{ model.userData, model.userData + model.userDataCount },
        m_userInventory{ model.userInventory, model.userInventory + model.userInventoryCount },
        m_userReadOnlyData{ model.userReadOnlyData, model.userReadOnlyData + model.userReadOnlyDataCount },
        m_userVirtualCurrency{ model.userVirtualCurrency, model.userVirtualCurrency + model.userVirtualCurrencyCount },
        m_userVirtualCurrencyRechargeTimes{ model.userVirtualCurrencyRechargeTimes, model.userVirtualCurrencyRechargeTimes + model.userVirtualCurrencyRechargeTimesCount }
    {
        SetModelPointers();
    }

    PFGetPlayerCombinedInfoResultPayloadWrapper(const PFGetPlayerCombinedInfoResultPayloadWrapper& src) :
        PFGetPlayerCombinedInfoResultPayloadWrapper{ src.Model() }
    {
    }

    PFGetPlayerCombinedInfoResultPayloadWrapper(PFGetPlayerCombinedInfoResultPayloadWrapper&& src) :
        PFGetPlayerCombinedInfoResultPayloadWrapper{}
    {
        swap(*this, src);
    }

    PFGetPlayerCombinedInfoResultPayloadWrapper& operator=(PFGetPlayerCombinedInfoResultPayloadWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFGetPlayerCombinedInfoResultPayloadWrapper() = default;

    friend void swap(PFGetPlayerCombinedInfoResultPayloadWrapper& lhs, PFGetPlayerCombinedInfoResultPayloadWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_accountInfo, rhs.m_accountInfo);
        swap(lhs.m_characterInventories, rhs.m_characterInventories);
        swap(lhs.m_characterList, rhs.m_characterList);
        swap(lhs.m_playerProfile, rhs.m_playerProfile);
        swap(lhs.m_playerStatistics, rhs.m_playerStatistics);
        swap(lhs.m_titleData, rhs.m_titleData);
        swap(lhs.m_userData, rhs.m_userData);
        swap(lhs.m_userInventory, rhs.m_userInventory);
        swap(lhs.m_userReadOnlyData, rhs.m_userReadOnlyData);
        swap(lhs.m_userVirtualCurrency, rhs.m_userVirtualCurrency);
        swap(lhs.m_userVirtualCurrencyRechargeTimes, rhs.m_userVirtualCurrencyRechargeTimes);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAccountInfo(std::optional<PFUserAccountInfoWrapper<Alloc>> value)
    {
        m_accountInfo = std::move(value);
        this->m_model.accountInfo = m_accountInfo ? &m_accountInfo->Model() : nullptr;
    }

    void SetCharacterInventories(ModelVector<PFCharacterInventoryWrapper<Alloc>, Alloc> value)
    {
        m_characterInventories = std::move(value);
        this->m_model.characterInventories =  m_characterInventories.empty() ? nullptr : m_characterInventories.data();
        this->m_model.characterInventoriesCount =  static_cast<uint32_t>(m_characterInventories.size());
    }

    void SetCharacterList(ModelVector<PFCharacterResultWrapper<Alloc>, Alloc> value)
    {
        m_characterList = std::move(value);
        this->m_model.characterList =  m_characterList.empty() ? nullptr : m_characterList.data();
        this->m_model.characterListCount =  static_cast<uint32_t>(m_characterList.size());
    }

    void SetPlayerProfile(std::optional<PFPlayerProfileModelWrapper<Alloc>> value)
    {
        m_playerProfile = std::move(value);
        this->m_model.playerProfile = m_playerProfile ? &m_playerProfile->Model() : nullptr;
    }

    void SetPlayerStatistics(ModelVector<PFStatisticValueWrapper<Alloc>, Alloc> value)
    {
        m_playerStatistics = std::move(value);
        this->m_model.playerStatistics =  m_playerStatistics.empty() ? nullptr : m_playerStatistics.data();
        this->m_model.playerStatisticsCount =  static_cast<uint32_t>(m_playerStatistics.size());
    }

    void SetTitleData(StringDictionaryEntryVector<Alloc> value)
    {
        m_titleData = std::move(value);
        this->m_model.titleData =  m_titleData.empty() ? nullptr : m_titleData.data();
        this->m_model.titleDataCount =  static_cast<uint32_t>(m_titleData.size());
    }

    void SetUserData(ModelDictionaryEntryVector<PFUserDataRecordWrapper<Alloc>, Alloc> value)
    {
        m_userData = std::move(value);
        this->m_model.userData =  m_userData.empty() ? nullptr : m_userData.data();
        this->m_model.userDataCount =  static_cast<uint32_t>(m_userData.size());
    }

    void SetUserDataVersion(uint32_t value)
    {
        this->m_model.userDataVersion = value;
    }

    void SetUserInventory(ModelVector<PFItemInstanceWrapper<Alloc>, Alloc> value)
    {
        m_userInventory = std::move(value);
        this->m_model.userInventory =  m_userInventory.empty() ? nullptr : m_userInventory.data();
        this->m_model.userInventoryCount =  static_cast<uint32_t>(m_userInventory.size());
    }

    void SetUserReadOnlyData(ModelDictionaryEntryVector<PFUserDataRecordWrapper<Alloc>, Alloc> value)
    {
        m_userReadOnlyData = std::move(value);
        this->m_model.userReadOnlyData =  m_userReadOnlyData.empty() ? nullptr : m_userReadOnlyData.data();
        this->m_model.userReadOnlyDataCount =  static_cast<uint32_t>(m_userReadOnlyData.size());
    }

    void SetUserReadOnlyDataVersion(uint32_t value)
    {
        this->m_model.userReadOnlyDataVersion = value;
    }

    void SetUserVirtualCurrency(DictionaryEntryVector<PFInt32DictionaryEntry, Alloc> value)
    {
        m_userVirtualCurrency = std::move(value);
        this->m_model.userVirtualCurrency =  m_userVirtualCurrency.empty() ? nullptr : m_userVirtualCurrency.data();
        this->m_model.userVirtualCurrencyCount =  static_cast<uint32_t>(m_userVirtualCurrency.size());
    }

    void SetUserVirtualCurrencyRechargeTimes(ModelDictionaryEntryVector<PFVirtualCurrencyRechargeTimeWrapper<Alloc>, Alloc> value)
    {
        m_userVirtualCurrencyRechargeTimes = std::move(value);
        this->m_model.userVirtualCurrencyRechargeTimes =  m_userVirtualCurrencyRechargeTimes.empty() ? nullptr : m_userVirtualCurrencyRechargeTimes.data();
        this->m_model.userVirtualCurrencyRechargeTimesCount =  static_cast<uint32_t>(m_userVirtualCurrencyRechargeTimes.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.accountInfo = m_accountInfo ?  &m_accountInfo->Model() : nullptr;
        this->m_model.characterInventories = m_characterInventories.empty() ? nullptr : m_characterInventories.data();
        this->m_model.characterList = m_characterList.empty() ? nullptr : m_characterList.data();
        this->m_model.playerProfile = m_playerProfile ?  &m_playerProfile->Model() : nullptr;
        this->m_model.playerStatistics = m_playerStatistics.empty() ? nullptr : m_playerStatistics.data();
        this->m_model.titleData = m_titleData.empty() ? nullptr : m_titleData.data();
        this->m_model.userData = m_userData.empty() ? nullptr : m_userData.data();
        this->m_model.userInventory = m_userInventory.empty() ? nullptr : m_userInventory.data();
        this->m_model.userReadOnlyData = m_userReadOnlyData.empty() ? nullptr : m_userReadOnlyData.data();
        this->m_model.userVirtualCurrency = m_userVirtualCurrency.empty() ? nullptr : m_userVirtualCurrency.data();
        this->m_model.userVirtualCurrencyRechargeTimes = m_userVirtualCurrencyRechargeTimes.empty() ? nullptr : m_userVirtualCurrencyRechargeTimes.data();
    }

    std::optional<PFUserAccountInfoWrapper<Alloc>> m_accountInfo;
    ModelVector<PFCharacterInventoryWrapper<Alloc>, Alloc> m_characterInventories;
    ModelVector<PFCharacterResultWrapper<Alloc>, Alloc> m_characterList;
    std::optional<PFPlayerProfileModelWrapper<Alloc>> m_playerProfile;
    ModelVector<PFStatisticValueWrapper<Alloc>, Alloc> m_playerStatistics;
    StringDictionaryEntryVector<Alloc> m_titleData;
    ModelDictionaryEntryVector<PFUserDataRecordWrapper<Alloc>, Alloc> m_userData;
    ModelVector<PFItemInstanceWrapper<Alloc>, Alloc> m_userInventory;
    ModelDictionaryEntryVector<PFUserDataRecordWrapper<Alloc>, Alloc> m_userReadOnlyData;
    DictionaryEntryVector<PFInt32DictionaryEntry, Alloc> m_userVirtualCurrency;
    ModelDictionaryEntryVector<PFVirtualCurrencyRechargeTimeWrapper<Alloc>, Alloc> m_userVirtualCurrencyRechargeTimes;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFVariableWrapper : public ModelWrapper<PFVariable, Alloc>
{
public:
    using ModelType = PFVariable;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFVariableWrapper() = default;

    PFVariableWrapper(const PFVariable& model) :
        ModelWrapper<PFVariable, Alloc>{ model },
        m_name{ SafeString(model.name) },
        m_value{ SafeString(model.value) }
    {
        SetModelPointers();
    }

    PFVariableWrapper(const PFVariableWrapper& src) :
        PFVariableWrapper{ src.Model() }
    {
    }

    PFVariableWrapper(PFVariableWrapper&& src) :
        PFVariableWrapper{}
    {
        swap(*this, src);
    }

    PFVariableWrapper& operator=(PFVariableWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFVariableWrapper() = default;

    friend void swap(PFVariableWrapper& lhs, PFVariableWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_name, rhs.m_name);
        swap(lhs.m_value, rhs.m_value);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

    void SetValue(String value)
    {
        m_value = std::move(value);
        this->m_model.value =  m_value.empty() ? nullptr : m_value.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
        this->m_model.value = m_value.empty() ? nullptr : m_value.data();
    }

    String m_name;
    String m_value;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFTreatmentAssignmentWrapper : public ModelWrapper<PFTreatmentAssignment, Alloc>
{
public:
    using ModelType = PFTreatmentAssignment;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFTreatmentAssignmentWrapper() = default;

    PFTreatmentAssignmentWrapper(const PFTreatmentAssignment& model) :
        ModelWrapper<PFTreatmentAssignment, Alloc>{ model },
        m_variables{ model.variables, model.variables + model.variablesCount },
        m_variants{ model.variants, model.variants + model.variantsCount }
    {
        SetModelPointers();
    }

    PFTreatmentAssignmentWrapper(const PFTreatmentAssignmentWrapper& src) :
        PFTreatmentAssignmentWrapper{ src.Model() }
    {
    }

    PFTreatmentAssignmentWrapper(PFTreatmentAssignmentWrapper&& src) :
        PFTreatmentAssignmentWrapper{}
    {
        swap(*this, src);
    }

    PFTreatmentAssignmentWrapper& operator=(PFTreatmentAssignmentWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFTreatmentAssignmentWrapper() = default;

    friend void swap(PFTreatmentAssignmentWrapper& lhs, PFTreatmentAssignmentWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_variables, rhs.m_variables);
        swap(lhs.m_variants, rhs.m_variants);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetVariables(ModelVector<PFVariableWrapper<Alloc>, Alloc> value)
    {
        m_variables = std::move(value);
        this->m_model.variables =  m_variables.empty() ? nullptr : m_variables.data();
        this->m_model.variablesCount =  static_cast<uint32_t>(m_variables.size());
    }

    void SetVariants(CStringVector<Alloc> value)
    {
        m_variants = std::move(value);
        this->m_model.variants =  m_variants.empty() ? nullptr : m_variants.data();
        this->m_model.variantsCount =  static_cast<uint32_t>(m_variants.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.variables = m_variables.empty() ? nullptr : m_variables.data();
        this->m_model.variants = m_variants.empty() ? nullptr : m_variants.data();
    }

    ModelVector<PFVariableWrapper<Alloc>, Alloc> m_variables;
    CStringVector<Alloc> m_variants;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEntityLineageWrapper : public ModelWrapper<PFEntityLineage, Alloc>
{
public:
    using ModelType = PFEntityLineage;
    using DictionaryEntryType = PFEntityLineageDictionaryEntry;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEntityLineageWrapper() = default;

    PFEntityLineageWrapper(const PFEntityLineage& model) :
        ModelWrapper<PFEntityLineage, Alloc>{ model },
        m_characterId{ SafeString(model.characterId) },
        m_groupId{ SafeString(model.groupId) },
        m_masterPlayerAccountId{ SafeString(model.masterPlayerAccountId) },
        m_namespaceId{ SafeString(model.namespaceId) },
        m_titleId{ SafeString(model.titleId) },
        m_titlePlayerAccountId{ SafeString(model.titlePlayerAccountId) }
    {
        SetModelPointers();
    }

    PFEntityLineageWrapper(const PFEntityLineageWrapper& src) :
        PFEntityLineageWrapper{ src.Model() }
    {
    }

    PFEntityLineageWrapper(PFEntityLineageWrapper&& src) :
        PFEntityLineageWrapper{}
    {
        swap(*this, src);
    }

    PFEntityLineageWrapper& operator=(PFEntityLineageWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEntityLineageWrapper() = default;

    friend void swap(PFEntityLineageWrapper& lhs, PFEntityLineageWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_characterId, rhs.m_characterId);
        swap(lhs.m_groupId, rhs.m_groupId);
        swap(lhs.m_masterPlayerAccountId, rhs.m_masterPlayerAccountId);
        swap(lhs.m_namespaceId, rhs.m_namespaceId);
        swap(lhs.m_titleId, rhs.m_titleId);
        swap(lhs.m_titlePlayerAccountId, rhs.m_titlePlayerAccountId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCharacterId(String value)
    {
        m_characterId = std::move(value);
        this->m_model.characterId =  m_characterId.empty() ? nullptr : m_characterId.data();
    }

    void SetGroupId(String value)
    {
        m_groupId = std::move(value);
        this->m_model.groupId =  m_groupId.empty() ? nullptr : m_groupId.data();
    }

    void SetMasterPlayerAccountId(String value)
    {
        m_masterPlayerAccountId = std::move(value);
        this->m_model.masterPlayerAccountId =  m_masterPlayerAccountId.empty() ? nullptr : m_masterPlayerAccountId.data();
    }

    void SetNamespaceId(String value)
    {
        m_namespaceId = std::move(value);
        this->m_model.namespaceId =  m_namespaceId.empty() ? nullptr : m_namespaceId.data();
    }

    void SetTitleId(String value)
    {
        m_titleId = std::move(value);
        this->m_model.titleId =  m_titleId.empty() ? nullptr : m_titleId.data();
    }

    void SetTitlePlayerAccountId(String value)
    {
        m_titlePlayerAccountId = std::move(value);
        this->m_model.titlePlayerAccountId =  m_titlePlayerAccountId.empty() ? nullptr : m_titlePlayerAccountId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.characterId = m_characterId.empty() ? nullptr : m_characterId.data();
        this->m_model.groupId = m_groupId.empty() ? nullptr : m_groupId.data();
        this->m_model.masterPlayerAccountId = m_masterPlayerAccountId.empty() ? nullptr : m_masterPlayerAccountId.data();
        this->m_model.namespaceId = m_namespaceId.empty() ? nullptr : m_namespaceId.data();
        this->m_model.titleId = m_titleId.empty() ? nullptr : m_titleId.data();
        this->m_model.titlePlayerAccountId = m_titlePlayerAccountId.empty() ? nullptr : m_titlePlayerAccountId.data();
    }

    String m_characterId;
    String m_groupId;
    String m_masterPlayerAccountId;
    String m_namespaceId;
    String m_titleId;
    String m_titlePlayerAccountId;
};

} // namespace Wrappers
} // namespace PlayFab

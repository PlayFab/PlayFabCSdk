// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFPushNotificationsTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFPushNotificationsAdvancedPushPlatformMsgWrapper : public ModelWrapper<PFPushNotificationsAdvancedPushPlatformMsg, Alloc>
{
public:
    using ModelType = PFPushNotificationsAdvancedPushPlatformMsg;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPushNotificationsAdvancedPushPlatformMsgWrapper() = default;

    PFPushNotificationsAdvancedPushPlatformMsgWrapper(const PFPushNotificationsAdvancedPushPlatformMsg& model) :
        ModelWrapper<PFPushNotificationsAdvancedPushPlatformMsg, Alloc>{ model },
        m_gCMDataOnly{ model.gCMDataOnly ? std::optional<bool>{ *model.gCMDataOnly } : std::nullopt },
        m_json{ SafeString(model.json) }
    {
        SetModelPointers();
    }

    PFPushNotificationsAdvancedPushPlatformMsgWrapper(const PFPushNotificationsAdvancedPushPlatformMsgWrapper& src) :
        PFPushNotificationsAdvancedPushPlatformMsgWrapper{ src.Model() }
    {
    }

    PFPushNotificationsAdvancedPushPlatformMsgWrapper(PFPushNotificationsAdvancedPushPlatformMsgWrapper&& src) :
        PFPushNotificationsAdvancedPushPlatformMsgWrapper{}
    {
        swap(*this, src);
    }

    PFPushNotificationsAdvancedPushPlatformMsgWrapper& operator=(PFPushNotificationsAdvancedPushPlatformMsgWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPushNotificationsAdvancedPushPlatformMsgWrapper() = default;

    friend void swap(PFPushNotificationsAdvancedPushPlatformMsgWrapper& lhs, PFPushNotificationsAdvancedPushPlatformMsgWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_gCMDataOnly, rhs.m_gCMDataOnly);
        swap(lhs.m_json, rhs.m_json);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetGCMDataOnly(std::optional<bool> value)
    {
        m_gCMDataOnly = std::move(value);
        this->m_model.gCMDataOnly = m_gCMDataOnly ? m_gCMDataOnly.operator->() : nullptr;
    }

    void SetJson(String value)
    {
        m_json = std::move(value);
        this->m_model.json =  m_json.empty() ? nullptr : m_json.data();
    }

    void SetPlatform(PFPushNotificationPlatform value)
    {
        this->m_model.platform = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.gCMDataOnly = m_gCMDataOnly ? m_gCMDataOnly.operator->() : nullptr;
        this->m_model.json = m_json.empty() ? nullptr : m_json.data();
    }

    std::optional<bool> m_gCMDataOnly;
    String m_json;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPushNotificationsPushNotificationPackageWrapper : public ModelWrapper<PFPushNotificationsPushNotificationPackage, Alloc>
{
public:
    using ModelType = PFPushNotificationsPushNotificationPackage;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPushNotificationsPushNotificationPackageWrapper() = default;

    PFPushNotificationsPushNotificationPackageWrapper(const PFPushNotificationsPushNotificationPackage& model) :
        ModelWrapper<PFPushNotificationsPushNotificationPackage, Alloc>{ model },
        m_customData{ SafeString(model.customData) },
        m_icon{ SafeString(model.icon) },
        m_message{ SafeString(model.message) },
        m_sound{ SafeString(model.sound) },
        m_title{ SafeString(model.title) }
    {
        SetModelPointers();
    }

    PFPushNotificationsPushNotificationPackageWrapper(const PFPushNotificationsPushNotificationPackageWrapper& src) :
        PFPushNotificationsPushNotificationPackageWrapper{ src.Model() }
    {
    }

    PFPushNotificationsPushNotificationPackageWrapper(PFPushNotificationsPushNotificationPackageWrapper&& src) :
        PFPushNotificationsPushNotificationPackageWrapper{}
    {
        swap(*this, src);
    }

    PFPushNotificationsPushNotificationPackageWrapper& operator=(PFPushNotificationsPushNotificationPackageWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPushNotificationsPushNotificationPackageWrapper() = default;

    friend void swap(PFPushNotificationsPushNotificationPackageWrapper& lhs, PFPushNotificationsPushNotificationPackageWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customData, rhs.m_customData);
        swap(lhs.m_icon, rhs.m_icon);
        swap(lhs.m_message, rhs.m_message);
        swap(lhs.m_sound, rhs.m_sound);
        swap(lhs.m_title, rhs.m_title);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetBadge(int32_t value)
    {
        this->m_model.badge = value;
    }

    void SetCustomData(String value)
    {
        m_customData = std::move(value);
        this->m_model.customData =  m_customData.empty() ? nullptr : m_customData.data();
    }

    void SetIcon(String value)
    {
        m_icon = std::move(value);
        this->m_model.icon =  m_icon.empty() ? nullptr : m_icon.data();
    }

    void SetMessage(String value)
    {
        m_message = std::move(value);
        this->m_model.message =  m_message.empty() ? nullptr : m_message.data();
    }

    void SetSound(String value)
    {
        m_sound = std::move(value);
        this->m_model.sound =  m_sound.empty() ? nullptr : m_sound.data();
    }

    void SetTitle(String value)
    {
        m_title = std::move(value);
        this->m_model.title =  m_title.empty() ? nullptr : m_title.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customData = m_customData.empty() ? nullptr : m_customData.data();
        this->m_model.icon = m_icon.empty() ? nullptr : m_icon.data();
        this->m_model.message = m_message.empty() ? nullptr : m_message.data();
        this->m_model.sound = m_sound.empty() ? nullptr : m_sound.data();
        this->m_model.title = m_title.empty() ? nullptr : m_title.data();
    }

    String m_customData;
    String m_icon;
    String m_message;
    String m_sound;
    String m_title;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPushNotificationsSendPushNotificationRequestWrapper : public ModelWrapper<PFPushNotificationsSendPushNotificationRequest, Alloc>
{
public:
    using ModelType = PFPushNotificationsSendPushNotificationRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPushNotificationsSendPushNotificationRequestWrapper() = default;

    PFPushNotificationsSendPushNotificationRequestWrapper(const PFPushNotificationsSendPushNotificationRequest& model) :
        ModelWrapper<PFPushNotificationsSendPushNotificationRequest, Alloc>{ model },
        m_advancedPlatformDelivery{ model.advancedPlatformDelivery, model.advancedPlatformDelivery + model.advancedPlatformDeliveryCount },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_message{ SafeString(model.message) },
        m_package{ model.package ? std::optional<PFPushNotificationsPushNotificationPackageWrapper<Alloc>>{ *model.package } : std::nullopt },
        m_recipient{ SafeString(model.recipient) },
        m_subject{ SafeString(model.subject) },
        m_targetPlatforms{ model.targetPlatforms, model.targetPlatforms + model.targetPlatformsCount }
    {
        SetModelPointers();
    }

    PFPushNotificationsSendPushNotificationRequestWrapper(const PFPushNotificationsSendPushNotificationRequestWrapper& src) :
        PFPushNotificationsSendPushNotificationRequestWrapper{ src.Model() }
    {
    }

    PFPushNotificationsSendPushNotificationRequestWrapper(PFPushNotificationsSendPushNotificationRequestWrapper&& src) :
        PFPushNotificationsSendPushNotificationRequestWrapper{}
    {
        swap(*this, src);
    }

    PFPushNotificationsSendPushNotificationRequestWrapper& operator=(PFPushNotificationsSendPushNotificationRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPushNotificationsSendPushNotificationRequestWrapper() = default;

    friend void swap(PFPushNotificationsSendPushNotificationRequestWrapper& lhs, PFPushNotificationsSendPushNotificationRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_advancedPlatformDelivery, rhs.m_advancedPlatformDelivery);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_message, rhs.m_message);
        swap(lhs.m_package, rhs.m_package);
        swap(lhs.m_recipient, rhs.m_recipient);
        swap(lhs.m_subject, rhs.m_subject);
        swap(lhs.m_targetPlatforms, rhs.m_targetPlatforms);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAdvancedPlatformDelivery(ModelVector<PFPushNotificationsAdvancedPushPlatformMsgWrapper<Alloc>, Alloc> value)
    {
        m_advancedPlatformDelivery = std::move(value);
        this->m_model.advancedPlatformDelivery =  m_advancedPlatformDelivery.empty() ? nullptr : m_advancedPlatformDelivery.data();
        this->m_model.advancedPlatformDeliveryCount =  static_cast<uint32_t>(m_advancedPlatformDelivery.size());
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetMessage(String value)
    {
        m_message = std::move(value);
        this->m_model.message =  m_message.empty() ? nullptr : m_message.data();
    }

    void SetPackage(std::optional<PFPushNotificationsPushNotificationPackageWrapper<Alloc>> value)
    {
        m_package = std::move(value);
        this->m_model.package = m_package ? &m_package->Model() : nullptr;
    }

    void SetRecipient(String value)
    {
        m_recipient = std::move(value);
        this->m_model.recipient =  m_recipient.empty() ? nullptr : m_recipient.data();
    }

    void SetSubject(String value)
    {
        m_subject = std::move(value);
        this->m_model.subject =  m_subject.empty() ? nullptr : m_subject.data();
    }

    void SetTargetPlatforms(Vector<PFPushNotificationPlatform> value)
    {
        m_targetPlatforms = std::move(value);
        this->m_model.targetPlatforms =  m_targetPlatforms.empty() ? nullptr : m_targetPlatforms.data();
        this->m_model.targetPlatformsCount =  static_cast<uint32_t>(m_targetPlatforms.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.advancedPlatformDelivery = m_advancedPlatformDelivery.empty() ? nullptr : m_advancedPlatformDelivery.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.message = m_message.empty() ? nullptr : m_message.data();
        this->m_model.package = m_package ?  &m_package->Model() : nullptr;
        this->m_model.recipient = m_recipient.empty() ? nullptr : m_recipient.data();
        this->m_model.subject = m_subject.empty() ? nullptr : m_subject.data();
        this->m_model.targetPlatforms = m_targetPlatforms.empty() ? nullptr : m_targetPlatforms.data();
    }

    ModelVector<PFPushNotificationsAdvancedPushPlatformMsgWrapper<Alloc>, Alloc> m_advancedPlatformDelivery;
    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_message;
    std::optional<PFPushNotificationsPushNotificationPackageWrapper<Alloc>> m_package;
    String m_recipient;
    String m_subject;
    Vector<PFPushNotificationPlatform> m_targetPlatforms;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPushNotificationsSendPushNotificationFromTemplateRequestWrapper : public ModelWrapper<PFPushNotificationsSendPushNotificationFromTemplateRequest, Alloc>
{
public:
    using ModelType = PFPushNotificationsSendPushNotificationFromTemplateRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPushNotificationsSendPushNotificationFromTemplateRequestWrapper() = default;

    PFPushNotificationsSendPushNotificationFromTemplateRequestWrapper(const PFPushNotificationsSendPushNotificationFromTemplateRequest& model) :
        ModelWrapper<PFPushNotificationsSendPushNotificationFromTemplateRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_pushNotificationTemplateId{ SafeString(model.pushNotificationTemplateId) },
        m_recipient{ SafeString(model.recipient) }
    {
        SetModelPointers();
    }

    PFPushNotificationsSendPushNotificationFromTemplateRequestWrapper(const PFPushNotificationsSendPushNotificationFromTemplateRequestWrapper& src) :
        PFPushNotificationsSendPushNotificationFromTemplateRequestWrapper{ src.Model() }
    {
    }

    PFPushNotificationsSendPushNotificationFromTemplateRequestWrapper(PFPushNotificationsSendPushNotificationFromTemplateRequestWrapper&& src) :
        PFPushNotificationsSendPushNotificationFromTemplateRequestWrapper{}
    {
        swap(*this, src);
    }

    PFPushNotificationsSendPushNotificationFromTemplateRequestWrapper& operator=(PFPushNotificationsSendPushNotificationFromTemplateRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPushNotificationsSendPushNotificationFromTemplateRequestWrapper() = default;

    friend void swap(PFPushNotificationsSendPushNotificationFromTemplateRequestWrapper& lhs, PFPushNotificationsSendPushNotificationFromTemplateRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_pushNotificationTemplateId, rhs.m_pushNotificationTemplateId);
        swap(lhs.m_recipient, rhs.m_recipient);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetPushNotificationTemplateId(String value)
    {
        m_pushNotificationTemplateId = std::move(value);
        this->m_model.pushNotificationTemplateId =  m_pushNotificationTemplateId.empty() ? nullptr : m_pushNotificationTemplateId.data();
    }

    void SetRecipient(String value)
    {
        m_recipient = std::move(value);
        this->m_model.recipient =  m_recipient.empty() ? nullptr : m_recipient.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.pushNotificationTemplateId = m_pushNotificationTemplateId.empty() ? nullptr : m_pushNotificationTemplateId.data();
        this->m_model.recipient = m_recipient.empty() ? nullptr : m_recipient.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_pushNotificationTemplateId;
    String m_recipient;
};

} // namespace Wrappers
} // namespace PlayFab

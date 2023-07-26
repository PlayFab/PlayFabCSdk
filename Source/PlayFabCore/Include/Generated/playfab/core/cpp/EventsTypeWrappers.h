// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/cpp/TypeWrappers.h>
#include <playfab/core/PFEventsTypes.h>

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsCreateTelemetryKeyRequestWrapper : public ModelWrapper<PFEventsCreateTelemetryKeyRequest, Alloc>
{
public:
    using ModelType = PFEventsCreateTelemetryKeyRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsCreateTelemetryKeyRequestWrapper() = default;

    PFEventsCreateTelemetryKeyRequestWrapper(const PFEventsCreateTelemetryKeyRequest& model) :
        ModelWrapper<PFEventsCreateTelemetryKeyRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_keyName{ SafeString(model.keyName) }
    {
        SetModelPointers();
    }

    PFEventsCreateTelemetryKeyRequestWrapper(const PFEventsCreateTelemetryKeyRequestWrapper& src) :
        PFEventsCreateTelemetryKeyRequestWrapper{ src.Model() }
    {
    }

    PFEventsCreateTelemetryKeyRequestWrapper(PFEventsCreateTelemetryKeyRequestWrapper&& src) :
        PFEventsCreateTelemetryKeyRequestWrapper{}
    {
        swap(*this, src);
    }

    PFEventsCreateTelemetryKeyRequestWrapper& operator=(PFEventsCreateTelemetryKeyRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsCreateTelemetryKeyRequestWrapper() = default;

    friend void swap(PFEventsCreateTelemetryKeyRequestWrapper& lhs, PFEventsCreateTelemetryKeyRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_keyName, rhs.m_keyName);
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

    void SetKeyName(String value)
    {
        m_keyName = std::move(value);
        this->m_model.keyName =  m_keyName.empty() ? nullptr : m_keyName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.keyName = m_keyName.empty() ? nullptr : m_keyName.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_keyName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsTelemetryKeyDetailsWrapper : public ModelWrapper<PFEventsTelemetryKeyDetails, Alloc>
{
public:
    using ModelType = PFEventsTelemetryKeyDetails;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsTelemetryKeyDetailsWrapper() = default;

    PFEventsTelemetryKeyDetailsWrapper(const PFEventsTelemetryKeyDetails& model) :
        ModelWrapper<PFEventsTelemetryKeyDetails, Alloc>{ model },
        m_keyValue{ SafeString(model.keyValue) },
        m_name{ SafeString(model.name) }
    {
        SetModelPointers();
    }

    PFEventsTelemetryKeyDetailsWrapper(const PFEventsTelemetryKeyDetailsWrapper& src) :
        PFEventsTelemetryKeyDetailsWrapper{ src.Model() }
    {
    }

    PFEventsTelemetryKeyDetailsWrapper(PFEventsTelemetryKeyDetailsWrapper&& src) :
        PFEventsTelemetryKeyDetailsWrapper{}
    {
        swap(*this, src);
    }

    PFEventsTelemetryKeyDetailsWrapper& operator=(PFEventsTelemetryKeyDetailsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsTelemetryKeyDetailsWrapper() = default;

    friend void swap(PFEventsTelemetryKeyDetailsWrapper& lhs, PFEventsTelemetryKeyDetailsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_keyValue, rhs.m_keyValue);
        swap(lhs.m_name, rhs.m_name);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCreateTime(time_t value)
    {
        this->m_model.createTime = value;
    }

    void SetIsActive(bool value)
    {
        this->m_model.isActive = value;
    }

    void SetKeyValue(String value)
    {
        m_keyValue = std::move(value);
        this->m_model.keyValue =  m_keyValue.empty() ? nullptr : m_keyValue.data();
    }

    void SetLastUpdateTime(time_t value)
    {
        this->m_model.lastUpdateTime = value;
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.keyValue = m_keyValue.empty() ? nullptr : m_keyValue.data();
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
    }

    String m_keyValue;
    String m_name;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsCreateTelemetryKeyResponseWrapper : public ModelWrapper<PFEventsCreateTelemetryKeyResponse, Alloc>
{
public:
    using ModelType = PFEventsCreateTelemetryKeyResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsCreateTelemetryKeyResponseWrapper() = default;

    PFEventsCreateTelemetryKeyResponseWrapper(const PFEventsCreateTelemetryKeyResponse& model) :
        ModelWrapper<PFEventsCreateTelemetryKeyResponse, Alloc>{ model },
        m_newKeyDetails{ model.newKeyDetails ? std::optional<PFEventsTelemetryKeyDetailsWrapper<Alloc>>{ *model.newKeyDetails } : std::nullopt }
    {
        SetModelPointers();
    }

    PFEventsCreateTelemetryKeyResponseWrapper(const PFEventsCreateTelemetryKeyResponseWrapper& src) :
        PFEventsCreateTelemetryKeyResponseWrapper{ src.Model() }
    {
    }

    PFEventsCreateTelemetryKeyResponseWrapper(PFEventsCreateTelemetryKeyResponseWrapper&& src) :
        PFEventsCreateTelemetryKeyResponseWrapper{}
    {
        swap(*this, src);
    }

    PFEventsCreateTelemetryKeyResponseWrapper& operator=(PFEventsCreateTelemetryKeyResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsCreateTelemetryKeyResponseWrapper() = default;

    friend void swap(PFEventsCreateTelemetryKeyResponseWrapper& lhs, PFEventsCreateTelemetryKeyResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_newKeyDetails, rhs.m_newKeyDetails);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetNewKeyDetails(std::optional<PFEventsTelemetryKeyDetailsWrapper<Alloc>> value)
    {
        m_newKeyDetails = std::move(value);
        this->m_model.newKeyDetails = m_newKeyDetails ? &m_newKeyDetails->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.newKeyDetails = m_newKeyDetails ?  &m_newKeyDetails->Model() : nullptr;
    }

    std::optional<PFEventsTelemetryKeyDetailsWrapper<Alloc>> m_newKeyDetails;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsDeleteTelemetryKeyRequestWrapper : public ModelWrapper<PFEventsDeleteTelemetryKeyRequest, Alloc>
{
public:
    using ModelType = PFEventsDeleteTelemetryKeyRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsDeleteTelemetryKeyRequestWrapper() = default;

    PFEventsDeleteTelemetryKeyRequestWrapper(const PFEventsDeleteTelemetryKeyRequest& model) :
        ModelWrapper<PFEventsDeleteTelemetryKeyRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_keyName{ SafeString(model.keyName) }
    {
        SetModelPointers();
    }

    PFEventsDeleteTelemetryKeyRequestWrapper(const PFEventsDeleteTelemetryKeyRequestWrapper& src) :
        PFEventsDeleteTelemetryKeyRequestWrapper{ src.Model() }
    {
    }

    PFEventsDeleteTelemetryKeyRequestWrapper(PFEventsDeleteTelemetryKeyRequestWrapper&& src) :
        PFEventsDeleteTelemetryKeyRequestWrapper{}
    {
        swap(*this, src);
    }

    PFEventsDeleteTelemetryKeyRequestWrapper& operator=(PFEventsDeleteTelemetryKeyRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsDeleteTelemetryKeyRequestWrapper() = default;

    friend void swap(PFEventsDeleteTelemetryKeyRequestWrapper& lhs, PFEventsDeleteTelemetryKeyRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_keyName, rhs.m_keyName);
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

    void SetKeyName(String value)
    {
        m_keyName = std::move(value);
        this->m_model.keyName =  m_keyName.empty() ? nullptr : m_keyName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.keyName = m_keyName.empty() ? nullptr : m_keyName.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_keyName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsDeleteTelemetryKeyResponseWrapper : public ModelWrapper<PFEventsDeleteTelemetryKeyResponse, Alloc>
{
public:
    using ModelType = PFEventsDeleteTelemetryKeyResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    using ModelWrapper<PFEventsDeleteTelemetryKeyResponse, Alloc>::ModelWrapper;

    void SetWasKeyDeleted(bool value)
    {
        this->m_model.wasKeyDeleted = value;
    }

private:
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsGetTelemetryKeyRequestWrapper : public ModelWrapper<PFEventsGetTelemetryKeyRequest, Alloc>
{
public:
    using ModelType = PFEventsGetTelemetryKeyRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsGetTelemetryKeyRequestWrapper() = default;

    PFEventsGetTelemetryKeyRequestWrapper(const PFEventsGetTelemetryKeyRequest& model) :
        ModelWrapper<PFEventsGetTelemetryKeyRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_keyName{ SafeString(model.keyName) }
    {
        SetModelPointers();
    }

    PFEventsGetTelemetryKeyRequestWrapper(const PFEventsGetTelemetryKeyRequestWrapper& src) :
        PFEventsGetTelemetryKeyRequestWrapper{ src.Model() }
    {
    }

    PFEventsGetTelemetryKeyRequestWrapper(PFEventsGetTelemetryKeyRequestWrapper&& src) :
        PFEventsGetTelemetryKeyRequestWrapper{}
    {
        swap(*this, src);
    }

    PFEventsGetTelemetryKeyRequestWrapper& operator=(PFEventsGetTelemetryKeyRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsGetTelemetryKeyRequestWrapper() = default;

    friend void swap(PFEventsGetTelemetryKeyRequestWrapper& lhs, PFEventsGetTelemetryKeyRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_keyName, rhs.m_keyName);
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

    void SetKeyName(String value)
    {
        m_keyName = std::move(value);
        this->m_model.keyName =  m_keyName.empty() ? nullptr : m_keyName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.keyName = m_keyName.empty() ? nullptr : m_keyName.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_keyName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsGetTelemetryKeyResponseWrapper : public ModelWrapper<PFEventsGetTelemetryKeyResponse, Alloc>
{
public:
    using ModelType = PFEventsGetTelemetryKeyResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsGetTelemetryKeyResponseWrapper() = default;

    PFEventsGetTelemetryKeyResponseWrapper(const PFEventsGetTelemetryKeyResponse& model) :
        ModelWrapper<PFEventsGetTelemetryKeyResponse, Alloc>{ model },
        m_keyDetails{ model.keyDetails ? std::optional<PFEventsTelemetryKeyDetailsWrapper<Alloc>>{ *model.keyDetails } : std::nullopt }
    {
        SetModelPointers();
    }

    PFEventsGetTelemetryKeyResponseWrapper(const PFEventsGetTelemetryKeyResponseWrapper& src) :
        PFEventsGetTelemetryKeyResponseWrapper{ src.Model() }
    {
    }

    PFEventsGetTelemetryKeyResponseWrapper(PFEventsGetTelemetryKeyResponseWrapper&& src) :
        PFEventsGetTelemetryKeyResponseWrapper{}
    {
        swap(*this, src);
    }

    PFEventsGetTelemetryKeyResponseWrapper& operator=(PFEventsGetTelemetryKeyResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsGetTelemetryKeyResponseWrapper() = default;

    friend void swap(PFEventsGetTelemetryKeyResponseWrapper& lhs, PFEventsGetTelemetryKeyResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_keyDetails, rhs.m_keyDetails);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetKeyDetails(std::optional<PFEventsTelemetryKeyDetailsWrapper<Alloc>> value)
    {
        m_keyDetails = std::move(value);
        this->m_model.keyDetails = m_keyDetails ? &m_keyDetails->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.keyDetails = m_keyDetails ?  &m_keyDetails->Model() : nullptr;
    }

    std::optional<PFEventsTelemetryKeyDetailsWrapper<Alloc>> m_keyDetails;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsListTelemetryKeysRequestWrapper : public ModelWrapper<PFEventsListTelemetryKeysRequest, Alloc>
{
public:
    using ModelType = PFEventsListTelemetryKeysRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsListTelemetryKeysRequestWrapper() = default;

    PFEventsListTelemetryKeysRequestWrapper(const PFEventsListTelemetryKeysRequest& model) :
        ModelWrapper<PFEventsListTelemetryKeysRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt }
    {
        SetModelPointers();
    }

    PFEventsListTelemetryKeysRequestWrapper(const PFEventsListTelemetryKeysRequestWrapper& src) :
        PFEventsListTelemetryKeysRequestWrapper{ src.Model() }
    {
    }

    PFEventsListTelemetryKeysRequestWrapper(PFEventsListTelemetryKeysRequestWrapper&& src) :
        PFEventsListTelemetryKeysRequestWrapper{}
    {
        swap(*this, src);
    }

    PFEventsListTelemetryKeysRequestWrapper& operator=(PFEventsListTelemetryKeysRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsListTelemetryKeysRequestWrapper() = default;

    friend void swap(PFEventsListTelemetryKeysRequestWrapper& lhs, PFEventsListTelemetryKeysRequestWrapper& rhs)
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
class PFEventsListTelemetryKeysResponseWrapper : public ModelWrapper<PFEventsListTelemetryKeysResponse, Alloc>
{
public:
    using ModelType = PFEventsListTelemetryKeysResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsListTelemetryKeysResponseWrapper() = default;

    PFEventsListTelemetryKeysResponseWrapper(const PFEventsListTelemetryKeysResponse& model) :
        ModelWrapper<PFEventsListTelemetryKeysResponse, Alloc>{ model },
        m_keyDetails{ model.keyDetails, model.keyDetails + model.keyDetailsCount }
    {
        SetModelPointers();
    }

    PFEventsListTelemetryKeysResponseWrapper(const PFEventsListTelemetryKeysResponseWrapper& src) :
        PFEventsListTelemetryKeysResponseWrapper{ src.Model() }
    {
    }

    PFEventsListTelemetryKeysResponseWrapper(PFEventsListTelemetryKeysResponseWrapper&& src) :
        PFEventsListTelemetryKeysResponseWrapper{}
    {
        swap(*this, src);
    }

    PFEventsListTelemetryKeysResponseWrapper& operator=(PFEventsListTelemetryKeysResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsListTelemetryKeysResponseWrapper() = default;

    friend void swap(PFEventsListTelemetryKeysResponseWrapper& lhs, PFEventsListTelemetryKeysResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_keyDetails, rhs.m_keyDetails);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetKeyDetails(ModelVector<PFEventsTelemetryKeyDetailsWrapper<Alloc>, Alloc> value)
    {
        m_keyDetails = std::move(value);
        this->m_model.keyDetails =  m_keyDetails.empty() ? nullptr : m_keyDetails.data();
        this->m_model.keyDetailsCount =  static_cast<uint32_t>(m_keyDetails.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.keyDetails = m_keyDetails.empty() ? nullptr : m_keyDetails.data();
    }

    ModelVector<PFEventsTelemetryKeyDetailsWrapper<Alloc>, Alloc> m_keyDetails;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsSetTelemetryKeyActiveRequestWrapper : public ModelWrapper<PFEventsSetTelemetryKeyActiveRequest, Alloc>
{
public:
    using ModelType = PFEventsSetTelemetryKeyActiveRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsSetTelemetryKeyActiveRequestWrapper() = default;

    PFEventsSetTelemetryKeyActiveRequestWrapper(const PFEventsSetTelemetryKeyActiveRequest& model) :
        ModelWrapper<PFEventsSetTelemetryKeyActiveRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_keyName{ SafeString(model.keyName) }
    {
        SetModelPointers();
    }

    PFEventsSetTelemetryKeyActiveRequestWrapper(const PFEventsSetTelemetryKeyActiveRequestWrapper& src) :
        PFEventsSetTelemetryKeyActiveRequestWrapper{ src.Model() }
    {
    }

    PFEventsSetTelemetryKeyActiveRequestWrapper(PFEventsSetTelemetryKeyActiveRequestWrapper&& src) :
        PFEventsSetTelemetryKeyActiveRequestWrapper{}
    {
        swap(*this, src);
    }

    PFEventsSetTelemetryKeyActiveRequestWrapper& operator=(PFEventsSetTelemetryKeyActiveRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsSetTelemetryKeyActiveRequestWrapper() = default;

    friend void swap(PFEventsSetTelemetryKeyActiveRequestWrapper& lhs, PFEventsSetTelemetryKeyActiveRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_keyName, rhs.m_keyName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetActive(bool value)
    {
        this->m_model.active = value;
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

    void SetKeyName(String value)
    {
        m_keyName = std::move(value);
        this->m_model.keyName =  m_keyName.empty() ? nullptr : m_keyName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.keyName = m_keyName.empty() ? nullptr : m_keyName.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_keyName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsSetTelemetryKeyActiveResponseWrapper : public ModelWrapper<PFEventsSetTelemetryKeyActiveResponse, Alloc>
{
public:
    using ModelType = PFEventsSetTelemetryKeyActiveResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsSetTelemetryKeyActiveResponseWrapper() = default;

    PFEventsSetTelemetryKeyActiveResponseWrapper(const PFEventsSetTelemetryKeyActiveResponse& model) :
        ModelWrapper<PFEventsSetTelemetryKeyActiveResponse, Alloc>{ model },
        m_keyDetails{ model.keyDetails ? std::optional<PFEventsTelemetryKeyDetailsWrapper<Alloc>>{ *model.keyDetails } : std::nullopt }
    {
        SetModelPointers();
    }

    PFEventsSetTelemetryKeyActiveResponseWrapper(const PFEventsSetTelemetryKeyActiveResponseWrapper& src) :
        PFEventsSetTelemetryKeyActiveResponseWrapper{ src.Model() }
    {
    }

    PFEventsSetTelemetryKeyActiveResponseWrapper(PFEventsSetTelemetryKeyActiveResponseWrapper&& src) :
        PFEventsSetTelemetryKeyActiveResponseWrapper{}
    {
        swap(*this, src);
    }

    PFEventsSetTelemetryKeyActiveResponseWrapper& operator=(PFEventsSetTelemetryKeyActiveResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsSetTelemetryKeyActiveResponseWrapper() = default;

    friend void swap(PFEventsSetTelemetryKeyActiveResponseWrapper& lhs, PFEventsSetTelemetryKeyActiveResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_keyDetails, rhs.m_keyDetails);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetKeyDetails(std::optional<PFEventsTelemetryKeyDetailsWrapper<Alloc>> value)
    {
        m_keyDetails = std::move(value);
        this->m_model.keyDetails = m_keyDetails ? &m_keyDetails->Model() : nullptr;
    }

    void SetWasKeyUpdated(bool value)
    {
        this->m_model.wasKeyUpdated = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.keyDetails = m_keyDetails ?  &m_keyDetails->Model() : nullptr;
    }

    std::optional<PFEventsTelemetryKeyDetailsWrapper<Alloc>> m_keyDetails;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsEventContentsWrapper : public ModelWrapper<PFEventsEventContents, Alloc>
{
public:
    using ModelType = PFEventsEventContents;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsEventContentsWrapper() = default;

    PFEventsEventContentsWrapper(const PFEventsEventContents& model) :
        ModelWrapper<PFEventsEventContents, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_eventNamespace{ SafeString(model.eventNamespace) },
        m_name{ SafeString(model.name) },
        m_originalId{ SafeString(model.originalId) },
        m_originalTimestamp{ model.originalTimestamp ? std::optional<time_t>{ *model.originalTimestamp } : std::nullopt },
        m_payload{ model.payload },
        m_payloadJSON{ SafeString(model.payloadJSON) }
    {
        SetModelPointers();
    }

    PFEventsEventContentsWrapper(const PFEventsEventContentsWrapper& src) :
        PFEventsEventContentsWrapper{ src.Model() }
    {
    }

    PFEventsEventContentsWrapper(PFEventsEventContentsWrapper&& src) :
        PFEventsEventContentsWrapper{}
    {
        swap(*this, src);
    }

    PFEventsEventContentsWrapper& operator=(PFEventsEventContentsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsEventContentsWrapper() = default;

    friend void swap(PFEventsEventContentsWrapper& lhs, PFEventsEventContentsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_eventNamespace, rhs.m_eventNamespace);
        swap(lhs.m_name, rhs.m_name);
        swap(lhs.m_originalId, rhs.m_originalId);
        swap(lhs.m_originalTimestamp, rhs.m_originalTimestamp);
        swap(lhs.m_payload, rhs.m_payload);
        swap(lhs.m_payloadJSON, rhs.m_payloadJSON);
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

    void SetEventNamespace(String value)
    {
        m_eventNamespace = std::move(value);
        this->m_model.eventNamespace =  m_eventNamespace.empty() ? nullptr : m_eventNamespace.data();
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

    void SetOriginalId(String value)
    {
        m_originalId = std::move(value);
        this->m_model.originalId =  m_originalId.empty() ? nullptr : m_originalId.data();
    }

    void SetOriginalTimestamp(std::optional<time_t> value)
    {
        m_originalTimestamp = std::move(value);
        this->m_model.originalTimestamp = m_originalTimestamp ? m_originalTimestamp.operator->() : nullptr;
    }

    void SetPayload(JsonObject<Alloc> value)
    {
        m_payload = std::move(value);
        this->m_model.payload.stringValue = m_payload.stringValue.empty() ? nullptr : m_payload.stringValue.data();
    }

    void SetPayloadJSON(String value)
    {
        m_payloadJSON = std::move(value);
        this->m_model.payloadJSON =  m_payloadJSON.empty() ? nullptr : m_payloadJSON.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.eventNamespace = m_eventNamespace.empty() ? nullptr : m_eventNamespace.data();
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
        this->m_model.originalId = m_originalId.empty() ? nullptr : m_originalId.data();
        this->m_model.originalTimestamp = m_originalTimestamp ? m_originalTimestamp.operator->() : nullptr;
        this->m_model.payload.stringValue = m_payload.stringValue.empty() ? nullptr : m_payload.stringValue.data();
        this->m_model.payloadJSON = m_payloadJSON.empty() ? nullptr : m_payloadJSON.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_eventNamespace;
    String m_name;
    String m_originalId;
    std::optional<time_t> m_originalTimestamp;
    JsonObject<Alloc> m_payload;
    String m_payloadJSON;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsWriteEventsRequestWrapper : public ModelWrapper<PFEventsWriteEventsRequest, Alloc>
{
public:
    using ModelType = PFEventsWriteEventsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsWriteEventsRequestWrapper() = default;

    PFEventsWriteEventsRequestWrapper(const PFEventsWriteEventsRequest& model) :
        ModelWrapper<PFEventsWriteEventsRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_events{ model.events, model.events + model.eventsCount }
    {
        SetModelPointers();
    }

    PFEventsWriteEventsRequestWrapper(const PFEventsWriteEventsRequestWrapper& src) :
        PFEventsWriteEventsRequestWrapper{ src.Model() }
    {
    }

    PFEventsWriteEventsRequestWrapper(PFEventsWriteEventsRequestWrapper&& src) :
        PFEventsWriteEventsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFEventsWriteEventsRequestWrapper& operator=(PFEventsWriteEventsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsWriteEventsRequestWrapper() = default;

    friend void swap(PFEventsWriteEventsRequestWrapper& lhs, PFEventsWriteEventsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_events, rhs.m_events);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEvents(ModelVector<PFEventsEventContentsWrapper<Alloc>, Alloc> value)
    {
        m_events = std::move(value);
        this->m_model.events =  m_events.empty() ? nullptr : m_events.data();
        this->m_model.eventsCount =  static_cast<uint32_t>(m_events.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.events = m_events.empty() ? nullptr : m_events.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    ModelVector<PFEventsEventContentsWrapper<Alloc>, Alloc> m_events;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFEventsWriteEventsResponseWrapper : public ModelWrapper<PFEventsWriteEventsResponse, Alloc>
{
public:
    using ModelType = PFEventsWriteEventsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFEventsWriteEventsResponseWrapper() = default;

    PFEventsWriteEventsResponseWrapper(const PFEventsWriteEventsResponse& model) :
        ModelWrapper<PFEventsWriteEventsResponse, Alloc>{ model },
        m_assignedEventIds{ model.assignedEventIds, model.assignedEventIds + model.assignedEventIdsCount }
    {
        SetModelPointers();
    }

    PFEventsWriteEventsResponseWrapper(const PFEventsWriteEventsResponseWrapper& src) :
        PFEventsWriteEventsResponseWrapper{ src.Model() }
    {
    }

    PFEventsWriteEventsResponseWrapper(PFEventsWriteEventsResponseWrapper&& src) :
        PFEventsWriteEventsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFEventsWriteEventsResponseWrapper& operator=(PFEventsWriteEventsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFEventsWriteEventsResponseWrapper() = default;

    friend void swap(PFEventsWriteEventsResponseWrapper& lhs, PFEventsWriteEventsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_assignedEventIds, rhs.m_assignedEventIds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAssignedEventIds(CStringVector<Alloc> value)
    {
        m_assignedEventIds = std::move(value);
        this->m_model.assignedEventIds =  m_assignedEventIds.empty() ? nullptr : m_assignedEventIds.data();
        this->m_model.assignedEventIdsCount =  static_cast<uint32_t>(m_assignedEventIds.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.assignedEventIds = m_assignedEventIds.empty() ? nullptr : m_assignedEventIds.data();
    }

    CStringVector<Alloc> m_assignedEventIds;
};

} // namespace Wrappers
} // namespace PlayFab

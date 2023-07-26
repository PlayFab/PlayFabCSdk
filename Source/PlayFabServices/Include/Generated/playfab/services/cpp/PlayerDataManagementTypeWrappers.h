// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFPlayerDataManagementTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayerDataManagementGetUserDataRequestWrapper : public ModelWrapper<PFPlayerDataManagementGetUserDataRequest, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementGetUserDataRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerDataManagementGetUserDataRequestWrapper() = default;

    PFPlayerDataManagementGetUserDataRequestWrapper(const PFPlayerDataManagementGetUserDataRequest& model) :
        ModelWrapper<PFPlayerDataManagementGetUserDataRequest, Alloc>{ model },
        m_ifChangedFromDataVersion{ model.ifChangedFromDataVersion ? std::optional<uint32_t>{ *model.ifChangedFromDataVersion } : std::nullopt },
        m_keys{ model.keys, model.keys + model.keysCount },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFPlayerDataManagementGetUserDataRequestWrapper(const PFPlayerDataManagementGetUserDataRequestWrapper& src) :
        PFPlayerDataManagementGetUserDataRequestWrapper{ src.Model() }
    {
    }

    PFPlayerDataManagementGetUserDataRequestWrapper(PFPlayerDataManagementGetUserDataRequestWrapper&& src) :
        PFPlayerDataManagementGetUserDataRequestWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerDataManagementGetUserDataRequestWrapper& operator=(PFPlayerDataManagementGetUserDataRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerDataManagementGetUserDataRequestWrapper() = default;

    friend void swap(PFPlayerDataManagementGetUserDataRequestWrapper& lhs, PFPlayerDataManagementGetUserDataRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_ifChangedFromDataVersion, rhs.m_ifChangedFromDataVersion);
        swap(lhs.m_keys, rhs.m_keys);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetIfChangedFromDataVersion(std::optional<uint32_t> value)
    {
        m_ifChangedFromDataVersion = std::move(value);
        this->m_model.ifChangedFromDataVersion = m_ifChangedFromDataVersion ? m_ifChangedFromDataVersion.operator->() : nullptr;
    }

    void SetKeys(CStringVector<Alloc> value)
    {
        m_keys = std::move(value);
        this->m_model.keys =  m_keys.empty() ? nullptr : m_keys.data();
        this->m_model.keysCount =  static_cast<uint32_t>(m_keys.size());
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.ifChangedFromDataVersion = m_ifChangedFromDataVersion ? m_ifChangedFromDataVersion.operator->() : nullptr;
        this->m_model.keys = m_keys.empty() ? nullptr : m_keys.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    std::optional<uint32_t> m_ifChangedFromDataVersion;
    CStringVector<Alloc> m_keys;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayerDataManagementClientGetUserDataResultWrapper : public ModelWrapper<PFPlayerDataManagementClientGetUserDataResult, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementClientGetUserDataResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerDataManagementClientGetUserDataResultWrapper() = default;

    PFPlayerDataManagementClientGetUserDataResultWrapper(const PFPlayerDataManagementClientGetUserDataResult& model) :
        ModelWrapper<PFPlayerDataManagementClientGetUserDataResult, Alloc>{ model },
        m_data{ model.data, model.data + model.dataCount }
    {
        SetModelPointers();
    }

    PFPlayerDataManagementClientGetUserDataResultWrapper(const PFPlayerDataManagementClientGetUserDataResultWrapper& src) :
        PFPlayerDataManagementClientGetUserDataResultWrapper{ src.Model() }
    {
    }

    PFPlayerDataManagementClientGetUserDataResultWrapper(PFPlayerDataManagementClientGetUserDataResultWrapper&& src) :
        PFPlayerDataManagementClientGetUserDataResultWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerDataManagementClientGetUserDataResultWrapper& operator=(PFPlayerDataManagementClientGetUserDataResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerDataManagementClientGetUserDataResultWrapper() = default;

    friend void swap(PFPlayerDataManagementClientGetUserDataResultWrapper& lhs, PFPlayerDataManagementClientGetUserDataResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_data, rhs.m_data);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetData(ModelDictionaryEntryVector<PFUserDataRecordWrapper<Alloc>, Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

    void SetDataVersion(uint32_t value)
    {
        this->m_model.dataVersion = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
    }

    ModelDictionaryEntryVector<PFUserDataRecordWrapper<Alloc>, Alloc> m_data;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayerDataManagementClientUpdateUserDataRequestWrapper : public ModelWrapper<PFPlayerDataManagementClientUpdateUserDataRequest, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementClientUpdateUserDataRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerDataManagementClientUpdateUserDataRequestWrapper() = default;

    PFPlayerDataManagementClientUpdateUserDataRequestWrapper(const PFPlayerDataManagementClientUpdateUserDataRequest& model) :
        ModelWrapper<PFPlayerDataManagementClientUpdateUserDataRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_data{ model.data, model.data + model.dataCount },
        m_keysToRemove{ model.keysToRemove, model.keysToRemove + model.keysToRemoveCount },
        m_permission{ model.permission ? std::optional<PFUserDataPermission>{ *model.permission } : std::nullopt }
    {
        SetModelPointers();
    }

    PFPlayerDataManagementClientUpdateUserDataRequestWrapper(const PFPlayerDataManagementClientUpdateUserDataRequestWrapper& src) :
        PFPlayerDataManagementClientUpdateUserDataRequestWrapper{ src.Model() }
    {
    }

    PFPlayerDataManagementClientUpdateUserDataRequestWrapper(PFPlayerDataManagementClientUpdateUserDataRequestWrapper&& src) :
        PFPlayerDataManagementClientUpdateUserDataRequestWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerDataManagementClientUpdateUserDataRequestWrapper& operator=(PFPlayerDataManagementClientUpdateUserDataRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerDataManagementClientUpdateUserDataRequestWrapper() = default;

    friend void swap(PFPlayerDataManagementClientUpdateUserDataRequestWrapper& lhs, PFPlayerDataManagementClientUpdateUserDataRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_data, rhs.m_data);
        swap(lhs.m_keysToRemove, rhs.m_keysToRemove);
        swap(lhs.m_permission, rhs.m_permission);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetData(StringDictionaryEntryVector<Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

    void SetKeysToRemove(CStringVector<Alloc> value)
    {
        m_keysToRemove = std::move(value);
        this->m_model.keysToRemove =  m_keysToRemove.empty() ? nullptr : m_keysToRemove.data();
        this->m_model.keysToRemoveCount =  static_cast<uint32_t>(m_keysToRemove.size());
    }

    void SetPermission(std::optional<PFUserDataPermission> value)
    {
        m_permission = std::move(value);
        this->m_model.permission = m_permission ? m_permission.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
        this->m_model.keysToRemove = m_keysToRemove.empty() ? nullptr : m_keysToRemove.data();
        this->m_model.permission = m_permission ? m_permission.operator->() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    StringDictionaryEntryVector<Alloc> m_data;
    CStringVector<Alloc> m_keysToRemove;
    std::optional<PFUserDataPermission> m_permission;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayerDataManagementUpdateUserDataResultWrapper : public ModelWrapper<PFPlayerDataManagementUpdateUserDataResult, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementUpdateUserDataResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    using ModelWrapper<PFPlayerDataManagementUpdateUserDataResult, Alloc>::ModelWrapper;

    void SetDataVersion(uint32_t value)
    {
        this->m_model.dataVersion = value;
    }

private:
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayerDataManagementServerGetUserDataResultWrapper : public ModelWrapper<PFPlayerDataManagementServerGetUserDataResult, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementServerGetUserDataResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerDataManagementServerGetUserDataResultWrapper() = default;

    PFPlayerDataManagementServerGetUserDataResultWrapper(const PFPlayerDataManagementServerGetUserDataResult& model) :
        ModelWrapper<PFPlayerDataManagementServerGetUserDataResult, Alloc>{ model },
        m_data{ model.data, model.data + model.dataCount },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFPlayerDataManagementServerGetUserDataResultWrapper(const PFPlayerDataManagementServerGetUserDataResultWrapper& src) :
        PFPlayerDataManagementServerGetUserDataResultWrapper{ src.Model() }
    {
    }

    PFPlayerDataManagementServerGetUserDataResultWrapper(PFPlayerDataManagementServerGetUserDataResultWrapper&& src) :
        PFPlayerDataManagementServerGetUserDataResultWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerDataManagementServerGetUserDataResultWrapper& operator=(PFPlayerDataManagementServerGetUserDataResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerDataManagementServerGetUserDataResultWrapper() = default;

    friend void swap(PFPlayerDataManagementServerGetUserDataResultWrapper& lhs, PFPlayerDataManagementServerGetUserDataResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_data, rhs.m_data);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetData(ModelDictionaryEntryVector<PFUserDataRecordWrapper<Alloc>, Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

    void SetDataVersion(uint32_t value)
    {
        this->m_model.dataVersion = value;
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    ModelDictionaryEntryVector<PFUserDataRecordWrapper<Alloc>, Alloc> m_data;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayerDataManagementServerUpdateUserDataRequestWrapper : public ModelWrapper<PFPlayerDataManagementServerUpdateUserDataRequest, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementServerUpdateUserDataRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerDataManagementServerUpdateUserDataRequestWrapper() = default;

    PFPlayerDataManagementServerUpdateUserDataRequestWrapper(const PFPlayerDataManagementServerUpdateUserDataRequest& model) :
        ModelWrapper<PFPlayerDataManagementServerUpdateUserDataRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_data{ model.data, model.data + model.dataCount },
        m_keysToRemove{ model.keysToRemove, model.keysToRemove + model.keysToRemoveCount },
        m_permission{ model.permission ? std::optional<PFUserDataPermission>{ *model.permission } : std::nullopt },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFPlayerDataManagementServerUpdateUserDataRequestWrapper(const PFPlayerDataManagementServerUpdateUserDataRequestWrapper& src) :
        PFPlayerDataManagementServerUpdateUserDataRequestWrapper{ src.Model() }
    {
    }

    PFPlayerDataManagementServerUpdateUserDataRequestWrapper(PFPlayerDataManagementServerUpdateUserDataRequestWrapper&& src) :
        PFPlayerDataManagementServerUpdateUserDataRequestWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerDataManagementServerUpdateUserDataRequestWrapper& operator=(PFPlayerDataManagementServerUpdateUserDataRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerDataManagementServerUpdateUserDataRequestWrapper() = default;

    friend void swap(PFPlayerDataManagementServerUpdateUserDataRequestWrapper& lhs, PFPlayerDataManagementServerUpdateUserDataRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_data, rhs.m_data);
        swap(lhs.m_keysToRemove, rhs.m_keysToRemove);
        swap(lhs.m_permission, rhs.m_permission);
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

    void SetData(StringDictionaryEntryVector<Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

    void SetKeysToRemove(CStringVector<Alloc> value)
    {
        m_keysToRemove = std::move(value);
        this->m_model.keysToRemove =  m_keysToRemove.empty() ? nullptr : m_keysToRemove.data();
        this->m_model.keysToRemoveCount =  static_cast<uint32_t>(m_keysToRemove.size());
    }

    void SetPermission(std::optional<PFUserDataPermission> value)
    {
        m_permission = std::move(value);
        this->m_model.permission = m_permission ? m_permission.operator->() : nullptr;
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
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
        this->m_model.keysToRemove = m_keysToRemove.empty() ? nullptr : m_keysToRemove.data();
        this->m_model.permission = m_permission ? m_permission.operator->() : nullptr;
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    StringDictionaryEntryVector<Alloc> m_data;
    CStringVector<Alloc> m_keysToRemove;
    std::optional<PFUserDataPermission> m_permission;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayerDataManagementUpdateUserInternalDataRequestWrapper : public ModelWrapper<PFPlayerDataManagementUpdateUserInternalDataRequest, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementUpdateUserInternalDataRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerDataManagementUpdateUserInternalDataRequestWrapper() = default;

    PFPlayerDataManagementUpdateUserInternalDataRequestWrapper(const PFPlayerDataManagementUpdateUserInternalDataRequest& model) :
        ModelWrapper<PFPlayerDataManagementUpdateUserInternalDataRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_data{ model.data, model.data + model.dataCount },
        m_keysToRemove{ model.keysToRemove, model.keysToRemove + model.keysToRemoveCount },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFPlayerDataManagementUpdateUserInternalDataRequestWrapper(const PFPlayerDataManagementUpdateUserInternalDataRequestWrapper& src) :
        PFPlayerDataManagementUpdateUserInternalDataRequestWrapper{ src.Model() }
    {
    }

    PFPlayerDataManagementUpdateUserInternalDataRequestWrapper(PFPlayerDataManagementUpdateUserInternalDataRequestWrapper&& src) :
        PFPlayerDataManagementUpdateUserInternalDataRequestWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerDataManagementUpdateUserInternalDataRequestWrapper& operator=(PFPlayerDataManagementUpdateUserInternalDataRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerDataManagementUpdateUserInternalDataRequestWrapper() = default;

    friend void swap(PFPlayerDataManagementUpdateUserInternalDataRequestWrapper& lhs, PFPlayerDataManagementUpdateUserInternalDataRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_data, rhs.m_data);
        swap(lhs.m_keysToRemove, rhs.m_keysToRemove);
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

    void SetData(StringDictionaryEntryVector<Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

    void SetKeysToRemove(CStringVector<Alloc> value)
    {
        m_keysToRemove = std::move(value);
        this->m_model.keysToRemove =  m_keysToRemove.empty() ? nullptr : m_keysToRemove.data();
        this->m_model.keysToRemoveCount =  static_cast<uint32_t>(m_keysToRemove.size());
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
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
        this->m_model.keysToRemove = m_keysToRemove.empty() ? nullptr : m_keysToRemove.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    StringDictionaryEntryVector<Alloc> m_data;
    CStringVector<Alloc> m_keysToRemove;
    String m_playFabId;
};

} // namespace Wrappers
} // namespace PlayFab

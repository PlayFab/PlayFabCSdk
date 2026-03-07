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
class PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequestWrapper : public ModelWrapper<PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequest, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequestWrapper() = default;

    PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequestWrapper(const PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequest& model) :
        ModelWrapper<PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_expectedPropertiesVersion{ model.expectedPropertiesVersion ? std::optional<int32_t>{ *model.expectedPropertiesVersion } : std::nullopt },
        m_propertyNames{ model.propertyNames, model.propertyNames + model.propertyNamesCount }
    {
        SetModelPointers();
    }

    PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequestWrapper(const PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequestWrapper& src) :
        PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequestWrapper{ src.Model() }
    {
    }

    PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequestWrapper(PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequestWrapper&& src) :
        PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequestWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequestWrapper& operator=(PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequestWrapper() = default;

    friend void swap(PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequestWrapper& lhs, PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_expectedPropertiesVersion, rhs.m_expectedPropertiesVersion);
        swap(lhs.m_propertyNames, rhs.m_propertyNames);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    StringDictionaryEntryVector<Alloc> const& GetCustomTags() const
    {
        return m_customTags;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    std::optional<int32_t> const& GetExpectedPropertiesVersion() const
    {
        return m_expectedPropertiesVersion;
    }

    void SetExpectedPropertiesVersion(std::optional<int32_t> value)
    {
        m_expectedPropertiesVersion = std::move(value);
        this->m_model.expectedPropertiesVersion = m_expectedPropertiesVersion ? m_expectedPropertiesVersion.operator->() : nullptr;
    }

    CStringVector<Alloc> const& GetPropertyNames() const
    {
        return m_propertyNames;
    }

    void SetPropertyNames(CStringVector<Alloc> value)
    {
        m_propertyNames = std::move(value);
        this->m_model.propertyNames =  m_propertyNames.empty() ? nullptr : m_propertyNames.data();
        this->m_model.propertyNamesCount =  static_cast<uint32_t>(m_propertyNames.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.expectedPropertiesVersion = m_expectedPropertiesVersion ? m_expectedPropertiesVersion.operator->() : nullptr;
        this->m_model.propertyNames = m_propertyNames.empty() ? nullptr : m_propertyNames.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<int32_t> m_expectedPropertiesVersion;
    CStringVector<Alloc> m_propertyNames;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayerDataManagementDeletedPropertyDetailsWrapper : public ModelWrapper<PFPlayerDataManagementDeletedPropertyDetails, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementDeletedPropertyDetails;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerDataManagementDeletedPropertyDetailsWrapper() = default;

    PFPlayerDataManagementDeletedPropertyDetailsWrapper(const PFPlayerDataManagementDeletedPropertyDetails& model) :
        ModelWrapper<PFPlayerDataManagementDeletedPropertyDetails, Alloc>{ model },
        m_name{ SafeString(model.name) }
    {
        SetModelPointers();
    }

    PFPlayerDataManagementDeletedPropertyDetailsWrapper(const PFPlayerDataManagementDeletedPropertyDetailsWrapper& src) :
        PFPlayerDataManagementDeletedPropertyDetailsWrapper{ src.Model() }
    {
    }

    PFPlayerDataManagementDeletedPropertyDetailsWrapper(PFPlayerDataManagementDeletedPropertyDetailsWrapper&& src) :
        PFPlayerDataManagementDeletedPropertyDetailsWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerDataManagementDeletedPropertyDetailsWrapper& operator=(PFPlayerDataManagementDeletedPropertyDetailsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerDataManagementDeletedPropertyDetailsWrapper() = default;

    friend void swap(PFPlayerDataManagementDeletedPropertyDetailsWrapper& lhs, PFPlayerDataManagementDeletedPropertyDetailsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_name, rhs.m_name);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetName() const
    {
        return m_name;
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

    bool GetWasDeleted() const
    {
        return this->m_model.wasDeleted;
    }

    void SetWasDeleted(bool value)
    {
        this->m_model.wasDeleted = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
    }

    String m_name;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayerDataManagementClientDeletePlayerCustomPropertiesResultWrapper : public ModelWrapper<PFPlayerDataManagementClientDeletePlayerCustomPropertiesResult, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementClientDeletePlayerCustomPropertiesResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerDataManagementClientDeletePlayerCustomPropertiesResultWrapper() = default;

    PFPlayerDataManagementClientDeletePlayerCustomPropertiesResultWrapper(const PFPlayerDataManagementClientDeletePlayerCustomPropertiesResult& model) :
        ModelWrapper<PFPlayerDataManagementClientDeletePlayerCustomPropertiesResult, Alloc>{ model },
        m_deletedProperties{ model.deletedProperties, model.deletedProperties + model.deletedPropertiesCount }
    {
        SetModelPointers();
    }

    PFPlayerDataManagementClientDeletePlayerCustomPropertiesResultWrapper(const PFPlayerDataManagementClientDeletePlayerCustomPropertiesResultWrapper& src) :
        PFPlayerDataManagementClientDeletePlayerCustomPropertiesResultWrapper{ src.Model() }
    {
    }

    PFPlayerDataManagementClientDeletePlayerCustomPropertiesResultWrapper(PFPlayerDataManagementClientDeletePlayerCustomPropertiesResultWrapper&& src) :
        PFPlayerDataManagementClientDeletePlayerCustomPropertiesResultWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerDataManagementClientDeletePlayerCustomPropertiesResultWrapper& operator=(PFPlayerDataManagementClientDeletePlayerCustomPropertiesResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerDataManagementClientDeletePlayerCustomPropertiesResultWrapper() = default;

    friend void swap(PFPlayerDataManagementClientDeletePlayerCustomPropertiesResultWrapper& lhs, PFPlayerDataManagementClientDeletePlayerCustomPropertiesResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_deletedProperties, rhs.m_deletedProperties);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    ModelVector<PFPlayerDataManagementDeletedPropertyDetailsWrapper<Alloc>, Alloc> const& GetDeletedProperties() const
    {
        return m_deletedProperties;
    }

    void SetDeletedProperties(ModelVector<PFPlayerDataManagementDeletedPropertyDetailsWrapper<Alloc>, Alloc> value)
    {
        m_deletedProperties = std::move(value);
        this->m_model.deletedProperties =  m_deletedProperties.empty() ? nullptr : m_deletedProperties.data();
        this->m_model.deletedPropertiesCount =  static_cast<uint32_t>(m_deletedProperties.size());
    }

    int32_t GetPropertiesVersion() const
    {
        return this->m_model.propertiesVersion;
    }

    void SetPropertiesVersion(int32_t value)
    {
        this->m_model.propertiesVersion = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.deletedProperties = m_deletedProperties.empty() ? nullptr : m_deletedProperties.data();
    }

    ModelVector<PFPlayerDataManagementDeletedPropertyDetailsWrapper<Alloc>, Alloc> m_deletedProperties;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayerDataManagementClientGetPlayerCustomPropertyRequestWrapper : public ModelWrapper<PFPlayerDataManagementClientGetPlayerCustomPropertyRequest, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementClientGetPlayerCustomPropertyRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerDataManagementClientGetPlayerCustomPropertyRequestWrapper() = default;

    PFPlayerDataManagementClientGetPlayerCustomPropertyRequestWrapper(const PFPlayerDataManagementClientGetPlayerCustomPropertyRequest& model) :
        ModelWrapper<PFPlayerDataManagementClientGetPlayerCustomPropertyRequest, Alloc>{ model },
        m_propertyName{ SafeString(model.propertyName) }
    {
        SetModelPointers();
    }

    PFPlayerDataManagementClientGetPlayerCustomPropertyRequestWrapper(const PFPlayerDataManagementClientGetPlayerCustomPropertyRequestWrapper& src) :
        PFPlayerDataManagementClientGetPlayerCustomPropertyRequestWrapper{ src.Model() }
    {
    }

    PFPlayerDataManagementClientGetPlayerCustomPropertyRequestWrapper(PFPlayerDataManagementClientGetPlayerCustomPropertyRequestWrapper&& src) :
        PFPlayerDataManagementClientGetPlayerCustomPropertyRequestWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerDataManagementClientGetPlayerCustomPropertyRequestWrapper& operator=(PFPlayerDataManagementClientGetPlayerCustomPropertyRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerDataManagementClientGetPlayerCustomPropertyRequestWrapper() = default;

    friend void swap(PFPlayerDataManagementClientGetPlayerCustomPropertyRequestWrapper& lhs, PFPlayerDataManagementClientGetPlayerCustomPropertyRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_propertyName, rhs.m_propertyName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetPropertyName() const
    {
        return m_propertyName;
    }

    void SetPropertyName(String value)
    {
        m_propertyName = std::move(value);
        this->m_model.propertyName =  m_propertyName.empty() ? nullptr : m_propertyName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.propertyName = m_propertyName.empty() ? nullptr : m_propertyName.data();
    }

    String m_propertyName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayerDataManagementCustomPropertyDetailsWrapper : public ModelWrapper<PFPlayerDataManagementCustomPropertyDetails, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementCustomPropertyDetails;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerDataManagementCustomPropertyDetailsWrapper() = default;

    PFPlayerDataManagementCustomPropertyDetailsWrapper(const PFPlayerDataManagementCustomPropertyDetails& model) :
        ModelWrapper<PFPlayerDataManagementCustomPropertyDetails, Alloc>{ model },
        m_name{ SafeString(model.name) },
        m_value{ model.value }
    {
        SetModelPointers();
    }

    PFPlayerDataManagementCustomPropertyDetailsWrapper(const PFPlayerDataManagementCustomPropertyDetailsWrapper& src) :
        PFPlayerDataManagementCustomPropertyDetailsWrapper{ src.Model() }
    {
    }

    PFPlayerDataManagementCustomPropertyDetailsWrapper(PFPlayerDataManagementCustomPropertyDetailsWrapper&& src) :
        PFPlayerDataManagementCustomPropertyDetailsWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerDataManagementCustomPropertyDetailsWrapper& operator=(PFPlayerDataManagementCustomPropertyDetailsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerDataManagementCustomPropertyDetailsWrapper() = default;

    friend void swap(PFPlayerDataManagementCustomPropertyDetailsWrapper& lhs, PFPlayerDataManagementCustomPropertyDetailsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_name, rhs.m_name);
        swap(lhs.m_value, rhs.m_value);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetName() const
    {
        return m_name;
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

    JsonObject<Alloc> const& GetValue() const
    {
        return m_value;
    }

    void SetValue(JsonObject<Alloc> value)
    {
        m_value = std::move(value);
        this->m_model.value.stringValue = m_value.stringValue.empty() ? nullptr : m_value.stringValue.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
        this->m_model.value.stringValue = m_value.stringValue.empty() ? nullptr : m_value.stringValue.data();
    }

    String m_name;
    JsonObject<Alloc> m_value;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayerDataManagementClientGetPlayerCustomPropertyResultWrapper : public ModelWrapper<PFPlayerDataManagementClientGetPlayerCustomPropertyResult, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementClientGetPlayerCustomPropertyResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerDataManagementClientGetPlayerCustomPropertyResultWrapper() = default;

    PFPlayerDataManagementClientGetPlayerCustomPropertyResultWrapper(const PFPlayerDataManagementClientGetPlayerCustomPropertyResult& model) :
        ModelWrapper<PFPlayerDataManagementClientGetPlayerCustomPropertyResult, Alloc>{ model },
        m_property{ model.property ? std::optional<PFPlayerDataManagementCustomPropertyDetailsWrapper<Alloc>>{ *model.property } : std::nullopt }
    {
        SetModelPointers();
    }

    PFPlayerDataManagementClientGetPlayerCustomPropertyResultWrapper(const PFPlayerDataManagementClientGetPlayerCustomPropertyResultWrapper& src) :
        PFPlayerDataManagementClientGetPlayerCustomPropertyResultWrapper{ src.Model() }
    {
    }

    PFPlayerDataManagementClientGetPlayerCustomPropertyResultWrapper(PFPlayerDataManagementClientGetPlayerCustomPropertyResultWrapper&& src) :
        PFPlayerDataManagementClientGetPlayerCustomPropertyResultWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerDataManagementClientGetPlayerCustomPropertyResultWrapper& operator=(PFPlayerDataManagementClientGetPlayerCustomPropertyResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerDataManagementClientGetPlayerCustomPropertyResultWrapper() = default;

    friend void swap(PFPlayerDataManagementClientGetPlayerCustomPropertyResultWrapper& lhs, PFPlayerDataManagementClientGetPlayerCustomPropertyResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_property, rhs.m_property);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    int32_t GetPropertiesVersion() const
    {
        return this->m_model.propertiesVersion;
    }

    void SetPropertiesVersion(int32_t value)
    {
        this->m_model.propertiesVersion = value;
    }

    std::optional<PFPlayerDataManagementCustomPropertyDetailsWrapper<Alloc>> const& GetProperty() const
    {
        return m_property;
    }

    void SetProperty(std::optional<PFPlayerDataManagementCustomPropertyDetailsWrapper<Alloc>> value)
    {
        m_property = std::move(value);
        this->m_model.property = m_property ? &m_property->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.property = m_property ?  &m_property->Model() : nullptr;
    }

    std::optional<PFPlayerDataManagementCustomPropertyDetailsWrapper<Alloc>> m_property;
};

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

    std::optional<uint32_t> const& GetIfChangedFromDataVersion() const
    {
        return m_ifChangedFromDataVersion;
    }

    void SetIfChangedFromDataVersion(std::optional<uint32_t> value)
    {
        m_ifChangedFromDataVersion = std::move(value);
        this->m_model.ifChangedFromDataVersion = m_ifChangedFromDataVersion ? m_ifChangedFromDataVersion.operator->() : nullptr;
    }

    CStringVector<Alloc> const& GetKeys() const
    {
        return m_keys;
    }

    void SetKeys(CStringVector<Alloc> value)
    {
        m_keys = std::move(value);
        this->m_model.keys =  m_keys.empty() ? nullptr : m_keys.data();
        this->m_model.keysCount =  static_cast<uint32_t>(m_keys.size());
    }

    String const& GetPlayFabId() const
    {
        return m_playFabId;
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

    ModelDictionaryEntryVector<PFUserDataRecordWrapper<Alloc>, Alloc> const& GetData() const
    {
        return m_data;
    }

    void SetData(ModelDictionaryEntryVector<PFUserDataRecordWrapper<Alloc>, Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

    uint32_t GetDataVersion() const
    {
        return this->m_model.dataVersion;
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
class PFPlayerDataManagementClientListPlayerCustomPropertiesResultWrapper : public ModelWrapper<PFPlayerDataManagementClientListPlayerCustomPropertiesResult, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementClientListPlayerCustomPropertiesResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerDataManagementClientListPlayerCustomPropertiesResultWrapper() = default;

    PFPlayerDataManagementClientListPlayerCustomPropertiesResultWrapper(const PFPlayerDataManagementClientListPlayerCustomPropertiesResult& model) :
        ModelWrapper<PFPlayerDataManagementClientListPlayerCustomPropertiesResult, Alloc>{ model },
        m_properties{ model.properties, model.properties + model.propertiesCount }
    {
        SetModelPointers();
    }

    PFPlayerDataManagementClientListPlayerCustomPropertiesResultWrapper(const PFPlayerDataManagementClientListPlayerCustomPropertiesResultWrapper& src) :
        PFPlayerDataManagementClientListPlayerCustomPropertiesResultWrapper{ src.Model() }
    {
    }

    PFPlayerDataManagementClientListPlayerCustomPropertiesResultWrapper(PFPlayerDataManagementClientListPlayerCustomPropertiesResultWrapper&& src) :
        PFPlayerDataManagementClientListPlayerCustomPropertiesResultWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerDataManagementClientListPlayerCustomPropertiesResultWrapper& operator=(PFPlayerDataManagementClientListPlayerCustomPropertiesResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerDataManagementClientListPlayerCustomPropertiesResultWrapper() = default;

    friend void swap(PFPlayerDataManagementClientListPlayerCustomPropertiesResultWrapper& lhs, PFPlayerDataManagementClientListPlayerCustomPropertiesResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_properties, rhs.m_properties);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    ModelVector<PFPlayerDataManagementCustomPropertyDetailsWrapper<Alloc>, Alloc> const& GetProperties() const
    {
        return m_properties;
    }

    void SetProperties(ModelVector<PFPlayerDataManagementCustomPropertyDetailsWrapper<Alloc>, Alloc> value)
    {
        m_properties = std::move(value);
        this->m_model.properties =  m_properties.empty() ? nullptr : m_properties.data();
        this->m_model.propertiesCount =  static_cast<uint32_t>(m_properties.size());
    }

    int32_t GetPropertiesVersion() const
    {
        return this->m_model.propertiesVersion;
    }

    void SetPropertiesVersion(int32_t value)
    {
        this->m_model.propertiesVersion = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.properties = m_properties.empty() ? nullptr : m_properties.data();
    }

    ModelVector<PFPlayerDataManagementCustomPropertyDetailsWrapper<Alloc>, Alloc> m_properties;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayerDataManagementUpdatePropertyWrapper : public ModelWrapper<PFPlayerDataManagementUpdateProperty, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementUpdateProperty;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerDataManagementUpdatePropertyWrapper() = default;

    PFPlayerDataManagementUpdatePropertyWrapper(const PFPlayerDataManagementUpdateProperty& model) :
        ModelWrapper<PFPlayerDataManagementUpdateProperty, Alloc>{ model },
        m_name{ SafeString(model.name) },
        m_value{ model.value }
    {
        SetModelPointers();
    }

    PFPlayerDataManagementUpdatePropertyWrapper(const PFPlayerDataManagementUpdatePropertyWrapper& src) :
        PFPlayerDataManagementUpdatePropertyWrapper{ src.Model() }
    {
    }

    PFPlayerDataManagementUpdatePropertyWrapper(PFPlayerDataManagementUpdatePropertyWrapper&& src) :
        PFPlayerDataManagementUpdatePropertyWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerDataManagementUpdatePropertyWrapper& operator=(PFPlayerDataManagementUpdatePropertyWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerDataManagementUpdatePropertyWrapper() = default;

    friend void swap(PFPlayerDataManagementUpdatePropertyWrapper& lhs, PFPlayerDataManagementUpdatePropertyWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_name, rhs.m_name);
        swap(lhs.m_value, rhs.m_value);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetName() const
    {
        return m_name;
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

    JsonObject<Alloc> const& GetValue() const
    {
        return m_value;
    }

    void SetValue(JsonObject<Alloc> value)
    {
        m_value = std::move(value);
        this->m_model.value.stringValue = m_value.stringValue.empty() ? nullptr : m_value.stringValue.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
        this->m_model.value.stringValue = m_value.stringValue.empty() ? nullptr : m_value.stringValue.data();
    }

    String m_name;
    JsonObject<Alloc> m_value;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequestWrapper : public ModelWrapper<PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequest, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequestWrapper() = default;

    PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequestWrapper(const PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequest& model) :
        ModelWrapper<PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_expectedPropertiesVersion{ model.expectedPropertiesVersion ? std::optional<int32_t>{ *model.expectedPropertiesVersion } : std::nullopt },
        m_properties{ model.properties, model.properties + model.propertiesCount }
    {
        SetModelPointers();
    }

    PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequestWrapper(const PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequestWrapper& src) :
        PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequestWrapper{ src.Model() }
    {
    }

    PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequestWrapper(PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequestWrapper&& src) :
        PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequestWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequestWrapper& operator=(PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequestWrapper() = default;

    friend void swap(PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequestWrapper& lhs, PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_expectedPropertiesVersion, rhs.m_expectedPropertiesVersion);
        swap(lhs.m_properties, rhs.m_properties);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    StringDictionaryEntryVector<Alloc> const& GetCustomTags() const
    {
        return m_customTags;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    std::optional<int32_t> const& GetExpectedPropertiesVersion() const
    {
        return m_expectedPropertiesVersion;
    }

    void SetExpectedPropertiesVersion(std::optional<int32_t> value)
    {
        m_expectedPropertiesVersion = std::move(value);
        this->m_model.expectedPropertiesVersion = m_expectedPropertiesVersion ? m_expectedPropertiesVersion.operator->() : nullptr;
    }

    ModelVector<PFPlayerDataManagementUpdatePropertyWrapper<Alloc>, Alloc> const& GetProperties() const
    {
        return m_properties;
    }

    void SetProperties(ModelVector<PFPlayerDataManagementUpdatePropertyWrapper<Alloc>, Alloc> value)
    {
        m_properties = std::move(value);
        this->m_model.properties =  m_properties.empty() ? nullptr : m_properties.data();
        this->m_model.propertiesCount =  static_cast<uint32_t>(m_properties.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.expectedPropertiesVersion = m_expectedPropertiesVersion ? m_expectedPropertiesVersion.operator->() : nullptr;
        this->m_model.properties = m_properties.empty() ? nullptr : m_properties.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<int32_t> m_expectedPropertiesVersion;
    ModelVector<PFPlayerDataManagementUpdatePropertyWrapper<Alloc>, Alloc> m_properties;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayerDataManagementClientUpdatePlayerCustomPropertiesResultWrapper : public ModelWrapper<PFPlayerDataManagementClientUpdatePlayerCustomPropertiesResult, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementClientUpdatePlayerCustomPropertiesResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    using ModelWrapper<PFPlayerDataManagementClientUpdatePlayerCustomPropertiesResult, Alloc>::ModelWrapper;

    int32_t GetPropertiesVersion() const
    {
        return this->m_model.propertiesVersion;
    }

    void SetPropertiesVersion(int32_t value)
    {
        this->m_model.propertiesVersion = value;
    }

private:
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

    StringDictionaryEntryVector<Alloc> const& GetCustomTags() const
    {
        return m_customTags;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    StringDictionaryEntryVector<Alloc> const& GetData() const
    {
        return m_data;
    }

    void SetData(StringDictionaryEntryVector<Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

    CStringVector<Alloc> const& GetKeysToRemove() const
    {
        return m_keysToRemove;
    }

    void SetKeysToRemove(CStringVector<Alloc> value)
    {
        m_keysToRemove = std::move(value);
        this->m_model.keysToRemove =  m_keysToRemove.empty() ? nullptr : m_keysToRemove.data();
        this->m_model.keysToRemoveCount =  static_cast<uint32_t>(m_keysToRemove.size());
    }

    std::optional<PFUserDataPermission> const& GetPermission() const
    {
        return m_permission;
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

    uint32_t GetDataVersion() const
    {
        return this->m_model.dataVersion;
    }

    void SetDataVersion(uint32_t value)
    {
        this->m_model.dataVersion = value;
    }

private:
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequestWrapper : public ModelWrapper<PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequest, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequestWrapper() = default;

    PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequestWrapper(const PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequest& model) :
        ModelWrapper<PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_expectedPropertiesVersion{ model.expectedPropertiesVersion ? std::optional<int32_t>{ *model.expectedPropertiesVersion } : std::nullopt },
        m_playFabId{ SafeString(model.playFabId) },
        m_propertyNames{ model.propertyNames, model.propertyNames + model.propertyNamesCount }
    {
        SetModelPointers();
    }

    PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequestWrapper(const PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequestWrapper& src) :
        PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequestWrapper{ src.Model() }
    {
    }

    PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequestWrapper(PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequestWrapper&& src) :
        PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequestWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequestWrapper& operator=(PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequestWrapper() = default;

    friend void swap(PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequestWrapper& lhs, PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_expectedPropertiesVersion, rhs.m_expectedPropertiesVersion);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_propertyNames, rhs.m_propertyNames);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    StringDictionaryEntryVector<Alloc> const& GetCustomTags() const
    {
        return m_customTags;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    std::optional<int32_t> const& GetExpectedPropertiesVersion() const
    {
        return m_expectedPropertiesVersion;
    }

    void SetExpectedPropertiesVersion(std::optional<int32_t> value)
    {
        m_expectedPropertiesVersion = std::move(value);
        this->m_model.expectedPropertiesVersion = m_expectedPropertiesVersion ? m_expectedPropertiesVersion.operator->() : nullptr;
    }

    String const& GetPlayFabId() const
    {
        return m_playFabId;
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    CStringVector<Alloc> const& GetPropertyNames() const
    {
        return m_propertyNames;
    }

    void SetPropertyNames(CStringVector<Alloc> value)
    {
        m_propertyNames = std::move(value);
        this->m_model.propertyNames =  m_propertyNames.empty() ? nullptr : m_propertyNames.data();
        this->m_model.propertyNamesCount =  static_cast<uint32_t>(m_propertyNames.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.expectedPropertiesVersion = m_expectedPropertiesVersion ? m_expectedPropertiesVersion.operator->() : nullptr;
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.propertyNames = m_propertyNames.empty() ? nullptr : m_propertyNames.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<int32_t> m_expectedPropertiesVersion;
    String m_playFabId;
    CStringVector<Alloc> m_propertyNames;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayerDataManagementServerDeletePlayerCustomPropertiesResultWrapper : public ModelWrapper<PFPlayerDataManagementServerDeletePlayerCustomPropertiesResult, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementServerDeletePlayerCustomPropertiesResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerDataManagementServerDeletePlayerCustomPropertiesResultWrapper() = default;

    PFPlayerDataManagementServerDeletePlayerCustomPropertiesResultWrapper(const PFPlayerDataManagementServerDeletePlayerCustomPropertiesResult& model) :
        ModelWrapper<PFPlayerDataManagementServerDeletePlayerCustomPropertiesResult, Alloc>{ model },
        m_deletedProperties{ model.deletedProperties, model.deletedProperties + model.deletedPropertiesCount },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFPlayerDataManagementServerDeletePlayerCustomPropertiesResultWrapper(const PFPlayerDataManagementServerDeletePlayerCustomPropertiesResultWrapper& src) :
        PFPlayerDataManagementServerDeletePlayerCustomPropertiesResultWrapper{ src.Model() }
    {
    }

    PFPlayerDataManagementServerDeletePlayerCustomPropertiesResultWrapper(PFPlayerDataManagementServerDeletePlayerCustomPropertiesResultWrapper&& src) :
        PFPlayerDataManagementServerDeletePlayerCustomPropertiesResultWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerDataManagementServerDeletePlayerCustomPropertiesResultWrapper& operator=(PFPlayerDataManagementServerDeletePlayerCustomPropertiesResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerDataManagementServerDeletePlayerCustomPropertiesResultWrapper() = default;

    friend void swap(PFPlayerDataManagementServerDeletePlayerCustomPropertiesResultWrapper& lhs, PFPlayerDataManagementServerDeletePlayerCustomPropertiesResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_deletedProperties, rhs.m_deletedProperties);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    ModelVector<PFPlayerDataManagementDeletedPropertyDetailsWrapper<Alloc>, Alloc> const& GetDeletedProperties() const
    {
        return m_deletedProperties;
    }

    void SetDeletedProperties(ModelVector<PFPlayerDataManagementDeletedPropertyDetailsWrapper<Alloc>, Alloc> value)
    {
        m_deletedProperties = std::move(value);
        this->m_model.deletedProperties =  m_deletedProperties.empty() ? nullptr : m_deletedProperties.data();
        this->m_model.deletedPropertiesCount =  static_cast<uint32_t>(m_deletedProperties.size());
    }

    String const& GetPlayFabId() const
    {
        return m_playFabId;
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    int32_t GetPropertiesVersion() const
    {
        return this->m_model.propertiesVersion;
    }

    void SetPropertiesVersion(int32_t value)
    {
        this->m_model.propertiesVersion = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.deletedProperties = m_deletedProperties.empty() ? nullptr : m_deletedProperties.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    ModelVector<PFPlayerDataManagementDeletedPropertyDetailsWrapper<Alloc>, Alloc> m_deletedProperties;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayerDataManagementServerGetPlayerCustomPropertyRequestWrapper : public ModelWrapper<PFPlayerDataManagementServerGetPlayerCustomPropertyRequest, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementServerGetPlayerCustomPropertyRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerDataManagementServerGetPlayerCustomPropertyRequestWrapper() = default;

    PFPlayerDataManagementServerGetPlayerCustomPropertyRequestWrapper(const PFPlayerDataManagementServerGetPlayerCustomPropertyRequest& model) :
        ModelWrapper<PFPlayerDataManagementServerGetPlayerCustomPropertyRequest, Alloc>{ model },
        m_playFabId{ SafeString(model.playFabId) },
        m_propertyName{ SafeString(model.propertyName) }
    {
        SetModelPointers();
    }

    PFPlayerDataManagementServerGetPlayerCustomPropertyRequestWrapper(const PFPlayerDataManagementServerGetPlayerCustomPropertyRequestWrapper& src) :
        PFPlayerDataManagementServerGetPlayerCustomPropertyRequestWrapper{ src.Model() }
    {
    }

    PFPlayerDataManagementServerGetPlayerCustomPropertyRequestWrapper(PFPlayerDataManagementServerGetPlayerCustomPropertyRequestWrapper&& src) :
        PFPlayerDataManagementServerGetPlayerCustomPropertyRequestWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerDataManagementServerGetPlayerCustomPropertyRequestWrapper& operator=(PFPlayerDataManagementServerGetPlayerCustomPropertyRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerDataManagementServerGetPlayerCustomPropertyRequestWrapper() = default;

    friend void swap(PFPlayerDataManagementServerGetPlayerCustomPropertyRequestWrapper& lhs, PFPlayerDataManagementServerGetPlayerCustomPropertyRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_propertyName, rhs.m_propertyName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetPlayFabId() const
    {
        return m_playFabId;
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String const& GetPropertyName() const
    {
        return m_propertyName;
    }

    void SetPropertyName(String value)
    {
        m_propertyName = std::move(value);
        this->m_model.propertyName =  m_propertyName.empty() ? nullptr : m_propertyName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.propertyName = m_propertyName.empty() ? nullptr : m_propertyName.data();
    }

    String m_playFabId;
    String m_propertyName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayerDataManagementServerGetPlayerCustomPropertyResultWrapper : public ModelWrapper<PFPlayerDataManagementServerGetPlayerCustomPropertyResult, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementServerGetPlayerCustomPropertyResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerDataManagementServerGetPlayerCustomPropertyResultWrapper() = default;

    PFPlayerDataManagementServerGetPlayerCustomPropertyResultWrapper(const PFPlayerDataManagementServerGetPlayerCustomPropertyResult& model) :
        ModelWrapper<PFPlayerDataManagementServerGetPlayerCustomPropertyResult, Alloc>{ model },
        m_playFabId{ SafeString(model.playFabId) },
        m_property{ model.property ? std::optional<PFPlayerDataManagementCustomPropertyDetailsWrapper<Alloc>>{ *model.property } : std::nullopt }
    {
        SetModelPointers();
    }

    PFPlayerDataManagementServerGetPlayerCustomPropertyResultWrapper(const PFPlayerDataManagementServerGetPlayerCustomPropertyResultWrapper& src) :
        PFPlayerDataManagementServerGetPlayerCustomPropertyResultWrapper{ src.Model() }
    {
    }

    PFPlayerDataManagementServerGetPlayerCustomPropertyResultWrapper(PFPlayerDataManagementServerGetPlayerCustomPropertyResultWrapper&& src) :
        PFPlayerDataManagementServerGetPlayerCustomPropertyResultWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerDataManagementServerGetPlayerCustomPropertyResultWrapper& operator=(PFPlayerDataManagementServerGetPlayerCustomPropertyResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerDataManagementServerGetPlayerCustomPropertyResultWrapper() = default;

    friend void swap(PFPlayerDataManagementServerGetPlayerCustomPropertyResultWrapper& lhs, PFPlayerDataManagementServerGetPlayerCustomPropertyResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_property, rhs.m_property);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetPlayFabId() const
    {
        return m_playFabId;
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    int32_t GetPropertiesVersion() const
    {
        return this->m_model.propertiesVersion;
    }

    void SetPropertiesVersion(int32_t value)
    {
        this->m_model.propertiesVersion = value;
    }

    std::optional<PFPlayerDataManagementCustomPropertyDetailsWrapper<Alloc>> const& GetProperty() const
    {
        return m_property;
    }

    void SetProperty(std::optional<PFPlayerDataManagementCustomPropertyDetailsWrapper<Alloc>> value)
    {
        m_property = std::move(value);
        this->m_model.property = m_property ? &m_property->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.property = m_property ?  &m_property->Model() : nullptr;
    }

    String m_playFabId;
    std::optional<PFPlayerDataManagementCustomPropertyDetailsWrapper<Alloc>> m_property;
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

    ModelDictionaryEntryVector<PFUserDataRecordWrapper<Alloc>, Alloc> const& GetData() const
    {
        return m_data;
    }

    void SetData(ModelDictionaryEntryVector<PFUserDataRecordWrapper<Alloc>, Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

    uint32_t GetDataVersion() const
    {
        return this->m_model.dataVersion;
    }

    void SetDataVersion(uint32_t value)
    {
        this->m_model.dataVersion = value;
    }

    String const& GetPlayFabId() const
    {
        return m_playFabId;
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
class PFPlayerDataManagementListPlayerCustomPropertiesRequestWrapper : public ModelWrapper<PFPlayerDataManagementListPlayerCustomPropertiesRequest, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementListPlayerCustomPropertiesRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerDataManagementListPlayerCustomPropertiesRequestWrapper() = default;

    PFPlayerDataManagementListPlayerCustomPropertiesRequestWrapper(const PFPlayerDataManagementListPlayerCustomPropertiesRequest& model) :
        ModelWrapper<PFPlayerDataManagementListPlayerCustomPropertiesRequest, Alloc>{ model },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFPlayerDataManagementListPlayerCustomPropertiesRequestWrapper(const PFPlayerDataManagementListPlayerCustomPropertiesRequestWrapper& src) :
        PFPlayerDataManagementListPlayerCustomPropertiesRequestWrapper{ src.Model() }
    {
    }

    PFPlayerDataManagementListPlayerCustomPropertiesRequestWrapper(PFPlayerDataManagementListPlayerCustomPropertiesRequestWrapper&& src) :
        PFPlayerDataManagementListPlayerCustomPropertiesRequestWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerDataManagementListPlayerCustomPropertiesRequestWrapper& operator=(PFPlayerDataManagementListPlayerCustomPropertiesRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerDataManagementListPlayerCustomPropertiesRequestWrapper() = default;

    friend void swap(PFPlayerDataManagementListPlayerCustomPropertiesRequestWrapper& lhs, PFPlayerDataManagementListPlayerCustomPropertiesRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetPlayFabId() const
    {
        return m_playFabId;
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
class PFPlayerDataManagementServerListPlayerCustomPropertiesResultWrapper : public ModelWrapper<PFPlayerDataManagementServerListPlayerCustomPropertiesResult, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementServerListPlayerCustomPropertiesResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerDataManagementServerListPlayerCustomPropertiesResultWrapper() = default;

    PFPlayerDataManagementServerListPlayerCustomPropertiesResultWrapper(const PFPlayerDataManagementServerListPlayerCustomPropertiesResult& model) :
        ModelWrapper<PFPlayerDataManagementServerListPlayerCustomPropertiesResult, Alloc>{ model },
        m_playFabId{ SafeString(model.playFabId) },
        m_properties{ model.properties, model.properties + model.propertiesCount }
    {
        SetModelPointers();
    }

    PFPlayerDataManagementServerListPlayerCustomPropertiesResultWrapper(const PFPlayerDataManagementServerListPlayerCustomPropertiesResultWrapper& src) :
        PFPlayerDataManagementServerListPlayerCustomPropertiesResultWrapper{ src.Model() }
    {
    }

    PFPlayerDataManagementServerListPlayerCustomPropertiesResultWrapper(PFPlayerDataManagementServerListPlayerCustomPropertiesResultWrapper&& src) :
        PFPlayerDataManagementServerListPlayerCustomPropertiesResultWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerDataManagementServerListPlayerCustomPropertiesResultWrapper& operator=(PFPlayerDataManagementServerListPlayerCustomPropertiesResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerDataManagementServerListPlayerCustomPropertiesResultWrapper() = default;

    friend void swap(PFPlayerDataManagementServerListPlayerCustomPropertiesResultWrapper& lhs, PFPlayerDataManagementServerListPlayerCustomPropertiesResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_properties, rhs.m_properties);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetPlayFabId() const
    {
        return m_playFabId;
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    ModelVector<PFPlayerDataManagementCustomPropertyDetailsWrapper<Alloc>, Alloc> const& GetProperties() const
    {
        return m_properties;
    }

    void SetProperties(ModelVector<PFPlayerDataManagementCustomPropertyDetailsWrapper<Alloc>, Alloc> value)
    {
        m_properties = std::move(value);
        this->m_model.properties =  m_properties.empty() ? nullptr : m_properties.data();
        this->m_model.propertiesCount =  static_cast<uint32_t>(m_properties.size());
    }

    int32_t GetPropertiesVersion() const
    {
        return this->m_model.propertiesVersion;
    }

    void SetPropertiesVersion(int32_t value)
    {
        this->m_model.propertiesVersion = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.properties = m_properties.empty() ? nullptr : m_properties.data();
    }

    String m_playFabId;
    ModelVector<PFPlayerDataManagementCustomPropertyDetailsWrapper<Alloc>, Alloc> m_properties;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequestWrapper : public ModelWrapper<PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequest, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequestWrapper() = default;

    PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequestWrapper(const PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequest& model) :
        ModelWrapper<PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_expectedPropertiesVersion{ model.expectedPropertiesVersion ? std::optional<int32_t>{ *model.expectedPropertiesVersion } : std::nullopt },
        m_playFabId{ SafeString(model.playFabId) },
        m_properties{ model.properties, model.properties + model.propertiesCount }
    {
        SetModelPointers();
    }

    PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequestWrapper(const PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequestWrapper& src) :
        PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequestWrapper{ src.Model() }
    {
    }

    PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequestWrapper(PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequestWrapper&& src) :
        PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequestWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequestWrapper& operator=(PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequestWrapper() = default;

    friend void swap(PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequestWrapper& lhs, PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_expectedPropertiesVersion, rhs.m_expectedPropertiesVersion);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_properties, rhs.m_properties);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    StringDictionaryEntryVector<Alloc> const& GetCustomTags() const
    {
        return m_customTags;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    std::optional<int32_t> const& GetExpectedPropertiesVersion() const
    {
        return m_expectedPropertiesVersion;
    }

    void SetExpectedPropertiesVersion(std::optional<int32_t> value)
    {
        m_expectedPropertiesVersion = std::move(value);
        this->m_model.expectedPropertiesVersion = m_expectedPropertiesVersion ? m_expectedPropertiesVersion.operator->() : nullptr;
    }

    String const& GetPlayFabId() const
    {
        return m_playFabId;
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    ModelVector<PFPlayerDataManagementUpdatePropertyWrapper<Alloc>, Alloc> const& GetProperties() const
    {
        return m_properties;
    }

    void SetProperties(ModelVector<PFPlayerDataManagementUpdatePropertyWrapper<Alloc>, Alloc> value)
    {
        m_properties = std::move(value);
        this->m_model.properties =  m_properties.empty() ? nullptr : m_properties.data();
        this->m_model.propertiesCount =  static_cast<uint32_t>(m_properties.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.expectedPropertiesVersion = m_expectedPropertiesVersion ? m_expectedPropertiesVersion.operator->() : nullptr;
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.properties = m_properties.empty() ? nullptr : m_properties.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<int32_t> m_expectedPropertiesVersion;
    String m_playFabId;
    ModelVector<PFPlayerDataManagementUpdatePropertyWrapper<Alloc>, Alloc> m_properties;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResultWrapper : public ModelWrapper<PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResult, Alloc>
{
public:
    using ModelType = PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResultWrapper() = default;

    PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResultWrapper(const PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResult& model) :
        ModelWrapper<PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResult, Alloc>{ model },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResultWrapper(const PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResultWrapper& src) :
        PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResultWrapper{ src.Model() }
    {
    }

    PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResultWrapper(PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResultWrapper&& src) :
        PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResultWrapper{}
    {
        swap(*this, src);
    }

    PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResultWrapper& operator=(PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResultWrapper() = default;

    friend void swap(PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResultWrapper& lhs, PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetPlayFabId() const
    {
        return m_playFabId;
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    int32_t GetPropertiesVersion() const
    {
        return this->m_model.propertiesVersion;
    }

    void SetPropertiesVersion(int32_t value)
    {
        this->m_model.propertiesVersion = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

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

    StringDictionaryEntryVector<Alloc> const& GetCustomTags() const
    {
        return m_customTags;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    StringDictionaryEntryVector<Alloc> const& GetData() const
    {
        return m_data;
    }

    void SetData(StringDictionaryEntryVector<Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

    CStringVector<Alloc> const& GetKeysToRemove() const
    {
        return m_keysToRemove;
    }

    void SetKeysToRemove(CStringVector<Alloc> value)
    {
        m_keysToRemove = std::move(value);
        this->m_model.keysToRemove =  m_keysToRemove.empty() ? nullptr : m_keysToRemove.data();
        this->m_model.keysToRemoveCount =  static_cast<uint32_t>(m_keysToRemove.size());
    }

    std::optional<PFUserDataPermission> const& GetPermission() const
    {
        return m_permission;
    }

    void SetPermission(std::optional<PFUserDataPermission> value)
    {
        m_permission = std::move(value);
        this->m_model.permission = m_permission ? m_permission.operator->() : nullptr;
    }

    String const& GetPlayFabId() const
    {
        return m_playFabId;
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

    StringDictionaryEntryVector<Alloc> const& GetCustomTags() const
    {
        return m_customTags;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    StringDictionaryEntryVector<Alloc> const& GetData() const
    {
        return m_data;
    }

    void SetData(StringDictionaryEntryVector<Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

    CStringVector<Alloc> const& GetKeysToRemove() const
    {
        return m_keysToRemove;
    }

    void SetKeysToRemove(CStringVector<Alloc> value)
    {
        m_keysToRemove = std::move(value);
        this->m_model.keysToRemove =  m_keysToRemove.empty() ? nullptr : m_keysToRemove.data();
        this->m_model.keysToRemoveCount =  static_cast<uint32_t>(m_keysToRemove.size());
    }

    String const& GetPlayFabId() const
    {
        return m_playFabId;
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

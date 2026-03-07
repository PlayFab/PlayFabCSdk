// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFStatisticsTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsStatisticColumnWrapper : public ModelWrapper<PFStatisticsStatisticColumn, Alloc>
{
public:
    using ModelType = PFStatisticsStatisticColumn;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsStatisticColumnWrapper() = default;

    PFStatisticsStatisticColumnWrapper(const PFStatisticsStatisticColumn& model) :
        ModelWrapper<PFStatisticsStatisticColumn, Alloc>{ model },
        m_name{ SafeString(model.name) }
    {
        SetModelPointers();
    }

    PFStatisticsStatisticColumnWrapper(const PFStatisticsStatisticColumnWrapper& src) :
        PFStatisticsStatisticColumnWrapper{ src.Model() }
    {
    }

    PFStatisticsStatisticColumnWrapper(PFStatisticsStatisticColumnWrapper&& src) :
        PFStatisticsStatisticColumnWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsStatisticColumnWrapper& operator=(PFStatisticsStatisticColumnWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsStatisticColumnWrapper() = default;

    friend void swap(PFStatisticsStatisticColumnWrapper& lhs, PFStatisticsStatisticColumnWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_name, rhs.m_name);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    PFStatisticsStatisticAggregationMethod GetAggregationMethod() const
    {
        return this->m_model.aggregationMethod;
    }

    void SetAggregationMethod(PFStatisticsStatisticAggregationMethod value)
    {
        this->m_model.aggregationMethod = value;
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

private:
    void SetModelPointers()
    {
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
    }

    String m_name;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsStatisticsUpdateEventConfigWrapper : public ModelWrapper<PFStatisticsStatisticsUpdateEventConfig, Alloc>
{
public:
    using ModelType = PFStatisticsStatisticsUpdateEventConfig;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    using ModelWrapper<PFStatisticsStatisticsUpdateEventConfig, Alloc>::ModelWrapper;

    PFEventType GetEventType() const
    {
        return this->m_model.eventType;
    }

    void SetEventType(PFEventType value)
    {
        this->m_model.eventType = value;
    }

private:
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsStatisticsEventEmissionConfigWrapper : public ModelWrapper<PFStatisticsStatisticsEventEmissionConfig, Alloc>
{
public:
    using ModelType = PFStatisticsStatisticsEventEmissionConfig;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsStatisticsEventEmissionConfigWrapper() = default;

    PFStatisticsStatisticsEventEmissionConfigWrapper(const PFStatisticsStatisticsEventEmissionConfig& model) :
        ModelWrapper<PFStatisticsStatisticsEventEmissionConfig, Alloc>{ model },
        m_updateEventConfig{ model.updateEventConfig ? std::optional<PFStatisticsStatisticsUpdateEventConfigWrapper<Alloc>>{ *model.updateEventConfig } : std::nullopt }
    {
        SetModelPointers();
    }

    PFStatisticsStatisticsEventEmissionConfigWrapper(const PFStatisticsStatisticsEventEmissionConfigWrapper& src) :
        PFStatisticsStatisticsEventEmissionConfigWrapper{ src.Model() }
    {
    }

    PFStatisticsStatisticsEventEmissionConfigWrapper(PFStatisticsStatisticsEventEmissionConfigWrapper&& src) :
        PFStatisticsStatisticsEventEmissionConfigWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsStatisticsEventEmissionConfigWrapper& operator=(PFStatisticsStatisticsEventEmissionConfigWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsStatisticsEventEmissionConfigWrapper() = default;

    friend void swap(PFStatisticsStatisticsEventEmissionConfigWrapper& lhs, PFStatisticsStatisticsEventEmissionConfigWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_updateEventConfig, rhs.m_updateEventConfig);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    std::optional<PFStatisticsStatisticsUpdateEventConfigWrapper<Alloc>> const& GetUpdateEventConfig() const
    {
        return m_updateEventConfig;
    }

    void SetUpdateEventConfig(std::optional<PFStatisticsStatisticsUpdateEventConfigWrapper<Alloc>> value)
    {
        m_updateEventConfig = std::move(value);
        this->m_model.updateEventConfig = m_updateEventConfig ? &m_updateEventConfig->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.updateEventConfig = m_updateEventConfig ?  &m_updateEventConfig->Model() : nullptr;
    }

    std::optional<PFStatisticsStatisticsUpdateEventConfigWrapper<Alloc>> m_updateEventConfig;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsCreateStatisticDefinitionRequestWrapper : public ModelWrapper<PFStatisticsCreateStatisticDefinitionRequest, Alloc>
{
public:
    using ModelType = PFStatisticsCreateStatisticDefinitionRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsCreateStatisticDefinitionRequestWrapper() = default;

    PFStatisticsCreateStatisticDefinitionRequestWrapper(const PFStatisticsCreateStatisticDefinitionRequest& model) :
        ModelWrapper<PFStatisticsCreateStatisticDefinitionRequest, Alloc>{ model },
        m_aggregationSources{ model.aggregationSources, model.aggregationSources + model.aggregationSourcesCount },
        m_columns{ model.columns, model.columns + model.columnsCount },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entityType{ SafeString(model.entityType) },
        m_eventEmissionConfig{ model.eventEmissionConfig ? std::optional<PFStatisticsStatisticsEventEmissionConfigWrapper<Alloc>>{ *model.eventEmissionConfig } : std::nullopt },
        m_name{ SafeString(model.name) },
        m_versionConfiguration{ model.versionConfiguration ? std::optional<PFVersionConfigurationWrapper<Alloc>>{ *model.versionConfiguration } : std::nullopt }
    {
        SetModelPointers();
    }

    PFStatisticsCreateStatisticDefinitionRequestWrapper(const PFStatisticsCreateStatisticDefinitionRequestWrapper& src) :
        PFStatisticsCreateStatisticDefinitionRequestWrapper{ src.Model() }
    {
    }

    PFStatisticsCreateStatisticDefinitionRequestWrapper(PFStatisticsCreateStatisticDefinitionRequestWrapper&& src) :
        PFStatisticsCreateStatisticDefinitionRequestWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsCreateStatisticDefinitionRequestWrapper& operator=(PFStatisticsCreateStatisticDefinitionRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsCreateStatisticDefinitionRequestWrapper() = default;

    friend void swap(PFStatisticsCreateStatisticDefinitionRequestWrapper& lhs, PFStatisticsCreateStatisticDefinitionRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_aggregationSources, rhs.m_aggregationSources);
        swap(lhs.m_columns, rhs.m_columns);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entityType, rhs.m_entityType);
        swap(lhs.m_eventEmissionConfig, rhs.m_eventEmissionConfig);
        swap(lhs.m_name, rhs.m_name);
        swap(lhs.m_versionConfiguration, rhs.m_versionConfiguration);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    CStringVector<Alloc> const& GetAggregationSources() const
    {
        return m_aggregationSources;
    }

    void SetAggregationSources(CStringVector<Alloc> value)
    {
        m_aggregationSources = std::move(value);
        this->m_model.aggregationSources =  m_aggregationSources.empty() ? nullptr : m_aggregationSources.data();
        this->m_model.aggregationSourcesCount =  static_cast<uint32_t>(m_aggregationSources.size());
    }

    ModelVector<PFStatisticsStatisticColumnWrapper<Alloc>, Alloc> const& GetColumns() const
    {
        return m_columns;
    }

    void SetColumns(ModelVector<PFStatisticsStatisticColumnWrapper<Alloc>, Alloc> value)
    {
        m_columns = std::move(value);
        this->m_model.columns =  m_columns.empty() ? nullptr : m_columns.data();
        this->m_model.columnsCount =  static_cast<uint32_t>(m_columns.size());
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

    String const& GetEntityType() const
    {
        return m_entityType;
    }

    void SetEntityType(String value)
    {
        m_entityType = std::move(value);
        this->m_model.entityType =  m_entityType.empty() ? nullptr : m_entityType.data();
    }

    std::optional<PFStatisticsStatisticsEventEmissionConfigWrapper<Alloc>> const& GetEventEmissionConfig() const
    {
        return m_eventEmissionConfig;
    }

    void SetEventEmissionConfig(std::optional<PFStatisticsStatisticsEventEmissionConfigWrapper<Alloc>> value)
    {
        m_eventEmissionConfig = std::move(value);
        this->m_model.eventEmissionConfig = m_eventEmissionConfig ? &m_eventEmissionConfig->Model() : nullptr;
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

    std::optional<PFVersionConfigurationWrapper<Alloc>> const& GetVersionConfiguration() const
    {
        return m_versionConfiguration;
    }

    void SetVersionConfiguration(std::optional<PFVersionConfigurationWrapper<Alloc>> value)
    {
        m_versionConfiguration = std::move(value);
        this->m_model.versionConfiguration = m_versionConfiguration ? &m_versionConfiguration->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.aggregationSources = m_aggregationSources.empty() ? nullptr : m_aggregationSources.data();
        this->m_model.columns = m_columns.empty() ? nullptr : m_columns.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entityType = m_entityType.empty() ? nullptr : m_entityType.data();
        this->m_model.eventEmissionConfig = m_eventEmissionConfig ?  &m_eventEmissionConfig->Model() : nullptr;
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
        this->m_model.versionConfiguration = m_versionConfiguration ?  &m_versionConfiguration->Model() : nullptr;
    }

    CStringVector<Alloc> m_aggregationSources;
    ModelVector<PFStatisticsStatisticColumnWrapper<Alloc>, Alloc> m_columns;
    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_entityType;
    std::optional<PFStatisticsStatisticsEventEmissionConfigWrapper<Alloc>> m_eventEmissionConfig;
    String m_name;
    std::optional<PFVersionConfigurationWrapper<Alloc>> m_versionConfiguration;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsDeleteStatisticDefinitionRequestWrapper : public ModelWrapper<PFStatisticsDeleteStatisticDefinitionRequest, Alloc>
{
public:
    using ModelType = PFStatisticsDeleteStatisticDefinitionRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsDeleteStatisticDefinitionRequestWrapper() = default;

    PFStatisticsDeleteStatisticDefinitionRequestWrapper(const PFStatisticsDeleteStatisticDefinitionRequest& model) :
        ModelWrapper<PFStatisticsDeleteStatisticDefinitionRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_name{ SafeString(model.name) }
    {
        SetModelPointers();
    }

    PFStatisticsDeleteStatisticDefinitionRequestWrapper(const PFStatisticsDeleteStatisticDefinitionRequestWrapper& src) :
        PFStatisticsDeleteStatisticDefinitionRequestWrapper{ src.Model() }
    {
    }

    PFStatisticsDeleteStatisticDefinitionRequestWrapper(PFStatisticsDeleteStatisticDefinitionRequestWrapper&& src) :
        PFStatisticsDeleteStatisticDefinitionRequestWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsDeleteStatisticDefinitionRequestWrapper& operator=(PFStatisticsDeleteStatisticDefinitionRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsDeleteStatisticDefinitionRequestWrapper() = default;

    friend void swap(PFStatisticsDeleteStatisticDefinitionRequestWrapper& lhs, PFStatisticsDeleteStatisticDefinitionRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_name, rhs.m_name);
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

    String const& GetName() const
    {
        return m_name;
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_name;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsStatisticDeleteWrapper : public ModelWrapper<PFStatisticsStatisticDelete, Alloc>
{
public:
    using ModelType = PFStatisticsStatisticDelete;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsStatisticDeleteWrapper() = default;

    PFStatisticsStatisticDeleteWrapper(const PFStatisticsStatisticDelete& model) :
        ModelWrapper<PFStatisticsStatisticDelete, Alloc>{ model },
        m_name{ SafeString(model.name) }
    {
        SetModelPointers();
    }

    PFStatisticsStatisticDeleteWrapper(const PFStatisticsStatisticDeleteWrapper& src) :
        PFStatisticsStatisticDeleteWrapper{ src.Model() }
    {
    }

    PFStatisticsStatisticDeleteWrapper(PFStatisticsStatisticDeleteWrapper&& src) :
        PFStatisticsStatisticDeleteWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsStatisticDeleteWrapper& operator=(PFStatisticsStatisticDeleteWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsStatisticDeleteWrapper() = default;

    friend void swap(PFStatisticsStatisticDeleteWrapper& lhs, PFStatisticsStatisticDeleteWrapper& rhs)
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

private:
    void SetModelPointers()
    {
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
    }

    String m_name;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsDeleteStatisticsRequestWrapper : public ModelWrapper<PFStatisticsDeleteStatisticsRequest, Alloc>
{
public:
    using ModelType = PFStatisticsDeleteStatisticsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsDeleteStatisticsRequestWrapper() = default;

    PFStatisticsDeleteStatisticsRequestWrapper(const PFStatisticsDeleteStatisticsRequest& model) :
        ModelWrapper<PFStatisticsDeleteStatisticsRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_statistics{ model.statistics, model.statistics + model.statisticsCount }
    {
        SetModelPointers();
    }

    PFStatisticsDeleteStatisticsRequestWrapper(const PFStatisticsDeleteStatisticsRequestWrapper& src) :
        PFStatisticsDeleteStatisticsRequestWrapper{ src.Model() }
    {
    }

    PFStatisticsDeleteStatisticsRequestWrapper(PFStatisticsDeleteStatisticsRequestWrapper&& src) :
        PFStatisticsDeleteStatisticsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsDeleteStatisticsRequestWrapper& operator=(PFStatisticsDeleteStatisticsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsDeleteStatisticsRequestWrapper() = default;

    friend void swap(PFStatisticsDeleteStatisticsRequestWrapper& lhs, PFStatisticsDeleteStatisticsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_statistics, rhs.m_statistics);
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

    std::optional<PFEntityKeyWrapper<Alloc>> const& GetEntity() const
    {
        return m_entity;
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    ModelVector<PFStatisticsStatisticDeleteWrapper<Alloc>, Alloc> const& GetStatistics() const
    {
        return m_statistics;
    }

    void SetStatistics(ModelVector<PFStatisticsStatisticDeleteWrapper<Alloc>, Alloc> value)
    {
        m_statistics = std::move(value);
        this->m_model.statistics =  m_statistics.empty() ? nullptr : m_statistics.data();
        this->m_model.statisticsCount =  static_cast<uint32_t>(m_statistics.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.statistics = m_statistics.empty() ? nullptr : m_statistics.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    ModelVector<PFStatisticsStatisticDeleteWrapper<Alloc>, Alloc> m_statistics;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsDeleteStatisticsResponseWrapper : public ModelWrapper<PFStatisticsDeleteStatisticsResponse, Alloc>
{
public:
    using ModelType = PFStatisticsDeleteStatisticsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsDeleteStatisticsResponseWrapper() = default;

    PFStatisticsDeleteStatisticsResponseWrapper(const PFStatisticsDeleteStatisticsResponse& model) :
        ModelWrapper<PFStatisticsDeleteStatisticsResponse, Alloc>{ model },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt }
    {
        SetModelPointers();
    }

    PFStatisticsDeleteStatisticsResponseWrapper(const PFStatisticsDeleteStatisticsResponseWrapper& src) :
        PFStatisticsDeleteStatisticsResponseWrapper{ src.Model() }
    {
    }

    PFStatisticsDeleteStatisticsResponseWrapper(PFStatisticsDeleteStatisticsResponseWrapper&& src) :
        PFStatisticsDeleteStatisticsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsDeleteStatisticsResponseWrapper& operator=(PFStatisticsDeleteStatisticsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsDeleteStatisticsResponseWrapper() = default;

    friend void swap(PFStatisticsDeleteStatisticsResponseWrapper& lhs, PFStatisticsDeleteStatisticsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_entity, rhs.m_entity);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    std::optional<PFEntityKeyWrapper<Alloc>> const& GetEntity() const
    {
        return m_entity;
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
    }

    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsGetStatisticDefinitionRequestWrapper : public ModelWrapper<PFStatisticsGetStatisticDefinitionRequest, Alloc>
{
public:
    using ModelType = PFStatisticsGetStatisticDefinitionRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsGetStatisticDefinitionRequestWrapper() = default;

    PFStatisticsGetStatisticDefinitionRequestWrapper(const PFStatisticsGetStatisticDefinitionRequest& model) :
        ModelWrapper<PFStatisticsGetStatisticDefinitionRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_name{ SafeString(model.name) }
    {
        SetModelPointers();
    }

    PFStatisticsGetStatisticDefinitionRequestWrapper(const PFStatisticsGetStatisticDefinitionRequestWrapper& src) :
        PFStatisticsGetStatisticDefinitionRequestWrapper{ src.Model() }
    {
    }

    PFStatisticsGetStatisticDefinitionRequestWrapper(PFStatisticsGetStatisticDefinitionRequestWrapper&& src) :
        PFStatisticsGetStatisticDefinitionRequestWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsGetStatisticDefinitionRequestWrapper& operator=(PFStatisticsGetStatisticDefinitionRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsGetStatisticDefinitionRequestWrapper() = default;

    friend void swap(PFStatisticsGetStatisticDefinitionRequestWrapper& lhs, PFStatisticsGetStatisticDefinitionRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_name, rhs.m_name);
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

    String const& GetName() const
    {
        return m_name;
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_name;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsGetStatisticDefinitionResponseWrapper : public ModelWrapper<PFStatisticsGetStatisticDefinitionResponse, Alloc>
{
public:
    using ModelType = PFStatisticsGetStatisticDefinitionResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsGetStatisticDefinitionResponseWrapper() = default;

    PFStatisticsGetStatisticDefinitionResponseWrapper(const PFStatisticsGetStatisticDefinitionResponse& model) :
        ModelWrapper<PFStatisticsGetStatisticDefinitionResponse, Alloc>{ model },
        m_aggregationDestinations{ model.aggregationDestinations, model.aggregationDestinations + model.aggregationDestinationsCount },
        m_aggregationSources{ model.aggregationSources, model.aggregationSources + model.aggregationSourcesCount },
        m_columns{ model.columns, model.columns + model.columnsCount },
        m_entityType{ SafeString(model.entityType) },
        m_eventEmissionConfig{ model.eventEmissionConfig ? std::optional<PFStatisticsStatisticsEventEmissionConfigWrapper<Alloc>>{ *model.eventEmissionConfig } : std::nullopt },
        m_lastResetTime{ model.lastResetTime ? std::optional<time_t>{ *model.lastResetTime } : std::nullopt },
        m_linkedLeaderboardNames{ model.linkedLeaderboardNames, model.linkedLeaderboardNames + model.linkedLeaderboardNamesCount },
        m_name{ SafeString(model.name) },
        m_versionConfiguration{ model.versionConfiguration ? std::optional<PFVersionConfigurationWrapper<Alloc>>{ *model.versionConfiguration } : std::nullopt }
    {
        SetModelPointers();
    }

    PFStatisticsGetStatisticDefinitionResponseWrapper(const PFStatisticsGetStatisticDefinitionResponseWrapper& src) :
        PFStatisticsGetStatisticDefinitionResponseWrapper{ src.Model() }
    {
    }

    PFStatisticsGetStatisticDefinitionResponseWrapper(PFStatisticsGetStatisticDefinitionResponseWrapper&& src) :
        PFStatisticsGetStatisticDefinitionResponseWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsGetStatisticDefinitionResponseWrapper& operator=(PFStatisticsGetStatisticDefinitionResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsGetStatisticDefinitionResponseWrapper() = default;

    friend void swap(PFStatisticsGetStatisticDefinitionResponseWrapper& lhs, PFStatisticsGetStatisticDefinitionResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_aggregationDestinations, rhs.m_aggregationDestinations);
        swap(lhs.m_aggregationSources, rhs.m_aggregationSources);
        swap(lhs.m_columns, rhs.m_columns);
        swap(lhs.m_entityType, rhs.m_entityType);
        swap(lhs.m_eventEmissionConfig, rhs.m_eventEmissionConfig);
        swap(lhs.m_lastResetTime, rhs.m_lastResetTime);
        swap(lhs.m_linkedLeaderboardNames, rhs.m_linkedLeaderboardNames);
        swap(lhs.m_name, rhs.m_name);
        swap(lhs.m_versionConfiguration, rhs.m_versionConfiguration);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    CStringVector<Alloc> const& GetAggregationDestinations() const
    {
        return m_aggregationDestinations;
    }

    void SetAggregationDestinations(CStringVector<Alloc> value)
    {
        m_aggregationDestinations = std::move(value);
        this->m_model.aggregationDestinations =  m_aggregationDestinations.empty() ? nullptr : m_aggregationDestinations.data();
        this->m_model.aggregationDestinationsCount =  static_cast<uint32_t>(m_aggregationDestinations.size());
    }

    CStringVector<Alloc> const& GetAggregationSources() const
    {
        return m_aggregationSources;
    }

    void SetAggregationSources(CStringVector<Alloc> value)
    {
        m_aggregationSources = std::move(value);
        this->m_model.aggregationSources =  m_aggregationSources.empty() ? nullptr : m_aggregationSources.data();
        this->m_model.aggregationSourcesCount =  static_cast<uint32_t>(m_aggregationSources.size());
    }

    ModelVector<PFStatisticsStatisticColumnWrapper<Alloc>, Alloc> const& GetColumns() const
    {
        return m_columns;
    }

    void SetColumns(ModelVector<PFStatisticsStatisticColumnWrapper<Alloc>, Alloc> value)
    {
        m_columns = std::move(value);
        this->m_model.columns =  m_columns.empty() ? nullptr : m_columns.data();
        this->m_model.columnsCount =  static_cast<uint32_t>(m_columns.size());
    }

    time_t GetCreated() const
    {
        return this->m_model.created;
    }

    void SetCreated(time_t value)
    {
        this->m_model.created = value;
    }

    String const& GetEntityType() const
    {
        return m_entityType;
    }

    void SetEntityType(String value)
    {
        m_entityType = std::move(value);
        this->m_model.entityType =  m_entityType.empty() ? nullptr : m_entityType.data();
    }

    std::optional<PFStatisticsStatisticsEventEmissionConfigWrapper<Alloc>> const& GetEventEmissionConfig() const
    {
        return m_eventEmissionConfig;
    }

    void SetEventEmissionConfig(std::optional<PFStatisticsStatisticsEventEmissionConfigWrapper<Alloc>> value)
    {
        m_eventEmissionConfig = std::move(value);
        this->m_model.eventEmissionConfig = m_eventEmissionConfig ? &m_eventEmissionConfig->Model() : nullptr;
    }

    std::optional<time_t> const& GetLastResetTime() const
    {
        return m_lastResetTime;
    }

    void SetLastResetTime(std::optional<time_t> value)
    {
        m_lastResetTime = std::move(value);
        this->m_model.lastResetTime = m_lastResetTime ? m_lastResetTime.operator->() : nullptr;
    }

    CStringVector<Alloc> const& GetLinkedLeaderboardNames() const
    {
        return m_linkedLeaderboardNames;
    }

    void SetLinkedLeaderboardNames(CStringVector<Alloc> value)
    {
        m_linkedLeaderboardNames = std::move(value);
        this->m_model.linkedLeaderboardNames =  m_linkedLeaderboardNames.empty() ? nullptr : m_linkedLeaderboardNames.data();
        this->m_model.linkedLeaderboardNamesCount =  static_cast<uint32_t>(m_linkedLeaderboardNames.size());
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

    uint32_t GetVersion() const
    {
        return this->m_model.version;
    }

    void SetVersion(uint32_t value)
    {
        this->m_model.version = value;
    }

    std::optional<PFVersionConfigurationWrapper<Alloc>> const& GetVersionConfiguration() const
    {
        return m_versionConfiguration;
    }

    void SetVersionConfiguration(std::optional<PFVersionConfigurationWrapper<Alloc>> value)
    {
        m_versionConfiguration = std::move(value);
        this->m_model.versionConfiguration = m_versionConfiguration ? &m_versionConfiguration->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.aggregationDestinations = m_aggregationDestinations.empty() ? nullptr : m_aggregationDestinations.data();
        this->m_model.aggregationSources = m_aggregationSources.empty() ? nullptr : m_aggregationSources.data();
        this->m_model.columns = m_columns.empty() ? nullptr : m_columns.data();
        this->m_model.entityType = m_entityType.empty() ? nullptr : m_entityType.data();
        this->m_model.eventEmissionConfig = m_eventEmissionConfig ?  &m_eventEmissionConfig->Model() : nullptr;
        this->m_model.lastResetTime = m_lastResetTime ? m_lastResetTime.operator->() : nullptr;
        this->m_model.linkedLeaderboardNames = m_linkedLeaderboardNames.empty() ? nullptr : m_linkedLeaderboardNames.data();
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
        this->m_model.versionConfiguration = m_versionConfiguration ?  &m_versionConfiguration->Model() : nullptr;
    }

    CStringVector<Alloc> m_aggregationDestinations;
    CStringVector<Alloc> m_aggregationSources;
    ModelVector<PFStatisticsStatisticColumnWrapper<Alloc>, Alloc> m_columns;
    String m_entityType;
    std::optional<PFStatisticsStatisticsEventEmissionConfigWrapper<Alloc>> m_eventEmissionConfig;
    std::optional<time_t> m_lastResetTime;
    CStringVector<Alloc> m_linkedLeaderboardNames;
    String m_name;
    std::optional<PFVersionConfigurationWrapper<Alloc>> m_versionConfiguration;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsGetStatisticsRequestWrapper : public ModelWrapper<PFStatisticsGetStatisticsRequest, Alloc>
{
public:
    using ModelType = PFStatisticsGetStatisticsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsGetStatisticsRequestWrapper() = default;

    PFStatisticsGetStatisticsRequestWrapper(const PFStatisticsGetStatisticsRequest& model) :
        ModelWrapper<PFStatisticsGetStatisticsRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_statisticNames{ model.statisticNames, model.statisticNames + model.statisticNamesCount }
    {
        SetModelPointers();
    }

    PFStatisticsGetStatisticsRequestWrapper(const PFStatisticsGetStatisticsRequestWrapper& src) :
        PFStatisticsGetStatisticsRequestWrapper{ src.Model() }
    {
    }

    PFStatisticsGetStatisticsRequestWrapper(PFStatisticsGetStatisticsRequestWrapper&& src) :
        PFStatisticsGetStatisticsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsGetStatisticsRequestWrapper& operator=(PFStatisticsGetStatisticsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsGetStatisticsRequestWrapper() = default;

    friend void swap(PFStatisticsGetStatisticsRequestWrapper& lhs, PFStatisticsGetStatisticsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_statisticNames, rhs.m_statisticNames);
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

    std::optional<PFEntityKeyWrapper<Alloc>> const& GetEntity() const
    {
        return m_entity;
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    CStringVector<Alloc> const& GetStatisticNames() const
    {
        return m_statisticNames;
    }

    void SetStatisticNames(CStringVector<Alloc> value)
    {
        m_statisticNames = std::move(value);
        this->m_model.statisticNames =  m_statisticNames.empty() ? nullptr : m_statisticNames.data();
        this->m_model.statisticNamesCount =  static_cast<uint32_t>(m_statisticNames.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.statisticNames = m_statisticNames.empty() ? nullptr : m_statisticNames.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    CStringVector<Alloc> m_statisticNames;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsStatisticColumnCollectionWrapper : public ModelWrapper<PFStatisticsStatisticColumnCollection, Alloc>
{
public:
    using ModelType = PFStatisticsStatisticColumnCollection;
    using DictionaryEntryType = PFStatisticsStatisticColumnCollectionDictionaryEntry;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsStatisticColumnCollectionWrapper() = default;

    PFStatisticsStatisticColumnCollectionWrapper(const PFStatisticsStatisticColumnCollection& model) :
        ModelWrapper<PFStatisticsStatisticColumnCollection, Alloc>{ model },
        m_columns{ model.columns, model.columns + model.columnsCount }
    {
        SetModelPointers();
    }

    PFStatisticsStatisticColumnCollectionWrapper(const PFStatisticsStatisticColumnCollectionWrapper& src) :
        PFStatisticsStatisticColumnCollectionWrapper{ src.Model() }
    {
    }

    PFStatisticsStatisticColumnCollectionWrapper(PFStatisticsStatisticColumnCollectionWrapper&& src) :
        PFStatisticsStatisticColumnCollectionWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsStatisticColumnCollectionWrapper& operator=(PFStatisticsStatisticColumnCollectionWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsStatisticColumnCollectionWrapper() = default;

    friend void swap(PFStatisticsStatisticColumnCollectionWrapper& lhs, PFStatisticsStatisticColumnCollectionWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_columns, rhs.m_columns);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    ModelVector<PFStatisticsStatisticColumnWrapper<Alloc>, Alloc> const& GetColumns() const
    {
        return m_columns;
    }

    void SetColumns(ModelVector<PFStatisticsStatisticColumnWrapper<Alloc>, Alloc> value)
    {
        m_columns = std::move(value);
        this->m_model.columns =  m_columns.empty() ? nullptr : m_columns.data();
        this->m_model.columnsCount =  static_cast<uint32_t>(m_columns.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.columns = m_columns.empty() ? nullptr : m_columns.data();
    }

    ModelVector<PFStatisticsStatisticColumnWrapper<Alloc>, Alloc> m_columns;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsEntityStatisticValueWrapper : public ModelWrapper<PFStatisticsEntityStatisticValue, Alloc>
{
public:
    using ModelType = PFStatisticsEntityStatisticValue;
    using DictionaryEntryType = PFStatisticsEntityStatisticValueDictionaryEntry;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsEntityStatisticValueWrapper() = default;

    PFStatisticsEntityStatisticValueWrapper(const PFStatisticsEntityStatisticValue& model) :
        ModelWrapper<PFStatisticsEntityStatisticValue, Alloc>{ model },
        m_metadata{ SafeString(model.metadata) },
        m_name{ SafeString(model.name) },
        m_scores{ model.scores, model.scores + model.scoresCount }
    {
        SetModelPointers();
    }

    PFStatisticsEntityStatisticValueWrapper(const PFStatisticsEntityStatisticValueWrapper& src) :
        PFStatisticsEntityStatisticValueWrapper{ src.Model() }
    {
    }

    PFStatisticsEntityStatisticValueWrapper(PFStatisticsEntityStatisticValueWrapper&& src) :
        PFStatisticsEntityStatisticValueWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsEntityStatisticValueWrapper& operator=(PFStatisticsEntityStatisticValueWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsEntityStatisticValueWrapper() = default;

    friend void swap(PFStatisticsEntityStatisticValueWrapper& lhs, PFStatisticsEntityStatisticValueWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_metadata, rhs.m_metadata);
        swap(lhs.m_name, rhs.m_name);
        swap(lhs.m_scores, rhs.m_scores);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetMetadata() const
    {
        return m_metadata;
    }

    void SetMetadata(String value)
    {
        m_metadata = std::move(value);
        this->m_model.metadata =  m_metadata.empty() ? nullptr : m_metadata.data();
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

    CStringVector<Alloc> const& GetScores() const
    {
        return m_scores;
    }

    void SetScores(CStringVector<Alloc> value)
    {
        m_scores = std::move(value);
        this->m_model.scores =  m_scores.empty() ? nullptr : m_scores.data();
        this->m_model.scoresCount =  static_cast<uint32_t>(m_scores.size());
    }

    int32_t GetVersion() const
    {
        return this->m_model.version;
    }

    void SetVersion(int32_t value)
    {
        this->m_model.version = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.metadata = m_metadata.empty() ? nullptr : m_metadata.data();
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
        this->m_model.scores = m_scores.empty() ? nullptr : m_scores.data();
    }

    String m_metadata;
    String m_name;
    CStringVector<Alloc> m_scores;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsGetStatisticsResponseWrapper : public ModelWrapper<PFStatisticsGetStatisticsResponse, Alloc>
{
public:
    using ModelType = PFStatisticsGetStatisticsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsGetStatisticsResponseWrapper() = default;

    PFStatisticsGetStatisticsResponseWrapper(const PFStatisticsGetStatisticsResponse& model) :
        ModelWrapper<PFStatisticsGetStatisticsResponse, Alloc>{ model },
        m_columnDetails{ model.columnDetails, model.columnDetails + model.columnDetailsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_statistics{ model.statistics, model.statistics + model.statisticsCount }
    {
        SetModelPointers();
    }

    PFStatisticsGetStatisticsResponseWrapper(const PFStatisticsGetStatisticsResponseWrapper& src) :
        PFStatisticsGetStatisticsResponseWrapper{ src.Model() }
    {
    }

    PFStatisticsGetStatisticsResponseWrapper(PFStatisticsGetStatisticsResponseWrapper&& src) :
        PFStatisticsGetStatisticsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsGetStatisticsResponseWrapper& operator=(PFStatisticsGetStatisticsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsGetStatisticsResponseWrapper() = default;

    friend void swap(PFStatisticsGetStatisticsResponseWrapper& lhs, PFStatisticsGetStatisticsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_columnDetails, rhs.m_columnDetails);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_statistics, rhs.m_statistics);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    ModelDictionaryEntryVector<PFStatisticsStatisticColumnCollectionWrapper<Alloc>, Alloc> const& GetColumnDetails() const
    {
        return m_columnDetails;
    }

    void SetColumnDetails(ModelDictionaryEntryVector<PFStatisticsStatisticColumnCollectionWrapper<Alloc>, Alloc> value)
    {
        m_columnDetails = std::move(value);
        this->m_model.columnDetails =  m_columnDetails.empty() ? nullptr : m_columnDetails.data();
        this->m_model.columnDetailsCount =  static_cast<uint32_t>(m_columnDetails.size());
    }

    std::optional<PFEntityKeyWrapper<Alloc>> const& GetEntity() const
    {
        return m_entity;
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    ModelDictionaryEntryVector<PFStatisticsEntityStatisticValueWrapper<Alloc>, Alloc> const& GetStatistics() const
    {
        return m_statistics;
    }

    void SetStatistics(ModelDictionaryEntryVector<PFStatisticsEntityStatisticValueWrapper<Alloc>, Alloc> value)
    {
        m_statistics = std::move(value);
        this->m_model.statistics =  m_statistics.empty() ? nullptr : m_statistics.data();
        this->m_model.statisticsCount =  static_cast<uint32_t>(m_statistics.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.columnDetails = m_columnDetails.empty() ? nullptr : m_columnDetails.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.statistics = m_statistics.empty() ? nullptr : m_statistics.data();
    }

    ModelDictionaryEntryVector<PFStatisticsStatisticColumnCollectionWrapper<Alloc>, Alloc> m_columnDetails;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    ModelDictionaryEntryVector<PFStatisticsEntityStatisticValueWrapper<Alloc>, Alloc> m_statistics;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsGetStatisticsForEntitiesRequestWrapper : public ModelWrapper<PFStatisticsGetStatisticsForEntitiesRequest, Alloc>
{
public:
    using ModelType = PFStatisticsGetStatisticsForEntitiesRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsGetStatisticsForEntitiesRequestWrapper() = default;

    PFStatisticsGetStatisticsForEntitiesRequestWrapper(const PFStatisticsGetStatisticsForEntitiesRequest& model) :
        ModelWrapper<PFStatisticsGetStatisticsForEntitiesRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entities{ model.entities, model.entities + model.entitiesCount },
        m_statisticNames{ model.statisticNames, model.statisticNames + model.statisticNamesCount }
    {
        SetModelPointers();
    }

    PFStatisticsGetStatisticsForEntitiesRequestWrapper(const PFStatisticsGetStatisticsForEntitiesRequestWrapper& src) :
        PFStatisticsGetStatisticsForEntitiesRequestWrapper{ src.Model() }
    {
    }

    PFStatisticsGetStatisticsForEntitiesRequestWrapper(PFStatisticsGetStatisticsForEntitiesRequestWrapper&& src) :
        PFStatisticsGetStatisticsForEntitiesRequestWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsGetStatisticsForEntitiesRequestWrapper& operator=(PFStatisticsGetStatisticsForEntitiesRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsGetStatisticsForEntitiesRequestWrapper() = default;

    friend void swap(PFStatisticsGetStatisticsForEntitiesRequestWrapper& lhs, PFStatisticsGetStatisticsForEntitiesRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entities, rhs.m_entities);
        swap(lhs.m_statisticNames, rhs.m_statisticNames);
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

    ModelVector<PFEntityKeyWrapper<Alloc>, Alloc> const& GetEntities() const
    {
        return m_entities;
    }

    void SetEntities(ModelVector<PFEntityKeyWrapper<Alloc>, Alloc> value)
    {
        m_entities = std::move(value);
        this->m_model.entities =  m_entities.empty() ? nullptr : m_entities.data();
        this->m_model.entitiesCount =  static_cast<uint32_t>(m_entities.size());
    }

    CStringVector<Alloc> const& GetStatisticNames() const
    {
        return m_statisticNames;
    }

    void SetStatisticNames(CStringVector<Alloc> value)
    {
        m_statisticNames = std::move(value);
        this->m_model.statisticNames =  m_statisticNames.empty() ? nullptr : m_statisticNames.data();
        this->m_model.statisticNamesCount =  static_cast<uint32_t>(m_statisticNames.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entities = m_entities.empty() ? nullptr : m_entities.data();
        this->m_model.statisticNames = m_statisticNames.empty() ? nullptr : m_statisticNames.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    ModelVector<PFEntityKeyWrapper<Alloc>, Alloc> m_entities;
    CStringVector<Alloc> m_statisticNames;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsEntityStatisticsWrapper : public ModelWrapper<PFStatisticsEntityStatistics, Alloc>
{
public:
    using ModelType = PFStatisticsEntityStatistics;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsEntityStatisticsWrapper() = default;

    PFStatisticsEntityStatisticsWrapper(const PFStatisticsEntityStatistics& model) :
        ModelWrapper<PFStatisticsEntityStatistics, Alloc>{ model },
        m_entityKey{ model.entityKey ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entityKey } : std::nullopt },
        m_statistics{ model.statistics, model.statistics + model.statisticsCount }
    {
        SetModelPointers();
    }

    PFStatisticsEntityStatisticsWrapper(const PFStatisticsEntityStatisticsWrapper& src) :
        PFStatisticsEntityStatisticsWrapper{ src.Model() }
    {
    }

    PFStatisticsEntityStatisticsWrapper(PFStatisticsEntityStatisticsWrapper&& src) :
        PFStatisticsEntityStatisticsWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsEntityStatisticsWrapper& operator=(PFStatisticsEntityStatisticsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsEntityStatisticsWrapper() = default;

    friend void swap(PFStatisticsEntityStatisticsWrapper& lhs, PFStatisticsEntityStatisticsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_entityKey, rhs.m_entityKey);
        swap(lhs.m_statistics, rhs.m_statistics);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    std::optional<PFEntityKeyWrapper<Alloc>> const& GetEntityKey() const
    {
        return m_entityKey;
    }

    void SetEntityKey(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entityKey = std::move(value);
        this->m_model.entityKey = m_entityKey ? &m_entityKey->Model() : nullptr;
    }

    ModelVector<PFStatisticsEntityStatisticValueWrapper<Alloc>, Alloc> const& GetStatistics() const
    {
        return m_statistics;
    }

    void SetStatistics(ModelVector<PFStatisticsEntityStatisticValueWrapper<Alloc>, Alloc> value)
    {
        m_statistics = std::move(value);
        this->m_model.statistics =  m_statistics.empty() ? nullptr : m_statistics.data();
        this->m_model.statisticsCount =  static_cast<uint32_t>(m_statistics.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.entityKey = m_entityKey ?  &m_entityKey->Model() : nullptr;
        this->m_model.statistics = m_statistics.empty() ? nullptr : m_statistics.data();
    }

    std::optional<PFEntityKeyWrapper<Alloc>> m_entityKey;
    ModelVector<PFStatisticsEntityStatisticValueWrapper<Alloc>, Alloc> m_statistics;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsGetStatisticsForEntitiesResponseWrapper : public ModelWrapper<PFStatisticsGetStatisticsForEntitiesResponse, Alloc>
{
public:
    using ModelType = PFStatisticsGetStatisticsForEntitiesResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsGetStatisticsForEntitiesResponseWrapper() = default;

    PFStatisticsGetStatisticsForEntitiesResponseWrapper(const PFStatisticsGetStatisticsForEntitiesResponse& model) :
        ModelWrapper<PFStatisticsGetStatisticsForEntitiesResponse, Alloc>{ model },
        m_columnDetails{ model.columnDetails, model.columnDetails + model.columnDetailsCount },
        m_entitiesStatistics{ model.entitiesStatistics, model.entitiesStatistics + model.entitiesStatisticsCount }
    {
        SetModelPointers();
    }

    PFStatisticsGetStatisticsForEntitiesResponseWrapper(const PFStatisticsGetStatisticsForEntitiesResponseWrapper& src) :
        PFStatisticsGetStatisticsForEntitiesResponseWrapper{ src.Model() }
    {
    }

    PFStatisticsGetStatisticsForEntitiesResponseWrapper(PFStatisticsGetStatisticsForEntitiesResponseWrapper&& src) :
        PFStatisticsGetStatisticsForEntitiesResponseWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsGetStatisticsForEntitiesResponseWrapper& operator=(PFStatisticsGetStatisticsForEntitiesResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsGetStatisticsForEntitiesResponseWrapper() = default;

    friend void swap(PFStatisticsGetStatisticsForEntitiesResponseWrapper& lhs, PFStatisticsGetStatisticsForEntitiesResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_columnDetails, rhs.m_columnDetails);
        swap(lhs.m_entitiesStatistics, rhs.m_entitiesStatistics);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    ModelDictionaryEntryVector<PFStatisticsStatisticColumnCollectionWrapper<Alloc>, Alloc> const& GetColumnDetails() const
    {
        return m_columnDetails;
    }

    void SetColumnDetails(ModelDictionaryEntryVector<PFStatisticsStatisticColumnCollectionWrapper<Alloc>, Alloc> value)
    {
        m_columnDetails = std::move(value);
        this->m_model.columnDetails =  m_columnDetails.empty() ? nullptr : m_columnDetails.data();
        this->m_model.columnDetailsCount =  static_cast<uint32_t>(m_columnDetails.size());
    }

    ModelVector<PFStatisticsEntityStatisticsWrapper<Alloc>, Alloc> const& GetEntitiesStatistics() const
    {
        return m_entitiesStatistics;
    }

    void SetEntitiesStatistics(ModelVector<PFStatisticsEntityStatisticsWrapper<Alloc>, Alloc> value)
    {
        m_entitiesStatistics = std::move(value);
        this->m_model.entitiesStatistics =  m_entitiesStatistics.empty() ? nullptr : m_entitiesStatistics.data();
        this->m_model.entitiesStatisticsCount =  static_cast<uint32_t>(m_entitiesStatistics.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.columnDetails = m_columnDetails.empty() ? nullptr : m_columnDetails.data();
        this->m_model.entitiesStatistics = m_entitiesStatistics.empty() ? nullptr : m_entitiesStatistics.data();
    }

    ModelDictionaryEntryVector<PFStatisticsStatisticColumnCollectionWrapper<Alloc>, Alloc> m_columnDetails;
    ModelVector<PFStatisticsEntityStatisticsWrapper<Alloc>, Alloc> m_entitiesStatistics;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsIncrementStatisticVersionRequestWrapper : public ModelWrapper<PFStatisticsIncrementStatisticVersionRequest, Alloc>
{
public:
    using ModelType = PFStatisticsIncrementStatisticVersionRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsIncrementStatisticVersionRequestWrapper() = default;

    PFStatisticsIncrementStatisticVersionRequestWrapper(const PFStatisticsIncrementStatisticVersionRequest& model) :
        ModelWrapper<PFStatisticsIncrementStatisticVersionRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_name{ SafeString(model.name) }
    {
        SetModelPointers();
    }

    PFStatisticsIncrementStatisticVersionRequestWrapper(const PFStatisticsIncrementStatisticVersionRequestWrapper& src) :
        PFStatisticsIncrementStatisticVersionRequestWrapper{ src.Model() }
    {
    }

    PFStatisticsIncrementStatisticVersionRequestWrapper(PFStatisticsIncrementStatisticVersionRequestWrapper&& src) :
        PFStatisticsIncrementStatisticVersionRequestWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsIncrementStatisticVersionRequestWrapper& operator=(PFStatisticsIncrementStatisticVersionRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsIncrementStatisticVersionRequestWrapper() = default;

    friend void swap(PFStatisticsIncrementStatisticVersionRequestWrapper& lhs, PFStatisticsIncrementStatisticVersionRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_name, rhs.m_name);
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

    String const& GetName() const
    {
        return m_name;
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_name;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsIncrementStatisticVersionResponseWrapper : public ModelWrapper<PFStatisticsIncrementStatisticVersionResponse, Alloc>
{
public:
    using ModelType = PFStatisticsIncrementStatisticVersionResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    using ModelWrapper<PFStatisticsIncrementStatisticVersionResponse, Alloc>::ModelWrapper;

    uint32_t GetVersion() const
    {
        return this->m_model.version;
    }

    void SetVersion(uint32_t value)
    {
        this->m_model.version = value;
    }

private:
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsListStatisticDefinitionsRequestWrapper : public ModelWrapper<PFStatisticsListStatisticDefinitionsRequest, Alloc>
{
public:
    using ModelType = PFStatisticsListStatisticDefinitionsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsListStatisticDefinitionsRequestWrapper() = default;

    PFStatisticsListStatisticDefinitionsRequestWrapper(const PFStatisticsListStatisticDefinitionsRequest& model) :
        ModelWrapper<PFStatisticsListStatisticDefinitionsRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_pageSize{ model.pageSize ? std::optional<int32_t>{ *model.pageSize } : std::nullopt },
        m_skipToken{ SafeString(model.skipToken) }
    {
        SetModelPointers();
    }

    PFStatisticsListStatisticDefinitionsRequestWrapper(const PFStatisticsListStatisticDefinitionsRequestWrapper& src) :
        PFStatisticsListStatisticDefinitionsRequestWrapper{ src.Model() }
    {
    }

    PFStatisticsListStatisticDefinitionsRequestWrapper(PFStatisticsListStatisticDefinitionsRequestWrapper&& src) :
        PFStatisticsListStatisticDefinitionsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsListStatisticDefinitionsRequestWrapper& operator=(PFStatisticsListStatisticDefinitionsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsListStatisticDefinitionsRequestWrapper() = default;

    friend void swap(PFStatisticsListStatisticDefinitionsRequestWrapper& lhs, PFStatisticsListStatisticDefinitionsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_pageSize, rhs.m_pageSize);
        swap(lhs.m_skipToken, rhs.m_skipToken);
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

    std::optional<int32_t> const& GetPageSize() const
    {
        return m_pageSize;
    }

    void SetPageSize(std::optional<int32_t> value)
    {
        m_pageSize = std::move(value);
        this->m_model.pageSize = m_pageSize ? m_pageSize.operator->() : nullptr;
    }

    String const& GetSkipToken() const
    {
        return m_skipToken;
    }

    void SetSkipToken(String value)
    {
        m_skipToken = std::move(value);
        this->m_model.skipToken =  m_skipToken.empty() ? nullptr : m_skipToken.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.pageSize = m_pageSize ? m_pageSize.operator->() : nullptr;
        this->m_model.skipToken = m_skipToken.empty() ? nullptr : m_skipToken.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<int32_t> m_pageSize;
    String m_skipToken;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsStatisticDefinitionWrapper : public ModelWrapper<PFStatisticsStatisticDefinition, Alloc>
{
public:
    using ModelType = PFStatisticsStatisticDefinition;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsStatisticDefinitionWrapper() = default;

    PFStatisticsStatisticDefinitionWrapper(const PFStatisticsStatisticDefinition& model) :
        ModelWrapper<PFStatisticsStatisticDefinition, Alloc>{ model },
        m_aggregationDestinations{ model.aggregationDestinations, model.aggregationDestinations + model.aggregationDestinationsCount },
        m_aggregationSources{ model.aggregationSources, model.aggregationSources + model.aggregationSourcesCount },
        m_columns{ model.columns, model.columns + model.columnsCount },
        m_entityType{ SafeString(model.entityType) },
        m_eventEmissionConfig{ model.eventEmissionConfig ? std::optional<PFStatisticsStatisticsEventEmissionConfigWrapper<Alloc>>{ *model.eventEmissionConfig } : std::nullopt },
        m_lastResetTime{ model.lastResetTime ? std::optional<time_t>{ *model.lastResetTime } : std::nullopt },
        m_linkedLeaderboardNames{ model.linkedLeaderboardNames, model.linkedLeaderboardNames + model.linkedLeaderboardNamesCount },
        m_name{ SafeString(model.name) },
        m_versionConfiguration{ model.versionConfiguration ? std::optional<PFVersionConfigurationWrapper<Alloc>>{ *model.versionConfiguration } : std::nullopt }
    {
        SetModelPointers();
    }

    PFStatisticsStatisticDefinitionWrapper(const PFStatisticsStatisticDefinitionWrapper& src) :
        PFStatisticsStatisticDefinitionWrapper{ src.Model() }
    {
    }

    PFStatisticsStatisticDefinitionWrapper(PFStatisticsStatisticDefinitionWrapper&& src) :
        PFStatisticsStatisticDefinitionWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsStatisticDefinitionWrapper& operator=(PFStatisticsStatisticDefinitionWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsStatisticDefinitionWrapper() = default;

    friend void swap(PFStatisticsStatisticDefinitionWrapper& lhs, PFStatisticsStatisticDefinitionWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_aggregationDestinations, rhs.m_aggregationDestinations);
        swap(lhs.m_aggregationSources, rhs.m_aggregationSources);
        swap(lhs.m_columns, rhs.m_columns);
        swap(lhs.m_entityType, rhs.m_entityType);
        swap(lhs.m_eventEmissionConfig, rhs.m_eventEmissionConfig);
        swap(lhs.m_lastResetTime, rhs.m_lastResetTime);
        swap(lhs.m_linkedLeaderboardNames, rhs.m_linkedLeaderboardNames);
        swap(lhs.m_name, rhs.m_name);
        swap(lhs.m_versionConfiguration, rhs.m_versionConfiguration);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    CStringVector<Alloc> const& GetAggregationDestinations() const
    {
        return m_aggregationDestinations;
    }

    void SetAggregationDestinations(CStringVector<Alloc> value)
    {
        m_aggregationDestinations = std::move(value);
        this->m_model.aggregationDestinations =  m_aggregationDestinations.empty() ? nullptr : m_aggregationDestinations.data();
        this->m_model.aggregationDestinationsCount =  static_cast<uint32_t>(m_aggregationDestinations.size());
    }

    CStringVector<Alloc> const& GetAggregationSources() const
    {
        return m_aggregationSources;
    }

    void SetAggregationSources(CStringVector<Alloc> value)
    {
        m_aggregationSources = std::move(value);
        this->m_model.aggregationSources =  m_aggregationSources.empty() ? nullptr : m_aggregationSources.data();
        this->m_model.aggregationSourcesCount =  static_cast<uint32_t>(m_aggregationSources.size());
    }

    ModelVector<PFStatisticsStatisticColumnWrapper<Alloc>, Alloc> const& GetColumns() const
    {
        return m_columns;
    }

    void SetColumns(ModelVector<PFStatisticsStatisticColumnWrapper<Alloc>, Alloc> value)
    {
        m_columns = std::move(value);
        this->m_model.columns =  m_columns.empty() ? nullptr : m_columns.data();
        this->m_model.columnsCount =  static_cast<uint32_t>(m_columns.size());
    }

    time_t GetCreated() const
    {
        return this->m_model.created;
    }

    void SetCreated(time_t value)
    {
        this->m_model.created = value;
    }

    String const& GetEntityType() const
    {
        return m_entityType;
    }

    void SetEntityType(String value)
    {
        m_entityType = std::move(value);
        this->m_model.entityType =  m_entityType.empty() ? nullptr : m_entityType.data();
    }

    std::optional<PFStatisticsStatisticsEventEmissionConfigWrapper<Alloc>> const& GetEventEmissionConfig() const
    {
        return m_eventEmissionConfig;
    }

    void SetEventEmissionConfig(std::optional<PFStatisticsStatisticsEventEmissionConfigWrapper<Alloc>> value)
    {
        m_eventEmissionConfig = std::move(value);
        this->m_model.eventEmissionConfig = m_eventEmissionConfig ? &m_eventEmissionConfig->Model() : nullptr;
    }

    std::optional<time_t> const& GetLastResetTime() const
    {
        return m_lastResetTime;
    }

    void SetLastResetTime(std::optional<time_t> value)
    {
        m_lastResetTime = std::move(value);
        this->m_model.lastResetTime = m_lastResetTime ? m_lastResetTime.operator->() : nullptr;
    }

    CStringVector<Alloc> const& GetLinkedLeaderboardNames() const
    {
        return m_linkedLeaderboardNames;
    }

    void SetLinkedLeaderboardNames(CStringVector<Alloc> value)
    {
        m_linkedLeaderboardNames = std::move(value);
        this->m_model.linkedLeaderboardNames =  m_linkedLeaderboardNames.empty() ? nullptr : m_linkedLeaderboardNames.data();
        this->m_model.linkedLeaderboardNamesCount =  static_cast<uint32_t>(m_linkedLeaderboardNames.size());
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

    uint32_t GetVersion() const
    {
        return this->m_model.version;
    }

    void SetVersion(uint32_t value)
    {
        this->m_model.version = value;
    }

    std::optional<PFVersionConfigurationWrapper<Alloc>> const& GetVersionConfiguration() const
    {
        return m_versionConfiguration;
    }

    void SetVersionConfiguration(std::optional<PFVersionConfigurationWrapper<Alloc>> value)
    {
        m_versionConfiguration = std::move(value);
        this->m_model.versionConfiguration = m_versionConfiguration ? &m_versionConfiguration->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.aggregationDestinations = m_aggregationDestinations.empty() ? nullptr : m_aggregationDestinations.data();
        this->m_model.aggregationSources = m_aggregationSources.empty() ? nullptr : m_aggregationSources.data();
        this->m_model.columns = m_columns.empty() ? nullptr : m_columns.data();
        this->m_model.entityType = m_entityType.empty() ? nullptr : m_entityType.data();
        this->m_model.eventEmissionConfig = m_eventEmissionConfig ?  &m_eventEmissionConfig->Model() : nullptr;
        this->m_model.lastResetTime = m_lastResetTime ? m_lastResetTime.operator->() : nullptr;
        this->m_model.linkedLeaderboardNames = m_linkedLeaderboardNames.empty() ? nullptr : m_linkedLeaderboardNames.data();
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
        this->m_model.versionConfiguration = m_versionConfiguration ?  &m_versionConfiguration->Model() : nullptr;
    }

    CStringVector<Alloc> m_aggregationDestinations;
    CStringVector<Alloc> m_aggregationSources;
    ModelVector<PFStatisticsStatisticColumnWrapper<Alloc>, Alloc> m_columns;
    String m_entityType;
    std::optional<PFStatisticsStatisticsEventEmissionConfigWrapper<Alloc>> m_eventEmissionConfig;
    std::optional<time_t> m_lastResetTime;
    CStringVector<Alloc> m_linkedLeaderboardNames;
    String m_name;
    std::optional<PFVersionConfigurationWrapper<Alloc>> m_versionConfiguration;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsListStatisticDefinitionsResponseWrapper : public ModelWrapper<PFStatisticsListStatisticDefinitionsResponse, Alloc>
{
public:
    using ModelType = PFStatisticsListStatisticDefinitionsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsListStatisticDefinitionsResponseWrapper() = default;

    PFStatisticsListStatisticDefinitionsResponseWrapper(const PFStatisticsListStatisticDefinitionsResponse& model) :
        ModelWrapper<PFStatisticsListStatisticDefinitionsResponse, Alloc>{ model },
        m_skipToken{ SafeString(model.skipToken) },
        m_statisticDefinitions{ model.statisticDefinitions, model.statisticDefinitions + model.statisticDefinitionsCount }
    {
        SetModelPointers();
    }

    PFStatisticsListStatisticDefinitionsResponseWrapper(const PFStatisticsListStatisticDefinitionsResponseWrapper& src) :
        PFStatisticsListStatisticDefinitionsResponseWrapper{ src.Model() }
    {
    }

    PFStatisticsListStatisticDefinitionsResponseWrapper(PFStatisticsListStatisticDefinitionsResponseWrapper&& src) :
        PFStatisticsListStatisticDefinitionsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsListStatisticDefinitionsResponseWrapper& operator=(PFStatisticsListStatisticDefinitionsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsListStatisticDefinitionsResponseWrapper() = default;

    friend void swap(PFStatisticsListStatisticDefinitionsResponseWrapper& lhs, PFStatisticsListStatisticDefinitionsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_skipToken, rhs.m_skipToken);
        swap(lhs.m_statisticDefinitions, rhs.m_statisticDefinitions);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    int32_t GetPageSize() const
    {
        return this->m_model.pageSize;
    }

    void SetPageSize(int32_t value)
    {
        this->m_model.pageSize = value;
    }

    String const& GetSkipToken() const
    {
        return m_skipToken;
    }

    void SetSkipToken(String value)
    {
        m_skipToken = std::move(value);
        this->m_model.skipToken =  m_skipToken.empty() ? nullptr : m_skipToken.data();
    }

    ModelVector<PFStatisticsStatisticDefinitionWrapper<Alloc>, Alloc> const& GetStatisticDefinitions() const
    {
        return m_statisticDefinitions;
    }

    void SetStatisticDefinitions(ModelVector<PFStatisticsStatisticDefinitionWrapper<Alloc>, Alloc> value)
    {
        m_statisticDefinitions = std::move(value);
        this->m_model.statisticDefinitions =  m_statisticDefinitions.empty() ? nullptr : m_statisticDefinitions.data();
        this->m_model.statisticDefinitionsCount =  static_cast<uint32_t>(m_statisticDefinitions.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.skipToken = m_skipToken.empty() ? nullptr : m_skipToken.data();
        this->m_model.statisticDefinitions = m_statisticDefinitions.empty() ? nullptr : m_statisticDefinitions.data();
    }

    String m_skipToken;
    ModelVector<PFStatisticsStatisticDefinitionWrapper<Alloc>, Alloc> m_statisticDefinitions;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsUnlinkAggregationSourceFromStatisticRequestWrapper : public ModelWrapper<PFStatisticsUnlinkAggregationSourceFromStatisticRequest, Alloc>
{
public:
    using ModelType = PFStatisticsUnlinkAggregationSourceFromStatisticRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsUnlinkAggregationSourceFromStatisticRequestWrapper() = default;

    PFStatisticsUnlinkAggregationSourceFromStatisticRequestWrapper(const PFStatisticsUnlinkAggregationSourceFromStatisticRequest& model) :
        ModelWrapper<PFStatisticsUnlinkAggregationSourceFromStatisticRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_name{ SafeString(model.name) },
        m_sourceStatisticName{ SafeString(model.sourceStatisticName) }
    {
        SetModelPointers();
    }

    PFStatisticsUnlinkAggregationSourceFromStatisticRequestWrapper(const PFStatisticsUnlinkAggregationSourceFromStatisticRequestWrapper& src) :
        PFStatisticsUnlinkAggregationSourceFromStatisticRequestWrapper{ src.Model() }
    {
    }

    PFStatisticsUnlinkAggregationSourceFromStatisticRequestWrapper(PFStatisticsUnlinkAggregationSourceFromStatisticRequestWrapper&& src) :
        PFStatisticsUnlinkAggregationSourceFromStatisticRequestWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsUnlinkAggregationSourceFromStatisticRequestWrapper& operator=(PFStatisticsUnlinkAggregationSourceFromStatisticRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsUnlinkAggregationSourceFromStatisticRequestWrapper() = default;

    friend void swap(PFStatisticsUnlinkAggregationSourceFromStatisticRequestWrapper& lhs, PFStatisticsUnlinkAggregationSourceFromStatisticRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_name, rhs.m_name);
        swap(lhs.m_sourceStatisticName, rhs.m_sourceStatisticName);
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

    String const& GetName() const
    {
        return m_name;
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

    String const& GetSourceStatisticName() const
    {
        return m_sourceStatisticName;
    }

    void SetSourceStatisticName(String value)
    {
        m_sourceStatisticName = std::move(value);
        this->m_model.sourceStatisticName =  m_sourceStatisticName.empty() ? nullptr : m_sourceStatisticName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
        this->m_model.sourceStatisticName = m_sourceStatisticName.empty() ? nullptr : m_sourceStatisticName.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_name;
    String m_sourceStatisticName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsUpdateStatisticDefinitionRequestWrapper : public ModelWrapper<PFStatisticsUpdateStatisticDefinitionRequest, Alloc>
{
public:
    using ModelType = PFStatisticsUpdateStatisticDefinitionRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsUpdateStatisticDefinitionRequestWrapper() = default;

    PFStatisticsUpdateStatisticDefinitionRequestWrapper(const PFStatisticsUpdateStatisticDefinitionRequest& model) :
        ModelWrapper<PFStatisticsUpdateStatisticDefinitionRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_eventEmissionConfig{ model.eventEmissionConfig ? std::optional<PFStatisticsStatisticsEventEmissionConfigWrapper<Alloc>>{ *model.eventEmissionConfig } : std::nullopt },
        m_name{ SafeString(model.name) },
        m_versionConfiguration{ model.versionConfiguration ? std::optional<PFVersionConfigurationWrapper<Alloc>>{ *model.versionConfiguration } : std::nullopt }
    {
        SetModelPointers();
    }

    PFStatisticsUpdateStatisticDefinitionRequestWrapper(const PFStatisticsUpdateStatisticDefinitionRequestWrapper& src) :
        PFStatisticsUpdateStatisticDefinitionRequestWrapper{ src.Model() }
    {
    }

    PFStatisticsUpdateStatisticDefinitionRequestWrapper(PFStatisticsUpdateStatisticDefinitionRequestWrapper&& src) :
        PFStatisticsUpdateStatisticDefinitionRequestWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsUpdateStatisticDefinitionRequestWrapper& operator=(PFStatisticsUpdateStatisticDefinitionRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsUpdateStatisticDefinitionRequestWrapper() = default;

    friend void swap(PFStatisticsUpdateStatisticDefinitionRequestWrapper& lhs, PFStatisticsUpdateStatisticDefinitionRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_eventEmissionConfig, rhs.m_eventEmissionConfig);
        swap(lhs.m_name, rhs.m_name);
        swap(lhs.m_versionConfiguration, rhs.m_versionConfiguration);
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

    std::optional<PFStatisticsStatisticsEventEmissionConfigWrapper<Alloc>> const& GetEventEmissionConfig() const
    {
        return m_eventEmissionConfig;
    }

    void SetEventEmissionConfig(std::optional<PFStatisticsStatisticsEventEmissionConfigWrapper<Alloc>> value)
    {
        m_eventEmissionConfig = std::move(value);
        this->m_model.eventEmissionConfig = m_eventEmissionConfig ? &m_eventEmissionConfig->Model() : nullptr;
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

    std::optional<PFVersionConfigurationWrapper<Alloc>> const& GetVersionConfiguration() const
    {
        return m_versionConfiguration;
    }

    void SetVersionConfiguration(std::optional<PFVersionConfigurationWrapper<Alloc>> value)
    {
        m_versionConfiguration = std::move(value);
        this->m_model.versionConfiguration = m_versionConfiguration ? &m_versionConfiguration->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.eventEmissionConfig = m_eventEmissionConfig ?  &m_eventEmissionConfig->Model() : nullptr;
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
        this->m_model.versionConfiguration = m_versionConfiguration ?  &m_versionConfiguration->Model() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFStatisticsStatisticsEventEmissionConfigWrapper<Alloc>> m_eventEmissionConfig;
    String m_name;
    std::optional<PFVersionConfigurationWrapper<Alloc>> m_versionConfiguration;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsStatisticUpdateWrapper : public ModelWrapper<PFStatisticsStatisticUpdate, Alloc>
{
public:
    using ModelType = PFStatisticsStatisticUpdate;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsStatisticUpdateWrapper() = default;

    PFStatisticsStatisticUpdateWrapper(const PFStatisticsStatisticUpdate& model) :
        ModelWrapper<PFStatisticsStatisticUpdate, Alloc>{ model },
        m_metadata{ SafeString(model.metadata) },
        m_name{ SafeString(model.name) },
        m_scores{ model.scores, model.scores + model.scoresCount },
        m_version{ model.version ? std::optional<uint32_t>{ *model.version } : std::nullopt }
    {
        SetModelPointers();
    }

    PFStatisticsStatisticUpdateWrapper(const PFStatisticsStatisticUpdateWrapper& src) :
        PFStatisticsStatisticUpdateWrapper{ src.Model() }
    {
    }

    PFStatisticsStatisticUpdateWrapper(PFStatisticsStatisticUpdateWrapper&& src) :
        PFStatisticsStatisticUpdateWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsStatisticUpdateWrapper& operator=(PFStatisticsStatisticUpdateWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsStatisticUpdateWrapper() = default;

    friend void swap(PFStatisticsStatisticUpdateWrapper& lhs, PFStatisticsStatisticUpdateWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_metadata, rhs.m_metadata);
        swap(lhs.m_name, rhs.m_name);
        swap(lhs.m_scores, rhs.m_scores);
        swap(lhs.m_version, rhs.m_version);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetMetadata() const
    {
        return m_metadata;
    }

    void SetMetadata(String value)
    {
        m_metadata = std::move(value);
        this->m_model.metadata =  m_metadata.empty() ? nullptr : m_metadata.data();
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

    CStringVector<Alloc> const& GetScores() const
    {
        return m_scores;
    }

    void SetScores(CStringVector<Alloc> value)
    {
        m_scores = std::move(value);
        this->m_model.scores =  m_scores.empty() ? nullptr : m_scores.data();
        this->m_model.scoresCount =  static_cast<uint32_t>(m_scores.size());
    }

    std::optional<uint32_t> const& GetVersion() const
    {
        return m_version;
    }

    void SetVersion(std::optional<uint32_t> value)
    {
        m_version = std::move(value);
        this->m_model.version = m_version ? m_version.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.metadata = m_metadata.empty() ? nullptr : m_metadata.data();
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
        this->m_model.scores = m_scores.empty() ? nullptr : m_scores.data();
        this->m_model.version = m_version ? m_version.operator->() : nullptr;
    }

    String m_metadata;
    String m_name;
    CStringVector<Alloc> m_scores;
    std::optional<uint32_t> m_version;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsUpdateStatisticsRequestWrapper : public ModelWrapper<PFStatisticsUpdateStatisticsRequest, Alloc>
{
public:
    using ModelType = PFStatisticsUpdateStatisticsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsUpdateStatisticsRequestWrapper() = default;

    PFStatisticsUpdateStatisticsRequestWrapper(const PFStatisticsUpdateStatisticsRequest& model) :
        ModelWrapper<PFStatisticsUpdateStatisticsRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_statistics{ model.statistics, model.statistics + model.statisticsCount },
        m_transactionId{ SafeString(model.transactionId) }
    {
        SetModelPointers();
    }

    PFStatisticsUpdateStatisticsRequestWrapper(const PFStatisticsUpdateStatisticsRequestWrapper& src) :
        PFStatisticsUpdateStatisticsRequestWrapper{ src.Model() }
    {
    }

    PFStatisticsUpdateStatisticsRequestWrapper(PFStatisticsUpdateStatisticsRequestWrapper&& src) :
        PFStatisticsUpdateStatisticsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsUpdateStatisticsRequestWrapper& operator=(PFStatisticsUpdateStatisticsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsUpdateStatisticsRequestWrapper() = default;

    friend void swap(PFStatisticsUpdateStatisticsRequestWrapper& lhs, PFStatisticsUpdateStatisticsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_statistics, rhs.m_statistics);
        swap(lhs.m_transactionId, rhs.m_transactionId);
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

    std::optional<PFEntityKeyWrapper<Alloc>> const& GetEntity() const
    {
        return m_entity;
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    ModelVector<PFStatisticsStatisticUpdateWrapper<Alloc>, Alloc> const& GetStatistics() const
    {
        return m_statistics;
    }

    void SetStatistics(ModelVector<PFStatisticsStatisticUpdateWrapper<Alloc>, Alloc> value)
    {
        m_statistics = std::move(value);
        this->m_model.statistics =  m_statistics.empty() ? nullptr : m_statistics.data();
        this->m_model.statisticsCount =  static_cast<uint32_t>(m_statistics.size());
    }

    String const& GetTransactionId() const
    {
        return m_transactionId;
    }

    void SetTransactionId(String value)
    {
        m_transactionId = std::move(value);
        this->m_model.transactionId =  m_transactionId.empty() ? nullptr : m_transactionId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.statistics = m_statistics.empty() ? nullptr : m_statistics.data();
        this->m_model.transactionId = m_transactionId.empty() ? nullptr : m_transactionId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    ModelVector<PFStatisticsStatisticUpdateWrapper<Alloc>, Alloc> m_statistics;
    String m_transactionId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFStatisticsUpdateStatisticsResponseWrapper : public ModelWrapper<PFStatisticsUpdateStatisticsResponse, Alloc>
{
public:
    using ModelType = PFStatisticsUpdateStatisticsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFStatisticsUpdateStatisticsResponseWrapper() = default;

    PFStatisticsUpdateStatisticsResponseWrapper(const PFStatisticsUpdateStatisticsResponse& model) :
        ModelWrapper<PFStatisticsUpdateStatisticsResponse, Alloc>{ model },
        m_columnDetails{ model.columnDetails, model.columnDetails + model.columnDetailsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_statistics{ model.statistics, model.statistics + model.statisticsCount }
    {
        SetModelPointers();
    }

    PFStatisticsUpdateStatisticsResponseWrapper(const PFStatisticsUpdateStatisticsResponseWrapper& src) :
        PFStatisticsUpdateStatisticsResponseWrapper{ src.Model() }
    {
    }

    PFStatisticsUpdateStatisticsResponseWrapper(PFStatisticsUpdateStatisticsResponseWrapper&& src) :
        PFStatisticsUpdateStatisticsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFStatisticsUpdateStatisticsResponseWrapper& operator=(PFStatisticsUpdateStatisticsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFStatisticsUpdateStatisticsResponseWrapper() = default;

    friend void swap(PFStatisticsUpdateStatisticsResponseWrapper& lhs, PFStatisticsUpdateStatisticsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_columnDetails, rhs.m_columnDetails);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_statistics, rhs.m_statistics);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    ModelDictionaryEntryVector<PFStatisticsStatisticColumnCollectionWrapper<Alloc>, Alloc> const& GetColumnDetails() const
    {
        return m_columnDetails;
    }

    void SetColumnDetails(ModelDictionaryEntryVector<PFStatisticsStatisticColumnCollectionWrapper<Alloc>, Alloc> value)
    {
        m_columnDetails = std::move(value);
        this->m_model.columnDetails =  m_columnDetails.empty() ? nullptr : m_columnDetails.data();
        this->m_model.columnDetailsCount =  static_cast<uint32_t>(m_columnDetails.size());
    }

    std::optional<PFEntityKeyWrapper<Alloc>> const& GetEntity() const
    {
        return m_entity;
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    ModelDictionaryEntryVector<PFStatisticsEntityStatisticValueWrapper<Alloc>, Alloc> const& GetStatistics() const
    {
        return m_statistics;
    }

    void SetStatistics(ModelDictionaryEntryVector<PFStatisticsEntityStatisticValueWrapper<Alloc>, Alloc> value)
    {
        m_statistics = std::move(value);
        this->m_model.statistics =  m_statistics.empty() ? nullptr : m_statistics.data();
        this->m_model.statisticsCount =  static_cast<uint32_t>(m_statistics.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.columnDetails = m_columnDetails.empty() ? nullptr : m_columnDetails.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.statistics = m_statistics.empty() ? nullptr : m_statistics.data();
    }

    ModelDictionaryEntryVector<PFStatisticsStatisticColumnCollectionWrapper<Alloc>, Alloc> m_columnDetails;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    ModelDictionaryEntryVector<PFStatisticsEntityStatisticValueWrapper<Alloc>, Alloc> m_statistics;
};

} // namespace Wrappers
} // namespace PlayFab

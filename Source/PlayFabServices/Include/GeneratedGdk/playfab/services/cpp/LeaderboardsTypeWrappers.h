// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFLeaderboardsTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFLeaderboardsLinkedStatisticColumnWrapper : public ModelWrapper<PFLeaderboardsLinkedStatisticColumn, Alloc>
{
public:
    using ModelType = PFLeaderboardsLinkedStatisticColumn;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLeaderboardsLinkedStatisticColumnWrapper() = default;

    PFLeaderboardsLinkedStatisticColumnWrapper(const PFLeaderboardsLinkedStatisticColumn& model) :
        ModelWrapper<PFLeaderboardsLinkedStatisticColumn, Alloc>{ model },
        m_linkedStatisticColumnName{ SafeString(model.linkedStatisticColumnName) },
        m_linkedStatisticName{ SafeString(model.linkedStatisticName) }
    {
        SetModelPointers();
    }

    PFLeaderboardsLinkedStatisticColumnWrapper(const PFLeaderboardsLinkedStatisticColumnWrapper& src) :
        PFLeaderboardsLinkedStatisticColumnWrapper{ src.Model() }
    {
    }

    PFLeaderboardsLinkedStatisticColumnWrapper(PFLeaderboardsLinkedStatisticColumnWrapper&& src) :
        PFLeaderboardsLinkedStatisticColumnWrapper{}
    {
        swap(*this, src);
    }

    PFLeaderboardsLinkedStatisticColumnWrapper& operator=(PFLeaderboardsLinkedStatisticColumnWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLeaderboardsLinkedStatisticColumnWrapper() = default;

    friend void swap(PFLeaderboardsLinkedStatisticColumnWrapper& lhs, PFLeaderboardsLinkedStatisticColumnWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_linkedStatisticColumnName, rhs.m_linkedStatisticColumnName);
        swap(lhs.m_linkedStatisticName, rhs.m_linkedStatisticName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetLinkedStatisticColumnName(String value)
    {
        m_linkedStatisticColumnName = std::move(value);
        this->m_model.linkedStatisticColumnName =  m_linkedStatisticColumnName.empty() ? nullptr : m_linkedStatisticColumnName.data();
    }

    void SetLinkedStatisticName(String value)
    {
        m_linkedStatisticName = std::move(value);
        this->m_model.linkedStatisticName =  m_linkedStatisticName.empty() ? nullptr : m_linkedStatisticName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.linkedStatisticColumnName = m_linkedStatisticColumnName.empty() ? nullptr : m_linkedStatisticColumnName.data();
        this->m_model.linkedStatisticName = m_linkedStatisticName.empty() ? nullptr : m_linkedStatisticName.data();
    }

    String m_linkedStatisticColumnName;
    String m_linkedStatisticName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFLeaderboardsLeaderboardColumnWrapper : public ModelWrapper<PFLeaderboardsLeaderboardColumn, Alloc>
{
public:
    using ModelType = PFLeaderboardsLeaderboardColumn;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLeaderboardsLeaderboardColumnWrapper() = default;

    PFLeaderboardsLeaderboardColumnWrapper(const PFLeaderboardsLeaderboardColumn& model) :
        ModelWrapper<PFLeaderboardsLeaderboardColumn, Alloc>{ model },
        m_linkedStatisticColumn{ model.linkedStatisticColumn ? std::optional<PFLeaderboardsLinkedStatisticColumnWrapper<Alloc>>{ *model.linkedStatisticColumn } : std::nullopt },
        m_name{ SafeString(model.name) }
    {
        SetModelPointers();
    }

    PFLeaderboardsLeaderboardColumnWrapper(const PFLeaderboardsLeaderboardColumnWrapper& src) :
        PFLeaderboardsLeaderboardColumnWrapper{ src.Model() }
    {
    }

    PFLeaderboardsLeaderboardColumnWrapper(PFLeaderboardsLeaderboardColumnWrapper&& src) :
        PFLeaderboardsLeaderboardColumnWrapper{}
    {
        swap(*this, src);
    }

    PFLeaderboardsLeaderboardColumnWrapper& operator=(PFLeaderboardsLeaderboardColumnWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLeaderboardsLeaderboardColumnWrapper() = default;

    friend void swap(PFLeaderboardsLeaderboardColumnWrapper& lhs, PFLeaderboardsLeaderboardColumnWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_linkedStatisticColumn, rhs.m_linkedStatisticColumn);
        swap(lhs.m_name, rhs.m_name);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetLinkedStatisticColumn(std::optional<PFLeaderboardsLinkedStatisticColumnWrapper<Alloc>> value)
    {
        m_linkedStatisticColumn = std::move(value);
        this->m_model.linkedStatisticColumn = m_linkedStatisticColumn ? &m_linkedStatisticColumn->Model() : nullptr;
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

    void SetSortDirection(PFLeaderboardsLeaderboardSortDirection value)
    {
        this->m_model.sortDirection = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.linkedStatisticColumn = m_linkedStatisticColumn ?  &m_linkedStatisticColumn->Model() : nullptr;
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
    }

    std::optional<PFLeaderboardsLinkedStatisticColumnWrapper<Alloc>> m_linkedStatisticColumn;
    String m_name;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFLeaderboardsCreateLeaderboardDefinitionRequestWrapper : public ModelWrapper<PFLeaderboardsCreateLeaderboardDefinitionRequest, Alloc>
{
public:
    using ModelType = PFLeaderboardsCreateLeaderboardDefinitionRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLeaderboardsCreateLeaderboardDefinitionRequestWrapper() = default;

    PFLeaderboardsCreateLeaderboardDefinitionRequestWrapper(const PFLeaderboardsCreateLeaderboardDefinitionRequest& model) :
        ModelWrapper<PFLeaderboardsCreateLeaderboardDefinitionRequest, Alloc>{ model },
        m_columns{ model.columns, model.columns + model.columnsCount },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entityType{ SafeString(model.entityType) },
        m_name{ SafeString(model.name) },
        m_versionConfiguration{ model.versionConfiguration ? *model.versionConfiguration : decltype(*model.versionConfiguration){} }
    {
        SetModelPointers();
    }

    PFLeaderboardsCreateLeaderboardDefinitionRequestWrapper(const PFLeaderboardsCreateLeaderboardDefinitionRequestWrapper& src) :
        PFLeaderboardsCreateLeaderboardDefinitionRequestWrapper{ src.Model() }
    {
    }

    PFLeaderboardsCreateLeaderboardDefinitionRequestWrapper(PFLeaderboardsCreateLeaderboardDefinitionRequestWrapper&& src) :
        PFLeaderboardsCreateLeaderboardDefinitionRequestWrapper{}
    {
        swap(*this, src);
    }

    PFLeaderboardsCreateLeaderboardDefinitionRequestWrapper& operator=(PFLeaderboardsCreateLeaderboardDefinitionRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLeaderboardsCreateLeaderboardDefinitionRequestWrapper() = default;

    friend void swap(PFLeaderboardsCreateLeaderboardDefinitionRequestWrapper& lhs, PFLeaderboardsCreateLeaderboardDefinitionRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_columns, rhs.m_columns);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entityType, rhs.m_entityType);
        swap(lhs.m_name, rhs.m_name);
        swap(lhs.m_versionConfiguration, rhs.m_versionConfiguration);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetColumns(ModelVector<PFLeaderboardsLeaderboardColumnWrapper<Alloc>, Alloc> value)
    {
        m_columns = std::move(value);
        this->m_model.columns =  m_columns.empty() ? nullptr : m_columns.data();
        this->m_model.columnsCount =  static_cast<uint32_t>(m_columns.size());
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntityType(String value)
    {
        m_entityType = std::move(value);
        this->m_model.entityType =  m_entityType.empty() ? nullptr : m_entityType.data();
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

    void SetSizeLimit(int32_t value)
    {
        this->m_model.sizeLimit = value;
    }

    void SetVersionConfiguration(PFVersionConfigurationWrapper<Alloc> value)
    {
        m_versionConfiguration = std::move(value);
        this->m_model.versionConfiguration = &m_versionConfiguration.Model();
    }

private:
    void SetModelPointers()
    {
        this->m_model.columns = m_columns.empty() ? nullptr : m_columns.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entityType = m_entityType.empty() ? nullptr : m_entityType.data();
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
        this->m_model.versionConfiguration = &m_versionConfiguration.Model();
    }

    ModelVector<PFLeaderboardsLeaderboardColumnWrapper<Alloc>, Alloc> m_columns;
    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_entityType;
    String m_name;
    PFVersionConfigurationWrapper<Alloc> m_versionConfiguration;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFLeaderboardsDeleteLeaderboardDefinitionRequestWrapper : public ModelWrapper<PFLeaderboardsDeleteLeaderboardDefinitionRequest, Alloc>
{
public:
    using ModelType = PFLeaderboardsDeleteLeaderboardDefinitionRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLeaderboardsDeleteLeaderboardDefinitionRequestWrapper() = default;

    PFLeaderboardsDeleteLeaderboardDefinitionRequestWrapper(const PFLeaderboardsDeleteLeaderboardDefinitionRequest& model) :
        ModelWrapper<PFLeaderboardsDeleteLeaderboardDefinitionRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_name{ SafeString(model.name) }
    {
        SetModelPointers();
    }

    PFLeaderboardsDeleteLeaderboardDefinitionRequestWrapper(const PFLeaderboardsDeleteLeaderboardDefinitionRequestWrapper& src) :
        PFLeaderboardsDeleteLeaderboardDefinitionRequestWrapper{ src.Model() }
    {
    }

    PFLeaderboardsDeleteLeaderboardDefinitionRequestWrapper(PFLeaderboardsDeleteLeaderboardDefinitionRequestWrapper&& src) :
        PFLeaderboardsDeleteLeaderboardDefinitionRequestWrapper{}
    {
        swap(*this, src);
    }

    PFLeaderboardsDeleteLeaderboardDefinitionRequestWrapper& operator=(PFLeaderboardsDeleteLeaderboardDefinitionRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLeaderboardsDeleteLeaderboardDefinitionRequestWrapper() = default;

    friend void swap(PFLeaderboardsDeleteLeaderboardDefinitionRequestWrapper& lhs, PFLeaderboardsDeleteLeaderboardDefinitionRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_name, rhs.m_name);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
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
class PFLeaderboardsDeleteLeaderboardEntriesRequestWrapper : public ModelWrapper<PFLeaderboardsDeleteLeaderboardEntriesRequest, Alloc>
{
public:
    using ModelType = PFLeaderboardsDeleteLeaderboardEntriesRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLeaderboardsDeleteLeaderboardEntriesRequestWrapper() = default;

    PFLeaderboardsDeleteLeaderboardEntriesRequestWrapper(const PFLeaderboardsDeleteLeaderboardEntriesRequest& model) :
        ModelWrapper<PFLeaderboardsDeleteLeaderboardEntriesRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entityIds{ model.entityIds, model.entityIds + model.entityIdsCount },
        m_name{ SafeString(model.name) }
    {
        SetModelPointers();
    }

    PFLeaderboardsDeleteLeaderboardEntriesRequestWrapper(const PFLeaderboardsDeleteLeaderboardEntriesRequestWrapper& src) :
        PFLeaderboardsDeleteLeaderboardEntriesRequestWrapper{ src.Model() }
    {
    }

    PFLeaderboardsDeleteLeaderboardEntriesRequestWrapper(PFLeaderboardsDeleteLeaderboardEntriesRequestWrapper&& src) :
        PFLeaderboardsDeleteLeaderboardEntriesRequestWrapper{}
    {
        swap(*this, src);
    }

    PFLeaderboardsDeleteLeaderboardEntriesRequestWrapper& operator=(PFLeaderboardsDeleteLeaderboardEntriesRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLeaderboardsDeleteLeaderboardEntriesRequestWrapper() = default;

    friend void swap(PFLeaderboardsDeleteLeaderboardEntriesRequestWrapper& lhs, PFLeaderboardsDeleteLeaderboardEntriesRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entityIds, rhs.m_entityIds);
        swap(lhs.m_name, rhs.m_name);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntityIds(CStringVector<Alloc> value)
    {
        m_entityIds = std::move(value);
        this->m_model.entityIds =  m_entityIds.empty() ? nullptr : m_entityIds.data();
        this->m_model.entityIdsCount =  static_cast<uint32_t>(m_entityIds.size());
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
        this->m_model.entityIds = m_entityIds.empty() ? nullptr : m_entityIds.data();
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    CStringVector<Alloc> m_entityIds;
    String m_name;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFLeaderboardsGetFriendLeaderboardForEntityRequestWrapper : public ModelWrapper<PFLeaderboardsGetFriendLeaderboardForEntityRequest, Alloc>
{
public:
    using ModelType = PFLeaderboardsGetFriendLeaderboardForEntityRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLeaderboardsGetFriendLeaderboardForEntityRequestWrapper() = default;

    PFLeaderboardsGetFriendLeaderboardForEntityRequestWrapper(const PFLeaderboardsGetFriendLeaderboardForEntityRequest& model) :
        ModelWrapper<PFLeaderboardsGetFriendLeaderboardForEntityRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_externalFriendSources{ model.externalFriendSources ? std::optional<PFExternalFriendSources>{ *model.externalFriendSources } : std::nullopt },
        m_leaderboardName{ SafeString(model.leaderboardName) },
        m_version{ model.version ? std::optional<uint32_t>{ *model.version } : std::nullopt },
        m_xboxToken{ SafeString(model.xboxToken) }
    {
        SetModelPointers();
    }

    PFLeaderboardsGetFriendLeaderboardForEntityRequestWrapper(const PFLeaderboardsGetFriendLeaderboardForEntityRequestWrapper& src) :
        PFLeaderboardsGetFriendLeaderboardForEntityRequestWrapper{ src.Model() }
    {
    }

    PFLeaderboardsGetFriendLeaderboardForEntityRequestWrapper(PFLeaderboardsGetFriendLeaderboardForEntityRequestWrapper&& src) :
        PFLeaderboardsGetFriendLeaderboardForEntityRequestWrapper{}
    {
        swap(*this, src);
    }

    PFLeaderboardsGetFriendLeaderboardForEntityRequestWrapper& operator=(PFLeaderboardsGetFriendLeaderboardForEntityRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLeaderboardsGetFriendLeaderboardForEntityRequestWrapper() = default;

    friend void swap(PFLeaderboardsGetFriendLeaderboardForEntityRequestWrapper& lhs, PFLeaderboardsGetFriendLeaderboardForEntityRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_externalFriendSources, rhs.m_externalFriendSources);
        swap(lhs.m_leaderboardName, rhs.m_leaderboardName);
        swap(lhs.m_version, rhs.m_version);
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

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    void SetExternalFriendSources(std::optional<PFExternalFriendSources> value)
    {
        m_externalFriendSources = std::move(value);
        this->m_model.externalFriendSources = m_externalFriendSources ? m_externalFriendSources.operator->() : nullptr;
    }

    void SetLeaderboardName(String value)
    {
        m_leaderboardName = std::move(value);
        this->m_model.leaderboardName =  m_leaderboardName.empty() ? nullptr : m_leaderboardName.data();
    }

    void SetVersion(std::optional<uint32_t> value)
    {
        m_version = std::move(value);
        this->m_model.version = m_version ? m_version.operator->() : nullptr;
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
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.externalFriendSources = m_externalFriendSources ? m_externalFriendSources.operator->() : nullptr;
        this->m_model.leaderboardName = m_leaderboardName.empty() ? nullptr : m_leaderboardName.data();
        this->m_model.version = m_version ? m_version.operator->() : nullptr;
        this->m_model.xboxToken = m_xboxToken.empty() ? nullptr : m_xboxToken.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    std::optional<PFExternalFriendSources> m_externalFriendSources;
    String m_leaderboardName;
    std::optional<uint32_t> m_version;
    String m_xboxToken;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFLeaderboardsEntityLeaderboardEntryWrapper : public ModelWrapper<PFLeaderboardsEntityLeaderboardEntry, Alloc>
{
public:
    using ModelType = PFLeaderboardsEntityLeaderboardEntry;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLeaderboardsEntityLeaderboardEntryWrapper() = default;

    PFLeaderboardsEntityLeaderboardEntryWrapper(const PFLeaderboardsEntityLeaderboardEntry& model) :
        ModelWrapper<PFLeaderboardsEntityLeaderboardEntry, Alloc>{ model },
        m_displayName{ SafeString(model.displayName) },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_metadata{ SafeString(model.metadata) },
        m_scores{ model.scores, model.scores + model.scoresCount }
    {
        SetModelPointers();
    }

    PFLeaderboardsEntityLeaderboardEntryWrapper(const PFLeaderboardsEntityLeaderboardEntryWrapper& src) :
        PFLeaderboardsEntityLeaderboardEntryWrapper{ src.Model() }
    {
    }

    PFLeaderboardsEntityLeaderboardEntryWrapper(PFLeaderboardsEntityLeaderboardEntryWrapper&& src) :
        PFLeaderboardsEntityLeaderboardEntryWrapper{}
    {
        swap(*this, src);
    }

    PFLeaderboardsEntityLeaderboardEntryWrapper& operator=(PFLeaderboardsEntityLeaderboardEntryWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLeaderboardsEntityLeaderboardEntryWrapper() = default;

    friend void swap(PFLeaderboardsEntityLeaderboardEntryWrapper& lhs, PFLeaderboardsEntityLeaderboardEntryWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_displayName, rhs.m_displayName);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_metadata, rhs.m_metadata);
        swap(lhs.m_scores, rhs.m_scores);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetDisplayName(String value)
    {
        m_displayName = std::move(value);
        this->m_model.displayName =  m_displayName.empty() ? nullptr : m_displayName.data();
    }

    void SetEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_entity = std::move(value);
        this->m_model.entity = m_entity ? &m_entity->Model() : nullptr;
    }

    void SetLastUpdated(time_t value)
    {
        this->m_model.lastUpdated = value;
    }

    void SetMetadata(String value)
    {
        m_metadata = std::move(value);
        this->m_model.metadata =  m_metadata.empty() ? nullptr : m_metadata.data();
    }

    void SetRank(int32_t value)
    {
        this->m_model.rank = value;
    }

    void SetScores(CStringVector<Alloc> value)
    {
        m_scores = std::move(value);
        this->m_model.scores =  m_scores.empty() ? nullptr : m_scores.data();
        this->m_model.scoresCount =  static_cast<uint32_t>(m_scores.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.displayName = m_displayName.empty() ? nullptr : m_displayName.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.metadata = m_metadata.empty() ? nullptr : m_metadata.data();
        this->m_model.scores = m_scores.empty() ? nullptr : m_scores.data();
    }

    String m_displayName;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_metadata;
    CStringVector<Alloc> m_scores;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFLeaderboardsGetEntityLeaderboardResponseWrapper : public ModelWrapper<PFLeaderboardsGetEntityLeaderboardResponse, Alloc>
{
public:
    using ModelType = PFLeaderboardsGetEntityLeaderboardResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLeaderboardsGetEntityLeaderboardResponseWrapper() = default;

    PFLeaderboardsGetEntityLeaderboardResponseWrapper(const PFLeaderboardsGetEntityLeaderboardResponse& model) :
        ModelWrapper<PFLeaderboardsGetEntityLeaderboardResponse, Alloc>{ model },
        m_columns{ model.columns, model.columns + model.columnsCount },
        m_rankings{ model.rankings, model.rankings + model.rankingsCount }
    {
        SetModelPointers();
    }

    PFLeaderboardsGetEntityLeaderboardResponseWrapper(const PFLeaderboardsGetEntityLeaderboardResponseWrapper& src) :
        PFLeaderboardsGetEntityLeaderboardResponseWrapper{ src.Model() }
    {
    }

    PFLeaderboardsGetEntityLeaderboardResponseWrapper(PFLeaderboardsGetEntityLeaderboardResponseWrapper&& src) :
        PFLeaderboardsGetEntityLeaderboardResponseWrapper{}
    {
        swap(*this, src);
    }

    PFLeaderboardsGetEntityLeaderboardResponseWrapper& operator=(PFLeaderboardsGetEntityLeaderboardResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLeaderboardsGetEntityLeaderboardResponseWrapper() = default;

    friend void swap(PFLeaderboardsGetEntityLeaderboardResponseWrapper& lhs, PFLeaderboardsGetEntityLeaderboardResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_columns, rhs.m_columns);
        swap(lhs.m_rankings, rhs.m_rankings);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetColumns(ModelVector<PFLeaderboardsLeaderboardColumnWrapper<Alloc>, Alloc> value)
    {
        m_columns = std::move(value);
        this->m_model.columns =  m_columns.empty() ? nullptr : m_columns.data();
        this->m_model.columnsCount =  static_cast<uint32_t>(m_columns.size());
    }

    void SetRankings(ModelVector<PFLeaderboardsEntityLeaderboardEntryWrapper<Alloc>, Alloc> value)
    {
        m_rankings = std::move(value);
        this->m_model.rankings =  m_rankings.empty() ? nullptr : m_rankings.data();
        this->m_model.rankingsCount =  static_cast<uint32_t>(m_rankings.size());
    }

    void SetVersion(uint32_t value)
    {
        this->m_model.version = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.columns = m_columns.empty() ? nullptr : m_columns.data();
        this->m_model.rankings = m_rankings.empty() ? nullptr : m_rankings.data();
    }

    ModelVector<PFLeaderboardsLeaderboardColumnWrapper<Alloc>, Alloc> m_columns;
    ModelVector<PFLeaderboardsEntityLeaderboardEntryWrapper<Alloc>, Alloc> m_rankings;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFLeaderboardsGetEntityLeaderboardRequestWrapper : public ModelWrapper<PFLeaderboardsGetEntityLeaderboardRequest, Alloc>
{
public:
    using ModelType = PFLeaderboardsGetEntityLeaderboardRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLeaderboardsGetEntityLeaderboardRequestWrapper() = default;

    PFLeaderboardsGetEntityLeaderboardRequestWrapper(const PFLeaderboardsGetEntityLeaderboardRequest& model) :
        ModelWrapper<PFLeaderboardsGetEntityLeaderboardRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_leaderboardName{ SafeString(model.leaderboardName) },
        m_startingPosition{ model.startingPosition ? std::optional<uint32_t>{ *model.startingPosition } : std::nullopt },
        m_version{ model.version ? std::optional<uint32_t>{ *model.version } : std::nullopt }
    {
        SetModelPointers();
    }

    PFLeaderboardsGetEntityLeaderboardRequestWrapper(const PFLeaderboardsGetEntityLeaderboardRequestWrapper& src) :
        PFLeaderboardsGetEntityLeaderboardRequestWrapper{ src.Model() }
    {
    }

    PFLeaderboardsGetEntityLeaderboardRequestWrapper(PFLeaderboardsGetEntityLeaderboardRequestWrapper&& src) :
        PFLeaderboardsGetEntityLeaderboardRequestWrapper{}
    {
        swap(*this, src);
    }

    PFLeaderboardsGetEntityLeaderboardRequestWrapper& operator=(PFLeaderboardsGetEntityLeaderboardRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLeaderboardsGetEntityLeaderboardRequestWrapper() = default;

    friend void swap(PFLeaderboardsGetEntityLeaderboardRequestWrapper& lhs, PFLeaderboardsGetEntityLeaderboardRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_leaderboardName, rhs.m_leaderboardName);
        swap(lhs.m_startingPosition, rhs.m_startingPosition);
        swap(lhs.m_version, rhs.m_version);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetLeaderboardName(String value)
    {
        m_leaderboardName = std::move(value);
        this->m_model.leaderboardName =  m_leaderboardName.empty() ? nullptr : m_leaderboardName.data();
    }

    void SetPageSize(uint32_t value)
    {
        this->m_model.pageSize = value;
    }

    void SetStartingPosition(std::optional<uint32_t> value)
    {
        m_startingPosition = std::move(value);
        this->m_model.startingPosition = m_startingPosition ? m_startingPosition.operator->() : nullptr;
    }

    void SetVersion(std::optional<uint32_t> value)
    {
        m_version = std::move(value);
        this->m_model.version = m_version ? m_version.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.leaderboardName = m_leaderboardName.empty() ? nullptr : m_leaderboardName.data();
        this->m_model.startingPosition = m_startingPosition ? m_startingPosition.operator->() : nullptr;
        this->m_model.version = m_version ? m_version.operator->() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_leaderboardName;
    std::optional<uint32_t> m_startingPosition;
    std::optional<uint32_t> m_version;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFLeaderboardsGetLeaderboardAroundEntityRequestWrapper : public ModelWrapper<PFLeaderboardsGetLeaderboardAroundEntityRequest, Alloc>
{
public:
    using ModelType = PFLeaderboardsGetLeaderboardAroundEntityRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLeaderboardsGetLeaderboardAroundEntityRequestWrapper() = default;

    PFLeaderboardsGetLeaderboardAroundEntityRequestWrapper(const PFLeaderboardsGetLeaderboardAroundEntityRequest& model) :
        ModelWrapper<PFLeaderboardsGetLeaderboardAroundEntityRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_leaderboardName{ SafeString(model.leaderboardName) },
        m_version{ model.version ? std::optional<uint32_t>{ *model.version } : std::nullopt }
    {
        SetModelPointers();
    }

    PFLeaderboardsGetLeaderboardAroundEntityRequestWrapper(const PFLeaderboardsGetLeaderboardAroundEntityRequestWrapper& src) :
        PFLeaderboardsGetLeaderboardAroundEntityRequestWrapper{ src.Model() }
    {
    }

    PFLeaderboardsGetLeaderboardAroundEntityRequestWrapper(PFLeaderboardsGetLeaderboardAroundEntityRequestWrapper&& src) :
        PFLeaderboardsGetLeaderboardAroundEntityRequestWrapper{}
    {
        swap(*this, src);
    }

    PFLeaderboardsGetLeaderboardAroundEntityRequestWrapper& operator=(PFLeaderboardsGetLeaderboardAroundEntityRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLeaderboardsGetLeaderboardAroundEntityRequestWrapper() = default;

    friend void swap(PFLeaderboardsGetLeaderboardAroundEntityRequestWrapper& lhs, PFLeaderboardsGetLeaderboardAroundEntityRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_leaderboardName, rhs.m_leaderboardName);
        swap(lhs.m_version, rhs.m_version);
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

    void SetLeaderboardName(String value)
    {
        m_leaderboardName = std::move(value);
        this->m_model.leaderboardName =  m_leaderboardName.empty() ? nullptr : m_leaderboardName.data();
    }

    void SetMaxSurroundingEntries(uint32_t value)
    {
        this->m_model.maxSurroundingEntries = value;
    }

    void SetVersion(std::optional<uint32_t> value)
    {
        m_version = std::move(value);
        this->m_model.version = m_version ? m_version.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.leaderboardName = m_leaderboardName.empty() ? nullptr : m_leaderboardName.data();
        this->m_model.version = m_version ? m_version.operator->() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_leaderboardName;
    std::optional<uint32_t> m_version;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFLeaderboardsGetLeaderboardDefinitionRequestWrapper : public ModelWrapper<PFLeaderboardsGetLeaderboardDefinitionRequest, Alloc>
{
public:
    using ModelType = PFLeaderboardsGetLeaderboardDefinitionRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLeaderboardsGetLeaderboardDefinitionRequestWrapper() = default;

    PFLeaderboardsGetLeaderboardDefinitionRequestWrapper(const PFLeaderboardsGetLeaderboardDefinitionRequest& model) :
        ModelWrapper<PFLeaderboardsGetLeaderboardDefinitionRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_name{ SafeString(model.name) }
    {
        SetModelPointers();
    }

    PFLeaderboardsGetLeaderboardDefinitionRequestWrapper(const PFLeaderboardsGetLeaderboardDefinitionRequestWrapper& src) :
        PFLeaderboardsGetLeaderboardDefinitionRequestWrapper{ src.Model() }
    {
    }

    PFLeaderboardsGetLeaderboardDefinitionRequestWrapper(PFLeaderboardsGetLeaderboardDefinitionRequestWrapper&& src) :
        PFLeaderboardsGetLeaderboardDefinitionRequestWrapper{}
    {
        swap(*this, src);
    }

    PFLeaderboardsGetLeaderboardDefinitionRequestWrapper& operator=(PFLeaderboardsGetLeaderboardDefinitionRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLeaderboardsGetLeaderboardDefinitionRequestWrapper() = default;

    friend void swap(PFLeaderboardsGetLeaderboardDefinitionRequestWrapper& lhs, PFLeaderboardsGetLeaderboardDefinitionRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_name, rhs.m_name);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
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
class PFLeaderboardsGetLeaderboardDefinitionResponseWrapper : public ModelWrapper<PFLeaderboardsGetLeaderboardDefinitionResponse, Alloc>
{
public:
    using ModelType = PFLeaderboardsGetLeaderboardDefinitionResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLeaderboardsGetLeaderboardDefinitionResponseWrapper() = default;

    PFLeaderboardsGetLeaderboardDefinitionResponseWrapper(const PFLeaderboardsGetLeaderboardDefinitionResponse& model) :
        ModelWrapper<PFLeaderboardsGetLeaderboardDefinitionResponse, Alloc>{ model },
        m_columns{ model.columns, model.columns + model.columnsCount },
        m_entityType{ SafeString(model.entityType) },
        m_lastResetTime{ model.lastResetTime ? std::optional<time_t>{ *model.lastResetTime } : std::nullopt },
        m_name{ SafeString(model.name) },
        m_versionConfiguration{ model.versionConfiguration ? *model.versionConfiguration : decltype(*model.versionConfiguration){} }
    {
        SetModelPointers();
    }

    PFLeaderboardsGetLeaderboardDefinitionResponseWrapper(const PFLeaderboardsGetLeaderboardDefinitionResponseWrapper& src) :
        PFLeaderboardsGetLeaderboardDefinitionResponseWrapper{ src.Model() }
    {
    }

    PFLeaderboardsGetLeaderboardDefinitionResponseWrapper(PFLeaderboardsGetLeaderboardDefinitionResponseWrapper&& src) :
        PFLeaderboardsGetLeaderboardDefinitionResponseWrapper{}
    {
        swap(*this, src);
    }

    PFLeaderboardsGetLeaderboardDefinitionResponseWrapper& operator=(PFLeaderboardsGetLeaderboardDefinitionResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLeaderboardsGetLeaderboardDefinitionResponseWrapper() = default;

    friend void swap(PFLeaderboardsGetLeaderboardDefinitionResponseWrapper& lhs, PFLeaderboardsGetLeaderboardDefinitionResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_columns, rhs.m_columns);
        swap(lhs.m_entityType, rhs.m_entityType);
        swap(lhs.m_lastResetTime, rhs.m_lastResetTime);
        swap(lhs.m_name, rhs.m_name);
        swap(lhs.m_versionConfiguration, rhs.m_versionConfiguration);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetColumns(ModelVector<PFLeaderboardsLeaderboardColumnWrapper<Alloc>, Alloc> value)
    {
        m_columns = std::move(value);
        this->m_model.columns =  m_columns.empty() ? nullptr : m_columns.data();
        this->m_model.columnsCount =  static_cast<uint32_t>(m_columns.size());
    }

    void SetCreated(time_t value)
    {
        this->m_model.created = value;
    }

    void SetEntityType(String value)
    {
        m_entityType = std::move(value);
        this->m_model.entityType =  m_entityType.empty() ? nullptr : m_entityType.data();
    }

    void SetLastResetTime(std::optional<time_t> value)
    {
        m_lastResetTime = std::move(value);
        this->m_model.lastResetTime = m_lastResetTime ? m_lastResetTime.operator->() : nullptr;
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

    void SetSizeLimit(int32_t value)
    {
        this->m_model.sizeLimit = value;
    }

    void SetVersion(uint32_t value)
    {
        this->m_model.version = value;
    }

    void SetVersionConfiguration(PFVersionConfigurationWrapper<Alloc> value)
    {
        m_versionConfiguration = std::move(value);
        this->m_model.versionConfiguration = &m_versionConfiguration.Model();
    }

private:
    void SetModelPointers()
    {
        this->m_model.columns = m_columns.empty() ? nullptr : m_columns.data();
        this->m_model.entityType = m_entityType.empty() ? nullptr : m_entityType.data();
        this->m_model.lastResetTime = m_lastResetTime ? m_lastResetTime.operator->() : nullptr;
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
        this->m_model.versionConfiguration = &m_versionConfiguration.Model();
    }

    ModelVector<PFLeaderboardsLeaderboardColumnWrapper<Alloc>, Alloc> m_columns;
    String m_entityType;
    std::optional<time_t> m_lastResetTime;
    String m_name;
    PFVersionConfigurationWrapper<Alloc> m_versionConfiguration;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFLeaderboardsGetLeaderboardForEntitiesRequestWrapper : public ModelWrapper<PFLeaderboardsGetLeaderboardForEntitiesRequest, Alloc>
{
public:
    using ModelType = PFLeaderboardsGetLeaderboardForEntitiesRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLeaderboardsGetLeaderboardForEntitiesRequestWrapper() = default;

    PFLeaderboardsGetLeaderboardForEntitiesRequestWrapper(const PFLeaderboardsGetLeaderboardForEntitiesRequest& model) :
        ModelWrapper<PFLeaderboardsGetLeaderboardForEntitiesRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entityIds{ model.entityIds, model.entityIds + model.entityIdsCount },
        m_leaderboardName{ SafeString(model.leaderboardName) },
        m_version{ model.version ? std::optional<uint32_t>{ *model.version } : std::nullopt }
    {
        SetModelPointers();
    }

    PFLeaderboardsGetLeaderboardForEntitiesRequestWrapper(const PFLeaderboardsGetLeaderboardForEntitiesRequestWrapper& src) :
        PFLeaderboardsGetLeaderboardForEntitiesRequestWrapper{ src.Model() }
    {
    }

    PFLeaderboardsGetLeaderboardForEntitiesRequestWrapper(PFLeaderboardsGetLeaderboardForEntitiesRequestWrapper&& src) :
        PFLeaderboardsGetLeaderboardForEntitiesRequestWrapper{}
    {
        swap(*this, src);
    }

    PFLeaderboardsGetLeaderboardForEntitiesRequestWrapper& operator=(PFLeaderboardsGetLeaderboardForEntitiesRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLeaderboardsGetLeaderboardForEntitiesRequestWrapper() = default;

    friend void swap(PFLeaderboardsGetLeaderboardForEntitiesRequestWrapper& lhs, PFLeaderboardsGetLeaderboardForEntitiesRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entityIds, rhs.m_entityIds);
        swap(lhs.m_leaderboardName, rhs.m_leaderboardName);
        swap(lhs.m_version, rhs.m_version);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntityIds(CStringVector<Alloc> value)
    {
        m_entityIds = std::move(value);
        this->m_model.entityIds =  m_entityIds.empty() ? nullptr : m_entityIds.data();
        this->m_model.entityIdsCount =  static_cast<uint32_t>(m_entityIds.size());
    }

    void SetLeaderboardName(String value)
    {
        m_leaderboardName = std::move(value);
        this->m_model.leaderboardName =  m_leaderboardName.empty() ? nullptr : m_leaderboardName.data();
    }

    void SetVersion(std::optional<uint32_t> value)
    {
        m_version = std::move(value);
        this->m_model.version = m_version ? m_version.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entityIds = m_entityIds.empty() ? nullptr : m_entityIds.data();
        this->m_model.leaderboardName = m_leaderboardName.empty() ? nullptr : m_leaderboardName.data();
        this->m_model.version = m_version ? m_version.operator->() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    CStringVector<Alloc> m_entityIds;
    String m_leaderboardName;
    std::optional<uint32_t> m_version;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFLeaderboardsIncrementLeaderboardVersionRequestWrapper : public ModelWrapper<PFLeaderboardsIncrementLeaderboardVersionRequest, Alloc>
{
public:
    using ModelType = PFLeaderboardsIncrementLeaderboardVersionRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLeaderboardsIncrementLeaderboardVersionRequestWrapper() = default;

    PFLeaderboardsIncrementLeaderboardVersionRequestWrapper(const PFLeaderboardsIncrementLeaderboardVersionRequest& model) :
        ModelWrapper<PFLeaderboardsIncrementLeaderboardVersionRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_name{ SafeString(model.name) }
    {
        SetModelPointers();
    }

    PFLeaderboardsIncrementLeaderboardVersionRequestWrapper(const PFLeaderboardsIncrementLeaderboardVersionRequestWrapper& src) :
        PFLeaderboardsIncrementLeaderboardVersionRequestWrapper{ src.Model() }
    {
    }

    PFLeaderboardsIncrementLeaderboardVersionRequestWrapper(PFLeaderboardsIncrementLeaderboardVersionRequestWrapper&& src) :
        PFLeaderboardsIncrementLeaderboardVersionRequestWrapper{}
    {
        swap(*this, src);
    }

    PFLeaderboardsIncrementLeaderboardVersionRequestWrapper& operator=(PFLeaderboardsIncrementLeaderboardVersionRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLeaderboardsIncrementLeaderboardVersionRequestWrapper() = default;

    friend void swap(PFLeaderboardsIncrementLeaderboardVersionRequestWrapper& lhs, PFLeaderboardsIncrementLeaderboardVersionRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_name, rhs.m_name);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
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
class PFLeaderboardsIncrementLeaderboardVersionResponseWrapper : public ModelWrapper<PFLeaderboardsIncrementLeaderboardVersionResponse, Alloc>
{
public:
    using ModelType = PFLeaderboardsIncrementLeaderboardVersionResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    using ModelWrapper<PFLeaderboardsIncrementLeaderboardVersionResponse, Alloc>::ModelWrapper;

    void SetVersion(uint32_t value)
    {
        this->m_model.version = value;
    }

private:
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFLeaderboardsListLeaderboardDefinitionsRequestWrapper : public ModelWrapper<PFLeaderboardsListLeaderboardDefinitionsRequest, Alloc>
{
public:
    using ModelType = PFLeaderboardsListLeaderboardDefinitionsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLeaderboardsListLeaderboardDefinitionsRequestWrapper() = default;

    PFLeaderboardsListLeaderboardDefinitionsRequestWrapper(const PFLeaderboardsListLeaderboardDefinitionsRequest& model) :
        ModelWrapper<PFLeaderboardsListLeaderboardDefinitionsRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount }
    {
        SetModelPointers();
    }

    PFLeaderboardsListLeaderboardDefinitionsRequestWrapper(const PFLeaderboardsListLeaderboardDefinitionsRequestWrapper& src) :
        PFLeaderboardsListLeaderboardDefinitionsRequestWrapper{ src.Model() }
    {
    }

    PFLeaderboardsListLeaderboardDefinitionsRequestWrapper(PFLeaderboardsListLeaderboardDefinitionsRequestWrapper&& src) :
        PFLeaderboardsListLeaderboardDefinitionsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFLeaderboardsListLeaderboardDefinitionsRequestWrapper& operator=(PFLeaderboardsListLeaderboardDefinitionsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLeaderboardsListLeaderboardDefinitionsRequestWrapper() = default;

    friend void swap(PFLeaderboardsListLeaderboardDefinitionsRequestWrapper& lhs, PFLeaderboardsListLeaderboardDefinitionsRequestWrapper& rhs)
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
class PFLeaderboardsLeaderboardDefinitionWrapper : public ModelWrapper<PFLeaderboardsLeaderboardDefinition, Alloc>
{
public:
    using ModelType = PFLeaderboardsLeaderboardDefinition;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLeaderboardsLeaderboardDefinitionWrapper() = default;

    PFLeaderboardsLeaderboardDefinitionWrapper(const PFLeaderboardsLeaderboardDefinition& model) :
        ModelWrapper<PFLeaderboardsLeaderboardDefinition, Alloc>{ model },
        m_columns{ model.columns, model.columns + model.columnsCount },
        m_entityType{ SafeString(model.entityType) },
        m_lastResetTime{ model.lastResetTime ? std::optional<time_t>{ *model.lastResetTime } : std::nullopt },
        m_name{ SafeString(model.name) },
        m_versionConfiguration{ model.versionConfiguration ? *model.versionConfiguration : decltype(*model.versionConfiguration){} }
    {
        SetModelPointers();
    }

    PFLeaderboardsLeaderboardDefinitionWrapper(const PFLeaderboardsLeaderboardDefinitionWrapper& src) :
        PFLeaderboardsLeaderboardDefinitionWrapper{ src.Model() }
    {
    }

    PFLeaderboardsLeaderboardDefinitionWrapper(PFLeaderboardsLeaderboardDefinitionWrapper&& src) :
        PFLeaderboardsLeaderboardDefinitionWrapper{}
    {
        swap(*this, src);
    }

    PFLeaderboardsLeaderboardDefinitionWrapper& operator=(PFLeaderboardsLeaderboardDefinitionWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLeaderboardsLeaderboardDefinitionWrapper() = default;

    friend void swap(PFLeaderboardsLeaderboardDefinitionWrapper& lhs, PFLeaderboardsLeaderboardDefinitionWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_columns, rhs.m_columns);
        swap(lhs.m_entityType, rhs.m_entityType);
        swap(lhs.m_lastResetTime, rhs.m_lastResetTime);
        swap(lhs.m_name, rhs.m_name);
        swap(lhs.m_versionConfiguration, rhs.m_versionConfiguration);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetColumns(ModelVector<PFLeaderboardsLeaderboardColumnWrapper<Alloc>, Alloc> value)
    {
        m_columns = std::move(value);
        this->m_model.columns =  m_columns.empty() ? nullptr : m_columns.data();
        this->m_model.columnsCount =  static_cast<uint32_t>(m_columns.size());
    }

    void SetCreated(time_t value)
    {
        this->m_model.created = value;
    }

    void SetEntityType(String value)
    {
        m_entityType = std::move(value);
        this->m_model.entityType =  m_entityType.empty() ? nullptr : m_entityType.data();
    }

    void SetLastResetTime(std::optional<time_t> value)
    {
        m_lastResetTime = std::move(value);
        this->m_model.lastResetTime = m_lastResetTime ? m_lastResetTime.operator->() : nullptr;
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

    void SetSizeLimit(int32_t value)
    {
        this->m_model.sizeLimit = value;
    }

    void SetVersion(uint32_t value)
    {
        this->m_model.version = value;
    }

    void SetVersionConfiguration(PFVersionConfigurationWrapper<Alloc> value)
    {
        m_versionConfiguration = std::move(value);
        this->m_model.versionConfiguration = &m_versionConfiguration.Model();
    }

private:
    void SetModelPointers()
    {
        this->m_model.columns = m_columns.empty() ? nullptr : m_columns.data();
        this->m_model.entityType = m_entityType.empty() ? nullptr : m_entityType.data();
        this->m_model.lastResetTime = m_lastResetTime ? m_lastResetTime.operator->() : nullptr;
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
        this->m_model.versionConfiguration = &m_versionConfiguration.Model();
    }

    ModelVector<PFLeaderboardsLeaderboardColumnWrapper<Alloc>, Alloc> m_columns;
    String m_entityType;
    std::optional<time_t> m_lastResetTime;
    String m_name;
    PFVersionConfigurationWrapper<Alloc> m_versionConfiguration;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFLeaderboardsListLeaderboardDefinitionsResponseWrapper : public ModelWrapper<PFLeaderboardsListLeaderboardDefinitionsResponse, Alloc>
{
public:
    using ModelType = PFLeaderboardsListLeaderboardDefinitionsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLeaderboardsListLeaderboardDefinitionsResponseWrapper() = default;

    PFLeaderboardsListLeaderboardDefinitionsResponseWrapper(const PFLeaderboardsListLeaderboardDefinitionsResponse& model) :
        ModelWrapper<PFLeaderboardsListLeaderboardDefinitionsResponse, Alloc>{ model },
        m_leaderboardDefinitions{ model.leaderboardDefinitions, model.leaderboardDefinitions + model.leaderboardDefinitionsCount }
    {
        SetModelPointers();
    }

    PFLeaderboardsListLeaderboardDefinitionsResponseWrapper(const PFLeaderboardsListLeaderboardDefinitionsResponseWrapper& src) :
        PFLeaderboardsListLeaderboardDefinitionsResponseWrapper{ src.Model() }
    {
    }

    PFLeaderboardsListLeaderboardDefinitionsResponseWrapper(PFLeaderboardsListLeaderboardDefinitionsResponseWrapper&& src) :
        PFLeaderboardsListLeaderboardDefinitionsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFLeaderboardsListLeaderboardDefinitionsResponseWrapper& operator=(PFLeaderboardsListLeaderboardDefinitionsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLeaderboardsListLeaderboardDefinitionsResponseWrapper() = default;

    friend void swap(PFLeaderboardsListLeaderboardDefinitionsResponseWrapper& lhs, PFLeaderboardsListLeaderboardDefinitionsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_leaderboardDefinitions, rhs.m_leaderboardDefinitions);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetLeaderboardDefinitions(ModelVector<PFLeaderboardsLeaderboardDefinitionWrapper<Alloc>, Alloc> value)
    {
        m_leaderboardDefinitions = std::move(value);
        this->m_model.leaderboardDefinitions =  m_leaderboardDefinitions.empty() ? nullptr : m_leaderboardDefinitions.data();
        this->m_model.leaderboardDefinitionsCount =  static_cast<uint32_t>(m_leaderboardDefinitions.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.leaderboardDefinitions = m_leaderboardDefinitions.empty() ? nullptr : m_leaderboardDefinitions.data();
    }

    ModelVector<PFLeaderboardsLeaderboardDefinitionWrapper<Alloc>, Alloc> m_leaderboardDefinitions;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFLeaderboardsUnlinkLeaderboardFromStatisticRequestWrapper : public ModelWrapper<PFLeaderboardsUnlinkLeaderboardFromStatisticRequest, Alloc>
{
public:
    using ModelType = PFLeaderboardsUnlinkLeaderboardFromStatisticRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLeaderboardsUnlinkLeaderboardFromStatisticRequestWrapper() = default;

    PFLeaderboardsUnlinkLeaderboardFromStatisticRequestWrapper(const PFLeaderboardsUnlinkLeaderboardFromStatisticRequest& model) :
        ModelWrapper<PFLeaderboardsUnlinkLeaderboardFromStatisticRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_name{ SafeString(model.name) },
        m_statisticName{ SafeString(model.statisticName) }
    {
        SetModelPointers();
    }

    PFLeaderboardsUnlinkLeaderboardFromStatisticRequestWrapper(const PFLeaderboardsUnlinkLeaderboardFromStatisticRequestWrapper& src) :
        PFLeaderboardsUnlinkLeaderboardFromStatisticRequestWrapper{ src.Model() }
    {
    }

    PFLeaderboardsUnlinkLeaderboardFromStatisticRequestWrapper(PFLeaderboardsUnlinkLeaderboardFromStatisticRequestWrapper&& src) :
        PFLeaderboardsUnlinkLeaderboardFromStatisticRequestWrapper{}
    {
        swap(*this, src);
    }

    PFLeaderboardsUnlinkLeaderboardFromStatisticRequestWrapper& operator=(PFLeaderboardsUnlinkLeaderboardFromStatisticRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLeaderboardsUnlinkLeaderboardFromStatisticRequestWrapper() = default;

    friend void swap(PFLeaderboardsUnlinkLeaderboardFromStatisticRequestWrapper& lhs, PFLeaderboardsUnlinkLeaderboardFromStatisticRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_name, rhs.m_name);
        swap(lhs.m_statisticName, rhs.m_statisticName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

    void SetStatisticName(String value)
    {
        m_statisticName = std::move(value);
        this->m_model.statisticName =  m_statisticName.empty() ? nullptr : m_statisticName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
        this->m_model.statisticName = m_statisticName.empty() ? nullptr : m_statisticName.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_name;
    String m_statisticName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFLeaderboardsLeaderboardEntryUpdateWrapper : public ModelWrapper<PFLeaderboardsLeaderboardEntryUpdate, Alloc>
{
public:
    using ModelType = PFLeaderboardsLeaderboardEntryUpdate;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLeaderboardsLeaderboardEntryUpdateWrapper() = default;

    PFLeaderboardsLeaderboardEntryUpdateWrapper(const PFLeaderboardsLeaderboardEntryUpdate& model) :
        ModelWrapper<PFLeaderboardsLeaderboardEntryUpdate, Alloc>{ model },
        m_entityId{ SafeString(model.entityId) },
        m_metadata{ SafeString(model.metadata) },
        m_scores{ model.scores, model.scores + model.scoresCount }
    {
        SetModelPointers();
    }

    PFLeaderboardsLeaderboardEntryUpdateWrapper(const PFLeaderboardsLeaderboardEntryUpdateWrapper& src) :
        PFLeaderboardsLeaderboardEntryUpdateWrapper{ src.Model() }
    {
    }

    PFLeaderboardsLeaderboardEntryUpdateWrapper(PFLeaderboardsLeaderboardEntryUpdateWrapper&& src) :
        PFLeaderboardsLeaderboardEntryUpdateWrapper{}
    {
        swap(*this, src);
    }

    PFLeaderboardsLeaderboardEntryUpdateWrapper& operator=(PFLeaderboardsLeaderboardEntryUpdateWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLeaderboardsLeaderboardEntryUpdateWrapper() = default;

    friend void swap(PFLeaderboardsLeaderboardEntryUpdateWrapper& lhs, PFLeaderboardsLeaderboardEntryUpdateWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_entityId, rhs.m_entityId);
        swap(lhs.m_metadata, rhs.m_metadata);
        swap(lhs.m_scores, rhs.m_scores);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetEntityId(String value)
    {
        m_entityId = std::move(value);
        this->m_model.entityId =  m_entityId.empty() ? nullptr : m_entityId.data();
    }

    void SetMetadata(String value)
    {
        m_metadata = std::move(value);
        this->m_model.metadata =  m_metadata.empty() ? nullptr : m_metadata.data();
    }

    void SetScores(CStringVector<Alloc> value)
    {
        m_scores = std::move(value);
        this->m_model.scores =  m_scores.empty() ? nullptr : m_scores.data();
        this->m_model.scoresCount =  static_cast<uint32_t>(m_scores.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.entityId = m_entityId.empty() ? nullptr : m_entityId.data();
        this->m_model.metadata = m_metadata.empty() ? nullptr : m_metadata.data();
        this->m_model.scores = m_scores.empty() ? nullptr : m_scores.data();
    }

    String m_entityId;
    String m_metadata;
    CStringVector<Alloc> m_scores;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFLeaderboardsUpdateLeaderboardEntriesRequestWrapper : public ModelWrapper<PFLeaderboardsUpdateLeaderboardEntriesRequest, Alloc>
{
public:
    using ModelType = PFLeaderboardsUpdateLeaderboardEntriesRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLeaderboardsUpdateLeaderboardEntriesRequestWrapper() = default;

    PFLeaderboardsUpdateLeaderboardEntriesRequestWrapper(const PFLeaderboardsUpdateLeaderboardEntriesRequest& model) :
        ModelWrapper<PFLeaderboardsUpdateLeaderboardEntriesRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entries{ model.entries, model.entries + model.entriesCount },
        m_leaderboardName{ SafeString(model.leaderboardName) }
    {
        SetModelPointers();
    }

    PFLeaderboardsUpdateLeaderboardEntriesRequestWrapper(const PFLeaderboardsUpdateLeaderboardEntriesRequestWrapper& src) :
        PFLeaderboardsUpdateLeaderboardEntriesRequestWrapper{ src.Model() }
    {
    }

    PFLeaderboardsUpdateLeaderboardEntriesRequestWrapper(PFLeaderboardsUpdateLeaderboardEntriesRequestWrapper&& src) :
        PFLeaderboardsUpdateLeaderboardEntriesRequestWrapper{}
    {
        swap(*this, src);
    }

    PFLeaderboardsUpdateLeaderboardEntriesRequestWrapper& operator=(PFLeaderboardsUpdateLeaderboardEntriesRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLeaderboardsUpdateLeaderboardEntriesRequestWrapper() = default;

    friend void swap(PFLeaderboardsUpdateLeaderboardEntriesRequestWrapper& lhs, PFLeaderboardsUpdateLeaderboardEntriesRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entries, rhs.m_entries);
        swap(lhs.m_leaderboardName, rhs.m_leaderboardName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetEntries(ModelVector<PFLeaderboardsLeaderboardEntryUpdateWrapper<Alloc>, Alloc> value)
    {
        m_entries = std::move(value);
        this->m_model.entries =  m_entries.empty() ? nullptr : m_entries.data();
        this->m_model.entriesCount =  static_cast<uint32_t>(m_entries.size());
    }

    void SetLeaderboardName(String value)
    {
        m_leaderboardName = std::move(value);
        this->m_model.leaderboardName =  m_leaderboardName.empty() ? nullptr : m_leaderboardName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entries = m_entries.empty() ? nullptr : m_entries.data();
        this->m_model.leaderboardName = m_leaderboardName.empty() ? nullptr : m_leaderboardName.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    ModelVector<PFLeaderboardsLeaderboardEntryUpdateWrapper<Alloc>, Alloc> m_entries;
    String m_leaderboardName;
};

} // namespace Wrappers
} // namespace PlayFab

// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFMultiplayerServerTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerDeleteSecretRequestWrapper : public ModelWrapper<PFMultiplayerServerDeleteSecretRequest, Alloc>
{
public:
    using ModelType = PFMultiplayerServerDeleteSecretRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerDeleteSecretRequestWrapper() = default;

    PFMultiplayerServerDeleteSecretRequestWrapper(const PFMultiplayerServerDeleteSecretRequest& model) :
        ModelWrapper<PFMultiplayerServerDeleteSecretRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_name{ SafeString(model.name) }
    {
        SetModelPointers();
    }

    PFMultiplayerServerDeleteSecretRequestWrapper(const PFMultiplayerServerDeleteSecretRequestWrapper& src) :
        PFMultiplayerServerDeleteSecretRequestWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerDeleteSecretRequestWrapper(PFMultiplayerServerDeleteSecretRequestWrapper&& src) :
        PFMultiplayerServerDeleteSecretRequestWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerDeleteSecretRequestWrapper& operator=(PFMultiplayerServerDeleteSecretRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerDeleteSecretRequestWrapper() = default;

    friend void swap(PFMultiplayerServerDeleteSecretRequestWrapper& lhs, PFMultiplayerServerDeleteSecretRequestWrapper& rhs)
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
class PFMultiplayerServerListBuildAliasesRequestWrapper : public ModelWrapper<PFMultiplayerServerListBuildAliasesRequest, Alloc>
{
public:
    using ModelType = PFMultiplayerServerListBuildAliasesRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerListBuildAliasesRequestWrapper() = default;

    PFMultiplayerServerListBuildAliasesRequestWrapper(const PFMultiplayerServerListBuildAliasesRequest& model) :
        ModelWrapper<PFMultiplayerServerListBuildAliasesRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_pageSize{ model.pageSize ? std::optional<int32_t>{ *model.pageSize } : std::nullopt },
        m_skipToken{ SafeString(model.skipToken) }
    {
        SetModelPointers();
    }

    PFMultiplayerServerListBuildAliasesRequestWrapper(const PFMultiplayerServerListBuildAliasesRequestWrapper& src) :
        PFMultiplayerServerListBuildAliasesRequestWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerListBuildAliasesRequestWrapper(PFMultiplayerServerListBuildAliasesRequestWrapper&& src) :
        PFMultiplayerServerListBuildAliasesRequestWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerListBuildAliasesRequestWrapper& operator=(PFMultiplayerServerListBuildAliasesRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerListBuildAliasesRequestWrapper() = default;

    friend void swap(PFMultiplayerServerListBuildAliasesRequestWrapper& lhs, PFMultiplayerServerListBuildAliasesRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_pageSize, rhs.m_pageSize);
        swap(lhs.m_skipToken, rhs.m_skipToken);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetPageSize(std::optional<int32_t> value)
    {
        m_pageSize = std::move(value);
        this->m_model.pageSize = m_pageSize ? m_pageSize.operator->() : nullptr;
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
class PFMultiplayerServerBuildSelectionCriterionWrapper : public ModelWrapper<PFMultiplayerServerBuildSelectionCriterion, Alloc>
{
public:
    using ModelType = PFMultiplayerServerBuildSelectionCriterion;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerBuildSelectionCriterionWrapper() = default;

    PFMultiplayerServerBuildSelectionCriterionWrapper(const PFMultiplayerServerBuildSelectionCriterion& model) :
        ModelWrapper<PFMultiplayerServerBuildSelectionCriterion, Alloc>{ model },
        m_buildWeightDistribution{ model.buildWeightDistribution, model.buildWeightDistribution + model.buildWeightDistributionCount }
    {
        SetModelPointers();
    }

    PFMultiplayerServerBuildSelectionCriterionWrapper(const PFMultiplayerServerBuildSelectionCriterionWrapper& src) :
        PFMultiplayerServerBuildSelectionCriterionWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerBuildSelectionCriterionWrapper(PFMultiplayerServerBuildSelectionCriterionWrapper&& src) :
        PFMultiplayerServerBuildSelectionCriterionWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerBuildSelectionCriterionWrapper& operator=(PFMultiplayerServerBuildSelectionCriterionWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerBuildSelectionCriterionWrapper() = default;

    friend void swap(PFMultiplayerServerBuildSelectionCriterionWrapper& lhs, PFMultiplayerServerBuildSelectionCriterionWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_buildWeightDistribution, rhs.m_buildWeightDistribution);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetBuildWeightDistribution(DictionaryEntryVector<PFUint32DictionaryEntry, Alloc> value)
    {
        m_buildWeightDistribution = std::move(value);
        this->m_model.buildWeightDistribution =  m_buildWeightDistribution.empty() ? nullptr : m_buildWeightDistribution.data();
        this->m_model.buildWeightDistributionCount =  static_cast<uint32_t>(m_buildWeightDistribution.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.buildWeightDistribution = m_buildWeightDistribution.empty() ? nullptr : m_buildWeightDistribution.data();
    }

    DictionaryEntryVector<PFUint32DictionaryEntry, Alloc> m_buildWeightDistribution;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerBuildAliasDetailsResponseWrapper : public ModelWrapper<PFMultiplayerServerBuildAliasDetailsResponse, Alloc>
{
public:
    using ModelType = PFMultiplayerServerBuildAliasDetailsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerBuildAliasDetailsResponseWrapper() = default;

    PFMultiplayerServerBuildAliasDetailsResponseWrapper(const PFMultiplayerServerBuildAliasDetailsResponse& model) :
        ModelWrapper<PFMultiplayerServerBuildAliasDetailsResponse, Alloc>{ model },
        m_aliasId{ SafeString(model.aliasId) },
        m_aliasName{ SafeString(model.aliasName) },
        m_buildSelectionCriteria{ model.buildSelectionCriteria, model.buildSelectionCriteria + model.buildSelectionCriteriaCount }
    {
        SetModelPointers();
    }

    PFMultiplayerServerBuildAliasDetailsResponseWrapper(const PFMultiplayerServerBuildAliasDetailsResponseWrapper& src) :
        PFMultiplayerServerBuildAliasDetailsResponseWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerBuildAliasDetailsResponseWrapper(PFMultiplayerServerBuildAliasDetailsResponseWrapper&& src) :
        PFMultiplayerServerBuildAliasDetailsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerBuildAliasDetailsResponseWrapper& operator=(PFMultiplayerServerBuildAliasDetailsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerBuildAliasDetailsResponseWrapper() = default;

    friend void swap(PFMultiplayerServerBuildAliasDetailsResponseWrapper& lhs, PFMultiplayerServerBuildAliasDetailsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_aliasId, rhs.m_aliasId);
        swap(lhs.m_aliasName, rhs.m_aliasName);
        swap(lhs.m_buildSelectionCriteria, rhs.m_buildSelectionCriteria);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAliasId(String value)
    {
        m_aliasId = std::move(value);
        this->m_model.aliasId =  m_aliasId.empty() ? nullptr : m_aliasId.data();
    }

    void SetAliasName(String value)
    {
        m_aliasName = std::move(value);
        this->m_model.aliasName =  m_aliasName.empty() ? nullptr : m_aliasName.data();
    }

    void SetBuildSelectionCriteria(ModelVector<PFMultiplayerServerBuildSelectionCriterionWrapper<Alloc>, Alloc> value)
    {
        m_buildSelectionCriteria = std::move(value);
        this->m_model.buildSelectionCriteria =  m_buildSelectionCriteria.empty() ? nullptr : m_buildSelectionCriteria.data();
        this->m_model.buildSelectionCriteriaCount =  static_cast<uint32_t>(m_buildSelectionCriteria.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.aliasId = m_aliasId.empty() ? nullptr : m_aliasId.data();
        this->m_model.aliasName = m_aliasName.empty() ? nullptr : m_aliasName.data();
        this->m_model.buildSelectionCriteria = m_buildSelectionCriteria.empty() ? nullptr : m_buildSelectionCriteria.data();
    }

    String m_aliasId;
    String m_aliasName;
    ModelVector<PFMultiplayerServerBuildSelectionCriterionWrapper<Alloc>, Alloc> m_buildSelectionCriteria;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerListBuildAliasesResponseWrapper : public ModelWrapper<PFMultiplayerServerListBuildAliasesResponse, Alloc>
{
public:
    using ModelType = PFMultiplayerServerListBuildAliasesResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerListBuildAliasesResponseWrapper() = default;

    PFMultiplayerServerListBuildAliasesResponseWrapper(const PFMultiplayerServerListBuildAliasesResponse& model) :
        ModelWrapper<PFMultiplayerServerListBuildAliasesResponse, Alloc>{ model },
        m_buildAliases{ model.buildAliases, model.buildAliases + model.buildAliasesCount },
        m_skipToken{ SafeString(model.skipToken) }
    {
        SetModelPointers();
    }

    PFMultiplayerServerListBuildAliasesResponseWrapper(const PFMultiplayerServerListBuildAliasesResponseWrapper& src) :
        PFMultiplayerServerListBuildAliasesResponseWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerListBuildAliasesResponseWrapper(PFMultiplayerServerListBuildAliasesResponseWrapper&& src) :
        PFMultiplayerServerListBuildAliasesResponseWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerListBuildAliasesResponseWrapper& operator=(PFMultiplayerServerListBuildAliasesResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerListBuildAliasesResponseWrapper() = default;

    friend void swap(PFMultiplayerServerListBuildAliasesResponseWrapper& lhs, PFMultiplayerServerListBuildAliasesResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_buildAliases, rhs.m_buildAliases);
        swap(lhs.m_skipToken, rhs.m_skipToken);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetBuildAliases(ModelVector<PFMultiplayerServerBuildAliasDetailsResponseWrapper<Alloc>, Alloc> value)
    {
        m_buildAliases = std::move(value);
        this->m_model.buildAliases =  m_buildAliases.empty() ? nullptr : m_buildAliases.data();
        this->m_model.buildAliasesCount =  static_cast<uint32_t>(m_buildAliases.size());
    }

    void SetPageSize(int32_t value)
    {
        this->m_model.pageSize = value;
    }

    void SetSkipToken(String value)
    {
        m_skipToken = std::move(value);
        this->m_model.skipToken =  m_skipToken.empty() ? nullptr : m_skipToken.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.buildAliases = m_buildAliases.empty() ? nullptr : m_buildAliases.data();
        this->m_model.skipToken = m_skipToken.empty() ? nullptr : m_skipToken.data();
    }

    ModelVector<PFMultiplayerServerBuildAliasDetailsResponseWrapper<Alloc>, Alloc> m_buildAliases;
    String m_skipToken;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerListBuildSummariesRequestWrapper : public ModelWrapper<PFMultiplayerServerListBuildSummariesRequest, Alloc>
{
public:
    using ModelType = PFMultiplayerServerListBuildSummariesRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerListBuildSummariesRequestWrapper() = default;

    PFMultiplayerServerListBuildSummariesRequestWrapper(const PFMultiplayerServerListBuildSummariesRequest& model) :
        ModelWrapper<PFMultiplayerServerListBuildSummariesRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_pageSize{ model.pageSize ? std::optional<int32_t>{ *model.pageSize } : std::nullopt },
        m_skipToken{ SafeString(model.skipToken) }
    {
        SetModelPointers();
    }

    PFMultiplayerServerListBuildSummariesRequestWrapper(const PFMultiplayerServerListBuildSummariesRequestWrapper& src) :
        PFMultiplayerServerListBuildSummariesRequestWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerListBuildSummariesRequestWrapper(PFMultiplayerServerListBuildSummariesRequestWrapper&& src) :
        PFMultiplayerServerListBuildSummariesRequestWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerListBuildSummariesRequestWrapper& operator=(PFMultiplayerServerListBuildSummariesRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerListBuildSummariesRequestWrapper() = default;

    friend void swap(PFMultiplayerServerListBuildSummariesRequestWrapper& lhs, PFMultiplayerServerListBuildSummariesRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_pageSize, rhs.m_pageSize);
        swap(lhs.m_skipToken, rhs.m_skipToken);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetPageSize(std::optional<int32_t> value)
    {
        m_pageSize = std::move(value);
        this->m_model.pageSize = m_pageSize ? m_pageSize.operator->() : nullptr;
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
class PFMultiplayerServerCurrentServerStatsWrapper : public ModelWrapper<PFMultiplayerServerCurrentServerStats, Alloc>
{
public:
    using ModelType = PFMultiplayerServerCurrentServerStats;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    using ModelWrapper<PFMultiplayerServerCurrentServerStats, Alloc>::ModelWrapper;

    void SetActive(int32_t value)
    {
        this->m_model.active = value;
    }

    void SetPropping(int32_t value)
    {
        this->m_model.propping = value;
    }

    void SetStandingBy(int32_t value)
    {
        this->m_model.standingBy = value;
    }

    void SetTotal(int32_t value)
    {
        this->m_model.total = value;
    }

private:
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerDynamicStandbyThresholdWrapper : public ModelWrapper<PFMultiplayerServerDynamicStandbyThreshold, Alloc>
{
public:
    using ModelType = PFMultiplayerServerDynamicStandbyThreshold;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    using ModelWrapper<PFMultiplayerServerDynamicStandbyThreshold, Alloc>::ModelWrapper;

    void SetMultiplier(double value)
    {
        this->m_model.multiplier = value;
    }

    void SetTriggerThresholdPercentage(double value)
    {
        this->m_model.triggerThresholdPercentage = value;
    }

private:
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerDynamicStandbySettingsWrapper : public ModelWrapper<PFMultiplayerServerDynamicStandbySettings, Alloc>
{
public:
    using ModelType = PFMultiplayerServerDynamicStandbySettings;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerDynamicStandbySettingsWrapper() = default;

    PFMultiplayerServerDynamicStandbySettingsWrapper(const PFMultiplayerServerDynamicStandbySettings& model) :
        ModelWrapper<PFMultiplayerServerDynamicStandbySettings, Alloc>{ model },
        m_dynamicFloorMultiplierThresholds{ model.dynamicFloorMultiplierThresholds, model.dynamicFloorMultiplierThresholds + model.dynamicFloorMultiplierThresholdsCount },
        m_rampDownSeconds{ model.rampDownSeconds ? std::optional<int32_t>{ *model.rampDownSeconds } : std::nullopt }
    {
        SetModelPointers();
    }

    PFMultiplayerServerDynamicStandbySettingsWrapper(const PFMultiplayerServerDynamicStandbySettingsWrapper& src) :
        PFMultiplayerServerDynamicStandbySettingsWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerDynamicStandbySettingsWrapper(PFMultiplayerServerDynamicStandbySettingsWrapper&& src) :
        PFMultiplayerServerDynamicStandbySettingsWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerDynamicStandbySettingsWrapper& operator=(PFMultiplayerServerDynamicStandbySettingsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerDynamicStandbySettingsWrapper() = default;

    friend void swap(PFMultiplayerServerDynamicStandbySettingsWrapper& lhs, PFMultiplayerServerDynamicStandbySettingsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_dynamicFloorMultiplierThresholds, rhs.m_dynamicFloorMultiplierThresholds);
        swap(lhs.m_rampDownSeconds, rhs.m_rampDownSeconds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetDynamicFloorMultiplierThresholds(ModelVector<PFMultiplayerServerDynamicStandbyThresholdWrapper<Alloc>, Alloc> value)
    {
        m_dynamicFloorMultiplierThresholds = std::move(value);
        this->m_model.dynamicFloorMultiplierThresholds =  m_dynamicFloorMultiplierThresholds.empty() ? nullptr : m_dynamicFloorMultiplierThresholds.data();
        this->m_model.dynamicFloorMultiplierThresholdsCount =  static_cast<uint32_t>(m_dynamicFloorMultiplierThresholds.size());
    }

    void SetIsEnabled(bool value)
    {
        this->m_model.isEnabled = value;
    }

    void SetRampDownSeconds(std::optional<int32_t> value)
    {
        m_rampDownSeconds = std::move(value);
        this->m_model.rampDownSeconds = m_rampDownSeconds ? m_rampDownSeconds.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.dynamicFloorMultiplierThresholds = m_dynamicFloorMultiplierThresholds.empty() ? nullptr : m_dynamicFloorMultiplierThresholds.data();
        this->m_model.rampDownSeconds = m_rampDownSeconds ? m_rampDownSeconds.operator->() : nullptr;
    }

    ModelVector<PFMultiplayerServerDynamicStandbyThresholdWrapper<Alloc>, Alloc> m_dynamicFloorMultiplierThresholds;
    std::optional<int32_t> m_rampDownSeconds;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerScheduleWrapper : public ModelWrapper<PFMultiplayerServerSchedule, Alloc>
{
public:
    using ModelType = PFMultiplayerServerSchedule;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerScheduleWrapper() = default;

    PFMultiplayerServerScheduleWrapper(const PFMultiplayerServerSchedule& model) :
        ModelWrapper<PFMultiplayerServerSchedule, Alloc>{ model },
        m_description{ SafeString(model.description) }
    {
        SetModelPointers();
    }

    PFMultiplayerServerScheduleWrapper(const PFMultiplayerServerScheduleWrapper& src) :
        PFMultiplayerServerScheduleWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerScheduleWrapper(PFMultiplayerServerScheduleWrapper&& src) :
        PFMultiplayerServerScheduleWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerScheduleWrapper& operator=(PFMultiplayerServerScheduleWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerScheduleWrapper() = default;

    friend void swap(PFMultiplayerServerScheduleWrapper& lhs, PFMultiplayerServerScheduleWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_description, rhs.m_description);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetDescription(String value)
    {
        m_description = std::move(value);
        this->m_model.description =  m_description.empty() ? nullptr : m_description.data();
    }

    void SetEndTime(time_t value)
    {
        this->m_model.endTime = value;
    }

    void SetIsDisabled(bool value)
    {
        this->m_model.isDisabled = value;
    }

    void SetIsRecurringWeekly(bool value)
    {
        this->m_model.isRecurringWeekly = value;
    }

    void SetStartTime(time_t value)
    {
        this->m_model.startTime = value;
    }

    void SetTargetStandby(int32_t value)
    {
        this->m_model.targetStandby = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.description = m_description.empty() ? nullptr : m_description.data();
    }

    String m_description;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerScheduledStandbySettingsWrapper : public ModelWrapper<PFMultiplayerServerScheduledStandbySettings, Alloc>
{
public:
    using ModelType = PFMultiplayerServerScheduledStandbySettings;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerScheduledStandbySettingsWrapper() = default;

    PFMultiplayerServerScheduledStandbySettingsWrapper(const PFMultiplayerServerScheduledStandbySettings& model) :
        ModelWrapper<PFMultiplayerServerScheduledStandbySettings, Alloc>{ model },
        m_scheduleList{ model.scheduleList, model.scheduleList + model.scheduleListCount }
    {
        SetModelPointers();
    }

    PFMultiplayerServerScheduledStandbySettingsWrapper(const PFMultiplayerServerScheduledStandbySettingsWrapper& src) :
        PFMultiplayerServerScheduledStandbySettingsWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerScheduledStandbySettingsWrapper(PFMultiplayerServerScheduledStandbySettingsWrapper&& src) :
        PFMultiplayerServerScheduledStandbySettingsWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerScheduledStandbySettingsWrapper& operator=(PFMultiplayerServerScheduledStandbySettingsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerScheduledStandbySettingsWrapper() = default;

    friend void swap(PFMultiplayerServerScheduledStandbySettingsWrapper& lhs, PFMultiplayerServerScheduledStandbySettingsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_scheduleList, rhs.m_scheduleList);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetIsEnabled(bool value)
    {
        this->m_model.isEnabled = value;
    }

    void SetScheduleList(ModelVector<PFMultiplayerServerScheduleWrapper<Alloc>, Alloc> value)
    {
        m_scheduleList = std::move(value);
        this->m_model.scheduleList =  m_scheduleList.empty() ? nullptr : m_scheduleList.data();
        this->m_model.scheduleListCount =  static_cast<uint32_t>(m_scheduleList.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.scheduleList = m_scheduleList.empty() ? nullptr : m_scheduleList.data();
    }

    ModelVector<PFMultiplayerServerScheduleWrapper<Alloc>, Alloc> m_scheduleList;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerBuildRegionWrapper : public ModelWrapper<PFMultiplayerServerBuildRegion, Alloc>
{
public:
    using ModelType = PFMultiplayerServerBuildRegion;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerBuildRegionWrapper() = default;

    PFMultiplayerServerBuildRegionWrapper(const PFMultiplayerServerBuildRegion& model) :
        ModelWrapper<PFMultiplayerServerBuildRegion, Alloc>{ model },
        m_currentServerStats{ model.currentServerStats ? std::optional<PFMultiplayerServerCurrentServerStatsWrapper<Alloc>>{ *model.currentServerStats } : std::nullopt },
        m_dynamicStandbySettings{ model.dynamicStandbySettings ? std::optional<PFMultiplayerServerDynamicStandbySettingsWrapper<Alloc>>{ *model.dynamicStandbySettings } : std::nullopt },
        m_multiplayerServerCountPerVm{ model.multiplayerServerCountPerVm ? std::optional<int32_t>{ *model.multiplayerServerCountPerVm } : std::nullopt },
        m_region{ SafeString(model.region) },
        m_scheduledStandbySettings{ model.scheduledStandbySettings ? std::optional<PFMultiplayerServerScheduledStandbySettingsWrapper<Alloc>>{ *model.scheduledStandbySettings } : std::nullopt },
        m_status{ SafeString(model.status) },
        m_vmSize{ model.vmSize ? std::optional<PFMultiplayerServerAzureVmSize>{ *model.vmSize } : std::nullopt }
    {
        SetModelPointers();
    }

    PFMultiplayerServerBuildRegionWrapper(const PFMultiplayerServerBuildRegionWrapper& src) :
        PFMultiplayerServerBuildRegionWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerBuildRegionWrapper(PFMultiplayerServerBuildRegionWrapper&& src) :
        PFMultiplayerServerBuildRegionWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerBuildRegionWrapper& operator=(PFMultiplayerServerBuildRegionWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerBuildRegionWrapper() = default;

    friend void swap(PFMultiplayerServerBuildRegionWrapper& lhs, PFMultiplayerServerBuildRegionWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_currentServerStats, rhs.m_currentServerStats);
        swap(lhs.m_dynamicStandbySettings, rhs.m_dynamicStandbySettings);
        swap(lhs.m_multiplayerServerCountPerVm, rhs.m_multiplayerServerCountPerVm);
        swap(lhs.m_region, rhs.m_region);
        swap(lhs.m_scheduledStandbySettings, rhs.m_scheduledStandbySettings);
        swap(lhs.m_status, rhs.m_status);
        swap(lhs.m_vmSize, rhs.m_vmSize);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCurrentServerStats(std::optional<PFMultiplayerServerCurrentServerStatsWrapper<Alloc>> value)
    {
        m_currentServerStats = std::move(value);
        this->m_model.currentServerStats = m_currentServerStats ? &m_currentServerStats->Model() : nullptr;
    }

    void SetDynamicStandbySettings(std::optional<PFMultiplayerServerDynamicStandbySettingsWrapper<Alloc>> value)
    {
        m_dynamicStandbySettings = std::move(value);
        this->m_model.dynamicStandbySettings = m_dynamicStandbySettings ? &m_dynamicStandbySettings->Model() : nullptr;
    }

    void SetIsAssetReplicationComplete(bool value)
    {
        this->m_model.isAssetReplicationComplete = value;
    }

    void SetMaxServers(int32_t value)
    {
        this->m_model.maxServers = value;
    }

    void SetMultiplayerServerCountPerVm(std::optional<int32_t> value)
    {
        m_multiplayerServerCountPerVm = std::move(value);
        this->m_model.multiplayerServerCountPerVm = m_multiplayerServerCountPerVm ? m_multiplayerServerCountPerVm.operator->() : nullptr;
    }

    void SetRegion(String value)
    {
        m_region = std::move(value);
        this->m_model.region =  m_region.empty() ? nullptr : m_region.data();
    }

    void SetScheduledStandbySettings(std::optional<PFMultiplayerServerScheduledStandbySettingsWrapper<Alloc>> value)
    {
        m_scheduledStandbySettings = std::move(value);
        this->m_model.scheduledStandbySettings = m_scheduledStandbySettings ? &m_scheduledStandbySettings->Model() : nullptr;
    }

    void SetStandbyServers(int32_t value)
    {
        this->m_model.standbyServers = value;
    }

    void SetStatus(String value)
    {
        m_status = std::move(value);
        this->m_model.status =  m_status.empty() ? nullptr : m_status.data();
    }

    void SetVmSize(std::optional<PFMultiplayerServerAzureVmSize> value)
    {
        m_vmSize = std::move(value);
        this->m_model.vmSize = m_vmSize ? m_vmSize.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.currentServerStats = m_currentServerStats ?  &m_currentServerStats->Model() : nullptr;
        this->m_model.dynamicStandbySettings = m_dynamicStandbySettings ?  &m_dynamicStandbySettings->Model() : nullptr;
        this->m_model.multiplayerServerCountPerVm = m_multiplayerServerCountPerVm ? m_multiplayerServerCountPerVm.operator->() : nullptr;
        this->m_model.region = m_region.empty() ? nullptr : m_region.data();
        this->m_model.scheduledStandbySettings = m_scheduledStandbySettings ?  &m_scheduledStandbySettings->Model() : nullptr;
        this->m_model.status = m_status.empty() ? nullptr : m_status.data();
        this->m_model.vmSize = m_vmSize ? m_vmSize.operator->() : nullptr;
    }

    std::optional<PFMultiplayerServerCurrentServerStatsWrapper<Alloc>> m_currentServerStats;
    std::optional<PFMultiplayerServerDynamicStandbySettingsWrapper<Alloc>> m_dynamicStandbySettings;
    std::optional<int32_t> m_multiplayerServerCountPerVm;
    String m_region;
    std::optional<PFMultiplayerServerScheduledStandbySettingsWrapper<Alloc>> m_scheduledStandbySettings;
    String m_status;
    std::optional<PFMultiplayerServerAzureVmSize> m_vmSize;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerBuildSummaryWrapper : public ModelWrapper<PFMultiplayerServerBuildSummary, Alloc>
{
public:
    using ModelType = PFMultiplayerServerBuildSummary;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerBuildSummaryWrapper() = default;

    PFMultiplayerServerBuildSummaryWrapper(const PFMultiplayerServerBuildSummary& model) :
        ModelWrapper<PFMultiplayerServerBuildSummary, Alloc>{ model },
        m_buildId{ SafeString(model.buildId) },
        m_buildName{ SafeString(model.buildName) },
        m_creationTime{ model.creationTime ? std::optional<time_t>{ *model.creationTime } : std::nullopt },
        m_metadata{ model.metadata, model.metadata + model.metadataCount },
        m_regionConfigurations{ model.regionConfigurations, model.regionConfigurations + model.regionConfigurationsCount }
    {
        SetModelPointers();
    }

    PFMultiplayerServerBuildSummaryWrapper(const PFMultiplayerServerBuildSummaryWrapper& src) :
        PFMultiplayerServerBuildSummaryWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerBuildSummaryWrapper(PFMultiplayerServerBuildSummaryWrapper&& src) :
        PFMultiplayerServerBuildSummaryWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerBuildSummaryWrapper& operator=(PFMultiplayerServerBuildSummaryWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerBuildSummaryWrapper() = default;

    friend void swap(PFMultiplayerServerBuildSummaryWrapper& lhs, PFMultiplayerServerBuildSummaryWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_buildId, rhs.m_buildId);
        swap(lhs.m_buildName, rhs.m_buildName);
        swap(lhs.m_creationTime, rhs.m_creationTime);
        swap(lhs.m_metadata, rhs.m_metadata);
        swap(lhs.m_regionConfigurations, rhs.m_regionConfigurations);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetBuildId(String value)
    {
        m_buildId = std::move(value);
        this->m_model.buildId =  m_buildId.empty() ? nullptr : m_buildId.data();
    }

    void SetBuildName(String value)
    {
        m_buildName = std::move(value);
        this->m_model.buildName =  m_buildName.empty() ? nullptr : m_buildName.data();
    }

    void SetCreationTime(std::optional<time_t> value)
    {
        m_creationTime = std::move(value);
        this->m_model.creationTime = m_creationTime ? m_creationTime.operator->() : nullptr;
    }

    void SetMetadata(StringDictionaryEntryVector<Alloc> value)
    {
        m_metadata = std::move(value);
        this->m_model.metadata =  m_metadata.empty() ? nullptr : m_metadata.data();
        this->m_model.metadataCount =  static_cast<uint32_t>(m_metadata.size());
    }

    void SetRegionConfigurations(ModelVector<PFMultiplayerServerBuildRegionWrapper<Alloc>, Alloc> value)
    {
        m_regionConfigurations = std::move(value);
        this->m_model.regionConfigurations =  m_regionConfigurations.empty() ? nullptr : m_regionConfigurations.data();
        this->m_model.regionConfigurationsCount =  static_cast<uint32_t>(m_regionConfigurations.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.buildId = m_buildId.empty() ? nullptr : m_buildId.data();
        this->m_model.buildName = m_buildName.empty() ? nullptr : m_buildName.data();
        this->m_model.creationTime = m_creationTime ? m_creationTime.operator->() : nullptr;
        this->m_model.metadata = m_metadata.empty() ? nullptr : m_metadata.data();
        this->m_model.regionConfigurations = m_regionConfigurations.empty() ? nullptr : m_regionConfigurations.data();
    }

    String m_buildId;
    String m_buildName;
    std::optional<time_t> m_creationTime;
    StringDictionaryEntryVector<Alloc> m_metadata;
    ModelVector<PFMultiplayerServerBuildRegionWrapper<Alloc>, Alloc> m_regionConfigurations;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerListBuildSummariesResponseWrapper : public ModelWrapper<PFMultiplayerServerListBuildSummariesResponse, Alloc>
{
public:
    using ModelType = PFMultiplayerServerListBuildSummariesResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerListBuildSummariesResponseWrapper() = default;

    PFMultiplayerServerListBuildSummariesResponseWrapper(const PFMultiplayerServerListBuildSummariesResponse& model) :
        ModelWrapper<PFMultiplayerServerListBuildSummariesResponse, Alloc>{ model },
        m_buildSummaries{ model.buildSummaries, model.buildSummaries + model.buildSummariesCount },
        m_skipToken{ SafeString(model.skipToken) }
    {
        SetModelPointers();
    }

    PFMultiplayerServerListBuildSummariesResponseWrapper(const PFMultiplayerServerListBuildSummariesResponseWrapper& src) :
        PFMultiplayerServerListBuildSummariesResponseWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerListBuildSummariesResponseWrapper(PFMultiplayerServerListBuildSummariesResponseWrapper&& src) :
        PFMultiplayerServerListBuildSummariesResponseWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerListBuildSummariesResponseWrapper& operator=(PFMultiplayerServerListBuildSummariesResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerListBuildSummariesResponseWrapper() = default;

    friend void swap(PFMultiplayerServerListBuildSummariesResponseWrapper& lhs, PFMultiplayerServerListBuildSummariesResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_buildSummaries, rhs.m_buildSummaries);
        swap(lhs.m_skipToken, rhs.m_skipToken);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetBuildSummaries(ModelVector<PFMultiplayerServerBuildSummaryWrapper<Alloc>, Alloc> value)
    {
        m_buildSummaries = std::move(value);
        this->m_model.buildSummaries =  m_buildSummaries.empty() ? nullptr : m_buildSummaries.data();
        this->m_model.buildSummariesCount =  static_cast<uint32_t>(m_buildSummaries.size());
    }

    void SetPageSize(int32_t value)
    {
        this->m_model.pageSize = value;
    }

    void SetSkipToken(String value)
    {
        m_skipToken = std::move(value);
        this->m_model.skipToken =  m_skipToken.empty() ? nullptr : m_skipToken.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.buildSummaries = m_buildSummaries.empty() ? nullptr : m_buildSummaries.data();
        this->m_model.skipToken = m_skipToken.empty() ? nullptr : m_skipToken.data();
    }

    ModelVector<PFMultiplayerServerBuildSummaryWrapper<Alloc>, Alloc> m_buildSummaries;
    String m_skipToken;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerListQosServersForTitleRequestWrapper : public ModelWrapper<PFMultiplayerServerListQosServersForTitleRequest, Alloc>
{
public:
    using ModelType = PFMultiplayerServerListQosServersForTitleRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerListQosServersForTitleRequestWrapper() = default;

    PFMultiplayerServerListQosServersForTitleRequestWrapper(const PFMultiplayerServerListQosServersForTitleRequest& model) :
        ModelWrapper<PFMultiplayerServerListQosServersForTitleRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_includeAllRegions{ model.includeAllRegions ? std::optional<bool>{ *model.includeAllRegions } : std::nullopt },
        m_routingPreference{ SafeString(model.routingPreference) }
    {
        SetModelPointers();
    }

    PFMultiplayerServerListQosServersForTitleRequestWrapper(const PFMultiplayerServerListQosServersForTitleRequestWrapper& src) :
        PFMultiplayerServerListQosServersForTitleRequestWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerListQosServersForTitleRequestWrapper(PFMultiplayerServerListQosServersForTitleRequestWrapper&& src) :
        PFMultiplayerServerListQosServersForTitleRequestWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerListQosServersForTitleRequestWrapper& operator=(PFMultiplayerServerListQosServersForTitleRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerListQosServersForTitleRequestWrapper() = default;

    friend void swap(PFMultiplayerServerListQosServersForTitleRequestWrapper& lhs, PFMultiplayerServerListQosServersForTitleRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_includeAllRegions, rhs.m_includeAllRegions);
        swap(lhs.m_routingPreference, rhs.m_routingPreference);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetIncludeAllRegions(std::optional<bool> value)
    {
        m_includeAllRegions = std::move(value);
        this->m_model.includeAllRegions = m_includeAllRegions ? m_includeAllRegions.operator->() : nullptr;
    }

    void SetRoutingPreference(String value)
    {
        m_routingPreference = std::move(value);
        this->m_model.routingPreference =  m_routingPreference.empty() ? nullptr : m_routingPreference.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.includeAllRegions = m_includeAllRegions ? m_includeAllRegions.operator->() : nullptr;
        this->m_model.routingPreference = m_routingPreference.empty() ? nullptr : m_routingPreference.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_includeAllRegions;
    String m_routingPreference;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerQosServerWrapper : public ModelWrapper<PFMultiplayerServerQosServer, Alloc>
{
public:
    using ModelType = PFMultiplayerServerQosServer;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerQosServerWrapper() = default;

    PFMultiplayerServerQosServerWrapper(const PFMultiplayerServerQosServer& model) :
        ModelWrapper<PFMultiplayerServerQosServer, Alloc>{ model },
        m_region{ SafeString(model.region) },
        m_serverUrl{ SafeString(model.serverUrl) }
    {
        SetModelPointers();
    }

    PFMultiplayerServerQosServerWrapper(const PFMultiplayerServerQosServerWrapper& src) :
        PFMultiplayerServerQosServerWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerQosServerWrapper(PFMultiplayerServerQosServerWrapper&& src) :
        PFMultiplayerServerQosServerWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerQosServerWrapper& operator=(PFMultiplayerServerQosServerWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerQosServerWrapper() = default;

    friend void swap(PFMultiplayerServerQosServerWrapper& lhs, PFMultiplayerServerQosServerWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_region, rhs.m_region);
        swap(lhs.m_serverUrl, rhs.m_serverUrl);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetRegion(String value)
    {
        m_region = std::move(value);
        this->m_model.region =  m_region.empty() ? nullptr : m_region.data();
    }

    void SetServerUrl(String value)
    {
        m_serverUrl = std::move(value);
        this->m_model.serverUrl =  m_serverUrl.empty() ? nullptr : m_serverUrl.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.region = m_region.empty() ? nullptr : m_region.data();
        this->m_model.serverUrl = m_serverUrl.empty() ? nullptr : m_serverUrl.data();
    }

    String m_region;
    String m_serverUrl;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerListQosServersForTitleResponseWrapper : public ModelWrapper<PFMultiplayerServerListQosServersForTitleResponse, Alloc>
{
public:
    using ModelType = PFMultiplayerServerListQosServersForTitleResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerListQosServersForTitleResponseWrapper() = default;

    PFMultiplayerServerListQosServersForTitleResponseWrapper(const PFMultiplayerServerListQosServersForTitleResponse& model) :
        ModelWrapper<PFMultiplayerServerListQosServersForTitleResponse, Alloc>{ model },
        m_qosServers{ model.qosServers, model.qosServers + model.qosServersCount },
        m_skipToken{ SafeString(model.skipToken) }
    {
        SetModelPointers();
    }

    PFMultiplayerServerListQosServersForTitleResponseWrapper(const PFMultiplayerServerListQosServersForTitleResponseWrapper& src) :
        PFMultiplayerServerListQosServersForTitleResponseWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerListQosServersForTitleResponseWrapper(PFMultiplayerServerListQosServersForTitleResponseWrapper&& src) :
        PFMultiplayerServerListQosServersForTitleResponseWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerListQosServersForTitleResponseWrapper& operator=(PFMultiplayerServerListQosServersForTitleResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerListQosServersForTitleResponseWrapper() = default;

    friend void swap(PFMultiplayerServerListQosServersForTitleResponseWrapper& lhs, PFMultiplayerServerListQosServersForTitleResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_qosServers, rhs.m_qosServers);
        swap(lhs.m_skipToken, rhs.m_skipToken);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetPageSize(int32_t value)
    {
        this->m_model.pageSize = value;
    }

    void SetQosServers(ModelVector<PFMultiplayerServerQosServerWrapper<Alloc>, Alloc> value)
    {
        m_qosServers = std::move(value);
        this->m_model.qosServers =  m_qosServers.empty() ? nullptr : m_qosServers.data();
        this->m_model.qosServersCount =  static_cast<uint32_t>(m_qosServers.size());
    }

    void SetSkipToken(String value)
    {
        m_skipToken = std::move(value);
        this->m_model.skipToken =  m_skipToken.empty() ? nullptr : m_skipToken.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.qosServers = m_qosServers.empty() ? nullptr : m_qosServers.data();
        this->m_model.skipToken = m_skipToken.empty() ? nullptr : m_skipToken.data();
    }

    ModelVector<PFMultiplayerServerQosServerWrapper<Alloc>, Alloc> m_qosServers;
    String m_skipToken;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerListSecretSummariesRequestWrapper : public ModelWrapper<PFMultiplayerServerListSecretSummariesRequest, Alloc>
{
public:
    using ModelType = PFMultiplayerServerListSecretSummariesRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerListSecretSummariesRequestWrapper() = default;

    PFMultiplayerServerListSecretSummariesRequestWrapper(const PFMultiplayerServerListSecretSummariesRequest& model) :
        ModelWrapper<PFMultiplayerServerListSecretSummariesRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_pageSize{ model.pageSize ? std::optional<int32_t>{ *model.pageSize } : std::nullopt },
        m_skipToken{ SafeString(model.skipToken) }
    {
        SetModelPointers();
    }

    PFMultiplayerServerListSecretSummariesRequestWrapper(const PFMultiplayerServerListSecretSummariesRequestWrapper& src) :
        PFMultiplayerServerListSecretSummariesRequestWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerListSecretSummariesRequestWrapper(PFMultiplayerServerListSecretSummariesRequestWrapper&& src) :
        PFMultiplayerServerListSecretSummariesRequestWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerListSecretSummariesRequestWrapper& operator=(PFMultiplayerServerListSecretSummariesRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerListSecretSummariesRequestWrapper() = default;

    friend void swap(PFMultiplayerServerListSecretSummariesRequestWrapper& lhs, PFMultiplayerServerListSecretSummariesRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_pageSize, rhs.m_pageSize);
        swap(lhs.m_skipToken, rhs.m_skipToken);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetPageSize(std::optional<int32_t> value)
    {
        m_pageSize = std::move(value);
        this->m_model.pageSize = m_pageSize ? m_pageSize.operator->() : nullptr;
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
class PFMultiplayerServerSecretSummaryWrapper : public ModelWrapper<PFMultiplayerServerSecretSummary, Alloc>
{
public:
    using ModelType = PFMultiplayerServerSecretSummary;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerSecretSummaryWrapper() = default;

    PFMultiplayerServerSecretSummaryWrapper(const PFMultiplayerServerSecretSummary& model) :
        ModelWrapper<PFMultiplayerServerSecretSummary, Alloc>{ model },
        m_expirationDate{ model.expirationDate ? std::optional<time_t>{ *model.expirationDate } : std::nullopt },
        m_name{ SafeString(model.name) },
        m_version{ SafeString(model.version) }
    {
        SetModelPointers();
    }

    PFMultiplayerServerSecretSummaryWrapper(const PFMultiplayerServerSecretSummaryWrapper& src) :
        PFMultiplayerServerSecretSummaryWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerSecretSummaryWrapper(PFMultiplayerServerSecretSummaryWrapper&& src) :
        PFMultiplayerServerSecretSummaryWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerSecretSummaryWrapper& operator=(PFMultiplayerServerSecretSummaryWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerSecretSummaryWrapper() = default;

    friend void swap(PFMultiplayerServerSecretSummaryWrapper& lhs, PFMultiplayerServerSecretSummaryWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_expirationDate, rhs.m_expirationDate);
        swap(lhs.m_name, rhs.m_name);
        swap(lhs.m_version, rhs.m_version);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetExpirationDate(std::optional<time_t> value)
    {
        m_expirationDate = std::move(value);
        this->m_model.expirationDate = m_expirationDate ? m_expirationDate.operator->() : nullptr;
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

    void SetVersion(String value)
    {
        m_version = std::move(value);
        this->m_model.version =  m_version.empty() ? nullptr : m_version.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.expirationDate = m_expirationDate ? m_expirationDate.operator->() : nullptr;
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
        this->m_model.version = m_version.empty() ? nullptr : m_version.data();
    }

    std::optional<time_t> m_expirationDate;
    String m_name;
    String m_version;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerListSecretSummariesResponseWrapper : public ModelWrapper<PFMultiplayerServerListSecretSummariesResponse, Alloc>
{
public:
    using ModelType = PFMultiplayerServerListSecretSummariesResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerListSecretSummariesResponseWrapper() = default;

    PFMultiplayerServerListSecretSummariesResponseWrapper(const PFMultiplayerServerListSecretSummariesResponse& model) :
        ModelWrapper<PFMultiplayerServerListSecretSummariesResponse, Alloc>{ model },
        m_secretSummaries{ model.secretSummaries, model.secretSummaries + model.secretSummariesCount },
        m_skipToken{ SafeString(model.skipToken) }
    {
        SetModelPointers();
    }

    PFMultiplayerServerListSecretSummariesResponseWrapper(const PFMultiplayerServerListSecretSummariesResponseWrapper& src) :
        PFMultiplayerServerListSecretSummariesResponseWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerListSecretSummariesResponseWrapper(PFMultiplayerServerListSecretSummariesResponseWrapper&& src) :
        PFMultiplayerServerListSecretSummariesResponseWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerListSecretSummariesResponseWrapper& operator=(PFMultiplayerServerListSecretSummariesResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerListSecretSummariesResponseWrapper() = default;

    friend void swap(PFMultiplayerServerListSecretSummariesResponseWrapper& lhs, PFMultiplayerServerListSecretSummariesResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_secretSummaries, rhs.m_secretSummaries);
        swap(lhs.m_skipToken, rhs.m_skipToken);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetPageSize(int32_t value)
    {
        this->m_model.pageSize = value;
    }

    void SetSecretSummaries(ModelVector<PFMultiplayerServerSecretSummaryWrapper<Alloc>, Alloc> value)
    {
        m_secretSummaries = std::move(value);
        this->m_model.secretSummaries =  m_secretSummaries.empty() ? nullptr : m_secretSummaries.data();
        this->m_model.secretSummariesCount =  static_cast<uint32_t>(m_secretSummaries.size());
    }

    void SetSkipToken(String value)
    {
        m_skipToken = std::move(value);
        this->m_model.skipToken =  m_skipToken.empty() ? nullptr : m_skipToken.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.secretSummaries = m_secretSummaries.empty() ? nullptr : m_secretSummaries.data();
        this->m_model.skipToken = m_skipToken.empty() ? nullptr : m_skipToken.data();
    }

    ModelVector<PFMultiplayerServerSecretSummaryWrapper<Alloc>, Alloc> m_secretSummaries;
    String m_skipToken;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerBuildAliasParamsWrapper : public ModelWrapper<PFMultiplayerServerBuildAliasParams, Alloc>
{
public:
    using ModelType = PFMultiplayerServerBuildAliasParams;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerBuildAliasParamsWrapper() = default;

    PFMultiplayerServerBuildAliasParamsWrapper(const PFMultiplayerServerBuildAliasParams& model) :
        ModelWrapper<PFMultiplayerServerBuildAliasParams, Alloc>{ model },
        m_aliasId{ SafeString(model.aliasId) }
    {
        SetModelPointers();
    }

    PFMultiplayerServerBuildAliasParamsWrapper(const PFMultiplayerServerBuildAliasParamsWrapper& src) :
        PFMultiplayerServerBuildAliasParamsWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerBuildAliasParamsWrapper(PFMultiplayerServerBuildAliasParamsWrapper&& src) :
        PFMultiplayerServerBuildAliasParamsWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerBuildAliasParamsWrapper& operator=(PFMultiplayerServerBuildAliasParamsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerBuildAliasParamsWrapper() = default;

    friend void swap(PFMultiplayerServerBuildAliasParamsWrapper& lhs, PFMultiplayerServerBuildAliasParamsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_aliasId, rhs.m_aliasId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAliasId(String value)
    {
        m_aliasId = std::move(value);
        this->m_model.aliasId =  m_aliasId.empty() ? nullptr : m_aliasId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.aliasId = m_aliasId.empty() ? nullptr : m_aliasId.data();
    }

    String m_aliasId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerRequestMultiplayerServerRequestWrapper : public ModelWrapper<PFMultiplayerServerRequestMultiplayerServerRequest, Alloc>
{
public:
    using ModelType = PFMultiplayerServerRequestMultiplayerServerRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerRequestMultiplayerServerRequestWrapper() = default;

    PFMultiplayerServerRequestMultiplayerServerRequestWrapper(const PFMultiplayerServerRequestMultiplayerServerRequest& model) :
        ModelWrapper<PFMultiplayerServerRequestMultiplayerServerRequest, Alloc>{ model },
        m_buildAliasParams{ model.buildAliasParams ? std::optional<PFMultiplayerServerBuildAliasParamsWrapper<Alloc>>{ *model.buildAliasParams } : std::nullopt },
        m_buildId{ SafeString(model.buildId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_initialPlayers{ model.initialPlayers, model.initialPlayers + model.initialPlayersCount },
        m_preferredRegions{ model.preferredRegions, model.preferredRegions + model.preferredRegionsCount },
        m_sessionCookie{ SafeString(model.sessionCookie) },
        m_sessionId{ SafeString(model.sessionId) }
    {
        SetModelPointers();
    }

    PFMultiplayerServerRequestMultiplayerServerRequestWrapper(const PFMultiplayerServerRequestMultiplayerServerRequestWrapper& src) :
        PFMultiplayerServerRequestMultiplayerServerRequestWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerRequestMultiplayerServerRequestWrapper(PFMultiplayerServerRequestMultiplayerServerRequestWrapper&& src) :
        PFMultiplayerServerRequestMultiplayerServerRequestWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerRequestMultiplayerServerRequestWrapper& operator=(PFMultiplayerServerRequestMultiplayerServerRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerRequestMultiplayerServerRequestWrapper() = default;

    friend void swap(PFMultiplayerServerRequestMultiplayerServerRequestWrapper& lhs, PFMultiplayerServerRequestMultiplayerServerRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_buildAliasParams, rhs.m_buildAliasParams);
        swap(lhs.m_buildId, rhs.m_buildId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_initialPlayers, rhs.m_initialPlayers);
        swap(lhs.m_preferredRegions, rhs.m_preferredRegions);
        swap(lhs.m_sessionCookie, rhs.m_sessionCookie);
        swap(lhs.m_sessionId, rhs.m_sessionId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetBuildAliasParams(std::optional<PFMultiplayerServerBuildAliasParamsWrapper<Alloc>> value)
    {
        m_buildAliasParams = std::move(value);
        this->m_model.buildAliasParams = m_buildAliasParams ? &m_buildAliasParams->Model() : nullptr;
    }

    void SetBuildId(String value)
    {
        m_buildId = std::move(value);
        this->m_model.buildId =  m_buildId.empty() ? nullptr : m_buildId.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetInitialPlayers(CStringVector<Alloc> value)
    {
        m_initialPlayers = std::move(value);
        this->m_model.initialPlayers =  m_initialPlayers.empty() ? nullptr : m_initialPlayers.data();
        this->m_model.initialPlayersCount =  static_cast<uint32_t>(m_initialPlayers.size());
    }

    void SetPreferredRegions(CStringVector<Alloc> value)
    {
        m_preferredRegions = std::move(value);
        this->m_model.preferredRegions =  m_preferredRegions.empty() ? nullptr : m_preferredRegions.data();
        this->m_model.preferredRegionsCount =  static_cast<uint32_t>(m_preferredRegions.size());
    }

    void SetSessionCookie(String value)
    {
        m_sessionCookie = std::move(value);
        this->m_model.sessionCookie =  m_sessionCookie.empty() ? nullptr : m_sessionCookie.data();
    }

    void SetSessionId(String value)
    {
        m_sessionId = std::move(value);
        this->m_model.sessionId =  m_sessionId.empty() ? nullptr : m_sessionId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.buildAliasParams = m_buildAliasParams ?  &m_buildAliasParams->Model() : nullptr;
        this->m_model.buildId = m_buildId.empty() ? nullptr : m_buildId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.initialPlayers = m_initialPlayers.empty() ? nullptr : m_initialPlayers.data();
        this->m_model.preferredRegions = m_preferredRegions.empty() ? nullptr : m_preferredRegions.data();
        this->m_model.sessionCookie = m_sessionCookie.empty() ? nullptr : m_sessionCookie.data();
        this->m_model.sessionId = m_sessionId.empty() ? nullptr : m_sessionId.data();
    }

    std::optional<PFMultiplayerServerBuildAliasParamsWrapper<Alloc>> m_buildAliasParams;
    String m_buildId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    CStringVector<Alloc> m_initialPlayers;
    CStringVector<Alloc> m_preferredRegions;
    String m_sessionCookie;
    String m_sessionId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerConnectedPlayerWrapper : public ModelWrapper<PFMultiplayerServerConnectedPlayer, Alloc>
{
public:
    using ModelType = PFMultiplayerServerConnectedPlayer;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerConnectedPlayerWrapper() = default;

    PFMultiplayerServerConnectedPlayerWrapper(const PFMultiplayerServerConnectedPlayer& model) :
        ModelWrapper<PFMultiplayerServerConnectedPlayer, Alloc>{ model },
        m_playerId{ SafeString(model.playerId) }
    {
        SetModelPointers();
    }

    PFMultiplayerServerConnectedPlayerWrapper(const PFMultiplayerServerConnectedPlayerWrapper& src) :
        PFMultiplayerServerConnectedPlayerWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerConnectedPlayerWrapper(PFMultiplayerServerConnectedPlayerWrapper&& src) :
        PFMultiplayerServerConnectedPlayerWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerConnectedPlayerWrapper& operator=(PFMultiplayerServerConnectedPlayerWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerConnectedPlayerWrapper() = default;

    friend void swap(PFMultiplayerServerConnectedPlayerWrapper& lhs, PFMultiplayerServerConnectedPlayerWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_playerId, rhs.m_playerId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetPlayerId(String value)
    {
        m_playerId = std::move(value);
        this->m_model.playerId =  m_playerId.empty() ? nullptr : m_playerId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.playerId = m_playerId.empty() ? nullptr : m_playerId.data();
    }

    String m_playerId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerPortWrapper : public ModelWrapper<PFMultiplayerServerPort, Alloc>
{
public:
    using ModelType = PFMultiplayerServerPort;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerPortWrapper() = default;

    PFMultiplayerServerPortWrapper(const PFMultiplayerServerPort& model) :
        ModelWrapper<PFMultiplayerServerPort, Alloc>{ model },
        m_name{ SafeString(model.name) }
    {
        SetModelPointers();
    }

    PFMultiplayerServerPortWrapper(const PFMultiplayerServerPortWrapper& src) :
        PFMultiplayerServerPortWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerPortWrapper(PFMultiplayerServerPortWrapper&& src) :
        PFMultiplayerServerPortWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerPortWrapper& operator=(PFMultiplayerServerPortWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerPortWrapper() = default;

    friend void swap(PFMultiplayerServerPortWrapper& lhs, PFMultiplayerServerPortWrapper& rhs)
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

    void SetNum(int32_t value)
    {
        this->m_model.num = value;
    }

    void SetProtocol(PFMultiplayerServerProtocolType value)
    {
        this->m_model.protocol = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
    }

    String m_name;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerPublicIpAddressWrapper : public ModelWrapper<PFMultiplayerServerPublicIpAddress, Alloc>
{
public:
    using ModelType = PFMultiplayerServerPublicIpAddress;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerPublicIpAddressWrapper() = default;

    PFMultiplayerServerPublicIpAddressWrapper(const PFMultiplayerServerPublicIpAddress& model) :
        ModelWrapper<PFMultiplayerServerPublicIpAddress, Alloc>{ model },
        m_fQDN{ SafeString(model.fQDN) },
        m_ipAddress{ SafeString(model.ipAddress) },
        m_routingType{ SafeString(model.routingType) }
    {
        SetModelPointers();
    }

    PFMultiplayerServerPublicIpAddressWrapper(const PFMultiplayerServerPublicIpAddressWrapper& src) :
        PFMultiplayerServerPublicIpAddressWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerPublicIpAddressWrapper(PFMultiplayerServerPublicIpAddressWrapper&& src) :
        PFMultiplayerServerPublicIpAddressWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerPublicIpAddressWrapper& operator=(PFMultiplayerServerPublicIpAddressWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerPublicIpAddressWrapper() = default;

    friend void swap(PFMultiplayerServerPublicIpAddressWrapper& lhs, PFMultiplayerServerPublicIpAddressWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_fQDN, rhs.m_fQDN);
        swap(lhs.m_ipAddress, rhs.m_ipAddress);
        swap(lhs.m_routingType, rhs.m_routingType);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFQDN(String value)
    {
        m_fQDN = std::move(value);
        this->m_model.fQDN =  m_fQDN.empty() ? nullptr : m_fQDN.data();
    }

    void SetIpAddress(String value)
    {
        m_ipAddress = std::move(value);
        this->m_model.ipAddress =  m_ipAddress.empty() ? nullptr : m_ipAddress.data();
    }

    void SetRoutingType(String value)
    {
        m_routingType = std::move(value);
        this->m_model.routingType =  m_routingType.empty() ? nullptr : m_routingType.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.fQDN = m_fQDN.empty() ? nullptr : m_fQDN.data();
        this->m_model.ipAddress = m_ipAddress.empty() ? nullptr : m_ipAddress.data();
        this->m_model.routingType = m_routingType.empty() ? nullptr : m_routingType.data();
    }

    String m_fQDN;
    String m_ipAddress;
    String m_routingType;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerRequestMultiplayerServerResponseWrapper : public ModelWrapper<PFMultiplayerServerRequestMultiplayerServerResponse, Alloc>
{
public:
    using ModelType = PFMultiplayerServerRequestMultiplayerServerResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerRequestMultiplayerServerResponseWrapper() = default;

    PFMultiplayerServerRequestMultiplayerServerResponseWrapper(const PFMultiplayerServerRequestMultiplayerServerResponse& model) :
        ModelWrapper<PFMultiplayerServerRequestMultiplayerServerResponse, Alloc>{ model },
        m_buildId{ SafeString(model.buildId) },
        m_connectedPlayers{ model.connectedPlayers, model.connectedPlayers + model.connectedPlayersCount },
        m_fQDN{ SafeString(model.fQDN) },
        m_iPV4Address{ SafeString(model.iPV4Address) },
        m_lastStateTransitionTime{ model.lastStateTransitionTime ? std::optional<time_t>{ *model.lastStateTransitionTime } : std::nullopt },
        m_ports{ model.ports, model.ports + model.portsCount },
        m_publicIPV4Addresses{ model.publicIPV4Addresses, model.publicIPV4Addresses + model.publicIPV4AddressesCount },
        m_region{ SafeString(model.region) },
        m_serverId{ SafeString(model.serverId) },
        m_sessionId{ SafeString(model.sessionId) },
        m_state{ SafeString(model.state) },
        m_vmId{ SafeString(model.vmId) }
    {
        SetModelPointers();
    }

    PFMultiplayerServerRequestMultiplayerServerResponseWrapper(const PFMultiplayerServerRequestMultiplayerServerResponseWrapper& src) :
        PFMultiplayerServerRequestMultiplayerServerResponseWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerRequestMultiplayerServerResponseWrapper(PFMultiplayerServerRequestMultiplayerServerResponseWrapper&& src) :
        PFMultiplayerServerRequestMultiplayerServerResponseWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerRequestMultiplayerServerResponseWrapper& operator=(PFMultiplayerServerRequestMultiplayerServerResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerRequestMultiplayerServerResponseWrapper() = default;

    friend void swap(PFMultiplayerServerRequestMultiplayerServerResponseWrapper& lhs, PFMultiplayerServerRequestMultiplayerServerResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_buildId, rhs.m_buildId);
        swap(lhs.m_connectedPlayers, rhs.m_connectedPlayers);
        swap(lhs.m_fQDN, rhs.m_fQDN);
        swap(lhs.m_iPV4Address, rhs.m_iPV4Address);
        swap(lhs.m_lastStateTransitionTime, rhs.m_lastStateTransitionTime);
        swap(lhs.m_ports, rhs.m_ports);
        swap(lhs.m_publicIPV4Addresses, rhs.m_publicIPV4Addresses);
        swap(lhs.m_region, rhs.m_region);
        swap(lhs.m_serverId, rhs.m_serverId);
        swap(lhs.m_sessionId, rhs.m_sessionId);
        swap(lhs.m_state, rhs.m_state);
        swap(lhs.m_vmId, rhs.m_vmId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetBuildId(String value)
    {
        m_buildId = std::move(value);
        this->m_model.buildId =  m_buildId.empty() ? nullptr : m_buildId.data();
    }

    void SetConnectedPlayers(ModelVector<PFMultiplayerServerConnectedPlayerWrapper<Alloc>, Alloc> value)
    {
        m_connectedPlayers = std::move(value);
        this->m_model.connectedPlayers =  m_connectedPlayers.empty() ? nullptr : m_connectedPlayers.data();
        this->m_model.connectedPlayersCount =  static_cast<uint32_t>(m_connectedPlayers.size());
    }

    void SetFQDN(String value)
    {
        m_fQDN = std::move(value);
        this->m_model.fQDN =  m_fQDN.empty() ? nullptr : m_fQDN.data();
    }

    void SetIPV4Address(String value)
    {
        m_iPV4Address = std::move(value);
        this->m_model.iPV4Address =  m_iPV4Address.empty() ? nullptr : m_iPV4Address.data();
    }

    void SetLastStateTransitionTime(std::optional<time_t> value)
    {
        m_lastStateTransitionTime = std::move(value);
        this->m_model.lastStateTransitionTime = m_lastStateTransitionTime ? m_lastStateTransitionTime.operator->() : nullptr;
    }

    void SetPorts(ModelVector<PFMultiplayerServerPortWrapper<Alloc>, Alloc> value)
    {
        m_ports = std::move(value);
        this->m_model.ports =  m_ports.empty() ? nullptr : m_ports.data();
        this->m_model.portsCount =  static_cast<uint32_t>(m_ports.size());
    }

    void SetPublicIPV4Addresses(ModelVector<PFMultiplayerServerPublicIpAddressWrapper<Alloc>, Alloc> value)
    {
        m_publicIPV4Addresses = std::move(value);
        this->m_model.publicIPV4Addresses =  m_publicIPV4Addresses.empty() ? nullptr : m_publicIPV4Addresses.data();
        this->m_model.publicIPV4AddressesCount =  static_cast<uint32_t>(m_publicIPV4Addresses.size());
    }

    void SetRegion(String value)
    {
        m_region = std::move(value);
        this->m_model.region =  m_region.empty() ? nullptr : m_region.data();
    }

    void SetServerId(String value)
    {
        m_serverId = std::move(value);
        this->m_model.serverId =  m_serverId.empty() ? nullptr : m_serverId.data();
    }

    void SetSessionId(String value)
    {
        m_sessionId = std::move(value);
        this->m_model.sessionId =  m_sessionId.empty() ? nullptr : m_sessionId.data();
    }

    void SetState(String value)
    {
        m_state = std::move(value);
        this->m_model.state =  m_state.empty() ? nullptr : m_state.data();
    }

    void SetVmId(String value)
    {
        m_vmId = std::move(value);
        this->m_model.vmId =  m_vmId.empty() ? nullptr : m_vmId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.buildId = m_buildId.empty() ? nullptr : m_buildId.data();
        this->m_model.connectedPlayers = m_connectedPlayers.empty() ? nullptr : m_connectedPlayers.data();
        this->m_model.fQDN = m_fQDN.empty() ? nullptr : m_fQDN.data();
        this->m_model.iPV4Address = m_iPV4Address.empty() ? nullptr : m_iPV4Address.data();
        this->m_model.lastStateTransitionTime = m_lastStateTransitionTime ? m_lastStateTransitionTime.operator->() : nullptr;
        this->m_model.ports = m_ports.empty() ? nullptr : m_ports.data();
        this->m_model.publicIPV4Addresses = m_publicIPV4Addresses.empty() ? nullptr : m_publicIPV4Addresses.data();
        this->m_model.region = m_region.empty() ? nullptr : m_region.data();
        this->m_model.serverId = m_serverId.empty() ? nullptr : m_serverId.data();
        this->m_model.sessionId = m_sessionId.empty() ? nullptr : m_sessionId.data();
        this->m_model.state = m_state.empty() ? nullptr : m_state.data();
        this->m_model.vmId = m_vmId.empty() ? nullptr : m_vmId.data();
    }

    String m_buildId;
    ModelVector<PFMultiplayerServerConnectedPlayerWrapper<Alloc>, Alloc> m_connectedPlayers;
    String m_fQDN;
    String m_iPV4Address;
    std::optional<time_t> m_lastStateTransitionTime;
    ModelVector<PFMultiplayerServerPortWrapper<Alloc>, Alloc> m_ports;
    ModelVector<PFMultiplayerServerPublicIpAddressWrapper<Alloc>, Alloc> m_publicIPV4Addresses;
    String m_region;
    String m_serverId;
    String m_sessionId;
    String m_state;
    String m_vmId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerPartyInvitationConfigurationWrapper : public ModelWrapper<PFMultiplayerServerPartyInvitationConfiguration, Alloc>
{
public:
    using ModelType = PFMultiplayerServerPartyInvitationConfiguration;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerPartyInvitationConfigurationWrapper() = default;

    PFMultiplayerServerPartyInvitationConfigurationWrapper(const PFMultiplayerServerPartyInvitationConfiguration& model) :
        ModelWrapper<PFMultiplayerServerPartyInvitationConfiguration, Alloc>{ model },
        m_entityKeys{ model.entityKeys, model.entityKeys + model.entityKeysCount },
        m_identifier{ SafeString(model.identifier) },
        m_revocability{ SafeString(model.revocability) }
    {
        SetModelPointers();
    }

    PFMultiplayerServerPartyInvitationConfigurationWrapper(const PFMultiplayerServerPartyInvitationConfigurationWrapper& src) :
        PFMultiplayerServerPartyInvitationConfigurationWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerPartyInvitationConfigurationWrapper(PFMultiplayerServerPartyInvitationConfigurationWrapper&& src) :
        PFMultiplayerServerPartyInvitationConfigurationWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerPartyInvitationConfigurationWrapper& operator=(PFMultiplayerServerPartyInvitationConfigurationWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerPartyInvitationConfigurationWrapper() = default;

    friend void swap(PFMultiplayerServerPartyInvitationConfigurationWrapper& lhs, PFMultiplayerServerPartyInvitationConfigurationWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_entityKeys, rhs.m_entityKeys);
        swap(lhs.m_identifier, rhs.m_identifier);
        swap(lhs.m_revocability, rhs.m_revocability);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetEntityKeys(ModelVector<PFEntityKeyWrapper<Alloc>, Alloc> value)
    {
        m_entityKeys = std::move(value);
        this->m_model.entityKeys =  m_entityKeys.empty() ? nullptr : m_entityKeys.data();
        this->m_model.entityKeysCount =  static_cast<uint32_t>(m_entityKeys.size());
    }

    void SetIdentifier(String value)
    {
        m_identifier = std::move(value);
        this->m_model.identifier =  m_identifier.empty() ? nullptr : m_identifier.data();
    }

    void SetRevocability(String value)
    {
        m_revocability = std::move(value);
        this->m_model.revocability =  m_revocability.empty() ? nullptr : m_revocability.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.entityKeys = m_entityKeys.empty() ? nullptr : m_entityKeys.data();
        this->m_model.identifier = m_identifier.empty() ? nullptr : m_identifier.data();
        this->m_model.revocability = m_revocability.empty() ? nullptr : m_revocability.data();
    }

    ModelVector<PFEntityKeyWrapper<Alloc>, Alloc> m_entityKeys;
    String m_identifier;
    String m_revocability;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerPartyNetworkConfigurationWrapper : public ModelWrapper<PFMultiplayerServerPartyNetworkConfiguration, Alloc>
{
public:
    using ModelType = PFMultiplayerServerPartyNetworkConfiguration;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerPartyNetworkConfigurationWrapper() = default;

    PFMultiplayerServerPartyNetworkConfigurationWrapper(const PFMultiplayerServerPartyNetworkConfiguration& model) :
        ModelWrapper<PFMultiplayerServerPartyNetworkConfiguration, Alloc>{ model },
        m_directPeerConnectivityOptions{ SafeString(model.directPeerConnectivityOptions) },
        m_partyInvitationConfiguration{ model.partyInvitationConfiguration ? std::optional<PFMultiplayerServerPartyInvitationConfigurationWrapper<Alloc>>{ *model.partyInvitationConfiguration } : std::nullopt }
    {
        SetModelPointers();
    }

    PFMultiplayerServerPartyNetworkConfigurationWrapper(const PFMultiplayerServerPartyNetworkConfigurationWrapper& src) :
        PFMultiplayerServerPartyNetworkConfigurationWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerPartyNetworkConfigurationWrapper(PFMultiplayerServerPartyNetworkConfigurationWrapper&& src) :
        PFMultiplayerServerPartyNetworkConfigurationWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerPartyNetworkConfigurationWrapper& operator=(PFMultiplayerServerPartyNetworkConfigurationWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerPartyNetworkConfigurationWrapper() = default;

    friend void swap(PFMultiplayerServerPartyNetworkConfigurationWrapper& lhs, PFMultiplayerServerPartyNetworkConfigurationWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_directPeerConnectivityOptions, rhs.m_directPeerConnectivityOptions);
        swap(lhs.m_partyInvitationConfiguration, rhs.m_partyInvitationConfiguration);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetDirectPeerConnectivityOptions(String value)
    {
        m_directPeerConnectivityOptions = std::move(value);
        this->m_model.directPeerConnectivityOptions =  m_directPeerConnectivityOptions.empty() ? nullptr : m_directPeerConnectivityOptions.data();
    }

    void SetMaxDevices(uint32_t value)
    {
        this->m_model.maxDevices = value;
    }

    void SetMaxDevicesPerUser(uint32_t value)
    {
        this->m_model.maxDevicesPerUser = value;
    }

    void SetMaxEndpointsPerDevice(uint32_t value)
    {
        this->m_model.maxEndpointsPerDevice = value;
    }

    void SetMaxUsers(uint32_t value)
    {
        this->m_model.maxUsers = value;
    }

    void SetMaxUsersPerDevice(uint32_t value)
    {
        this->m_model.maxUsersPerDevice = value;
    }

    void SetPartyInvitationConfiguration(std::optional<PFMultiplayerServerPartyInvitationConfigurationWrapper<Alloc>> value)
    {
        m_partyInvitationConfiguration = std::move(value);
        this->m_model.partyInvitationConfiguration = m_partyInvitationConfiguration ? &m_partyInvitationConfiguration->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.directPeerConnectivityOptions = m_directPeerConnectivityOptions.empty() ? nullptr : m_directPeerConnectivityOptions.data();
        this->m_model.partyInvitationConfiguration = m_partyInvitationConfiguration ?  &m_partyInvitationConfiguration->Model() : nullptr;
    }

    String m_directPeerConnectivityOptions;
    std::optional<PFMultiplayerServerPartyInvitationConfigurationWrapper<Alloc>> m_partyInvitationConfiguration;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerRequestPartyServiceRequestWrapper : public ModelWrapper<PFMultiplayerServerRequestPartyServiceRequest, Alloc>
{
public:
    using ModelType = PFMultiplayerServerRequestPartyServiceRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerRequestPartyServiceRequestWrapper() = default;

    PFMultiplayerServerRequestPartyServiceRequestWrapper(const PFMultiplayerServerRequestPartyServiceRequest& model) :
        ModelWrapper<PFMultiplayerServerRequestPartyServiceRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_networkConfiguration{ model.networkConfiguration ? *model.networkConfiguration : decltype(*model.networkConfiguration){} },
        m_partyId{ SafeString(model.partyId) },
        m_preferredRegions{ model.preferredRegions, model.preferredRegions + model.preferredRegionsCount }
    {
        SetModelPointers();
    }

    PFMultiplayerServerRequestPartyServiceRequestWrapper(const PFMultiplayerServerRequestPartyServiceRequestWrapper& src) :
        PFMultiplayerServerRequestPartyServiceRequestWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerRequestPartyServiceRequestWrapper(PFMultiplayerServerRequestPartyServiceRequestWrapper&& src) :
        PFMultiplayerServerRequestPartyServiceRequestWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerRequestPartyServiceRequestWrapper& operator=(PFMultiplayerServerRequestPartyServiceRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerRequestPartyServiceRequestWrapper() = default;

    friend void swap(PFMultiplayerServerRequestPartyServiceRequestWrapper& lhs, PFMultiplayerServerRequestPartyServiceRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_networkConfiguration, rhs.m_networkConfiguration);
        swap(lhs.m_partyId, rhs.m_partyId);
        swap(lhs.m_preferredRegions, rhs.m_preferredRegions);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetNetworkConfiguration(PFMultiplayerServerPartyNetworkConfigurationWrapper<Alloc> value)
    {
        m_networkConfiguration = std::move(value);
        this->m_model.networkConfiguration = &m_networkConfiguration.Model();
    }

    void SetPartyId(String value)
    {
        m_partyId = std::move(value);
        this->m_model.partyId =  m_partyId.empty() ? nullptr : m_partyId.data();
    }

    void SetPreferredRegions(CStringVector<Alloc> value)
    {
        m_preferredRegions = std::move(value);
        this->m_model.preferredRegions =  m_preferredRegions.empty() ? nullptr : m_preferredRegions.data();
        this->m_model.preferredRegionsCount =  static_cast<uint32_t>(m_preferredRegions.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.networkConfiguration = &m_networkConfiguration.Model();
        this->m_model.partyId = m_partyId.empty() ? nullptr : m_partyId.data();
        this->m_model.preferredRegions = m_preferredRegions.empty() ? nullptr : m_preferredRegions.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    PFMultiplayerServerPartyNetworkConfigurationWrapper<Alloc> m_networkConfiguration;
    String m_partyId;
    CStringVector<Alloc> m_preferredRegions;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerRequestPartyServiceResponseWrapper : public ModelWrapper<PFMultiplayerServerRequestPartyServiceResponse, Alloc>
{
public:
    using ModelType = PFMultiplayerServerRequestPartyServiceResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerRequestPartyServiceResponseWrapper() = default;

    PFMultiplayerServerRequestPartyServiceResponseWrapper(const PFMultiplayerServerRequestPartyServiceResponse& model) :
        ModelWrapper<PFMultiplayerServerRequestPartyServiceResponse, Alloc>{ model },
        m_invitationId{ SafeString(model.invitationId) },
        m_partyId{ SafeString(model.partyId) },
        m_serializedNetworkDescriptor{ SafeString(model.serializedNetworkDescriptor) }
    {
        SetModelPointers();
    }

    PFMultiplayerServerRequestPartyServiceResponseWrapper(const PFMultiplayerServerRequestPartyServiceResponseWrapper& src) :
        PFMultiplayerServerRequestPartyServiceResponseWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerRequestPartyServiceResponseWrapper(PFMultiplayerServerRequestPartyServiceResponseWrapper&& src) :
        PFMultiplayerServerRequestPartyServiceResponseWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerRequestPartyServiceResponseWrapper& operator=(PFMultiplayerServerRequestPartyServiceResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerRequestPartyServiceResponseWrapper() = default;

    friend void swap(PFMultiplayerServerRequestPartyServiceResponseWrapper& lhs, PFMultiplayerServerRequestPartyServiceResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_invitationId, rhs.m_invitationId);
        swap(lhs.m_partyId, rhs.m_partyId);
        swap(lhs.m_serializedNetworkDescriptor, rhs.m_serializedNetworkDescriptor);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetInvitationId(String value)
    {
        m_invitationId = std::move(value);
        this->m_model.invitationId =  m_invitationId.empty() ? nullptr : m_invitationId.data();
    }

    void SetPartyId(String value)
    {
        m_partyId = std::move(value);
        this->m_model.partyId =  m_partyId.empty() ? nullptr : m_partyId.data();
    }

    void SetSerializedNetworkDescriptor(String value)
    {
        m_serializedNetworkDescriptor = std::move(value);
        this->m_model.serializedNetworkDescriptor =  m_serializedNetworkDescriptor.empty() ? nullptr : m_serializedNetworkDescriptor.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.invitationId = m_invitationId.empty() ? nullptr : m_invitationId.data();
        this->m_model.partyId = m_partyId.empty() ? nullptr : m_partyId.data();
        this->m_model.serializedNetworkDescriptor = m_serializedNetworkDescriptor.empty() ? nullptr : m_serializedNetworkDescriptor.data();
    }

    String m_invitationId;
    String m_partyId;
    String m_serializedNetworkDescriptor;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerSecretWrapper : public ModelWrapper<PFMultiplayerServerSecret, Alloc>
{
public:
    using ModelType = PFMultiplayerServerSecret;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerSecretWrapper() = default;

    PFMultiplayerServerSecretWrapper(const PFMultiplayerServerSecret& model) :
        ModelWrapper<PFMultiplayerServerSecret, Alloc>{ model },
        m_expirationDate{ model.expirationDate ? std::optional<time_t>{ *model.expirationDate } : std::nullopt },
        m_name{ SafeString(model.name) },
        m_value{ SafeString(model.value) }
    {
        SetModelPointers();
    }

    PFMultiplayerServerSecretWrapper(const PFMultiplayerServerSecretWrapper& src) :
        PFMultiplayerServerSecretWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerSecretWrapper(PFMultiplayerServerSecretWrapper&& src) :
        PFMultiplayerServerSecretWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerSecretWrapper& operator=(PFMultiplayerServerSecretWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerSecretWrapper() = default;

    friend void swap(PFMultiplayerServerSecretWrapper& lhs, PFMultiplayerServerSecretWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_expirationDate, rhs.m_expirationDate);
        swap(lhs.m_name, rhs.m_name);
        swap(lhs.m_value, rhs.m_value);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetExpirationDate(std::optional<time_t> value)
    {
        m_expirationDate = std::move(value);
        this->m_model.expirationDate = m_expirationDate ? m_expirationDate.operator->() : nullptr;
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
        this->m_model.expirationDate = m_expirationDate ? m_expirationDate.operator->() : nullptr;
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
        this->m_model.value = m_value.empty() ? nullptr : m_value.data();
    }

    std::optional<time_t> m_expirationDate;
    String m_name;
    String m_value;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFMultiplayerServerUploadSecretRequestWrapper : public ModelWrapper<PFMultiplayerServerUploadSecretRequest, Alloc>
{
public:
    using ModelType = PFMultiplayerServerUploadSecretRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFMultiplayerServerUploadSecretRequestWrapper() = default;

    PFMultiplayerServerUploadSecretRequestWrapper(const PFMultiplayerServerUploadSecretRequest& model) :
        ModelWrapper<PFMultiplayerServerUploadSecretRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_forceUpdate{ model.forceUpdate ? std::optional<bool>{ *model.forceUpdate } : std::nullopt },
        m_gameSecret{ model.gameSecret ? *model.gameSecret : decltype(*model.gameSecret){} }
    {
        SetModelPointers();
    }

    PFMultiplayerServerUploadSecretRequestWrapper(const PFMultiplayerServerUploadSecretRequestWrapper& src) :
        PFMultiplayerServerUploadSecretRequestWrapper{ src.Model() }
    {
    }

    PFMultiplayerServerUploadSecretRequestWrapper(PFMultiplayerServerUploadSecretRequestWrapper&& src) :
        PFMultiplayerServerUploadSecretRequestWrapper{}
    {
        swap(*this, src);
    }

    PFMultiplayerServerUploadSecretRequestWrapper& operator=(PFMultiplayerServerUploadSecretRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFMultiplayerServerUploadSecretRequestWrapper() = default;

    friend void swap(PFMultiplayerServerUploadSecretRequestWrapper& lhs, PFMultiplayerServerUploadSecretRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_forceUpdate, rhs.m_forceUpdate);
        swap(lhs.m_gameSecret, rhs.m_gameSecret);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetForceUpdate(std::optional<bool> value)
    {
        m_forceUpdate = std::move(value);
        this->m_model.forceUpdate = m_forceUpdate ? m_forceUpdate.operator->() : nullptr;
    }

    void SetGameSecret(PFMultiplayerServerSecretWrapper<Alloc> value)
    {
        m_gameSecret = std::move(value);
        this->m_model.gameSecret = &m_gameSecret.Model();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.forceUpdate = m_forceUpdate ? m_forceUpdate.operator->() : nullptr;
        this->m_model.gameSecret = &m_gameSecret.Model();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_forceUpdate;
    PFMultiplayerServerSecretWrapper<Alloc> m_gameSecret;
};

} // namespace Wrappers
} // namespace PlayFab

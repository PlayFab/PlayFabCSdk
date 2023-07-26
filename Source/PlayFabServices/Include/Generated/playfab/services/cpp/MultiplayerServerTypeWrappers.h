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
        m_includeAllRegions{ model.includeAllRegions ? std::optional<bool>{ *model.includeAllRegions } : std::nullopt }
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

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.includeAllRegions = m_includeAllRegions ? m_includeAllRegions.operator->() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_includeAllRegions;
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

} // namespace Wrappers
} // namespace PlayFab

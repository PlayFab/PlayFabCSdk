// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFCatalogTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogCatalogAlternateIdWrapper : public ModelWrapper<PFCatalogCatalogAlternateId, Alloc>
{
public:
    using ModelType = PFCatalogCatalogAlternateId;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogCatalogAlternateIdWrapper() = default;

    PFCatalogCatalogAlternateIdWrapper(const PFCatalogCatalogAlternateId& model) :
        ModelWrapper<PFCatalogCatalogAlternateId, Alloc>{ model },
        m_type{ SafeString(model.type) },
        m_value{ SafeString(model.value) }
    {
        SetModelPointers();
    }

    PFCatalogCatalogAlternateIdWrapper(const PFCatalogCatalogAlternateIdWrapper& src) :
        PFCatalogCatalogAlternateIdWrapper{ src.Model() }
    {
    }

    PFCatalogCatalogAlternateIdWrapper(PFCatalogCatalogAlternateIdWrapper&& src) :
        PFCatalogCatalogAlternateIdWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogCatalogAlternateIdWrapper& operator=(PFCatalogCatalogAlternateIdWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogCatalogAlternateIdWrapper() = default;

    friend void swap(PFCatalogCatalogAlternateIdWrapper& lhs, PFCatalogCatalogAlternateIdWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_type, rhs.m_type);
        swap(lhs.m_value, rhs.m_value);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetType(String value)
    {
        m_type = std::move(value);
        this->m_model.type =  m_type.empty() ? nullptr : m_type.data();
    }

    void SetValue(String value)
    {
        m_value = std::move(value);
        this->m_model.value =  m_value.empty() ? nullptr : m_value.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.type = m_type.empty() ? nullptr : m_type.data();
        this->m_model.value = m_value.empty() ? nullptr : m_value.data();
    }

    String m_type;
    String m_value;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogContentWrapper : public ModelWrapper<PFCatalogContent, Alloc>
{
public:
    using ModelType = PFCatalogContent;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogContentWrapper() = default;

    PFCatalogContentWrapper(const PFCatalogContent& model) :
        ModelWrapper<PFCatalogContent, Alloc>{ model },
        m_id{ SafeString(model.id) },
        m_maxClientVersion{ SafeString(model.maxClientVersion) },
        m_minClientVersion{ SafeString(model.minClientVersion) },
        m_tags{ model.tags, model.tags + model.tagsCount },
        m_type{ SafeString(model.type) },
        m_url{ SafeString(model.url) }
    {
        SetModelPointers();
    }

    PFCatalogContentWrapper(const PFCatalogContentWrapper& src) :
        PFCatalogContentWrapper{ src.Model() }
    {
    }

    PFCatalogContentWrapper(PFCatalogContentWrapper&& src) :
        PFCatalogContentWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogContentWrapper& operator=(PFCatalogContentWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogContentWrapper() = default;

    friend void swap(PFCatalogContentWrapper& lhs, PFCatalogContentWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_id, rhs.m_id);
        swap(lhs.m_maxClientVersion, rhs.m_maxClientVersion);
        swap(lhs.m_minClientVersion, rhs.m_minClientVersion);
        swap(lhs.m_tags, rhs.m_tags);
        swap(lhs.m_type, rhs.m_type);
        swap(lhs.m_url, rhs.m_url);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

    void SetMaxClientVersion(String value)
    {
        m_maxClientVersion = std::move(value);
        this->m_model.maxClientVersion =  m_maxClientVersion.empty() ? nullptr : m_maxClientVersion.data();
    }

    void SetMinClientVersion(String value)
    {
        m_minClientVersion = std::move(value);
        this->m_model.minClientVersion =  m_minClientVersion.empty() ? nullptr : m_minClientVersion.data();
    }

    void SetTags(CStringVector<Alloc> value)
    {
        m_tags = std::move(value);
        this->m_model.tags =  m_tags.empty() ? nullptr : m_tags.data();
        this->m_model.tagsCount =  static_cast<uint32_t>(m_tags.size());
    }

    void SetType(String value)
    {
        m_type = std::move(value);
        this->m_model.type =  m_type.empty() ? nullptr : m_type.data();
    }

    void SetUrl(String value)
    {
        m_url = std::move(value);
        this->m_model.url =  m_url.empty() ? nullptr : m_url.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
        this->m_model.maxClientVersion = m_maxClientVersion.empty() ? nullptr : m_maxClientVersion.data();
        this->m_model.minClientVersion = m_minClientVersion.empty() ? nullptr : m_minClientVersion.data();
        this->m_model.tags = m_tags.empty() ? nullptr : m_tags.data();
        this->m_model.type = m_type.empty() ? nullptr : m_type.data();
        this->m_model.url = m_url.empty() ? nullptr : m_url.data();
    }

    String m_id;
    String m_maxClientVersion;
    String m_minClientVersion;
    CStringVector<Alloc> m_tags;
    String m_type;
    String m_url;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogDeepLinkWrapper : public ModelWrapper<PFCatalogDeepLink, Alloc>
{
public:
    using ModelType = PFCatalogDeepLink;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogDeepLinkWrapper() = default;

    PFCatalogDeepLinkWrapper(const PFCatalogDeepLink& model) :
        ModelWrapper<PFCatalogDeepLink, Alloc>{ model },
        m_platform{ SafeString(model.platform) },
        m_url{ SafeString(model.url) }
    {
        SetModelPointers();
    }

    PFCatalogDeepLinkWrapper(const PFCatalogDeepLinkWrapper& src) :
        PFCatalogDeepLinkWrapper{ src.Model() }
    {
    }

    PFCatalogDeepLinkWrapper(PFCatalogDeepLinkWrapper&& src) :
        PFCatalogDeepLinkWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogDeepLinkWrapper& operator=(PFCatalogDeepLinkWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogDeepLinkWrapper() = default;

    friend void swap(PFCatalogDeepLinkWrapper& lhs, PFCatalogDeepLinkWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_platform, rhs.m_platform);
        swap(lhs.m_url, rhs.m_url);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetPlatform(String value)
    {
        m_platform = std::move(value);
        this->m_model.platform =  m_platform.empty() ? nullptr : m_platform.data();
    }

    void SetUrl(String value)
    {
        m_url = std::move(value);
        this->m_model.url =  m_url.empty() ? nullptr : m_url.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.platform = m_platform.empty() ? nullptr : m_platform.data();
        this->m_model.url = m_url.empty() ? nullptr : m_url.data();
    }

    String m_platform;
    String m_url;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogImageWrapper : public ModelWrapper<PFCatalogImage, Alloc>
{
public:
    using ModelType = PFCatalogImage;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogImageWrapper() = default;

    PFCatalogImageWrapper(const PFCatalogImage& model) :
        ModelWrapper<PFCatalogImage, Alloc>{ model },
        m_id{ SafeString(model.id) },
        m_tag{ SafeString(model.tag) },
        m_type{ SafeString(model.type) },
        m_url{ SafeString(model.url) }
    {
        SetModelPointers();
    }

    PFCatalogImageWrapper(const PFCatalogImageWrapper& src) :
        PFCatalogImageWrapper{ src.Model() }
    {
    }

    PFCatalogImageWrapper(PFCatalogImageWrapper&& src) :
        PFCatalogImageWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogImageWrapper& operator=(PFCatalogImageWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogImageWrapper() = default;

    friend void swap(PFCatalogImageWrapper& lhs, PFCatalogImageWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_id, rhs.m_id);
        swap(lhs.m_tag, rhs.m_tag);
        swap(lhs.m_type, rhs.m_type);
        swap(lhs.m_url, rhs.m_url);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

    void SetTag(String value)
    {
        m_tag = std::move(value);
        this->m_model.tag =  m_tag.empty() ? nullptr : m_tag.data();
    }

    void SetType(String value)
    {
        m_type = std::move(value);
        this->m_model.type =  m_type.empty() ? nullptr : m_type.data();
    }

    void SetUrl(String value)
    {
        m_url = std::move(value);
        this->m_model.url =  m_url.empty() ? nullptr : m_url.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
        this->m_model.tag = m_tag.empty() ? nullptr : m_tag.data();
        this->m_model.type = m_type.empty() ? nullptr : m_type.data();
        this->m_model.url = m_url.empty() ? nullptr : m_url.data();
    }

    String m_id;
    String m_tag;
    String m_type;
    String m_url;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogCatalogPriceAmountWrapper : public ModelWrapper<PFCatalogCatalogPriceAmount, Alloc>
{
public:
    using ModelType = PFCatalogCatalogPriceAmount;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogCatalogPriceAmountWrapper() = default;

    PFCatalogCatalogPriceAmountWrapper(const PFCatalogCatalogPriceAmount& model) :
        ModelWrapper<PFCatalogCatalogPriceAmount, Alloc>{ model },
        m_itemId{ SafeString(model.itemId) }
    {
        SetModelPointers();
    }

    PFCatalogCatalogPriceAmountWrapper(const PFCatalogCatalogPriceAmountWrapper& src) :
        PFCatalogCatalogPriceAmountWrapper{ src.Model() }
    {
    }

    PFCatalogCatalogPriceAmountWrapper(PFCatalogCatalogPriceAmountWrapper&& src) :
        PFCatalogCatalogPriceAmountWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogCatalogPriceAmountWrapper& operator=(PFCatalogCatalogPriceAmountWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogCatalogPriceAmountWrapper() = default;

    friend void swap(PFCatalogCatalogPriceAmountWrapper& lhs, PFCatalogCatalogPriceAmountWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_itemId, rhs.m_itemId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAmount(int32_t value)
    {
        this->m_model.amount = value;
    }

    void SetItemId(String value)
    {
        m_itemId = std::move(value);
        this->m_model.itemId =  m_itemId.empty() ? nullptr : m_itemId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.itemId = m_itemId.empty() ? nullptr : m_itemId.data();
    }

    String m_itemId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogCatalogPriceWrapper : public ModelWrapper<PFCatalogCatalogPrice, Alloc>
{
public:
    using ModelType = PFCatalogCatalogPrice;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogCatalogPriceWrapper() = default;

    PFCatalogCatalogPriceWrapper(const PFCatalogCatalogPrice& model) :
        ModelWrapper<PFCatalogCatalogPrice, Alloc>{ model },
        m_amounts{ model.amounts, model.amounts + model.amountsCount },
        m_unitAmount{ model.unitAmount ? std::optional<int32_t>{ *model.unitAmount } : std::nullopt },
        m_unitDurationInSeconds{ model.unitDurationInSeconds ? std::optional<double>{ *model.unitDurationInSeconds } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogCatalogPriceWrapper(const PFCatalogCatalogPriceWrapper& src) :
        PFCatalogCatalogPriceWrapper{ src.Model() }
    {
    }

    PFCatalogCatalogPriceWrapper(PFCatalogCatalogPriceWrapper&& src) :
        PFCatalogCatalogPriceWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogCatalogPriceWrapper& operator=(PFCatalogCatalogPriceWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogCatalogPriceWrapper() = default;

    friend void swap(PFCatalogCatalogPriceWrapper& lhs, PFCatalogCatalogPriceWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_amounts, rhs.m_amounts);
        swap(lhs.m_unitAmount, rhs.m_unitAmount);
        swap(lhs.m_unitDurationInSeconds, rhs.m_unitDurationInSeconds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAmounts(ModelVector<PFCatalogCatalogPriceAmountWrapper<Alloc>, Alloc> value)
    {
        m_amounts = std::move(value);
        this->m_model.amounts =  m_amounts.empty() ? nullptr : m_amounts.data();
        this->m_model.amountsCount =  static_cast<uint32_t>(m_amounts.size());
    }

    void SetUnitAmount(std::optional<int32_t> value)
    {
        m_unitAmount = std::move(value);
        this->m_model.unitAmount = m_unitAmount ? m_unitAmount.operator->() : nullptr;
    }

    void SetUnitDurationInSeconds(std::optional<double> value)
    {
        m_unitDurationInSeconds = std::move(value);
        this->m_model.unitDurationInSeconds = m_unitDurationInSeconds ? m_unitDurationInSeconds.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.amounts = m_amounts.empty() ? nullptr : m_amounts.data();
        this->m_model.unitAmount = m_unitAmount ? m_unitAmount.operator->() : nullptr;
        this->m_model.unitDurationInSeconds = m_unitDurationInSeconds ? m_unitDurationInSeconds.operator->() : nullptr;
    }

    ModelVector<PFCatalogCatalogPriceAmountWrapper<Alloc>, Alloc> m_amounts;
    std::optional<int32_t> m_unitAmount;
    std::optional<double> m_unitDurationInSeconds;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogCatalogPriceOptionsWrapper : public ModelWrapper<PFCatalogCatalogPriceOptions, Alloc>
{
public:
    using ModelType = PFCatalogCatalogPriceOptions;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogCatalogPriceOptionsWrapper() = default;

    PFCatalogCatalogPriceOptionsWrapper(const PFCatalogCatalogPriceOptions& model) :
        ModelWrapper<PFCatalogCatalogPriceOptions, Alloc>{ model },
        m_prices{ model.prices, model.prices + model.pricesCount }
    {
        SetModelPointers();
    }

    PFCatalogCatalogPriceOptionsWrapper(const PFCatalogCatalogPriceOptionsWrapper& src) :
        PFCatalogCatalogPriceOptionsWrapper{ src.Model() }
    {
    }

    PFCatalogCatalogPriceOptionsWrapper(PFCatalogCatalogPriceOptionsWrapper&& src) :
        PFCatalogCatalogPriceOptionsWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogCatalogPriceOptionsWrapper& operator=(PFCatalogCatalogPriceOptionsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogCatalogPriceOptionsWrapper() = default;

    friend void swap(PFCatalogCatalogPriceOptionsWrapper& lhs, PFCatalogCatalogPriceOptionsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_prices, rhs.m_prices);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetPrices(ModelVector<PFCatalogCatalogPriceWrapper<Alloc>, Alloc> value)
    {
        m_prices = std::move(value);
        this->m_model.prices =  m_prices.empty() ? nullptr : m_prices.data();
        this->m_model.pricesCount =  static_cast<uint32_t>(m_prices.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.prices = m_prices.empty() ? nullptr : m_prices.data();
    }

    ModelVector<PFCatalogCatalogPriceWrapper<Alloc>, Alloc> m_prices;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogCatalogItemReferenceWrapper : public ModelWrapper<PFCatalogCatalogItemReference, Alloc>
{
public:
    using ModelType = PFCatalogCatalogItemReference;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogCatalogItemReferenceWrapper() = default;

    PFCatalogCatalogItemReferenceWrapper(const PFCatalogCatalogItemReference& model) :
        ModelWrapper<PFCatalogCatalogItemReference, Alloc>{ model },
        m_amount{ model.amount ? std::optional<int32_t>{ *model.amount } : std::nullopt },
        m_id{ SafeString(model.id) },
        m_priceOptions{ model.priceOptions ? std::optional<PFCatalogCatalogPriceOptionsWrapper<Alloc>>{ *model.priceOptions } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogCatalogItemReferenceWrapper(const PFCatalogCatalogItemReferenceWrapper& src) :
        PFCatalogCatalogItemReferenceWrapper{ src.Model() }
    {
    }

    PFCatalogCatalogItemReferenceWrapper(PFCatalogCatalogItemReferenceWrapper&& src) :
        PFCatalogCatalogItemReferenceWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogCatalogItemReferenceWrapper& operator=(PFCatalogCatalogItemReferenceWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogCatalogItemReferenceWrapper() = default;

    friend void swap(PFCatalogCatalogItemReferenceWrapper& lhs, PFCatalogCatalogItemReferenceWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_amount, rhs.m_amount);
        swap(lhs.m_id, rhs.m_id);
        swap(lhs.m_priceOptions, rhs.m_priceOptions);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAmount(std::optional<int32_t> value)
    {
        m_amount = std::move(value);
        this->m_model.amount = m_amount ? m_amount.operator->() : nullptr;
    }

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

    void SetPriceOptions(std::optional<PFCatalogCatalogPriceOptionsWrapper<Alloc>> value)
    {
        m_priceOptions = std::move(value);
        this->m_model.priceOptions = m_priceOptions ? &m_priceOptions->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.amount = m_amount ? m_amount.operator->() : nullptr;
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
        this->m_model.priceOptions = m_priceOptions ?  &m_priceOptions->Model() : nullptr;
    }

    std::optional<int32_t> m_amount;
    String m_id;
    std::optional<PFCatalogCatalogPriceOptionsWrapper<Alloc>> m_priceOptions;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogKeywordSetWrapper : public ModelWrapper<PFCatalogKeywordSet, Alloc>
{
public:
    using ModelType = PFCatalogKeywordSet;
    using DictionaryEntryType = PFCatalogKeywordSetDictionaryEntry;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogKeywordSetWrapper() = default;

    PFCatalogKeywordSetWrapper(const PFCatalogKeywordSet& model) :
        ModelWrapper<PFCatalogKeywordSet, Alloc>{ model },
        m_values{ model.values, model.values + model.valuesCount }
    {
        SetModelPointers();
    }

    PFCatalogKeywordSetWrapper(const PFCatalogKeywordSetWrapper& src) :
        PFCatalogKeywordSetWrapper{ src.Model() }
    {
    }

    PFCatalogKeywordSetWrapper(PFCatalogKeywordSetWrapper&& src) :
        PFCatalogKeywordSetWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogKeywordSetWrapper& operator=(PFCatalogKeywordSetWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogKeywordSetWrapper() = default;

    friend void swap(PFCatalogKeywordSetWrapper& lhs, PFCatalogKeywordSetWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_values, rhs.m_values);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetValues(CStringVector<Alloc> value)
    {
        m_values = std::move(value);
        this->m_model.values =  m_values.empty() ? nullptr : m_values.data();
        this->m_model.valuesCount =  static_cast<uint32_t>(m_values.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.values = m_values.empty() ? nullptr : m_values.data();
    }

    CStringVector<Alloc> m_values;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogModerationStateWrapper : public ModelWrapper<PFCatalogModerationState, Alloc>
{
public:
    using ModelType = PFCatalogModerationState;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogModerationStateWrapper() = default;

    PFCatalogModerationStateWrapper(const PFCatalogModerationState& model) :
        ModelWrapper<PFCatalogModerationState, Alloc>{ model },
        m_lastModifiedDate{ model.lastModifiedDate ? std::optional<time_t>{ *model.lastModifiedDate } : std::nullopt },
        m_reason{ SafeString(model.reason) },
        m_status{ model.status ? std::optional<PFCatalogModerationStatus>{ *model.status } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogModerationStateWrapper(const PFCatalogModerationStateWrapper& src) :
        PFCatalogModerationStateWrapper{ src.Model() }
    {
    }

    PFCatalogModerationStateWrapper(PFCatalogModerationStateWrapper&& src) :
        PFCatalogModerationStateWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogModerationStateWrapper& operator=(PFCatalogModerationStateWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogModerationStateWrapper() = default;

    friend void swap(PFCatalogModerationStateWrapper& lhs, PFCatalogModerationStateWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_lastModifiedDate, rhs.m_lastModifiedDate);
        swap(lhs.m_reason, rhs.m_reason);
        swap(lhs.m_status, rhs.m_status);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetLastModifiedDate(std::optional<time_t> value)
    {
        m_lastModifiedDate = std::move(value);
        this->m_model.lastModifiedDate = m_lastModifiedDate ? m_lastModifiedDate.operator->() : nullptr;
    }

    void SetReason(String value)
    {
        m_reason = std::move(value);
        this->m_model.reason =  m_reason.empty() ? nullptr : m_reason.data();
    }

    void SetStatus(std::optional<PFCatalogModerationStatus> value)
    {
        m_status = std::move(value);
        this->m_model.status = m_status ? m_status.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.lastModifiedDate = m_lastModifiedDate ? m_lastModifiedDate.operator->() : nullptr;
        this->m_model.reason = m_reason.empty() ? nullptr : m_reason.data();
        this->m_model.status = m_status ? m_status.operator->() : nullptr;
    }

    std::optional<time_t> m_lastModifiedDate;
    String m_reason;
    std::optional<PFCatalogModerationStatus> m_status;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogRatingWrapper : public ModelWrapper<PFCatalogRating, Alloc>
{
public:
    using ModelType = PFCatalogRating;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogRatingWrapper() = default;

    PFCatalogRatingWrapper(const PFCatalogRating& model) :
        ModelWrapper<PFCatalogRating, Alloc>{ model },
        m_average{ model.average ? std::optional<float>{ *model.average } : std::nullopt },
        m_count1Star{ model.count1Star ? std::optional<int32_t>{ *model.count1Star } : std::nullopt },
        m_count2Star{ model.count2Star ? std::optional<int32_t>{ *model.count2Star } : std::nullopt },
        m_count3Star{ model.count3Star ? std::optional<int32_t>{ *model.count3Star } : std::nullopt },
        m_count4Star{ model.count4Star ? std::optional<int32_t>{ *model.count4Star } : std::nullopt },
        m_count5Star{ model.count5Star ? std::optional<int32_t>{ *model.count5Star } : std::nullopt },
        m_totalCount{ model.totalCount ? std::optional<int32_t>{ *model.totalCount } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogRatingWrapper(const PFCatalogRatingWrapper& src) :
        PFCatalogRatingWrapper{ src.Model() }
    {
    }

    PFCatalogRatingWrapper(PFCatalogRatingWrapper&& src) :
        PFCatalogRatingWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogRatingWrapper& operator=(PFCatalogRatingWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogRatingWrapper() = default;

    friend void swap(PFCatalogRatingWrapper& lhs, PFCatalogRatingWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_average, rhs.m_average);
        swap(lhs.m_count1Star, rhs.m_count1Star);
        swap(lhs.m_count2Star, rhs.m_count2Star);
        swap(lhs.m_count3Star, rhs.m_count3Star);
        swap(lhs.m_count4Star, rhs.m_count4Star);
        swap(lhs.m_count5Star, rhs.m_count5Star);
        swap(lhs.m_totalCount, rhs.m_totalCount);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAverage(std::optional<float> value)
    {
        m_average = std::move(value);
        this->m_model.average = m_average ? m_average.operator->() : nullptr;
    }

    void SetCount1Star(std::optional<int32_t> value)
    {
        m_count1Star = std::move(value);
        this->m_model.count1Star = m_count1Star ? m_count1Star.operator->() : nullptr;
    }

    void SetCount2Star(std::optional<int32_t> value)
    {
        m_count2Star = std::move(value);
        this->m_model.count2Star = m_count2Star ? m_count2Star.operator->() : nullptr;
    }

    void SetCount3Star(std::optional<int32_t> value)
    {
        m_count3Star = std::move(value);
        this->m_model.count3Star = m_count3Star ? m_count3Star.operator->() : nullptr;
    }

    void SetCount4Star(std::optional<int32_t> value)
    {
        m_count4Star = std::move(value);
        this->m_model.count4Star = m_count4Star ? m_count4Star.operator->() : nullptr;
    }

    void SetCount5Star(std::optional<int32_t> value)
    {
        m_count5Star = std::move(value);
        this->m_model.count5Star = m_count5Star ? m_count5Star.operator->() : nullptr;
    }

    void SetTotalCount(std::optional<int32_t> value)
    {
        m_totalCount = std::move(value);
        this->m_model.totalCount = m_totalCount ? m_totalCount.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.average = m_average ? m_average.operator->() : nullptr;
        this->m_model.count1Star = m_count1Star ? m_count1Star.operator->() : nullptr;
        this->m_model.count2Star = m_count2Star ? m_count2Star.operator->() : nullptr;
        this->m_model.count3Star = m_count3Star ? m_count3Star.operator->() : nullptr;
        this->m_model.count4Star = m_count4Star ? m_count4Star.operator->() : nullptr;
        this->m_model.count5Star = m_count5Star ? m_count5Star.operator->() : nullptr;
        this->m_model.totalCount = m_totalCount ? m_totalCount.operator->() : nullptr;
    }

    std::optional<float> m_average;
    std::optional<int32_t> m_count1Star;
    std::optional<int32_t> m_count2Star;
    std::optional<int32_t> m_count3Star;
    std::optional<int32_t> m_count4Star;
    std::optional<int32_t> m_count5Star;
    std::optional<int32_t> m_totalCount;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogFilterOptionsWrapper : public ModelWrapper<PFCatalogFilterOptions, Alloc>
{
public:
    using ModelType = PFCatalogFilterOptions;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogFilterOptionsWrapper() = default;

    PFCatalogFilterOptionsWrapper(const PFCatalogFilterOptions& model) :
        ModelWrapper<PFCatalogFilterOptions, Alloc>{ model },
        m_filter{ SafeString(model.filter) },
        m_includeAllItems{ model.includeAllItems ? std::optional<bool>{ *model.includeAllItems } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogFilterOptionsWrapper(const PFCatalogFilterOptionsWrapper& src) :
        PFCatalogFilterOptionsWrapper{ src.Model() }
    {
    }

    PFCatalogFilterOptionsWrapper(PFCatalogFilterOptionsWrapper&& src) :
        PFCatalogFilterOptionsWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogFilterOptionsWrapper& operator=(PFCatalogFilterOptionsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogFilterOptionsWrapper() = default;

    friend void swap(PFCatalogFilterOptionsWrapper& lhs, PFCatalogFilterOptionsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_filter, rhs.m_filter);
        swap(lhs.m_includeAllItems, rhs.m_includeAllItems);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFilter(String value)
    {
        m_filter = std::move(value);
        this->m_model.filter =  m_filter.empty() ? nullptr : m_filter.data();
    }

    void SetIncludeAllItems(std::optional<bool> value)
    {
        m_includeAllItems = std::move(value);
        this->m_model.includeAllItems = m_includeAllItems ? m_includeAllItems.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.filter = m_filter.empty() ? nullptr : m_filter.data();
        this->m_model.includeAllItems = m_includeAllItems ? m_includeAllItems.operator->() : nullptr;
    }

    String m_filter;
    std::optional<bool> m_includeAllItems;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogPermissionsWrapper : public ModelWrapper<PFCatalogPermissions, Alloc>
{
public:
    using ModelType = PFCatalogPermissions;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogPermissionsWrapper() = default;

    PFCatalogPermissionsWrapper(const PFCatalogPermissions& model) :
        ModelWrapper<PFCatalogPermissions, Alloc>{ model },
        m_segmentIds{ model.segmentIds, model.segmentIds + model.segmentIdsCount }
    {
        SetModelPointers();
    }

    PFCatalogPermissionsWrapper(const PFCatalogPermissionsWrapper& src) :
        PFCatalogPermissionsWrapper{ src.Model() }
    {
    }

    PFCatalogPermissionsWrapper(PFCatalogPermissionsWrapper&& src) :
        PFCatalogPermissionsWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogPermissionsWrapper& operator=(PFCatalogPermissionsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogPermissionsWrapper() = default;

    friend void swap(PFCatalogPermissionsWrapper& lhs, PFCatalogPermissionsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_segmentIds, rhs.m_segmentIds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetSegmentIds(CStringVector<Alloc> value)
    {
        m_segmentIds = std::move(value);
        this->m_model.segmentIds =  m_segmentIds.empty() ? nullptr : m_segmentIds.data();
        this->m_model.segmentIdsCount =  static_cast<uint32_t>(m_segmentIds.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.segmentIds = m_segmentIds.empty() ? nullptr : m_segmentIds.data();
    }

    CStringVector<Alloc> m_segmentIds;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogCatalogPriceAmountOverrideWrapper : public ModelWrapper<PFCatalogCatalogPriceAmountOverride, Alloc>
{
public:
    using ModelType = PFCatalogCatalogPriceAmountOverride;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogCatalogPriceAmountOverrideWrapper() = default;

    PFCatalogCatalogPriceAmountOverrideWrapper(const PFCatalogCatalogPriceAmountOverride& model) :
        ModelWrapper<PFCatalogCatalogPriceAmountOverride, Alloc>{ model },
        m_fixedValue{ model.fixedValue ? std::optional<int32_t>{ *model.fixedValue } : std::nullopt },
        m_itemId{ SafeString(model.itemId) },
        m_multiplier{ model.multiplier ? std::optional<double>{ *model.multiplier } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogCatalogPriceAmountOverrideWrapper(const PFCatalogCatalogPriceAmountOverrideWrapper& src) :
        PFCatalogCatalogPriceAmountOverrideWrapper{ src.Model() }
    {
    }

    PFCatalogCatalogPriceAmountOverrideWrapper(PFCatalogCatalogPriceAmountOverrideWrapper&& src) :
        PFCatalogCatalogPriceAmountOverrideWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogCatalogPriceAmountOverrideWrapper& operator=(PFCatalogCatalogPriceAmountOverrideWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogCatalogPriceAmountOverrideWrapper() = default;

    friend void swap(PFCatalogCatalogPriceAmountOverrideWrapper& lhs, PFCatalogCatalogPriceAmountOverrideWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_fixedValue, rhs.m_fixedValue);
        swap(lhs.m_itemId, rhs.m_itemId);
        swap(lhs.m_multiplier, rhs.m_multiplier);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFixedValue(std::optional<int32_t> value)
    {
        m_fixedValue = std::move(value);
        this->m_model.fixedValue = m_fixedValue ? m_fixedValue.operator->() : nullptr;
    }

    void SetItemId(String value)
    {
        m_itemId = std::move(value);
        this->m_model.itemId =  m_itemId.empty() ? nullptr : m_itemId.data();
    }

    void SetMultiplier(std::optional<double> value)
    {
        m_multiplier = std::move(value);
        this->m_model.multiplier = m_multiplier ? m_multiplier.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.fixedValue = m_fixedValue ? m_fixedValue.operator->() : nullptr;
        this->m_model.itemId = m_itemId.empty() ? nullptr : m_itemId.data();
        this->m_model.multiplier = m_multiplier ? m_multiplier.operator->() : nullptr;
    }

    std::optional<int32_t> m_fixedValue;
    String m_itemId;
    std::optional<double> m_multiplier;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogCatalogPriceOverrideWrapper : public ModelWrapper<PFCatalogCatalogPriceOverride, Alloc>
{
public:
    using ModelType = PFCatalogCatalogPriceOverride;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogCatalogPriceOverrideWrapper() = default;

    PFCatalogCatalogPriceOverrideWrapper(const PFCatalogCatalogPriceOverride& model) :
        ModelWrapper<PFCatalogCatalogPriceOverride, Alloc>{ model },
        m_amounts{ model.amounts, model.amounts + model.amountsCount }
    {
        SetModelPointers();
    }

    PFCatalogCatalogPriceOverrideWrapper(const PFCatalogCatalogPriceOverrideWrapper& src) :
        PFCatalogCatalogPriceOverrideWrapper{ src.Model() }
    {
    }

    PFCatalogCatalogPriceOverrideWrapper(PFCatalogCatalogPriceOverrideWrapper&& src) :
        PFCatalogCatalogPriceOverrideWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogCatalogPriceOverrideWrapper& operator=(PFCatalogCatalogPriceOverrideWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogCatalogPriceOverrideWrapper() = default;

    friend void swap(PFCatalogCatalogPriceOverrideWrapper& lhs, PFCatalogCatalogPriceOverrideWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_amounts, rhs.m_amounts);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAmounts(ModelVector<PFCatalogCatalogPriceAmountOverrideWrapper<Alloc>, Alloc> value)
    {
        m_amounts = std::move(value);
        this->m_model.amounts =  m_amounts.empty() ? nullptr : m_amounts.data();
        this->m_model.amountsCount =  static_cast<uint32_t>(m_amounts.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.amounts = m_amounts.empty() ? nullptr : m_amounts.data();
    }

    ModelVector<PFCatalogCatalogPriceAmountOverrideWrapper<Alloc>, Alloc> m_amounts;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogCatalogPriceOptionsOverrideWrapper : public ModelWrapper<PFCatalogCatalogPriceOptionsOverride, Alloc>
{
public:
    using ModelType = PFCatalogCatalogPriceOptionsOverride;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogCatalogPriceOptionsOverrideWrapper() = default;

    PFCatalogCatalogPriceOptionsOverrideWrapper(const PFCatalogCatalogPriceOptionsOverride& model) :
        ModelWrapper<PFCatalogCatalogPriceOptionsOverride, Alloc>{ model },
        m_prices{ model.prices, model.prices + model.pricesCount }
    {
        SetModelPointers();
    }

    PFCatalogCatalogPriceOptionsOverrideWrapper(const PFCatalogCatalogPriceOptionsOverrideWrapper& src) :
        PFCatalogCatalogPriceOptionsOverrideWrapper{ src.Model() }
    {
    }

    PFCatalogCatalogPriceOptionsOverrideWrapper(PFCatalogCatalogPriceOptionsOverrideWrapper&& src) :
        PFCatalogCatalogPriceOptionsOverrideWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogCatalogPriceOptionsOverrideWrapper& operator=(PFCatalogCatalogPriceOptionsOverrideWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogCatalogPriceOptionsOverrideWrapper() = default;

    friend void swap(PFCatalogCatalogPriceOptionsOverrideWrapper& lhs, PFCatalogCatalogPriceOptionsOverrideWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_prices, rhs.m_prices);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetPrices(ModelVector<PFCatalogCatalogPriceOverrideWrapper<Alloc>, Alloc> value)
    {
        m_prices = std::move(value);
        this->m_model.prices =  m_prices.empty() ? nullptr : m_prices.data();
        this->m_model.pricesCount =  static_cast<uint32_t>(m_prices.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.prices = m_prices.empty() ? nullptr : m_prices.data();
    }

    ModelVector<PFCatalogCatalogPriceOverrideWrapper<Alloc>, Alloc> m_prices;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogStoreDetailsWrapper : public ModelWrapper<PFCatalogStoreDetails, Alloc>
{
public:
    using ModelType = PFCatalogStoreDetails;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogStoreDetailsWrapper() = default;

    PFCatalogStoreDetailsWrapper(const PFCatalogStoreDetails& model) :
        ModelWrapper<PFCatalogStoreDetails, Alloc>{ model },
        m_filterOptions{ model.filterOptions ? std::optional<PFCatalogFilterOptionsWrapper<Alloc>>{ *model.filterOptions } : std::nullopt },
        m_permissions{ model.permissions ? std::optional<PFCatalogPermissionsWrapper<Alloc>>{ *model.permissions } : std::nullopt },
        m_priceOptionsOverride{ model.priceOptionsOverride ? std::optional<PFCatalogCatalogPriceOptionsOverrideWrapper<Alloc>>{ *model.priceOptionsOverride } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogStoreDetailsWrapper(const PFCatalogStoreDetailsWrapper& src) :
        PFCatalogStoreDetailsWrapper{ src.Model() }
    {
    }

    PFCatalogStoreDetailsWrapper(PFCatalogStoreDetailsWrapper&& src) :
        PFCatalogStoreDetailsWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogStoreDetailsWrapper& operator=(PFCatalogStoreDetailsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogStoreDetailsWrapper() = default;

    friend void swap(PFCatalogStoreDetailsWrapper& lhs, PFCatalogStoreDetailsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_filterOptions, rhs.m_filterOptions);
        swap(lhs.m_permissions, rhs.m_permissions);
        swap(lhs.m_priceOptionsOverride, rhs.m_priceOptionsOverride);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFilterOptions(std::optional<PFCatalogFilterOptionsWrapper<Alloc>> value)
    {
        m_filterOptions = std::move(value);
        this->m_model.filterOptions = m_filterOptions ? &m_filterOptions->Model() : nullptr;
    }

    void SetPermissions(std::optional<PFCatalogPermissionsWrapper<Alloc>> value)
    {
        m_permissions = std::move(value);
        this->m_model.permissions = m_permissions ? &m_permissions->Model() : nullptr;
    }

    void SetPriceOptionsOverride(std::optional<PFCatalogCatalogPriceOptionsOverrideWrapper<Alloc>> value)
    {
        m_priceOptionsOverride = std::move(value);
        this->m_model.priceOptionsOverride = m_priceOptionsOverride ? &m_priceOptionsOverride->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.filterOptions = m_filterOptions ?  &m_filterOptions->Model() : nullptr;
        this->m_model.permissions = m_permissions ?  &m_permissions->Model() : nullptr;
        this->m_model.priceOptionsOverride = m_priceOptionsOverride ?  &m_priceOptionsOverride->Model() : nullptr;
    }

    std::optional<PFCatalogFilterOptionsWrapper<Alloc>> m_filterOptions;
    std::optional<PFCatalogPermissionsWrapper<Alloc>> m_permissions;
    std::optional<PFCatalogCatalogPriceOptionsOverrideWrapper<Alloc>> m_priceOptionsOverride;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogCatalogItemWrapper : public ModelWrapper<PFCatalogCatalogItem, Alloc>
{
public:
    using ModelType = PFCatalogCatalogItem;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogCatalogItemWrapper() = default;

    PFCatalogCatalogItemWrapper(const PFCatalogCatalogItem& model) :
        ModelWrapper<PFCatalogCatalogItem, Alloc>{ model },
        m_alternateIds{ model.alternateIds, model.alternateIds + model.alternateIdsCount },
        m_contents{ model.contents, model.contents + model.contentsCount },
        m_contentType{ SafeString(model.contentType) },
        m_creationDate{ model.creationDate ? std::optional<time_t>{ *model.creationDate } : std::nullopt },
        m_creatorEntity{ model.creatorEntity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.creatorEntity } : std::nullopt },
        m_deepLinks{ model.deepLinks, model.deepLinks + model.deepLinksCount },
        m_defaultStackId{ SafeString(model.defaultStackId) },
        m_description{ model.description, model.description + model.descriptionCount },
        m_displayProperties{ model.displayProperties },
        m_displayVersion{ SafeString(model.displayVersion) },
        m_endDate{ model.endDate ? std::optional<time_t>{ *model.endDate } : std::nullopt },
        m_eTag{ SafeString(model.eTag) },
        m_id{ SafeString(model.id) },
        m_images{ model.images, model.images + model.imagesCount },
        m_isHidden{ model.isHidden ? std::optional<bool>{ *model.isHidden } : std::nullopt },
        m_itemReferences{ model.itemReferences, model.itemReferences + model.itemReferencesCount },
        m_keywords{ model.keywords, model.keywords + model.keywordsCount },
        m_lastModifiedDate{ model.lastModifiedDate ? std::optional<time_t>{ *model.lastModifiedDate } : std::nullopt },
        m_moderation{ model.moderation ? std::optional<PFCatalogModerationStateWrapper<Alloc>>{ *model.moderation } : std::nullopt },
        m_platforms{ model.platforms, model.platforms + model.platformsCount },
        m_priceOptions{ model.priceOptions ? std::optional<PFCatalogCatalogPriceOptionsWrapper<Alloc>>{ *model.priceOptions } : std::nullopt },
        m_rating{ model.rating ? std::optional<PFCatalogRatingWrapper<Alloc>>{ *model.rating } : std::nullopt },
        m_startDate{ model.startDate ? std::optional<time_t>{ *model.startDate } : std::nullopt },
        m_storeDetails{ model.storeDetails ? std::optional<PFCatalogStoreDetailsWrapper<Alloc>>{ *model.storeDetails } : std::nullopt },
        m_tags{ model.tags, model.tags + model.tagsCount },
        m_title{ model.title, model.title + model.titleCount },
        m_type{ SafeString(model.type) }
    {
        SetModelPointers();
    }

    PFCatalogCatalogItemWrapper(const PFCatalogCatalogItemWrapper& src) :
        PFCatalogCatalogItemWrapper{ src.Model() }
    {
    }

    PFCatalogCatalogItemWrapper(PFCatalogCatalogItemWrapper&& src) :
        PFCatalogCatalogItemWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogCatalogItemWrapper& operator=(PFCatalogCatalogItemWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogCatalogItemWrapper() = default;

    friend void swap(PFCatalogCatalogItemWrapper& lhs, PFCatalogCatalogItemWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_alternateIds, rhs.m_alternateIds);
        swap(lhs.m_contents, rhs.m_contents);
        swap(lhs.m_contentType, rhs.m_contentType);
        swap(lhs.m_creationDate, rhs.m_creationDate);
        swap(lhs.m_creatorEntity, rhs.m_creatorEntity);
        swap(lhs.m_deepLinks, rhs.m_deepLinks);
        swap(lhs.m_defaultStackId, rhs.m_defaultStackId);
        swap(lhs.m_description, rhs.m_description);
        swap(lhs.m_displayProperties, rhs.m_displayProperties);
        swap(lhs.m_displayVersion, rhs.m_displayVersion);
        swap(lhs.m_endDate, rhs.m_endDate);
        swap(lhs.m_eTag, rhs.m_eTag);
        swap(lhs.m_id, rhs.m_id);
        swap(lhs.m_images, rhs.m_images);
        swap(lhs.m_isHidden, rhs.m_isHidden);
        swap(lhs.m_itemReferences, rhs.m_itemReferences);
        swap(lhs.m_keywords, rhs.m_keywords);
        swap(lhs.m_lastModifiedDate, rhs.m_lastModifiedDate);
        swap(lhs.m_moderation, rhs.m_moderation);
        swap(lhs.m_platforms, rhs.m_platforms);
        swap(lhs.m_priceOptions, rhs.m_priceOptions);
        swap(lhs.m_rating, rhs.m_rating);
        swap(lhs.m_startDate, rhs.m_startDate);
        swap(lhs.m_storeDetails, rhs.m_storeDetails);
        swap(lhs.m_tags, rhs.m_tags);
        swap(lhs.m_title, rhs.m_title);
        swap(lhs.m_type, rhs.m_type);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAlternateIds(ModelVector<PFCatalogCatalogAlternateIdWrapper<Alloc>, Alloc> value)
    {
        m_alternateIds = std::move(value);
        this->m_model.alternateIds =  m_alternateIds.empty() ? nullptr : m_alternateIds.data();
        this->m_model.alternateIdsCount =  static_cast<uint32_t>(m_alternateIds.size());
    }

    void SetContents(ModelVector<PFCatalogContentWrapper<Alloc>, Alloc> value)
    {
        m_contents = std::move(value);
        this->m_model.contents =  m_contents.empty() ? nullptr : m_contents.data();
        this->m_model.contentsCount =  static_cast<uint32_t>(m_contents.size());
    }

    void SetContentType(String value)
    {
        m_contentType = std::move(value);
        this->m_model.contentType =  m_contentType.empty() ? nullptr : m_contentType.data();
    }

    void SetCreationDate(std::optional<time_t> value)
    {
        m_creationDate = std::move(value);
        this->m_model.creationDate = m_creationDate ? m_creationDate.operator->() : nullptr;
    }

    void SetCreatorEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_creatorEntity = std::move(value);
        this->m_model.creatorEntity = m_creatorEntity ? &m_creatorEntity->Model() : nullptr;
    }

    void SetDeepLinks(ModelVector<PFCatalogDeepLinkWrapper<Alloc>, Alloc> value)
    {
        m_deepLinks = std::move(value);
        this->m_model.deepLinks =  m_deepLinks.empty() ? nullptr : m_deepLinks.data();
        this->m_model.deepLinksCount =  static_cast<uint32_t>(m_deepLinks.size());
    }

    void SetDefaultStackId(String value)
    {
        m_defaultStackId = std::move(value);
        this->m_model.defaultStackId =  m_defaultStackId.empty() ? nullptr : m_defaultStackId.data();
    }

    void SetDescription(StringDictionaryEntryVector<Alloc> value)
    {
        m_description = std::move(value);
        this->m_model.description =  m_description.empty() ? nullptr : m_description.data();
        this->m_model.descriptionCount =  static_cast<uint32_t>(m_description.size());
    }

    void SetDisplayProperties(JsonObject<Alloc> value)
    {
        m_displayProperties = std::move(value);
        this->m_model.displayProperties.stringValue = m_displayProperties.stringValue.empty() ? nullptr : m_displayProperties.stringValue.data();
    }

    void SetDisplayVersion(String value)
    {
        m_displayVersion = std::move(value);
        this->m_model.displayVersion =  m_displayVersion.empty() ? nullptr : m_displayVersion.data();
    }

    void SetEndDate(std::optional<time_t> value)
    {
        m_endDate = std::move(value);
        this->m_model.endDate = m_endDate ? m_endDate.operator->() : nullptr;
    }

    void SetETag(String value)
    {
        m_eTag = std::move(value);
        this->m_model.eTag =  m_eTag.empty() ? nullptr : m_eTag.data();
    }

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

    void SetImages(ModelVector<PFCatalogImageWrapper<Alloc>, Alloc> value)
    {
        m_images = std::move(value);
        this->m_model.images =  m_images.empty() ? nullptr : m_images.data();
        this->m_model.imagesCount =  static_cast<uint32_t>(m_images.size());
    }

    void SetIsHidden(std::optional<bool> value)
    {
        m_isHidden = std::move(value);
        this->m_model.isHidden = m_isHidden ? m_isHidden.operator->() : nullptr;
    }

    void SetItemReferences(ModelVector<PFCatalogCatalogItemReferenceWrapper<Alloc>, Alloc> value)
    {
        m_itemReferences = std::move(value);
        this->m_model.itemReferences =  m_itemReferences.empty() ? nullptr : m_itemReferences.data();
        this->m_model.itemReferencesCount =  static_cast<uint32_t>(m_itemReferences.size());
    }

    void SetKeywords(ModelDictionaryEntryVector<PFCatalogKeywordSetWrapper<Alloc>, Alloc> value)
    {
        m_keywords = std::move(value);
        this->m_model.keywords =  m_keywords.empty() ? nullptr : m_keywords.data();
        this->m_model.keywordsCount =  static_cast<uint32_t>(m_keywords.size());
    }

    void SetLastModifiedDate(std::optional<time_t> value)
    {
        m_lastModifiedDate = std::move(value);
        this->m_model.lastModifiedDate = m_lastModifiedDate ? m_lastModifiedDate.operator->() : nullptr;
    }

    void SetModeration(std::optional<PFCatalogModerationStateWrapper<Alloc>> value)
    {
        m_moderation = std::move(value);
        this->m_model.moderation = m_moderation ? &m_moderation->Model() : nullptr;
    }

    void SetPlatforms(CStringVector<Alloc> value)
    {
        m_platforms = std::move(value);
        this->m_model.platforms =  m_platforms.empty() ? nullptr : m_platforms.data();
        this->m_model.platformsCount =  static_cast<uint32_t>(m_platforms.size());
    }

    void SetPriceOptions(std::optional<PFCatalogCatalogPriceOptionsWrapper<Alloc>> value)
    {
        m_priceOptions = std::move(value);
        this->m_model.priceOptions = m_priceOptions ? &m_priceOptions->Model() : nullptr;
    }

    void SetRating(std::optional<PFCatalogRatingWrapper<Alloc>> value)
    {
        m_rating = std::move(value);
        this->m_model.rating = m_rating ? &m_rating->Model() : nullptr;
    }

    void SetStartDate(std::optional<time_t> value)
    {
        m_startDate = std::move(value);
        this->m_model.startDate = m_startDate ? m_startDate.operator->() : nullptr;
    }

    void SetStoreDetails(std::optional<PFCatalogStoreDetailsWrapper<Alloc>> value)
    {
        m_storeDetails = std::move(value);
        this->m_model.storeDetails = m_storeDetails ? &m_storeDetails->Model() : nullptr;
    }

    void SetTags(CStringVector<Alloc> value)
    {
        m_tags = std::move(value);
        this->m_model.tags =  m_tags.empty() ? nullptr : m_tags.data();
        this->m_model.tagsCount =  static_cast<uint32_t>(m_tags.size());
    }

    void SetTitle(StringDictionaryEntryVector<Alloc> value)
    {
        m_title = std::move(value);
        this->m_model.title =  m_title.empty() ? nullptr : m_title.data();
        this->m_model.titleCount =  static_cast<uint32_t>(m_title.size());
    }

    void SetType(String value)
    {
        m_type = std::move(value);
        this->m_model.type =  m_type.empty() ? nullptr : m_type.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.alternateIds = m_alternateIds.empty() ? nullptr : m_alternateIds.data();
        this->m_model.contents = m_contents.empty() ? nullptr : m_contents.data();
        this->m_model.contentType = m_contentType.empty() ? nullptr : m_contentType.data();
        this->m_model.creationDate = m_creationDate ? m_creationDate.operator->() : nullptr;
        this->m_model.creatorEntity = m_creatorEntity ?  &m_creatorEntity->Model() : nullptr;
        this->m_model.deepLinks = m_deepLinks.empty() ? nullptr : m_deepLinks.data();
        this->m_model.defaultStackId = m_defaultStackId.empty() ? nullptr : m_defaultStackId.data();
        this->m_model.description = m_description.empty() ? nullptr : m_description.data();
        this->m_model.displayProperties.stringValue = m_displayProperties.stringValue.empty() ? nullptr : m_displayProperties.stringValue.data();
        this->m_model.displayVersion = m_displayVersion.empty() ? nullptr : m_displayVersion.data();
        this->m_model.endDate = m_endDate ? m_endDate.operator->() : nullptr;
        this->m_model.eTag = m_eTag.empty() ? nullptr : m_eTag.data();
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
        this->m_model.images = m_images.empty() ? nullptr : m_images.data();
        this->m_model.isHidden = m_isHidden ? m_isHidden.operator->() : nullptr;
        this->m_model.itemReferences = m_itemReferences.empty() ? nullptr : m_itemReferences.data();
        this->m_model.keywords = m_keywords.empty() ? nullptr : m_keywords.data();
        this->m_model.lastModifiedDate = m_lastModifiedDate ? m_lastModifiedDate.operator->() : nullptr;
        this->m_model.moderation = m_moderation ?  &m_moderation->Model() : nullptr;
        this->m_model.platforms = m_platforms.empty() ? nullptr : m_platforms.data();
        this->m_model.priceOptions = m_priceOptions ?  &m_priceOptions->Model() : nullptr;
        this->m_model.rating = m_rating ?  &m_rating->Model() : nullptr;
        this->m_model.startDate = m_startDate ? m_startDate.operator->() : nullptr;
        this->m_model.storeDetails = m_storeDetails ?  &m_storeDetails->Model() : nullptr;
        this->m_model.tags = m_tags.empty() ? nullptr : m_tags.data();
        this->m_model.title = m_title.empty() ? nullptr : m_title.data();
        this->m_model.type = m_type.empty() ? nullptr : m_type.data();
    }

    ModelVector<PFCatalogCatalogAlternateIdWrapper<Alloc>, Alloc> m_alternateIds;
    ModelVector<PFCatalogContentWrapper<Alloc>, Alloc> m_contents;
    String m_contentType;
    std::optional<time_t> m_creationDate;
    std::optional<PFEntityKeyWrapper<Alloc>> m_creatorEntity;
    ModelVector<PFCatalogDeepLinkWrapper<Alloc>, Alloc> m_deepLinks;
    String m_defaultStackId;
    StringDictionaryEntryVector<Alloc> m_description;
    JsonObject<Alloc> m_displayProperties;
    String m_displayVersion;
    std::optional<time_t> m_endDate;
    String m_eTag;
    String m_id;
    ModelVector<PFCatalogImageWrapper<Alloc>, Alloc> m_images;
    std::optional<bool> m_isHidden;
    ModelVector<PFCatalogCatalogItemReferenceWrapper<Alloc>, Alloc> m_itemReferences;
    ModelDictionaryEntryVector<PFCatalogKeywordSetWrapper<Alloc>, Alloc> m_keywords;
    std::optional<time_t> m_lastModifiedDate;
    std::optional<PFCatalogModerationStateWrapper<Alloc>> m_moderation;
    CStringVector<Alloc> m_platforms;
    std::optional<PFCatalogCatalogPriceOptionsWrapper<Alloc>> m_priceOptions;
    std::optional<PFCatalogRatingWrapper<Alloc>> m_rating;
    std::optional<time_t> m_startDate;
    std::optional<PFCatalogStoreDetailsWrapper<Alloc>> m_storeDetails;
    CStringVector<Alloc> m_tags;
    StringDictionaryEntryVector<Alloc> m_title;
    String m_type;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogCreateDraftItemRequestWrapper : public ModelWrapper<PFCatalogCreateDraftItemRequest, Alloc>
{
public:
    using ModelType = PFCatalogCreateDraftItemRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogCreateDraftItemRequestWrapper() = default;

    PFCatalogCreateDraftItemRequestWrapper(const PFCatalogCreateDraftItemRequest& model) :
        ModelWrapper<PFCatalogCreateDraftItemRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_item{ model.item ? std::optional<PFCatalogCatalogItemWrapper<Alloc>>{ *model.item } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogCreateDraftItemRequestWrapper(const PFCatalogCreateDraftItemRequestWrapper& src) :
        PFCatalogCreateDraftItemRequestWrapper{ src.Model() }
    {
    }

    PFCatalogCreateDraftItemRequestWrapper(PFCatalogCreateDraftItemRequestWrapper&& src) :
        PFCatalogCreateDraftItemRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogCreateDraftItemRequestWrapper& operator=(PFCatalogCreateDraftItemRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogCreateDraftItemRequestWrapper() = default;

    friend void swap(PFCatalogCreateDraftItemRequestWrapper& lhs, PFCatalogCreateDraftItemRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_item, rhs.m_item);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetItem(std::optional<PFCatalogCatalogItemWrapper<Alloc>> value)
    {
        m_item = std::move(value);
        this->m_model.item = m_item ? &m_item->Model() : nullptr;
    }

    void SetPublish(bool value)
    {
        this->m_model.publish = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.item = m_item ?  &m_item->Model() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFCatalogCatalogItemWrapper<Alloc>> m_item;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogCreateDraftItemResponseWrapper : public ModelWrapper<PFCatalogCreateDraftItemResponse, Alloc>
{
public:
    using ModelType = PFCatalogCreateDraftItemResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogCreateDraftItemResponseWrapper() = default;

    PFCatalogCreateDraftItemResponseWrapper(const PFCatalogCreateDraftItemResponse& model) :
        ModelWrapper<PFCatalogCreateDraftItemResponse, Alloc>{ model },
        m_item{ model.item ? std::optional<PFCatalogCatalogItemWrapper<Alloc>>{ *model.item } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogCreateDraftItemResponseWrapper(const PFCatalogCreateDraftItemResponseWrapper& src) :
        PFCatalogCreateDraftItemResponseWrapper{ src.Model() }
    {
    }

    PFCatalogCreateDraftItemResponseWrapper(PFCatalogCreateDraftItemResponseWrapper&& src) :
        PFCatalogCreateDraftItemResponseWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogCreateDraftItemResponseWrapper& operator=(PFCatalogCreateDraftItemResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogCreateDraftItemResponseWrapper() = default;

    friend void swap(PFCatalogCreateDraftItemResponseWrapper& lhs, PFCatalogCreateDraftItemResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_item, rhs.m_item);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetItem(std::optional<PFCatalogCatalogItemWrapper<Alloc>> value)
    {
        m_item = std::move(value);
        this->m_model.item = m_item ? &m_item->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.item = m_item ?  &m_item->Model() : nullptr;
    }

    std::optional<PFCatalogCatalogItemWrapper<Alloc>> m_item;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogUploadInfoWrapper : public ModelWrapper<PFCatalogUploadInfo, Alloc>
{
public:
    using ModelType = PFCatalogUploadInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogUploadInfoWrapper() = default;

    PFCatalogUploadInfoWrapper(const PFCatalogUploadInfo& model) :
        ModelWrapper<PFCatalogUploadInfo, Alloc>{ model },
        m_fileName{ SafeString(model.fileName) }
    {
        SetModelPointers();
    }

    PFCatalogUploadInfoWrapper(const PFCatalogUploadInfoWrapper& src) :
        PFCatalogUploadInfoWrapper{ src.Model() }
    {
    }

    PFCatalogUploadInfoWrapper(PFCatalogUploadInfoWrapper&& src) :
        PFCatalogUploadInfoWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogUploadInfoWrapper& operator=(PFCatalogUploadInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogUploadInfoWrapper() = default;

    friend void swap(PFCatalogUploadInfoWrapper& lhs, PFCatalogUploadInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_fileName, rhs.m_fileName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFileName(String value)
    {
        m_fileName = std::move(value);
        this->m_model.fileName =  m_fileName.empty() ? nullptr : m_fileName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.fileName = m_fileName.empty() ? nullptr : m_fileName.data();
    }

    String m_fileName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogCreateUploadUrlsRequestWrapper : public ModelWrapper<PFCatalogCreateUploadUrlsRequest, Alloc>
{
public:
    using ModelType = PFCatalogCreateUploadUrlsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogCreateUploadUrlsRequestWrapper() = default;

    PFCatalogCreateUploadUrlsRequestWrapper(const PFCatalogCreateUploadUrlsRequest& model) :
        ModelWrapper<PFCatalogCreateUploadUrlsRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_files{ model.files, model.files + model.filesCount }
    {
        SetModelPointers();
    }

    PFCatalogCreateUploadUrlsRequestWrapper(const PFCatalogCreateUploadUrlsRequestWrapper& src) :
        PFCatalogCreateUploadUrlsRequestWrapper{ src.Model() }
    {
    }

    PFCatalogCreateUploadUrlsRequestWrapper(PFCatalogCreateUploadUrlsRequestWrapper&& src) :
        PFCatalogCreateUploadUrlsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogCreateUploadUrlsRequestWrapper& operator=(PFCatalogCreateUploadUrlsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogCreateUploadUrlsRequestWrapper() = default;

    friend void swap(PFCatalogCreateUploadUrlsRequestWrapper& lhs, PFCatalogCreateUploadUrlsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_files, rhs.m_files);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetFiles(ModelVector<PFCatalogUploadInfoWrapper<Alloc>, Alloc> value)
    {
        m_files = std::move(value);
        this->m_model.files =  m_files.empty() ? nullptr : m_files.data();
        this->m_model.filesCount =  static_cast<uint32_t>(m_files.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.files = m_files.empty() ? nullptr : m_files.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    ModelVector<PFCatalogUploadInfoWrapper<Alloc>, Alloc> m_files;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogUploadUrlMetadataWrapper : public ModelWrapper<PFCatalogUploadUrlMetadata, Alloc>
{
public:
    using ModelType = PFCatalogUploadUrlMetadata;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogUploadUrlMetadataWrapper() = default;

    PFCatalogUploadUrlMetadataWrapper(const PFCatalogUploadUrlMetadata& model) :
        ModelWrapper<PFCatalogUploadUrlMetadata, Alloc>{ model },
        m_fileName{ SafeString(model.fileName) },
        m_id{ SafeString(model.id) },
        m_url{ SafeString(model.url) }
    {
        SetModelPointers();
    }

    PFCatalogUploadUrlMetadataWrapper(const PFCatalogUploadUrlMetadataWrapper& src) :
        PFCatalogUploadUrlMetadataWrapper{ src.Model() }
    {
    }

    PFCatalogUploadUrlMetadataWrapper(PFCatalogUploadUrlMetadataWrapper&& src) :
        PFCatalogUploadUrlMetadataWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogUploadUrlMetadataWrapper& operator=(PFCatalogUploadUrlMetadataWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogUploadUrlMetadataWrapper() = default;

    friend void swap(PFCatalogUploadUrlMetadataWrapper& lhs, PFCatalogUploadUrlMetadataWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_fileName, rhs.m_fileName);
        swap(lhs.m_id, rhs.m_id);
        swap(lhs.m_url, rhs.m_url);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFileName(String value)
    {
        m_fileName = std::move(value);
        this->m_model.fileName =  m_fileName.empty() ? nullptr : m_fileName.data();
    }

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

    void SetUrl(String value)
    {
        m_url = std::move(value);
        this->m_model.url =  m_url.empty() ? nullptr : m_url.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.fileName = m_fileName.empty() ? nullptr : m_fileName.data();
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
        this->m_model.url = m_url.empty() ? nullptr : m_url.data();
    }

    String m_fileName;
    String m_id;
    String m_url;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogCreateUploadUrlsResponseWrapper : public ModelWrapper<PFCatalogCreateUploadUrlsResponse, Alloc>
{
public:
    using ModelType = PFCatalogCreateUploadUrlsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogCreateUploadUrlsResponseWrapper() = default;

    PFCatalogCreateUploadUrlsResponseWrapper(const PFCatalogCreateUploadUrlsResponse& model) :
        ModelWrapper<PFCatalogCreateUploadUrlsResponse, Alloc>{ model },
        m_uploadUrls{ model.uploadUrls, model.uploadUrls + model.uploadUrlsCount }
    {
        SetModelPointers();
    }

    PFCatalogCreateUploadUrlsResponseWrapper(const PFCatalogCreateUploadUrlsResponseWrapper& src) :
        PFCatalogCreateUploadUrlsResponseWrapper{ src.Model() }
    {
    }

    PFCatalogCreateUploadUrlsResponseWrapper(PFCatalogCreateUploadUrlsResponseWrapper&& src) :
        PFCatalogCreateUploadUrlsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogCreateUploadUrlsResponseWrapper& operator=(PFCatalogCreateUploadUrlsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogCreateUploadUrlsResponseWrapper() = default;

    friend void swap(PFCatalogCreateUploadUrlsResponseWrapper& lhs, PFCatalogCreateUploadUrlsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_uploadUrls, rhs.m_uploadUrls);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetUploadUrls(ModelVector<PFCatalogUploadUrlMetadataWrapper<Alloc>, Alloc> value)
    {
        m_uploadUrls = std::move(value);
        this->m_model.uploadUrls =  m_uploadUrls.empty() ? nullptr : m_uploadUrls.data();
        this->m_model.uploadUrlsCount =  static_cast<uint32_t>(m_uploadUrls.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.uploadUrls = m_uploadUrls.empty() ? nullptr : m_uploadUrls.data();
    }

    ModelVector<PFCatalogUploadUrlMetadataWrapper<Alloc>, Alloc> m_uploadUrls;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogDeleteEntityItemReviewsRequestWrapper : public ModelWrapper<PFCatalogDeleteEntityItemReviewsRequest, Alloc>
{
public:
    using ModelType = PFCatalogDeleteEntityItemReviewsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogDeleteEntityItemReviewsRequestWrapper() = default;

    PFCatalogDeleteEntityItemReviewsRequestWrapper(const PFCatalogDeleteEntityItemReviewsRequest& model) :
        ModelWrapper<PFCatalogDeleteEntityItemReviewsRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogDeleteEntityItemReviewsRequestWrapper(const PFCatalogDeleteEntityItemReviewsRequestWrapper& src) :
        PFCatalogDeleteEntityItemReviewsRequestWrapper{ src.Model() }
    {
    }

    PFCatalogDeleteEntityItemReviewsRequestWrapper(PFCatalogDeleteEntityItemReviewsRequestWrapper&& src) :
        PFCatalogDeleteEntityItemReviewsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogDeleteEntityItemReviewsRequestWrapper& operator=(PFCatalogDeleteEntityItemReviewsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogDeleteEntityItemReviewsRequestWrapper() = default;

    friend void swap(PFCatalogDeleteEntityItemReviewsRequestWrapper& lhs, PFCatalogDeleteEntityItemReviewsRequestWrapper& rhs)
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
class PFCatalogDeleteItemRequestWrapper : public ModelWrapper<PFCatalogDeleteItemRequest, Alloc>
{
public:
    using ModelType = PFCatalogDeleteItemRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogDeleteItemRequestWrapper() = default;

    PFCatalogDeleteItemRequestWrapper(const PFCatalogDeleteItemRequest& model) :
        ModelWrapper<PFCatalogDeleteItemRequest, Alloc>{ model },
        m_alternateId{ model.alternateId ? std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>>{ *model.alternateId } : std::nullopt },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_id{ SafeString(model.id) }
    {
        SetModelPointers();
    }

    PFCatalogDeleteItemRequestWrapper(const PFCatalogDeleteItemRequestWrapper& src) :
        PFCatalogDeleteItemRequestWrapper{ src.Model() }
    {
    }

    PFCatalogDeleteItemRequestWrapper(PFCatalogDeleteItemRequestWrapper&& src) :
        PFCatalogDeleteItemRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogDeleteItemRequestWrapper& operator=(PFCatalogDeleteItemRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogDeleteItemRequestWrapper() = default;

    friend void swap(PFCatalogDeleteItemRequestWrapper& lhs, PFCatalogDeleteItemRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_alternateId, rhs.m_alternateId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_id, rhs.m_id);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAlternateId(std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> value)
    {
        m_alternateId = std::move(value);
        this->m_model.alternateId = m_alternateId ? &m_alternateId->Model() : nullptr;
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

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.alternateId = m_alternateId ?  &m_alternateId->Model() : nullptr;
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
    }

    std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> m_alternateId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_id;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetCatalogConfigRequestWrapper : public ModelWrapper<PFCatalogGetCatalogConfigRequest, Alloc>
{
public:
    using ModelType = PFCatalogGetCatalogConfigRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetCatalogConfigRequestWrapper() = default;

    PFCatalogGetCatalogConfigRequestWrapper(const PFCatalogGetCatalogConfigRequest& model) :
        ModelWrapper<PFCatalogGetCatalogConfigRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount }
    {
        SetModelPointers();
    }

    PFCatalogGetCatalogConfigRequestWrapper(const PFCatalogGetCatalogConfigRequestWrapper& src) :
        PFCatalogGetCatalogConfigRequestWrapper{ src.Model() }
    {
    }

    PFCatalogGetCatalogConfigRequestWrapper(PFCatalogGetCatalogConfigRequestWrapper&& src) :
        PFCatalogGetCatalogConfigRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetCatalogConfigRequestWrapper& operator=(PFCatalogGetCatalogConfigRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetCatalogConfigRequestWrapper() = default;

    friend void swap(PFCatalogGetCatalogConfigRequestWrapper& lhs, PFCatalogGetCatalogConfigRequestWrapper& rhs)
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
class PFCatalogCatalogSpecificConfigWrapper : public ModelWrapper<PFCatalogCatalogSpecificConfig, Alloc>
{
public:
    using ModelType = PFCatalogCatalogSpecificConfig;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogCatalogSpecificConfigWrapper() = default;

    PFCatalogCatalogSpecificConfigWrapper(const PFCatalogCatalogSpecificConfig& model) :
        ModelWrapper<PFCatalogCatalogSpecificConfig, Alloc>{ model },
        m_contentTypes{ model.contentTypes, model.contentTypes + model.contentTypesCount },
        m_tags{ model.tags, model.tags + model.tagsCount }
    {
        SetModelPointers();
    }

    PFCatalogCatalogSpecificConfigWrapper(const PFCatalogCatalogSpecificConfigWrapper& src) :
        PFCatalogCatalogSpecificConfigWrapper{ src.Model() }
    {
    }

    PFCatalogCatalogSpecificConfigWrapper(PFCatalogCatalogSpecificConfigWrapper&& src) :
        PFCatalogCatalogSpecificConfigWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogCatalogSpecificConfigWrapper& operator=(PFCatalogCatalogSpecificConfigWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogCatalogSpecificConfigWrapper() = default;

    friend void swap(PFCatalogCatalogSpecificConfigWrapper& lhs, PFCatalogCatalogSpecificConfigWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_contentTypes, rhs.m_contentTypes);
        swap(lhs.m_tags, rhs.m_tags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetContentTypes(CStringVector<Alloc> value)
    {
        m_contentTypes = std::move(value);
        this->m_model.contentTypes =  m_contentTypes.empty() ? nullptr : m_contentTypes.data();
        this->m_model.contentTypesCount =  static_cast<uint32_t>(m_contentTypes.size());
    }

    void SetTags(CStringVector<Alloc> value)
    {
        m_tags = std::move(value);
        this->m_model.tags =  m_tags.empty() ? nullptr : m_tags.data();
        this->m_model.tagsCount =  static_cast<uint32_t>(m_tags.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.contentTypes = m_contentTypes.empty() ? nullptr : m_contentTypes.data();
        this->m_model.tags = m_tags.empty() ? nullptr : m_tags.data();
    }

    CStringVector<Alloc> m_contentTypes;
    CStringVector<Alloc> m_tags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogDeepLinkFormatWrapper : public ModelWrapper<PFCatalogDeepLinkFormat, Alloc>
{
public:
    using ModelType = PFCatalogDeepLinkFormat;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogDeepLinkFormatWrapper() = default;

    PFCatalogDeepLinkFormatWrapper(const PFCatalogDeepLinkFormat& model) :
        ModelWrapper<PFCatalogDeepLinkFormat, Alloc>{ model },
        m_format{ SafeString(model.format) },
        m_platform{ SafeString(model.platform) }
    {
        SetModelPointers();
    }

    PFCatalogDeepLinkFormatWrapper(const PFCatalogDeepLinkFormatWrapper& src) :
        PFCatalogDeepLinkFormatWrapper{ src.Model() }
    {
    }

    PFCatalogDeepLinkFormatWrapper(PFCatalogDeepLinkFormatWrapper&& src) :
        PFCatalogDeepLinkFormatWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogDeepLinkFormatWrapper& operator=(PFCatalogDeepLinkFormatWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogDeepLinkFormatWrapper() = default;

    friend void swap(PFCatalogDeepLinkFormatWrapper& lhs, PFCatalogDeepLinkFormatWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_format, rhs.m_format);
        swap(lhs.m_platform, rhs.m_platform);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFormat(String value)
    {
        m_format = std::move(value);
        this->m_model.format =  m_format.empty() ? nullptr : m_format.data();
    }

    void SetPlatform(String value)
    {
        m_platform = std::move(value);
        this->m_model.platform =  m_platform.empty() ? nullptr : m_platform.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.format = m_format.empty() ? nullptr : m_format.data();
        this->m_model.platform = m_platform.empty() ? nullptr : m_platform.data();
    }

    String m_format;
    String m_platform;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogDisplayPropertyIndexInfoWrapper : public ModelWrapper<PFCatalogDisplayPropertyIndexInfo, Alloc>
{
public:
    using ModelType = PFCatalogDisplayPropertyIndexInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogDisplayPropertyIndexInfoWrapper() = default;

    PFCatalogDisplayPropertyIndexInfoWrapper(const PFCatalogDisplayPropertyIndexInfo& model) :
        ModelWrapper<PFCatalogDisplayPropertyIndexInfo, Alloc>{ model },
        m_name{ SafeString(model.name) },
        m_type{ model.type ? std::optional<PFCatalogDisplayPropertyType>{ *model.type } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogDisplayPropertyIndexInfoWrapper(const PFCatalogDisplayPropertyIndexInfoWrapper& src) :
        PFCatalogDisplayPropertyIndexInfoWrapper{ src.Model() }
    {
    }

    PFCatalogDisplayPropertyIndexInfoWrapper(PFCatalogDisplayPropertyIndexInfoWrapper&& src) :
        PFCatalogDisplayPropertyIndexInfoWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogDisplayPropertyIndexInfoWrapper& operator=(PFCatalogDisplayPropertyIndexInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogDisplayPropertyIndexInfoWrapper() = default;

    friend void swap(PFCatalogDisplayPropertyIndexInfoWrapper& lhs, PFCatalogDisplayPropertyIndexInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_name, rhs.m_name);
        swap(lhs.m_type, rhs.m_type);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

    void SetType(std::optional<PFCatalogDisplayPropertyType> value)
    {
        m_type = std::move(value);
        this->m_model.type = m_type ? m_type.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
        this->m_model.type = m_type ? m_type.operator->() : nullptr;
    }

    String m_name;
    std::optional<PFCatalogDisplayPropertyType> m_type;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogFileConfigWrapper : public ModelWrapper<PFCatalogFileConfig, Alloc>
{
public:
    using ModelType = PFCatalogFileConfig;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogFileConfigWrapper() = default;

    PFCatalogFileConfigWrapper(const PFCatalogFileConfig& model) :
        ModelWrapper<PFCatalogFileConfig, Alloc>{ model },
        m_contentTypes{ model.contentTypes, model.contentTypes + model.contentTypesCount },
        m_tags{ model.tags, model.tags + model.tagsCount }
    {
        SetModelPointers();
    }

    PFCatalogFileConfigWrapper(const PFCatalogFileConfigWrapper& src) :
        PFCatalogFileConfigWrapper{ src.Model() }
    {
    }

    PFCatalogFileConfigWrapper(PFCatalogFileConfigWrapper&& src) :
        PFCatalogFileConfigWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogFileConfigWrapper& operator=(PFCatalogFileConfigWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogFileConfigWrapper() = default;

    friend void swap(PFCatalogFileConfigWrapper& lhs, PFCatalogFileConfigWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_contentTypes, rhs.m_contentTypes);
        swap(lhs.m_tags, rhs.m_tags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetContentTypes(CStringVector<Alloc> value)
    {
        m_contentTypes = std::move(value);
        this->m_model.contentTypes =  m_contentTypes.empty() ? nullptr : m_contentTypes.data();
        this->m_model.contentTypesCount =  static_cast<uint32_t>(m_contentTypes.size());
    }

    void SetTags(CStringVector<Alloc> value)
    {
        m_tags = std::move(value);
        this->m_model.tags =  m_tags.empty() ? nullptr : m_tags.data();
        this->m_model.tagsCount =  static_cast<uint32_t>(m_tags.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.contentTypes = m_contentTypes.empty() ? nullptr : m_contentTypes.data();
        this->m_model.tags = m_tags.empty() ? nullptr : m_tags.data();
    }

    CStringVector<Alloc> m_contentTypes;
    CStringVector<Alloc> m_tags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogImageConfigWrapper : public ModelWrapper<PFCatalogImageConfig, Alloc>
{
public:
    using ModelType = PFCatalogImageConfig;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogImageConfigWrapper() = default;

    PFCatalogImageConfigWrapper(const PFCatalogImageConfig& model) :
        ModelWrapper<PFCatalogImageConfig, Alloc>{ model },
        m_tags{ model.tags, model.tags + model.tagsCount }
    {
        SetModelPointers();
    }

    PFCatalogImageConfigWrapper(const PFCatalogImageConfigWrapper& src) :
        PFCatalogImageConfigWrapper{ src.Model() }
    {
    }

    PFCatalogImageConfigWrapper(PFCatalogImageConfigWrapper&& src) :
        PFCatalogImageConfigWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogImageConfigWrapper& operator=(PFCatalogImageConfigWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogImageConfigWrapper() = default;

    friend void swap(PFCatalogImageConfigWrapper& lhs, PFCatalogImageConfigWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_tags, rhs.m_tags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetTags(CStringVector<Alloc> value)
    {
        m_tags = std::move(value);
        this->m_model.tags =  m_tags.empty() ? nullptr : m_tags.data();
        this->m_model.tagsCount =  static_cast<uint32_t>(m_tags.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.tags = m_tags.empty() ? nullptr : m_tags.data();
    }

    CStringVector<Alloc> m_tags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogCategoryRatingConfigWrapper : public ModelWrapper<PFCatalogCategoryRatingConfig, Alloc>
{
public:
    using ModelType = PFCatalogCategoryRatingConfig;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogCategoryRatingConfigWrapper() = default;

    PFCatalogCategoryRatingConfigWrapper(const PFCatalogCategoryRatingConfig& model) :
        ModelWrapper<PFCatalogCategoryRatingConfig, Alloc>{ model },
        m_name{ SafeString(model.name) }
    {
        SetModelPointers();
    }

    PFCatalogCategoryRatingConfigWrapper(const PFCatalogCategoryRatingConfigWrapper& src) :
        PFCatalogCategoryRatingConfigWrapper{ src.Model() }
    {
    }

    PFCatalogCategoryRatingConfigWrapper(PFCatalogCategoryRatingConfigWrapper&& src) :
        PFCatalogCategoryRatingConfigWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogCategoryRatingConfigWrapper& operator=(PFCatalogCategoryRatingConfigWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogCategoryRatingConfigWrapper() = default;

    friend void swap(PFCatalogCategoryRatingConfigWrapper& lhs, PFCatalogCategoryRatingConfigWrapper& rhs)
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

private:
    void SetModelPointers()
    {
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
    }

    String m_name;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogReviewConfigWrapper : public ModelWrapper<PFCatalogReviewConfig, Alloc>
{
public:
    using ModelType = PFCatalogReviewConfig;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogReviewConfigWrapper() = default;

    PFCatalogReviewConfigWrapper(const PFCatalogReviewConfig& model) :
        ModelWrapper<PFCatalogReviewConfig, Alloc>{ model },
        m_categoryRatings{ model.categoryRatings, model.categoryRatings + model.categoryRatingsCount }
    {
        SetModelPointers();
    }

    PFCatalogReviewConfigWrapper(const PFCatalogReviewConfigWrapper& src) :
        PFCatalogReviewConfigWrapper{ src.Model() }
    {
    }

    PFCatalogReviewConfigWrapper(PFCatalogReviewConfigWrapper&& src) :
        PFCatalogReviewConfigWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogReviewConfigWrapper& operator=(PFCatalogReviewConfigWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogReviewConfigWrapper() = default;

    friend void swap(PFCatalogReviewConfigWrapper& lhs, PFCatalogReviewConfigWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_categoryRatings, rhs.m_categoryRatings);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCategoryRatings(ModelVector<PFCatalogCategoryRatingConfigWrapper<Alloc>, Alloc> value)
    {
        m_categoryRatings = std::move(value);
        this->m_model.categoryRatings =  m_categoryRatings.empty() ? nullptr : m_categoryRatings.data();
        this->m_model.categoryRatingsCount =  static_cast<uint32_t>(m_categoryRatings.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.categoryRatings = m_categoryRatings.empty() ? nullptr : m_categoryRatings.data();
    }

    ModelVector<PFCatalogCategoryRatingConfigWrapper<Alloc>, Alloc> m_categoryRatings;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogUserGeneratedContentSpecificConfigWrapper : public ModelWrapper<PFCatalogUserGeneratedContentSpecificConfig, Alloc>
{
public:
    using ModelType = PFCatalogUserGeneratedContentSpecificConfig;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogUserGeneratedContentSpecificConfigWrapper() = default;

    PFCatalogUserGeneratedContentSpecificConfigWrapper(const PFCatalogUserGeneratedContentSpecificConfig& model) :
        ModelWrapper<PFCatalogUserGeneratedContentSpecificConfig, Alloc>{ model },
        m_contentTypes{ model.contentTypes, model.contentTypes + model.contentTypesCount },
        m_tags{ model.tags, model.tags + model.tagsCount }
    {
        SetModelPointers();
    }

    PFCatalogUserGeneratedContentSpecificConfigWrapper(const PFCatalogUserGeneratedContentSpecificConfigWrapper& src) :
        PFCatalogUserGeneratedContentSpecificConfigWrapper{ src.Model() }
    {
    }

    PFCatalogUserGeneratedContentSpecificConfigWrapper(PFCatalogUserGeneratedContentSpecificConfigWrapper&& src) :
        PFCatalogUserGeneratedContentSpecificConfigWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogUserGeneratedContentSpecificConfigWrapper& operator=(PFCatalogUserGeneratedContentSpecificConfigWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogUserGeneratedContentSpecificConfigWrapper() = default;

    friend void swap(PFCatalogUserGeneratedContentSpecificConfigWrapper& lhs, PFCatalogUserGeneratedContentSpecificConfigWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_contentTypes, rhs.m_contentTypes);
        swap(lhs.m_tags, rhs.m_tags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetContentTypes(CStringVector<Alloc> value)
    {
        m_contentTypes = std::move(value);
        this->m_model.contentTypes =  m_contentTypes.empty() ? nullptr : m_contentTypes.data();
        this->m_model.contentTypesCount =  static_cast<uint32_t>(m_contentTypes.size());
    }

    void SetTags(CStringVector<Alloc> value)
    {
        m_tags = std::move(value);
        this->m_model.tags =  m_tags.empty() ? nullptr : m_tags.data();
        this->m_model.tagsCount =  static_cast<uint32_t>(m_tags.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.contentTypes = m_contentTypes.empty() ? nullptr : m_contentTypes.data();
        this->m_model.tags = m_tags.empty() ? nullptr : m_tags.data();
    }

    CStringVector<Alloc> m_contentTypes;
    CStringVector<Alloc> m_tags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogCatalogConfigWrapper : public ModelWrapper<PFCatalogCatalogConfig, Alloc>
{
public:
    using ModelType = PFCatalogCatalogConfig;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogCatalogConfigWrapper() = default;

    PFCatalogCatalogConfigWrapper(const PFCatalogCatalogConfig& model) :
        ModelWrapper<PFCatalogCatalogConfig, Alloc>{ model },
        m_adminEntities{ model.adminEntities, model.adminEntities + model.adminEntitiesCount },
        m_catalog{ model.catalog ? std::optional<PFCatalogCatalogSpecificConfigWrapper<Alloc>>{ *model.catalog } : std::nullopt },
        m_deepLinkFormats{ model.deepLinkFormats, model.deepLinkFormats + model.deepLinkFormatsCount },
        m_displayPropertyIndexInfos{ model.displayPropertyIndexInfos, model.displayPropertyIndexInfos + model.displayPropertyIndexInfosCount },
        m_file{ model.file ? std::optional<PFCatalogFileConfigWrapper<Alloc>>{ *model.file } : std::nullopt },
        m_image{ model.image ? std::optional<PFCatalogImageConfigWrapper<Alloc>>{ *model.image } : std::nullopt },
        m_platforms{ model.platforms, model.platforms + model.platformsCount },
        m_review{ model.review ? std::optional<PFCatalogReviewConfigWrapper<Alloc>>{ *model.review } : std::nullopt },
        m_reviewerEntities{ model.reviewerEntities, model.reviewerEntities + model.reviewerEntitiesCount },
        m_userGeneratedContent{ model.userGeneratedContent ? std::optional<PFCatalogUserGeneratedContentSpecificConfigWrapper<Alloc>>{ *model.userGeneratedContent } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogCatalogConfigWrapper(const PFCatalogCatalogConfigWrapper& src) :
        PFCatalogCatalogConfigWrapper{ src.Model() }
    {
    }

    PFCatalogCatalogConfigWrapper(PFCatalogCatalogConfigWrapper&& src) :
        PFCatalogCatalogConfigWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogCatalogConfigWrapper& operator=(PFCatalogCatalogConfigWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogCatalogConfigWrapper() = default;

    friend void swap(PFCatalogCatalogConfigWrapper& lhs, PFCatalogCatalogConfigWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_adminEntities, rhs.m_adminEntities);
        swap(lhs.m_catalog, rhs.m_catalog);
        swap(lhs.m_deepLinkFormats, rhs.m_deepLinkFormats);
        swap(lhs.m_displayPropertyIndexInfos, rhs.m_displayPropertyIndexInfos);
        swap(lhs.m_file, rhs.m_file);
        swap(lhs.m_image, rhs.m_image);
        swap(lhs.m_platforms, rhs.m_platforms);
        swap(lhs.m_review, rhs.m_review);
        swap(lhs.m_reviewerEntities, rhs.m_reviewerEntities);
        swap(lhs.m_userGeneratedContent, rhs.m_userGeneratedContent);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAdminEntities(ModelVector<PFEntityKeyWrapper<Alloc>, Alloc> value)
    {
        m_adminEntities = std::move(value);
        this->m_model.adminEntities =  m_adminEntities.empty() ? nullptr : m_adminEntities.data();
        this->m_model.adminEntitiesCount =  static_cast<uint32_t>(m_adminEntities.size());
    }

    void SetCatalog(std::optional<PFCatalogCatalogSpecificConfigWrapper<Alloc>> value)
    {
        m_catalog = std::move(value);
        this->m_model.catalog = m_catalog ? &m_catalog->Model() : nullptr;
    }

    void SetDeepLinkFormats(ModelVector<PFCatalogDeepLinkFormatWrapper<Alloc>, Alloc> value)
    {
        m_deepLinkFormats = std::move(value);
        this->m_model.deepLinkFormats =  m_deepLinkFormats.empty() ? nullptr : m_deepLinkFormats.data();
        this->m_model.deepLinkFormatsCount =  static_cast<uint32_t>(m_deepLinkFormats.size());
    }

    void SetDisplayPropertyIndexInfos(ModelVector<PFCatalogDisplayPropertyIndexInfoWrapper<Alloc>, Alloc> value)
    {
        m_displayPropertyIndexInfos = std::move(value);
        this->m_model.displayPropertyIndexInfos =  m_displayPropertyIndexInfos.empty() ? nullptr : m_displayPropertyIndexInfos.data();
        this->m_model.displayPropertyIndexInfosCount =  static_cast<uint32_t>(m_displayPropertyIndexInfos.size());
    }

    void SetFile(std::optional<PFCatalogFileConfigWrapper<Alloc>> value)
    {
        m_file = std::move(value);
        this->m_model.file = m_file ? &m_file->Model() : nullptr;
    }

    void SetImage(std::optional<PFCatalogImageConfigWrapper<Alloc>> value)
    {
        m_image = std::move(value);
        this->m_model.image = m_image ? &m_image->Model() : nullptr;
    }

    void SetIsCatalogEnabled(bool value)
    {
        this->m_model.isCatalogEnabled = value;
    }

    void SetPlatforms(CStringVector<Alloc> value)
    {
        m_platforms = std::move(value);
        this->m_model.platforms =  m_platforms.empty() ? nullptr : m_platforms.data();
        this->m_model.platformsCount =  static_cast<uint32_t>(m_platforms.size());
    }

    void SetReview(std::optional<PFCatalogReviewConfigWrapper<Alloc>> value)
    {
        m_review = std::move(value);
        this->m_model.review = m_review ? &m_review->Model() : nullptr;
    }

    void SetReviewerEntities(ModelVector<PFEntityKeyWrapper<Alloc>, Alloc> value)
    {
        m_reviewerEntities = std::move(value);
        this->m_model.reviewerEntities =  m_reviewerEntities.empty() ? nullptr : m_reviewerEntities.data();
        this->m_model.reviewerEntitiesCount =  static_cast<uint32_t>(m_reviewerEntities.size());
    }

    void SetUserGeneratedContent(std::optional<PFCatalogUserGeneratedContentSpecificConfigWrapper<Alloc>> value)
    {
        m_userGeneratedContent = std::move(value);
        this->m_model.userGeneratedContent = m_userGeneratedContent ? &m_userGeneratedContent->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.adminEntities = m_adminEntities.empty() ? nullptr : m_adminEntities.data();
        this->m_model.catalog = m_catalog ?  &m_catalog->Model() : nullptr;
        this->m_model.deepLinkFormats = m_deepLinkFormats.empty() ? nullptr : m_deepLinkFormats.data();
        this->m_model.displayPropertyIndexInfos = m_displayPropertyIndexInfos.empty() ? nullptr : m_displayPropertyIndexInfos.data();
        this->m_model.file = m_file ?  &m_file->Model() : nullptr;
        this->m_model.image = m_image ?  &m_image->Model() : nullptr;
        this->m_model.platforms = m_platforms.empty() ? nullptr : m_platforms.data();
        this->m_model.review = m_review ?  &m_review->Model() : nullptr;
        this->m_model.reviewerEntities = m_reviewerEntities.empty() ? nullptr : m_reviewerEntities.data();
        this->m_model.userGeneratedContent = m_userGeneratedContent ?  &m_userGeneratedContent->Model() : nullptr;
    }

    ModelVector<PFEntityKeyWrapper<Alloc>, Alloc> m_adminEntities;
    std::optional<PFCatalogCatalogSpecificConfigWrapper<Alloc>> m_catalog;
    ModelVector<PFCatalogDeepLinkFormatWrapper<Alloc>, Alloc> m_deepLinkFormats;
    ModelVector<PFCatalogDisplayPropertyIndexInfoWrapper<Alloc>, Alloc> m_displayPropertyIndexInfos;
    std::optional<PFCatalogFileConfigWrapper<Alloc>> m_file;
    std::optional<PFCatalogImageConfigWrapper<Alloc>> m_image;
    CStringVector<Alloc> m_platforms;
    std::optional<PFCatalogReviewConfigWrapper<Alloc>> m_review;
    ModelVector<PFEntityKeyWrapper<Alloc>, Alloc> m_reviewerEntities;
    std::optional<PFCatalogUserGeneratedContentSpecificConfigWrapper<Alloc>> m_userGeneratedContent;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetCatalogConfigResponseWrapper : public ModelWrapper<PFCatalogGetCatalogConfigResponse, Alloc>
{
public:
    using ModelType = PFCatalogGetCatalogConfigResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetCatalogConfigResponseWrapper() = default;

    PFCatalogGetCatalogConfigResponseWrapper(const PFCatalogGetCatalogConfigResponse& model) :
        ModelWrapper<PFCatalogGetCatalogConfigResponse, Alloc>{ model },
        m_config{ model.config ? std::optional<PFCatalogCatalogConfigWrapper<Alloc>>{ *model.config } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogGetCatalogConfigResponseWrapper(const PFCatalogGetCatalogConfigResponseWrapper& src) :
        PFCatalogGetCatalogConfigResponseWrapper{ src.Model() }
    {
    }

    PFCatalogGetCatalogConfigResponseWrapper(PFCatalogGetCatalogConfigResponseWrapper&& src) :
        PFCatalogGetCatalogConfigResponseWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetCatalogConfigResponseWrapper& operator=(PFCatalogGetCatalogConfigResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetCatalogConfigResponseWrapper() = default;

    friend void swap(PFCatalogGetCatalogConfigResponseWrapper& lhs, PFCatalogGetCatalogConfigResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_config, rhs.m_config);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetConfig(std::optional<PFCatalogCatalogConfigWrapper<Alloc>> value)
    {
        m_config = std::move(value);
        this->m_model.config = m_config ? &m_config->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.config = m_config ?  &m_config->Model() : nullptr;
    }

    std::optional<PFCatalogCatalogConfigWrapper<Alloc>> m_config;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetDraftItemRequestWrapper : public ModelWrapper<PFCatalogGetDraftItemRequest, Alloc>
{
public:
    using ModelType = PFCatalogGetDraftItemRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetDraftItemRequestWrapper() = default;

    PFCatalogGetDraftItemRequestWrapper(const PFCatalogGetDraftItemRequest& model) :
        ModelWrapper<PFCatalogGetDraftItemRequest, Alloc>{ model },
        m_alternateId{ model.alternateId ? std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>>{ *model.alternateId } : std::nullopt },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_id{ SafeString(model.id) }
    {
        SetModelPointers();
    }

    PFCatalogGetDraftItemRequestWrapper(const PFCatalogGetDraftItemRequestWrapper& src) :
        PFCatalogGetDraftItemRequestWrapper{ src.Model() }
    {
    }

    PFCatalogGetDraftItemRequestWrapper(PFCatalogGetDraftItemRequestWrapper&& src) :
        PFCatalogGetDraftItemRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetDraftItemRequestWrapper& operator=(PFCatalogGetDraftItemRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetDraftItemRequestWrapper() = default;

    friend void swap(PFCatalogGetDraftItemRequestWrapper& lhs, PFCatalogGetDraftItemRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_alternateId, rhs.m_alternateId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_id, rhs.m_id);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAlternateId(std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> value)
    {
        m_alternateId = std::move(value);
        this->m_model.alternateId = m_alternateId ? &m_alternateId->Model() : nullptr;
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

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.alternateId = m_alternateId ?  &m_alternateId->Model() : nullptr;
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
    }

    std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> m_alternateId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_id;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetDraftItemResponseWrapper : public ModelWrapper<PFCatalogGetDraftItemResponse, Alloc>
{
public:
    using ModelType = PFCatalogGetDraftItemResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetDraftItemResponseWrapper() = default;

    PFCatalogGetDraftItemResponseWrapper(const PFCatalogGetDraftItemResponse& model) :
        ModelWrapper<PFCatalogGetDraftItemResponse, Alloc>{ model },
        m_item{ model.item ? std::optional<PFCatalogCatalogItemWrapper<Alloc>>{ *model.item } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogGetDraftItemResponseWrapper(const PFCatalogGetDraftItemResponseWrapper& src) :
        PFCatalogGetDraftItemResponseWrapper{ src.Model() }
    {
    }

    PFCatalogGetDraftItemResponseWrapper(PFCatalogGetDraftItemResponseWrapper&& src) :
        PFCatalogGetDraftItemResponseWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetDraftItemResponseWrapper& operator=(PFCatalogGetDraftItemResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetDraftItemResponseWrapper() = default;

    friend void swap(PFCatalogGetDraftItemResponseWrapper& lhs, PFCatalogGetDraftItemResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_item, rhs.m_item);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetItem(std::optional<PFCatalogCatalogItemWrapper<Alloc>> value)
    {
        m_item = std::move(value);
        this->m_model.item = m_item ? &m_item->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.item = m_item ?  &m_item->Model() : nullptr;
    }

    std::optional<PFCatalogCatalogItemWrapper<Alloc>> m_item;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetDraftItemsRequestWrapper : public ModelWrapper<PFCatalogGetDraftItemsRequest, Alloc>
{
public:
    using ModelType = PFCatalogGetDraftItemsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetDraftItemsRequestWrapper() = default;

    PFCatalogGetDraftItemsRequestWrapper(const PFCatalogGetDraftItemsRequest& model) :
        ModelWrapper<PFCatalogGetDraftItemsRequest, Alloc>{ model },
        m_alternateIds{ model.alternateIds, model.alternateIds + model.alternateIdsCount },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_ids{ model.ids, model.ids + model.idsCount }
    {
        SetModelPointers();
    }

    PFCatalogGetDraftItemsRequestWrapper(const PFCatalogGetDraftItemsRequestWrapper& src) :
        PFCatalogGetDraftItemsRequestWrapper{ src.Model() }
    {
    }

    PFCatalogGetDraftItemsRequestWrapper(PFCatalogGetDraftItemsRequestWrapper&& src) :
        PFCatalogGetDraftItemsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetDraftItemsRequestWrapper& operator=(PFCatalogGetDraftItemsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetDraftItemsRequestWrapper() = default;

    friend void swap(PFCatalogGetDraftItemsRequestWrapper& lhs, PFCatalogGetDraftItemsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_alternateIds, rhs.m_alternateIds);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_ids, rhs.m_ids);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAlternateIds(ModelVector<PFCatalogCatalogAlternateIdWrapper<Alloc>, Alloc> value)
    {
        m_alternateIds = std::move(value);
        this->m_model.alternateIds =  m_alternateIds.empty() ? nullptr : m_alternateIds.data();
        this->m_model.alternateIdsCount =  static_cast<uint32_t>(m_alternateIds.size());
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

    void SetIds(CStringVector<Alloc> value)
    {
        m_ids = std::move(value);
        this->m_model.ids =  m_ids.empty() ? nullptr : m_ids.data();
        this->m_model.idsCount =  static_cast<uint32_t>(m_ids.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.alternateIds = m_alternateIds.empty() ? nullptr : m_alternateIds.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.ids = m_ids.empty() ? nullptr : m_ids.data();
    }

    ModelVector<PFCatalogCatalogAlternateIdWrapper<Alloc>, Alloc> m_alternateIds;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    CStringVector<Alloc> m_ids;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetDraftItemsResponseWrapper : public ModelWrapper<PFCatalogGetDraftItemsResponse, Alloc>
{
public:
    using ModelType = PFCatalogGetDraftItemsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetDraftItemsResponseWrapper() = default;

    PFCatalogGetDraftItemsResponseWrapper(const PFCatalogGetDraftItemsResponse& model) :
        ModelWrapper<PFCatalogGetDraftItemsResponse, Alloc>{ model },
        m_continuationToken{ SafeString(model.continuationToken) },
        m_items{ model.items, model.items + model.itemsCount }
    {
        SetModelPointers();
    }

    PFCatalogGetDraftItemsResponseWrapper(const PFCatalogGetDraftItemsResponseWrapper& src) :
        PFCatalogGetDraftItemsResponseWrapper{ src.Model() }
    {
    }

    PFCatalogGetDraftItemsResponseWrapper(PFCatalogGetDraftItemsResponseWrapper&& src) :
        PFCatalogGetDraftItemsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetDraftItemsResponseWrapper& operator=(PFCatalogGetDraftItemsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetDraftItemsResponseWrapper() = default;

    friend void swap(PFCatalogGetDraftItemsResponseWrapper& lhs, PFCatalogGetDraftItemsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_continuationToken, rhs.m_continuationToken);
        swap(lhs.m_items, rhs.m_items);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetContinuationToken(String value)
    {
        m_continuationToken = std::move(value);
        this->m_model.continuationToken =  m_continuationToken.empty() ? nullptr : m_continuationToken.data();
    }

    void SetItems(ModelVector<PFCatalogCatalogItemWrapper<Alloc>, Alloc> value)
    {
        m_items = std::move(value);
        this->m_model.items =  m_items.empty() ? nullptr : m_items.data();
        this->m_model.itemsCount =  static_cast<uint32_t>(m_items.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.continuationToken = m_continuationToken.empty() ? nullptr : m_continuationToken.data();
        this->m_model.items = m_items.empty() ? nullptr : m_items.data();
    }

    String m_continuationToken;
    ModelVector<PFCatalogCatalogItemWrapper<Alloc>, Alloc> m_items;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetEntityDraftItemsRequestWrapper : public ModelWrapper<PFCatalogGetEntityDraftItemsRequest, Alloc>
{
public:
    using ModelType = PFCatalogGetEntityDraftItemsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetEntityDraftItemsRequestWrapper() = default;

    PFCatalogGetEntityDraftItemsRequestWrapper(const PFCatalogGetEntityDraftItemsRequest& model) :
        ModelWrapper<PFCatalogGetEntityDraftItemsRequest, Alloc>{ model },
        m_continuationToken{ SafeString(model.continuationToken) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_filter{ SafeString(model.filter) }
    {
        SetModelPointers();
    }

    PFCatalogGetEntityDraftItemsRequestWrapper(const PFCatalogGetEntityDraftItemsRequestWrapper& src) :
        PFCatalogGetEntityDraftItemsRequestWrapper{ src.Model() }
    {
    }

    PFCatalogGetEntityDraftItemsRequestWrapper(PFCatalogGetEntityDraftItemsRequestWrapper&& src) :
        PFCatalogGetEntityDraftItemsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetEntityDraftItemsRequestWrapper& operator=(PFCatalogGetEntityDraftItemsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetEntityDraftItemsRequestWrapper() = default;

    friend void swap(PFCatalogGetEntityDraftItemsRequestWrapper& lhs, PFCatalogGetEntityDraftItemsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_continuationToken, rhs.m_continuationToken);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_filter, rhs.m_filter);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetContinuationToken(String value)
    {
        m_continuationToken = std::move(value);
        this->m_model.continuationToken =  m_continuationToken.empty() ? nullptr : m_continuationToken.data();
    }

    void SetCount(int32_t value)
    {
        this->m_model.count = value;
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

    void SetFilter(String value)
    {
        m_filter = std::move(value);
        this->m_model.filter =  m_filter.empty() ? nullptr : m_filter.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.continuationToken = m_continuationToken.empty() ? nullptr : m_continuationToken.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.filter = m_filter.empty() ? nullptr : m_filter.data();
    }

    String m_continuationToken;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_filter;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetEntityDraftItemsResponseWrapper : public ModelWrapper<PFCatalogGetEntityDraftItemsResponse, Alloc>
{
public:
    using ModelType = PFCatalogGetEntityDraftItemsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetEntityDraftItemsResponseWrapper() = default;

    PFCatalogGetEntityDraftItemsResponseWrapper(const PFCatalogGetEntityDraftItemsResponse& model) :
        ModelWrapper<PFCatalogGetEntityDraftItemsResponse, Alloc>{ model },
        m_continuationToken{ SafeString(model.continuationToken) },
        m_items{ model.items, model.items + model.itemsCount }
    {
        SetModelPointers();
    }

    PFCatalogGetEntityDraftItemsResponseWrapper(const PFCatalogGetEntityDraftItemsResponseWrapper& src) :
        PFCatalogGetEntityDraftItemsResponseWrapper{ src.Model() }
    {
    }

    PFCatalogGetEntityDraftItemsResponseWrapper(PFCatalogGetEntityDraftItemsResponseWrapper&& src) :
        PFCatalogGetEntityDraftItemsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetEntityDraftItemsResponseWrapper& operator=(PFCatalogGetEntityDraftItemsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetEntityDraftItemsResponseWrapper() = default;

    friend void swap(PFCatalogGetEntityDraftItemsResponseWrapper& lhs, PFCatalogGetEntityDraftItemsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_continuationToken, rhs.m_continuationToken);
        swap(lhs.m_items, rhs.m_items);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetContinuationToken(String value)
    {
        m_continuationToken = std::move(value);
        this->m_model.continuationToken =  m_continuationToken.empty() ? nullptr : m_continuationToken.data();
    }

    void SetItems(ModelVector<PFCatalogCatalogItemWrapper<Alloc>, Alloc> value)
    {
        m_items = std::move(value);
        this->m_model.items =  m_items.empty() ? nullptr : m_items.data();
        this->m_model.itemsCount =  static_cast<uint32_t>(m_items.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.continuationToken = m_continuationToken.empty() ? nullptr : m_continuationToken.data();
        this->m_model.items = m_items.empty() ? nullptr : m_items.data();
    }

    String m_continuationToken;
    ModelVector<PFCatalogCatalogItemWrapper<Alloc>, Alloc> m_items;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetEntityItemReviewRequestWrapper : public ModelWrapper<PFCatalogGetEntityItemReviewRequest, Alloc>
{
public:
    using ModelType = PFCatalogGetEntityItemReviewRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetEntityItemReviewRequestWrapper() = default;

    PFCatalogGetEntityItemReviewRequestWrapper(const PFCatalogGetEntityItemReviewRequest& model) :
        ModelWrapper<PFCatalogGetEntityItemReviewRequest, Alloc>{ model },
        m_alternateId{ model.alternateId ? std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>>{ *model.alternateId } : std::nullopt },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_id{ SafeString(model.id) }
    {
        SetModelPointers();
    }

    PFCatalogGetEntityItemReviewRequestWrapper(const PFCatalogGetEntityItemReviewRequestWrapper& src) :
        PFCatalogGetEntityItemReviewRequestWrapper{ src.Model() }
    {
    }

    PFCatalogGetEntityItemReviewRequestWrapper(PFCatalogGetEntityItemReviewRequestWrapper&& src) :
        PFCatalogGetEntityItemReviewRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetEntityItemReviewRequestWrapper& operator=(PFCatalogGetEntityItemReviewRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetEntityItemReviewRequestWrapper() = default;

    friend void swap(PFCatalogGetEntityItemReviewRequestWrapper& lhs, PFCatalogGetEntityItemReviewRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_alternateId, rhs.m_alternateId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_id, rhs.m_id);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAlternateId(std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> value)
    {
        m_alternateId = std::move(value);
        this->m_model.alternateId = m_alternateId ? &m_alternateId->Model() : nullptr;
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

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.alternateId = m_alternateId ?  &m_alternateId->Model() : nullptr;
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
    }

    std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> m_alternateId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_id;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogReviewWrapper : public ModelWrapper<PFCatalogReview, Alloc>
{
public:
    using ModelType = PFCatalogReview;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogReviewWrapper() = default;

    PFCatalogReviewWrapper(const PFCatalogReview& model) :
        ModelWrapper<PFCatalogReview, Alloc>{ model },
        m_categoryRatings{ model.categoryRatings, model.categoryRatings + model.categoryRatingsCount },
        m_itemId{ SafeString(model.itemId) },
        m_itemVersion{ SafeString(model.itemVersion) },
        m_locale{ SafeString(model.locale) },
        m_reviewerEntity{ model.reviewerEntity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.reviewerEntity } : std::nullopt },
        m_reviewerId{ SafeString(model.reviewerId) },
        m_reviewId{ SafeString(model.reviewId) },
        m_reviewText{ SafeString(model.reviewText) },
        m_title{ SafeString(model.title) }
    {
        SetModelPointers();
    }

    PFCatalogReviewWrapper(const PFCatalogReviewWrapper& src) :
        PFCatalogReviewWrapper{ src.Model() }
    {
    }

    PFCatalogReviewWrapper(PFCatalogReviewWrapper&& src) :
        PFCatalogReviewWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogReviewWrapper& operator=(PFCatalogReviewWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogReviewWrapper() = default;

    friend void swap(PFCatalogReviewWrapper& lhs, PFCatalogReviewWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_categoryRatings, rhs.m_categoryRatings);
        swap(lhs.m_itemId, rhs.m_itemId);
        swap(lhs.m_itemVersion, rhs.m_itemVersion);
        swap(lhs.m_locale, rhs.m_locale);
        swap(lhs.m_reviewerEntity, rhs.m_reviewerEntity);
        swap(lhs.m_reviewerId, rhs.m_reviewerId);
        swap(lhs.m_reviewId, rhs.m_reviewId);
        swap(lhs.m_reviewText, rhs.m_reviewText);
        swap(lhs.m_title, rhs.m_title);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCategoryRatings(DictionaryEntryVector<PFInt32DictionaryEntry, Alloc> value)
    {
        m_categoryRatings = std::move(value);
        this->m_model.categoryRatings =  m_categoryRatings.empty() ? nullptr : m_categoryRatings.data();
        this->m_model.categoryRatingsCount =  static_cast<uint32_t>(m_categoryRatings.size());
    }

    void SetHelpfulNegative(int32_t value)
    {
        this->m_model.helpfulNegative = value;
    }

    void SetHelpfulPositive(int32_t value)
    {
        this->m_model.helpfulPositive = value;
    }

    void SetIsInstalled(bool value)
    {
        this->m_model.isInstalled = value;
    }

    void SetItemId(String value)
    {
        m_itemId = std::move(value);
        this->m_model.itemId =  m_itemId.empty() ? nullptr : m_itemId.data();
    }

    void SetItemVersion(String value)
    {
        m_itemVersion = std::move(value);
        this->m_model.itemVersion =  m_itemVersion.empty() ? nullptr : m_itemVersion.data();
    }

    void SetLocale(String value)
    {
        m_locale = std::move(value);
        this->m_model.locale =  m_locale.empty() ? nullptr : m_locale.data();
    }

    void SetRating(int32_t value)
    {
        this->m_model.rating = value;
    }

    void SetReviewerEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_reviewerEntity = std::move(value);
        this->m_model.reviewerEntity = m_reviewerEntity ? &m_reviewerEntity->Model() : nullptr;
    }

    void SetReviewerId(String value)
    {
        m_reviewerId = std::move(value);
        this->m_model.reviewerId =  m_reviewerId.empty() ? nullptr : m_reviewerId.data();
    }

    void SetReviewId(String value)
    {
        m_reviewId = std::move(value);
        this->m_model.reviewId =  m_reviewId.empty() ? nullptr : m_reviewId.data();
    }

    void SetReviewText(String value)
    {
        m_reviewText = std::move(value);
        this->m_model.reviewText =  m_reviewText.empty() ? nullptr : m_reviewText.data();
    }

    void SetSubmitted(time_t value)
    {
        this->m_model.submitted = value;
    }

    void SetTitle(String value)
    {
        m_title = std::move(value);
        this->m_model.title =  m_title.empty() ? nullptr : m_title.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.categoryRatings = m_categoryRatings.empty() ? nullptr : m_categoryRatings.data();
        this->m_model.itemId = m_itemId.empty() ? nullptr : m_itemId.data();
        this->m_model.itemVersion = m_itemVersion.empty() ? nullptr : m_itemVersion.data();
        this->m_model.locale = m_locale.empty() ? nullptr : m_locale.data();
        this->m_model.reviewerEntity = m_reviewerEntity ?  &m_reviewerEntity->Model() : nullptr;
        this->m_model.reviewerId = m_reviewerId.empty() ? nullptr : m_reviewerId.data();
        this->m_model.reviewId = m_reviewId.empty() ? nullptr : m_reviewId.data();
        this->m_model.reviewText = m_reviewText.empty() ? nullptr : m_reviewText.data();
        this->m_model.title = m_title.empty() ? nullptr : m_title.data();
    }

    DictionaryEntryVector<PFInt32DictionaryEntry, Alloc> m_categoryRatings;
    String m_itemId;
    String m_itemVersion;
    String m_locale;
    std::optional<PFEntityKeyWrapper<Alloc>> m_reviewerEntity;
    String m_reviewerId;
    String m_reviewId;
    String m_reviewText;
    String m_title;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetEntityItemReviewResponseWrapper : public ModelWrapper<PFCatalogGetEntityItemReviewResponse, Alloc>
{
public:
    using ModelType = PFCatalogGetEntityItemReviewResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetEntityItemReviewResponseWrapper() = default;

    PFCatalogGetEntityItemReviewResponseWrapper(const PFCatalogGetEntityItemReviewResponse& model) :
        ModelWrapper<PFCatalogGetEntityItemReviewResponse, Alloc>{ model },
        m_review{ model.review ? std::optional<PFCatalogReviewWrapper<Alloc>>{ *model.review } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogGetEntityItemReviewResponseWrapper(const PFCatalogGetEntityItemReviewResponseWrapper& src) :
        PFCatalogGetEntityItemReviewResponseWrapper{ src.Model() }
    {
    }

    PFCatalogGetEntityItemReviewResponseWrapper(PFCatalogGetEntityItemReviewResponseWrapper&& src) :
        PFCatalogGetEntityItemReviewResponseWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetEntityItemReviewResponseWrapper& operator=(PFCatalogGetEntityItemReviewResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetEntityItemReviewResponseWrapper() = default;

    friend void swap(PFCatalogGetEntityItemReviewResponseWrapper& lhs, PFCatalogGetEntityItemReviewResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_review, rhs.m_review);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetReview(std::optional<PFCatalogReviewWrapper<Alloc>> value)
    {
        m_review = std::move(value);
        this->m_model.review = m_review ? &m_review->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.review = m_review ?  &m_review->Model() : nullptr;
    }

    std::optional<PFCatalogReviewWrapper<Alloc>> m_review;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetItemRequestWrapper : public ModelWrapper<PFCatalogGetItemRequest, Alloc>
{
public:
    using ModelType = PFCatalogGetItemRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetItemRequestWrapper() = default;

    PFCatalogGetItemRequestWrapper(const PFCatalogGetItemRequest& model) :
        ModelWrapper<PFCatalogGetItemRequest, Alloc>{ model },
        m_alternateId{ model.alternateId ? std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>>{ *model.alternateId } : std::nullopt },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_id{ SafeString(model.id) }
    {
        SetModelPointers();
    }

    PFCatalogGetItemRequestWrapper(const PFCatalogGetItemRequestWrapper& src) :
        PFCatalogGetItemRequestWrapper{ src.Model() }
    {
    }

    PFCatalogGetItemRequestWrapper(PFCatalogGetItemRequestWrapper&& src) :
        PFCatalogGetItemRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetItemRequestWrapper& operator=(PFCatalogGetItemRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetItemRequestWrapper() = default;

    friend void swap(PFCatalogGetItemRequestWrapper& lhs, PFCatalogGetItemRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_alternateId, rhs.m_alternateId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_id, rhs.m_id);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAlternateId(std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> value)
    {
        m_alternateId = std::move(value);
        this->m_model.alternateId = m_alternateId ? &m_alternateId->Model() : nullptr;
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

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.alternateId = m_alternateId ?  &m_alternateId->Model() : nullptr;
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
    }

    std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> m_alternateId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_id;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetItemResponseWrapper : public ModelWrapper<PFCatalogGetItemResponse, Alloc>
{
public:
    using ModelType = PFCatalogGetItemResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetItemResponseWrapper() = default;

    PFCatalogGetItemResponseWrapper(const PFCatalogGetItemResponse& model) :
        ModelWrapper<PFCatalogGetItemResponse, Alloc>{ model },
        m_item{ model.item ? std::optional<PFCatalogCatalogItemWrapper<Alloc>>{ *model.item } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogGetItemResponseWrapper(const PFCatalogGetItemResponseWrapper& src) :
        PFCatalogGetItemResponseWrapper{ src.Model() }
    {
    }

    PFCatalogGetItemResponseWrapper(PFCatalogGetItemResponseWrapper&& src) :
        PFCatalogGetItemResponseWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetItemResponseWrapper& operator=(PFCatalogGetItemResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetItemResponseWrapper() = default;

    friend void swap(PFCatalogGetItemResponseWrapper& lhs, PFCatalogGetItemResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_item, rhs.m_item);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetItem(std::optional<PFCatalogCatalogItemWrapper<Alloc>> value)
    {
        m_item = std::move(value);
        this->m_model.item = m_item ? &m_item->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.item = m_item ?  &m_item->Model() : nullptr;
    }

    std::optional<PFCatalogCatalogItemWrapper<Alloc>> m_item;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetItemContainersRequestWrapper : public ModelWrapper<PFCatalogGetItemContainersRequest, Alloc>
{
public:
    using ModelType = PFCatalogGetItemContainersRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetItemContainersRequestWrapper() = default;

    PFCatalogGetItemContainersRequestWrapper(const PFCatalogGetItemContainersRequest& model) :
        ModelWrapper<PFCatalogGetItemContainersRequest, Alloc>{ model },
        m_alternateId{ model.alternateId ? std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>>{ *model.alternateId } : std::nullopt },
        m_continuationToken{ SafeString(model.continuationToken) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_id{ SafeString(model.id) }
    {
        SetModelPointers();
    }

    PFCatalogGetItemContainersRequestWrapper(const PFCatalogGetItemContainersRequestWrapper& src) :
        PFCatalogGetItemContainersRequestWrapper{ src.Model() }
    {
    }

    PFCatalogGetItemContainersRequestWrapper(PFCatalogGetItemContainersRequestWrapper&& src) :
        PFCatalogGetItemContainersRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetItemContainersRequestWrapper& operator=(PFCatalogGetItemContainersRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetItemContainersRequestWrapper() = default;

    friend void swap(PFCatalogGetItemContainersRequestWrapper& lhs, PFCatalogGetItemContainersRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_alternateId, rhs.m_alternateId);
        swap(lhs.m_continuationToken, rhs.m_continuationToken);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_id, rhs.m_id);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAlternateId(std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> value)
    {
        m_alternateId = std::move(value);
        this->m_model.alternateId = m_alternateId ? &m_alternateId->Model() : nullptr;
    }

    void SetContinuationToken(String value)
    {
        m_continuationToken = std::move(value);
        this->m_model.continuationToken =  m_continuationToken.empty() ? nullptr : m_continuationToken.data();
    }

    void SetCount(int32_t value)
    {
        this->m_model.count = value;
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

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.alternateId = m_alternateId ?  &m_alternateId->Model() : nullptr;
        this->m_model.continuationToken = m_continuationToken.empty() ? nullptr : m_continuationToken.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
    }

    std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> m_alternateId;
    String m_continuationToken;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_id;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetItemContainersResponseWrapper : public ModelWrapper<PFCatalogGetItemContainersResponse, Alloc>
{
public:
    using ModelType = PFCatalogGetItemContainersResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetItemContainersResponseWrapper() = default;

    PFCatalogGetItemContainersResponseWrapper(const PFCatalogGetItemContainersResponse& model) :
        ModelWrapper<PFCatalogGetItemContainersResponse, Alloc>{ model },
        m_containers{ model.containers, model.containers + model.containersCount },
        m_continuationToken{ SafeString(model.continuationToken) }
    {
        SetModelPointers();
    }

    PFCatalogGetItemContainersResponseWrapper(const PFCatalogGetItemContainersResponseWrapper& src) :
        PFCatalogGetItemContainersResponseWrapper{ src.Model() }
    {
    }

    PFCatalogGetItemContainersResponseWrapper(PFCatalogGetItemContainersResponseWrapper&& src) :
        PFCatalogGetItemContainersResponseWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetItemContainersResponseWrapper& operator=(PFCatalogGetItemContainersResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetItemContainersResponseWrapper() = default;

    friend void swap(PFCatalogGetItemContainersResponseWrapper& lhs, PFCatalogGetItemContainersResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_containers, rhs.m_containers);
        swap(lhs.m_continuationToken, rhs.m_continuationToken);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetContainers(ModelVector<PFCatalogCatalogItemWrapper<Alloc>, Alloc> value)
    {
        m_containers = std::move(value);
        this->m_model.containers =  m_containers.empty() ? nullptr : m_containers.data();
        this->m_model.containersCount =  static_cast<uint32_t>(m_containers.size());
    }

    void SetContinuationToken(String value)
    {
        m_continuationToken = std::move(value);
        this->m_model.continuationToken =  m_continuationToken.empty() ? nullptr : m_continuationToken.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.containers = m_containers.empty() ? nullptr : m_containers.data();
        this->m_model.continuationToken = m_continuationToken.empty() ? nullptr : m_continuationToken.data();
    }

    ModelVector<PFCatalogCatalogItemWrapper<Alloc>, Alloc> m_containers;
    String m_continuationToken;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetItemModerationStateRequestWrapper : public ModelWrapper<PFCatalogGetItemModerationStateRequest, Alloc>
{
public:
    using ModelType = PFCatalogGetItemModerationStateRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetItemModerationStateRequestWrapper() = default;

    PFCatalogGetItemModerationStateRequestWrapper(const PFCatalogGetItemModerationStateRequest& model) :
        ModelWrapper<PFCatalogGetItemModerationStateRequest, Alloc>{ model },
        m_alternateId{ model.alternateId ? std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>>{ *model.alternateId } : std::nullopt },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_id{ SafeString(model.id) }
    {
        SetModelPointers();
    }

    PFCatalogGetItemModerationStateRequestWrapper(const PFCatalogGetItemModerationStateRequestWrapper& src) :
        PFCatalogGetItemModerationStateRequestWrapper{ src.Model() }
    {
    }

    PFCatalogGetItemModerationStateRequestWrapper(PFCatalogGetItemModerationStateRequestWrapper&& src) :
        PFCatalogGetItemModerationStateRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetItemModerationStateRequestWrapper& operator=(PFCatalogGetItemModerationStateRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetItemModerationStateRequestWrapper() = default;

    friend void swap(PFCatalogGetItemModerationStateRequestWrapper& lhs, PFCatalogGetItemModerationStateRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_alternateId, rhs.m_alternateId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_id, rhs.m_id);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAlternateId(std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> value)
    {
        m_alternateId = std::move(value);
        this->m_model.alternateId = m_alternateId ? &m_alternateId->Model() : nullptr;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.alternateId = m_alternateId ?  &m_alternateId->Model() : nullptr;
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
    }

    std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> m_alternateId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_id;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetItemModerationStateResponseWrapper : public ModelWrapper<PFCatalogGetItemModerationStateResponse, Alloc>
{
public:
    using ModelType = PFCatalogGetItemModerationStateResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetItemModerationStateResponseWrapper() = default;

    PFCatalogGetItemModerationStateResponseWrapper(const PFCatalogGetItemModerationStateResponse& model) :
        ModelWrapper<PFCatalogGetItemModerationStateResponse, Alloc>{ model },
        m_state{ model.state ? std::optional<PFCatalogModerationStateWrapper<Alloc>>{ *model.state } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogGetItemModerationStateResponseWrapper(const PFCatalogGetItemModerationStateResponseWrapper& src) :
        PFCatalogGetItemModerationStateResponseWrapper{ src.Model() }
    {
    }

    PFCatalogGetItemModerationStateResponseWrapper(PFCatalogGetItemModerationStateResponseWrapper&& src) :
        PFCatalogGetItemModerationStateResponseWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetItemModerationStateResponseWrapper& operator=(PFCatalogGetItemModerationStateResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetItemModerationStateResponseWrapper() = default;

    friend void swap(PFCatalogGetItemModerationStateResponseWrapper& lhs, PFCatalogGetItemModerationStateResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_state, rhs.m_state);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetState(std::optional<PFCatalogModerationStateWrapper<Alloc>> value)
    {
        m_state = std::move(value);
        this->m_model.state = m_state ? &m_state->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.state = m_state ?  &m_state->Model() : nullptr;
    }

    std::optional<PFCatalogModerationStateWrapper<Alloc>> m_state;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetItemPublishStatusRequestWrapper : public ModelWrapper<PFCatalogGetItemPublishStatusRequest, Alloc>
{
public:
    using ModelType = PFCatalogGetItemPublishStatusRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetItemPublishStatusRequestWrapper() = default;

    PFCatalogGetItemPublishStatusRequestWrapper(const PFCatalogGetItemPublishStatusRequest& model) :
        ModelWrapper<PFCatalogGetItemPublishStatusRequest, Alloc>{ model },
        m_alternateId{ model.alternateId ? std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>>{ *model.alternateId } : std::nullopt },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_id{ SafeString(model.id) }
    {
        SetModelPointers();
    }

    PFCatalogGetItemPublishStatusRequestWrapper(const PFCatalogGetItemPublishStatusRequestWrapper& src) :
        PFCatalogGetItemPublishStatusRequestWrapper{ src.Model() }
    {
    }

    PFCatalogGetItemPublishStatusRequestWrapper(PFCatalogGetItemPublishStatusRequestWrapper&& src) :
        PFCatalogGetItemPublishStatusRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetItemPublishStatusRequestWrapper& operator=(PFCatalogGetItemPublishStatusRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetItemPublishStatusRequestWrapper() = default;

    friend void swap(PFCatalogGetItemPublishStatusRequestWrapper& lhs, PFCatalogGetItemPublishStatusRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_alternateId, rhs.m_alternateId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_id, rhs.m_id);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAlternateId(std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> value)
    {
        m_alternateId = std::move(value);
        this->m_model.alternateId = m_alternateId ? &m_alternateId->Model() : nullptr;
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

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.alternateId = m_alternateId ?  &m_alternateId->Model() : nullptr;
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
    }

    std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> m_alternateId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_id;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetItemPublishStatusResponseWrapper : public ModelWrapper<PFCatalogGetItemPublishStatusResponse, Alloc>
{
public:
    using ModelType = PFCatalogGetItemPublishStatusResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetItemPublishStatusResponseWrapper() = default;

    PFCatalogGetItemPublishStatusResponseWrapper(const PFCatalogGetItemPublishStatusResponse& model) :
        ModelWrapper<PFCatalogGetItemPublishStatusResponse, Alloc>{ model },
        m_result{ model.result ? std::optional<PFCatalogPublishResult>{ *model.result } : std::nullopt },
        m_statusMessage{ SafeString(model.statusMessage) }
    {
        SetModelPointers();
    }

    PFCatalogGetItemPublishStatusResponseWrapper(const PFCatalogGetItemPublishStatusResponseWrapper& src) :
        PFCatalogGetItemPublishStatusResponseWrapper{ src.Model() }
    {
    }

    PFCatalogGetItemPublishStatusResponseWrapper(PFCatalogGetItemPublishStatusResponseWrapper&& src) :
        PFCatalogGetItemPublishStatusResponseWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetItemPublishStatusResponseWrapper& operator=(PFCatalogGetItemPublishStatusResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetItemPublishStatusResponseWrapper() = default;

    friend void swap(PFCatalogGetItemPublishStatusResponseWrapper& lhs, PFCatalogGetItemPublishStatusResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_result, rhs.m_result);
        swap(lhs.m_statusMessage, rhs.m_statusMessage);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetResult(std::optional<PFCatalogPublishResult> value)
    {
        m_result = std::move(value);
        this->m_model.result = m_result ? m_result.operator->() : nullptr;
    }

    void SetStatusMessage(String value)
    {
        m_statusMessage = std::move(value);
        this->m_model.statusMessage =  m_statusMessage.empty() ? nullptr : m_statusMessage.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.result = m_result ? m_result.operator->() : nullptr;
        this->m_model.statusMessage = m_statusMessage.empty() ? nullptr : m_statusMessage.data();
    }

    std::optional<PFCatalogPublishResult> m_result;
    String m_statusMessage;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetItemReviewsRequestWrapper : public ModelWrapper<PFCatalogGetItemReviewsRequest, Alloc>
{
public:
    using ModelType = PFCatalogGetItemReviewsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetItemReviewsRequestWrapper() = default;

    PFCatalogGetItemReviewsRequestWrapper(const PFCatalogGetItemReviewsRequest& model) :
        ModelWrapper<PFCatalogGetItemReviewsRequest, Alloc>{ model },
        m_alternateId{ model.alternateId ? std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>>{ *model.alternateId } : std::nullopt },
        m_continuationToken{ SafeString(model.continuationToken) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_id{ SafeString(model.id) },
        m_orderBy{ SafeString(model.orderBy) }
    {
        SetModelPointers();
    }

    PFCatalogGetItemReviewsRequestWrapper(const PFCatalogGetItemReviewsRequestWrapper& src) :
        PFCatalogGetItemReviewsRequestWrapper{ src.Model() }
    {
    }

    PFCatalogGetItemReviewsRequestWrapper(PFCatalogGetItemReviewsRequestWrapper&& src) :
        PFCatalogGetItemReviewsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetItemReviewsRequestWrapper& operator=(PFCatalogGetItemReviewsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetItemReviewsRequestWrapper() = default;

    friend void swap(PFCatalogGetItemReviewsRequestWrapper& lhs, PFCatalogGetItemReviewsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_alternateId, rhs.m_alternateId);
        swap(lhs.m_continuationToken, rhs.m_continuationToken);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_id, rhs.m_id);
        swap(lhs.m_orderBy, rhs.m_orderBy);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAlternateId(std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> value)
    {
        m_alternateId = std::move(value);
        this->m_model.alternateId = m_alternateId ? &m_alternateId->Model() : nullptr;
    }

    void SetContinuationToken(String value)
    {
        m_continuationToken = std::move(value);
        this->m_model.continuationToken =  m_continuationToken.empty() ? nullptr : m_continuationToken.data();
    }

    void SetCount(int32_t value)
    {
        this->m_model.count = value;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

    void SetOrderBy(String value)
    {
        m_orderBy = std::move(value);
        this->m_model.orderBy =  m_orderBy.empty() ? nullptr : m_orderBy.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.alternateId = m_alternateId ?  &m_alternateId->Model() : nullptr;
        this->m_model.continuationToken = m_continuationToken.empty() ? nullptr : m_continuationToken.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
        this->m_model.orderBy = m_orderBy.empty() ? nullptr : m_orderBy.data();
    }

    std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> m_alternateId;
    String m_continuationToken;
    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_id;
    String m_orderBy;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetItemReviewsResponseWrapper : public ModelWrapper<PFCatalogGetItemReviewsResponse, Alloc>
{
public:
    using ModelType = PFCatalogGetItemReviewsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetItemReviewsResponseWrapper() = default;

    PFCatalogGetItemReviewsResponseWrapper(const PFCatalogGetItemReviewsResponse& model) :
        ModelWrapper<PFCatalogGetItemReviewsResponse, Alloc>{ model },
        m_continuationToken{ SafeString(model.continuationToken) },
        m_reviews{ model.reviews, model.reviews + model.reviewsCount }
    {
        SetModelPointers();
    }

    PFCatalogGetItemReviewsResponseWrapper(const PFCatalogGetItemReviewsResponseWrapper& src) :
        PFCatalogGetItemReviewsResponseWrapper{ src.Model() }
    {
    }

    PFCatalogGetItemReviewsResponseWrapper(PFCatalogGetItemReviewsResponseWrapper&& src) :
        PFCatalogGetItemReviewsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetItemReviewsResponseWrapper& operator=(PFCatalogGetItemReviewsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetItemReviewsResponseWrapper() = default;

    friend void swap(PFCatalogGetItemReviewsResponseWrapper& lhs, PFCatalogGetItemReviewsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_continuationToken, rhs.m_continuationToken);
        swap(lhs.m_reviews, rhs.m_reviews);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetContinuationToken(String value)
    {
        m_continuationToken = std::move(value);
        this->m_model.continuationToken =  m_continuationToken.empty() ? nullptr : m_continuationToken.data();
    }

    void SetReviews(ModelVector<PFCatalogReviewWrapper<Alloc>, Alloc> value)
    {
        m_reviews = std::move(value);
        this->m_model.reviews =  m_reviews.empty() ? nullptr : m_reviews.data();
        this->m_model.reviewsCount =  static_cast<uint32_t>(m_reviews.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.continuationToken = m_continuationToken.empty() ? nullptr : m_continuationToken.data();
        this->m_model.reviews = m_reviews.empty() ? nullptr : m_reviews.data();
    }

    String m_continuationToken;
    ModelVector<PFCatalogReviewWrapper<Alloc>, Alloc> m_reviews;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetItemReviewSummaryRequestWrapper : public ModelWrapper<PFCatalogGetItemReviewSummaryRequest, Alloc>
{
public:
    using ModelType = PFCatalogGetItemReviewSummaryRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetItemReviewSummaryRequestWrapper() = default;

    PFCatalogGetItemReviewSummaryRequestWrapper(const PFCatalogGetItemReviewSummaryRequest& model) :
        ModelWrapper<PFCatalogGetItemReviewSummaryRequest, Alloc>{ model },
        m_alternateId{ model.alternateId ? std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>>{ *model.alternateId } : std::nullopt },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_id{ SafeString(model.id) }
    {
        SetModelPointers();
    }

    PFCatalogGetItemReviewSummaryRequestWrapper(const PFCatalogGetItemReviewSummaryRequestWrapper& src) :
        PFCatalogGetItemReviewSummaryRequestWrapper{ src.Model() }
    {
    }

    PFCatalogGetItemReviewSummaryRequestWrapper(PFCatalogGetItemReviewSummaryRequestWrapper&& src) :
        PFCatalogGetItemReviewSummaryRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetItemReviewSummaryRequestWrapper& operator=(PFCatalogGetItemReviewSummaryRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetItemReviewSummaryRequestWrapper() = default;

    friend void swap(PFCatalogGetItemReviewSummaryRequestWrapper& lhs, PFCatalogGetItemReviewSummaryRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_alternateId, rhs.m_alternateId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_id, rhs.m_id);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAlternateId(std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> value)
    {
        m_alternateId = std::move(value);
        this->m_model.alternateId = m_alternateId ? &m_alternateId->Model() : nullptr;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.alternateId = m_alternateId ?  &m_alternateId->Model() : nullptr;
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
    }

    std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> m_alternateId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_id;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetItemReviewSummaryResponseWrapper : public ModelWrapper<PFCatalogGetItemReviewSummaryResponse, Alloc>
{
public:
    using ModelType = PFCatalogGetItemReviewSummaryResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetItemReviewSummaryResponseWrapper() = default;

    PFCatalogGetItemReviewSummaryResponseWrapper(const PFCatalogGetItemReviewSummaryResponse& model) :
        ModelWrapper<PFCatalogGetItemReviewSummaryResponse, Alloc>{ model },
        m_leastFavorableReview{ model.leastFavorableReview ? std::optional<PFCatalogReviewWrapper<Alloc>>{ *model.leastFavorableReview } : std::nullopt },
        m_mostFavorableReview{ model.mostFavorableReview ? std::optional<PFCatalogReviewWrapper<Alloc>>{ *model.mostFavorableReview } : std::nullopt },
        m_rating{ model.rating ? std::optional<PFCatalogRatingWrapper<Alloc>>{ *model.rating } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogGetItemReviewSummaryResponseWrapper(const PFCatalogGetItemReviewSummaryResponseWrapper& src) :
        PFCatalogGetItemReviewSummaryResponseWrapper{ src.Model() }
    {
    }

    PFCatalogGetItemReviewSummaryResponseWrapper(PFCatalogGetItemReviewSummaryResponseWrapper&& src) :
        PFCatalogGetItemReviewSummaryResponseWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetItemReviewSummaryResponseWrapper& operator=(PFCatalogGetItemReviewSummaryResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetItemReviewSummaryResponseWrapper() = default;

    friend void swap(PFCatalogGetItemReviewSummaryResponseWrapper& lhs, PFCatalogGetItemReviewSummaryResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_leastFavorableReview, rhs.m_leastFavorableReview);
        swap(lhs.m_mostFavorableReview, rhs.m_mostFavorableReview);
        swap(lhs.m_rating, rhs.m_rating);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetLeastFavorableReview(std::optional<PFCatalogReviewWrapper<Alloc>> value)
    {
        m_leastFavorableReview = std::move(value);
        this->m_model.leastFavorableReview = m_leastFavorableReview ? &m_leastFavorableReview->Model() : nullptr;
    }

    void SetMostFavorableReview(std::optional<PFCatalogReviewWrapper<Alloc>> value)
    {
        m_mostFavorableReview = std::move(value);
        this->m_model.mostFavorableReview = m_mostFavorableReview ? &m_mostFavorableReview->Model() : nullptr;
    }

    void SetRating(std::optional<PFCatalogRatingWrapper<Alloc>> value)
    {
        m_rating = std::move(value);
        this->m_model.rating = m_rating ? &m_rating->Model() : nullptr;
    }

    void SetReviewsCount(int32_t value)
    {
        this->m_model.reviewsCount = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.leastFavorableReview = m_leastFavorableReview ?  &m_leastFavorableReview->Model() : nullptr;
        this->m_model.mostFavorableReview = m_mostFavorableReview ?  &m_mostFavorableReview->Model() : nullptr;
        this->m_model.rating = m_rating ?  &m_rating->Model() : nullptr;
    }

    std::optional<PFCatalogReviewWrapper<Alloc>> m_leastFavorableReview;
    std::optional<PFCatalogReviewWrapper<Alloc>> m_mostFavorableReview;
    std::optional<PFCatalogRatingWrapper<Alloc>> m_rating;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetItemsRequestWrapper : public ModelWrapper<PFCatalogGetItemsRequest, Alloc>
{
public:
    using ModelType = PFCatalogGetItemsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetItemsRequestWrapper() = default;

    PFCatalogGetItemsRequestWrapper(const PFCatalogGetItemsRequest& model) :
        ModelWrapper<PFCatalogGetItemsRequest, Alloc>{ model },
        m_alternateIds{ model.alternateIds, model.alternateIds + model.alternateIdsCount },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_ids{ model.ids, model.ids + model.idsCount }
    {
        SetModelPointers();
    }

    PFCatalogGetItemsRequestWrapper(const PFCatalogGetItemsRequestWrapper& src) :
        PFCatalogGetItemsRequestWrapper{ src.Model() }
    {
    }

    PFCatalogGetItemsRequestWrapper(PFCatalogGetItemsRequestWrapper&& src) :
        PFCatalogGetItemsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetItemsRequestWrapper& operator=(PFCatalogGetItemsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetItemsRequestWrapper() = default;

    friend void swap(PFCatalogGetItemsRequestWrapper& lhs, PFCatalogGetItemsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_alternateIds, rhs.m_alternateIds);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_ids, rhs.m_ids);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAlternateIds(ModelVector<PFCatalogCatalogAlternateIdWrapper<Alloc>, Alloc> value)
    {
        m_alternateIds = std::move(value);
        this->m_model.alternateIds =  m_alternateIds.empty() ? nullptr : m_alternateIds.data();
        this->m_model.alternateIdsCount =  static_cast<uint32_t>(m_alternateIds.size());
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

    void SetIds(CStringVector<Alloc> value)
    {
        m_ids = std::move(value);
        this->m_model.ids =  m_ids.empty() ? nullptr : m_ids.data();
        this->m_model.idsCount =  static_cast<uint32_t>(m_ids.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.alternateIds = m_alternateIds.empty() ? nullptr : m_alternateIds.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.ids = m_ids.empty() ? nullptr : m_ids.data();
    }

    ModelVector<PFCatalogCatalogAlternateIdWrapper<Alloc>, Alloc> m_alternateIds;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    CStringVector<Alloc> m_ids;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogGetItemsResponseWrapper : public ModelWrapper<PFCatalogGetItemsResponse, Alloc>
{
public:
    using ModelType = PFCatalogGetItemsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogGetItemsResponseWrapper() = default;

    PFCatalogGetItemsResponseWrapper(const PFCatalogGetItemsResponse& model) :
        ModelWrapper<PFCatalogGetItemsResponse, Alloc>{ model },
        m_items{ model.items, model.items + model.itemsCount }
    {
        SetModelPointers();
    }

    PFCatalogGetItemsResponseWrapper(const PFCatalogGetItemsResponseWrapper& src) :
        PFCatalogGetItemsResponseWrapper{ src.Model() }
    {
    }

    PFCatalogGetItemsResponseWrapper(PFCatalogGetItemsResponseWrapper&& src) :
        PFCatalogGetItemsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogGetItemsResponseWrapper& operator=(PFCatalogGetItemsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogGetItemsResponseWrapper() = default;

    friend void swap(PFCatalogGetItemsResponseWrapper& lhs, PFCatalogGetItemsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_items, rhs.m_items);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetItems(ModelVector<PFCatalogCatalogItemWrapper<Alloc>, Alloc> value)
    {
        m_items = std::move(value);
        this->m_model.items =  m_items.empty() ? nullptr : m_items.data();
        this->m_model.itemsCount =  static_cast<uint32_t>(m_items.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.items = m_items.empty() ? nullptr : m_items.data();
    }

    ModelVector<PFCatalogCatalogItemWrapper<Alloc>, Alloc> m_items;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogPublishDraftItemRequestWrapper : public ModelWrapper<PFCatalogPublishDraftItemRequest, Alloc>
{
public:
    using ModelType = PFCatalogPublishDraftItemRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogPublishDraftItemRequestWrapper() = default;

    PFCatalogPublishDraftItemRequestWrapper(const PFCatalogPublishDraftItemRequest& model) :
        ModelWrapper<PFCatalogPublishDraftItemRequest, Alloc>{ model },
        m_alternateId{ model.alternateId ? std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>>{ *model.alternateId } : std::nullopt },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_eTag{ SafeString(model.eTag) },
        m_id{ SafeString(model.id) }
    {
        SetModelPointers();
    }

    PFCatalogPublishDraftItemRequestWrapper(const PFCatalogPublishDraftItemRequestWrapper& src) :
        PFCatalogPublishDraftItemRequestWrapper{ src.Model() }
    {
    }

    PFCatalogPublishDraftItemRequestWrapper(PFCatalogPublishDraftItemRequestWrapper&& src) :
        PFCatalogPublishDraftItemRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogPublishDraftItemRequestWrapper& operator=(PFCatalogPublishDraftItemRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogPublishDraftItemRequestWrapper() = default;

    friend void swap(PFCatalogPublishDraftItemRequestWrapper& lhs, PFCatalogPublishDraftItemRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_alternateId, rhs.m_alternateId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_eTag, rhs.m_eTag);
        swap(lhs.m_id, rhs.m_id);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAlternateId(std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> value)
    {
        m_alternateId = std::move(value);
        this->m_model.alternateId = m_alternateId ? &m_alternateId->Model() : nullptr;
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

    void SetETag(String value)
    {
        m_eTag = std::move(value);
        this->m_model.eTag =  m_eTag.empty() ? nullptr : m_eTag.data();
    }

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.alternateId = m_alternateId ?  &m_alternateId->Model() : nullptr;
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.eTag = m_eTag.empty() ? nullptr : m_eTag.data();
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
    }

    std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> m_alternateId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_eTag;
    String m_id;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogReportItemRequestWrapper : public ModelWrapper<PFCatalogReportItemRequest, Alloc>
{
public:
    using ModelType = PFCatalogReportItemRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogReportItemRequestWrapper() = default;

    PFCatalogReportItemRequestWrapper(const PFCatalogReportItemRequest& model) :
        ModelWrapper<PFCatalogReportItemRequest, Alloc>{ model },
        m_alternateId{ model.alternateId ? std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>>{ *model.alternateId } : std::nullopt },
        m_concernCategory{ model.concernCategory ? std::optional<PFCatalogConcernCategory>{ *model.concernCategory } : std::nullopt },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_id{ SafeString(model.id) },
        m_reason{ SafeString(model.reason) }
    {
        SetModelPointers();
    }

    PFCatalogReportItemRequestWrapper(const PFCatalogReportItemRequestWrapper& src) :
        PFCatalogReportItemRequestWrapper{ src.Model() }
    {
    }

    PFCatalogReportItemRequestWrapper(PFCatalogReportItemRequestWrapper&& src) :
        PFCatalogReportItemRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogReportItemRequestWrapper& operator=(PFCatalogReportItemRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogReportItemRequestWrapper() = default;

    friend void swap(PFCatalogReportItemRequestWrapper& lhs, PFCatalogReportItemRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_alternateId, rhs.m_alternateId);
        swap(lhs.m_concernCategory, rhs.m_concernCategory);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_id, rhs.m_id);
        swap(lhs.m_reason, rhs.m_reason);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAlternateId(std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> value)
    {
        m_alternateId = std::move(value);
        this->m_model.alternateId = m_alternateId ? &m_alternateId->Model() : nullptr;
    }

    void SetConcernCategory(std::optional<PFCatalogConcernCategory> value)
    {
        m_concernCategory = std::move(value);
        this->m_model.concernCategory = m_concernCategory ? m_concernCategory.operator->() : nullptr;
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

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

    void SetReason(String value)
    {
        m_reason = std::move(value);
        this->m_model.reason =  m_reason.empty() ? nullptr : m_reason.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.alternateId = m_alternateId ?  &m_alternateId->Model() : nullptr;
        this->m_model.concernCategory = m_concernCategory ? m_concernCategory.operator->() : nullptr;
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
        this->m_model.reason = m_reason.empty() ? nullptr : m_reason.data();
    }

    std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> m_alternateId;
    std::optional<PFCatalogConcernCategory> m_concernCategory;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_id;
    String m_reason;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogReportItemReviewRequestWrapper : public ModelWrapper<PFCatalogReportItemReviewRequest, Alloc>
{
public:
    using ModelType = PFCatalogReportItemReviewRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogReportItemReviewRequestWrapper() = default;

    PFCatalogReportItemReviewRequestWrapper(const PFCatalogReportItemReviewRequest& model) :
        ModelWrapper<PFCatalogReportItemReviewRequest, Alloc>{ model },
        m_alternateId{ model.alternateId ? std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>>{ *model.alternateId } : std::nullopt },
        m_concernCategory{ model.concernCategory ? std::optional<PFCatalogConcernCategory>{ *model.concernCategory } : std::nullopt },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_itemId{ SafeString(model.itemId) },
        m_reason{ SafeString(model.reason) },
        m_reviewId{ SafeString(model.reviewId) }
    {
        SetModelPointers();
    }

    PFCatalogReportItemReviewRequestWrapper(const PFCatalogReportItemReviewRequestWrapper& src) :
        PFCatalogReportItemReviewRequestWrapper{ src.Model() }
    {
    }

    PFCatalogReportItemReviewRequestWrapper(PFCatalogReportItemReviewRequestWrapper&& src) :
        PFCatalogReportItemReviewRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogReportItemReviewRequestWrapper& operator=(PFCatalogReportItemReviewRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogReportItemReviewRequestWrapper() = default;

    friend void swap(PFCatalogReportItemReviewRequestWrapper& lhs, PFCatalogReportItemReviewRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_alternateId, rhs.m_alternateId);
        swap(lhs.m_concernCategory, rhs.m_concernCategory);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_itemId, rhs.m_itemId);
        swap(lhs.m_reason, rhs.m_reason);
        swap(lhs.m_reviewId, rhs.m_reviewId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAlternateId(std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> value)
    {
        m_alternateId = std::move(value);
        this->m_model.alternateId = m_alternateId ? &m_alternateId->Model() : nullptr;
    }

    void SetConcernCategory(std::optional<PFCatalogConcernCategory> value)
    {
        m_concernCategory = std::move(value);
        this->m_model.concernCategory = m_concernCategory ? m_concernCategory.operator->() : nullptr;
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

    void SetItemId(String value)
    {
        m_itemId = std::move(value);
        this->m_model.itemId =  m_itemId.empty() ? nullptr : m_itemId.data();
    }

    void SetReason(String value)
    {
        m_reason = std::move(value);
        this->m_model.reason =  m_reason.empty() ? nullptr : m_reason.data();
    }

    void SetReviewId(String value)
    {
        m_reviewId = std::move(value);
        this->m_model.reviewId =  m_reviewId.empty() ? nullptr : m_reviewId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.alternateId = m_alternateId ?  &m_alternateId->Model() : nullptr;
        this->m_model.concernCategory = m_concernCategory ? m_concernCategory.operator->() : nullptr;
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.itemId = m_itemId.empty() ? nullptr : m_itemId.data();
        this->m_model.reason = m_reason.empty() ? nullptr : m_reason.data();
        this->m_model.reviewId = m_reviewId.empty() ? nullptr : m_reviewId.data();
    }

    std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> m_alternateId;
    std::optional<PFCatalogConcernCategory> m_concernCategory;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_itemId;
    String m_reason;
    String m_reviewId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogReviewItemRequestWrapper : public ModelWrapper<PFCatalogReviewItemRequest, Alloc>
{
public:
    using ModelType = PFCatalogReviewItemRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogReviewItemRequestWrapper() = default;

    PFCatalogReviewItemRequestWrapper(const PFCatalogReviewItemRequest& model) :
        ModelWrapper<PFCatalogReviewItemRequest, Alloc>{ model },
        m_alternateId{ model.alternateId ? std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>>{ *model.alternateId } : std::nullopt },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_id{ SafeString(model.id) },
        m_review{ model.review ? std::optional<PFCatalogReviewWrapper<Alloc>>{ *model.review } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogReviewItemRequestWrapper(const PFCatalogReviewItemRequestWrapper& src) :
        PFCatalogReviewItemRequestWrapper{ src.Model() }
    {
    }

    PFCatalogReviewItemRequestWrapper(PFCatalogReviewItemRequestWrapper&& src) :
        PFCatalogReviewItemRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogReviewItemRequestWrapper& operator=(PFCatalogReviewItemRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogReviewItemRequestWrapper() = default;

    friend void swap(PFCatalogReviewItemRequestWrapper& lhs, PFCatalogReviewItemRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_alternateId, rhs.m_alternateId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_id, rhs.m_id);
        swap(lhs.m_review, rhs.m_review);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAlternateId(std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> value)
    {
        m_alternateId = std::move(value);
        this->m_model.alternateId = m_alternateId ? &m_alternateId->Model() : nullptr;
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

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

    void SetReview(std::optional<PFCatalogReviewWrapper<Alloc>> value)
    {
        m_review = std::move(value);
        this->m_model.review = m_review ? &m_review->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.alternateId = m_alternateId ?  &m_alternateId->Model() : nullptr;
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
        this->m_model.review = m_review ?  &m_review->Model() : nullptr;
    }

    std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> m_alternateId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_id;
    std::optional<PFCatalogReviewWrapper<Alloc>> m_review;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogStoreReferenceWrapper : public ModelWrapper<PFCatalogStoreReference, Alloc>
{
public:
    using ModelType = PFCatalogStoreReference;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogStoreReferenceWrapper() = default;

    PFCatalogStoreReferenceWrapper(const PFCatalogStoreReference& model) :
        ModelWrapper<PFCatalogStoreReference, Alloc>{ model },
        m_alternateId{ model.alternateId ? std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>>{ *model.alternateId } : std::nullopt },
        m_id{ SafeString(model.id) }
    {
        SetModelPointers();
    }

    PFCatalogStoreReferenceWrapper(const PFCatalogStoreReferenceWrapper& src) :
        PFCatalogStoreReferenceWrapper{ src.Model() }
    {
    }

    PFCatalogStoreReferenceWrapper(PFCatalogStoreReferenceWrapper&& src) :
        PFCatalogStoreReferenceWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogStoreReferenceWrapper& operator=(PFCatalogStoreReferenceWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogStoreReferenceWrapper() = default;

    friend void swap(PFCatalogStoreReferenceWrapper& lhs, PFCatalogStoreReferenceWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_alternateId, rhs.m_alternateId);
        swap(lhs.m_id, rhs.m_id);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAlternateId(std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> value)
    {
        m_alternateId = std::move(value);
        this->m_model.alternateId = m_alternateId ? &m_alternateId->Model() : nullptr;
    }

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.alternateId = m_alternateId ?  &m_alternateId->Model() : nullptr;
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
    }

    std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> m_alternateId;
    String m_id;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogSearchItemsRequestWrapper : public ModelWrapper<PFCatalogSearchItemsRequest, Alloc>
{
public:
    using ModelType = PFCatalogSearchItemsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogSearchItemsRequestWrapper() = default;

    PFCatalogSearchItemsRequestWrapper(const PFCatalogSearchItemsRequest& model) :
        ModelWrapper<PFCatalogSearchItemsRequest, Alloc>{ model },
        m_continuationToken{ SafeString(model.continuationToken) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_filter{ SafeString(model.filter) },
        m_language{ SafeString(model.language) },
        m_orderBy{ SafeString(model.orderBy) },
        m_search{ SafeString(model.search) },
        m_select{ SafeString(model.select) },
        m_store{ model.store ? std::optional<PFCatalogStoreReferenceWrapper<Alloc>>{ *model.store } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogSearchItemsRequestWrapper(const PFCatalogSearchItemsRequestWrapper& src) :
        PFCatalogSearchItemsRequestWrapper{ src.Model() }
    {
    }

    PFCatalogSearchItemsRequestWrapper(PFCatalogSearchItemsRequestWrapper&& src) :
        PFCatalogSearchItemsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogSearchItemsRequestWrapper& operator=(PFCatalogSearchItemsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogSearchItemsRequestWrapper() = default;

    friend void swap(PFCatalogSearchItemsRequestWrapper& lhs, PFCatalogSearchItemsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_continuationToken, rhs.m_continuationToken);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_filter, rhs.m_filter);
        swap(lhs.m_language, rhs.m_language);
        swap(lhs.m_orderBy, rhs.m_orderBy);
        swap(lhs.m_search, rhs.m_search);
        swap(lhs.m_select, rhs.m_select);
        swap(lhs.m_store, rhs.m_store);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetContinuationToken(String value)
    {
        m_continuationToken = std::move(value);
        this->m_model.continuationToken =  m_continuationToken.empty() ? nullptr : m_continuationToken.data();
    }

    void SetCount(int32_t value)
    {
        this->m_model.count = value;
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

    void SetFilter(String value)
    {
        m_filter = std::move(value);
        this->m_model.filter =  m_filter.empty() ? nullptr : m_filter.data();
    }

    void SetLanguage(String value)
    {
        m_language = std::move(value);
        this->m_model.language =  m_language.empty() ? nullptr : m_language.data();
    }

    void SetOrderBy(String value)
    {
        m_orderBy = std::move(value);
        this->m_model.orderBy =  m_orderBy.empty() ? nullptr : m_orderBy.data();
    }

    void SetSearch(String value)
    {
        m_search = std::move(value);
        this->m_model.search =  m_search.empty() ? nullptr : m_search.data();
    }

    void SetSelect(String value)
    {
        m_select = std::move(value);
        this->m_model.select =  m_select.empty() ? nullptr : m_select.data();
    }

    void SetStore(std::optional<PFCatalogStoreReferenceWrapper<Alloc>> value)
    {
        m_store = std::move(value);
        this->m_model.store = m_store ? &m_store->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.continuationToken = m_continuationToken.empty() ? nullptr : m_continuationToken.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.filter = m_filter.empty() ? nullptr : m_filter.data();
        this->m_model.language = m_language.empty() ? nullptr : m_language.data();
        this->m_model.orderBy = m_orderBy.empty() ? nullptr : m_orderBy.data();
        this->m_model.search = m_search.empty() ? nullptr : m_search.data();
        this->m_model.select = m_select.empty() ? nullptr : m_select.data();
        this->m_model.store = m_store ?  &m_store->Model() : nullptr;
    }

    String m_continuationToken;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_filter;
    String m_language;
    String m_orderBy;
    String m_search;
    String m_select;
    std::optional<PFCatalogStoreReferenceWrapper<Alloc>> m_store;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogSearchItemsResponseWrapper : public ModelWrapper<PFCatalogSearchItemsResponse, Alloc>
{
public:
    using ModelType = PFCatalogSearchItemsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogSearchItemsResponseWrapper() = default;

    PFCatalogSearchItemsResponseWrapper(const PFCatalogSearchItemsResponse& model) :
        ModelWrapper<PFCatalogSearchItemsResponse, Alloc>{ model },
        m_continuationToken{ SafeString(model.continuationToken) },
        m_items{ model.items, model.items + model.itemsCount }
    {
        SetModelPointers();
    }

    PFCatalogSearchItemsResponseWrapper(const PFCatalogSearchItemsResponseWrapper& src) :
        PFCatalogSearchItemsResponseWrapper{ src.Model() }
    {
    }

    PFCatalogSearchItemsResponseWrapper(PFCatalogSearchItemsResponseWrapper&& src) :
        PFCatalogSearchItemsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogSearchItemsResponseWrapper& operator=(PFCatalogSearchItemsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogSearchItemsResponseWrapper() = default;

    friend void swap(PFCatalogSearchItemsResponseWrapper& lhs, PFCatalogSearchItemsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_continuationToken, rhs.m_continuationToken);
        swap(lhs.m_items, rhs.m_items);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetContinuationToken(String value)
    {
        m_continuationToken = std::move(value);
        this->m_model.continuationToken =  m_continuationToken.empty() ? nullptr : m_continuationToken.data();
    }

    void SetItems(ModelVector<PFCatalogCatalogItemWrapper<Alloc>, Alloc> value)
    {
        m_items = std::move(value);
        this->m_model.items =  m_items.empty() ? nullptr : m_items.data();
        this->m_model.itemsCount =  static_cast<uint32_t>(m_items.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.continuationToken = m_continuationToken.empty() ? nullptr : m_continuationToken.data();
        this->m_model.items = m_items.empty() ? nullptr : m_items.data();
    }

    String m_continuationToken;
    ModelVector<PFCatalogCatalogItemWrapper<Alloc>, Alloc> m_items;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogSetItemModerationStateRequestWrapper : public ModelWrapper<PFCatalogSetItemModerationStateRequest, Alloc>
{
public:
    using ModelType = PFCatalogSetItemModerationStateRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogSetItemModerationStateRequestWrapper() = default;

    PFCatalogSetItemModerationStateRequestWrapper(const PFCatalogSetItemModerationStateRequest& model) :
        ModelWrapper<PFCatalogSetItemModerationStateRequest, Alloc>{ model },
        m_alternateId{ model.alternateId ? std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>>{ *model.alternateId } : std::nullopt },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_id{ SafeString(model.id) },
        m_reason{ SafeString(model.reason) },
        m_status{ model.status ? std::optional<PFCatalogModerationStatus>{ *model.status } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogSetItemModerationStateRequestWrapper(const PFCatalogSetItemModerationStateRequestWrapper& src) :
        PFCatalogSetItemModerationStateRequestWrapper{ src.Model() }
    {
    }

    PFCatalogSetItemModerationStateRequestWrapper(PFCatalogSetItemModerationStateRequestWrapper&& src) :
        PFCatalogSetItemModerationStateRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogSetItemModerationStateRequestWrapper& operator=(PFCatalogSetItemModerationStateRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogSetItemModerationStateRequestWrapper() = default;

    friend void swap(PFCatalogSetItemModerationStateRequestWrapper& lhs, PFCatalogSetItemModerationStateRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_alternateId, rhs.m_alternateId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_id, rhs.m_id);
        swap(lhs.m_reason, rhs.m_reason);
        swap(lhs.m_status, rhs.m_status);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAlternateId(std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> value)
    {
        m_alternateId = std::move(value);
        this->m_model.alternateId = m_alternateId ? &m_alternateId->Model() : nullptr;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

    void SetReason(String value)
    {
        m_reason = std::move(value);
        this->m_model.reason =  m_reason.empty() ? nullptr : m_reason.data();
    }

    void SetStatus(std::optional<PFCatalogModerationStatus> value)
    {
        m_status = std::move(value);
        this->m_model.status = m_status ? m_status.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.alternateId = m_alternateId ?  &m_alternateId->Model() : nullptr;
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
        this->m_model.reason = m_reason.empty() ? nullptr : m_reason.data();
        this->m_model.status = m_status ? m_status.operator->() : nullptr;
    }

    std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> m_alternateId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_id;
    String m_reason;
    std::optional<PFCatalogModerationStatus> m_status;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogSubmitItemReviewVoteRequestWrapper : public ModelWrapper<PFCatalogSubmitItemReviewVoteRequest, Alloc>
{
public:
    using ModelType = PFCatalogSubmitItemReviewVoteRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogSubmitItemReviewVoteRequestWrapper() = default;

    PFCatalogSubmitItemReviewVoteRequestWrapper(const PFCatalogSubmitItemReviewVoteRequest& model) :
        ModelWrapper<PFCatalogSubmitItemReviewVoteRequest, Alloc>{ model },
        m_alternateId{ model.alternateId ? std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>>{ *model.alternateId } : std::nullopt },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_itemId{ SafeString(model.itemId) },
        m_reviewId{ SafeString(model.reviewId) },
        m_vote{ model.vote ? std::optional<PFCatalogHelpfulnessVote>{ *model.vote } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogSubmitItemReviewVoteRequestWrapper(const PFCatalogSubmitItemReviewVoteRequestWrapper& src) :
        PFCatalogSubmitItemReviewVoteRequestWrapper{ src.Model() }
    {
    }

    PFCatalogSubmitItemReviewVoteRequestWrapper(PFCatalogSubmitItemReviewVoteRequestWrapper&& src) :
        PFCatalogSubmitItemReviewVoteRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogSubmitItemReviewVoteRequestWrapper& operator=(PFCatalogSubmitItemReviewVoteRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogSubmitItemReviewVoteRequestWrapper() = default;

    friend void swap(PFCatalogSubmitItemReviewVoteRequestWrapper& lhs, PFCatalogSubmitItemReviewVoteRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_alternateId, rhs.m_alternateId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_itemId, rhs.m_itemId);
        swap(lhs.m_reviewId, rhs.m_reviewId);
        swap(lhs.m_vote, rhs.m_vote);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAlternateId(std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> value)
    {
        m_alternateId = std::move(value);
        this->m_model.alternateId = m_alternateId ? &m_alternateId->Model() : nullptr;
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

    void SetItemId(String value)
    {
        m_itemId = std::move(value);
        this->m_model.itemId =  m_itemId.empty() ? nullptr : m_itemId.data();
    }

    void SetReviewId(String value)
    {
        m_reviewId = std::move(value);
        this->m_model.reviewId =  m_reviewId.empty() ? nullptr : m_reviewId.data();
    }

    void SetVote(std::optional<PFCatalogHelpfulnessVote> value)
    {
        m_vote = std::move(value);
        this->m_model.vote = m_vote ? m_vote.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.alternateId = m_alternateId ?  &m_alternateId->Model() : nullptr;
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.itemId = m_itemId.empty() ? nullptr : m_itemId.data();
        this->m_model.reviewId = m_reviewId.empty() ? nullptr : m_reviewId.data();
        this->m_model.vote = m_vote ? m_vote.operator->() : nullptr;
    }

    std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> m_alternateId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_itemId;
    String m_reviewId;
    std::optional<PFCatalogHelpfulnessVote> m_vote;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogReviewTakedownWrapper : public ModelWrapper<PFCatalogReviewTakedown, Alloc>
{
public:
    using ModelType = PFCatalogReviewTakedown;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogReviewTakedownWrapper() = default;

    PFCatalogReviewTakedownWrapper(const PFCatalogReviewTakedown& model) :
        ModelWrapper<PFCatalogReviewTakedown, Alloc>{ model },
        m_alternateId{ model.alternateId ? std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>>{ *model.alternateId } : std::nullopt },
        m_itemId{ SafeString(model.itemId) },
        m_reviewId{ SafeString(model.reviewId) }
    {
        SetModelPointers();
    }

    PFCatalogReviewTakedownWrapper(const PFCatalogReviewTakedownWrapper& src) :
        PFCatalogReviewTakedownWrapper{ src.Model() }
    {
    }

    PFCatalogReviewTakedownWrapper(PFCatalogReviewTakedownWrapper&& src) :
        PFCatalogReviewTakedownWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogReviewTakedownWrapper& operator=(PFCatalogReviewTakedownWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogReviewTakedownWrapper() = default;

    friend void swap(PFCatalogReviewTakedownWrapper& lhs, PFCatalogReviewTakedownWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_alternateId, rhs.m_alternateId);
        swap(lhs.m_itemId, rhs.m_itemId);
        swap(lhs.m_reviewId, rhs.m_reviewId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAlternateId(std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> value)
    {
        m_alternateId = std::move(value);
        this->m_model.alternateId = m_alternateId ? &m_alternateId->Model() : nullptr;
    }

    void SetItemId(String value)
    {
        m_itemId = std::move(value);
        this->m_model.itemId =  m_itemId.empty() ? nullptr : m_itemId.data();
    }

    void SetReviewId(String value)
    {
        m_reviewId = std::move(value);
        this->m_model.reviewId =  m_reviewId.empty() ? nullptr : m_reviewId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.alternateId = m_alternateId ?  &m_alternateId->Model() : nullptr;
        this->m_model.itemId = m_itemId.empty() ? nullptr : m_itemId.data();
        this->m_model.reviewId = m_reviewId.empty() ? nullptr : m_reviewId.data();
    }

    std::optional<PFCatalogCatalogAlternateIdWrapper<Alloc>> m_alternateId;
    String m_itemId;
    String m_reviewId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogTakedownItemReviewsRequestWrapper : public ModelWrapper<PFCatalogTakedownItemReviewsRequest, Alloc>
{
public:
    using ModelType = PFCatalogTakedownItemReviewsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogTakedownItemReviewsRequestWrapper() = default;

    PFCatalogTakedownItemReviewsRequestWrapper(const PFCatalogTakedownItemReviewsRequest& model) :
        ModelWrapper<PFCatalogTakedownItemReviewsRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_reviews{ model.reviews, model.reviews + model.reviewsCount }
    {
        SetModelPointers();
    }

    PFCatalogTakedownItemReviewsRequestWrapper(const PFCatalogTakedownItemReviewsRequestWrapper& src) :
        PFCatalogTakedownItemReviewsRequestWrapper{ src.Model() }
    {
    }

    PFCatalogTakedownItemReviewsRequestWrapper(PFCatalogTakedownItemReviewsRequestWrapper&& src) :
        PFCatalogTakedownItemReviewsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogTakedownItemReviewsRequestWrapper& operator=(PFCatalogTakedownItemReviewsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogTakedownItemReviewsRequestWrapper() = default;

    friend void swap(PFCatalogTakedownItemReviewsRequestWrapper& lhs, PFCatalogTakedownItemReviewsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_reviews, rhs.m_reviews);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetReviews(ModelVector<PFCatalogReviewTakedownWrapper<Alloc>, Alloc> value)
    {
        m_reviews = std::move(value);
        this->m_model.reviews =  m_reviews.empty() ? nullptr : m_reviews.data();
        this->m_model.reviewsCount =  static_cast<uint32_t>(m_reviews.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.reviews = m_reviews.empty() ? nullptr : m_reviews.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    ModelVector<PFCatalogReviewTakedownWrapper<Alloc>, Alloc> m_reviews;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogUpdateCatalogConfigRequestWrapper : public ModelWrapper<PFCatalogUpdateCatalogConfigRequest, Alloc>
{
public:
    using ModelType = PFCatalogUpdateCatalogConfigRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogUpdateCatalogConfigRequestWrapper() = default;

    PFCatalogUpdateCatalogConfigRequestWrapper(const PFCatalogUpdateCatalogConfigRequest& model) :
        ModelWrapper<PFCatalogUpdateCatalogConfigRequest, Alloc>{ model },
        m_config{ model.config ? std::optional<PFCatalogCatalogConfigWrapper<Alloc>>{ *model.config } : std::nullopt },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount }
    {
        SetModelPointers();
    }

    PFCatalogUpdateCatalogConfigRequestWrapper(const PFCatalogUpdateCatalogConfigRequestWrapper& src) :
        PFCatalogUpdateCatalogConfigRequestWrapper{ src.Model() }
    {
    }

    PFCatalogUpdateCatalogConfigRequestWrapper(PFCatalogUpdateCatalogConfigRequestWrapper&& src) :
        PFCatalogUpdateCatalogConfigRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogUpdateCatalogConfigRequestWrapper& operator=(PFCatalogUpdateCatalogConfigRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogUpdateCatalogConfigRequestWrapper() = default;

    friend void swap(PFCatalogUpdateCatalogConfigRequestWrapper& lhs, PFCatalogUpdateCatalogConfigRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_config, rhs.m_config);
        swap(lhs.m_customTags, rhs.m_customTags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetConfig(std::optional<PFCatalogCatalogConfigWrapper<Alloc>> value)
    {
        m_config = std::move(value);
        this->m_model.config = m_config ? &m_config->Model() : nullptr;
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
        this->m_model.config = m_config ?  &m_config->Model() : nullptr;
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
    }

    std::optional<PFCatalogCatalogConfigWrapper<Alloc>> m_config;
    StringDictionaryEntryVector<Alloc> m_customTags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogUpdateDraftItemRequestWrapper : public ModelWrapper<PFCatalogUpdateDraftItemRequest, Alloc>
{
public:
    using ModelType = PFCatalogUpdateDraftItemRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogUpdateDraftItemRequestWrapper() = default;

    PFCatalogUpdateDraftItemRequestWrapper(const PFCatalogUpdateDraftItemRequest& model) :
        ModelWrapper<PFCatalogUpdateDraftItemRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_item{ model.item ? std::optional<PFCatalogCatalogItemWrapper<Alloc>>{ *model.item } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogUpdateDraftItemRequestWrapper(const PFCatalogUpdateDraftItemRequestWrapper& src) :
        PFCatalogUpdateDraftItemRequestWrapper{ src.Model() }
    {
    }

    PFCatalogUpdateDraftItemRequestWrapper(PFCatalogUpdateDraftItemRequestWrapper&& src) :
        PFCatalogUpdateDraftItemRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogUpdateDraftItemRequestWrapper& operator=(PFCatalogUpdateDraftItemRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogUpdateDraftItemRequestWrapper() = default;

    friend void swap(PFCatalogUpdateDraftItemRequestWrapper& lhs, PFCatalogUpdateDraftItemRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_item, rhs.m_item);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetItem(std::optional<PFCatalogCatalogItemWrapper<Alloc>> value)
    {
        m_item = std::move(value);
        this->m_model.item = m_item ? &m_item->Model() : nullptr;
    }

    void SetPublish(bool value)
    {
        this->m_model.publish = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.item = m_item ?  &m_item->Model() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFCatalogCatalogItemWrapper<Alloc>> m_item;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCatalogUpdateDraftItemResponseWrapper : public ModelWrapper<PFCatalogUpdateDraftItemResponse, Alloc>
{
public:
    using ModelType = PFCatalogUpdateDraftItemResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCatalogUpdateDraftItemResponseWrapper() = default;

    PFCatalogUpdateDraftItemResponseWrapper(const PFCatalogUpdateDraftItemResponse& model) :
        ModelWrapper<PFCatalogUpdateDraftItemResponse, Alloc>{ model },
        m_item{ model.item ? std::optional<PFCatalogCatalogItemWrapper<Alloc>>{ *model.item } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCatalogUpdateDraftItemResponseWrapper(const PFCatalogUpdateDraftItemResponseWrapper& src) :
        PFCatalogUpdateDraftItemResponseWrapper{ src.Model() }
    {
    }

    PFCatalogUpdateDraftItemResponseWrapper(PFCatalogUpdateDraftItemResponseWrapper&& src) :
        PFCatalogUpdateDraftItemResponseWrapper{}
    {
        swap(*this, src);
    }

    PFCatalogUpdateDraftItemResponseWrapper& operator=(PFCatalogUpdateDraftItemResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCatalogUpdateDraftItemResponseWrapper() = default;

    friend void swap(PFCatalogUpdateDraftItemResponseWrapper& lhs, PFCatalogUpdateDraftItemResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_item, rhs.m_item);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetItem(std::optional<PFCatalogCatalogItemWrapper<Alloc>> value)
    {
        m_item = std::move(value);
        this->m_model.item = m_item ? &m_item->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.item = m_item ?  &m_item->Model() : nullptr;
    }

    std::optional<PFCatalogCatalogItemWrapper<Alloc>> m_item;
};

} // namespace Wrappers
} // namespace PlayFab

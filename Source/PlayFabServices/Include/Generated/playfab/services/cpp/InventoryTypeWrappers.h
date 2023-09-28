// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFInventoryTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>
#if HC_PLATFORM == HC_PLATFORM_GDK
#include <playfab/core/cpp/AuthenticationTypeWrappers.h>
#endif

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryAlternateIdWrapper : public ModelWrapper<PFInventoryAlternateId, Alloc>
{
public:
    using ModelType = PFInventoryAlternateId;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryAlternateIdWrapper() = default;

    PFInventoryAlternateIdWrapper(const PFInventoryAlternateId& model) :
        ModelWrapper<PFInventoryAlternateId, Alloc>{ model },
        m_type{ SafeString(model.type) },
        m_value{ SafeString(model.value) }
    {
        SetModelPointers();
    }

    PFInventoryAlternateIdWrapper(const PFInventoryAlternateIdWrapper& src) :
        PFInventoryAlternateIdWrapper{ src.Model() }
    {
    }

    PFInventoryAlternateIdWrapper(PFInventoryAlternateIdWrapper&& src) :
        PFInventoryAlternateIdWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryAlternateIdWrapper& operator=(PFInventoryAlternateIdWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryAlternateIdWrapper() = default;

    friend void swap(PFInventoryAlternateIdWrapper& lhs, PFInventoryAlternateIdWrapper& rhs)
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
class PFInventoryInventoryItemReferenceWrapper : public ModelWrapper<PFInventoryInventoryItemReference, Alloc>
{
public:
    using ModelType = PFInventoryInventoryItemReference;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryInventoryItemReferenceWrapper() = default;

    PFInventoryInventoryItemReferenceWrapper(const PFInventoryInventoryItemReference& model) :
        ModelWrapper<PFInventoryInventoryItemReference, Alloc>{ model },
        m_alternateId{ model.alternateId ? std::optional<PFInventoryAlternateIdWrapper<Alloc>>{ *model.alternateId } : std::nullopt },
        m_id{ SafeString(model.id) },
        m_stackId{ SafeString(model.stackId) }
    {
        SetModelPointers();
    }

    PFInventoryInventoryItemReferenceWrapper(const PFInventoryInventoryItemReferenceWrapper& src) :
        PFInventoryInventoryItemReferenceWrapper{ src.Model() }
    {
    }

    PFInventoryInventoryItemReferenceWrapper(PFInventoryInventoryItemReferenceWrapper&& src) :
        PFInventoryInventoryItemReferenceWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryInventoryItemReferenceWrapper& operator=(PFInventoryInventoryItemReferenceWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryInventoryItemReferenceWrapper() = default;

    friend void swap(PFInventoryInventoryItemReferenceWrapper& lhs, PFInventoryInventoryItemReferenceWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_alternateId, rhs.m_alternateId);
        swap(lhs.m_id, rhs.m_id);
        swap(lhs.m_stackId, rhs.m_stackId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAlternateId(std::optional<PFInventoryAlternateIdWrapper<Alloc>> value)
    {
        m_alternateId = std::move(value);
        this->m_model.alternateId = m_alternateId ? &m_alternateId->Model() : nullptr;
    }

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

    void SetStackId(String value)
    {
        m_stackId = std::move(value);
        this->m_model.stackId =  m_stackId.empty() ? nullptr : m_stackId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.alternateId = m_alternateId ?  &m_alternateId->Model() : nullptr;
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
        this->m_model.stackId = m_stackId.empty() ? nullptr : m_stackId.data();
    }

    std::optional<PFInventoryAlternateIdWrapper<Alloc>> m_alternateId;
    String m_id;
    String m_stackId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryInitialValuesWrapper : public ModelWrapper<PFInventoryInitialValues, Alloc>
{
public:
    using ModelType = PFInventoryInitialValues;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryInitialValuesWrapper() = default;

    PFInventoryInitialValuesWrapper(const PFInventoryInitialValues& model) :
        ModelWrapper<PFInventoryInitialValues, Alloc>{ model },
        m_displayProperties{ model.displayProperties }
    {
        SetModelPointers();
    }

    PFInventoryInitialValuesWrapper(const PFInventoryInitialValuesWrapper& src) :
        PFInventoryInitialValuesWrapper{ src.Model() }
    {
    }

    PFInventoryInitialValuesWrapper(PFInventoryInitialValuesWrapper&& src) :
        PFInventoryInitialValuesWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryInitialValuesWrapper& operator=(PFInventoryInitialValuesWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryInitialValuesWrapper() = default;

    friend void swap(PFInventoryInitialValuesWrapper& lhs, PFInventoryInitialValuesWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_displayProperties, rhs.m_displayProperties);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetDisplayProperties(JsonObject<Alloc> value)
    {
        m_displayProperties = std::move(value);
        this->m_model.displayProperties.stringValue = m_displayProperties.stringValue.empty() ? nullptr : m_displayProperties.stringValue.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.displayProperties.stringValue = m_displayProperties.stringValue.empty() ? nullptr : m_displayProperties.stringValue.data();
    }

    JsonObject<Alloc> m_displayProperties;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryAddInventoryItemsRequestWrapper : public ModelWrapper<PFInventoryAddInventoryItemsRequest, Alloc>
{
public:
    using ModelType = PFInventoryAddInventoryItemsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryAddInventoryItemsRequestWrapper() = default;

    PFInventoryAddInventoryItemsRequestWrapper(const PFInventoryAddInventoryItemsRequest& model) :
        ModelWrapper<PFInventoryAddInventoryItemsRequest, Alloc>{ model },
        m_amount{ model.amount ? std::optional<int32_t>{ *model.amount } : std::nullopt },
        m_collectionId{ SafeString(model.collectionId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_durationInSeconds{ model.durationInSeconds ? std::optional<double>{ *model.durationInSeconds } : std::nullopt },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_eTag{ SafeString(model.eTag) },
        m_idempotencyId{ SafeString(model.idempotencyId) },
        m_item{ model.item ? std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>>{ *model.item } : std::nullopt },
        m_newStackValues{ model.newStackValues ? std::optional<PFInventoryInitialValuesWrapper<Alloc>>{ *model.newStackValues } : std::nullopt }
    {
        SetModelPointers();
    }

    PFInventoryAddInventoryItemsRequestWrapper(const PFInventoryAddInventoryItemsRequestWrapper& src) :
        PFInventoryAddInventoryItemsRequestWrapper{ src.Model() }
    {
    }

    PFInventoryAddInventoryItemsRequestWrapper(PFInventoryAddInventoryItemsRequestWrapper&& src) :
        PFInventoryAddInventoryItemsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryAddInventoryItemsRequestWrapper& operator=(PFInventoryAddInventoryItemsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryAddInventoryItemsRequestWrapper() = default;

    friend void swap(PFInventoryAddInventoryItemsRequestWrapper& lhs, PFInventoryAddInventoryItemsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_amount, rhs.m_amount);
        swap(lhs.m_collectionId, rhs.m_collectionId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_durationInSeconds, rhs.m_durationInSeconds);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_eTag, rhs.m_eTag);
        swap(lhs.m_idempotencyId, rhs.m_idempotencyId);
        swap(lhs.m_item, rhs.m_item);
        swap(lhs.m_newStackValues, rhs.m_newStackValues);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAmount(std::optional<int32_t> value)
    {
        m_amount = std::move(value);
        this->m_model.amount = m_amount ? m_amount.operator->() : nullptr;
    }

    void SetCollectionId(String value)
    {
        m_collectionId = std::move(value);
        this->m_model.collectionId =  m_collectionId.empty() ? nullptr : m_collectionId.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetDurationInSeconds(std::optional<double> value)
    {
        m_durationInSeconds = std::move(value);
        this->m_model.durationInSeconds = m_durationInSeconds ? m_durationInSeconds.operator->() : nullptr;
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

    void SetIdempotencyId(String value)
    {
        m_idempotencyId = std::move(value);
        this->m_model.idempotencyId =  m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
    }

    void SetItem(std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> value)
    {
        m_item = std::move(value);
        this->m_model.item = m_item ? &m_item->Model() : nullptr;
    }

    void SetNewStackValues(std::optional<PFInventoryInitialValuesWrapper<Alloc>> value)
    {
        m_newStackValues = std::move(value);
        this->m_model.newStackValues = m_newStackValues ? &m_newStackValues->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.amount = m_amount ? m_amount.operator->() : nullptr;
        this->m_model.collectionId = m_collectionId.empty() ? nullptr : m_collectionId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.durationInSeconds = m_durationInSeconds ? m_durationInSeconds.operator->() : nullptr;
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.eTag = m_eTag.empty() ? nullptr : m_eTag.data();
        this->m_model.idempotencyId = m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
        this->m_model.item = m_item ?  &m_item->Model() : nullptr;
        this->m_model.newStackValues = m_newStackValues ?  &m_newStackValues->Model() : nullptr;
    }

    std::optional<int32_t> m_amount;
    String m_collectionId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<double> m_durationInSeconds;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_eTag;
    String m_idempotencyId;
    std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> m_item;
    std::optional<PFInventoryInitialValuesWrapper<Alloc>> m_newStackValues;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryAddInventoryItemsResponseWrapper : public ModelWrapper<PFInventoryAddInventoryItemsResponse, Alloc>
{
public:
    using ModelType = PFInventoryAddInventoryItemsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryAddInventoryItemsResponseWrapper() = default;

    PFInventoryAddInventoryItemsResponseWrapper(const PFInventoryAddInventoryItemsResponse& model) :
        ModelWrapper<PFInventoryAddInventoryItemsResponse, Alloc>{ model },
        m_eTag{ SafeString(model.eTag) },
        m_idempotencyId{ SafeString(model.idempotencyId) },
        m_transactionIds{ model.transactionIds, model.transactionIds + model.transactionIdsCount }
    {
        SetModelPointers();
    }

    PFInventoryAddInventoryItemsResponseWrapper(const PFInventoryAddInventoryItemsResponseWrapper& src) :
        PFInventoryAddInventoryItemsResponseWrapper{ src.Model() }
    {
    }

    PFInventoryAddInventoryItemsResponseWrapper(PFInventoryAddInventoryItemsResponseWrapper&& src) :
        PFInventoryAddInventoryItemsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryAddInventoryItemsResponseWrapper& operator=(PFInventoryAddInventoryItemsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryAddInventoryItemsResponseWrapper() = default;

    friend void swap(PFInventoryAddInventoryItemsResponseWrapper& lhs, PFInventoryAddInventoryItemsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_eTag, rhs.m_eTag);
        swap(lhs.m_idempotencyId, rhs.m_idempotencyId);
        swap(lhs.m_transactionIds, rhs.m_transactionIds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetETag(String value)
    {
        m_eTag = std::move(value);
        this->m_model.eTag =  m_eTag.empty() ? nullptr : m_eTag.data();
    }

    void SetIdempotencyId(String value)
    {
        m_idempotencyId = std::move(value);
        this->m_model.idempotencyId =  m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
    }

    void SetTransactionIds(CStringVector<Alloc> value)
    {
        m_transactionIds = std::move(value);
        this->m_model.transactionIds =  m_transactionIds.empty() ? nullptr : m_transactionIds.data();
        this->m_model.transactionIdsCount =  static_cast<uint32_t>(m_transactionIds.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.eTag = m_eTag.empty() ? nullptr : m_eTag.data();
        this->m_model.idempotencyId = m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
        this->m_model.transactionIds = m_transactionIds.empty() ? nullptr : m_transactionIds.data();
    }

    String m_eTag;
    String m_idempotencyId;
    CStringVector<Alloc> m_transactionIds;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryDeleteInventoryCollectionRequestWrapper : public ModelWrapper<PFInventoryDeleteInventoryCollectionRequest, Alloc>
{
public:
    using ModelType = PFInventoryDeleteInventoryCollectionRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryDeleteInventoryCollectionRequestWrapper() = default;

    PFInventoryDeleteInventoryCollectionRequestWrapper(const PFInventoryDeleteInventoryCollectionRequest& model) :
        ModelWrapper<PFInventoryDeleteInventoryCollectionRequest, Alloc>{ model },
        m_collectionId{ SafeString(model.collectionId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_eTag{ SafeString(model.eTag) }
    {
        SetModelPointers();
    }

    PFInventoryDeleteInventoryCollectionRequestWrapper(const PFInventoryDeleteInventoryCollectionRequestWrapper& src) :
        PFInventoryDeleteInventoryCollectionRequestWrapper{ src.Model() }
    {
    }

    PFInventoryDeleteInventoryCollectionRequestWrapper(PFInventoryDeleteInventoryCollectionRequestWrapper&& src) :
        PFInventoryDeleteInventoryCollectionRequestWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryDeleteInventoryCollectionRequestWrapper& operator=(PFInventoryDeleteInventoryCollectionRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryDeleteInventoryCollectionRequestWrapper() = default;

    friend void swap(PFInventoryDeleteInventoryCollectionRequestWrapper& lhs, PFInventoryDeleteInventoryCollectionRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_collectionId, rhs.m_collectionId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_eTag, rhs.m_eTag);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCollectionId(String value)
    {
        m_collectionId = std::move(value);
        this->m_model.collectionId =  m_collectionId.empty() ? nullptr : m_collectionId.data();
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

private:
    void SetModelPointers()
    {
        this->m_model.collectionId = m_collectionId.empty() ? nullptr : m_collectionId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.eTag = m_eTag.empty() ? nullptr : m_eTag.data();
    }

    String m_collectionId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_eTag;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryDeleteInventoryItemsRequestWrapper : public ModelWrapper<PFInventoryDeleteInventoryItemsRequest, Alloc>
{
public:
    using ModelType = PFInventoryDeleteInventoryItemsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryDeleteInventoryItemsRequestWrapper() = default;

    PFInventoryDeleteInventoryItemsRequestWrapper(const PFInventoryDeleteInventoryItemsRequest& model) :
        ModelWrapper<PFInventoryDeleteInventoryItemsRequest, Alloc>{ model },
        m_collectionId{ SafeString(model.collectionId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_eTag{ SafeString(model.eTag) },
        m_idempotencyId{ SafeString(model.idempotencyId) },
        m_item{ model.item ? std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>>{ *model.item } : std::nullopt }
    {
        SetModelPointers();
    }

    PFInventoryDeleteInventoryItemsRequestWrapper(const PFInventoryDeleteInventoryItemsRequestWrapper& src) :
        PFInventoryDeleteInventoryItemsRequestWrapper{ src.Model() }
    {
    }

    PFInventoryDeleteInventoryItemsRequestWrapper(PFInventoryDeleteInventoryItemsRequestWrapper&& src) :
        PFInventoryDeleteInventoryItemsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryDeleteInventoryItemsRequestWrapper& operator=(PFInventoryDeleteInventoryItemsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryDeleteInventoryItemsRequestWrapper() = default;

    friend void swap(PFInventoryDeleteInventoryItemsRequestWrapper& lhs, PFInventoryDeleteInventoryItemsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_collectionId, rhs.m_collectionId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_eTag, rhs.m_eTag);
        swap(lhs.m_idempotencyId, rhs.m_idempotencyId);
        swap(lhs.m_item, rhs.m_item);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCollectionId(String value)
    {
        m_collectionId = std::move(value);
        this->m_model.collectionId =  m_collectionId.empty() ? nullptr : m_collectionId.data();
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

    void SetIdempotencyId(String value)
    {
        m_idempotencyId = std::move(value);
        this->m_model.idempotencyId =  m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
    }

    void SetItem(std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> value)
    {
        m_item = std::move(value);
        this->m_model.item = m_item ? &m_item->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.collectionId = m_collectionId.empty() ? nullptr : m_collectionId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.eTag = m_eTag.empty() ? nullptr : m_eTag.data();
        this->m_model.idempotencyId = m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
        this->m_model.item = m_item ?  &m_item->Model() : nullptr;
    }

    String m_collectionId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_eTag;
    String m_idempotencyId;
    std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> m_item;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryDeleteInventoryItemsResponseWrapper : public ModelWrapper<PFInventoryDeleteInventoryItemsResponse, Alloc>
{
public:
    using ModelType = PFInventoryDeleteInventoryItemsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryDeleteInventoryItemsResponseWrapper() = default;

    PFInventoryDeleteInventoryItemsResponseWrapper(const PFInventoryDeleteInventoryItemsResponse& model) :
        ModelWrapper<PFInventoryDeleteInventoryItemsResponse, Alloc>{ model },
        m_eTag{ SafeString(model.eTag) },
        m_idempotencyId{ SafeString(model.idempotencyId) },
        m_transactionIds{ model.transactionIds, model.transactionIds + model.transactionIdsCount }
    {
        SetModelPointers();
    }

    PFInventoryDeleteInventoryItemsResponseWrapper(const PFInventoryDeleteInventoryItemsResponseWrapper& src) :
        PFInventoryDeleteInventoryItemsResponseWrapper{ src.Model() }
    {
    }

    PFInventoryDeleteInventoryItemsResponseWrapper(PFInventoryDeleteInventoryItemsResponseWrapper&& src) :
        PFInventoryDeleteInventoryItemsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryDeleteInventoryItemsResponseWrapper& operator=(PFInventoryDeleteInventoryItemsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryDeleteInventoryItemsResponseWrapper() = default;

    friend void swap(PFInventoryDeleteInventoryItemsResponseWrapper& lhs, PFInventoryDeleteInventoryItemsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_eTag, rhs.m_eTag);
        swap(lhs.m_idempotencyId, rhs.m_idempotencyId);
        swap(lhs.m_transactionIds, rhs.m_transactionIds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetETag(String value)
    {
        m_eTag = std::move(value);
        this->m_model.eTag =  m_eTag.empty() ? nullptr : m_eTag.data();
    }

    void SetIdempotencyId(String value)
    {
        m_idempotencyId = std::move(value);
        this->m_model.idempotencyId =  m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
    }

    void SetTransactionIds(CStringVector<Alloc> value)
    {
        m_transactionIds = std::move(value);
        this->m_model.transactionIds =  m_transactionIds.empty() ? nullptr : m_transactionIds.data();
        this->m_model.transactionIdsCount =  static_cast<uint32_t>(m_transactionIds.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.eTag = m_eTag.empty() ? nullptr : m_eTag.data();
        this->m_model.idempotencyId = m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
        this->m_model.transactionIds = m_transactionIds.empty() ? nullptr : m_transactionIds.data();
    }

    String m_eTag;
    String m_idempotencyId;
    CStringVector<Alloc> m_transactionIds;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryAddInventoryItemsOperationWrapper : public ModelWrapper<PFInventoryAddInventoryItemsOperation, Alloc>
{
public:
    using ModelType = PFInventoryAddInventoryItemsOperation;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryAddInventoryItemsOperationWrapper() = default;

    PFInventoryAddInventoryItemsOperationWrapper(const PFInventoryAddInventoryItemsOperation& model) :
        ModelWrapper<PFInventoryAddInventoryItemsOperation, Alloc>{ model },
        m_amount{ model.amount ? std::optional<int32_t>{ *model.amount } : std::nullopt },
        m_durationInSeconds{ model.durationInSeconds ? std::optional<double>{ *model.durationInSeconds } : std::nullopt },
        m_item{ model.item ? std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>>{ *model.item } : std::nullopt },
        m_newStackValues{ model.newStackValues ? std::optional<PFInventoryInitialValuesWrapper<Alloc>>{ *model.newStackValues } : std::nullopt }
    {
        SetModelPointers();
    }

    PFInventoryAddInventoryItemsOperationWrapper(const PFInventoryAddInventoryItemsOperationWrapper& src) :
        PFInventoryAddInventoryItemsOperationWrapper{ src.Model() }
    {
    }

    PFInventoryAddInventoryItemsOperationWrapper(PFInventoryAddInventoryItemsOperationWrapper&& src) :
        PFInventoryAddInventoryItemsOperationWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryAddInventoryItemsOperationWrapper& operator=(PFInventoryAddInventoryItemsOperationWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryAddInventoryItemsOperationWrapper() = default;

    friend void swap(PFInventoryAddInventoryItemsOperationWrapper& lhs, PFInventoryAddInventoryItemsOperationWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_amount, rhs.m_amount);
        swap(lhs.m_durationInSeconds, rhs.m_durationInSeconds);
        swap(lhs.m_item, rhs.m_item);
        swap(lhs.m_newStackValues, rhs.m_newStackValues);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAmount(std::optional<int32_t> value)
    {
        m_amount = std::move(value);
        this->m_model.amount = m_amount ? m_amount.operator->() : nullptr;
    }

    void SetDurationInSeconds(std::optional<double> value)
    {
        m_durationInSeconds = std::move(value);
        this->m_model.durationInSeconds = m_durationInSeconds ? m_durationInSeconds.operator->() : nullptr;
    }

    void SetItem(std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> value)
    {
        m_item = std::move(value);
        this->m_model.item = m_item ? &m_item->Model() : nullptr;
    }

    void SetNewStackValues(std::optional<PFInventoryInitialValuesWrapper<Alloc>> value)
    {
        m_newStackValues = std::move(value);
        this->m_model.newStackValues = m_newStackValues ? &m_newStackValues->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.amount = m_amount ? m_amount.operator->() : nullptr;
        this->m_model.durationInSeconds = m_durationInSeconds ? m_durationInSeconds.operator->() : nullptr;
        this->m_model.item = m_item ?  &m_item->Model() : nullptr;
        this->m_model.newStackValues = m_newStackValues ?  &m_newStackValues->Model() : nullptr;
    }

    std::optional<int32_t> m_amount;
    std::optional<double> m_durationInSeconds;
    std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> m_item;
    std::optional<PFInventoryInitialValuesWrapper<Alloc>> m_newStackValues;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryDeleteInventoryItemsOperationWrapper : public ModelWrapper<PFInventoryDeleteInventoryItemsOperation, Alloc>
{
public:
    using ModelType = PFInventoryDeleteInventoryItemsOperation;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryDeleteInventoryItemsOperationWrapper() = default;

    PFInventoryDeleteInventoryItemsOperationWrapper(const PFInventoryDeleteInventoryItemsOperation& model) :
        ModelWrapper<PFInventoryDeleteInventoryItemsOperation, Alloc>{ model },
        m_item{ model.item ? std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>>{ *model.item } : std::nullopt }
    {
        SetModelPointers();
    }

    PFInventoryDeleteInventoryItemsOperationWrapper(const PFInventoryDeleteInventoryItemsOperationWrapper& src) :
        PFInventoryDeleteInventoryItemsOperationWrapper{ src.Model() }
    {
    }

    PFInventoryDeleteInventoryItemsOperationWrapper(PFInventoryDeleteInventoryItemsOperationWrapper&& src) :
        PFInventoryDeleteInventoryItemsOperationWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryDeleteInventoryItemsOperationWrapper& operator=(PFInventoryDeleteInventoryItemsOperationWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryDeleteInventoryItemsOperationWrapper() = default;

    friend void swap(PFInventoryDeleteInventoryItemsOperationWrapper& lhs, PFInventoryDeleteInventoryItemsOperationWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_item, rhs.m_item);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetItem(std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> value)
    {
        m_item = std::move(value);
        this->m_model.item = m_item ? &m_item->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.item = m_item ?  &m_item->Model() : nullptr;
    }

    std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> m_item;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryPurchasePriceAmountWrapper : public ModelWrapper<PFInventoryPurchasePriceAmount, Alloc>
{
public:
    using ModelType = PFInventoryPurchasePriceAmount;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryPurchasePriceAmountWrapper() = default;

    PFInventoryPurchasePriceAmountWrapper(const PFInventoryPurchasePriceAmount& model) :
        ModelWrapper<PFInventoryPurchasePriceAmount, Alloc>{ model },
        m_itemId{ SafeString(model.itemId) },
        m_stackId{ SafeString(model.stackId) }
    {
        SetModelPointers();
    }

    PFInventoryPurchasePriceAmountWrapper(const PFInventoryPurchasePriceAmountWrapper& src) :
        PFInventoryPurchasePriceAmountWrapper{ src.Model() }
    {
    }

    PFInventoryPurchasePriceAmountWrapper(PFInventoryPurchasePriceAmountWrapper&& src) :
        PFInventoryPurchasePriceAmountWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryPurchasePriceAmountWrapper& operator=(PFInventoryPurchasePriceAmountWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryPurchasePriceAmountWrapper() = default;

    friend void swap(PFInventoryPurchasePriceAmountWrapper& lhs, PFInventoryPurchasePriceAmountWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_itemId, rhs.m_itemId);
        swap(lhs.m_stackId, rhs.m_stackId);
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

    void SetStackId(String value)
    {
        m_stackId = std::move(value);
        this->m_model.stackId =  m_stackId.empty() ? nullptr : m_stackId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.itemId = m_itemId.empty() ? nullptr : m_itemId.data();
        this->m_model.stackId = m_stackId.empty() ? nullptr : m_stackId.data();
    }

    String m_itemId;
    String m_stackId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryPurchaseInventoryItemsOperationWrapper : public ModelWrapper<PFInventoryPurchaseInventoryItemsOperation, Alloc>
{
public:
    using ModelType = PFInventoryPurchaseInventoryItemsOperation;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryPurchaseInventoryItemsOperationWrapper() = default;

    PFInventoryPurchaseInventoryItemsOperationWrapper(const PFInventoryPurchaseInventoryItemsOperation& model) :
        ModelWrapper<PFInventoryPurchaseInventoryItemsOperation, Alloc>{ model },
        m_amount{ model.amount ? std::optional<int32_t>{ *model.amount } : std::nullopt },
        m_durationInSeconds{ model.durationInSeconds ? std::optional<double>{ *model.durationInSeconds } : std::nullopt },
        m_item{ model.item ? std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>>{ *model.item } : std::nullopt },
        m_newStackValues{ model.newStackValues ? std::optional<PFInventoryInitialValuesWrapper<Alloc>>{ *model.newStackValues } : std::nullopt },
        m_priceAmounts{ model.priceAmounts, model.priceAmounts + model.priceAmountsCount },
        m_storeId{ SafeString(model.storeId) }
    {
        SetModelPointers();
    }

    PFInventoryPurchaseInventoryItemsOperationWrapper(const PFInventoryPurchaseInventoryItemsOperationWrapper& src) :
        PFInventoryPurchaseInventoryItemsOperationWrapper{ src.Model() }
    {
    }

    PFInventoryPurchaseInventoryItemsOperationWrapper(PFInventoryPurchaseInventoryItemsOperationWrapper&& src) :
        PFInventoryPurchaseInventoryItemsOperationWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryPurchaseInventoryItemsOperationWrapper& operator=(PFInventoryPurchaseInventoryItemsOperationWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryPurchaseInventoryItemsOperationWrapper() = default;

    friend void swap(PFInventoryPurchaseInventoryItemsOperationWrapper& lhs, PFInventoryPurchaseInventoryItemsOperationWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_amount, rhs.m_amount);
        swap(lhs.m_durationInSeconds, rhs.m_durationInSeconds);
        swap(lhs.m_item, rhs.m_item);
        swap(lhs.m_newStackValues, rhs.m_newStackValues);
        swap(lhs.m_priceAmounts, rhs.m_priceAmounts);
        swap(lhs.m_storeId, rhs.m_storeId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAmount(std::optional<int32_t> value)
    {
        m_amount = std::move(value);
        this->m_model.amount = m_amount ? m_amount.operator->() : nullptr;
    }

    void SetDeleteEmptyStacks(bool value)
    {
        this->m_model.deleteEmptyStacks = value;
    }

    void SetDurationInSeconds(std::optional<double> value)
    {
        m_durationInSeconds = std::move(value);
        this->m_model.durationInSeconds = m_durationInSeconds ? m_durationInSeconds.operator->() : nullptr;
    }

    void SetItem(std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> value)
    {
        m_item = std::move(value);
        this->m_model.item = m_item ? &m_item->Model() : nullptr;
    }

    void SetNewStackValues(std::optional<PFInventoryInitialValuesWrapper<Alloc>> value)
    {
        m_newStackValues = std::move(value);
        this->m_model.newStackValues = m_newStackValues ? &m_newStackValues->Model() : nullptr;
    }

    void SetPriceAmounts(ModelVector<PFInventoryPurchasePriceAmountWrapper<Alloc>, Alloc> value)
    {
        m_priceAmounts = std::move(value);
        this->m_model.priceAmounts =  m_priceAmounts.empty() ? nullptr : m_priceAmounts.data();
        this->m_model.priceAmountsCount =  static_cast<uint32_t>(m_priceAmounts.size());
    }

    void SetStoreId(String value)
    {
        m_storeId = std::move(value);
        this->m_model.storeId =  m_storeId.empty() ? nullptr : m_storeId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.amount = m_amount ? m_amount.operator->() : nullptr;
        this->m_model.durationInSeconds = m_durationInSeconds ? m_durationInSeconds.operator->() : nullptr;
        this->m_model.item = m_item ?  &m_item->Model() : nullptr;
        this->m_model.newStackValues = m_newStackValues ?  &m_newStackValues->Model() : nullptr;
        this->m_model.priceAmounts = m_priceAmounts.empty() ? nullptr : m_priceAmounts.data();
        this->m_model.storeId = m_storeId.empty() ? nullptr : m_storeId.data();
    }

    std::optional<int32_t> m_amount;
    std::optional<double> m_durationInSeconds;
    std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> m_item;
    std::optional<PFInventoryInitialValuesWrapper<Alloc>> m_newStackValues;
    ModelVector<PFInventoryPurchasePriceAmountWrapper<Alloc>, Alloc> m_priceAmounts;
    String m_storeId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventorySubtractInventoryItemsOperationWrapper : public ModelWrapper<PFInventorySubtractInventoryItemsOperation, Alloc>
{
public:
    using ModelType = PFInventorySubtractInventoryItemsOperation;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventorySubtractInventoryItemsOperationWrapper() = default;

    PFInventorySubtractInventoryItemsOperationWrapper(const PFInventorySubtractInventoryItemsOperation& model) :
        ModelWrapper<PFInventorySubtractInventoryItemsOperation, Alloc>{ model },
        m_amount{ model.amount ? std::optional<int32_t>{ *model.amount } : std::nullopt },
        m_durationInSeconds{ model.durationInSeconds ? std::optional<double>{ *model.durationInSeconds } : std::nullopt },
        m_item{ model.item ? std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>>{ *model.item } : std::nullopt }
    {
        SetModelPointers();
    }

    PFInventorySubtractInventoryItemsOperationWrapper(const PFInventorySubtractInventoryItemsOperationWrapper& src) :
        PFInventorySubtractInventoryItemsOperationWrapper{ src.Model() }
    {
    }

    PFInventorySubtractInventoryItemsOperationWrapper(PFInventorySubtractInventoryItemsOperationWrapper&& src) :
        PFInventorySubtractInventoryItemsOperationWrapper{}
    {
        swap(*this, src);
    }

    PFInventorySubtractInventoryItemsOperationWrapper& operator=(PFInventorySubtractInventoryItemsOperationWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventorySubtractInventoryItemsOperationWrapper() = default;

    friend void swap(PFInventorySubtractInventoryItemsOperationWrapper& lhs, PFInventorySubtractInventoryItemsOperationWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_amount, rhs.m_amount);
        swap(lhs.m_durationInSeconds, rhs.m_durationInSeconds);
        swap(lhs.m_item, rhs.m_item);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAmount(std::optional<int32_t> value)
    {
        m_amount = std::move(value);
        this->m_model.amount = m_amount ? m_amount.operator->() : nullptr;
    }

    void SetDeleteEmptyStacks(bool value)
    {
        this->m_model.deleteEmptyStacks = value;
    }

    void SetDurationInSeconds(std::optional<double> value)
    {
        m_durationInSeconds = std::move(value);
        this->m_model.durationInSeconds = m_durationInSeconds ? m_durationInSeconds.operator->() : nullptr;
    }

    void SetItem(std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> value)
    {
        m_item = std::move(value);
        this->m_model.item = m_item ? &m_item->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.amount = m_amount ? m_amount.operator->() : nullptr;
        this->m_model.durationInSeconds = m_durationInSeconds ? m_durationInSeconds.operator->() : nullptr;
        this->m_model.item = m_item ?  &m_item->Model() : nullptr;
    }

    std::optional<int32_t> m_amount;
    std::optional<double> m_durationInSeconds;
    std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> m_item;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryTransferInventoryItemsOperationWrapper : public ModelWrapper<PFInventoryTransferInventoryItemsOperation, Alloc>
{
public:
    using ModelType = PFInventoryTransferInventoryItemsOperation;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryTransferInventoryItemsOperationWrapper() = default;

    PFInventoryTransferInventoryItemsOperationWrapper(const PFInventoryTransferInventoryItemsOperation& model) :
        ModelWrapper<PFInventoryTransferInventoryItemsOperation, Alloc>{ model },
        m_amount{ model.amount ? std::optional<int32_t>{ *model.amount } : std::nullopt },
        m_givingItem{ model.givingItem ? std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>>{ *model.givingItem } : std::nullopt },
        m_newStackValues{ model.newStackValues ? std::optional<PFInventoryInitialValuesWrapper<Alloc>>{ *model.newStackValues } : std::nullopt },
        m_receivingItem{ model.receivingItem ? std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>>{ *model.receivingItem } : std::nullopt }
    {
        SetModelPointers();
    }

    PFInventoryTransferInventoryItemsOperationWrapper(const PFInventoryTransferInventoryItemsOperationWrapper& src) :
        PFInventoryTransferInventoryItemsOperationWrapper{ src.Model() }
    {
    }

    PFInventoryTransferInventoryItemsOperationWrapper(PFInventoryTransferInventoryItemsOperationWrapper&& src) :
        PFInventoryTransferInventoryItemsOperationWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryTransferInventoryItemsOperationWrapper& operator=(PFInventoryTransferInventoryItemsOperationWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryTransferInventoryItemsOperationWrapper() = default;

    friend void swap(PFInventoryTransferInventoryItemsOperationWrapper& lhs, PFInventoryTransferInventoryItemsOperationWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_amount, rhs.m_amount);
        swap(lhs.m_givingItem, rhs.m_givingItem);
        swap(lhs.m_newStackValues, rhs.m_newStackValues);
        swap(lhs.m_receivingItem, rhs.m_receivingItem);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAmount(std::optional<int32_t> value)
    {
        m_amount = std::move(value);
        this->m_model.amount = m_amount ? m_amount.operator->() : nullptr;
    }

    void SetDeleteEmptyStacks(bool value)
    {
        this->m_model.deleteEmptyStacks = value;
    }

    void SetGivingItem(std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> value)
    {
        m_givingItem = std::move(value);
        this->m_model.givingItem = m_givingItem ? &m_givingItem->Model() : nullptr;
    }

    void SetNewStackValues(std::optional<PFInventoryInitialValuesWrapper<Alloc>> value)
    {
        m_newStackValues = std::move(value);
        this->m_model.newStackValues = m_newStackValues ? &m_newStackValues->Model() : nullptr;
    }

    void SetReceivingItem(std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> value)
    {
        m_receivingItem = std::move(value);
        this->m_model.receivingItem = m_receivingItem ? &m_receivingItem->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.amount = m_amount ? m_amount.operator->() : nullptr;
        this->m_model.givingItem = m_givingItem ?  &m_givingItem->Model() : nullptr;
        this->m_model.newStackValues = m_newStackValues ?  &m_newStackValues->Model() : nullptr;
        this->m_model.receivingItem = m_receivingItem ?  &m_receivingItem->Model() : nullptr;
    }

    std::optional<int32_t> m_amount;
    std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> m_givingItem;
    std::optional<PFInventoryInitialValuesWrapper<Alloc>> m_newStackValues;
    std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> m_receivingItem;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryInventoryItemWrapper : public ModelWrapper<PFInventoryInventoryItem, Alloc>
{
public:
    using ModelType = PFInventoryInventoryItem;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryInventoryItemWrapper() = default;

    PFInventoryInventoryItemWrapper(const PFInventoryInventoryItem& model) :
        ModelWrapper<PFInventoryInventoryItem, Alloc>{ model },
        m_amount{ model.amount ? std::optional<int32_t>{ *model.amount } : std::nullopt },
        m_displayProperties{ model.displayProperties },
        m_expirationDate{ model.expirationDate ? std::optional<time_t>{ *model.expirationDate } : std::nullopt },
        m_id{ SafeString(model.id) },
        m_stackId{ SafeString(model.stackId) },
        m_type{ SafeString(model.type) }
    {
        SetModelPointers();
    }

    PFInventoryInventoryItemWrapper(const PFInventoryInventoryItemWrapper& src) :
        PFInventoryInventoryItemWrapper{ src.Model() }
    {
    }

    PFInventoryInventoryItemWrapper(PFInventoryInventoryItemWrapper&& src) :
        PFInventoryInventoryItemWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryInventoryItemWrapper& operator=(PFInventoryInventoryItemWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryInventoryItemWrapper() = default;

    friend void swap(PFInventoryInventoryItemWrapper& lhs, PFInventoryInventoryItemWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_amount, rhs.m_amount);
        swap(lhs.m_displayProperties, rhs.m_displayProperties);
        swap(lhs.m_expirationDate, rhs.m_expirationDate);
        swap(lhs.m_id, rhs.m_id);
        swap(lhs.m_stackId, rhs.m_stackId);
        swap(lhs.m_type, rhs.m_type);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAmount(std::optional<int32_t> value)
    {
        m_amount = std::move(value);
        this->m_model.amount = m_amount ? m_amount.operator->() : nullptr;
    }

    void SetDisplayProperties(JsonObject<Alloc> value)
    {
        m_displayProperties = std::move(value);
        this->m_model.displayProperties.stringValue = m_displayProperties.stringValue.empty() ? nullptr : m_displayProperties.stringValue.data();
    }

    void SetExpirationDate(std::optional<time_t> value)
    {
        m_expirationDate = std::move(value);
        this->m_model.expirationDate = m_expirationDate ? m_expirationDate.operator->() : nullptr;
    }

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

    void SetStackId(String value)
    {
        m_stackId = std::move(value);
        this->m_model.stackId =  m_stackId.empty() ? nullptr : m_stackId.data();
    }

    void SetType(String value)
    {
        m_type = std::move(value);
        this->m_model.type =  m_type.empty() ? nullptr : m_type.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.amount = m_amount ? m_amount.operator->() : nullptr;
        this->m_model.displayProperties.stringValue = m_displayProperties.stringValue.empty() ? nullptr : m_displayProperties.stringValue.data();
        this->m_model.expirationDate = m_expirationDate ? m_expirationDate.operator->() : nullptr;
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
        this->m_model.stackId = m_stackId.empty() ? nullptr : m_stackId.data();
        this->m_model.type = m_type.empty() ? nullptr : m_type.data();
    }

    std::optional<int32_t> m_amount;
    JsonObject<Alloc> m_displayProperties;
    std::optional<time_t> m_expirationDate;
    String m_id;
    String m_stackId;
    String m_type;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryUpdateInventoryItemsOperationWrapper : public ModelWrapper<PFInventoryUpdateInventoryItemsOperation, Alloc>
{
public:
    using ModelType = PFInventoryUpdateInventoryItemsOperation;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryUpdateInventoryItemsOperationWrapper() = default;

    PFInventoryUpdateInventoryItemsOperationWrapper(const PFInventoryUpdateInventoryItemsOperation& model) :
        ModelWrapper<PFInventoryUpdateInventoryItemsOperation, Alloc>{ model },
        m_item{ model.item ? std::optional<PFInventoryInventoryItemWrapper<Alloc>>{ *model.item } : std::nullopt }
    {
        SetModelPointers();
    }

    PFInventoryUpdateInventoryItemsOperationWrapper(const PFInventoryUpdateInventoryItemsOperationWrapper& src) :
        PFInventoryUpdateInventoryItemsOperationWrapper{ src.Model() }
    {
    }

    PFInventoryUpdateInventoryItemsOperationWrapper(PFInventoryUpdateInventoryItemsOperationWrapper&& src) :
        PFInventoryUpdateInventoryItemsOperationWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryUpdateInventoryItemsOperationWrapper& operator=(PFInventoryUpdateInventoryItemsOperationWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryUpdateInventoryItemsOperationWrapper() = default;

    friend void swap(PFInventoryUpdateInventoryItemsOperationWrapper& lhs, PFInventoryUpdateInventoryItemsOperationWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_item, rhs.m_item);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetItem(std::optional<PFInventoryInventoryItemWrapper<Alloc>> value)
    {
        m_item = std::move(value);
        this->m_model.item = m_item ? &m_item->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.item = m_item ?  &m_item->Model() : nullptr;
    }

    std::optional<PFInventoryInventoryItemWrapper<Alloc>> m_item;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryInventoryOperationWrapper : public ModelWrapper<PFInventoryInventoryOperation, Alloc>
{
public:
    using ModelType = PFInventoryInventoryOperation;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryInventoryOperationWrapper() = default;

    PFInventoryInventoryOperationWrapper(const PFInventoryInventoryOperation& model) :
        ModelWrapper<PFInventoryInventoryOperation, Alloc>{ model },
        m_add{ model.add ? std::optional<PFInventoryAddInventoryItemsOperationWrapper<Alloc>>{ *model.add } : std::nullopt },
        m_deleteOp{ model.deleteOp ? std::optional<PFInventoryDeleteInventoryItemsOperationWrapper<Alloc>>{ *model.deleteOp } : std::nullopt },
        m_purchase{ model.purchase ? std::optional<PFInventoryPurchaseInventoryItemsOperationWrapper<Alloc>>{ *model.purchase } : std::nullopt },
        m_subtract{ model.subtract ? std::optional<PFInventorySubtractInventoryItemsOperationWrapper<Alloc>>{ *model.subtract } : std::nullopt },
        m_transfer{ model.transfer ? std::optional<PFInventoryTransferInventoryItemsOperationWrapper<Alloc>>{ *model.transfer } : std::nullopt },
        m_update{ model.update ? std::optional<PFInventoryUpdateInventoryItemsOperationWrapper<Alloc>>{ *model.update } : std::nullopt }
    {
        SetModelPointers();
    }

    PFInventoryInventoryOperationWrapper(const PFInventoryInventoryOperationWrapper& src) :
        PFInventoryInventoryOperationWrapper{ src.Model() }
    {
    }

    PFInventoryInventoryOperationWrapper(PFInventoryInventoryOperationWrapper&& src) :
        PFInventoryInventoryOperationWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryInventoryOperationWrapper& operator=(PFInventoryInventoryOperationWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryInventoryOperationWrapper() = default;

    friend void swap(PFInventoryInventoryOperationWrapper& lhs, PFInventoryInventoryOperationWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_add, rhs.m_add);
        swap(lhs.m_deleteOp, rhs.m_deleteOp);
        swap(lhs.m_purchase, rhs.m_purchase);
        swap(lhs.m_subtract, rhs.m_subtract);
        swap(lhs.m_transfer, rhs.m_transfer);
        swap(lhs.m_update, rhs.m_update);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAdd(std::optional<PFInventoryAddInventoryItemsOperationWrapper<Alloc>> value)
    {
        m_add = std::move(value);
        this->m_model.add = m_add ? &m_add->Model() : nullptr;
    }

    void SetDeleteOp(std::optional<PFInventoryDeleteInventoryItemsOperationWrapper<Alloc>> value)
    {
        m_deleteOp = std::move(value);
        this->m_model.deleteOp = m_deleteOp ? &m_deleteOp->Model() : nullptr;
    }

    void SetPurchase(std::optional<PFInventoryPurchaseInventoryItemsOperationWrapper<Alloc>> value)
    {
        m_purchase = std::move(value);
        this->m_model.purchase = m_purchase ? &m_purchase->Model() : nullptr;
    }

    void SetSubtract(std::optional<PFInventorySubtractInventoryItemsOperationWrapper<Alloc>> value)
    {
        m_subtract = std::move(value);
        this->m_model.subtract = m_subtract ? &m_subtract->Model() : nullptr;
    }

    void SetTransfer(std::optional<PFInventoryTransferInventoryItemsOperationWrapper<Alloc>> value)
    {
        m_transfer = std::move(value);
        this->m_model.transfer = m_transfer ? &m_transfer->Model() : nullptr;
    }

    void SetUpdate(std::optional<PFInventoryUpdateInventoryItemsOperationWrapper<Alloc>> value)
    {
        m_update = std::move(value);
        this->m_model.update = m_update ? &m_update->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.add = m_add ?  &m_add->Model() : nullptr;
        this->m_model.deleteOp = m_deleteOp ?  &m_deleteOp->Model() : nullptr;
        this->m_model.purchase = m_purchase ?  &m_purchase->Model() : nullptr;
        this->m_model.subtract = m_subtract ?  &m_subtract->Model() : nullptr;
        this->m_model.transfer = m_transfer ?  &m_transfer->Model() : nullptr;
        this->m_model.update = m_update ?  &m_update->Model() : nullptr;
    }

    std::optional<PFInventoryAddInventoryItemsOperationWrapper<Alloc>> m_add;
    std::optional<PFInventoryDeleteInventoryItemsOperationWrapper<Alloc>> m_deleteOp;
    std::optional<PFInventoryPurchaseInventoryItemsOperationWrapper<Alloc>> m_purchase;
    std::optional<PFInventorySubtractInventoryItemsOperationWrapper<Alloc>> m_subtract;
    std::optional<PFInventoryTransferInventoryItemsOperationWrapper<Alloc>> m_transfer;
    std::optional<PFInventoryUpdateInventoryItemsOperationWrapper<Alloc>> m_update;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryExecuteInventoryOperationsRequestWrapper : public ModelWrapper<PFInventoryExecuteInventoryOperationsRequest, Alloc>
{
public:
    using ModelType = PFInventoryExecuteInventoryOperationsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryExecuteInventoryOperationsRequestWrapper() = default;

    PFInventoryExecuteInventoryOperationsRequestWrapper(const PFInventoryExecuteInventoryOperationsRequest& model) :
        ModelWrapper<PFInventoryExecuteInventoryOperationsRequest, Alloc>{ model },
        m_collectionId{ SafeString(model.collectionId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_eTag{ SafeString(model.eTag) },
        m_idempotencyId{ SafeString(model.idempotencyId) },
        m_operations{ model.operations, model.operations + model.operationsCount }
    {
        SetModelPointers();
    }

    PFInventoryExecuteInventoryOperationsRequestWrapper(const PFInventoryExecuteInventoryOperationsRequestWrapper& src) :
        PFInventoryExecuteInventoryOperationsRequestWrapper{ src.Model() }
    {
    }

    PFInventoryExecuteInventoryOperationsRequestWrapper(PFInventoryExecuteInventoryOperationsRequestWrapper&& src) :
        PFInventoryExecuteInventoryOperationsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryExecuteInventoryOperationsRequestWrapper& operator=(PFInventoryExecuteInventoryOperationsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryExecuteInventoryOperationsRequestWrapper() = default;

    friend void swap(PFInventoryExecuteInventoryOperationsRequestWrapper& lhs, PFInventoryExecuteInventoryOperationsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_collectionId, rhs.m_collectionId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_eTag, rhs.m_eTag);
        swap(lhs.m_idempotencyId, rhs.m_idempotencyId);
        swap(lhs.m_operations, rhs.m_operations);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCollectionId(String value)
    {
        m_collectionId = std::move(value);
        this->m_model.collectionId =  m_collectionId.empty() ? nullptr : m_collectionId.data();
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

    void SetIdempotencyId(String value)
    {
        m_idempotencyId = std::move(value);
        this->m_model.idempotencyId =  m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
    }

    void SetOperations(ModelVector<PFInventoryInventoryOperationWrapper<Alloc>, Alloc> value)
    {
        m_operations = std::move(value);
        this->m_model.operations =  m_operations.empty() ? nullptr : m_operations.data();
        this->m_model.operationsCount =  static_cast<uint32_t>(m_operations.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.collectionId = m_collectionId.empty() ? nullptr : m_collectionId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.eTag = m_eTag.empty() ? nullptr : m_eTag.data();
        this->m_model.idempotencyId = m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
        this->m_model.operations = m_operations.empty() ? nullptr : m_operations.data();
    }

    String m_collectionId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_eTag;
    String m_idempotencyId;
    ModelVector<PFInventoryInventoryOperationWrapper<Alloc>, Alloc> m_operations;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryExecuteInventoryOperationsResponseWrapper : public ModelWrapper<PFInventoryExecuteInventoryOperationsResponse, Alloc>
{
public:
    using ModelType = PFInventoryExecuteInventoryOperationsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryExecuteInventoryOperationsResponseWrapper() = default;

    PFInventoryExecuteInventoryOperationsResponseWrapper(const PFInventoryExecuteInventoryOperationsResponse& model) :
        ModelWrapper<PFInventoryExecuteInventoryOperationsResponse, Alloc>{ model },
        m_eTag{ SafeString(model.eTag) },
        m_idempotencyId{ SafeString(model.idempotencyId) },
        m_transactionIds{ model.transactionIds, model.transactionIds + model.transactionIdsCount }
    {
        SetModelPointers();
    }

    PFInventoryExecuteInventoryOperationsResponseWrapper(const PFInventoryExecuteInventoryOperationsResponseWrapper& src) :
        PFInventoryExecuteInventoryOperationsResponseWrapper{ src.Model() }
    {
    }

    PFInventoryExecuteInventoryOperationsResponseWrapper(PFInventoryExecuteInventoryOperationsResponseWrapper&& src) :
        PFInventoryExecuteInventoryOperationsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryExecuteInventoryOperationsResponseWrapper& operator=(PFInventoryExecuteInventoryOperationsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryExecuteInventoryOperationsResponseWrapper() = default;

    friend void swap(PFInventoryExecuteInventoryOperationsResponseWrapper& lhs, PFInventoryExecuteInventoryOperationsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_eTag, rhs.m_eTag);
        swap(lhs.m_idempotencyId, rhs.m_idempotencyId);
        swap(lhs.m_transactionIds, rhs.m_transactionIds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetETag(String value)
    {
        m_eTag = std::move(value);
        this->m_model.eTag =  m_eTag.empty() ? nullptr : m_eTag.data();
    }

    void SetIdempotencyId(String value)
    {
        m_idempotencyId = std::move(value);
        this->m_model.idempotencyId =  m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
    }

    void SetTransactionIds(CStringVector<Alloc> value)
    {
        m_transactionIds = std::move(value);
        this->m_model.transactionIds =  m_transactionIds.empty() ? nullptr : m_transactionIds.data();
        this->m_model.transactionIdsCount =  static_cast<uint32_t>(m_transactionIds.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.eTag = m_eTag.empty() ? nullptr : m_eTag.data();
        this->m_model.idempotencyId = m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
        this->m_model.transactionIds = m_transactionIds.empty() ? nullptr : m_transactionIds.data();
    }

    String m_eTag;
    String m_idempotencyId;
    CStringVector<Alloc> m_transactionIds;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryGetInventoryCollectionIdsRequestWrapper : public ModelWrapper<PFInventoryGetInventoryCollectionIdsRequest, Alloc>
{
public:
    using ModelType = PFInventoryGetInventoryCollectionIdsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryGetInventoryCollectionIdsRequestWrapper() = default;

    PFInventoryGetInventoryCollectionIdsRequestWrapper(const PFInventoryGetInventoryCollectionIdsRequest& model) :
        ModelWrapper<PFInventoryGetInventoryCollectionIdsRequest, Alloc>{ model },
        m_continuationToken{ SafeString(model.continuationToken) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt }
    {
        SetModelPointers();
    }

    PFInventoryGetInventoryCollectionIdsRequestWrapper(const PFInventoryGetInventoryCollectionIdsRequestWrapper& src) :
        PFInventoryGetInventoryCollectionIdsRequestWrapper{ src.Model() }
    {
    }

    PFInventoryGetInventoryCollectionIdsRequestWrapper(PFInventoryGetInventoryCollectionIdsRequestWrapper&& src) :
        PFInventoryGetInventoryCollectionIdsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryGetInventoryCollectionIdsRequestWrapper& operator=(PFInventoryGetInventoryCollectionIdsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryGetInventoryCollectionIdsRequestWrapper() = default;

    friend void swap(PFInventoryGetInventoryCollectionIdsRequestWrapper& lhs, PFInventoryGetInventoryCollectionIdsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_continuationToken, rhs.m_continuationToken);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
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

private:
    void SetModelPointers()
    {
        this->m_model.continuationToken = m_continuationToken.empty() ? nullptr : m_continuationToken.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
    }

    String m_continuationToken;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryGetInventoryCollectionIdsResponseWrapper : public ModelWrapper<PFInventoryGetInventoryCollectionIdsResponse, Alloc>
{
public:
    using ModelType = PFInventoryGetInventoryCollectionIdsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryGetInventoryCollectionIdsResponseWrapper() = default;

    PFInventoryGetInventoryCollectionIdsResponseWrapper(const PFInventoryGetInventoryCollectionIdsResponse& model) :
        ModelWrapper<PFInventoryGetInventoryCollectionIdsResponse, Alloc>{ model },
        m_collectionIds{ model.collectionIds, model.collectionIds + model.collectionIdsCount },
        m_continuationToken{ SafeString(model.continuationToken) }
    {
        SetModelPointers();
    }

    PFInventoryGetInventoryCollectionIdsResponseWrapper(const PFInventoryGetInventoryCollectionIdsResponseWrapper& src) :
        PFInventoryGetInventoryCollectionIdsResponseWrapper{ src.Model() }
    {
    }

    PFInventoryGetInventoryCollectionIdsResponseWrapper(PFInventoryGetInventoryCollectionIdsResponseWrapper&& src) :
        PFInventoryGetInventoryCollectionIdsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryGetInventoryCollectionIdsResponseWrapper& operator=(PFInventoryGetInventoryCollectionIdsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryGetInventoryCollectionIdsResponseWrapper() = default;

    friend void swap(PFInventoryGetInventoryCollectionIdsResponseWrapper& lhs, PFInventoryGetInventoryCollectionIdsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_collectionIds, rhs.m_collectionIds);
        swap(lhs.m_continuationToken, rhs.m_continuationToken);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCollectionIds(CStringVector<Alloc> value)
    {
        m_collectionIds = std::move(value);
        this->m_model.collectionIds =  m_collectionIds.empty() ? nullptr : m_collectionIds.data();
        this->m_model.collectionIdsCount =  static_cast<uint32_t>(m_collectionIds.size());
    }

    void SetContinuationToken(String value)
    {
        m_continuationToken = std::move(value);
        this->m_model.continuationToken =  m_continuationToken.empty() ? nullptr : m_continuationToken.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.collectionIds = m_collectionIds.empty() ? nullptr : m_collectionIds.data();
        this->m_model.continuationToken = m_continuationToken.empty() ? nullptr : m_continuationToken.data();
    }

    CStringVector<Alloc> m_collectionIds;
    String m_continuationToken;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryGetInventoryItemsRequestWrapper : public ModelWrapper<PFInventoryGetInventoryItemsRequest, Alloc>
{
public:
    using ModelType = PFInventoryGetInventoryItemsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryGetInventoryItemsRequestWrapper() = default;

    PFInventoryGetInventoryItemsRequestWrapper(const PFInventoryGetInventoryItemsRequest& model) :
        ModelWrapper<PFInventoryGetInventoryItemsRequest, Alloc>{ model },
        m_collectionId{ SafeString(model.collectionId) },
        m_continuationToken{ SafeString(model.continuationToken) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_filter{ SafeString(model.filter) }
    {
        SetModelPointers();
    }

    PFInventoryGetInventoryItemsRequestWrapper(const PFInventoryGetInventoryItemsRequestWrapper& src) :
        PFInventoryGetInventoryItemsRequestWrapper{ src.Model() }
    {
    }

    PFInventoryGetInventoryItemsRequestWrapper(PFInventoryGetInventoryItemsRequestWrapper&& src) :
        PFInventoryGetInventoryItemsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryGetInventoryItemsRequestWrapper& operator=(PFInventoryGetInventoryItemsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryGetInventoryItemsRequestWrapper() = default;

    friend void swap(PFInventoryGetInventoryItemsRequestWrapper& lhs, PFInventoryGetInventoryItemsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_collectionId, rhs.m_collectionId);
        swap(lhs.m_continuationToken, rhs.m_continuationToken);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_filter, rhs.m_filter);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCollectionId(String value)
    {
        m_collectionId = std::move(value);
        this->m_model.collectionId =  m_collectionId.empty() ? nullptr : m_collectionId.data();
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
        this->m_model.collectionId = m_collectionId.empty() ? nullptr : m_collectionId.data();
        this->m_model.continuationToken = m_continuationToken.empty() ? nullptr : m_continuationToken.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.filter = m_filter.empty() ? nullptr : m_filter.data();
    }

    String m_collectionId;
    String m_continuationToken;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_filter;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryGetInventoryItemsResponseWrapper : public ModelWrapper<PFInventoryGetInventoryItemsResponse, Alloc>
{
public:
    using ModelType = PFInventoryGetInventoryItemsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryGetInventoryItemsResponseWrapper() = default;

    PFInventoryGetInventoryItemsResponseWrapper(const PFInventoryGetInventoryItemsResponse& model) :
        ModelWrapper<PFInventoryGetInventoryItemsResponse, Alloc>{ model },
        m_continuationToken{ SafeString(model.continuationToken) },
        m_eTag{ SafeString(model.eTag) },
        m_items{ model.items, model.items + model.itemsCount }
    {
        SetModelPointers();
    }

    PFInventoryGetInventoryItemsResponseWrapper(const PFInventoryGetInventoryItemsResponseWrapper& src) :
        PFInventoryGetInventoryItemsResponseWrapper{ src.Model() }
    {
    }

    PFInventoryGetInventoryItemsResponseWrapper(PFInventoryGetInventoryItemsResponseWrapper&& src) :
        PFInventoryGetInventoryItemsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryGetInventoryItemsResponseWrapper& operator=(PFInventoryGetInventoryItemsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryGetInventoryItemsResponseWrapper() = default;

    friend void swap(PFInventoryGetInventoryItemsResponseWrapper& lhs, PFInventoryGetInventoryItemsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_continuationToken, rhs.m_continuationToken);
        swap(lhs.m_eTag, rhs.m_eTag);
        swap(lhs.m_items, rhs.m_items);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetContinuationToken(String value)
    {
        m_continuationToken = std::move(value);
        this->m_model.continuationToken =  m_continuationToken.empty() ? nullptr : m_continuationToken.data();
    }

    void SetETag(String value)
    {
        m_eTag = std::move(value);
        this->m_model.eTag =  m_eTag.empty() ? nullptr : m_eTag.data();
    }

    void SetItems(ModelVector<PFInventoryInventoryItemWrapper<Alloc>, Alloc> value)
    {
        m_items = std::move(value);
        this->m_model.items =  m_items.empty() ? nullptr : m_items.data();
        this->m_model.itemsCount =  static_cast<uint32_t>(m_items.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.continuationToken = m_continuationToken.empty() ? nullptr : m_continuationToken.data();
        this->m_model.eTag = m_eTag.empty() ? nullptr : m_eTag.data();
        this->m_model.items = m_items.empty() ? nullptr : m_items.data();
    }

    String m_continuationToken;
    String m_eTag;
    ModelVector<PFInventoryInventoryItemWrapper<Alloc>, Alloc> m_items;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryGetMicrosoftStoreAccessTokensRequestWrapper : public ModelWrapper<PFInventoryGetMicrosoftStoreAccessTokensRequest, Alloc>
{
public:
    using ModelType = PFInventoryGetMicrosoftStoreAccessTokensRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryGetMicrosoftStoreAccessTokensRequestWrapper() = default;

    PFInventoryGetMicrosoftStoreAccessTokensRequestWrapper(const PFInventoryGetMicrosoftStoreAccessTokensRequest& model) :
        ModelWrapper<PFInventoryGetMicrosoftStoreAccessTokensRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount }
    {
        SetModelPointers();
    }

    PFInventoryGetMicrosoftStoreAccessTokensRequestWrapper(const PFInventoryGetMicrosoftStoreAccessTokensRequestWrapper& src) :
        PFInventoryGetMicrosoftStoreAccessTokensRequestWrapper{ src.Model() }
    {
    }

    PFInventoryGetMicrosoftStoreAccessTokensRequestWrapper(PFInventoryGetMicrosoftStoreAccessTokensRequestWrapper&& src) :
        PFInventoryGetMicrosoftStoreAccessTokensRequestWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryGetMicrosoftStoreAccessTokensRequestWrapper& operator=(PFInventoryGetMicrosoftStoreAccessTokensRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryGetMicrosoftStoreAccessTokensRequestWrapper() = default;

    friend void swap(PFInventoryGetMicrosoftStoreAccessTokensRequestWrapper& lhs, PFInventoryGetMicrosoftStoreAccessTokensRequestWrapper& rhs)
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
class PFInventoryGetMicrosoftStoreAccessTokensResponseWrapper : public ModelWrapper<PFInventoryGetMicrosoftStoreAccessTokensResponse, Alloc>
{
public:
    using ModelType = PFInventoryGetMicrosoftStoreAccessTokensResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryGetMicrosoftStoreAccessTokensResponseWrapper() = default;

    PFInventoryGetMicrosoftStoreAccessTokensResponseWrapper(const PFInventoryGetMicrosoftStoreAccessTokensResponse& model) :
        ModelWrapper<PFInventoryGetMicrosoftStoreAccessTokensResponse, Alloc>{ model },
        m_collectionsAccessToken{ SafeString(model.collectionsAccessToken) }
    {
        SetModelPointers();
    }

    PFInventoryGetMicrosoftStoreAccessTokensResponseWrapper(const PFInventoryGetMicrosoftStoreAccessTokensResponseWrapper& src) :
        PFInventoryGetMicrosoftStoreAccessTokensResponseWrapper{ src.Model() }
    {
    }

    PFInventoryGetMicrosoftStoreAccessTokensResponseWrapper(PFInventoryGetMicrosoftStoreAccessTokensResponseWrapper&& src) :
        PFInventoryGetMicrosoftStoreAccessTokensResponseWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryGetMicrosoftStoreAccessTokensResponseWrapper& operator=(PFInventoryGetMicrosoftStoreAccessTokensResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryGetMicrosoftStoreAccessTokensResponseWrapper() = default;

    friend void swap(PFInventoryGetMicrosoftStoreAccessTokensResponseWrapper& lhs, PFInventoryGetMicrosoftStoreAccessTokensResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_collectionsAccessToken, rhs.m_collectionsAccessToken);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCollectionsAccessToken(String value)
    {
        m_collectionsAccessToken = std::move(value);
        this->m_model.collectionsAccessToken =  m_collectionsAccessToken.empty() ? nullptr : m_collectionsAccessToken.data();
    }

    void SetCollectionsAccessTokenExpirationDate(time_t value)
    {
        this->m_model.collectionsAccessTokenExpirationDate = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.collectionsAccessToken = m_collectionsAccessToken.empty() ? nullptr : m_collectionsAccessToken.data();
    }

    String m_collectionsAccessToken;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryGetTransactionHistoryRequestWrapper : public ModelWrapper<PFInventoryGetTransactionHistoryRequest, Alloc>
{
public:
    using ModelType = PFInventoryGetTransactionHistoryRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryGetTransactionHistoryRequestWrapper() = default;

    PFInventoryGetTransactionHistoryRequestWrapper(const PFInventoryGetTransactionHistoryRequest& model) :
        ModelWrapper<PFInventoryGetTransactionHistoryRequest, Alloc>{ model },
        m_collectionId{ SafeString(model.collectionId) },
        m_continuationToken{ SafeString(model.continuationToken) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_filter{ SafeString(model.filter) }
    {
        SetModelPointers();
    }

    PFInventoryGetTransactionHistoryRequestWrapper(const PFInventoryGetTransactionHistoryRequestWrapper& src) :
        PFInventoryGetTransactionHistoryRequestWrapper{ src.Model() }
    {
    }

    PFInventoryGetTransactionHistoryRequestWrapper(PFInventoryGetTransactionHistoryRequestWrapper&& src) :
        PFInventoryGetTransactionHistoryRequestWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryGetTransactionHistoryRequestWrapper& operator=(PFInventoryGetTransactionHistoryRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryGetTransactionHistoryRequestWrapper() = default;

    friend void swap(PFInventoryGetTransactionHistoryRequestWrapper& lhs, PFInventoryGetTransactionHistoryRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_collectionId, rhs.m_collectionId);
        swap(lhs.m_continuationToken, rhs.m_continuationToken);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_filter, rhs.m_filter);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCollectionId(String value)
    {
        m_collectionId = std::move(value);
        this->m_model.collectionId =  m_collectionId.empty() ? nullptr : m_collectionId.data();
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
        this->m_model.collectionId = m_collectionId.empty() ? nullptr : m_collectionId.data();
        this->m_model.continuationToken = m_continuationToken.empty() ? nullptr : m_continuationToken.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.filter = m_filter.empty() ? nullptr : m_filter.data();
    }

    String m_collectionId;
    String m_continuationToken;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_filter;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryTransactionOperationWrapper : public ModelWrapper<PFInventoryTransactionOperation, Alloc>
{
public:
    using ModelType = PFInventoryTransactionOperation;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryTransactionOperationWrapper() = default;

    PFInventoryTransactionOperationWrapper(const PFInventoryTransactionOperation& model) :
        ModelWrapper<PFInventoryTransactionOperation, Alloc>{ model },
        m_amount{ model.amount ? std::optional<int32_t>{ *model.amount } : std::nullopt },
        m_durationInSeconds{ model.durationInSeconds ? std::optional<double>{ *model.durationInSeconds } : std::nullopt },
        m_itemId{ SafeString(model.itemId) },
        m_itemType{ SafeString(model.itemType) },
        m_stackId{ SafeString(model.stackId) },
        m_type{ SafeString(model.type) }
    {
        SetModelPointers();
    }

    PFInventoryTransactionOperationWrapper(const PFInventoryTransactionOperationWrapper& src) :
        PFInventoryTransactionOperationWrapper{ src.Model() }
    {
    }

    PFInventoryTransactionOperationWrapper(PFInventoryTransactionOperationWrapper&& src) :
        PFInventoryTransactionOperationWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryTransactionOperationWrapper& operator=(PFInventoryTransactionOperationWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryTransactionOperationWrapper() = default;

    friend void swap(PFInventoryTransactionOperationWrapper& lhs, PFInventoryTransactionOperationWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_amount, rhs.m_amount);
        swap(lhs.m_durationInSeconds, rhs.m_durationInSeconds);
        swap(lhs.m_itemId, rhs.m_itemId);
        swap(lhs.m_itemType, rhs.m_itemType);
        swap(lhs.m_stackId, rhs.m_stackId);
        swap(lhs.m_type, rhs.m_type);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAmount(std::optional<int32_t> value)
    {
        m_amount = std::move(value);
        this->m_model.amount = m_amount ? m_amount.operator->() : nullptr;
    }

    void SetDurationInSeconds(std::optional<double> value)
    {
        m_durationInSeconds = std::move(value);
        this->m_model.durationInSeconds = m_durationInSeconds ? m_durationInSeconds.operator->() : nullptr;
    }

    void SetItemId(String value)
    {
        m_itemId = std::move(value);
        this->m_model.itemId =  m_itemId.empty() ? nullptr : m_itemId.data();
    }

    void SetItemType(String value)
    {
        m_itemType = std::move(value);
        this->m_model.itemType =  m_itemType.empty() ? nullptr : m_itemType.data();
    }

    void SetStackId(String value)
    {
        m_stackId = std::move(value);
        this->m_model.stackId =  m_stackId.empty() ? nullptr : m_stackId.data();
    }

    void SetType(String value)
    {
        m_type = std::move(value);
        this->m_model.type =  m_type.empty() ? nullptr : m_type.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.amount = m_amount ? m_amount.operator->() : nullptr;
        this->m_model.durationInSeconds = m_durationInSeconds ? m_durationInSeconds.operator->() : nullptr;
        this->m_model.itemId = m_itemId.empty() ? nullptr : m_itemId.data();
        this->m_model.itemType = m_itemType.empty() ? nullptr : m_itemType.data();
        this->m_model.stackId = m_stackId.empty() ? nullptr : m_stackId.data();
        this->m_model.type = m_type.empty() ? nullptr : m_type.data();
    }

    std::optional<int32_t> m_amount;
    std::optional<double> m_durationInSeconds;
    String m_itemId;
    String m_itemType;
    String m_stackId;
    String m_type;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryTransactionPurchaseDetailsWrapper : public ModelWrapper<PFInventoryTransactionPurchaseDetails, Alloc>
{
public:
    using ModelType = PFInventoryTransactionPurchaseDetails;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryTransactionPurchaseDetailsWrapper() = default;

    PFInventoryTransactionPurchaseDetailsWrapper(const PFInventoryTransactionPurchaseDetails& model) :
        ModelWrapper<PFInventoryTransactionPurchaseDetails, Alloc>{ model },
        m_storeId{ SafeString(model.storeId) }
    {
        SetModelPointers();
    }

    PFInventoryTransactionPurchaseDetailsWrapper(const PFInventoryTransactionPurchaseDetailsWrapper& src) :
        PFInventoryTransactionPurchaseDetailsWrapper{ src.Model() }
    {
    }

    PFInventoryTransactionPurchaseDetailsWrapper(PFInventoryTransactionPurchaseDetailsWrapper&& src) :
        PFInventoryTransactionPurchaseDetailsWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryTransactionPurchaseDetailsWrapper& operator=(PFInventoryTransactionPurchaseDetailsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryTransactionPurchaseDetailsWrapper() = default;

    friend void swap(PFInventoryTransactionPurchaseDetailsWrapper& lhs, PFInventoryTransactionPurchaseDetailsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_storeId, rhs.m_storeId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetStoreId(String value)
    {
        m_storeId = std::move(value);
        this->m_model.storeId =  m_storeId.empty() ? nullptr : m_storeId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.storeId = m_storeId.empty() ? nullptr : m_storeId.data();
    }

    String m_storeId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryTransactionRedeemDetailsWrapper : public ModelWrapper<PFInventoryTransactionRedeemDetails, Alloc>
{
public:
    using ModelType = PFInventoryTransactionRedeemDetails;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryTransactionRedeemDetailsWrapper() = default;

    PFInventoryTransactionRedeemDetailsWrapper(const PFInventoryTransactionRedeemDetails& model) :
        ModelWrapper<PFInventoryTransactionRedeemDetails, Alloc>{ model },
        m_marketplace{ SafeString(model.marketplace) },
        m_marketplaceTransactionId{ SafeString(model.marketplaceTransactionId) },
        m_offerId{ SafeString(model.offerId) }
    {
        SetModelPointers();
    }

    PFInventoryTransactionRedeemDetailsWrapper(const PFInventoryTransactionRedeemDetailsWrapper& src) :
        PFInventoryTransactionRedeemDetailsWrapper{ src.Model() }
    {
    }

    PFInventoryTransactionRedeemDetailsWrapper(PFInventoryTransactionRedeemDetailsWrapper&& src) :
        PFInventoryTransactionRedeemDetailsWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryTransactionRedeemDetailsWrapper& operator=(PFInventoryTransactionRedeemDetailsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryTransactionRedeemDetailsWrapper() = default;

    friend void swap(PFInventoryTransactionRedeemDetailsWrapper& lhs, PFInventoryTransactionRedeemDetailsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_marketplace, rhs.m_marketplace);
        swap(lhs.m_marketplaceTransactionId, rhs.m_marketplaceTransactionId);
        swap(lhs.m_offerId, rhs.m_offerId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetMarketplace(String value)
    {
        m_marketplace = std::move(value);
        this->m_model.marketplace =  m_marketplace.empty() ? nullptr : m_marketplace.data();
    }

    void SetMarketplaceTransactionId(String value)
    {
        m_marketplaceTransactionId = std::move(value);
        this->m_model.marketplaceTransactionId =  m_marketplaceTransactionId.empty() ? nullptr : m_marketplaceTransactionId.data();
    }

    void SetOfferId(String value)
    {
        m_offerId = std::move(value);
        this->m_model.offerId =  m_offerId.empty() ? nullptr : m_offerId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.marketplace = m_marketplace.empty() ? nullptr : m_marketplace.data();
        this->m_model.marketplaceTransactionId = m_marketplaceTransactionId.empty() ? nullptr : m_marketplaceTransactionId.data();
        this->m_model.offerId = m_offerId.empty() ? nullptr : m_offerId.data();
    }

    String m_marketplace;
    String m_marketplaceTransactionId;
    String m_offerId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryTransactionTransferDetailsWrapper : public ModelWrapper<PFInventoryTransactionTransferDetails, Alloc>
{
public:
    using ModelType = PFInventoryTransactionTransferDetails;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryTransactionTransferDetailsWrapper() = default;

    PFInventoryTransactionTransferDetailsWrapper(const PFInventoryTransactionTransferDetails& model) :
        ModelWrapper<PFInventoryTransactionTransferDetails, Alloc>{ model },
        m_givingCollectionId{ SafeString(model.givingCollectionId) },
        m_givingEntity{ model.givingEntity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.givingEntity } : std::nullopt },
        m_receivingCollectionId{ SafeString(model.receivingCollectionId) },
        m_receivingEntity{ model.receivingEntity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.receivingEntity } : std::nullopt },
        m_transferId{ SafeString(model.transferId) }
    {
        SetModelPointers();
    }

    PFInventoryTransactionTransferDetailsWrapper(const PFInventoryTransactionTransferDetailsWrapper& src) :
        PFInventoryTransactionTransferDetailsWrapper{ src.Model() }
    {
    }

    PFInventoryTransactionTransferDetailsWrapper(PFInventoryTransactionTransferDetailsWrapper&& src) :
        PFInventoryTransactionTransferDetailsWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryTransactionTransferDetailsWrapper& operator=(PFInventoryTransactionTransferDetailsWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryTransactionTransferDetailsWrapper() = default;

    friend void swap(PFInventoryTransactionTransferDetailsWrapper& lhs, PFInventoryTransactionTransferDetailsWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_givingCollectionId, rhs.m_givingCollectionId);
        swap(lhs.m_givingEntity, rhs.m_givingEntity);
        swap(lhs.m_receivingCollectionId, rhs.m_receivingCollectionId);
        swap(lhs.m_receivingEntity, rhs.m_receivingEntity);
        swap(lhs.m_transferId, rhs.m_transferId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetGivingCollectionId(String value)
    {
        m_givingCollectionId = std::move(value);
        this->m_model.givingCollectionId =  m_givingCollectionId.empty() ? nullptr : m_givingCollectionId.data();
    }

    void SetGivingEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_givingEntity = std::move(value);
        this->m_model.givingEntity = m_givingEntity ? &m_givingEntity->Model() : nullptr;
    }

    void SetReceivingCollectionId(String value)
    {
        m_receivingCollectionId = std::move(value);
        this->m_model.receivingCollectionId =  m_receivingCollectionId.empty() ? nullptr : m_receivingCollectionId.data();
    }

    void SetReceivingEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_receivingEntity = std::move(value);
        this->m_model.receivingEntity = m_receivingEntity ? &m_receivingEntity->Model() : nullptr;
    }

    void SetTransferId(String value)
    {
        m_transferId = std::move(value);
        this->m_model.transferId =  m_transferId.empty() ? nullptr : m_transferId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.givingCollectionId = m_givingCollectionId.empty() ? nullptr : m_givingCollectionId.data();
        this->m_model.givingEntity = m_givingEntity ?  &m_givingEntity->Model() : nullptr;
        this->m_model.receivingCollectionId = m_receivingCollectionId.empty() ? nullptr : m_receivingCollectionId.data();
        this->m_model.receivingEntity = m_receivingEntity ?  &m_receivingEntity->Model() : nullptr;
        this->m_model.transferId = m_transferId.empty() ? nullptr : m_transferId.data();
    }

    String m_givingCollectionId;
    std::optional<PFEntityKeyWrapper<Alloc>> m_givingEntity;
    String m_receivingCollectionId;
    std::optional<PFEntityKeyWrapper<Alloc>> m_receivingEntity;
    String m_transferId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryTransactionWrapper : public ModelWrapper<PFInventoryTransaction, Alloc>
{
public:
    using ModelType = PFInventoryTransaction;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryTransactionWrapper() = default;

    PFInventoryTransactionWrapper(const PFInventoryTransaction& model) :
        ModelWrapper<PFInventoryTransaction, Alloc>{ model },
        m_apiName{ SafeString(model.apiName) },
        m_itemType{ SafeString(model.itemType) },
        m_operations{ model.operations, model.operations + model.operationsCount },
        m_operationType{ SafeString(model.operationType) },
        m_purchaseDetails{ model.purchaseDetails ? std::optional<PFInventoryTransactionPurchaseDetailsWrapper<Alloc>>{ *model.purchaseDetails } : std::nullopt },
        m_redeemDetails{ model.redeemDetails ? std::optional<PFInventoryTransactionRedeemDetailsWrapper<Alloc>>{ *model.redeemDetails } : std::nullopt },
        m_transactionId{ SafeString(model.transactionId) },
        m_transferDetails{ model.transferDetails ? std::optional<PFInventoryTransactionTransferDetailsWrapper<Alloc>>{ *model.transferDetails } : std::nullopt }
    {
        SetModelPointers();
    }

    PFInventoryTransactionWrapper(const PFInventoryTransactionWrapper& src) :
        PFInventoryTransactionWrapper{ src.Model() }
    {
    }

    PFInventoryTransactionWrapper(PFInventoryTransactionWrapper&& src) :
        PFInventoryTransactionWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryTransactionWrapper& operator=(PFInventoryTransactionWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryTransactionWrapper() = default;

    friend void swap(PFInventoryTransactionWrapper& lhs, PFInventoryTransactionWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_apiName, rhs.m_apiName);
        swap(lhs.m_itemType, rhs.m_itemType);
        swap(lhs.m_operations, rhs.m_operations);
        swap(lhs.m_operationType, rhs.m_operationType);
        swap(lhs.m_purchaseDetails, rhs.m_purchaseDetails);
        swap(lhs.m_redeemDetails, rhs.m_redeemDetails);
        swap(lhs.m_transactionId, rhs.m_transactionId);
        swap(lhs.m_transferDetails, rhs.m_transferDetails);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetApiName(String value)
    {
        m_apiName = std::move(value);
        this->m_model.apiName =  m_apiName.empty() ? nullptr : m_apiName.data();
    }

    void SetItemType(String value)
    {
        m_itemType = std::move(value);
        this->m_model.itemType =  m_itemType.empty() ? nullptr : m_itemType.data();
    }

    void SetOperations(ModelVector<PFInventoryTransactionOperationWrapper<Alloc>, Alloc> value)
    {
        m_operations = std::move(value);
        this->m_model.operations =  m_operations.empty() ? nullptr : m_operations.data();
        this->m_model.operationsCount =  static_cast<uint32_t>(m_operations.size());
    }

    void SetOperationType(String value)
    {
        m_operationType = std::move(value);
        this->m_model.operationType =  m_operationType.empty() ? nullptr : m_operationType.data();
    }

    void SetPurchaseDetails(std::optional<PFInventoryTransactionPurchaseDetailsWrapper<Alloc>> value)
    {
        m_purchaseDetails = std::move(value);
        this->m_model.purchaseDetails = m_purchaseDetails ? &m_purchaseDetails->Model() : nullptr;
    }

    void SetRedeemDetails(std::optional<PFInventoryTransactionRedeemDetailsWrapper<Alloc>> value)
    {
        m_redeemDetails = std::move(value);
        this->m_model.redeemDetails = m_redeemDetails ? &m_redeemDetails->Model() : nullptr;
    }

    void SetTimestamp(time_t value)
    {
        this->m_model.timestamp = value;
    }

    void SetTransactionId(String value)
    {
        m_transactionId = std::move(value);
        this->m_model.transactionId =  m_transactionId.empty() ? nullptr : m_transactionId.data();
    }

    void SetTransferDetails(std::optional<PFInventoryTransactionTransferDetailsWrapper<Alloc>> value)
    {
        m_transferDetails = std::move(value);
        this->m_model.transferDetails = m_transferDetails ? &m_transferDetails->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.apiName = m_apiName.empty() ? nullptr : m_apiName.data();
        this->m_model.itemType = m_itemType.empty() ? nullptr : m_itemType.data();
        this->m_model.operations = m_operations.empty() ? nullptr : m_operations.data();
        this->m_model.operationType = m_operationType.empty() ? nullptr : m_operationType.data();
        this->m_model.purchaseDetails = m_purchaseDetails ?  &m_purchaseDetails->Model() : nullptr;
        this->m_model.redeemDetails = m_redeemDetails ?  &m_redeemDetails->Model() : nullptr;
        this->m_model.transactionId = m_transactionId.empty() ? nullptr : m_transactionId.data();
        this->m_model.transferDetails = m_transferDetails ?  &m_transferDetails->Model() : nullptr;
    }

    String m_apiName;
    String m_itemType;
    ModelVector<PFInventoryTransactionOperationWrapper<Alloc>, Alloc> m_operations;
    String m_operationType;
    std::optional<PFInventoryTransactionPurchaseDetailsWrapper<Alloc>> m_purchaseDetails;
    std::optional<PFInventoryTransactionRedeemDetailsWrapper<Alloc>> m_redeemDetails;
    String m_transactionId;
    std::optional<PFInventoryTransactionTransferDetailsWrapper<Alloc>> m_transferDetails;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryGetTransactionHistoryResponseWrapper : public ModelWrapper<PFInventoryGetTransactionHistoryResponse, Alloc>
{
public:
    using ModelType = PFInventoryGetTransactionHistoryResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryGetTransactionHistoryResponseWrapper() = default;

    PFInventoryGetTransactionHistoryResponseWrapper(const PFInventoryGetTransactionHistoryResponse& model) :
        ModelWrapper<PFInventoryGetTransactionHistoryResponse, Alloc>{ model },
        m_continuationToken{ SafeString(model.continuationToken) },
        m_transactions{ model.transactions, model.transactions + model.transactionsCount }
    {
        SetModelPointers();
    }

    PFInventoryGetTransactionHistoryResponseWrapper(const PFInventoryGetTransactionHistoryResponseWrapper& src) :
        PFInventoryGetTransactionHistoryResponseWrapper{ src.Model() }
    {
    }

    PFInventoryGetTransactionHistoryResponseWrapper(PFInventoryGetTransactionHistoryResponseWrapper&& src) :
        PFInventoryGetTransactionHistoryResponseWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryGetTransactionHistoryResponseWrapper& operator=(PFInventoryGetTransactionHistoryResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryGetTransactionHistoryResponseWrapper() = default;

    friend void swap(PFInventoryGetTransactionHistoryResponseWrapper& lhs, PFInventoryGetTransactionHistoryResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_continuationToken, rhs.m_continuationToken);
        swap(lhs.m_transactions, rhs.m_transactions);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetContinuationToken(String value)
    {
        m_continuationToken = std::move(value);
        this->m_model.continuationToken =  m_continuationToken.empty() ? nullptr : m_continuationToken.data();
    }

    void SetTransactions(ModelVector<PFInventoryTransactionWrapper<Alloc>, Alloc> value)
    {
        m_transactions = std::move(value);
        this->m_model.transactions =  m_transactions.empty() ? nullptr : m_transactions.data();
        this->m_model.transactionsCount =  static_cast<uint32_t>(m_transactions.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.continuationToken = m_continuationToken.empty() ? nullptr : m_continuationToken.data();
        this->m_model.transactions = m_transactions.empty() ? nullptr : m_transactions.data();
    }

    String m_continuationToken;
    ModelVector<PFInventoryTransactionWrapper<Alloc>, Alloc> m_transactions;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryPurchaseInventoryItemsRequestWrapper : public ModelWrapper<PFInventoryPurchaseInventoryItemsRequest, Alloc>
{
public:
    using ModelType = PFInventoryPurchaseInventoryItemsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryPurchaseInventoryItemsRequestWrapper() = default;

    PFInventoryPurchaseInventoryItemsRequestWrapper(const PFInventoryPurchaseInventoryItemsRequest& model) :
        ModelWrapper<PFInventoryPurchaseInventoryItemsRequest, Alloc>{ model },
        m_amount{ model.amount ? std::optional<int32_t>{ *model.amount } : std::nullopt },
        m_collectionId{ SafeString(model.collectionId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_durationInSeconds{ model.durationInSeconds ? std::optional<double>{ *model.durationInSeconds } : std::nullopt },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_eTag{ SafeString(model.eTag) },
        m_idempotencyId{ SafeString(model.idempotencyId) },
        m_item{ model.item ? std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>>{ *model.item } : std::nullopt },
        m_newStackValues{ model.newStackValues ? std::optional<PFInventoryInitialValuesWrapper<Alloc>>{ *model.newStackValues } : std::nullopt },
        m_priceAmounts{ model.priceAmounts, model.priceAmounts + model.priceAmountsCount },
        m_storeId{ SafeString(model.storeId) }
    {
        SetModelPointers();
    }

    PFInventoryPurchaseInventoryItemsRequestWrapper(const PFInventoryPurchaseInventoryItemsRequestWrapper& src) :
        PFInventoryPurchaseInventoryItemsRequestWrapper{ src.Model() }
    {
    }

    PFInventoryPurchaseInventoryItemsRequestWrapper(PFInventoryPurchaseInventoryItemsRequestWrapper&& src) :
        PFInventoryPurchaseInventoryItemsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryPurchaseInventoryItemsRequestWrapper& operator=(PFInventoryPurchaseInventoryItemsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryPurchaseInventoryItemsRequestWrapper() = default;

    friend void swap(PFInventoryPurchaseInventoryItemsRequestWrapper& lhs, PFInventoryPurchaseInventoryItemsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_amount, rhs.m_amount);
        swap(lhs.m_collectionId, rhs.m_collectionId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_durationInSeconds, rhs.m_durationInSeconds);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_eTag, rhs.m_eTag);
        swap(lhs.m_idempotencyId, rhs.m_idempotencyId);
        swap(lhs.m_item, rhs.m_item);
        swap(lhs.m_newStackValues, rhs.m_newStackValues);
        swap(lhs.m_priceAmounts, rhs.m_priceAmounts);
        swap(lhs.m_storeId, rhs.m_storeId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAmount(std::optional<int32_t> value)
    {
        m_amount = std::move(value);
        this->m_model.amount = m_amount ? m_amount.operator->() : nullptr;
    }

    void SetCollectionId(String value)
    {
        m_collectionId = std::move(value);
        this->m_model.collectionId =  m_collectionId.empty() ? nullptr : m_collectionId.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetDeleteEmptyStacks(bool value)
    {
        this->m_model.deleteEmptyStacks = value;
    }

    void SetDurationInSeconds(std::optional<double> value)
    {
        m_durationInSeconds = std::move(value);
        this->m_model.durationInSeconds = m_durationInSeconds ? m_durationInSeconds.operator->() : nullptr;
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

    void SetIdempotencyId(String value)
    {
        m_idempotencyId = std::move(value);
        this->m_model.idempotencyId =  m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
    }

    void SetItem(std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> value)
    {
        m_item = std::move(value);
        this->m_model.item = m_item ? &m_item->Model() : nullptr;
    }

    void SetNewStackValues(std::optional<PFInventoryInitialValuesWrapper<Alloc>> value)
    {
        m_newStackValues = std::move(value);
        this->m_model.newStackValues = m_newStackValues ? &m_newStackValues->Model() : nullptr;
    }

    void SetPriceAmounts(ModelVector<PFInventoryPurchasePriceAmountWrapper<Alloc>, Alloc> value)
    {
        m_priceAmounts = std::move(value);
        this->m_model.priceAmounts =  m_priceAmounts.empty() ? nullptr : m_priceAmounts.data();
        this->m_model.priceAmountsCount =  static_cast<uint32_t>(m_priceAmounts.size());
    }

    void SetStoreId(String value)
    {
        m_storeId = std::move(value);
        this->m_model.storeId =  m_storeId.empty() ? nullptr : m_storeId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.amount = m_amount ? m_amount.operator->() : nullptr;
        this->m_model.collectionId = m_collectionId.empty() ? nullptr : m_collectionId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.durationInSeconds = m_durationInSeconds ? m_durationInSeconds.operator->() : nullptr;
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.eTag = m_eTag.empty() ? nullptr : m_eTag.data();
        this->m_model.idempotencyId = m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
        this->m_model.item = m_item ?  &m_item->Model() : nullptr;
        this->m_model.newStackValues = m_newStackValues ?  &m_newStackValues->Model() : nullptr;
        this->m_model.priceAmounts = m_priceAmounts.empty() ? nullptr : m_priceAmounts.data();
        this->m_model.storeId = m_storeId.empty() ? nullptr : m_storeId.data();
    }

    std::optional<int32_t> m_amount;
    String m_collectionId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<double> m_durationInSeconds;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_eTag;
    String m_idempotencyId;
    std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> m_item;
    std::optional<PFInventoryInitialValuesWrapper<Alloc>> m_newStackValues;
    ModelVector<PFInventoryPurchasePriceAmountWrapper<Alloc>, Alloc> m_priceAmounts;
    String m_storeId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryPurchaseInventoryItemsResponseWrapper : public ModelWrapper<PFInventoryPurchaseInventoryItemsResponse, Alloc>
{
public:
    using ModelType = PFInventoryPurchaseInventoryItemsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryPurchaseInventoryItemsResponseWrapper() = default;

    PFInventoryPurchaseInventoryItemsResponseWrapper(const PFInventoryPurchaseInventoryItemsResponse& model) :
        ModelWrapper<PFInventoryPurchaseInventoryItemsResponse, Alloc>{ model },
        m_eTag{ SafeString(model.eTag) },
        m_idempotencyId{ SafeString(model.idempotencyId) },
        m_transactionIds{ model.transactionIds, model.transactionIds + model.transactionIdsCount }
    {
        SetModelPointers();
    }

    PFInventoryPurchaseInventoryItemsResponseWrapper(const PFInventoryPurchaseInventoryItemsResponseWrapper& src) :
        PFInventoryPurchaseInventoryItemsResponseWrapper{ src.Model() }
    {
    }

    PFInventoryPurchaseInventoryItemsResponseWrapper(PFInventoryPurchaseInventoryItemsResponseWrapper&& src) :
        PFInventoryPurchaseInventoryItemsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryPurchaseInventoryItemsResponseWrapper& operator=(PFInventoryPurchaseInventoryItemsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryPurchaseInventoryItemsResponseWrapper() = default;

    friend void swap(PFInventoryPurchaseInventoryItemsResponseWrapper& lhs, PFInventoryPurchaseInventoryItemsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_eTag, rhs.m_eTag);
        swap(lhs.m_idempotencyId, rhs.m_idempotencyId);
        swap(lhs.m_transactionIds, rhs.m_transactionIds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetETag(String value)
    {
        m_eTag = std::move(value);
        this->m_model.eTag =  m_eTag.empty() ? nullptr : m_eTag.data();
    }

    void SetIdempotencyId(String value)
    {
        m_idempotencyId = std::move(value);
        this->m_model.idempotencyId =  m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
    }

    void SetTransactionIds(CStringVector<Alloc> value)
    {
        m_transactionIds = std::move(value);
        this->m_model.transactionIds =  m_transactionIds.empty() ? nullptr : m_transactionIds.data();
        this->m_model.transactionIdsCount =  static_cast<uint32_t>(m_transactionIds.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.eTag = m_eTag.empty() ? nullptr : m_eTag.data();
        this->m_model.idempotencyId = m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
        this->m_model.transactionIds = m_transactionIds.empty() ? nullptr : m_transactionIds.data();
    }

    String m_eTag;
    String m_idempotencyId;
    CStringVector<Alloc> m_transactionIds;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryRedeemAppleAppStoreInventoryItemsRequestWrapper : public ModelWrapper<PFInventoryRedeemAppleAppStoreInventoryItemsRequest, Alloc>
{
public:
    using ModelType = PFInventoryRedeemAppleAppStoreInventoryItemsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryRedeemAppleAppStoreInventoryItemsRequestWrapper() = default;

    PFInventoryRedeemAppleAppStoreInventoryItemsRequestWrapper(const PFInventoryRedeemAppleAppStoreInventoryItemsRequest& model) :
        ModelWrapper<PFInventoryRedeemAppleAppStoreInventoryItemsRequest, Alloc>{ model },
        m_collectionId{ SafeString(model.collectionId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_receipt{ SafeString(model.receipt) }
    {
        SetModelPointers();
    }

    PFInventoryRedeemAppleAppStoreInventoryItemsRequestWrapper(const PFInventoryRedeemAppleAppStoreInventoryItemsRequestWrapper& src) :
        PFInventoryRedeemAppleAppStoreInventoryItemsRequestWrapper{ src.Model() }
    {
    }

    PFInventoryRedeemAppleAppStoreInventoryItemsRequestWrapper(PFInventoryRedeemAppleAppStoreInventoryItemsRequestWrapper&& src) :
        PFInventoryRedeemAppleAppStoreInventoryItemsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryRedeemAppleAppStoreInventoryItemsRequestWrapper& operator=(PFInventoryRedeemAppleAppStoreInventoryItemsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryRedeemAppleAppStoreInventoryItemsRequestWrapper() = default;

    friend void swap(PFInventoryRedeemAppleAppStoreInventoryItemsRequestWrapper& lhs, PFInventoryRedeemAppleAppStoreInventoryItemsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_collectionId, rhs.m_collectionId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_receipt, rhs.m_receipt);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCollectionId(String value)
    {
        m_collectionId = std::move(value);
        this->m_model.collectionId =  m_collectionId.empty() ? nullptr : m_collectionId.data();
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

    void SetReceipt(String value)
    {
        m_receipt = std::move(value);
        this->m_model.receipt =  m_receipt.empty() ? nullptr : m_receipt.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.collectionId = m_collectionId.empty() ? nullptr : m_collectionId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.receipt = m_receipt.empty() ? nullptr : m_receipt.data();
    }

    String m_collectionId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_receipt;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryRedemptionFailureWrapper : public ModelWrapper<PFInventoryRedemptionFailure, Alloc>
{
public:
    using ModelType = PFInventoryRedemptionFailure;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryRedemptionFailureWrapper() = default;

    PFInventoryRedemptionFailureWrapper(const PFInventoryRedemptionFailure& model) :
        ModelWrapper<PFInventoryRedemptionFailure, Alloc>{ model },
        m_failureCode{ SafeString(model.failureCode) },
        m_failureDetails{ SafeString(model.failureDetails) },
        m_marketplaceTransactionId{ SafeString(model.marketplaceTransactionId) },
        m_offerId{ SafeString(model.offerId) }
    {
        SetModelPointers();
    }

    PFInventoryRedemptionFailureWrapper(const PFInventoryRedemptionFailureWrapper& src) :
        PFInventoryRedemptionFailureWrapper{ src.Model() }
    {
    }

    PFInventoryRedemptionFailureWrapper(PFInventoryRedemptionFailureWrapper&& src) :
        PFInventoryRedemptionFailureWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryRedemptionFailureWrapper& operator=(PFInventoryRedemptionFailureWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryRedemptionFailureWrapper() = default;

    friend void swap(PFInventoryRedemptionFailureWrapper& lhs, PFInventoryRedemptionFailureWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_failureCode, rhs.m_failureCode);
        swap(lhs.m_failureDetails, rhs.m_failureDetails);
        swap(lhs.m_marketplaceTransactionId, rhs.m_marketplaceTransactionId);
        swap(lhs.m_offerId, rhs.m_offerId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFailureCode(String value)
    {
        m_failureCode = std::move(value);
        this->m_model.failureCode =  m_failureCode.empty() ? nullptr : m_failureCode.data();
    }

    void SetFailureDetails(String value)
    {
        m_failureDetails = std::move(value);
        this->m_model.failureDetails =  m_failureDetails.empty() ? nullptr : m_failureDetails.data();
    }

    void SetMarketplaceTransactionId(String value)
    {
        m_marketplaceTransactionId = std::move(value);
        this->m_model.marketplaceTransactionId =  m_marketplaceTransactionId.empty() ? nullptr : m_marketplaceTransactionId.data();
    }

    void SetOfferId(String value)
    {
        m_offerId = std::move(value);
        this->m_model.offerId =  m_offerId.empty() ? nullptr : m_offerId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.failureCode = m_failureCode.empty() ? nullptr : m_failureCode.data();
        this->m_model.failureDetails = m_failureDetails.empty() ? nullptr : m_failureDetails.data();
        this->m_model.marketplaceTransactionId = m_marketplaceTransactionId.empty() ? nullptr : m_marketplaceTransactionId.data();
        this->m_model.offerId = m_offerId.empty() ? nullptr : m_offerId.data();
    }

    String m_failureCode;
    String m_failureDetails;
    String m_marketplaceTransactionId;
    String m_offerId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryRedemptionSuccessWrapper : public ModelWrapper<PFInventoryRedemptionSuccess, Alloc>
{
public:
    using ModelType = PFInventoryRedemptionSuccess;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryRedemptionSuccessWrapper() = default;

    PFInventoryRedemptionSuccessWrapper(const PFInventoryRedemptionSuccess& model) :
        ModelWrapper<PFInventoryRedemptionSuccess, Alloc>{ model },
        m_marketplaceTransactionId{ SafeString(model.marketplaceTransactionId) },
        m_offerId{ SafeString(model.offerId) }
    {
        SetModelPointers();
    }

    PFInventoryRedemptionSuccessWrapper(const PFInventoryRedemptionSuccessWrapper& src) :
        PFInventoryRedemptionSuccessWrapper{ src.Model() }
    {
    }

    PFInventoryRedemptionSuccessWrapper(PFInventoryRedemptionSuccessWrapper&& src) :
        PFInventoryRedemptionSuccessWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryRedemptionSuccessWrapper& operator=(PFInventoryRedemptionSuccessWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryRedemptionSuccessWrapper() = default;

    friend void swap(PFInventoryRedemptionSuccessWrapper& lhs, PFInventoryRedemptionSuccessWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_marketplaceTransactionId, rhs.m_marketplaceTransactionId);
        swap(lhs.m_offerId, rhs.m_offerId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetMarketplaceTransactionId(String value)
    {
        m_marketplaceTransactionId = std::move(value);
        this->m_model.marketplaceTransactionId =  m_marketplaceTransactionId.empty() ? nullptr : m_marketplaceTransactionId.data();
    }

    void SetOfferId(String value)
    {
        m_offerId = std::move(value);
        this->m_model.offerId =  m_offerId.empty() ? nullptr : m_offerId.data();
    }

    void SetSuccessTimestamp(time_t value)
    {
        this->m_model.successTimestamp = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.marketplaceTransactionId = m_marketplaceTransactionId.empty() ? nullptr : m_marketplaceTransactionId.data();
        this->m_model.offerId = m_offerId.empty() ? nullptr : m_offerId.data();
    }

    String m_marketplaceTransactionId;
    String m_offerId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryRedeemAppleAppStoreInventoryItemsResponseWrapper : public ModelWrapper<PFInventoryRedeemAppleAppStoreInventoryItemsResponse, Alloc>
{
public:
    using ModelType = PFInventoryRedeemAppleAppStoreInventoryItemsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryRedeemAppleAppStoreInventoryItemsResponseWrapper() = default;

    PFInventoryRedeemAppleAppStoreInventoryItemsResponseWrapper(const PFInventoryRedeemAppleAppStoreInventoryItemsResponse& model) :
        ModelWrapper<PFInventoryRedeemAppleAppStoreInventoryItemsResponse, Alloc>{ model },
        m_failed{ model.failed, model.failed + model.failedCount },
        m_succeeded{ model.succeeded, model.succeeded + model.succeededCount },
        m_transactionIds{ model.transactionIds, model.transactionIds + model.transactionIdsCount }
    {
        SetModelPointers();
    }

    PFInventoryRedeemAppleAppStoreInventoryItemsResponseWrapper(const PFInventoryRedeemAppleAppStoreInventoryItemsResponseWrapper& src) :
        PFInventoryRedeemAppleAppStoreInventoryItemsResponseWrapper{ src.Model() }
    {
    }

    PFInventoryRedeemAppleAppStoreInventoryItemsResponseWrapper(PFInventoryRedeemAppleAppStoreInventoryItemsResponseWrapper&& src) :
        PFInventoryRedeemAppleAppStoreInventoryItemsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryRedeemAppleAppStoreInventoryItemsResponseWrapper& operator=(PFInventoryRedeemAppleAppStoreInventoryItemsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryRedeemAppleAppStoreInventoryItemsResponseWrapper() = default;

    friend void swap(PFInventoryRedeemAppleAppStoreInventoryItemsResponseWrapper& lhs, PFInventoryRedeemAppleAppStoreInventoryItemsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_failed, rhs.m_failed);
        swap(lhs.m_succeeded, rhs.m_succeeded);
        swap(lhs.m_transactionIds, rhs.m_transactionIds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFailed(ModelVector<PFInventoryRedemptionFailureWrapper<Alloc>, Alloc> value)
    {
        m_failed = std::move(value);
        this->m_model.failed =  m_failed.empty() ? nullptr : m_failed.data();
        this->m_model.failedCount =  static_cast<uint32_t>(m_failed.size());
    }

    void SetSucceeded(ModelVector<PFInventoryRedemptionSuccessWrapper<Alloc>, Alloc> value)
    {
        m_succeeded = std::move(value);
        this->m_model.succeeded =  m_succeeded.empty() ? nullptr : m_succeeded.data();
        this->m_model.succeededCount =  static_cast<uint32_t>(m_succeeded.size());
    }

    void SetTransactionIds(CStringVector<Alloc> value)
    {
        m_transactionIds = std::move(value);
        this->m_model.transactionIds =  m_transactionIds.empty() ? nullptr : m_transactionIds.data();
        this->m_model.transactionIdsCount =  static_cast<uint32_t>(m_transactionIds.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.failed = m_failed.empty() ? nullptr : m_failed.data();
        this->m_model.succeeded = m_succeeded.empty() ? nullptr : m_succeeded.data();
        this->m_model.transactionIds = m_transactionIds.empty() ? nullptr : m_transactionIds.data();
    }

    ModelVector<PFInventoryRedemptionFailureWrapper<Alloc>, Alloc> m_failed;
    ModelVector<PFInventoryRedemptionSuccessWrapper<Alloc>, Alloc> m_succeeded;
    CStringVector<Alloc> m_transactionIds;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryGooglePlayProductPurchaseWrapper : public ModelWrapper<PFInventoryGooglePlayProductPurchase, Alloc>
{
public:
    using ModelType = PFInventoryGooglePlayProductPurchase;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryGooglePlayProductPurchaseWrapper() = default;

    PFInventoryGooglePlayProductPurchaseWrapper(const PFInventoryGooglePlayProductPurchase& model) :
        ModelWrapper<PFInventoryGooglePlayProductPurchase, Alloc>{ model },
        m_productId{ SafeString(model.productId) },
        m_token{ SafeString(model.token) }
    {
        SetModelPointers();
    }

    PFInventoryGooglePlayProductPurchaseWrapper(const PFInventoryGooglePlayProductPurchaseWrapper& src) :
        PFInventoryGooglePlayProductPurchaseWrapper{ src.Model() }
    {
    }

    PFInventoryGooglePlayProductPurchaseWrapper(PFInventoryGooglePlayProductPurchaseWrapper&& src) :
        PFInventoryGooglePlayProductPurchaseWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryGooglePlayProductPurchaseWrapper& operator=(PFInventoryGooglePlayProductPurchaseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryGooglePlayProductPurchaseWrapper() = default;

    friend void swap(PFInventoryGooglePlayProductPurchaseWrapper& lhs, PFInventoryGooglePlayProductPurchaseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_productId, rhs.m_productId);
        swap(lhs.m_token, rhs.m_token);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetProductId(String value)
    {
        m_productId = std::move(value);
        this->m_model.productId =  m_productId.empty() ? nullptr : m_productId.data();
    }

    void SetToken(String value)
    {
        m_token = std::move(value);
        this->m_model.token =  m_token.empty() ? nullptr : m_token.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.productId = m_productId.empty() ? nullptr : m_productId.data();
        this->m_model.token = m_token.empty() ? nullptr : m_token.data();
    }

    String m_productId;
    String m_token;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryRedeemGooglePlayInventoryItemsRequestWrapper : public ModelWrapper<PFInventoryRedeemGooglePlayInventoryItemsRequest, Alloc>
{
public:
    using ModelType = PFInventoryRedeemGooglePlayInventoryItemsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryRedeemGooglePlayInventoryItemsRequestWrapper() = default;

    PFInventoryRedeemGooglePlayInventoryItemsRequestWrapper(const PFInventoryRedeemGooglePlayInventoryItemsRequest& model) :
        ModelWrapper<PFInventoryRedeemGooglePlayInventoryItemsRequest, Alloc>{ model },
        m_collectionId{ SafeString(model.collectionId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_purchases{ model.purchases, model.purchases + model.purchasesCount }
    {
        SetModelPointers();
    }

    PFInventoryRedeemGooglePlayInventoryItemsRequestWrapper(const PFInventoryRedeemGooglePlayInventoryItemsRequestWrapper& src) :
        PFInventoryRedeemGooglePlayInventoryItemsRequestWrapper{ src.Model() }
    {
    }

    PFInventoryRedeemGooglePlayInventoryItemsRequestWrapper(PFInventoryRedeemGooglePlayInventoryItemsRequestWrapper&& src) :
        PFInventoryRedeemGooglePlayInventoryItemsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryRedeemGooglePlayInventoryItemsRequestWrapper& operator=(PFInventoryRedeemGooglePlayInventoryItemsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryRedeemGooglePlayInventoryItemsRequestWrapper() = default;

    friend void swap(PFInventoryRedeemGooglePlayInventoryItemsRequestWrapper& lhs, PFInventoryRedeemGooglePlayInventoryItemsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_collectionId, rhs.m_collectionId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_purchases, rhs.m_purchases);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCollectionId(String value)
    {
        m_collectionId = std::move(value);
        this->m_model.collectionId =  m_collectionId.empty() ? nullptr : m_collectionId.data();
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

    void SetPurchases(ModelVector<PFInventoryGooglePlayProductPurchaseWrapper<Alloc>, Alloc> value)
    {
        m_purchases = std::move(value);
        this->m_model.purchases =  m_purchases.empty() ? nullptr : m_purchases.data();
        this->m_model.purchasesCount =  static_cast<uint32_t>(m_purchases.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.collectionId = m_collectionId.empty() ? nullptr : m_collectionId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.purchases = m_purchases.empty() ? nullptr : m_purchases.data();
    }

    String m_collectionId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    ModelVector<PFInventoryGooglePlayProductPurchaseWrapper<Alloc>, Alloc> m_purchases;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryRedeemGooglePlayInventoryItemsResponseWrapper : public ModelWrapper<PFInventoryRedeemGooglePlayInventoryItemsResponse, Alloc>
{
public:
    using ModelType = PFInventoryRedeemGooglePlayInventoryItemsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryRedeemGooglePlayInventoryItemsResponseWrapper() = default;

    PFInventoryRedeemGooglePlayInventoryItemsResponseWrapper(const PFInventoryRedeemGooglePlayInventoryItemsResponse& model) :
        ModelWrapper<PFInventoryRedeemGooglePlayInventoryItemsResponse, Alloc>{ model },
        m_failed{ model.failed, model.failed + model.failedCount },
        m_succeeded{ model.succeeded, model.succeeded + model.succeededCount },
        m_transactionIds{ model.transactionIds, model.transactionIds + model.transactionIdsCount }
    {
        SetModelPointers();
    }

    PFInventoryRedeemGooglePlayInventoryItemsResponseWrapper(const PFInventoryRedeemGooglePlayInventoryItemsResponseWrapper& src) :
        PFInventoryRedeemGooglePlayInventoryItemsResponseWrapper{ src.Model() }
    {
    }

    PFInventoryRedeemGooglePlayInventoryItemsResponseWrapper(PFInventoryRedeemGooglePlayInventoryItemsResponseWrapper&& src) :
        PFInventoryRedeemGooglePlayInventoryItemsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryRedeemGooglePlayInventoryItemsResponseWrapper& operator=(PFInventoryRedeemGooglePlayInventoryItemsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryRedeemGooglePlayInventoryItemsResponseWrapper() = default;

    friend void swap(PFInventoryRedeemGooglePlayInventoryItemsResponseWrapper& lhs, PFInventoryRedeemGooglePlayInventoryItemsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_failed, rhs.m_failed);
        swap(lhs.m_succeeded, rhs.m_succeeded);
        swap(lhs.m_transactionIds, rhs.m_transactionIds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFailed(ModelVector<PFInventoryRedemptionFailureWrapper<Alloc>, Alloc> value)
    {
        m_failed = std::move(value);
        this->m_model.failed =  m_failed.empty() ? nullptr : m_failed.data();
        this->m_model.failedCount =  static_cast<uint32_t>(m_failed.size());
    }

    void SetSucceeded(ModelVector<PFInventoryRedemptionSuccessWrapper<Alloc>, Alloc> value)
    {
        m_succeeded = std::move(value);
        this->m_model.succeeded =  m_succeeded.empty() ? nullptr : m_succeeded.data();
        this->m_model.succeededCount =  static_cast<uint32_t>(m_succeeded.size());
    }

    void SetTransactionIds(CStringVector<Alloc> value)
    {
        m_transactionIds = std::move(value);
        this->m_model.transactionIds =  m_transactionIds.empty() ? nullptr : m_transactionIds.data();
        this->m_model.transactionIdsCount =  static_cast<uint32_t>(m_transactionIds.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.failed = m_failed.empty() ? nullptr : m_failed.data();
        this->m_model.succeeded = m_succeeded.empty() ? nullptr : m_succeeded.data();
        this->m_model.transactionIds = m_transactionIds.empty() ? nullptr : m_transactionIds.data();
    }

    ModelVector<PFInventoryRedemptionFailureWrapper<Alloc>, Alloc> m_failed;
    ModelVector<PFInventoryRedemptionSuccessWrapper<Alloc>, Alloc> m_succeeded;
    CStringVector<Alloc> m_transactionIds;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryRedeemMicrosoftStoreInventoryItemsRequestWrapper : public ModelWrapper<PFInventoryRedeemMicrosoftStoreInventoryItemsRequest, Alloc>
{
public:
    using ModelType = PFInventoryRedeemMicrosoftStoreInventoryItemsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

#if HC_PLATFORM == HC_PLATFORM_GDK
    PFInventoryRedeemMicrosoftStoreInventoryItemsRequestWrapper(XUser user)
        : m_user{ std::move(user) }
    {
        SetModelPointers();
    }
#else
    PFInventoryRedeemMicrosoftStoreInventoryItemsRequestWrapper() = default;
#endif

    PFInventoryRedeemMicrosoftStoreInventoryItemsRequestWrapper(const PFInventoryRedeemMicrosoftStoreInventoryItemsRequest& model) :
        ModelWrapper<PFInventoryRedeemMicrosoftStoreInventoryItemsRequest, Alloc>{ model },
        m_collectionId{ SafeString(model.collectionId) },
        m_collectionsIdKey{ SafeString(model.collectionsIdKey) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
#if HC_PLATFORM == HC_PLATFORM_GDK
        m_user{ XUser::Duplicate(model.user) }
#else
        m_xboxToken{ SafeString(model.xboxToken) }
#endif
    {
        SetModelPointers();
    }

    PFInventoryRedeemMicrosoftStoreInventoryItemsRequestWrapper(const PFInventoryRedeemMicrosoftStoreInventoryItemsRequestWrapper& src) :
        PFInventoryRedeemMicrosoftStoreInventoryItemsRequestWrapper{ src.Model() }
    {
    }

    PFInventoryRedeemMicrosoftStoreInventoryItemsRequestWrapper(PFInventoryRedeemMicrosoftStoreInventoryItemsRequestWrapper&& src) :
#if HC_PLATFORM == HC_PLATFORM_GDK
        PFInventoryRedeemMicrosoftStoreInventoryItemsRequestWrapper{ XUser::Wrap(nullptr) }
#else
        PFInventoryRedeemMicrosoftStoreInventoryItemsRequestWrapper{}
#endif
    {
        swap(*this, src);
    }

    PFInventoryRedeemMicrosoftStoreInventoryItemsRequestWrapper& operator=(PFInventoryRedeemMicrosoftStoreInventoryItemsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryRedeemMicrosoftStoreInventoryItemsRequestWrapper() = default;

    friend void swap(PFInventoryRedeemMicrosoftStoreInventoryItemsRequestWrapper& lhs, PFInventoryRedeemMicrosoftStoreInventoryItemsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_collectionId, rhs.m_collectionId);
        swap(lhs.m_collectionsIdKey, rhs.m_collectionsIdKey);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
#if HC_PLATFORM == HC_PLATFORM_GDK
        swap(lhs.m_user, rhs.m_user);
#endif
        swap(lhs.m_xboxToken, rhs.m_xboxToken);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCollectionId(String value)
    {
        m_collectionId = std::move(value);
        this->m_model.collectionId =  m_collectionId.empty() ? nullptr : m_collectionId.data();
    }

    void SetCollectionsIdKey(String value)
    {
        m_collectionsIdKey = std::move(value);
        this->m_model.collectionsIdKey =  m_collectionsIdKey.empty() ? nullptr : m_collectionsIdKey.data();
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

#if HC_PLATFORM == HC_PLATFORM_GDK
    void SetUser(XUser value)
    {
        m_user = std::move(value);
        this->m_model.user = m_user.Handle();
    }
#else
    void SetXboxToken(String value)
    {
        m_xboxToken = std::move(value);
        this->m_model.xboxToken =  m_xboxToken.empty() ? nullptr : m_xboxToken.data();
    }
#endif

private:
    void SetModelPointers()
    {
        this->m_model.collectionId = m_collectionId.empty() ? nullptr : m_collectionId.data();
        this->m_model.collectionsIdKey = m_collectionsIdKey.empty() ? nullptr : m_collectionsIdKey.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
#if HC_PLATFORM == HC_PLATFORM_GDK
        this->m_model.user = m_user.Handle();
#else
        this->m_model.xboxToken = m_xboxToken.empty() ? nullptr : m_xboxToken.data();
#endif
    }

    String m_collectionId;
    String m_collectionsIdKey;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
#if HC_PLATFORM == HC_PLATFORM_GDK
    XUser m_user;
#endif
    String m_xboxToken;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryRedeemMicrosoftStoreInventoryItemsResponseWrapper : public ModelWrapper<PFInventoryRedeemMicrosoftStoreInventoryItemsResponse, Alloc>
{
public:
    using ModelType = PFInventoryRedeemMicrosoftStoreInventoryItemsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryRedeemMicrosoftStoreInventoryItemsResponseWrapper() = default;

    PFInventoryRedeemMicrosoftStoreInventoryItemsResponseWrapper(const PFInventoryRedeemMicrosoftStoreInventoryItemsResponse& model) :
        ModelWrapper<PFInventoryRedeemMicrosoftStoreInventoryItemsResponse, Alloc>{ model },
        m_failed{ model.failed, model.failed + model.failedCount },
        m_succeeded{ model.succeeded, model.succeeded + model.succeededCount },
        m_transactionIds{ model.transactionIds, model.transactionIds + model.transactionIdsCount }
    {
        SetModelPointers();
    }

    PFInventoryRedeemMicrosoftStoreInventoryItemsResponseWrapper(const PFInventoryRedeemMicrosoftStoreInventoryItemsResponseWrapper& src) :
        PFInventoryRedeemMicrosoftStoreInventoryItemsResponseWrapper{ src.Model() }
    {
    }

    PFInventoryRedeemMicrosoftStoreInventoryItemsResponseWrapper(PFInventoryRedeemMicrosoftStoreInventoryItemsResponseWrapper&& src) :
        PFInventoryRedeemMicrosoftStoreInventoryItemsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryRedeemMicrosoftStoreInventoryItemsResponseWrapper& operator=(PFInventoryRedeemMicrosoftStoreInventoryItemsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryRedeemMicrosoftStoreInventoryItemsResponseWrapper() = default;

    friend void swap(PFInventoryRedeemMicrosoftStoreInventoryItemsResponseWrapper& lhs, PFInventoryRedeemMicrosoftStoreInventoryItemsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_failed, rhs.m_failed);
        swap(lhs.m_succeeded, rhs.m_succeeded);
        swap(lhs.m_transactionIds, rhs.m_transactionIds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFailed(ModelVector<PFInventoryRedemptionFailureWrapper<Alloc>, Alloc> value)
    {
        m_failed = std::move(value);
        this->m_model.failed =  m_failed.empty() ? nullptr : m_failed.data();
        this->m_model.failedCount =  static_cast<uint32_t>(m_failed.size());
    }

    void SetSucceeded(ModelVector<PFInventoryRedemptionSuccessWrapper<Alloc>, Alloc> value)
    {
        m_succeeded = std::move(value);
        this->m_model.succeeded =  m_succeeded.empty() ? nullptr : m_succeeded.data();
        this->m_model.succeededCount =  static_cast<uint32_t>(m_succeeded.size());
    }

    void SetTransactionIds(CStringVector<Alloc> value)
    {
        m_transactionIds = std::move(value);
        this->m_model.transactionIds =  m_transactionIds.empty() ? nullptr : m_transactionIds.data();
        this->m_model.transactionIdsCount =  static_cast<uint32_t>(m_transactionIds.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.failed = m_failed.empty() ? nullptr : m_failed.data();
        this->m_model.succeeded = m_succeeded.empty() ? nullptr : m_succeeded.data();
        this->m_model.transactionIds = m_transactionIds.empty() ? nullptr : m_transactionIds.data();
    }

    ModelVector<PFInventoryRedemptionFailureWrapper<Alloc>, Alloc> m_failed;
    ModelVector<PFInventoryRedemptionSuccessWrapper<Alloc>, Alloc> m_succeeded;
    CStringVector<Alloc> m_transactionIds;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryRedeemNintendoEShopInventoryItemsRequestWrapper : public ModelWrapper<PFInventoryRedeemNintendoEShopInventoryItemsRequest, Alloc>
{
public:
    using ModelType = PFInventoryRedeemNintendoEShopInventoryItemsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryRedeemNintendoEShopInventoryItemsRequestWrapper() = default;

    PFInventoryRedeemNintendoEShopInventoryItemsRequestWrapper(const PFInventoryRedeemNintendoEShopInventoryItemsRequest& model) :
        ModelWrapper<PFInventoryRedeemNintendoEShopInventoryItemsRequest, Alloc>{ model },
        m_collectionId{ SafeString(model.collectionId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_nintendoServiceAccountIdToken{ SafeString(model.nintendoServiceAccountIdToken) }
    {
        SetModelPointers();
    }

    PFInventoryRedeemNintendoEShopInventoryItemsRequestWrapper(const PFInventoryRedeemNintendoEShopInventoryItemsRequestWrapper& src) :
        PFInventoryRedeemNintendoEShopInventoryItemsRequestWrapper{ src.Model() }
    {
    }

    PFInventoryRedeemNintendoEShopInventoryItemsRequestWrapper(PFInventoryRedeemNintendoEShopInventoryItemsRequestWrapper&& src) :
        PFInventoryRedeemNintendoEShopInventoryItemsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryRedeemNintendoEShopInventoryItemsRequestWrapper& operator=(PFInventoryRedeemNintendoEShopInventoryItemsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryRedeemNintendoEShopInventoryItemsRequestWrapper() = default;

    friend void swap(PFInventoryRedeemNintendoEShopInventoryItemsRequestWrapper& lhs, PFInventoryRedeemNintendoEShopInventoryItemsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_collectionId, rhs.m_collectionId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_nintendoServiceAccountIdToken, rhs.m_nintendoServiceAccountIdToken);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCollectionId(String value)
    {
        m_collectionId = std::move(value);
        this->m_model.collectionId =  m_collectionId.empty() ? nullptr : m_collectionId.data();
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

    void SetNintendoServiceAccountIdToken(String value)
    {
        m_nintendoServiceAccountIdToken = std::move(value);
        this->m_model.nintendoServiceAccountIdToken =  m_nintendoServiceAccountIdToken.empty() ? nullptr : m_nintendoServiceAccountIdToken.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.collectionId = m_collectionId.empty() ? nullptr : m_collectionId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.nintendoServiceAccountIdToken = m_nintendoServiceAccountIdToken.empty() ? nullptr : m_nintendoServiceAccountIdToken.data();
    }

    String m_collectionId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_nintendoServiceAccountIdToken;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryRedeemNintendoEShopInventoryItemsResponseWrapper : public ModelWrapper<PFInventoryRedeemNintendoEShopInventoryItemsResponse, Alloc>
{
public:
    using ModelType = PFInventoryRedeemNintendoEShopInventoryItemsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryRedeemNintendoEShopInventoryItemsResponseWrapper() = default;

    PFInventoryRedeemNintendoEShopInventoryItemsResponseWrapper(const PFInventoryRedeemNintendoEShopInventoryItemsResponse& model) :
        ModelWrapper<PFInventoryRedeemNintendoEShopInventoryItemsResponse, Alloc>{ model },
        m_failed{ model.failed, model.failed + model.failedCount },
        m_succeeded{ model.succeeded, model.succeeded + model.succeededCount },
        m_transactionIds{ model.transactionIds, model.transactionIds + model.transactionIdsCount }
    {
        SetModelPointers();
    }

    PFInventoryRedeemNintendoEShopInventoryItemsResponseWrapper(const PFInventoryRedeemNintendoEShopInventoryItemsResponseWrapper& src) :
        PFInventoryRedeemNintendoEShopInventoryItemsResponseWrapper{ src.Model() }
    {
    }

    PFInventoryRedeemNintendoEShopInventoryItemsResponseWrapper(PFInventoryRedeemNintendoEShopInventoryItemsResponseWrapper&& src) :
        PFInventoryRedeemNintendoEShopInventoryItemsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryRedeemNintendoEShopInventoryItemsResponseWrapper& operator=(PFInventoryRedeemNintendoEShopInventoryItemsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryRedeemNintendoEShopInventoryItemsResponseWrapper() = default;

    friend void swap(PFInventoryRedeemNintendoEShopInventoryItemsResponseWrapper& lhs, PFInventoryRedeemNintendoEShopInventoryItemsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_failed, rhs.m_failed);
        swap(lhs.m_succeeded, rhs.m_succeeded);
        swap(lhs.m_transactionIds, rhs.m_transactionIds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFailed(ModelVector<PFInventoryRedemptionFailureWrapper<Alloc>, Alloc> value)
    {
        m_failed = std::move(value);
        this->m_model.failed =  m_failed.empty() ? nullptr : m_failed.data();
        this->m_model.failedCount =  static_cast<uint32_t>(m_failed.size());
    }

    void SetSucceeded(ModelVector<PFInventoryRedemptionSuccessWrapper<Alloc>, Alloc> value)
    {
        m_succeeded = std::move(value);
        this->m_model.succeeded =  m_succeeded.empty() ? nullptr : m_succeeded.data();
        this->m_model.succeededCount =  static_cast<uint32_t>(m_succeeded.size());
    }

    void SetTransactionIds(CStringVector<Alloc> value)
    {
        m_transactionIds = std::move(value);
        this->m_model.transactionIds =  m_transactionIds.empty() ? nullptr : m_transactionIds.data();
        this->m_model.transactionIdsCount =  static_cast<uint32_t>(m_transactionIds.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.failed = m_failed.empty() ? nullptr : m_failed.data();
        this->m_model.succeeded = m_succeeded.empty() ? nullptr : m_succeeded.data();
        this->m_model.transactionIds = m_transactionIds.empty() ? nullptr : m_transactionIds.data();
    }

    ModelVector<PFInventoryRedemptionFailureWrapper<Alloc>, Alloc> m_failed;
    ModelVector<PFInventoryRedemptionSuccessWrapper<Alloc>, Alloc> m_succeeded;
    CStringVector<Alloc> m_transactionIds;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryRedeemPlayStationStoreInventoryItemsRequestWrapper : public ModelWrapper<PFInventoryRedeemPlayStationStoreInventoryItemsRequest, Alloc>
{
public:
    using ModelType = PFInventoryRedeemPlayStationStoreInventoryItemsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryRedeemPlayStationStoreInventoryItemsRequestWrapper() = default;

    PFInventoryRedeemPlayStationStoreInventoryItemsRequestWrapper(const PFInventoryRedeemPlayStationStoreInventoryItemsRequest& model) :
        ModelWrapper<PFInventoryRedeemPlayStationStoreInventoryItemsRequest, Alloc>{ model },
        m_authorizationCode{ SafeString(model.authorizationCode) },
        m_collectionId{ SafeString(model.collectionId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_redirectUri{ SafeString(model.redirectUri) },
        m_serviceLabel{ SafeString(model.serviceLabel) }
    {
        SetModelPointers();
    }

    PFInventoryRedeemPlayStationStoreInventoryItemsRequestWrapper(const PFInventoryRedeemPlayStationStoreInventoryItemsRequestWrapper& src) :
        PFInventoryRedeemPlayStationStoreInventoryItemsRequestWrapper{ src.Model() }
    {
    }

    PFInventoryRedeemPlayStationStoreInventoryItemsRequestWrapper(PFInventoryRedeemPlayStationStoreInventoryItemsRequestWrapper&& src) :
        PFInventoryRedeemPlayStationStoreInventoryItemsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryRedeemPlayStationStoreInventoryItemsRequestWrapper& operator=(PFInventoryRedeemPlayStationStoreInventoryItemsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryRedeemPlayStationStoreInventoryItemsRequestWrapper() = default;

    friend void swap(PFInventoryRedeemPlayStationStoreInventoryItemsRequestWrapper& lhs, PFInventoryRedeemPlayStationStoreInventoryItemsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_authorizationCode, rhs.m_authorizationCode);
        swap(lhs.m_collectionId, rhs.m_collectionId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_redirectUri, rhs.m_redirectUri);
        swap(lhs.m_serviceLabel, rhs.m_serviceLabel);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAuthorizationCode(String value)
    {
        m_authorizationCode = std::move(value);
        this->m_model.authorizationCode =  m_authorizationCode.empty() ? nullptr : m_authorizationCode.data();
    }

    void SetCollectionId(String value)
    {
        m_collectionId = std::move(value);
        this->m_model.collectionId =  m_collectionId.empty() ? nullptr : m_collectionId.data();
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

    void SetRedirectUri(String value)
    {
        m_redirectUri = std::move(value);
        this->m_model.redirectUri =  m_redirectUri.empty() ? nullptr : m_redirectUri.data();
    }

    void SetServiceLabel(String value)
    {
        m_serviceLabel = std::move(value);
        this->m_model.serviceLabel =  m_serviceLabel.empty() ? nullptr : m_serviceLabel.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.authorizationCode = m_authorizationCode.empty() ? nullptr : m_authorizationCode.data();
        this->m_model.collectionId = m_collectionId.empty() ? nullptr : m_collectionId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.redirectUri = m_redirectUri.empty() ? nullptr : m_redirectUri.data();
        this->m_model.serviceLabel = m_serviceLabel.empty() ? nullptr : m_serviceLabel.data();
    }

    String m_authorizationCode;
    String m_collectionId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_redirectUri;
    String m_serviceLabel;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryRedeemPlayStationStoreInventoryItemsResponseWrapper : public ModelWrapper<PFInventoryRedeemPlayStationStoreInventoryItemsResponse, Alloc>
{
public:
    using ModelType = PFInventoryRedeemPlayStationStoreInventoryItemsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryRedeemPlayStationStoreInventoryItemsResponseWrapper() = default;

    PFInventoryRedeemPlayStationStoreInventoryItemsResponseWrapper(const PFInventoryRedeemPlayStationStoreInventoryItemsResponse& model) :
        ModelWrapper<PFInventoryRedeemPlayStationStoreInventoryItemsResponse, Alloc>{ model },
        m_failed{ model.failed, model.failed + model.failedCount },
        m_succeeded{ model.succeeded, model.succeeded + model.succeededCount },
        m_transactionIds{ model.transactionIds, model.transactionIds + model.transactionIdsCount }
    {
        SetModelPointers();
    }

    PFInventoryRedeemPlayStationStoreInventoryItemsResponseWrapper(const PFInventoryRedeemPlayStationStoreInventoryItemsResponseWrapper& src) :
        PFInventoryRedeemPlayStationStoreInventoryItemsResponseWrapper{ src.Model() }
    {
    }

    PFInventoryRedeemPlayStationStoreInventoryItemsResponseWrapper(PFInventoryRedeemPlayStationStoreInventoryItemsResponseWrapper&& src) :
        PFInventoryRedeemPlayStationStoreInventoryItemsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryRedeemPlayStationStoreInventoryItemsResponseWrapper& operator=(PFInventoryRedeemPlayStationStoreInventoryItemsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryRedeemPlayStationStoreInventoryItemsResponseWrapper() = default;

    friend void swap(PFInventoryRedeemPlayStationStoreInventoryItemsResponseWrapper& lhs, PFInventoryRedeemPlayStationStoreInventoryItemsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_failed, rhs.m_failed);
        swap(lhs.m_succeeded, rhs.m_succeeded);
        swap(lhs.m_transactionIds, rhs.m_transactionIds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFailed(ModelVector<PFInventoryRedemptionFailureWrapper<Alloc>, Alloc> value)
    {
        m_failed = std::move(value);
        this->m_model.failed =  m_failed.empty() ? nullptr : m_failed.data();
        this->m_model.failedCount =  static_cast<uint32_t>(m_failed.size());
    }

    void SetSucceeded(ModelVector<PFInventoryRedemptionSuccessWrapper<Alloc>, Alloc> value)
    {
        m_succeeded = std::move(value);
        this->m_model.succeeded =  m_succeeded.empty() ? nullptr : m_succeeded.data();
        this->m_model.succeededCount =  static_cast<uint32_t>(m_succeeded.size());
    }

    void SetTransactionIds(CStringVector<Alloc> value)
    {
        m_transactionIds = std::move(value);
        this->m_model.transactionIds =  m_transactionIds.empty() ? nullptr : m_transactionIds.data();
        this->m_model.transactionIdsCount =  static_cast<uint32_t>(m_transactionIds.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.failed = m_failed.empty() ? nullptr : m_failed.data();
        this->m_model.succeeded = m_succeeded.empty() ? nullptr : m_succeeded.data();
        this->m_model.transactionIds = m_transactionIds.empty() ? nullptr : m_transactionIds.data();
    }

    ModelVector<PFInventoryRedemptionFailureWrapper<Alloc>, Alloc> m_failed;
    ModelVector<PFInventoryRedemptionSuccessWrapper<Alloc>, Alloc> m_succeeded;
    CStringVector<Alloc> m_transactionIds;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryRedeemSteamInventoryItemsRequestWrapper : public ModelWrapper<PFInventoryRedeemSteamInventoryItemsRequest, Alloc>
{
public:
    using ModelType = PFInventoryRedeemSteamInventoryItemsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryRedeemSteamInventoryItemsRequestWrapper() = default;

    PFInventoryRedeemSteamInventoryItemsRequestWrapper(const PFInventoryRedeemSteamInventoryItemsRequest& model) :
        ModelWrapper<PFInventoryRedeemSteamInventoryItemsRequest, Alloc>{ model },
        m_collectionId{ SafeString(model.collectionId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt }
    {
        SetModelPointers();
    }

    PFInventoryRedeemSteamInventoryItemsRequestWrapper(const PFInventoryRedeemSteamInventoryItemsRequestWrapper& src) :
        PFInventoryRedeemSteamInventoryItemsRequestWrapper{ src.Model() }
    {
    }

    PFInventoryRedeemSteamInventoryItemsRequestWrapper(PFInventoryRedeemSteamInventoryItemsRequestWrapper&& src) :
        PFInventoryRedeemSteamInventoryItemsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryRedeemSteamInventoryItemsRequestWrapper& operator=(PFInventoryRedeemSteamInventoryItemsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryRedeemSteamInventoryItemsRequestWrapper() = default;

    friend void swap(PFInventoryRedeemSteamInventoryItemsRequestWrapper& lhs, PFInventoryRedeemSteamInventoryItemsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_collectionId, rhs.m_collectionId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCollectionId(String value)
    {
        m_collectionId = std::move(value);
        this->m_model.collectionId =  m_collectionId.empty() ? nullptr : m_collectionId.data();
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
        this->m_model.collectionId = m_collectionId.empty() ? nullptr : m_collectionId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
    }

    String m_collectionId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryRedeemSteamInventoryItemsResponseWrapper : public ModelWrapper<PFInventoryRedeemSteamInventoryItemsResponse, Alloc>
{
public:
    using ModelType = PFInventoryRedeemSteamInventoryItemsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryRedeemSteamInventoryItemsResponseWrapper() = default;

    PFInventoryRedeemSteamInventoryItemsResponseWrapper(const PFInventoryRedeemSteamInventoryItemsResponse& model) :
        ModelWrapper<PFInventoryRedeemSteamInventoryItemsResponse, Alloc>{ model },
        m_failed{ model.failed, model.failed + model.failedCount },
        m_succeeded{ model.succeeded, model.succeeded + model.succeededCount },
        m_transactionIds{ model.transactionIds, model.transactionIds + model.transactionIdsCount }
    {
        SetModelPointers();
    }

    PFInventoryRedeemSteamInventoryItemsResponseWrapper(const PFInventoryRedeemSteamInventoryItemsResponseWrapper& src) :
        PFInventoryRedeemSteamInventoryItemsResponseWrapper{ src.Model() }
    {
    }

    PFInventoryRedeemSteamInventoryItemsResponseWrapper(PFInventoryRedeemSteamInventoryItemsResponseWrapper&& src) :
        PFInventoryRedeemSteamInventoryItemsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryRedeemSteamInventoryItemsResponseWrapper& operator=(PFInventoryRedeemSteamInventoryItemsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryRedeemSteamInventoryItemsResponseWrapper() = default;

    friend void swap(PFInventoryRedeemSteamInventoryItemsResponseWrapper& lhs, PFInventoryRedeemSteamInventoryItemsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_failed, rhs.m_failed);
        swap(lhs.m_succeeded, rhs.m_succeeded);
        swap(lhs.m_transactionIds, rhs.m_transactionIds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetFailed(ModelVector<PFInventoryRedemptionFailureWrapper<Alloc>, Alloc> value)
    {
        m_failed = std::move(value);
        this->m_model.failed =  m_failed.empty() ? nullptr : m_failed.data();
        this->m_model.failedCount =  static_cast<uint32_t>(m_failed.size());
    }

    void SetSucceeded(ModelVector<PFInventoryRedemptionSuccessWrapper<Alloc>, Alloc> value)
    {
        m_succeeded = std::move(value);
        this->m_model.succeeded =  m_succeeded.empty() ? nullptr : m_succeeded.data();
        this->m_model.succeededCount =  static_cast<uint32_t>(m_succeeded.size());
    }

    void SetTransactionIds(CStringVector<Alloc> value)
    {
        m_transactionIds = std::move(value);
        this->m_model.transactionIds =  m_transactionIds.empty() ? nullptr : m_transactionIds.data();
        this->m_model.transactionIdsCount =  static_cast<uint32_t>(m_transactionIds.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.failed = m_failed.empty() ? nullptr : m_failed.data();
        this->m_model.succeeded = m_succeeded.empty() ? nullptr : m_succeeded.data();
        this->m_model.transactionIds = m_transactionIds.empty() ? nullptr : m_transactionIds.data();
    }

    ModelVector<PFInventoryRedemptionFailureWrapper<Alloc>, Alloc> m_failed;
    ModelVector<PFInventoryRedemptionSuccessWrapper<Alloc>, Alloc> m_succeeded;
    CStringVector<Alloc> m_transactionIds;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventorySubtractInventoryItemsRequestWrapper : public ModelWrapper<PFInventorySubtractInventoryItemsRequest, Alloc>
{
public:
    using ModelType = PFInventorySubtractInventoryItemsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventorySubtractInventoryItemsRequestWrapper() = default;

    PFInventorySubtractInventoryItemsRequestWrapper(const PFInventorySubtractInventoryItemsRequest& model) :
        ModelWrapper<PFInventorySubtractInventoryItemsRequest, Alloc>{ model },
        m_amount{ model.amount ? std::optional<int32_t>{ *model.amount } : std::nullopt },
        m_collectionId{ SafeString(model.collectionId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_durationInSeconds{ model.durationInSeconds ? std::optional<double>{ *model.durationInSeconds } : std::nullopt },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_eTag{ SafeString(model.eTag) },
        m_idempotencyId{ SafeString(model.idempotencyId) },
        m_item{ model.item ? std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>>{ *model.item } : std::nullopt }
    {
        SetModelPointers();
    }

    PFInventorySubtractInventoryItemsRequestWrapper(const PFInventorySubtractInventoryItemsRequestWrapper& src) :
        PFInventorySubtractInventoryItemsRequestWrapper{ src.Model() }
    {
    }

    PFInventorySubtractInventoryItemsRequestWrapper(PFInventorySubtractInventoryItemsRequestWrapper&& src) :
        PFInventorySubtractInventoryItemsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFInventorySubtractInventoryItemsRequestWrapper& operator=(PFInventorySubtractInventoryItemsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventorySubtractInventoryItemsRequestWrapper() = default;

    friend void swap(PFInventorySubtractInventoryItemsRequestWrapper& lhs, PFInventorySubtractInventoryItemsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_amount, rhs.m_amount);
        swap(lhs.m_collectionId, rhs.m_collectionId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_durationInSeconds, rhs.m_durationInSeconds);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_eTag, rhs.m_eTag);
        swap(lhs.m_idempotencyId, rhs.m_idempotencyId);
        swap(lhs.m_item, rhs.m_item);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAmount(std::optional<int32_t> value)
    {
        m_amount = std::move(value);
        this->m_model.amount = m_amount ? m_amount.operator->() : nullptr;
    }

    void SetCollectionId(String value)
    {
        m_collectionId = std::move(value);
        this->m_model.collectionId =  m_collectionId.empty() ? nullptr : m_collectionId.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetDeleteEmptyStacks(bool value)
    {
        this->m_model.deleteEmptyStacks = value;
    }

    void SetDurationInSeconds(std::optional<double> value)
    {
        m_durationInSeconds = std::move(value);
        this->m_model.durationInSeconds = m_durationInSeconds ? m_durationInSeconds.operator->() : nullptr;
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

    void SetIdempotencyId(String value)
    {
        m_idempotencyId = std::move(value);
        this->m_model.idempotencyId =  m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
    }

    void SetItem(std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> value)
    {
        m_item = std::move(value);
        this->m_model.item = m_item ? &m_item->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.amount = m_amount ? m_amount.operator->() : nullptr;
        this->m_model.collectionId = m_collectionId.empty() ? nullptr : m_collectionId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.durationInSeconds = m_durationInSeconds ? m_durationInSeconds.operator->() : nullptr;
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.eTag = m_eTag.empty() ? nullptr : m_eTag.data();
        this->m_model.idempotencyId = m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
        this->m_model.item = m_item ?  &m_item->Model() : nullptr;
    }

    std::optional<int32_t> m_amount;
    String m_collectionId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<double> m_durationInSeconds;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_eTag;
    String m_idempotencyId;
    std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> m_item;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventorySubtractInventoryItemsResponseWrapper : public ModelWrapper<PFInventorySubtractInventoryItemsResponse, Alloc>
{
public:
    using ModelType = PFInventorySubtractInventoryItemsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventorySubtractInventoryItemsResponseWrapper() = default;

    PFInventorySubtractInventoryItemsResponseWrapper(const PFInventorySubtractInventoryItemsResponse& model) :
        ModelWrapper<PFInventorySubtractInventoryItemsResponse, Alloc>{ model },
        m_eTag{ SafeString(model.eTag) },
        m_idempotencyId{ SafeString(model.idempotencyId) },
        m_transactionIds{ model.transactionIds, model.transactionIds + model.transactionIdsCount }
    {
        SetModelPointers();
    }

    PFInventorySubtractInventoryItemsResponseWrapper(const PFInventorySubtractInventoryItemsResponseWrapper& src) :
        PFInventorySubtractInventoryItemsResponseWrapper{ src.Model() }
    {
    }

    PFInventorySubtractInventoryItemsResponseWrapper(PFInventorySubtractInventoryItemsResponseWrapper&& src) :
        PFInventorySubtractInventoryItemsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFInventorySubtractInventoryItemsResponseWrapper& operator=(PFInventorySubtractInventoryItemsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventorySubtractInventoryItemsResponseWrapper() = default;

    friend void swap(PFInventorySubtractInventoryItemsResponseWrapper& lhs, PFInventorySubtractInventoryItemsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_eTag, rhs.m_eTag);
        swap(lhs.m_idempotencyId, rhs.m_idempotencyId);
        swap(lhs.m_transactionIds, rhs.m_transactionIds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetETag(String value)
    {
        m_eTag = std::move(value);
        this->m_model.eTag =  m_eTag.empty() ? nullptr : m_eTag.data();
    }

    void SetIdempotencyId(String value)
    {
        m_idempotencyId = std::move(value);
        this->m_model.idempotencyId =  m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
    }

    void SetTransactionIds(CStringVector<Alloc> value)
    {
        m_transactionIds = std::move(value);
        this->m_model.transactionIds =  m_transactionIds.empty() ? nullptr : m_transactionIds.data();
        this->m_model.transactionIdsCount =  static_cast<uint32_t>(m_transactionIds.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.eTag = m_eTag.empty() ? nullptr : m_eTag.data();
        this->m_model.idempotencyId = m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
        this->m_model.transactionIds = m_transactionIds.empty() ? nullptr : m_transactionIds.data();
    }

    String m_eTag;
    String m_idempotencyId;
    CStringVector<Alloc> m_transactionIds;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryTransferInventoryItemsRequestWrapper : public ModelWrapper<PFInventoryTransferInventoryItemsRequest, Alloc>
{
public:
    using ModelType = PFInventoryTransferInventoryItemsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryTransferInventoryItemsRequestWrapper() = default;

    PFInventoryTransferInventoryItemsRequestWrapper(const PFInventoryTransferInventoryItemsRequest& model) :
        ModelWrapper<PFInventoryTransferInventoryItemsRequest, Alloc>{ model },
        m_amount{ model.amount ? std::optional<int32_t>{ *model.amount } : std::nullopt },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_givingCollectionId{ SafeString(model.givingCollectionId) },
        m_givingEntity{ model.givingEntity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.givingEntity } : std::nullopt },
        m_givingETag{ SafeString(model.givingETag) },
        m_givingItem{ model.givingItem ? std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>>{ *model.givingItem } : std::nullopt },
        m_idempotencyId{ SafeString(model.idempotencyId) },
        m_newStackValues{ model.newStackValues ? std::optional<PFInventoryInitialValuesWrapper<Alloc>>{ *model.newStackValues } : std::nullopt },
        m_receivingCollectionId{ SafeString(model.receivingCollectionId) },
        m_receivingEntity{ model.receivingEntity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.receivingEntity } : std::nullopt },
        m_receivingItem{ model.receivingItem ? std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>>{ *model.receivingItem } : std::nullopt }
    {
        SetModelPointers();
    }

    PFInventoryTransferInventoryItemsRequestWrapper(const PFInventoryTransferInventoryItemsRequestWrapper& src) :
        PFInventoryTransferInventoryItemsRequestWrapper{ src.Model() }
    {
    }

    PFInventoryTransferInventoryItemsRequestWrapper(PFInventoryTransferInventoryItemsRequestWrapper&& src) :
        PFInventoryTransferInventoryItemsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryTransferInventoryItemsRequestWrapper& operator=(PFInventoryTransferInventoryItemsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryTransferInventoryItemsRequestWrapper() = default;

    friend void swap(PFInventoryTransferInventoryItemsRequestWrapper& lhs, PFInventoryTransferInventoryItemsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_amount, rhs.m_amount);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_givingCollectionId, rhs.m_givingCollectionId);
        swap(lhs.m_givingEntity, rhs.m_givingEntity);
        swap(lhs.m_givingETag, rhs.m_givingETag);
        swap(lhs.m_givingItem, rhs.m_givingItem);
        swap(lhs.m_idempotencyId, rhs.m_idempotencyId);
        swap(lhs.m_newStackValues, rhs.m_newStackValues);
        swap(lhs.m_receivingCollectionId, rhs.m_receivingCollectionId);
        swap(lhs.m_receivingEntity, rhs.m_receivingEntity);
        swap(lhs.m_receivingItem, rhs.m_receivingItem);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAmount(std::optional<int32_t> value)
    {
        m_amount = std::move(value);
        this->m_model.amount = m_amount ? m_amount.operator->() : nullptr;
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetDeleteEmptyStacks(bool value)
    {
        this->m_model.deleteEmptyStacks = value;
    }

    void SetGivingCollectionId(String value)
    {
        m_givingCollectionId = std::move(value);
        this->m_model.givingCollectionId =  m_givingCollectionId.empty() ? nullptr : m_givingCollectionId.data();
    }

    void SetGivingEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_givingEntity = std::move(value);
        this->m_model.givingEntity = m_givingEntity ? &m_givingEntity->Model() : nullptr;
    }

    void SetGivingETag(String value)
    {
        m_givingETag = std::move(value);
        this->m_model.givingETag =  m_givingETag.empty() ? nullptr : m_givingETag.data();
    }

    void SetGivingItem(std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> value)
    {
        m_givingItem = std::move(value);
        this->m_model.givingItem = m_givingItem ? &m_givingItem->Model() : nullptr;
    }

    void SetIdempotencyId(String value)
    {
        m_idempotencyId = std::move(value);
        this->m_model.idempotencyId =  m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
    }

    void SetNewStackValues(std::optional<PFInventoryInitialValuesWrapper<Alloc>> value)
    {
        m_newStackValues = std::move(value);
        this->m_model.newStackValues = m_newStackValues ? &m_newStackValues->Model() : nullptr;
    }

    void SetReceivingCollectionId(String value)
    {
        m_receivingCollectionId = std::move(value);
        this->m_model.receivingCollectionId =  m_receivingCollectionId.empty() ? nullptr : m_receivingCollectionId.data();
    }

    void SetReceivingEntity(std::optional<PFEntityKeyWrapper<Alloc>> value)
    {
        m_receivingEntity = std::move(value);
        this->m_model.receivingEntity = m_receivingEntity ? &m_receivingEntity->Model() : nullptr;
    }

    void SetReceivingItem(std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> value)
    {
        m_receivingItem = std::move(value);
        this->m_model.receivingItem = m_receivingItem ? &m_receivingItem->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.amount = m_amount ? m_amount.operator->() : nullptr;
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.givingCollectionId = m_givingCollectionId.empty() ? nullptr : m_givingCollectionId.data();
        this->m_model.givingEntity = m_givingEntity ?  &m_givingEntity->Model() : nullptr;
        this->m_model.givingETag = m_givingETag.empty() ? nullptr : m_givingETag.data();
        this->m_model.givingItem = m_givingItem ?  &m_givingItem->Model() : nullptr;
        this->m_model.idempotencyId = m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
        this->m_model.newStackValues = m_newStackValues ?  &m_newStackValues->Model() : nullptr;
        this->m_model.receivingCollectionId = m_receivingCollectionId.empty() ? nullptr : m_receivingCollectionId.data();
        this->m_model.receivingEntity = m_receivingEntity ?  &m_receivingEntity->Model() : nullptr;
        this->m_model.receivingItem = m_receivingItem ?  &m_receivingItem->Model() : nullptr;
    }

    std::optional<int32_t> m_amount;
    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_givingCollectionId;
    std::optional<PFEntityKeyWrapper<Alloc>> m_givingEntity;
    String m_givingETag;
    std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> m_givingItem;
    String m_idempotencyId;
    std::optional<PFInventoryInitialValuesWrapper<Alloc>> m_newStackValues;
    String m_receivingCollectionId;
    std::optional<PFEntityKeyWrapper<Alloc>> m_receivingEntity;
    std::optional<PFInventoryInventoryItemReferenceWrapper<Alloc>> m_receivingItem;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryTransferInventoryItemsResponseWrapper : public ModelWrapper<PFInventoryTransferInventoryItemsResponse, Alloc>
{
public:
    using ModelType = PFInventoryTransferInventoryItemsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryTransferInventoryItemsResponseWrapper() = default;

    PFInventoryTransferInventoryItemsResponseWrapper(const PFInventoryTransferInventoryItemsResponse& model) :
        ModelWrapper<PFInventoryTransferInventoryItemsResponse, Alloc>{ model },
        m_givingETag{ SafeString(model.givingETag) },
        m_givingTransactionIds{ model.givingTransactionIds, model.givingTransactionIds + model.givingTransactionIdsCount },
        m_idempotencyId{ SafeString(model.idempotencyId) },
        m_operationStatus{ SafeString(model.operationStatus) },
        m_receivingTransactionIds{ model.receivingTransactionIds, model.receivingTransactionIds + model.receivingTransactionIdsCount }
    {
        SetModelPointers();
    }

    PFInventoryTransferInventoryItemsResponseWrapper(const PFInventoryTransferInventoryItemsResponseWrapper& src) :
        PFInventoryTransferInventoryItemsResponseWrapper{ src.Model() }
    {
    }

    PFInventoryTransferInventoryItemsResponseWrapper(PFInventoryTransferInventoryItemsResponseWrapper&& src) :
        PFInventoryTransferInventoryItemsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryTransferInventoryItemsResponseWrapper& operator=(PFInventoryTransferInventoryItemsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryTransferInventoryItemsResponseWrapper() = default;

    friend void swap(PFInventoryTransferInventoryItemsResponseWrapper& lhs, PFInventoryTransferInventoryItemsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_givingETag, rhs.m_givingETag);
        swap(lhs.m_givingTransactionIds, rhs.m_givingTransactionIds);
        swap(lhs.m_idempotencyId, rhs.m_idempotencyId);
        swap(lhs.m_operationStatus, rhs.m_operationStatus);
        swap(lhs.m_receivingTransactionIds, rhs.m_receivingTransactionIds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetGivingETag(String value)
    {
        m_givingETag = std::move(value);
        this->m_model.givingETag =  m_givingETag.empty() ? nullptr : m_givingETag.data();
    }

    void SetGivingTransactionIds(CStringVector<Alloc> value)
    {
        m_givingTransactionIds = std::move(value);
        this->m_model.givingTransactionIds =  m_givingTransactionIds.empty() ? nullptr : m_givingTransactionIds.data();
        this->m_model.givingTransactionIdsCount =  static_cast<uint32_t>(m_givingTransactionIds.size());
    }

    void SetIdempotencyId(String value)
    {
        m_idempotencyId = std::move(value);
        this->m_model.idempotencyId =  m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
    }

    void SetOperationStatus(String value)
    {
        m_operationStatus = std::move(value);
        this->m_model.operationStatus =  m_operationStatus.empty() ? nullptr : m_operationStatus.data();
    }

    void SetReceivingTransactionIds(CStringVector<Alloc> value)
    {
        m_receivingTransactionIds = std::move(value);
        this->m_model.receivingTransactionIds =  m_receivingTransactionIds.empty() ? nullptr : m_receivingTransactionIds.data();
        this->m_model.receivingTransactionIdsCount =  static_cast<uint32_t>(m_receivingTransactionIds.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.givingETag = m_givingETag.empty() ? nullptr : m_givingETag.data();
        this->m_model.givingTransactionIds = m_givingTransactionIds.empty() ? nullptr : m_givingTransactionIds.data();
        this->m_model.idempotencyId = m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
        this->m_model.operationStatus = m_operationStatus.empty() ? nullptr : m_operationStatus.data();
        this->m_model.receivingTransactionIds = m_receivingTransactionIds.empty() ? nullptr : m_receivingTransactionIds.data();
    }

    String m_givingETag;
    CStringVector<Alloc> m_givingTransactionIds;
    String m_idempotencyId;
    String m_operationStatus;
    CStringVector<Alloc> m_receivingTransactionIds;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryUpdateInventoryItemsRequestWrapper : public ModelWrapper<PFInventoryUpdateInventoryItemsRequest, Alloc>
{
public:
    using ModelType = PFInventoryUpdateInventoryItemsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryUpdateInventoryItemsRequestWrapper() = default;

    PFInventoryUpdateInventoryItemsRequestWrapper(const PFInventoryUpdateInventoryItemsRequest& model) :
        ModelWrapper<PFInventoryUpdateInventoryItemsRequest, Alloc>{ model },
        m_collectionId{ SafeString(model.collectionId) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_eTag{ SafeString(model.eTag) },
        m_idempotencyId{ SafeString(model.idempotencyId) },
        m_item{ model.item ? std::optional<PFInventoryInventoryItemWrapper<Alloc>>{ *model.item } : std::nullopt }
    {
        SetModelPointers();
    }

    PFInventoryUpdateInventoryItemsRequestWrapper(const PFInventoryUpdateInventoryItemsRequestWrapper& src) :
        PFInventoryUpdateInventoryItemsRequestWrapper{ src.Model() }
    {
    }

    PFInventoryUpdateInventoryItemsRequestWrapper(PFInventoryUpdateInventoryItemsRequestWrapper&& src) :
        PFInventoryUpdateInventoryItemsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryUpdateInventoryItemsRequestWrapper& operator=(PFInventoryUpdateInventoryItemsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryUpdateInventoryItemsRequestWrapper() = default;

    friend void swap(PFInventoryUpdateInventoryItemsRequestWrapper& lhs, PFInventoryUpdateInventoryItemsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_collectionId, rhs.m_collectionId);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_eTag, rhs.m_eTag);
        swap(lhs.m_idempotencyId, rhs.m_idempotencyId);
        swap(lhs.m_item, rhs.m_item);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCollectionId(String value)
    {
        m_collectionId = std::move(value);
        this->m_model.collectionId =  m_collectionId.empty() ? nullptr : m_collectionId.data();
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

    void SetIdempotencyId(String value)
    {
        m_idempotencyId = std::move(value);
        this->m_model.idempotencyId =  m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
    }

    void SetItem(std::optional<PFInventoryInventoryItemWrapper<Alloc>> value)
    {
        m_item = std::move(value);
        this->m_model.item = m_item ? &m_item->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.collectionId = m_collectionId.empty() ? nullptr : m_collectionId.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.eTag = m_eTag.empty() ? nullptr : m_eTag.data();
        this->m_model.idempotencyId = m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
        this->m_model.item = m_item ?  &m_item->Model() : nullptr;
    }

    String m_collectionId;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_eTag;
    String m_idempotencyId;
    std::optional<PFInventoryInventoryItemWrapper<Alloc>> m_item;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFInventoryUpdateInventoryItemsResponseWrapper : public ModelWrapper<PFInventoryUpdateInventoryItemsResponse, Alloc>
{
public:
    using ModelType = PFInventoryUpdateInventoryItemsResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFInventoryUpdateInventoryItemsResponseWrapper() = default;

    PFInventoryUpdateInventoryItemsResponseWrapper(const PFInventoryUpdateInventoryItemsResponse& model) :
        ModelWrapper<PFInventoryUpdateInventoryItemsResponse, Alloc>{ model },
        m_eTag{ SafeString(model.eTag) },
        m_idempotencyId{ SafeString(model.idempotencyId) },
        m_transactionIds{ model.transactionIds, model.transactionIds + model.transactionIdsCount }
    {
        SetModelPointers();
    }

    PFInventoryUpdateInventoryItemsResponseWrapper(const PFInventoryUpdateInventoryItemsResponseWrapper& src) :
        PFInventoryUpdateInventoryItemsResponseWrapper{ src.Model() }
    {
    }

    PFInventoryUpdateInventoryItemsResponseWrapper(PFInventoryUpdateInventoryItemsResponseWrapper&& src) :
        PFInventoryUpdateInventoryItemsResponseWrapper{}
    {
        swap(*this, src);
    }

    PFInventoryUpdateInventoryItemsResponseWrapper& operator=(PFInventoryUpdateInventoryItemsResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFInventoryUpdateInventoryItemsResponseWrapper() = default;

    friend void swap(PFInventoryUpdateInventoryItemsResponseWrapper& lhs, PFInventoryUpdateInventoryItemsResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_eTag, rhs.m_eTag);
        swap(lhs.m_idempotencyId, rhs.m_idempotencyId);
        swap(lhs.m_transactionIds, rhs.m_transactionIds);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetETag(String value)
    {
        m_eTag = std::move(value);
        this->m_model.eTag =  m_eTag.empty() ? nullptr : m_eTag.data();
    }

    void SetIdempotencyId(String value)
    {
        m_idempotencyId = std::move(value);
        this->m_model.idempotencyId =  m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
    }

    void SetTransactionIds(CStringVector<Alloc> value)
    {
        m_transactionIds = std::move(value);
        this->m_model.transactionIds =  m_transactionIds.empty() ? nullptr : m_transactionIds.data();
        this->m_model.transactionIdsCount =  static_cast<uint32_t>(m_transactionIds.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.eTag = m_eTag.empty() ? nullptr : m_eTag.data();
        this->m_model.idempotencyId = m_idempotencyId.empty() ? nullptr : m_idempotencyId.data();
        this->m_model.transactionIds = m_transactionIds.empty() ? nullptr : m_transactionIds.data();
    }

    String m_eTag;
    String m_idempotencyId;
    CStringVector<Alloc> m_transactionIds;
};

} // namespace Wrappers
} // namespace PlayFab

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

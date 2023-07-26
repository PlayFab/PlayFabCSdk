// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFSegmentsTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFSegmentsGetSegmentResultWrapper : public ModelWrapper<PFSegmentsGetSegmentResult, Alloc>
{
public:
    using ModelType = PFSegmentsGetSegmentResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFSegmentsGetSegmentResultWrapper() = default;

    PFSegmentsGetSegmentResultWrapper(const PFSegmentsGetSegmentResult& model) :
        ModelWrapper<PFSegmentsGetSegmentResult, Alloc>{ model },
        m_aBTestParent{ SafeString(model.aBTestParent) },
        m_id{ SafeString(model.id) },
        m_name{ SafeString(model.name) }
    {
        SetModelPointers();
    }

    PFSegmentsGetSegmentResultWrapper(const PFSegmentsGetSegmentResultWrapper& src) :
        PFSegmentsGetSegmentResultWrapper{ src.Model() }
    {
    }

    PFSegmentsGetSegmentResultWrapper(PFSegmentsGetSegmentResultWrapper&& src) :
        PFSegmentsGetSegmentResultWrapper{}
    {
        swap(*this, src);
    }

    PFSegmentsGetSegmentResultWrapper& operator=(PFSegmentsGetSegmentResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFSegmentsGetSegmentResultWrapper() = default;

    friend void swap(PFSegmentsGetSegmentResultWrapper& lhs, PFSegmentsGetSegmentResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_aBTestParent, rhs.m_aBTestParent);
        swap(lhs.m_id, rhs.m_id);
        swap(lhs.m_name, rhs.m_name);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetABTestParent(String value)
    {
        m_aBTestParent = std::move(value);
        this->m_model.aBTestParent =  m_aBTestParent.empty() ? nullptr : m_aBTestParent.data();
    }

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.aBTestParent = m_aBTestParent.empty() ? nullptr : m_aBTestParent.data();
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
    }

    String m_aBTestParent;
    String m_id;
    String m_name;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFSegmentsGetPlayerSegmentsResultWrapper : public ModelWrapper<PFSegmentsGetPlayerSegmentsResult, Alloc>
{
public:
    using ModelType = PFSegmentsGetPlayerSegmentsResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFSegmentsGetPlayerSegmentsResultWrapper() = default;

    PFSegmentsGetPlayerSegmentsResultWrapper(const PFSegmentsGetPlayerSegmentsResult& model) :
        ModelWrapper<PFSegmentsGetPlayerSegmentsResult, Alloc>{ model },
        m_segments{ model.segments, model.segments + model.segmentsCount }
    {
        SetModelPointers();
    }

    PFSegmentsGetPlayerSegmentsResultWrapper(const PFSegmentsGetPlayerSegmentsResultWrapper& src) :
        PFSegmentsGetPlayerSegmentsResultWrapper{ src.Model() }
    {
    }

    PFSegmentsGetPlayerSegmentsResultWrapper(PFSegmentsGetPlayerSegmentsResultWrapper&& src) :
        PFSegmentsGetPlayerSegmentsResultWrapper{}
    {
        swap(*this, src);
    }

    PFSegmentsGetPlayerSegmentsResultWrapper& operator=(PFSegmentsGetPlayerSegmentsResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFSegmentsGetPlayerSegmentsResultWrapper() = default;

    friend void swap(PFSegmentsGetPlayerSegmentsResultWrapper& lhs, PFSegmentsGetPlayerSegmentsResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_segments, rhs.m_segments);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetSegments(ModelVector<PFSegmentsGetSegmentResultWrapper<Alloc>, Alloc> value)
    {
        m_segments = std::move(value);
        this->m_model.segments =  m_segments.empty() ? nullptr : m_segments.data();
        this->m_model.segmentsCount =  static_cast<uint32_t>(m_segments.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.segments = m_segments.empty() ? nullptr : m_segments.data();
    }

    ModelVector<PFSegmentsGetSegmentResultWrapper<Alloc>, Alloc> m_segments;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFSegmentsGetPlayerTagsRequestWrapper : public ModelWrapper<PFSegmentsGetPlayerTagsRequest, Alloc>
{
public:
    using ModelType = PFSegmentsGetPlayerTagsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFSegmentsGetPlayerTagsRequestWrapper() = default;

    PFSegmentsGetPlayerTagsRequestWrapper(const PFSegmentsGetPlayerTagsRequest& model) :
        ModelWrapper<PFSegmentsGetPlayerTagsRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_playfabNamespace{ SafeString(model.playfabNamespace) },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFSegmentsGetPlayerTagsRequestWrapper(const PFSegmentsGetPlayerTagsRequestWrapper& src) :
        PFSegmentsGetPlayerTagsRequestWrapper{ src.Model() }
    {
    }

    PFSegmentsGetPlayerTagsRequestWrapper(PFSegmentsGetPlayerTagsRequestWrapper&& src) :
        PFSegmentsGetPlayerTagsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFSegmentsGetPlayerTagsRequestWrapper& operator=(PFSegmentsGetPlayerTagsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFSegmentsGetPlayerTagsRequestWrapper() = default;

    friend void swap(PFSegmentsGetPlayerTagsRequestWrapper& lhs, PFSegmentsGetPlayerTagsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_playfabNamespace, rhs.m_playfabNamespace);
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

    void SetplayfabNamespace(String value)
    {
        m_playfabNamespace = std::move(value);
        this->m_model.playfabNamespace =  m_playfabNamespace.empty() ? nullptr : m_playfabNamespace.data();
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
        this->m_model.playfabNamespace = m_playfabNamespace.empty() ? nullptr : m_playfabNamespace.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_playfabNamespace;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFSegmentsGetPlayerTagsResultWrapper : public ModelWrapper<PFSegmentsGetPlayerTagsResult, Alloc>
{
public:
    using ModelType = PFSegmentsGetPlayerTagsResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFSegmentsGetPlayerTagsResultWrapper() = default;

    PFSegmentsGetPlayerTagsResultWrapper(const PFSegmentsGetPlayerTagsResult& model) :
        ModelWrapper<PFSegmentsGetPlayerTagsResult, Alloc>{ model },
        m_playFabId{ SafeString(model.playFabId) },
        m_tags{ model.tags, model.tags + model.tagsCount }
    {
        SetModelPointers();
    }

    PFSegmentsGetPlayerTagsResultWrapper(const PFSegmentsGetPlayerTagsResultWrapper& src) :
        PFSegmentsGetPlayerTagsResultWrapper{ src.Model() }
    {
    }

    PFSegmentsGetPlayerTagsResultWrapper(PFSegmentsGetPlayerTagsResultWrapper&& src) :
        PFSegmentsGetPlayerTagsResultWrapper{}
    {
        swap(*this, src);
    }

    PFSegmentsGetPlayerTagsResultWrapper& operator=(PFSegmentsGetPlayerTagsResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFSegmentsGetPlayerTagsResultWrapper() = default;

    friend void swap(PFSegmentsGetPlayerTagsResultWrapper& lhs, PFSegmentsGetPlayerTagsResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_tags, rhs.m_tags);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
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
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.tags = m_tags.empty() ? nullptr : m_tags.data();
    }

    String m_playFabId;
    CStringVector<Alloc> m_tags;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFSegmentsAddPlayerTagRequestWrapper : public ModelWrapper<PFSegmentsAddPlayerTagRequest, Alloc>
{
public:
    using ModelType = PFSegmentsAddPlayerTagRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFSegmentsAddPlayerTagRequestWrapper() = default;

    PFSegmentsAddPlayerTagRequestWrapper(const PFSegmentsAddPlayerTagRequest& model) :
        ModelWrapper<PFSegmentsAddPlayerTagRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_playFabId{ SafeString(model.playFabId) },
        m_tagName{ SafeString(model.tagName) }
    {
        SetModelPointers();
    }

    PFSegmentsAddPlayerTagRequestWrapper(const PFSegmentsAddPlayerTagRequestWrapper& src) :
        PFSegmentsAddPlayerTagRequestWrapper{ src.Model() }
    {
    }

    PFSegmentsAddPlayerTagRequestWrapper(PFSegmentsAddPlayerTagRequestWrapper&& src) :
        PFSegmentsAddPlayerTagRequestWrapper{}
    {
        swap(*this, src);
    }

    PFSegmentsAddPlayerTagRequestWrapper& operator=(PFSegmentsAddPlayerTagRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFSegmentsAddPlayerTagRequestWrapper() = default;

    friend void swap(PFSegmentsAddPlayerTagRequestWrapper& lhs, PFSegmentsAddPlayerTagRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_tagName, rhs.m_tagName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetTagName(String value)
    {
        m_tagName = std::move(value);
        this->m_model.tagName =  m_tagName.empty() ? nullptr : m_tagName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.tagName = m_tagName.empty() ? nullptr : m_tagName.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_playFabId;
    String m_tagName;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFSegmentsGetAllSegmentsResultWrapper : public ModelWrapper<PFSegmentsGetAllSegmentsResult, Alloc>
{
public:
    using ModelType = PFSegmentsGetAllSegmentsResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFSegmentsGetAllSegmentsResultWrapper() = default;

    PFSegmentsGetAllSegmentsResultWrapper(const PFSegmentsGetAllSegmentsResult& model) :
        ModelWrapper<PFSegmentsGetAllSegmentsResult, Alloc>{ model },
        m_segments{ model.segments, model.segments + model.segmentsCount }
    {
        SetModelPointers();
    }

    PFSegmentsGetAllSegmentsResultWrapper(const PFSegmentsGetAllSegmentsResultWrapper& src) :
        PFSegmentsGetAllSegmentsResultWrapper{ src.Model() }
    {
    }

    PFSegmentsGetAllSegmentsResultWrapper(PFSegmentsGetAllSegmentsResultWrapper&& src) :
        PFSegmentsGetAllSegmentsResultWrapper{}
    {
        swap(*this, src);
    }

    PFSegmentsGetAllSegmentsResultWrapper& operator=(PFSegmentsGetAllSegmentsResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFSegmentsGetAllSegmentsResultWrapper() = default;

    friend void swap(PFSegmentsGetAllSegmentsResultWrapper& lhs, PFSegmentsGetAllSegmentsResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_segments, rhs.m_segments);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetSegments(ModelVector<PFSegmentsGetSegmentResultWrapper<Alloc>, Alloc> value)
    {
        m_segments = std::move(value);
        this->m_model.segments =  m_segments.empty() ? nullptr : m_segments.data();
        this->m_model.segmentsCount =  static_cast<uint32_t>(m_segments.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.segments = m_segments.empty() ? nullptr : m_segments.data();
    }

    ModelVector<PFSegmentsGetSegmentResultWrapper<Alloc>, Alloc> m_segments;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFSegmentsGetPlayersSegmentsRequestWrapper : public ModelWrapper<PFSegmentsGetPlayersSegmentsRequest, Alloc>
{
public:
    using ModelType = PFSegmentsGetPlayersSegmentsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFSegmentsGetPlayersSegmentsRequestWrapper() = default;

    PFSegmentsGetPlayersSegmentsRequestWrapper(const PFSegmentsGetPlayersSegmentsRequest& model) :
        ModelWrapper<PFSegmentsGetPlayersSegmentsRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_playFabId{ SafeString(model.playFabId) }
    {
        SetModelPointers();
    }

    PFSegmentsGetPlayersSegmentsRequestWrapper(const PFSegmentsGetPlayersSegmentsRequestWrapper& src) :
        PFSegmentsGetPlayersSegmentsRequestWrapper{ src.Model() }
    {
    }

    PFSegmentsGetPlayersSegmentsRequestWrapper(PFSegmentsGetPlayersSegmentsRequestWrapper&& src) :
        PFSegmentsGetPlayersSegmentsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFSegmentsGetPlayersSegmentsRequestWrapper& operator=(PFSegmentsGetPlayersSegmentsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFSegmentsGetPlayersSegmentsRequestWrapper() = default;

    friend void swap(PFSegmentsGetPlayersSegmentsRequestWrapper& lhs, PFSegmentsGetPlayersSegmentsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
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

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_playFabId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFSegmentsGetPlayersInSegmentRequestWrapper : public ModelWrapper<PFSegmentsGetPlayersInSegmentRequest, Alloc>
{
public:
    using ModelType = PFSegmentsGetPlayersInSegmentRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFSegmentsGetPlayersInSegmentRequestWrapper() = default;

    PFSegmentsGetPlayersInSegmentRequestWrapper(const PFSegmentsGetPlayersInSegmentRequest& model) :
        ModelWrapper<PFSegmentsGetPlayersInSegmentRequest, Alloc>{ model },
        m_continuationToken{ SafeString(model.continuationToken) },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_getProfilesAsync{ model.getProfilesAsync ? std::optional<bool>{ *model.getProfilesAsync } : std::nullopt },
        m_maxBatchSize{ model.maxBatchSize ? std::optional<uint32_t>{ *model.maxBatchSize } : std::nullopt },
        m_secondsToLive{ model.secondsToLive ? std::optional<uint32_t>{ *model.secondsToLive } : std::nullopt },
        m_segmentId{ SafeString(model.segmentId) }
    {
        SetModelPointers();
    }

    PFSegmentsGetPlayersInSegmentRequestWrapper(const PFSegmentsGetPlayersInSegmentRequestWrapper& src) :
        PFSegmentsGetPlayersInSegmentRequestWrapper{ src.Model() }
    {
    }

    PFSegmentsGetPlayersInSegmentRequestWrapper(PFSegmentsGetPlayersInSegmentRequestWrapper&& src) :
        PFSegmentsGetPlayersInSegmentRequestWrapper{}
    {
        swap(*this, src);
    }

    PFSegmentsGetPlayersInSegmentRequestWrapper& operator=(PFSegmentsGetPlayersInSegmentRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFSegmentsGetPlayersInSegmentRequestWrapper() = default;

    friend void swap(PFSegmentsGetPlayersInSegmentRequestWrapper& lhs, PFSegmentsGetPlayersInSegmentRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_continuationToken, rhs.m_continuationToken);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_getProfilesAsync, rhs.m_getProfilesAsync);
        swap(lhs.m_maxBatchSize, rhs.m_maxBatchSize);
        swap(lhs.m_secondsToLive, rhs.m_secondsToLive);
        swap(lhs.m_segmentId, rhs.m_segmentId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetContinuationToken(String value)
    {
        m_continuationToken = std::move(value);
        this->m_model.continuationToken =  m_continuationToken.empty() ? nullptr : m_continuationToken.data();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetGetProfilesAsync(std::optional<bool> value)
    {
        m_getProfilesAsync = std::move(value);
        this->m_model.getProfilesAsync = m_getProfilesAsync ? m_getProfilesAsync.operator->() : nullptr;
    }

    void SetMaxBatchSize(std::optional<uint32_t> value)
    {
        m_maxBatchSize = std::move(value);
        this->m_model.maxBatchSize = m_maxBatchSize ? m_maxBatchSize.operator->() : nullptr;
    }

    void SetSecondsToLive(std::optional<uint32_t> value)
    {
        m_secondsToLive = std::move(value);
        this->m_model.secondsToLive = m_secondsToLive ? m_secondsToLive.operator->() : nullptr;
    }

    void SetSegmentId(String value)
    {
        m_segmentId = std::move(value);
        this->m_model.segmentId =  m_segmentId.empty() ? nullptr : m_segmentId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.continuationToken = m_continuationToken.empty() ? nullptr : m_continuationToken.data();
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.getProfilesAsync = m_getProfilesAsync ? m_getProfilesAsync.operator->() : nullptr;
        this->m_model.maxBatchSize = m_maxBatchSize ? m_maxBatchSize.operator->() : nullptr;
        this->m_model.secondsToLive = m_secondsToLive ? m_secondsToLive.operator->() : nullptr;
        this->m_model.segmentId = m_segmentId.empty() ? nullptr : m_segmentId.data();
    }

    String m_continuationToken;
    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<bool> m_getProfilesAsync;
    std::optional<uint32_t> m_maxBatchSize;
    std::optional<uint32_t> m_secondsToLive;
    String m_segmentId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFSegmentsAdCampaignAttributionWrapper : public ModelWrapper<PFSegmentsAdCampaignAttribution, Alloc>
{
public:
    using ModelType = PFSegmentsAdCampaignAttribution;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFSegmentsAdCampaignAttributionWrapper() = default;

    PFSegmentsAdCampaignAttributionWrapper(const PFSegmentsAdCampaignAttribution& model) :
        ModelWrapper<PFSegmentsAdCampaignAttribution, Alloc>{ model },
        m_campaignId{ SafeString(model.campaignId) },
        m_platform{ SafeString(model.platform) }
    {
        SetModelPointers();
    }

    PFSegmentsAdCampaignAttributionWrapper(const PFSegmentsAdCampaignAttributionWrapper& src) :
        PFSegmentsAdCampaignAttributionWrapper{ src.Model() }
    {
    }

    PFSegmentsAdCampaignAttributionWrapper(PFSegmentsAdCampaignAttributionWrapper&& src) :
        PFSegmentsAdCampaignAttributionWrapper{}
    {
        swap(*this, src);
    }

    PFSegmentsAdCampaignAttributionWrapper& operator=(PFSegmentsAdCampaignAttributionWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFSegmentsAdCampaignAttributionWrapper() = default;

    friend void swap(PFSegmentsAdCampaignAttributionWrapper& lhs, PFSegmentsAdCampaignAttributionWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_campaignId, rhs.m_campaignId);
        swap(lhs.m_platform, rhs.m_platform);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAttributedAt(time_t value)
    {
        this->m_model.attributedAt = value;
    }

    void SetCampaignId(String value)
    {
        m_campaignId = std::move(value);
        this->m_model.campaignId =  m_campaignId.empty() ? nullptr : m_campaignId.data();
    }

    void SetPlatform(String value)
    {
        m_platform = std::move(value);
        this->m_model.platform =  m_platform.empty() ? nullptr : m_platform.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.campaignId = m_campaignId.empty() ? nullptr : m_campaignId.data();
        this->m_model.platform = m_platform.empty() ? nullptr : m_platform.data();
    }

    String m_campaignId;
    String m_platform;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFSegmentsContactEmailInfoWrapper : public ModelWrapper<PFSegmentsContactEmailInfo, Alloc>
{
public:
    using ModelType = PFSegmentsContactEmailInfo;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFSegmentsContactEmailInfoWrapper() = default;

    PFSegmentsContactEmailInfoWrapper(const PFSegmentsContactEmailInfo& model) :
        ModelWrapper<PFSegmentsContactEmailInfo, Alloc>{ model },
        m_emailAddress{ SafeString(model.emailAddress) },
        m_name{ SafeString(model.name) },
        m_verificationStatus{ model.verificationStatus ? std::optional<PFEmailVerificationStatus>{ *model.verificationStatus } : std::nullopt }
    {
        SetModelPointers();
    }

    PFSegmentsContactEmailInfoWrapper(const PFSegmentsContactEmailInfoWrapper& src) :
        PFSegmentsContactEmailInfoWrapper{ src.Model() }
    {
    }

    PFSegmentsContactEmailInfoWrapper(PFSegmentsContactEmailInfoWrapper&& src) :
        PFSegmentsContactEmailInfoWrapper{}
    {
        swap(*this, src);
    }

    PFSegmentsContactEmailInfoWrapper& operator=(PFSegmentsContactEmailInfoWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFSegmentsContactEmailInfoWrapper() = default;

    friend void swap(PFSegmentsContactEmailInfoWrapper& lhs, PFSegmentsContactEmailInfoWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_emailAddress, rhs.m_emailAddress);
        swap(lhs.m_name, rhs.m_name);
        swap(lhs.m_verificationStatus, rhs.m_verificationStatus);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetEmailAddress(String value)
    {
        m_emailAddress = std::move(value);
        this->m_model.emailAddress =  m_emailAddress.empty() ? nullptr : m_emailAddress.data();
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

    void SetVerificationStatus(std::optional<PFEmailVerificationStatus> value)
    {
        m_verificationStatus = std::move(value);
        this->m_model.verificationStatus = m_verificationStatus ? m_verificationStatus.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.emailAddress = m_emailAddress.empty() ? nullptr : m_emailAddress.data();
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
        this->m_model.verificationStatus = m_verificationStatus ? m_verificationStatus.operator->() : nullptr;
    }

    String m_emailAddress;
    String m_name;
    std::optional<PFEmailVerificationStatus> m_verificationStatus;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFSegmentsPlayerLinkedAccountWrapper : public ModelWrapper<PFSegmentsPlayerLinkedAccount, Alloc>
{
public:
    using ModelType = PFSegmentsPlayerLinkedAccount;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFSegmentsPlayerLinkedAccountWrapper() = default;

    PFSegmentsPlayerLinkedAccountWrapper(const PFSegmentsPlayerLinkedAccount& model) :
        ModelWrapper<PFSegmentsPlayerLinkedAccount, Alloc>{ model },
        m_email{ SafeString(model.email) },
        m_platform{ model.platform ? std::optional<PFLoginIdentityProvider>{ *model.platform } : std::nullopt },
        m_platformUserId{ SafeString(model.platformUserId) },
        m_username{ SafeString(model.username) }
    {
        SetModelPointers();
    }

    PFSegmentsPlayerLinkedAccountWrapper(const PFSegmentsPlayerLinkedAccountWrapper& src) :
        PFSegmentsPlayerLinkedAccountWrapper{ src.Model() }
    {
    }

    PFSegmentsPlayerLinkedAccountWrapper(PFSegmentsPlayerLinkedAccountWrapper&& src) :
        PFSegmentsPlayerLinkedAccountWrapper{}
    {
        swap(*this, src);
    }

    PFSegmentsPlayerLinkedAccountWrapper& operator=(PFSegmentsPlayerLinkedAccountWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFSegmentsPlayerLinkedAccountWrapper() = default;

    friend void swap(PFSegmentsPlayerLinkedAccountWrapper& lhs, PFSegmentsPlayerLinkedAccountWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_email, rhs.m_email);
        swap(lhs.m_platform, rhs.m_platform);
        swap(lhs.m_platformUserId, rhs.m_platformUserId);
        swap(lhs.m_username, rhs.m_username);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetEmail(String value)
    {
        m_email = std::move(value);
        this->m_model.email =  m_email.empty() ? nullptr : m_email.data();
    }

    void SetPlatform(std::optional<PFLoginIdentityProvider> value)
    {
        m_platform = std::move(value);
        this->m_model.platform = m_platform ? m_platform.operator->() : nullptr;
    }

    void SetPlatformUserId(String value)
    {
        m_platformUserId = std::move(value);
        this->m_model.platformUserId =  m_platformUserId.empty() ? nullptr : m_platformUserId.data();
    }

    void SetUsername(String value)
    {
        m_username = std::move(value);
        this->m_model.username =  m_username.empty() ? nullptr : m_username.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.email = m_email.empty() ? nullptr : m_email.data();
        this->m_model.platform = m_platform ? m_platform.operator->() : nullptr;
        this->m_model.platformUserId = m_platformUserId.empty() ? nullptr : m_platformUserId.data();
        this->m_model.username = m_username.empty() ? nullptr : m_username.data();
    }

    String m_email;
    std::optional<PFLoginIdentityProvider> m_platform;
    String m_platformUserId;
    String m_username;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFSegmentsPlayerLocationWrapper : public ModelWrapper<PFSegmentsPlayerLocation, Alloc>
{
public:
    using ModelType = PFSegmentsPlayerLocation;
    using DictionaryEntryType = PFSegmentsPlayerLocationDictionaryEntry;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFSegmentsPlayerLocationWrapper() = default;

    PFSegmentsPlayerLocationWrapper(const PFSegmentsPlayerLocation& model) :
        ModelWrapper<PFSegmentsPlayerLocation, Alloc>{ model },
        m_city{ SafeString(model.city) },
        m_latitude{ model.latitude ? std::optional<double>{ *model.latitude } : std::nullopt },
        m_longitude{ model.longitude ? std::optional<double>{ *model.longitude } : std::nullopt }
    {
        SetModelPointers();
    }

    PFSegmentsPlayerLocationWrapper(const PFSegmentsPlayerLocationWrapper& src) :
        PFSegmentsPlayerLocationWrapper{ src.Model() }
    {
    }

    PFSegmentsPlayerLocationWrapper(PFSegmentsPlayerLocationWrapper&& src) :
        PFSegmentsPlayerLocationWrapper{}
    {
        swap(*this, src);
    }

    PFSegmentsPlayerLocationWrapper& operator=(PFSegmentsPlayerLocationWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFSegmentsPlayerLocationWrapper() = default;

    friend void swap(PFSegmentsPlayerLocationWrapper& lhs, PFSegmentsPlayerLocationWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_city, rhs.m_city);
        swap(lhs.m_latitude, rhs.m_latitude);
        swap(lhs.m_longitude, rhs.m_longitude);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCity(String value)
    {
        m_city = std::move(value);
        this->m_model.city =  m_city.empty() ? nullptr : m_city.data();
    }

    void SetContinentCode(PFContinentCode value)
    {
        this->m_model.continentCode = value;
    }

    void SetCountryCode(PFCountryCode value)
    {
        this->m_model.countryCode = value;
    }

    void SetLatitude(std::optional<double> value)
    {
        m_latitude = std::move(value);
        this->m_model.latitude = m_latitude ? m_latitude.operator->() : nullptr;
    }

    void SetLongitude(std::optional<double> value)
    {
        m_longitude = std::move(value);
        this->m_model.longitude = m_longitude ? m_longitude.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.city = m_city.empty() ? nullptr : m_city.data();
        this->m_model.latitude = m_latitude ? m_latitude.operator->() : nullptr;
        this->m_model.longitude = m_longitude ? m_longitude.operator->() : nullptr;
    }

    String m_city;
    std::optional<double> m_latitude;
    std::optional<double> m_longitude;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFSegmentsPlayerStatisticWrapper : public ModelWrapper<PFSegmentsPlayerStatistic, Alloc>
{
public:
    using ModelType = PFSegmentsPlayerStatistic;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFSegmentsPlayerStatisticWrapper() = default;

    PFSegmentsPlayerStatisticWrapper(const PFSegmentsPlayerStatistic& model) :
        ModelWrapper<PFSegmentsPlayerStatistic, Alloc>{ model },
        m_id{ SafeString(model.id) },
        m_name{ SafeString(model.name) }
    {
        SetModelPointers();
    }

    PFSegmentsPlayerStatisticWrapper(const PFSegmentsPlayerStatisticWrapper& src) :
        PFSegmentsPlayerStatisticWrapper{ src.Model() }
    {
    }

    PFSegmentsPlayerStatisticWrapper(PFSegmentsPlayerStatisticWrapper&& src) :
        PFSegmentsPlayerStatisticWrapper{}
    {
        swap(*this, src);
    }

    PFSegmentsPlayerStatisticWrapper& operator=(PFSegmentsPlayerStatisticWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFSegmentsPlayerStatisticWrapper() = default;

    friend void swap(PFSegmentsPlayerStatisticWrapper& lhs, PFSegmentsPlayerStatisticWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_id, rhs.m_id);
        swap(lhs.m_name, rhs.m_name);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
    }

    void SetName(String value)
    {
        m_name = std::move(value);
        this->m_model.name =  m_name.empty() ? nullptr : m_name.data();
    }

    void SetStatisticValue(int32_t value)
    {
        this->m_model.statisticValue = value;
    }

    void SetStatisticVersion(int32_t value)
    {
        this->m_model.statisticVersion = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.id = m_id.empty() ? nullptr : m_id.data();
        this->m_model.name = m_name.empty() ? nullptr : m_name.data();
    }

    String m_id;
    String m_name;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFSegmentsPushNotificationRegistrationWrapper : public ModelWrapper<PFSegmentsPushNotificationRegistration, Alloc>
{
public:
    using ModelType = PFSegmentsPushNotificationRegistration;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFSegmentsPushNotificationRegistrationWrapper() = default;

    PFSegmentsPushNotificationRegistrationWrapper(const PFSegmentsPushNotificationRegistration& model) :
        ModelWrapper<PFSegmentsPushNotificationRegistration, Alloc>{ model },
        m_notificationEndpointARN{ SafeString(model.notificationEndpointARN) },
        m_platform{ model.platform ? std::optional<PFPushNotificationPlatform>{ *model.platform } : std::nullopt }
    {
        SetModelPointers();
    }

    PFSegmentsPushNotificationRegistrationWrapper(const PFSegmentsPushNotificationRegistrationWrapper& src) :
        PFSegmentsPushNotificationRegistrationWrapper{ src.Model() }
    {
    }

    PFSegmentsPushNotificationRegistrationWrapper(PFSegmentsPushNotificationRegistrationWrapper&& src) :
        PFSegmentsPushNotificationRegistrationWrapper{}
    {
        swap(*this, src);
    }

    PFSegmentsPushNotificationRegistrationWrapper& operator=(PFSegmentsPushNotificationRegistrationWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFSegmentsPushNotificationRegistrationWrapper() = default;

    friend void swap(PFSegmentsPushNotificationRegistrationWrapper& lhs, PFSegmentsPushNotificationRegistrationWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_notificationEndpointARN, rhs.m_notificationEndpointARN);
        swap(lhs.m_platform, rhs.m_platform);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetNotificationEndpointARN(String value)
    {
        m_notificationEndpointARN = std::move(value);
        this->m_model.notificationEndpointARN =  m_notificationEndpointARN.empty() ? nullptr : m_notificationEndpointARN.data();
    }

    void SetPlatform(std::optional<PFPushNotificationPlatform> value)
    {
        m_platform = std::move(value);
        this->m_model.platform = m_platform ? m_platform.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.notificationEndpointARN = m_notificationEndpointARN.empty() ? nullptr : m_notificationEndpointARN.data();
        this->m_model.platform = m_platform ? m_platform.operator->() : nullptr;
    }

    String m_notificationEndpointARN;
    std::optional<PFPushNotificationPlatform> m_platform;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFSegmentsPlayerProfileWrapper : public ModelWrapper<PFSegmentsPlayerProfile, Alloc>
{
public:
    using ModelType = PFSegmentsPlayerProfile;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFSegmentsPlayerProfileWrapper() = default;

    PFSegmentsPlayerProfileWrapper(const PFSegmentsPlayerProfile& model) :
        ModelWrapper<PFSegmentsPlayerProfile, Alloc>{ model },
        m_adCampaignAttributions{ model.adCampaignAttributions, model.adCampaignAttributions + model.adCampaignAttributionsCount },
        m_avatarUrl{ SafeString(model.avatarUrl) },
        m_bannedUntil{ model.bannedUntil ? std::optional<time_t>{ *model.bannedUntil } : std::nullopt },
        m_churnPrediction{ model.churnPrediction ? std::optional<PFSegmentsChurnRiskLevel>{ *model.churnPrediction } : std::nullopt },
        m_contactEmailAddresses{ model.contactEmailAddresses, model.contactEmailAddresses + model.contactEmailAddressesCount },
        m_created{ model.created ? std::optional<time_t>{ *model.created } : std::nullopt },
        m_displayName{ SafeString(model.displayName) },
        m_lastLogin{ model.lastLogin ? std::optional<time_t>{ *model.lastLogin } : std::nullopt },
        m_linkedAccounts{ model.linkedAccounts, model.linkedAccounts + model.linkedAccountsCount },
        m_locations{ model.locations, model.locations + model.locationsCount },
        m_origination{ model.origination ? std::optional<PFLoginIdentityProvider>{ *model.origination } : std::nullopt },
        m_playerExperimentVariants{ model.playerExperimentVariants, model.playerExperimentVariants + model.playerExperimentVariantsCount },
        m_playerId{ SafeString(model.playerId) },
        m_playerStatistics{ model.playerStatistics, model.playerStatistics + model.playerStatisticsCount },
        m_publisherId{ SafeString(model.publisherId) },
        m_pushNotificationRegistrations{ model.pushNotificationRegistrations, model.pushNotificationRegistrations + model.pushNotificationRegistrationsCount },
        m_statistics{ model.statistics, model.statistics + model.statisticsCount },
        m_tags{ model.tags, model.tags + model.tagsCount },
        m_titleId{ SafeString(model.titleId) },
        m_totalValueToDateInUSD{ model.totalValueToDateInUSD ? std::optional<uint32_t>{ *model.totalValueToDateInUSD } : std::nullopt },
        m_valuesToDate{ model.valuesToDate, model.valuesToDate + model.valuesToDateCount },
        m_virtualCurrencyBalances{ model.virtualCurrencyBalances, model.virtualCurrencyBalances + model.virtualCurrencyBalancesCount }
    {
        SetModelPointers();
    }

    PFSegmentsPlayerProfileWrapper(const PFSegmentsPlayerProfileWrapper& src) :
        PFSegmentsPlayerProfileWrapper{ src.Model() }
    {
    }

    PFSegmentsPlayerProfileWrapper(PFSegmentsPlayerProfileWrapper&& src) :
        PFSegmentsPlayerProfileWrapper{}
    {
        swap(*this, src);
    }

    PFSegmentsPlayerProfileWrapper& operator=(PFSegmentsPlayerProfileWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFSegmentsPlayerProfileWrapper() = default;

    friend void swap(PFSegmentsPlayerProfileWrapper& lhs, PFSegmentsPlayerProfileWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_adCampaignAttributions, rhs.m_adCampaignAttributions);
        swap(lhs.m_avatarUrl, rhs.m_avatarUrl);
        swap(lhs.m_bannedUntil, rhs.m_bannedUntil);
        swap(lhs.m_churnPrediction, rhs.m_churnPrediction);
        swap(lhs.m_contactEmailAddresses, rhs.m_contactEmailAddresses);
        swap(lhs.m_created, rhs.m_created);
        swap(lhs.m_displayName, rhs.m_displayName);
        swap(lhs.m_lastLogin, rhs.m_lastLogin);
        swap(lhs.m_linkedAccounts, rhs.m_linkedAccounts);
        swap(lhs.m_locations, rhs.m_locations);
        swap(lhs.m_origination, rhs.m_origination);
        swap(lhs.m_playerExperimentVariants, rhs.m_playerExperimentVariants);
        swap(lhs.m_playerId, rhs.m_playerId);
        swap(lhs.m_playerStatistics, rhs.m_playerStatistics);
        swap(lhs.m_publisherId, rhs.m_publisherId);
        swap(lhs.m_pushNotificationRegistrations, rhs.m_pushNotificationRegistrations);
        swap(lhs.m_statistics, rhs.m_statistics);
        swap(lhs.m_tags, rhs.m_tags);
        swap(lhs.m_titleId, rhs.m_titleId);
        swap(lhs.m_totalValueToDateInUSD, rhs.m_totalValueToDateInUSD);
        swap(lhs.m_valuesToDate, rhs.m_valuesToDate);
        swap(lhs.m_virtualCurrencyBalances, rhs.m_virtualCurrencyBalances);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAdCampaignAttributions(ModelVector<PFSegmentsAdCampaignAttributionWrapper<Alloc>, Alloc> value)
    {
        m_adCampaignAttributions = std::move(value);
        this->m_model.adCampaignAttributions =  m_adCampaignAttributions.empty() ? nullptr : m_adCampaignAttributions.data();
        this->m_model.adCampaignAttributionsCount =  static_cast<uint32_t>(m_adCampaignAttributions.size());
    }

    void SetAvatarUrl(String value)
    {
        m_avatarUrl = std::move(value);
        this->m_model.avatarUrl =  m_avatarUrl.empty() ? nullptr : m_avatarUrl.data();
    }

    void SetBannedUntil(std::optional<time_t> value)
    {
        m_bannedUntil = std::move(value);
        this->m_model.bannedUntil = m_bannedUntil ? m_bannedUntil.operator->() : nullptr;
    }

    void SetChurnPrediction(std::optional<PFSegmentsChurnRiskLevel> value)
    {
        m_churnPrediction = std::move(value);
        this->m_model.churnPrediction = m_churnPrediction ? m_churnPrediction.operator->() : nullptr;
    }

    void SetContactEmailAddresses(ModelVector<PFSegmentsContactEmailInfoWrapper<Alloc>, Alloc> value)
    {
        m_contactEmailAddresses = std::move(value);
        this->m_model.contactEmailAddresses =  m_contactEmailAddresses.empty() ? nullptr : m_contactEmailAddresses.data();
        this->m_model.contactEmailAddressesCount =  static_cast<uint32_t>(m_contactEmailAddresses.size());
    }

    void SetCreated(std::optional<time_t> value)
    {
        m_created = std::move(value);
        this->m_model.created = m_created ? m_created.operator->() : nullptr;
    }

    void SetDisplayName(String value)
    {
        m_displayName = std::move(value);
        this->m_model.displayName =  m_displayName.empty() ? nullptr : m_displayName.data();
    }

    void SetLastLogin(std::optional<time_t> value)
    {
        m_lastLogin = std::move(value);
        this->m_model.lastLogin = m_lastLogin ? m_lastLogin.operator->() : nullptr;
    }

    void SetLinkedAccounts(ModelVector<PFSegmentsPlayerLinkedAccountWrapper<Alloc>, Alloc> value)
    {
        m_linkedAccounts = std::move(value);
        this->m_model.linkedAccounts =  m_linkedAccounts.empty() ? nullptr : m_linkedAccounts.data();
        this->m_model.linkedAccountsCount =  static_cast<uint32_t>(m_linkedAccounts.size());
    }

    void SetLocations(ModelDictionaryEntryVector<PFSegmentsPlayerLocationWrapper<Alloc>, Alloc> value)
    {
        m_locations = std::move(value);
        this->m_model.locations =  m_locations.empty() ? nullptr : m_locations.data();
        this->m_model.locationsCount =  static_cast<uint32_t>(m_locations.size());
    }

    void SetOrigination(std::optional<PFLoginIdentityProvider> value)
    {
        m_origination = std::move(value);
        this->m_model.origination = m_origination ? m_origination.operator->() : nullptr;
    }

    void SetPlayerExperimentVariants(CStringVector<Alloc> value)
    {
        m_playerExperimentVariants = std::move(value);
        this->m_model.playerExperimentVariants =  m_playerExperimentVariants.empty() ? nullptr : m_playerExperimentVariants.data();
        this->m_model.playerExperimentVariantsCount =  static_cast<uint32_t>(m_playerExperimentVariants.size());
    }

    void SetPlayerId(String value)
    {
        m_playerId = std::move(value);
        this->m_model.playerId =  m_playerId.empty() ? nullptr : m_playerId.data();
    }

    void SetPlayerStatistics(ModelVector<PFSegmentsPlayerStatisticWrapper<Alloc>, Alloc> value)
    {
        m_playerStatistics = std::move(value);
        this->m_model.playerStatistics =  m_playerStatistics.empty() ? nullptr : m_playerStatistics.data();
        this->m_model.playerStatisticsCount =  static_cast<uint32_t>(m_playerStatistics.size());
    }

    void SetPublisherId(String value)
    {
        m_publisherId = std::move(value);
        this->m_model.publisherId =  m_publisherId.empty() ? nullptr : m_publisherId.data();
    }

    void SetPushNotificationRegistrations(ModelVector<PFSegmentsPushNotificationRegistrationWrapper<Alloc>, Alloc> value)
    {
        m_pushNotificationRegistrations = std::move(value);
        this->m_model.pushNotificationRegistrations =  m_pushNotificationRegistrations.empty() ? nullptr : m_pushNotificationRegistrations.data();
        this->m_model.pushNotificationRegistrationsCount =  static_cast<uint32_t>(m_pushNotificationRegistrations.size());
    }

    void SetStatistics(DictionaryEntryVector<PFInt32DictionaryEntry, Alloc> value)
    {
        m_statistics = std::move(value);
        this->m_model.statistics =  m_statistics.empty() ? nullptr : m_statistics.data();
        this->m_model.statisticsCount =  static_cast<uint32_t>(m_statistics.size());
    }

    void SetTags(CStringVector<Alloc> value)
    {
        m_tags = std::move(value);
        this->m_model.tags =  m_tags.empty() ? nullptr : m_tags.data();
        this->m_model.tagsCount =  static_cast<uint32_t>(m_tags.size());
    }

    void SetTitleId(String value)
    {
        m_titleId = std::move(value);
        this->m_model.titleId =  m_titleId.empty() ? nullptr : m_titleId.data();
    }

    void SetTotalValueToDateInUSD(std::optional<uint32_t> value)
    {
        m_totalValueToDateInUSD = std::move(value);
        this->m_model.totalValueToDateInUSD = m_totalValueToDateInUSD ? m_totalValueToDateInUSD.operator->() : nullptr;
    }

    void SetValuesToDate(DictionaryEntryVector<PFUint32DictionaryEntry, Alloc> value)
    {
        m_valuesToDate = std::move(value);
        this->m_model.valuesToDate =  m_valuesToDate.empty() ? nullptr : m_valuesToDate.data();
        this->m_model.valuesToDateCount =  static_cast<uint32_t>(m_valuesToDate.size());
    }

    void SetVirtualCurrencyBalances(DictionaryEntryVector<PFInt32DictionaryEntry, Alloc> value)
    {
        m_virtualCurrencyBalances = std::move(value);
        this->m_model.virtualCurrencyBalances =  m_virtualCurrencyBalances.empty() ? nullptr : m_virtualCurrencyBalances.data();
        this->m_model.virtualCurrencyBalancesCount =  static_cast<uint32_t>(m_virtualCurrencyBalances.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.adCampaignAttributions = m_adCampaignAttributions.empty() ? nullptr : m_adCampaignAttributions.data();
        this->m_model.avatarUrl = m_avatarUrl.empty() ? nullptr : m_avatarUrl.data();
        this->m_model.bannedUntil = m_bannedUntil ? m_bannedUntil.operator->() : nullptr;
        this->m_model.churnPrediction = m_churnPrediction ? m_churnPrediction.operator->() : nullptr;
        this->m_model.contactEmailAddresses = m_contactEmailAddresses.empty() ? nullptr : m_contactEmailAddresses.data();
        this->m_model.created = m_created ? m_created.operator->() : nullptr;
        this->m_model.displayName = m_displayName.empty() ? nullptr : m_displayName.data();
        this->m_model.lastLogin = m_lastLogin ? m_lastLogin.operator->() : nullptr;
        this->m_model.linkedAccounts = m_linkedAccounts.empty() ? nullptr : m_linkedAccounts.data();
        this->m_model.locations = m_locations.empty() ? nullptr : m_locations.data();
        this->m_model.origination = m_origination ? m_origination.operator->() : nullptr;
        this->m_model.playerExperimentVariants = m_playerExperimentVariants.empty() ? nullptr : m_playerExperimentVariants.data();
        this->m_model.playerId = m_playerId.empty() ? nullptr : m_playerId.data();
        this->m_model.playerStatistics = m_playerStatistics.empty() ? nullptr : m_playerStatistics.data();
        this->m_model.publisherId = m_publisherId.empty() ? nullptr : m_publisherId.data();
        this->m_model.pushNotificationRegistrations = m_pushNotificationRegistrations.empty() ? nullptr : m_pushNotificationRegistrations.data();
        this->m_model.statistics = m_statistics.empty() ? nullptr : m_statistics.data();
        this->m_model.tags = m_tags.empty() ? nullptr : m_tags.data();
        this->m_model.titleId = m_titleId.empty() ? nullptr : m_titleId.data();
        this->m_model.totalValueToDateInUSD = m_totalValueToDateInUSD ? m_totalValueToDateInUSD.operator->() : nullptr;
        this->m_model.valuesToDate = m_valuesToDate.empty() ? nullptr : m_valuesToDate.data();
        this->m_model.virtualCurrencyBalances = m_virtualCurrencyBalances.empty() ? nullptr : m_virtualCurrencyBalances.data();
    }

    ModelVector<PFSegmentsAdCampaignAttributionWrapper<Alloc>, Alloc> m_adCampaignAttributions;
    String m_avatarUrl;
    std::optional<time_t> m_bannedUntil;
    std::optional<PFSegmentsChurnRiskLevel> m_churnPrediction;
    ModelVector<PFSegmentsContactEmailInfoWrapper<Alloc>, Alloc> m_contactEmailAddresses;
    std::optional<time_t> m_created;
    String m_displayName;
    std::optional<time_t> m_lastLogin;
    ModelVector<PFSegmentsPlayerLinkedAccountWrapper<Alloc>, Alloc> m_linkedAccounts;
    ModelDictionaryEntryVector<PFSegmentsPlayerLocationWrapper<Alloc>, Alloc> m_locations;
    std::optional<PFLoginIdentityProvider> m_origination;
    CStringVector<Alloc> m_playerExperimentVariants;
    String m_playerId;
    ModelVector<PFSegmentsPlayerStatisticWrapper<Alloc>, Alloc> m_playerStatistics;
    String m_publisherId;
    ModelVector<PFSegmentsPushNotificationRegistrationWrapper<Alloc>, Alloc> m_pushNotificationRegistrations;
    DictionaryEntryVector<PFInt32DictionaryEntry, Alloc> m_statistics;
    CStringVector<Alloc> m_tags;
    String m_titleId;
    std::optional<uint32_t> m_totalValueToDateInUSD;
    DictionaryEntryVector<PFUint32DictionaryEntry, Alloc> m_valuesToDate;
    DictionaryEntryVector<PFInt32DictionaryEntry, Alloc> m_virtualCurrencyBalances;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFSegmentsGetPlayersInSegmentResultWrapper : public ModelWrapper<PFSegmentsGetPlayersInSegmentResult, Alloc>
{
public:
    using ModelType = PFSegmentsGetPlayersInSegmentResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFSegmentsGetPlayersInSegmentResultWrapper() = default;

    PFSegmentsGetPlayersInSegmentResultWrapper(const PFSegmentsGetPlayersInSegmentResult& model) :
        ModelWrapper<PFSegmentsGetPlayersInSegmentResult, Alloc>{ model },
        m_continuationToken{ SafeString(model.continuationToken) },
        m_playerProfiles{ model.playerProfiles, model.playerProfiles + model.playerProfilesCount }
    {
        SetModelPointers();
    }

    PFSegmentsGetPlayersInSegmentResultWrapper(const PFSegmentsGetPlayersInSegmentResultWrapper& src) :
        PFSegmentsGetPlayersInSegmentResultWrapper{ src.Model() }
    {
    }

    PFSegmentsGetPlayersInSegmentResultWrapper(PFSegmentsGetPlayersInSegmentResultWrapper&& src) :
        PFSegmentsGetPlayersInSegmentResultWrapper{}
    {
        swap(*this, src);
    }

    PFSegmentsGetPlayersInSegmentResultWrapper& operator=(PFSegmentsGetPlayersInSegmentResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFSegmentsGetPlayersInSegmentResultWrapper() = default;

    friend void swap(PFSegmentsGetPlayersInSegmentResultWrapper& lhs, PFSegmentsGetPlayersInSegmentResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_continuationToken, rhs.m_continuationToken);
        swap(lhs.m_playerProfiles, rhs.m_playerProfiles);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetContinuationToken(String value)
    {
        m_continuationToken = std::move(value);
        this->m_model.continuationToken =  m_continuationToken.empty() ? nullptr : m_continuationToken.data();
    }

    void SetPlayerProfiles(ModelVector<PFSegmentsPlayerProfileWrapper<Alloc>, Alloc> value)
    {
        m_playerProfiles = std::move(value);
        this->m_model.playerProfiles =  m_playerProfiles.empty() ? nullptr : m_playerProfiles.data();
        this->m_model.playerProfilesCount =  static_cast<uint32_t>(m_playerProfiles.size());
    }

    void SetProfilesInSegment(int32_t value)
    {
        this->m_model.profilesInSegment = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.continuationToken = m_continuationToken.empty() ? nullptr : m_continuationToken.data();
        this->m_model.playerProfiles = m_playerProfiles.empty() ? nullptr : m_playerProfiles.data();
    }

    String m_continuationToken;
    ModelVector<PFSegmentsPlayerProfileWrapper<Alloc>, Alloc> m_playerProfiles;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFSegmentsRemovePlayerTagRequestWrapper : public ModelWrapper<PFSegmentsRemovePlayerTagRequest, Alloc>
{
public:
    using ModelType = PFSegmentsRemovePlayerTagRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFSegmentsRemovePlayerTagRequestWrapper() = default;

    PFSegmentsRemovePlayerTagRequestWrapper(const PFSegmentsRemovePlayerTagRequest& model) :
        ModelWrapper<PFSegmentsRemovePlayerTagRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_playFabId{ SafeString(model.playFabId) },
        m_tagName{ SafeString(model.tagName) }
    {
        SetModelPointers();
    }

    PFSegmentsRemovePlayerTagRequestWrapper(const PFSegmentsRemovePlayerTagRequestWrapper& src) :
        PFSegmentsRemovePlayerTagRequestWrapper{ src.Model() }
    {
    }

    PFSegmentsRemovePlayerTagRequestWrapper(PFSegmentsRemovePlayerTagRequestWrapper&& src) :
        PFSegmentsRemovePlayerTagRequestWrapper{}
    {
        swap(*this, src);
    }

    PFSegmentsRemovePlayerTagRequestWrapper& operator=(PFSegmentsRemovePlayerTagRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFSegmentsRemovePlayerTagRequestWrapper() = default;

    friend void swap(PFSegmentsRemovePlayerTagRequestWrapper& lhs, PFSegmentsRemovePlayerTagRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_tagName, rhs.m_tagName);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetTagName(String value)
    {
        m_tagName = std::move(value);
        this->m_model.tagName =  m_tagName.empty() ? nullptr : m_tagName.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.tagName = m_tagName.empty() ? nullptr : m_tagName.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_playFabId;
    String m_tagName;
};

} // namespace Wrappers
} // namespace PlayFab

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

    String const& GetABTestParent() const
    {
        return m_aBTestParent;
    }

    void SetABTestParent(String value)
    {
        m_aBTestParent = std::move(value);
        this->m_model.aBTestParent =  m_aBTestParent.empty() ? nullptr : m_aBTestParent.data();
    }

    String const& GetId() const
    {
        return m_id;
    }

    void SetId(String value)
    {
        m_id = std::move(value);
        this->m_model.id =  m_id.empty() ? nullptr : m_id.data();
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

    ModelVector<PFSegmentsGetSegmentResultWrapper<Alloc>, Alloc> const& GetSegments() const
    {
        return m_segments;
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

    String const& GetplayfabNamespace() const
    {
        return m_playfabNamespace;
    }

    void SetplayfabNamespace(String value)
    {
        m_playfabNamespace = std::move(value);
        this->m_model.playfabNamespace =  m_playfabNamespace.empty() ? nullptr : m_playfabNamespace.data();
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

    String const& GetPlayFabId() const
    {
        return m_playFabId;
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    CStringVector<Alloc> const& GetTags() const
    {
        return m_tags;
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

    String const& GetPlayFabId() const
    {
        return m_playFabId;
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String const& GetTagName() const
    {
        return m_tagName;
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

    ModelVector<PFSegmentsGetSegmentResultWrapper<Alloc>, Alloc> const& GetSegments() const
    {
        return m_segments;
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
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_playFabId;
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

    String const& GetPlayFabId() const
    {
        return m_playFabId;
    }

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    String const& GetTagName() const
    {
        return m_tagName;
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

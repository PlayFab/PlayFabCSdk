// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFPlayStreamTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayStreamExportPlayersInSegmentRequestWrapper : public ModelWrapper<PFPlayStreamExportPlayersInSegmentRequest, Alloc>
{
public:
    using ModelType = PFPlayStreamExportPlayersInSegmentRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayStreamExportPlayersInSegmentRequestWrapper() = default;

    PFPlayStreamExportPlayersInSegmentRequestWrapper(const PFPlayStreamExportPlayersInSegmentRequest& model) :
        ModelWrapper<PFPlayStreamExportPlayersInSegmentRequest, Alloc>{ model },
        m_segmentId{ SafeString(model.segmentId) }
    {
        SetModelPointers();
    }

    PFPlayStreamExportPlayersInSegmentRequestWrapper(const PFPlayStreamExportPlayersInSegmentRequestWrapper& src) :
        PFPlayStreamExportPlayersInSegmentRequestWrapper{ src.Model() }
    {
    }

    PFPlayStreamExportPlayersInSegmentRequestWrapper(PFPlayStreamExportPlayersInSegmentRequestWrapper&& src) :
        PFPlayStreamExportPlayersInSegmentRequestWrapper{}
    {
        swap(*this, src);
    }

    PFPlayStreamExportPlayersInSegmentRequestWrapper& operator=(PFPlayStreamExportPlayersInSegmentRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayStreamExportPlayersInSegmentRequestWrapper() = default;

    friend void swap(PFPlayStreamExportPlayersInSegmentRequestWrapper& lhs, PFPlayStreamExportPlayersInSegmentRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_segmentId, rhs.m_segmentId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetSegmentId() const
    {
        return m_segmentId;
    }

    void SetSegmentId(String value)
    {
        m_segmentId = std::move(value);
        this->m_model.segmentId =  m_segmentId.empty() ? nullptr : m_segmentId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.segmentId = m_segmentId.empty() ? nullptr : m_segmentId.data();
    }

    String m_segmentId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayStreamExportPlayersInSegmentResultWrapper : public ModelWrapper<PFPlayStreamExportPlayersInSegmentResult, Alloc>
{
public:
    using ModelType = PFPlayStreamExportPlayersInSegmentResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayStreamExportPlayersInSegmentResultWrapper() = default;

    PFPlayStreamExportPlayersInSegmentResultWrapper(const PFPlayStreamExportPlayersInSegmentResult& model) :
        ModelWrapper<PFPlayStreamExportPlayersInSegmentResult, Alloc>{ model },
        m_exportId{ SafeString(model.exportId) },
        m_segmentId{ SafeString(model.segmentId) }
    {
        SetModelPointers();
    }

    PFPlayStreamExportPlayersInSegmentResultWrapper(const PFPlayStreamExportPlayersInSegmentResultWrapper& src) :
        PFPlayStreamExportPlayersInSegmentResultWrapper{ src.Model() }
    {
    }

    PFPlayStreamExportPlayersInSegmentResultWrapper(PFPlayStreamExportPlayersInSegmentResultWrapper&& src) :
        PFPlayStreamExportPlayersInSegmentResultWrapper{}
    {
        swap(*this, src);
    }

    PFPlayStreamExportPlayersInSegmentResultWrapper& operator=(PFPlayStreamExportPlayersInSegmentResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayStreamExportPlayersInSegmentResultWrapper() = default;

    friend void swap(PFPlayStreamExportPlayersInSegmentResultWrapper& lhs, PFPlayStreamExportPlayersInSegmentResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_exportId, rhs.m_exportId);
        swap(lhs.m_segmentId, rhs.m_segmentId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetExportId() const
    {
        return m_exportId;
    }

    void SetExportId(String value)
    {
        m_exportId = std::move(value);
        this->m_model.exportId =  m_exportId.empty() ? nullptr : m_exportId.data();
    }

    String const& GetSegmentId() const
    {
        return m_segmentId;
    }

    void SetSegmentId(String value)
    {
        m_segmentId = std::move(value);
        this->m_model.segmentId =  m_segmentId.empty() ? nullptr : m_segmentId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.exportId = m_exportId.empty() ? nullptr : m_exportId.data();
        this->m_model.segmentId = m_segmentId.empty() ? nullptr : m_segmentId.data();
    }

    String m_exportId;
    String m_segmentId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayStreamGetPlayersInSegmentExportRequestWrapper : public ModelWrapper<PFPlayStreamGetPlayersInSegmentExportRequest, Alloc>
{
public:
    using ModelType = PFPlayStreamGetPlayersInSegmentExportRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayStreamGetPlayersInSegmentExportRequestWrapper() = default;

    PFPlayStreamGetPlayersInSegmentExportRequestWrapper(const PFPlayStreamGetPlayersInSegmentExportRequest& model) :
        ModelWrapper<PFPlayStreamGetPlayersInSegmentExportRequest, Alloc>{ model },
        m_exportId{ SafeString(model.exportId) }
    {
        SetModelPointers();
    }

    PFPlayStreamGetPlayersInSegmentExportRequestWrapper(const PFPlayStreamGetPlayersInSegmentExportRequestWrapper& src) :
        PFPlayStreamGetPlayersInSegmentExportRequestWrapper{ src.Model() }
    {
    }

    PFPlayStreamGetPlayersInSegmentExportRequestWrapper(PFPlayStreamGetPlayersInSegmentExportRequestWrapper&& src) :
        PFPlayStreamGetPlayersInSegmentExportRequestWrapper{}
    {
        swap(*this, src);
    }

    PFPlayStreamGetPlayersInSegmentExportRequestWrapper& operator=(PFPlayStreamGetPlayersInSegmentExportRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayStreamGetPlayersInSegmentExportRequestWrapper() = default;

    friend void swap(PFPlayStreamGetPlayersInSegmentExportRequestWrapper& lhs, PFPlayStreamGetPlayersInSegmentExportRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_exportId, rhs.m_exportId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetExportId() const
    {
        return m_exportId;
    }

    void SetExportId(String value)
    {
        m_exportId = std::move(value);
        this->m_model.exportId =  m_exportId.empty() ? nullptr : m_exportId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.exportId = m_exportId.empty() ? nullptr : m_exportId.data();
    }

    String m_exportId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayStreamGetPlayersInSegmentExportResponseWrapper : public ModelWrapper<PFPlayStreamGetPlayersInSegmentExportResponse, Alloc>
{
public:
    using ModelType = PFPlayStreamGetPlayersInSegmentExportResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayStreamGetPlayersInSegmentExportResponseWrapper() = default;

    PFPlayStreamGetPlayersInSegmentExportResponseWrapper(const PFPlayStreamGetPlayersInSegmentExportResponse& model) :
        ModelWrapper<PFPlayStreamGetPlayersInSegmentExportResponse, Alloc>{ model },
        m_indexUrl{ SafeString(model.indexUrl) },
        m_state{ SafeString(model.state) }
    {
        SetModelPointers();
    }

    PFPlayStreamGetPlayersInSegmentExportResponseWrapper(const PFPlayStreamGetPlayersInSegmentExportResponseWrapper& src) :
        PFPlayStreamGetPlayersInSegmentExportResponseWrapper{ src.Model() }
    {
    }

    PFPlayStreamGetPlayersInSegmentExportResponseWrapper(PFPlayStreamGetPlayersInSegmentExportResponseWrapper&& src) :
        PFPlayStreamGetPlayersInSegmentExportResponseWrapper{}
    {
        swap(*this, src);
    }

    PFPlayStreamGetPlayersInSegmentExportResponseWrapper& operator=(PFPlayStreamGetPlayersInSegmentExportResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayStreamGetPlayersInSegmentExportResponseWrapper() = default;

    friend void swap(PFPlayStreamGetPlayersInSegmentExportResponseWrapper& lhs, PFPlayStreamGetPlayersInSegmentExportResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_indexUrl, rhs.m_indexUrl);
        swap(lhs.m_state, rhs.m_state);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetIndexUrl() const
    {
        return m_indexUrl;
    }

    void SetIndexUrl(String value)
    {
        m_indexUrl = std::move(value);
        this->m_model.indexUrl =  m_indexUrl.empty() ? nullptr : m_indexUrl.data();
    }

    String const& GetState() const
    {
        return m_state;
    }

    void SetState(String value)
    {
        m_state = std::move(value);
        this->m_model.state =  m_state.empty() ? nullptr : m_state.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.indexUrl = m_indexUrl.empty() ? nullptr : m_indexUrl.data();
        this->m_model.state = m_state.empty() ? nullptr : m_state.data();
    }

    String m_indexUrl;
    String m_state;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayStreamGetSegmentPlayerCountRequestWrapper : public ModelWrapper<PFPlayStreamGetSegmentPlayerCountRequest, Alloc>
{
public:
    using ModelType = PFPlayStreamGetSegmentPlayerCountRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFPlayStreamGetSegmentPlayerCountRequestWrapper() = default;

    PFPlayStreamGetSegmentPlayerCountRequestWrapper(const PFPlayStreamGetSegmentPlayerCountRequest& model) :
        ModelWrapper<PFPlayStreamGetSegmentPlayerCountRequest, Alloc>{ model },
        m_segmentId{ SafeString(model.segmentId) }
    {
        SetModelPointers();
    }

    PFPlayStreamGetSegmentPlayerCountRequestWrapper(const PFPlayStreamGetSegmentPlayerCountRequestWrapper& src) :
        PFPlayStreamGetSegmentPlayerCountRequestWrapper{ src.Model() }
    {
    }

    PFPlayStreamGetSegmentPlayerCountRequestWrapper(PFPlayStreamGetSegmentPlayerCountRequestWrapper&& src) :
        PFPlayStreamGetSegmentPlayerCountRequestWrapper{}
    {
        swap(*this, src);
    }

    PFPlayStreamGetSegmentPlayerCountRequestWrapper& operator=(PFPlayStreamGetSegmentPlayerCountRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFPlayStreamGetSegmentPlayerCountRequestWrapper() = default;

    friend void swap(PFPlayStreamGetSegmentPlayerCountRequestWrapper& lhs, PFPlayStreamGetSegmentPlayerCountRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_segmentId, rhs.m_segmentId);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    String const& GetSegmentId() const
    {
        return m_segmentId;
    }

    void SetSegmentId(String value)
    {
        m_segmentId = std::move(value);
        this->m_model.segmentId =  m_segmentId.empty() ? nullptr : m_segmentId.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.segmentId = m_segmentId.empty() ? nullptr : m_segmentId.data();
    }

    String m_segmentId;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFPlayStreamGetSegmentPlayerCountResultWrapper : public ModelWrapper<PFPlayStreamGetSegmentPlayerCountResult, Alloc>
{
public:
    using ModelType = PFPlayStreamGetSegmentPlayerCountResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    using ModelWrapper<PFPlayStreamGetSegmentPlayerCountResult, Alloc>::ModelWrapper;

    int32_t GetProfilesInSegment() const
    {
        return this->m_model.profilesInSegment;
    }

    void SetProfilesInSegment(int32_t value)
    {
        this->m_model.profilesInSegment = value;
    }

private:
};

} // namespace Wrappers
} // namespace PlayFab

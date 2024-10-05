// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFTitleDataManagementTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFTitleDataManagementGetPublisherDataRequestWrapper : public ModelWrapper<PFTitleDataManagementGetPublisherDataRequest, Alloc>
{
public:
    using ModelType = PFTitleDataManagementGetPublisherDataRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFTitleDataManagementGetPublisherDataRequestWrapper() = default;

    PFTitleDataManagementGetPublisherDataRequestWrapper(const PFTitleDataManagementGetPublisherDataRequest& model) :
        ModelWrapper<PFTitleDataManagementGetPublisherDataRequest, Alloc>{ model },
        m_keys{ model.keys, model.keys + model.keysCount }
    {
        SetModelPointers();
    }

    PFTitleDataManagementGetPublisherDataRequestWrapper(const PFTitleDataManagementGetPublisherDataRequestWrapper& src) :
        PFTitleDataManagementGetPublisherDataRequestWrapper{ src.Model() }
    {
    }

    PFTitleDataManagementGetPublisherDataRequestWrapper(PFTitleDataManagementGetPublisherDataRequestWrapper&& src) :
        PFTitleDataManagementGetPublisherDataRequestWrapper{}
    {
        swap(*this, src);
    }

    PFTitleDataManagementGetPublisherDataRequestWrapper& operator=(PFTitleDataManagementGetPublisherDataRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFTitleDataManagementGetPublisherDataRequestWrapper() = default;

    friend void swap(PFTitleDataManagementGetPublisherDataRequestWrapper& lhs, PFTitleDataManagementGetPublisherDataRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_keys, rhs.m_keys);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetKeys(CStringVector<Alloc> value)
    {
        m_keys = std::move(value);
        this->m_model.keys =  m_keys.empty() ? nullptr : m_keys.data();
        this->m_model.keysCount =  static_cast<uint32_t>(m_keys.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.keys = m_keys.empty() ? nullptr : m_keys.data();
    }

    CStringVector<Alloc> m_keys;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFTitleDataManagementGetPublisherDataResultWrapper : public ModelWrapper<PFTitleDataManagementGetPublisherDataResult, Alloc>
{
public:
    using ModelType = PFTitleDataManagementGetPublisherDataResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFTitleDataManagementGetPublisherDataResultWrapper() = default;

    PFTitleDataManagementGetPublisherDataResultWrapper(const PFTitleDataManagementGetPublisherDataResult& model) :
        ModelWrapper<PFTitleDataManagementGetPublisherDataResult, Alloc>{ model },
        m_data{ model.data, model.data + model.dataCount }
    {
        SetModelPointers();
    }

    PFTitleDataManagementGetPublisherDataResultWrapper(const PFTitleDataManagementGetPublisherDataResultWrapper& src) :
        PFTitleDataManagementGetPublisherDataResultWrapper{ src.Model() }
    {
    }

    PFTitleDataManagementGetPublisherDataResultWrapper(PFTitleDataManagementGetPublisherDataResultWrapper&& src) :
        PFTitleDataManagementGetPublisherDataResultWrapper{}
    {
        swap(*this, src);
    }

    PFTitleDataManagementGetPublisherDataResultWrapper& operator=(PFTitleDataManagementGetPublisherDataResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFTitleDataManagementGetPublisherDataResultWrapper() = default;

    friend void swap(PFTitleDataManagementGetPublisherDataResultWrapper& lhs, PFTitleDataManagementGetPublisherDataResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_data, rhs.m_data);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetData(StringDictionaryEntryVector<Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
    }

    StringDictionaryEntryVector<Alloc> m_data;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFTitleDataManagementGetTimeResultWrapper : public ModelWrapper<PFTitleDataManagementGetTimeResult, Alloc>
{
public:
    using ModelType = PFTitleDataManagementGetTimeResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    using ModelWrapper<PFTitleDataManagementGetTimeResult, Alloc>::ModelWrapper;

    void SetTime(time_t value)
    {
        this->m_model.time = value;
    }

private:
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFTitleDataManagementGetTitleDataRequestWrapper : public ModelWrapper<PFTitleDataManagementGetTitleDataRequest, Alloc>
{
public:
    using ModelType = PFTitleDataManagementGetTitleDataRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFTitleDataManagementGetTitleDataRequestWrapper() = default;

    PFTitleDataManagementGetTitleDataRequestWrapper(const PFTitleDataManagementGetTitleDataRequest& model) :
        ModelWrapper<PFTitleDataManagementGetTitleDataRequest, Alloc>{ model },
        m_keys{ model.keys, model.keys + model.keysCount },
        m_overrideLabel{ SafeString(model.overrideLabel) }
    {
        SetModelPointers();
    }

    PFTitleDataManagementGetTitleDataRequestWrapper(const PFTitleDataManagementGetTitleDataRequestWrapper& src) :
        PFTitleDataManagementGetTitleDataRequestWrapper{ src.Model() }
    {
    }

    PFTitleDataManagementGetTitleDataRequestWrapper(PFTitleDataManagementGetTitleDataRequestWrapper&& src) :
        PFTitleDataManagementGetTitleDataRequestWrapper{}
    {
        swap(*this, src);
    }

    PFTitleDataManagementGetTitleDataRequestWrapper& operator=(PFTitleDataManagementGetTitleDataRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFTitleDataManagementGetTitleDataRequestWrapper() = default;

    friend void swap(PFTitleDataManagementGetTitleDataRequestWrapper& lhs, PFTitleDataManagementGetTitleDataRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_keys, rhs.m_keys);
        swap(lhs.m_overrideLabel, rhs.m_overrideLabel);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetKeys(CStringVector<Alloc> value)
    {
        m_keys = std::move(value);
        this->m_model.keys =  m_keys.empty() ? nullptr : m_keys.data();
        this->m_model.keysCount =  static_cast<uint32_t>(m_keys.size());
    }

    void SetOverrideLabel(String value)
    {
        m_overrideLabel = std::move(value);
        this->m_model.overrideLabel =  m_overrideLabel.empty() ? nullptr : m_overrideLabel.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.keys = m_keys.empty() ? nullptr : m_keys.data();
        this->m_model.overrideLabel = m_overrideLabel.empty() ? nullptr : m_overrideLabel.data();
    }

    CStringVector<Alloc> m_keys;
    String m_overrideLabel;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFTitleDataManagementGetTitleDataResultWrapper : public ModelWrapper<PFTitleDataManagementGetTitleDataResult, Alloc>
{
public:
    using ModelType = PFTitleDataManagementGetTitleDataResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFTitleDataManagementGetTitleDataResultWrapper() = default;

    PFTitleDataManagementGetTitleDataResultWrapper(const PFTitleDataManagementGetTitleDataResult& model) :
        ModelWrapper<PFTitleDataManagementGetTitleDataResult, Alloc>{ model },
        m_data{ model.data, model.data + model.dataCount }
    {
        SetModelPointers();
    }

    PFTitleDataManagementGetTitleDataResultWrapper(const PFTitleDataManagementGetTitleDataResultWrapper& src) :
        PFTitleDataManagementGetTitleDataResultWrapper{ src.Model() }
    {
    }

    PFTitleDataManagementGetTitleDataResultWrapper(PFTitleDataManagementGetTitleDataResultWrapper&& src) :
        PFTitleDataManagementGetTitleDataResultWrapper{}
    {
        swap(*this, src);
    }

    PFTitleDataManagementGetTitleDataResultWrapper& operator=(PFTitleDataManagementGetTitleDataResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFTitleDataManagementGetTitleDataResultWrapper() = default;

    friend void swap(PFTitleDataManagementGetTitleDataResultWrapper& lhs, PFTitleDataManagementGetTitleDataResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_data, rhs.m_data);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetData(StringDictionaryEntryVector<Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data =  m_data.empty() ? nullptr : m_data.data();
        this->m_model.dataCount =  static_cast<uint32_t>(m_data.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.data = m_data.empty() ? nullptr : m_data.data();
    }

    StringDictionaryEntryVector<Alloc> m_data;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFTitleDataManagementGetTitleNewsRequestWrapper : public ModelWrapper<PFTitleDataManagementGetTitleNewsRequest, Alloc>
{
public:
    using ModelType = PFTitleDataManagementGetTitleNewsRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFTitleDataManagementGetTitleNewsRequestWrapper() = default;

    PFTitleDataManagementGetTitleNewsRequestWrapper(const PFTitleDataManagementGetTitleNewsRequest& model) :
        ModelWrapper<PFTitleDataManagementGetTitleNewsRequest, Alloc>{ model },
        m_count{ model.count ? std::optional<int32_t>{ *model.count } : std::nullopt }
    {
        SetModelPointers();
    }

    PFTitleDataManagementGetTitleNewsRequestWrapper(const PFTitleDataManagementGetTitleNewsRequestWrapper& src) :
        PFTitleDataManagementGetTitleNewsRequestWrapper{ src.Model() }
    {
    }

    PFTitleDataManagementGetTitleNewsRequestWrapper(PFTitleDataManagementGetTitleNewsRequestWrapper&& src) :
        PFTitleDataManagementGetTitleNewsRequestWrapper{}
    {
        swap(*this, src);
    }

    PFTitleDataManagementGetTitleNewsRequestWrapper& operator=(PFTitleDataManagementGetTitleNewsRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFTitleDataManagementGetTitleNewsRequestWrapper() = default;

    friend void swap(PFTitleDataManagementGetTitleNewsRequestWrapper& lhs, PFTitleDataManagementGetTitleNewsRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_count, rhs.m_count);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCount(std::optional<int32_t> value)
    {
        m_count = std::move(value);
        this->m_model.count = m_count ? m_count.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.count = m_count ? m_count.operator->() : nullptr;
    }

    std::optional<int32_t> m_count;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFTitleDataManagementTitleNewsItemWrapper : public ModelWrapper<PFTitleDataManagementTitleNewsItem, Alloc>
{
public:
    using ModelType = PFTitleDataManagementTitleNewsItem;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFTitleDataManagementTitleNewsItemWrapper() = default;

    PFTitleDataManagementTitleNewsItemWrapper(const PFTitleDataManagementTitleNewsItem& model) :
        ModelWrapper<PFTitleDataManagementTitleNewsItem, Alloc>{ model },
        m_body{ SafeString(model.body) },
        m_newsId{ SafeString(model.newsId) },
        m_title{ SafeString(model.title) }
    {
        SetModelPointers();
    }

    PFTitleDataManagementTitleNewsItemWrapper(const PFTitleDataManagementTitleNewsItemWrapper& src) :
        PFTitleDataManagementTitleNewsItemWrapper{ src.Model() }
    {
    }

    PFTitleDataManagementTitleNewsItemWrapper(PFTitleDataManagementTitleNewsItemWrapper&& src) :
        PFTitleDataManagementTitleNewsItemWrapper{}
    {
        swap(*this, src);
    }

    PFTitleDataManagementTitleNewsItemWrapper& operator=(PFTitleDataManagementTitleNewsItemWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFTitleDataManagementTitleNewsItemWrapper() = default;

    friend void swap(PFTitleDataManagementTitleNewsItemWrapper& lhs, PFTitleDataManagementTitleNewsItemWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_body, rhs.m_body);
        swap(lhs.m_newsId, rhs.m_newsId);
        swap(lhs.m_title, rhs.m_title);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetBody(String value)
    {
        m_body = std::move(value);
        this->m_model.body =  m_body.empty() ? nullptr : m_body.data();
    }

    void SetNewsId(String value)
    {
        m_newsId = std::move(value);
        this->m_model.newsId =  m_newsId.empty() ? nullptr : m_newsId.data();
    }

    void SetTimestamp(time_t value)
    {
        this->m_model.timestamp = value;
    }

    void SetTitle(String value)
    {
        m_title = std::move(value);
        this->m_model.title =  m_title.empty() ? nullptr : m_title.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.body = m_body.empty() ? nullptr : m_body.data();
        this->m_model.newsId = m_newsId.empty() ? nullptr : m_newsId.data();
        this->m_model.title = m_title.empty() ? nullptr : m_title.data();
    }

    String m_body;
    String m_newsId;
    String m_title;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFTitleDataManagementGetTitleNewsResultWrapper : public ModelWrapper<PFTitleDataManagementGetTitleNewsResult, Alloc>
{
public:
    using ModelType = PFTitleDataManagementGetTitleNewsResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFTitleDataManagementGetTitleNewsResultWrapper() = default;

    PFTitleDataManagementGetTitleNewsResultWrapper(const PFTitleDataManagementGetTitleNewsResult& model) :
        ModelWrapper<PFTitleDataManagementGetTitleNewsResult, Alloc>{ model },
        m_news{ model.news, model.news + model.newsCount }
    {
        SetModelPointers();
    }

    PFTitleDataManagementGetTitleNewsResultWrapper(const PFTitleDataManagementGetTitleNewsResultWrapper& src) :
        PFTitleDataManagementGetTitleNewsResultWrapper{ src.Model() }
    {
    }

    PFTitleDataManagementGetTitleNewsResultWrapper(PFTitleDataManagementGetTitleNewsResultWrapper&& src) :
        PFTitleDataManagementGetTitleNewsResultWrapper{}
    {
        swap(*this, src);
    }

    PFTitleDataManagementGetTitleNewsResultWrapper& operator=(PFTitleDataManagementGetTitleNewsResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFTitleDataManagementGetTitleNewsResultWrapper() = default;

    friend void swap(PFTitleDataManagementGetTitleNewsResultWrapper& lhs, PFTitleDataManagementGetTitleNewsResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_news, rhs.m_news);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetNews(ModelVector<PFTitleDataManagementTitleNewsItemWrapper<Alloc>, Alloc> value)
    {
        m_news = std::move(value);
        this->m_model.news =  m_news.empty() ? nullptr : m_news.data();
        this->m_model.newsCount =  static_cast<uint32_t>(m_news.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.news = m_news.empty() ? nullptr : m_news.data();
    }

    ModelVector<PFTitleDataManagementTitleNewsItemWrapper<Alloc>, Alloc> m_news;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFTitleDataManagementSetPublisherDataRequestWrapper : public ModelWrapper<PFTitleDataManagementSetPublisherDataRequest, Alloc>
{
public:
    using ModelType = PFTitleDataManagementSetPublisherDataRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFTitleDataManagementSetPublisherDataRequestWrapper() = default;

    PFTitleDataManagementSetPublisherDataRequestWrapper(const PFTitleDataManagementSetPublisherDataRequest& model) :
        ModelWrapper<PFTitleDataManagementSetPublisherDataRequest, Alloc>{ model },
        m_key{ SafeString(model.key) },
        m_value{ SafeString(model.value) }
    {
        SetModelPointers();
    }

    PFTitleDataManagementSetPublisherDataRequestWrapper(const PFTitleDataManagementSetPublisherDataRequestWrapper& src) :
        PFTitleDataManagementSetPublisherDataRequestWrapper{ src.Model() }
    {
    }

    PFTitleDataManagementSetPublisherDataRequestWrapper(PFTitleDataManagementSetPublisherDataRequestWrapper&& src) :
        PFTitleDataManagementSetPublisherDataRequestWrapper{}
    {
        swap(*this, src);
    }

    PFTitleDataManagementSetPublisherDataRequestWrapper& operator=(PFTitleDataManagementSetPublisherDataRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFTitleDataManagementSetPublisherDataRequestWrapper() = default;

    friend void swap(PFTitleDataManagementSetPublisherDataRequestWrapper& lhs, PFTitleDataManagementSetPublisherDataRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_key, rhs.m_key);
        swap(lhs.m_value, rhs.m_value);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetKey(String value)
    {
        m_key = std::move(value);
        this->m_model.key =  m_key.empty() ? nullptr : m_key.data();
    }

    void SetValue(String value)
    {
        m_value = std::move(value);
        this->m_model.value =  m_value.empty() ? nullptr : m_value.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.key = m_key.empty() ? nullptr : m_key.data();
        this->m_model.value = m_value.empty() ? nullptr : m_value.data();
    }

    String m_key;
    String m_value;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFTitleDataManagementSetTitleDataRequestWrapper : public ModelWrapper<PFTitleDataManagementSetTitleDataRequest, Alloc>
{
public:
    using ModelType = PFTitleDataManagementSetTitleDataRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFTitleDataManagementSetTitleDataRequestWrapper() = default;

    PFTitleDataManagementSetTitleDataRequestWrapper(const PFTitleDataManagementSetTitleDataRequest& model) :
        ModelWrapper<PFTitleDataManagementSetTitleDataRequest, Alloc>{ model },
        m_key{ SafeString(model.key) },
        m_value{ SafeString(model.value) }
    {
        SetModelPointers();
    }

    PFTitleDataManagementSetTitleDataRequestWrapper(const PFTitleDataManagementSetTitleDataRequestWrapper& src) :
        PFTitleDataManagementSetTitleDataRequestWrapper{ src.Model() }
    {
    }

    PFTitleDataManagementSetTitleDataRequestWrapper(PFTitleDataManagementSetTitleDataRequestWrapper&& src) :
        PFTitleDataManagementSetTitleDataRequestWrapper{}
    {
        swap(*this, src);
    }

    PFTitleDataManagementSetTitleDataRequestWrapper& operator=(PFTitleDataManagementSetTitleDataRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFTitleDataManagementSetTitleDataRequestWrapper() = default;

    friend void swap(PFTitleDataManagementSetTitleDataRequestWrapper& lhs, PFTitleDataManagementSetTitleDataRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_key, rhs.m_key);
        swap(lhs.m_value, rhs.m_value);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetKey(String value)
    {
        m_key = std::move(value);
        this->m_model.key =  m_key.empty() ? nullptr : m_key.data();
    }

    void SetValue(String value)
    {
        m_value = std::move(value);
        this->m_model.value =  m_value.empty() ? nullptr : m_value.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.key = m_key.empty() ? nullptr : m_key.data();
        this->m_model.value = m_value.empty() ? nullptr : m_value.data();
    }

    String m_key;
    String m_value;
};

} // namespace Wrappers
} // namespace PlayFab

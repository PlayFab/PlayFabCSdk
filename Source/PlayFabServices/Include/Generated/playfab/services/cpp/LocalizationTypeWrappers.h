// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFLocalizationTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFLocalizationGetLanguageListRequestWrapper : public ModelWrapper<PFLocalizationGetLanguageListRequest, Alloc>
{
public:
    using ModelType = PFLocalizationGetLanguageListRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLocalizationGetLanguageListRequestWrapper() = default;

    PFLocalizationGetLanguageListRequestWrapper(const PFLocalizationGetLanguageListRequest& model) :
        ModelWrapper<PFLocalizationGetLanguageListRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount }
    {
        SetModelPointers();
    }

    PFLocalizationGetLanguageListRequestWrapper(const PFLocalizationGetLanguageListRequestWrapper& src) :
        PFLocalizationGetLanguageListRequestWrapper{ src.Model() }
    {
    }

    PFLocalizationGetLanguageListRequestWrapper(PFLocalizationGetLanguageListRequestWrapper&& src) :
        PFLocalizationGetLanguageListRequestWrapper{}
    {
        swap(*this, src);
    }

    PFLocalizationGetLanguageListRequestWrapper& operator=(PFLocalizationGetLanguageListRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLocalizationGetLanguageListRequestWrapper() = default;

    friend void swap(PFLocalizationGetLanguageListRequestWrapper& lhs, PFLocalizationGetLanguageListRequestWrapper& rhs)
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
class PFLocalizationGetLanguageListResponseWrapper : public ModelWrapper<PFLocalizationGetLanguageListResponse, Alloc>
{
public:
    using ModelType = PFLocalizationGetLanguageListResponse;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFLocalizationGetLanguageListResponseWrapper() = default;

    PFLocalizationGetLanguageListResponseWrapper(const PFLocalizationGetLanguageListResponse& model) :
        ModelWrapper<PFLocalizationGetLanguageListResponse, Alloc>{ model },
        m_languageList{ model.languageList, model.languageList + model.languageListCount }
    {
        SetModelPointers();
    }

    PFLocalizationGetLanguageListResponseWrapper(const PFLocalizationGetLanguageListResponseWrapper& src) :
        PFLocalizationGetLanguageListResponseWrapper{ src.Model() }
    {
    }

    PFLocalizationGetLanguageListResponseWrapper(PFLocalizationGetLanguageListResponseWrapper&& src) :
        PFLocalizationGetLanguageListResponseWrapper{}
    {
        swap(*this, src);
    }

    PFLocalizationGetLanguageListResponseWrapper& operator=(PFLocalizationGetLanguageListResponseWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFLocalizationGetLanguageListResponseWrapper() = default;

    friend void swap(PFLocalizationGetLanguageListResponseWrapper& lhs, PFLocalizationGetLanguageListResponseWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_languageList, rhs.m_languageList);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetLanguageList(CStringVector<Alloc> value)
    {
        m_languageList = std::move(value);
        this->m_model.languageList =  m_languageList.empty() ? nullptr : m_languageList.data();
        this->m_model.languageListCount =  static_cast<uint32_t>(m_languageList.size());
    }

private:
    void SetModelPointers()
    {
        this->m_model.languageList = m_languageList.empty() ? nullptr : m_languageList.data();
    }

    CStringVector<Alloc> m_languageList;
};

} // namespace Wrappers
} // namespace PlayFab

// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFExperimentationTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFExperimentationGetTreatmentAssignmentRequestWrapper : public ModelWrapper<PFExperimentationGetTreatmentAssignmentRequest, Alloc>
{
public:
    using ModelType = PFExperimentationGetTreatmentAssignmentRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFExperimentationGetTreatmentAssignmentRequestWrapper() = default;

    PFExperimentationGetTreatmentAssignmentRequestWrapper(const PFExperimentationGetTreatmentAssignmentRequest& model) :
        ModelWrapper<PFExperimentationGetTreatmentAssignmentRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt }
    {
        SetModelPointers();
    }

    PFExperimentationGetTreatmentAssignmentRequestWrapper(const PFExperimentationGetTreatmentAssignmentRequestWrapper& src) :
        PFExperimentationGetTreatmentAssignmentRequestWrapper{ src.Model() }
    {
    }

    PFExperimentationGetTreatmentAssignmentRequestWrapper(PFExperimentationGetTreatmentAssignmentRequestWrapper&& src) :
        PFExperimentationGetTreatmentAssignmentRequestWrapper{}
    {
        swap(*this, src);
    }

    PFExperimentationGetTreatmentAssignmentRequestWrapper& operator=(PFExperimentationGetTreatmentAssignmentRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFExperimentationGetTreatmentAssignmentRequestWrapper() = default;

    friend void swap(PFExperimentationGetTreatmentAssignmentRequestWrapper& lhs, PFExperimentationGetTreatmentAssignmentRequestWrapper& rhs)
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
class PFExperimentationGetTreatmentAssignmentResultWrapper : public ModelWrapper<PFExperimentationGetTreatmentAssignmentResult, Alloc>
{
public:
    using ModelType = PFExperimentationGetTreatmentAssignmentResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFExperimentationGetTreatmentAssignmentResultWrapper() = default;

    PFExperimentationGetTreatmentAssignmentResultWrapper(const PFExperimentationGetTreatmentAssignmentResult& model) :
        ModelWrapper<PFExperimentationGetTreatmentAssignmentResult, Alloc>{ model },
        m_treatmentAssignment{ model.treatmentAssignment ? std::optional<PFTreatmentAssignmentWrapper<Alloc>>{ *model.treatmentAssignment } : std::nullopt }
    {
        SetModelPointers();
    }

    PFExperimentationGetTreatmentAssignmentResultWrapper(const PFExperimentationGetTreatmentAssignmentResultWrapper& src) :
        PFExperimentationGetTreatmentAssignmentResultWrapper{ src.Model() }
    {
    }

    PFExperimentationGetTreatmentAssignmentResultWrapper(PFExperimentationGetTreatmentAssignmentResultWrapper&& src) :
        PFExperimentationGetTreatmentAssignmentResultWrapper{}
    {
        swap(*this, src);
    }

    PFExperimentationGetTreatmentAssignmentResultWrapper& operator=(PFExperimentationGetTreatmentAssignmentResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFExperimentationGetTreatmentAssignmentResultWrapper() = default;

    friend void swap(PFExperimentationGetTreatmentAssignmentResultWrapper& lhs, PFExperimentationGetTreatmentAssignmentResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_treatmentAssignment, rhs.m_treatmentAssignment);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetTreatmentAssignment(std::optional<PFTreatmentAssignmentWrapper<Alloc>> value)
    {
        m_treatmentAssignment = std::move(value);
        this->m_model.treatmentAssignment = m_treatmentAssignment ? &m_treatmentAssignment->Model() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.treatmentAssignment = m_treatmentAssignment ?  &m_treatmentAssignment->Model() : nullptr;
    }

    std::optional<PFTreatmentAssignmentWrapper<Alloc>> m_treatmentAssignment;
};

} // namespace Wrappers
} // namespace PlayFab

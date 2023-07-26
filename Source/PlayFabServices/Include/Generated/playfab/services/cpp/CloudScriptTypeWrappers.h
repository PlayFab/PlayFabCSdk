// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFCloudScriptTypes.h>
#include <playfab/services/cpp/TypeWrappers.h>

namespace PlayFab
{
namespace Wrappers
{

template<template<typename AllocT> class Alloc = std::allocator>
class PFCloudScriptExecuteFunctionRequestWrapper : public ModelWrapper<PFCloudScriptExecuteFunctionRequest, Alloc>
{
public:
    using ModelType = PFCloudScriptExecuteFunctionRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCloudScriptExecuteFunctionRequestWrapper() = default;

    PFCloudScriptExecuteFunctionRequestWrapper(const PFCloudScriptExecuteFunctionRequest& model) :
        ModelWrapper<PFCloudScriptExecuteFunctionRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_functionName{ SafeString(model.functionName) },
        m_functionParameter{ model.functionParameter },
        m_generatePlayStreamEvent{ model.generatePlayStreamEvent ? std::optional<bool>{ *model.generatePlayStreamEvent } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCloudScriptExecuteFunctionRequestWrapper(const PFCloudScriptExecuteFunctionRequestWrapper& src) :
        PFCloudScriptExecuteFunctionRequestWrapper{ src.Model() }
    {
    }

    PFCloudScriptExecuteFunctionRequestWrapper(PFCloudScriptExecuteFunctionRequestWrapper&& src) :
        PFCloudScriptExecuteFunctionRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCloudScriptExecuteFunctionRequestWrapper& operator=(PFCloudScriptExecuteFunctionRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCloudScriptExecuteFunctionRequestWrapper() = default;

    friend void swap(PFCloudScriptExecuteFunctionRequestWrapper& lhs, PFCloudScriptExecuteFunctionRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_functionName, rhs.m_functionName);
        swap(lhs.m_functionParameter, rhs.m_functionParameter);
        swap(lhs.m_generatePlayStreamEvent, rhs.m_generatePlayStreamEvent);
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

    void SetFunctionName(String value)
    {
        m_functionName = std::move(value);
        this->m_model.functionName =  m_functionName.empty() ? nullptr : m_functionName.data();
    }

    void SetFunctionParameter(JsonObject<Alloc> value)
    {
        m_functionParameter = std::move(value);
        this->m_model.functionParameter.stringValue = m_functionParameter.stringValue.empty() ? nullptr : m_functionParameter.stringValue.data();
    }

    void SetGeneratePlayStreamEvent(std::optional<bool> value)
    {
        m_generatePlayStreamEvent = std::move(value);
        this->m_model.generatePlayStreamEvent = m_generatePlayStreamEvent ? m_generatePlayStreamEvent.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.functionName = m_functionName.empty() ? nullptr : m_functionName.data();
        this->m_model.functionParameter.stringValue = m_functionParameter.stringValue.empty() ? nullptr : m_functionParameter.stringValue.data();
        this->m_model.generatePlayStreamEvent = m_generatePlayStreamEvent ? m_generatePlayStreamEvent.operator->() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_functionName;
    JsonObject<Alloc> m_functionParameter;
    std::optional<bool> m_generatePlayStreamEvent;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCloudScriptFunctionExecutionErrorWrapper : public ModelWrapper<PFCloudScriptFunctionExecutionError, Alloc>
{
public:
    using ModelType = PFCloudScriptFunctionExecutionError;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCloudScriptFunctionExecutionErrorWrapper() = default;

    PFCloudScriptFunctionExecutionErrorWrapper(const PFCloudScriptFunctionExecutionError& model) :
        ModelWrapper<PFCloudScriptFunctionExecutionError, Alloc>{ model },
        m_error{ SafeString(model.error) },
        m_message{ SafeString(model.message) },
        m_stackTrace{ SafeString(model.stackTrace) }
    {
        SetModelPointers();
    }

    PFCloudScriptFunctionExecutionErrorWrapper(const PFCloudScriptFunctionExecutionErrorWrapper& src) :
        PFCloudScriptFunctionExecutionErrorWrapper{ src.Model() }
    {
    }

    PFCloudScriptFunctionExecutionErrorWrapper(PFCloudScriptFunctionExecutionErrorWrapper&& src) :
        PFCloudScriptFunctionExecutionErrorWrapper{}
    {
        swap(*this, src);
    }

    PFCloudScriptFunctionExecutionErrorWrapper& operator=(PFCloudScriptFunctionExecutionErrorWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCloudScriptFunctionExecutionErrorWrapper() = default;

    friend void swap(PFCloudScriptFunctionExecutionErrorWrapper& lhs, PFCloudScriptFunctionExecutionErrorWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_error, rhs.m_error);
        swap(lhs.m_message, rhs.m_message);
        swap(lhs.m_stackTrace, rhs.m_stackTrace);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetError(String value)
    {
        m_error = std::move(value);
        this->m_model.error =  m_error.empty() ? nullptr : m_error.data();
    }

    void SetMessage(String value)
    {
        m_message = std::move(value);
        this->m_model.message =  m_message.empty() ? nullptr : m_message.data();
    }

    void SetStackTrace(String value)
    {
        m_stackTrace = std::move(value);
        this->m_model.stackTrace =  m_stackTrace.empty() ? nullptr : m_stackTrace.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.error = m_error.empty() ? nullptr : m_error.data();
        this->m_model.message = m_message.empty() ? nullptr : m_message.data();
        this->m_model.stackTrace = m_stackTrace.empty() ? nullptr : m_stackTrace.data();
    }

    String m_error;
    String m_message;
    String m_stackTrace;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCloudScriptExecuteFunctionResultWrapper : public ModelWrapper<PFCloudScriptExecuteFunctionResult, Alloc>
{
public:
    using ModelType = PFCloudScriptExecuteFunctionResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCloudScriptExecuteFunctionResultWrapper() = default;

    PFCloudScriptExecuteFunctionResultWrapper(const PFCloudScriptExecuteFunctionResult& model) :
        ModelWrapper<PFCloudScriptExecuteFunctionResult, Alloc>{ model },
        m_error{ model.error ? std::optional<PFCloudScriptFunctionExecutionErrorWrapper<Alloc>>{ *model.error } : std::nullopt },
        m_functionName{ SafeString(model.functionName) },
        m_functionResult{ model.functionResult },
        m_functionResultTooLarge{ model.functionResultTooLarge ? std::optional<bool>{ *model.functionResultTooLarge } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCloudScriptExecuteFunctionResultWrapper(const PFCloudScriptExecuteFunctionResultWrapper& src) :
        PFCloudScriptExecuteFunctionResultWrapper{ src.Model() }
    {
    }

    PFCloudScriptExecuteFunctionResultWrapper(PFCloudScriptExecuteFunctionResultWrapper&& src) :
        PFCloudScriptExecuteFunctionResultWrapper{}
    {
        swap(*this, src);
    }

    PFCloudScriptExecuteFunctionResultWrapper& operator=(PFCloudScriptExecuteFunctionResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCloudScriptExecuteFunctionResultWrapper() = default;

    friend void swap(PFCloudScriptExecuteFunctionResultWrapper& lhs, PFCloudScriptExecuteFunctionResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_error, rhs.m_error);
        swap(lhs.m_functionName, rhs.m_functionName);
        swap(lhs.m_functionResult, rhs.m_functionResult);
        swap(lhs.m_functionResultTooLarge, rhs.m_functionResultTooLarge);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetError(std::optional<PFCloudScriptFunctionExecutionErrorWrapper<Alloc>> value)
    {
        m_error = std::move(value);
        this->m_model.error = m_error ? &m_error->Model() : nullptr;
    }

    void SetExecutionTimeMilliseconds(int32_t value)
    {
        this->m_model.executionTimeMilliseconds = value;
    }

    void SetFunctionName(String value)
    {
        m_functionName = std::move(value);
        this->m_model.functionName =  m_functionName.empty() ? nullptr : m_functionName.data();
    }

    void SetFunctionResult(JsonObject<Alloc> value)
    {
        m_functionResult = std::move(value);
        this->m_model.functionResult.stringValue = m_functionResult.stringValue.empty() ? nullptr : m_functionResult.stringValue.data();
    }

    void SetFunctionResultTooLarge(std::optional<bool> value)
    {
        m_functionResultTooLarge = std::move(value);
        this->m_model.functionResultTooLarge = m_functionResultTooLarge ? m_functionResultTooLarge.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.error = m_error ?  &m_error->Model() : nullptr;
        this->m_model.functionName = m_functionName.empty() ? nullptr : m_functionName.data();
        this->m_model.functionResult.stringValue = m_functionResult.stringValue.empty() ? nullptr : m_functionResult.stringValue.data();
        this->m_model.functionResultTooLarge = m_functionResultTooLarge ? m_functionResultTooLarge.operator->() : nullptr;
    }

    std::optional<PFCloudScriptFunctionExecutionErrorWrapper<Alloc>> m_error;
    String m_functionName;
    JsonObject<Alloc> m_functionResult;
    std::optional<bool> m_functionResultTooLarge;
};

} // namespace Wrappers
} // namespace PlayFab

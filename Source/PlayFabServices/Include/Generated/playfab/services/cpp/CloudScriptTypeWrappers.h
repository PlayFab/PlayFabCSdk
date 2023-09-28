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
class PFCloudScriptExecuteCloudScriptRequestWrapper : public ModelWrapper<PFCloudScriptExecuteCloudScriptRequest, Alloc>
{
public:
    using ModelType = PFCloudScriptExecuteCloudScriptRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCloudScriptExecuteCloudScriptRequestWrapper() = default;

    PFCloudScriptExecuteCloudScriptRequestWrapper(const PFCloudScriptExecuteCloudScriptRequest& model) :
        ModelWrapper<PFCloudScriptExecuteCloudScriptRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_functionName{ SafeString(model.functionName) },
        m_functionParameter{ model.functionParameter },
        m_generatePlayStreamEvent{ model.generatePlayStreamEvent ? std::optional<bool>{ *model.generatePlayStreamEvent } : std::nullopt },
        m_revisionSelection{ model.revisionSelection ? std::optional<PFCloudScriptCloudScriptRevisionOption>{ *model.revisionSelection } : std::nullopt },
        m_specificRevision{ model.specificRevision ? std::optional<int32_t>{ *model.specificRevision } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCloudScriptExecuteCloudScriptRequestWrapper(const PFCloudScriptExecuteCloudScriptRequestWrapper& src) :
        PFCloudScriptExecuteCloudScriptRequestWrapper{ src.Model() }
    {
    }

    PFCloudScriptExecuteCloudScriptRequestWrapper(PFCloudScriptExecuteCloudScriptRequestWrapper&& src) :
        PFCloudScriptExecuteCloudScriptRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCloudScriptExecuteCloudScriptRequestWrapper& operator=(PFCloudScriptExecuteCloudScriptRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCloudScriptExecuteCloudScriptRequestWrapper() = default;

    friend void swap(PFCloudScriptExecuteCloudScriptRequestWrapper& lhs, PFCloudScriptExecuteCloudScriptRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_functionName, rhs.m_functionName);
        swap(lhs.m_functionParameter, rhs.m_functionParameter);
        swap(lhs.m_generatePlayStreamEvent, rhs.m_generatePlayStreamEvent);
        swap(lhs.m_revisionSelection, rhs.m_revisionSelection);
        swap(lhs.m_specificRevision, rhs.m_specificRevision);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
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

    void SetRevisionSelection(std::optional<PFCloudScriptCloudScriptRevisionOption> value)
    {
        m_revisionSelection = std::move(value);
        this->m_model.revisionSelection = m_revisionSelection ? m_revisionSelection.operator->() : nullptr;
    }

    void SetSpecificRevision(std::optional<int32_t> value)
    {
        m_specificRevision = std::move(value);
        this->m_model.specificRevision = m_specificRevision ? m_specificRevision.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.functionName = m_functionName.empty() ? nullptr : m_functionName.data();
        this->m_model.functionParameter.stringValue = m_functionParameter.stringValue.empty() ? nullptr : m_functionParameter.stringValue.data();
        this->m_model.generatePlayStreamEvent = m_generatePlayStreamEvent ? m_generatePlayStreamEvent.operator->() : nullptr;
        this->m_model.revisionSelection = m_revisionSelection ? m_revisionSelection.operator->() : nullptr;
        this->m_model.specificRevision = m_specificRevision ? m_specificRevision.operator->() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_functionName;
    JsonObject<Alloc> m_functionParameter;
    std::optional<bool> m_generatePlayStreamEvent;
    std::optional<PFCloudScriptCloudScriptRevisionOption> m_revisionSelection;
    std::optional<int32_t> m_specificRevision;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCloudScriptScriptExecutionErrorWrapper : public ModelWrapper<PFCloudScriptScriptExecutionError, Alloc>
{
public:
    using ModelType = PFCloudScriptScriptExecutionError;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCloudScriptScriptExecutionErrorWrapper() = default;

    PFCloudScriptScriptExecutionErrorWrapper(const PFCloudScriptScriptExecutionError& model) :
        ModelWrapper<PFCloudScriptScriptExecutionError, Alloc>{ model },
        m_error{ SafeString(model.error) },
        m_message{ SafeString(model.message) },
        m_stackTrace{ SafeString(model.stackTrace) }
    {
        SetModelPointers();
    }

    PFCloudScriptScriptExecutionErrorWrapper(const PFCloudScriptScriptExecutionErrorWrapper& src) :
        PFCloudScriptScriptExecutionErrorWrapper{ src.Model() }
    {
    }

    PFCloudScriptScriptExecutionErrorWrapper(PFCloudScriptScriptExecutionErrorWrapper&& src) :
        PFCloudScriptScriptExecutionErrorWrapper{}
    {
        swap(*this, src);
    }

    PFCloudScriptScriptExecutionErrorWrapper& operator=(PFCloudScriptScriptExecutionErrorWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCloudScriptScriptExecutionErrorWrapper() = default;

    friend void swap(PFCloudScriptScriptExecutionErrorWrapper& lhs, PFCloudScriptScriptExecutionErrorWrapper& rhs)
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
class PFCloudScriptLogStatementWrapper : public ModelWrapper<PFCloudScriptLogStatement, Alloc>
{
public:
    using ModelType = PFCloudScriptLogStatement;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCloudScriptLogStatementWrapper() = default;

    PFCloudScriptLogStatementWrapper(const PFCloudScriptLogStatement& model) :
        ModelWrapper<PFCloudScriptLogStatement, Alloc>{ model },
        m_data{ model.data },
        m_level{ SafeString(model.level) },
        m_message{ SafeString(model.message) }
    {
        SetModelPointers();
    }

    PFCloudScriptLogStatementWrapper(const PFCloudScriptLogStatementWrapper& src) :
        PFCloudScriptLogStatementWrapper{ src.Model() }
    {
    }

    PFCloudScriptLogStatementWrapper(PFCloudScriptLogStatementWrapper&& src) :
        PFCloudScriptLogStatementWrapper{}
    {
        swap(*this, src);
    }

    PFCloudScriptLogStatementWrapper& operator=(PFCloudScriptLogStatementWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCloudScriptLogStatementWrapper() = default;

    friend void swap(PFCloudScriptLogStatementWrapper& lhs, PFCloudScriptLogStatementWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_data, rhs.m_data);
        swap(lhs.m_level, rhs.m_level);
        swap(lhs.m_message, rhs.m_message);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetData(JsonObject<Alloc> value)
    {
        m_data = std::move(value);
        this->m_model.data.stringValue = m_data.stringValue.empty() ? nullptr : m_data.stringValue.data();
    }

    void SetLevel(String value)
    {
        m_level = std::move(value);
        this->m_model.level =  m_level.empty() ? nullptr : m_level.data();
    }

    void SetMessage(String value)
    {
        m_message = std::move(value);
        this->m_model.message =  m_message.empty() ? nullptr : m_message.data();
    }

private:
    void SetModelPointers()
    {
        this->m_model.data.stringValue = m_data.stringValue.empty() ? nullptr : m_data.stringValue.data();
        this->m_model.level = m_level.empty() ? nullptr : m_level.data();
        this->m_model.message = m_message.empty() ? nullptr : m_message.data();
    }

    JsonObject<Alloc> m_data;
    String m_level;
    String m_message;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCloudScriptExecuteCloudScriptResultWrapper : public ModelWrapper<PFCloudScriptExecuteCloudScriptResult, Alloc>
{
public:
    using ModelType = PFCloudScriptExecuteCloudScriptResult;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCloudScriptExecuteCloudScriptResultWrapper() = default;

    PFCloudScriptExecuteCloudScriptResultWrapper(const PFCloudScriptExecuteCloudScriptResult& model) :
        ModelWrapper<PFCloudScriptExecuteCloudScriptResult, Alloc>{ model },
        m_error{ model.error ? std::optional<PFCloudScriptScriptExecutionErrorWrapper<Alloc>>{ *model.error } : std::nullopt },
        m_functionName{ SafeString(model.functionName) },
        m_functionResult{ model.functionResult },
        m_functionResultTooLarge{ model.functionResultTooLarge ? std::optional<bool>{ *model.functionResultTooLarge } : std::nullopt },
        m_logs{ model.logs, model.logs + model.logsCount },
        m_logsTooLarge{ model.logsTooLarge ? std::optional<bool>{ *model.logsTooLarge } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCloudScriptExecuteCloudScriptResultWrapper(const PFCloudScriptExecuteCloudScriptResultWrapper& src) :
        PFCloudScriptExecuteCloudScriptResultWrapper{ src.Model() }
    {
    }

    PFCloudScriptExecuteCloudScriptResultWrapper(PFCloudScriptExecuteCloudScriptResultWrapper&& src) :
        PFCloudScriptExecuteCloudScriptResultWrapper{}
    {
        swap(*this, src);
    }

    PFCloudScriptExecuteCloudScriptResultWrapper& operator=(PFCloudScriptExecuteCloudScriptResultWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCloudScriptExecuteCloudScriptResultWrapper() = default;

    friend void swap(PFCloudScriptExecuteCloudScriptResultWrapper& lhs, PFCloudScriptExecuteCloudScriptResultWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_error, rhs.m_error);
        swap(lhs.m_functionName, rhs.m_functionName);
        swap(lhs.m_functionResult, rhs.m_functionResult);
        swap(lhs.m_functionResultTooLarge, rhs.m_functionResultTooLarge);
        swap(lhs.m_logs, rhs.m_logs);
        swap(lhs.m_logsTooLarge, rhs.m_logsTooLarge);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetAPIRequestsIssued(int32_t value)
    {
        this->m_model.aPIRequestsIssued = value;
    }

    void SetError(std::optional<PFCloudScriptScriptExecutionErrorWrapper<Alloc>> value)
    {
        m_error = std::move(value);
        this->m_model.error = m_error ? &m_error->Model() : nullptr;
    }

    void SetExecutionTimeSeconds(double value)
    {
        this->m_model.executionTimeSeconds = value;
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

    void SetHttpRequestsIssued(int32_t value)
    {
        this->m_model.httpRequestsIssued = value;
    }

    void SetLogs(ModelVector<PFCloudScriptLogStatementWrapper<Alloc>, Alloc> value)
    {
        m_logs = std::move(value);
        this->m_model.logs =  m_logs.empty() ? nullptr : m_logs.data();
        this->m_model.logsCount =  static_cast<uint32_t>(m_logs.size());
    }

    void SetLogsTooLarge(std::optional<bool> value)
    {
        m_logsTooLarge = std::move(value);
        this->m_model.logsTooLarge = m_logsTooLarge ? m_logsTooLarge.operator->() : nullptr;
    }

    void SetMemoryConsumedBytes(uint32_t value)
    {
        this->m_model.memoryConsumedBytes = value;
    }

    void SetProcessorTimeSeconds(double value)
    {
        this->m_model.processorTimeSeconds = value;
    }

    void SetRevision(int32_t value)
    {
        this->m_model.revision = value;
    }

private:
    void SetModelPointers()
    {
        this->m_model.error = m_error ?  &m_error->Model() : nullptr;
        this->m_model.functionName = m_functionName.empty() ? nullptr : m_functionName.data();
        this->m_model.functionResult.stringValue = m_functionResult.stringValue.empty() ? nullptr : m_functionResult.stringValue.data();
        this->m_model.functionResultTooLarge = m_functionResultTooLarge ? m_functionResultTooLarge.operator->() : nullptr;
        this->m_model.logs = m_logs.empty() ? nullptr : m_logs.data();
        this->m_model.logsTooLarge = m_logsTooLarge ? m_logsTooLarge.operator->() : nullptr;
    }

    std::optional<PFCloudScriptScriptExecutionErrorWrapper<Alloc>> m_error;
    String m_functionName;
    JsonObject<Alloc> m_functionResult;
    std::optional<bool> m_functionResultTooLarge;
    ModelVector<PFCloudScriptLogStatementWrapper<Alloc>, Alloc> m_logs;
    std::optional<bool> m_logsTooLarge;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCloudScriptExecuteCloudScriptServerRequestWrapper : public ModelWrapper<PFCloudScriptExecuteCloudScriptServerRequest, Alloc>
{
public:
    using ModelType = PFCloudScriptExecuteCloudScriptServerRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCloudScriptExecuteCloudScriptServerRequestWrapper() = default;

    PFCloudScriptExecuteCloudScriptServerRequestWrapper(const PFCloudScriptExecuteCloudScriptServerRequest& model) :
        ModelWrapper<PFCloudScriptExecuteCloudScriptServerRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_functionName{ SafeString(model.functionName) },
        m_functionParameter{ model.functionParameter },
        m_generatePlayStreamEvent{ model.generatePlayStreamEvent ? std::optional<bool>{ *model.generatePlayStreamEvent } : std::nullopt },
        m_playFabId{ SafeString(model.playFabId) },
        m_revisionSelection{ model.revisionSelection ? std::optional<PFCloudScriptCloudScriptRevisionOption>{ *model.revisionSelection } : std::nullopt },
        m_specificRevision{ model.specificRevision ? std::optional<int32_t>{ *model.specificRevision } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCloudScriptExecuteCloudScriptServerRequestWrapper(const PFCloudScriptExecuteCloudScriptServerRequestWrapper& src) :
        PFCloudScriptExecuteCloudScriptServerRequestWrapper{ src.Model() }
    {
    }

    PFCloudScriptExecuteCloudScriptServerRequestWrapper(PFCloudScriptExecuteCloudScriptServerRequestWrapper&& src) :
        PFCloudScriptExecuteCloudScriptServerRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCloudScriptExecuteCloudScriptServerRequestWrapper& operator=(PFCloudScriptExecuteCloudScriptServerRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCloudScriptExecuteCloudScriptServerRequestWrapper() = default;

    friend void swap(PFCloudScriptExecuteCloudScriptServerRequestWrapper& lhs, PFCloudScriptExecuteCloudScriptServerRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_functionName, rhs.m_functionName);
        swap(lhs.m_functionParameter, rhs.m_functionParameter);
        swap(lhs.m_generatePlayStreamEvent, rhs.m_generatePlayStreamEvent);
        swap(lhs.m_playFabId, rhs.m_playFabId);
        swap(lhs.m_revisionSelection, rhs.m_revisionSelection);
        swap(lhs.m_specificRevision, rhs.m_specificRevision);
        lhs.SetModelPointers();
        rhs.SetModelPointers();
    }

    void SetCustomTags(StringDictionaryEntryVector<Alloc> value)
    {
        m_customTags = std::move(value);
        this->m_model.customTags =  m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.customTagsCount =  static_cast<uint32_t>(m_customTags.size());
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

    void SetPlayFabId(String value)
    {
        m_playFabId = std::move(value);
        this->m_model.playFabId =  m_playFabId.empty() ? nullptr : m_playFabId.data();
    }

    void SetRevisionSelection(std::optional<PFCloudScriptCloudScriptRevisionOption> value)
    {
        m_revisionSelection = std::move(value);
        this->m_model.revisionSelection = m_revisionSelection ? m_revisionSelection.operator->() : nullptr;
    }

    void SetSpecificRevision(std::optional<int32_t> value)
    {
        m_specificRevision = std::move(value);
        this->m_model.specificRevision = m_specificRevision ? m_specificRevision.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.functionName = m_functionName.empty() ? nullptr : m_functionName.data();
        this->m_model.functionParameter.stringValue = m_functionParameter.stringValue.empty() ? nullptr : m_functionParameter.stringValue.data();
        this->m_model.generatePlayStreamEvent = m_generatePlayStreamEvent ? m_generatePlayStreamEvent.operator->() : nullptr;
        this->m_model.playFabId = m_playFabId.empty() ? nullptr : m_playFabId.data();
        this->m_model.revisionSelection = m_revisionSelection ? m_revisionSelection.operator->() : nullptr;
        this->m_model.specificRevision = m_specificRevision ? m_specificRevision.operator->() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    String m_functionName;
    JsonObject<Alloc> m_functionParameter;
    std::optional<bool> m_generatePlayStreamEvent;
    String m_playFabId;
    std::optional<PFCloudScriptCloudScriptRevisionOption> m_revisionSelection;
    std::optional<int32_t> m_specificRevision;
};

template<template<typename AllocT> class Alloc = std::allocator>
class PFCloudScriptExecuteEntityCloudScriptRequestWrapper : public ModelWrapper<PFCloudScriptExecuteEntityCloudScriptRequest, Alloc>
{
public:
    using ModelType = PFCloudScriptExecuteEntityCloudScriptRequest;
    using String = typename std::basic_string<char, std::char_traits<char>, Alloc<char>>;
    template<typename T> using Vector = typename std::vector<T, Alloc<T>>;

    PFCloudScriptExecuteEntityCloudScriptRequestWrapper() = default;

    PFCloudScriptExecuteEntityCloudScriptRequestWrapper(const PFCloudScriptExecuteEntityCloudScriptRequest& model) :
        ModelWrapper<PFCloudScriptExecuteEntityCloudScriptRequest, Alloc>{ model },
        m_customTags{ model.customTags, model.customTags + model.customTagsCount },
        m_entity{ model.entity ? std::optional<PFEntityKeyWrapper<Alloc>>{ *model.entity } : std::nullopt },
        m_functionName{ SafeString(model.functionName) },
        m_functionParameter{ model.functionParameter },
        m_generatePlayStreamEvent{ model.generatePlayStreamEvent ? std::optional<bool>{ *model.generatePlayStreamEvent } : std::nullopt },
        m_revisionSelection{ model.revisionSelection ? std::optional<PFCloudScriptCloudScriptRevisionOption>{ *model.revisionSelection } : std::nullopt },
        m_specificRevision{ model.specificRevision ? std::optional<int32_t>{ *model.specificRevision } : std::nullopt }
    {
        SetModelPointers();
    }

    PFCloudScriptExecuteEntityCloudScriptRequestWrapper(const PFCloudScriptExecuteEntityCloudScriptRequestWrapper& src) :
        PFCloudScriptExecuteEntityCloudScriptRequestWrapper{ src.Model() }
    {
    }

    PFCloudScriptExecuteEntityCloudScriptRequestWrapper(PFCloudScriptExecuteEntityCloudScriptRequestWrapper&& src) :
        PFCloudScriptExecuteEntityCloudScriptRequestWrapper{}
    {
        swap(*this, src);
    }

    PFCloudScriptExecuteEntityCloudScriptRequestWrapper& operator=(PFCloudScriptExecuteEntityCloudScriptRequestWrapper src) 
    {
        swap(*this, src);
        return *this;
    }

    virtual ~PFCloudScriptExecuteEntityCloudScriptRequestWrapper() = default;

    friend void swap(PFCloudScriptExecuteEntityCloudScriptRequestWrapper& lhs, PFCloudScriptExecuteEntityCloudScriptRequestWrapper& rhs)
    {
        using std::swap;
        swap(lhs.m_model, rhs.m_model);
        swap(lhs.m_customTags, rhs.m_customTags);
        swap(lhs.m_entity, rhs.m_entity);
        swap(lhs.m_functionName, rhs.m_functionName);
        swap(lhs.m_functionParameter, rhs.m_functionParameter);
        swap(lhs.m_generatePlayStreamEvent, rhs.m_generatePlayStreamEvent);
        swap(lhs.m_revisionSelection, rhs.m_revisionSelection);
        swap(lhs.m_specificRevision, rhs.m_specificRevision);
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

    void SetRevisionSelection(std::optional<PFCloudScriptCloudScriptRevisionOption> value)
    {
        m_revisionSelection = std::move(value);
        this->m_model.revisionSelection = m_revisionSelection ? m_revisionSelection.operator->() : nullptr;
    }

    void SetSpecificRevision(std::optional<int32_t> value)
    {
        m_specificRevision = std::move(value);
        this->m_model.specificRevision = m_specificRevision ? m_specificRevision.operator->() : nullptr;
    }

private:
    void SetModelPointers()
    {
        this->m_model.customTags = m_customTags.empty() ? nullptr : m_customTags.data();
        this->m_model.entity = m_entity ?  &m_entity->Model() : nullptr;
        this->m_model.functionName = m_functionName.empty() ? nullptr : m_functionName.data();
        this->m_model.functionParameter.stringValue = m_functionParameter.stringValue.empty() ? nullptr : m_functionParameter.stringValue.data();
        this->m_model.generatePlayStreamEvent = m_generatePlayStreamEvent ? m_generatePlayStreamEvent.operator->() : nullptr;
        this->m_model.revisionSelection = m_revisionSelection ? m_revisionSelection.operator->() : nullptr;
        this->m_model.specificRevision = m_specificRevision ? m_specificRevision.operator->() : nullptr;
    }

    StringDictionaryEntryVector<Alloc> m_customTags;
    std::optional<PFEntityKeyWrapper<Alloc>> m_entity;
    String m_functionName;
    JsonObject<Alloc> m_functionParameter;
    std::optional<bool> m_generatePlayStreamEvent;
    std::optional<PFCloudScriptCloudScriptRevisionOption> m_revisionSelection;
    std::optional<int32_t> m_specificRevision;
};

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

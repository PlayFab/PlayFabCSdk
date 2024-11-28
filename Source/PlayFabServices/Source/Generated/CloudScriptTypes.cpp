#include "stdafx.h"
#include "CloudScriptTypes.h"
#include "JsonUtils.h"
#include "Types.h"

namespace PlayFab
{
namespace CloudScript
{

JsonValue ExecuteCloudScriptRequest::ToJson() const
{
    return ExecuteCloudScriptRequest::ToJson(this->Model());
}

JsonValue ExecuteCloudScriptRequest::ToJson(const PFCloudScriptExecuteCloudScriptRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "FunctionName", input.functionName);
    JsonUtils::ObjectAddMember(output, "FunctionParameter", input.functionParameter);
    JsonUtils::ObjectAddMember(output, "GeneratePlayStreamEvent", input.generatePlayStreamEvent);
    JsonUtils::ObjectAddMember(output, "RevisionSelection", input.revisionSelection);
    JsonUtils::ObjectAddMember(output, "SpecificRevision", input.specificRevision);
    return output;
}

HRESULT ScriptExecutionError::FromJson(const JsonValue& input)
{
    String error{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Error", error));
    this->SetError(std::move(error));

    String message{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Message", message));
    this->SetMessage(std::move(message));

    String stackTrace{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "StackTrace", stackTrace));
    this->SetStackTrace(std::move(stackTrace));

    return S_OK;
}

size_t ScriptExecutionError::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCloudScriptScriptExecutionError const*> ScriptExecutionError::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ScriptExecutionError>(&this->Model());
}

size_t ScriptExecutionError::RequiredBufferSize(const PFCloudScriptScriptExecutionError& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.error)
    {
        requiredSize += (std::strlen(model.error) + 1);
    }
    if (model.message)
    {
        requiredSize += (std::strlen(model.message) + 1);
    }
    if (model.stackTrace)
    {
        requiredSize += (std::strlen(model.stackTrace) + 1);
    }
    return requiredSize;
}

HRESULT ScriptExecutionError::Copy(const PFCloudScriptScriptExecutionError& input, PFCloudScriptScriptExecutionError& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.error);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.error = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.message);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.message = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.stackTrace);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.stackTrace = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT LogStatement::FromJson(const JsonValue& input)
{
    JsonObject data{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Data", data));
    this->SetData(std::move(data));

    String level{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Level", level));
    this->SetLevel(std::move(level));

    String message{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Message", message));
    this->SetMessage(std::move(message));

    return S_OK;
}

size_t LogStatement::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCloudScriptLogStatement const*> LogStatement::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<LogStatement>(&this->Model());
}

size_t LogStatement::RequiredBufferSize(const PFCloudScriptLogStatement& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.data.stringValue)
    {
    requiredSize += (std::strlen(model.data.stringValue) + 1);
    }
    if (model.level)
    {
        requiredSize += (std::strlen(model.level) + 1);
    }
    if (model.message)
    {
        requiredSize += (std::strlen(model.message) + 1);
    }
    return requiredSize;
}

HRESULT LogStatement::Copy(const PFCloudScriptLogStatement& input, PFCloudScriptLogStatement& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.data.stringValue);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.data.stringValue = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.level);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.level = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.message);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.message = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT ExecuteCloudScriptResult::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "APIRequestsIssued", this->m_model.aPIRequestsIssued));

    std::optional<ScriptExecutionError> error{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Error", error));
    if (error)
    {
        this->SetError(std::move(*error));
    }

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ExecutionTimeSeconds", this->m_model.executionTimeSeconds));

    String functionName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FunctionName", functionName));
    this->SetFunctionName(std::move(functionName));

    JsonObject functionResult{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FunctionResult", functionResult));
    this->SetFunctionResult(std::move(functionResult));

    std::optional<bool> functionResultTooLarge{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FunctionResultTooLarge", functionResultTooLarge));
    this->SetFunctionResultTooLarge(std::move(functionResultTooLarge));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "HttpRequestsIssued", this->m_model.httpRequestsIssued));

    ModelVector<LogStatement> logs{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<LogStatement>(input, "Logs", logs));
    this->SetLogs(std::move(logs));

    std::optional<bool> logsTooLarge{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "LogsTooLarge", logsTooLarge));
    this->SetLogsTooLarge(std::move(logsTooLarge));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "MemoryConsumedBytes", this->m_model.memoryConsumedBytes));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ProcessorTimeSeconds", this->m_model.processorTimeSeconds));

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Revision", this->m_model.revision));

    return S_OK;
}

size_t ExecuteCloudScriptResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCloudScriptExecuteCloudScriptResult const*> ExecuteCloudScriptResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ExecuteCloudScriptResult>(&this->Model());
}

size_t ExecuteCloudScriptResult::RequiredBufferSize(const PFCloudScriptExecuteCloudScriptResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.error)
    {
        requiredSize += ScriptExecutionError::RequiredBufferSize(*model.error);
    }
    if (model.functionName)
    {
        requiredSize += (std::strlen(model.functionName) + 1);
    }
    if (model.functionResult.stringValue)
    {
    requiredSize += (std::strlen(model.functionResult.stringValue) + 1);
    }
    if (model.functionResultTooLarge)
    {
        requiredSize += (alignof(bool) + sizeof(bool));
    }
    requiredSize += (alignof(PFCloudScriptLogStatement*) + sizeof(PFCloudScriptLogStatement*) * model.logsCount);
    for (size_t i = 0; i < model.logsCount; ++i)
    {
        requiredSize += LogStatement::RequiredBufferSize(*model.logs[i]);
    }
    if (model.logsTooLarge)
    {
        requiredSize += (alignof(bool) + sizeof(bool));
    }
    return requiredSize;
}

HRESULT ExecuteCloudScriptResult::Copy(const PFCloudScriptExecuteCloudScriptResult& input, PFCloudScriptExecuteCloudScriptResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<ScriptExecutionError>(input.error);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.error = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.functionName);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.functionName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.functionResult.stringValue);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.functionResult.stringValue = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.functionResultTooLarge);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.functionResultTooLarge = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyToArray<LogStatement>(input.logs, input.logsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.logs = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.logsTooLarge);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.logsTooLarge = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ExecuteCloudScriptServerRequest::ToJson() const
{
    return ExecuteCloudScriptServerRequest::ToJson(this->Model());
}

JsonValue ExecuteCloudScriptServerRequest::ToJson(const PFCloudScriptExecuteCloudScriptServerRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "FunctionName", input.functionName);
    JsonUtils::ObjectAddMember(output, "FunctionParameter", input.functionParameter);
    JsonUtils::ObjectAddMember(output, "GeneratePlayStreamEvent", input.generatePlayStreamEvent);
    JsonUtils::ObjectAddMember(output, "PlayFabId", input.playFabId);
    JsonUtils::ObjectAddMember(output, "RevisionSelection", input.revisionSelection);
    JsonUtils::ObjectAddMember(output, "SpecificRevision", input.specificRevision);
    return output;
}

JsonValue ExecuteEntityCloudScriptRequest::ToJson() const
{
    return ExecuteEntityCloudScriptRequest::ToJson(this->Model());
}

JsonValue ExecuteEntityCloudScriptRequest::ToJson(const PFCloudScriptExecuteEntityCloudScriptRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "FunctionName", input.functionName);
    JsonUtils::ObjectAddMember(output, "FunctionParameter", input.functionParameter);
    JsonUtils::ObjectAddMember(output, "GeneratePlayStreamEvent", input.generatePlayStreamEvent);
    JsonUtils::ObjectAddMember(output, "RevisionSelection", input.revisionSelection);
    JsonUtils::ObjectAddMember(output, "SpecificRevision", input.specificRevision);
    return output;
}

JsonValue ExecuteFunctionRequest::ToJson() const
{
    return ExecuteFunctionRequest::ToJson(this->Model());
}

JsonValue ExecuteFunctionRequest::ToJson(const PFCloudScriptExecuteFunctionRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember<EntityKey>(output, "Entity", input.entity);
    JsonUtils::ObjectAddMember(output, "FunctionName", input.functionName);
    JsonUtils::ObjectAddMember(output, "FunctionParameter", input.functionParameter);
    JsonUtils::ObjectAddMember(output, "GeneratePlayStreamEvent", input.generatePlayStreamEvent);
    return output;
}

HRESULT FunctionExecutionError::FromJson(const JsonValue& input)
{
    String error{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Error", error));
    this->SetError(std::move(error));

    String message{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Message", message));
    this->SetMessage(std::move(message));

    String stackTrace{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "StackTrace", stackTrace));
    this->SetStackTrace(std::move(stackTrace));

    return S_OK;
}

size_t FunctionExecutionError::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCloudScriptFunctionExecutionError const*> FunctionExecutionError::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<FunctionExecutionError>(&this->Model());
}

size_t FunctionExecutionError::RequiredBufferSize(const PFCloudScriptFunctionExecutionError& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.error)
    {
        requiredSize += (std::strlen(model.error) + 1);
    }
    if (model.message)
    {
        requiredSize += (std::strlen(model.message) + 1);
    }
    if (model.stackTrace)
    {
        requiredSize += (std::strlen(model.stackTrace) + 1);
    }
    return requiredSize;
}

HRESULT FunctionExecutionError::Copy(const PFCloudScriptFunctionExecutionError& input, PFCloudScriptFunctionExecutionError& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.error);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.error = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.message);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.message = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.stackTrace);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.stackTrace = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT ExecuteFunctionResult::FromJson(const JsonValue& input)
{
    std::optional<FunctionExecutionError> error{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "Error", error));
    if (error)
    {
        this->SetError(std::move(*error));
    }

    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ExecutionTimeMilliseconds", this->m_model.executionTimeMilliseconds));

    String functionName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FunctionName", functionName));
    this->SetFunctionName(std::move(functionName));

    JsonObject functionResult{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FunctionResult", functionResult));
    this->SetFunctionResult(std::move(functionResult));

    std::optional<bool> functionResultTooLarge{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FunctionResultTooLarge", functionResultTooLarge));
    this->SetFunctionResultTooLarge(std::move(functionResultTooLarge));

    return S_OK;
}

size_t ExecuteFunctionResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCloudScriptExecuteFunctionResult const*> ExecuteFunctionResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ExecuteFunctionResult>(&this->Model());
}

size_t ExecuteFunctionResult::RequiredBufferSize(const PFCloudScriptExecuteFunctionResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.error)
    {
        requiredSize += FunctionExecutionError::RequiredBufferSize(*model.error);
    }
    if (model.functionName)
    {
        requiredSize += (std::strlen(model.functionName) + 1);
    }
    if (model.functionResult.stringValue)
    {
    requiredSize += (std::strlen(model.functionResult.stringValue) + 1);
    }
    if (model.functionResultTooLarge)
    {
        requiredSize += (alignof(bool) + sizeof(bool));
    }
    return requiredSize;
}

HRESULT ExecuteFunctionResult::Copy(const PFCloudScriptExecuteFunctionResult& input, PFCloudScriptExecuteFunctionResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo<FunctionExecutionError>(input.error);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.error = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.functionName);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.functionName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.functionResult.stringValue);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.functionResult.stringValue = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.functionResultTooLarge);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.functionResultTooLarge = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue ListFunctionsRequest::ToJson() const
{
    return ListFunctionsRequest::ToJson(this->Model());
}

JsonValue ListFunctionsRequest::ToJson(const PFCloudScriptListFunctionsRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    return output;
}

HRESULT EventHubFunctionModel::FromJson(const JsonValue& input)
{
    String connectionString{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "ConnectionString", connectionString));
    this->SetConnectionString(std::move(connectionString));

    String eventHubName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "EventHubName", eventHubName));
    this->SetEventHubName(std::move(eventHubName));

    String functionName{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "FunctionName", functionName));
    this->SetFunctionName(std::move(functionName));

    return S_OK;
}

size_t EventHubFunctionModel::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCloudScriptEventHubFunctionModel const*> EventHubFunctionModel::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<EventHubFunctionModel>(&this->Model());
}

size_t EventHubFunctionModel::RequiredBufferSize(const PFCloudScriptEventHubFunctionModel& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    if (model.connectionString)
    {
        requiredSize += (std::strlen(model.connectionString) + 1);
    }
    if (model.eventHubName)
    {
        requiredSize += (std::strlen(model.eventHubName) + 1);
    }
    if (model.functionName)
    {
        requiredSize += (std::strlen(model.functionName) + 1);
    }
    return requiredSize;
}

HRESULT EventHubFunctionModel::Copy(const PFCloudScriptEventHubFunctionModel& input, PFCloudScriptEventHubFunctionModel& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyTo(input.connectionString);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.connectionString = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.eventHubName);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.eventHubName = propCopyResult.ExtractPayload();
    }
    {
        auto propCopyResult = buffer.CopyTo(input.functionName);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.functionName = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

HRESULT ListEventHubFunctionsResult::FromJson(const JsonValue& input)
{
    ModelVector<EventHubFunctionModel> functions{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember<EventHubFunctionModel>(input, "Functions", functions));
    this->SetFunctions(std::move(functions));

    return S_OK;
}

size_t ListEventHubFunctionsResult::RequiredBufferSize() const
{
    return RequiredBufferSize(this->Model());
}

Result<PFCloudScriptListEventHubFunctionsResult const*> ListEventHubFunctionsResult::Copy(ModelBuffer& buffer) const
{
    return buffer.CopyTo<ListEventHubFunctionsResult>(&this->Model());
}

size_t ListEventHubFunctionsResult::RequiredBufferSize(const PFCloudScriptListEventHubFunctionsResult& model)
{
    size_t requiredSize{ alignof(ModelType) + sizeof(ModelType) };
    requiredSize += (alignof(PFCloudScriptEventHubFunctionModel*) + sizeof(PFCloudScriptEventHubFunctionModel*) * model.functionsCount);
    for (size_t i = 0; i < model.functionsCount; ++i)
    {
        requiredSize += EventHubFunctionModel::RequiredBufferSize(*model.functions[i]);
    }
    return requiredSize;
}

HRESULT ListEventHubFunctionsResult::Copy(const PFCloudScriptListEventHubFunctionsResult& input, PFCloudScriptListEventHubFunctionsResult& output, ModelBuffer& buffer)
{
    output = input;
    {
        auto propCopyResult = buffer.CopyToArray<EventHubFunctionModel>(input.functions, input.functionsCount);
        RETURN_IF_FAILED(propCopyResult.hr);
        output.functions = propCopyResult.ExtractPayload();
    }
    return S_OK;
}

JsonValue RegisterEventHubFunctionRequest::ToJson() const
{
    return RegisterEventHubFunctionRequest::ToJson(this->Model());
}

JsonValue RegisterEventHubFunctionRequest::ToJson(const PFCloudScriptRegisterEventHubFunctionRequest& input)
{
    JsonValue output { JsonValue::object() };
    JsonUtils::ObjectAddMember(output, "ConnectionString", input.connectionString);
    JsonUtils::ObjectAddMemberDictionary(output, "CustomTags", input.customTags, input.customTagsCount);
    JsonUtils::ObjectAddMember(output, "EventHubName", input.eventHubName);
    JsonUtils::ObjectAddMember(output, "FunctionName", input.functionName);
    return output;
}

} // namespace CloudScript

// Json serialization helpers

} // namespace PlayFab

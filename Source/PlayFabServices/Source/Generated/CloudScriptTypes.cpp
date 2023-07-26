#include "stdafx.h"
#include "CloudScriptTypes.h"
#include "JsonUtils.h"
#include "Types.h"

namespace PlayFab
{
namespace CloudScript
{

JsonValue ExecuteFunctionRequest::ToJson() const
{
    return ExecuteFunctionRequest::ToJson(this->Model());
}

JsonValue ExecuteFunctionRequest::ToJson(const PFCloudScriptExecuteFunctionRequest& input)
{
    JsonValue output{ rapidjson::kObjectType };
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

} // namespace CloudScript
} // namespace PlayFab

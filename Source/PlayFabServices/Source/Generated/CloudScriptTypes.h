#pragma once

#include <playfab/services/cpp/CloudScriptTypeWrappers.h>
#include "Generated/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace CloudScript
{

// CloudScript Classes
class ExecuteFunctionRequest : public Wrappers::PFCloudScriptExecuteFunctionRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCloudScriptExecuteFunctionRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCloudScriptExecuteFunctionRequest& input);
};

class FunctionExecutionError : public Wrappers::PFCloudScriptFunctionExecutionErrorWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCloudScriptFunctionExecutionError>
{
public:
    using ModelWrapperType = typename Wrappers::PFCloudScriptFunctionExecutionErrorWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCloudScriptFunctionExecutionError const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCloudScriptFunctionExecutionError& model);
    static HRESULT Copy(const PFCloudScriptFunctionExecutionError& input, PFCloudScriptFunctionExecutionError& output, ModelBuffer& buffer);
};

class ExecuteFunctionResult : public Wrappers::PFCloudScriptExecuteFunctionResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCloudScriptExecuteFunctionResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFCloudScriptExecuteFunctionResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCloudScriptExecuteFunctionResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCloudScriptExecuteFunctionResult& model);
    static HRESULT Copy(const PFCloudScriptExecuteFunctionResult& input, PFCloudScriptExecuteFunctionResult& output, ModelBuffer& buffer);
};

} // namespace CloudScript
// EnumRange definitions used for Enum (de)serialization
} // namespace PlayFab

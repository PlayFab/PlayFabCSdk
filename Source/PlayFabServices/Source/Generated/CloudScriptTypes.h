#pragma once

#include <playfab/services/cpp/CloudScriptTypeWrappers.h>
#include "Generated/CoreTypes.h"
#include "BaseModel.h"

namespace PlayFab
{
namespace CloudScript
{

// CloudScript Classes
class ExecuteCloudScriptRequest : public Wrappers::PFCloudScriptExecuteCloudScriptRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCloudScriptExecuteCloudScriptRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCloudScriptExecuteCloudScriptRequest& input);
};

class ScriptExecutionError : public Wrappers::PFCloudScriptScriptExecutionErrorWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCloudScriptScriptExecutionError>
{
public:
    using ModelWrapperType = typename Wrappers::PFCloudScriptScriptExecutionErrorWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCloudScriptScriptExecutionError const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCloudScriptScriptExecutionError& model);
    static HRESULT Copy(const PFCloudScriptScriptExecutionError& input, PFCloudScriptScriptExecutionError& output, ModelBuffer& buffer);
};

class LogStatement : public Wrappers::PFCloudScriptLogStatementWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCloudScriptLogStatement>
{
public:
    using ModelWrapperType = typename Wrappers::PFCloudScriptLogStatementWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCloudScriptLogStatement const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCloudScriptLogStatement& model);
    static HRESULT Copy(const PFCloudScriptLogStatement& input, PFCloudScriptLogStatement& output, ModelBuffer& buffer);
};

class ExecuteCloudScriptResult : public Wrappers::PFCloudScriptExecuteCloudScriptResultWrapper<Allocator>, public ServiceOutputModel, public ClientOutputModel<PFCloudScriptExecuteCloudScriptResult>
{
public:
    using ModelWrapperType = typename Wrappers::PFCloudScriptExecuteCloudScriptResultWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // ServiceOutputModel
    HRESULT FromJson(const JsonValue& input) override;
    // ClientOutputModel
    size_t RequiredBufferSize() const override;
    Result<PFCloudScriptExecuteCloudScriptResult const*> Copy(ModelBuffer& buffer) const override;

    static size_t RequiredBufferSize(const PFCloudScriptExecuteCloudScriptResult& model);
    static HRESULT Copy(const PFCloudScriptExecuteCloudScriptResult& input, PFCloudScriptExecuteCloudScriptResult& output, ModelBuffer& buffer);
};

class ExecuteCloudScriptServerRequest : public Wrappers::PFCloudScriptExecuteCloudScriptServerRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCloudScriptExecuteCloudScriptServerRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCloudScriptExecuteCloudScriptServerRequest& input);
};

class ExecuteEntityCloudScriptRequest : public Wrappers::PFCloudScriptExecuteEntityCloudScriptRequestWrapper<Allocator>, public InputModel
{
public:
    using ModelWrapperType = typename Wrappers::PFCloudScriptExecuteEntityCloudScriptRequestWrapper<Allocator>;
    using ModelWrapperType::ModelType;

    // Constructors
    using ModelWrapperType::ModelWrapperType;

    // InputModel
    JsonValue ToJson() const override;
    static JsonValue ToJson(const PFCloudScriptExecuteEntityCloudScriptRequest& input);
};

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
template<typename T> struct EnumRange;

template<> struct EnumRange<PFCloudScriptCloudScriptRevisionOption>
{
    static constexpr PFCloudScriptCloudScriptRevisionOption maxValue = PFCloudScriptCloudScriptRevisionOption::Specific;
};

} // namespace PlayFab

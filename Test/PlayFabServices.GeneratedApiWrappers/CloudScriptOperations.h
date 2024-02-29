#pragma once

#include <playfab/services/cpp/CloudScriptTypeWrappers.h>
#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

class ClientExecuteCloudScriptOperation : public XAsyncOperation<Wrappers::PFCloudScriptExecuteCloudScriptResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCloudScriptExecuteCloudScriptRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCloudScriptExecuteCloudScriptResultWrapper<Allocator>;

    ClientExecuteCloudScriptOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCloudScriptExecuteCloudScriptResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class ServerExecuteCloudScriptOperation : public XAsyncOperation<Wrappers::PFCloudScriptExecuteCloudScriptResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCloudScriptExecuteCloudScriptServerRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCloudScriptExecuteCloudScriptResultWrapper<Allocator>;

    ServerExecuteCloudScriptOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCloudScriptExecuteCloudScriptResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

class ExecuteEntityCloudScriptOperation : public XAsyncOperation<Wrappers::PFCloudScriptExecuteCloudScriptResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCloudScriptExecuteEntityCloudScriptRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCloudScriptExecuteCloudScriptResultWrapper<Allocator>;

    ExecuteEntityCloudScriptOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCloudScriptExecuteCloudScriptResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ExecuteFunctionOperation : public XAsyncOperation<Wrappers::PFCloudScriptExecuteFunctionResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFCloudScriptExecuteFunctionRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFCloudScriptExecuteFunctionResultWrapper<Allocator>;

    ExecuteFunctionOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFCloudScriptExecuteFunctionResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

}
}

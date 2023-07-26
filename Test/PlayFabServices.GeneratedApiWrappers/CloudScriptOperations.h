#pragma once

#include <playfab/services/cpp/CloudScriptTypeWrappers.h>
#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

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

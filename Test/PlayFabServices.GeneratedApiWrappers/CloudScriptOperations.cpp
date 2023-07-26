#include "TestAppPch.h"
#include "CloudScriptOperations.h"
#include <playfab/services/PFCloudScript.h>

namespace PlayFab
{
namespace Test
{


ExecuteFunctionOperation::ExecuteFunctionOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ExecuteFunctionOperation::ResultType> ExecuteFunctionOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ExecuteFunctionOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ExecuteFunctionOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCloudScriptExecuteFunctionAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ExecuteFunctionOperation::ResultType> ExecuteFunctionOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCloudScriptExecuteFunctionGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCloudScriptExecuteFunctionResult* result;
    RETURN_IF_FAILED(PFCloudScriptExecuteFunctionGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}

}
}

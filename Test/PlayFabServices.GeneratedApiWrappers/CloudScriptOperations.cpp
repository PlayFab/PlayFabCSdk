#include "TestAppPch.h"
#include "CloudScriptOperations.h"
#include <playfab/services/PFCloudScript.h>

namespace PlayFab
{
namespace Test
{


ClientExecuteCloudScriptOperation::ClientExecuteCloudScriptOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientExecuteCloudScriptOperation::ResultType> ClientExecuteCloudScriptOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientExecuteCloudScriptOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientExecuteCloudScriptOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCloudScriptClientExecuteCloudScriptAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientExecuteCloudScriptOperation::ResultType> ClientExecuteCloudScriptOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCloudScriptClientExecuteCloudScriptGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCloudScriptExecuteCloudScriptResult* result;
    RETURN_IF_FAILED(PFCloudScriptClientExecuteCloudScriptGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX

ServerExecuteCloudScriptOperation::ServerExecuteCloudScriptOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerExecuteCloudScriptOperation::ResultType> ServerExecuteCloudScriptOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerExecuteCloudScriptOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerExecuteCloudScriptOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCloudScriptServerExecuteCloudScriptAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerExecuteCloudScriptOperation::ResultType> ServerExecuteCloudScriptOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCloudScriptServerExecuteCloudScriptGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCloudScriptExecuteCloudScriptResult* result;
    RETURN_IF_FAILED(PFCloudScriptServerExecuteCloudScriptGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif


ExecuteEntityCloudScriptOperation::ExecuteEntityCloudScriptOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ExecuteEntityCloudScriptOperation::ResultType> ExecuteEntityCloudScriptOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ExecuteEntityCloudScriptOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ExecuteEntityCloudScriptOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFCloudScriptExecuteEntityCloudScriptAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ExecuteEntityCloudScriptOperation::ResultType> ExecuteEntityCloudScriptOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFCloudScriptExecuteEntityCloudScriptGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFCloudScriptExecuteCloudScriptResult* result;
    RETURN_IF_FAILED(PFCloudScriptExecuteEntityCloudScriptGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


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

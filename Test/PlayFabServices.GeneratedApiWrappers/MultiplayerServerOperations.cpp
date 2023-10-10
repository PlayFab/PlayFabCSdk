#include "TestAppPch.h"
#include "MultiplayerServerOperations.h"
#include <playfab/services/PFMultiplayerServer.h>

namespace PlayFab
{
namespace Test
{


ListBuildAliasesOperation::ListBuildAliasesOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ListBuildAliasesOperation::ResultType> ListBuildAliasesOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ListBuildAliasesOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ListBuildAliasesOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFMultiplayerServerListBuildAliasesAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ListBuildAliasesOperation::ResultType> ListBuildAliasesOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFMultiplayerServerListBuildAliasesGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFMultiplayerServerListBuildAliasesResponse* result;
    RETURN_IF_FAILED(PFMultiplayerServerListBuildAliasesGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


ListBuildSummariesV2Operation::ListBuildSummariesV2Operation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ListBuildSummariesV2Operation::ResultType> ListBuildSummariesV2Operation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ListBuildSummariesV2Operation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ListBuildSummariesV2Operation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFMultiplayerServerListBuildSummariesV2Async(m_entity.Handle(), &m_request.Model(), async);
}

Result<ListBuildSummariesV2Operation::ResultType> ListBuildSummariesV2Operation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFMultiplayerServerListBuildSummariesV2GetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFMultiplayerServerListBuildSummariesResponse* result;
    RETURN_IF_FAILED(PFMultiplayerServerListBuildSummariesV2GetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


ListQosServersForTitleOperation::ListQosServersForTitleOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ListQosServersForTitleOperation::ResultType> ListQosServersForTitleOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ListQosServersForTitleOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ListQosServersForTitleOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFMultiplayerServerListQosServersForTitleAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ListQosServersForTitleOperation::ResultType> ListQosServersForTitleOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFMultiplayerServerListQosServersForTitleGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFMultiplayerServerListQosServersForTitleResponse* result;
    RETURN_IF_FAILED(PFMultiplayerServerListQosServersForTitleGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


RequestMultiplayerServerOperation::RequestMultiplayerServerOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<RequestMultiplayerServerOperation::ResultType> RequestMultiplayerServerOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<RequestMultiplayerServerOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT RequestMultiplayerServerOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFMultiplayerServerRequestMultiplayerServerAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<RequestMultiplayerServerOperation::ResultType> RequestMultiplayerServerOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFMultiplayerServerRequestMultiplayerServerGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFMultiplayerServerRequestMultiplayerServerResponse* result;
    RETURN_IF_FAILED(PFMultiplayerServerRequestMultiplayerServerGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}

#if 0

RequestPartyServiceOperation::RequestPartyServiceOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<RequestPartyServiceOperation::ResultType> RequestPartyServiceOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<RequestPartyServiceOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT RequestPartyServiceOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFMultiplayerServerRequestPartyServiceAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<RequestPartyServiceOperation::ResultType> RequestPartyServiceOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFMultiplayerServerRequestPartyServiceGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFMultiplayerServerRequestPartyServiceResponse* result;
    RETURN_IF_FAILED(PFMultiplayerServerRequestPartyServiceGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

}
}

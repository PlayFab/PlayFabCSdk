#include "TestAppPch.h"
#include "PlayStreamOperations.h"
#include <playfab/services/PFPlayStream.h>

namespace PlayFab
{
namespace Test
{

#if 0

ServerExportPlayersInSegmentOperation::ServerExportPlayersInSegmentOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerExportPlayersInSegmentOperation::ResultType> ServerExportPlayersInSegmentOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerExportPlayersInSegmentOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerExportPlayersInSegmentOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlayStreamServerExportPlayersInSegmentAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerExportPlayersInSegmentOperation::ResultType> ServerExportPlayersInSegmentOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFPlayStreamServerExportPlayersInSegmentGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFPlayStreamExportPlayersInSegmentResult* result;
    RETURN_IF_FAILED(PFPlayStreamServerExportPlayersInSegmentGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if 0

ServerGetSegmentExportOperation::ServerGetSegmentExportOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetSegmentExportOperation::ResultType> ServerGetSegmentExportOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetSegmentExportOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetSegmentExportOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlayStreamServerGetSegmentExportAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetSegmentExportOperation::ResultType> ServerGetSegmentExportOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFPlayStreamServerGetSegmentExportGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFPlayStreamGetPlayersInSegmentExportResponse* result;
    RETURN_IF_FAILED(PFPlayStreamServerGetSegmentExportGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if 0

ServerGetSegmentPlayerCountOperation::ServerGetSegmentPlayerCountOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetSegmentPlayerCountOperation::ResultType> ServerGetSegmentPlayerCountOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetSegmentPlayerCountOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetSegmentPlayerCountOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlayStreamServerGetSegmentPlayerCountAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetSegmentPlayerCountOperation::ResultType> ServerGetSegmentPlayerCountOperation::GetResult(XAsyncBlock* async) noexcept
{
    PFPlayStreamGetSegmentPlayerCountResult result{};
    RETURN_IF_FAILED(PFPlayStreamServerGetSegmentPlayerCountGetResult(async, &result));
    return ResultType{ result };
}
#endif

}
}

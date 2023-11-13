#include "TestAppPch.h"
#include "SegmentsOperations.h"
#include <playfab/services/PFSegments.h>

namespace PlayFab
{
namespace Test
{


ClientGetPlayerSegmentsOperation::ClientGetPlayerSegmentsOperation(Entity entity, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) }
{
}

AsyncOp<ClientGetPlayerSegmentsOperation::ResultType> ClientGetPlayerSegmentsOperation::Run(Entity entity, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetPlayerSegmentsOperation>(std::move(entity), std::move(rc)));
}

HRESULT ClientGetPlayerSegmentsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFSegmentsClientGetPlayerSegmentsAsync(m_entity.Handle(), async);
}

Result<ClientGetPlayerSegmentsOperation::ResultType> ClientGetPlayerSegmentsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFSegmentsClientGetPlayerSegmentsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFSegmentsGetPlayerSegmentsResult* result;
    RETURN_IF_FAILED(PFSegmentsClientGetPlayerSegmentsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


ClientGetPlayerTagsOperation::ClientGetPlayerTagsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetPlayerTagsOperation::ResultType> ClientGetPlayerTagsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetPlayerTagsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetPlayerTagsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFSegmentsClientGetPlayerTagsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetPlayerTagsOperation::ResultType> ClientGetPlayerTagsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFSegmentsClientGetPlayerTagsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFSegmentsGetPlayerTagsResult* result;
    RETURN_IF_FAILED(PFSegmentsClientGetPlayerTagsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX

ServerAddPlayerTagOperation::ServerAddPlayerTagOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerAddPlayerTagOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerAddPlayerTagOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerAddPlayerTagOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFSegmentsServerAddPlayerTagAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX

ServerGetAllSegmentsOperation::ServerGetAllSegmentsOperation(Entity entity, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) }
{
}

AsyncOp<ServerGetAllSegmentsOperation::ResultType> ServerGetAllSegmentsOperation::Run(Entity entity, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetAllSegmentsOperation>(std::move(entity), std::move(rc)));
}

HRESULT ServerGetAllSegmentsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFSegmentsServerGetAllSegmentsAsync(m_entity.Handle(), async);
}

Result<ServerGetAllSegmentsOperation::ResultType> ServerGetAllSegmentsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFSegmentsServerGetAllSegmentsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFSegmentsGetAllSegmentsResult* result;
    RETURN_IF_FAILED(PFSegmentsServerGetAllSegmentsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX

ServerGetPlayerSegmentsOperation::ServerGetPlayerSegmentsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetPlayerSegmentsOperation::ResultType> ServerGetPlayerSegmentsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetPlayerSegmentsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetPlayerSegmentsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFSegmentsServerGetPlayerSegmentsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetPlayerSegmentsOperation::ResultType> ServerGetPlayerSegmentsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFSegmentsServerGetPlayerSegmentsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFSegmentsGetPlayerSegmentsResult* result;
    RETURN_IF_FAILED(PFSegmentsServerGetPlayerSegmentsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX

ServerGetPlayersInSegmentOperation::ServerGetPlayersInSegmentOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetPlayersInSegmentOperation::ResultType> ServerGetPlayersInSegmentOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetPlayersInSegmentOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetPlayersInSegmentOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFSegmentsServerGetPlayersInSegmentAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetPlayersInSegmentOperation::ResultType> ServerGetPlayersInSegmentOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFSegmentsServerGetPlayersInSegmentGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFSegmentsGetPlayersInSegmentResult* result;
    RETURN_IF_FAILED(PFSegmentsServerGetPlayersInSegmentGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX

ServerGetPlayerTagsOperation::ServerGetPlayerTagsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetPlayerTagsOperation::ResultType> ServerGetPlayerTagsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetPlayerTagsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetPlayerTagsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFSegmentsServerGetPlayerTagsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetPlayerTagsOperation::ResultType> ServerGetPlayerTagsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFSegmentsServerGetPlayerTagsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFSegmentsGetPlayerTagsResult* result;
    RETURN_IF_FAILED(PFSegmentsServerGetPlayerTagsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX

ServerRemovePlayerTagOperation::ServerRemovePlayerTagOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerRemovePlayerTagOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerRemovePlayerTagOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerRemovePlayerTagOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFSegmentsServerRemovePlayerTagAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

}
}

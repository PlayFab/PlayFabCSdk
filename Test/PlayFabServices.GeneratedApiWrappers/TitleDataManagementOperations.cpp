#include "TestAppPch.h"
#include "TitleDataManagementOperations.h"
#include <playfab/services/PFTitleDataManagement.h>

namespace PlayFab
{
namespace Test
{


ClientGetPublisherDataOperation::ClientGetPublisherDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetPublisherDataOperation::ResultType> ClientGetPublisherDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetPublisherDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetPublisherDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFTitleDataManagementClientGetPublisherDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetPublisherDataOperation::ResultType> ClientGetPublisherDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFTitleDataManagementClientGetPublisherDataGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFTitleDataManagementGetPublisherDataResult* result;
    RETURN_IF_FAILED(PFTitleDataManagementClientGetPublisherDataGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


ClientGetTimeOperation::ClientGetTimeOperation(Entity entity, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) }
{
}

AsyncOp<ClientGetTimeOperation::ResultType> ClientGetTimeOperation::Run(Entity entity, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetTimeOperation>(std::move(entity), std::move(rc)));
}

HRESULT ClientGetTimeOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFTitleDataManagementClientGetTimeAsync(m_entity.Handle(), async);
}

Result<ClientGetTimeOperation::ResultType> ClientGetTimeOperation::GetResult(XAsyncBlock* async) noexcept
{
    PFTitleDataManagementGetTimeResult result{};
    RETURN_IF_FAILED(PFTitleDataManagementClientGetTimeGetResult(async, &result));
    return ResultType{ result };
}


ClientGetTitleDataOperation::ClientGetTitleDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetTitleDataOperation::ResultType> ClientGetTitleDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetTitleDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetTitleDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFTitleDataManagementClientGetTitleDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetTitleDataOperation::ResultType> ClientGetTitleDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFTitleDataManagementClientGetTitleDataGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFTitleDataManagementGetTitleDataResult* result;
    RETURN_IF_FAILED(PFTitleDataManagementClientGetTitleDataGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


ClientGetTitleNewsOperation::ClientGetTitleNewsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetTitleNewsOperation::ResultType> ClientGetTitleNewsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetTitleNewsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetTitleNewsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFTitleDataManagementClientGetTitleNewsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetTitleNewsOperation::ResultType> ClientGetTitleNewsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFTitleDataManagementClientGetTitleNewsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFTitleDataManagementGetTitleNewsResult* result;
    RETURN_IF_FAILED(PFTitleDataManagementClientGetTitleNewsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}

#if 0

ServerGetPublisherDataOperation::ServerGetPublisherDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetPublisherDataOperation::ResultType> ServerGetPublisherDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetPublisherDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetPublisherDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFTitleDataManagementServerGetPublisherDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetPublisherDataOperation::ResultType> ServerGetPublisherDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFTitleDataManagementServerGetPublisherDataGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFTitleDataManagementGetPublisherDataResult* result;
    RETURN_IF_FAILED(PFTitleDataManagementServerGetPublisherDataGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if 0

ServerGetTimeOperation::ServerGetTimeOperation(Entity entity, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) }
{
}

AsyncOp<ServerGetTimeOperation::ResultType> ServerGetTimeOperation::Run(Entity entity, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetTimeOperation>(std::move(entity), std::move(rc)));
}

HRESULT ServerGetTimeOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFTitleDataManagementServerGetTimeAsync(m_entity.Handle(), async);
}

Result<ServerGetTimeOperation::ResultType> ServerGetTimeOperation::GetResult(XAsyncBlock* async) noexcept
{
    PFTitleDataManagementGetTimeResult result{};
    RETURN_IF_FAILED(PFTitleDataManagementServerGetTimeGetResult(async, &result));
    return ResultType{ result };
}
#endif

#if 0

ServerGetTitleDataOperation::ServerGetTitleDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetTitleDataOperation::ResultType> ServerGetTitleDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetTitleDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetTitleDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFTitleDataManagementServerGetTitleDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetTitleDataOperation::ResultType> ServerGetTitleDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFTitleDataManagementServerGetTitleDataGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFTitleDataManagementGetTitleDataResult* result;
    RETURN_IF_FAILED(PFTitleDataManagementServerGetTitleDataGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if 0

ServerGetTitleInternalDataOperation::ServerGetTitleInternalDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetTitleInternalDataOperation::ResultType> ServerGetTitleInternalDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetTitleInternalDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetTitleInternalDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFTitleDataManagementServerGetTitleInternalDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetTitleInternalDataOperation::ResultType> ServerGetTitleInternalDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFTitleDataManagementServerGetTitleInternalDataGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFTitleDataManagementGetTitleDataResult* result;
    RETURN_IF_FAILED(PFTitleDataManagementServerGetTitleInternalDataGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if 0

ServerGetTitleNewsOperation::ServerGetTitleNewsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetTitleNewsOperation::ResultType> ServerGetTitleNewsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetTitleNewsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetTitleNewsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFTitleDataManagementServerGetTitleNewsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetTitleNewsOperation::ResultType> ServerGetTitleNewsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFTitleDataManagementServerGetTitleNewsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFTitleDataManagementGetTitleNewsResult* result;
    RETURN_IF_FAILED(PFTitleDataManagementServerGetTitleNewsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if 0

ServerSetPublisherDataOperation::ServerSetPublisherDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerSetPublisherDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerSetPublisherDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerSetPublisherDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFTitleDataManagementServerSetPublisherDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if 0

ServerSetTitleDataOperation::ServerSetTitleDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerSetTitleDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerSetTitleDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerSetTitleDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFTitleDataManagementServerSetTitleDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if 0

ServerSetTitleInternalDataOperation::ServerSetTitleInternalDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerSetTitleInternalDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerSetTitleInternalDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerSetTitleInternalDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFTitleDataManagementServerSetTitleInternalDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

}
}

#include "TestAppPch.h"
#include "PlayerDataManagementOperations.h"
#include <playfab/services/PFPlayerDataManagement.h>

namespace PlayFab
{
namespace Test
{


ClientGetUserDataOperation::ClientGetUserDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetUserDataOperation::ResultType> ClientGetUserDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetUserDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetUserDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlayerDataManagementClientGetUserDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetUserDataOperation::ResultType> ClientGetUserDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFPlayerDataManagementClientGetUserDataGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFPlayerDataManagementClientGetUserDataResult* result;
    RETURN_IF_FAILED(PFPlayerDataManagementClientGetUserDataGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


ClientGetUserPublisherDataOperation::ClientGetUserPublisherDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetUserPublisherDataOperation::ResultType> ClientGetUserPublisherDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetUserPublisherDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetUserPublisherDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlayerDataManagementClientGetUserPublisherDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetUserPublisherDataOperation::ResultType> ClientGetUserPublisherDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFPlayerDataManagementClientGetUserPublisherDataGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFPlayerDataManagementClientGetUserDataResult* result;
    RETURN_IF_FAILED(PFPlayerDataManagementClientGetUserPublisherDataGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


ClientGetUserPublisherReadOnlyDataOperation::ClientGetUserPublisherReadOnlyDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetUserPublisherReadOnlyDataOperation::ResultType> ClientGetUserPublisherReadOnlyDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetUserPublisherReadOnlyDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetUserPublisherReadOnlyDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlayerDataManagementClientGetUserPublisherReadOnlyDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetUserPublisherReadOnlyDataOperation::ResultType> ClientGetUserPublisherReadOnlyDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFPlayerDataManagementClientGetUserPublisherReadOnlyDataGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFPlayerDataManagementClientGetUserDataResult* result;
    RETURN_IF_FAILED(PFPlayerDataManagementClientGetUserPublisherReadOnlyDataGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


ClientGetUserReadOnlyDataOperation::ClientGetUserReadOnlyDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetUserReadOnlyDataOperation::ResultType> ClientGetUserReadOnlyDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetUserReadOnlyDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetUserReadOnlyDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlayerDataManagementClientGetUserReadOnlyDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetUserReadOnlyDataOperation::ResultType> ClientGetUserReadOnlyDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFPlayerDataManagementClientGetUserReadOnlyDataGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFPlayerDataManagementClientGetUserDataResult* result;
    RETURN_IF_FAILED(PFPlayerDataManagementClientGetUserReadOnlyDataGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


ClientUpdateUserDataOperation::ClientUpdateUserDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientUpdateUserDataOperation::ResultType> ClientUpdateUserDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientUpdateUserDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientUpdateUserDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlayerDataManagementClientUpdateUserDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientUpdateUserDataOperation::ResultType> ClientUpdateUserDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    PFPlayerDataManagementUpdateUserDataResult result{};
    RETURN_IF_FAILED(PFPlayerDataManagementClientUpdateUserDataGetResult(async, &result));
    return ResultType{ result };
}


ClientUpdateUserPublisherDataOperation::ClientUpdateUserPublisherDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientUpdateUserPublisherDataOperation::ResultType> ClientUpdateUserPublisherDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientUpdateUserPublisherDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientUpdateUserPublisherDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlayerDataManagementClientUpdateUserPublisherDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientUpdateUserPublisherDataOperation::ResultType> ClientUpdateUserPublisherDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    PFPlayerDataManagementUpdateUserDataResult result{};
    RETURN_IF_FAILED(PFPlayerDataManagementClientUpdateUserPublisherDataGetResult(async, &result));
    return ResultType{ result };
}

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX

ServerGetUserDataOperation::ServerGetUserDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetUserDataOperation::ResultType> ServerGetUserDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetUserDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetUserDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlayerDataManagementServerGetUserDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetUserDataOperation::ResultType> ServerGetUserDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFPlayerDataManagementServerGetUserDataGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFPlayerDataManagementServerGetUserDataResult* result;
    RETURN_IF_FAILED(PFPlayerDataManagementServerGetUserDataGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX

ServerGetUserInternalDataOperation::ServerGetUserInternalDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetUserInternalDataOperation::ResultType> ServerGetUserInternalDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetUserInternalDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetUserInternalDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlayerDataManagementServerGetUserInternalDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetUserInternalDataOperation::ResultType> ServerGetUserInternalDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFPlayerDataManagementServerGetUserInternalDataGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFPlayerDataManagementServerGetUserDataResult* result;
    RETURN_IF_FAILED(PFPlayerDataManagementServerGetUserInternalDataGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX

ServerGetUserPublisherDataOperation::ServerGetUserPublisherDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetUserPublisherDataOperation::ResultType> ServerGetUserPublisherDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetUserPublisherDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetUserPublisherDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlayerDataManagementServerGetUserPublisherDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetUserPublisherDataOperation::ResultType> ServerGetUserPublisherDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFPlayerDataManagementServerGetUserPublisherDataGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFPlayerDataManagementServerGetUserDataResult* result;
    RETURN_IF_FAILED(PFPlayerDataManagementServerGetUserPublisherDataGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX

ServerGetUserPublisherInternalDataOperation::ServerGetUserPublisherInternalDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetUserPublisherInternalDataOperation::ResultType> ServerGetUserPublisherInternalDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetUserPublisherInternalDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetUserPublisherInternalDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlayerDataManagementServerGetUserPublisherInternalDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetUserPublisherInternalDataOperation::ResultType> ServerGetUserPublisherInternalDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFPlayerDataManagementServerGetUserPublisherInternalDataGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFPlayerDataManagementServerGetUserDataResult* result;
    RETURN_IF_FAILED(PFPlayerDataManagementServerGetUserPublisherInternalDataGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX

ServerGetUserPublisherReadOnlyDataOperation::ServerGetUserPublisherReadOnlyDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetUserPublisherReadOnlyDataOperation::ResultType> ServerGetUserPublisherReadOnlyDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetUserPublisherReadOnlyDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetUserPublisherReadOnlyDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlayerDataManagementServerGetUserPublisherReadOnlyDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetUserPublisherReadOnlyDataOperation::ResultType> ServerGetUserPublisherReadOnlyDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFPlayerDataManagementServerGetUserPublisherReadOnlyDataGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFPlayerDataManagementServerGetUserDataResult* result;
    RETURN_IF_FAILED(PFPlayerDataManagementServerGetUserPublisherReadOnlyDataGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX

ServerGetUserReadOnlyDataOperation::ServerGetUserReadOnlyDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetUserReadOnlyDataOperation::ResultType> ServerGetUserReadOnlyDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetUserReadOnlyDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetUserReadOnlyDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlayerDataManagementServerGetUserReadOnlyDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetUserReadOnlyDataOperation::ResultType> ServerGetUserReadOnlyDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFPlayerDataManagementServerGetUserReadOnlyDataGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFPlayerDataManagementServerGetUserDataResult* result;
    RETURN_IF_FAILED(PFPlayerDataManagementServerGetUserReadOnlyDataGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX

ServerUpdateUserDataOperation::ServerUpdateUserDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerUpdateUserDataOperation::ResultType> ServerUpdateUserDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerUpdateUserDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerUpdateUserDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlayerDataManagementServerUpdateUserDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerUpdateUserDataOperation::ResultType> ServerUpdateUserDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    PFPlayerDataManagementUpdateUserDataResult result{};
    RETURN_IF_FAILED(PFPlayerDataManagementServerUpdateUserDataGetResult(async, &result));
    return ResultType{ result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX

ServerUpdateUserInternalDataOperation::ServerUpdateUserInternalDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerUpdateUserInternalDataOperation::ResultType> ServerUpdateUserInternalDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerUpdateUserInternalDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerUpdateUserInternalDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlayerDataManagementServerUpdateUserInternalDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerUpdateUserInternalDataOperation::ResultType> ServerUpdateUserInternalDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    PFPlayerDataManagementUpdateUserDataResult result{};
    RETURN_IF_FAILED(PFPlayerDataManagementServerUpdateUserInternalDataGetResult(async, &result));
    return ResultType{ result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX

ServerUpdateUserPublisherDataOperation::ServerUpdateUserPublisherDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerUpdateUserPublisherDataOperation::ResultType> ServerUpdateUserPublisherDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerUpdateUserPublisherDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerUpdateUserPublisherDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlayerDataManagementServerUpdateUserPublisherDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerUpdateUserPublisherDataOperation::ResultType> ServerUpdateUserPublisherDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    PFPlayerDataManagementUpdateUserDataResult result{};
    RETURN_IF_FAILED(PFPlayerDataManagementServerUpdateUserPublisherDataGetResult(async, &result));
    return ResultType{ result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX

ServerUpdateUserPublisherInternalDataOperation::ServerUpdateUserPublisherInternalDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerUpdateUserPublisherInternalDataOperation::ResultType> ServerUpdateUserPublisherInternalDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerUpdateUserPublisherInternalDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerUpdateUserPublisherInternalDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlayerDataManagementServerUpdateUserPublisherInternalDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerUpdateUserPublisherInternalDataOperation::ResultType> ServerUpdateUserPublisherInternalDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    PFPlayerDataManagementUpdateUserDataResult result{};
    RETURN_IF_FAILED(PFPlayerDataManagementServerUpdateUserPublisherInternalDataGetResult(async, &result));
    return ResultType{ result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX

ServerUpdateUserPublisherReadOnlyDataOperation::ServerUpdateUserPublisherReadOnlyDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerUpdateUserPublisherReadOnlyDataOperation::ResultType> ServerUpdateUserPublisherReadOnlyDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerUpdateUserPublisherReadOnlyDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerUpdateUserPublisherReadOnlyDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlayerDataManagementServerUpdateUserPublisherReadOnlyDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerUpdateUserPublisherReadOnlyDataOperation::ResultType> ServerUpdateUserPublisherReadOnlyDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    PFPlayerDataManagementUpdateUserDataResult result{};
    RETURN_IF_FAILED(PFPlayerDataManagementServerUpdateUserPublisherReadOnlyDataGetResult(async, &result));
    return ResultType{ result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX

ServerUpdateUserReadOnlyDataOperation::ServerUpdateUserReadOnlyDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerUpdateUserReadOnlyDataOperation::ResultType> ServerUpdateUserReadOnlyDataOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerUpdateUserReadOnlyDataOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerUpdateUserReadOnlyDataOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFPlayerDataManagementServerUpdateUserReadOnlyDataAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerUpdateUserReadOnlyDataOperation::ResultType> ServerUpdateUserReadOnlyDataOperation::GetResult(XAsyncBlock* async) noexcept
{
    PFPlayerDataManagementUpdateUserDataResult result{};
    RETURN_IF_FAILED(PFPlayerDataManagementServerUpdateUserReadOnlyDataGetResult(async, &result));
    return ResultType{ result };
}
#endif

}
}

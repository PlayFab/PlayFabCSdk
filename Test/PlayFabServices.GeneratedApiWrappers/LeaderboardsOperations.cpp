#include "TestAppPch.h"
#include "LeaderboardsOperations.h"
#include <playfab/services/PFLeaderboards.h>

namespace PlayFab
{
namespace Test
{

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC

CreateLeaderboardDefinitionOperation::CreateLeaderboardDefinitionOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> CreateLeaderboardDefinitionOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<CreateLeaderboardDefinitionOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT CreateLeaderboardDefinitionOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFLeaderboardsCreateLeaderboardDefinitionAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC

DeleteLeaderboardDefinitionOperation::DeleteLeaderboardDefinitionOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> DeleteLeaderboardDefinitionOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<DeleteLeaderboardDefinitionOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT DeleteLeaderboardDefinitionOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFLeaderboardsDeleteLeaderboardDefinitionAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC

DeleteLeaderboardEntriesOperation::DeleteLeaderboardEntriesOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> DeleteLeaderboardEntriesOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<DeleteLeaderboardEntriesOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT DeleteLeaderboardEntriesOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFLeaderboardsDeleteLeaderboardEntriesAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif


GetFriendLeaderboardForEntityOperation::GetFriendLeaderboardForEntityOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetFriendLeaderboardForEntityOperation::ResultType> GetFriendLeaderboardForEntityOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetFriendLeaderboardForEntityOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetFriendLeaderboardForEntityOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFLeaderboardsGetFriendLeaderboardForEntityAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetFriendLeaderboardForEntityOperation::ResultType> GetFriendLeaderboardForEntityOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFLeaderboardsGetFriendLeaderboardForEntityGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFLeaderboardsGetEntityLeaderboardResponse* result;
    RETURN_IF_FAILED(PFLeaderboardsGetFriendLeaderboardForEntityGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


GetLeaderboardOperation::GetLeaderboardOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetLeaderboardOperation::ResultType> GetLeaderboardOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetLeaderboardOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetLeaderboardOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFLeaderboardsGetLeaderboardAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetLeaderboardOperation::ResultType> GetLeaderboardOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFLeaderboardsGetLeaderboardGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFLeaderboardsGetEntityLeaderboardResponse* result;
    RETURN_IF_FAILED(PFLeaderboardsGetLeaderboardGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


GetLeaderboardAroundEntityOperation::GetLeaderboardAroundEntityOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetLeaderboardAroundEntityOperation::ResultType> GetLeaderboardAroundEntityOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetLeaderboardAroundEntityOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetLeaderboardAroundEntityOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFLeaderboardsGetLeaderboardAroundEntityAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetLeaderboardAroundEntityOperation::ResultType> GetLeaderboardAroundEntityOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFLeaderboardsGetLeaderboardAroundEntityGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFLeaderboardsGetEntityLeaderboardResponse* result;
    RETURN_IF_FAILED(PFLeaderboardsGetLeaderboardAroundEntityGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC

GetLeaderboardDefinitionOperation::GetLeaderboardDefinitionOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetLeaderboardDefinitionOperation::ResultType> GetLeaderboardDefinitionOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetLeaderboardDefinitionOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetLeaderboardDefinitionOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFLeaderboardsGetLeaderboardDefinitionAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetLeaderboardDefinitionOperation::ResultType> GetLeaderboardDefinitionOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFLeaderboardsGetLeaderboardDefinitionGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFLeaderboardsGetLeaderboardDefinitionResponse* result;
    RETURN_IF_FAILED(PFLeaderboardsGetLeaderboardDefinitionGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC

GetLeaderboardForEntitiesOperation::GetLeaderboardForEntitiesOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetLeaderboardForEntitiesOperation::ResultType> GetLeaderboardForEntitiesOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetLeaderboardForEntitiesOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetLeaderboardForEntitiesOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFLeaderboardsGetLeaderboardForEntitiesAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetLeaderboardForEntitiesOperation::ResultType> GetLeaderboardForEntitiesOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFLeaderboardsGetLeaderboardForEntitiesGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFLeaderboardsGetEntityLeaderboardResponse* result;
    RETURN_IF_FAILED(PFLeaderboardsGetLeaderboardForEntitiesGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC

IncrementLeaderboardVersionOperation::IncrementLeaderboardVersionOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<IncrementLeaderboardVersionOperation::ResultType> IncrementLeaderboardVersionOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<IncrementLeaderboardVersionOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT IncrementLeaderboardVersionOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFLeaderboardsIncrementLeaderboardVersionAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<IncrementLeaderboardVersionOperation::ResultType> IncrementLeaderboardVersionOperation::GetResult(XAsyncBlock* async) noexcept
{
    PFLeaderboardsIncrementLeaderboardVersionResponse result{};
    RETURN_IF_FAILED(PFLeaderboardsIncrementLeaderboardVersionGetResult(async, &result));
    return ResultType{ result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC

ListLeaderboardDefinitionsOperation::ListLeaderboardDefinitionsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ListLeaderboardDefinitionsOperation::ResultType> ListLeaderboardDefinitionsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ListLeaderboardDefinitionsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ListLeaderboardDefinitionsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFLeaderboardsListLeaderboardDefinitionsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ListLeaderboardDefinitionsOperation::ResultType> ListLeaderboardDefinitionsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFLeaderboardsListLeaderboardDefinitionsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFLeaderboardsListLeaderboardDefinitionsResponse* result;
    RETURN_IF_FAILED(PFLeaderboardsListLeaderboardDefinitionsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC

UnlinkLeaderboardFromStatisticOperation::UnlinkLeaderboardFromStatisticOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> UnlinkLeaderboardFromStatisticOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<UnlinkLeaderboardFromStatisticOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT UnlinkLeaderboardFromStatisticOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFLeaderboardsUnlinkLeaderboardFromStatisticAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC

UpdateLeaderboardEntriesOperation::UpdateLeaderboardEntriesOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> UpdateLeaderboardEntriesOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<UpdateLeaderboardEntriesOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT UpdateLeaderboardEntriesOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFLeaderboardsUpdateLeaderboardEntriesAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

}
}

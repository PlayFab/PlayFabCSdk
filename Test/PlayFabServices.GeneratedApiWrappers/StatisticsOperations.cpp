#include "TestAppPch.h"
#include "StatisticsOperations.h"
#include <playfab/services/PFStatistics.h>

namespace PlayFab
{
namespace Test
{

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC

CreateStatisticDefinitionOperation::CreateStatisticDefinitionOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> CreateStatisticDefinitionOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<CreateStatisticDefinitionOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT CreateStatisticDefinitionOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFStatisticsCreateStatisticDefinitionAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC

DeleteStatisticDefinitionOperation::DeleteStatisticDefinitionOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> DeleteStatisticDefinitionOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<DeleteStatisticDefinitionOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT DeleteStatisticDefinitionOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFStatisticsDeleteStatisticDefinitionAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif


DeleteStatisticsOperation::DeleteStatisticsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<DeleteStatisticsOperation::ResultType> DeleteStatisticsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<DeleteStatisticsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT DeleteStatisticsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFStatisticsDeleteStatisticsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<DeleteStatisticsOperation::ResultType> DeleteStatisticsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFStatisticsDeleteStatisticsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFStatisticsDeleteStatisticsResponse* result;
    RETURN_IF_FAILED(PFStatisticsDeleteStatisticsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC

GetStatisticDefinitionOperation::GetStatisticDefinitionOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetStatisticDefinitionOperation::ResultType> GetStatisticDefinitionOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetStatisticDefinitionOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetStatisticDefinitionOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFStatisticsGetStatisticDefinitionAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetStatisticDefinitionOperation::ResultType> GetStatisticDefinitionOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFStatisticsGetStatisticDefinitionGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFStatisticsGetStatisticDefinitionResponse* result;
    RETURN_IF_FAILED(PFStatisticsGetStatisticDefinitionGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif


GetStatisticsOperation::GetStatisticsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetStatisticsOperation::ResultType> GetStatisticsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetStatisticsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetStatisticsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFStatisticsGetStatisticsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetStatisticsOperation::ResultType> GetStatisticsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFStatisticsGetStatisticsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFStatisticsGetStatisticsResponse* result;
    RETURN_IF_FAILED(PFStatisticsGetStatisticsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC

GetStatisticsForEntitiesOperation::GetStatisticsForEntitiesOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<GetStatisticsForEntitiesOperation::ResultType> GetStatisticsForEntitiesOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<GetStatisticsForEntitiesOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT GetStatisticsForEntitiesOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFStatisticsGetStatisticsForEntitiesAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<GetStatisticsForEntitiesOperation::ResultType> GetStatisticsForEntitiesOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFStatisticsGetStatisticsForEntitiesGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFStatisticsGetStatisticsForEntitiesResponse* result;
    RETURN_IF_FAILED(PFStatisticsGetStatisticsForEntitiesGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC

IncrementStatisticVersionOperation::IncrementStatisticVersionOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<IncrementStatisticVersionOperation::ResultType> IncrementStatisticVersionOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<IncrementStatisticVersionOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT IncrementStatisticVersionOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFStatisticsIncrementStatisticVersionAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<IncrementStatisticVersionOperation::ResultType> IncrementStatisticVersionOperation::GetResult(XAsyncBlock* async) noexcept
{
    PFStatisticsIncrementStatisticVersionResponse result{};
    RETURN_IF_FAILED(PFStatisticsIncrementStatisticVersionGetResult(async, &result));
    return ResultType{ result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC

ListStatisticDefinitionsOperation::ListStatisticDefinitionsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ListStatisticDefinitionsOperation::ResultType> ListStatisticDefinitionsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ListStatisticDefinitionsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ListStatisticDefinitionsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFStatisticsListStatisticDefinitionsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ListStatisticDefinitionsOperation::ResultType> ListStatisticDefinitionsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFStatisticsListStatisticDefinitionsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFStatisticsListStatisticDefinitionsResponse* result;
    RETURN_IF_FAILED(PFStatisticsListStatisticDefinitionsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif


UpdateStatisticsOperation::UpdateStatisticsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<UpdateStatisticsOperation::ResultType> UpdateStatisticsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<UpdateStatisticsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT UpdateStatisticsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFStatisticsUpdateStatisticsAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<UpdateStatisticsOperation::ResultType> UpdateStatisticsOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFStatisticsUpdateStatisticsGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFStatisticsUpdateStatisticsResponse* result;
    RETURN_IF_FAILED(PFStatisticsUpdateStatisticsGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}

}
}

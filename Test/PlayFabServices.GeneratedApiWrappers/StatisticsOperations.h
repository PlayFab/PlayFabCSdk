#pragma once

#include <playfab/services/cpp/StatisticsTypeWrappers.h>
#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class CreateStatisticDefinitionOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFStatisticsCreateStatisticDefinitionRequestWrapper<Allocator>;

    CreateStatisticDefinitionOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class DeleteStatisticDefinitionOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFStatisticsDeleteStatisticDefinitionRequestWrapper<Allocator>;

    DeleteStatisticDefinitionOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

class DeleteStatisticsOperation : public XAsyncOperation<Wrappers::PFStatisticsDeleteStatisticsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFStatisticsDeleteStatisticsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFStatisticsDeleteStatisticsResponseWrapper<Allocator>;

    DeleteStatisticsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFStatisticsDeleteStatisticsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class GetStatisticDefinitionOperation : public XAsyncOperation<Wrappers::PFStatisticsGetStatisticDefinitionResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFStatisticsGetStatisticDefinitionRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFStatisticsGetStatisticDefinitionResponseWrapper<Allocator>;

    GetStatisticDefinitionOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFStatisticsGetStatisticDefinitionResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

class GetStatisticsOperation : public XAsyncOperation<Wrappers::PFStatisticsGetStatisticsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFStatisticsGetStatisticsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFStatisticsGetStatisticsResponseWrapper<Allocator>;

    GetStatisticsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFStatisticsGetStatisticsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class GetStatisticsForEntitiesOperation : public XAsyncOperation<Wrappers::PFStatisticsGetStatisticsForEntitiesResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFStatisticsGetStatisticsForEntitiesRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFStatisticsGetStatisticsForEntitiesResponseWrapper<Allocator>;

    GetStatisticsForEntitiesOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFStatisticsGetStatisticsForEntitiesResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class IncrementStatisticVersionOperation : public XAsyncOperation<Wrappers::PFStatisticsIncrementStatisticVersionResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFStatisticsIncrementStatisticVersionRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFStatisticsIncrementStatisticVersionResponseWrapper<Allocator>;

    IncrementStatisticVersionOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFStatisticsIncrementStatisticVersionResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class ListStatisticDefinitionsOperation : public XAsyncOperation<Wrappers::PFStatisticsListStatisticDefinitionsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFStatisticsListStatisticDefinitionsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFStatisticsListStatisticDefinitionsResponseWrapper<Allocator>;

    ListStatisticDefinitionsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFStatisticsListStatisticDefinitionsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

class UpdateStatisticsOperation : public XAsyncOperation<Wrappers::PFStatisticsUpdateStatisticsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFStatisticsUpdateStatisticsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFStatisticsUpdateStatisticsResponseWrapper<Allocator>;

    UpdateStatisticsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFStatisticsUpdateStatisticsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

}
}

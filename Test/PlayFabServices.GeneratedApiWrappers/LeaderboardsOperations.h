#pragma once

#include <playfab/services/cpp/LeaderboardsTypeWrappers.h>
#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class CreateLeaderboardDefinitionOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFLeaderboardsCreateLeaderboardDefinitionRequestWrapper<Allocator>;

    CreateLeaderboardDefinitionOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class DeleteLeaderboardDefinitionOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFLeaderboardsDeleteLeaderboardDefinitionRequestWrapper<Allocator>;

    DeleteLeaderboardDefinitionOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class DeleteLeaderboardEntriesOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFLeaderboardsDeleteLeaderboardEntriesRequestWrapper<Allocator>;

    DeleteLeaderboardEntriesOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

class GetFriendLeaderboardForEntityOperation : public XAsyncOperation<Wrappers::PFLeaderboardsGetEntityLeaderboardResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFLeaderboardsGetFriendLeaderboardForEntityRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFLeaderboardsGetEntityLeaderboardResponseWrapper<Allocator>;

    GetFriendLeaderboardForEntityOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFLeaderboardsGetEntityLeaderboardResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class GetLeaderboardOperation : public XAsyncOperation<Wrappers::PFLeaderboardsGetEntityLeaderboardResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFLeaderboardsGetEntityLeaderboardRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFLeaderboardsGetEntityLeaderboardResponseWrapper<Allocator>;

    GetLeaderboardOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFLeaderboardsGetEntityLeaderboardResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class GetLeaderboardAroundEntityOperation : public XAsyncOperation<Wrappers::PFLeaderboardsGetEntityLeaderboardResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFLeaderboardsGetLeaderboardAroundEntityRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFLeaderboardsGetEntityLeaderboardResponseWrapper<Allocator>;

    GetLeaderboardAroundEntityOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFLeaderboardsGetEntityLeaderboardResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class GetLeaderboardDefinitionOperation : public XAsyncOperation<Wrappers::PFLeaderboardsGetLeaderboardDefinitionResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFLeaderboardsGetLeaderboardDefinitionRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFLeaderboardsGetLeaderboardDefinitionResponseWrapper<Allocator>;

    GetLeaderboardDefinitionOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFLeaderboardsGetLeaderboardDefinitionResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class GetLeaderboardForEntitiesOperation : public XAsyncOperation<Wrappers::PFLeaderboardsGetEntityLeaderboardResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFLeaderboardsGetLeaderboardForEntitiesRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFLeaderboardsGetEntityLeaderboardResponseWrapper<Allocator>;

    GetLeaderboardForEntitiesOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFLeaderboardsGetEntityLeaderboardResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class IncrementLeaderboardVersionOperation : public XAsyncOperation<Wrappers::PFLeaderboardsIncrementLeaderboardVersionResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFLeaderboardsIncrementLeaderboardVersionRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFLeaderboardsIncrementLeaderboardVersionResponseWrapper<Allocator>;

    IncrementLeaderboardVersionOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFLeaderboardsIncrementLeaderboardVersionResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class ListLeaderboardDefinitionsOperation : public XAsyncOperation<Wrappers::PFLeaderboardsListLeaderboardDefinitionsResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFLeaderboardsListLeaderboardDefinitionsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFLeaderboardsListLeaderboardDefinitionsResponseWrapper<Allocator>;

    ListLeaderboardDefinitionsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFLeaderboardsListLeaderboardDefinitionsResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class UnlinkLeaderboardFromStatisticOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFLeaderboardsUnlinkLeaderboardFromStatisticRequestWrapper<Allocator>;

    UnlinkLeaderboardFromStatisticOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class UpdateLeaderboardEntriesOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFLeaderboardsUpdateLeaderboardEntriesRequestWrapper<Allocator>;

    UpdateLeaderboardEntriesOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

}
}

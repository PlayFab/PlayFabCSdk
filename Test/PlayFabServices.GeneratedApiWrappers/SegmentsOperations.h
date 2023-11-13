#pragma once

#include <playfab/services/cpp/SegmentsTypeWrappers.h>
#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

class ClientGetPlayerSegmentsOperation : public XAsyncOperation<Wrappers::PFSegmentsGetPlayerSegmentsResultWrapper<Allocator>>
{
public:
    using ResultType = Wrappers::PFSegmentsGetPlayerSegmentsResultWrapper<Allocator>;

    ClientGetPlayerSegmentsOperation(Entity entity, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFSegmentsGetPlayerSegmentsResultWrapper<Allocator>> Run(Entity entity, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
};

class ClientGetPlayerTagsOperation : public XAsyncOperation<Wrappers::PFSegmentsGetPlayerTagsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFSegmentsGetPlayerTagsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFSegmentsGetPlayerTagsResultWrapper<Allocator>;

    ClientGetPlayerTagsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFSegmentsGetPlayerTagsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
class ServerAddPlayerTagOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFSegmentsAddPlayerTagRequestWrapper<Allocator>;

    ServerAddPlayerTagOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
class ServerGetAllSegmentsOperation : public XAsyncOperation<Wrappers::PFSegmentsGetAllSegmentsResultWrapper<Allocator>>
{
public:
    using ResultType = Wrappers::PFSegmentsGetAllSegmentsResultWrapper<Allocator>;

    ServerGetAllSegmentsOperation(Entity entity, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFSegmentsGetAllSegmentsResultWrapper<Allocator>> Run(Entity entity, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
class ServerGetPlayerSegmentsOperation : public XAsyncOperation<Wrappers::PFSegmentsGetPlayerSegmentsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFSegmentsGetPlayersSegmentsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFSegmentsGetPlayerSegmentsResultWrapper<Allocator>;

    ServerGetPlayerSegmentsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFSegmentsGetPlayerSegmentsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
class ServerGetPlayersInSegmentOperation : public XAsyncOperation<Wrappers::PFSegmentsGetPlayersInSegmentResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFSegmentsGetPlayersInSegmentRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFSegmentsGetPlayersInSegmentResultWrapper<Allocator>;

    ServerGetPlayersInSegmentOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFSegmentsGetPlayersInSegmentResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
class ServerGetPlayerTagsOperation : public XAsyncOperation<Wrappers::PFSegmentsGetPlayerTagsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFSegmentsGetPlayerTagsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFSegmentsGetPlayerTagsResultWrapper<Allocator>;

    ServerGetPlayerTagsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFSegmentsGetPlayerTagsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
class ServerRemovePlayerTagOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFSegmentsRemovePlayerTagRequestWrapper<Allocator>;

    ServerRemovePlayerTagOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

}
}

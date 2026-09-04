#pragma once

#include <playfab/services/cpp/PlayStreamTypeWrappers.h>
#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

#if 0
class ServerExportPlayersInSegmentOperation : public XAsyncOperation<Wrappers::PFPlayStreamExportPlayersInSegmentResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayStreamExportPlayersInSegmentRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayStreamExportPlayersInSegmentResultWrapper<Allocator>;

    ServerExportPlayersInSegmentOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayStreamExportPlayersInSegmentResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerGetSegmentExportOperation : public XAsyncOperation<Wrappers::PFPlayStreamGetPlayersInSegmentExportResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayStreamGetPlayersInSegmentExportRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayStreamGetPlayersInSegmentExportResponseWrapper<Allocator>;

    ServerGetSegmentExportOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayStreamGetPlayersInSegmentExportResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if 0
class ServerGetSegmentPlayerCountOperation : public XAsyncOperation<Wrappers::PFPlayStreamGetSegmentPlayerCountResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayStreamGetSegmentPlayerCountRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayStreamGetSegmentPlayerCountResultWrapper<Allocator>;

    ServerGetSegmentPlayerCountOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayStreamGetSegmentPlayerCountResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

}
}

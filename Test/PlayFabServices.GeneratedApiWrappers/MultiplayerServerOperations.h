#pragma once

#include <playfab/services/cpp/MultiplayerServerTypeWrappers.h>
#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

class ListBuildAliasesOperation : public XAsyncOperation<Wrappers::PFMultiplayerServerListBuildAliasesResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFMultiplayerServerListBuildAliasesRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFMultiplayerServerListBuildAliasesResponseWrapper<Allocator>;

    ListBuildAliasesOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFMultiplayerServerListBuildAliasesResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ListBuildSummariesV2Operation : public XAsyncOperation<Wrappers::PFMultiplayerServerListBuildSummariesResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFMultiplayerServerListBuildSummariesRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFMultiplayerServerListBuildSummariesResponseWrapper<Allocator>;

    ListBuildSummariesV2Operation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFMultiplayerServerListBuildSummariesResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ListQosServersForTitleOperation : public XAsyncOperation<Wrappers::PFMultiplayerServerListQosServersForTitleResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFMultiplayerServerListQosServersForTitleRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFMultiplayerServerListQosServersForTitleResponseWrapper<Allocator>;

    ListQosServersForTitleOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFMultiplayerServerListQosServersForTitleResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class RequestMultiplayerServerOperation : public XAsyncOperation<Wrappers::PFMultiplayerServerRequestMultiplayerServerResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFMultiplayerServerRequestMultiplayerServerRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFMultiplayerServerRequestMultiplayerServerResponseWrapper<Allocator>;

    RequestMultiplayerServerOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFMultiplayerServerRequestMultiplayerServerResponseWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

}
}

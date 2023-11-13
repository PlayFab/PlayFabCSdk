#pragma once

#include <playfab/services/cpp/TitleDataManagementTypeWrappers.h>
#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

class ClientGetPublisherDataOperation : public XAsyncOperation<Wrappers::PFTitleDataManagementGetPublisherDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFTitleDataManagementGetPublisherDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFTitleDataManagementGetPublisherDataResultWrapper<Allocator>;

    ClientGetPublisherDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFTitleDataManagementGetPublisherDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ClientGetTimeOperation : public XAsyncOperation<Wrappers::PFTitleDataManagementGetTimeResultWrapper<Allocator>>
{
public:
    using ResultType = Wrappers::PFTitleDataManagementGetTimeResultWrapper<Allocator>;

    ClientGetTimeOperation(Entity entity, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFTitleDataManagementGetTimeResultWrapper<Allocator>> Run(Entity entity, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
};

class ClientGetTitleDataOperation : public XAsyncOperation<Wrappers::PFTitleDataManagementGetTitleDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFTitleDataManagementGetTitleDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFTitleDataManagementGetTitleDataResultWrapper<Allocator>;

    ClientGetTitleDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFTitleDataManagementGetTitleDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ClientGetTitleNewsOperation : public XAsyncOperation<Wrappers::PFTitleDataManagementGetTitleNewsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFTitleDataManagementGetTitleNewsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFTitleDataManagementGetTitleNewsResultWrapper<Allocator>;

    ClientGetTitleNewsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFTitleDataManagementGetTitleNewsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
class ServerGetPublisherDataOperation : public XAsyncOperation<Wrappers::PFTitleDataManagementGetPublisherDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFTitleDataManagementGetPublisherDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFTitleDataManagementGetPublisherDataResultWrapper<Allocator>;

    ServerGetPublisherDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFTitleDataManagementGetPublisherDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
class ServerGetTimeOperation : public XAsyncOperation<Wrappers::PFTitleDataManagementGetTimeResultWrapper<Allocator>>
{
public:
    using ResultType = Wrappers::PFTitleDataManagementGetTimeResultWrapper<Allocator>;

    ServerGetTimeOperation(Entity entity, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFTitleDataManagementGetTimeResultWrapper<Allocator>> Run(Entity entity, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
class ServerGetTitleDataOperation : public XAsyncOperation<Wrappers::PFTitleDataManagementGetTitleDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFTitleDataManagementGetTitleDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFTitleDataManagementGetTitleDataResultWrapper<Allocator>;

    ServerGetTitleDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFTitleDataManagementGetTitleDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
class ServerGetTitleInternalDataOperation : public XAsyncOperation<Wrappers::PFTitleDataManagementGetTitleDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFTitleDataManagementGetTitleDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFTitleDataManagementGetTitleDataResultWrapper<Allocator>;

    ServerGetTitleInternalDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFTitleDataManagementGetTitleDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
class ServerGetTitleNewsOperation : public XAsyncOperation<Wrappers::PFTitleDataManagementGetTitleNewsResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFTitleDataManagementGetTitleNewsRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFTitleDataManagementGetTitleNewsResultWrapper<Allocator>;

    ServerGetTitleNewsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFTitleDataManagementGetTitleNewsResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
class ServerSetPublisherDataOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFTitleDataManagementSetPublisherDataRequestWrapper<Allocator>;

    ServerSetPublisherDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
class ServerSetTitleDataOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFTitleDataManagementSetTitleDataRequestWrapper<Allocator>;

    ServerSetTitleDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
class ServerSetTitleInternalDataOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFTitleDataManagementSetTitleDataRequestWrapper<Allocator>;

    ServerSetTitleInternalDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

}
}

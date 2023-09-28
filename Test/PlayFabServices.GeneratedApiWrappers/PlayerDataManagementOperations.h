#pragma once

#include <playfab/services/cpp/PlayerDataManagementTypeWrappers.h>
#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

class ClientGetUserDataOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementClientGetUserDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementGetUserDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementClientGetUserDataResultWrapper<Allocator>;

    ClientGetUserDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementClientGetUserDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ClientGetUserPublisherDataOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementClientGetUserDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementGetUserDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementClientGetUserDataResultWrapper<Allocator>;

    ClientGetUserPublisherDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementClientGetUserDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ClientGetUserPublisherReadOnlyDataOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementClientGetUserDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementGetUserDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementClientGetUserDataResultWrapper<Allocator>;

    ClientGetUserPublisherReadOnlyDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementClientGetUserDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ClientGetUserReadOnlyDataOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementClientGetUserDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementGetUserDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementClientGetUserDataResultWrapper<Allocator>;

    ClientGetUserReadOnlyDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementClientGetUserDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ClientUpdateUserDataOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementClientUpdateUserDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>;

    ClientUpdateUserDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ClientUpdateUserPublisherDataOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementClientUpdateUserDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>;

    ClientUpdateUserPublisherDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

#if HC_PLATFORM == HC_PLATFORM_WIN32
class ServerGetUserDataOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementServerGetUserDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementGetUserDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementServerGetUserDataResultWrapper<Allocator>;

    ServerGetUserDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementServerGetUserDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
class ServerGetUserInternalDataOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementServerGetUserDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementGetUserDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementServerGetUserDataResultWrapper<Allocator>;

    ServerGetUserInternalDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementServerGetUserDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
class ServerGetUserPublisherDataOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementServerGetUserDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementGetUserDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementServerGetUserDataResultWrapper<Allocator>;

    ServerGetUserPublisherDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementServerGetUserDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
class ServerGetUserPublisherInternalDataOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementServerGetUserDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementGetUserDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementServerGetUserDataResultWrapper<Allocator>;

    ServerGetUserPublisherInternalDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementServerGetUserDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
class ServerGetUserPublisherReadOnlyDataOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementServerGetUserDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementGetUserDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementServerGetUserDataResultWrapper<Allocator>;

    ServerGetUserPublisherReadOnlyDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementServerGetUserDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
class ServerGetUserReadOnlyDataOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementServerGetUserDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementGetUserDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementServerGetUserDataResultWrapper<Allocator>;

    ServerGetUserReadOnlyDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementServerGetUserDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
class ServerUpdateUserDataOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementServerUpdateUserDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>;

    ServerUpdateUserDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
class ServerUpdateUserInternalDataOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementUpdateUserInternalDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>;

    ServerUpdateUserInternalDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
class ServerUpdateUserPublisherDataOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementServerUpdateUserDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>;

    ServerUpdateUserPublisherDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
class ServerUpdateUserPublisherInternalDataOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementUpdateUserInternalDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>;

    ServerUpdateUserPublisherInternalDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
class ServerUpdateUserPublisherReadOnlyDataOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementServerUpdateUserDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>;

    ServerUpdateUserPublisherReadOnlyDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
class ServerUpdateUserReadOnlyDataOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementServerUpdateUserDataRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>;

    ServerUpdateUserReadOnlyDataOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementUpdateUserDataResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

}
}

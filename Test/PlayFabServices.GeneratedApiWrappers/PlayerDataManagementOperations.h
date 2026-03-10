#pragma once

#include <playfab/services/cpp/PlayerDataManagementTypeWrappers.h>
#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

class ClientDeletePlayerCustomPropertiesOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementClientDeletePlayerCustomPropertiesResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementClientDeletePlayerCustomPropertiesRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementClientDeletePlayerCustomPropertiesResultWrapper<Allocator>;

    ClientDeletePlayerCustomPropertiesOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementClientDeletePlayerCustomPropertiesResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ClientGetPlayerCustomPropertyOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementClientGetPlayerCustomPropertyResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementClientGetPlayerCustomPropertyRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementClientGetPlayerCustomPropertyResultWrapper<Allocator>;

    ClientGetPlayerCustomPropertyOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementClientGetPlayerCustomPropertyResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

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

class ClientListPlayerCustomPropertiesOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementClientListPlayerCustomPropertiesResultWrapper<Allocator>>
{
public:
    using ResultType = Wrappers::PFPlayerDataManagementClientListPlayerCustomPropertiesResultWrapper<Allocator>;

    ClientListPlayerCustomPropertiesOperation(Entity entity, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementClientListPlayerCustomPropertiesResultWrapper<Allocator>> Run(Entity entity, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
};

class ClientUpdatePlayerCustomPropertiesOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementClientUpdatePlayerCustomPropertiesResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementClientUpdatePlayerCustomPropertiesRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementClientUpdatePlayerCustomPropertiesResultWrapper<Allocator>;

    ClientUpdatePlayerCustomPropertiesOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementClientUpdatePlayerCustomPropertiesResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

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

#if HC_PLATFORM == HC_PLATFORM_GDK
class ServerDeletePlayerCustomPropertiesOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementServerDeletePlayerCustomPropertiesResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementServerDeletePlayerCustomPropertiesRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementServerDeletePlayerCustomPropertiesResultWrapper<Allocator>;

    ServerDeletePlayerCustomPropertiesOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementServerDeletePlayerCustomPropertiesResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
class ServerGetPlayerCustomPropertyOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementServerGetPlayerCustomPropertyResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementServerGetPlayerCustomPropertyRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementServerGetPlayerCustomPropertyResultWrapper<Allocator>;

    ServerGetPlayerCustomPropertyOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementServerGetPlayerCustomPropertyResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
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

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
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

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
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

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
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

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
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

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
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

#if HC_PLATFORM == HC_PLATFORM_GDK
class ServerListPlayerCustomPropertiesOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementServerListPlayerCustomPropertiesResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementListPlayerCustomPropertiesRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementServerListPlayerCustomPropertiesResultWrapper<Allocator>;

    ServerListPlayerCustomPropertiesOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementServerListPlayerCustomPropertiesResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
class ServerUpdatePlayerCustomPropertiesOperation : public XAsyncOperation<Wrappers::PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFPlayerDataManagementServerUpdatePlayerCustomPropertiesRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResultWrapper<Allocator>;

    ServerUpdatePlayerCustomPropertiesOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFPlayerDataManagementServerUpdatePlayerCustomPropertiesResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
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

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
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

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
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

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
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

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
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

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
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

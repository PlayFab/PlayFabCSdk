#pragma once

#include <playfab/services/cpp/FriendsTypeWrappers.h>
#include "TestContext.h"

namespace PlayFab
{
namespace Test
{

class ClientAddFriendOperation : public XAsyncOperation<Wrappers::PFFriendsAddFriendResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFFriendsClientAddFriendRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFFriendsAddFriendResultWrapper<Allocator>;

    ClientAddFriendOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFFriendsAddFriendResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ClientGetFriendsListOperation : public XAsyncOperation<Wrappers::PFFriendsGetFriendsListResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFFriendsClientGetFriendsListRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFFriendsGetFriendsListResultWrapper<Allocator>;

    ClientGetFriendsListOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFFriendsGetFriendsListResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ClientRemoveFriendOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFFriendsClientRemoveFriendRequestWrapper<Allocator>;

    ClientRemoveFriendOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class ClientSetFriendTagsOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFFriendsClientSetFriendTagsRequestWrapper<Allocator>;

    ClientSetFriendTagsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class ServerAddFriendOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFFriendsServerAddFriendRequestWrapper<Allocator>;

    ServerAddFriendOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class ServerGetFriendsListOperation : public XAsyncOperation<Wrappers::PFFriendsGetFriendsListResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFFriendsServerGetFriendsListRequestWrapper<Allocator>;
    using ResultType = Wrappers::PFFriendsGetFriendsListResultWrapper<Allocator>;

    ServerGetFriendsListOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<Wrappers::PFFriendsGetFriendsListResultWrapper<Allocator>> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ResultType> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class ServerRemoveFriendOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFFriendsServerRemoveFriendRequestWrapper<Allocator>;

    ServerRemoveFriendOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
class ServerSetFriendTagsOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFFriendsServerSetFriendTagsRequestWrapper<Allocator>;

    ServerSetFriendTagsOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

    static AsyncOp<void> Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif

}
}

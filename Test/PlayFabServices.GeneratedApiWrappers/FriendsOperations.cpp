#include "TestAppPch.h"
#include "FriendsOperations.h"
#include <playfab/services/PFFriends.h>

namespace PlayFab
{
namespace Test
{


ClientAddFriendOperation::ClientAddFriendOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientAddFriendOperation::ResultType> ClientAddFriendOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientAddFriendOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientAddFriendOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFFriendsClientAddFriendAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientAddFriendOperation::ResultType> ClientAddFriendOperation::GetResult(XAsyncBlock* async) noexcept
{
    PFFriendsAddFriendResult result{};
    RETURN_IF_FAILED(PFFriendsClientAddFriendGetResult(async, &result));
    return ResultType{ result };
}


ClientGetFriendsListOperation::ClientGetFriendsListOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ClientGetFriendsListOperation::ResultType> ClientGetFriendsListOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientGetFriendsListOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientGetFriendsListOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFFriendsClientGetFriendsListAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ClientGetFriendsListOperation::ResultType> ClientGetFriendsListOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFFriendsClientGetFriendsListGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFFriendsGetFriendsListResult* result;
    RETURN_IF_FAILED(PFFriendsClientGetFriendsListGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}


ClientRemoveFriendOperation::ClientRemoveFriendOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientRemoveFriendOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientRemoveFriendOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientRemoveFriendOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFFriendsClientRemoveFriendAsync(m_entity.Handle(), &m_request.Model(), async);
}



ClientSetFriendTagsOperation::ClientSetFriendTagsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ClientSetFriendTagsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ClientSetFriendTagsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ClientSetFriendTagsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFFriendsClientSetFriendTagsAsync(m_entity.Handle(), &m_request.Model(), async);
}


#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerAddFriendOperation::ServerAddFriendOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerAddFriendOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerAddFriendOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerAddFriendOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFFriendsServerAddFriendAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerGetFriendsListOperation::ServerGetFriendsListOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<ServerGetFriendsListOperation::ResultType> ServerGetFriendsListOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerGetFriendsListOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerGetFriendsListOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFFriendsServerGetFriendsListAsync(m_entity.Handle(), &m_request.Model(), async);
}

Result<ServerGetFriendsListOperation::ResultType> ServerGetFriendsListOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t resultSize;
    RETURN_IF_FAILED(PFFriendsServerGetFriendsListGetResultSize(async, &resultSize));
    Vector<char> resultBuffer(resultSize);
    PFFriendsGetFriendsListResult* result;
    RETURN_IF_FAILED(PFFriendsServerGetFriendsListGetResult(async, resultBuffer.size(), resultBuffer.data(), &result, nullptr));
    return ResultType{ *result };
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerRemoveFriendOperation::ServerRemoveFriendOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerRemoveFriendOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerRemoveFriendOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerRemoveFriendOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFFriendsServerRemoveFriendAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32

ServerSetFriendTagsOperation::ServerSetFriendTagsOperation(Entity entity, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_entity{ std::move(entity) },
    m_request{ std::move(request) }
{
}

AsyncOp<void> ServerSetFriendTagsOperation::Run(Entity entity, RequestType request, PlayFab::RunContext rc) noexcept
{
    return RunOperation(MakeUnique<ServerSetFriendTagsOperation>(std::move(entity), std::move(request), std::move(rc)));
}

HRESULT ServerSetFriendTagsOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFFriendsServerSetFriendTagsAsync(m_entity.Handle(), &m_request.Model(), async);
}

#endif

}
}

#include "stdafx.h"
#include <playfab/services/PFFriends.h>
#include "Generated/Friends.h"
#include "ApiXAsyncProvider.h"
#include "GlobalState.h"
#include <playfab/core/cpp/Entity.h>

using namespace PlayFab;
using namespace PlayFab::Friends;

PF_API PFFriendsClientAddFriendAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFFriendsClientAddFriendRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFFriendsClientAddFriendAsync),
        std::bind(&FriendsAPI::ClientAddFriend, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFFriendsClientAddFriendGetResult(
    _In_ XAsyncBlock* async,
    _Out_ PFFriendsAddFriendResult* result
) noexcept
{
    return XAsyncGetResult(async, nullptr, sizeof(PFFriendsAddFriendResult), result, nullptr);
}

PF_API PFFriendsClientGetFriendsListAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFFriendsClientGetFriendsListRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFFriendsClientGetFriendsListAsync),
        std::bind(&FriendsAPI::ClientGetFriendsList, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFFriendsClientGetFriendsListGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFFriendsClientGetFriendsListGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFFriendsGetFriendsListResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFFriendsGetFriendsListResult*>(buffer);

    return S_OK;
}

PF_API PFFriendsClientRemoveFriendAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFFriendsClientRemoveFriendRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFFriendsClientRemoveFriendAsync),
        std::bind(&FriendsAPI::ClientRemoveFriend, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFFriendsClientSetFriendTagsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFFriendsClientSetFriendTagsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFFriendsClientSetFriendTagsAsync),
        std::bind(&FriendsAPI::ClientSetFriendTags, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

#if HC_PLATFORM == HC_PLATFORM_WIN32
PF_API PFFriendsServerAddFriendAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFFriendsServerAddFriendRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFFriendsServerAddFriendAsync),
        std::bind(&FriendsAPI::ServerAddFriend, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
PF_API PFFriendsServerGetFriendsListAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFFriendsServerGetFriendsListRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFFriendsServerGetFriendsListAsync),
        std::bind(&FriendsAPI::ServerGetFriendsList, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}

PF_API PFFriendsServerGetFriendsListGetResultSize(
    _In_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept
{
    return XAsyncGetResultSize(async, bufferSize);
}

PF_API PFFriendsServerGetFriendsListGetResult(
    _In_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFFriendsGetFriendsListResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(result);

    RETURN_IF_FAILED(XAsyncGetResult(async, nullptr, bufferSize, buffer, bufferUsed));
    *result = static_cast<PFFriendsGetFriendsListResult*>(buffer);

    return S_OK;
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
PF_API PFFriendsServerRemoveFriendAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFFriendsServerRemoveFriendRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFFriendsServerRemoveFriendAsync),
        std::bind(&FriendsAPI::ServerRemoveFriend, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
PF_API PFFriendsServerSetFriendTagsAsync(
    _In_ PFEntityHandle contextHandle,
    _In_ const PFFriendsServerSetFriendTagsRequest* request,
    _In_ XAsyncBlock* async
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(request);

    SharedPtr<GlobalState> state{ nullptr };
    RETURN_IF_FAILED(GlobalState::Get(state));

    auto provider = MakeProvider(
        state->RunContext().DeriveOnQueue(async->queue),
        async,
        XASYNC_IDENTITY(PFFriendsServerSetFriendTagsAsync),
        std::bind(&FriendsAPI::ServerSetFriendTags, Entity::Duplicate(contextHandle), *request, std::placeholders::_1)
    );
    return XAsyncProviderBase::Run(std::move(provider));
}
#endif


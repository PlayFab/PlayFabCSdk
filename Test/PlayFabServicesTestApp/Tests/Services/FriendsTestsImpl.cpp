#include "TestAppPch.h"
#include "FriendsTests.h"
#include "FriendsOperations.h"
#if HC_PLATFORM == HC_PLATFORM_GDK
#include "GDK/PlatformUser_GDK.h"
#endif

namespace PlayFab
{
namespace Test
{

constexpr char kFriendCustomId[]{ "FriendCustomId" };

AsyncOp<void> FriendsTests::Initialize()
{
    return ServicesTestClass::Initialize();
}

AsyncOp<void> FriendsTests::Uninitialize()
{
    return ServicesTestClass::Uninitialize();
}

void FriendsTests::TestClientAddFriend(TestContext& tc)
{
#if HC_PLATFORM != HC_PLATFORM_GDK
    // Already covered by TestClientSetFriendTags
    tc.EndTest(S_OK);
#else
    // This test uses GetPlayFabIdFromCustomId which is not supported on GDK
    tc.Skip();
#endif
}

void FriendsTests::TestClientGetFriendsList(TestContext& tc)
{
#if HC_PLATFORM != HC_PLATFORM_GDK
    SharedPtr<String> friendId = MakeShared<String>();

    ServicesTestClass::GetPlayFabIdFromCustomId(kFriendCustomId).Then([&, friendId](Result<String> result) -> AsyncOp<ClientAddFriendOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
    
        *friendId = result.ExtractPayload(); 
        ClientAddFriendOperation::RequestType request;
        request.SetFriendPlayFabId(*friendId); 
        
        return ClientAddFriendOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<ClientAddFriendOperation::ResultType> result) -> AsyncOp<ClientGetFriendsListOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual(true, result.Payload().Model().created, "created");

        PFFriendsExternalFriendSources friendSources{ PFFriendsExternalFriendSources::None };

        ClientGetFriendsListOperation::RequestType request{};
        request.SetExternalPlatformFriends(friendSources);

        return ClientGetFriendsListOperation::Run(DefaultTitlePlayer(), std::move(request), RunContext());
    })
    .Then([&, friendId](Result<ClientGetFriendsListOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
    
        auto& model = result.Payload().Model();
        tc.AssertEqual(1u, model.friendsCount, "friendsCount");
        tc.AssertEqual<String>(*friendId, model.friends[0]->friendPlayFabId, "friendPlayFabId");

        return S_OK;
    })
    .Then([&, friendId](Result<void> result) -> AsyncOp<void> 
    {
        tc.RecordResult(std::move(result));
    
        // Cleanup
        ClientRemoveFriendOperation::RequestType request;
        request.SetFriendPlayFabId(*friendId);
        
        return ClientRemoveFriendOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
#else
    ClientGetFriendsListOperation::RequestType request{ Wrappers::XUser::Duplicate(DefaultPlatformUser()->Handle()) };

    return ClientGetFriendsListOperation::Run(DefaultTitlePlayer(), request, RunContext())
    .Finally([&](Result<ClientGetFriendsListOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
#endif
}

void FriendsTests::TestClientRemoveFriend(TestContext& tc)
{
#if HC_PLATFORM != HC_PLATFORM_GDK
    // Already covered by TestClientSetFriendTags
    tc.EndTest(S_OK);
#else
    // This test uses GetPlayFabIdFromCustomId which is not supported on GDK
    tc.Skip();
#endif
}

void FriendsTests::TestClientSetFriendTags(TestContext& tc)
{
#if HC_PLATFORM != HC_PLATFORM_GDK
    SharedPtr<String> friendId = MakeShared<String>(); 

    ServicesTestClass::GetPlayFabIdFromCustomId(kFriendCustomId).Then([&, friendId](Result<String> result) -> AsyncOp<ClientAddFriendOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        *friendId = result.ExtractPayload();
        ClientAddFriendOperation::RequestType request;
        request.SetFriendPlayFabId(*friendId);
        return ClientAddFriendOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&, friendId](Result<ClientAddFriendOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        ClientSetFriendTagsOperation::RequestType request;
        request.SetFriendPlayFabId(*friendId);
        request.SetTags({ "tag1", "tag2" });
        return ClientSetFriendTagsOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&, friendId](Result<void> result)
    {
        tc.RecordResult(std::move(result));
        // Cleanup
        ClientRemoveFriendOperation::RequestType request;
        request.SetFriendPlayFabId(*friendId);
        return ClientRemoveFriendOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
#else
    // This test uses GetPlayFabIdFromCustomId which is not supported on GDK
    tc.Skip();
#endif
}

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC 
void FriendsTests::TestServerAddFriend(TestContext& tc)
{
    // Already covered by TestServerGetFriendsList
    tc.EndTest(S_OK);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void FriendsTests::TestServerGetFriendsList(TestContext& tc)
{
    SharedPtr<String> friendId = MakeShared<String>();

    ServicesTestClass::GetPlayFabIdFromCustomId(kFriendCustomId).Then([&, friendId](Result<String> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        *friendId = result.ExtractPayload();

        ServerAddFriendOperation::RequestType request;
        request.SetFriendPlayFabId(*friendId);
        request.SetPlayFabId(DefaultTitlePlayerId());

        return ServerAddFriendOperation::Run(TitleEntity(), request, RunContext());
    }).Then([&, friendId](Result<void> result) -> AsyncOp<ServerGetFriendsListOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        ServerGetFriendsListOperation::RequestType request;
        request.SetPlayFabId(DefaultTitlePlayerId());

        return ServerGetFriendsListOperation::Run(TitleEntity(), request, RunContext());
    }).Then([&, friendId](Result<ServerGetFriendsListOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual(1u, model.friendsCount, "friendsCount");
        tc.AssertEqual<String>(*friendId, model.friends[0]->friendPlayFabId, "friendPlayFabId");

        return S_OK;
    }).Then([&,friendId](Result<void> result) -> AsyncOp<void>
    {
        // Record result
        tc.RecordResult(std::move(result));

        // Cleanup
        ServerRemoveFriendOperation::RequestType request;
        request.SetFriendPlayFabId(*friendId);
        request.SetPlayFabId(DefaultTitlePlayerId());

        return ServerRemoveFriendOperation::Run(TitleEntity(), request, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void FriendsTests::TestServerRemoveFriend(TestContext& tc)
{
    // Already covered by TestServerSetFriendTags
    tc.EndTest(S_OK);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void FriendsTests::TestServerSetFriendTags(TestContext& tc)
{
    SharedPtr<String> friendId = MakeShared<String>();

    ServicesTestClass::GetPlayFabIdFromCustomId(kFriendCustomId).Then([&, friendId](Result<String> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        *friendId = result.ExtractPayload();
        ServerAddFriendOperation::RequestType request;
        request.SetPlayFabId(DefaultTitlePlayerId());
        request.SetFriendPlayFabId(*friendId);
        return ServerAddFriendOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&, friendId](Result<void> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
        ServerSetFriendTagsOperation::RequestType request;
        request.SetPlayFabId(DefaultTitlePlayerId());
        request.SetFriendPlayFabId(*friendId);
        request.SetTags({ "tag1", "tag2" });

        return ServerSetFriendTagsOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&, friendId](Result<void> result) -> AsyncOp<void>
    {
        tc.RecordResult(std::move(result));
        // Cleanup
        ServerRemoveFriendOperation::RequestType request;
        request.SetFriendPlayFabId(*friendId);
        request.SetPlayFabId(DefaultTitlePlayerId());
        return ServerRemoveFriendOperation::Run(TitleEntity(), request, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

}
}

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
        
        return ClientGetFriendsListOperation::Run(DefaultTitlePlayer(), ClientGetFriendsListOperation::RequestType{}, RunContext());
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

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32
void FriendsTests::TestServerAddFriend(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32
void FriendsTests::TestServerGetFriendsList(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32
void FriendsTests::TestServerRemoveFriend(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32
void FriendsTests::TestServerSetFriendTags(TestContext& tc)
{
    tc.Skip();
}
#endif


}
}

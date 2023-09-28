#include "TestAppPch.h"
#include "FriendsTests.h"

namespace PlayFab
{
namespace Test
{

void FriendsTests::AddTests()
{
    AddTest("TestClientAddFriend", &FriendsTests::TestClientAddFriend);
    AddTest("TestClientGetFriendsList", &FriendsTests::TestClientGetFriendsList);
    AddTest("TestClientRemoveFriend", &FriendsTests::TestClientRemoveFriend);
    AddTest("TestClientSetFriendTags", &FriendsTests::TestClientSetFriendTags);
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerAddFriend", &FriendsTests::TestServerAddFriend);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerGetFriendsList", &FriendsTests::TestServerGetFriendsList);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerRemoveFriend", &FriendsTests::TestServerRemoveFriend);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerSetFriendTags", &FriendsTests::TestServerSetFriendTags);
#endif

}

}
}

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
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerAddFriend", &FriendsTests::TestServerAddFriend);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetFriendsList", &FriendsTests::TestServerGetFriendsList);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerRemoveFriend", &FriendsTests::TestServerRemoveFriend);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerSetFriendTags", &FriendsTests::TestServerSetFriendTags);
#endif

}

}
}

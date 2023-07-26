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
#if 0
    AddTest("TestServerAddFriend", &FriendsTests::TestServerAddFriend);
#endif
#if 0
    AddTest("TestServerGetFriendsList", &FriendsTests::TestServerGetFriendsList);
#endif
#if 0
    AddTest("TestServerRemoveFriend", &FriendsTests::TestServerRemoveFriend);
#endif
#if 0
    AddTest("TestServerSetFriendTags", &FriendsTests::TestServerSetFriendTags);
#endif

}

}
}

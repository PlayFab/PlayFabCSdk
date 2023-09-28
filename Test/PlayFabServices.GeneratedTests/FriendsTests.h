#pragma once

#include "ServicesTestClass.h"

namespace PlayFab
{
namespace Test
{

class FriendsTests : public ServicesTestClass
{
public:
    using ServicesTestClass::ServicesTestClass;

private:
    void AddTests() override;

    AsyncOp<void> Initialize() override;
    AsyncOp<void> Uninitialize() override;

    SharedPtr<struct FriendsTestsState> m_state;

private:
    // Autogenerated tests
    void TestClientAddFriend(TestContext& testContext);
    void TestClientGetFriendsList(TestContext& testContext);
    void TestClientRemoveFriend(TestContext& testContext);
    void TestClientSetFriendTags(TestContext& testContext);
#if HC_PLATFORM == HC_PLATFORM_WIN32
    void TestServerAddFriend(TestContext& testContext);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    void TestServerGetFriendsList(TestContext& testContext);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    void TestServerRemoveFriend(TestContext& testContext);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    void TestServerSetFriendTags(TestContext& testContext);
#endif

};

} // namespace Test
} // namespace PlayFab

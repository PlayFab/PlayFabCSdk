#pragma once

#include "ServicesTestClass.h"

namespace PlayFab
{
namespace Test
{

class LeaderboardsTests : public ServicesTestClass
{
public:
    using ServicesTestClass::ServicesTestClass;

private:
    void AddTests() override;

    AsyncOp<void> Initialize() override;
    AsyncOp<void> Uninitialize() override;

    SharedPtr<struct LeaderboardsTestsState> m_state;

private:
    // Autogenerated tests
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    void TestCreateLeaderboardDefinition(TestContext& testContext);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    void TestDeleteLeaderboardDefinition(TestContext& testContext);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    void TestDeleteLeaderboardEntries(TestContext& testContext);
#endif
    void TestGetFriendLeaderboardForEntity(TestContext& testContext);
    void TestGetLeaderboard(TestContext& testContext);
    void TestGetLeaderboardAroundEntity(TestContext& testContext);
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    void TestGetLeaderboardDefinition(TestContext& testContext);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    void TestGetLeaderboardForEntities(TestContext& testContext);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    void TestIncrementLeaderboardVersion(TestContext& testContext);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    void TestListLeaderboardDefinitions(TestContext& testContext);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    void TestUnlinkLeaderboardFromStatistic(TestContext& testContext);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    void TestUpdateLeaderboardEntries(TestContext& testContext);
#endif

};

} // namespace Test
} // namespace PlayFab

#include "TestAppPch.h"
#include "LeaderboardsTests.h"

namespace PlayFab
{
namespace Test
{

void LeaderboardsTests::AddTests()
{
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestCreateLeaderboardDefinition", &LeaderboardsTests::TestCreateLeaderboardDefinition);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestDeleteLeaderboardDefinition", &LeaderboardsTests::TestDeleteLeaderboardDefinition);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestDeleteLeaderboardEntries", &LeaderboardsTests::TestDeleteLeaderboardEntries);
#endif
    AddTest("TestGetFriendLeaderboardForEntity", &LeaderboardsTests::TestGetFriendLeaderboardForEntity);
    AddTest("TestGetLeaderboard", &LeaderboardsTests::TestGetLeaderboard);
    AddTest("TestGetLeaderboardAroundEntity", &LeaderboardsTests::TestGetLeaderboardAroundEntity);
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestGetLeaderboardDefinition", &LeaderboardsTests::TestGetLeaderboardDefinition);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestGetLeaderboardForEntities", &LeaderboardsTests::TestGetLeaderboardForEntities);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestIncrementLeaderboardVersion", &LeaderboardsTests::TestIncrementLeaderboardVersion);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestListLeaderboardDefinitions", &LeaderboardsTests::TestListLeaderboardDefinitions);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestUnlinkLeaderboardFromStatistic", &LeaderboardsTests::TestUnlinkLeaderboardFromStatistic);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestUpdateLeaderboardEntries", &LeaderboardsTests::TestUpdateLeaderboardEntries);
#endif

}

}
}

#include "TestAppPch.h"
#include "StatisticsTests.h"

namespace PlayFab
{
namespace Test
{

void StatisticsTests::AddTests()
{
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestCreateStatisticDefinition", &StatisticsTests::TestCreateStatisticDefinition);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestDeleteStatisticDefinition", &StatisticsTests::TestDeleteStatisticDefinition);
#endif
    AddTest("TestDeleteStatistics", &StatisticsTests::TestDeleteStatistics);
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestGetStatisticDefinition", &StatisticsTests::TestGetStatisticDefinition);
#endif
    AddTest("TestGetStatistics", &StatisticsTests::TestGetStatistics);
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestGetStatisticsForEntities", &StatisticsTests::TestGetStatisticsForEntities);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestIncrementStatisticVersion", &StatisticsTests::TestIncrementStatisticVersion);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestListStatisticDefinitions", &StatisticsTests::TestListStatisticDefinitions);
#endif
    AddTest("TestUpdateStatistics", &StatisticsTests::TestUpdateStatistics);

}

}
}

#include "TestAppPch.h"
#include "MultiplayerServerTests.h"

namespace PlayFab
{
namespace Test
{

void MultiplayerServerTests::AddTests()
{
    AddTest("TestListBuildAliases", &MultiplayerServerTests::TestListBuildAliases);
    AddTest("TestListBuildSummariesV2", &MultiplayerServerTests::TestListBuildSummariesV2);
    AddTest("TestListQosServersForTitle", &MultiplayerServerTests::TestListQosServersForTitle);
    AddTest("TestRequestMultiplayerServer", &MultiplayerServerTests::TestRequestMultiplayerServer);
#if 0
    AddTest("TestRequestPartyService", &MultiplayerServerTests::TestRequestPartyService);
#endif

}

}
}

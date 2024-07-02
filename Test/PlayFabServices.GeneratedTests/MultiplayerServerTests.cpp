#include "TestAppPch.h"
#include "MultiplayerServerTests.h"

namespace PlayFab
{
namespace Test
{

void MultiplayerServerTests::AddTests()
{
#if 0
    AddTest("TestDeleteSecret", &MultiplayerServerTests::TestDeleteSecret);
#endif
    AddTest("TestListBuildAliases", &MultiplayerServerTests::TestListBuildAliases);
    AddTest("TestListBuildSummariesV2", &MultiplayerServerTests::TestListBuildSummariesV2);
    AddTest("TestListQosServersForTitle", &MultiplayerServerTests::TestListQosServersForTitle);
#if 0
    AddTest("TestListSecretSummaries", &MultiplayerServerTests::TestListSecretSummaries);
#endif
    AddTest("TestRequestMultiplayerServer", &MultiplayerServerTests::TestRequestMultiplayerServer);
#if 0
    AddTest("TestRequestPartyService", &MultiplayerServerTests::TestRequestPartyService);
#endif
#if 0
    AddTest("TestUploadSecret", &MultiplayerServerTests::TestUploadSecret);
#endif

}

}
}

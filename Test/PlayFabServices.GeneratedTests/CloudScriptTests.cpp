#include "TestAppPch.h"
#include "CloudScriptTests.h"

namespace PlayFab
{
namespace Test
{

void CloudScriptTests::AddTests()
{
    AddTest("TestClientExecuteCloudScript", &CloudScriptTests::TestClientExecuteCloudScript);
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerExecuteCloudScript", &CloudScriptTests::TestServerExecuteCloudScript);
#endif
    AddTest("TestExecuteEntityCloudScript", &CloudScriptTests::TestExecuteEntityCloudScript);
    AddTest("TestExecuteFunction", &CloudScriptTests::TestExecuteFunction);
#if 0
    AddTest("TestListEventHubFunctions", &CloudScriptTests::TestListEventHubFunctions);
#endif
#if 0
    AddTest("TestRegisterEventHubFunction", &CloudScriptTests::TestRegisterEventHubFunction);
#endif

}

}
}

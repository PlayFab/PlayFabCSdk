#include "TestAppPch.h"
#include "CloudScriptTests.h"

namespace PlayFab
{
namespace Test
{

void CloudScriptTests::AddTests()
{
    AddTest("TestClientExecuteCloudScript", &CloudScriptTests::TestClientExecuteCloudScript);
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerExecuteCloudScript", &CloudScriptTests::TestServerExecuteCloudScript);
#endif
    AddTest("TestExecuteEntityCloudScript", &CloudScriptTests::TestExecuteEntityCloudScript);
    AddTest("TestExecuteFunction", &CloudScriptTests::TestExecuteFunction);

}

}
}

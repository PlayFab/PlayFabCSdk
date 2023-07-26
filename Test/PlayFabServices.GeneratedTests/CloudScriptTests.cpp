#include "TestAppPch.h"
#include "CloudScriptTests.h"

namespace PlayFab
{
namespace Test
{

void CloudScriptTests::AddTests()
{
    AddTest("TestExecuteFunction", &CloudScriptTests::TestExecuteFunction);

}

}
}

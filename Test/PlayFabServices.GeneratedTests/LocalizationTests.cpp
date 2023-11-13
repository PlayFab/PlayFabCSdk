#include "TestAppPch.h"
#include "LocalizationTests.h"

namespace PlayFab
{
namespace Test
{

void LocalizationTests::AddTests()
{
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestGetLanguageList", &LocalizationTests::TestGetLanguageList);
#endif

}

}
}

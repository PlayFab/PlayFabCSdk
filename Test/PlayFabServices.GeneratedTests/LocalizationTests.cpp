#include "TestAppPch.h"
#include "LocalizationTests.h"

namespace PlayFab
{
namespace Test
{

void LocalizationTests::AddTests()
{
#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestGetLanguageList", &LocalizationTests::TestGetLanguageList, true);
#endif

}

}
}

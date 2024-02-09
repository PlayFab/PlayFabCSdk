#include "TestAppPch.h"
#include "TitleDataManagementTests.h"

namespace PlayFab
{
namespace Test
{

void TitleDataManagementTests::AddTests()
{
    AddTest("TestClientGetPublisherData", &TitleDataManagementTests::TestClientGetPublisherData);
    AddTest("TestClientGetTime", &TitleDataManagementTests::TestClientGetTime);
    AddTest("TestClientGetTitleData", &TitleDataManagementTests::TestClientGetTitleData);
    AddTest("TestClientGetTitleNews", &TitleDataManagementTests::TestClientGetTitleNews);
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetPublisherData", &TitleDataManagementTests::TestServerGetPublisherData);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetTime", &TitleDataManagementTests::TestServerGetTime);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetTitleData", &TitleDataManagementTests::TestServerGetTitleData);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetTitleInternalData", &TitleDataManagementTests::TestServerGetTitleInternalData);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetTitleNews", &TitleDataManagementTests::TestServerGetTitleNews);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerSetPublisherData", &TitleDataManagementTests::TestServerSetPublisherData);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerSetTitleData", &TitleDataManagementTests::TestServerSetTitleData);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerSetTitleInternalData", &TitleDataManagementTests::TestServerSetTitleInternalData);
#endif

}

}
}

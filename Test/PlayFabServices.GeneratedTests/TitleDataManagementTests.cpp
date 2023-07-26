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
#if 0
    AddTest("TestServerGetPublisherData", &TitleDataManagementTests::TestServerGetPublisherData);
#endif
#if 0
    AddTest("TestServerGetTime", &TitleDataManagementTests::TestServerGetTime);
#endif
#if 0
    AddTest("TestServerGetTitleData", &TitleDataManagementTests::TestServerGetTitleData);
#endif
#if 0
    AddTest("TestServerGetTitleInternalData", &TitleDataManagementTests::TestServerGetTitleInternalData);
#endif
#if 0
    AddTest("TestServerGetTitleNews", &TitleDataManagementTests::TestServerGetTitleNews);
#endif
#if 0
    AddTest("TestServerSetPublisherData", &TitleDataManagementTests::TestServerSetPublisherData);
#endif
#if 0
    AddTest("TestServerSetTitleData", &TitleDataManagementTests::TestServerSetTitleData);
#endif
#if 0
    AddTest("TestServerSetTitleInternalData", &TitleDataManagementTests::TestServerSetTitleInternalData);
#endif

}

}
}

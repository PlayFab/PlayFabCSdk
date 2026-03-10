#include "TestAppPch.h"
#include "PlayerDataManagementTests.h"

namespace PlayFab
{
namespace Test
{

void PlayerDataManagementTests::AddTests()
{
    AddTest("TestClientDeletePlayerCustomProperties", &PlayerDataManagementTests::TestClientDeletePlayerCustomProperties);
    AddTest("TestClientGetPlayerCustomProperty", &PlayerDataManagementTests::TestClientGetPlayerCustomProperty);
    AddTest("TestClientGetUserData", &PlayerDataManagementTests::TestClientGetUserData);
    AddTest("TestClientGetUserPublisherData", &PlayerDataManagementTests::TestClientGetUserPublisherData);
    AddTest("TestClientGetUserPublisherReadOnlyData", &PlayerDataManagementTests::TestClientGetUserPublisherReadOnlyData);
    AddTest("TestClientGetUserReadOnlyData", &PlayerDataManagementTests::TestClientGetUserReadOnlyData);
    AddTest("TestClientListPlayerCustomProperties", &PlayerDataManagementTests::TestClientListPlayerCustomProperties);
    AddTest("TestClientUpdatePlayerCustomProperties", &PlayerDataManagementTests::TestClientUpdatePlayerCustomProperties);
    AddTest("TestClientUpdateUserData", &PlayerDataManagementTests::TestClientUpdateUserData);
    AddTest("TestClientUpdateUserPublisherData", &PlayerDataManagementTests::TestClientUpdateUserPublisherData);
#if HC_PLATFORM == HC_PLATFORM_GDK
    AddTest("TestServerDeletePlayerCustomProperties", &PlayerDataManagementTests::TestServerDeletePlayerCustomProperties);
#endif
#if HC_PLATFORM == HC_PLATFORM_GDK
    AddTest("TestServerGetPlayerCustomProperty", &PlayerDataManagementTests::TestServerGetPlayerCustomProperty);
#endif
#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetUserData", &PlayerDataManagementTests::TestServerGetUserData);
#endif
#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetUserInternalData", &PlayerDataManagementTests::TestServerGetUserInternalData);
#endif
#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetUserPublisherData", &PlayerDataManagementTests::TestServerGetUserPublisherData);
#endif
#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetUserPublisherInternalData", &PlayerDataManagementTests::TestServerGetUserPublisherInternalData);
#endif
#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetUserPublisherReadOnlyData", &PlayerDataManagementTests::TestServerGetUserPublisherReadOnlyData);
#endif
#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerGetUserReadOnlyData", &PlayerDataManagementTests::TestServerGetUserReadOnlyData);
#endif
#if HC_PLATFORM == HC_PLATFORM_GDK
    AddTest("TestServerListPlayerCustomProperties", &PlayerDataManagementTests::TestServerListPlayerCustomProperties);
#endif
#if HC_PLATFORM == HC_PLATFORM_GDK
    AddTest("TestServerUpdatePlayerCustomProperties", &PlayerDataManagementTests::TestServerUpdatePlayerCustomProperties);
#endif
#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerUpdateUserData", &PlayerDataManagementTests::TestServerUpdateUserData);
#endif
#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerUpdateUserInternalData", &PlayerDataManagementTests::TestServerUpdateUserInternalData);
#endif
#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerUpdateUserPublisherData", &PlayerDataManagementTests::TestServerUpdateUserPublisherData);
#endif
#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerUpdateUserPublisherInternalData", &PlayerDataManagementTests::TestServerUpdateUserPublisherInternalData);
#endif
#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerUpdateUserPublisherReadOnlyData", &PlayerDataManagementTests::TestServerUpdateUserPublisherReadOnlyData);
#endif
#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerUpdateUserReadOnlyData", &PlayerDataManagementTests::TestServerUpdateUserReadOnlyData);
#endif

}

}
}

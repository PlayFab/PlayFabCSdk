#include "TestAppPch.h"
#include "PlayerDataManagementTests.h"

namespace PlayFab
{
namespace Test
{

void PlayerDataManagementTests::AddTests()
{
    AddTest("TestClientGetUserData", &PlayerDataManagementTests::TestClientGetUserData);
    AddTest("TestClientGetUserPublisherData", &PlayerDataManagementTests::TestClientGetUserPublisherData);
    AddTest("TestClientGetUserPublisherReadOnlyData", &PlayerDataManagementTests::TestClientGetUserPublisherReadOnlyData);
    AddTest("TestClientGetUserReadOnlyData", &PlayerDataManagementTests::TestClientGetUserReadOnlyData);
    AddTest("TestClientUpdateUserData", &PlayerDataManagementTests::TestClientUpdateUserData);
    AddTest("TestClientUpdateUserPublisherData", &PlayerDataManagementTests::TestClientUpdateUserPublisherData);
#if 0
    AddTest("TestServerGetUserData", &PlayerDataManagementTests::TestServerGetUserData);
#endif
#if 0
    AddTest("TestServerGetUserInternalData", &PlayerDataManagementTests::TestServerGetUserInternalData);
#endif
#if 0
    AddTest("TestServerGetUserPublisherData", &PlayerDataManagementTests::TestServerGetUserPublisherData);
#endif
#if 0
    AddTest("TestServerGetUserPublisherInternalData", &PlayerDataManagementTests::TestServerGetUserPublisherInternalData);
#endif
#if 0
    AddTest("TestServerGetUserPublisherReadOnlyData", &PlayerDataManagementTests::TestServerGetUserPublisherReadOnlyData);
#endif
#if 0
    AddTest("TestServerGetUserReadOnlyData", &PlayerDataManagementTests::TestServerGetUserReadOnlyData);
#endif
#if 0
    AddTest("TestServerUpdateUserData", &PlayerDataManagementTests::TestServerUpdateUserData);
#endif
#if 0
    AddTest("TestServerUpdateUserInternalData", &PlayerDataManagementTests::TestServerUpdateUserInternalData);
#endif
#if 0
    AddTest("TestServerUpdateUserPublisherData", &PlayerDataManagementTests::TestServerUpdateUserPublisherData);
#endif
#if 0
    AddTest("TestServerUpdateUserPublisherInternalData", &PlayerDataManagementTests::TestServerUpdateUserPublisherInternalData);
#endif
#if 0
    AddTest("TestServerUpdateUserPublisherReadOnlyData", &PlayerDataManagementTests::TestServerUpdateUserPublisherReadOnlyData);
#endif
#if 0
    AddTest("TestServerUpdateUserReadOnlyData", &PlayerDataManagementTests::TestServerUpdateUserReadOnlyData);
#endif

}

}
}

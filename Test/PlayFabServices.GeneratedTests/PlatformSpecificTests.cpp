#include "TestAppPch.h"
#include "PlatformSpecificTests.h"

namespace PlayFab
{
namespace Test
{

void PlatformSpecificTests::AddTests()
{
#if 0
    AddTest("TestClientAndroidDevicePushNotificationRegistration", &PlatformSpecificTests::TestClientAndroidDevicePushNotificationRegistration);
#endif
#if 0
    AddTest("TestClientRefreshPSNAuthToken", &PlatformSpecificTests::TestClientRefreshPSNAuthToken);
#endif
#if 0
    AddTest("TestClientRegisterForIOSPushNotification", &PlatformSpecificTests::TestClientRegisterForIOSPushNotification);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerAwardSteamAchievement", &PlatformSpecificTests::TestServerAwardSteamAchievement);
#endif

}

}
}

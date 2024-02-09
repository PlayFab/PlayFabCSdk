#include "TestAppPch.h"
#include "PlatformSpecificTests.h"

namespace PlayFab
{
namespace Test
{

void PlatformSpecificTests::AddTests()
{
#if HC_PLATFORM == HC_PLATFORM_ANDROID
    AddTest("TestClientAndroidDevicePushNotificationRegistration", &PlatformSpecificTests::TestClientAndroidDevicePushNotificationRegistration);
#endif
#if 0
    AddTest("TestClientRefreshPSNAuthToken", &PlatformSpecificTests::TestClientRefreshPSNAuthToken);
#endif
#if HC_PLATFORM == HC_PLATFORM_IOS
    AddTest("TestClientRegisterForIOSPushNotification", &PlatformSpecificTests::TestClientRegisterForIOSPushNotification);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerAwardSteamAchievement", &PlatformSpecificTests::TestServerAwardSteamAchievement);
#endif

}

}
}

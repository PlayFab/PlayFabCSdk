#include "TestAppPch.h"
#include "PlatformSpecificTests.h"
#include "PlatformSpecificOperations.h"

namespace PlayFab
{
namespace Test
{

AsyncOp<void> PlatformSpecificTests::Initialize()
{
    return ServicesTestClass::Initialize();
}

AsyncOp<void> PlatformSpecificTests::Uninitialize()
{
    return ServicesTestClass::Uninitialize();
}

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32
void PlatformSpecificTests::TestClientAndroidDevicePushNotificationRegistration(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32
void PlatformSpecificTests::TestClientRefreshPSNAuthToken(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32
void PlatformSpecificTests::TestClientRegisterForIOSPushNotification(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32
void PlatformSpecificTests::TestServerAwardSteamAchievement(TestContext& tc)
{
    tc.Skip();
}
#endif


}
}

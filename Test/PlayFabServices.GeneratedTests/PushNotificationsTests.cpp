#include "TestAppPch.h"
#include "PushNotificationsTests.h"

namespace PlayFab
{
namespace Test
{

void PushNotificationsTests::AddTests()
{
#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerSendPushNotification", &PushNotificationsTests::TestServerSendPushNotification);
#endif
#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    AddTest("TestServerSendPushNotificationFromTemplate", &PushNotificationsTests::TestServerSendPushNotificationFromTemplate, true);
#endif

}

}
}

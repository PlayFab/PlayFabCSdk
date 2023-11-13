#include "TestAppPch.h"
#include "PushNotificationsTests.h"

namespace PlayFab
{
namespace Test
{

void PushNotificationsTests::AddTests()
{
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerSendPushNotification", &PushNotificationsTests::TestServerSendPushNotification);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX
    AddTest("TestServerSendPushNotificationFromTemplate", &PushNotificationsTests::TestServerSendPushNotificationFromTemplate);
#endif

}

}
}

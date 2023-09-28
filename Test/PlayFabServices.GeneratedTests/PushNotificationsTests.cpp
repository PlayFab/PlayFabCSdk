#include "TestAppPch.h"
#include "PushNotificationsTests.h"

namespace PlayFab
{
namespace Test
{

void PushNotificationsTests::AddTests()
{
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerSendPushNotification", &PushNotificationsTests::TestServerSendPushNotification);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32
    AddTest("TestServerSendPushNotificationFromTemplate", &PushNotificationsTests::TestServerSendPushNotificationFromTemplate);
#endif

}

}
}

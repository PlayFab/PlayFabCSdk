#include "TestAppPch.h"
#include "PushNotificationsTests.h"
#include "PushNotificationsOperations.h"

namespace PlayFab
{
namespace Test
{

AsyncOp<void> PushNotificationsTests::Initialize()
{
    return ServicesTestClass::Initialize();
}

AsyncOp<void> PushNotificationsTests::Uninitialize()
{
    return ServicesTestClass::Uninitialize();
}

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void PushNotificationsTests::TestServerSendPushNotification(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void PushNotificationsTests::TestServerSendPushNotificationFromTemplate(TestContext& tc)
{
    tc.Skip();
}
#endif


}
}

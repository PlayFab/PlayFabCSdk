#pragma once

#include "ServicesTestClass.h"

namespace PlayFab
{
namespace Test
{

class PushNotificationsTests : public ServicesTestClass
{
public:
    using ServicesTestClass::ServicesTestClass;

private:
    void AddTests() override;

    AsyncOp<void> Initialize() override;
    AsyncOp<void> Uninitialize() override;

    SharedPtr<struct PushNotificationsTestsState> m_state;

private:
    // Autogenerated tests
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    void TestServerSendPushNotification(TestContext& testContext);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    void TestServerSendPushNotificationFromTemplate(TestContext& testContext);
#endif

};

} // namespace Test
} // namespace PlayFab

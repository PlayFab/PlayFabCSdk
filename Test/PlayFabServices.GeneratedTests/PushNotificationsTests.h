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
#if 0
    void TestServerSendPushNotification(TestContext& testContext);
#endif
#if 0
    void TestServerSendPushNotificationFromTemplate(TestContext& testContext);
#endif

};

} // namespace Test
} // namespace PlayFab
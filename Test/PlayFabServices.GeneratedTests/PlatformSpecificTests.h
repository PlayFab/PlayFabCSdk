#pragma once

#include "ServicesTestClass.h"

namespace PlayFab
{
namespace Test
{

class PlatformSpecificTests : public ServicesTestClass
{
public:
    using ServicesTestClass::ServicesTestClass;

private:
    void AddTests() override;

    AsyncOp<void> Initialize() override;
    AsyncOp<void> Uninitialize() override;

    SharedPtr<struct PlatformSpecificTestsState> m_state;

private:
    // Autogenerated tests
#if HC_PLATFORM == HC_PLATFORM_ANDROID
    void TestClientAndroidDevicePushNotificationRegistration(TestContext& testContext);
#endif
#if 0
    void TestClientRefreshPSNAuthToken(TestContext& testContext);
#endif
#if HC_PLATFORM == HC_PLATFORM_IOS
    void TestClientRegisterForIOSPushNotification(TestContext& testContext);
#endif
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    void TestServerAwardSteamAchievement(TestContext& testContext);
#endif

};

} // namespace Test
} // namespace PlayFab

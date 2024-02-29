#include "TestAppPch.h"
#include "PlatformSpecificTests.h"
#include "PlatformSpecificOperations.h"
#include "PlatformUtils.h"
#if HC_PLATFORM == HC_PLATFORM_ANDROID
#include "Platform/Android/AndroidTestApp.h"
#endif

namespace PlayFab
{
namespace Test
{
#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
    constexpr char kAchievementName[] = { "testAchievement" };
    constexpr char customSteamId[] = { "SteamPlayerCustomID" };
#endif // HC_PLATFORM_WIN32

#if HC_PLATFORM == HC_PLATFORM_IOS
    constexpr char dummyDeviceToken[] = { "1e5b2337a9abd90c7f2e8534006283d3b578295c6bc416e3213bb0d174a9ab62" };
#endif

AsyncOp<void> PlatformSpecificTests::Initialize()
{
    return ServicesTestClass::Initialize();
}

AsyncOp<void> PlatformSpecificTests::Uninitialize()
{
    return ServicesTestClass::Uninitialize();
}

#if HC_PLATFORM == HC_PLATFORM_ANDROID
void PlatformSpecificTests::TestClientAndroidDevicePushNotificationRegistration(TestContext& tc)
{
    ClientAndroidDevicePushNotificationRegistrationOperation::RequestType request;
    request.SetDeviceToken(AndroidTestApp::TestApp::GetInstance().GetDeviceToken());
    request.SetConfirmationMessage("Test Confirmation Message");
    request.SetSendPushNotificationConfirmation(true);

    ClientAndroidDevicePushNotificationRegistrationOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
    {
        tc.AssertEqual(E_PF_NO_PUSH_NOTIFICATION_ARN_FOR_TITLE, result.hr, "errorName");
        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if 0
void PlatformSpecificTests::TestClientRefreshPSNAuthToken(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM == HC_PLATFORM_IOS

#ifdef __cplusplus
extern "C"
{
#endif

// This is a Swift method that we call to get the Device Token when registering for iOS Push Notifications
extern const char* GetDeviceToken();

#ifdef __cplusplus
}
#endif

Result<String> GetDeviceTokenForNotifications() noexcept
{
    std::string deviceToken = "";
    const char* token = GetDeviceToken();
    deviceToken = token;
    return String{ deviceToken };
}

void PlatformSpecificTests::TestClientRegisterForIOSPushNotification(TestContext& tc)
{
    String token = GetDeviceTokenForNotifications().ExtractPayload();
    
    if (token == "")
    {
        token = dummyDeviceToken;
    }

    ClientRegisterForIOSPushNotificationOperation::RequestType request;
    request.SetDeviceToken(token);
    request.SetConfirmationMessage("Test Confirmation Message");
    request.SetSendPushNotificationConfirmation(true);
    
    ClientRegisterForIOSPushNotificationOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<void> result) -> Result<void>
                                                                                                         {
        RETURN_IF_FAILED_PLAYFAB(result);
        return S_OK;
    })
    .Finally([&](Result<void> result)
             {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void PlatformSpecificTests::TestServerAwardSteamAchievement(TestContext& tc)
{ 

    ServicesTestClass::GetPlayFabIdFromCustomId(customSteamId).Then([&](Result<String> result) -> AsyncOp<ServerAwardSteamAchievementOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        ServerAwardSteamAchievementOperation::RequestType request;
        ModelVector<Wrappers::PFPlatformSpecificAwardSteamAchievementItemWrapper<Allocator>> achievements;
        Wrappers::PFPlatformSpecificAwardSteamAchievementItemWrapper<Allocator> testAchievement;
        testAchievement.SetAchievementName(kAchievementName);
        testAchievement.SetPlayFabId(result.Payload());
        achievements.push_back(std::move(testAchievement));
        request.SetAchievements(std::move(achievements));
        return ServerAwardSteamAchievementOperation::Run(TitleEntity(), request, RunContext());

    })
    .Then([&](Result<ServerAwardSteamAchievementOperation::ResultType> result) -> AsyncOp<void>
    { 
        RETURN_IF_FAILED_PLAYFAB(result);
        auto model = result.Payload().Model();
        tc.AssertEqual(1u, model.achievementResultsCount, "achieveResultCount");
        tc.AssertEqual(kAchievementName, model.achievementResults[0]->achievementName, "achievementName");
        tc.AssertEqual(false, model.achievementResults[0]->result, "achievementResult");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
    
}
#endif


}
}

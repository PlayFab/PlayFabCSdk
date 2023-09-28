#include "TestAppPch.h"
#include "PlatformSpecificTests.h"
#include "PlatformSpecificOperations.h"

namespace PlayFab
{
namespace Test
{
#if HC_PLATFORM == HC_PLATFORM_WIN32
    constexpr char kAchievementName[] = { "testAchievement" };
    constexpr char customSteamId[] = { "SteamPlayerCustomID" };
#endif // HC_PLATFORM_WIN32

AsyncOp<void> PlatformSpecificTests::Initialize()
{
    return ServicesTestClass::Initialize();
}

AsyncOp<void> PlatformSpecificTests::Uninitialize()
{
    return ServicesTestClass::Uninitialize();
}

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void PlatformSpecificTests::TestClientAndroidDevicePushNotificationRegistration(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void PlatformSpecificTests::TestClientRefreshPSNAuthToken(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_GDK && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_4 && HC_PLATFORM != HC_PLATFORM_SONY_PLAYSTATION_5
void PlatformSpecificTests::TestClientRegisterForIOSPushNotification(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32
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

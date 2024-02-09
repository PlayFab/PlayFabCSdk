#include "TestAppPch.h"
#include "PushNotificationsTests.h"
#include "PushNotificationsOperations.h"
#include "SdkVersion.h"
#include "HttpRequest.h"
#include "JsonUtils.h"
#include "Error.h"

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

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void PushNotificationsTests::TestServerSendPushNotification(TestContext& tc)
{
     ServerSendPushNotificationOperation::RequestType request;
     request.SetRecipient(DefaultTitlePlayerId());
     request.SetMessage("Test message");

     ServerSendPushNotificationOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<void> result) -> AsyncOp<void>
     {
         tc.AssertTrue(result.hr == E_PF_PUSH_NOT_ENABLED_FOR_ACCOUNT || result.hr == E_PF_PUSH_SERVICE_ERROR, "Unexpected error received");
         return S_OK;
     })
     .Finally([&](Result<void> result)
     {
         tc.EndTest(std::move(result));
     });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void PushNotificationsTests::TestServerSendPushNotificationFromTemplate(TestContext& tc)
{
     ServerSendPushNotificationFromTemplateOperation::RequestType request;
     request.SetRecipient(DefaultTitlePlayerId());
     request.SetPushNotificationTemplateId("4402ef11-de66-4c2b-89bb-e0c8df157aeb");

     ServerSendPushNotificationFromTemplateOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<void> result) -> AsyncOp<void>
     {
         tc.AssertTrue(result.hr == E_PF_PUSH_NOT_ENABLED_FOR_ACCOUNT || result.hr == E_PF_PUSH_SERVICE_ERROR, "Unexpected error received");
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

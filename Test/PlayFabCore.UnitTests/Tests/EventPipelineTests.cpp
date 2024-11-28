#include "stdafx.h"
#include "TestIncludes.h"
#include <playfab/core/cpp/EventPipeline.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PlayFab
{
namespace UnitTests
{

constexpr const char* kTestEventTemplate =
R"({
  "eventId": 0
})";

TEST_CLASS(EventPipelineTests)
{
public:
    TEST_METHOD(TestSingleEventDefaultSettings)
    {
        // TODO Re-enable when pipeline is fixed

        //Wrappers::TestSession session;
        //Wrappers::ServiceConfig<> serviceConfig{ "mockAPIEndpoint", "mockTitleId" };

        //PlayFabServiceMock loginMock{serviceConfig, "LoginWithCustomID"};

        ////const char* titleId = "AADE6";
        ////const char* endpoint = "https://AADE6.playfabapi.com";
        ////const char* customId = "TestIntegrationLoginCustomId";

        ////PFServiceConfigHandle serviceConfigHandle{ nullptr };
        ////HRESULT hr = PFServiceConfigCreateHandle(
        ////    endpoint,
        ////    titleId,
        ////    &serviceConfigHandle);
        ////std::cout << hr;

        //// Synchronously authenticate
        //XAsyncBlock async{};
        //PFAuthenticationLoginWithCustomIDRequest request{};
        ////request.createAccount = true;
        ////request.customId = customId;
        //VERIFY_SUCCEEDED(PFAuthenticationLoginWithCustomIDAsync(serviceConfig.Handle(), &request, &async));
        //VERIFY_SUCCEEDED(XAsyncGetStatus(&async, true));
        //PFEntityHandle entityHandle{ nullptr };
        //VERIFY_SUCCEEDED(PFAuthenticationLoginWithCustomIDGetResult(&async, &entityHandle, 0, nullptr, nullptr, nullptr));
        //auto entity = Wrappers::Entity<>::Wrap(entityHandle);

        //PlayFabServiceMock writeEventsMock{ serviceConfig, "WriteEvents" };

        //AsyncTestContext tc;
        //writeEventsMock.SetCallback([&](PlayFabServiceMock& mock, std::string url, std::string requestBody, uint32_t hitCount)
        //    {
        //        nlohmann::Document requestJson;
        //        requestJson.Parse(requestBody.data());
        //        nlohmann::Document payloadJson;
        //        payloadJson.Parse(requestJson["Events"][0]["PayloadJSON"].get<String>());
        //        tc.Assert(payloadJson["eventId"].GetInt() == 0);
        //        std::cout << hitCount << url;
        //        mock.JsonAllocator();
        //        tc.Complete();
        //    });
        //
        //PFEventPipelineHandle eventsHandle;
        //VERIFY_SUCCEEDED(PFEventPipelineCreateTelemetryPipelineHandleWithEntity(entityHandle, nullptr, nullptr, nullptr, nullptr, &eventsHandle));
        //PFEvent event
        //{
        //    nullptr,
        //    "playfab.core",
        //    "testEvent",
        //    nullptr,
        //    kTestEventTemplate
        //};

        //VERIFY_SUCCEEDED(PFEventPipelineEmitEvent(eventsHandle, &event));

        //tc.AwaitResult();
    }

    TEST_METHOD(TestMultipleEvents)
    {
        // TODO Re-enable when pipeline is fixed

        //Wrappers::TestSession session;
        //Wrappers::ServiceConfig<> serviceConfig{ "mockAPIEndpoint", "mockTitleId" };

        //// Configure Mocks
        //PlayFabServiceMock loginMock{ serviceConfig, "LoginWithCustomID" };

        //// Synchronously authenticate
        //XAsyncBlock async{};
        //PFAuthenticationLoginWithCustomIDRequest request{};
        //VERIFY_SUCCEEDED(PFAuthenticationLoginWithCustomIDAsync(serviceConfig.Handle(), &request, &async));
        //VERIFY_SUCCEEDED(XAsyncGetStatus(&async, true));
        //PFEntityHandle entityHandle{ nullptr };
        //VERIFY_SUCCEEDED(PFAuthenticationLoginWithCustomIDGetResult(&async, &entityHandle, 0, nullptr, nullptr, nullptr));
        //auto entity = Wrappers::Entity<>::Wrap(entityHandle);

        //Wrappers::TelemetryPipeline pipeline{ entity.Handle(), nullptr };

        //PlayFabServiceMock writeEventsMock{ serviceConfig, "WriteEvents" };

        //std::set<int> eventIdsToWrite{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        //size_t eventsWritten = 0;

        //AsyncTestContext tc;
        //writeEventsMock.SetCallback([&](PlayFabServiceMock& mock, std::string url, std::string requestBody, uint32_t hitCount)
        //{
        //    nlohmann::Document requestJson;
        //    requestJson.Parse(requestBody.data());

        //    auto& eventsJson = requestJson["Events"];
        //    for (auto& eventJson : eventsJson.GetArray())
        //    {
        //        nlohmann::Document payloadJson;
        //        payloadJson.Parse(eventJson["PayloadJSON"].get<String>());
        //        tc.Assert(eventIdsToWrite.find(payloadJson["eventId"].GetInt()) != eventIdsToWrite.end());
        //        ++eventsWritten;
        //    }

        //    if (eventsWritten == eventIdsToWrite.size())
        //    {
        //        tc.Complete();
        //    }
        //});

        //nlohmann::Document eventPayloadJson;
        //eventPayloadJson.Parse(kTestEventTemplate);

        //PFTelemetryEvent event
        //{
        //    nullptr,
        //    "playfab.core",
        //    "testEvent",
        //    nullptr,
        //    nullptr
        //};

        //for (auto& eventId : eventIdsToWrite)
        //{
        //    eventPayloadJson["eventId"] = eventId;

        //    rapidjson::StringBuffer buffer;
        //    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        //    eventPayloadJson.Accept(writer);

        //    event.payloadJson = buffer.get<String>();
        //    pipeline.EmitEvent(&event);
        //}

        //tc.AwaitResult();
    }
};

}
}
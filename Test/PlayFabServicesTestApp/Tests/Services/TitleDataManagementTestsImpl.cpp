#include "TestAppPch.h"
#include "TitleDataManagementTests.h"
#include "TitleDataManagementOperations.h"
#include "SdkVersion.h"
#include "HttpRequest.h"
#include "JsonUtils.h"
#include "Platform/PlatformUtils.h"

namespace PlayFab
{
namespace Test
{

constexpr char kTestKey[]{ "testKey" };
constexpr char kTestVal[]{ "testVal" };
constexpr time_t kTestTime{ 12345u };

AsyncOp<void> ServerSetTitleData(ServiceConfig serviceConfig, String secretKey, String path, bool cleanup, PlayFab::RunContext rc) noexcept
{
    JsonValue requestBody{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(requestBody, "key", kTestKey);

    if (cleanup)
    {
        JsonValue nullVal{ rapidjson::kNullType };
        JsonUtils::ObjectAddMember(requestBody, "value", nullVal);
    }
    else
    {
        JsonUtils::ObjectAddMember(requestBody, "value", kTestVal);
    }

    Stringstream headerVal;
    headerVal << versionString << sdkVersion;
    UnorderedMap<String, String> headers{ { kSDKVersionStringHeaderName, headerVal.str() } };
    headers[kSecretKeyHeaderName] = secretKey;

    Stringstream fullUrl;
    fullUrl << serviceConfig.APIEndpoint();

    // Append path
    fullUrl << path;

    // Add sdk version param (used by service for telemetry)
    fullUrl << "?sdk=" << versionString << sdkVersion;

    return RunOperation(MakeUnique<HCHttpCall>(kPostMethod, fullUrl.str(), std::move(headers), JsonUtils::WriteToString(requestBody), std::move(rc)))
    .Then([](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);
        return ServiceErrorToHR(result.Payload().ErrorCode);
    });
}

AsyncOp<void> InitialAdminAddNews(ServiceConfig serviceConfig, String secretKey, PlayFab::RunContext rc) noexcept
{
    const char* path{ "/Admin/AddNews" };

    JsonValue requestBody{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(requestBody, "title", kTestKey);
    JsonUtils::ObjectAddMember(requestBody, "body", kTestVal);
    JsonUtils::ObjectAddMember(requestBody, "timestamp", JsonUtils::ToJsonTime(kTestTime));

    Stringstream headerVal;
    headerVal << versionString << sdkVersion;
    UnorderedMap<String, String> headers{ { kSDKVersionStringHeaderName, headerVal.str() } };
    headers[kSecretKeyHeaderName] = secretKey;

    Stringstream fullUrl;
    fullUrl << serviceConfig.APIEndpoint();

    // Append path
    fullUrl << path;

    // Add sdk version param (used by service for telemetry)
    fullUrl << "?sdk=" << versionString << sdkVersion;

    return RunOperation(MakeUnique<HCHttpCall>(kPostMethod, fullUrl.str(), std::move(headers), JsonUtils::WriteToString(requestBody), std::move(rc)))
    .Then([](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);
        return ServiceErrorToHR(result.Payload().ErrorCode);
    });
}

AsyncOp<void> TitleDataManagementTests::Initialize()
{
    return ServicesTestClass::Initialize();
}

AsyncOp<void> TitleDataManagementTests::Uninitialize()
{
    return ServicesTestClass::Uninitialize();
}

void TitleDataManagementTests::TestClientGetPublisherData(TestContext& tc)
{
    // Docs specify it may take results from SetPublisherData up to a couple of minutes to show in Get requests. 
    // Not adding a cleanup step here to ensure consistent results.
    ServerSetTitleData(ServiceConfig(), m_testTitleData.secretKey, "/Server/SetPublisherData", false, RunContext()).Then([&](Result<void> result) -> AsyncOp<ClientGetPublisherDataOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        return ClientGetPublisherDataOperation::Run(DefaultTitlePlayer(), {}, RunContext());
    })
    .Then([&](Result<ClientGetPublisherDataOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual(1u, model.dataCount, "dataCount");
        tc.AssertEqual(kTestKey, model.data[0].key, "data[0].key");
        tc.AssertEqual(kTestVal, model.data[0].value, "data[0].value");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void TitleDataManagementTests::TestClientGetTime(TestContext& tc)
{
    ClientGetTimeOperation::Run(DefaultTitlePlayer(), RunContext()).Then([&](Result<ClientGetTimeOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        // Verify result time is within the last 10 seconds. Docs specify there may be a variance of up to 5 seconds.
        const time_t now = Platform::GetSystemTime();
        const int expectedSeconds = 6;
        const auto actualSeconds = abs(difftime(now, result.Payload().Model().time));

        tc.AssertTrue(actualSeconds < expectedSeconds, "Returned time differs from system time by more than expected variance");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void TitleDataManagementTests::TestClientGetTitleData(TestContext& tc)
{
    ServerSetTitleData(ServiceConfig(), m_testTitleData.secretKey, "/Server/SetTitleData", false, RunContext()).Then([&](Result<void> result) -> AsyncOp<ClientGetTitleDataOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        return ClientGetTitleDataOperation::Run(DefaultTitlePlayer(), {}, RunContext());
    })
    .Then([&](Result<ClientGetTitleDataOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual(1u, model.dataCount, "dataCount");
        tc.AssertEqual(kTestKey, model.data[0].key, "data[0].key");
        tc.AssertEqual(kTestVal, model.data[0].value, "data[0].value");

        return S_OK;
    })
    .Then([&](Result<void> result)
    {
        tc.RecordResult(std::move(result));

        return ServerSetTitleData(ServiceConfig(), m_testTitleData.secretKey, "/Server/SetTitleData", true, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void TitleDataManagementTests::TestClientGetTitleNews(TestContext& tc)
{
    // Deleting news can only be done manually through the portal, and there is a very low limit (about 10) for news entries.
    // The setup for this test will only create a news item if needed and afterward, the test will check against that permanent item.
    ClientGetTitleNewsOperation::Run(DefaultTitlePlayer(), {}, RunContext()).Then([&](Result<ClientGetTitleNewsOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
    
        auto& model = result.Payload().Model();
        if (model.newsCount == 0)
        {
            return InitialAdminAddNews(ServiceConfig(), m_testTitleData.secretKey, RunContext());
        }

        return S_OK;
    })
    .Then([&](Result<void> result) -> AsyncOp<ClientGetTitleNewsOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        return ClientGetTitleNewsOperation::Run(DefaultTitlePlayer(), {}, RunContext());
    })
    .Then([&](Result<ClientGetTitleNewsOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);
    
        auto& model = result.Payload().Model();
        tc.AssertEqual(1u, model.newsCount, "newsCount");
        tc.AssertEqual(kTestKey, model.news[0]->title, "news[0]->title");
        tc.AssertEqual(kTestVal, model.news[0]->body, "news[0]->body");
        tc.AssertEqual(kTestTime, model.news[0]->timestamp, "news[0]->timestamp");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void TitleDataManagementTests::TestServerGetPublisherData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void TitleDataManagementTests::TestServerGetTime(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void TitleDataManagementTests::TestServerGetTitleData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void TitleDataManagementTests::TestServerGetTitleInternalData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void TitleDataManagementTests::TestServerGetTitleNews(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void TitleDataManagementTests::TestServerSetPublisherData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void TitleDataManagementTests::TestServerSetTitleData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void TitleDataManagementTests::TestServerSetTitleInternalData(TestContext& tc)
{
    tc.Skip();
}
#endif


}
}

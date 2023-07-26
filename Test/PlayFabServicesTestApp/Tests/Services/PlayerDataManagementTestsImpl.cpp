#include "TestAppPch.h"
#include "PlayerDataManagementTests.h"
#include "PlayerDataManagementOperations.h"
#include "SdkVersion.h"
#include "HttpRequest.h"
#include "JsonUtils.h"

namespace PlayFab
{
namespace Test
{

constexpr char kTestKey[]{ "testKey" };
constexpr char kTestVal[]{ "testVal" };

AsyncOp<void> ServerUpdateUserReadonlyData(ServiceConfig serviceConfig, String secretKey, String playFabId, String path, bool cleanup, PlayFab::RunContext rc) noexcept
{
    JsonValue requestBody{ rapidjson::kObjectType };
    JsonUtils::ObjectAddMember(requestBody, "PlayFabId", playFabId);

    if (cleanup)
    {
        JsonUtils::ObjectAddMemberArray(requestBody, "KeysToRemove", Vector<String>{ kTestKey });
    }
    else
    {
        JsonUtils::ObjectAddMemberDictionary(requestBody, "Data", Map<String, String>{ {kTestKey, kTestVal} });
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

AsyncOp<void> PlayerDataManagementTests::Initialize()
{
    return ServicesTestClass::Initialize();
}

AsyncOp<void> PlayerDataManagementTests::Uninitialize()
{
    return ServicesTestClass::Uninitialize();
}

void PlayerDataManagementTests::TestClientGetUserData(TestContext& tc)
{
    // Already covered in TestClientUpdateUserData
    tc.EndTest(S_OK);
}

void PlayerDataManagementTests::TestClientGetUserPublisherData(TestContext& tc)
{
    // Already covered in TestClientUpdateUserPublisherData
    tc.EndTest(S_OK);
}

void PlayerDataManagementTests::TestClientGetUserPublisherReadOnlyData(TestContext& tc)
{
    ServerUpdateUserReadonlyData(ServiceConfig(), m_testTitleData.secretKey, DefaultTitlePlayerId(), "/Server/UpdateUserPublisherReadOnlyData", false, RunContext()).Then([&](Result<void> result) -> AsyncOp<ClientGetUserPublisherReadOnlyDataOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        ClientGetUserPublisherReadOnlyDataOperation::RequestType request;
        request.SetKeys({ kTestKey });

        return ClientGetUserPublisherReadOnlyDataOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<ClientGetUserPublisherReadOnlyDataOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual(1u, result.Payload().Model().dataCount, "dataCount");
        tc.AssertEqual<String>(kTestKey, result.Payload().Model().data[0].key, "dataKey");
        tc.AssertEqual<String>(kTestVal, result.Payload().Model().data[0].value->value, "dataValue");

        return S_OK;
    })
    .Then([&](Result<void> result) -> AsyncOp<void>
    {
        tc.RecordResult(std::move(result));

        // Cleanup: remove keys
        return ServerUpdateUserReadonlyData(ServiceConfig(), m_testTitleData.secretKey, DefaultTitlePlayerId(), "/Server/UpdateUserPublisherReadOnlyData", true, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void PlayerDataManagementTests::TestClientGetUserReadOnlyData(TestContext& tc)
{
    ServerUpdateUserReadonlyData(ServiceConfig(), m_testTitleData.secretKey, DefaultTitlePlayerId(), "/Server/UpdateUserReadOnlyData", false, RunContext()).Then([&](Result<void> result) -> AsyncOp<ClientGetUserReadOnlyDataOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        ClientGetUserReadOnlyDataOperation::RequestType request;
        request.SetKeys({ kTestKey });

        return ClientGetUserReadOnlyDataOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<ClientGetUserReadOnlyDataOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual(1u, result.Payload().Model().dataCount, "dataCount");
        tc.AssertEqual<String>(kTestKey, result.Payload().Model().data[0].key, "dataKey");
        tc.AssertEqual<String>(kTestVal, result.Payload().Model().data[0].value->value, "dataValue");

        return S_OK;
    })
    .Then([&](Result<void> result) -> AsyncOp<void>
    {
        tc.RecordResult(std::move(result));

        // Cleanup: remove keys
        return ServerUpdateUserReadonlyData(ServiceConfig(), m_testTitleData.secretKey, DefaultTitlePlayerId(), "/Server/UpdateUserReadOnlyData", true, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void PlayerDataManagementTests::TestClientUpdateUserData(TestContext& tc)
{
    ClientUpdateUserDataOperation::RequestType request{};
    StringDictionaryEntryVector data{};
    data.insert_or_assign(kTestKey, kTestVal);
    request.SetData(data);

    ClientUpdateUserDataOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<ClientUpdateUserDataOperation::ResultType> result) -> AsyncOp<ClientGetUserDataOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        ClientGetUserDataOperation::RequestType request;
        request.SetKeys({ kTestKey });

        return ClientGetUserDataOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<ClientGetUserDataOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual(1u, result.Payload().Model().dataCount, "dataCount");
        tc.AssertEqual<String>(kTestKey, result.Payload().Model().data[0].key, "dataKey");
        tc.AssertEqual<String>(kTestVal, result.Payload().Model().data[0].value->value, "dataValue");
        
        return S_OK;
    })
    .Then([&](Result<void> result) ->AsyncOp<ClientUpdateUserDataOperation::ResultType>
    {
        tc.RecordResult(std::move(result));

        // Cleanup: remove keys
        ClientUpdateUserDataOperation::RequestType request;
        request.SetKeysToRemove({ kTestKey });

        return ClientUpdateUserDataOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Finally([&](Result<ClientUpdateUserDataOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
}

void PlayerDataManagementTests::TestClientUpdateUserPublisherData(TestContext& tc)
{
    ClientUpdateUserPublisherDataOperation::RequestType request{};
    StringDictionaryEntryVector data{};
    data.insert_or_assign(kTestKey, kTestVal);
    request.SetData(data);

    ClientUpdateUserPublisherDataOperation::Run(DefaultTitlePlayer(), request, RunContext()).Then([&](Result<ClientUpdateUserPublisherDataOperation::ResultType> result) -> AsyncOp<ClientGetUserPublisherDataOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        ClientGetUserPublisherDataOperation::RequestType request;
        request.SetKeys({ kTestKey });

        return ClientGetUserPublisherDataOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&](Result<ClientGetUserPublisherDataOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual(1u, result.Payload().Model().dataCount, "dataCount");
        tc.AssertEqual<String>(kTestKey, result.Payload().Model().data[0].key, "dataKey");
        tc.AssertEqual<String>(kTestVal, result.Payload().Model().data[0].value->value, "dataValue");

        return S_OK;
    })
    .Then([&](Result<void> result)->AsyncOp<ClientUpdateUserPublisherDataOperation::ResultType>
    {
        tc.RecordResult(std::move(result));

        // Cleanup: remove keys
        ClientUpdateUserPublisherDataOperation::RequestType request;
        request.SetKeysToRemove({ kTestKey });

        return ClientUpdateUserPublisherDataOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Finally([&](Result<ClientUpdateUserPublisherDataOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
}

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void PlayerDataManagementTests::TestServerGetUserData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void PlayerDataManagementTests::TestServerGetUserInternalData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void PlayerDataManagementTests::TestServerGetUserPublisherData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void PlayerDataManagementTests::TestServerGetUserPublisherInternalData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void PlayerDataManagementTests::TestServerGetUserPublisherReadOnlyData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void PlayerDataManagementTests::TestServerGetUserReadOnlyData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void PlayerDataManagementTests::TestServerUpdateUserData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void PlayerDataManagementTests::TestServerUpdateUserInternalData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void PlayerDataManagementTests::TestServerUpdateUserPublisherData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void PlayerDataManagementTests::TestServerUpdateUserPublisherInternalData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void PlayerDataManagementTests::TestServerUpdateUserPublisherReadOnlyData(TestContext& tc)
{
    tc.Skip();
}
#endif

#if HC_PLATFORM != HC_PLATFORM_WIN32 && HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH && HC_PLATFORM != HC_PLATFORM_GDK
void PlayerDataManagementTests::TestServerUpdateUserReadOnlyData(TestContext& tc)
{
    tc.Skip();
}
#endif


}
}

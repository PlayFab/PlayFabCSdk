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
    JsonValue requestBody= JsonValue::object();;
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

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void PlayerDataManagementTests::TestServerGetUserData(TestContext& tc)
{
    // Already covered in TestServerUpdateUserData
    tc.EndTest(S_OK);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void PlayerDataManagementTests::TestServerGetUserInternalData(TestContext& tc)
{
    // Already covered in TestServerUpdateUserInternalData
    tc.EndTest(S_OK);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void PlayerDataManagementTests::TestServerGetUserPublisherData(TestContext& tc)
{
    // Already covered in TestServerUpdateUserPublisherData
    tc.EndTest(S_OK);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void PlayerDataManagementTests::TestServerGetUserPublisherInternalData(TestContext& tc)
{
    // Already covered in TestServerUpdateUserPublisherInternalData
    tc.EndTest(S_OK);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void PlayerDataManagementTests::TestServerGetUserPublisherReadOnlyData(TestContext& tc)
{
    // Already covered in TestServerUpdateUserPublisherReadOnlyData
    tc.EndTest(S_OK);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void PlayerDataManagementTests::TestServerGetUserReadOnlyData(TestContext& tc)
{
    // Already covered in TestServerUpdateUserReadOnlyData
    tc.EndTest(S_OK);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void PlayerDataManagementTests::TestServerUpdateUserData(TestContext& tc)
{
    ServerUpdateUserDataOperation::RequestType request;
    StringDictionaryEntryVector data{};
    data.insert_or_assign(kTestKey, kTestVal);
    request.SetData(data);
    request.SetPlayFabId(DefaultTitlePlayerId());

    ServerUpdateUserDataOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<ServerUpdateUserDataOperation::ResultType> result) -> AsyncOp<ServerGetUserDataOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        ServerGetUserDataOperation::RequestType request;
        request.SetKeys({ kTestKey });
        request.SetPlayFabId(DefaultTitlePlayerId());

        return ServerGetUserDataOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&](Result<ServerGetUserDataOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual(1u, result.Payload().Model().dataCount, "dataCount");
        tc.AssertEqual(kTestKey, result.Payload().Model().data[0].key, "dataKey");
        tc.AssertEqual(kTestVal, result.Payload().Model().data[0].value->value, "dataValue");

        return S_OK;
    })
    .Then([&](Result<void> result) -> AsyncOp<ServerUpdateUserDataOperation::ResultType>
    {
        tc.RecordResult(std::move(result));

        // Cleanup
        ServerUpdateUserDataOperation::RequestType request;
        request.SetKeysToRemove({ kTestKey });
        request.SetPlayFabId(DefaultTitlePlayerId());

        return ServerUpdateUserDataOperation::Run(TitleEntity(), request, RunContext());
    })
    .Finally([&](Result<ServerUpdateUserDataOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void PlayerDataManagementTests::TestServerUpdateUserInternalData(TestContext& tc)
{
    ServerUpdateUserInternalDataOperation::RequestType request;
    StringDictionaryEntryVector data{};
    data.insert_or_assign(kTestKey, kTestVal);
    request.SetData(data);
    request.SetPlayFabId(DefaultTitlePlayerId());

    ServerUpdateUserInternalDataOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<ServerUpdateUserInternalDataOperation::ResultType> result) -> AsyncOp<ServerGetUserInternalDataOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        ServerGetUserInternalDataOperation::RequestType request;
        request.SetKeys({ kTestKey });
        request.SetPlayFabId(DefaultTitlePlayerId());

        return ServerGetUserInternalDataOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&](Result<ServerGetUserInternalDataOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual(1u, result.Payload().Model().dataCount, "dataCount");
        tc.AssertEqual(kTestKey, result.Payload().Model().data[0].key, "dataKey");
        tc.AssertEqual(kTestVal, result.Payload().Model().data[0].value->value, "dataValue");

        return S_OK;
    })
    .Then([&](Result<void> result) -> AsyncOp<ServerUpdateUserInternalDataOperation::ResultType>
    {
        tc.RecordResult(std::move(result));

        // Cleanup
        ServerUpdateUserInternalDataOperation::RequestType request;
        request.SetKeysToRemove({ kTestKey });
        request.SetPlayFabId(DefaultTitlePlayerId());

        return ServerUpdateUserInternalDataOperation::Run(TitleEntity(), request, RunContext());
    })
    .Finally([&](Result<ServerUpdateUserInternalDataOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void PlayerDataManagementTests::TestServerUpdateUserPublisherData(TestContext& tc)
{
    ServerUpdateUserPublisherDataOperation::RequestType request;
    StringDictionaryEntryVector data{};
    data.insert_or_assign(kTestKey, kTestVal);
    request.SetData(data);
    request.SetPlayFabId(DefaultTitlePlayerId());

    ServerUpdateUserPublisherDataOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<ServerUpdateUserPublisherDataOperation::ResultType> result) -> AsyncOp<ServerGetUserPublisherDataOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        ServerGetUserPublisherDataOperation::RequestType request;
        request.SetKeys({ kTestKey });
        request.SetPlayFabId(DefaultTitlePlayerId());

        return ServerGetUserPublisherDataOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&](Result<ServerGetUserPublisherDataOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual(1u, result.Payload().Model().dataCount, "dataCount");
        tc.AssertEqual(kTestKey, result.Payload().Model().data[0].key, "dataKey");
        tc.AssertEqual(kTestVal, result.Payload().Model().data[0].value->value, "dataValue");

        return S_OK;
    })
    .Then([&](Result<void> result) -> AsyncOp<ServerUpdateUserPublisherDataOperation::ResultType>
    {
        tc.RecordResult(std::move(result));

        // Cleanup
        ServerUpdateUserPublisherDataOperation::RequestType request;
        request.SetKeysToRemove({ kTestKey });
        request.SetPlayFabId(DefaultTitlePlayerId());

        return ServerUpdateUserPublisherDataOperation::Run(TitleEntity(), request, RunContext());
    })
    .Finally([&](Result<ServerUpdateUserPublisherDataOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void PlayerDataManagementTests::TestServerUpdateUserPublisherInternalData(TestContext& tc)
{
    ServerUpdateUserPublisherInternalDataOperation::RequestType request;
    StringDictionaryEntryVector data{};
    data.insert_or_assign(kTestKey, kTestVal);
    request.SetData(data);
    request.SetPlayFabId(DefaultTitlePlayerId());

    ServerUpdateUserPublisherInternalDataOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<ServerUpdateUserPublisherInternalDataOperation::ResultType> result) -> AsyncOp<ServerGetUserPublisherInternalDataOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        ServerGetUserPublisherInternalDataOperation::RequestType request;
        request.SetKeys({ kTestKey });
        request.SetPlayFabId(DefaultTitlePlayerId());

        return ServerGetUserPublisherInternalDataOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&](Result<ServerGetUserPublisherInternalDataOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual(1u, result.Payload().Model().dataCount, "dataCount");
        tc.AssertEqual(kTestKey, result.Payload().Model().data[0].key, "dataKey");
        tc.AssertEqual(kTestVal, result.Payload().Model().data[0].value->value, "dataValue");

        return S_OK;
    })
    .Then([&](Result<void> result) -> AsyncOp<ServerUpdateUserPublisherInternalDataOperation::ResultType>
    {
        tc.RecordResult(std::move(result));

        // Cleanup
        ServerUpdateUserPublisherInternalDataOperation::RequestType request;
        request.SetKeysToRemove({ kTestKey });
        request.SetPlayFabId(DefaultTitlePlayerId());

        return ServerUpdateUserPublisherInternalDataOperation::Run(TitleEntity(), request, RunContext());
    })
    .Finally([&](Result<ServerUpdateUserPublisherInternalDataOperation::ResultType> result)
    {
        tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void PlayerDataManagementTests::TestServerUpdateUserPublisherReadOnlyData(TestContext& tc)
{
    ServerUpdateUserPublisherReadOnlyDataOperation::RequestType request;
    StringDictionaryEntryVector data{};
    data.insert_or_assign(kTestKey, kTestVal);
    request.SetData(data);
    request.SetPlayFabId(DefaultTitlePlayerId());

    ServerUpdateUserPublisherReadOnlyDataOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<ServerUpdateUserPublisherReadOnlyDataOperation::ResultType> result) -> AsyncOp<ServerGetUserPublisherReadOnlyDataOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        ServerGetUserPublisherReadOnlyDataOperation::RequestType request;
        request.SetKeys({ kTestKey });
        request.SetPlayFabId(DefaultTitlePlayerId());

        return ServerGetUserPublisherReadOnlyDataOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&](Result<ServerGetUserPublisherReadOnlyDataOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual(1u, result.Payload().Model().dataCount, "dataCount");
        tc.AssertEqual(kTestKey, result.Payload().Model().data[0].key, "dataKey");
        tc.AssertEqual(kTestVal, result.Payload().Model().data[0].value->value, "dataKey");

        return S_OK;
    })
    .Then([&](Result<void> result) -> AsyncOp<ServerUpdateUserPublisherReadOnlyDataOperation::ResultType>
    {
        tc.RecordResult(std::move(result));

        ServerUpdateUserPublisherReadOnlyDataOperation::RequestType request;
        request.SetKeysToRemove({ kTestKey });
        request.SetPlayFabId(DefaultTitlePlayerId());

        return ServerUpdateUserPublisherReadOnlyDataOperation::Run(TitleEntity(), request, RunContext());
    })
    .Finally([&](Result<ServerUpdateUserPublisherReadOnlyDataOperation::ResultType> result)
    {
        return tc.EndTest(std::move(result));
    });
}
#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
void PlayerDataManagementTests::TestServerUpdateUserReadOnlyData(TestContext& tc)
{
    ServerUpdateUserReadOnlyDataOperation::RequestType request;
    StringDictionaryEntryVector data{};
    data.insert_or_assign(kTestKey, kTestVal);
    request.SetData(data);
    request.SetPlayFabId(DefaultTitlePlayerId());

    ServerUpdateUserReadOnlyDataOperation::Run(TitleEntity(), request, RunContext()).Then([&](Result<ServerUpdateUserReadOnlyDataOperation::ResultType> result) -> AsyncOp<ServerGetUserReadOnlyDataOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        ServerGetUserReadOnlyDataOperation::RequestType request;
        request.SetKeys({ kTestKey });
        request.SetPlayFabId(DefaultTitlePlayerId());

        return ServerGetUserReadOnlyDataOperation::Run(TitleEntity(), request, RunContext());
    })
    .Then([&](Result<ServerGetUserReadOnlyDataOperation::ResultType> result) -> AsyncOp<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertEqual(1u, result.Payload().Model().dataCount, "dataCount");
        tc.AssertEqual(kTestKey, result.Payload().Model().data[0].key, "dataKey");
        tc.AssertEqual(kTestVal, result.Payload().Model().data[0].value->value, "dataKey");

        return S_OK;
    })
    .Then([&](Result<void> result) -> AsyncOp<ServerUpdateUserReadOnlyDataOperation::ResultType>
    {
        tc.RecordResult(std::move(result));

        ServerUpdateUserReadOnlyDataOperation::RequestType request;
        request.SetKeysToRemove({ kTestKey });
        request.SetPlayFabId(DefaultTitlePlayerId());

        return ServerUpdateUserReadOnlyDataOperation::Run(TitleEntity(), request, RunContext());
    })
    .Finally([&](Result<ServerUpdateUserReadOnlyDataOperation::ResultType> result)
    {
        return tc.EndTest(std::move(result));
    });
}
#endif


}
}

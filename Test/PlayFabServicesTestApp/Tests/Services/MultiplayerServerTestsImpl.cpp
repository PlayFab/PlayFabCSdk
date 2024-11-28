#include "TestAppPch.h"
#include "MultiplayerServerTests.h"
#include "MultiplayerServerOperations.h"
#include "SdkVersion.h"
#include "HttpRequest.h"
#include "JsonUtils.h"

namespace PlayFab
{
namespace Test
{

constexpr char kTestBuildAlias[]{ "testBuildAlias" };
constexpr char kTestSessionId[]{ "239179e3-09dd-4a87-9263-e43abd546189" };

class GetEntityTokenOperation : public XAsyncOperation<String>
{
public:
    GetEntityTokenOperation(Entity const& entity, PlayFab::RunContext runContext) noexcept :
        XAsyncOperation<String>{ std::move(runContext) },
        m_entity{ entity }
    {
    }

    static AsyncOp<String> Run(Entity entity, PlayFab::RunContext rc) noexcept
    {
        return RunOperation(MakeUnique<GetEntityTokenOperation>(std::move(entity), std::move(rc)));
    }

private:
    // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override
    {
        return PFEntityGetEntityTokenAsync(m_entity.Handle(), async);
    }

    Result<String> GetResult(XAsyncBlock* async) noexcept override
    {
        size_t bufferSize;
        Vector<char> buffer;
        PFEntityToken const* entityToken;

        RETURN_IF_FAILED(PFEntityGetEntityTokenResultSize(async, &bufferSize));
        buffer.resize(bufferSize);
        RETURN_IF_FAILED(PFEntityGetEntityTokenResult(async, buffer.size(), buffer.data(), &entityToken, nullptr));
        RETURN_HR_IF(E_UNEXPECTED, !entityToken->token);

        return String{ entityToken->token };
    }

    Entity const m_entity;
};

#if HC_PLATFORM != HC_PLATFORM_GDK
AsyncOp<String> CreateBuildAlias(ServiceConfig serviceConfig, Entity title, RunContext rc, TestContext& tc)
{
    JsonValue requestBody= JsonValue::object();;
    JsonUtils::ObjectAddMember(requestBody, "AliasName", kTestBuildAlias);
    const char* path{ "/MultiplayerServer/CreateBuildAlias" };

    Stringstream headerVal;
    headerVal << versionString << sdkVersion;
    UnorderedMap<String, String> headers{ { kSDKVersionStringHeaderName, headerVal.str() }};

    Stringstream fullUrl;
    fullUrl << serviceConfig.APIEndpoint();

    // Append path
    fullUrl << path;

    // Add sdk version param (used by service for telemetry)
    fullUrl << "?sdk=" << versionString << sdkVersion;

    return GetEntityTokenOperation::Run(title, rc).Then(
        [
            &,
            url = fullUrl.str(),
            body = JsonUtils::WriteToString(requestBody),
            rc
        ]
    (Result<String> result) mutable -> AsyncOp<ServiceResponse>
    {
        RETURN_IF_FAILED(result.hr);

        Stringstream headerVal;
        headerVal << versionString << sdkVersion;
        UnorderedMap<String, String> headers{ { kSDKVersionStringHeaderName, headerVal.str() } };
        headers[kEntityTokenHeaderName] = result.ExtractPayload();

        return RunOperation(MakeUnique<HCHttpCall>(kPostMethod, url, std::move(headers), body, std::move(rc)));
    })
    .Then([&](Result<ServiceResponse> result) -> Result<String>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            tc.AssertTrue(serviceResponse.Data.contains("AliasName"), "AliasName");
            tc.AssertEqual(kTestBuildAlias, serviceResponse.Data["AliasName"].get<String>().c_str(), "AliasName");
            tc.AssertTrue(serviceResponse.Data.contains("AliasId"), "AliasId");

            return Result<String>(serviceResponse.Data["AliasId"].get<String>());
        }
        else
        {
            return Result<String>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> DeleteBuildAlias(ServiceConfig serviceConfig, Entity title, String aliasId, RunContext rc)
{
    JsonValue requestBody= JsonValue::object();;
    JsonUtils::ObjectAddMember(requestBody, "AliasId", aliasId);
    const char* path{ "/MultiplayerServer/DeleteBuildAlias" };

    Stringstream headerVal;
    headerVal << versionString << sdkVersion;
    UnorderedMap<String, String> headers{ { kSDKVersionStringHeaderName, headerVal.str() } };

    Stringstream fullUrl;
    fullUrl << serviceConfig.APIEndpoint();

    // Append path
    fullUrl << path;

    // Add sdk version param (used by service for telemetry)
    fullUrl << "?sdk=" << versionString << sdkVersion;

    return GetEntityTokenOperation::Run(title, rc).Then(
        [
            url = fullUrl.str(),
            body = JsonUtils::WriteToString(requestBody),
            rc
        ]
    (Result<String> result) mutable -> AsyncOp<ServiceResponse>
    {
        RETURN_IF_FAILED(result.hr);

        Stringstream headerVal;
        headerVal << versionString << sdkVersion;
        UnorderedMap<String, String> headers{ { kSDKVersionStringHeaderName, headerVal.str() } };
        headers[kEntityTokenHeaderName] = result.ExtractPayload();

        return RunOperation(MakeUnique<HCHttpCall>(kPostMethod, url, std::move(headers), body, std::move(rc)));
    })
    .Then([&](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return S_OK;
        }
        else
        {
            return Result<void>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}

AsyncOp<void> ShutdownServerSession(ServiceConfig serviceConfig, Entity titlePlayer, RunContext rc)
{
    JsonValue requestBody= JsonValue::object();;
    JsonUtils::ObjectAddMember(requestBody, "SessionId", kTestSessionId);
    const char* path{ "/MultiplayerServer/ShutdownMultiplayerServer" };

    Stringstream headerVal;
    headerVal << versionString << sdkVersion;
    UnorderedMap<String, String> headers{ { kSDKVersionStringHeaderName, headerVal.str() } };

    Stringstream fullUrl;
    fullUrl << serviceConfig.APIEndpoint();

    // Append path
    fullUrl << path;

    // Add sdk version param (used by service for telemetry)
    fullUrl << "?sdk=" << versionString << sdkVersion;

    return GetEntityTokenOperation::Run(titlePlayer, rc).Then(
        [
            url = fullUrl.str(),
            body = JsonUtils::WriteToString(requestBody),
            rc
        ]
    (Result<String> result) mutable -> AsyncOp<ServiceResponse>
    {
        RETURN_IF_FAILED(result.hr);

        Stringstream headerVal;
        headerVal << versionString << sdkVersion;
        UnorderedMap<String, String> headers{ { kSDKVersionStringHeaderName, headerVal.str() } };
        headers[kEntityTokenHeaderName] = result.ExtractPayload();

        return RunOperation(MakeUnique<HCHttpCall>(kPostMethod, url, std::move(headers), body, std::move(rc)));
    })
    .Then([&](Result<ServiceResponse> result) -> Result<void>
    {
        RETURN_IF_FAILED(result.hr);

        auto serviceResponse = result.ExtractPayload();
        if (serviceResponse.HttpCode >= 200 && serviceResponse.HttpCode < 300)
        {
            return S_OK;
        }
        else
        {
            return Result<void>{ ServiceErrorToHR(serviceResponse.ErrorCode), std::move(serviceResponse.ErrorMessage) };
        }
    });
}
#endif

AsyncOp<void> MultiplayerServerTests::Initialize()
{
    return ServicesTestClass::Initialize();
}

AsyncOp<void> MultiplayerServerTests::Uninitialize()
{
    return ServicesTestClass::Uninitialize();
}

void MultiplayerServerTests::TestListBuildAliases(TestContext& tc)
{
#if HC_PLATFORM == HC_PLATFORM_WIN32
    auto aliasId = MakeShared<String>();

    CreateBuildAlias(ServiceConfig(), TitleEntity(), RunContext(), tc)
    .Then([&, aliasId](Result<String> result) -> AsyncOp<ListBuildAliasesOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        *aliasId = result.ExtractPayload();

        ListBuildAliasesOperation::RequestType request{};
        return ListBuildAliasesOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&, aliasId](Result<ListBuildAliasesOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        //auto& model = result.Payload().Model();
        tc.AssertEqual(1u, result.Payload().Model().buildAliasesCount, "buildAliasesCount");
        tc.AssertEqual(kTestBuildAlias, result.Payload().Model().buildAliases[0]->aliasName, "aliasName");
        tc.AssertEqual<String>(*aliasId, result.Payload().Model().buildAliases[0]->aliasId, "aliasId");

        return S_OK;
    })
    .Then([&, aliasId](Result<void> result) -> AsyncOp<void>
    {
        tc.RecordResult(std::move(result));

        // Cleanup
        return DeleteBuildAlias(ServiceConfig(), TitleEntity(), *aliasId, RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
#else
    // Test code uses TitleEntity so skipping except on Win32
    tc.Skip();
#endif
}

void MultiplayerServerTests::TestListBuildSummariesV2(TestContext& tc)
{
    // Already covered in TestRequestMultiplayerServer
    tc.EndTest(S_OK);
}

void MultiplayerServerTests::TestListQosServersForTitle(TestContext& tc)
{
    ListQosServersForTitleOperation::Run(DefaultTitlePlayer(), ListQosServersForTitleOperation::RequestType{}, RunContext())
    .Then([&](Result<ListQosServersForTitleOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        tc.AssertTrue(1u <= result.Payload().Model().qosServersCount, "qosServersCount");
        tc.AssertTrue(result.Payload().Model().qosServers[0]->serverUrl, "serverUrl");

        return S_OK;
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
}

void MultiplayerServerTests::TestRequestMultiplayerServer(TestContext& tc)
{
#if HC_PLATFORM == HC_PLATFORM_WIN32
    // Requires manual server set up in Game Manager portal
    constexpr char kTestRegion[]{ "EastUs" };

    auto buildId = MakeShared<String>();

    ListBuildSummariesV2Operation::Run(DefaultTitlePlayer(), ListBuildSummariesV2Operation::RequestType{}, RunContext())
    .Then([&, buildId, kTestRegion](Result<ListBuildSummariesV2Operation::ResultType> result) -> AsyncOp<RequestMultiplayerServerOperation::ResultType>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual(1u, model.buildSummariesCount, "buildSummariesCount");
        tc.AssertTrue(model.buildSummaries[0]->buildId, "buildId");

        *buildId = model.buildSummaries[0]->buildId;

        RequestMultiplayerServerOperation::RequestType request{};
        request.SetBuildId(*buildId);
        request.SetPreferredRegions({ kTestRegion });
        request.SetSessionId(kTestSessionId);

        return RequestMultiplayerServerOperation::Run(DefaultTitlePlayer(), request, RunContext());
    })
    .Then([&, buildId, kTestRegion](Result<RequestMultiplayerServerOperation::ResultType> result) -> Result<void>
    {
        RETURN_IF_FAILED_PLAYFAB(result);

        auto& model = result.Payload().Model();
        tc.AssertEqual<String>(*buildId, model.buildId, "buildId");
        tc.AssertEqual(0u, model.connectedPlayersCount, "connectedPlayersCount");
        tc.AssertEqual(kTestRegion, model.region, "region");
        tc.AssertEqual(kTestSessionId, model.sessionId, "sessionId");

        return S_OK;
    })
    .Then([&, buildId](Result<void> result) -> AsyncOp<void>
    {
        tc.RecordResult(std::move(result));

        // Cleanup
        return ShutdownServerSession(ServiceConfig(), TitleEntity(), RunContext());
    })
    .Finally([&](Result<void> result)
    {
        tc.EndTest(std::move(result));
    });
#else
    // Test code uses TitleEntity so skipping except on Win32
    tc.Skip();
#endif
}


}
}

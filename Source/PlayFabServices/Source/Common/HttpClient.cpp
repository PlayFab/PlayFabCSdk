#include "stdafx.h"
#include "HttpClient.h"
#include "HttpRequest.h"
#include "JsonUtils.h"
#include "SdkVersion.h"
#include "XAsyncOperation.h"

namespace PlayFab
{

class GetEntityTokenOperation : public XAsyncOperation<String>
{
public:
    GetEntityTokenOperation(Entity const& entity, PlayFab::RunContext&& runContext) noexcept;
    ~GetEntityTokenOperation() = default;

private: 
    // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<String> GetResult(XAsyncBlock* async) noexcept override;

    Entity const m_entity;
};

ServicesHttpClient::ServicesHttpClient()
{
}

String ServicesHttpClient::GetUrl(String const& connectionString, const char* path)
{
    Stringstream fullUrl;
    fullUrl << connectionString;

    // Append path
    fullUrl << path;

    // Add sdk version param (used by service for telemetry)
    fullUrl << "?sdk=" << versionString << sdkVersion;

    fullUrl << "&platform=";

    // Add Platform param (used by service for telemetry)
#if HC_PLATFORM == HC_PLATFORM_WIN32
    fullUrl << win32Plat;
#elif HC_PLATFORM == HC_PLATFORM_GDK
    fullUrl << gdkPlat;
#elif HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4
    fullUrl << ps4Plat;
#elif HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
    fullUrl << ps5Plat;
#elif HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH
    fullUrl << nintendoPlat;
#elif HC_PLATFORM ==  HC_PLATFORM_LINUX
    fullUrl << linuxPlat;
#elif HC_PLATFORM ==  HC_PLATFORM_IOS
    fullUrl << iosPlat;
#elif HC_PLATFORM ==  HC_PLATFORM_MAC
    fullUrl << macosPlat;
#elif HC_PLATFORM ==  HC_PLATFORM_ANDROID
    fullUrl << androidPlat;
#endif

    // Add Build Origin param (used by service for telemetry)
    fullUrl << "&origin=" << buildOrigin;

    return fullUrl.str();
}

UnorderedMap<String, String> ServicesHttpClient::GetDefaultHeaders()
{
    Stringstream headerVal;
    headerVal << versionString << sdkVersion;
    return UnorderedMap<String, String>{ { kSDKVersionStringHeaderName, headerVal.str() } };
}

AsyncOp<ServiceResponse> ServicesHttpClient::MakePostRequest(
    ServicesCacheId cacheId,
    String&& endpoint,
    const char* path,
    JsonValue const& requestBody,
    RunContext&& runContext
)
{
    PFHttpRetrySettings retrySettings{};
    RETURN_IF_FAILED(PFGetHttpRetrySettings(&retrySettings));

    auto requestOp = MakeUnique<HCHttpCall>(
        kPostMethod,
        GetUrl(endpoint, path),
        GetDefaultHeaders(),
        JsonUtils::WriteToString(requestBody),
        static_cast<uint32_t>(cacheId),
        retrySettings,
        std::move(runContext)
    );

    return RunOperation(std::move(requestOp));
}

AsyncOp<ServiceResponse> ServicesHttpClient::MakeEntityRequest(
    ServicesCacheId cacheId,
    Entity const& entity,
    const char* path,
    const JsonValue& requestBody,
    RunContext&& runContext
)
{
    return RunOperation(MakeUnique<GetEntityTokenOperation>(entity, runContext.Derive())).Then(
        [
            cacheId,
            url = GetUrl(entity.APIEndpoint(), path),
            body = JsonUtils::WriteToString(requestBody),
            runContextDerived{ runContext.Derive() }
        ]
    (Result<String> result) mutable -> AsyncOp<ServiceResponse>
    {
        RETURN_IF_FAILED(result.hr);

        UnorderedMap<String, String> headers = GetDefaultHeaders();
        headers[kEntityTokenHeaderName] = result.ExtractPayload();

        PFHttpRetrySettings retrySettings{};
        RETURN_IF_FAILED(PFGetHttpRetrySettings(&retrySettings));

        auto requestOp = MakeUnique<HCHttpCall>(
            kPostMethod,
            url,
            std::move(headers),
            std::move(body),
            static_cast<uint32_t>(cacheId),
            retrySettings,
            std::move(runContextDerived)
        );

        return RunOperation(std::move(requestOp));
    });
}


AsyncOp<ServiceResponse> ServicesHttpClient::MakeSecretKeyRequest(
    ServicesCacheId cacheId,
    Entity const& entity,
    const char* path,
    JsonValue const& requestBody,
    RunContext&& runContext
)
{
    String secretKey;
    RETURN_IF_FAILED(entity.TryGetSecretKey(secretKey));

    UnorderedMap<String, String> headers = GetDefaultHeaders();
    headers[kSecretKeyHeaderName] = std::move(secretKey);

    PFHttpRetrySettings retrySettings{};
    RETURN_IF_FAILED(PFGetHttpRetrySettings(&retrySettings));

    auto requestOp = MakeUnique<HCHttpCall>(
        kPostMethod,
        GetUrl(entity.APIEndpoint(), path),
        std::move(headers),
        JsonUtils::WriteToString(requestBody),
        static_cast<uint32_t>(cacheId),
        retrySettings,
        std::move(runContext)
    );

    return RunOperation(std::move(requestOp));
}

GetEntityTokenOperation::GetEntityTokenOperation(Entity const& entity, PlayFab::RunContext&& runContext) noexcept :
    XAsyncOperation<String>{ std::move(runContext) },
    m_entity{ entity }
{
}

HRESULT GetEntityTokenOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFEntityGetEntityTokenAsync(m_entity.Handle(), async);
}

Result<String> GetEntityTokenOperation::GetResult(XAsyncBlock* async) noexcept
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

} // namespace PlayFab

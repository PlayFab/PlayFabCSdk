#include "stdafx.h"
#include "HttpClient.h"
#include "Entity.h"
#include "SdkVersion.h"
#include "JsonUtils.h"

namespace PlayFab
{

HttpClient::HttpClient(String&& apiEndpoint, SharedPtr<PFHttpRetrySettings> retrySettings) :
    m_apiEndpoint{ std::move(apiEndpoint) },
    m_retrySettings{ std::move(retrySettings) }
{
}

String HttpClient::GetUrl(const char* path) const
{
    Stringstream fullUrl;
    fullUrl << m_apiEndpoint;

    // Append path
    fullUrl << path;

    // Add sdk version param (used by service for telemetry)
    fullUrl << "?sdk=" << versionString << sdkVersion;

    return fullUrl.str();
}

AsyncOp<ServiceResponse> HttpClient::MakePostRequest(
    const char* path,
    UnorderedMap<String, String> headers,
    const JsonValue& requestBody,
    CacheId retryCacheId,
    RunContext&& runContext
) const
{
    auto httpCallOp = MakeUnique<HCHttpCall>(
        kPostMethod,
        GetUrl(path),
        std::move(headers),
        JsonUtils::WriteToString(requestBody),
        static_cast<uint32_t>(retryCacheId),
        *m_retrySettings,
        std::move(runContext)
    );

    return RunOperation(std::move(httpCallOp));
}

AsyncOp<ServiceResponse> HttpClient::MakeEntityRequest(
    SharedPtr<Entity const> entity,
    const char* path,
    const JsonValue& requestBody,
    CacheId retryCacheId,
    RunContext&& runContext
) const
{
    auto entityTokenResult = entity->GetEntityToken();
    RETURN_IF_FAILED(entityTokenResult.hr);

    UnorderedMap<String, String> headers{};
    headers[kEntityTokenHeaderName] = entityTokenResult.Payload().token;

    return MakePostRequest(path, std::move(headers), requestBody, retryCacheId, std::move(runContext));
}

AsyncOp<ServiceResponse> HttpClient::MakeSecretKeyRequest(
    SharedPtr<ServiceConfig const> serviceConfig,
    String const& secretKey,
    const char* path,
    const JsonValue& requestBody,
    CacheId retryCacheId,
    RunContext&& runContext
) const
{
    UnorderedMap<String, String> headers{};
    headers[kSecretKeyHeaderName] = secretKey;

    return MakePostRequest(path, std::move(headers), requestBody, retryCacheId, std::move(runContext));
}

AsyncOp<ServiceResponse> HttpClient::MakeSecretKeyRequest(
    SharedPtr<Entity const> entity,
    const char* path,
    const JsonValue& requestBody,
    CacheId retryCacheID,
    RunContext&& runContext
) const
{
    auto secretKeyResult = entity->GetSecretKey();
    RETURN_IF_FAILED(secretKeyResult.hr);

    return MakeSecretKeyRequest(entity->ServiceConfig(), secretKeyResult.Payload(), path, requestBody, retryCacheID, std::move(runContext));
}

AsyncOp<ServiceResponse> HttpClient::MakeTelemetryKeyRequest(
    String const& telemetryKey,
    const char* path,
    const JsonValue& requestBody,
    CacheId retryCacheId,
    RunContext&& runContext
) const
{
    UnorderedMap<String, String> headers{};
    headers[kTelemetryKeyHeaderName] = telemetryKey;

    return MakePostRequest(path, std::move(headers), requestBody, retryCacheId, std::move(runContext));
}

String const& HttpClient::APIEndpoint() const noexcept
{
    return m_apiEndpoint;
}

}

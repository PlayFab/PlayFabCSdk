#pragma once

#include <playfab/core/PFServiceConfig.h>
#include <playfab/core/PFHttpConfig.h>
#include <HttpRequest.h>
#include "Generated/CacheId.h"

namespace PlayFab
{
class Entity;
class ServiceConfig;

// An Http client for make PlayFab service requests
class HttpClient
{
public:
    HttpClient(String&& apiEndpoint, SharedPtr<PFHttpRetrySettings> retrySettings, SharedPtr<PFHttpSettings> httpSettings);
    HttpClient(const HttpClient&) = default;
    ~HttpClient() = default;

    AsyncOp<ServiceResponse> MakePostRequest(
        const char* path,
        UnorderedMap<String, String> headers,
        const JsonValue& requestBody,
        CacheId retryCacheId,
        RunContext&& runContext,
        HCCompressionLevel compressionLevel = HCCompressionLevel::None
    ) const;

    AsyncOp<ServiceResponse> MakeEntityRequest(
        SharedPtr<Entity const> entity,
        const char* path,
        const JsonValue& requestBody,
        CacheId retryCacheId,
        RunContext&& runContext,
        HCCompressionLevel compressionLevel = HCCompressionLevel::None
    ) const;

    AsyncOp<ServiceResponse> MakeSecretKeyRequest(
        SharedPtr<ServiceConfig const> serviceConfig,
        String const& secretKey,
        const char* path,
        const JsonValue& requestBody,
        CacheId retryCacheId,
        RunContext&& runContext,
        HCCompressionLevel compressionLevel = HCCompressionLevel::None
    ) const;

    AsyncOp<ServiceResponse> MakeSecretKeyRequest(
        SharedPtr<Entity const> entity,
        const char* path,
        const JsonValue& requestBody,
        CacheId retryCacheID,
        RunContext&& runContext,
        HCCompressionLevel compressionLevel = HCCompressionLevel::None
    ) const;

    AsyncOp<ServiceResponse> MakeTelemetryKeyRequest(
        String const& telemetryKey,
        const char* path,
        const JsonValue& requestBody,
        CacheId retryCacheId,
        RunContext&& runContext,
        HCCompressionLevel compressionLevel = HCCompressionLevel::None
    ) const;

    String const& APIEndpoint() const noexcept;

private:
    String GetUrl(const char* path) const;

    String const m_apiEndpoint;
    SharedPtr<PFHttpRetrySettings> const m_retrySettings;
    SharedPtr<PFHttpSettings> const m_httpSettings;
};

}

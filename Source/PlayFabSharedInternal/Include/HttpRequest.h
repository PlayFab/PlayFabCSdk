#pragma once

#include <playfab/httpClient/PFHttpClient.h>
#include <playfab/core/PFHttpConfig.h>
#include "XAsyncOperation.h"
#include "RunContext.h"

namespace PlayFab
{

// PlayFab authorization header names
constexpr char kSessionTicketHeaderName[]{ "X-Authorization" };
constexpr char kSecretKeyHeaderName[]{ "X-SecretKey" };
constexpr char kEntityTokenHeaderName[]{ "X-EntityToken" };
constexpr char kTelemetryKeyHeaderName[]{ "X-TelemetryKey" };

// PlayFab SDK version string header
constexpr char kSDKVersionStringHeaderName[]{ "X-PlayFabSDK" };
constexpr char kSDKUserAgentHeader[]{ "User-Agent" };

// Http POST method static string
constexpr char kPostMethod[]{ "POST" };

// Default Http retry settings
const bool kDefaultHttpRetryAllowed{ true };
const uint32_t kDefaultHttpRetryDelay{ 2 };
const uint32_t kDefaultHttpTimeoutWindow{ 20 };

// Wrapper around PlayFab service response.
// See https://docs.microsoft.com/en-us/rest/api/playfab/client/authentication/loginwithcustomid?view=playfab-rest#apierrorwrapper for
// more information.
struct ServiceResponse
{
    ServiceResponse() = default;
    ServiceResponse(const ServiceResponse&);
    ServiceResponse(ServiceResponse&&) = default;
    ServiceResponse& operator=(const ServiceResponse&);
    ServiceResponse& operator=(ServiceResponse&&) = default;
    ~ServiceResponse() = default;

    HRESULT FromJson(const JsonValue& input);

    // Fields from response body
    int HttpCode;
    String HttpStatus;
    ServiceErrorCode ErrorCode;
    String ErrorName;
    String ErrorMessage;
    JsonValue ErrorDetails;
    JsonValue Data;

    // From response header
    String RequestId;
};

// Wrapper around PFHCHttpCallPerformAsync XAsync operation making a PlayFab service call
class HCHttpCall : public XAsyncOperation<ServiceResponse>
{
public:
    HCHttpCall(
        String method,
        String url,
        UnorderedMap<String, String> headers,
        String requestBody,
        PlayFab::RunContext runContext
    ) noexcept;

    HCHttpCall(
        String method,
        String url,
        UnorderedMap<String, String> headers,
        String requestBody,
        uint32_t retryCacheId,
        PFHttpRetrySettings const& retrySettings,
        PlayFab::RunContext runContext
    ) noexcept;

    HCHttpCall(HCHttpCall const&) = delete;
    HCHttpCall& operator=(HCHttpCall) = delete;
    ~HCHttpCall() noexcept;

    void SetHeader(String headerName, String headerValue) noexcept;

private:
    // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ServiceResponse> GetResult(XAsyncBlock* async) noexcept override;

    static HRESULT CALLBACK HCRequestBodyRead(
        _In_ PFHCCallHandle callHandle,
        _In_ size_t offset,
        _In_ size_t bytesAvailable,
        _In_opt_ void* context,
        _Out_writes_bytes_to_(bytesAvailable, *bytesWritten) uint8_t* destination,
        _Out_ size_t* bytesWritten
    );

    static HRESULT CALLBACK HCResponseBodyWrite(
        _In_ PFHCCallHandle callHandle,
        _In_reads_bytes_(bytesAvailable) const uint8_t* source,
        _In_ size_t bytesAvailable,
        _In_opt_ void* context
    );

    String const m_method;
    String const m_url;
    UnorderedMap<String, String> m_headers;
    String const m_requestBody;
    Vector<char> m_responseBody;
    std::optional<uint32_t> m_retryCacheId;
    bool m_retryAllowed{ kDefaultHttpRetryAllowed };
    uint32_t m_retryDelay{ kDefaultHttpRetryDelay };
    uint32_t m_timeoutWindow{ kDefaultHttpTimeoutWindow };
    PFHCCallHandle m_callHandle{ nullptr };
};

}

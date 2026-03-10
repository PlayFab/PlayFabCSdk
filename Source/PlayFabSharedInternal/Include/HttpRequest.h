#pragma once

#include <httpClient/httpClient.h>
#include <httpClient/httpProvider.h>
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

// Default Http decompression settings
const bool kDefaultHttpCompressedResponsesExpected { false };

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
    HRESULT FromVector(const Vector<char>& input);
    inline bool IsHttpSuccess() const { return HttpCode >= 200 && HttpCode < 300; }

    // Fields from response body
    int HttpCode{ 0 };
    String HttpStatus;
    ServiceErrorCode ErrorCode{ ServiceErrorCode::Success };
    String ErrorName;
    String ErrorMessage;
    JsonValue ErrorDetails;
    JsonValue Data;
    Vector<char> ResponseBody; // Empty unless not PF call.  Use "Data" for PF calls

    // From response header
    String RequestId;
};

// Wrapper around HCHttpCallPerformAsync XAsync operation making a PlayFab service call
class HCHttpCall : public XAsyncOperation<ServiceResponse>
{
public:
    HCHttpCall(
        String method,
        String url,
        UnorderedMap<String, String> headers,
        String requestBody,
        PlayFab::RunContext runContext,
        HCCompressionLevel compressionLevel = HCCompressionLevel::None
    ) noexcept;

    HCHttpCall(
        String method,
        String url,
        UnorderedMap<String, String> headers,
        String requestBody,
        uint32_t retryCacheId,
        PFHttpRetrySettings const& retrySettings,
        PlayFab::RunContext runContext,
        PFHttpSettings const& httpSettings,
        HCCompressionLevel compressionLevel = HCCompressionLevel::None
    ) noexcept;

    HCHttpCall(HCHttpCall const&) = delete;
    HCHttpCall& operator=(HCHttpCall) = delete;
    virtual ~HCHttpCall() noexcept;

    void SetHeader(String headerName, String headerValue) noexcept;
    void SetIsPlayfabCall(bool isPlayfabCall) noexcept { m_isPlayfabCall = isPlayfabCall; }
    void SetDynamicSize(uint64_t totalSize, uint64_t currentSize) noexcept;
    void SetProgressReportCallback(bool isUploadFunction, void* context, HCHttpCallProgressReportFunction callback) noexcept;

private:
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ServiceResponse> GetResult(XAsyncBlock* async) noexcept override;
    Result<ServiceResponse> GetResultOfPlayfabCall() noexcept;

protected:
    HRESULT SetupCall() noexcept;

    static HRESULT CALLBACK HCRequestBodyRead(
        _In_ HCCallHandle callHandle,
        _In_ size_t offset,
        _In_ size_t bytesAvailable,
        _In_opt_ void* context,
        _Out_writes_bytes_to_(bytesAvailable, *bytesWritten) uint8_t* destination,
        _Out_ size_t* bytesWritten
    );

    static HRESULT CALLBACK HCResponseBodyWrite(
        _In_ HCCallHandle callHandle,
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
    bool m_isPlayfabCall{ true };
    uint32_t m_retryDelay{ kDefaultHttpRetryDelay };
    uint32_t m_timeoutWindow{ kDefaultHttpTimeoutWindow };
    HCCallHandle m_callHandle{ nullptr };
    HCCompressionLevel m_compressionLevel;
    HCHttpCallProgressReportFunction m_progressReportCallback{ nullptr };
    void* m_progressReportContext{ nullptr };
    bool m_isUploadFunction{ false };
    uint64_t m_dynamicTotalSize{ 0 };
    uint64_t m_dynamicCurrentSize{ 0 };
    bool m_compressedResponsesExpected{ kDefaultHttpCompressedResponsesExpected };
};

}

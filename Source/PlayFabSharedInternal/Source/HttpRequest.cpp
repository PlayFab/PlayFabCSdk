#include "pch.h"
#include "HttpRequest.h"
#include "JsonUtils.h"
#include "SdkVersion.h"

namespace PlayFab
{

ServiceResponse::ServiceResponse(const ServiceResponse& src) :
    HttpCode{ src.HttpCode },
    HttpStatus{ src.HttpStatus },
    ErrorCode{ src.ErrorCode },
    ErrorName{ src.ErrorName },
    ErrorMessage{ src.ErrorMessage },
    RequestId{ src.RequestId }
{
    JsonUtils::FromJson(src.ErrorDetails, ErrorDetails);
    JsonUtils::FromJson(src.Data, Data);
}

ServiceResponse& ServiceResponse::operator=(const ServiceResponse& src)
{
    HttpCode = src.HttpCode;
    HttpStatus = src.HttpStatus;
    ErrorCode = src.ErrorCode;
    ErrorName = src.ErrorName;
    ErrorMessage = src.ErrorMessage;
    JsonUtils::FromJson(src.ErrorDetails, ErrorDetails);
    JsonUtils::FromJson(src.Data, Data);
    RequestId = src.RequestId;
    return *this;
}

HRESULT ServiceResponse::FromJson(const JsonValue& input)
{
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "code", HttpCode));
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "status", HttpStatus));
    std::optional<int64_t> errorCode{};
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "errorCode", errorCode));
    if (errorCode.has_value())
    {
        ErrorCode = static_cast<ServiceErrorCode>(*errorCode);
    }
    else
    {
        ErrorCode = ServiceErrorCode::Success;
    }
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "error", ErrorName));
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "errorMessage", ErrorMessage));
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "errorDetails", ErrorDetails));
    RETURN_IF_FAILED(JsonUtils::ObjectGetMember(input, "data", Data));

    return S_OK;
}

HCHttpCall::HCHttpCall(
    String method,
    String url,
    UnorderedMap<String, String> headers,
    String requestBody,
    PlayFab::RunContext runContext,
    HCCompressionLevel compressionLevel
) noexcept :
    XAsyncOperation<ServiceResponse>{ std::move(runContext) },
    m_method{ std::move(method) },
    m_url{ std::move(url) },
    m_headers{ std::move(headers) },
    m_requestBody{ std::move(requestBody) },
    m_compressionLevel { compressionLevel }
{
}

HCHttpCall::HCHttpCall(
    String method,
    String url,
    UnorderedMap<String, String> headers,
    String requestBody,
    uint32_t retryCacheId,
    PFHttpRetrySettings const& retrySettings,
    PlayFab::RunContext runContext,
    PFHttpSettings const& httpSettings,
    HCCompressionLevel compressionLevel
) noexcept :
    XAsyncOperation<ServiceResponse>{ std::move(runContext) },
    m_method{ std::move(method) },
    m_url{ std::move(url) },
    m_headers{ std::move(headers) },
    m_requestBody{ std::move(requestBody) },
    m_retryCacheId{ retryCacheId },
    m_retryAllowed{ retrySettings.allowRetry },
    m_retryDelay{ retrySettings.minimumRetryDelayInSeconds },
    m_timeoutWindow{ retrySettings.timeoutWindowInSeconds },
    m_compressionLevel{ compressionLevel },
    m_compressedResponsesExpected {httpSettings.requestResponseCompression }
{
}

HCHttpCall::~HCHttpCall() noexcept
{
    if (m_callHandle)
    {
        HCHttpCallCloseHandle(m_callHandle);
    }
}

void HCHttpCall::SetHeader(String headerName, String headerValue) noexcept
{
    m_headers[std::move(headerName)] = std::move(headerValue);
}

HRESULT HCHttpCall::OnStarted(XAsyncBlock* async) noexcept
{
    // Set up HCHttpCallHandle
    RETURN_IF_FAILED(HCHttpCallCreate(&m_callHandle));
    RETURN_IF_FAILED(HCHttpCallRequestSetUrl(m_callHandle, m_method.data(), m_url.data()));
    RETURN_IF_FAILED(HCHttpCallResponseSetResponseBodyWriteFunction(m_callHandle, HCHttpCall::HCResponseBodyWrite, this));

    // Setup Compression level
    RETURN_IF_FAILED(HCHttpCallRequestEnableGzipCompression(m_callHandle, m_compressionLevel));

    // Set whether the expected response should be compressed
    if (m_compressedResponsesExpected)
    {
        RETURN_IF_FAILED(HCHttpCallRequestSetHeader(m_callHandle, "Accept-Encoding", "application/gzip", true));
        RETURN_IF_FAILED(HCHttpCallResponseSetGzipCompressed(m_callHandle, true));
    }

    // Doing this assignment to avoid guarding the variable and having to add several guards on the header file.
    // This is due to a PlayStation build warning about private field not being used
    m_compressionLevel = HCCompressionLevel::None;

    // Add default PlayFab headers
    RETURN_IF_FAILED(HCHttpCallRequestSetHeader(m_callHandle, "Accept", "application/json", true));
    RETURN_IF_FAILED(HCHttpCallRequestSetHeader(m_callHandle, "Content-Type", "application/json; charset=utf-8", true));
    RETURN_IF_FAILED(HCHttpCallRequestSetHeader(m_callHandle, "X-ReportErrorAsSuccess", "true", true));

    // Configure retries
    RETURN_IF_FAILED(HCHttpCallRequestSetRetryAllowed(m_callHandle, m_retryAllowed));
    RETURN_IF_FAILED(HCHttpCallRequestSetRetryDelay(m_callHandle, m_retryDelay));
    RETURN_IF_FAILED(HCHttpCallRequestSetTimeoutWindow(m_callHandle, m_timeoutWindow));

    // Add additional version string header
    Stringstream sdkFullVer;
    sdkFullVer << versionString << sdkVersion;
    String strFullVerStr = sdkFullVer.str();
    RETURN_IF_FAILED(HCHttpCallRequestSetHeader(m_callHandle, kSDKVersionStringHeaderName, strFullVerStr.c_str(), true));

    Stringstream sdkUserAgent;
    sdkUserAgent << userAgent << sdkVersion;
    String sdkUserAgentStr = sdkUserAgent.str();
    RETURN_IF_FAILED(HCHttpCallRequestSetHeader(m_callHandle, kSDKUserAgentHeader, sdkUserAgentStr.c_str(), true));

    for (const auto& pair : m_headers)
    {
        if (!pair.first.empty() && !pair.second.empty())
        {
            RETURN_IF_FAILED(HCHttpCallRequestSetHeader(m_callHandle, pair.first.data(), pair.second.data(), true));
        }
    }

    if (!m_requestBody.empty())
    {
        RETURN_IF_FAILED(HCHttpCallRequestSetRequestBodyReadFunction(m_callHandle, HCHttpCall::HCRequestBodyRead, m_requestBody.size(), this));
    }

    if (m_retryCacheId.has_value())
    {
        RETURN_IF_FAILED(HCHttpCallRequestSetRetryCacheId(m_callHandle, *m_retryCacheId));
    }

    return HCHttpCallPerformAsync(m_callHandle, async);
}

Result<ServiceResponse> HCHttpCall::GetResult(XAsyncBlock* async) noexcept
{
    // By design XAsyncBlock should succeed for HttpCall Perform
    RETURN_IF_FAILED(XAsyncGetStatus(async, false));

    // Try to parse the response body no matter what. PlayFab often returns a response body even
    // on failure and they can provide more details about what went wrong. If we are unable to parse the response
    // body correctly, fall back to returning the Http status code.

    // Ensure response is null terminated before treating as a string
    m_responseBody.push_back(0);

    JsonDocument responseJson{ &JsonUtils::allocator, JsonUtils::kDefaultStackCapacity, &JsonUtils::allocator };
    responseJson.Parse(m_responseBody.data());
    if (responseJson.HasParseError())
    {
        // Couldn't parse response body, fall back to Http status code
        uint32_t httpCode{ 0 };
        RETURN_IF_FAILED(HCHttpCallResponseGetStatusCode(m_callHandle, &httpCode));
        RETURN_IF_FAILED(HttpStatusToHR(httpCode));

        // This is an unusual case. We weren't able to parse the response body, but the Http status code indicates that the
        // call was successful. Return the Json parse error in this case.
        Stringstream errorMessage;
        errorMessage << "Failed to parse PlayFab service response: " << rapidjson::GetParseError_En(responseJson.GetParseError());
        TRACE_ERROR(errorMessage.str().data());

        return Result<ServiceResponse>{ E_FAIL, errorMessage.str() };
    }

    // Successful response from service (doesn't always indicate the call was successful, just that the service responded successfully)
    ServiceResponse response{};
    response.FromJson(responseJson);

    // Get requestId response header
    const char* requestId;
    RETURN_IF_FAILED(HCHttpCallResponseGetHeader(m_callHandle, "X-RequestId", &requestId));

    if (requestId)
    {
        response.RequestId = requestId;
    }

    return response;
}

HRESULT HCHttpCall::HCRequestBodyRead(
    _In_ HCCallHandle callHandle,
    _In_ size_t offset,
    _In_ size_t bytesAvailable,
    _In_opt_ void* context,
    _Out_writes_bytes_to_(bytesAvailable, *bytesWritten) uint8_t* destination,
    _Out_ size_t* bytesWritten
)
{
    UNREFERENCED_PARAMETER(callHandle);

    assert(destination);
    assert(bytesAvailable > 0);
    assert(bytesWritten);

    auto call{ static_cast<HCHttpCall*>(context) }; // non-owning
    assert(offset < call->m_requestBody.size());

    *bytesWritten = std::min(bytesAvailable, call->m_requestBody.size() - offset);
    std::memcpy(destination, call->m_requestBody.data() + offset, *bytesWritten);

    return S_OK;
}

HRESULT HCHttpCall::HCResponseBodyWrite(
    _In_ HCCallHandle callHandle,
    _In_reads_bytes_(bytesAvailable) const uint8_t* source,
    _In_ size_t bytesAvailable,
    _In_opt_ void* context
)
{
    UNREFERENCED_PARAMETER(callHandle);

    assert(source);
    assert(bytesAvailable > 0);

    auto call{ static_cast<HCHttpCall*>(context) }; // non-owning
    call->m_responseBody.insert(call->m_responseBody.end(), source, source + bytesAvailable);

    return S_OK;
}

}

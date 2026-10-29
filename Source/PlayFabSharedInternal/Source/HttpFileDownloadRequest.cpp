#include "pch.h"
#include "HttpFileDownloadRequest.h"
#include "JsonUtils.h"
#include "SdkVersion.h"

namespace PlayFab
{

HCHttpFileDownloadCall::HCHttpFileDownloadCall(
    String method,
    String url,
    UnorderedMap<String, String> headers,
    String requestBody,
    String responseBodyFilePath,
    PlayFab::RunContext runContext,
    HCCompressionLevel compressionLevel
) noexcept :
    HCHttpCall(std::move(method), std::move(url), std::move(headers), std::move(requestBody), std::move(runContext), compressionLevel),
    m_responseBodyFilePath{ std::move(responseBodyFilePath) }
{
}

HCHttpFileDownloadCall::HCHttpFileDownloadCall(
    String method,
    String url,
    UnorderedMap<String, String> headers,
    String requestBody,
    String responseBodyFilePath,
    uint32_t retryCacheId,
    PFHttpRetrySettings const& retrySettings,
    PlayFab::RunContext runContext,
    PFHttpSettings const& httpSettings,
    HCCompressionLevel compressionLevel
) noexcept :
    HCHttpCall(std::move(method), std::move(url), std::move(headers), std::move(requestBody), retryCacheId, retrySettings, std::move(runContext), httpSettings, compressionLevel),
    m_responseBodyFilePath{ std::move(responseBodyFilePath) }
{
}

HRESULT HCHttpFileDownloadCall::OnStarted(XAsyncBlock* async) noexcept
{
    // Skip native LHC progress registration during SetupCall — this subclass
    // provides per-chunk progress from HCResponseBodyWriteToFile which is more
    // frequent and accurate. Registering both causes interleaved/oscillating values.
    auto savedCallback = m_progressReportCallback;
    m_progressReportCallback = nullptr;
    HRESULT setupHr = SetupCall();
    m_progressReportCallback = savedCallback;
    RETURN_IF_FAILED(setupHr);

    // Override the response body write function to write to a file
    auto fileResult = FilePAL::OpenFile(m_responseBodyFilePath, FileOpenMode::Write);
    RETURN_IF_FAILED(fileResult.hr);
    m_responseBodyFileStream = fileResult.ExtractPayload();

    HRESULT hr = HCHttpCallResponseSetResponseBodyWriteFunction(m_callHandle, HCHttpFileDownloadCall::HCResponseBodyWriteToFile, this);
    if (FAILED(hr))
    {
        FilePAL::CloseFile(m_responseBodyFileStream);
        return hr;
    }

    hr = HCHttpCallPerformAsync(m_callHandle, async);
    if (FAILED(hr))
    {
        FilePAL::CloseFile(m_responseBodyFileStream);
        return hr;
    }

    return S_OK;
}

HRESULT HCHttpFileDownloadCall::HCResponseBodyWriteToFile(
    _In_ HCCallHandle callHandle,
    _In_reads_bytes_(bytesAvailable) const uint8_t* source,
    _In_ size_t bytesAvailable,
    _In_opt_ void* context
)
{
    UNREFERENCED_PARAMETER(callHandle);

    assert(context);
    assert(source);
    assert(bytesAvailable > 0);

    auto call{ static_cast<HCHttpFileDownloadCall*>(context) }; // non-owning

    // libHttpClient retries a failed transfer on the same call handle - including a transfer whose
    // body write failed here - and the new attempt restarts the body from byte 0. Start the file over
    // for every attempt after the first: appending would corrupt it, and a stream left bad by the
    // previous attempt's failed write would refuse every byte of this one.
    uint32_t filePerformCount{};
    if (SUCCEEDED(HCHttpCallGetPerformCount(callHandle, &filePerformCount)) && filePerformCount != call->m_filePerformCount)
    {
        if (call->m_filePerformCount != 0 || !call->m_responseBodyFileStream)
        {
            if (call->m_responseBodyFileStream)
            {
                FilePAL::CloseFile(call->m_responseBodyFileStream);
            }
            auto fileResult = FilePAL::OpenFile(call->m_responseBodyFilePath, FileOpenMode::Write);
            if (FAILED(fileResult.hr))
            {
                call->m_bodyWriteHr = fileResult.hr;
                return fileResult.hr;
            }
            call->m_responseBodyFileStream = fileResult.ExtractPayload();
        }
        call->m_filePerformCount = filePerformCount;
        call->m_bodyWriteHr = S_OK;
    }

    if (!call->m_responseBodyFileStream)
    {
        return FAILED(call->m_bodyWriteHr) ? call->m_bodyWriteHr : E_UNEXPECTED;
    }

    HRESULT writeHr = FilePAL::WriteFileBytes(call->m_responseBodyFileStream, reinterpret_cast<const char*>(source), bytesAvailable);
    if (FAILED(writeHr))
    {
        // Remembered because the platform HTTP stack reports a failed body write only as the call's
        // network error, and may retry past it; GetResult must surface the real local cause.
        call->m_bodyWriteHr = writeHr;
        return writeHr;
    }

    if (call->m_dynamicTotalSize > 0)
    {
        RETURN_IF_FAILED(HCHttpCallResponseAddDynamicBytesWritten(call->m_callHandle, bytesAvailable));
    }

    // Fire per-chunk download progress so callers get frequent updates.
    // The platform HTTP stack's native progress reporting may be very infrequent.
    if (call->m_progressReportCallback)
    {
        // libHttpClient retries a failed transfer on the same call handle, and the bytes from the
        // abandoned attempt are not replayed to the caller. Accumulating across attempts and
        // clamping the result made progress saturate at 100% and stop moving for the remainder of
        // a retried transfer - the stall this per-chunk reporting exists to avoid. Reset the
        // counter when the perform attempt changes so each attempt measures only its own bytes.
        //
        // Deliberately not RETURN_IF_FAILED: this is a progress-only query, and this function is
        // the response body write callback, so returning a failure here aborts the download - for
        // a chunk that has already been written to the file above. Skip this chunk's update
        // instead, including the accumulation: without a trustworthy perform count there is no way
        // to tell whether a reset was due, and counting anyway risks carrying an abandoned
        // attempt's bytes forward. A later chunk re-reads the count.
        uint32_t performCount{};
        if (SUCCEEDED(HCHttpCallGetPerformCount(callHandle, &performCount)))
        {
            if (performCount != call->m_progressPerformCount)
            {
                call->m_progressPerformCount = performCount;
                call->m_totalBytesReceived = 0;
            }

            call->m_totalBytesReceived += bytesAvailable;

            uint64_t totalProgress = (call->m_dynamicTotalSize > 0) ? call->m_dynamicTotalSize : call->m_totalBytesReceived;
            uint64_t currentProgress = std::min(call->m_dynamicCurrentSize, totalProgress);
            currentProgress += std::min(call->m_totalBytesReceived, totalProgress - currentProgress);

            call->m_progressReportCallback(call->m_callHandle, currentProgress, totalProgress, call->m_progressReportContext);
        }
    }

    return S_OK;
}

PlayFab::Result<PlayFab::ServiceResponse> HCHttpFileDownloadCall::GetResult(XAsyncBlock* async) noexcept
{
    if (m_responseBodyFileStream)
    {
        FilePAL::CloseFile(m_responseBodyFileStream);
    }
    
    // By design XAsyncBlock should succeed for HttpCall Perform
    RETURN_IF_FAILED(XAsyncGetStatus(async, false));

    // Successful response from service (doesn't always indicate the call was successful, just that the service responded successfully)
    ServiceResponse response{};

    uint32_t callCount{ 1 };
    uint32_t httpCode{ 0 };
    HCHttpCallGetPerformCount(m_callHandle, &callCount);
    RETURN_IF_FAILED(HCHttpCallResponseGetStatusCode(m_callHandle, &httpCode));
    HttpResult httpResult{ callCount - 1, httpCode };
    response.HttpCode = httpCode;

    // The status line arrives before the body, so a transfer that failed part-way through the body
    // still reports 200. The platform stack records that failure only as the network error, and
    // the file on disk is truncated. A failed local write is the more specific cause (a full disk,
    // for instance), so it takes precedence.
    if (FAILED(m_bodyWriteHr))
    {
        TRACE_ERROR("HCHttpFileDownloadCall: writing the response body to '%s' failed hr=0x%08X", m_responseBodyFilePath.c_str(), m_bodyWriteHr);
        return Result<ServiceResponse>{ m_bodyWriteHr, std::move(httpResult) };
    }

    HRESULT networkErrorCode{ S_OK };
    uint32_t platformNetworkErrorCode{ 0 };
    if (SUCCEEDED(HCHttpCallResponseGetNetworkErrorCode(m_callHandle, &networkErrorCode, &platformNetworkErrorCode)) && FAILED(networkErrorCode))
    {
        TRACE_ERROR("HCHttpFileDownloadCall: transfer failed hr=0x%08X (platform error %u), http status %u", networkErrorCode, platformNetworkErrorCode, httpCode);
        return Result<ServiceResponse>{ networkErrorCode, std::move(httpResult) };
    }

    RETURN_IF_FAILED(HttpStatusToHR(httpCode));

    return Result<ServiceResponse>{ std::move(response), std::move(httpResult) };
}


}

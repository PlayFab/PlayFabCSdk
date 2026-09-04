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
    RETURN_IF_FAILED(FilePAL::WriteFileBytes(call->m_responseBodyFileStream, reinterpret_cast<const char*>(source), bytesAvailable));

    if (call->m_dynamicTotalSize > 0)
    {
        RETURN_IF_FAILED(HCHttpCallResponseAddDynamicBytesWritten(call->m_callHandle, bytesAvailable));
    }

    // Fire per-chunk download progress so callers get frequent updates.
    // The platform HTTP stack's native progress reporting may be very infrequent.
    if (call->m_progressReportCallback)
    {
        call->m_totalBytesReceived += bytesAvailable;
        uint64_t currentProgress = call->m_dynamicCurrentSize + call->m_totalBytesReceived;
        uint64_t totalProgress = (call->m_dynamicTotalSize > 0) ? call->m_dynamicTotalSize : call->m_totalBytesReceived;

        // Clamp to prevent reporting progress > 100% (e.g., if bytes accumulate across HTTP retries)
        if (currentProgress > totalProgress)
        {
            currentProgress = totalProgress;
        }

        call->m_progressReportCallback(call->m_callHandle, currentProgress, totalProgress, call->m_progressReportContext);
    }

    return S_OK;
}

PlayFab::Result<PlayFab::ServiceResponse> HCHttpFileDownloadCall::GetResult(XAsyncBlock* async) noexcept
{
    FilePAL::CloseFile(m_responseBodyFileStream);
    
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
    RETURN_IF_FAILED(HttpStatusToHR(httpCode));

    return Result<ServiceResponse>{ std::move(response), std::move(httpResult) };
}


}

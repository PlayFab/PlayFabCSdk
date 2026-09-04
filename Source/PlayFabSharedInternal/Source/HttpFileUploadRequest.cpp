#include "pch.h"
#include "HttpFileUploadRequest.h"
#include "JsonUtils.h"
#include "SdkVersion.h"

namespace PlayFab
{

HCHttpFileUploadCall::HCHttpFileUploadCall(
    String method,
    String url,
    UnorderedMap<String, String> headers,
    String requestBodyFilePath,
    PlayFab::RunContext runContext
) noexcept :
    HCHttpCall(std::move(method), std::move(url), std::move(headers), "", std::move(runContext)),
    m_requestBodyFilePath{ std::move(requestBodyFilePath) }
{
}

HCHttpFileUploadCall::HCHttpFileUploadCall(
    String method,
    String url,
    UnorderedMap<String, String> headers,
    String requestBodyFilePath,
    uint32_t retryCacheId,
    PFHttpRetrySettings const& retrySettings,
    PlayFab::RunContext runContext,
    PFHttpSettings const& httpSettings
) noexcept :
    HCHttpCall(std::move(method), std::move(url), std::move(headers), "", retryCacheId, retrySettings, std::move(runContext), httpSettings),
    m_requestBodyFilePath{ std::move(requestBodyFilePath) }
{
}

HRESULT HCHttpFileUploadCall::OnStarted(XAsyncBlock* async) noexcept
{
    // Skip native LHC progress registration during SetupCall — this subclass
    // provides per-chunk progress from HCRequestBodyReadFromFile which is more
    // frequent and accurate. Registering both causes interleaved/oscillating values.
    auto savedCallback = m_progressReportCallback;
    m_progressReportCallback = nullptr;
    HRESULT setupHr = SetupCall();
    m_progressReportCallback = savedCallback;
    RETURN_IF_FAILED(setupHr);

    // Override the request body read function to read from the file
    auto sizeResult = FilePAL::GetFileSize(m_requestBodyFilePath);
    RETURN_IF_FAILED(sizeResult.hr);

    m_requestBodyFileSize = sizeResult.Payload(); // Store for per-chunk progress reporting

    auto result = FilePAL::OpenFile(m_requestBodyFilePath, FileOpenMode::Read);
    RETURN_IF_FAILED(result.hr);
    m_requestBodyFileStream = result.ExtractPayload();

    HRESULT hr = HCHttpCallRequestSetRequestBodyReadFunction(m_callHandle, HCHttpFileUploadCall::HCRequestBodyReadFromFile, sizeResult.Payload(), this);
    if (FAILED(hr))
    {
        FilePAL::CloseFile(m_requestBodyFileStream);
        return hr;
    }

    if (m_retryCacheId.has_value())
    {
        hr = HCHttpCallRequestSetRetryCacheId(m_callHandle, *m_retryCacheId);
        if (FAILED(hr))
        {
            FilePAL::CloseFile(m_requestBodyFileStream);
            return hr;
        }
    }

    hr = HCHttpCallPerformAsync(m_callHandle, async);
    if (FAILED(hr))
    {
        FilePAL::CloseFile(m_requestBodyFileStream);
        return hr;
    }

    return S_OK;
}

HRESULT HCHttpFileUploadCall::HCRequestBodyReadFromFile(
    _In_ HCCallHandle callHandle,
    _In_ size_t offset,
    _In_ size_t bytesAvailable,
    _In_opt_ void* context,
    _Out_writes_bytes_to_(bytesAvailable, *bytesWritten) uint8_t* destination,
    _Out_ size_t* bytesWritten
)
{
    UNREFERENCED_PARAMETER(callHandle);

    assert(context);
    assert(destination);
    assert(bytesAvailable > 0);
    assert(bytesWritten);

    auto call{ static_cast<HCHttpFileUploadCall*>(context) }; // non-owning

    RETURN_IF_FAILED(FilePAL::ReadFileBytes(call->m_requestBodyFileStream, bytesAvailable, reinterpret_cast<char*>(destination), bytesWritten));

    if (call->m_dynamicTotalSize > 0)
    {
        RETURN_IF_FAILED(HCHttpCallRequestAddDynamicBytesWritten(call->m_callHandle, *bytesWritten));
    }

    // Fire per-chunk upload progress so callers get frequent updates.
    // The platform HTTP stack's native progress reporting may be very infrequent
    // (e.g., only a few callbacks for an entire multi-MB upload on PlayStation).
    // Since this function is called for every chunk read from disk, we can provide
    // much more granular progress by computing it from the file offset.
    if (call->m_progressReportCallback)
    {
        uint64_t bytesReadSoFar = static_cast<uint64_t>(offset) + static_cast<uint64_t>(*bytesWritten);
        uint64_t currentProgress = call->m_dynamicCurrentSize + bytesReadSoFar;
        uint64_t totalProgress = (call->m_dynamicTotalSize > 0) ? call->m_dynamicTotalSize : call->m_requestBodyFileSize;

        // Clamp to prevent reporting progress > 100% (e.g., if bytes accumulate across HTTP retries)
        if (currentProgress > totalProgress)
        {
            currentProgress = totalProgress;
        }

        call->m_progressReportCallback(call->m_callHandle, currentProgress, totalProgress, call->m_progressReportContext);
    }

    return S_OK;
}

PlayFab::Result<PlayFab::ServiceResponse> HCHttpFileUploadCall::GetResult(XAsyncBlock* async) noexcept
{
    FilePAL::CloseFile(m_requestBodyFileStream);

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

#include "pch.h"
#include "HttpArchiveDownloadRequest.h"
#include "JsonUtils.h"
#include "SdkVersion.h"

#if HC_PLATFORM != HC_PLATFORM_NINTENDO_SWITCH

namespace PlayFab
{

HCHttpArchiveDownloadCall::HCHttpArchiveDownloadCall(
    String method,
    String url,
    UnorderedMap<String, String> headers,
    String requestBody,
    SharedPtr<ArchiveContext> archiveHandle,
    PlayFab::RunContext runContext,
    HCCompressionLevel compressionLevel
) noexcept :
    HCHttpCall(std::move(method), std::move(url), std::move(headers), std::move(requestBody), std::move(runContext), compressionLevel),
    m_archiveHandle{ archiveHandle }
{
}

HCHttpArchiveDownloadCall::HCHttpArchiveDownloadCall(
    String method,
    String url,
    UnorderedMap<String, String> headers,
    String requestBody,
    SharedPtr<ArchiveContext> archiveHandle,
    uint32_t retryCacheId,
    PFHttpRetrySettings const& retrySettings,
    PlayFab::RunContext runContext,
    PFHttpSettings const& httpSettings,
    HCCompressionLevel compressionLevel
) noexcept :
    HCHttpCall(std::move(method), std::move(url), std::move(headers), std::move(requestBody), retryCacheId, retrySettings, std::move(runContext), httpSettings, compressionLevel),
    m_archiveHandle{ archiveHandle }
{
}

HRESULT HCHttpArchiveDownloadCall::OnStarted(XAsyncBlock* async) noexcept
{
    // Skip native LHC progress registration during SetupCall — this subclass
    // provides per-chunk progress from HCResponseBodyDecompressAndWriteToFile which is more
    // frequent and accurate. Registering both causes interleaved/oscillating values.
    auto savedCallback = m_progressReportCallback;
    m_progressReportCallback = nullptr;
    HRESULT setupHr = SetupCall();
    m_progressReportCallback = savedCallback;
    RETURN_IF_FAILED(setupHr);

    // Override the response body write function to decompress the archive and write the files
    if (m_dynamicTotalSize == 0)
    {
        uint64_t uncompressedSize = m_archiveHandle->GetTotalUncompressedSize();
        RETURN_IF_FAILED(HCHttpCallResponseSetDynamicSize(m_callHandle, uncompressedSize));
    }

    RETURN_IF_FAILED(HCHttpCallResponseSetResponseBodyWriteFunction(m_callHandle, HCHttpArchiveDownloadCall::HCResponseBodyDecompressAndWriteToFile, this));

    return HCHttpCallPerformAsync(m_callHandle, async);
}

HRESULT HCHttpArchiveDownloadCall::HCResponseBodyDecompressAndWriteToFile(
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

    auto call{ static_cast<HCHttpArchiveDownloadCall*>(context) }; // non-owning

    uint64_t uncompressedBytesWritten{};
    
    RETURN_IF_FAILED(call->m_archiveHandle->DecompressBytesProvideData(reinterpret_cast<const char*>(source), bytesAvailable, &uncompressedBytesWritten));
    RETURN_IF_FAILED(HCHttpCallResponseAddDynamicBytesWritten(callHandle, uncompressedBytesWritten));

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

}

#endif

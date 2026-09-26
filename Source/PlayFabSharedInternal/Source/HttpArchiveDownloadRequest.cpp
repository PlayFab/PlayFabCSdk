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
        // libHttpClient retries a failed transfer on the same call handle, and the bytes from the
        // abandoned attempt are not replayed to the caller. Accumulating across attempts and
        // clamping the result made progress saturate at 100% and stop moving for the remainder of
        // a retried transfer - the stall this per-chunk reporting exists to avoid. Reset the
        // counters when the perform attempt changes so each attempt measures only its own bytes.
        //
        // Deliberately not RETURN_IF_FAILED: this is a progress-only query, and this function is
        // the response body write callback, so returning a failure here aborts the download - for
        // a chunk that has already been decompressed and accounted for above. Skip this chunk's
        // update instead. Skipping the accumulation too is intentional: without a trustworthy
        // perform count we cannot tell whether a reset was due, and counting anyway risks carrying
        // an abandoned attempt's bytes forward, which is the bug being fixed. A later chunk
        // re-reads the count, so the worst case is progress lagging by the skipped bytes.
        uint32_t performCount{};
        if (SUCCEEDED(HCHttpCallGetPerformCount(callHandle, &performCount)))
        {
            if (performCount != call->m_progressPerformCount)
            {
                call->m_progressPerformCount = performCount;
                call->m_totalBytesReceived = 0;
                call->m_totalUncompressedBytesWritten = 0;
            }

            call->m_totalBytesReceived += bytesAvailable;
            call->m_totalUncompressedBytesWritten += uncompressedBytesWritten;

            // A caller-supplied dynamic total is in compressed transfer bytes, so report compressed
            // bytes against it. Without one the only total available is the archive's own output size,
            // which is in uncompressed units - so report the uncompressed bytes written instead.
            // Mixing the two is what made current and total disagree at file completion.
            uint64_t totalProgress = (call->m_dynamicTotalSize > 0) ? call->m_dynamicTotalSize : call->m_archiveHandle->GetTotalUncompressedSize();
            uint64_t bytesTransferred = (call->m_dynamicTotalSize > 0) ? call->m_totalBytesReceived : call->m_totalUncompressedBytesWritten;
            uint64_t currentProgress = std::min(call->m_dynamicCurrentSize, totalProgress);
            currentProgress += std::min(bytesTransferred, totalProgress - currentProgress);

            call->m_progressReportCallback(call->m_callHandle, currentProgress, totalProgress, call->m_progressReportContext);
        }
    }

    return S_OK;
}

}

#endif

#include "stdafx.h"
#include "ProgressHelpers.h"

namespace PlayFab
{
namespace GameSave
{

HRESULT InnerProgressCallback(
    _In_ HCCallHandle call,
    _In_ uint64_t current,
    _In_ uint64_t total,
    _In_opt_ void* context)
{
    UNREFERENCED_PARAMETER(call);
    UNREFERENCED_PARAMETER(total);
    RETURN_HR_IF(E_INVALIDARG, !context);

    assert(context);

    auto innerContext{ static_cast<InnerProgressContext*>(context) };

    // Anchor at the uncompressed bytes already completed by earlier files, then scale this file's
    // compressed transfer into this file's own uncompressed range.
    //
    // A single global ratio (current / totalCompressed * totalUncompressed) is NOT monotonic here,
    // because these chunk callbacks are interleaved with completion callbacks that report the true
    // cumulative uncompressed count (UploadStep::UploadFileFinally, DownloadStep). A file that
    // compresses worse than the operation average over-reports while it transfers, and completion
    // then snaps the value back down. With two 8 MB archives at 1:1 and 8:1, the reported value
    // falls from 14,222,222 to 8,000,000 - a visible ~39% backward jump on the title's progress UI.
    // Scaling per file keeps every report bounded by the completion value that follows it.
    uint64_t reportedTotal = innerContext->totalUncompressedBytes;
    uint64_t reportedCurrent = std::min(innerContext->completedUncompressedBytes, reportedTotal);
    uint64_t compressedCurrent = std::min(current, innerContext->totalCompressedBytes);
    uint64_t fileCurrent = (compressedCurrent > innerContext->completedCompressedBytes)
        ? compressedCurrent - innerContext->completedCompressedBytes : 0;

    if (innerContext->fileCompressedBytes > 0)
    {
        uint64_t fileTotal = std::min(innerContext->fileUncompressedBytes, reportedTotal - reportedCurrent);
        double fileProgress = static_cast<double>(std::min(fileCurrent, innerContext->fileCompressedBytes))
            / static_cast<double>(innerContext->fileCompressedBytes) * static_cast<double>(fileTotal);

        // Bound the floating-point value before conversion, including rounding at the endpoint.
        reportedCurrent += (fileProgress >= static_cast<double>(fileTotal))
            ? fileTotal : static_cast<uint64_t>(fileProgress);
    }

    innerContext->callback(innerContext->syncState, reportedCurrent, reportedTotal, innerContext->callbackContext);
    return S_OK;
}

} // namespace GameSave
} // namespace PlayFab
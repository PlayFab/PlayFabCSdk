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

    uint64_t reportedCurrent = current;
    uint64_t reportedTotal = total;

    // Convert from compressed transfer bytes to uncompressed units using global ratio.
    // Progress is monotonically increasing and proportional to wall-clock transfer time.
    // Uses double intermediate to avoid uint64_t overflow in the multiplication for large files.
    if (innerContext->totalCompressedBytes > 0 && innerContext->totalUncompressedBytes > 0)
    {
        reportedCurrent = static_cast<uint64_t>(
            static_cast<double>(current) / static_cast<double>(innerContext->totalCompressedBytes)
            * static_cast<double>(innerContext->totalUncompressedBytes));
        reportedTotal = innerContext->totalUncompressedBytes;

        // Clamp to prevent reporting progress > 100% due to size mismatches
        if (reportedCurrent > reportedTotal)
        {
            reportedCurrent = reportedTotal;
        }
    }

    innerContext->callback(innerContext->syncState, reportedCurrent, reportedTotal, innerContext->callbackContext);
    return S_OK;
}

} // namespace GameSave
} // namespace PlayFab
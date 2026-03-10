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
    RETURN_HR_IF(E_INVALIDARG, !context);

    assert(context);

    auto innerContext{ static_cast<InnerProgressContext*>(context) };

    innerContext->callback(innerContext->syncState, current, total, innerContext->callbackContext);
    return S_OK;
}

} // namespace GameSave
} // namespace PlayFab
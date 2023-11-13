#include "stdafx.h"
#include <playfab/core/PFPlatform.h>
#include "Platform/Platform.h"
#include "PFCoreGlobalState.h"

using namespace PlayFab;

PF_API PFMemSetFunctions(
    _In_ PFMemoryHooks* hooks
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(hooks);

    SharedPtr<PFCoreGlobalState> state;
    PFCoreGlobalState::Get(state);
    RETURN_HR_IF(E_PF_CORE_ALREADY_INITIALIZED, state);

    RETURN_IF_FAILED(PlayFab::SetMemoryHooks(*hooks));

    // Try to set the memory hooks for libHttpClient as well. If it has already be initialized, there is nothing we can do
    HRESULT hr = HCMemSetFunctions([](size_t size, HCMemoryType)
    {
        return PlayFab::Alloc(size);
    },
    [](void* pointer, HCMemoryType)
    {
        return PlayFab::Free(pointer);
    });

    if (FAILED(hr) && hr != E_HC_ALREADY_INITIALISED)
    {
        return hr;
    }

    return S_OK;
}

PF_API PFMemGetFunctions(
    _Out_ PFMemoryHooks* hooks
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(hooks);

    *hooks = PlayFab::GetMemoryHooks();
    return S_OK;
}


PF_API PFPlatformLocalStorageSetHandlers(
    _In_ PFLocalStorageHooks* hooks
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(hooks);

    SharedPtr<PFCoreGlobalState> state;
    PFCoreGlobalState::Get(state);
    RETURN_HR_IF(E_PF_CORE_ALREADY_INITIALIZED, state);

    return PlayFab::SetLocalStorageHandlers(*hooks);
}

#include "stdafx.h"
#include <playfab/core/PFPlatform.h>
#include "Platform/Platform.h"
#include "PFCoreGlobalState.h"
#include "ApiHelpers.h"

using namespace PlayFab;

PF_API PFMemSetFunctions(
    _In_ PFMemoryHooks* hooks
) noexcept
{
    try
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
    catch (...)
    {
        TRACE_WARNING("[0x%08X] Exception reached api boundary %s\n    %s:%u", E_FAIL, XASYNC_IDENTITY(PFMemSetFunctions), __FILE__, __LINE__);
        return CurrentExceptionToHR();
    }
}

PF_API PFMemGetFunctions(
    _Out_ PFMemoryHooks* hooks
) noexcept
{
    try
    {
        RETURN_HR_INVALIDARG_IF_NULL(hooks);

        *hooks = PlayFab::GetMemoryHooks();
        return S_OK;
    }
    catch (...)
    {
        TRACE_WARNING("[0x%08X] Exception reached api boundary %s\n    %s:%u", E_FAIL, XASYNC_IDENTITY(PFMemGetFunctions), __FILE__, __LINE__);
        return CurrentExceptionToHR();
    }
}

PF_API PFMemIsUsingCustomMemoryFunctions(
    _Out_ bool* isUsingCustomMemoryFunctions
) noexcept
{
    try
    {
        RETURN_HR_INVALIDARG_IF_NULL(isUsingCustomMemoryFunctions);

        *isUsingCustomMemoryFunctions = PlayFab::IsUsingCustomMemoryHooks();
        return S_OK;
    }
    catch (...)
    {
        TRACE_WARNING("[0x%08X] Exception reached api boundary %s\n    %s:%u", E_FAIL, XASYNC_IDENTITY(PFMemIsUsingCustomMemoryFunctions), __FILE__, __LINE__);
        return CurrentExceptionToHR();
    }
}

PF_API PFPlatformLocalStorageSetHandlers(
    _In_ PFLocalStorageHooks* hooks
) noexcept
{
    try
    {
        RETURN_HR_INVALIDARG_IF_NULL(hooks);

        SharedPtr<PFCoreGlobalState> state;
        PFCoreGlobalState::Get(state);
        RETURN_HR_IF(E_PF_CORE_ALREADY_INITIALIZED, state);

        return PlayFab::SetLocalStorageHandlers(*hooks);
    }
    catch (...)
    {
        TRACE_WARNING("[0x%08X] Exception reached api boundary %s\n    %s:%u", E_FAIL, XASYNC_IDENTITY(PFPlatformLocalStorageSetHandlers), __FILE__, __LINE__);
        return CurrentExceptionToHR();
    }
}

PF_API PFPlatformIsGRTSAvailable(
    _Out_ bool* isGRTSAvailable
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFPlatformIsGRTSAvailable), [&](PFCoreGlobalState& state)
    {
        RETURN_HR_INVALIDARG_IF_NULL(isGRTSAvailable);
        *isGRTSAvailable = state.IsGRTSAvailable();
        return S_OK;
    });
}

PF_API PFPlatformGetPlatformType(
    _Out_ PFPlatformType* platformType
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFPlatformGetPlatformType), [&](PFCoreGlobalState& state)
    {
        RETURN_HR_INVALIDARG_IF_NULL(platformType);
        *platformType = state.GetPlatformType();
        return S_OK;
    });
}

#if HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFPlatformGetGameSaveContext(
    _Outptr_ void** gameSaveContext
) noexcept
{
    return ApiImpl(XASYNC_IDENTITY(PFPlatformGetGameSaveContext), [&](PFCoreGlobalState& state)
    {
        RETURN_HR_INVALIDARG_IF_NULL(gameSaveContext);
        *gameSaveContext = state.GameSaveContext();
        return S_OK;
    });
}
#endif

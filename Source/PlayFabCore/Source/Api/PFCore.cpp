#include "stdafx.h"
#include <playfab/core/PFCore.h>
#include "Common/PFCoreGlobalState.h"

using namespace PlayFab;

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFInitialize(
    _In_opt_ XTaskQueueHandle backgroundQueue
) noexcept
{
    return PFCoreGlobalState::Create(backgroundQueue, nullptr);
}
#endif

/// <summary>
/// Internal PFCore function to enable PFServices's init to also init'd PFCore with HCInitArgs on demand as needed
/// </summary>
PF_API PFInitializeWithLHC(
    _In_opt_ XTaskQueueHandle backgroundQueue,
    _In_ HCInitArgs* args
) noexcept
{
    return PFCoreGlobalState::Create(backgroundQueue, args);
}

PF_API PFUninitializeAsync(
    _In_ XAsyncBlock* async
) noexcept
{
    return PFCoreGlobalState::CleanupAsync(async);
}

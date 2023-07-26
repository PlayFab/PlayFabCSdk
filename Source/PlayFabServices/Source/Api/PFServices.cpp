#include "stdafx.h"
#include <playfab/services/PFServices.h>
#include "GlobalState.h"

using namespace PlayFab;

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
PF_API PFServicesInitialize(
    _In_opt_ XTaskQueueHandle backgroundQueue
) noexcept
{
    UNREFERENCED_PARAMETER(backgroundQueue);
    return GlobalState::Create(nullptr);
}
#endif

PF_API PFServicesUninitializeAsync(
    _In_ XAsyncBlock* async
) noexcept
{
    return GlobalState::CleanupAsync(async);
}

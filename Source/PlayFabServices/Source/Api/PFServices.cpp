#include "stdafx.h"
#include <playfab/services/PFServices.h>
#include "GlobalState.h"

using namespace PlayFab;

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFServicesInitialize(
    _In_opt_ XTaskQueueHandle backgroundQueue
) noexcept
{
    UNREFERENCED_PARAMETER(backgroundQueue);
    return GlobalState::Create(nullptr);
}
#endif

#if HC_PLATFORM == HC_PLATFORM_ANDROID
PF_API PFServicesInitialize(
    _In_opt_ XTaskQueueHandle backgroundQueue,
    _In_ HCInitArgs* initArgs
) noexcept
{
    UNREFERENCED_PARAMETER(backgroundQueue);
    return GlobalState::Create(initArgs);
}
#endif

PF_API PFServicesUninitializeAsync(
    _In_ XAsyncBlock* async
) noexcept
{
    return GlobalState::CleanupAsync(async);
}

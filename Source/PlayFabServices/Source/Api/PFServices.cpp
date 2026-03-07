#include "stdafx.h"
#include <playfab/services/PFServices.h>
#include "GlobalState.h"

using namespace PlayFab;

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
PF_API PFServicesInitialize(
    _In_opt_ XTaskQueueHandle backgroundQueue
) noexcept
{
    try
    {
        UNREFERENCED_PARAMETER(backgroundQueue);
        return GlobalState::Create(nullptr);
    }
    catch (...)
    {
        TRACE_WARNING("[0x%08X] Exception reached api boundary %s\n    %s:%u", E_FAIL, XASYNC_IDENTITY(PFServicesInitialize), __FILE__, __LINE__);
        return CurrentExceptionToHR();
    }
}
#endif

#if HC_PLATFORM == HC_PLATFORM_ANDROID
PF_API PFServicesInitialize(
    _In_opt_ XTaskQueueHandle backgroundQueue,
    _In_ HCInitArgs* initArgs
) noexcept
{
    try
    {
        UNREFERENCED_PARAMETER(backgroundQueue);
        return GlobalState::Create(initArgs);
    }
    catch (...)
    {
        TRACE_WARNING("[0x%08X] Exception reached api boundary %s\n    %s:%u", E_FAIL, XASYNC_IDENTITY(PFServicesInitialize), __FILE__, __LINE__);
        return CurrentExceptionToHR();
    }
}
#endif

PF_API PFServicesUninitializeAsync(
    _In_ XAsyncBlock* async
) noexcept
{
    return GlobalState::CleanupAsync(async);
}

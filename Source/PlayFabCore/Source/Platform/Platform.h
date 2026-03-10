#pragma once

#include <playfab/core/PFPlatform.h>

namespace PlayFab
{

enum PlatformInfo
{
    None = 0,
    GRTSAvailable = 1
};

HRESULT PlatformInitialize() noexcept;
HRESULT PlatformGetPlatformType(PlatformInfo& platformInfo, PFPlatformType& platformType) noexcept;

PFLocalStorageHooks& GetLocalStorageHandlers();
HRESULT SetLocalStorageHandlers(PFLocalStorageHooks& hooks);

}

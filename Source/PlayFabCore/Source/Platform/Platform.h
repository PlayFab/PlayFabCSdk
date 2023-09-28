#pragma once

#include <playfab/core/PFPlatform.h>

namespace PlayFab
{

HRESULT PlatformInitialize() noexcept;

PFLocalStorageHooks& GetLocalStorageHandlers();
HRESULT SetLocalStorageHandlers(PFLocalStorageHooks& hooks);

}

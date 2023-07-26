#pragma once

#include <playfab/core/PFPlatform.h>

namespace PlayFab
{
namespace Detail
{

PFLocalStorageHooks& GetLocalStorageHandlers();
HRESULT SetLocalStorageHandlers(PFLocalStorageHooks& hooks);

}

}

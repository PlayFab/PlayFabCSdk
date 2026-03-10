#pragma once

#if HC_PLATFORM == HC_PLATFORM_GDK

#include "Entity.h"
#include "Authentication/LoginHandler.h"
#include <playfab/core/PFLocalUser.h>

namespace PlayFab
{

Result<SharedPtr<LocalUser>> MakeSteamUser(
    _In_ SharedPtr<ServiceConfig> serviceConfig,
    _In_opt_ void* clientContext
);

bool IsRunningOnSteam();

}

#endif
#pragma once

#include "TestTypes.h"
#include "PlatformUser.h"

namespace PlayFab
{
namespace Test
{
namespace Platform
{

HRESULT SetHooks();

// Platform dependent PlayFabCore wrappers
HRESULT CoreInitialize(
    XTaskQueueHandle queue
) noexcept;

AsyncOp<PlayFab::Platform::UserPtr> GetDefaultPlatformUser(
    RunContext rc
) noexcept;

AsyncOp<LoginResult> LoginDefaultTitlePlayer(
    ServiceConfig serviceConfig,
    PlayFab::Platform::UserPtr platformUser,
    RunContext rc
) noexcept;

// Platform dependent PlayFabServices wrappers
HRESULT ServicesInitialize();

}
}
}

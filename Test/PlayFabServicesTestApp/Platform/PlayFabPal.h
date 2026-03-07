#pragma once

#include "TestTypes.h"

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

AsyncOp<TitleLocalUser> GetDefaultLocalUser(
    ServiceConfig const& serviceConfig,
    RunContext rc
) noexcept;

// Platform dependent PlayFabServices wrappers
HRESULT ServicesInitialize();

}
}
}

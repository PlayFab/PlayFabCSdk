#include "TestAppPch.h"
#include <Memory.h>
#include <playfab/services/PFServices.h>
#include "Platform/PlayFabPal.h"
#include "Platform/Generic/MemoryManager.h"
#include "Operations/Core/AuthenticationOperations.h"

using namespace PlayFab::Platform;

namespace PlayFab
{
namespace Test
{
namespace Platform
{

constexpr char defaultPlayerCustomId[] = "TestCustomId";

HRESULT SetHooks()
{
    // Using built in LocalStorage on Linux
    RETURN_IF_FAILED(MemoryManager::Instance().SetHooks());
    return S_OK;
}

HRESULT CoreInitialize(XTaskQueueHandle queue) noexcept
{
    return PFInitialize(queue);
}

AsyncOp<UserPtr> GetDefaultPlatformUser(
    RunContext rc
) noexcept
{
    // There is no platform user for Linux
    UNREFERENCED_PARAMETER(rc);
    return AsyncOp<UserPtr>{ UserPtr{} };
}

AsyncOp<LoginResult> LoginDefaultTitlePlayer(
    ServiceConfig serviceConfig,
    UserPtr platformUser,
    RunContext rc
) noexcept
{
    assert(!platformUser);
    UNREFERENCED_PARAMETER(platformUser);

    LoginWithCustomIDOperation::RequestType request;
    request.SetCustomId(defaultPlayerCustomId);
    request.SetCreateAccount(true);
    return RunOperation(MakeUnique<LoginWithCustomIDOperation>(serviceConfig, request, rc));
}

// Platform dependent PlayFabServices wrappers
HRESULT ServicesInitialize()
{
    return PFServicesInitialize(nullptr);
}

}
}
}
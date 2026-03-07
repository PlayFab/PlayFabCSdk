#include "TestAppPch.h"
#include "Platform/PlayFabPal.h"
#include "Platform/Generic/MemoryManager.h"
#include "Operations/Core/AuthenticationOperations.h"

namespace PlayFab
{
namespace Test
{
namespace Platform
{

HRESULT SetHooks()
{
    // Using built in LocalStorage on Win32
    RETURN_IF_FAILED(MemoryManager::Instance().SetHooks());
    return S_OK;
}

HRESULT CoreInitialize(XTaskQueueHandle queue) noexcept
{
    return PFInitialize(queue);
}

AsyncOp<TitleLocalUser> GetDefaultLocalUser(
    ServiceConfig const& serviceConfig,
    RunContext rc
) noexcept
{
    PFLocalUserHandle handle;
    RETURN_IF_FAILED(PFLocalUserCreateHandleWithSteamUser(serviceConfig.Handle(), nullptr, &handle));
    return TitleLocalUser{ LocalUserHandleWrapper::Wrap(handle), nullptr };
}

// Platform dependent PlayFabServices wrappers
HRESULT ServicesInitialize()
{
    return PFServicesInitialize(nullptr);
}

}
}
}
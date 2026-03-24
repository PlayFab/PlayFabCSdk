#include "TestAppPch.h"
#include "Platform/PlayFabPal.h"
#include "Platform/Generic/MemoryManager.h"
#include "Operations/Core/AuthenticationOperations.h"
#include "Platform/Generic/TitleLocalUser_Generic.h"

namespace PlayFab
{
namespace Test
{
namespace Platform
{

constexpr char defaultPlayerCustomId[] = "TestCustomIdLinux";

HRESULT SetHooks()
{
    // TODO: Investigate why this causes the test app to hang
    // RETURN_IF_FAILED(MemoryManager::Instance().SetHooks());
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
    auto customContext = MakeShared<CustomLocalUserContext>(defaultPlayerCustomId);

    PFLocalUserHandle handle;
    RETURN_IF_FAILED(PFLocalUserCreateHandleWithPersistedLocalId(serviceConfig.Handle(), customContext->customId.data(), CustomLocalUserContext::LocalUserLoginHandler, customContext.get(), &handle));
    return AsyncOp<TitleLocalUser>{ TitleLocalUser{ LocalUserHandleWrapper::Wrap(handle), std::move(customContext) } };
}

// Platform dependent PlayFabServices wrappers
HRESULT ServicesInitialize()
{
    return PFServicesInitialize(nullptr);
}

}
}
}
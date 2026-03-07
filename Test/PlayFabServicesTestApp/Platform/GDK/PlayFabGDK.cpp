#include "TestAppPch.h"
#include "Platform/PlayFabPal.h"
#include "Platform/Generic/MemoryManager.h"
#include "Operations/Core/AuthenticationOperations.h"
#include "Platform/GDK/TitleLocalUser_GDK.h"

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
    return XUser::Add(XUserAddOptions::AddDefaultUserAllowingUI, rc).Then([ sc = ServiceConfig{ serviceConfig }](Result<XUser> result) -> Result<TitleLocalUser>
    {
        RETURN_IF_FAILED(result.hr);

        auto customContext = MakeShared<GDKLocalUserContext>(result.ExtractPayload());

        PFLocalUserHandle handle;
        RETURN_IF_FAILED(PFLocalUserCreateHandleWithXboxUser(sc.Handle(), customContext->xuser.Handle(), customContext.get(), &handle));
        return TitleLocalUser{ LocalUserHandleWrapper::Wrap(handle), std::move(customContext) };
    });
}

// Platform dependent PlayFabServices wrappers
HRESULT ServicesInitialize()
{
    return PFServicesInitialize(nullptr);
}

}
}
}
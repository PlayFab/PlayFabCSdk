#include "TestAppPch.h"
#include "Platform/PlayFabPal.h"
#include "Platform/Generic/MemoryManager.h"
#include "Operations/Core/AuthenticationOperations.h"
#include "GDK/PlatformUser_GDK.h"

using namespace PlayFab::Platform;

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

AsyncOp<UserPtr> GetDefaultPlatformUser(
    RunContext rc
) noexcept
{
    return XUser::Add(XUserAddOptions::AddDefaultUserAllowingUI, rc).Then([](Result<XUser> result) -> Result<UserPtr>
    {
        RETURN_IF_FAILED(result.hr);
        return MakeShared<User>(result.ExtractPayload());
    });
}

class LoginWithXUserOperation : public XAsyncOperation<LoginResult>
{
public:
    using RequestType = Wrappers::PFAuthenticationLoginWithXUserRequestWrapper<Allocator>;

    LoginWithXUserOperation(ServiceConfig serviceConfig, RequestType request, PlayFab::RunContext rc);

private: // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<LoginResult> GetResult(XAsyncBlock* async) noexcept override;

    ServiceConfig m_serviceConfig;
    RequestType m_request;
};

LoginWithXUserOperation::LoginWithXUserOperation(
    ServiceConfig serviceConfig,
    RequestType request,
    PlayFab::RunContext rc
) :
    XAsyncOperation{ std::move(rc) },
    m_serviceConfig{ std::move(serviceConfig) },
    m_request{ std::move(request) }
{
}

HRESULT LoginWithXUserOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAuthenticationLoginWithXUserAsync(m_serviceConfig.Handle(), &m_request.Model(), async);
}

Result<LoginResult> LoginWithXUserOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t bufferSize;
    RETURN_IF_FAILED(PFAuthenticationLoginWithXUserGetResultSize(async, &bufferSize));
    Vector<char> buffer(bufferSize);
    PFEntityHandle entityHandle;
    PFAuthenticationLoginResult const* loginResult;
    RETURN_IF_FAILED(PFAuthenticationLoginWithXUserGetResult(async, &entityHandle, buffer.size(), buffer.data(), &loginResult, nullptr));
    return LoginResult{ Entity::Wrap(entityHandle), *loginResult };
}

AsyncOp<LoginResult> LoginDefaultTitlePlayer(
    ServiceConfig serviceConfig,
    UserPtr platformUser,
    RunContext rc
) noexcept
{
    RETURN_HR_INVALIDARG_IF_NULL(platformUser);
    LoginWithXUserOperation::RequestType loginRequest{ Wrappers::XUser::Duplicate(platformUser->Handle()) };
    loginRequest.SetCreateAccount(true);
    return RunOperation(MakeUnique<LoginWithXUserOperation>(serviceConfig, std::move(loginRequest), rc));
}

// Platform dependent PlayFabServices wrappers
HRESULT ServicesInitialize()
{
    return PFServicesInitialize(nullptr);
}

}
}
}
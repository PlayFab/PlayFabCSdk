#include "TestAppPch.h"
#include "AndroidTestApp.h"
#include <httpClient/httpClient.h>
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
    // TODO: Investigate why this causes the test app to hang
    // RETURN_IF_FAILED(MemoryManager::Instance().SetHooks());
    return S_OK;
}

HRESULT CoreInitialize(XTaskQueueHandle queue) noexcept
{
    return PFInitialize(queue, AndroidTestApp::TestApp::GetInstance().GetJavaVM(), AndroidTestApp::TestApp::GetInstance().GetAppContext());
}

AsyncOp<UserPtr> GetDefaultPlatformUser(
    RunContext rc
) noexcept
{
    UNREFERENCED_PARAMETER(rc);
    return AsyncOp<UserPtr>{ UserPtr{} };
}

class LoginWithAndroidOperation : public XAsyncOperation<LoginResult>
{
public:
    using RequestType = Wrappers::PFAuthenticationLoginWithAndroidDeviceIDRequestWrapper<Allocator>;
    LoginWithAndroidOperation(ServiceConfig serviceConfig, RequestType request, PlayFab::RunContext rc);

private: // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<LoginResult> GetResult(XAsyncBlock* async) noexcept override;
    ServiceConfig m_serviceConfig;
    RequestType m_request;
};

LoginWithAndroidOperation::LoginWithAndroidOperation(
    ServiceConfig serviceConfig,
    RequestType request,
    PlayFab::RunContext rc
) :
    XAsyncOperation{ std::move(rc) },
    m_serviceConfig{ std::move(serviceConfig) },
    m_request{ std::move(request) }
{
}

HRESULT LoginWithAndroidOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAuthenticationLoginWithAndroidDeviceIDAsync(m_serviceConfig.Handle(), &m_request.Model(), async);
}

Result<LoginResult> LoginWithAndroidOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t bufferSize;
    RETURN_IF_FAILED(PFAuthenticationLoginWithAndroidDeviceIDGetResultSize(async, &bufferSize));
    Vector<char> buffer(bufferSize);
    PFEntityHandle entityHandle;
    PFAuthenticationLoginResult const* loginResult;
    RETURN_IF_FAILED(PFAuthenticationLoginWithAndroidDeviceIDGetResult(async, &entityHandle, buffer.size(), buffer.data(), &loginResult, nullptr));
    return LoginResult{ Entity::Wrap(entityHandle), *loginResult };
}

AsyncOp<LoginResult> LoginDefaultTitlePlayer(
    ServiceConfig serviceConfig,
    UserPtr platformUser,
    RunContext rc
) noexcept
{
    assert(!platformUser);
    UNREFERENCED_PARAMETER(platformUser);

    LoginWithAndroidOperation::RequestType request;
    request.SetAndroidDeviceId(AndroidTestApp::TestApp::GetInstance().GetDeviceToken());
    request.SetCreateAccount(true);
    return RunOperation(MakeUnique<LoginWithAndroidOperation>(serviceConfig, request, rc));
}

// Platform dependent PlayFabServices wrappers
HRESULT ServicesInitialize()
{
    HCInitArgs initArgs;
    initArgs.javaVM = AndroidTestApp::TestApp::GetInstance().GetJavaVM();
    initArgs.applicationContext = AndroidTestApp::TestApp::GetInstance().GetAppContext();
    return PFServicesInitialize(nullptr, &initArgs);
}

}
}
}
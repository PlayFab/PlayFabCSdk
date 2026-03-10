#include "TestAppPch.h"
#include "AndroidTestApp.h"
#include <httpClient/httpClient.h>
#include <Memory.h>
#include <playfab/services/PFServices.h>
#include "Platform/PlayFabPal.h"
#include "Platform/Generic/MemoryManager.h"
#include "Platform/Android/TitleLocalUser_Android.h"

namespace PlayFab
{
namespace Test
{
namespace Platform
{

constexpr char defaultPlayerCustomId[] = "TestCustomIdAndroid";

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

AsyncOp<TitleLocalUser> GetDefaultLocalUser(
    ServiceConfig const& serviceConfig,
    RunContext rc
) noexcept
{
    auto playerContext = MakeShared<AndroidLocalUserContext>(defaultPlayerCustomId);
    PFLocalUserHandle handle;
    RETURN_IF_FAILED(PFLocalUserCreateHandleWithPersistedLocalId(serviceConfig.Handle(), playerContext->customId.data(), AndroidLocalUserContext::LocalUserLoginHandler, playerContext.get(), &handle));
    return AsyncOp<TitleLocalUser>{ TitleLocalUser{ LocalUserHandleWrapper::Wrap(handle), std::move(playerContext) } };
}

class LoginWithGooglePlayGamesServicesOperation : public XAsyncOperation<LoginResult>
{
public:
    using RequestType = Wrappers::PFAuthenticationLoginWithGooglePlayGamesServicesRequestWrapper<Allocator>;
    LoginWithGooglePlayGamesServicesOperation(ServiceConfig serviceConfig, RequestType request, PlayFab::RunContext rc);

private: // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<LoginResult> GetResult(XAsyncBlock* async) noexcept override;
    ServiceConfig m_serviceConfig;
    RequestType m_request;
};

LoginWithGooglePlayGamesServicesOperation::LoginWithGooglePlayGamesServicesOperation(
    ServiceConfig serviceConfig,
    RequestType request,
    PlayFab::RunContext rc
) :
    XAsyncOperation{ std::move(rc) },
    m_serviceConfig{ std::move(serviceConfig) },
    m_request{ std::move(request) }
{
}

HRESULT LoginWithGooglePlayGamesServicesOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAuthenticationLoginWithGooglePlayGamesServicesAsync(m_serviceConfig.Handle(), &m_request.Model(), async);
}

Result<LoginResult> LoginWithGooglePlayGamesServicesOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t bufferSize;
    RETURN_IF_FAILED(PFAuthenticationLoginWithGooglePlayGamesServicesGetResultSize(async, &bufferSize));
    Vector<char> buffer(bufferSize);
    PFEntityHandle entityHandle;
    PFAuthenticationLoginResult const* loginResult;
    RETURN_IF_FAILED(PFAuthenticationLoginWithGooglePlayGamesServicesGetResult(async, &entityHandle, buffer.size(), buffer.data(), &loginResult, nullptr));
    return LoginResult{ Entity::Wrap(entityHandle), *loginResult };
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
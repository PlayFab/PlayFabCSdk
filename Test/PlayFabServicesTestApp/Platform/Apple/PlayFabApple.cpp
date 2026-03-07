#include "TestAppPch.h"
#include <Memory.h>
#include <playfab/services/PFServices.h>
#include "Platform/PlayFabPal.h"
#include "Platform/Generic/MemoryManager.h"
#include "Operations/Core/AuthenticationOperations.h"
#include "Platform/Apple/TitleLocalUser_Apple.h"
#include "PlayFabApple.h"

namespace PlayFab
{
namespace Test
{
namespace Platform
{

constexpr char defaultPlayerCustomId[] = "TestCustomIdApple";

HRESULT SetHooks()
{
    // TODO: Investigate why setting platform hooks causes execution flow never return to Swift in Apple Platforms
    // RETURN_IF_FAILED(MemoryManager::Instance().SetHooks());
    return S_OK;
}

HRESULT CoreInitialize(XTaskQueueHandle queue) noexcept
{
    return PFInitialize(queue);
}

AsyncOp<TitleLocalUser> GetDefaultLocalUser(ServiceConfig const& serviceConfig, RunContext rc) noexcept
{
    auto playerContext = MakeShared<AppleLocalUserContext>(defaultPlayerCustomId);

    PFLocalUserHandle handle;
    RETURN_IF_FAILED(PFLocalUserCreateHandleWithPersistedLocalId(serviceConfig.Handle(), playerContext->customId.data(), AppleLocalUserContext::LocalUserLoginHandler, playerContext.get(), &handle));
    return AsyncOp<TitleLocalUser>{ TitleLocalUser{ LocalUserHandleWrapper::Wrap(handle), std::move(playerContext) } };
}

LoginWithAppleOperation::LoginWithAppleOperation(ServiceConfig serviceConfig, RequestType request, PlayFab::RunContext rc) :
    XAsyncOperation{ std::move(rc) },
    m_serviceConfig{ std::move(serviceConfig) },
    m_request{ std::move(request) }
{
}

HRESULT LoginWithAppleOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAuthenticationLoginWithAppleAsync(m_serviceConfig.Handle(), &m_request.Model(), async);
}

Result<LoginResult> LoginWithAppleOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t bufferSize;
    RETURN_IF_FAILED(PFAuthenticationLoginWithAppleGetResultSize(async, &bufferSize));
    Vector<char> buffer(bufferSize);
    PFEntityHandle entityHandle;
    PFAuthenticationLoginResult const* loginResult;
    RETURN_IF_FAILED(PFAuthenticationLoginWithAppleGetResult(async, &entityHandle, buffer.size(), buffer.data(), &loginResult, nullptr));
    return LoginResult{ Entity::Wrap(entityHandle), *loginResult };
}

// Platform dependent PlayFabServices wrappers
HRESULT ServicesInitialize()
{
    return PFServicesInitialize(nullptr);
}

}
}
}

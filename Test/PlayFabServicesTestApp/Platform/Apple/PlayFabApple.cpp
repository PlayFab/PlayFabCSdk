#include "TestAppPch.h"
#include <Memory.h>
#include <playfab/services/PFServices.h>
#include "Platform/PlayFabPal.h"
#include "Platform/Generic/MemoryManager.h"
#include "Operations/Core/AuthenticationOperations.h"
#include "PlatformUser_Apple.h"

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
    // TODO: Investigate why setting platform hooks causes execution flow never return to Swift in Apple Platforms
    // RETURN_IF_FAILED(MemoryManager::Instance().SetHooks());
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

class LoginWithAppleOperation : public XAsyncOperation<LoginResult>
{
public:
    
    using RequestType = Wrappers::PFAuthenticationLoginWithAppleRequestWrapper<Allocator>;

    LoginWithAppleOperation(ServiceConfig serviceConfig, RequestType request, PlayFab::RunContext rc);

private: // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<LoginResult> GetResult(XAsyncBlock* async) noexcept override;

    ServiceConfig m_serviceConfig;
    RequestType m_request;
};

LoginWithAppleOperation::LoginWithAppleOperation(
    ServiceConfig serviceConfig,
    RequestType request,
    PlayFab::RunContext rc
) :
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

AsyncOp<LoginResult> LoginDefaultTitlePlayer(
    ServiceConfig serviceConfig,
    UserPtr platformUser,
    RunContext rc
) noexcept
{
    assert(!platformUser);
    UNREFERENCED_PARAMETER(platformUser);

    return platformUser->GetUserIdentityToken(rc.Derive()).Then([serviceConfig, rc](Result<String> result) -> AsyncOp<LoginResult>
    {
        if (result.ExtractPayload() != "")
        {
            RETURN_IF_FAILED_PLAYFAB(result);
            LoginWithAppleOperation::RequestType request;
            request.SetIdentityToken(result.ExtractPayload());
            request.SetCreateAccount(true);
            return RunOperation(MakeUnique<LoginWithAppleOperation>(serviceConfig, request, rc));
        }
        else
        {
            // If we weren't able to get an Apple User, fallback to LoginWithCustomId
            LoginWithCustomIDOperation::RequestType request;
            request.SetCustomId(defaultPlayerCustomId);
            request.SetCreateAccount(true);
            return RunOperation(MakeUnique<LoginWithCustomIDOperation>(serviceConfig, request, rc));
        }
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

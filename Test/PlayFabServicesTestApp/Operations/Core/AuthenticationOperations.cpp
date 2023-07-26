#include "TestAppPch.h"
#include "AuthenticationOperations.h"

namespace PlayFab
{
namespace Test
{

LoginWithCustomIDOperation::LoginWithCustomIDOperation(
    ServiceConfig serviceConfig,
    RequestType request,
    PlayFab::RunContext rc
) :
    XAsyncOperation{ std::move(rc) },
    m_serviceConfig{ std::move(serviceConfig) },
    m_request{ std::move(request) }
{
}

HRESULT LoginWithCustomIDOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAuthenticationLoginWithCustomIDAsync(m_serviceConfig.Handle(), &m_request.Model(), async);
}

Result<LoginResult> LoginWithCustomIDOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t bufferSize;
    RETURN_IF_FAILED(PFAuthenticationLoginWithCustomIDGetResultSize(async, &bufferSize));
    Vector<char> buffer(bufferSize);
    PFEntityHandle entityHandle;
    PFAuthenticationLoginResult const* loginResult;
    RETURN_IF_FAILED(PFAuthenticationLoginWithCustomIDGetResult(async, &entityHandle, buffer.size(), buffer.data(), &loginResult, nullptr));
    return LoginResult{ Entity::Wrap(entityHandle), *loginResult };
}

#if HC_PLATFORM == HC_PLATFORM_WIN32
GetEntityWithSecretKeyOperation::GetEntityWithSecretKeyOperation(
    ServiceConfig serviceConfig,
    String secretKey,
    RequestType request,
    PlayFab::RunContext rc
) :
    XAsyncOperation{ std::move(rc) },
    m_serviceConfig{ std::move(serviceConfig) },
    m_secretKey{ std::move(secretKey) },
    m_request{ std::move(request) }
{
}

HRESULT GetEntityWithSecretKeyOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAuthenticationGetEntityWithSecretKeyAsync(m_serviceConfig.Handle(), m_secretKey.data(), &m_request.Model(), async);
}

Result<Entity> GetEntityWithSecretKeyOperation::GetResult(XAsyncBlock* async) noexcept
{
    PFEntityHandle entityHandle;
    RETURN_IF_FAILED(PFAuthenticationGetEntityWithSecretKeyGetResult(async, &entityHandle));
    return Entity::Wrap(entityHandle);
}

#if 0 // removed for now
ServerLoginWithServerCustomIdOperation::ServerLoginWithServerCustomIdOperation(
    ServiceConfig serviceConfig,
    String secretKey,
    RequestType request,
    PlayFab::RunContext rc
) :
    XAsyncOperation{ std::move(rc) },
    m_serviceConfig{ std::move(serviceConfig) },
    m_secretKey{ std::move(secretKey) },
    m_request{ std::move(request) }
{
}

HRESULT ServerLoginWithServerCustomIdOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAuthenticationServerLoginWithServerCustomIdAsync(m_serviceConfig.Handle(), m_secretKey.c_str(), &m_request.Model(), async);
}

Result<ServerLoginResult> ServerLoginWithServerCustomIdOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t bufferSize;
    RETURN_IF_FAILED(PFAuthenticationServerLoginWithServerCustomIdGetResultSize(async, &bufferSize));
    Vector<char> buffer(bufferSize);
    PFAuthenticationEntityTokenResponse const* entityTokenResponse;
    PFAuthenticationLoginResult const* loginResult;
    RETURN_IF_FAILED(PFAuthenticationServerLoginWithServerCustomIdGetResult(async, &entityTokenResponse, buffer.size(), buffer.data(), &loginResult, nullptr));
    return ServerLoginResult{ *entityTokenResponse, *loginResult };
}

ServerLoginWithSteamIdOperation::ServerLoginWithSteamIdOperation(
    ServiceConfig serviceConfig,
    String secretKey,
    RequestType request,
    PlayFab::RunContext rc
) :
    XAsyncOperation{ std::move(rc) },
    m_serviceConfig{ std::move(serviceConfig) },
    m_secretKey{ std::move(secretKey) },
    m_request{ std::move(request) }
{
}

HRESULT ServerLoginWithSteamIdOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAuthenticationServerLoginWithSteamIdAsync(m_serviceConfig.Handle(), m_secretKey.c_str(), &m_request.Model(), async);
}

Result<ServerLoginResult> ServerLoginWithSteamIdOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t bufferSize;
    RETURN_IF_FAILED(PFAuthenticationServerLoginWithSteamIdGetResultSize(async, &bufferSize));
    Vector<char> buffer(bufferSize);
    PFAuthenticationEntityTokenResponse const* entityTokenResponse;
    PFAuthenticationLoginResult const* loginResult;
    RETURN_IF_FAILED(PFAuthenticationServerLoginWithSteamIdGetResult(async, &entityTokenResponse, buffer.size(), buffer.data(), &loginResult, nullptr));
    return ServerLoginResult{ *entityTokenResponse, *loginResult };
}

ServerLoginWithXboxOperation::ServerLoginWithXboxOperation(
    ServiceConfig serviceConfig,
    String secretKey,
    RequestType request,
    PlayFab::RunContext rc
) :
    XAsyncOperation{ std::move(rc) },
    m_serviceConfig{ std::move(serviceConfig) },
    m_secretKey{ std::move(secretKey) },
    m_request{ std::move(request) }
{
}

HRESULT ServerLoginWithXboxOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAuthenticationServerLoginWithXboxAsync(m_serviceConfig.Handle(), m_secretKey.c_str(), &m_request.Model(), async);
}

Result<ServerLoginResult> ServerLoginWithXboxOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t bufferSize;
    RETURN_IF_FAILED(PFAuthenticationServerLoginWithXboxGetResultSize(async, &bufferSize));
    Vector<char> buffer(bufferSize);
    PFAuthenticationEntityTokenResponse const* entityTokenResponse;
    PFAuthenticationLoginResult const* loginResult;
    RETURN_IF_FAILED(PFAuthenticationServerLoginWithXboxGetResult(async, &entityTokenResponse, buffer.size(), buffer.data(), &loginResult, nullptr));
    return ServerLoginResult{ *entityTokenResponse, *loginResult };
}

ServerLoginWithXboxIdOperation::ServerLoginWithXboxIdOperation(
    ServiceConfig serviceConfig,
    String secretKey,
    RequestType request,
    PlayFab::RunContext rc
) :
    XAsyncOperation{ std::move(rc) },
    m_serviceConfig{ std::move(serviceConfig) },
    m_secretKey{ std::move(secretKey) },
    m_request{ std::move(request) }
{
}

HRESULT ServerLoginWithXboxIdOperation::OnStarted(XAsyncBlock* async) noexcept
{
    return PFAuthenticationServerLoginWithXboxIdAsync(m_serviceConfig.Handle(), m_secretKey.c_str(), &m_request.Model(), async);
}

Result<ServerLoginResult> ServerLoginWithXboxIdOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t bufferSize;
    RETURN_IF_FAILED(PFAuthenticationServerLoginWithXboxIdGetResultSize(async, &bufferSize));
    Vector<char> buffer(bufferSize);
    PFAuthenticationEntityTokenResponse const* entityTokenResponse;
    PFAuthenticationLoginResult const* loginResult;
    RETURN_IF_FAILED(PFAuthenticationServerLoginWithXboxIdGetResult(async, &entityTokenResponse, buffer.size(), buffer.data(), &loginResult, nullptr));
    return ServerLoginResult{ *entityTokenResponse, *loginResult };
}
#endif // 0
#endif // HC_PLATFORM == HC_PLATFORM_WIN32

}
}

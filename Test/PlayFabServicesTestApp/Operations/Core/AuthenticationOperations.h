#pragma once

#include <playfab/core/cpp/AuthenticationTypeWrappers.h>

namespace PlayFab
{
namespace Test
{

class LoginWithCustomIDOperation : public XAsyncOperation<LoginResult>
{
public:
    using RequestType = Wrappers::PFAuthenticationLoginWithCustomIDRequestWrapper<Allocator>;

    LoginWithCustomIDOperation(ServiceConfig serviceConfig, RequestType request, PlayFab::RunContext rc);

private: // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<LoginResult> GetResult(XAsyncBlock* async) noexcept override;

    ServiceConfig m_serviceConfig;
    RequestType m_request;
};

#if HC_PLATFORM == HC_PLATFORM_WIN32
struct ServerLoginResult
{
    Wrappers::PFAuthenticationEntityTokenResponseWrapper<Allocator> entityTokenResponse;
    Wrappers::PFAuthenticationLoginResultWrapper<Allocator> loginResult;
};

class GetEntityWithSecretKeyOperation : public XAsyncOperation<Entity>
{
public:
    using RequestType = Wrappers::PFAuthenticationGetEntityRequestWrapper<Allocator>;

    GetEntityWithSecretKeyOperation(ServiceConfig serviceConfig, String secretKey, RequestType request, PlayFab::RunContext rc);

private: // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<Entity> GetResult(XAsyncBlock* async) noexcept override;

    ServiceConfig m_serviceConfig;
    String m_secretKey;
    RequestType m_request;
};

#if 0 // removed for now
class ServerLoginWithServerCustomIdOperation : public XAsyncOperation<ServerLoginResult>
{
public:
    using RequestType = Wrappers::PFAuthenticationLoginWithServerCustomIdRequestWrapper<Allocator>;

    ServerLoginWithServerCustomIdOperation(ServiceConfig serviceConfig, String secretKey, RequestType request, PlayFab::RunContext rc);

private: // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ServerLoginResult> GetResult(XAsyncBlock* async) noexcept override;

    ServiceConfig m_serviceConfig;
    String m_secretKey;
    RequestType m_request;
};

class ServerLoginWithSteamIdOperation : public XAsyncOperation<ServerLoginResult>
{
public:
    using RequestType = Wrappers::PFAuthenticationLoginWithSteamIdRequestWrapper<Allocator>;

    ServerLoginWithSteamIdOperation(ServiceConfig serviceConfig, String secretKey, RequestType request, PlayFab::RunContext rc);

private: // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ServerLoginResult> GetResult(XAsyncBlock* async) noexcept override;

    ServiceConfig m_serviceConfig;
    String m_secretKey;
    RequestType m_request;
};

class ServerLoginWithXboxOperation : public XAsyncOperation<ServerLoginResult>
{
public:
    using RequestType = Wrappers::PFAuthenticationServerLoginWithXboxRequestWrapper<Allocator>;

    ServerLoginWithXboxOperation(ServiceConfig serviceConfig, String secretKey, RequestType request, PlayFab::RunContext rc);

private: // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ServerLoginResult> GetResult(XAsyncBlock* async) noexcept override;

    ServiceConfig m_serviceConfig;
    String m_secretKey;
    RequestType m_request;
};

class ServerLoginWithXboxIdOperation : public XAsyncOperation<ServerLoginResult>
{
public:
    using RequestType = Wrappers::PFAuthenticationLoginWithXboxIdRequestWrapper<Allocator>;

    ServerLoginWithXboxIdOperation(ServiceConfig serviceConfig, String secretKey, RequestType request, PlayFab::RunContext rc);

private: // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<ServerLoginResult> GetResult(XAsyncBlock* async) noexcept override;

    ServiceConfig m_serviceConfig;
    String m_secretKey;
    RequestType m_request;
};
#endif // 0
#endif // HC_PLATFORM == HC_PLATFORM_WIN32

}
}

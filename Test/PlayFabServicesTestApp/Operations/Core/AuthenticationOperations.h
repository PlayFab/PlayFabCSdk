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

class AuthenticateGameServerWithCustomIdOperation : public XAsyncOperation<Wrappers::PFAuthenticationAuthenticateCustomIdResultWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAuthenticationAuthenticateCustomIdRequestWrapper<Allocator>;

    AuthenticateGameServerWithCustomIdOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

private: // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<Wrappers::PFAuthenticationAuthenticateCustomIdResultWrapper<Allocator>> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class DeleteOperation : public XAsyncOperation<void>
{
public:
    using RequestType = Wrappers::PFAuthenticationDeleteRequestWrapper<Allocator>;

    DeleteOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

private: // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};

class GetEntityOperation : public XAsyncOperation<Entity>
{
public:
    using RequestType = Wrappers::PFAuthenticationGetEntityRequestWrapper<Allocator>;

    GetEntityOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

private: // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<Entity> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
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

class ValidateEntityTokenOperation : public XAsyncOperation<Wrappers::PFAuthenticationValidateEntityTokenResponseWrapper<Allocator>>
{
public:
    using RequestType = Wrappers::PFAuthenticationValidateEntityTokenRequestWrapper<Allocator>;

    ValidateEntityTokenOperation(Entity entity, RequestType request, PlayFab::RunContext rc);

private: // XAsyncOperation
    HRESULT OnStarted(XAsyncBlock* async) noexcept override;
    Result<Wrappers::PFAuthenticationValidateEntityTokenResponseWrapper<Allocator>> GetResult(XAsyncBlock* async) noexcept override;

    Entity m_entity;
    RequestType m_request;
};
#endif // HC_PLATFORM == HC_PLATFORM_WIN32

}
}

#pragma once

#include "Entity.h"
#include "Authentication/LoginHandler.h"
#include <playfab/core/PFLocalUser.h>

namespace PlayFab
{

class LocalUser : public std::enable_shared_from_this<LocalUser>
{
public:
    LocalUser(SharedPtr<ServiceConfig> serviceConfig, void* customContext);
    virtual ~LocalUser() = default;

    virtual String LocalId() const = 0;
    void* CustomContext() const;
    SharedPtr<Entity> Entity() const;
    SharedPtr<ServiceConfig> GetServiceConfig() const;

    AsyncOp<Authentication::CombinedLoginResult> LoginOrGetCachedLoginResult(
        SharedPtr<PFCoreGlobalState> state,
        bool createAccount,
        RunContext rc
    ) noexcept;

    virtual SharedPtr<Authentication::ILoginHandler> LoginHandler() const noexcept = 0;

private:
    mutable std::mutex m_mutex;
    SharedPtr<ServiceConfig> m_serviceConfig;
    void* const m_customContext;
    std::optional<Authentication::CombinedLoginResult> m_loginResult;
    struct ClientLoginContext
    {
        SharedPtr<AsyncOpContext<Authentication::CombinedLoginResult>> asyncContext;
        RunContext runContext;
    };
    Vector<ClientLoginContext> m_clientLoginContexts;
};

class CustomLocalUser : public LocalUser
{
public:
    static Result<SharedPtr<CustomLocalUser>> Make(
        SharedPtr<ServiceConfig> serviceConfig,
        void* customContext,
        String localId,
        PFCoreGlobalState& state,
        PFLocalUserLoginHandler& loginHandler
    ) noexcept;

    CustomLocalUser(CustomLocalUser const&) = delete;
    CustomLocalUser& operator=(CustomLocalUser&) = delete;
    ~CustomLocalUser() = default;

    String LocalId() const override;

    SharedPtr<Authentication::ILoginHandler> LoginHandler() const noexcept override;

private:
    CustomLocalUser(
        SharedPtr<ServiceConfig> serviceConfig,
        void* customContext,
        String localId
    );

    String const m_localId;
    SharedPtr<struct CustomLocalUserLoginHandler> m_loginHandler;
};

}
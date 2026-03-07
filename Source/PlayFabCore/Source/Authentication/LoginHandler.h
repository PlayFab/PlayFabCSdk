#pragma once

#include "CombinedLoginResult.h"

namespace PlayFab
{
namespace Authentication
{

struct ILoginHandler
{
    virtual ~ILoginHandler() = default;

    virtual AsyncOp<CombinedLoginResult> Login(
        SharedPtr<PFCoreGlobalState> state,
        SharedPtr<ServiceConfig> serviceConfig,
        bool createAccount,
        RunContext rc
    ) noexcept = 0;

    virtual AsyncOp<void> ReLogin(
        SharedPtr<Entity> entity,
        RunContext rc
    ) noexcept = 0;

    // static helper functions to for XAsyncProviders to bind to
    static AsyncOp<CombinedLoginResult> LoginHelper(
        SharedPtr<ILoginHandler> loginHandler,
        SharedPtr<PFCoreGlobalState> state,
        SharedPtr<ServiceConfig> serviceConfig,
        RunContext rc
    ) noexcept;

    static AsyncOp<void> ReLoginHelper(
        SharedPtr<ILoginHandler> loginHandler,
        SharedPtr<Entity> entity,
        RunContext rc
    ) noexcept;
};

class LoginWithRequestHandler : public ILoginHandler, public std::enable_shared_from_this<LoginWithRequestHandler>
{
public:
    LoginWithRequestHandler(
        const char* path,
        JsonValue&& requestBody,
        CacheId retryCacheId
    ) noexcept;

public: // ILoginHandler
    AsyncOp<CombinedLoginResult> Login(
        SharedPtr<PFCoreGlobalState> state,
        SharedPtr<ServiceConfig> serviceConfig,
        bool createAccount,
        RunContext rc
    ) noexcept override;

    AsyncOp<void> ReLogin(
        SharedPtr<Entity> entity,
        RunContext rc
    ) noexcept override;

protected:
    AsyncOp<ServiceResponse> MakeLoginRequest(
        SharedPtr<ServiceConfig const> serviceConfig,
        RunContext rc
    ) noexcept;

    String m_path;
    JsonValue m_requestBody;
    CacheId m_cacheId;
};

}
}

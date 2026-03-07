#include <stdafx.h>
#include "LocalUser.h"
#include "XAsyncOperation.h"
#include <playfab/core/PFAuthentication.h>

namespace PlayFab
{

LocalUser::LocalUser(SharedPtr<ServiceConfig> serviceConfig, void* customContext)
    : m_serviceConfig{ std::move(serviceConfig) },
    m_customContext{ customContext }
{
}

void* LocalUser::CustomContext() const
{
    return m_customContext;
}

SharedPtr<Entity> LocalUser::Entity() const
{
    std::unique_lock<std::mutex> lock{ m_mutex };
    return m_loginResult.has_value() ? m_loginResult->entity : nullptr;
}

SharedPtr<ServiceConfig> LocalUser::GetServiceConfig() const
{
    return m_serviceConfig;
}

AsyncOp<Authentication::CombinedLoginResult> LocalUser::LoginOrGetCachedLoginResult(
    SharedPtr<PFCoreGlobalState> state,
    bool createAccount,
    RunContext rc
) noexcept
{
    bool beginLogin{ false };

    std::unique_lock<std::mutex> lock{ m_mutex };
    if (m_loginResult.has_value())
    {
        // If this user has already logged in, simply return the cached login result.
        return Result<Authentication::CombinedLoginResult>{ Authentication::CombinedLoginResult{ *m_loginResult } };
    }
    else if (m_clientLoginContexts.empty())
    {
        // If there isn't a pending login for this user, begin login now
        beginLogin = true;
    }

    auto asyncContext = MakeShared<AsyncOpContext<Authentication::CombinedLoginResult>>();
    m_clientLoginContexts.emplace_back(ClientLoginContext{ asyncContext, rc });
    lock.unlock();

    if (beginLogin)
    {
        LoginHandler()->Login(std::move(state), m_serviceConfig, createAccount, rc).Finally([sharedThis = shared_from_this()](Result<Authentication::CombinedLoginResult> result)
        {
            std::unique_lock<std::mutex> lock{ sharedThis->m_mutex };
            auto clientLoginContexts = std::move(sharedThis->m_clientLoginContexts);
            if (SUCCEEDED(result.hr))
            {
                sharedThis->m_loginResult.emplace(result.Payload());
            }
            lock.unlock();

            for (auto& clientContext : clientLoginContexts)
            {
                clientContext.runContext.TaskQueueSubmitCompletion([result, asyncContext = clientContext.asyncContext]() mutable
                {
                    asyncContext->Complete(std::move(result));
                });
            }
        });
    }

    return asyncContext;
}

// The client callback effectively wraps an Auth XAsync API, so we can leverage our XAsyncOperation class
class CustomLocalUserLoginOperation : public XAsyncOperation<Authentication::CombinedLoginResult>
{
public:
    CustomLocalUserLoginOperation(
        SharedPtr<CustomLocalUserLoginHandler> handler,
        SharedPtr<PFCoreGlobalState> state,
        PlayFab::RunContext rc
    );

    ~CustomLocalUserLoginOperation();

private:
    HRESULT OnStarted(XAsyncBlock* async) override;
    Result<Authentication::CombinedLoginResult> GetResult(XAsyncBlock* async) noexcept override;

    SharedPtr<CustomLocalUserLoginHandler> m_handler;
    SharedPtr<PFCoreGlobalState> m_state;

    // Handles to pass to client callback. Only valid for the duration of the login operation
    PFLocalUserHandle m_userHandle{ nullptr };
    PFEntityHandle m_entityHandle{ nullptr };
};

class CustomLocalUserReloginOperation : public XAsyncOperation<void>
{
public:
    CustomLocalUserReloginOperation(
        SharedPtr<CustomLocalUserLoginHandler> handler,
        SharedPtr<PlayFab::Entity> entity,
        PlayFab::RunContext rc
    );

    ~CustomLocalUserReloginOperation();

private:
    HRESULT OnStarted(XAsyncBlock* async) override;

    SharedPtr<CustomLocalUserLoginHandler> m_handler;
    SharedPtr<Entity> m_entity;

    // Handles to pass to client callback. Only valid for the duration of the login operation
    PFLocalUserHandle m_userHandle{ nullptr };
    PFEntityHandle m_entityHandle{ nullptr };
};

// ILoginHandler for a CustomLocalUser
struct CustomLocalUserLoginHandler : public Authentication::ILoginHandler, public std::enable_shared_from_this<CustomLocalUserLoginHandler>
{
    CustomLocalUserLoginHandler(
        SharedPtr<CustomLocalUser> _localUser,
        PFLocalUserLoginHandler& _clientLoginHandler,
        PFCoreGlobalState& _state
    );

    ~CustomLocalUserLoginHandler();

    AsyncOp<Authentication::CombinedLoginResult> Login(
        SharedPtr<PFCoreGlobalState> state,
        SharedPtr<ServiceConfig> serviceConfig,
        bool createAccount,
        RunContext rc
    ) noexcept override;

    AsyncOp<void> ReLogin(
        SharedPtr<PlayFab::Entity> entity,
        RunContext rc
    ) noexcept override;

    // Login Handler holds a Weak reference to the LocalUser to prevent the LocalUser and the associated Entity from keeping each 
    // other alive forever.
    WeakPtr<CustomLocalUser> localUser;
    PFLocalUserLoginHandler& clientLoginHandler;

    // Reference to global state so that we can create handles to pass to client handler
    PFCoreGlobalState& state;

    // ServiceConfig handle to pass to clientLoginHandler
    PFServiceConfigHandle serviceConfigHandle{ nullptr };
};


Result<SharedPtr<CustomLocalUser>> CustomLocalUser::Make(
    SharedPtr<ServiceConfig> serviceConfig,
    void* customContext,
    String localId,
    PFCoreGlobalState& state,
    PFLocalUserLoginHandler& loginHandler
) noexcept
{
    Allocator<CustomLocalUser> a;
    SharedPtr<CustomLocalUser> user{ new (a.allocate(1)) CustomLocalUser{ serviceConfig, customContext, std::move(localId) }, Deleter<CustomLocalUser>{}, a };

    user->m_loginHandler = MakeShared<CustomLocalUserLoginHandler>(user, loginHandler, state);
    RETURN_IF_FAILED(state.ServiceConfigs().MakeHandle(serviceConfig, user->m_loginHandler->serviceConfigHandle));

    return user;
}

CustomLocalUser::CustomLocalUser(
    SharedPtr<ServiceConfig> serviceConfig,
    void* customContext,
    String localId
) :
    LocalUser{ std::move(serviceConfig), customContext },
    m_localId{ std::move(localId) }
{
}

String CustomLocalUser::LocalId() const
{
    return m_localId;
}

SharedPtr<Authentication::ILoginHandler> CustomLocalUser::LoginHandler() const noexcept
{
    return m_loginHandler;
}

CustomLocalUserLoginOperation::CustomLocalUserLoginOperation(
    SharedPtr<CustomLocalUserLoginHandler> handler,
    SharedPtr<PFCoreGlobalState> state,
    PlayFab::RunContext rc
) :
    XAsyncOperation<Authentication::CombinedLoginResult>{ std::move(rc) },
    m_handler{ std::move(handler) },
    m_state{ std::move(state) }
{
}

CustomLocalUserLoginOperation::~CustomLocalUserLoginOperation()
{
    m_state->LocalUsers().CloseHandle(m_userHandle);
    m_state->Entities().CloseHandle(m_entityHandle);
}

HRESULT CustomLocalUserLoginOperation::OnStarted(XAsyncBlock* async)
{
    SharedPtr<LocalUser> localUser = m_handler->localUser.lock();
    if (!localUser)
    {
        TRACE_INFORMATION("Unable to relogin because the LocalUser was destroyed");
        return E_FAIL;
    }

    RETURN_IF_FAILED(m_handler->state.LocalUsers().MakeHandle(localUser, m_userHandle));

    return m_handler->clientLoginHandler(m_userHandle, m_handler->serviceConfigHandle, nullptr, async);
}

Result<Authentication::CombinedLoginResult> CustomLocalUserLoginOperation::GetResult(XAsyncBlock* async) noexcept
{
    size_t bufferSize;
    RETURN_IF_FAILED(PFAuthenticationLoginWithCustomIDGetResultSize(async, &bufferSize));
    Vector<uint8_t> buffer(bufferSize, 0);

    PFAuthenticationLoginResult const* result;
    RETURN_IF_FAILED(PFAuthenticationLoginWithCustomIDGetResult(async, &m_entityHandle, buffer.size(), buffer.data(), &result, nullptr));

    SharedPtr<Entity> entity;
    RETURN_IF_FAILED(m_state->Entities().FromHandle(m_entityHandle, entity));
    RETURN_IF_FAILED(entity->OnLoginContextUpdated(m_handler));

    return Authentication::CombinedLoginResult{ m_state, Authentication::LoginResult{ *result }, std::move(entity) };
}

CustomLocalUserReloginOperation::CustomLocalUserReloginOperation(
    SharedPtr<CustomLocalUserLoginHandler> handler,
    SharedPtr<PlayFab::Entity> entity,
    PlayFab::RunContext rc
) :
    XAsyncOperation<void>{ std::move(rc) },
    m_handler{ std::move(handler) },
    m_entity{ std::move(entity) }
{
}

CustomLocalUserReloginOperation::~CustomLocalUserReloginOperation()
{
    m_handler->state.LocalUsers().CloseHandle(m_userHandle);
    m_handler->state.Entities().CloseHandle(m_entityHandle);
}

HRESULT CustomLocalUserReloginOperation::OnStarted(XAsyncBlock* async)
{
    SharedPtr<LocalUser> localUser = m_handler->localUser.lock();
    if (!localUser)
    {
        TRACE_INFORMATION("Unable to relogin because the LocalUser was destroyed");
        return E_FAIL;
    }

    RETURN_IF_FAILED(m_handler->state.LocalUsers().MakeHandle(localUser, m_userHandle));
    RETURN_IF_FAILED(m_handler->state.Entities().MakeHandle(m_entity, m_entityHandle));

    return m_handler->clientLoginHandler(m_userHandle, m_handler->serviceConfigHandle, m_entityHandle, async);
}

CustomLocalUserLoginHandler::CustomLocalUserLoginHandler(
    SharedPtr<CustomLocalUser> _localUser,
    PFLocalUserLoginHandler& _clientLoginHandler,
    PFCoreGlobalState& _state
) :
    localUser{ std::move(_localUser) },
    clientLoginHandler{ _clientLoginHandler },
    state{ _state }
{
}

CustomLocalUserLoginHandler::~CustomLocalUserLoginHandler()
{
    state.ServiceConfigs().CloseHandle(serviceConfigHandle);
}

AsyncOp<Authentication::CombinedLoginResult> CustomLocalUserLoginHandler::Login(
    SharedPtr<PFCoreGlobalState> sharedState,
    SharedPtr<ServiceConfig> /*serviceConfig*/,
    bool /*createAccount*/,
    RunContext rc
) noexcept
{
    auto loginOperation = MakeUnique<CustomLocalUserLoginOperation>(
        shared_from_this(),
        std::move(sharedState),
        std::move(rc)
    );

    return RunOperation(std::move(loginOperation));
}

AsyncOp<void> CustomLocalUserLoginHandler::ReLogin(
    SharedPtr<PlayFab::Entity> entity,
    RunContext rc
) noexcept
{
    auto loginOperation = MakeUnique<CustomLocalUserReloginOperation>(
        shared_from_this(),
        std::move(entity),
        std::move(rc)
    );

    return RunOperation(std::move(loginOperation));
}

} // PlayFab

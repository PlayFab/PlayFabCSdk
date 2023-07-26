#include "stdafx.h"
#include "Entity.h"
#include "Generated/Authentication.h"
#include "JsonUtils.h"

#if HC_PLATFORM == HC_PLATFORM_GDK
#include <XGameRuntimeFeature.h>
#include <XNetworking.h>
#include <appnotify.h>
#endif

namespace PlayFab
{

class TokenRefreshWorker : public ITaskQueueWork, public std::enable_shared_from_this<TokenRefreshWorker>
{
public:
    static SharedPtr<TokenRefreshWorker> MakeAndStart(
        SharedPtr<Entity> const& entity,
        SharedPtr<Authentication::LoginContext> loginContext,
        TokenExpiredHandler tokenExpiredHandler,
        RunContext&& rc
    ) noexcept;

    ~TokenRefreshWorker();

    HRESULT OnLoginContextUpdated(SharedPtr<Authentication::LoginContext> loginContext);

    static void SetInterval(uint32_t interval) { s_interval = interval; }
#if _DEBUG
    static void SetDebugForceExpireToken(bool expire) { s_debugForceExpireToken = expire; }
#endif

private:
    TokenRefreshWorker(SharedPtr<Entity> const& entity, SharedPtr<Authentication::LoginContext> loginContext, TokenExpiredHandler tokenExpiredHandler, RunContext&& rc);

    // ITaskQueueWork
    void Run() override;

    // Check validity of token and refresh if needed. Handles all errors and invokes TokenExpiredHandler as needed
    void CheckAndRefreshToken(SharedPtr<Entity> entity) noexcept;

    static bool CheckRefreshRequired(EntityToken const& token) noexcept;

    std::mutex m_mutex;
    WeakPtr<Entity> m_weakEntity;
    SharedPtr<Authentication::LoginContext> m_loginContext;
    TokenExpiredHandler m_tokenExpiredHandler;
    PlayFab::RunContext m_rc;

#if HC_PLATFORM == HC_PLATFORM_GDK
    static void CALLBACK NetworkConnectivityChangedCallback(void* context, const XNetworkingConnectivityHint* hint);
    static void CALLBACK AppStateChangedCallback(BOOLEAN isSuspended, void* context);

    bool m_resumeTokenCheckPending{ false };
    bool m_isSuspended{ false };

    XTaskQueueRegistrationToken m_networkConnectivityChangedToken{};
    PAPPSTATE_REGISTRATION m_appStateChangedToken{ nullptr };
#endif

    static uint32_t s_interval;
#if _DEBUG
    static bool s_debugForceExpireToken;
#endif
};

uint32_t TokenRefreshWorker::s_interval = 1000 * 60 * 30; // 30 Minutes
#if _DEBUG
bool TokenRefreshWorker::s_debugForceExpireToken = false;
#endif

#if _DEBUG
extern "C" __declspec(dllexport) void PFDebugSetTokenRefreshDebugState(bool expireToken)
{
    TokenRefreshWorker::SetDebugForceExpireToken(expireToken);
}

extern "C" __declspec(dllexport) void PFDebugSetSetInterval(uint32_t interval)
{
    TokenRefreshWorker::SetInterval(interval);
}
#endif

TokenRefreshWorker::TokenRefreshWorker(SharedPtr<Entity> const& entity, SharedPtr<Authentication::LoginContext> loginContext, TokenExpiredHandler tokenExpiredHandler, PlayFab::RunContext&& rc) :
    m_weakEntity{ entity },
    m_loginContext{ std::move(loginContext) },
    m_tokenExpiredHandler{ std::move(tokenExpiredHandler) },
    m_rc{ std::move(rc) }
{
#if HC_PLATFORM == HC_PLATFORM_GDK
    HRESULT hr = XNetworkingRegisterConnectivityHintChanged(m_rc.TaskQueueHandle(), this, TokenRefreshWorker::NetworkConnectivityChangedCallback, &m_networkConnectivityChangedToken);
    if (FAILED(hr))
    {
        TRACE_ERROR_HR(hr, "Failed to register for connectivity changed notifications");
    }

    hr = HRESULT_FROM_WIN32(RegisterAppStateChangeNotification(TokenRefreshWorker::AppStateChangedCallback, this, &m_appStateChangedToken));
    if (FAILED(hr))
    {
        TRACE_ERROR_HR(hr, "Failed to register for app state change notifications");
    }
#endif
}

TokenRefreshWorker::~TokenRefreshWorker()
{
#if HC_PLATFORM == HC_PLATFORM_GDK
    if (m_appStateChangedToken)
    {
        UnregisterAppStateChangeNotification(m_appStateChangedToken);
    }

    if (m_networkConnectivityChangedToken.token)
    {
        XNetworkingUnregisterConnectivityHintChanged(m_networkConnectivityChangedToken, true);
    }
#endif
}

SharedPtr<TokenRefreshWorker> TokenRefreshWorker::MakeAndStart(
    SharedPtr<Entity> const& entity,
    SharedPtr<Authentication::LoginContext> loginContext,
    TokenExpiredHandler tokenExpiredHandler,
    RunContext&& rc
) noexcept
{
    Allocator<TokenRefreshWorker> a;
    SharedPtr<TokenRefreshWorker> worker{ new (a.allocate(1)) TokenRefreshWorker{ entity, std::move(loginContext), std::move(tokenExpiredHandler), std::move(rc) }, Deleter<TokenRefreshWorker>{}, a };

    worker->m_rc.TaskQueueSubmitWork(worker);
    return worker;
}

HRESULT TokenRefreshWorker::OnLoginContextUpdated(SharedPtr<Authentication::LoginContext> loginContext)
{
    std::lock_guard<std::mutex> lock{ m_mutex };
    m_loginContext = std::move(loginContext);
    return S_OK;
}

void TokenRefreshWorker::Run()
{
    SharedPtr<Entity> entity{ m_weakEntity.lock() };
    if (entity)
    {
        CheckAndRefreshToken(std::move(entity));

        // Reschedule ourselves.
        // Note that with this implementation the TokenExpiredHandler will be invoked every s_interval until the token is restored.
        // This may be fine, but we could include some additional logic to avoid that if desired.
        m_rc.TaskQueueSubmitWork(shared_from_this(), s_interval);
    }
}

void TokenRefreshWorker::CheckAndRefreshToken(SharedPtr<Entity> entity) noexcept
{
    auto getEntityTokenResult = entity->GetEntityToken();
    if (FAILED(getEntityTokenResult.hr))
    {
        // If we are able to get an entity token for the Entity, invoke the TokenExpired handler.
        TRACE_INFORMATION("TokenRefreshWorker unable to get EntityToken (hr = 0x%08x)");
        m_tokenExpiredHandler.Invoke(entity->EntityKey());
    }
    else if (CheckRefreshRequired(getEntityTokenResult.Payload()))
    {
        // If the Entity doesn't have a LoginContext (i.e. it wasn't obtained by a login) invoked the TokenExpiredHandler so that the
        // title can resolve the issue. We may want to have additional handling for this case in the future.
        if (!m_loginContext)
        {
            TRACE_VERBOSE("TokenRefreshWorker: EntityToken requires refresh but has no LoginContext, invoking TokenExpiredHandler");
            m_tokenExpiredHandler.Invoke(entity->EntityKey());
            return;
        }

        SharedPtr<TokenRefreshWorker> self{ shared_from_this() };

        m_loginContext->GetRequestBody(m_rc).Then([entity, self](Result<JsonValue> result) -> AsyncOp<ServiceResponse>
        {
            RETURN_IF_FAILED(result.hr);

            return entity->ServiceConfig()->HttpClient()->MakePostRequest(
                self->m_loginContext->RequestPath(),
                self->m_loginContext->RequestHeaders(),
                result.ExtractPayload(),
                self->m_loginContext->RetryCacheId(),
                self->m_rc.Derive()
            );
        }).Then([entity, self](Result<ServiceResponse> result) -> Result<void>
        {
            RETURN_IF_FAILED(result.hr);

#if _DEBUG
            if (TokenRefreshWorker::s_debugForceExpireToken)
            {
                TokenRefreshWorker::s_debugForceExpireToken = false;
                return E_FAIL;
            }
#endif

            Authentication::EntityTokenResponse entityToken;
            RETURN_IF_FAILED(JsonUtils::ObjectGetMember(result.Payload().Data, "EntityToken", entityToken));
            RETURN_IF_FAILED(entity->OnEntityTokenRefreshed(std::move(entityToken)));

            return S_OK;

        }).Finally([entity, self](Result<void> result)
        {
            // If we are unable to re-login for any reason, invoke the TokenExpiredHandler so the title can resolve the issue
            if (Failed(result))
            {
                TRACE_ERROR_HR(result.hr, "TokenRefreshWorker unable to restore EntityToken, invoking TokenExpiredHandler");
                self->m_tokenExpiredHandler.Invoke(entity->EntityKey());
            }
            else
            {
                TRACE_VERBOSE("TokenRefreshWorker: Tokens successfully refreshed");
            }
        });
    }
}

bool TokenRefreshWorker::CheckRefreshRequired(EntityToken const& token) noexcept
{
    // This was just a first crack at a refresh heuristic. We may want to refine this and sync with the
    // service team about recommended refresh policy.
    time_t const* expiration{ token.expiration };
    if (expiration && (*expiration - time(nullptr)) < (60 * 60))
    {
        return true;
    }

#if _DEBUG
    if (TokenRefreshWorker::s_debugForceExpireToken)
    {
        return true;
    }
#endif

    return false;
}

#if HC_PLATFORM == HC_PLATFORM_GDK

void TokenRefreshWorker::NetworkConnectivityChangedCallback(void* context, const XNetworkingConnectivityHint* /*hint*/)
{
    assert(context);
    auto worker = static_cast<TokenRefreshWorker*>(context);

    std::unique_lock<std::mutex> lock{ worker->m_mutex };

    // Ignore network connectivity changes if we are suspended
    if (!worker->m_isSuspended)
    {
        // Always requery the latest network connectivity hint rather than relying on the passed parameter in case this is a stale notification
        XNetworkingConnectivityHint hint{};
        HRESULT hr = XNetworkingGetConnectivityHint(&hint);
        if (SUCCEEDED(hr))
        {
            TRACE_VERBOSE("NetworkConnectivityChangedCallback, hint.networkInitialized=%d, hint.connectivityLevel=%d", hint.networkInitialized, hint.connectivityLevel);
            if (hint.networkInitialized && worker->m_resumeTokenCheckPending)
            {
                worker->m_resumeTokenCheckPending = false;
                lock.unlock();

                SharedPtr<Entity> entity{ worker->m_weakEntity.lock() };
                if (entity)
                {
                    worker->CheckAndRefreshToken(std::move(entity));
                }
            }
        }
        else
        {
            TRACE_ERROR_HR(hr, "Unable to get NetworkConnectivityHint");
        }
    }
}

void CALLBACK TokenRefreshWorker::AppStateChangedCallback(BOOLEAN isSuspended, void* context)
{
    assert(context);
    auto worker = static_cast<TokenRefreshWorker*>(context);

    std::unique_lock<std::mutex> lock{ worker->m_mutex };

    if (isSuspended)
    {
        worker->m_isSuspended = true;
        worker->m_resumeTokenCheckPending = true;
    }
    else
    {
        assert(worker->m_isSuspended);
        worker->m_isSuspended = false;

        lock.unlock();

        // Force a query of network state since we've ignored notifications during suspend. The EntityToken will
        // also be checked and refresh if needed here
        NetworkConnectivityChangedCallback(worker, nullptr);
    }
}
#endif

Result<SharedPtr<Entity>> Entity::Make(
    Authentication::EntityTokenResponse&& entityTokenResponse,
    SharedPtr<PlayFab::ServiceConfig const> serviceConfig,
    RunContext&& tokenRefreshContext,
    SharedPtr<Authentication::LoginContext> loginContext,
    TokenExpiredHandler tokenExpiredHandler,
    TokenRefreshedHandler tokenRefreshedHandler,
    std::optional<String> secretKey
) noexcept
{
    RETURN_HR_IF(E_FAIL, !entityTokenResponse.Model().entity);

    Allocator<Entity> a{};
    SharedPtr<Entity> entity{
        new (a.allocate(1)) Entity{
            std::move(serviceConfig),
            PlayFab::EntityKey(*entityTokenResponse.Model().entity),
            PlayFab::EntityToken(entityTokenResponse),
            std::move(secretKey),
            std::move(tokenRefreshedHandler)
        }, Deleter<Entity>(), a
    };

    entity->m_tokenRefreshWorker = TokenRefreshWorker::MakeAndStart(entity, std::move(loginContext), std::move(tokenExpiredHandler), std::move(tokenRefreshContext));

    return entity;
}

Result<SharedPtr<Entity>> Entity::Make(
    Authentication::GetEntityTokenResponse&& entityTokenResponse,
    SharedPtr<PlayFab::ServiceConfig const> serviceConfig,
    RunContext&& tokenRefreshContext,
    TokenExpiredHandler tokenExpiredHandler,
    TokenRefreshedHandler tokenRefreshedHandler,
    std::optional<String> secretKey
) noexcept
{
    RETURN_HR_IF(E_FAIL, !entityTokenResponse.entity);

    Allocator<Entity> a{};
    SharedPtr<Entity> entity{
        new (a.allocate(1)) Entity{
            std::move(serviceConfig),
            PlayFab::EntityKey(*entityTokenResponse.entity),
            PlayFab::EntityToken(entityTokenResponse),
            std::move(secretKey),
            std::move(tokenRefreshedHandler)
        }, Deleter<Entity>(), a
    };

    entity->m_tokenRefreshWorker = TokenRefreshWorker::MakeAndStart(entity, nullptr, std::move(tokenExpiredHandler), std::move(tokenRefreshContext));

    return entity;
}

Entity::Entity(
    SharedPtr<PlayFab::ServiceConfig const> serviceConfig,
    PlayFab::EntityKey&& entityKey,
    PlayFab::EntityToken&& entityToken,
    std::optional<String> secretKey,
    TokenRefreshedHandler tokenRefreshedHandler
) noexcept :
    m_key{ std::move(entityKey) },
    m_entityToken{ std::move(entityToken) },
    m_secretKey{ std::move(secretKey) },
    m_serviceConfig{ std::move(serviceConfig) },
    m_tokenRefreshedHandler{ std::move(tokenRefreshedHandler) }
{
}

SharedPtr<PlayFab::ServiceConfig const> Entity::ServiceConfig() const
{
    return m_serviceConfig;
}

PlayFab::EntityKey const& Entity::EntityKey() const
{
    return m_key;
}

Result<EntityToken> Entity::GetEntityToken() const
{
    std::unique_lock<std::mutex> lock{ m_mutex };
    if (m_entityToken.expiration && *m_entityToken.expiration < GetTimeTNow())
    {
        return E_PF_NOENTITYTOKEN;
    }
    return PlayFab::EntityToken{ m_entityToken };
}

Result<String> Entity::GetSecretKey() const
{
    // No lock needed here since SecretKey is const
    if (!m_secretKey.has_value())
    {
        return E_PF_NOSECRETKEY;
    }
    return String{ m_secretKey.value() };
}

HRESULT Entity::OnEntityTokenRefreshed(Authentication::EntityTokenResponse const& entityTokenResponse)
{
    std::unique_lock<std::mutex> lock{ m_mutex };

    RETURN_HR_IF(E_FAIL, entityTokenResponse.Model().entity == nullptr);

    if (std::strcmp(entityTokenResponse.Model().entity->id, m_key.Model().id))
    {
        TRACE_ERROR("%s: attempting to set EntityToken with mismatched entityKey", __FUNCTION__);
        return E_FAIL;
    }

    PlayFab::EntityToken newToken{ entityTokenResponse };
    m_entityToken = newToken;

    // Release lock before invoke handler (m_key and m_tokenRefreshedHandler are const and safe to access without lock)
    lock.unlock();
    m_tokenRefreshedHandler.Invoke(m_key, newToken);

    return S_OK;
}

HRESULT Entity::OnEntityTokenRefreshed(Authentication::GetEntityTokenResponse const& entityTokenResponse)
{
    std::unique_lock<std::mutex> lock{ m_mutex };

    RETURN_HR_IF(E_FAIL, !entityTokenResponse.entity.has_value());

    if (std::strcmp(entityTokenResponse.entity->Model().id, m_key.Model().id))
    {
        TRACE_ERROR("%s: attempting to set EntityToken with mismatched entityKey", __FUNCTION__);
        return E_FAIL;
    }

    PlayFab::EntityToken newToken{ entityTokenResponse };
    m_entityToken = newToken;

    // Release lock before invoke handler (m_key and m_tokenRefreshedHandler are const and safe to access without lock)
    lock.unlock();
    m_tokenRefreshedHandler.Invoke(m_key, newToken);

    return S_OK;
}

HRESULT Entity::OnLoginContextUpdated(SharedPtr<Authentication::LoginContext> loginContext)
{
    std::unique_lock<std::mutex> lock{ m_mutex };
    return m_tokenRefreshWorker->OnLoginContextUpdated(std::move(loginContext));
}

}

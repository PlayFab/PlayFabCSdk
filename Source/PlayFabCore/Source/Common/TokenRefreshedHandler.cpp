#include "stdafx.h"
#include "TokenRefreshedHandler.h"

namespace PlayFab
{

class TokenRefreshedClientHandler
{
public:
    TokenRefreshedClientHandler(RunContext&& rc, void* context, PFEntityTokenRefreshedEventHandler* handler) noexcept;

    RunContext RunContext() const noexcept;
    void Invoke(EntityKey const& entityKey, EntityToken const& newToken) const noexcept;

private:
    PlayFab::RunContext m_rc;
    void* const m_context;
    PFEntityTokenRefreshedEventHandler* const m_handler;
};

class TokenRefreshedHandler::SharedState
{
public:
    SharedState() noexcept;
    SharedState(SharedState const&) = delete;
    SharedState(SharedState&&) = delete;
    SharedState& operator=(SharedState const&) = delete;
    SharedState& operator=(SharedState&&) = delete;
    ~SharedState() noexcept = default;

    HRESULT RegisterClientHandler(
        RunContext&& rc,
        void* context,
        PFEntityTokenRefreshedEventHandler* handler,
        PFRegistrationToken* token
    ) noexcept;

    void UnregisterClientHandler(PFRegistrationToken token) noexcept;

    void Invoke(EntityKey const& entityKey, EntityToken const& newToken) const noexcept;

private:
    mutable std::recursive_mutex m_mutex; // recursive for unregister

    Map<PFRegistrationToken, SharedPtr<TokenRefreshedClientHandler>> m_clientHandlers;
    PFRegistrationToken m_nextToken;
};

TokenRefreshedClientHandler::TokenRefreshedClientHandler(PlayFab::RunContext&& rc, void* context, PFEntityTokenRefreshedEventHandler* handler) noexcept :
    m_rc{ std::move(rc) },
    m_context{ context },
    m_handler{ handler }
{
}

RunContext TokenRefreshedClientHandler::RunContext() const noexcept
{
    return m_rc;
}

void TokenRefreshedClientHandler::Invoke(EntityKey const& entityKey, EntityToken const& newToken) const noexcept
{
    try
    {
        m_handler(m_context, &entityKey.Model(), &newToken);
    }
    catch (...)
    {
        TRACE_ERROR("Caught unhandled exception in client TokenRefreshed handler");
    }
}

namespace Detail
{
    // Arbitrary first token thats recognizable when debugging
    PFRegistrationToken const kFirstCallbackToken = 0xBAC0000000000000;
}


TokenRefreshedHandler::SharedState::SharedState() noexcept :
    m_nextToken{ Detail::kFirstCallbackToken }
{
}

HRESULT TokenRefreshedHandler::SharedState::RegisterClientHandler(RunContext&& rc, void* context, PFEntityTokenRefreshedEventHandler* handler, PFRegistrationToken* token) noexcept
{
    std::unique_lock<std::recursive_mutex> lock{ m_mutex };

    RETURN_HR_INVALIDARG_IF_NULL(handler);
    RETURN_HR_INVALIDARG_IF_NULL(token);

    *token = m_nextToken++;
    m_clientHandlers.emplace(*token, MakeShared<TokenRefreshedClientHandler>(std::move(rc), context, handler));

    return S_OK;
}

void TokenRefreshedHandler::SharedState::UnregisterClientHandler(PFRegistrationToken token) noexcept
{
    std::unique_lock<std::recursive_mutex> lock{ m_mutex };

    TRACE_VERBOSE(__FUNCTION__);

    auto it = m_clientHandlers.find(token);
    if (it == m_clientHandlers.end())
    {
        TRACE_WARNING("%s: Attempted to unregister handler with unrecognized token", __FUNCTION__);
        return;
    }

    it->second->RunContext().CancellationToken().Cancel();

    m_clientHandlers.erase(it);
}

void TokenRefreshedHandler::SharedState::Invoke(EntityKey const& entityKey, EntityToken const& newToken) const noexcept
{
    std::unique_lock<std::recursive_mutex> lock{ m_mutex };
    for (auto& pair : m_clientHandlers)
    {
        // TokenRefreshedHandler handles submitting the work to the TaskQueue so that it can synchronize the event handler invocation
        // and unregistration of the handler

        auto handler = pair.second;
        handler->RunContext().TaskQueueSubmitWork([this, handler, key = EntityKey{ entityKey }, token = EntityToken{ newToken }]()
        {
            std::unique_lock<std::recursive_mutex> lock{ m_mutex }; // lock to avoid races with unregistering the handler

            if (!handler->RunContext().CancellationToken().IsCancelled())
            {
                handler->Invoke(key, token);
            }
        });
    }
}

TokenRefreshedHandler::TokenRefreshedHandler() noexcept :
    m_state{ MakeShared<SharedState>() }
{
}

HRESULT TokenRefreshedHandler::RegisterClientHandler(RunContext&& rc, void* context, PFEntityTokenRefreshedEventHandler* handler, PFRegistrationToken* token) noexcept
{
    return m_state->RegisterClientHandler(std::move(rc), context, handler, token);
}

void TokenRefreshedHandler::UnregisterClientHandler(PFRegistrationToken token) noexcept
{
    m_state->UnregisterClientHandler(token);
}

void TokenRefreshedHandler::Invoke(EntityKey const& entityKey, EntityToken const& newToken) const noexcept
{
    m_state->Invoke(entityKey, newToken);
}

}

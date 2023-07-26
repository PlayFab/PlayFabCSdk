#include "stdafx.h"
#include "TokenExpiredHandler.h"

namespace PlayFab
{

class ClientHandler
{
public:
    ClientHandler(RunContext&& rc, void* context, PFEntityTokenExpiredEventHandler* handler) noexcept;

    RunContext RunContext() const noexcept;
    void Invoke(EntityKey const& entityKey) const noexcept;

private:
    PlayFab::RunContext m_rc;
    void* const m_context;
    PFEntityTokenExpiredEventHandler* const m_handler;
};

class TokenExpiredHandler::SharedState
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
        PFEntityTokenExpiredEventHandler* handler,
        PFRegistrationToken* token
    ) noexcept;

    void UnregisterClientHandler(PFRegistrationToken token) noexcept;

    void Invoke(EntityKey const& entityKey) const noexcept;

private:
    mutable std::recursive_mutex m_mutex; // recursive for unregister

    Map<PFRegistrationToken, SharedPtr<ClientHandler>> m_clientHandlers;
    PFRegistrationToken m_nextToken;
};

ClientHandler::ClientHandler(PlayFab::RunContext&& rc, void* context, PFEntityTokenExpiredEventHandler* handler) noexcept :
    m_rc{ std::move(rc) },
    m_context{ context },
    m_handler{ handler }
{
}

RunContext ClientHandler::RunContext() const noexcept
{
    return m_rc;
}

void ClientHandler::Invoke(EntityKey const& entityKey) const noexcept
{
    try
    {
        m_handler(m_context, &entityKey.Model());
    }
    catch (...)
    {
        TRACE_ERROR("Caught unhandled exception in client TokenExpired handler");
    }
}

namespace Detail
{
// Arbitrary first token thats recognizable when debugging
PFRegistrationToken const kFirstCallbackToken = 0xBAC0000000000000;
}


TokenExpiredHandler::SharedState::SharedState() noexcept :
    m_nextToken{ Detail::kFirstCallbackToken }
{
}

HRESULT TokenExpiredHandler::SharedState::RegisterClientHandler(RunContext&& rc, void* context, PFEntityTokenExpiredEventHandler* handler, PFRegistrationToken* token) noexcept
{
    std::unique_lock<std::recursive_mutex> lock{ m_mutex };

    RETURN_HR_INVALIDARG_IF_NULL(handler);
    RETURN_HR_INVALIDARG_IF_NULL(token);

    *token = m_nextToken++;
    m_clientHandlers.emplace(*token, MakeShared<ClientHandler>(std::move(rc), context, handler));

    return S_OK;
}

void TokenExpiredHandler::SharedState::UnregisterClientHandler(PFRegistrationToken token) noexcept
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

void TokenExpiredHandler::SharedState::Invoke(EntityKey const& entityKey) const noexcept
{
    std::unique_lock<std::recursive_mutex> lock{ m_mutex };
    for (auto& pair : m_clientHandlers)
    {
        // TokenExpiredHandler handles submitting the work to the TaskQueue so that it can synchronize the event handler invocation
        // and unregistration of the handler

        auto handler = pair.second;
        handler->RunContext().TaskQueueSubmitWork([this, handler, key = EntityKey{ entityKey } ]()
        {
            std::unique_lock<std::recursive_mutex> lock{ m_mutex }; // lock to avoid races with unregistering the handler
            if (!handler->RunContext().CancellationToken().IsCancelled())
            {
                handler->Invoke(key);
            }
        });
    }
}

TokenExpiredHandler::TokenExpiredHandler() noexcept :
    m_state{ MakeShared<SharedState>() }
{
}

HRESULT TokenExpiredHandler::RegisterClientHandler(RunContext&& rc, void* context, PFEntityTokenExpiredEventHandler* handler, PFRegistrationToken* token) noexcept
{
    return m_state->RegisterClientHandler(std::move(rc), context, handler, token);
}

void TokenExpiredHandler::UnregisterClientHandler(PFRegistrationToken token) noexcept
{
    m_state->UnregisterClientHandler(token);
}

void TokenExpiredHandler::Invoke(EntityKey entityKey) const noexcept
{
    m_state->Invoke(std::move(entityKey));
}

}

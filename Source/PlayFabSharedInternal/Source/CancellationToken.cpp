#include "pch.h"
#include "CancellationToken.h"

namespace PlayFab
{

class CancellationToken::SharedState : public std::enable_shared_from_this<SharedState>
{
public:
    SharedPtr<SharedState> Derive() noexcept;

    SharedState() = default;
    SharedState(SharedState const&) = delete;
    SharedState(SharedState&&) = delete;
    SharedState& operator=(SharedState const&) = delete;
    SharedState& operator=(SharedState&&) = delete;
    ~SharedState() noexcept = default;

    bool IsCanceled() const noexcept;

    bool Cancel() noexcept;

    bool RegisterForNotificationAndCheck(ICancellationListener& listener);
    bool UnregisterForNotificationAndCheck(ICancellationListener& listener) noexcept;

private:
    mutable std::mutex m_mutex;
    std::recursive_mutex m_cancellationMutex; // intentionally recursive
    std::atomic<bool> m_cancelled{ false };
    Vector<ICancellationListener*> m_listeners; // non-owning
    Vector<WeakPtr<SharedState>> m_children;
    SharedPtr<SharedState> m_parent;
};

SharedPtr<CancellationToken::SharedState> CancellationToken::SharedState::Derive() noexcept
{
    SharedPtr<SharedState> child = MakeShared<SharedState>();

    std::unique_lock<std::mutex> lock{ m_mutex };

    // Clean up stale child references
    m_children.erase(
        std::remove_if(
            m_children.begin(),
            m_children.end(),
            [](WeakPtr<SharedState> const& p) { return p.expired(); }
        ),
        m_children.end()
    );

    // Set up parent/child pointers depending on whether the token has been cancelled
    if (m_cancelled)
    {
        child->m_cancelled = true;
    }
    else
    {
        child->m_parent = shared_from_this();
        m_children.push_back(child);
    }

    return child;
}

bool CancellationToken::SharedState::IsCanceled() const noexcept
{
    std::unique_lock<std::mutex> lock{ m_mutex };
    return m_cancelled;
}

bool CancellationToken::SharedState::Cancel() noexcept
{
    // Whoever is calling us has a strong reference but we still want to take a
    // strong reference to ourselves just in case they let go of us in the
    // callback
    SharedPtr<SharedState> self{ shared_from_this() };

    std::unique_lock<std::recursive_mutex> cancellationLock{ m_cancellationMutex };
    std::unique_lock<std::mutex> lock{ m_mutex };
    
    // If we've already been cancelled, nothing to do
    if (m_cancelled.exchange(true))
    {
        return false;
    }

    // We only needed to keep m_parent alive so we'd be recursively cancelled, safe to release it now
    m_parent.reset();

    Vector<ICancellationListener*> listeners{};
    Vector<WeakPtr<SharedState>> children{};

    // Move the listeners in a local to clear the list
    std::swap(listeners, m_listeners);

    // Move the child tokens into a local to clear the list
    std::swap(children, m_children);

    lock.unlock();

    // Intentionally hold the cancellationLock while notifying to avoid races with unregister
    for (auto l : listeners)
    {
        // We do not have ownership of l, but we do have an existance guarantee
        // thanks to the mutual exclusion of this method and
        // 'UnregisterForNotificationAndCheck'
        // l could be destroyed after 'OnCancellation' returns
        l->OnCancellation();
    }

    for (auto& weakChild : children)
    {
        SharedPtr<SharedState> child = weakChild.lock();
        if (child)
        {
            child->Cancel();
        }
    }

    return !listeners.empty() || !children.empty();
}

bool CancellationToken::SharedState::RegisterForNotificationAndCheck(ICancellationListener& listener)
{
    // We don't need to sync on cancellation here, either the callback will
    // happen entirely after we are done, or it will never happen at all
    std::unique_lock<std::mutex> lock{ m_mutex };

    // We return the current cancellation state as part of this function so that
    // callers can know if the callbacks will be invoked later or not

    if (m_cancelled)
    {
        return true;
    }

    m_listeners.push_back(&listener);

    return false;
}

bool CancellationToken::SharedState::UnregisterForNotificationAndCheck(ICancellationListener& listener) noexcept
{
    // We take the cancellationGuard to ensure that we are properly serialized
    // with respect to the callbacks. Because it's a recursive lock,
    // unregistering from within the callback will also work (and it's safe)
    std::unique_lock<std::recursive_mutex> cancallationLock{ m_cancellationMutex };
    std::unique_lock<std::mutex> lock{ m_mutex };

    auto it = std::find(m_listeners.begin(), m_listeners.end(), &listener);
    if (it != m_listeners.end())
    {
        m_listeners.erase(it);
    }

    return m_cancelled;
}

CancellationToken::CancellationToken(SharedPtr<SharedState> state) noexcept :
    m_state{ std::move(state) }
{
}

CancellationToken CancellationToken::Root() noexcept
{
    return CancellationToken{ MakeShared<SharedState>() };
}

CancellationToken CancellationToken::Derive() const noexcept
{
    return CancellationToken{ m_state->SharedState::Derive() };
}

bool CancellationToken::IsCancelled() const noexcept
{
    return m_state->IsCanceled();
}

bool CancellationToken::Cancel() noexcept
{
    return m_state->Cancel();
}

bool CancellationToken::RegisterForNotificationAndCheck(ICancellationListener& listener) noexcept
{
    return m_state->RegisterForNotificationAndCheck(listener);
}

bool CancellationToken::UnregisterForNotificationAndCheck(ICancellationListener& listener) noexcept
{
    return m_state->UnregisterForNotificationAndCheck(listener);
}

}
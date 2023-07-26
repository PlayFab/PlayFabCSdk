// Token that coordinates cancellation between consumers and implementers of asyncronous operations

#pragma once

#include "Memory.h"

namespace PlayFab
{

struct ICancellationListener
{
    virtual ~ICancellationListener() = default;
    virtual void OnCancellation() = 0;
};

class CancellationToken
{
public:
    static CancellationToken Root() noexcept;
    CancellationToken Derive() const noexcept;

    CancellationToken(CancellationToken const& other) noexcept = default;
    CancellationToken(CancellationToken&& other) noexcept = default;
    CancellationToken& operator=(CancellationToken const& other) noexcept = delete;
    CancellationToken& operator=(CancellationToken&& other) noexcept = delete;
    ~CancellationToken() noexcept = default;

    bool IsCancelled() const noexcept;

    // returns true if there were listeners that have been cancelled
    bool Cancel() noexcept;

    // Register a cancellation listener. Returns true if cancellation has already happened,
    // and the listener will not be invoked in that case
    bool RegisterForNotificationAndCheck(ICancellationListener& listener) noexcept;
    bool UnregisterForNotificationAndCheck(ICancellationListener& listener) noexcept;

private:
    class SharedState;

    CancellationToken(SharedPtr<SharedState> state) noexcept;

    SharedPtr<SharedState> m_state;
};

}

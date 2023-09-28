#include "pch.h"
#include "RunContext.h"

namespace PlayFab
{

static std::atomic<uint32_t> g_nextId{ 0 };

// RAII wrapper of XTaskQueueHandle
class TaskQueue
{
public:
    // The only queues we should use internally should be derived from another Queue. We don't want to directly
    // submit work to a client owned queue because we will terminate queues during cleanup.
    TaskQueue DeriveWorkQueue() const noexcept;
    static TaskQueue DeriveWorkQueue(XTaskQueueHandle handle) noexcept;

    TaskQueue(XTaskQueueHandle handle) noexcept; // Takes ownership of handle
    TaskQueue(const TaskQueue& other) noexcept = delete;
    TaskQueue(TaskQueue&& other) noexcept;
    TaskQueue& operator=(TaskQueue const& other) noexcept = delete;
    TaskQueue& operator=(TaskQueue&& other) noexcept = delete;
    ~TaskQueue() noexcept;

    XTaskQueueHandle Handle() const noexcept;

private:
    XTaskQueueHandle m_handle{ nullptr };
};

class RunContextState : public ITerminable, public ITerminationListener, public std::enable_shared_from_this<RunContextState>
{
public:
    static SharedPtr<RunContextState> Root(XTaskQueueHandle queueHandle) noexcept;
    SharedPtr<RunContextState> Derive() noexcept;
    SharedPtr<RunContextState> DeriveOnQueue(XTaskQueueHandle queueHandle) noexcept;

    RunContextState(TaskQueue&& queue, PlayFab::CancellationToken&& ct, SharedPtr<RunContextState> parent) noexcept;
    RunContextState(RunContextState const&) noexcept = delete;
    RunContextState(RunContextState&&) noexcept = delete;
    RunContextState& operator=(RunContextState const&) noexcept = delete;
    RunContextState& operator=(RunContextState&&) noexcept = delete;
    ~RunContextState() noexcept;

public:
    XTaskQueueHandle TaskQueueHandle() const noexcept;
    void TaskQueueSubmitCallback(XTaskQueuePort port, SharedPtr<ITaskQueueWork> work, uint32_t delayInMs) noexcept;
    void TaskQueueTerminate() noexcept;

    PlayFab::CancellationToken CancellationToken() const noexcept;

    bool RegisterTerminableAndCheck(ITerminable& terminable) noexcept;
    bool UnregisterTerminableAndCheck(ITerminable& terminable) noexcept;

    void Terminate(ITerminationListener& listener, void* context) override;

private:
    void AppendChild(SharedPtr<RunContextState> child) noexcept;

    // ITerminationListener
    void OnTerminated(void* context) noexcept override; 

    // XTaskQueue callbacks
    static void CALLBACK TaskQueueCallback(void* context, bool cancelled) noexcept;
    static void CALLBACK TaskQueueTerminated(void* context) noexcept;

    static void CheckTerminationAndNotifyListener(SharedPtr<RunContextState> runContext, std::unique_lock<std::mutex> lock) noexcept;

    std::mutex m_mutex;
    std::recursive_mutex m_terminationMutex;
    TaskQueue const m_queue;
    PlayFab::CancellationToken m_cancellationToken;
    SharedPtr<RunContextState> m_parent;
    Vector<WeakPtr<RunContextState>> m_children;
    Vector<ITerminable*> m_terminables; // non-owning
    size_t m_pendingTaskQueueCallbacks{ 0 };
    bool m_terminated{ false };
    ITerminationListener* m_terminationListener{ nullptr }; // non-owning
    void* m_terminationListenerContext{ nullptr };
    size_t m_pendingTerminations{ 0 };
    bool m_queueTerminated{ false };

    // For debugging purposes only
    uint32_t m_id;
    uint32_t m_depth;
};

//------------------------------------------------------------------------------
// TaskQueue
//------------------------------------------------------------------------------

TaskQueue::TaskQueue(XTaskQueueHandle handle) noexcept :
    m_handle{ handle }
{
    assert(handle);
}

TaskQueue::TaskQueue(TaskQueue&& other) noexcept :
    m_handle{ other.m_handle }
{
    other.m_handle = nullptr;
}


TaskQueue::~TaskQueue() noexcept
{
    if (m_handle)
    {
        XTaskQueueCloseHandle(m_handle);
    }
}

TaskQueue TaskQueue::DeriveWorkQueue() const noexcept
{
    return DeriveWorkQueue(m_handle);
}

TaskQueue TaskQueue::DeriveWorkQueue(XTaskQueueHandle handle) noexcept
{
    std::optional<TaskQueue> processQueue{}; // We will derive from process queue if handle is null
    if (!handle)
    {
        bool haveProcessQueue = XTaskQueueGetCurrentProcessTaskQueue(&handle);
        if (haveProcessQueue)
        {
            // Wrap process queue handle so that it gets closed
            processQueue.emplace(handle);
        }
        else
        {
            // Fatal situation
            TRACE_ERROR("Client provided null XTaskQueueHandle and no default process queue is available");
            assert(false);
        }
    }

    XTaskQueuePortHandle workPort{ nullptr };
    HRESULT hr = XTaskQueueGetPort(handle, XTaskQueuePort::Work, &workPort);
    if (FAILED(hr))
    {
        TRACE_ERROR_HR(hr, "Failed to get work port from queue");
        assert(false); // handle this
    }

    XTaskQueueHandle derivedHandle{ nullptr };
    hr = XTaskQueueCreateComposite(workPort, workPort, &derivedHandle);
    if (FAILED(hr))
    {
        TRACE_ERROR_HR(hr, "Failed to create composite queue");
        assert(false);
    }

    return TaskQueue{ derivedHandle };
}

XTaskQueueHandle TaskQueue::Handle() const noexcept
{
    return m_handle;
}

//------------------------------------------------------------------------------
// RunContextState
//------------------------------------------------------------------------------

RunContextState::RunContextState(TaskQueue&& q, PlayFab::CancellationToken&& ct, SharedPtr<RunContextState> parent) noexcept :
    m_queue{ std::move(q) },
    m_cancellationToken{ std::move(ct) },
    m_parent{ std::move(parent) },
    m_id{ g_nextId++ },
    m_depth{ m_parent ? m_parent->m_depth + 1 : 0 }
{
}

RunContextState::~RunContextState() noexcept
{
    TRACE_VERBOSE("RunContextState[id=%u]::~RunContextState", m_id);
}

SharedPtr<RunContextState> RunContextState::Root(XTaskQueueHandle queueHandle) noexcept
{
    return MakeShared<RunContextState>(TaskQueue::DeriveWorkQueue(queueHandle), CancellationToken::Root(), nullptr);
}

SharedPtr<RunContextState> RunContextState::Derive() noexcept
{
    SharedPtr<RunContextState> derived = MakeShared<RunContextState>(m_queue.DeriveWorkQueue(), m_cancellationToken.Derive(), shared_from_this());
    AppendChild(derived);
    return derived;
}

SharedPtr<RunContextState> RunContextState::DeriveOnQueue(XTaskQueueHandle queueHandle) noexcept
{
    SharedPtr<RunContextState> derived = MakeShared<RunContextState>(TaskQueue::DeriveWorkQueue(queueHandle), m_cancellationToken.Derive(), shared_from_this());
    AppendChild(derived);
    return derived;
}

void RunContextState::AppendChild(SharedPtr<RunContextState> child) noexcept
{
    std::unique_lock<std::mutex> lock{ m_mutex };
    // Clean up stale child references
    m_children.erase(
        std::remove_if(
            m_children.begin(),
            m_children.end(),
            [](WeakPtr<RunContextState> const& p) { return p.expired(); }
        ),
        m_children.end()
    );

    m_children.push_back(child);
}

XTaskQueueHandle RunContextState::TaskQueueHandle() const noexcept
{
    return m_queue.Handle();
}

struct XTaskQueueCallbackContext
{
    SharedPtr<RunContextState> runContext;
    SharedPtr<ITaskQueueWork> work;
};

void RunContextState::TaskQueueSubmitCallback(XTaskQueuePort port, SharedPtr<ITaskQueueWork> work, uint32_t delayInMs) noexcept
{
    assert(work);
    assert(m_queue.Handle());

    Allocator<XTaskQueueCallbackContext> a;
    XTaskQueueCallbackContext* context = new (a.allocate(1)) XTaskQueueCallbackContext{ shared_from_this(), std::move(work) }; // reclaimed in TaskQueueCallback

    std::unique_lock<std::mutex> lock{ m_mutex };
    ++m_pendingTaskQueueCallbacks;
    lock.unlock();

    TRACE_VERBOSE("RunContextState[id=%u] TaskQueue callback submitted", m_id);

    HRESULT hr = XTaskQueueSubmitDelayedCallback(m_queue.Handle(), port, delayInMs, context, TaskQueueCallback);
    if (FAILED(hr))
    {
        // Treat other errors as cancellations by calling TaskQueueCallback so that the cancellation handler is called and
        // m_pendingTaskQueueCallbacks is updated correctly
        TaskQueueCallback(context, true);

        TRACE_WARNING_HR(hr, "XTaskQueueSubmitDelayedCallback failed");
        assert(hr == E_ABORT); // The only error we expect to ever see here is E_ABORT
    }
}

void CALLBACK RunContextState::TaskQueueCallback(void* c, bool cancelled) noexcept
{
    assert(c);
    UniquePtr<XTaskQueueCallbackContext> callbackContext{ static_cast<XTaskQueueCallbackContext*>(c) };
    assert(callbackContext->runContext && callbackContext->work);

    TRACE_VERBOSE("RunContextState[id=%u] TaskQueueCallback", callbackContext->runContext->m_id);

    if (cancelled)
    {
        callbackContext->work->WorkCancelled();
    }
    else
    {
        callbackContext->work->Run();
    }

    SharedPtr<RunContextState> runContext{ std::move(callbackContext->runContext) };
    callbackContext.reset();
    
    std::unique_lock<std::mutex> lock{ runContext->m_mutex };
    --runContext->m_pendingTaskQueueCallbacks;

    TRACE_VERBOSE("RunContextState[id=%u] TaskQueueCallback complete, %u remaining", runContext->m_id, runContext->m_pendingTaskQueueCallbacks);

    CheckTerminationAndNotifyListener(std::move(runContext), std::move(lock));
}

void RunContextState::TaskQueueTerminate() noexcept
{
    std::unique_lock<std::mutex> lock{ m_mutex };

    if (m_queueTerminated)
    {
        // Early out if the queue has already been terminated. Check needed because unlike XTaskQueueSubmitDelayedCallback, 
        // XTaskQueueTerminate will succeed even after the queue has been terminated (creating another asyncronous callback that could
        // run beyond RunContext::Terminate completing in some cases)
        return;
    }

    // Track termination callbacks together with work callbacks. Because XTaskQueueTerminate doesn't guarantee that all submitted work callbacks 
    // have completed before invoking termination callbacks (it only guarantees that they've been started), we need additional tracking to ensure
    // all work has completed during RunContext termination
    ++m_pendingTaskQueueCallbacks;
    m_queueTerminated = true;
    lock.unlock();

    TRACE_VERBOSE("RunContextState[id=%u] TaskQueue terminating, %u callbacks remaining", m_id, m_pendingTaskQueueCallbacks);

    assert(m_queue.Handle());
    Allocator<XTaskQueueCallbackContext> a;
    XTaskQueueCallbackContext* context = new (a.allocate(1)) XTaskQueueCallbackContext{ shared_from_this() }; // reclaimed in TaskQueueTerminated

    HRESULT hr = XTaskQueueTerminate(m_queue.Handle(), false, context, TaskQueueTerminated);
    if (FAILED(hr))
    {
        // This likely indicates a bug, but consider the queue terminated to not block cleanup
        assert(SUCCEEDED(hr));
        TRACE_ERROR_HR(hr, "Failed to terminate queue");

        TaskQueueTerminated(context);
    }
}

void CALLBACK RunContextState::TaskQueueTerminated(void* c) noexcept
{
    assert(c);
    UniquePtr<XTaskQueueCallbackContext> callbackContext{ static_cast<XTaskQueueCallbackContext*>(c) };
    assert(callbackContext->runContext && !callbackContext->work);

    SharedPtr<RunContextState> runContext{ std::move(callbackContext->runContext) };
    callbackContext.reset();

    std::unique_lock<std::mutex> lock{ runContext->m_mutex };
    --runContext->m_pendingTaskQueueCallbacks;

    TRACE_VERBOSE("RunContextState[id=%u] TaskQueueTerminated, %u callbacks remaining", runContext->m_id, runContext->m_pendingTaskQueueCallbacks);

    CheckTerminationAndNotifyListener(std::move(runContext), std::move(lock));
}

PlayFab::CancellationToken RunContextState::CancellationToken() const noexcept
{
    return m_cancellationToken;
}

bool RunContextState::RegisterTerminableAndCheck(ITerminable& terminable) noexcept
{
    std::unique_lock<std::mutex> lock{ m_mutex };

    if (m_terminated)
    {
        return true;
    }

    m_terminables.push_back(&terminable);

    return false;
}

bool RunContextState::UnregisterTerminableAndCheck(ITerminable& terminable) noexcept
{
    // We take the terminationMutex to ensure that we are properly serialized with respect to the terminating m_terminables.
    // Because it's a recursive lock, unregistering from within Terminate is safe
    std::unique_lock<std::recursive_mutex> terminationLock{ m_terminationMutex };
    std::unique_lock<std::mutex> lock{ m_mutex };

    auto it = std::find(m_terminables.begin(), m_terminables.end(), &terminable);
    if (it != m_terminables.end())
    {
        m_terminables.erase(it);
    }

    return m_terminated;
}

using TerminationContext = SharedPtr<RunContextState>;

void RunContextState::Terminate(ITerminationListener& listener, void* listenerContext)
{
    TRACE_VERBOSE(__FUNCTION__);

    std::unique_lock<std::recursive_mutex> terminationLock{ m_terminationMutex };
    std::unique_lock<std::mutex> lock{ m_mutex };

    assert(!m_terminated);
    m_terminated = true;
    m_terminationListener = &listener;
    m_terminationListenerContext = listenerContext;

    // We only needed to keep m_parent alive so we'd be recursively terminated. Safe to release it now that we've been
    // terminated.
    m_parent.reset();

    Vector<SharedPtr<RunContextState>> children;
    for (auto& weakChild : m_children)
    {
        if (auto child = weakChild.lock())
        {
            children.emplace_back(child);
        }
    }

    m_pendingTerminations = m_terminables.size() + children.size();
    // Add an additional dummy pending termination representing the completion of this method. This addresses a race condition
    // where all terminations asynchronously complete before this method returns, potentially cause 'this' to be destroyed unexpectedly
    m_pendingTerminations++;
    
    TRACE_VERBOSE("RunContextState[id=%u] terminating with %u terminables", m_id, m_pendingTerminations);

    // context will ensure our lifetime until Termination completes.
    TerminationContext* context = MakeUnique<TerminationContext>(shared_from_this()).release(); // reclaimed in OnTerminated;

    // Release state lock but intentionally hold terminationLock while notifying terminables to avoid races with unregister
    lock.unlock();

    // Terminate Queue, registered terminables, and children
    TaskQueueTerminate();

    for (auto& terminable : m_terminables)
    {
        terminable->Terminate(*this, context);
    }

    for (auto& child : children)
    {
        child->Terminate(*this, context);
    }

    // Dummy termination task added above complete
    OnTerminated(context);
}

void RunContextState::OnTerminated(void* c) noexcept
{
    std::unique_lock<std::mutex> lock{ m_mutex };

    assert(m_pendingTerminations);
    --m_pendingTerminations;
    TRACE_VERBOSE("RunContextState[id=%u] terminable terminated, %u remaining", m_id, m_pendingTerminations);

    if (!m_pendingTerminations)
    {
        // Free context and check if termination is complete
        UniquePtr<TerminationContext> terminationContext{ static_cast<TerminationContext*>(c) };
        SharedPtr<RunContextState> self = std::move(*terminationContext);
        terminationContext.reset();

        CheckTerminationAndNotifyListener(std::move(self), std::move(lock));
    }
}

void RunContextState::CheckTerminationAndNotifyListener(SharedPtr<RunContextState> runContext, std::unique_lock<std::mutex> lock) noexcept
{
    // Notify listener iff
    // 1) m_terminationListener is non-null (Terminate has been called and hasn't previously been completed)
    // 2) there are no pending TaskQueue callbacks
    // 3) there are no pending terminations of registered or child terminables

    assert(lock.owns_lock());

    if (runContext->m_terminationListener && !runContext->m_pendingTaskQueueCallbacks && !runContext->m_pendingTerminations)
    {
        // Move listener and listenerContext to the stack before releasing runContext, it may be destroyed here
        ITerminationListener& listener = *runContext->m_terminationListener;
        void* listenerContext = runContext->m_terminationListenerContext;

        // reset m_terminationListener to avoid double notifying in some race scenarios
        runContext->m_terminationListener = nullptr; 

        TRACE_VERBOSE("RunContextState[id=%u] Termination complete, notifying listener", runContext->m_id);

        lock.unlock();
        runContext.reset();

        listener.OnTerminated(listenerContext);
    }
    else
    {
        // Explicitly unlock before exiting function scope to ensure runContext isn't destroyed while the mutex is held.
        // Needed because C++ doesn't define destruction order of function parameters so it may vary depending on compiler
        lock.unlock();
    }
}

//------------------------------------------------------------------------------
// RunContext
//------------------------------------------------------------------------------

RunContext::RunContext(SharedPtr<RunContextState> state) noexcept :
    m_state{ std::move(state) }
{
}

RunContext RunContext::Root(XTaskQueueHandle queue) noexcept
{
    return RunContext{ RunContextState::Root(queue) };
}

RunContext RunContext::Derive() noexcept
{
    return RunContext{ m_state->Derive() };
}

RunContext RunContext::DeriveOnQueue(XTaskQueueHandle queueHandle) noexcept
{
    return RunContext{ m_state->DeriveOnQueue(queueHandle) };
}

XTaskQueueHandle RunContext::TaskQueueHandle() const noexcept
{
    return m_state->TaskQueueHandle();
}

void RunContext::TaskQueueSubmitWork(SharedPtr<ITaskQueueWork> work, uint32_t delayInMs) const noexcept
{
    m_state->TaskQueueSubmitCallback(XTaskQueuePort::Work, std::move(work), delayInMs);
}

void RunContext::TaskQueueSubmitCompletion(SharedPtr<ITaskQueueWork> completion) const noexcept
{
    m_state->TaskQueueSubmitCallback(XTaskQueuePort::Completion, std::move(completion), 0);
}

void RunContext::TaskQueueTerminate() noexcept
{
    m_state->TaskQueueTerminate();
}

CancellationToken RunContext::CancellationToken() const noexcept
{
    return m_state->CancellationToken();
}

bool RunContext::RegisterTerminableAndCheck(ITerminable& terminable) noexcept
{
    return m_state->RegisterTerminableAndCheck(terminable);
}

bool RunContext::UnregisterTerminableAndCheck(ITerminable& terminable) noexcept
{
    return m_state->UnregisterTerminableAndCheck(terminable);
}

void RunContext::Terminate(ITerminationListener& listener, void* context) noexcept
{
    m_state->Terminate(listener, context);
}

} // namespace PlayFab

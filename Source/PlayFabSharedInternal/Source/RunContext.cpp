#include "pch.h"
#include "RunContext.h"
#if defined(__cpp_rtti) || defined(_CPPRTTI)
#include <typeinfo>
#endif

namespace PlayFab
{

#pragma warning( push )
#pragma warning( disable : 6328 ) // Size mismatch : 'unsigned __int64' passed as _Param_(5) when 'int' is required in call to 'HCTraceImplMessage'

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

    // Diagnostic tracking of pending callbacks (populated only after termination starts)
    struct PendingCallbackInfo
    {
        uint32_t submitId;
        const char* typeName;
        XTaskQueuePort port;
        uint32_t delayInMs;
    };
    std::atomic<uint32_t> m_nextSubmitId{ 0 };
    Vector<PendingCallbackInfo> m_pendingCallbackTracker; // guarded by m_mutex
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
    TRACE_VERBOSE("RunContextState[id=%u]::RunContextState", m_id);
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
    TRACE_VERBOSE("RunContextState[id=%u]::Derive", m_id);
    if (m_terminated)
    {
        TRACE_WARNING("RunContextState[id=%u]::Derive called AFTER termination! (depth=%u)", m_id, m_depth);
    }
    SharedPtr<RunContextState> derived = MakeShared<RunContextState>(m_queue.DeriveWorkQueue(), m_cancellationToken.Derive(), shared_from_this());
    AppendChild(derived);
    return derived;
}

SharedPtr<RunContextState> RunContextState::DeriveOnQueue(XTaskQueueHandle queueHandle) noexcept
{
    TRACE_VERBOSE("RunContextState[id=%u]::DeriveOnQueue", m_id);
    if (m_terminated)
    {
        TRACE_WARNING("RunContextState[id=%u]::DeriveOnQueue called AFTER termination! (depth=%u)", m_id, m_depth);
    }
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
    uint32_t submitId{ 0 };
    const char* typeName{ nullptr };
};

void RunContextState::TaskQueueSubmitCallback(XTaskQueuePort port, SharedPtr<ITaskQueueWork> work, uint32_t delayInMs) noexcept
{
    assert(work);
    assert(m_queue.Handle());

#if defined(__cpp_rtti) || defined(_CPPRTTI)
    auto& workRef = *work;
    const char* workTypeName = typeid(workRef).name();
#else
    const char* workTypeName = "ITaskQueueWork";
#endif
    uint32_t submitId = m_nextSubmitId++;

    Allocator<XTaskQueueCallbackContext> a;
    XTaskQueueCallbackContext* context = new (a.allocate(1)) XTaskQueueCallbackContext{ shared_from_this(), std::move(work), submitId, workTypeName }; // reclaimed in TaskQueueCallback

    std::unique_lock<std::mutex> lock{ m_mutex };
    ++m_pendingTaskQueueCallbacks;
    bool isTerminated = m_terminated;
    size_t currentCount = m_pendingTaskQueueCallbacks;

    // Track pending callbacks for diagnostics
    m_pendingCallbackTracker.push_back(PendingCallbackInfo{ submitId, workTypeName, port, delayInMs });

    lock.unlock();

    if (isTerminated)
    {
        TRACE_WARNING("RunContextState[id=%u] TaskQueue callback SUBMITTED AFTER TERMINATION: submitId=%u, type='%s', port=%d, delay=%ums, pending=%zu, depth=%u",
            m_id, submitId, workTypeName, static_cast<int>(port), delayInMs, currentCount, m_depth);
    }
    else if (delayInMs > 0)
    {
        TRACE_INFORMATION("RunContextState[id=%u] TaskQueue submit: id=%u, type='%s', DELAY=%ums, port=%d, pending=%zu",
            m_id, submitId, workTypeName, delayInMs, static_cast<int>(port), currentCount);
    }
    else
    {
        TRACE_VERBOSE("RunContextState[id=%u] TaskQueue submit: id=%u, type='%s', pending=%zu", m_id, submitId, workTypeName, currentCount);
    }

    HRESULT hr = XTaskQueueSubmitDelayedCallback(m_queue.Handle(), port, delayInMs, context, TaskQueueCallback);
    if (FAILED(hr))
    {
        // Treat other errors as cancellations by calling TaskQueueCallback so that the cancellation handler is called and
        // m_pendingTaskQueueCallbacks is updated correctly
        TaskQueueCallback(context, true);

        if (hr == E_ABORT)
        {
            TRACE_WARNING_HR(hr, "XTaskQueueSubmitDelayedCallback failed (expected abort)");
        }
        else
        {
            TRACE_ERROR_HR(hr, "XTaskQueueSubmitDelayedCallback failed with unexpected error");
        }
        assert(hr == E_ABORT); // The only error we expect to ever see here is E_ABORT
    }
}

void CALLBACK RunContextState::TaskQueueCallback(void* c, bool cancelled) noexcept
{
    assert(c);
    UniquePtr<XTaskQueueCallbackContext> callbackContext{ static_cast<XTaskQueueCallbackContext*>(c) };
    assert(callbackContext->runContext && callbackContext->work);

    uint32_t rcId = callbackContext->runContext->m_id;
    uint32_t submitId = callbackContext->submitId;
    const char* typeName = callbackContext->typeName;
    bool isTerminated = callbackContext->runContext->m_terminated;

    if (isTerminated)
    {
        TRACE_WARNING("RunContextState[id=%u] TaskQueueCallback firing during termination: submitId=%u, type='%s', cancelled=%s",
            rcId, submitId, typeName, cancelled ? "true" : "false");
    }
    else
    {
        TRACE_VERBOSE("RunContextState[id=%u] TaskQueueCallback: submitId=%u, cancelled=%s", rcId, submitId, cancelled ? "true" : "false");
    }

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

    // Remove from tracker
    auto it = std::find_if(runContext->m_pendingCallbackTracker.begin(), runContext->m_pendingCallbackTracker.end(),
        [submitId](const PendingCallbackInfo& info) { return info.submitId == submitId; });
    if (it != runContext->m_pendingCallbackTracker.end())
    {
        runContext->m_pendingCallbackTracker.erase(it);
    }

    if (runContext->m_terminated)
    {
        TRACE_WARNING("RunContextState[id=%u] TaskQueueCallback complete (submitId=%u): %zu callbacks remaining, %zu terminables remaining (depth=%u)",
            runContext->m_id, submitId, runContext->m_pendingTaskQueueCallbacks, runContext->m_pendingTerminations, runContext->m_depth);
    }
    else
    {
        TRACE_VERBOSE("RunContextState[id=%u] TaskQueueCallback complete (submitId=%u), %zu remaining", runContext->m_id, submitId, runContext->m_pendingTaskQueueCallbacks);
    }

    CheckTerminationAndNotifyListener(std::move(runContext), std::move(lock));
}

void RunContextState::TaskQueueTerminate() noexcept
{
    std::unique_lock<std::mutex> lock{ m_mutex };

    if (m_queueTerminated)
    {
        // Early out if the queue has already been terminated. Check needed because unlike XTaskQueueSubmitDelayedCallback, 
        // XTaskQueueTerminate will succeed even after the queue has been terminated (creating another asynchronous callback that could
        // run beyond RunContext::Terminate completing in some cases)
        return;
    }

    // Track termination callbacks together with work callbacks. Because XTaskQueueTerminate doesn't guarantee that all submitted work callbacks 
    // have completed before invoking termination callbacks (it only guarantees that they've been started), we need additional tracking to ensure
    // all work has completed during RunContext termination
    ++m_pendingTaskQueueCallbacks;
    m_queueTerminated = true;

    // Track the queue-terminate sentinel in the pending list
    uint32_t terminateSubmitId = m_nextSubmitId++;
    m_pendingCallbackTracker.push_back(PendingCallbackInfo{ terminateSubmitId, "XTaskQueueTerminate_sentinel", XTaskQueuePort::Work, 0 });
    lock.unlock();

    TRACE_WARNING("RunContextState[id=%u] TaskQueue terminating, %zu callbacks pending (including %zu pre-existing work items)",
        m_id, m_pendingTaskQueueCallbacks, m_pendingCallbackTracker.size() - 1);

    assert(m_queue.Handle());
    Allocator<XTaskQueueCallbackContext> a;
    XTaskQueueCallbackContext* context = new (a.allocate(1)) XTaskQueueCallbackContext{ shared_from_this(), nullptr, terminateSubmitId, "XTaskQueueTerminate_sentinel" }; // reclaimed in TaskQueueTerminated

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

    uint32_t submitId = callbackContext->submitId;
    SharedPtr<RunContextState> runContext{ std::move(callbackContext->runContext) };
    callbackContext.reset();

    std::unique_lock<std::mutex> lock{ runContext->m_mutex };
    --runContext->m_pendingTaskQueueCallbacks;

    // Remove sentinel from tracker
    auto it = std::find_if(runContext->m_pendingCallbackTracker.begin(), runContext->m_pendingCallbackTracker.end(),
        [submitId](const PendingCallbackInfo& info) { return info.submitId == submitId; });
    if (it != runContext->m_pendingCallbackTracker.end())
    {
        runContext->m_pendingCallbackTracker.erase(it);
    }

    TRACE_WARNING("RunContextState[id=%u] TaskQueueTerminated callback fired: %zu pending callbacks remaining, %zu pending terminables (depth=%u)",
        runContext->m_id, runContext->m_pendingTaskQueueCallbacks, runContext->m_pendingTerminations, runContext->m_depth);
    for (auto const& info : runContext->m_pendingCallbackTracker)
    {
        TRACE_WARNING("RunContextState[id=%u]   STILL PENDING after queue terminated: submitId=%u, type='%s', port=%d, delay=%ums",
            runContext->m_id, info.submitId, info.typeName, static_cast<int>(info.port), info.delayInMs);
    }

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
    
    TRACE_VERBOSE("RunContextState[id=%u] terminating with %zu terminables", m_id, m_pendingTerminations);
    TRACE_WARNING("RunContextState[id=%u] terminating: %zu registered terminables, %zu children, depth=%u",
        m_id, m_terminables.size(), children.size(), m_depth);
    for (auto& child : children)
    {
        TRACE_WARNING("RunContextState[id=%u]   child: RunContextState[id=%u] (depth=%u)", m_id, child->m_id, child->m_depth);
    }

    // context will ensure our lifetime until Termination completes.
    TerminationContext* context = MakeUnique<TerminationContext>(shared_from_this()).release(); // reclaimed in OnTerminated;

    // Release state lock but intentionally hold terminationLock while notifying terminables to avoid races with unregister
    lock.unlock();

    // Cancel the CancellationToken to signal all registered listeners (e.g. XAsyncOperationBase instances)
    // to abort. This ensures in-flight internal operations (like token refresh HTTP calls) are force-cancelled
    // and don't block termination indefinitely.
    m_cancellationToken.Cancel();

    // Terminate Queue, registered terminables, and children
    TaskQueueTerminate();

    for (auto& terminable : m_terminables)
    {
        terminable->Terminate(*this, context);
    }

    for (auto& child : children)
    {
        TRACE_WARNING("RunContextState[id=%u] terminating child RunContextState[id=%u]", m_id, child->m_id);
        child->Terminate(*this, context);
    }

    // Now that terminables have been notified, we can release the termination mutex. If termination is complete,
    // the following OnTerminated call may cause 'this' to be destroyed.
    terminationLock.unlock();

    // Dummy termination task added above complete
    OnTerminated(context);
}

void RunContextState::OnTerminated(void* c) noexcept
{
    std::unique_lock<std::mutex> lock{ m_mutex };

    assert(m_pendingTerminations);
    --m_pendingTerminations;
    TRACE_WARNING("RunContextState[id=%u] terminable/child terminated, %zu terminables remaining, %zu callbacks remaining (depth=%u)",
        m_id, m_pendingTerminations, m_pendingTaskQueueCallbacks, m_depth);

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

    if (runContext->m_terminationListener && runContext->m_pendingTaskQueueCallbacks)
    {
        TRACE_WARNING("RunContextState[id=%u] Termination BLOCKED: %zu pending TaskQueue callbacks (depth=%u, queueTerminated=%s)",
            runContext->m_id, runContext->m_pendingTaskQueueCallbacks, runContext->m_depth,
            runContext->m_queueTerminated ? "true" : "false");
        for (auto const& info : runContext->m_pendingCallbackTracker)
        {
            TRACE_WARNING("RunContextState[id=%u]   STUCK callback: submitId=%u, type='%s', port=%d, delay=%ums",
                runContext->m_id, info.submitId, info.typeName, static_cast<int>(info.port), info.delayInMs);
        }
    }
    if (runContext->m_terminationListener && runContext->m_pendingTerminations)
    {
        TRACE_WARNING("RunContextState[id=%u] Termination BLOCKED: %zu pending terminables (depth=%u)",
            runContext->m_id, runContext->m_pendingTerminations, runContext->m_depth);
    }

    if (runContext->m_terminationListener && !runContext->m_pendingTaskQueueCallbacks && !runContext->m_pendingTerminations)
    {
        // Move listener and listenerContext to the stack before releasing runContext, it may be destroyed here
        ITerminationListener& listener = *runContext->m_terminationListener;
        void* listenerContext = runContext->m_terminationListenerContext;

        // reset m_terminationListener to avoid double notifying in some race scenarios
        runContext->m_terminationListener = nullptr; 

        TRACE_WARNING("RunContextState[id=%u] Termination COMPLETE, notifying listener (depth=%u)", runContext->m_id, runContext->m_depth);

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

#pragma warning( pop )

} // namespace PlayFab

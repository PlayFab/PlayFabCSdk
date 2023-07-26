#pragma once

#include "XTaskQueue.h"
#include "CancellationToken.h"

namespace PlayFab
{

// Interfaces used with RunContext
struct ITerminationListener
{
    virtual ~ITerminationListener() = default;
    virtual void OnTerminated(void* context) = 0;
};

struct ITerminable
{
    virtual ~ITerminable() = default;
    virtual void Terminate(ITerminationListener& listener, void* context) = 0;
};

struct ITaskQueueWork
{
    virtual ~ITaskQueueWork() = default;
    virtual void Run() = 0;
    virtual void WorkCancelled() {}
};

// RunContext is an execution context for asynchronous work in PlayFab. It provides an interface for basic XAsync primitives, as well
// as a way to attach and track arbitrary asynchronous work associated with the RunContext.
// 
// During SDK cleanup, RunContext::Terminate will be called for all remaining RunContexts and all pending async work will be forcibly
// ended. The RunContext will then await its completion (including cleanup of any associated state), and then notify the Terminate caller 
// that everything it is safe to return the the client so they can continue their cleanup.
class RunContext : public ITerminable
{
public:
    // Creates a Root RunContext. Should only done by global state
    static RunContext Root(XTaskQueueHandle queue) noexcept;

    // Derive a new RunContext from another. Subtasks that can be canceled, succeed, or fail independently should derive their own RunContext
    // from their parent's RunContext. The derived RunContext will use a derived TaskQueue as well.
    RunContext Derive() noexcept;

    // Derive a new RunContext from another, but use an independent TaskQueue rather than a derived one. This is typically used when a client has
    // specifed a specific queue for a given piece of work (ex. a public XAsync API call or a client callback that was registered with a queue)
    RunContext DeriveOnQueue(XTaskQueueHandle queueHandle) noexcept;

    RunContext(RunContext const&) noexcept = default;
    RunContext(RunContext&&) noexcept = default;
    RunContext& operator=(RunContext const&) noexcept = delete;
    RunContext& operator=(RunContext&&) noexcept = delete;
    ~RunContext() noexcept = default;

public: // XTaskQueue shims
    // Retreive an XTaskQueueHandle to be passed to XAsyncAsync APIs
    XTaskQueueHandle TaskQueueHandle() const noexcept;

    // Submits an ITaskQueueWork object to the TaskQueue work port
    void TaskQueueSubmitWork(SharedPtr<ITaskQueueWork> work, uint32_t delayInMs = 0) const noexcept;

    // Submits an arbitrary lambda to the TaskQueue work port
    template<typename TCallback, typename std::enable_if_t<!std::is_assignable_v<SharedPtr<ITaskQueueWork>, TCallback>, bool> = true>
    void TaskQueueSubmitWork(TCallback work, uint32_t delayInMs = 0) const noexcept;

    // Submits an ITaskQueueWork object to the TaskQueue completion port
    void TaskQueueSubmitCompletion(SharedPtr<ITaskQueueWork> completion) const noexcept;

    // Submits an arbitrary lambda to the TaskQueue completion port
    template<typename TCallback, typename std::enable_if_t<!std::is_assignable_v<SharedPtr<ITaskQueueWork>, TCallback>, bool> = true>
    void TaskQueueSubmitCompletion(TCallback completion) const noexcept;

    // Terminates the XTaskQueue. Can be used to cancel all work submitted to the TaskQueue. Note however that this will also cancel work submitted
    // to derived RunContext's TaskQueue
    void TaskQueueTerminate() noexcept;

public:
    // CancellationToken that can be used to cancel associated work and/or be notified when a cancellation request has been made
    PlayFab::CancellationToken CancellationToken() const noexcept;

    // Registers an arbitrary piece of work with the RunContext. During Termination, if the work has not yet been unregistered,
    // the RunContext will alert the registered terminable that the client has requested cleanup and all work should be terminated
    // as soon as possible.
    bool RegisterTerminableAndCheck(ITerminable& terminable) noexcept;

    // Unregisters a previously registered terminable. Typically done when the work has completed and has cleaned up any associated state.
    bool UnregisterTerminableAndCheck(ITerminable& terminable) noexcept;

    // Terminates the TaskQueue, any terminable registered with RegisterTerminableAndCheck, and any Derived RunContexts. The provided
    // ITerminationListener will be notified upon completion.
    // 
    // Terminate should only called during PFCleanup. Allowing multiple terminations if a single RunContext prevents the RunContext from
    // guaranteeing that it has completely cleaning up its state prior to each listener being notified. 
    void Terminate(ITerminationListener& listener, void* context) noexcept override;

private:
    RunContext(SharedPtr<class RunContextState> state) noexcept;

    SharedPtr<class RunContextState> m_state;
};

//------------------------------------------------------------------------------
// Template implementations
//------------------------------------------------------------------------------

namespace Detail
{

// Wrapper to enable submitting arbitrary lambdas to TaskQueues
template<typename TCallback>
class TaskQueueWork : public ITaskQueueWork
{
public:
    TaskQueueWork(TCallback&& callback) : m_callback{ std::move(callback) } 
    {
    }
    
private:
    void Run() override
    {
        m_callback();
    }

    TCallback m_callback;
};

} // namespace Detail

template<typename TCallback, typename std::enable_if_t<!std::is_assignable_v<SharedPtr<ITaskQueueWork>, TCallback>, bool>>
void RunContext::TaskQueueSubmitWork(TCallback work, uint32_t delayInMs) const noexcept
{
    TaskQueueSubmitWork(MakeShared<Detail::TaskQueueWork<TCallback>>(std::move(work)), delayInMs);
}

template<typename TCallback, typename std::enable_if_t<!std::is_assignable_v<SharedPtr<ITaskQueueWork>, TCallback>, bool>>
void RunContext::TaskQueueSubmitCompletion(TCallback completion) const noexcept
{
    TaskQueueSubmitCompletion(MakeShared<Detail::TaskQueueWork<TCallback>>(std::move(completion)));
}

}

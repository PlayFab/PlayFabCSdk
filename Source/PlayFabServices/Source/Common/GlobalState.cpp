#include "stdafx.h"
#include "GlobalState.h"
#include "Platform.h"

/// <summary>
/// Internal PFCore function to enable PFServices's init to also init'd PFCore with HCInitArgs on demand as needed
/// </summary>
extern "C" PF_API PFInitializeWithLHC(
    _In_opt_ XTaskQueueHandle backgroundQueue,
    _In_ HCInitArgs* args
) noexcept;

namespace PlayFab
{

enum class AccessMode
{
    Initialize,
    Get,
    Cleanup
};

HRESULT ServicesAccessGlobalState(AccessMode mode, SharedPtr<GlobalState>& state)
{
    struct GlobalStateHolder
    {
        SharedPtr<GlobalState> const state;
    };

    static std::atomic<GlobalStateHolder*> s_globalStateHolder{ nullptr };
    assert(s_globalStateHolder.is_lock_free());

    try
    {
        switch (mode)
        {
        case AccessMode::Initialize:
        {
            RETURN_HR_INVALIDARG_IF_NULL(state);

            UniquePtr<GlobalStateHolder> stateHolder{ new (Allocator<GlobalStateHolder>{}.allocate(1)) GlobalStateHolder{ state } };

            GlobalStateHolder* expected{ nullptr };
            if (!s_globalStateHolder.compare_exchange_strong(expected, stateHolder.get()))
            {
                return E_PF_SERVICES_ALREADY_INITIALIZED;
            }

            stateHolder.release(); // reclaimed via AccessMode::Cleanup

            return S_OK;
        }
        case AccessMode::Get:
        {
            GlobalStateHolder* stateHolder = s_globalStateHolder.load();

            RETURN_HR_IF(E_PF_SERVICES_NOT_INITIALIZED, !stateHolder);
            assert(stateHolder->state);
            state = stateHolder->state;

            return S_OK;
        }
        case AccessMode::Cleanup:
        {
            UniquePtr<GlobalStateHolder> stateHolder{ s_globalStateHolder.exchange(nullptr) };

            RETURN_HR_IF(E_PF_SERVICES_NOT_INITIALIZED, !stateHolder);
            state = stateHolder->state;

            return S_OK;
        }
        default:
        {
            assert(false);
            return E_UNEXPECTED;
        }
        }
    }
    catch (...)
    {
        return CurrentExceptionToHR();
    }
}

GlobalState::GlobalState(bool uninitPlayFabCore) noexcept :
    m_runContext{ RunContext::Root(nullptr) }, // No background work for Services SDK currently
    m_uninitPlayFabCore{ uninitPlayFabCore }
{
    TRACE_VERBOSE("PlayFabServices::GlobalState::GlobalState");
}

GlobalState::~GlobalState() noexcept
{
    TRACE_VERBOSE("PlayFabServices::GlobalState::~GlobalState");
}

HRESULT GlobalState::Create(_In_opt_ HCInitArgs* args) noexcept
{
    // Before creating global state, initialize platform hooks
    RETURN_IF_FAILED(PlayFab::InitializePlatformHooks());

    // As a convenience, initialize PFCore on behalf of the title if they haven't done so already.
    // This does mean that the title sacrifices the ability to provide a XTaskQueue for PFCore background work,
    // but it makes some golden path scenarios more streamlined if they don't need that level of control.
    HRESULT hr = PFInitializeWithLHC(nullptr, args);
    RETURN_HR_IF(hr, FAILED(hr) && hr != E_PF_CORE_ALREADY_INITIALIZED);

    Allocator<GlobalState> a{};
    auto state = SharedPtr<GlobalState>{ new (a.allocate(1)) GlobalState{ SUCCEEDED(hr) }, Deleter<GlobalState>(), a};
    return ServicesAccessGlobalState(AccessMode::Initialize, state);
}

HRESULT GlobalState::Get(SharedPtr<GlobalState>& state) noexcept
{
    return ServicesAccessGlobalState(AccessMode::Get, state);
}

struct CleanupContext
{
    SharedPtr<GlobalState> state;
    XAsyncBlock pfCoreCleanupAsyncBlock{};
    XAsyncBlock* clientAsyncBlock{};
};

HRESULT GlobalState::CleanupAsync(XAsyncBlock* async) noexcept
{
    try
    {
        TRACE_VERBOSE("PlayFabServices::GlobalState::CleanupAsync");

        UniquePtr<CleanupContext> context = MakeUnique<CleanupContext>();
        RETURN_IF_FAILED(XAsyncBegin(async, context.get(), __FUNCTION__, __FUNCTION__, CleanupAsyncProvider));
        context.release();
        return S_OK;
    }
    catch (...)
    {
        return CurrentExceptionToHR();
    }
}

HRESULT CALLBACK GlobalState::CleanupAsyncProvider(XAsyncOp op, XAsyncProviderData const* data)
{
    CleanupContext* context{ static_cast<CleanupContext*>(data->context) };

    switch (op)
    {
    case XAsyncOp::Begin:
    try
    {
        TRACE_VERBOSE("PlayFabServices::GlobalState::CleanupAsyncProvider::Begin");

        RETURN_IF_FAILED(ServicesAccessGlobalState(AccessMode::Cleanup, context->state));
        context->clientAsyncBlock = data->async;

        context->state->m_runContext.Terminate(*context->state, context);
        return S_OK;
    }
    catch (...)
    {
        return CurrentExceptionToHR();
    }
    default:
    {
        return S_OK;
    }
    }
}

void CALLBACK PFUninitializeComplete(XAsyncBlock* async)
{
    TRACE_VERBOSE(__FUNCTION__);

    UniquePtr<CleanupContext> context{ static_cast<CleanupContext*>(async->context) };
    XAsyncBlock* asyncBlock{ context->clientAsyncBlock }; // Keep copy of asyncBlock pointer to complete after cleaning up context

    HRESULT hr = XAsyncGetStatus(async, true);

    // Free CleanupContext
    context.reset();

    // GlobalState::Cleanup complete!
    XAsyncComplete(asyncBlock, hr, 0);
}

void GlobalState::OnTerminated(void* c) noexcept
{
    TRACE_VERBOSE(__FUNCTION__);

    UniquePtr<CleanupContext> context{ static_cast<CleanupContext*>(c) };
    XAsyncBlock* asyncBlock{ context->clientAsyncBlock }; // Keep copy of asyncBlock pointer to complete after cleaning up context

    if (context->state->m_uninitPlayFabCore)
    {
        // Free GlobalState before calling PFUninitializeAsync - 'this' will be destroyed here
        context->state.reset();

        // Uninitialize PlayFab Core
        context->pfCoreCleanupAsyncBlock.callback = PFUninitializeComplete;
        context->pfCoreCleanupAsyncBlock.queue = context->clientAsyncBlock->queue; // Should use derived queue
        context->pfCoreCleanupAsyncBlock.context = context.get();

        HRESULT hr = PFUninitializeAsync(&context->pfCoreCleanupAsyncBlock);
        if (FAILED(hr))
        {
            // We never expect this to fail, but complete cleanup if it does
            assert(false); 

            // Free CleanupContext 
            context.reset();

            // Complete GlobalState::Cleanup with failure
            XAsyncComplete(asyncBlock, hr, 0);

            return;
        }

        context.release();
    }
    else
    {
        // Cleanup complete. Flow returns to client now
        context.reset();
        XAsyncComplete(asyncBlock, S_OK, 0);
    }
}

RunContext GlobalState::RunContext() const noexcept
{
    return m_runContext;
}

}

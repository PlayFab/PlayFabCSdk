#include "stdafx.h"
#include "GlobalState.h"
#include "Trace/TraceState.h"
#include <httpClient/httpClient.h>

using namespace PlayFab;

namespace PlayFab
{

namespace Detail
{

// Choose arbitrary but recognizable values for handles
uintptr_t const kFirstServiceConfigHandle = 0x10000000;
uintptr_t const kFirstEventPipelineHandle = 0x30000000;
uintptr_t const kFirstEntityHandle = 0x40000000;

}

enum class AccessMode
{
    Initialize,
    Get,
    Cleanup
};

HRESULT AccessGlobalState(AccessMode mode, SharedPtr<GlobalState>& state)
{
    struct GlobalStateHolder
    {
        SharedPtr<GlobalState> const state;
    };

    static std::atomic<GlobalStateHolder*> s_globalStateHolder{ nullptr }; // intentional non-owning pointer
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
                return E_PF_CORE_ALREADY_INITIALIZED;
            }

            stateHolder.release(); // reclaimed via AccessMode::Cleanup

            return S_OK;
        }
        case AccessMode::Get:
        {
            GlobalStateHolder* stateHolder = s_globalStateHolder.load();

            RETURN_HR_IF(E_PF_CORE_NOT_INITIALIZED, !stateHolder);
            assert(stateHolder->state);
            state = stateHolder->state;

            return S_OK;
        }
        case AccessMode::Cleanup:
        {
            UniquePtr<GlobalStateHolder> stateHolder{ s_globalStateHolder.exchange(nullptr) };

            RETURN_HR_IF(E_PF_CORE_NOT_INITIALIZED, !stateHolder);
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


GlobalState::GlobalState(XTaskQueueHandle backgroundQueue) noexcept :
    m_runContext{ RunContext::Root(backgroundQueue) },
    m_serviceConfigs{ Detail::kFirstServiceConfigHandle },
    m_entities{ Detail::kFirstEntityHandle },
    m_clientEventPipelines{ Detail::kFirstEventPipelineHandle },
    m_httpRetrySettings{ MakeShared<PFHttpRetrySettings>() }
{
    TRACE_VERBOSE("PlayFabCore::GlobalState::GlobalState");

    m_httpRetrySettings->allowRetry = kDefaultHttpRetryAllowed;
    m_httpRetrySettings->minimumRetryDelayInSeconds = kDefaultHttpRetryDelay;
    m_httpRetrySettings->timeoutWindowInSeconds = kDefaultHttpTimeoutWindow;
}

GlobalState::~GlobalState() noexcept
{
    TRACE_VERBOSE("PlayFabCore::GlobalState::~GlobalState");
}

HRESULT GlobalState::Create(XTaskQueueHandle backgroundQueue, HCInitArgs* args) noexcept
{
    // Set up tracing before doing anything else
    // LocalStorage not needed anywhere else currently so create an instance just for TraceState. If
    // it is needed elsewhere, there should be a single shared instance hanging off of GlobalState
    RETURN_IF_FAILED(TraceState::Create(RunContext::Root(backgroundQueue), LocalStorage()));

    RETURN_IF_FAILED(HCInitialize(args));

    Allocator<GlobalState> a{};
    SharedPtr<GlobalState> state = SharedPtr<GlobalState>{ new (a.allocate(1)) GlobalState{ backgroundQueue }, Deleter<GlobalState>(), a };
    return AccessGlobalState(AccessMode::Initialize, state);
}

HRESULT GlobalState::Get(SharedPtr<GlobalState>& state) noexcept
{
    return AccessGlobalState(AccessMode::Get, state);
}

struct CleanupContext 
{
    SharedPtr<GlobalState> state;
    XAsyncBlock lhcCleanupAsyncBlock{};
    XAsyncBlock traceStateCleanupAsyncBlock{};
    XAsyncBlock* clientAsyncBlock{};
};

HRESULT GlobalState::CleanupAsync(XAsyncBlock* async) noexcept
{
    try
    {
        TRACE_VERBOSE("PlayFabCore::GlobalState::CleanupAsync");

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
        TRACE_VERBOSE("PlayFabCore::GlobalState::CleanupAsyncProvider::Begin");

        RETURN_IF_FAILED(AccessGlobalState(AccessMode::Cleanup, context->state));
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

void CALLBACK TraceStateCleanupComplete(XAsyncBlock* async)
{
    // This is the last step of cleanup, so reclaim the context
    UniquePtr<CleanupContext> context{ static_cast<CleanupContext*>(async->context) };
    XAsyncBlock* asyncBlock{ context->clientAsyncBlock }; // Keep copy of asyncBlock pointer to complete after cleaning up context

    HRESULT hr = XAsyncGetStatus(async, true);

    // Free CleanupContext
    context.reset();

    // GlobalState::Cleanup complete!
    XAsyncComplete(asyncBlock, hr, 0);
}

void CALLBACK HCCleanupComplete(XAsyncBlock* async)
{
    TRACE_VERBOSE(__FUNCTION__);

    UniquePtr<CleanupContext> context{ static_cast<CleanupContext*>(async->context) };
    XAsyncBlock* asyncBlock{ context->clientAsyncBlock }; // Keep copy of asyncBlock pointer to complete after cleaning up context

    HRESULT hr = XAsyncGetStatus(async, true);
    if (hr == E_HC_INTERNAL_STILLINUSE)
    {
        // If something else is still referencing libHttpClient we don't care
        hr = S_OK;
    }

    if (FAILED(hr))
    {
        TRACE_ERROR_HR(hr, "HCCleanupAsync Failed unexpectedly!");

        // Free CleanupContext 
        context.reset();

        // Complete GlobalState::Cleanup with failure
        XAsyncComplete(asyncBlock, hr, 0);

        return;
    }

    context->traceStateCleanupAsyncBlock.callback = TraceStateCleanupComplete;
    context->traceStateCleanupAsyncBlock.queue = context->clientAsyncBlock->queue; // Should use derived queue
    context->traceStateCleanupAsyncBlock.context = context.get();

    hr = TraceState::CleanupAsync(&context->traceStateCleanupAsyncBlock);
    if (FAILED(hr))
    {
        assert(false);

        // Free CleanupContext 
        context.reset();

        // Complete GlobalState::Cleanup with failure
        XAsyncComplete(asyncBlock, hr, 0);

        return;
    }

    context.release();
}

void GlobalState::OnTerminated(void* c) noexcept
{
    TRACE_VERBOSE(__FUNCTION__);

    UniquePtr<CleanupContext> context{ static_cast<CleanupContext*>(c) };
    XAsyncBlock* asyncBlock{ context->clientAsyncBlock }; // Keep copy of asyncBlock pointer to complete after cleaning up context

    // Free GlobalState before calling HCCleanup - 'this' will be destroyed here
    context->state.reset();

    context->lhcCleanupAsyncBlock.callback = HCCleanupComplete;
    context->lhcCleanupAsyncBlock.queue = context->clientAsyncBlock->queue; // Should use derived queue
    context->lhcCleanupAsyncBlock.context = context.get();

    HRESULT hr = HCCleanupAsync(&context->lhcCleanupAsyncBlock);
    if (FAILED(hr))
    {
        assert(false);

        // Free CleanupContext 
        context.reset();

        // Complete GlobalState::Cleanup with failure
        XAsyncComplete(asyncBlock, hr, 0);

        return;
    }

    context.release();
}

RunContext GlobalState::RunContext() const noexcept
{
    return m_runContext;
}

ServiceConfigHandleTable& GlobalState::ServiceConfigs() noexcept
{
    return m_serviceConfigs;
}

EntityHandleTable& GlobalState::Entities() noexcept
{
    return m_entities;
}

EventPipelineHandleTable& GlobalState::ClientEventPipelines() noexcept
{
    return m_clientEventPipelines;
}

TokenExpiredHandler GlobalState::TokenExpiredHandler() const noexcept
{
    return m_tokenExpiredHandler;
}

TokenRefreshedHandler GlobalState::TokenRefreshedHandler() const noexcept
{
    return m_tokenRefreshedHandler;
}

SharedPtr<PFHttpRetrySettings> GlobalState::HttpRetrySettings() const noexcept
{
    return m_httpRetrySettings;
}

} // namespace PlayFab

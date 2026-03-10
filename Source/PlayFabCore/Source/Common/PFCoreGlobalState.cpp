#include "stdafx.h"
#include "PFCoreGlobalState.h"
#include "LocalUserCache.h"
#include "Platform/Platform.h"
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
uintptr_t const kFirstLocalUserHandle = 0x50000000;

}

enum class AccessMode
{
    Initialize,
    Get,
    Cleanup
};

HRESULT AccessPFCoreGlobalState(AccessMode mode, SharedPtr<PFCoreGlobalState>& state)
{
    struct PFCoreGlobalStateHolder
    {
        SharedPtr<PFCoreGlobalState> const state;
    };

    static std::atomic<PFCoreGlobalStateHolder*> s_PFCoreGlobalStateHolder{ nullptr }; // intentional non-owning pointer
    assert(s_PFCoreGlobalStateHolder.is_lock_free());

    try
    {

        switch (mode)
        {
        case AccessMode::Initialize:
        {
            RETURN_HR_INVALIDARG_IF_NULL(state);

            UniquePtr<PFCoreGlobalStateHolder> stateHolder{ new (Allocator<PFCoreGlobalStateHolder>{}.allocate(1)) PFCoreGlobalStateHolder{ state } };

            PFCoreGlobalStateHolder* expected{ nullptr };
            if (!s_PFCoreGlobalStateHolder.compare_exchange_strong(expected, stateHolder.get()))
            {
                return E_PF_CORE_ALREADY_INITIALIZED;
            }

            stateHolder.release(); // reclaimed via AccessMode::Cleanup

            return S_OK;
        }
        case AccessMode::Get:
        {
            PFCoreGlobalStateHolder* stateHolder = s_PFCoreGlobalStateHolder.load();

            RETURN_HR_IF(E_PF_CORE_NOT_INITIALIZED, !stateHolder);
            assert(stateHolder->state);
            state = stateHolder->state;

            return S_OK;
        }
        case AccessMode::Cleanup:
        {
            UniquePtr<PFCoreGlobalStateHolder> stateHolder{ s_PFCoreGlobalStateHolder.exchange(nullptr) };

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


PFCoreGlobalState::PFCoreGlobalState(XTaskQueueHandle backgroundQueue) noexcept :
    m_runContext{ RunContext::Root(backgroundQueue) },
    m_serviceConfigs{ Detail::kFirstServiceConfigHandle },
    m_localUsers{ Detail::kFirstLocalUserHandle },
    m_entities{ Detail::kFirstEntityHandle },
    m_clientEventPipelines{ Detail::kFirstEventPipelineHandle },
    m_httpRetrySettings{ MakeShared<PFHttpRetrySettings>() },
    m_httpSettings{ MakeShared<PFHttpSettings>() }
{
    TRACE_VERBOSE("PlayFabCore::PFCoreGlobalState::PFCoreGlobalState");

    m_httpRetrySettings->allowRetry = kDefaultHttpRetryAllowed;
    m_httpRetrySettings->minimumRetryDelayInSeconds = kDefaultHttpRetryDelay;
    m_httpRetrySettings->timeoutWindowInSeconds = kDefaultHttpTimeoutWindow;
    m_httpSettings->requestResponseCompression = kDefaultHttpCompressedResponsesExpected;
}

PFCoreGlobalState::~PFCoreGlobalState() noexcept
{
    TRACE_VERBOSE("PlayFabCore::PFCoreGlobalState::~PFCoreGlobalState");
}

HRESULT PFCoreGlobalState::Create(XTaskQueueHandle backgroundQueue, HCInitArgs* args) noexcept
{
    try
    {
        // Initialize any platform hooks. This happens first because these hooks may be used after this point
        RETURN_IF_FAILED(PlatformInitialize());

        // Next set up tracing so that we can trace as much of initialization as possible.
        // LocalStorage not needed anywhere else currently so create an instance just for TraceState. If
        // it is needed elsewhere, there should be a single shared instance hanging off of PFCoreGlobalState
        RETURN_IF_FAILED(TraceState::Create(RunContext::Root(backgroundQueue), LocalStorage()));

        RETURN_IF_FAILED(HCInitialize(args));

        PFPlatformType platformType;
        PlatformInfo platformInfo{};
        RETURN_IF_FAILED(PlatformGetPlatformType(platformInfo, platformType));

        Allocator<PFCoreGlobalState> a{};
        SharedPtr<PFCoreGlobalState> state = SharedPtr<PFCoreGlobalState>{ new (a.allocate(1)) PFCoreGlobalState{ backgroundQueue }, Deleter<PFCoreGlobalState>(), a };
        state->m_platformType = platformType;
        state->m_isGRTSAvailable = (platformInfo & PlatformInfo::GRTSAvailable) != 0;

#if HC_PLATFORM == HC_PLATFORM_ANDROID
        auto platformInitResult = PlatformComponents_Android::Initialize(args->javaVM, args->applicationContext);
        RETURN_IF_FAILED(platformInitResult.hr);
        state->m_platformComponents = platformInitResult.ExtractPayload();
#endif

        return AccessPFCoreGlobalState(AccessMode::Initialize, state);
    }
    catch (...)
    {
        TRACE_WARNING("[0x%08X] Exception thrown in PFCoreGlobalState::Create\n    %s:%u", E_FAIL, __FILE__, __LINE__);
        return CurrentExceptionToHR();
    }
}

HRESULT PFCoreGlobalState::Get(SharedPtr<PFCoreGlobalState>& state) noexcept
{
    return AccessPFCoreGlobalState(AccessMode::Get, state);
}

struct CleanupContext 
{
    SharedPtr<PFCoreGlobalState> state;
    XAsyncBlock lhcCleanupAsyncBlock{};
    XAsyncBlock traceStateCleanupAsyncBlock{};
    XAsyncBlock* clientAsyncBlock{};
};

HRESULT PFCoreGlobalState::CleanupAsync(XAsyncBlock* async) noexcept
{
    try
    {
        TRACE_VERBOSE("PlayFabCore::PFCoreGlobalState::CleanupAsync");

        Allocator<PFCoreGlobalState> a{};
        UniquePtr<CleanupContext> context = UniquePtr<CleanupContext>(new (a.allocate(1)) CleanupContext{});
        RETURN_IF_FAILED(XAsyncBegin(async, context.get(), __FUNCTION__, __FUNCTION__, CleanupAsyncProvider));
        context.release();
        return S_OK;
    }
    catch (...)
    {
        return CurrentExceptionToHR();
    }
}

HRESULT CALLBACK PFCoreGlobalState::CleanupAsyncProvider(XAsyncOp op, XAsyncProviderData const* data)
{
    switch (op)
    {
    case XAsyncOp::Begin:
    try
    {
        TRACE_VERBOSE("PlayFabCore::PFCoreGlobalState::CleanupAsyncProvider::Begin");
        UniquePtr<CleanupContext> context{ static_cast<CleanupContext*>(data->context) };

        RETURN_IF_FAILED(AccessPFCoreGlobalState(AccessMode::Cleanup, context->state));
        context->clientAsyncBlock = data->async;

        // Clear the local user cache early in the cleanup process to prevent resource leaks
        Core::LocalUserCache::Instance().ClearAllLocalUsers();

        context->state->m_runContext.Terminate(*context->state, context.get());
        context.release();
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

    // PFCoreGlobalState::Cleanup complete!
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

        // Complete PFCoreGlobalState::Cleanup with failure
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

        // Complete PFCoreGlobalState::Cleanup with failure
        XAsyncComplete(asyncBlock, hr, 0);

        return;
    }

    context.release();
}

void PFCoreGlobalState::OnTerminated(void* c) noexcept
{
    TRACE_VERBOSE(__FUNCTION__);

    UniquePtr<CleanupContext> context{ static_cast<CleanupContext*>(c) };
    XAsyncBlock* asyncBlock{ context->clientAsyncBlock }; // Keep copy of asyncBlock pointer to complete after cleaning up context

    // Free PFCoreGlobalState before calling HCCleanup - 'this' will be destroyed here
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

        // Complete PFCoreGlobalState::Cleanup with failure
        XAsyncComplete(asyncBlock, hr, 0);

        return;
    }

    context.release();
}

RunContext PFCoreGlobalState::RunContext() const noexcept
{
    return m_runContext;
}

ServiceConfigHandleTable& PFCoreGlobalState::ServiceConfigs() noexcept
{
    return m_serviceConfigs;
}

LocalUserHandleTable& PFCoreGlobalState::LocalUsers() noexcept
{
    return m_localUsers;
}

EntityHandleTable& PFCoreGlobalState::Entities() noexcept
{
    return m_entities;
}

EventPipelineHandleTable& PFCoreGlobalState::ClientEventPipelines() noexcept
{
    return m_clientEventPipelines;
}

TokenExpiredHandler PFCoreGlobalState::TokenExpiredHandler() const noexcept
{
    return m_tokenExpiredHandler;
}

TokenRefreshedHandler PFCoreGlobalState::TokenRefreshedHandler() const noexcept
{
    return m_tokenRefreshedHandler;
}

SharedPtr<PFHttpRetrySettings> PFCoreGlobalState::HttpRetrySettings() const noexcept
{
    return m_httpRetrySettings;
}

SharedPtr<PFHttpSettings> PFCoreGlobalState::HttpSettings() const noexcept
{
    return m_httpSettings;
}

PFPlatformType const PFCoreGlobalState::GetPlatformType() const noexcept
{
    return m_platformType;
}

#if HC_PLATFORM == HC_PLATFORM_GDK
PFXPALGameSaveContext* PFCoreGlobalState::GameSaveContext() noexcept
{
    return &m_gamesave;
}
#endif

bool PFCoreGlobalState::IsGRTSAvailable() const noexcept
{
    return m_isGRTSAvailable;
}

#if HC_PLATFORM == HC_PLATFORM_ANDROID
SharedPtr<PlatformComponents_Android> PFCoreGlobalState::PlatformComponents() const noexcept
{
    return m_platformComponents;
}
#endif

} // namespace PlayFab

// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include <playfab/core/cpp/LocalUser.h>
#include "Platform/Platform.h"

/// <summary>
/// Internal PFCore function to enable PFGameSave's init to also initialized PFCore with HCInitArgs on demand as needed
/// </summary>
extern "C" PF_API PFInitializeWithLHC(
    _In_opt_ XTaskQueueHandle backgroundQueue,
    _In_opt_ HCInitArgs* args
) noexcept;

namespace PlayFab
{
namespace GameSave
{

namespace Detail
{

    // Choose arbitrary but recognizable values for handles
    uintptr_t const kFirstEntityHandle = 0x40000000;
}

enum class AccessMode
{
    Initialize,
    Get,
    Cleanup
};

HRESULT GameSaveAccessGlobalState(AccessMode mode, SharedPtr<GameSaveGlobalState>& state)
{
    struct GlobalStateHolder
    {
        SharedPtr<GameSaveGlobalState> const state;
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
                return E_PF_GAMESAVE_ALREADY_INITIALIZED;
            }

            stateHolder.release(); // reclaimed via AccessMode::Cleanup

            return S_OK;
        }
        case AccessMode::Get:
        {
            GlobalStateHolder* stateHolder = s_globalStateHolder.load();

            RETURN_HR_IF(E_PF_GAMESAVE_NOT_INITIALIZED, !stateHolder);
            assert(stateHolder->state);
            state = stateHolder->state;

            return S_OK;
        }
        case AccessMode::Cleanup:
        {
            UniquePtr<GlobalStateHolder> stateHolder{ s_globalStateHolder.exchange(nullptr) };

            RETURN_HR_IF(E_PF_GAMESAVE_NOT_INITIALIZED, !stateHolder);
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

GameSaveGlobalState::GameSaveGlobalState(bool uninitPlayFabCore, _In_opt_ XTaskQueueHandle backgroundQueue) noexcept :
    m_runContext{ RunContext::Root(backgroundQueue) },
    m_uninitPlayFabCore{ uninitPlayFabCore }
{
    TRACE_VERBOSE("PFGameSave::GameSaveGlobalState::GameSaveGlobalState");
}

GameSaveGlobalState::~GameSaveGlobalState() noexcept
{
    TRACE_VERBOSE("PFGameSave::GameSaveGlobalState::~GameSaveGlobalState");
}

HRESULT GameSaveGlobalState::Create(_In_opt_ HCInitArgs* args, _In_ uint64_t options, _In_opt_ XTaskQueueHandle backgroundQueue) noexcept
{
    // Before creating global state, initialize platform hooks
    RETURN_IF_FAILED(GameSaveInitializePlatformHooks());

    // As a convenience, initialize PFCore on behalf of the title if they haven't done so already.
    // This does mean that the title sacrifices the ability to provide a XTaskQueue for PFCore background work,
    // but it makes some golden path scenarios more streamlined if they don't need that level of control.
    HRESULT hr = PFInitializeWithLHC(nullptr, args);
    RETURN_HR_IF(hr, FAILED(hr) && hr != E_PF_CORE_ALREADY_INITIALIZED);

    Allocator<GameSaveGlobalState> a{};
    auto state = SharedPtr<GameSaveGlobalState>{ new (a.allocate(1)) GameSaveGlobalState{ SUCCEEDED(hr), backgroundQueue }, Deleter<GameSaveGlobalState>(), a};

    bool forceInproc = false;
    // If we need to force inproc game saves via flag at some future point, this is the spot
    // For now, its always false and controlled via regkey:
    // HKLM\\SOFTWARE\\Microsoft\\GamingServices\\ForceUseInprocGameSaves == 1
    UNREFERENCED_PARAMETER(options);
    state->m_apiProvider = PlatformGetAPIProvider(forceInproc);

    return GameSaveAccessGlobalState(AccessMode::Initialize, state);
}

HRESULT GameSaveGlobalState::Get(SharedPtr<GameSaveGlobalState>& state) noexcept
{
    return GameSaveAccessGlobalState(AccessMode::Get, state);
}

struct CleanupContext
{
    SharedPtr<GameSaveGlobalState> state;
    XAsyncBlock pfCoreCleanupAsyncBlock{};
    XAsyncBlock* clientAsyncBlock{};
};

HRESULT GameSaveGlobalState::CleanupAsync(XAsyncBlock* async) noexcept
{
    try
    {
        TRACE_VERBOSE("PFGameSave::GameSaveGlobalState::CleanupAsync");

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

HRESULT CALLBACK GameSaveGlobalState::CleanupAsyncProvider(XAsyncOp op, XAsyncProviderData const* data)
{
    CleanupContext* context{ static_cast<CleanupContext*>(data->context) };

    switch (op)
    {
    case XAsyncOp::Begin:
    try
    {
        TRACE_VERBOSE("PFGameSave::GameSaveGlobalState::CleanupAsyncProvider::Begin");

        RETURN_IF_FAILED(GameSaveAccessGlobalState(AccessMode::Cleanup, context->state));
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

void CALLBACK GameSavePFUninitializeComplete(XAsyncBlock* async)
{
    TRACE_VERBOSE(__FUNCTION__);

    UniquePtr<CleanupContext> context{ static_cast<CleanupContext*>(async->context) };
    XAsyncBlock* asyncBlock{ context->clientAsyncBlock }; // Keep copy of asyncBlock pointer to complete after cleaning up context

    HRESULT hr = XAsyncGetStatus(async, true);

    // Free CleanupContext
    context.reset();

    // GameSaveGlobalState::Cleanup complete!
    XAsyncComplete(asyncBlock, hr, 0);
}

void GameSaveGlobalState::OnTerminated(void* c) noexcept
{
    TRACE_VERBOSE(__FUNCTION__);

    UniquePtr<CleanupContext> context{ static_cast<CleanupContext*>(c) };
    XAsyncBlock* asyncBlock{ context->clientAsyncBlock }; // Keep copy of asyncBlock pointer to complete after cleaning up context

    if (context->state->m_uninitPlayFabCore)
    {
        // Free GameSaveGlobalState before calling PFUninitializeAsync - 'this' will be destroyed here
        context->state.reset();

        // Uninitialize PlayFab Core
        context->pfCoreCleanupAsyncBlock.callback = GameSavePFUninitializeComplete;
        context->pfCoreCleanupAsyncBlock.queue = context->clientAsyncBlock->queue; // Should use derived queue
        context->pfCoreCleanupAsyncBlock.context = context.get();

        HRESULT hr = PFUninitializeAsync(&context->pfCoreCleanupAsyncBlock);
        if (FAILED(hr))
        {
            // We never expect this to fail, but complete cleanup if it does
            assert(false); 

            // Free CleanupContext 
            context.reset();

            // Complete GameSaveGlobalState::Cleanup with failure
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

SharedPtr<FolderSyncManager> GameSaveGlobalState::GetFolderSyncManagerFromLocalUser(PFLocalUserHandle handle, bool createOnDemand)
{
    LocalUser user = LocalUser::Duplicate(handle);
    String localId = user.LocalId();

    auto it = m_managers.find(localId);
    if (it == m_managers.end())
    {
        if (createOnDemand)
        {
            SharedPtr<FolderSyncManager> folderSync = MakeShared<FolderSyncManager>(user);
            m_managers[localId] = folderSync;
            return folderSync;
        }
        else
        {
            return nullptr;
        }
    }
    else
    {
        return it->second;
    }
}

RunContext GameSaveGlobalState::RunContext() const noexcept
{
    return m_runContext;
}

GameSaveAPIProvider& GameSaveGlobalState::ApiProvider() noexcept
{
    return *m_apiProvider;
}

void TRACE_TASK(const String& msg)
{
    Stringstream threadIdStream;
    threadIdStream << std::this_thread::get_id();
    TRACE_INFORMATION("[GAME SAVE] [ThreadID %s] [TASK] %s", threadIdStream.str().c_str(), msg.c_str());
}

} // namespace GameSave
} // namespace PlayFab
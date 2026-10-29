// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"

namespace PlayFab
{
namespace GameSave
{

HRESULT UICallbackManager::SetAction(UIAction action)
{
    if (m_shutdown.load())
    {
        TRACE_WARNING("[GAME SAVE] UICallbackManager::SetAction called after shutdown — ignoring");
        return E_UNEXPECTED;
    }

    ISchedulableTask* activeTask = nullptr;
    {
        std::lock_guard<Mutex> lock(m_waitMutex);
        if (m_activeTask == nullptr)
        {
            TRACE_WARNING("[GAME SAVE] UICallbackManager::SetAction called with no active UI waiting");
            return E_UNEXPECTED; // No UI waiting
        }
        if (PromptKindOf(action) != m_pendingPrompt)
        {
            // OQ-7: a response API for another prompt (e.g. a conflict response while a SyncFailed
            // prompt waits). Consuming the wait with an answer the waiting step doesn't handle left the
            // operation parked with nobody able to wake it (not the right answer, not XAsyncCancel, not
            // uninitialize): it never completed. The prompt keeps waiting for its own answer instead.
            TRACE_WARNING("[GAME SAVE] UICallbackManager::SetAction: '%s' doesn't answer the pending prompt (kind %d); still waiting for its answer",
                EnumName<UIAction>(action), static_cast<int>(m_pendingPrompt));
            return E_NOT_VALID_STATE;
        }
        activeTask = m_activeTask;
        m_activeTask = nullptr;
        m_pendingPrompt = UIPromptKind::None;
        m_action.store(action);
    }

    TRACE_INFORMATION("[GAME SAVE] UICallbackManager::SetAction: user chose '%s'", EnumName<UIAction>(action));
#if defined(_DEBUG)
    ScopedUiWakeForDebug uiWake; // a wake from the answering / cancelling thread
#endif
    activeTask->ScheduleNow();
    return S_OK;
}

UIPromptKind UICallbackManager::PromptKindOf(UIAction action) noexcept
{
    switch (action)
    {
        case UIAction::UISyncFailedRetry:
        case UIAction::UISyncFailedCancel:
        case UIAction::UISyncFailedUseOffline:
            return UIPromptKind::SyncFailed;
        case UIAction::UIActiveDeviceContentionSyncLastSaved:
        case UIAction::UIActiveDeviceContentionRetry:
        case UIAction::UIActiveDeviceContentionCancel:
            return UIPromptKind::ActiveDeviceContention;
        case UIAction::UIConflictTakeLocal:
        case UIAction::UIConflictTakeRemote:
        case UIAction::UIConflictCancel:
            return UIPromptKind::Conflict;
        case UIAction::UIOutOfStorageSpaceCleared:
        case UIAction::UIOutOfStorageCancel:
            return UIPromptKind::OutOfStorage;
        default:
            return UIPromptKind::None; // UINone, UIProgressCancel: answer no prompt
    }
}

UIAction UICallbackManager::GetAction() const
{
    return m_action.load();
}

bool UICallbackManager::ShowProgressUI(ISchedulableTask& task, const LocalUser& localUser, PFGameSaveFilesSyncState syncState)
{
    // NOTE: Progress callbacks are fire-and-forget (informational). We do NOT set m_activeTask here
    // because the caller will continue processing without waiting for a response.
    // Cancel is handled via m_progressCancelRequested atomic flag which is checked during download/upload loops.
    UNREFERENCED_PARAMETER(task);

    PFGameSaveFilesUiProgressCallback* callback = nullptr;
    void* context = nullptr;
    {
        std::lock_guard<Mutex> lock(GetGameSaveUiCallbackMutex());
        auto& uiInfo = GetGameSaveUiCallbackInfo();
        callback = uiInfo.progressCallback;
        context = uiInfo.progressContext;
    }
    if (callback)
    {
        callback(localUser.Handle(), syncState, context);
        return true;
    }
    return false;
}

void UICallbackManager::RequestProgressCancel()
{
    m_progressCancelRequested.store(true, std::memory_order_release);
}

bool UICallbackManager::IsProgressCancelRequested() const
{
    return m_progressCancelRequested.load(std::memory_order_acquire);
}

void UICallbackManager::ClearProgressCancel()
{
    m_progressCancelRequested.store(false, std::memory_order_release);
}

bool UICallbackManager::ShowOutOfStorageUI(ISchedulableTask& task, const LocalUser& localUser, uint64_t requiredBytes)
{
    PFGameSaveFilesUiOutOfStorageCallback* callback = nullptr;
    void* context = nullptr;
    {
        std::lock_guard<Mutex> lock(GetGameSaveUiCallbackMutex());
        auto& uiInfo = GetGameSaveUiCallbackInfo();
        callback = uiInfo.outOfStorageCallback;
        context = uiInfo.outOfStorageContext;
    }
    if (callback)
    {
        if (!BeginUIWait(task, UIPromptKind::OutOfStorage))
        {
            return true; // cancelled: rescheduled to observe it, no UI raised
        }
        callback(localUser.Handle(), requiredBytes, context);
        return true;
    }
    return false;
}

bool UICallbackManager::ShowSyncFailedUI(ISchedulableTask& task, const LocalUser& localUser, HRESULT syncFailedError, PFGameSaveFilesSyncState syncState)
{
    const char* syncStateName = "Unknown";
    switch (syncState)
    {
        case PFGameSaveFilesSyncState::NotStarted: syncStateName = "NotStarted"; break;
        case PFGameSaveFilesSyncState::PreparingForDownload: syncStateName = "PreparingForDownload"; break;
        case PFGameSaveFilesSyncState::Downloading: syncStateName = "Downloading"; break;
        case PFGameSaveFilesSyncState::PreparingForUpload: syncStateName = "PreparingForUpload"; break;
        case PFGameSaveFilesSyncState::Uploading: syncStateName = "Uploading"; break;
        case PFGameSaveFilesSyncState::SyncComplete: syncStateName = "SyncComplete"; break;
        default: break;
    }
    TRACE_WARNING("[GAME SAVE] ShowSyncFailedUI: sync failed with HR:0x%08X during '%s'. Waiting for user response (Retry/UseOffline/Cancel).", 
        syncFailedError, syncStateName);

    PFGameSaveFilesUiSyncFailedCallback* callback = nullptr;
    void* context = nullptr;
    {
        std::lock_guard<Mutex> lock(GetGameSaveUiCallbackMutex());
        auto& uiInfo = GetGameSaveUiCallbackInfo();
        callback = uiInfo.syncFailedCallback;
        context = uiInfo.syncFailedContext;
    }
    if (callback)
    {
        if (!BeginUIWait(task, UIPromptKind::SyncFailed))
        {
            return true; // cancelled: rescheduled to observe it, no UI raised
        }
        callback(localUser.Handle(), syncState, syncFailedError, context);
        return true;
    }
    TRACE_WARNING("[GAME SAVE] ShowSyncFailedUI: no syncFailedCallback registered, sync will fail");
    return false;
}

bool UICallbackManager::ShowConflictUI(
    ISchedulableTask& task,
    const LocalUser& localUser,
    PFGameSaveDescriptor& localGameSave,
    PFGameSaveDescriptor& remoteGameSave
    )
{
    PFGameSaveFilesUiConflictCallback* callback = nullptr;
    void* context = nullptr;
    {
        std::lock_guard<Mutex> lock(GetGameSaveUiCallbackMutex());
        auto& uiInfo = GetGameSaveUiCallbackInfo();
        callback = uiInfo.conflictCallback;
        context = uiInfo.conflictContext;
    }
    if (callback)
    {
        if (!BeginUIWait(task, UIPromptKind::Conflict))
        {
            return true; // cancelled: rescheduled to observe it, no UI raised
        }
        callback(
            localUser.Handle(),
            &localGameSave,
            &remoteGameSave,
            context);
        return true;
    }
    return false;
}

bool UICallbackManager::ShowActiveDeviceContentionUI(ISchedulableTask& task, const LocalUser& localUser, PFGameSaveDescriptor& currentLockHolderGameSave, PFGameSaveDescriptor& previousLockHolderGameSave)
{
    PFGameSaveFilesUiActiveDeviceContentionCallback* callback = nullptr;
    void* context = nullptr;
    {
        std::lock_guard<Mutex> lock(GetGameSaveUiCallbackMutex());
        auto& uiInfo = GetGameSaveUiCallbackInfo();
        callback = uiInfo.activeDeviceContentionCallback;
        context = uiInfo.activeDeviceContentionContext;
    }
    if (callback)
    {
        if (!BeginUIWait(task, UIPromptKind::ActiveDeviceContention))
        {
            return true; // cancelled: rescheduled to observe it, no UI raised
        }
        callback(localUser.Handle(),
            &currentLockHolderGameSave,
            &previousLockHolderGameSave,
            context);
        return true;
    }
    return false;
}

HRESULT UICallbackManager::HandleFailedUI(
    ISchedulableTask& task, 
    const std::function <void(void)>& retryAction,
    const std::function <void(void)>& useOfflineAction
    )
{
    UIAction uiAction = GetAction();
    if (uiAction == UIAction::UISyncFailedRetry)
    {
        TRACE_INFORMATION("[GAME SAVE] HandleFailedUI: retrying sync operation");
        if (m_syncFailedRetryObserver)
        {
            m_syncFailedRetryObserver();
        }
        retryAction();
        task.ScheduleNow();
    }
    else if (uiAction == UIAction::UISyncFailedUseOffline)
    {
        TRACE_WARNING("[GAME SAVE] HandleFailedUI: user chose to continue OFFLINE - skipping cloud sync");
        useOfflineAction();
        task.ScheduleNow();
    }
    else if (uiAction == UIAction::UISyncFailedCancel)
    {
        TRACE_WARNING("[GAME SAVE] HandleFailedUI: user CANCELLED sync operation");
        return E_PF_GAMESAVE_USER_CANCELLED;
    }
    else
    {
        // No action set yet - user hasn't responded to the syncFailedCallback.
        // Return E_PENDING to keep the async operation alive. SetAction() will
        // call ScheduleNow() when the user responds, waking up the provider.
        TRACE_VERBOSE("[GAME SAVE] HandleFailedUI: still waiting for user response (action=%d)", static_cast<int>(uiAction));
        return E_PENDING;
    }
    return S_OK;
}

bool UICallbackManager::TriggerActiveDeviceChangedCallback(
    RunContext runContext,
    LocalUser& localUser, 
    PFGameSaveDescriptor& activeDevice)
{
    PFGameSaveFilesActiveDeviceChangedCallback* callbackSnapshot = nullptr;
    void* contextSnapshot = nullptr;
    XTaskQueueHandle queueSnapshot = nullptr;
    {
        std::lock_guard<Mutex> lock(GetGameSaveUiCallbackMutex());
        auto& uiInfo = GetGameSaveUiCallbackInfo();
        if (uiInfo.activeDeviceChangedCallback == nullptr)
        {
            return false;
        }
        callbackSnapshot = uiInfo.activeDeviceChangedCallback;
        contextSnapshot = uiInfo.activeDeviceChangedContext;
        queueSnapshot = uiInfo.activeDeviceChangedCallbackQueue;
    }

    // call the callback on the correct thread using the queue passed into PFGameSaveFilesSetActiveDeviceChangedCallback()
    RunContext rc = runContext.DeriveOnQueue(queueSnapshot);
    auto handle = localUser.Handle();
    rc.TaskQueueSubmitCompletion([activeDevice, handle, callbackSnapshot, contextSnapshot]() mutable
    {
        callbackSnapshot(handle, &activeDevice, contextSnapshot);
    });
    return true;
}

void UICallbackManager::CancelPendingUIWait()
{
    m_shutdown.store(true);
    ISchedulableTask* activeTask = nullptr;
    {
        std::lock_guard<Mutex> lock(m_waitMutex);
        activeTask = m_activeTask;
        m_activeTask = nullptr;
        m_pendingPrompt = UIPromptKind::None;
        if (activeTask)
        {
            m_action.store(UIAction::UISyncFailedCancel);
        }
    }
    if (activeTask)
    {
        TRACE_WARNING("[GAME SAVE] UICallbackManager::CancelPendingUIWait: Cancelling pending UI wait during shutdown");
#if defined(_DEBUG)
        ScopedUiWakeForDebug uiWake; // a wake from the uninitializing thread
#endif
        activeTask->ScheduleNow();
    }
}

bool UICallbackManager::BeginUIWait(ISchedulableTask& task, UIPromptKind kind)
{
    {
        std::lock_guard<Mutex> lock(m_waitMutex);
        m_action.store(UIAction::UINone); // Clear stale action from any previous UI interaction
        m_activeTask = &task;
        m_pendingPrompt = kind;
    }

    // Checked after publishing the task: a cancellation either sees the task parked (and wakes it
    // via CancelUIWaitForTask) or is seen here. CancelUIWaitForTask takes the task under m_waitMutex,
    // which keeps the two from both rescheduling it.
    if (task.IsCancellationRequested())
    {
        TRACE_WARNING("[GAME SAVE] UICallbackManager::BeginUIWait: operation already cancelled, not raising UI");
        CancelUIWaitForTask(task);
        return false;
    }
    return true;
}

bool UICallbackManager::CancelUIWaitForTask(ISchedulableTask& task)
{
    // Under m_waitMutex so this cannot race SetAction: exactly one of them takes the waiting task and
    // reschedules it.
    {
        std::lock_guard<Mutex> lock(m_waitMutex);
        if (m_activeTask != &task)
        {
            return false;
        }
        m_activeTask = nullptr;
        m_pendingPrompt = UIPromptKind::None;
        m_action.store(UIAction::UISyncFailedCancel);
    }

    TRACE_WARNING("[GAME SAVE] UICallbackManager::CancelUIWaitForTask: operation cancelled while waiting for a UI response");
#if defined(_DEBUG)
    ScopedUiWakeForDebug uiWake; // a wake from the answering / cancelling thread
#endif
    task.ScheduleNow();
    return true;
}

void UICallbackManager::ReleaseUIWaitForTask(ISchedulableTask& task)
{
    std::lock_guard<Mutex> lock(m_waitMutex);
    if (m_activeTask == &task)
    {
        m_activeTask = nullptr;
        m_pendingPrompt = UIPromptKind::None;
    }
}


} // namespace GameSave
} // namespace PlayFab
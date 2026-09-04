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

    ISchedulableTask* activeTask = m_activeTask.exchange(nullptr);
    if (activeTask)
    {
        TRACE_INFORMATION("[GAME SAVE] UICallbackManager::SetAction: user chose '%s'", EnumName<UIAction>(action));
        
        m_action.store(action);
        activeTask->ScheduleNow();
        return S_OK;
    }            
    else
    {
        TRACE_WARNING("[GAME SAVE] UICallbackManager::SetAction called with no active UI waiting");
        return E_UNEXPECTED; // No UI waiting
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
        std::lock_guard<std::mutex> lock(GetGameSaveUiCallbackMutex());
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
        std::lock_guard<std::mutex> lock(GetGameSaveUiCallbackMutex());
        auto& uiInfo = GetGameSaveUiCallbackInfo();
        callback = uiInfo.outOfStorageCallback;
        context = uiInfo.outOfStorageContext;
    }
    if (callback)
    {
        m_action.store(UIAction::UINone); // Clear stale action from any previous UI interaction
        m_activeTask.store(&task);
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
        std::lock_guard<std::mutex> lock(GetGameSaveUiCallbackMutex());
        auto& uiInfo = GetGameSaveUiCallbackInfo();
        callback = uiInfo.syncFailedCallback;
        context = uiInfo.syncFailedContext;
    }
    if (callback)
    {
        m_action.store(UIAction::UINone); // Clear stale action from any previous UI interaction
        m_activeTask.store(&task);
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
        std::lock_guard<std::mutex> lock(GetGameSaveUiCallbackMutex());
        auto& uiInfo = GetGameSaveUiCallbackInfo();
        callback = uiInfo.conflictCallback;
        context = uiInfo.conflictContext;
    }
    if (callback)
    {
        m_action.store(UIAction::UINone); // Clear stale action from any previous UI interaction
        m_activeTask.store(&task);
        callback(
            localUser.Handle(),
            &localGameSave,
            &remoteGameSave,
            context);
        return true;
    }
    return false;
}

bool UICallbackManager::ShowActiveDeviceContentionUI(ISchedulableTask& task, const LocalUser& localUser, PFGameSaveDescriptor& localGameSave, PFGameSaveDescriptor& remoteGameSave)
{
    PFGameSaveFilesUiActiveDeviceContentionCallback* callback = nullptr;
    void* context = nullptr;
    {
        std::lock_guard<std::mutex> lock(GetGameSaveUiCallbackMutex());
        auto& uiInfo = GetGameSaveUiCallbackInfo();
        callback = uiInfo.activeDeviceContentionCallback;
        context = uiInfo.activeDeviceContentionContext;
    }
    if (callback)
    {
        m_action.store(UIAction::UINone); // Clear stale action from any previous UI interaction
        m_activeTask.store(&task);
        callback(localUser.Handle(),
            &localGameSave,
            &remoteGameSave,
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
        std::lock_guard<std::mutex> lock(GetGameSaveUiCallbackMutex());
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
    ISchedulableTask* activeTask = m_activeTask.exchange(nullptr);
    if (activeTask)
    {
        TRACE_WARNING("[GAME SAVE] UICallbackManager::CancelPendingUIWait: Cancelling pending UI wait during shutdown");
        m_action.store(UIAction::UISyncFailedCancel);
        activeTask->ScheduleNow();
    }
}


} // namespace GameSave
} // namespace PlayFab
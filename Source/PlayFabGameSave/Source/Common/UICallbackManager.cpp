// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"

namespace PlayFab
{
namespace GameSave
{

HRESULT UICallbackManager::SetAction(UIAction action)
{
    ISchedulableTask* activeTask = m_activeTask;
    if (activeTask)
    {
        m_activeTask = nullptr;
        m_action = action;
        activeTask->ScheduleNow();
        return S_OK;
    }            
    else
    {
        return E_UNEXPECTED; // No UI waiting
    }
}

UIAction UICallbackManager::GetAction() const
{
    return m_action;
}

bool UICallbackManager::ShowProgressUI(ISchedulableTask& task, const LocalUser& localUser, PFGameSaveFilesSyncState syncState)
{
    m_activeTask = &task;

    auto& uiInfo = GetGameSaveUiCallbackInfo();
    if (uiInfo.progressCallback)
    {
        uiInfo.progressCallback(localUser.Handle(), syncState, uiInfo.syncFailedContext);
        return true;
    }
    return false;
}

bool UICallbackManager::ShowOutOfStorageUI(ISchedulableTask& task, const LocalUser& localUser, uint64_t requiredBytes)
{
    m_activeTask = &task;

    auto& uiInfo = GetGameSaveUiCallbackInfo();
    if (uiInfo.outOfStorageCallback)
    {
        uiInfo.outOfStorageCallback(localUser.Handle(), requiredBytes, uiInfo.outOfStorageContext);
        return true;
    }
    return false;
}

bool UICallbackManager::ShowSyncFailedUI(ISchedulableTask& task, const LocalUser& localUser, HRESULT syncFailedError, PFGameSaveFilesSyncState syncState)
{
    m_activeTask = &task;

    auto& uiInfo = GetGameSaveUiCallbackInfo();
    if (uiInfo.syncFailedCallback)
    {
        uiInfo.syncFailedCallback(localUser.Handle(), syncState, syncFailedError, uiInfo.syncFailedContext);
        return true;
    }
    return false;
}

bool UICallbackManager::ShowConflictUI(
    ISchedulableTask& task,
    const LocalUser& localUser,
    PFGameSaveDescriptor& localGameSave,
    PFGameSaveDescriptor& remoteGameSave
    )
{
    m_activeTask = &task;

    auto& uiInfo = GetGameSaveUiCallbackInfo();
    if (uiInfo.conflictCallback)
    {
        uiInfo.conflictCallback(
            localUser.Handle(),
            &localGameSave,
            &remoteGameSave,
            uiInfo.conflictContext);
        return true;
    }
    return false;
}

bool UICallbackManager::ShowActiveDeviceContentionUI(ISchedulableTask& task, const LocalUser& localUser, PFGameSaveDescriptor& localGameSave, PFGameSaveDescriptor& remoteGameSave)
{
    m_activeTask = &task;

    auto& uiInfo = GetGameSaveUiCallbackInfo();
    if (uiInfo.activeDeviceContentionCallback != nullptr)
    {
        uiInfo.activeDeviceContentionCallback(localUser.Handle(),
            &localGameSave,
            &remoteGameSave,
            uiInfo.activeDeviceContentionContext);
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
        retryAction();
        task.ScheduleNow();
    }
    else if (uiAction == UIAction::UISyncFailedUseOffline)
    {
        useOfflineAction();
        task.ScheduleNow();
    }
    else if (uiAction == UIAction::UISyncFailedCancel)
    {
        return E_PF_GAMESAVE_USER_CANCELLED;
    }
    return S_OK;
}

bool UICallbackManager::TriggerActiveDeviceChangedCallback(
    RunContext runContext,
    LocalUser& localUser, 
    PFGameSaveDescriptor& activeDevice)
{
    auto& uiInfo = GetGameSaveUiCallbackInfo();
    if (uiInfo.activeDeviceChangedCallback != nullptr)
    {
        // call the callback on the correct thread using the queue passed into PFGameSaveFilesSetActiveDeviceChangedCallback()
        RunContext rc = runContext.DeriveOnQueue(uiInfo.activeDeviceChangedCallbackQueue);
        auto handle = localUser.Handle();
        rc.TaskQueueSubmitCompletion([activeDevice, handle]() mutable
        {
            auto& uiInfo = GetGameSaveUiCallbackInfo();
            uiInfo.activeDeviceChangedCallback(handle, &activeDevice, uiInfo.activeDeviceChangedContext);
        });
        return true;
    }
    return false;
}


} // namespace GameSave
} // namespace PlayFab
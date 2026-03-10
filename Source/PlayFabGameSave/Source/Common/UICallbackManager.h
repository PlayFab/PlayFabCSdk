// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

namespace PlayFab
{
namespace GameSave
{

enum class UIAction
{
    UINone = 0,
    UIProgressCancel,
    UISyncFailedRetry,
    UISyncFailedCancel,
    UISyncFailedUseOffline,
    UIActiveDeviceContentionSyncLastSaved,
    UIActiveDeviceContentionRetry,
    UIActiveDeviceContentionCancel,
    UIConflictTakeLocal,
    UIConflictTakeRemote,
    UIConflictCancel,
    UIOutOfStorageSpaceCleared,
    UIOutOfStorageCancel
};

class UICallbackManager
{
public:
    HRESULT SetAction(UIAction action);
    UIAction GetAction() const;

    bool ShowProgressUI(ISchedulableTask& task, const LocalUser& localUser, PFGameSaveFilesSyncState syncState);
    bool ShowOutOfStorageUI(ISchedulableTask& task, const LocalUser& localUser, uint64_t requiredBytes);
    bool ShowSyncFailedUI(ISchedulableTask& task, const LocalUser& localUser, HRESULT syncFailedError, PFGameSaveFilesSyncState syncState);
    bool ShowConflictUI(ISchedulableTask& task, const LocalUser& localUser, PFGameSaveDescriptor& localGameSave, PFGameSaveDescriptor& remoteGameSave);
    bool ShowActiveDeviceContentionUI(ISchedulableTask& task, const LocalUser& localUser, PFGameSaveDescriptor& localGameSave, PFGameSaveDescriptor& remoteGameSave);
    static bool TriggerActiveDeviceChangedCallback(RunContext runContext, LocalUser& localUser, PFGameSaveDescriptor& activeDevice);

    HRESULT HandleFailedUI(ISchedulableTask& task, const std::function <void(void)>& retryAction, const std::function <void(void)>& useOfflineAction);

    // Progress cancel support - uses atomic flag instead of SetAction/ScheduleNow to avoid race conditions
    void RequestProgressCancel();
    bool IsProgressCancelRequested() const;
    void ClearProgressCancel();

    // Cancel any pending UI wait (for use during shutdown/termination)
    void CancelPendingUIWait();

private:
    UIAction m_action{ UIAction::UINone };
    ISchedulableTask* m_activeTask{ nullptr };
    std::atomic<bool> m_progressCancelRequested{ false };
};

} // namespace GameSave
} // namespace PlayFab
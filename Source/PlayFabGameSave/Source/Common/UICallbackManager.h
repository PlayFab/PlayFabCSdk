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

// The prompt a task is parked on. Each UIAction answers exactly one kind (UIProgressCancel answers
// none: progress is fire-and-forget and cancels through RequestProgressCancel).
enum class UIPromptKind
{
    None = 0,
    SyncFailed,
    ActiveDeviceContention,
    Conflict,
    OutOfStorage
};

class UICallbackManager
{
public:
    // Answers the prompt that is waiting. E_UNEXPECTED when none is (or after shutdown);
    // E_NOT_VALID_STATE when `action` belongs to another prompt kind (for example a conflict response
    // while a SyncFailed prompt waits): the prompt keeps waiting for its own answer (OQ-7).
    HRESULT SetAction(UIAction action);
    static UIPromptKind PromptKindOf(UIAction action) noexcept;
    UIAction GetAction() const;

    bool ShowProgressUI(ISchedulableTask& task, const LocalUser& localUser, PFGameSaveFilesSyncState syncState);
    bool ShowOutOfStorageUI(ISchedulableTask& task, const LocalUser& localUser, uint64_t requiredBytes);
    bool ShowSyncFailedUI(ISchedulableTask& task, const LocalUser& localUser, HRESULT syncFailedError, PFGameSaveFilesSyncState syncState);
    bool ShowConflictUI(ISchedulableTask& task, const LocalUser& localUser, PFGameSaveDescriptor& localGameSave, PFGameSaveDescriptor& remoteGameSave);
    bool ShowActiveDeviceContentionUI(ISchedulableTask& task, const LocalUser& localUser, PFGameSaveDescriptor& currentLockHolderGameSave, PFGameSaveDescriptor& previousLockHolderGameSave);
    static bool TriggerActiveDeviceChangedCallback(RunContext runContext, LocalUser& localUser, PFGameSaveDescriptor& activeDevice);

    HRESULT HandleFailedUI(ISchedulableTask& task, const std::function <void(void)>& retryAction, const std::function <void(void)>& useOfflineAction);

    // Called by HandleFailedUI when the answer is Retry, before the retry action (the owner re-arms
    // its once-per-attempt telemetry). Set once at construction.
    void SetSyncFailedRetryObserver(std::function<void(void)> observer) { m_syncFailedRetryObserver = std::move(observer); }

    // Progress cancel support - uses atomic flag instead of SetAction/ScheduleNow to avoid race conditions
    void RequestProgressCancel();
    bool IsProgressCancelRequested() const;
    void ClearProgressCancel();

    // Cancel any pending UI wait (for use during shutdown/termination)
    void CancelPendingUIWait();

    // If 'task' is the one parked waiting for a UI response, stop waiting and reschedule it so it can
    // observe its cancelled RunContext and complete. Used when the title calls XAsyncCancel: nothing
    // else would ever reschedule the task, so the operation would never complete. Unlike
    // CancelPendingUIWait this does not shut the manager down, so later operations can still use UI.
    // Returns true if 'task' was waiting and has been rescheduled.
    bool CancelUIWaitForTask(ISchedulableTask& task);

    // Drops the waiting-task pointer if it still refers to 'task', so a late SetAction can never
    // schedule a task that has been destroyed.
    void ReleaseUIWaitForTask(ISchedulableTask& task);

private:
    // Parks 'task' as the one waiting for a UI response. Returns false - and raises no UI - if the
    // task's operation has already been cancelled; the task is rescheduled instead so it observes
    // the cancellation. Needed because the cancellation can be delivered before the task is parked
    // (e.g. it aborts an in-flight service call whose continuation then raises the sync-failed UI),
    // in which case there was nothing for CancelUIWaitForTask to wake at that moment.
    // `kind` is the prompt the task waits on: SetAction accepts only that kind's answers.
    bool BeginUIWait(ISchedulableTask& task, UIPromptKind kind);

    std::atomic<UIAction> m_action{ UIAction::UINone };
    // The parked task and the prompt kind it waits on change together under m_waitMutex, so an answer
    // is checked against the prompt it actually answers. Exactly one of SetAction,
    // CancelUIWaitForTask, CancelPendingUIWait (and ReleaseUIWaitForTask, which drops it) takes a
    // parked task. m_waitMutex is a leaf: nothing else is locked and nothing is scheduled under it.
    mutable Mutex m_waitMutex;
    ISchedulableTask* m_activeTask{ nullptr };          // guarded by m_waitMutex
    UIPromptKind m_pendingPrompt{ UIPromptKind::None }; // guarded by m_waitMutex
    std::atomic<bool> m_progressCancelRequested{ false };
    std::atomic<bool> m_shutdown{ false };
    std::function<void(void)> m_syncFailedRetryObserver; // see SetSyncFailedRetryObserver
};

} // namespace GameSave
} // namespace PlayFab
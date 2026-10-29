// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include "FolderSyncManager.h"

namespace PlayFab
{
namespace GameSave
{

class DownloadAsyncProvider : public GameSaveAsyncProvider<void>, public ISchedulableTask, public ICancellationListener
{
public:
    // The caller has already reserved the user's sync state (FolderSyncManager::TryReserveDownload)
    // and hands ownership of that reservation to this provider; previousSyncState and
    // previousForcedDisconnectFromCloud are what TryReserveDownload replaced.
    template<size_t n>
    DownloadAsyncProvider(
        RunContext&& rc,
        XAsyncBlock* async,
        const char(&identityName)[n],
        SharedPtr<FolderSyncManager>&& folderSync,
        PFGameSaveFilesSyncState previousSyncState,
        bool previousForcedDisconnectFromCloud) :
        GameSaveAsyncProvider{ std::move(rc), async, identityName },
        m_folderSync{ std::move(folderSync) },
        m_previousSyncState{ previousSyncState },
        m_previousForcedDisconnectFromCloud{ previousForcedDisconnectFromCloud }
    {
        TRACE_TASK("DownloadAsyncProvider ctor");
        // Under the manager's lock: this resets the steps and drops the manifests and file/folder
        // sets, which a SetSaveDescription provider can be reading concurrently on a worker thread.
        std::lock_guard<std::recursive_mutex> lock(m_folderSync->GetSyncMutex());
        m_folderSync->InitForDownload();
    }

    ~DownloadAsyncProvider()
    {
        TRACE_TASK("DownloadAsyncProvider dtor");
        if (m_cancellationToken.has_value())
        {
            m_cancellationToken->UnregisterForNotificationAndCheck(*this);
        }

        if (m_folderSync)
        {
            m_folderSync->GetUIManager().ReleaseUIWaitForTask(*this);
            ReleaseReservationOnce(true);
        }
    }

    // Gives up the sync-state reservation exactly once, whichever of DoWork's terminal branch or
    // the destructor gets there first (same scheme as FileResetCloudAsyncProvider).
    //  - DoWork's terminal branch leaves the state the workflow set, it only drops ownership.
    //  - The destructor is the backstop for every path that never reaches a terminal DoWork: Begin
    //    failing, Run failing, termination. It restores the state the reservation replaced, which
    //    otherwise parks the user in PreparingForDownload and rejects every later AddUser with
    //    E_PF_GAMESAVE_USER_ALREADY_ADDED.
    // Tracking ownership, rather than restoring whenever the state still looks like a download,
    // matters because XAsyncOp::Cleanup is not ordered against the completion callback: a title can
    // start a new AddUser from that callback, and this provider's destructor must not then undo the
    // new operation's reservation.
    void ReleaseReservationOnce(bool restorePreviousState)
    {
        if (!m_reservationHeld.exchange(false) || !restorePreviousState)
        {
            return;
        }

        TRACE_WARNING("DownloadAsyncProvider: download never completed, releasing its sync-state reservation");
        // TryReserveDownload clears the forced-disconnect flag when admitting a reconnect attempt.
        // Put it back first so the reservation is never visibly released while connectivity still
        // reads stale; leave it alone when it was not set, so a disconnect the active-device poller
        // published in the meantime is not erased.
        if (m_previousForcedDisconnectFromCloud)
        {
            m_folderSync->SetForcedDisconnectFromCloud(true);
        }
        // H17: compare-and-set on the operation kind. While this provider still holds the reservation
        // nothing else can be admitted, so the state is one this AddUser owns: the download states, or
        // the upload states of its conflict upload.
        if (!m_folderSync->TrySetSyncStateIfOwnedBy({ SyncOperationKind::AddUser, SyncOperationKind::Upload }, m_previousSyncState))
        {
            TRACE_WARNING("DownloadAsyncProvider: sync state no longer owned by this AddUser; left unchanged");
        }
    }

    // ICancellationListener. XAsyncOp::Cancel only cancels the RunContext token; if the workflow is
    // parked waiting for a UI response nothing would ever run DoWork again, so the operation would
    // never complete and the reservation would never be released. Wake it so DoWork observes the
    // cancellation and fails with E_ABORT. In-flight service calls observe the token themselves.
    void OnCancellation() override
    {
        if (m_folderSync)
        {
            m_folderSync->GetUIManager().CancelUIWaitForTask(*this);
        }
    }

    bool IsCancellationRequested() const noexcept override
    {
        return m_cancellationToken.has_value() && m_cancellationToken->IsCancelled();
    }

    void ScheduleNow() override
    {
        TRACE_TASK("DownloadAsyncProvider.ScheduleNow");
#if defined(_DEBUG)
        m_singleThreadProviderValidation.AssertUponSchedule();
#endif
        Schedule(0);
    }

    void ScheduleAfter(uint32_t delayMs) override
    {
        Schedule(delayMs);
    }

protected:
    HRESULT Begin(RunContext runContext) override
    {
        m_cancellationToken.emplace(runContext.CancellationToken());
        // Already cancelled: no notification will come, but DoWork checks the token first anyway.
        (void)m_cancellationToken->RegisterForNotificationAndCheck(*this);
        return GameSaveAsyncProvider::Begin(std::move(runContext));
    }

    HRESULT DoWork(RunContext runContext) override;
    SharedPtr<FolderSyncManager> m_folderSync;
    const PFGameSaveFilesSyncState m_previousSyncState;
    const bool m_previousForcedDisconnectFromCloud;
    // True while this provider owns the sync-state reservation. See ReleaseReservationOnce().
    std::atomic<bool> m_reservationHeld{ true };
    std::optional<CancellationToken> m_cancellationToken;
    // Owned by the FolderSyncManager, not by this provider: see FolderSyncManager::GetSyncMutex().
    RecursiveMutex& m_folderSyncMutex{ m_folderSync->GetSyncMutex() };
#if defined(_DEBUG)
    SingleThreadProviderValidation m_singleThreadProviderValidation;
#endif
};

template<size_t n>
UniquePtr<DownloadAsyncProvider> MakeDownloadAsyncProvider(
    RunContext&& rc,
    XAsyncBlock* async,
    const char(&identityName)[n],
    SharedPtr<FolderSyncManager>&& folderSync,
    PFGameSaveFilesSyncState previousSyncState,
    bool previousForcedDisconnectFromCloud)
{
    return MakeUnique<DownloadAsyncProvider>(std::move(rc), async, identityName, std::move(folderSync), previousSyncState, previousForcedDisconnectFromCloud);
}

} // namespace GameSave
} // namespace PlayFab

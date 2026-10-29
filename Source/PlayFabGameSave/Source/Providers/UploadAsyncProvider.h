// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include "FolderSyncManager.h"

namespace PlayFab
{
namespace GameSave
{

class GameSaveGlobalState;

class UploadAsyncProvider : public GameSaveAsyncProvider<void>, public ISchedulableTask, private ICancellationListener
{
public:
    // When reservationHeld is true the caller has already reserved the user's sync state
    // (FolderSyncManager::TryReserveUpload) and hands ownership of that reservation to this
    // provider; previousSyncState is what the reservation replaced.
    template<size_t n>
    UploadAsyncProvider(
        RunContext&& rc,
        PFGameSaveFilesUploadOption option,
        XAsyncBlock* async,
        const char(&identityName)[n],
        SharedPtr<FolderSyncManager>&& folderSync,
        bool reservationHeld,
        PFGameSaveFilesSyncState previousSyncState,
        SharedPtr<GameSaveGlobalState> globalState = nullptr,
        std::function<HRESULT()> uploadProcessingComplete = {}) :
        GameSaveAsyncProvider{ std::move(rc), async, identityName },
        m_folderSync{ std::move(folderSync) },
        m_option{ option },
        m_previousSyncState{ previousSyncState },
        m_reservationHeld{ reservationHeld },
        m_globalState{ std::move(globalState) },
        m_uploadProcessingComplete{ std::move(uploadProcessingComplete) }
    {
        TRACE_TASK("UploadAsyncProvider ctor");
        // Under the manager's lock: this resets the compare/upload/relock steps, which another
        // provider for the same user can be reading concurrently on a worker thread.
        std::lock_guard<std::recursive_mutex> lock(m_folderSync->GetSyncMutex());
        m_folderSync->InitForUpload();
    }

    ~UploadAsyncProvider()
    {
        TRACE_TASK("UploadAsyncProvider dtor");
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
    // the destructor gets there first. See DownloadAsyncProvider::ReleaseReservationOnce: the
    // destructor restores the previous state for paths that never reach a terminal DoWork (which
    // otherwise rejects every later upload with E_PF_GAMESAVE_OPERATION_IN_PROGRESS), and ownership
    // is tracked so a destructor running after the completion callback cannot clear a reservation
    // a new upload has since taken.
    void ReleaseReservationOnce(bool restorePreviousState)
    {
        if (!m_reservationHeld.exchange(false) || !restorePreviousState)
        {
            return;
        }

        TRACE_WARNING("UploadAsyncProvider: upload never completed, releasing its sync-state reservation");
        // H17: compare-and-set, so it can only ever move an upload-owned state.
        if (!m_folderSync->TrySetSyncStateIfOwnedBy({ SyncOperationKind::Upload }, m_previousSyncState))
        {
            TRACE_WARNING("UploadAsyncProvider: sync state no longer owned by this upload; left unchanged");
        }
    }

    // ICancellationListener. See DownloadAsyncProvider::OnCancellation.
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
        TRACE_TASK("UploadAsyncProvider.ScheduleNow");
        Schedule(0);
    }

    void ScheduleAfter(uint32_t delayMs) override
    {
        Schedule(delayMs);
    }

protected:
    HRESULT Begin(RunContext runContext) override;
    HRESULT DoWork(RunContext runContext) override;
    SharedPtr<FolderSyncManager> m_folderSync;
    PFGameSaveFilesUploadOption m_option{ PFGameSaveFilesUploadOption::KeepDeviceActive };
    const PFGameSaveFilesSyncState m_previousSyncState;
    // True while this provider owns the sync-state reservation. See ReleaseReservationOnce().
    std::atomic<bool> m_reservationHeld;
    std::optional<CancellationToken> m_cancellationToken;
    // Owned by the FolderSyncManager, not by this provider: see FolderSyncManager::GetSyncMutex().
    RecursiveMutex& m_folderSyncMutex{ m_folderSync->GetSyncMutex() };

private:
    HRESULT FinishUpload(HRESULT hr);
    SharedPtr<GameSaveGlobalState> m_globalState;
    std::function<HRESULT()> m_uploadProcessingComplete;
};

template<size_t n>
UniquePtr<UploadAsyncProvider> MakeStartFileSyncProvider(
    RunContext&& rc,
    PFGameSaveFilesUploadOption option,
    XAsyncBlock* async,
    const char(&identityName)[n],
    SharedPtr<FolderSyncManager>&& folderSync,
    bool reservationHeld,
    PFGameSaveFilesSyncState previousSyncState,
    SharedPtr<GameSaveGlobalState> globalState = nullptr,
    std::function<HRESULT()> uploadProcessingComplete = {})
{
    return MakeUnique<UploadAsyncProvider>(std::move(rc), option, async, identityName, std::move(folderSync), reservationHeld, previousSyncState, std::move(globalState), std::move(uploadProcessingComplete));
}

} // namespace GameSave
} // namespace PlayFab

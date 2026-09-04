// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include "FolderSyncManager.h"

namespace PlayFab
{
namespace GameSave
{

class FileResetCloudAsyncProvider : public GameSaveAsyncProvider<void>, public ISchedulableTask
{
public:
    template<size_t n>
    FileResetCloudAsyncProvider(RunContext&& rc, XAsyncBlock* async, const char(&identityName)[n], SharedPtr<FolderSyncManager>&& folderSync) :
        GameSaveAsyncProvider{ std::move(rc), async, identityName },
        m_folderSync{ std::move(folderSync) }
    {
        TRACE_TASK("FileResetCloudAsyncProvider ctor");
    }

    ~FileResetCloudAsyncProvider()
    {
        TRACE_TASK("FileResetCloudAsyncProvider dtor");
        ReleaseReservationOnce();
    }

    // Releases the reset-cloud reservation exactly once, whichever of DoWork or the destructor
    // reaches it first.
    //
    // Both paths are needed, and neither alone is sufficient:
    //  - DoWork's terminal branch must release promptly, so a title that retries from its
    //    completion callback is not rejected with E_PF_GAMESAVE_OPERATION_IN_PROGRESS.
    //  - The destructor must release as a backstop, because XAsyncOp::Cancel does not run DoWork
    //    (XAsyncProviderBase only cancels the RunContext token), so a cancel mid-flight would
    //    otherwise leave the reservation set for the life of the FolderSyncManager.
    //
    // The exchange is what makes having both safe. Releasing unconditionally from each is NOT
    // idempotent, because a second reset can own the reservation by the time the first provider is
    // destroyed: DoWork releases, XAsyncComplete fires, the title's callback starts a new
    // ResetCloud that successfully reserves, and only then does XAsyncOp::Cleanup destroy this
    // provider - whose dtor would clear the new reservation. An upload or AddUser could then be
    // admitted while a reset is running, which is exactly what this flag exists to prevent (see
    // the comment in TryReserveUpload). Cleanup is not ordered against the completion callback in
    // any way we control, so ownership is tracked explicitly rather than assumed.
    void ReleaseReservationOnce()
    {
        if (m_folderSync && m_reservationHeld.exchange(false))
        {
            m_folderSync->ReleaseResetCloudReservation();
        }
    }

    void ScheduleNow() override
    {
        TRACE_TASK("FileResetCloudAsyncProvider.ScheduleNow");
#if defined(_DEBUG)
        m_singleThreadProvider.AssertUponSchedule();
#endif
        Schedule(0);
    }

protected:
    HRESULT DoWork(RunContext runContext) override;
    SharedPtr<FolderSyncManager> m_folderSync;
    // True while this provider owns the reset-cloud reservation. See ReleaseReservationOnce().
    std::atomic<bool> m_reservationHeld{ true };
    // Owned by the FolderSyncManager, not by this provider: see FolderSyncManager::GetSyncMutex().
    std::recursive_mutex& m_folderSyncMutex{ m_folderSync->GetSyncMutex() };
#if defined(_DEBUG)
    SingleThreadProviderValidation m_singleThreadProvider;
#endif
};

template<size_t n>
UniquePtr<FileResetCloudAsyncProvider> MakeStartFileResetCloudAsyncProvider(RunContext&& rc, XAsyncBlock* async, const char(&identityName)[n], SharedPtr<FolderSyncManager>&& folderSync)
{
    return MakeUnique<FileResetCloudAsyncProvider>(std::move(rc), async, identityName, std::move(folderSync));
}

} // namespace GameSave
} // namespace PlayFab

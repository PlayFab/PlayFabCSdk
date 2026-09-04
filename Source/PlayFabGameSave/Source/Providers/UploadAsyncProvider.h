// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include "FolderSyncManager.h"

namespace PlayFab
{
namespace GameSave
{

class UploadAsyncProvider : public GameSaveAsyncProvider<void>, public ISchedulableTask
{
public:
    template<size_t n>
    UploadAsyncProvider(RunContext&& rc, PFGameSaveFilesUploadOption option, XAsyncBlock* async, const char(&identityName)[n], SharedPtr<FolderSyncManager>&& folderSync) :
        GameSaveAsyncProvider{ std::move(rc), async, identityName },
        m_folderSync{ std::move(folderSync) },
        m_option{ option }
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
        // If provider is destroyed without completing (e.g., app suspend/crash),
        // reset sync state to prevent stale state on next attempt.
        // Only reset if still in upload states to avoid clobbering valid states.
        if (m_folderSync)
        {
            FolderSyncManagerProgress progress = m_folderSync->GetSyncProgress();
            if (progress.syncState == PFGameSaveFilesSyncState::PreparingForUpload ||
                progress.syncState == PFGameSaveFilesSyncState::Uploading)
            {
                TRACE_WARNING("UploadAsyncProvider destroyed while in upload state, resetting to SyncComplete");
                m_folderSync->SetSyncStateProgress(PFGameSaveFilesSyncState::SyncComplete, 0, 0);
            }
        }
    }

    void ScheduleNow() override
    {
        TRACE_TASK("UploadAsyncProvider.ScheduleNow");
#if defined(_DEBUG)
        m_singleThreadProvider.AssertUponSchedule();
#endif
        Schedule(0);
    }

protected:
    HRESULT DoWork(RunContext runContext) override;
    SharedPtr<FolderSyncManager> m_folderSync;
    PFGameSaveFilesUploadOption m_option{ PFGameSaveFilesUploadOption::KeepDeviceActive };
    // Owned by the FolderSyncManager, not by this provider: see FolderSyncManager::GetSyncMutex().
    std::recursive_mutex& m_folderSyncMutex{ m_folderSync->GetSyncMutex() };
#if defined(_DEBUG)
    SingleThreadProviderValidation m_singleThreadProvider;
#endif
};

template<size_t n>
UniquePtr<UploadAsyncProvider> MakeStartFileSyncProvider(RunContext&& rc, PFGameSaveFilesUploadOption option, XAsyncBlock* async, const char(&identityName)[n], SharedPtr<FolderSyncManager>&& folderSync)
{
    return MakeUnique<UploadAsyncProvider>(std::move(rc), option, async, identityName, std::move(folderSync));
}

} // namespace GameSave
} // namespace PlayFab

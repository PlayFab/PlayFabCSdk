// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include "FolderSyncManager.h"

namespace PlayFab
{
namespace GameSave
{

class DownloadAsyncProvider : public GameSaveAsyncProvider<void>, public ISchedulableTask
{
public:
    template<size_t n>
    DownloadAsyncProvider(RunContext&& rc, XAsyncBlock* async, const char(&identityName)[n], SharedPtr<FolderSyncManager>&& folderSync) :
        GameSaveAsyncProvider{ std::move(rc), async, identityName },
        m_folderSync{ folderSync }
    {
        TRACE_TASK("DownloadAsyncProvider ctor");
        m_folderSync->InitForDownload();
    }

    ~DownloadAsyncProvider()
    {
        TRACE_TASK("DownloadAsyncProvider dtor");
        // If provider is destroyed without completing (e.g., app suspend/crash),
        // reset sync state to prevent E_PF_GAMESAVE_DOWNLOAD_IN_PROGRESS on next attempt.
        // Only reset if still in download states to avoid clobbering valid states.
        if (m_folderSync)
        {
            FolderSyncManagerProgress progress = m_folderSync->GetSyncProgress();
            if (progress.syncState == PFGameSaveFilesSyncState::PreparingForDownload ||
                progress.syncState == PFGameSaveFilesSyncState::Downloading)
            {
                TRACE_WARNING("DownloadAsyncProvider destroyed while in download state, resetting to NotStarted");
                m_folderSync->SetSyncStateProgress(PFGameSaveFilesSyncState::NotStarted, 0, 0);
            }
        }
    }

    void ScheduleNow() override
    {
        TRACE_TASK("DownloadAsyncProvider.ScheduleNow");
#if _DEBUG
        m_singleThreadProviderValidation.AssertUponSchedule();
#endif
        Schedule(0);
    }

protected:
    HRESULT DoWork(RunContext runContext) override;
    SharedPtr<FolderSyncManager> m_folderSync;
    std::recursive_mutex m_folderSyncMutex;
#if _DEBUG
    SingleThreadProviderValidation m_singleThreadProviderValidation;
#endif
};

template<size_t n>
UniquePtr<DownloadAsyncProvider> MakeDownloadAsyncProvider(RunContext&& rc, XAsyncBlock* async, const char(&identityName)[n], SharedPtr<FolderSyncManager>&& folderSync)
{
    return MakeUnique<DownloadAsyncProvider>(std::move(rc), async, identityName, std::move(folderSync));
}

} // namespace GameSave
} // namespace PlayFab

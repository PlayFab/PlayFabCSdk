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

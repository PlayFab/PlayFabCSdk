// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

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
        m_folderSync{ folderSync },
        m_option{ option }
    {
        TRACE_TASK("UploadAsyncProvider ctor");
        m_folderSync->InitForUpload();
    }

    ~UploadAsyncProvider()
    {
        TRACE_TASK("UploadAsyncProvider dtor");
    }

    void ScheduleNow() override
    {
        TRACE_TASK("UploadAsyncProvider.ScheduleNow");
#if _DEBUG
        m_singleThreadProvider.AssertUponSchedule();
#endif
        Schedule(0);
    }

protected:
    HRESULT DoWork(RunContext runContext) override;
    SharedPtr<FolderSyncManager> m_folderSync;
    PFGameSaveFilesUploadOption m_option{ PFGameSaveFilesUploadOption::KeepDeviceActive };
    std::recursive_mutex m_folderSyncMutex;
#if _DEBUG
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

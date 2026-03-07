// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

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
        m_folderSync{ folderSync }
    {
        TRACE_TASK("FileResetCloudAsyncProvider ctor");
    }

    ~FileResetCloudAsyncProvider()
    {
        TRACE_TASK("FileResetCloudAsyncProvider dtor");
    }

    void ScheduleNow() override
    {
        TRACE_TASK("FileResetCloudAsyncProvider.ScheduleNow");
#if _DEBUG
        m_singleThreadProvider.AssertUponSchedule();
#endif
        Schedule(0);
    }

protected:
    HRESULT DoWork(RunContext runContext) override;
    SharedPtr<FolderSyncManager> m_folderSync;
    std::recursive_mutex m_folderSyncMutex;
#if _DEBUG
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

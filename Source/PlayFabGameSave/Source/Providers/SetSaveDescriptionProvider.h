// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

namespace PlayFab
{
namespace GameSave
{

class SetSaveDescriptionProvider : public GameSaveAsyncProvider<void>, public ISchedulableTask
{
public:
    template<size_t n>
    SetSaveDescriptionProvider(RunContext&& rc, XAsyncBlock* async, const char(&identityName)[n], SharedPtr<FolderSyncManager>&& folderSync, String&& shortSaveDescription) :
        GameSaveAsyncProvider{ std::move(rc), async, identityName },
        m_folderSync{ folderSync },
        m_shortSaveDescription{ std::move(shortSaveDescription) }
    {
        TRACE_TASK("SetSaveDescriptionProvider ctor");
    }

    ~SetSaveDescriptionProvider()
    {
        TRACE_TASK("SetSaveDescriptionProvider dtor");
    }

    void ScheduleNow() override
    {
        TRACE_TASK("SetSaveDescriptionProvider.ScheduleNow");
#if _DEBUG
        m_singleThreadProvider.AssertUponSchedule();
#endif
        Schedule(0);
    }

protected:
    HRESULT DoWork(RunContext runContext) override;
    SharedPtr<FolderSyncManager> m_folderSync;
    String m_shortSaveDescription;
    std::recursive_mutex m_folderSyncMutex;
    bool m_waitingForFinalize{ false };
    uint32_t m_waitElapsedMs{ 0 };
    static constexpr uint32_t c_waitSliceMs = 50;
    static constexpr uint32_t c_waitMaxMs = 5000; // cap wait at 5s
#if _DEBUG
    SingleThreadProviderValidation m_singleThreadProvider;
#endif
};

template<size_t n>
UniquePtr<SetSaveDescriptionProvider> MakeStartSetSaveDescriptionProvider(RunContext&& rc, XAsyncBlock* async, const char(&identityName)[n], SharedPtr<FolderSyncManager>&& folderSync, String shortSaveDescription)
{
    return MakeUnique<SetSaveDescriptionProvider>(std::move(rc), async, identityName, std::move(folderSync), std::move(shortSaveDescription));
}

} // namespace GameSave
} // namespace PlayFab

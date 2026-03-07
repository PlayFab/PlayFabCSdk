// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

#include "LockStep.h"
#include "CompareStep.h"
#include "DownloadStep.h"
#include "UploadStep.h"
#include "ResetCloudStep.h"
#include "SetSaveDescriptionStep.h"
#include "Manifest.h"
#include "FileFolderSet.h"
#include "ActiveDevicePollWorker.h"

namespace PlayFab
{
namespace GameSave
{
    
struct FolderSyncManagerProgress
{
    PFGameSaveFilesSyncState syncState;
    uint64_t current;
    uint64_t total;
};

enum class DeleteAllStage
{
    DeleteAllStageStarted,
    DeleteAllStageDone
};

class FolderSyncManager
{
public:
    FolderSyncManager(_In_ LocalUser const& localUser);
    ~FolderSyncManager();

    HRESULT DoWorkFolderDownload(_In_ const RunContext& runContext, _In_ ISchedulableTask& task, _In_ std::recursive_mutex& folderSyncMutex);
    HRESULT DoWorkFolderUpload(_In_ RunContext& runContext, _In_ ISchedulableTask& task, _In_ std::recursive_mutex& folderSyncMutex, _In_ PFGameSaveFilesUploadOption option);
    HRESULT DoWorkResetCloud(_In_ const RunContext& runContext, _In_ ISchedulableTask& task, _In_ std::recursive_mutex& folderSyncMutex);
    HRESULT DoWorkSetSaveDescription(_In_ const RunContext& runContext, _In_ ISchedulableTask& task, _In_ std::recursive_mutex& folderSyncMutex, _In_ const String& shortSaveDescription);
    HRESULT InitForDownload();
    HRESULT InitForUpload();

    FolderSyncManagerProgress GetSyncProgress();
    void SetSyncStateProgress(_In_ PFGameSaveFilesSyncState state, _In_ uint64_t cur, _In_ uint64_t total);
    int64_t GetRemainingQuota() const;
    const String& GetFolder() const;
    UICallbackManager& GetUIManager();
    void SetStatsForDebug();
    const String& GetStatsJsonForDebug() const;
    void SetForcedDisconnectFromCloud(bool isForcedDisconnectFromCloud) { m_isForcedDisconnectFromCloud = isForcedDisconnectFromCloud; }
    bool IsForcedDisconnectFromCloud() const { return m_isForcedDisconnectFromCloud; }
    bool IsDeviceReleasedAsActive() const { return m_deviceReleasedAsActive; }
    void ResetSetSaveDescriptionStep();
    void SetLastShortSaveDescription(const String& shortSaveDescription);
    bool HasStartedFinalizeManifest() const { return m_uploadStep.HasStartedFinalizeManifest(); }

    void SetAddUserOptions(PFGameSaveFilesAddUserOptions o) { m_addUserOptions = o; }
    PFGameSaveFilesAddUserOptions GetAddUserOptions() const { return m_addUserOptions; }

    static HRESULT ConvertToPFGameSaveDescriptor(const ManifestWrap& manifest, PFGameSaveDescriptor& gameSave);
private:
    void FireActivationFailedTelemetry(HRESULT hr, bool offline);

    LocalUser m_localUser;
    String m_saveFolder;
    String m_statsJsonForDebug;
    UICallbackManager m_uiManager;

    SharedPtr<GameSaveTelemetryManager> m_telemetryManager{};

    LockStep m_lockStep;
    CompareStep m_compareStep;
    DownloadStep m_downloadStep;
    UploadStep m_uploadStep;
    ResetCloudStep m_resetCloudStep;
    SetSaveDescriptionStep m_setSaveDescriptionStep;

    SharedPtr<ManifestInternal> m_latestPendingManifest;
    SharedPtr<ManifestInternal> m_latestFinalizedManifest;
    SharedPtr<FileFolderSet> m_localFileFolderSet;
    SharedPtr<FileFolderSet> m_remoteFileFolderSet;
    String m_lastShortSaveDescription;

    std::mutex m_progressMutex;
    bool m_deviceReleasedAsActive{ false };
    bool m_isForcedDisconnectFromCloud{ false };
    bool m_pollingForActiveDeviceChange{ false };
    FolderSyncManagerProgress m_syncProgress{ PFGameSaveFilesSyncState::NotStarted, 0, 0 };
    PFGameSaveFilesAddUserOptions m_addUserOptions{ PFGameSaveFilesAddUserOptions::None };
    SharedPtr<ActiveDevicePollWorker> m_tokenRefreshWorker;
                      
    DeleteAllStage m_deleteStage{ DeleteAllStage::DeleteAllStageStarted };
};

} // namespace GameSave
} // namespace PlayFab
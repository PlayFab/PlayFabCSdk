// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

#include "LockStep.h"
#include "RelockStep.h"
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

    // The one lock that serializes all workflow state for this user. It has to live on the manager
    // rather than on each async provider: a per-provider lock only orders one provider's DoWork
    // against its own continuations, so two providers driving the same manager (e.g.
    // SetSaveDescription while an AddUser download is still in flight) held different locks and
    // mutated the same steps, manifests and file/folder sets concurrently.
    std::recursive_mutex& GetSyncMutex() { return m_syncMutex; }

    FolderSyncManagerProgress GetSyncProgress();
    void SetSyncStateProgress(_In_ PFGameSaveFilesSyncState state, _In_ uint64_t cur, _In_ uint64_t total);
    int64_t GetRemainingQuota() const;
    const String& GetFolder() const;
    UICallbackManager& GetUIManager();
    void SetStatsForDebug();
    const String& GetStatsJsonForDebug() const;
    void SetForcedDisconnectFromCloud(bool isForcedDisconnectFromCloud) { m_isForcedDisconnectFromCloud.store(isForcedDisconnectFromCloud); }
    bool IsForcedDisconnectFromCloud() const { return m_isForcedDisconnectFromCloud.load(); }
    bool IsDeviceReleasedAsActive() const { return m_deviceReleasedAsActive; }
    bool IsSetSaveDescriptionStepDone() const { return m_setSaveDescriptionStep.IsSetDone(); }
    void ResetSetSaveDescriptionStep();
    void ResetResetCloudStep();
    bool IsResetCloudInProgress() const { return m_resetCloudStep.IsResetInProgress(); }
    // Atomically admits one upload: validates the current sync state and, on success, transitions
    // to PreparingForUpload. Returns the state it replaced so a failed provider start can restore it.
    HRESULT TryReserveUpload(_Out_ PFGameSaveFilesSyncState& previousSyncState);
    // Atomically admits one AddUser/download: validates the reset reservation and the current sync
    // state and, on success, transitions to PreparingForDownload. Returns the state it replaced so
    // a failed provider start can restore it.
    HRESULT TryReserveDownload(_In_ PFGameSaveFilesAddUserOptions options, _Out_ PFGameSaveFilesSyncState& previousSyncState, _Out_ bool& previousForcedDisconnectFromCloud);
    // Atomically admits one ResetCloud and resets its step. Returns false if one is already active.
    bool TryReserveResetCloud();
    bool IsResetCloudReserved();
    void ReleaseResetCloudReservation();
    void SetLastShortSaveDescription(const String& shortSaveDescription, bool dirty = false);
    void ClearDescriptionDirty() { m_descriptionDirty = false; }
    String GetSaveDescriptionForDebug() const;
    bool HasStartedFinalizeManifest() const { return m_uploadStep.HasStartedFinalizeManifest(); }

    void SetAddUserOptions(PFGameSaveFilesAddUserOptions o) { m_addUserOptions = o; }
    PFGameSaveFilesAddUserOptions GetAddUserOptions() const { return m_addUserOptions; }
    
    // Cancel any pending UI wait during cleanup/shutdown.
    // Called by GameSaveGlobalState before termination to prevent hangs.
    void CancelPendingUIWaitForCleanup();

    // This function internal to the SDK allows platforms that mount the underlying file system folder to override
    // where the files are stored, as it might not correspond to the save folder specified in the public API calls.
    void SetSaveFolderOverride(const String& folder);

    static HRESULT ConvertToPFGameSaveDescriptor(const ManifestWrap& manifest, PFGameSaveDescriptor& gameSave);

private:
    void FireActivationFailedTelemetry(HRESULT hr, bool offline);

    LocalUser m_localUser;
    String m_saveFolder;
    String m_statsJsonForDebug;
    UICallbackManager m_uiManager;

    SharedPtr<GameSaveTelemetryManager> m_telemetryManager{};

    LockStep m_lockStep;
    RelockStep m_relockStep;
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
    bool m_descriptionDirty{ false };          // True if description was set offline and not yet uploaded

    std::mutex m_progressMutex;
    std::recursive_mutex m_syncMutex; // see GetSyncMutex()
    bool m_resetCloudReserved{ false }; // guarded by m_progressMutex
    bool m_deviceReleasedAsActive{ false };
    std::atomic<bool> m_isForcedDisconnectFromCloud{ false };
    bool m_pollingForActiveDeviceChange{ false };
    FolderSyncManagerProgress m_syncProgress{ PFGameSaveFilesSyncState::NotStarted, 0, 0 };
    PFGameSaveFilesAddUserOptions m_addUserOptions{ PFGameSaveFilesAddUserOptions::None };
    SharedPtr<ActiveDevicePollWorker> m_tokenRefreshWorker;
                      
    DeleteAllStage m_deleteStage{ DeleteAllStage::DeleteAllStageStarted };
    bool m_conflictUploadStarted{ false };   // Reuse UploadStep during AddUser conflict path
    bool m_conflictUploadCompleted{ false }; // Set once UploadStep finishes
};

} // namespace GameSave
} // namespace PlayFab

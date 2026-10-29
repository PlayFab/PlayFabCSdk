// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include "ExtendedManifest.h"
#include "Manifest.h"
#include "ProgressHelpers.h"

namespace PlayFab
{
namespace GameSave
{
    
// Forward declare
class FolderSyncManager;
struct FolderSyncManagerProgress;

enum class DownloadStage
{
    DownloadStart = 0,
    QueryStorage,
    Download,
    RefreshLinks,
    UncompressFiles,
    UpdateLocalManifest,
    WaitForFailedUI_Download,
    WaitForOutOfStorageUI,
    DownloadStepFailure,
    DownloadDone
};

class DownloadStep
{
public:
    DownloadStep(_In_ LocalUser const& localUser, _In_ SharedPtr<GameSaveTelemetryManager> telemetryManager);
    void SetEntity(_In_ const Entity& entity);

    void Reset();

    // AddUser (OQ-15): the finalized version being downloaded. When a transfer fails with an expired
    // link (HTTP 403) and the player answers Retry, the step fetches fresh links for this version
    // (GetManifestDownloadDetails) before downloading again; retrying with the expired links could
    // only fail again. 0 (the default; Reset() restores it): Retry reuses the links in hand.
    void SetLinkRefreshVersion(uint64_t version) { m_linkRefreshVersion = version; }

    bool IsDownloadDone() const;
    // True once Download() left its first stage (core unit test plan H-5: "Download.AfterLocalOps"
    // fires until the transfers start).
    bool HasStarted() const { return m_stage != DownloadStage::DownloadStart; }
    bool IsForceDisconnectFromCloud() const { return m_forceDisconnectFromCloud; }
    HRESULT Download(
        _In_ const RunContext& runContext,
        _In_ ISchedulableTask& task,
        _In_ const String& saveFolder,
        _In_ UICallbackManager& uiCallbackManager,
        _In_ SharedPtr<FileFolderSet>& localFileFolderSet,
        _In_ SharedPtr<FileFolderSet>& remoteFileFolderSet,
        _In_ FolderSyncManagerProgress& syncProgress,
        _In_ std::recursive_mutex& folderSyncMutex,
        _In_ ProgressCallback progressCallback,
        _In_ void* progressCallbackContext,
        _In_ const String& shortSaveDescription,
        _In_ bool descriptionDirty
    );

    HRESULT DeleteFiles(
        _In_ const String& saveFolder,
        _In_ const SharedPtr<FileFolderSet>& localFileFolderSet
    );

    HRESULT DeleteFolders(
        _In_ const String& saveFolder,
        _In_ const SharedPtr<FileFolderSet>& localFileFolderSet
    );

    HRESULT CreateEmptyFolders(
        _In_ const String& saveFolder, 
        _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet
    );

    bool IsLocalOperationsDone() const;
    void SetLocalOperationsDone(bool state);

private:
    HRESULT UncompressFile(
        _In_ const CompressedFile& remoteFile, 
        _In_ const String& saveFolder, 
        _In_ const String& fullCompressedFilePath, 
        _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet
    );

    // Moves to the out-of-storage UI (or fails with E_PF_GAMESAVE_DISK_FULL when no callback is set).
    void EnterOutOfStorage(_In_ ISchedulableTask& task, _In_ UICallbackManager& uiCallbackManager, _In_ uint64_t requiredBytes);

    uint64_t m_linkRefreshVersion{ 0 };
    bool m_lastFailureExpiredLink{ false }; // the failure the SyncFailed prompt shows was an expired link
    bool m_linkRefreshInFlight{ false };

    uint64_t m_totalUncompressedSizeBytes{};
    uint64_t m_currentUncompressedSizeBytes{};
    uint64_t m_totalCompressedSizeBytes{};
    // Cumulative compressed bytes successfully downloaded so far. Used for accurate dynamic progress.
    uint64_t m_currentCompressedSizeBytes{};
    LocalUser m_localUser;
    std::optional<Entity> m_entity;
    DownloadStage m_stage{ DownloadStage::DownloadStart };
    HRESULT m_failureHR{ S_OK };
    bool m_forceDisconnectFromCloud{ false };
    bool m_isLocalOperationsDone{ false };
    SharedPtr<GameSaveTelemetryManager> m_telemetryManager;
};

} // namespace GameSave
} // namespace PlayFab

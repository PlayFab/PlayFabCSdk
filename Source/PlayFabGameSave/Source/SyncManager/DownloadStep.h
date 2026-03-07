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
    UncompressFiles,
    UpdateLocalManifest,
    WaitForFailedUI_Download,
    WaitForOutOfStorageUI,
    LockStepFailure,
    DownloadDone
};

class DownloadStep
{
public:
    DownloadStep(_In_ LocalUser const& localUser, _In_ SharedPtr<GameSaveTelemetryManager> telemetryManager);
    void SetEntity(_In_ const Entity& entity);

    void Reset();

    bool IsDownloadDone() const;
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
        _In_ const String& shortSaveDescription
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

    uint64_t m_totalUncompressedSizeBytes{};
    uint64_t m_currentUncompressedSizeBytes{};
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

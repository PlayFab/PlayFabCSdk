// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include "ExtendedManifest.h"
#include "Manifest.h"

namespace PlayFab
{
namespace GameSave
{

// Forward declare
class FolderSyncManager;

enum class CompareStage
{
    GetManifestDownloadDetails,
    GetExtendedManifest,
    ReadLocalManifest,
    WaitForConflictUI,
    WaitForFailedUI_GetManifestDownloadDetails,
    WaitForFailedUI_GetExtendedManifest,
    CompareStepFailure,
    CompareDone
};

enum class TakeUIChoice
{
    NoChoiceYet,
    TakeLocal,
    TakeRemote
};

class CompareStep
{
public:
    CompareStep(_In_ LocalUser const& localUser, _In_ SharedPtr<GameSaveTelemetryManager> telemetryManager);
    void SetEntity(_In_ const Entity& entity);
    void Reset();

    bool IsCompareDone() const;
    bool IsForceDisconnectFromCloud() const { return m_forceDisconnectFromCloud; }
    HRESULT CompareWithCloud(
        _In_ RunContext runContext,
        _In_ ISchedulableTask& task,
        _In_ const String& saveFolder,
        _In_ UICallbackManager& uiCallbackManager,
        _In_ bool downloading,
        _In_ const SharedPtr<ManifestInternal>& latestFinalizedManifest,
        _In_ SharedPtr<FileFolderSet>& localFileFolderSet,
        _In_ SharedPtr<FileFolderSet>& remoteFileFolderSet,
        _In_ std::recursive_mutex& folderSyncMutex
    );

private:
    HRESULT HandleConflict(
        _In_ UICallbackManager& uiCallbackManager,
        _In_ ISchedulableTask& task,
        _In_ const SharedPtr<ManifestInternal>& latestFinalizedManifest,
        _In_ SharedPtr<FileFolderSet>& localFileFolderSet,
        _In_ SharedPtr<FileFolderSet>& remoteFileFolderSet,
        _In_ const String& saveFolder,
        _In_ const String& shortSaveDescription
        );

    static void MarkFilesToSync(
        _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
        _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet,
        _In_ const String& saveFolder,
        _Out_ bool& conflictFound);

    static void MarkFilesToTransferUponUpload(_In_ const SharedPtr<FileFolderSet>& localFileFolderSet);
    static void MarkFilesToTransferUponDownload(_In_ const SharedPtr<FileFolderSet>& localFileFolderSet, _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet, _In_ const String& saveFolder);
    static void ScanForConflicts(_In_ const SharedPtr<FileFolderSet>& localFileFolderSet, _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet, _Out_ bool& conflictFound);
    static void ScanForConflictHelper(_In_ const SharedPtr<FileFolderSet>& localFileFolderSet, _In_ const FileDetail* localFileToUpload, _In_ const Set<String>& topLevelFoldersNeedingDownload, _Out_ bool& conflictFound);

    static void MarkFilesToDeleteUponUpload(_In_ const SharedPtr<FileFolderSet>& localFileFolderSet);
    static void MarkFilesToDeleteUponDownload(_In_ const SharedPtr<FileFolderSet>& localFileFolderSet, _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet);
    static void MarkFoldersToDeleteUponUpload(_In_ const SharedPtr<FileFolderSet>& localFileFolderSet, _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet);
    static void MarkFoldersToCreateUponUpload(_In_ const SharedPtr<FileFolderSet>& localFileFolderSet, _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet);
    static void MarkFoldersToCreateUponDownload(_In_ const SharedPtr<FileFolderSet>& localFileFolderSet, _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet);
    static void MarkFoldersToDeleteUponDownload(_In_ const SharedPtr<FileFolderSet>& localFileFolderSet, _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet);

    static void MarkCompressedFilesToKeep(_In_ const SharedPtr<FileFolderSet>& localFileFolderSet, _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet);
    static void HasLocalFileChanged(_In_ const FileDetail& localFile, _Out_ bool& localFileDeleted, _Out_ bool& localFileChanged);
    static bool HasRemoteFileChanged(_In_ const FileDetail& localFile, _In_ const FileDetail& remoteFile);

    LocalUser m_localUser;
    std::optional<Entity> m_entity;
    CompareStage m_stage{ CompareStage::GetManifestDownloadDetails };
    HRESULT m_failureHR{ S_OK };
    bool m_forceDisconnectFromCloud{ false };
    TakeUIChoice m_takeUIChoice{ TakeUIChoice::NoChoiceYet };
    SharedPtr<GameSaveTelemetryManager> m_telemetryManager{};
};

} // namespace GameSave
} // namespace PlayFab

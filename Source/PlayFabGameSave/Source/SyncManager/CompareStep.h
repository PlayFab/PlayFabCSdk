// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include "ExtendedManifest.h"
#include "Manifest.h"
#include "Wrappers/GameSaveServiceSelector.h" // for ManifestWrap / ManifestWrapVector

namespace PlayFab
{
namespace GameSave
{

// Forward declare
class FolderSyncManager;

// The atomic sync unit of a folder: its top-level folder (CompareStepRules.cpp). The root files' unit
// is "".
String GetTopLevelFolder(const String& relFolderPath);

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
    bool DidConflictOccur() const { return m_conflictOccurred; }
    bool ConflictRequiresUpload() const { return m_conflictRequiresUpload; }
    TakeUIChoice GetConflictChoice() const { return m_takeUIChoice; }
    const String& GetLoadedShortSaveDescription() const { return m_loadedShortSaveDescription; }
    bool GetLoadedDescriptionDirty() const { return m_loadedDescriptionDirty; }
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
    // Unit tests only (core unit test plan H-1): calls the private static rules below (CompareStepRules.cpp).
    // Defined in the test executable; nothing in the SDK defines or uses it.
    friend class CompareStepTestAccess;

    HRESULT HandleConflict(
        _In_ UICallbackManager& uiCallbackManager,
        _In_ ISchedulableTask& task,
        _In_ const SharedPtr<ManifestInternal>& latestFinalizedManifest,
        _In_ SharedPtr<FileFolderSet>& localFileFolderSet,
        _In_ SharedPtr<FileFolderSet>& remoteFileFolderSet,
        _In_ const String& saveFolder,
        _In_ const String& shortSaveDescription
        );

    // OQ-1 (Take Remote): every conflicting unit becomes the cloud's copy. Its files that differ from
    // the cloud (edited or deleted locally) download again, and what the cloud doesn't have (files and
    // folders, synced or not) is removed locally. Other units are untouched.
    void MirrorConflictingUnitsFromCloud(
        _In_ const Set<String>& conflictingUnits,
        _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
        _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet,
        _In_ const String& saveFolder);

    // OQ-1 (Take Local): every conflicting unit's local copy is made authoritative for the next upload.
    // Each synced file of the unit gets a baseline time no file has, so the next upload sends the whole
    // unit (and drops the cloud-only files of it); persisted, so a session that ends before that upload
    // prompts again rather than reading the unit as in sync.
    HRESULT PersistTakeLocalAuthority(
        _In_ const Set<String>& conflictingUnits,
        _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
        _In_ const String& saveFolder);

    void ReconcileMetadataLoss(
        _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
        _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet,
        _In_ const String& saveFolder,
        _In_ const String& shortSaveDescription,
        _In_ bool descriptionDirty);

    static void MarkFilesToSync(
        _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
        _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet,
        _In_ const String& saveFolder,
        _Out_ bool& conflictFound);

    static void MarkFilesToTransferUponUpload(_In_ const SharedPtr<FileFolderSet>& localFileFolderSet);
    static void MarkFilesToTransferUponDownload(_In_ const SharedPtr<FileFolderSet>& localFileFolderSet, _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet, _In_ const String& saveFolder);
    static void ScanForConflicts(_In_ const SharedPtr<FileFolderSet>& localFileFolderSet, _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet, _Out_ bool& conflictFound);
    // The atomic units (GetTopLevelFolder) in conflict: ScanForConflicts' rule, unit by unit. OQ-1: the
    // conflict choice applies to these units whole.
    static Set<String> CollectConflictingUnitKeys(_In_ const SharedPtr<FileFolderSet>& localFileFolderSet, _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet);

    static void MarkFilesToDeleteUponUpload(_In_ const SharedPtr<FileFolderSet>& localFileFolderSet);
    static void MarkFilesToDeleteUponDownload(_In_ const SharedPtr<FileFolderSet>& localFileFolderSet, _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet);
    static void MarkFoldersToDeleteUponUpload(_In_ const SharedPtr<FileFolderSet>& localFileFolderSet, _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet);
    static void MarkFoldersToCreateUponUpload(_In_ const SharedPtr<FileFolderSet>& localFileFolderSet, _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet);
    static void MarkFoldersToCreateUponDownload(_In_ const SharedPtr<FileFolderSet>& localFileFolderSet, _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet);
    static void MarkFoldersToDeleteUponDownload(_In_ const SharedPtr<FileFolderSet>& localFileFolderSet, _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet);

    static void MarkCompressedFilesToKeep(_In_ const SharedPtr<FileFolderSet>& localFileFolderSet, _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet);
    static void HasLocalFileChanged(_In_ const FileDetail& localFile, _Out_ bool& localFileChanged, _Out_ bool& localFileDeleted);
    static bool HasRemoteFileChanged(_In_ const FileDetail& localFile, _In_ const FileDetail& remoteFile);

    LocalUser m_localUser;
    std::optional<Entity> m_entity;
    CompareStage m_stage{ CompareStage::GetManifestDownloadDetails };
    HRESULT m_failureHR{ S_OK };
    bool m_forceDisconnectFromCloud{ false };
    TakeUIChoice m_takeUIChoice{ TakeUIChoice::NoChoiceYet };
    SharedPtr<GameSaveTelemetryManager> m_telemetryManager{};
    bool m_conflictOccurred{ false };          // Set true once a conflict path was taken
    bool m_conflictRequiresUpload{ false };    // Signals FolderSyncManager to perform single upload finalize
    String m_loadedShortSaveDescription;       // Description loaded from localstate.json during compare
    bool m_loadedDescriptionDirty{ false };    // True if description was set offline and not yet uploaded
    HRESULT m_extendedManifestFailureHR{ S_OK }; // Non-S_OK once an extended manifest download failed for this activation
};

} // namespace GameSave
} // namespace PlayFab

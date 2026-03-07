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
struct FolderSyncManagerProgress;

enum class UploadStage
{
    UploadStart = 0,
    CompressFiles,
    InitiateUpload,
    UploadFile,
    FinalizeManifest,
    ListManifestsAfterUpload,
    TakeLockAfterUpload,
    ForceDisconnectFromCloud,
    WaitForFailedUI_InitiateUpload,
    WaitForFailedUI_UploadFile,
    WaitForFailedUI_FinalizeManifest,
    WaitForFailedUI_ListManifestsAfterUpload,
    WaitForFailedUI_TakeLockAfterUpload,
    LockStepFailure,
    UploadDone
};

enum class DeleteManifestStage
{
    DeleteManifestStart = 0,
    DeleteDone
};

class UploadStep
{
public:
    UploadStep(_In_ LocalUser const& localUser, _In_ SharedPtr<GameSaveTelemetryManager> telemetryManager);
    void SetEntity(const Entity& entity);
    void Reset();

    bool IsUploadDone() const;
    bool IsDeletePendingManifestDone() const;
    const ManifestWrap& GetPostUploadPendingPFManifest() const;
    const ManifestWrap& GetPostUploadLatestFinalizedPFManifest() const;
    uint64_t GetTotalCompressedSizeBytesUploaded() const;
#if _DEBUG // just for debug stats
    size_t GetNumFilesInFinalizedManifest() const { return m_numFilesInFinalizedManifest; }
#endif
    const FileDetail* PopThumbnail(_In_ Vector<const FileDetail*>& filesToUpload);

    HRESULT Upload(
        _In_ const RunContext& runContext,
        _In_ ISchedulableTask& task,
        _In_ const SharedPtr<ManifestInternal>& latestPendingManifest,
        _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
        _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet,
        _In_ const String& saveFolder,
        _In_ PFGameSaveFilesUploadOption option,
        _In_ UICallbackManager& uiCallbackManager,
        _In_ FolderSyncManagerProgress& syncProgress,
        _In_ std::recursive_mutex& folderSyncMutex,
        _In_ ProgressCallback progressCallback,
        _In_ void* progressCallbackContext,
        _In_ const String& shortSaveDescription
    );

    // Returns true once we have first entered the FinalizeManifest stage for the current upload sequence.
    // This remains true until Reset() is called.
    bool HasStartedFinalizeManifest() const { return m_hasStartedFinalizeManifest; }

    void SetToUploadFullSet(const SharedPtr<FileFolderSet>& localFileFolderSet, const SharedPtr<FileFolderSet>& remoteFileFolderSet);

    HRESULT DeletePendingManifest(
        _In_ const RunContext& runContext,
        _In_ ISchedulableTask& task,
        _In_ const SharedPtr<ManifestInternal>& latestPendingManifest,
        _In_ std::recursive_mutex& folderSyncMutex
    );

private:
    Result<Vector<ExtendedManifestCompressedFileDetail>> CompressFiles(
        _In_ const SharedPtr<FileFolderSet>& localFileFolderSet, 
        _In_ const String& saveFolder,
        _In_ const String& version);

    static Vector<Vector<const FileDetail*>> SplitUploadsIntoZipBatches(
        _In_ Vector<const FileDetail*> filesToUpload);

    static const FileDetail* UploadStep::PopFileDetailBelowSize(
        _In_ Vector<const FileDetail*>& filesToUpload, 
        _In_ uint64_t sizeLimit);

    static HRESULT AddUncompressedFile(
        _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
        _In_ const FileDetail* fileToUpload,
        _In_ const String& saveFolder,
        _Inout_ ExtendedManifestCompressedFileDetail& u);

    static HRESULT AddCompressedFile(
        _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
        _In_ const FileDetail* fileToUpload,
        _In_ const String& saveFolder,
        _Inout_ ExtendedManifestCompressedFileDetail& u);

    void UploadFileFinally(
        _In_ ISchedulableTask& task,
        _In_ UICallbackManager& uiCallbackManager,
        _In_ std::recursive_mutex& folderSyncMutex,
        _In_ const SharedPtr<InnerProgressContext>& innerProgressContext,
        _In_ HRESULT hr);

    LocalUser m_localUser;
    std::optional<Entity> m_entity;
    uint32_t m_compressedFilesToUploadCurIndex{ 0 };
    Vector<ExtendedManifestCompressedFileDetail> m_compressedFilesToUpload;
    PlayFab::GameSaveWrapper::InitiateUploadResponse m_initiateResult;
    UploadStage m_stage{ UploadStage::UploadStart };
    HRESULT m_failureHR{ S_OK };
    DeleteManifestStage m_deleteManifestStage{ DeleteManifestStage::DeleteManifestStart };

    uint64_t m_totalUncompressedSizeBytes{};
    uint64_t m_currentUncompressedSizeBytes{};
    uint64_t m_totalCompressedSizeBytes{};
    uint64_t m_manifestVersionOffset{ 0 };
    uint64_t m_uploadFullSetRetryCount{ 0 };
    ManifestWrapVector m_manifests;
    ManifestWrap m_postUploadPendingPFManifest;
    ManifestWrap m_postUploadLatestFinalizedPFManifest;
    SharedPtr<GameSaveTelemetryManager> m_telemetryManager;

    // Becomes true the first time we transition into UploadStage::FinalizeManifest for a given upload run.
    bool m_hasStartedFinalizeManifest{ false };

#if _DEBUG // just for debug stats
    size_t m_numFilesInFinalizedManifest{ 0 };
#endif
};

} // namespace GameSave
} // namespace PlayFab
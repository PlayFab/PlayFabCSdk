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
    PromoteIfNeeded, 
    TakeLock,        
    ForceDisconnectFromCloud,
    WaitForFailedUI_InitiateUpload,
    WaitForFailedUI_UploadFile,
    WaitForFailedUI_FinalizeManifest,
    WaitForFailedUI_ListManifestsAfterUpload,
    WaitForFailedUI_PromoteIfNeeded, 
    WaitForFailedUI_TakeLock,
    UploadStepFailure,
    UploadDone
};

enum class DeleteManifestStage
{
    DeleteManifestStart = 0,
    DeleteDone
};

// Structure to hold conflict metadata for FinalizeManifest
struct ConflictMetadata
{
    bool hasConflict = false;
    bool isWinner = false;
    String conflictingVersion;
    
    ConflictMetadata() = default;
    ConflictMetadata(bool winner, const String& conflictingVer) 
        : hasConflict(true), isWinner(winner), conflictingVersion(conflictingVer) {}
};

class UploadStep
{
public:
    UploadStep(_In_ LocalUser const& localUser, _In_ SharedPtr<GameSaveTelemetryManager> telemetryManager);
    void SetEntity(_In_ const Entity& entity);
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
    void SetOriginalActivationBaselineVersion(uint64_t v) { m_originalActivationBaselineVersion = v; }
    uint64_t GetOriginalActivationBaselineVersion() const { return m_originalActivationBaselineVersion; }

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
        _In_ const String& shortSaveDescription,
        _In_ const ConflictMetadata& conflictMetadata = ConflictMetadata()
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

    static const FileDetail* PopFileDetailBelowSize(
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

    enum class KnownGoodPromotionResult
    {
        NotApplicable,     // No baseline / no successor yet
        AlreadyKnownGood,  // Baseline already marked (or previously promoted)
        NeedsUpdateCall    // Eligible: issue UpdateManifest(IsKnownGood=true)
    };

    KnownGoodPromotionResult EvaluateKnownGoodPromotionEligibility();

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
    // Cumulative compressed bytes successfully uploaded so far. Used for accurate dynamic progress.
    uint64_t m_currentCompressedSizeBytes{};
    uint64_t m_manifestVersionOffset{ 0 };
    uint64_t m_uploadFullSetRetryCount{ 0 };
    ManifestWrapVector m_manifests;
    ManifestWrap m_postUploadPendingPFManifest;
    ManifestWrap m_postUploadLatestFinalizedPFManifest;
    SharedPtr<GameSaveTelemetryManager> m_telemetryManager;
    uint64_t m_originalActivationBaselineVersion{ 0 }; // Tracks the finalized manifest version that was chosen as the baseline when AddUser/activation began.
    bool m_originalBaselinePromoted{ false };
    ConflictMetadata m_conflictMetadata; // Conflict metadata for FinalizeManifest

    // Becomes true the first time we transition into UploadStage::FinalizeManifest for a given upload run.
    bool m_hasStartedFinalizeManifest{ false };

#if _DEBUG // just for debug stats
    size_t m_numFilesInFinalizedManifest{ 0 };
#endif
};

} // namespace GameSave
} // namespace PlayFab
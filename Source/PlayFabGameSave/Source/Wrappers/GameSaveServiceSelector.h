#pragma once

#include "Generated/GameSaveTypeWrappers.h"
#include "Generated/GameSaveTypes.h"
#include "Wrappers/Types.h"
#include "Compression.h"

namespace PlayFab
{

using ManifestWrap = PlayFab::Wrappers::PFGameSaveManifestWrapper<Allocator>;
using ManifestWrapVector = PlayFab::Wrappers::ModelVector<ManifestWrap, Allocator>;

using DownloadDetailsWrap = PlayFab::Wrappers::PFGameSaveFileDownloadDetailsWrapper<Allocator>;
using DownloadDetailsWrapVector = PlayFab::Wrappers::ModelVector<DownloadDetailsWrap, Allocator>;

using FileMetadataWrap = PlayFab::Wrappers::PFGameSaveFileMetadataWrapper<Allocator>;
using FileMetadataWrapVector = PlayFab::Wrappers::ModelVector<FileMetadataWrap, Allocator>;

using FileToUploadWrap = PlayFab::Wrappers::PFGameSaveFileToUploadWrapper<Allocator>;
using FileToUploadWrapVector = PlayFab::Wrappers::ModelVector<FileToUploadWrap, Allocator>;

using AllocatedFileWrap = PlayFab::Wrappers::PFGameSaveAllocatedFileWrapper<Allocator>;
using AllocatedFileWrapVector = PlayFab::Wrappers::ModelVector<AllocatedFileWrap, Allocator>;

using StringWrapVector = PlayFab::Wrappers::CStringVector<Allocator>;

namespace GameSaveWrapper
{

enum class ManifestStatus
{
    Initialized,
    Uploading,
    Finalized,
    Quarantined,
    PendingDeletion
};

struct DownloadFileDetail
{
    String fullFilePath;
    SharedPtr<ArchiveContext> archiveContext;
};

struct UploadFileDetail
{
    String fileName;
    String fullFilePath;
    SharedPtr<ArchiveContext> archiveContext;
};

class GameSaveServiceSelector
{
public:
    GameSaveServiceSelector() = delete;
    GameSaveServiceSelector(const GameSaveServiceSelector& source) = delete;
    GameSaveServiceSelector& operator=(const GameSaveServiceSelector& source) = delete;
    ~GameSaveServiceSelector() = default;

    static AsyncOp<void> DownloadFileFromCloud(
        RunContext runContext,
        const DownloadFileDetail& fileDetail,
        const String& downloadUrl,
        HCHttpCallProgressReportFunction progressCallback,
        void* progressCallbackContext,
        const uint64_t customTotalSizeBytes,
        const uint64_t customCurrentSizeBytes
    );
    static AsyncOp<Vector<char>> DownloadFileFromCloudToBytes(
        RunContext runContext, 
        const String& downloadUrl, 
        const String& mockFilePath
    );
    static AsyncOp<void> UploadSingleFileToCloud(
        RunContext runContext,
        const UploadFileDetail& fileDetail,
        const PlayFab::Wrappers::ModelVector<PlayFab::Wrappers::PFGameSaveAllocatedFileWrapper<Allocator>, Allocator>& initiateResult,
        HCHttpCallProgressReportFunction progressCallback,
        void* progressCallbackContext,
        const uint64_t customTotalSizeBytes,
        const uint64_t customCurrentSizeBytes
    );
    static AsyncOp<void> UploadFileFromStringToCloud(
        RunContext runContext,
        const UploadFileDetail& fileDetail,
        const String& fileContent,
        const PlayFab::Wrappers::ModelVector<PlayFab::Wrappers::PFGameSaveAllocatedFileWrapper<Allocator>, Allocator>& initiateResult
    );

    // These should match generated API calls
    static AsyncOp<ListManifestsResponse> ListManifests(Entity const& entity, const ListManifestsRequest& request, RunContext rc);
    static AsyncOp<GetManifestDownloadDetailsResponse> GetManifestDownloadDetails(Entity const& entity, const GetManifestDownloadDetailsRequest& request, RunContext rc);
    static AsyncOp<GetQuotaForPlayerResponse> GetQuotaForPlayer(Entity const& entity, const GetQuotaForPlayerRequest& request, RunContext rc);
    static AsyncOp<InitializeManifestResponse> InitializeManifest(Entity const& entity, const InitializeManifestRequest& request, RunContext rc);
    static AsyncOp<InitiateUploadResponse> InitiateUpload(Entity const& entity, const InitiateUploadRequest& request, RunContext rc, const Vector<UploadFileDetail>& mockFilesRequest);
    static AsyncOp<FinalizeManifestResponse> FinalizeManifest(Entity const& entity, const FinalizeManifestRequest& request, RunContext rc);
    static AsyncOp<UpdateManifestResponse> UpdateManifest(Entity const& entity, const UpdateManifestRequest& request, RunContext rc);
    static AsyncOp<void> DeleteManifest(Entity const& entity, const DeleteManifestRequest& request, RunContext rc);

    static bool useMocks;
};


} // namespace GameSave
} // namespace PlayFab

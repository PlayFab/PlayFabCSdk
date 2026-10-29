// Copyright (C) Microsoft Corporation. All rights reserved.
// CompareStep's pure change and conflict detection rules (MarkFilesToSync and the lists it builds,
// HasLocalFileChanged, HasRemoteFileChanged, ScanForConflicts, MarkCompressedFilesToKeep). They only
// read the two FileFolderSets, so they live in their own translation unit: the unit test executable
// compiles this file directly (white-box seam, core unit test plan H-1) without the rest of the step.
// Moved unchanged from CompareStep.cpp.
#include "stdafx.h"
#include "CompareStep.h"
#include "ApiHelpers.h"
#include "FileFolderSet.h"
#include "Platform/PFGameSaveFilesAPIProvider.h"

using namespace PlayFab::GameSaveWrapper;

namespace PlayFab
{
namespace GameSave
{

bool IsFileQueuedForUpload(const FileDetail* localFile, const SharedPtr<FileFolderSet>& localFileFolderSet)
{
    const Vector<const FileDetail*>& localFilesToUpload = localFileFolderSet->GetFilesToUpload();
    for (const FileDetail* localFileToUpload : localFilesToUpload)
    {
        if (localFileToUpload == localFile)
        {
            return true;
        }
    }

    return false;
}

void CompareStep::MarkFilesToSync(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet,
    _In_ const String& saveFolder,
    _Out_ bool& conflictFound)
{    
    MarkFilesToTransferUponUpload(localFileFolderSet); 
    MarkFilesToTransferUponDownload(localFileFolderSet, remoteFileFolderSet, saveFolder);
    MarkFilesToDeleteUponUpload(localFileFolderSet);
    MarkFilesToDeleteUponDownload(localFileFolderSet, remoteFileFolderSet);
    MarkFoldersToCreateUponUpload(localFileFolderSet, remoteFileFolderSet);
    MarkFoldersToCreateUponDownload(localFileFolderSet, remoteFileFolderSet);
    MarkFoldersToDeleteUponUpload(localFileFolderSet, remoteFileFolderSet);
    MarkFoldersToDeleteUponDownload(localFileFolderSet, remoteFileFolderSet);
    // TODO: ScanForConflicts checks file uploads/deletions against topLevelFoldersNeedingDownload
    // (and a remote delete of a locally changed file). Folder-only changes (empty folder create/delete)
    // in an atomic unit that also has remote changes will not trigger conflict UI.
    // To fully enforce the all-or-nothing conflict model, the scan should also check
    // local folder creates/deletes against topLevelFoldersNeedingDownload.
    //
    // Note: a remotely-deleted folder is also invisible here, because it has no incoming
    // downloads and so never lands in changedRemoteFolderIndexes. That used to cause silent
    // data loss when the folder still held never-synced local files. DownloadStep::DeleteFolders
    // now prunes instead of recursively deleting, so those files survive (Bug 63588283); the
    // conflict-detection gap described above is still open.
    ScanForConflicts(localFileFolderSet, remoteFileFolderSet, conflictFound);
}

void CompareStep::MarkFilesToTransferUponUpload(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet)
{
    const Vector<FileDetail>& localFiles = localFileFolderSet->GetFiles();
    Vector<const FileDetail*> filesToUpload;
    for (const FileDetail& localFile : localFiles)
    {
        // for each local file, see if the size or last write time changed since last time we 
        // uploaded/downloaded the file. if any did, that means it changed locally and that change 
        // needs to be uploaded to cloud
        bool localFileChanged = false;
        bool localFileDeleted = false;
        HasLocalFileChanged(localFile, localFileChanged, localFileDeleted);
        if (localFileChanged)
        {
            filesToUpload.push_back(&localFile);
        }
    }
    TRACE_INFORMATION("[GAME SAVE] CompareStep: FilesToUpload: %zd", filesToUpload.size());
    localFileFolderSet->SetFilesToUpload(std::move(filesToUpload));
}

void CompareStep::MarkFilesToTransferUponDownload(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet, 
    _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet,
    _In_ const String& saveFolder)
{
    const Vector<FileDetail>& remoteFiles = remoteFileFolderSet->GetFiles();
    const Vector<CompressedFile>& compressedFiles = remoteFileFolderSet->GetCompressedFiles();
    Set<size_t> compressedFileIndexSet;
    Set<size_t> changedRemoteFolderIndexSet;

    // Conflict resolution re-enters ReadLocalManifest, which runs this selection again against the
    // same remote set (and therefore the same ArchiveContext objects). Start from a clean slate so
    // re-adding the same files doesn't accumulate against the archive size limits.
    for (const CompressedFile& compressedFile : compressedFiles)
    {
        if (compressedFile.archiveContext != nullptr)
        {
            compressedFile.archiveContext->ClearFiles();
        }
    }

#if defined(_DEBUG)
    Vector<const FileDetail*> filesToDownload;
#endif
    for (const FileDetail& remoteFile : remoteFiles)
    {
        // for each remote file, see if remote file's ID or size or last write time changed since 
        // last time we uploaded/downloaded the file.  if any did, that means it changed remotely 
        // and that change needs to be downloaded from cloud

        String relFilePath = remoteFileFolderSet->GetRelFilePath(&remoteFile);
        const FileDetail* localFile = localFileFolderSet->GetFileDetailFromRelFilePath(relFilePath);
        if (localFile)
        {
            if (localFile->behindLink)
            {
                // Its recorded path runs through a link the scan doesn't follow: never written
                // through it (the cloud copy stays in the cloud; this device keeps what it has).
                continue;
            }
            if (HasRemoteFileChanged(*localFile, remoteFile))
            {
                ArchiveFileDetail afd{};
                if (FAILED(JoinPathHelper(saveFolder, relFilePath, afd.fullPath)))
                {
                    TRACE_WARNING("[GAME SAVE] CompareStep: JoinPathHelper failed for download file '%s'", relFilePath.c_str());
                    continue;
                }
                afd.uncompressedSize = remoteFile.fileSizeBytes;
                afd.timeLastModified = remoteFile.timeLastModified;
                afd.timeCreated = remoteFile.timeCreated;
                HRESULT addHr = compressedFiles[remoteFile.compressedFileIndex].archiveContext->AddFile(relFilePath, std::move(afd));
                if (FAILED(addHr))
                {
                    // Rejected by the archive's path/size validation - don't schedule an
                    // extraction the archive layer will never perform.
                    TRACE_WARNING("[GAME SAVE] CompareStep: AddFile rejected download file '%s' (hr=0x%08X)", relFilePath.c_str(), addHr);
                    continue;
                }

                // Only schedule the bundle once the file is actually queued for extraction,
                // otherwise a rejected file still causes its bundle to be downloaded and the file
                // to be recorded as synced without ever having been written.
                compressedFileIndexSet.insert(remoteFile.compressedFileIndex); // Set<> doesn't allow duplicates
                changedRemoteFolderIndexSet.insert(remoteFile.folderIndex); // Set<> doesn't allow duplicates.
#if defined(_DEBUG)
                filesToDownload.push_back(&remoteFile);
#endif
            }
        }
        else
        {
            // no local file, so need to download it
            ArchiveFileDetail afd{};
            if (FAILED(JoinPathHelper(saveFolder, relFilePath, afd.fullPath)))
            {
                TRACE_WARNING("[GAME SAVE] CompareStep: JoinPathHelper failed for new download file '%s'", relFilePath.c_str());
                continue;
            }
            afd.uncompressedSize = remoteFile.fileSizeBytes;
            afd.timeLastModified = remoteFile.timeLastModified;
            afd.timeCreated = remoteFile.timeCreated;
            HRESULT addHr = compressedFiles[remoteFile.compressedFileIndex].archiveContext->AddFile(relFilePath, std::move(afd));
            if (FAILED(addHr))
            {
                // Rejected by the archive's path/size validation - don't schedule an extraction
                // the archive layer will never perform.
                TRACE_WARNING("[GAME SAVE] CompareStep: AddFile rejected new download file '%s' (hr=0x%08X)", relFilePath.c_str(), addHr);
                continue;
            }

            compressedFileIndexSet.insert(remoteFile.compressedFileIndex); // Set<> doesn't allow duplicates
            changedRemoteFolderIndexSet.insert(remoteFile.folderIndex); // Set<> doesn't allow duplicates.
#if defined(_DEBUG)
            filesToDownload.push_back(&remoteFile);
#endif
        }
    }

    Vector<size_t> compressedFilesToDownload;
    for (auto it = compressedFileIndexSet.begin(); it != compressedFileIndexSet.end(); it++)
    {
        compressedFilesToDownload.push_back(*it);
    }
    TRACE_INFORMATION("[GAME SAVE] CompareStep: CompressedFilesToDownload: %zd", compressedFilesToDownload.size());
    remoteFileFolderSet->SetCompressedFilesToDownload(std::move(compressedFilesToDownload));

    Vector<size_t> changedRemoteFolderIndexes;
    for (auto it = changedRemoteFolderIndexSet.begin(); it != changedRemoteFolderIndexSet.end(); it++)
    {
        changedRemoteFolderIndexes.push_back(*it);
    }
    TRACE_INFORMATION("[GAME SAVE] CompareStep: ChangedRemoteFolderIndexSet %zd", changedRemoteFolderIndexes.size());
    remoteFileFolderSet->SetChangedRemoteFolderIndexSet(std::move(changedRemoteFolderIndexes));

#if defined(_DEBUG)
    remoteFileFolderSet->SetFilesToDownload(std::move(filesToDownload));  // used only for debug stats
#endif
}


String GetTopLevelFolder(const String& relFolderPath)
{
    if (relFolderPath.empty())
    {
        return relFolderPath;
    }

    // Find the first path separator, skipping a leading one if present
    size_t startPos = (relFolderPath[0] == FilePAL::GetPathSeparatorChar()) ? 1 : 0;
    const char* searchStart = relFolderPath.c_str() + startPos;
    const char* firstSep = strchr(searchStart, FilePAL::GetPathSeparatorChar());

    if (firstSep)
    {
        // Found a separator, return everything up to it (including leading sep if present)
        size_t firstSepIndex = firstSep - relFolderPath.c_str();
        return relFolderPath.substr(0, firstSepIndex);
    }

    return relFolderPath;
}

Set<String> CompareStep::CollectConflictingUnitKeys(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet)
{
    Set<String> conflictingUnits;
    Set<String> topLevelFoldersNeedingDownload;

    // For each remote folder that has changes, get the top level folder and mark it as needs download
    const PlayFab::Vector<size_t>& changedRemoteFolderIndexes = remoteFileFolderSet->GetChangedRemoteFolderIndexSet();
    const PlayFab::Vector<PlayFab::GameSave::FolderDetail>& remoteFolders = remoteFileFolderSet->GetFolders();
    for (size_t changedRemoteFolderIndex : changedRemoteFolderIndexes)
    {
        const FolderDetail& folder = remoteFolders[changedRemoteFolderIndex];
        String topLevelFolder = GetTopLevelFolder(folder.relFolderPath);

        topLevelFoldersNeedingDownload.insert(topLevelFolder.c_str());
    }

    // A synced file the cloud deleted that this device changed since the last sync is a conflict
    // (client-dev-spec.md §Delete Replication): applying the delete would silently drop the player's
    // edit. An unchanged file follows the cloud delete; a file deleted on both sides converged.
    for (const FileDetail* remoteDeletedFile : localFileFolderSet->GetFilesToDeleteUponDownload())
    {
        if (remoteDeletedFile == nullptr)
        {
            continue;
        }
        bool localFileChanged = false;
        bool localFileDeleted = false;
        HasLocalFileChanged(*remoteDeletedFile, localFileChanged, localFileDeleted);
        if (localFileChanged)
        {
            TRACE_INFORMATION("[GAME SAVE] CompareStep: '%s' was deleted in the cloud and changed locally - conflict", remoteDeletedFile->fileName.c_str());
            conflictingUnits.insert(GetTopLevelFolder(localFileFolderSet->GetFileFolder(remoteDeletedFile).relFolderPath));
        }
    }

    // For each local file that needs upload (or a delete uploaded), its top level folder is in conflict
    // when that folder also needs a download.
    auto addIfNeedsDownload = [&](const FileDetail* localFile)
    {
        String topLevelFolder = GetTopLevelFolder(localFileFolderSet->GetFileFolder(localFile).relFolderPath);
        if (topLevelFoldersNeedingDownload.find(topLevelFolder) != topLevelFoldersNeedingDownload.end())
        {
            conflictingUnits.insert(std::move(topLevelFolder));
        }
    };
    for (const FileDetail* localFile : localFileFolderSet->GetFilesToUpload())
    {
        addIfNeedsDownload(localFile);
    }
    for (const FileDetail* localFile : localFileFolderSet->GetFilesToDeleteUponUpload())
    {
        addIfNeedsDownload(localFile);
    }
    return conflictingUnits;
}

void CompareStep::ScanForConflicts(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet,
    _Out_ bool& conflictFound)
{
    conflictFound = !CollectConflictingUnitKeys(localFileFolderSet, remoteFileFolderSet).empty();
}

void CompareStep::MarkFilesToDeleteUponUpload(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet)
{
    const Vector<FileDetail>& localFiles = localFileFolderSet->GetFiles();
    Vector<const FileDetail*> filesToDeleteUponUpload;
    for (const FileDetail& localFile : localFiles)
    {
        // If the file has been synced before but is no longer on disk, it was deleted locally.
        // hasLastSync (not lastSyncFileSize) is the existence record: a synced zero-byte file has
        // a last-sync size of 0 and would otherwise never be deletable.
        if (localFile.hasLastSync && !localFile.existsLocally)
        {
            TRACE_INFORMATION("[GAME SAVE] CompareStep: FilesToDeleteUponUpload file:%s lastSyncFileSize:%llu existsLocally:%d", localFile.fileName.c_str(), localFile.lastSyncFileSize, localFile.existsLocally);
            filesToDeleteUponUpload.push_back(&localFile);
        }
    }

    TRACE_INFORMATION("[GAME SAVE] CompareStep: FilesToDeleteUponUpload: %zd", filesToDeleteUponUpload.size());
    localFileFolderSet->SetFilesToDeleteUponUpload(std::move(filesToDeleteUponUpload));
}

void CompareStep::MarkFilesToDeleteUponDownload(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet)
{
    // If we never parsed the cloud's extended manifest, this set is "unknown", not "empty". Every
    // previously-synced local file would look absent from the cloud and be queued for deletion,
    // wiping the player's save. Preserve instead - a later sync that can read the manifest will
    // reconcile properly.
    if (!remoteFileFolderSet->IsInitializedFromExtendedManifest())
    {
        TRACE_ERROR("[GAME SAVE] CompareStep: remote file set was never populated from an extended manifest; skipping FilesToDeleteUponDownload so local files are not treated as cloud deletions");
        localFileFolderSet->SetFilesToDeleteUponDownload({});
        return;
    }

    // If there's last sync data but no remote file, then remote file was deleted
    Vector<const FileDetail*> filesToDeleteUponDownload;
    const Vector<FileDetail>& localFiles = localFileFolderSet->GetFiles();
    for (const FileDetail& localFile : localFiles)
    {
        String relFilePath = localFileFolderSet->GetRelFilePath(&localFile);
        const FileDetail* remoteFile = remoteFileFolderSet->GetFileDetailFromRelFilePath(relFilePath);

        TRACE_INFORMATION("[GAME SAVE] CompareStep: FilesToDeleteUponDownload: file:%s hasLastSync:%d", localFile.fileName.c_str(), localFile.hasLastSync);
        if (localFile.hasLastSync &&
            remoteFile == nullptr)
        {
            filesToDeleteUponDownload.push_back(&localFile);
        }
    }

    TRACE_INFORMATION("[GAME SAVE] CompareStep: FilesToDeleteUponDownload: %zd", filesToDeleteUponDownload.size());
    localFileFolderSet->SetFilesToDeleteUponDownload(std::move(filesToDeleteUponDownload));
}

void CompareStep::MarkFoldersToCreateUponUpload(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet
)
{
    Vector<const FolderDetail*> foldersToCreateUponUpload;
    const Vector<FolderDetail>& localFolders = localFileFolderSet->GetFolders();
    for (const FolderDetail& localFolder : localFolders)
    {
        if (localFolder.relFolderPath.length() == 0)
        {
            continue; // skip the root folder
        }

        // Skip folders that don't exist locally - they were deleted and should not be
        // re-created on the remote side. Without this check, a folder that was deleted
        // locally (and already removed from cloud) would be perpetually re-marked for
        // creation because it remains in localstate.json but is absent from the remote
        // manifest, causing every subsequent upload to perform a full upload cycle.
        if (!localFolder.existsLocally)
        {
            continue;
        }

        const FolderDetail* remoteFolder = remoteFileFolderSet->GetFolderDetailFromRelFilePath(localFolder.relFolderPath);
        if (remoteFolder == nullptr)
        {
            foldersToCreateUponUpload.push_back(&localFolder);
        }
    }
    TRACE_INFORMATION("[GAME SAVE] CompareStep: FoldersToCreateUponUpload: %zd", foldersToCreateUponUpload.size());
    localFileFolderSet->SetFoldersToCreateUponUpload(std::move(foldersToCreateUponUpload));
}

void CompareStep::MarkFoldersToCreateUponDownload(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet
)
{
    Vector<const FolderDetail*> foldersToCreateUponDownload;
    const Vector<FolderDetail>& remoteFolders = remoteFileFolderSet->GetFolders();
    for (const FolderDetail& remoteFolder : remoteFolders)
    {
        if (remoteFolder.relFolderPath.length() == 0)
        {
            continue; // skip the root folder
        }

        const FolderDetail* localFolder = localFileFolderSet->GetFolderDetailFromRelFilePath(remoteFolder.relFolderPath);
        if (localFolder == nullptr)
        {
            foldersToCreateUponDownload.push_back(&remoteFolder);
        }
    }
    TRACE_INFORMATION("[GAME SAVE] CompareStep: FoldersToCreateUponDownload: %zd", foldersToCreateUponDownload.size());
    remoteFileFolderSet->SetFoldersToCreateUponDownload(std::move(foldersToCreateUponDownload));
}

void CompareStep::MarkFoldersToDeleteUponUpload(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet)
{
    Vector<const FolderDetail*> foldersToDeleteUponUpload;
    const Vector<FolderDetail>& remoteFolders = remoteFileFolderSet->GetFolders();
    for (const FolderDetail& remoteFolder : remoteFolders)
    {
        if (remoteFolder.relFolderPath.length() == 0)
        {
            continue; // skip the root folder
        }

        const FolderDetail* localFolder = localFileFolderSet->GetFolderDetailFromRelFilePath(remoteFolder.relFolderPath);
        if (localFolder == nullptr)
        {
            TRACE_INFORMATION("[GAME SAVE] CompareStep: MarkFoldersToDeleteUponUpload %s: added. no localFolder", remoteFolder.relFolderPath.c_str());
            foldersToDeleteUponUpload.push_back(&remoteFolder);
        }
        else
        {
            TRACE_INFORMATION("[GAME SAVE] CompareStep: MarkFoldersToDeleteUponUpload %s: existsLocally: %d", remoteFolder.relFolderPath.c_str(), localFolder->existsLocally);
            if (!localFolder->existsLocally)
            {
                foldersToDeleteUponUpload.push_back(&remoteFolder);
            }
        }

    }
    TRACE_INFORMATION("[GAME SAVE] CompareStep: FoldersToDeleteUponUpload: %zd", foldersToDeleteUponUpload.size());
    remoteFileFolderSet->SetFoldersToDeleteUponUpload(std::move(foldersToDeleteUponUpload));
}

void CompareStep::MarkFoldersToDeleteUponDownload(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet)
{
    // Same reasoning as MarkFilesToDeleteUponDownload: an unpopulated remote set is "unknown", not
    // "empty", and would otherwise queue every synced folder for recursive deletion.
    if (!remoteFileFolderSet->IsInitializedFromExtendedManifest())
    {
        TRACE_ERROR("[GAME SAVE] CompareStep: remote file set was never populated from an extended manifest; skipping FoldersToDeleteUponDownload so local folders are not treated as cloud deletions");
        localFileFolderSet->SetFoldersToDeleteUponDownload({});
        return;
    }

    Vector<const FolderDetail*> foldersToDeleteUponDownload;
    const Vector<FolderDetail>& localFolders = localFileFolderSet->GetFolders();
    for (const FolderDetail& localFolder : localFolders)
    {
        if (localFolder.relFolderPath.length() == 0)
        {
            continue; // skip the root folder
        }

        const FolderDetail* remoteFolder = remoteFileFolderSet->GetFolderDetailFromRelFilePath(localFolder.relFolderPath);
        
        TRACE_INFORMATION("[GAME SAVE] CompareStep: MarkFoldersToDeleteUponDownload %s: remoteFolder:%d localFolder.hasLastSync:%d", localFolder.relFolderPath.c_str(), remoteFolder != nullptr, localFolder.hasLastSync);
        if (remoteFolder == nullptr && localFolder.hasLastSync)
        {
            foldersToDeleteUponDownload.push_back(&localFolder);
        }
    }
    TRACE_INFORMATION("[GAME SAVE] CompareStep: FoldersToDeleteUponDownload: %zd", foldersToDeleteUponDownload.size());
    localFileFolderSet->SetFoldersToDeleteUponDownload(std::move(foldersToDeleteUponDownload));
}

void CompareStep::HasLocalFileChanged(
    _In_ const FileDetail& localFile, 
    _Out_ bool& localFileChanged, 
    _Out_ bool& localFileDeleted)
{
    // existsLocally is set by the disk scan in MergeLocalFiles. File size can't be used as an
    // existence proxy: a legitimate zero-byte save file would be reported as deleted here and
    // then dropped from the cloud manifest on the next upload (and deleted on other devices).
    if (localFile.existsLocally)
    {
        localFileChanged =
            (localFile.lastSyncFileSize != localFile.fileSizeBytes ||
             localFile.lastSyncTimeLastModified != localFile.timeLastModified);
        localFileDeleted = false;
    }
    else
    {
        localFileDeleted = true;
        localFileChanged = false; // don't count deleted files as changed
    }
}

bool CompareStep::HasRemoteFileChanged(_In_ const FileDetail& localFile, _In_ const FileDetail& remoteFile)
{
    // On platforms where SetFileLastModifiedTime is a no-op, lastSyncTimeLastModified contains
    // the local disk timestamp which won't match the cloud's timestamp. Use the dedicated
    // lastSyncRemoteTimeLastModified field which stores the cloud timestamp at last sync.
    time_t lastSyncTimestampForRemoteComparison = localFile.lastSyncTimeLastModified;

    SharedPtr<GameSaveGlobalState> globalState;
    if (SUCCEEDED(GameSaveGlobalState::Get(globalState)))
    {
        if (globalState->ApiProvider().PlatformNeedsRemoteTimestamp() && localFile.lastSyncRemoteTimeLastModified != 0)
        {
            lastSyncTimestampForRemoteComparison = localFile.lastSyncRemoteTimeLastModified;
        }
    }

    bool hasRemoteFileChanged =
        (localFile.lastSyncFileSize != remoteFile.fileSizeBytes ||
         lastSyncTimestampForRemoteComparison != remoteFile.timeLastModified);

    return hasRemoteFileChanged;
}

void CompareStep::MarkCompressedFilesToKeep(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet)
{
    const Vector<FileDetail>& remoteFiles = remoteFileFolderSet->GetFiles();

    Set<size_t> compressedFileIndexSet;
    for (const FileDetail& remoteFile : remoteFiles)
    {
        String relFilePath = remoteFileFolderSet->GetRelFilePath(&remoteFile);

        bool hasExtractedFileChangedLocally = false;
        bool hasExtractedFileDeletedLocally = false;
        const FileDetail* localFile = localFileFolderSet->GetFileDetailFromRelFilePath(relFilePath);
        if (localFile)
        {
            HasLocalFileChanged(*localFile, hasExtractedFileChangedLocally, hasExtractedFileDeletedLocally);
        }

        if (hasExtractedFileChangedLocally || hasExtractedFileDeletedLocally || localFile == nullptr)
        {
            // Mark old extracted files that will be uploaded as "skip" since they won't be needed in upcoming manifest
            remoteFile.skipFile = true;
            localFileFolderSet->AddSkippedFile(remoteFile); // recording just for logging purposes
        }
        else
        {
            // Otherwise, mark that we need found an extracted file needed in this compressed file so we need to keep it.
            // skipFile is recomputed on every pass (a retry re-runs this on the same remote set), so
            // a skip left over from an earlier attempt can't drop a file this attempt keeps.
            remoteFile.skipFile = false;
            compressedFileIndexSet.insert(remoteFile.compressedFileIndex); // Set<> doesn't allow duplicates
        }
    }

    Vector<size_t> compressedFilesToKeep;
    for (auto it = compressedFileIndexSet.begin(); it != compressedFileIndexSet.end(); it++)
    {
        compressedFilesToKeep.push_back(*it);
    }
    TRACE_INFORMATION("[GAME SAVE] CompareStep: CompressedFilesToKeep: %zd", compressedFilesToKeep.size());
    remoteFileFolderSet->SetCompressedFilesToKeep(std::move(compressedFilesToKeep));
}

} // namespace GameSave
} // namespace PlayFab

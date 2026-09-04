// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "Manifest.h"
#include "ApiHelpers.h"

using namespace PlayFab::GameSaveWrapper;

namespace PlayFab
{
namespace GameSave
{

void FileFolderSet::Clear()
{
    m_files.clear();
    m_filePathMap.clear();

    m_folders.clear();
    m_folderRelPathMap.clear();
    m_folderFolderIdMap.clear();

    m_compressedFiles.clear();
    m_compressedFilesMap.clear();

    m_compressedFilesToDownload.clear();
    m_compressedFilesToUpload.clear();
    m_changedRemoteFolderIndexes.clear();
    m_compressedFilesToKeep.clear();
    m_skippedFiles.clear();
    m_filesToUpload.clear();
    m_filesToDownload.clear();
    m_filesToDeleteUponUpload.clear();
    m_filesToDeleteUponDownload.clear();
    m_foldersToCreateUponUpload.clear();
    m_foldersToCreateUponDownload.clear();
    m_foldersToDeleteUponUpload.clear();
    m_foldersToDeleteUponDownload.clear();
    m_initializedFromExtendedManifest = false;
}

size_t FileFolderSet::AddCompressedFile(CompressedFile&& compressedFile)
{
    size_t newIndex = m_compressedFiles.size();
    compressedFile.compressedFileIndex = newIndex;
    m_compressedFilesMap[compressedFile.fileId] = newIndex;

    m_compressedFiles.push_back(std::move(compressedFile));
    return newIndex;
}

size_t FileFolderSet::AddFileDetail(FileDetail&& fileDetail)
{
    size_t newIndex = m_files.size();
    if (fileDetail.folderIndex >= m_folders.size())
    {
        TRACE_ERROR("[GAME SAVE] FileFolderSet::AddFileDetail: folderIndex %zu out of bounds (size: %zu) for file '%s'", 
            fileDetail.folderIndex, m_folders.size(), fileDetail.fileName.c_str());
        return SIZE_MAX;
    }
    FolderDetail& folderDetail = m_folders[fileDetail.folderIndex];
    String relFilePath;
    HRESULT hr = JoinPathHelper(folderDetail.relFolderPath, fileDetail.fileName, relFilePath);
    if (FAILED(hr))
    {
        TRACE_ERROR("[GAME SAVE] FileFolderSet::AddFileDetail: JoinPathHelper failed hr=0x%08X", hr);
        return SIZE_MAX;
    }
    m_filePathMap[relFilePath] = newIndex;

    m_files.push_back(std::move(fileDetail));
    return newIndex;
}

size_t FileFolderSet::AddFolderDetail(FolderDetail&& folderDetail)
{
    size_t newIndex = m_folders.size();
    TRACE_VERBOSE("[GAME SAVE] FileFolderSet::AddFolderDetail: Adding folder at index %zu, folderId='%s', relPath='%s'",
        newIndex, folderDetail.folderId.c_str(), folderDetail.relFolderPath.c_str());
    m_folderFolderIdMap[folderDetail.folderId] = newIndex;
    m_folderRelPathMap[folderDetail.relFolderPath] = newIndex;

    m_folders.push_back(std::move(folderDetail));
    return newIndex;
}

void FileFolderSet::UpdateFilesWithUploadData()
{
    const Vector<const FileDetail*>& filesToUpload = GetFilesToUpload();
    for (const FileDetail* fileToUpload : filesToUpload)
    {
        fileToUpload->lastSyncFileSize = fileToUpload->fileSizeBytes;
        fileToUpload->hasLastSync = true;
        fileToUpload->lastSyncTimeLastModified = fileToUpload->timeLastModified;
        // After upload, the cloud will have this device's disk timestamp
        fileToUpload->lastSyncRemoteTimeLastModified = fileToUpload->timeLastModified;
    }

    const Vector<const FileDetail*>& filesToDeleteUponUpload = GetFilesToDeleteUponUpload();
    for (const FileDetail* fileToDeleteUponUpload : filesToDeleteUponUpload)
    {
        fileToDeleteUponUpload->lastSyncFileSize = 0;
        fileToDeleteUponUpload->hasLastSync = false;
        fileToDeleteUponUpload->lastSyncTimeLastModified = 0;
        fileToDeleteUponUpload->lastSyncRemoteTimeLastModified = 0;
    }
}

void FileFolderSet::UpdateFilesWithDownloadData(const FileDetail& remoteFile, const String& relFilePath, const String& saveFolder)
{
    const FileDetail* localFile = GetFileDetailFromRelFilePath(relFilePath);
    if (localFile)
    {
        // Only record a sync for a file the download actually produced. This set was just
        // re-scanned from disk, so fileSizeBytes is the real on-disk length; if it doesn't match
        // what the remote manifest advertised, this entry was never extracted - it can be
        // rejected by ArchiveContext::AddFile (path/size validation) while a sibling in the same
        // bundle still schedules the download, or simply omitted from the archive. Recording it
        // anyway would set lastSyncFileSize/lastSyncRemoteTimeLastModified to the remote values,
        // making HasRemoteFileChanged() report "unchanged" forever and stranding the stale local
        // save file permanently. Skipping the record leaves it eligible for re-download next sync.
        if (localFile->fileSizeBytes != remoteFile.fileSizeBytes)
        {
            TRACE_WARNING("[GAME SAVE] UpdateFilesWithDownloadData: '%s' is %llu bytes on disk but the manifest advertised %llu - not recording as synced",
                relFilePath.c_str(),
                static_cast<unsigned long long>(localFile->fileSizeBytes),
                static_cast<unsigned long long>(remoteFile.fileSizeBytes));
            return;
        }

        // The file has just been written to disk by the download, so it exists locally even if
        // the pre-download scan didn't see it.
        localFile->existsLocally = true;
        localFile->lastSyncFileSize = remoteFile.fileSizeBytes;
        localFile->hasLastSync = true;
        // Store the cloud's timestamp for remote change detection
        localFile->lastSyncRemoteTimeLastModified = remoteFile.timeLastModified;

        // Get actual file times from disk in case they differ from what was expected.  This is required on some 
        // platform(s) that do not support setting the modified time on files differently than the actual modified time.
        String fullFilePath;
        if (SUCCEEDED(JoinPathHelper(saveFolder, relFilePath, fullFilePath)))
        {
            time_t actualCreated = 0;
            time_t actualModified = 0;
            if (SUCCEEDED(FilePAL::GetFileTimes(fullFilePath, actualCreated, actualModified)))
            {
                localFile->lastSyncTimeLastModified = actualModified; // Store ACTUAL modified time
            }
            else
            {
                localFile->lastSyncTimeLastModified = remoteFile.timeLastModified; // Fallback
            }
        }
        else
        {
            // Handle error in path joining, fallback to remote file's modified time
            localFile->lastSyncTimeLastModified = remoteFile.timeLastModified;
        }
    }
}

void FileFolderSet::SetCompressedFilesToDownload(Vector<size_t>&& compressedFileIndices)
{
    m_compressedFilesToDownload = std::move(compressedFileIndices);
}

void FileFolderSet::SetCompressedFilesToUpload(Vector<ExtendedManifestCompressedFileDetail>&& compressedFiles)
{
    m_compressedFilesToUpload = std::move(compressedFiles);
}

void FileFolderSet::SetChangedRemoteFolderIndexSet(Vector<size_t>&& changedRemoteFolderIndexes)
{
    m_changedRemoteFolderIndexes = std::move(changedRemoteFolderIndexes);
}

const Vector<size_t>& FileFolderSet::GetChangedRemoteFolderIndexSet() const
{
    return m_changedRemoteFolderIndexes;
}

const PlayFab::Vector<size_t>& FileFolderSet::GetCompressedFilesToDownload() const
{
    return m_compressedFilesToDownload;
}

const Vector<ExtendedManifestCompressedFileDetail>& FileFolderSet::GetCompressedFilesToUpload() const
{
    return m_compressedFilesToUpload;
}

void FileFolderSet::SetCompressedFilesToKeep(Vector<size_t>&& compressedFileIndices)
{
    m_compressedFilesToKeep = std::move(compressedFileIndices);
}

const PlayFab::Vector<size_t>& FileFolderSet::GetCompressedFilesToKeep() const
{
    return m_compressedFilesToKeep;
}

#if defined(_DEBUG)
void FileFolderSet::SetFilesToDownload(Vector<const FileDetail*>&& filesToDownload)
{
    m_filesToDownload = std::move(filesToDownload);
}

const PlayFab::Vector<const FileDetail*>& FileFolderSet::GetFilesToDownload() const
{
    return m_filesToDownload;
}
#endif

void FileFolderSet::SetFilesToUpload(Vector<const FileDetail*>&& filesToUpload)
{
    m_filesToUpload = std::move(filesToUpload);
}

const PlayFab::Vector<const FileDetail*>& FileFolderSet::GetFilesToUpload() const
{
    return m_filesToUpload;
}

const PlayFab::Vector<PlayFab::GameSave::FileDetail>& FileFolderSet::GetSkippedFiles() const
{
    return m_skippedFiles;
}

void FileFolderSet::AddSkippedFile(FileDetail fileDetail)
{
    m_skippedFiles.push_back(std::move(fileDetail));
}

void FileFolderSet::SetFilesToDeleteUponUpload(Vector<const FileDetail*>&& filesToDeleteUponUpload)
{
    m_filesToDeleteUponUpload = std::move(filesToDeleteUponUpload);
}

void FileFolderSet::SetFoldersToCreateUponUpload(Vector<const FolderDetail*>&& foldersToCreateUponUpload)
{
    m_foldersToCreateUponUpload = std::move(foldersToCreateUponUpload);
}

const Vector<const FolderDetail*>& FileFolderSet::GetFoldersToCreateUponUpload() const
{
    return m_foldersToCreateUponUpload;
}

void FileFolderSet::SetFoldersToCreateUponDownload(Vector<const FolderDetail*>&& foldersToCreateUponDownload)
{
    m_foldersToCreateUponDownload = std::move(foldersToCreateUponDownload);
}

const Vector<const FolderDetail*>& FileFolderSet::GetFoldersToCreateUponDownload() const
{
    return m_foldersToCreateUponDownload;
}

void FileFolderSet::SetFoldersToDeleteUponUpload(Vector<const FolderDetail*>&& foldersToDeleteUponUpload)
{
    m_foldersToDeleteUponUpload = std::move(foldersToDeleteUponUpload);
}

const Vector<const FolderDetail*>& FileFolderSet::GetFoldersToDeleteUponUpload() const
{
    return m_foldersToDeleteUponUpload;
}

void FileFolderSet::SetFoldersToDeleteUponDownload(Vector<const FolderDetail*>&& foldersToDeleteUponDownload)
{
    m_foldersToDeleteUponDownload = std::move(foldersToDeleteUponDownload);
}

const Vector<const FolderDetail*>& FileFolderSet::GetFoldersToDeleteUponDownload() const
{
    return m_foldersToDeleteUponDownload;
}

const PlayFab::Vector<const FileDetail*>& FileFolderSet::GetFilesToDeleteUponUpload() const
{
    return m_filesToDeleteUponUpload;
}

void FileFolderSet::SetFilesToDeleteUponDownload(Vector<const FileDetail*>&& filesToDeleteUponDownload)
{
    m_filesToDeleteUponDownload = std::move(filesToDeleteUponDownload);
}

const PlayFab::Vector<const FileDetail*>& FileFolderSet::GetFilesToDeleteUponDownload() const
{
    return m_filesToDeleteUponDownload;
}

const PlayFab::Vector<PlayFab::GameSave::FileDetail>& FileFolderSet::GetFiles() const
{
    return m_files;
}

const PlayFab::Vector<PlayFab::GameSave::FolderDetail>& FileFolderSet::GetFolders() const
{
    return m_folders;
}

const PlayFab::Vector<PlayFab::GameSave::CompressedFile>& FileFolderSet::GetCompressedFiles() const
{
    return m_compressedFiles;
}

const FileDetail* FileFolderSet::GetFileDetailFromRelFilePath(const String& relFilePath) const
{
    auto it = m_filePathMap.find(relFilePath);
    size_t index = 0;
    if (it == m_filePathMap.end())
    {
        return nullptr;
    }
    else
    {
        index = it->second;
        return &m_files[index];
    }
}

const FolderDetail* FileFolderSet::GetFolderDetailFromRelFilePath(const String& relFolderPath) const
{
    auto it = m_folderRelPathMap.find(relFolderPath);
    size_t index = 0;
    if (it == m_folderRelPathMap.end())
    {
        return nullptr;
    }
    else
    {
        index = it->second;
        return &m_folders[index];
    }
}

size_t FileFolderSet::GetFolderDetailIndexFromFolderId(const String& folderId) const
{
    auto it = m_folderFolderIdMap.find(folderId);
    if (it == m_folderFolderIdMap.end())
    {
        // Log detailed diagnostic info - this can happen with stale/corrupted manifests
        // where files reference folders that no longer exist. The caller should handle
        // SIZE_MAX return by skipping the file.
        TRACE_ERROR("[GAME SAVE] FileFolderSet: FolderId '%s' not found in folder map (map size: %zu, folders size: %zu)",
            folderId.c_str(),
            m_folderFolderIdMap.size(),
            m_folders.size());
        
        // Log all folder IDs currently in the map for debugging
        TRACE_ERROR("[GAME SAVE] FileFolderSet: Available folder IDs in map:");
        size_t logCount = 0;
        for (const auto& entry : m_folderFolderIdMap)
        {
            TRACE_ERROR("[GAME SAVE]   [%zu] folderId='%s' -> index=%zu", logCount, entry.first.c_str(), entry.second);
            logCount++;
            if (logCount >= 20) // Limit logging to avoid spam
            {
                TRACE_ERROR("[GAME SAVE]   ... and %zu more", m_folderFolderIdMap.size() - logCount);
                break;
            }
        }
        
        // Also log the folders vector for comparison
        TRACE_ERROR("[GAME SAVE] FileFolderSet: Folders in vector:");
        for (size_t i = 0; i < m_folders.size() && i < 20; i++)
        {
            TRACE_ERROR("[GAME SAVE]   [%zu] folderId='%s' relPath='%s'", 
                i, m_folders[i].folderId.c_str(), m_folders[i].relFolderPath.c_str());
        }
        if (m_folders.size() > 20)
        {
            TRACE_ERROR("[GAME SAVE]   ... and %zu more", m_folders.size() - 20);
        }
        
        return SIZE_MAX;
    }

    return it->second;
}

const FolderDetail* FileFolderSet::GetFolderDetailFromFolderId(const String& folderId) const
{
    size_t index = GetFolderDetailIndexFromFolderId(folderId);
    if (index == SIZE_MAX || index >= m_folders.size())
    {
        TRACE_ERROR("[GAME SAVE] FileFolderSet::GetFolderDetailFromFolderId: Invalid folderId '%s'", folderId.c_str());
        return nullptr;
    }
    return &m_folders[index];
}

const CompressedFile* FileFolderSet::GetCompressedFileFromFileId(const String& fileId) const
{
    auto it = m_compressedFilesMap.find(fileId);
    if (it == m_compressedFilesMap.end())
    {
        TRACE_ERROR("[GAME SAVE] FileFolderSet::GetCompressedFileFromFileId: fileId '%s' not found in map (map size: %zu)", 
            fileId.c_str(), m_compressedFilesMap.size());
        return nullptr;
    }

    return &m_compressedFiles[it->second];
}

const FolderDetail& FileFolderSet::GetFileFolder(const FileDetail* file) const
{    
    if (file->folderIndex >= m_folders.size())
    {
        TRACE_ERROR("[GAME SAVE] FileFolderSet::GetFileFolder: folderIndex %zu out of bounds (size: %zu)", file->folderIndex, m_folders.size());
        static const FolderDetail emptyFolder{};
        return emptyFolder;
    }
    return m_folders[file->folderIndex];
}

PlayFab::String FileFolderSet::GetRelFilePath(const FileDetail* file) const
{
    String result;
    if (file->folderIndex >= m_folders.size())
    {
        TRACE_ERROR("[GAME SAVE] FileFolderSet::GetRelFilePath: folderIndex %zu out of bounds (size: %zu)", file->folderIndex, m_folders.size());
        return file->fileName;
    }
    HRESULT hr = JoinPathHelper(m_folders[file->folderIndex].relFolderPath, file->fileName, result);
    if (FAILED(hr))
    {
        TRACE_ERROR("[GAME SAVE] FileFolderSet::GetRelFilePath: JoinPathHelper failed hr=0x%08X for file '%s'", hr, file->fileName.c_str());
    }
    return result;
}

const FileDetail* FileFolderSet::GetThumbnail() const
{
    for (const FileDetail& file : m_files)
    {
        if (file.isThumbnail)
        {
            return &file;
        }
    }

    return nullptr;
}

const CompressedFile* FileFolderSet::GetThumbnailFromCompressedList(const String& version) const
{
    String thumbnailName = FormatString("pfthumbnail_%s.png", version.c_str());

    for (const CompressedFile& file : m_compressedFiles)
    {
        if (file.fileName == thumbnailName)
        {
            return &file;
        }
    }

    return nullptr;
}

uint64_t FileFolderSet::GetTotalUncompressedSize() const
{
    uint64_t totalSize = 0;
    for (size_t iFile = 0; iFile < m_files.size(); iFile++)
    {
        const FileDetail& file = m_files[iFile];
        if (!file.skipFile)
        {
            totalSize += file.fileSizeBytes;
        }
    }
    return totalSize;
}

} // namespace GameSave
} // namespace PlayFab
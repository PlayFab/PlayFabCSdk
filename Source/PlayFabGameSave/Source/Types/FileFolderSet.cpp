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
    m_changedRemoteFolderIndexes.clear();
    m_compressedFilesToKeep.clear();
    m_filesToUpload.clear();
    m_filesToDownload.clear();
    m_filesToDeleteUponUpload.clear();
    m_filesToDeleteUponDownload.clear();
    m_foldersToCreateUponUpload.clear();
    m_foldersToDeleteUponDownload.clear();
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
    FolderDetail& folderDetail = m_folders[fileDetail.folderIndex];
    String relFilePath;
    RETURN_IF_FAILED(JoinPathHelper(folderDetail.relFolderPath, fileDetail.fileName, relFilePath));
    m_filePathMap[relFilePath] = newIndex;

    m_files.push_back(std::move(fileDetail));
    return newIndex;
}

size_t FileFolderSet::AddFolderDetail(FolderDetail&& folderDetail)
{
    size_t newIndex = m_folders.size();
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
        fileToUpload->lastSyncTimeLastModified = fileToUpload->timeLastModified;
    }

    const Vector<const FileDetail*>& filesToDeleteUponUpload = GetFilesToDeleteUponUpload();
    for (const FileDetail* fileToDeleteUponUpload : filesToDeleteUponUpload)
    {
        fileToDeleteUponUpload->lastSyncFileSize = 0;
        fileToDeleteUponUpload->lastSyncTimeLastModified = 0;
    }
}

void FileFolderSet::UpdateFilesWithDownloadData(const FileDetail& remoteFile, const String& relFilePath)
{
    const FileDetail* localFile = GetFileDetailFromRelFilePath(relFilePath);
    if (localFile)
    {
        localFile->lastSyncFileSize = remoteFile.fileSizeBytes;
        localFile->lastSyncTimeLastModified = remoteFile.timeLastModified;
    }
}

void FileFolderSet::SetCompressedFilesToDownload(Vector<size_t>&& compressedFileIndies)
{
    m_compressedFilesToDownload = std::move(compressedFileIndies);
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

const Vector<ExtendedManifestCompressedFileDetail> FileFolderSet::GetCompressedFilesToUpload() const
{
    return m_compressedFilesToUpload;
}

void FileFolderSet::SetCompressedFilesToKeep(Vector<size_t>&& compressedFileIndies)
{
    m_compressedFilesToKeep = std::move(compressedFileIndies);
}

const PlayFab::Vector<size_t>& FileFolderSet::GetCompressedFilesToKeep() const
{
    return m_compressedFilesToKeep;
}

#if _DEBUG
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

void FileFolderSet::SetFoldersToDeleteUponUpload(Vector<const FolderDetail*>&& foldersToCreateUponUpload)
{
    m_foldersToCreateUponUpload = std::move(foldersToCreateUponUpload);
}

const Vector<const FolderDetail*>& FileFolderSet::GetFoldersToDeleteUponUpload() const
{
    return m_foldersToCreateUponUpload;
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
    size_t index = 0;
    if (it == m_folderFolderIdMap.end())
    {
        assert(false);
    }
    else
    {
        index = it->second;
    }

    return index;
}

const FolderDetail& FileFolderSet::GetFolderDetailFromFolderId(const String& folderId) const
{
    size_t index = GetFolderDetailIndexFromFolderId(folderId);
    return m_folders[index];
}

const CompressedFile& FileFolderSet::GetCompressedFileFromFileId(const String& fileId) const
{
    auto it = m_compressedFilesMap.find(fileId);
    size_t index = 0;
    if (it == m_compressedFilesMap.end())
    {
        assert(false);
    }
    else
    {
        index = it->second;
    }

    return m_compressedFiles[index];
}

const FolderDetail& FileFolderSet::GetFileFolder(const FileDetail* file) const
{    
    return m_folders[file->folderIndex];
}

PlayFab::String FileFolderSet::GetRelFilePath(const FileDetail* file) const
{
    String result;
    JoinPathHelper(m_folders[file->folderIndex].relFolderPath, file->fileName, result);
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
    for (int iFile = 0; iFile < m_files.size(); iFile++)
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
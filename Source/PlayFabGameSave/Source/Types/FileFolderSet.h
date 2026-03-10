// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include "FileFolderSetTypes.h"
#include "ExtendedManifest.h"

namespace PlayFab
{
namespace GameSave
{

class FileFolderSet
{
public:
    // Init functions setup m_files, m_folders, m_compressedFiles and the related maps.
    HRESULT InitWithLocalFilesAndFolders(const String& saveFolder, _Out_opt_ String* shortSaveDescription, _Out_opt_ bool* descriptionDirty);
    HRESULT InitWithExtendedManifest(const Vector<char>& manifestBytes, const DownloadDetailsWrapVector& remoteFileDetails, const String& saveFolder);
    void UpdateFilesWithUploadData();
    void UpdateFilesWithDownloadData(const FileDetail& remoteFile, const String& relFilePath, const String& saveFolder);

    // Indices into m_compressedFiles
    void SetCompressedFilesToDownload(Vector<size_t>&& compressedFileIndies);
    const Vector<size_t>& GetCompressedFilesToDownload() const;

    void SetCompressedFilesToUpload(Vector<ExtendedManifestCompressedFileDetail>&& compressedFiles);
    const Vector<ExtendedManifestCompressedFileDetail> GetCompressedFilesToUpload() const;

    void SetChangedRemoteFolderIndexSet(Vector<size_t>&& changedRemoteFolderIndexes);
    const Vector<size_t>& GetChangedRemoteFolderIndexSet() const;

    // Indices into m_compressedFiles
    void SetCompressedFilesToKeep(Vector<size_t>&& compressedFileIndies);
    const Vector<size_t>& GetCompressedFilesToKeep() const;

    void AddSkippedFile(FileDetail fileDetail);
    const Vector<FileDetail>& GetSkippedFiles() const;

    // raw pointers into m_files
#if _DEBUG
    void SetFilesToDownload(Vector<const FileDetail*>&& filesToDownload);
    const Vector<const FileDetail*>& GetFilesToDownload() const;
#endif

    void SetFilesToUpload(Vector<const FileDetail*>&& filesToUpload);
    const Vector<const FileDetail*>& GetFilesToUpload() const;

    void SetFilesToDeleteUponUpload(Vector<const FileDetail*>&& filesToDeleteUponUpload);
    const Vector<const FileDetail*>& GetFilesToDeleteUponUpload() const;

    void SetFilesToDeleteUponDownload(Vector<const FileDetail*>&& filesToDeleteUponDownload);
    const Vector<const FileDetail*>& GetFilesToDeleteUponDownload() const;

    void SetFoldersToCreateUponUpload(Vector<const FolderDetail*>&& foldersToCreateUponUpload);
    const Vector<const FolderDetail*>& GetFoldersToCreateUponUpload() const;

    void SetFoldersToCreateUponDownload(Vector<const FolderDetail*>&& foldersToCreateUponDownload);
    const Vector<const FolderDetail*>& GetFoldersToCreateUponDownload() const;

    void SetFoldersToDeleteUponUpload(Vector<const FolderDetail*>&& foldersToDeleteUponUpload);
    const Vector<const FolderDetail*>& GetFoldersToDeleteUponUpload() const;

    void SetFoldersToDeleteUponDownload(Vector<const FolderDetail*>&& foldersToDeleteUponDownload);
    const Vector<const FolderDetail*>& GetFoldersToDeleteUponDownload() const;

    // The APIs below return const data.
    // Also the data does not change after calling one of the Init* functions above, so its able to return ptrs and indexes for fast lookups.
    // The index and raw pointers should not be saved by caller.
    const Vector<FileDetail>& GetFiles() const;
    const Vector<FolderDetail>& GetFolders() const;
    const Vector<CompressedFile>& GetCompressedFiles() const;

    // Helper functions
    const FileDetail* GetFileDetailFromRelFilePath(const String& relFilePath) const;
    const FolderDetail* GetFolderDetailFromRelFilePath(const String& relFolderPath) const;
    const FolderDetail& GetFolderDetailFromFolderId(const String& folderId) const;
    size_t GetFolderDetailIndexFromFolderId(const String& folderId) const;
    const CompressedFile& GetCompressedFileFromFileId(const String& fileId) const;
    const FolderDetail& GetFileFolder(const FileDetail* file) const;
    String GetRelFilePath(const FileDetail* file) const;
    const FileDetail* GetThumbnail() const;
    const CompressedFile* GetThumbnailFromCompressedList(const String& version) const;
    uint64_t GetTotalUncompressedSize() const;

    HRESULT ExtendedManifestParseFolderJson(const JsonValue& folderJson, const String& curPath, bool isRoot, const String& saveFolder);

    void Clear(); // Clears all data, returning to initial empty state

private:
    // Internal private functions used by Init* functions
    HRESULT MergeLocalFolders(const String& rootPath, const String& folderName, const String& fullFolderPath);
    HRESULT MergeLocalFiles(const String& rootPath, const String& fullFolderPath, size_t folderIndex);
    size_t AddFileDetail(FileDetail&& fileDetail); // returns index of FileDetail in m_files
    size_t AddFolderDetail(FolderDetail&& folderDetail); // returns index of FolderDetail in m_folders
    size_t AddCompressedFile(CompressedFile&& compressedFile); // returns index of CompressedFile in m_compressedFiles

private:
    Vector<FileDetail> m_files;
    UnorderedMap<String, size_t> m_filePathMap;  // relFilePath -> index into m_files
    Vector<FileDetail> m_skippedFiles; // files marked with skipFile during upload

    Vector<FolderDetail> m_folders;
    UnorderedMap<String, size_t> m_folderRelPathMap;  // relFolderPath -> index into m_folders
    UnorderedMap<String, size_t> m_folderFolderIdMap;  // folderId -> index into m_folders

    Vector<CompressedFile> m_compressedFiles;
    UnorderedMap<String, size_t> m_compressedFilesMap;  // fileId -> index into m_compressedFiles

    Vector<size_t> m_compressedFilesToDownload; // index into m_compressedFiles
    Vector<ExtendedManifestCompressedFileDetail> m_compressedFilesToUpload;
    Vector<size_t> m_changedRemoteFolderIndexes; // index into m_folders
    Vector<size_t> m_compressedFilesToKeep; // index into m_compressedFiles
    Vector<const FileDetail*> m_filesToUpload; // raw pointers into m_files
    Vector<const FileDetail*> m_filesToDownload; // raw pointers into m_files
    Vector<const FileDetail*> m_filesToDeleteUponUpload; // raw pointers into m_files
    Vector<const FileDetail*> m_filesToDeleteUponDownload; // raw pointers into m_files
    Vector<const FolderDetail*> m_foldersToCreateUponUpload; // raw pointers into m_folders
    Vector<const FolderDetail*> m_foldersToCreateUponDownload; // raw pointers into m_folders
    Vector<const FolderDetail*> m_foldersToDeleteUponDownload; // raw pointers into m_folders
};

} // namespace GameSave
} // namespace PlayFab
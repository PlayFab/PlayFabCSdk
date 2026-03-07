// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

#include "Compression.h"

namespace PlayFab
{
namespace GameSave
{

static const char* THUMBNAIL_FILE_NAME = "pfthumbnail.png";

struct FileDetail
{
    String fileId;
    size_t folderIndex{ 0 }; // index into m_folders
    String fileName;
    mutable uint64_t fileSizeBytes{ 0 }; // changeable by const types
    mutable time_t timeLastModified{ 0 }; // changeable by const types
    mutable time_t timeCreated{ 0 }; // changeable by const types

    // local file
    mutable uint64_t lastSyncFileSize{ 0 }; // changeable by const types
    mutable time_t lastSyncTimeLastModified{ 0 }; // changeable by const types

    // remote file
    size_t compressedFileIndex{ 0 };  // index into m_compressedFiles
    mutable bool skipFile{ false }; // changeable by const types
    bool isThumbnail{ false };
};

struct FolderDetail
{
    String folderId;
    String folderName;
    String relFolderPath;
    bool hasLastSync{ false };
    bool existsOnRemote{ false };
    mutable bool existsLocally{ false };
};

struct CompressedFile
{
    String fileId;
    String fileName;
    String downloadUrl;
    uint64_t compressedSizeBytes;
    uint64_t uncompressedSizeBytes;
    time_t timeLastModified;
    CompressionType compression;
    size_t compressedFileIndex; // this CompressedFile's index in m_compressedFiles
    mutable bool hasDownloadedLocally; // changeable by const types
    SharedPtr<ArchiveContext> archiveContext;
};

} // namespace GameSave
} // namespace PlayFab
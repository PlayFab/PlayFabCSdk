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
    // Set when the file was actually seen on disk during the local scan (MergeLocalFiles). File
    // size can't stand in for this: a legitimate zero-byte save file would otherwise be reported
    // as deleted and removed from the cloud on the next upload.
    mutable bool existsLocally{ false }; // changeable by const types
    mutable uint64_t lastSyncFileSize{ 0 }; // changeable by const types
    mutable time_t lastSyncTimeLastModified{ 0 }; // changeable by const types

    // Cloud timestamp at last sync. On platforms where SetFileLastModifiedTime is a no-op,
    // lastSyncTimeLastModified stores the local disk timestamp which differs from the cloud's timestamp.
    // This field stores the cloud's timeLastModified so HasRemoteFileChanged can compare correctly.
    // On platforms where SetFileLastModifiedTime works, this equals lastSyncTimeLastModified.
    mutable time_t lastSyncRemoteTimeLastModified{ 0 }; // changeable by const types

    // True when this file has been synced with the cloud at least once (it is present in
    // localstate.json, or it was just uploaded/downloaded). Deletion detection needs this instead
    // of "lastSyncFileSize != 0", which cannot distinguish a synced zero-byte file from a file
    // that was never synced at all.
    mutable bool hasLastSync{ false }; // changeable by const types

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
    mutable bool hasLastSync{ false };
    bool existsOnRemote{ false };
    mutable bool existsLocally{ false };
};

struct CompressedFile
{
    String fileId;
    String fileName;
    String downloadUrl;
    uint64_t compressedSizeBytes{ 0 };
    uint64_t uncompressedSizeBytes{ 0 };
    time_t timeLastModified{ 0 };
    CompressionType compression{ CompressionType::None };
    size_t compressedFileIndex{ 0 }; // this CompressedFile's index in m_compressedFiles
    mutable bool hasDownloadedLocally{ false }; // changeable by const types
    SharedPtr<ArchiveContext> archiveContext;
};

} // namespace GameSave
} // namespace PlayFab
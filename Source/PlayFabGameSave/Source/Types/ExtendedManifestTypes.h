// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

#include "Compression.h"

namespace PlayFab
{
namespace GameSave
{
    
enum class CompressionType
{
    None,
    GZip,
    Zip
};

struct ExtendedManifestExtractedFileDetail
{
    String fileName;
    String fileId;
    String folderId;
    String relFolderPath;
    String folderName;
    uint64_t uncompressedSizeBytes;
    bool skipFile;
    time_t timeLastModified;
    time_t timeCreated;
};

struct ExtendedManifestCompressedFileDetail
{
    String fileName;
    String fullFilePath;
    String fileId;
    String downloadUrl;
    uint64_t compressedSizeBytes;
    uint64_t uncompressedSizeBytes;
    time_t timeLastModified;
    CompressionType compression;
    Vector<ExtendedManifestExtractedFileDetail> extractedFiles;
    SharedPtr<ArchiveContext> archiveContext;
};

struct ExtendedManifestNestedFolder
{
    String relFolderPath;
    String folderId;
    UnorderedMap<String, ExtendedManifestNestedFolder> subfolders;
};

} // namespace GameSave
} // namespace PlayFab
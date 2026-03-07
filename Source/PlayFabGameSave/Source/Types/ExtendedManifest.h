// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include "FileFolderSetTypes.h"

namespace PlayFab
{
namespace GameSave
{

class FileFolderSet;

class ExtendedManifest
{
public:
    static Result<String> WriteExtendedManifest(
        const Vector<ExtendedManifestCompressedFileDetail>& compressedFilesToUpload, 
        const SharedPtr<FileFolderSet>& localFileFolderSet, 
        const SharedPtr<FileFolderSet>& remoteFileFolderSet,
        const String& saveFolder,
        bool compressedIncludesExtendedManifest);

    static String ConvertCompressionToString(CompressionType compression);
    static CompressionType ConvertStringToCompression(const String& compressionStr);

private:
    static void WriteCompressedFileJson(JsonValue& jsonObj, const ExtendedManifestCompressedFileDetail& compressedFile, const ExtendedManifestNestedFolder& nested, Set<String>& folderIdsInFiles);
    static void WriteCompressedFileIndexJson(JsonValue& jsonObj, size_t compressedFileIndex, const SharedPtr<FileFolderSet>& remoteFileFolderSet, const ExtendedManifestNestedFolder& nested, Set<String>& folderIdsInFiles);
    static JsonValue CreateNestedFolderJson(const SharedPtr<FileFolderSet>& localFileFolderSet, const String& parentPath, const String& folderName, ExtendedManifestNestedFolder& folder);
    static void CreateNestedStructure(const SharedPtr<FileFolderSet>& localFileFolderSet, ExtendedManifestNestedFolder& nestedStructure, const String& saveFolder);
    static void AddPath(ExtendedManifestNestedFolder& root, const String& path);
};

} // namespace GameSave
} // namespace PlayFab

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
    // Returns E_INVALIDARG for any value outside the manifest schema's "none"/"gzip"/"zip".
    static HRESULT ConvertStringToCompression(const String& compressionStr, _Out_ CompressionType& compression);

private:
    static void WriteCompressedFileJson(JsonValue& jsonObj, const ExtendedManifestCompressedFileDetail& compressedFile, Set<String>& folderIdsInFiles);
    static void WriteCompressedFileIndexJson(JsonValue& jsonObj, size_t compressedFileIndex, const SharedPtr<FileFolderSet>& localFileFolderSet, const SharedPtr<FileFolderSet>& remoteFileFolderSet, Set<String>& folderIdsInFiles);
    static JsonValue CreateNestedFolderJson(const SharedPtr<FileFolderSet>& localFileFolderSet, const SharedPtr<FileFolderSet>& remoteFileFolderSet, const String& parentPath, const String& folderName, ExtendedManifestNestedFolder& folder);
    static void CreateNestedStructure(const SharedPtr<FileFolderSet>& localFileFolderSet, ExtendedManifestNestedFolder& nestedStructure, const String& saveFolder);
    static void AddPath(ExtendedManifestNestedFolder& root, const String& path);
};

} // namespace GameSave
} // namespace PlayFab

// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "ExtendedManifest.h"
#include "ApiHelpers.h"
#include "JsonUtils.h"

using namespace PlayFab::GameSaveWrapper;

namespace PlayFab
{
namespace GameSave
{

namespace
{

// The extended manifest is an opaque JSON blob written by another device, so every name taken
// from it that becomes part of a local path has to be validated. Names whose spelling changes
// under Win32 path normalization are rejected outright: two logically distinct manifest entries
// that normalize to the same destination (e.g. "save.dat" and "save.dat. ") are tracked and
// conflict-compared separately but open the same file, so whichever is extracted last silently
// overwrites the other player save.
bool IsUnsafeManifestName(const String& name)
{
    // Embedded NUL/control characters would be truncated by the C-string path APIs downstream,
    // so a name like "..\0x" could pass the checks below and still resolve to "..".
    for (char c : name)
    {
        if (static_cast<unsigned char>(c) < 0x20)
        {
            return true;
        }
    }

    // Win32 strips trailing dots and spaces during path normalization, so any name carrying them
    // aliases the trimmed spelling. This also rejects the pure-traversal forms, since ".", ".."
    // and ".. " are all nothing but dots and spaces.
    auto lastValid = name.find_last_not_of(" .");
    if (lastValid == String::npos || lastValid != name.size() - 1)
    {
        return true;
    }

    // Separators would escape the intended folder. A colon anywhere selects an NTFS alternate
    // data stream ("save.dat:x" writes a stream of save.dat) or a drive ("C:..."), both of which
    // are further spellings that resolve onto something other than the intended file.
    return name.find('/') != String::npos ||
        name.find('\\') != String::npos ||
        name.find(':') != String::npos;
}

} // anonymous namespace

HRESULT FileFolderSet::InitWithExtendedManifest(const Vector<char>& manifestBytes, const DownloadDetailsWrapVector& remoteFileDetails, const String& saveFolder)
{
    Clear();

    if (manifestBytes.empty())
    {
        TRACE_ERROR("[GAME SAVE] InitWithExtendedManifest: manifestBytes is empty");
        return E_INVALIDARG;
    }

    JsonValue jsonBase;
    bool parseError = false;
    String parseErrorMsg;

    try
    {
        String manifestString(manifestBytes.data(), manifestBytes.size());
        bool isEmpty = (std::all_of(manifestString.begin(), manifestString.end(), [](char c) { return c == '\0'; }));
        if (isEmpty)
        {
            TRACE_ERROR("[GAME SAVE] InitWithExtendedManifest: manifest content is all null bytes");
            return E_INVALIDARG;
        }
        
        jsonBase = JsonValue::parse(manifestString);
    }
    catch (const JsonValue::parse_error& e)
    {
        parseErrorMsg = e.what();
        parseError = true;
        TRACE_ERROR("[GAME SAVE] InitWithExtendedManifest: JSON parse error: %s", parseErrorMsg.c_str());
    }

    if (parseError)
    {
        return E_FAIL;
    }

    if (!jsonBase.contains("v1"))
    {
        TRACE_ERROR("[GAME SAVE] InitWithExtendedManifest: JSON does not contain 'v1' key");
        return E_FAIL;
    }

    if (jsonBase.contains("v1"))
    {
        auto& json = jsonBase["v1"];

        if (json.contains("Folders"))
        {
            auto& foldersJson = json["Folders"];
            if (foldersJson.is_array())
            {
                String curPath = "";
                RETURN_IF_FAILED(ExtendedManifestParseFolderJson(json, curPath, true, saveFolder));
            }
        }

        // Log folder map state after folder parsing completes
        TRACE_INFORMATION("[GAME SAVE] ExtendedManifest: After folder parsing, folderIdMap has %zu entries, folders vector has %zu entries",
            m_folderFolderIdMap.size(), m_folders.size());

        if (json.contains("Files"))
        {
            auto& filesJson = json["Files"];
            if (filesJson.is_array() && filesJson.size() > 0)
            {
                for (const auto& fileJson : filesJson.get<Vector<JsonValue>>())
                {
                    CompressedFile f{};
                    JsonUtils::ObjectGetMember(fileJson, "Name", f.fileName);

                    // The compressed file name is joined onto the local cloudsync folder to build
                    // the download destination, so it needs the same traversal validation the
                    // extracted-file and folder names get.
                    //
                    // Reject the whole manifest rather than dropping the entry: a partially
                    // populated remote set looks to CompareStep like the cloud deleted those
                    // files, which would delete the healthy local copies.
                    if (IsUnsafeManifestName(f.fileName))
                    {
                        TRACE_ERROR("[GAME SAVE] ExtendedManifest: Rejecting manifest - unsafe compressed file name '%s'", f.fileName.c_str());
                        return E_INVALIDARG;
                    }

                    JsonUtils::ObjectGetMember(fileJson, "FileId", f.fileId);
                    JsonUtils::ObjectGetMember(fileJson, "CompressSize", f.compressedSizeBytes);
                    JsonUtils::ObjectGetMember(fileJson, "Size", f.uncompressedSizeBytes);
                    String lastModifiedStr;
                    JsonUtils::ObjectGetMember(fileJson, "LastModified", lastModifiedStr);
                    f.timeLastModified = Iso8601StringToTimeT(lastModifiedStr);
                    if (f.timeLastModified == 0 && !lastModifiedStr.empty())
                    {
                        TRACE_WARNING("[GAME SAVE] ExtendedManifest: Failed to parse LastModified timestamp '%s' for file '%s'", lastModifiedStr.c_str(), f.fileName.c_str());
                    }
                    String compressionTypeStr;
                    JsonUtils::ObjectGetMember(fileJson, "Compression", compressionTypeStr);
                    f.downloadUrl = ManifestInternal::GetDownloadUrlForFile(f.fileName, remoteFileDetails);
                    if (FAILED(ExtendedManifest::ConvertStringToCompression(compressionTypeStr, f.compression)))
                    {
                        TRACE_ERROR("[GAME SAVE] ExtendedManifest: Rejecting manifest - unsupported Compression '%s' for file '%s'", compressionTypeStr.c_str(), f.fileName.c_str());
                        return E_INVALIDARG;
                    }
                    CompressionType compression = f.compression;
                    f.archiveContext = MakeShared<ArchiveContext>();
                    size_t compressionFileIndex = AddCompressedFile(std::move(f));

                    JsonValue extractedFilesJson;
                    JsonUtils::ObjectGetMember(fileJson, "Extract", extractedFilesJson);

                    if (extractedFilesJson.is_array() && extractedFilesJson.size() > 0)
                    {
                        for (auto& extractedFileJson : extractedFilesJson.get<Vector<JsonValue>>())
                        {
                            FileDetail e{};
                            String folderId;
                            String dateStr;
                            JsonUtils::ObjectGetMember(extractedFileJson, "Name", e.fileName);

                            // Validate file name against path traversal. As above, a bad entry
                            // fails the whole manifest: silently omitting it would be read as a
                            // cloud-side deletion of a file that is still there.
                            if (IsUnsafeManifestName(e.fileName))
                            {
                                TRACE_ERROR("[GAME SAVE] ExtendedManifest: Rejecting manifest - unsafe file name '%s'", e.fileName.c_str());
                                return E_INVALIDARG;
                            }

                            JsonUtils::ObjectGetMember(extractedFileJson, "FileId", e.fileId);
                            JsonUtils::ObjectGetMember(extractedFileJson, "FolderId", folderId);
                            e.folderIndex = GetFolderDetailIndexFromFolderId(folderId);
                            if (e.folderIndex == SIZE_MAX)
                            {
                                TRACE_ERROR("[GAME SAVE] ExtendedManifest: Rejecting manifest - unknown folderId '%s' for file '%s'", folderId.c_str(), e.fileName.c_str());
                                return E_INVALIDARG;
                            }
                            JsonUtils::ObjectGetMember(extractedFileJson, "Size", e.fileSizeBytes);
                            JsonUtils::ObjectGetMember(extractedFileJson, "SkipFile", e.skipFile);
                            JsonUtils::ObjectGetMember(extractedFileJson, "LastModified", dateStr);
                            e.timeLastModified = Iso8601StringToTimeT(dateStr);
                            JsonUtils::ObjectGetMember(extractedFileJson, "Created", dateStr);
                            e.timeCreated = Iso8601StringToTimeT(dateStr);
                            e.compressedFileIndex = compressionFileIndex;

                            if (!e.skipFile) // don't bother recording skipped files
                            {
                                if (AddFileDetail(std::move(e)) == SIZE_MAX)
                                {
                                    TRACE_ERROR("[GAME SAVE] ExtendedManifest: Rejecting manifest - AddFileDetail failed for file in compressed bundle");
                                    return E_INVALIDARG;
                                }
                            }
                        }
                    }
                    else if (compression == CompressionType::None || compression == CompressionType::GZip)
                    {
                        TRACE_ERROR("[GAME SAVE] ExtendedManifest: %s entry has no extracted files — malformed manifest", compressionTypeStr.c_str());
                        return E_UNEXPECTED;
                    }
                }
            }
        }
    }

    // Only now is this set an authoritative picture of what the cloud holds. Callers use this to
    // distinguish "the cloud has no files" from "we never learned what the cloud has", which is the
    // difference between a correct deletion and wiping the player's save.
    m_initializedFromExtendedManifest = true;

    return S_OK;
}

void ExtendedManifest::AddPath(ExtendedManifestNestedFolder& root, const String& path)
{
    size_t start = 0;
    size_t end = 0;
    ExtendedManifestNestedFolder* current = &root;
    char sep = FilePAL::GetPathSeparatorChar();

    while ((end = path.find(sep, start)) != std::string::npos)
    {
        String folder = path.substr(start, end - start);
        if (!folder.empty())
        {
            if (current->subfolders.find(folder) == current->subfolders.end())
            {
                current->subfolders[folder] = ExtendedManifestNestedFolder();
            }
            current = &current->subfolders[folder];
        }
        start = end + 1;
    }

    // Add the last segment after the final sep
    String folder = path.substr(start);
    if (!folder.empty())
    {
        if (current->subfolders.find(folder) == current->subfolders.end())
        {
            current->subfolders[folder] = ExtendedManifestNestedFolder();
        }
    }
}

void ExtendedManifest::CreateNestedStructure(const SharedPtr<FileFolderSet>& localFileFolderSet, ExtendedManifestNestedFolder& nestedStructure, const String& saveFolder)
{
    const Vector<FolderDetail>& folders = localFileFolderSet->GetFolders();
    for (const FolderDetail& folder : folders)
    {
        if (!folder.existsLocally)
        {
            String fullPath;
            HRESULT hr = JoinPathHelper(saveFolder, folder.relFolderPath, fullPath);
            if (FAILED(hr))
            {
                TRACE_WARNING("[GAME SAVE] ExtendedManifest: JoinPathHelper failed for folder '%s', hr=0x%08X", folder.relFolderPath.c_str(), hr);
                continue;
            }
            folder.existsLocally = FilePAL::DoesDirectoryExist(fullPath);
            assert(folder.existsLocally == false);
        }

        if (folder.existsLocally)
        {
            AddPath(nestedStructure, folder.relFolderPath);
        }
    }
}

JsonValue ExtendedManifest::CreateNestedFolderJson(const SharedPtr<FileFolderSet>& localFileFolderSet, const SharedPtr<FileFolderSet>& remoteFileFolderSet, const String& parentPath, const String& folderName, ExtendedManifestNestedFolder& nestedFolder)
{
    JsonValue foldersJson = JsonValue::object();
    if (!folderName.empty())
    {
        HRESULT hr = JoinPathHelper(parentPath, folderName, nestedFolder.relFolderPath);
        if (FAILED(hr))
        {
            TRACE_WARNING("[GAME SAVE] ExtendedManifest: JoinPathHelper failed for folder '%s/%s', hr=0x%08X", parentPath.c_str(), folderName.c_str(), hr);
        }
    }
    else
    {
        nestedFolder.relFolderPath = parentPath;
    }

    // Map to an existing folder GUID from local manifest first.
    // Only consider folders that actually exist on disk — stale local entries (existsLocally == false)
    // may have outdated folder IDs that conflict with the remote manifest's IDs.
    const Vector<FolderDetail>& localFolders = localFileFolderSet->GetFolders();
    for (const FolderDetail& localFolder : localFolders)
    {
        if (localFolder.existsLocally && localFolder.relFolderPath == nestedFolder.relFolderPath)
        {
            nestedFolder.folderId = localFolder.folderId;
            break;
        }
    }
    
    // If not found in local, check remote manifest (for kept remote files)
    if (nestedFolder.folderId.empty() && remoteFileFolderSet)
    {
        const Vector<FolderDetail>& remoteFolders = remoteFileFolderSet->GetFolders();
        for (const FolderDetail& remoteFolder : remoteFolders)
        {
            if (remoteFolder.relFolderPath == nestedFolder.relFolderPath)
            {
                nestedFolder.folderId = remoteFolder.folderId;
                TRACE_VERBOSE("[GAME SAVE] ExtendedManifest: Using remote folderId '%s' for path '%s'",
                    nestedFolder.folderId.c_str(), nestedFolder.relFolderPath.c_str());
                break;
            }
        }
    }
    
    if (nestedFolder.folderId.empty())
    {
        nestedFolder.folderId = CreateGUID();
    }

    JsonUtils::ObjectAddMember(foldersJson, "Name", folderName);
    JsonUtils::ObjectAddMember(foldersJson, "Id", nestedFolder.folderId);

    JsonValue subfoldersJsonArray = JsonValue::array();
    for (auto& subfolder : nestedFolder.subfolders)
    {
        JsonValue subfolderJson = CreateNestedFolderJson(localFileFolderSet, remoteFileFolderSet, nestedFolder.relFolderPath, subfolder.first, subfolder.second);
        subfoldersJsonArray.push_back(subfolderJson);
    }
    JsonUtils::ObjectAddMember(foldersJson, "Folders", std::move(subfoldersJsonArray));

    return foldersJson;
}

Result<String> ExtendedManifest::WriteExtendedManifest(
    const Vector<ExtendedManifestCompressedFileDetail>& compressedFilesToUpload,
    const SharedPtr<FileFolderSet>& localFileFolderSet,
    const SharedPtr<FileFolderSet>& remoteFileFolderSet,
    const String& saveFolder,
    bool compressedIncludesExtendedManifest)
{
    const Vector<size_t>& compressedFilesToKeep = remoteFileFolderSet->GetCompressedFilesToKeep();

    // Convert local state folder structure (foldersToUpload) to a nested folder JSON extended manifest
    ExtendedManifestNestedFolder nested;
    CreateNestedStructure(localFileFolderSet, nested, saveFolder);
    // Note: We don't generate the folder JSON yet - we need to first collect folder IDs from files
    // and potentially add missing remote folders to the nested structure

    Set<String> folderIdsInFiles;

    JsonValue fileJsonArray = JsonValue::array();
    size_t numCompressedFiles = compressedFilesToUpload.size();
    if (compressedIncludesExtendedManifest && numCompressedFiles > 0)
    {
        numCompressedFiles--;
    }
    for (size_t i = 0; i < numCompressedFiles; ++i)
    {
        const ExtendedManifestCompressedFileDetail& compressedFile = compressedFilesToUpload[i];
        JsonValue jsonObj = JsonValue::object();
        WriteCompressedFileJson(jsonObj, compressedFile, folderIdsInFiles);
        fileJsonArray.push_back(jsonObj);
    }

    for (size_t compressedFileIndex : compressedFilesToKeep)
    {
        JsonValue jsonObj = JsonValue::object();
        WriteCompressedFileIndexJson(jsonObj, compressedFileIndex, localFileFolderSet, remoteFileFolderSet, folderIdsInFiles);
        fileJsonArray.push_back(jsonObj);
    }

    // Check for folderId mismatches between Files and Folders sections.
    // This can happen when keeping remote files whose folders don't exist locally.
    // We need to add those missing remote folder paths to the nested structure before
    // regenerating the JSON, so that proper folder hierarchy is preserved.
    const Vector<FolderDetail>& localFolders = localFileFolderSet->GetFolders();
    const Vector<FolderDetail>& remoteFolders = remoteFileFolderSet->GetFolders();
    
    // Build a set of local folder IDs that actually exist locally (match what CreateNestedStructure writes).
    // Folders that are in the local manifest but deleted on disk (existsLocally == false) are NOT
    // written to the Folders JSON by CreateNestedStructure, so we must not include them here.
    Set<String> localFolderIds;
    for (const FolderDetail& localFolder : localFolders)
    {
        if (localFolder.existsLocally)
        {
            localFolderIds.insert(localFolder.folderId);
        }
    }
    
    // Build a map of remote folder IDs to their full paths for quick lookup
    Map<String, const FolderDetail*> remoteFolderById;
    for (const FolderDetail& remoteFolder : remoteFolders)
    {
        remoteFolderById[remoteFolder.folderId] = &remoteFolder;
    }
    
    // Find missing folders and add their paths to the nested structure
    for (const String& folderIdInFile : folderIdsInFiles)
    {
        // Skip if already in local folders that exist on disk
        if (localFolderIds.find(folderIdInFile) != localFolderIds.end())
        {
            continue;
        }
        
        // Skip the root folder (empty guid)
        if (folderIdInFile == "{00000000-0000-0000-0000-000000000000}")
        {
            continue;
        }
        
        // Look for this folder in remote folders
        auto remoteIt = remoteFolderById.find(folderIdInFile);
        if (remoteIt != remoteFolderById.end())
        {
            const FolderDetail* remoteFolder = remoteIt->second;
            TRACE_INFORMATION("[GAME SAVE] ExtendedManifest: Adding remote folder path '%s' (folderId='%s') to nested structure for kept files",
                remoteFolder->relFolderPath.c_str(), folderIdInFile.c_str());
            
            // Add the full path to the nested structure. This properly handles nested folders
            // like "saves/profiles" by adding each path segment in order (saves -> profiles).
            // The nested structure will then be serialized correctly by CreateNestedFolderJson.
            AddPath(nested, remoteFolder->relFolderPath);
        }
        else
        {
            TRACE_WARNING("[GAME SAVE] ExtendedManifest: FolderId '%s' not found in local or remote folders - orphaned file reference",
                folderIdInFile.c_str());
        }
    }
    
    // Regenerate the folders JSON now that we've added any missing remote folder paths.
    // Pass remoteFileFolderSet so we can look up folder IDs for paths that came from remote.
    JsonValue foldersRootJson = CreateNestedFolderJson(localFileFolderSet, remoteFileFolderSet, "", "", nested);

    JsonValue v1Json = JsonValue::object();
    JsonUtils::ObjectAddMember(v1Json, "Files", std::move(fileJsonArray));
    if (foldersRootJson.contains("Folders")) // Don't write out root folder into ext manifest, just the subfolders
    {
        auto& subFoldersJson = foldersRootJson["Folders"];
        if (subFoldersJson.is_array())
        {
            JsonUtils::ObjectAddMember(v1Json, "Folders", std::move(subFoldersJson));
        }
    }

    JsonValue rootJson = JsonValue::object();
    JsonUtils::ObjectAddMember(rootJson, "v1", std::move(v1Json));

    return JsonUtils::WriteToString(rootJson);
}

String ExtendedManifest::ConvertCompressionToString(CompressionType compression)
{
    switch (compression)
    {
        case CompressionType::GZip: return "gzip"; break;
        case CompressionType::Zip: return "zip"; break;
        default: return "none"; break;
    }
}

HRESULT ExtendedManifest::ConvertStringToCompression(const String& compressionStr, _Out_ CompressionType& compression)
{
    // Compression is untrusted manifest input and must be parsed strictly. Defaulting an
    // unrecognized value (or a missing field) to None made the download path move the raw bundle
    // blob straight onto the advertised save-file path, so a bundle that is really a zip but
    // labelled e.g. "zstd" replaced valid save content with archive bytes and then recorded it as
    // synchronized. Anything outside the schema fails the manifest instead.
    if (compressionStr == "zip")
    {
        compression = CompressionType::Zip;
        return S_OK;
    }
    if (compressionStr == "gzip")
    {
        compression = CompressionType::GZip;
        return S_OK;
    }
    if (compressionStr == "none")
    {
        compression = CompressionType::None;
        return S_OK;
    }

    compression = CompressionType::None;
    return E_INVALIDARG;
}

void ExtendedManifest::WriteCompressedFileIndexJson(JsonValue& jsonObj, size_t compressedFileIndex, const SharedPtr<FileFolderSet>& localFileFolderSet, const SharedPtr<FileFolderSet>& remoteFileFolderSet, Set<String>& folderIdsInFiles)
{
    const Vector<CompressedFile>& compressedFiles = remoteFileFolderSet->GetCompressedFiles();
    const CompressedFile& compressedFile = compressedFiles[compressedFileIndex];
    JsonUtils::ObjectAddMember(jsonObj, "Name", compressedFile.fileName);
    JsonUtils::ObjectAddMember(jsonObj, "FileId", compressedFile.fileId);
    JsonUtils::ObjectAddMember(jsonObj, "CompressSize", compressedFile.compressedSizeBytes);
    JsonUtils::ObjectAddMember(jsonObj, "Size", compressedFile.uncompressedSizeBytes);
    JsonUtils::ObjectAddMember(jsonObj, "LastModified", TimeTToIso8601String(compressedFile.timeLastModified));
    JsonUtils::ObjectAddMember(jsonObj, "Compression", ConvertCompressionToString(compressedFile.compression));

    const Vector<FileDetail>& files = remoteFileFolderSet->GetFiles();
    JsonValue extractedFilesJsonArray = JsonValue::array();
    for (const FileDetail& file : files)
    {
        if (file.compressedFileIndex != compressedFileIndex)
        {
            continue; // skip any files that aren't part of this zip
        }
        JsonValue jsonFileObj = JsonValue::object();
        JsonUtils::ObjectAddMember(jsonFileObj, "Name", file.fileName);
        JsonUtils::ObjectAddMember(jsonFileObj, "FileId", file.fileId);
        const FolderDetail& folderDetail = remoteFileFolderSet->GetFileFolder(&file);
        if (file.skipFile)
        {
            // Skipped file's folder doesn't exist anymore so not in nested layout so just record root's ID
            String rootFolderId = "{00000000-0000-0000-0000-000000000000}";
            folderIdsInFiles.insert(rootFolderId);
            JsonUtils::ObjectAddMember(jsonFileObj, "FolderId", rootFolderId);
        }
        else
        {
            // Use the remote folder's ID, but normalize to the local folder's ID if the same
            // path exists locally. This ensures the Files section matches the Folders section
            // (which is generated from CreateNestedFolderJson, which prefers local IDs).
            String folderId = folderDetail.folderId;
            const Vector<FolderDetail>& localFolders = localFileFolderSet->GetFolders();
            for (const FolderDetail& localFolder : localFolders)
            {
                if (localFolder.existsLocally && localFolder.relFolderPath == folderDetail.relFolderPath)
                {
                    folderId = localFolder.folderId;
                    break;
                }
            }
            folderIdsInFiles.insert(folderId);
            JsonUtils::ObjectAddMember(jsonFileObj, "FolderId", folderId);
        }
        JsonUtils::ObjectAddMember(jsonFileObj, "Size", file.fileSizeBytes);
        JsonUtils::ObjectAddMember(jsonFileObj, "SkipFile", file.skipFile);
        JsonUtils::ObjectAddMember(jsonFileObj, "LastModified", TimeTToIso8601String(file.timeLastModified));
        JsonUtils::ObjectAddMember(jsonFileObj, "Created", TimeTToIso8601String(file.timeCreated));
        extractedFilesJsonArray.push_back(jsonFileObj);
    }
    JsonUtils::ObjectAddMember(jsonObj, "Extract", std::move(extractedFilesJsonArray));
}

void ExtendedManifest::WriteCompressedFileJson(JsonValue& jsonObj, const ExtendedManifestCompressedFileDetail& compressedFile, Set<String>& folderIdsInFiles)
{
    JsonUtils::ObjectAddMember(jsonObj, "Name", compressedFile.fileName);
    JsonUtils::ObjectAddMember(jsonObj, "FileId", compressedFile.fileId);
    JsonUtils::ObjectAddMember(jsonObj, "CompressSize", compressedFile.compressedSizeBytes);
    JsonUtils::ObjectAddMember(jsonObj, "Size", compressedFile.uncompressedSizeBytes);
    JsonUtils::ObjectAddMember(jsonObj, "LastModified", TimeTToIso8601String(compressedFile.timeLastModified));
    JsonUtils::ObjectAddMember(jsonObj, "Compression", ConvertCompressionToString(compressedFile.compression));

    JsonValue extractedFilesJsonArray = JsonValue::array();
    for (const ExtendedManifestExtractedFileDetail& extractedFileDetail : compressedFile.extractedFiles)
    {
        JsonValue jsonFileObj = JsonValue::object();
        JsonUtils::ObjectAddMember(jsonFileObj, "Name", extractedFileDetail.fileName);
        JsonUtils::ObjectAddMember(jsonFileObj, "FileId", extractedFileDetail.fileId);
        String folderId = extractedFileDetail.skipFile ? "{00000000-0000-0000-0000-000000000000}" : extractedFileDetail.folderId;
        JsonUtils::ObjectAddMember(jsonFileObj, "FolderId", folderId);
        if (!extractedFileDetail.skipFile) {
            folderIdsInFiles.insert(folderId);
        }
        JsonUtils::ObjectAddMember(jsonFileObj, "Size", extractedFileDetail.uncompressedSizeBytes);
        JsonUtils::ObjectAddMember(jsonFileObj, "SkipFile", extractedFileDetail.skipFile);
        JsonUtils::ObjectAddMember(jsonFileObj, "LastModified", TimeTToIso8601String(extractedFileDetail.timeLastModified));
        JsonUtils::ObjectAddMember(jsonFileObj, "Created", TimeTToIso8601String(extractedFileDetail.timeCreated));
        extractedFilesJsonArray.push_back(jsonFileObj);
    }
    JsonUtils::ObjectAddMember(jsonObj, "Extract", std::move(extractedFilesJsonArray));
}

HRESULT FileFolderSet::ExtendedManifestParseFolderJson(const JsonValue& folderJson, const String& curPath, bool isRoot, const String& saveFolder, uint32_t depth)
{
    // The manifest is untrusted cross-device input and this walk is recursive, so bound it.
    // Real saves nest a handful of levels; anything deeper is malformed or hostile.
    constexpr uint32_t kMaxFolderDepth = 64;
    if (depth > kMaxFolderDepth)
    {
        TRACE_ERROR("[GAME SAVE] ExtendedManifestParseFolderJson: folder nesting exceeds %u levels, rejecting manifest", kMaxFolderDepth);
        return E_INVALIDARG;
    }

    FolderDetail f{};

    if (isRoot)
    {
        // Root is treated as special.  Its not in extended manifest JSON and assigned empty guid
        f.folderId = "{00000000-0000-0000-0000-000000000000}";
        f.relFolderPath = "";
        f.folderName = "";
        f.existsOnRemote = true;
        f.existsLocally = true;
        TRACE_VERBOSE("[GAME SAVE] ExtendedManifestParseFolderJson: Processing ROOT folder");
    }
    else
    {
        JsonUtils::ObjectGetMember(folderJson, "Name", f.folderName);
        JsonUtils::ObjectGetMember(folderJson, "Id", f.folderId);
        f.existsOnRemote = true;

        // Validate folder name against path traversal: reject "..", absolute paths, and path separators.
        if (!f.folderName.empty() && IsUnsafeManifestName(f.folderName))
        {
            TRACE_ERROR("[GAME SAVE] ExtendedManifestParseFolderJson: Rejecting unsafe folder name '%s'", f.folderName.c_str());
            return E_INVALIDARG;
        }

        TRACE_VERBOSE("[GAME SAVE] ExtendedManifestParseFolderJson: Processing folder name='%s' folderId='%s'", 
            f.folderName.c_str(), f.folderId.c_str());
        if (f.folderName.empty())
        {
            f.relFolderPath = curPath;
        }
        else
        {
            RETURN_IF_FAILED(JoinPathHelper(curPath, f.folderName, f.relFolderPath));
            String fullPath;
            RETURN_IF_FAILED(JoinPathHelper(saveFolder, f.relFolderPath, fullPath));
            f.existsLocally = FilePAL::DoesDirectoryExist(fullPath);
        }
    }

    if (folderJson.contains("Folders"))
    {
        auto& subFoldersJson = folderJson["Folders"];
        if (subFoldersJson.is_array() && subFoldersJson.size() > 0)
        {
            TRACE_VERBOSE("[GAME SAVE] ExtendedManifestParseFolderJson: Folder '%s' has %zu subfolders", 
                f.relFolderPath.c_str(), subFoldersJson.size());
            for (const auto& subFolderJson : subFoldersJson.get<Vector<JsonValue>>())
            {
                RETURN_IF_FAILED(ExtendedManifestParseFolderJson(subFolderJson, f.relFolderPath, false, saveFolder, depth + 1));
            }
        }
    }
    TRACE_INFORMATION("[GAME SAVE] ExtManifestParseJson AddFolderDetail: path='%s' folderId='%s'", 
        f.relFolderPath.c_str(), f.folderId.c_str());
    AddFolderDetail(std::move(f));

    return S_OK;
}

} // namespace GameSave
} // namespace PlayFab

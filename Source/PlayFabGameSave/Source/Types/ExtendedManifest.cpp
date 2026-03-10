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
                ExtendedManifestParseFolderJson(json, curPath, true, saveFolder);
            }
        }

        if (json.contains("Files"))
        {
            auto& filesJson = json["Files"];
            if (filesJson.is_array() && filesJson.size() > 0)
            {
                for (const auto& fileJson : filesJson.get<Vector<JsonValue>>())
                {
                    CompressedFile f{};
                    JsonUtils::ObjectGetMember(fileJson, "Name", f.fileName);
                    JsonUtils::ObjectGetMember(fileJson, "FileId", f.fileId);
                    JsonUtils::ObjectGetMember(fileJson, "CompressSize", f.compressedSizeBytes);
                    JsonUtils::ObjectGetMember(fileJson, "Size", f.uncompressedSizeBytes);
                    String lastModifiedStr;
                    JsonUtils::ObjectGetMember(fileJson, "LastModified", lastModifiedStr);
                    f.timeLastModified = Iso8601StringToTimeT(lastModifiedStr);
                    String compressionTypeStr;
                    JsonUtils::ObjectGetMember(fileJson, "Compression", compressionTypeStr);
                    f.downloadUrl = ManifestInternal::GetDownloadUrlForFile(f.fileName, remoteFileDetails);
                    f.compression = ExtendedManifest::ConvertStringToCompression(compressionTypeStr);
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
                            JsonUtils::ObjectGetMember(extractedFileJson, "FileId", e.fileId);
                            JsonUtils::ObjectGetMember(extractedFileJson, "FolderId", folderId);
                            e.folderIndex = GetFolderDetailIndexFromFolderId(folderId);
                            JsonUtils::ObjectGetMember(extractedFileJson, "Size", e.fileSizeBytes);
                            JsonUtils::ObjectGetMember(extractedFileJson, "SkipFile", e.skipFile);
                            JsonUtils::ObjectGetMember(extractedFileJson, "LastModified", dateStr);
                            e.timeLastModified = Iso8601StringToTimeT(dateStr);
                            JsonUtils::ObjectGetMember(extractedFileJson, "Created", dateStr);
                            e.timeCreated = Iso8601StringToTimeT(dateStr);
                            e.compressedFileIndex = compressionFileIndex;

                            if (!e.skipFile) // don't bother recording skipped files
                            {
                                AddFileDetail(std::move(e));
                            }
                        }
                    }
                }
            }
        }
    }

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
            JoinPathHelper(saveFolder, folder.relFolderPath, fullPath);
            folder.existsLocally = FilePAL::DoesDirectoryExist(fullPath);
            assert(folder.existsLocally == false);
        }

        if (folder.existsLocally)
        {
            AddPath(nestedStructure, folder.relFolderPath);
        }
    }
}

JsonValue ExtendedManifest::CreateNestedFolderJson(const SharedPtr<FileFolderSet>& localFileFolderSet, const String& parentPath, const String& folderName, ExtendedManifestNestedFolder& nestedFolder)
{
    JsonValue foldersJson = JsonValue::object();
    if (!folderName.empty())
    {
        JoinPathHelper(parentPath, folderName, nestedFolder.relFolderPath);
    }
    else
    {
        nestedFolder.relFolderPath = parentPath;
    }

    // Map to an existing folder GUID from local manifest
    const Vector<FolderDetail>& folders = localFileFolderSet->GetFolders();
    for (const FolderDetail& localFolder : folders)
    {
        if (localFolder.relFolderPath == nestedFolder.relFolderPath)
        {
            nestedFolder.folderId = localFolder.folderId;
            break;
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
        JsonValue subfolderJson = CreateNestedFolderJson(localFileFolderSet, nestedFolder.relFolderPath, subfolder.first, subfolder.second);
        subfoldersJsonArray.push_back(subfolderJson);
    }
    JsonUtils::ObjectAddMember(foldersJson, "Folders", std::move(subfoldersJsonArray));

    return foldersJson;
}

const ExtendedManifestNestedFolder* FindNestedFolder(const ExtendedManifestNestedFolder& root, const String& folderName)
{
    Vector<const ExtendedManifestNestedFolder*> stack;
    stack.push_back(&root);

    while (!stack.empty())
    {
        const ExtendedManifestNestedFolder* currentFolder = stack.back();
        stack.pop_back();

        if (currentFolder->relFolderPath == folderName)
        {
            return currentFolder; // Found the folder, return its address
        }

        // Not found, push all subfolders into the stack for further search
        for (const auto& subfolder : currentFolder->subfolders)
        {
            stack.push_back(&(subfolder.second));
        }
    }

    return nullptr; // Folder not found
}

String GetFolderId(const ExtendedManifestNestedFolder& nested, const String& folderPath)
{
    const ExtendedManifestNestedFolder* foundFolder = FindNestedFolder(nested, folderPath);
    if (foundFolder)
    {
        return foundFolder->folderId;
    }

    assert(false);
    return String();
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
    JsonValue foldersRootJson = CreateNestedFolderJson(localFileFolderSet, "", "", nested);

    Set<String> folderIdsInFiles;

    JsonValue fileJsonArray = JsonValue::array();
    int32_t numCompressedFiles = (int32_t)compressedFilesToUpload.size();
    if (compressedIncludesExtendedManifest)
    {
        numCompressedFiles--;
    }
    for (int32_t i = 0; i < (int32_t)numCompressedFiles; ++i)
    {
        const ExtendedManifestCompressedFileDetail& compressedFile = compressedFilesToUpload[i];
        JsonValue jsonObj = JsonValue::object();
        WriteCompressedFileJson(jsonObj, compressedFile, nested, folderIdsInFiles);
        fileJsonArray.push_back(jsonObj);
    }

    for (size_t compressedFileIndex : compressedFilesToKeep)
    {
        JsonValue jsonObj = JsonValue::object();
        WriteCompressedFileIndexJson(jsonObj, compressedFileIndex, remoteFileFolderSet, nested, folderIdsInFiles);
        fileJsonArray.push_back(jsonObj);
    }

    JsonValue v1Json = JsonValue::object();
    JsonUtils::ObjectAddMember(v1Json, "Files", std::move(fileJsonArray));
    if (foldersRootJson.contains("Folders")) // Don't write out root folder into ext manifest, just the subfolders
    {
        auto& subFoldersJson = foldersRootJson["Folders"];
        if (subFoldersJson.is_array())
        {
            const Vector<FolderDetail>& localFolders = localFileFolderSet->GetFolders();
            assert(folderIdsInFiles.size() <= localFolders.size());
            for (auto it = folderIdsInFiles.begin(); it != folderIdsInFiles.end(); it++)
            {
                String folderIdsInFile = *it;
                bool found = false;
                for (const FolderDetail& folderDetail : localFolders)
                {
                    if (folderDetail.folderId == folderIdsInFile)
                    {
                        found = true;
                    }
                }
                TRACE_INFORMATION("[GAME SAVE] found: %d", found);
                if (!found)
                {
                    assert(false);
                }
            }

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

CompressionType ExtendedManifest::ConvertStringToCompression(const String& compressionStr)
{
    if (compressionStr == "zip")
    {
        return CompressionType::Zip;
    }
    else if (compressionStr == "gzip")
    {
        return CompressionType::GZip;
    }
    return CompressionType::None;
}

void ExtendedManifest::WriteCompressedFileIndexJson(JsonValue& jsonObj, size_t compressedFileIndex, const SharedPtr<FileFolderSet>& remoteFileFolderSet, const ExtendedManifestNestedFolder& nested, Set<String>& folderIdsInFiles)
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
            String folderId = GetFolderId(nested, folderDetail.relFolderPath); // need to fetch local FolderID
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

void ExtendedManifest::WriteCompressedFileJson(JsonValue& jsonObj, const ExtendedManifestCompressedFileDetail& compressedFile, const ExtendedManifestNestedFolder& nested, Set<String>& folderIdsInFiles)
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
        String folderId = GetFolderId(nested, extractedFileDetail.relFolderPath);
        JsonUtils::ObjectAddMember(jsonFileObj, "FolderId", folderId);
        folderIdsInFiles.insert(folderId);
        JsonUtils::ObjectAddMember(jsonFileObj, "Size", extractedFileDetail.uncompressedSizeBytes);
        JsonUtils::ObjectAddMember(jsonFileObj, "SkipFile", extractedFileDetail.skipFile);
        JsonUtils::ObjectAddMember(jsonFileObj, "LastModified", TimeTToIso8601String(extractedFileDetail.timeLastModified));
        JsonUtils::ObjectAddMember(jsonFileObj, "Created", TimeTToIso8601String(extractedFileDetail.timeCreated));
        extractedFilesJsonArray.push_back(jsonFileObj);
    }
    JsonUtils::ObjectAddMember(jsonObj, "Extract", std::move(extractedFilesJsonArray));
}

HRESULT FileFolderSet::ExtendedManifestParseFolderJson(const JsonValue& folderJson, const String& curPath, bool isRoot, const String& saveFolder)
{
    FolderDetail f{};

    if (isRoot)
    {
        // Root is treated as special.  Its not in extended manifest JSON and assigned empty guid
        f.folderId = "{00000000-0000-0000-0000-000000000000}";
        f.relFolderPath = "";
        f.folderName = "";
        f.existsOnRemote = true;
        f.existsLocally = true;
    }
    else
    {
        JsonUtils::ObjectGetMember(folderJson, "Name", f.folderName);
        JsonUtils::ObjectGetMember(folderJson, "Id", f.folderId);
        f.existsOnRemote = true;
        if (f.folderName.empty())
        {
            f.relFolderPath = curPath;
        }
        else
        {
            RETURN_IF_FAILED(JoinPathHelper(curPath, f.folderName, f.relFolderPath));
            String fullPath;
            JoinPathHelper(saveFolder, f.relFolderPath, fullPath);
            f.existsLocally = FilePAL::DoesDirectoryExist(fullPath);
        }
    }

    if (folderJson.contains("Folders"))
    {
        auto& subFoldersJson = folderJson["Folders"];
        if (subFoldersJson.is_array() && subFoldersJson.size() > 0)
        {
            for (const auto& subFolderJson : subFoldersJson.get<Vector<JsonValue>>())
            {
                ExtendedManifestParseFolderJson(subFolderJson, f.relFolderPath, false, saveFolder);
            }
        }
    }
    TRACE_INFORMATION("[GAME SAVE] ExtManifestParseJson AddFolderDetail: %s", f.relFolderPath.c_str());
    AddFolderDetail(std::move(f));

    return S_OK;
}

} // namespace GameSave
} // namespace PlayFab

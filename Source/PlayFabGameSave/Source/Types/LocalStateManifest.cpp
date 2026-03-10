// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "LocalStateManifest.h"
#include "ApiHelpers.h"

namespace PlayFab
{
namespace GameSave
{

HRESULT FileFolderSet::InitWithLocalFilesAndFolders(const String& saveFolder, _Out_opt_ String* shortSaveDescription, _Out_opt_ bool* descriptionDirty)
{
    Clear();

    if (shortSaveDescription != nullptr)
    {
        shortSaveDescription->clear();
    }
    if (descriptionDirty != nullptr)
    {
        *descriptionDirty = false;
    }

    String folderPath, filePath;
    RETURN_IF_FAILED(JoinPathHelper(saveFolder, "cloudsync", folderPath));
    RETURN_IF_FAILED(JoinPathHelper(folderPath, "localstate.json", filePath));
    
    Vector<char> fileData;
    RETURN_IF_FAILED(ReadEntireFile(filePath, fileData));

    JsonValue json;
    bool parseError = false;
    String parseErrorMsg;

    try
    {
        String fileString(fileData.data(), fileData.size());
        bool isEmpty = (std::all_of(fileString.begin(), fileString.end(), [](char c) { return c == '\0'; }));
        json = !isEmpty ? JsonValue::parse(fileString) : "";
    }
    catch (const JsonValue::parse_error& e)
    {
        parseErrorMsg = e.what();
        parseError = true;
        TRACE_ERROR("[GAME SAVE] InitWithLocalFilesAndFolders: JSON parse error: %s", parseErrorMsg.c_str());
    }

    if (!parseError && json.contains("Folders"))
    {
        auto& foldersJson = json["Folders"];
        if (foldersJson.is_array() && foldersJson.size() > 0)
        {
            for (auto& folderJson : foldersJson.get<Vector<JsonValue>>())
            {
                FolderDetail folder{};
                JsonUtils::ObjectGetMember(folderJson, "relFolderPath", folder.relFolderPath);
                JsonUtils::ObjectGetMember(folderJson, "folderName", folder.folderName);
                JsonUtils::ObjectGetMember(folderJson, "folderId", folder.folderId);
                String fullPath;
                JoinPathHelper(saveFolder, folder.relFolderPath, fullPath);
                folder.existsLocally = FilePAL::DoesDirectoryExist(fullPath);
                folder.hasLastSync = true;
                folder.existsOnRemote = false;
                bool isRootFolder = folder.folderName.empty();
                size_t folderIndex = AddFolderDetail(std::move(folder));

                JsonValue filesJson;
                JsonUtils::ObjectGetMember(folderJson, "Files", filesJson);

                if (filesJson.is_array() && filesJson.size() > 0)
                {
                    for (auto& fileJson : filesJson.get<Vector<JsonValue>>())
                    {
                        FileDetail fd{};
                        JsonUtils::ObjectGetMember(fileJson, "fileName", fd.fileName);
                        JsonUtils::ObjectGetMember(fileJson, "fileId", fd.fileId);
                        fd.folderIndex = folderIndex;
                        JsonUtils::ObjectGetMember(fileJson, "lastSyncFileSize", fd.lastSyncFileSize);
                        String dateStr;
                        JsonUtils::ObjectGetMember(fileJson, "lastSyncTimeLastModified", dateStr);
                        fd.lastSyncTimeLastModified = Iso8601StringToTimeT(dateStr);
                        if (isRootFolder && fd.fileName == THUMBNAIL_FILE_NAME)
                        {
                            fd.isThumbnail = true;
                        }
                        AddFileDetail(std::move(fd));
                    }
                }
            }
        }
    }

    if (!parseError && json.contains("Metadata"))
    {
        auto& metadataJson = json["Metadata"];
        if (shortSaveDescription != nullptr)
        {
            JsonUtils::ObjectGetMember(metadataJson, "shortSaveDescription", *shortSaveDescription);
        }
        if (descriptionDirty != nullptr)
        {
            JsonUtils::ObjectGetMember(metadataJson, "descriptionDirty", *descriptionDirty);
        }
    }

    // Merge with local files and folders with recursive search
    MergeLocalFolders(saveFolder, "", saveFolder);
    return S_OK;
}

HRESULT FileFolderSet::MergeLocalFolders(const String& rootPath, const String& folderName, const String& fullFolderPath)
{
    if (folderName == "cloudsync")
    {
        return S_OK;
    }

    const Vector<FolderDetail>& folders = GetFolders();
    bool foundMatchingFolder = false;
    for (size_t folderIndex = 0; folderIndex < folders.size(); folderIndex++)
    {
        const FolderDetail& folder = m_folders[folderIndex];
        String curFullFolderPath;
        RETURN_IF_FAILED(JoinPathHelper(rootPath, folder.relFolderPath, curFullFolderPath));
        if (curFullFolderPath == fullFolderPath)
        {
            folder.existsLocally = FilePAL::DoesDirectoryExist(curFullFolderPath);
            foundMatchingFolder = true;
            MergeLocalFiles(rootPath, fullFolderPath, folderIndex);
            break;
        }
    }

    if (!foundMatchingFolder)
    {
        FolderDetail folder{};
        folder.relFolderPath = RemoveRootPath(fullFolderPath, rootPath);
        folder.folderName = folderName;
        if (folderName.empty())
        {
            // Root is treated as special GUID
            folder.folderId = "{00000000-0000-0000-0000-000000000000}";
        }
        else
        {
            folder.folderId = CreateGUID();
        }
        folder.existsLocally = FilePAL::DoesDirectoryExist(fullFolderPath);
        folder.existsOnRemote = false;
        size_t folderIndex = AddFolderDetail(std::move(folder));

        MergeLocalFiles(rootPath, fullFolderPath, folderIndex);
    }

    Result<Vector<String>> subfoldersResult = FilePAL::EnumDirectories(fullFolderPath);
    RETURN_IF_FAILED(subfoldersResult.hr);
    Vector<String> subfolders = subfoldersResult.ExtractPayload();
    for (const String& subfolder : subfolders)
    {
        String fullSubfolderPath;
        RETURN_IF_FAILED(JoinPathHelper(fullFolderPath, subfolder, fullSubfolderPath));
        MergeLocalFolders(rootPath, subfolder, fullSubfolderPath); // recursively search
    }

    return S_OK;
}

HRESULT FileFolderSet::MergeLocalFiles(const String& rootPath, const String& fullFolderPath, size_t folderIndex)
{
    const FolderDetail& folder = m_folders[folderIndex];
    bool isRootFolder = folder.folderName.empty();

    const Vector<FileDetail>& files = GetFiles();

    Result<Vector<String>> localFilesResult = FilePAL::EnumFiles(fullFolderPath);
    RETURN_IF_FAILED(localFilesResult.hr);
    Vector<String> localFiles = localFilesResult.ExtractPayload();
    for (const String& localFile : localFiles)
    {
        String localFullFilePath;
        RETURN_IF_FAILED(JoinPathHelper(fullFolderPath, localFile, localFullFilePath));
        String localRelFilePath = RemoveRootPath(localFullFilePath, rootPath);

        bool foundMatchingFile = false;
        for (const FileDetail& file : files)
        {
            if (file.folderIndex != folderIndex)
            {
                continue;
            }

            String relFilePath = GetRelFilePath(&file);
            if (relFilePath == localRelFilePath)
            {
                foundMatchingFile = true;
                file.fileSizeBytes = FilePAL::GetFileSize(localFullFilePath).Payload();
                FilePAL::GetFileTimes(localFullFilePath, file.timeCreated, file.timeLastModified);
                break;
            }
        }

        if (!foundMatchingFile)
        {
            FileDetail fd{};
            fd.fileName = localFile;
            fd.fileId = CreateGUID();
            fd.folderIndex = folderIndex;
            fd.fileSizeBytes = FilePAL::GetFileSize(localFullFilePath).Payload();
            FilePAL::GetFileTimes(localFullFilePath, fd.timeCreated, fd.timeLastModified);
            if (isRootFolder && fd.fileName == THUMBNAIL_FILE_NAME)
            {
                fd.isThumbnail = true;
            }
            AddFileDetail(std::move(fd));
        }
    }

    return S_OK;
}

HRESULT LocalStateManifest::WriteLocalManifest(
    const String& rootPath, 
    const SharedPtr<FileFolderSet>& localFileFolderSet, 
    const String& shortSaveDescription,
    bool descriptionDirty)
{
    const Vector<FolderDetail>& folders = localFileFolderSet->GetFolders();
    const Vector<FileDetail>& files = localFileFolderSet->GetFiles();

    JsonValue fileJsonArray = JsonValue::array();
    for (size_t folderIndex = 0; folderIndex < folders.size(); folderIndex++)
    {
        const FolderDetail& folderDetail = folders[folderIndex];

        JsonValue jsonObj = JsonValue::object();
        JsonUtils::ObjectAddMember(jsonObj, "relFolderPath", folderDetail.relFolderPath);
        JsonUtils::ObjectAddMember(jsonObj, "folderName", folderDetail.folderName);
        JsonUtils::ObjectAddMember(jsonObj, "folderId", folderDetail.folderId);

        JsonValue filesJsonArray = JsonValue::array();
        for (const FileDetail& fileDetail : files)
        {
            if (fileDetail.folderIndex != folderIndex) // skip files not in this folder
                continue;

            if( fileDetail.fileSizeBytes == 0 && fileDetail.lastSyncFileSize == 0 ) // don't bother keep track files deleted locally and deleted in last sync
                continue;

            JsonValue fileDetailObj = JsonValue::object();
            //String relFilePath;
            //RETURN_IF_FAILED(JoinPathHelper(folderDetail.relFolderPath, fileDetail.fileName, relFilePath));
            JsonUtils::ObjectAddMember(fileDetailObj, "fileName", fileDetail.fileName);
            JsonUtils::ObjectAddMember(fileDetailObj, "fileId", fileDetail.fileId);
            JsonUtils::ObjectAddMember(fileDetailObj, "lastSyncFileSize", fileDetail.lastSyncFileSize);
            JsonUtils::ObjectAddMember(fileDetailObj, "lastSyncTimeLastModified", TimeTToIso8601String(fileDetail.lastSyncTimeLastModified));
            filesJsonArray.push_back(fileDetailObj);
        }
        JsonUtils::ObjectAddMember(jsonObj, "Files", std::move(filesJsonArray));

        fileJsonArray.push_back(jsonObj);
    }

    JsonValue outerJson = JsonValue::object();
    JsonUtils::ObjectAddMember(outerJson, "Folders", std::move(fileJsonArray));

    JsonValue jsonMetadata = JsonValue::object();
    JsonUtils::ObjectAddMember(jsonMetadata, "shortSaveDescription", shortSaveDescription);
    JsonUtils::ObjectAddMember(jsonMetadata, "descriptionDirty", descriptionDirty);
    JsonUtils::ObjectAddMember(outerJson, "Metadata", std::move(jsonMetadata));

    String str = JsonUtils::WriteToString(outerJson);
    Vector<char> vData;
    std::copy(str.begin(), str.end(), std::back_inserter(vData));
    
    String folderPath, filePath;
    RETURN_IF_FAILED(JoinPathHelper(rootPath, "cloudsync", folderPath));
    FilePAL::CreatePath(folderPath);
    RETURN_IF_FAILED(JoinPathHelper(folderPath, "localstate.json", filePath));
    
    HRESULT writeHr = WriteEntireFile(filePath, vData);
    if (FAILED(writeHr))
    {
        TRACE_ERROR("[GAME SAVE] WriteLocalManifest: WriteEntireFile FAILED hr=0x%08X", writeHr);
        return writeHr;
    }

    return S_OK;
}


} // namespace GameSave
} // namespace PlayFab
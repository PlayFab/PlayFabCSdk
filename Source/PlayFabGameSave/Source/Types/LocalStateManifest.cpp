// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "LocalStateManifest.h"
#include "ApiHelpers.h"
#include "Platform/PFGameSaveFilesAPIProvider.h"


namespace PlayFab
{
namespace GameSave
{

HRESULT FileFolderSet::InitWithLocalFilesAndFolders(const String& saveFolder, _Out_opt_ String* shortSaveDescription, _Out_opt_ bool* descriptionDirty, _Out_opt_ bool* localStateFound)
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
    if (localStateFound != nullptr)
    {
        *localStateFound = false;
    }

    // Acquire cloud sync storage if the platform stores localstate.json separately
    SharedPtr<GameSaveGlobalState> globalState;
    String cloudSyncRoot;
    bool acquiredCloudSyncStorage = false;
    if (SUCCEEDED(GameSaveGlobalState::Get(globalState)))
    {
        RETURN_IF_FAILED(globalState->ApiProvider().AcquireCloudSyncStorage(cloudSyncRoot));
        acquiredCloudSyncStorage = true;
    }
    String metadataRoot = cloudSyncRoot.empty() ? saveFolder : cloudSyncRoot;

    String folderPath, filePath;
    HRESULT pathHr = JoinPathHelper(metadataRoot, "cloudsync", folderPath);
    if (SUCCEEDED(pathHr))
    {
        pathHr = JoinPathHelper(folderPath, "localstate.json", filePath);
    }
    
    Vector<char> fileData;
    if (SUCCEEDED(pathHr))
    {
        HRESULT readHr = ReadEntireFile(filePath, fileData);
        bool foundLocalState = SUCCEEDED(readHr) && !fileData.empty();
        if (localStateFound != nullptr)
        {
            *localStateFound = foundLocalState;
        }
        if (FAILED(readHr))
        {
            // localstate.json may not exist yet for new saves — continue with empty data
            // so MergeLocalFolders still discovers files on disk
            fileData.clear();
        }
    }

    // Release cloud sync storage after reading
    if (globalState && acquiredCloudSyncStorage)
    {
        globalState->ApiProvider().ReleaseCloudSyncStorage();
    }

    RETURN_IF_FAILED(pathHr);

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

    bool platformNeedsRemoteTimestamp = false;
    if (globalState)
    {
        platformNeedsRemoteTimestamp = globalState->ApiProvider().PlatformNeedsRemoteTimestamp();
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
                        // hasLastSync has to round-trip explicitly: localstate.json also records
                        // files that exist locally but have never been uploaded, and promoting
                        // those to "synced" on reload makes the next compare treat them as cloud
                        // deletions and delete them before they can ever be uploaded.
                        if (fileJson.contains("hasLastSync"))
                        {
                            JsonUtils::ObjectGetMember(fileJson, "hasLastSync", fd.hasLastSync);
                        }
                        else
                        {
                            // Written by an older SDK that only persisted synced files plus files
                            // with live data; a non-zero last-sync size is the only evidence of a
                            // completed sync available in that format.
                            fd.hasLastSync = (fd.lastSyncFileSize != 0);
                        }
                        String dateStr;
                        JsonUtils::ObjectGetMember(fileJson, "lastSyncTimeLastModified", dateStr);
                        fd.lastSyncTimeLastModified = Iso8601StringToTimeT(dateStr);

                        // Read lastSyncRemoteTimeLastModified if present; fall back to lastSyncTimeLastModified
                        if (platformNeedsRemoteTimestamp)
                        {
                            String remoteDateStr;
                            JsonUtils::ObjectGetMember(fileJson, "lastSyncRemoteTimeLastModified", remoteDateStr);
                            if (!remoteDateStr.empty())
                            {
                                fd.lastSyncRemoteTimeLastModified = Iso8601StringToTimeT(remoteDateStr);
                            }
                            else
                            {
                                // Backward compatibility: old localstate.json without this field.
                                // Fall back to lastSyncTimeLastModified. This may cause one unnecessary
                                // re-download in multi-device scenarios, then self-corrects.
                                fd.lastSyncRemoteTimeLastModified = fd.lastSyncTimeLastModified;
                            }
                        }
                        else
                        {
                            fd.lastSyncRemoteTimeLastModified = fd.lastSyncTimeLastModified;
                        }

                        if (isRootFolder && fd.fileName == THUMBNAIL_FILE_NAME)
                        {
                            fd.isThumbnail = true;
                        }
                        if (AddFileDetail(std::move(fd)) == SIZE_MAX)
                        {
                            TRACE_WARNING("[GAME SAVE] LocalStateManifest: AddFileDetail failed for file during parse, skipping");
                        }
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
    RETURN_IF_FAILED(MergeLocalFolders(saveFolder, "", saveFolder));
    return S_OK;
}

HRESULT FileFolderSet::MergeLocalFolders(const String& rootPath, const String& folderName, const String& fullFolderPath)
{
    if (folderName == "cloudsync" || folderName == kMockSaveFolderName)
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
            RETURN_IF_FAILED(MergeLocalFiles(rootPath, fullFolderPath, folderIndex));
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

        RETURN_IF_FAILED(MergeLocalFiles(rootPath, fullFolderPath, folderIndex));
    }

    Result<Vector<String>> subfoldersResult = FilePAL::EnumDirectories(fullFolderPath);
    RETURN_IF_FAILED(subfoldersResult.hr);
    Vector<String> subfolders = subfoldersResult.ExtractPayload();
    for (const String& subfolder : subfolders)
    {
        String fullSubfolderPath;
        RETURN_IF_FAILED(JoinPathHelper(fullFolderPath, subfolder, fullSubfolderPath));

        RETURN_IF_FAILED(MergeLocalFolders(rootPath, subfolder, fullSubfolderPath));
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

        String localRelFilePath= RemoveRootPath(localFullFilePath, rootPath);

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
                // Directory enumeration already proved the file exists; record that before the
                // metadata reads so a transient GetFileSize failure can't make an existing file
                // look deleted (which would remove it from the cloud on the next upload).
                file.existsLocally = true;
                // NOTE: File sizes and timestamps read here may differ from content read later during
                // zip creation if the game modifies save files during sync. Callers must ensure save
                // files are not modified while a sync operation is in progress.
                Result<uint64_t> fileSizeResult = FilePAL::GetFileSize(localFullFilePath);
                if (FAILED(fileSizeResult.hr))
                {
                    TRACE_WARNING("[GAME SAVE] MergeLocalFiles: GetFileSize failed for '%s' (hr=0x%08X), treating as unchanged this pass", localFullFilePath.c_str(), fileSizeResult.hr);
                    // Present but unreadable right now (e.g. another process holds it). Mirror the
                    // last synced metadata so this pass neither uploads a file we can't read nor
                    // reports it as changed; the next sync re-reads it.
                    file.fileSizeBytes = file.lastSyncFileSize;
                    file.timeLastModified = file.lastSyncTimeLastModified;
                    break;
                }
                file.fileSizeBytes = fileSizeResult.Payload();
                file.existsLocally = true;
                if (FAILED(FilePAL::GetFileTimes(localFullFilePath, file.timeCreated, file.timeLastModified)))
                {
                    TRACE_WARNING("[GAME SAVE] MergeLocalFiles: GetFileTimes failed for '%s', using existing timestamps", localFullFilePath.c_str());
                }
                break;
            }
        }

        if (!foundMatchingFile)
        {
            Result<uint64_t> fileSizeResult = FilePAL::GetFileSize(localFullFilePath);
            if (FAILED(fileSizeResult.hr))
            {
                TRACE_WARNING("[GAME SAVE] MergeLocalFiles: GetFileSize failed for new file '%s' (hr=0x%08X), skipping", localFullFilePath.c_str(), fileSizeResult.hr);
                continue;
            }

            FileDetail fd{};
            fd.fileName = localFile;
            fd.fileId = CreateGUID();
            fd.folderIndex = folderIndex;
            fd.fileSizeBytes = fileSizeResult.Payload();
            fd.existsLocally = true;
            if (FAILED(FilePAL::GetFileTimes(localFullFilePath, fd.timeCreated, fd.timeLastModified)))
            {
                TRACE_WARNING("[GAME SAVE] MergeLocalFiles: GetFileTimes failed for new file '%s', using default timestamps", localFullFilePath.c_str());
            }
            if (isRootFolder && fd.fileName == THUMBNAIL_FILE_NAME)
            {
                fd.isThumbnail = true;
            }
            if (AddFileDetail(std::move(fd)) == SIZE_MAX)
            {
                TRACE_WARNING("[GAME SAVE] MergeLocalFiles: AddFileDetail failed for new file '%s', skipping", localFile.c_str());
            }
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

    // Determine if we need to write the remote timestamp field
    SharedPtr<GameSaveGlobalState> globalState;
    bool platformNeedsRemoteTimestamp = false;
    if (SUCCEEDED(GameSaveGlobalState::Get(globalState)))
    {
        platformNeedsRemoteTimestamp = globalState->ApiProvider().PlatformNeedsRemoteTimestamp();
    }

    JsonValue fileJsonArray = JsonValue::array();
    for (size_t folderIndex = 0; folderIndex < folders.size(); folderIndex++)
    {
        const FolderDetail& folderDetail = folders[folderIndex];

        bool isRootFolder = folderDetail.folderName.empty();

        // Don't persist non-root folders that were deleted locally and have no remaining
        // tracked files. This mirrors the per-file cleanup below (fileSizeBytes == 0 &&
        // lastSyncFileSize == 0) and prevents stale folder entries from accumulating in
        // localstate.json, which would cause MarkFoldersToCreateUponUpload to
        // perpetually trigger full upload cycles.
        if (!isRootFolder && !folderDetail.existsLocally)
        {
            bool hasRemainingFiles = false;
            for (const FileDetail& fileDetail : files)
            {
                if (fileDetail.folderIndex != folderIndex)
                    continue;
                // A file is "remaining" if it is on disk or has unsynced last-sync state
                if (fileDetail.existsLocally || fileDetail.hasLastSync)
                {
                    hasRemainingFiles = true;
                    break;
                }
            }

            if (!hasRemainingFiles)
            {
                TRACE_INFORMATION("[GAME SAVE] WriteLocalManifest: Skipping deleted folder '%s' with no remaining files",
                    folderDetail.relFolderPath.c_str());
                continue;
            }
        }

        JsonValue jsonObj = JsonValue::object();
        JsonUtils::ObjectAddMember(jsonObj, "relFolderPath", folderDetail.relFolderPath);
        JsonUtils::ObjectAddMember(jsonObj, "folderName", folderDetail.folderName);
        JsonUtils::ObjectAddMember(jsonObj, "folderId", folderDetail.folderId);

        JsonValue filesJsonArray = JsonValue::array();
        for (const FileDetail& fileDetail : files)
        {
            if (fileDetail.folderIndex != folderIndex) // skip files not in this folder
                continue;

            if( !fileDetail.existsLocally && !fileDetail.hasLastSync ) // don't bother keep track files deleted locally and deleted in last sync
                continue;

            JsonValue fileDetailObj = JsonValue::object();
            JsonUtils::ObjectAddMember(fileDetailObj, "fileName", fileDetail.fileName);
            JsonUtils::ObjectAddMember(fileDetailObj, "fileId", fileDetail.fileId);
            JsonUtils::ObjectAddMember(fileDetailObj, "lastSyncFileSize", fileDetail.lastSyncFileSize);
            JsonUtils::ObjectAddMember(fileDetailObj, "hasLastSync", fileDetail.hasLastSync);
            JsonUtils::ObjectAddMember(fileDetailObj, "lastSyncTimeLastModified", TimeTToIso8601String(fileDetail.lastSyncTimeLastModified));

            if (platformNeedsRemoteTimestamp)
            {
                JsonUtils::ObjectAddMember(fileDetailObj, "lastSyncRemoteTimeLastModified", TimeTToIso8601String(fileDetail.lastSyncRemoteTimeLastModified));
            }

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

    // Acquire cloud sync storage for writing
    String cloudSyncRoot;
    bool acquiredCloudSyncStorage = false;
    if (globalState)
    {
        RETURN_IF_FAILED(globalState->ApiProvider().AcquireCloudSyncStorage(cloudSyncRoot));
        acquiredCloudSyncStorage = true;
    }
    String metadataRoot = cloudSyncRoot.empty() ? rootPath : cloudSyncRoot;
    
    String folderPath, filePath;
    HRESULT pathHr = JoinPathHelper(metadataRoot, "cloudsync", folderPath);
    if (SUCCEEDED(pathHr))
    {
        pathHr = FilePAL::CreatePath(folderPath);
    }
    if (SUCCEEDED(pathHr))
    {
        pathHr = JoinPathHelper(folderPath, "localstate.json", filePath);
    }
    
    HRESULT writeHr = S_OK;
    if (SUCCEEDED(pathHr))
    {
        // Write to temp file first, then rename for crash safety
        String tempFilePath = filePath + ".tmp";
        writeHr = WriteEntireFile(tempFilePath, vData);
        if (SUCCEEDED(writeHr))
        {
            writeHr = FilePAL::MoveLocalFile(tempFilePath, filePath);
            if (FAILED(writeHr))
            {
                TRACE_ERROR("[GAME SAVE] WriteLocalManifest: MoveLocalFile failed hr=0x%08X", writeHr);
                FilePAL::DeleteLocalFile(tempFilePath);
            }
        }
    }

    // Release cloud sync storage after writing (even if write failed)
    if (globalState && acquiredCloudSyncStorage)
    {
        globalState->ApiProvider().ReleaseCloudSyncStorage();
    }

    RETURN_IF_FAILED(pathHr);

    if (FAILED(writeHr))
    {
        TRACE_ERROR("[GAME SAVE] WriteLocalManifest: WriteEntireFile FAILED hr=0x%08X", writeHr);
        return writeHr;
    }

    return S_OK;
}


} // namespace GameSave
} // namespace PlayFab
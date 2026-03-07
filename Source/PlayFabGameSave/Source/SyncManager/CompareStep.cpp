// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "CompareStep.h"
#include "ApiHelpers.h"
#include "FileFolderSet.h"
#include "Metadata.h"

using namespace PlayFab::GameSaveWrapper;

namespace PlayFab
{
namespace GameSave
{
    
CompareStep::CompareStep(_In_ LocalUser const& localUser, _In_ SharedPtr<GameSaveTelemetryManager> telemetryManager) :
    m_localUser{ localUser },
    m_telemetryManager{ telemetryManager }
{
}

void CompareStep::SetEntity(_In_ const Entity& entity)
{
    m_entity = entity;
}

bool CompareStep::IsCompareDone() const
{
    return m_stage == CompareStage::CompareDone;
}

HRESULT CompareStep::CompareWithCloud(
    _In_ RunContext runContext,
    _In_ ISchedulableTask& task,
    _In_ const String& saveFolder,
    _In_ UICallbackManager& uiCallbackManager,
    _In_ bool downloading,
    _In_ const SharedPtr<ManifestInternal>& latestFinalizedManifest,
    _In_ SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ SharedPtr<FileFolderSet>& remoteFileFolderSet,
    _In_ std::recursive_mutex& folderSyncMutex
    )
{
    ScopeTracer scopeTracer(FormatString("CompareStep - %s", EnumName(m_stage)));
    m_telemetryManager->SetContextActivationCallingLocation(EnumValue<StateMachineLocation>(EnumName(m_stage)));

    switch (m_stage)
    {
        case CompareStage::GetManifestDownloadDetails:
        {
            if (latestFinalizedManifest->GotRemoteFileDetails() || 
                !latestFinalizedManifest->HasVersion() )
            {
                TRACE_TASK("GetManifestDownloadDetails.ReadLocalManifest");
                // Already have GetManifestDownloadDetails & extended-manifest so skip those
                m_stage = CompareStage::ReadLocalManifest;
                task.ScheduleNow();
            }
            else
            {
                TRACE_TASK("GetManifestDownloadDetails");
                GetManifestDownloadDetailsRequest request;
                //request.SetEntity(m_entity.value().EntityKey());
                request.SetVersion(Uint64ToString(latestFinalizedManifest->Version()));

                GameSaveServiceSelector::GetManifestDownloadDetails(m_entity.value(), request, runContext)
                .Finally([this, &task, latestFinalizedManifest, downloading, &uiCallbackManager, &folderSyncMutex](Result<GetManifestDownloadDetailsResponse> result)
                {
                    std::lock_guard<std::recursive_mutex> lock(folderSyncMutex); // Prevent any of the Finally blocks from changing the state while DoWork thread is active
                    TRACE_TASK(FormatString("GetManifestDownloadDetailsFinally HR:0x%0.8x", result.hr));
                    m_telemetryManager->SetContextActivationHttpInfo(result.httpResult);

                    if (FAILED(result.hr))
                    {
                        m_telemetryManager->SetContextActivationHResult(result.hr);
                        m_telemetryManager->EmitContextActivationFailureEvent();
                        m_stage = CompareStage::WaitForFailedUI_GetManifestDownloadDetails;
                        if (false == uiCallbackManager.ShowSyncFailedUI(task, m_localUser, result.hr, (downloading) ? PFGameSaveFilesSyncState::PreparingForDownload : PFGameSaveFilesSyncState::PreparingForUpload))
                        {
                            // No fail callback set, so just fail API
                            m_stage = CompareStage::CompareStepFailure;
                            m_failureHR = result.hr;
                            task.ScheduleNow();
                        }
                    }
                    else
                    {
                        DownloadDetailsWrapVector files = result.Payload().GetFiles();
                        latestFinalizedManifest->SetRemoteFileDetails(std::move(files));
                        m_stage = CompareStage::GetExtendedManifest;
                        task.ScheduleNow();
                    }
                });
            }

            return S_OK;
        }

        case CompareStage::GetExtendedManifest:
        {
            String extendedManifestUrl;
            String extendedManifestName = FormatString("extended-%u-manifest.json", static_cast<uint32_t>(latestFinalizedManifest->Version()));
            m_telemetryManager->SetContextSyncSyncErrorSource(SyncErrorSource::SER_GetManifest);

            const DownloadDetailsWrapVector& detailsVec = latestFinalizedManifest->GetRemoteFileDetails();
            for (uint64_t i=0; i< detailsVec.size(); i++)
            {
                const DownloadDetailsWrap& fileDetail = detailsVec[i];
                if (fileDetail.GetFileName() == extendedManifestName) 
                {
                    extendedManifestUrl = fileDetail.GetDownloadUrl();
                    break;
                }
            }

            if (extendedManifestUrl.empty())
            {
                // Not found so skip processing it
                m_stage = CompareStage::ReadLocalManifest;
                task.ScheduleNow();
            }
            else
            {
                String folderPath, manifestFilePath;
                if (GameSaveServiceSelector::useMocks || GetWriteManifestsToDisk())
                {
                    RETURN_IF_FAILED(JoinPathHelper(saveFolder, "cloudsync", folderPath));
                    FilePAL::CreatePath(folderPath);
                    RETURN_IF_FAILED(JoinPathHelper(folderPath, extendedManifestName, manifestFilePath));
                }

                TRACE_TASK("DownloadFileFromCloud");
                GameSaveServiceSelector::DownloadFileFromCloudToBytes(runContext, extendedManifestUrl, manifestFilePath)
                .Finally([this, &task, latestFinalizedManifest, downloading, remoteFileFolderSet, manifestFilePath, &uiCallbackManager, &folderSyncMutex, saveFolder](Result<Vector<char>> result)
                {
                    std::lock_guard<std::recursive_mutex> lock(folderSyncMutex); // Prevent any of the Finally blocks from changing the state while DoWork thread is active
                    TRACE_TASK(FormatString("DownloadFileFromCloudFinally HR:0x%0.8x", result.hr));

                    if (FAILED(result.hr))
                    {
                        m_telemetryManager->SetContextActivationHResult(result.hr);
                        m_telemetryManager->EmitContextActivationFailureEvent();
                        m_stage = CompareStage::WaitForFailedUI_GetExtendedManifest;
                        if( false == uiCallbackManager.ShowSyncFailedUI(task, m_localUser, result.hr, (downloading) ? PFGameSaveFilesSyncState::PreparingForDownload : PFGameSaveFilesSyncState::PreparingForUpload) )
                        {
                            // No fail callback set, so just fail API
                            m_stage = CompareStage::CompareStepFailure;
                            m_failureHR = result.hr;
                            task.ScheduleNow();
                        }
                    }
                    else
                    {
                        const Vector<char>& manifestBytes = result.ExtractPayload();
                        if (GetWriteManifestsToDisk())
                        {
                            WriteEntireFile(manifestFilePath, manifestBytes);
                        }
                        remoteFileFolderSet->InitWithExtendedManifest(manifestBytes, latestFinalizedManifest->GetRemoteFileDetails(), saveFolder);
                        m_stage = CompareStage::ReadLocalManifest;
                        task.ScheduleNow();
                    }
                });
            }

            return S_OK;
        }

        case CompareStage::ReadLocalManifest:
        {
            String shortSaveDescription;
            localFileFolderSet->InitWithLocalFilesAndFolders(saveFolder, &shortSaveDescription);
            bool conflictFound = false;
            MarkFilesToSync(localFileFolderSet, remoteFileFolderSet, saveFolder, conflictFound);

            if (downloading)
            {
                if (conflictFound)
                {
                    m_telemetryManager->SetContextActivationConflictVersion();
                    RETURN_IF_FAILED(HandleConflict(uiCallbackManager, task, latestFinalizedManifest, localFileFolderSet, remoteFileFolderSet, saveFolder, shortSaveDescription));
                }
                else
                {
                    // No conflict so just mark as done
                    m_stage = CompareStage::CompareDone;
                    task.ScheduleNow();
                }
            }
            else // uploading 
            {
                MarkCompressedFilesToKeep(localFileFolderSet, remoteFileFolderSet);
                m_stage = CompareStage::CompareDone;
                task.ScheduleNow();
            }
            
            return S_OK;
        }

        case CompareStage::WaitForConflictUI:
        {
            UIAction uiAction = uiCallbackManager.GetAction();

            if (uiAction == UIAction::UIConflictTakeLocal)
            {
                m_telemetryManager->SetContextActivationConflictResolution(ConflictResolution::CR_KeepLocal);
                m_takeUIChoice = TakeUIChoice::TakeLocal;
                m_stage = CompareStage::ReadLocalManifest;
                task.ScheduleNow();
            }
            else if (uiAction == UIAction::UIConflictTakeRemote)
            {
                m_telemetryManager->SetContextActivationConflictResolution(ConflictResolution::CR_TakeRemote);
                m_takeUIChoice = TakeUIChoice::TakeRemote;
                m_stage = CompareStage::ReadLocalManifest;
                task.ScheduleNow();
            }
            else if (uiAction == UIAction::UIConflictCancel)
            {
                m_telemetryManager->SetContextActivationConflictResolution(ConflictResolution::CR_NoResolutionChosen);
                return E_PF_GAMESAVE_USER_CANCELLED;
            }
            return S_OK;
        }

        case CompareStage::WaitForFailedUI_GetManifestDownloadDetails:
        {
            return uiCallbackManager.HandleFailedUI(task, 
                [this]() { m_stage = CompareStage::GetManifestDownloadDetails; },
                [this]() { m_forceDisconnectFromCloud = true; m_stage = CompareStage::CompareDone; }
            );
        }

        case CompareStage::WaitForFailedUI_GetExtendedManifest:
        {
            return uiCallbackManager.HandleFailedUI(task, 
                [this]() { m_stage = CompareStage::GetExtendedManifest; },
                [this]() { m_forceDisconnectFromCloud = true; m_stage = CompareStage::CompareDone; }
            );
        }

        case CompareStage::CompareStepFailure:
        {
            return m_failureHR;
        }

        case CompareStage::CompareDone:
        {
            assert(false);
            return S_OK;
        }
    }

    return S_OK;
}

HRESULT CompareStep::HandleConflict(
    _In_ UICallbackManager& uiCallbackManager,
    _In_ ISchedulableTask& task,
    _In_ const SharedPtr<ManifestInternal>& latestFinalizedManifest,
    _In_ SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ SharedPtr<FileFolderSet>& remoteFileFolderSet,
    _In_ const String& saveFolder,
    _In_ const String& shortSaveDescription)
{
    switch (m_takeUIChoice)
    {
        case TakeUIChoice::NoChoiceYet:
        {
            m_stage = CompareStage::WaitForConflictUI;

            PFGameSaveDescriptor localGameSave{};
            localGameSave.time = GetTimeTNow();
            localGameSave.totalBytes = 0;
            for (const FileDetail& localFile : localFileFolderSet->GetFiles())
            {
                localGameSave.totalBytes += localFile.fileSizeBytes;
            }
            localGameSave.uploadedBytes = localGameSave.totalBytes;
            String deviceId = GetLocalDeviceID(saveFolder);
            String deviceType = GetDeviceType();
            String deviceFriendlyName = GetDeviceFriendlyName();
            StrCpy(localGameSave.deviceType, sizeof(localGameSave.deviceType), deviceType.c_str());
            StrCpy(localGameSave.deviceId, sizeof(localGameSave.deviceId), deviceId.c_str());
            StrCpy(localGameSave.deviceFriendlyName, sizeof(localGameSave.deviceFriendlyName), deviceFriendlyName.c_str());
            String thumbnailPath;
            const FileDetail* thumbnailFile = localFileFolderSet->GetThumbnail();
            if (thumbnailFile)
            {
                String relFilePath;
                const FolderDetail& folderDetail = localFileFolderSet->GetFileFolder(thumbnailFile);
                RETURN_IF_FAILED(JoinPathHelper(folderDetail.relFolderPath, thumbnailFile->fileName, relFilePath));
                RETURN_IF_FAILED(JoinPathHelper(saveFolder, relFilePath, thumbnailPath));
            }
            StrCpy(localGameSave.thumbnailUri, sizeof(localGameSave.thumbnailUri), thumbnailPath.c_str());
            StrCpy(localGameSave.shortSaveDescription, sizeof(localGameSave.shortSaveDescription), shortSaveDescription.c_str());

            PFGameSaveDescriptor remoteGameSave{};
            FolderSyncManager::ConvertToPFGameSaveDescriptor(latestFinalizedManifest->GetManifest(), remoteGameSave);
            remoteGameSave.totalBytes = remoteFileFolderSet->GetTotalUncompressedSize();
            remoteGameSave.uploadedBytes = remoteGameSave.totalBytes;
            const CompressedFile* remoteThumbnail = remoteFileFolderSet->GetThumbnailFromCompressedList(latestFinalizedManifest->VersionString());
            if (remoteThumbnail)
            {
                StrCpy(remoteGameSave.thumbnailUri, sizeof(remoteGameSave.thumbnailUri), remoteThumbnail->downloadUrl.c_str());
            }

            if (false == uiCallbackManager.ShowConflictUI(task, m_localUser, localGameSave, remoteGameSave))
            {
                // No callback set, so just assume cancel
                m_telemetryManager->SetContextActivationConflictResolution(ConflictResolution::CR_NoResolutionChosen);
                return E_PF_GAMESAVE_USER_CANCELLED;
            }
            break;
        }

        case TakeUIChoice::TakeLocal:
        {
            // Take local mean skip all downloads
            Vector<size_t> noDownloads{};
            remoteFileFolderSet->SetCompressedFilesToDownload(std::move(noDownloads));
            m_stage = CompareStage::CompareDone;
            task.ScheduleNow();
            break;
        }

        case TakeUIChoice::TakeRemote:
        {
            // Take remote mean just download what's needed and overwrite as usual. No special action needed
            m_stage = CompareStage::CompareDone;
            task.ScheduleNow();
            break;
        }
    }

    return S_OK;
}

void CompareStep::Reset()
{
    m_stage = CompareStage::GetManifestDownloadDetails;
    m_takeUIChoice = TakeUIChoice::NoChoiceYet;
    m_forceDisconnectFromCloud = false;
}

bool IsFileQueuedForUpload(const FileDetail* localFile, const SharedPtr<FileFolderSet>& localFileFolderSet)
{
    const Vector<const FileDetail*>& localFilesToUpload = localFileFolderSet->GetFilesToUpload();
    for (const FileDetail* localFileToUpload : localFilesToUpload)
    {
        if (localFileToUpload == localFile)
        {
            return true;
        }
    }

    return false;
}

void CompareStep::MarkFilesToSync(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet,
    _In_ const String& saveFolder,
    _Out_ bool& conflictFound)
{    
    MarkFilesToTransferUponUpload(localFileFolderSet); 
    MarkFilesToTransferUponDownload(localFileFolderSet, remoteFileFolderSet, saveFolder);
    MarkFilesToDeleteUponUpload(localFileFolderSet);
    MarkFilesToDeleteUponDownload(localFileFolderSet, remoteFileFolderSet);
    ScanForConflicts(localFileFolderSet, remoteFileFolderSet, conflictFound);
    MarkFoldersToCreateUponUpload(localFileFolderSet, remoteFileFolderSet);
    MarkFoldersToCreateUponDownload(localFileFolderSet, remoteFileFolderSet);
    MarkFoldersToDeleteUponUpload(localFileFolderSet, remoteFileFolderSet);
    MarkFoldersToDeleteUponDownload(localFileFolderSet, remoteFileFolderSet);
}

void CompareStep::MarkFilesToTransferUponUpload(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet)
{
    const Vector<FileDetail>& localFiles = localFileFolderSet->GetFiles();
    Vector<const FileDetail*> filesToUpload;
    for (const FileDetail& localFile : localFiles)
    {
        // for each local file, see if the size or last write time changed since last time we 
        // uploaded/downloaded the file. if any did, that means it changed locally and that change 
        // needs to be uploaded to cloud
        bool localFileChanged = false;
        bool localFileDeleted = false;
        HasLocalFileChanged(localFile, localFileChanged, localFileDeleted);
        if (localFileChanged)
        {
            filesToUpload.push_back(&localFile);
        }
    }
    TRACE_INFORMATION("[GAME SAVE] CompareStep: FilesToUpload: %zd", filesToUpload.size());
    localFileFolderSet->SetFilesToUpload(std::move(filesToUpload));
}

void CompareStep::MarkFilesToTransferUponDownload(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet, 
    _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet,
    _In_ const String& saveFolder)
{
    const Vector<FileDetail>& remoteFiles = remoteFileFolderSet->GetFiles();
    const Vector<CompressedFile>& compressedFiles = remoteFileFolderSet->GetCompressedFiles();
    Set<size_t> compressedFileIndexSet;
    Set<size_t> changedRemoteFolderIndexSet;
#if _DEBUG
    Vector<const FileDetail*> filesToDownload;
#endif
    for (const FileDetail& remoteFile : remoteFiles)
    {
        // for each remote file, see if remote file's ID or size or last write time changed since 
        // last time we uploaded/downloaded the file.  if any did, that means it changed remotely 
        // and that change needs to be downloaded from cloud

        String relFilePath = remoteFileFolderSet->GetRelFilePath(&remoteFile);
        const FileDetail* localFile = localFileFolderSet->GetFileDetailFromRelFilePath(relFilePath);
        if (localFile)
        {
            if (HasRemoteFileChanged(*localFile, remoteFile))
            {
                compressedFileIndexSet.insert(remoteFile.compressedFileIndex); // Set<> doesn't allow duplicates
                changedRemoteFolderIndexSet.insert(remoteFile.folderIndex); // Set<> doesn't allow duplicates.

                ArchiveFileDetail afd{};
                JoinPathHelper(saveFolder, relFilePath, afd.fullPath);
                afd.uncompressedSize = remoteFile.fileSizeBytes;
                afd.timeLastModified = remoteFile.timeLastModified;
                afd.timeCreated = remoteFile.timeCreated;
                compressedFiles[remoteFile.compressedFileIndex].archiveContext->AddFile(relFilePath, std::move(afd));
#if _DEBUG
                filesToDownload.push_back(&remoteFile);
#endif
            }
        }
        else
        {
            // no local file, so need to download it
            compressedFileIndexSet.insert(remoteFile.compressedFileIndex); // Set<> doesn't allow duplicates
            changedRemoteFolderIndexSet.insert(remoteFile.folderIndex); // Set<> doesn't allow duplicates.

            ArchiveFileDetail afd{};
            JoinPathHelper(saveFolder, relFilePath, afd.fullPath);
            afd.uncompressedSize = remoteFile.fileSizeBytes;
            afd.timeLastModified = remoteFile.timeLastModified;
            afd.timeCreated = remoteFile.timeCreated;
            compressedFiles[remoteFile.compressedFileIndex].archiveContext->AddFile(relFilePath, std::move(afd));
#if _DEBUG
            filesToDownload.push_back(&remoteFile);
#endif
        }
    }

    Vector<size_t> compressedFilesToDownload;
    for (auto it = compressedFileIndexSet.begin(); it != compressedFileIndexSet.end(); it++)
    {
        compressedFilesToDownload.push_back(*it);
    }
    TRACE_INFORMATION("[GAME SAVE] CompareStep: CompressedFilesToDownload: %zd", compressedFilesToDownload.size());
    remoteFileFolderSet->SetCompressedFilesToDownload(std::move(compressedFilesToDownload));

    Vector<size_t> changedRemoteFolderIndexes;
    for (auto it = changedRemoteFolderIndexSet.begin(); it != changedRemoteFolderIndexSet.end(); it++)
    {
        changedRemoteFolderIndexes.push_back(*it);
    }
    TRACE_INFORMATION("[GAME SAVE] CompareStep: ChangedRemoteFolderIndexSet %zd", changedRemoteFolderIndexes.size());
    remoteFileFolderSet->SetChangedRemoteFolderIndexSet(std::move(changedRemoteFolderIndexes));

#if _DEBUG
    remoteFileFolderSet->SetFilesToDownload(std::move(filesToDownload));  // used only for debug stats
#endif
}


String GetTopLevelFolder(const String& relFolderPath)
{
    if (relFolderPath.length() > 1)
    {
        const char* firstSep = strchr(
            relFolderPath.c_str() + 1, // +1 to skip the leading path separator
            FilePAL::GetPathSeparatorChar());

        if (firstSep)
        {
            // found a separator, so return just the top level folder
            size_t firstSepIndex = firstSep - relFolderPath.c_str();
            return relFolderPath.substr(0, firstSepIndex);
        }
    }

    return relFolderPath;
}

void CompareStep::ScanForConflicts(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet,
    _Out_ bool& conflictFound)
{
    conflictFound = false;

    Set<String> topLevelFoldersNeedingDownload;

    // For each remote folder that has changes, get the top level folder and mark it as needs download
    const PlayFab::Vector<size_t>& changedRemoteFolderIndexes = remoteFileFolderSet->GetChangedRemoteFolderIndexSet();
    const PlayFab::Vector<PlayFab::GameSave::FolderDetail>& remoteFolders = remoteFileFolderSet->GetFolders();
    for (size_t changedRemoteFolderIndex : changedRemoteFolderIndexes)
    {
        const FolderDetail& folder = remoteFolders[changedRemoteFolderIndex];
        String topLevelFolder = GetTopLevelFolder(folder.relFolderPath);

        topLevelFoldersNeedingDownload.insert(topLevelFolder.c_str());
    }

    // For each local file that needs upload, check if its top level folder needs download.  If so, conflict found
    const PlayFab::Vector<const FileDetail*>& localFilesToUpload = localFileFolderSet->GetFilesToUpload();
    for (const FileDetail* localFile : localFilesToUpload)
    {
        ScanForConflictHelper(localFileFolderSet, localFile, topLevelFoldersNeedingDownload, conflictFound);
    }
    const PlayFab::Vector<const FileDetail*>& localFilesToDeleteUponUpload = localFileFolderSet->GetFilesToDeleteUponUpload();
    for (const FileDetail* localFile : localFilesToDeleteUponUpload)
    {
        ScanForConflictHelper(localFileFolderSet, localFile, topLevelFoldersNeedingDownload, conflictFound);
    }
}

void CompareStep::ScanForConflictHelper(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const FileDetail* localFileToUpload,
    _In_ const Set<String>& topLevelFoldersNeedingDownload, 
    _Out_ bool& conflictFound
    )
{
    const FolderDetail& folder = localFileFolderSet->GetFileFolder(localFileToUpload);
    String topLevelFolder = GetTopLevelFolder(folder.relFolderPath);
    conflictFound = false;

    auto iter = std::find_if(
        topLevelFoldersNeedingDownload.begin(),
        topLevelFoldersNeedingDownload.end(),
        [&topLevelFolder](const String& str)
        {
            return (str.compare(topLevelFolder.c_str()) == 0);
        });
    if (iter != topLevelFoldersNeedingDownload.end())
    {
        conflictFound = true;
        return;
    }
}

void CompareStep::MarkFilesToDeleteUponUpload(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet)
{
    const Vector<FileDetail>& localFiles = localFileFolderSet->GetFiles();
    Vector<const FileDetail*> filesToDeleteUponUpload;
    for (const FileDetail& localFile : localFiles)
    {
        // If there's last sync data but no local file, then local file was deleted
        if (localFile.lastSyncFileSize != 0 && localFile.fileSizeBytes == 0)
        {
            TRACE_INFORMATION("[GAME SAVE] CompareStep: FilesToDeleteUponUpload file:%s lastSyncFileSize:%llu fileSizeBytes:%llu", localFile.fileName.c_str(), localFile.lastSyncFileSize, localFile.fileSizeBytes);
            filesToDeleteUponUpload.push_back(&localFile);
        }
    }

    TRACE_INFORMATION("[GAME SAVE] CompareStep: FilesToDeleteUponUpload: %zd", filesToDeleteUponUpload.size());
    localFileFolderSet->SetFilesToDeleteUponUpload(std::move(filesToDeleteUponUpload));
}

void CompareStep::MarkFilesToDeleteUponDownload(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet)
{
    // If there's last sync data but no remote file, then remote file was deleted
    Vector<const FileDetail*> filesToDeleteUponDownload;
    const Vector<FileDetail>& localFiles = localFileFolderSet->GetFiles();
    for (const FileDetail& localFile : localFiles)
    {
        String relFilePath = localFileFolderSet->GetRelFilePath(&localFile);
        const FileDetail* remoteFile = remoteFileFolderSet->GetFileDetailFromRelFilePath(relFilePath);

        TRACE_INFORMATION("[GAME SAVE] CompareStep: FilesToDeleteUponDownload: file:%s lastSyncFileSize:%llu", localFile.fileName.c_str(), localFile.lastSyncFileSize);
        if (localFile.lastSyncFileSize != 0 &&
            remoteFile == nullptr)
        {
            filesToDeleteUponDownload.push_back(&localFile);
        }
    }

    TRACE_INFORMATION("[GAME SAVE] CompareStep: FilesToDeleteUponDownload: %zd", filesToDeleteUponDownload.size());
    localFileFolderSet->SetFilesToDeleteUponDownload(std::move(filesToDeleteUponDownload));
}

void CompareStep::MarkFoldersToCreateUponUpload(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet
)
{
    Vector<const FolderDetail*> foldersToCreateUponUpload;
    const Vector<FolderDetail>& localFolders = localFileFolderSet->GetFolders();
    for (const FolderDetail& localFolder : localFolders)
    {
        if (localFolder.relFolderPath.length() == 0)
        {
            continue; // skip the root folder
        }

        const FolderDetail* remoteFolder = remoteFileFolderSet->GetFolderDetailFromRelFilePath(localFolder.relFolderPath);
        if (remoteFolder == nullptr)
        {
            foldersToCreateUponUpload.push_back(&localFolder);
        }
    }
    TRACE_INFORMATION("[GAME SAVE] CompareStep: FoldersToCreateUponUpload: %zd", foldersToCreateUponUpload.size());
    localFileFolderSet->SetFoldersToCreateUponUpload(std::move(foldersToCreateUponUpload));
}

void CompareStep::MarkFoldersToCreateUponDownload(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet
)
{
    Vector<const FolderDetail*> foldersToCreateUponDownload;
    const Vector<FolderDetail>& remoteFolders = remoteFileFolderSet->GetFolders();
    for (const FolderDetail& remoteFolder : remoteFolders)
    {
        if (remoteFolder.relFolderPath.length() == 0)
        {
            continue; // skip the root folder
        }

        const FolderDetail* localFolder = localFileFolderSet->GetFolderDetailFromRelFilePath(remoteFolder.relFolderPath);
        if (localFolder == nullptr)
        {
            foldersToCreateUponDownload.push_back(&remoteFolder);
        }
    }
    TRACE_INFORMATION("[GAME SAVE] CompareStep: FoldersToCreateUponDownload: %zd", foldersToCreateUponDownload.size());
    remoteFileFolderSet->SetFoldersToCreateUponDownload(std::move(foldersToCreateUponDownload));
}

void CompareStep::MarkFoldersToDeleteUponUpload(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet)
{
    Vector<const FolderDetail*> foldersToDeleteUponUpload;
    const Vector<FolderDetail>& remoteFolders = remoteFileFolderSet->GetFolders();
    for (const FolderDetail& remoteFolder : remoteFolders)
    {
        if (remoteFolder.relFolderPath.length() == 0)
        {
            continue; // skip the root folder
        }

        const FolderDetail* localFolder = localFileFolderSet->GetFolderDetailFromRelFilePath(remoteFolder.relFolderPath);
        if (localFolder == nullptr)
        {
            TRACE_INFORMATION("[GAME SAVE] CompareStep: MarkFoldersToDeleteUponUpload %s: added. no localFolder", remoteFolder.relFolderPath.c_str());
            foldersToDeleteUponUpload.push_back(&remoteFolder);
        }
        else
        {
            TRACE_INFORMATION("[GAME SAVE] CompareStep: MarkFoldersToDeleteUponUpload %s: existsLocally: %d", remoteFolder.relFolderPath.c_str());
            if (!localFolder->existsLocally)
            {
                foldersToDeleteUponUpload.push_back(&remoteFolder);
            }
        }

    }
    TRACE_INFORMATION("[GAME SAVE] CompareStep: FoldersToDeleteUponUpload: %zd", foldersToDeleteUponUpload.size());
    remoteFileFolderSet->SetFoldersToDeleteUponUpload(std::move(foldersToDeleteUponUpload));
}

void CompareStep::MarkFoldersToDeleteUponDownload(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet)
{
    Vector<const FolderDetail*> foldersToDeleteUponDownload;
    const Vector<FolderDetail>& localFolders = localFileFolderSet->GetFolders();
    for (const FolderDetail& localFolder : localFolders)
    {
        if (localFolder.relFolderPath.length() == 0)
        {
            continue; // skip the root folder
        }

        const FolderDetail* remoteFolder = remoteFileFolderSet->GetFolderDetailFromRelFilePath(localFolder.relFolderPath);
        
        TRACE_INFORMATION("[GAME SAVE] CompareStep: MarkFoldersToDeleteUponDownload %s: remoteFolder:%d localFolder.hasLastSync:%d", localFolder.relFolderPath.c_str(), remoteFolder != nullptr, localFolder.hasLastSync);
        if (remoteFolder == nullptr && localFolder.hasLastSync)
        {
            foldersToDeleteUponDownload.push_back(&localFolder);
        }
    }
    TRACE_INFORMATION("[GAME SAVE] CompareStep: FoldersToDeleteUponDownload: %zd", foldersToDeleteUponDownload.size());
    localFileFolderSet->SetFoldersToDeleteUponDownload(std::move(foldersToDeleteUponDownload));
}

void CompareStep::HasLocalFileChanged(
    _In_ const FileDetail& localFile, 
    _Out_ bool& localFileChanged, 
    _Out_ bool& localFileDeleted)
{
    if (localFile.fileSizeBytes > 0)
    {
        localFileChanged =
            (localFile.lastSyncFileSize != localFile.fileSizeBytes ||
             localFile.lastSyncTimeLastModified != localFile.timeLastModified);
        localFileDeleted = false;
    }
    else
    {
        localFileDeleted = true;
        localFileChanged = false; // don't count deleted files as changed
    }
}

bool CompareStep::HasRemoteFileChanged(_In_ const FileDetail& localFile, _In_ const FileDetail& remoteFile)
{
    bool hasRemoteFileChanged =
        (localFile.lastSyncFileSize != remoteFile.fileSizeBytes ||
         localFile.lastSyncTimeLastModified != remoteFile.timeLastModified);

    return hasRemoteFileChanged;
}

void CompareStep::MarkCompressedFilesToKeep(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet)
{
    const Vector<FileDetail>& remoteFiles = remoteFileFolderSet->GetFiles();

    Set<size_t> compressedFileIndexSet;
    for (const FileDetail& remoteFile : remoteFiles)
    {
        String relFilePath = remoteFileFolderSet->GetRelFilePath(&remoteFile);

        bool hasExtractedFileChangedLocally = false;
        bool hasExtractedFileDeletedLocally = false;
        const FileDetail* localFile = localFileFolderSet->GetFileDetailFromRelFilePath(relFilePath);
        if (localFile)
        {
            HasLocalFileChanged(*localFile, hasExtractedFileChangedLocally, hasExtractedFileDeletedLocally);
        }

        if (hasExtractedFileChangedLocally || hasExtractedFileDeletedLocally || localFile == nullptr)
        {
            // Mark old extracted files that will be uploaded as "skip" since they won't be needed in upcoming manifest
            remoteFile.skipFile = true;
            localFileFolderSet->AddSkippedFile(remoteFile); // recording just for logging purposes
        }
        else
        {
            // Otherwise, mark that we need found an extracted file needed in this compressed file so we need to keep it
            compressedFileIndexSet.insert(remoteFile.compressedFileIndex); // Set<> doesn't allow duplicates
        }
    }

    Vector<size_t> compressedFilesToKeep;
    for (auto it = compressedFileIndexSet.begin(); it != compressedFileIndexSet.end(); it++)
    {
        compressedFilesToKeep.push_back(*it);
    }
    TRACE_INFORMATION("[GAME SAVE] CompareStep: CompressedFilesToKeep: %zd", compressedFilesToKeep.size());
    remoteFileFolderSet->SetCompressedFilesToKeep(std::move(compressedFilesToKeep));
}

} // namespace GameSave
} // namespace PlayFab

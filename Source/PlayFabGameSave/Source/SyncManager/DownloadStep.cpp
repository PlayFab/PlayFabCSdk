// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "LocalStateManifest.h"
#include "DownloadStep.h"
#include "ZipUtils.h"

using namespace PlayFab::GameSaveWrapper;

namespace PlayFab
{
namespace GameSave
{

DownloadStep::DownloadStep(_In_ LocalUser const& localUser, _In_ SharedPtr<GameSaveTelemetryManager> telemetryManager) :
    m_localUser{ localUser },
    m_telemetryManager{ telemetryManager }
{
}

void DownloadStep::SetEntity(_In_ const Entity& entity)
{
    m_entity = entity;
}

void DownloadStep::Reset()
{
    m_stage = DownloadStage::DownloadStart;
    m_forceDisconnectFromCloud = false;
    m_isLocalOperationsDone = false;
    m_totalUncompressedSizeBytes = 0;
    m_currentUncompressedSizeBytes = 0;
}

bool DownloadStep::IsDownloadDone() const
{
    return m_stage == DownloadStage::DownloadDone;
}

bool DownloadStep::IsLocalOperationsDone() const
{
    return m_isLocalOperationsDone;
}

void DownloadStep::SetLocalOperationsDone(bool state)
{
    m_isLocalOperationsDone = state;
}

HRESULT DownloadStep::UncompressFile(
    _In_ const CompressedFile& remoteFile, 
    _In_ const String& saveFolder, 
    _In_ const String& fullCompressedFilePath, 
    _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet)
{
    switch (remoteFile.compression)
    {
        case CompressionType::None:
        case CompressionType::GZip: // should be handled at HTTP layer
        {
            const Vector<FileDetail>& files = remoteFileFolderSet->GetFiles();
            int numExtractedFiles = 0;
            for (const FileDetail& extractedFile : files)
            {
                if (extractedFile.compressedFileIndex == remoteFile.compressedFileIndex)
                {
                    String fullExtractedFolderPath, fullExtractedFilePath;
                    RETURN_IF_FAILED(JoinPathHelper(saveFolder, remoteFileFolderSet->GetFileFolder(&extractedFile).relFolderPath, fullExtractedFolderPath));
                    RETURN_IF_FAILED(FilePAL::CreatePath(fullExtractedFolderPath));
                    RETURN_IF_FAILED(JoinPathHelper(fullExtractedFolderPath, extractedFile.fileName, fullExtractedFilePath));
                    TRACE_INFORMATION("[GAME SAVE] DownloadStep: Move to %s", fullExtractedFilePath.c_str());
                    RETURN_IF_FAILED(FilePAL::MoveLocalFile(fullCompressedFilePath, fullExtractedFilePath));
                    RETURN_IF_FAILED(FilePAL::SetFileLastModifiedTime(fullExtractedFilePath, extractedFile.timeCreated, extractedFile.timeLastModified));

                    numExtractedFiles++;
                }
            }
            assert(numExtractedFiles == 1); // should be 1 extracted file in CompressionType::None file
            break;
        }

        case CompressionType::Zip:
            RETURN_IF_FAILED(ZipUtils::UnzipFilesFromSingleZip(remoteFile.archiveContext, fullCompressedFilePath));
            m_telemetryManager->AddContextSyncFileCount(static_cast<uint32_t>(remoteFile.archiveContext->GetFiles().size()));
            // Ensure the zip file is deleted after it has been unzipped
            TRACE_INFORMATION("[GAME SAVE] DownloadStep: Deleting zip file after extraction: %s", fullCompressedFilePath.c_str());
            RETURN_IF_FAILED(FilePAL::DeleteLocalFile(fullCompressedFilePath));
            break;
    }

    return S_OK;
}

HRESULT DownloadStep::Download(
    _In_ const RunContext& runContext,
    _In_ ISchedulableTask& task,
    _In_ const String& saveFolder,
    _In_ UICallbackManager& uiCallbackManager,
    _In_ SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ SharedPtr<FileFolderSet>& remoteFileFolderSet,
    _In_ FolderSyncManagerProgress& syncProgress,
    _In_ std::recursive_mutex& folderSyncMutex,
    _In_ ProgressCallback progressCallback,
    _In_ void* progressCallbackContext,
    _In_ const String& shortSaveDescription
    )
{
    ScopeTracer scopeTracer(FormatString("DownloadStep - %s", EnumName(m_stage)));

    switch (m_stage)
    {
        case DownloadStage::DownloadStart:
        {
            m_telemetryManager->SetContextSyncStartTime();
            m_telemetryManager->SetContextSyncSyncDownload(true);
            m_telemetryManager->SetContextSyncSyncErrorSource(SyncErrorSource::SER_DownloadData);
            syncProgress.syncState = PFGameSaveFilesSyncState::Downloading;
            uiCallbackManager.ShowProgressUI(task, m_localUser, PFGameSaveFilesSyncState::Downloading); // no issue if callback not set
            this->m_stage = DownloadStage::QueryStorage;
            task.ScheduleNow();
            break;
        }

        case DownloadStage::QueryStorage:
        {
            // Create cloudsync folder if it doesn't exist
            String cloudSyncFolder;
            RETURN_IF_FAILED(JoinPathHelper(saveFolder, "cloudsync", cloudSyncFolder));
            RETURN_IF_FAILED(FilePAL::CreatePath(cloudSyncFolder));
            
            // Clean up any existing zip files in the cloudsync folder
            Result<Vector<String>> existingFilesResult = FilePAL::EnumFiles(cloudSyncFolder);
            if (SUCCEEDED(existingFilesResult.hr))
            {
                Vector<String> existingFiles = existingFilesResult.ExtractPayload();
                for (const String& fileName : existingFiles)
                {
                    if (fileName.find(".zip") != String::npos)
                    {
                        String fullFilePath;
                        RETURN_IF_FAILED(JoinPathHelper(cloudSyncFolder, fileName, fullFilePath));
                        TRACE_INFORMATION("[GAME SAVE] DownloadStep: Cleaning up old zip file %s", fullFilePath.c_str());

                        HRESULT deleteResult = FilePAL::DeleteLocalFile(fullFilePath);
                        if (FAILED(deleteResult))
                        {
                            TRACE_ERROR("[GAME SAVE] UploadStep: Failed to delete old zip file %s, HR:0x%0.8x", fullFilePath.c_str(), deleteResult);
                            // don't bother failing here, just log the error
                        }
                    }
                }
            }
            
            m_totalUncompressedSizeBytes = 0;
            const Vector<size_t>& remoteFileIndexToDownload = remoteFileFolderSet->GetCompressedFilesToDownload();
            const Vector<CompressedFile>& compressedFiles = remoteFileFolderSet->GetCompressedFiles();
            for (int iRemoteFile = 0; iRemoteFile < remoteFileIndexToDownload.size(); iRemoteFile++)
            {
                size_t index = remoteFileIndexToDownload[iRemoteFile];
                const CompressedFile& remoteCompressedFile = compressedFiles[index];

                if (!remoteCompressedFile.hasDownloadedLocally)
                {
                    m_totalUncompressedSizeBytes += remoteCompressedFile.uncompressedSizeBytes;
                }
            }

            m_telemetryManager->SetContextSyncTotalSize(m_totalUncompressedSizeBytes);

            Result<uint64_t> availableStorageResult = FilePAL::GetAvailableStorageSize(saveFolder);
            RETURN_IF_FAILED(availableStorageResult.hr);

            if (GetForceOutOfStorageError() ||
                availableStorageResult.Payload() < m_totalUncompressedSizeBytes)
            {
                this->m_stage = DownloadStage::WaitForOutOfStorageUI;
                if (false == uiCallbackManager.ShowOutOfStorageUI(task, m_localUser, m_totalUncompressedSizeBytes))
                {
                    // No fail callback set, so just fail API
                    m_stage = DownloadStage::LockStepFailure;
                    m_failureHR = E_PF_GAMESAVE_DISK_FULL;
                    task.ScheduleNow();
                }
            }
            else
            {
                this->m_stage = DownloadStage::Download;
                progressCallback(PFGameSaveFilesSyncState::Downloading, 0, m_totalUncompressedSizeBytes, progressCallbackContext);
                task.ScheduleNow();
            }

            break;
        }

        case DownloadStage::Download:
        {
            // TODO: Task 50948316: PFGameSave: hook up cancel from progression dialog
            bool downloadNeeded = false;
            const Vector<size_t>& remoteFileIndexToDownload = remoteFileFolderSet->GetCompressedFilesToDownload();
            const Vector<CompressedFile>& compressedFiles = remoteFileFolderSet->GetCompressedFiles();
            TRACE_INFORMATION("[GAME SAVE] DownloadStep: Files to download %llu", remoteFileIndexToDownload.size());
            for (int iRemoteFile = 0; iRemoteFile < remoteFileIndexToDownload.size(); iRemoteFile++)
            {
                size_t index = remoteFileIndexToDownload[iRemoteFile];
                const CompressedFile& remoteCompressedFile = compressedFiles[index];
                m_telemetryManager->SetContextSyncBlockName(remoteCompressedFile.fileName);

                if (!remoteCompressedFile.hasDownloadedLocally)
                {
                    downloadNeeded = true;

                    TRACE_TASK("DownloadFileFromCloud");
                    String cloudSyncFolder, filePath;
                    RETURN_IF_FAILED(JoinPathHelper(saveFolder, "cloudsync", cloudSyncFolder));
                    RETURN_IF_FAILED(JoinPathHelper(cloudSyncFolder, remoteCompressedFile.fileName, filePath));
                    assert(!remoteCompressedFile.downloadUrl.empty());

                    TRACE_INFORMATION("[GAME SAVE] DownloadStep: Downloading %s to %s", remoteCompressedFile.fileName.c_str(), filePath.c_str());
                    // Don't include archive context to force download to local file in case of download errors
                    DownloadFileDetail downloadDetail{};
                    downloadDetail.fullFilePath = filePath;

                    auto innerProgressContext = MakeShared<InnerProgressContext>(progressCallback, progressCallbackContext, task, m_localUser, PFGameSaveFilesSyncState::Downloading);
                    
                    GameSaveServiceSelector::DownloadFileFromCloud(runContext, downloadDetail, remoteCompressedFile.downloadUrl, InnerProgressCallback, innerProgressContext.get(), m_totalUncompressedSizeBytes, m_currentUncompressedSizeBytes)
                    .Finally([this, &task, &remoteCompressedFile, filePath, saveFolder, remoteFileFolderSet, &uiCallbackManager, &folderSyncMutex, innerProgressContext](Result<void> result)
                    {
                        std::lock_guard<std::recursive_mutex> lock(folderSyncMutex); // Prevent any of the Finally blocks from changing the state while DoWork thread is active
                        TRACE_TASK(FormatString("DownloadFileFromCloudFinally HR:0x%0.8x", result.hr));
                        m_telemetryManager->SetContextSyncHttpInfo(result.httpResult);

                        if (GetForceSyncFailedError() || FAILED(result.hr))
                        {
                            m_telemetryManager->SetContextSyncHResult(result.hr);
                            m_telemetryManager->EmitContextSyncErrorEvent();
                            this->m_stage = DownloadStage::WaitForFailedUI_Download;
                            if (false == uiCallbackManager.ShowSyncFailedUI(task, m_localUser, result.hr, PFGameSaveFilesSyncState::Downloading))
                            {
                                // No fail callback set, so just fail API
                                m_stage = DownloadStage::LockStepFailure;
                                m_failureHR = result.hr;
                                task.ScheduleNow();
                            }
                        }
                        else
                        {
                            if (remoteCompressedFile.archiveContext)
                            {
                                m_currentUncompressedSizeBytes += remoteCompressedFile.uncompressedSizeBytes;
                                innerProgressContext->callback(PFGameSaveFilesSyncState::Downloading, m_currentUncompressedSizeBytes, m_totalUncompressedSizeBytes, innerProgressContext->callbackContext);
                            }

                            remoteCompressedFile.hasDownloadedLocally = true; // loop until nothing left to download
                            task.ScheduleNow();
                        }
                    });
                }

                if (downloadNeeded)
                {
                    break;
                }
            }            
            
            if (!downloadNeeded)
            {
                this->m_stage = DownloadStage::UncompressFiles;
                task.ScheduleNow();
            }

            return S_OK;
        }        
        
        case DownloadStage::UncompressFiles:
        {
            const Vector<size_t>& remoteFileIndexToDownload = remoteFileFolderSet->GetCompressedFilesToDownload();
            const Vector<CompressedFile>& compressedFiles = remoteFileFolderSet->GetCompressedFiles();
            TRACE_INFORMATION("[GAME SAVE] DownloadStep: Uncompressing %llu downloaded files", remoteFileIndexToDownload.size());
            
            for (int iRemoteFile = 0; iRemoteFile < remoteFileIndexToDownload.size(); iRemoteFile++)
            {
                size_t index = remoteFileIndexToDownload[iRemoteFile];
                const CompressedFile& remoteCompressedFile = compressedFiles[index];
                
                if (remoteCompressedFile.hasDownloadedLocally)
                {
                    m_telemetryManager->IncrementContextSyncBlockCount();
                    m_telemetryManager->AddContextSyncSyncSize(remoteCompressedFile.compressedSizeBytes);
                    m_telemetryManager->AddContextSyncOriginalSize(remoteCompressedFile.uncompressedSizeBytes);

                    String cloudSyncFolder, filePath;
                    RETURN_IF_FAILED(JoinPathHelper(saveFolder, "cloudsync", cloudSyncFolder));
                    RETURN_IF_FAILED(JoinPathHelper(cloudSyncFolder, remoteCompressedFile.fileName, filePath));
                    
                    TRACE_INFORMATION("[GAME SAVE] DownloadStep: Uncompressing %s", remoteCompressedFile.fileName.c_str());
                    HRESULT hr = UncompressFile(remoteCompressedFile, saveFolder, filePath, remoteFileFolderSet);
                    if (FAILED(hr))
                    {
                        m_telemetryManager->SetContextSyncHResult(hr);
                        m_telemetryManager->EmitContextSyncErrorEvent();
                        this->m_failureHR = hr;
                        this->m_stage = DownloadStage::LockStepFailure;
                        task.ScheduleNow();
                        return S_OK;
                    }
                }
            }
            
            this->m_stage = DownloadStage::UpdateLocalManifest;
            task.ScheduleNow();
            return S_OK;
        }
        
        case DownloadStage::UpdateLocalManifest:
        {
            localFileFolderSet->InitWithLocalFilesAndFolders(saveFolder, nullptr);

            const Vector<size_t>& remoteFileIndexToDownload = remoteFileFolderSet->GetCompressedFilesToDownload();
            const Vector<CompressedFile>& compressedFiles = remoteFileFolderSet->GetCompressedFiles();
            for (int iRemoteFile = 0; iRemoteFile < remoteFileIndexToDownload.size(); iRemoteFile++)
            {
                size_t index = remoteFileIndexToDownload[iRemoteFile];
                const CompressedFile& remoteCompressedFile = compressedFiles[index];

                if (remoteCompressedFile.hasDownloadedLocally)
                {
                    const Vector<FileDetail>& remoteFiles = remoteFileFolderSet->GetFiles();
                    for (const FileDetail& remoteFile : remoteFiles)
                    {
                        if (remoteFile.compressedFileIndex == remoteCompressedFile.compressedFileIndex)
                        {
                            String relFilePath = remoteFileFolderSet->GetRelFilePath(&remoteFile);
                            localFileFolderSet->UpdateFilesWithDownloadData(remoteFile, relFilePath);
                        }
                    }
                }
            }
            LocalStateManifest::WriteLocalManifest(saveFolder, localFileFolderSet, shortSaveDescription);
            this->m_stage = DownloadStage::DownloadDone;
            task.ScheduleNow();
            return S_OK;
        }

        case DownloadStage::WaitForFailedUI_Download:
        {
            // TODO: what exactly to do if download fails? What about UISyncFailedUseOffline here in UiActionHelper()
            return uiCallbackManager.HandleFailedUI(task,
                [this]() { m_stage = DownloadStage::Download; },
                [this]() { m_stage = DownloadStage::DownloadDone; }
            );
        }

        case DownloadStage::WaitForOutOfStorageUI:
        {
            TRACE_TASK("WaitForOutOfStorageUI");
            UIAction uiAction = uiCallbackManager.GetAction();

            if (uiAction == UIAction::UIOutOfStorageSpaceCleared)
            {
                m_stage = DownloadStage::Download;
                task.ScheduleNow();
            }
            else if (uiAction == UIAction::UIOutOfStorageCancel)
            {
                return E_PF_GAMESAVE_USER_CANCELLED;
            }

            return S_OK;
        }

        case DownloadStage::LockStepFailure:
        {
            return m_failureHR;
        }

        case DownloadStage::DownloadDone:
        {
            assert(false);
            return S_OK;
        }
    }

    return S_OK;
}

HRESULT DownloadStep::DeleteFiles(
    _In_ const String& saveFolder,
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet
    )
{
    const PlayFab::Vector<const FileDetail*>& filesToDeleteUponDownload = localFileFolderSet->GetFilesToDeleteUponDownload();
    for (const FileDetail* fileToDeleteUponDownload : filesToDeleteUponDownload)
    {
        String relPath = localFileFolderSet->GetRelFilePath(fileToDeleteUponDownload);
        String fullFilePath;
        RETURN_IF_FAILED(JoinPathHelper(saveFolder, relPath, fullFilePath));
        TRACE_INFORMATION("[GAME SAVE] Delete file %s", fullFilePath.c_str());
        FilePAL::DeleteLocalFile(fullFilePath);
    }
    return S_OK;
}

HRESULT DownloadStep::DeleteFolders(
    _In_ const String& saveFolder,
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet
    )
{
    const PlayFab::Vector<const FolderDetail*>& foldersToDeleteUponDownload = localFileFolderSet->GetFoldersToDeleteUponDownload();
    for (const FolderDetail* folderToDeleteUponDownload : foldersToDeleteUponDownload)
    {
        String fullFolderPath;
        RETURN_IF_FAILED(JoinPathHelper(saveFolder, folderToDeleteUponDownload->relFolderPath, fullFolderPath));
        TRACE_INFORMATION("[GAME SAVE] Delete folder %s", fullFolderPath.c_str());
        FilePAL::DeletePath(fullFolderPath);
    }

    return S_OK;
}

HRESULT DownloadStep::CreateEmptyFolders(_In_ const String& saveFolder, _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet)
{
    const PlayFab::Vector<const FolderDetail*>& foldersToCreateUponDownload = remoteFileFolderSet->GetFoldersToCreateUponDownload();   
    for (const FolderDetail* folderToCreateUponDownload : foldersToCreateUponDownload)
    {
        String fullFilePath;
        RETURN_IF_FAILED(JoinPathHelper(saveFolder, folderToCreateUponDownload->relFolderPath, fullFilePath));
        TRACE_INFORMATION("[GAME SAVE] Create empty folder %s", fullFilePath.c_str());
        FilePAL::CreatePath(fullFilePath);
    }
    return S_OK;
}

} // namespace GameSave
} // namespace PlayFab

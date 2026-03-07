// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "LocalStateManifest.h"
#include "UploadStep.h"
#include "ApiHelpers.h"
#include "ProgressHelpers.h"
#include "ZipUtils.h"

using namespace PlayFab::GameSaveWrapper;

namespace PlayFab
{
namespace GameSave
{
    
UploadStep::UploadStep(_In_ LocalUser const& localUser, _In_ SharedPtr<GameSaveTelemetryManager> telemetryManager) :
    m_localUser{ localUser },
    m_telemetryManager{ telemetryManager }
{
}

void UploadStep::SetEntity(_In_ const Entity& entity)
{
    m_entity = entity;
}

void UploadStep::Reset()
{
    m_compressedFilesToUploadCurIndex = 0;
    m_compressedFilesToUpload = {};
    m_initiateResult = {};
    m_stage = UploadStage::UploadStart;
    m_deleteManifestStage = DeleteManifestStage::DeleteManifestStart;
    m_hasStartedFinalizeManifest = false; // reset finalize tracking

    m_totalUncompressedSizeBytes = 0;
    m_currentUncompressedSizeBytes = 0;
    m_totalCompressedSizeBytes = 0;
    m_manifests.clear();
    m_postUploadPendingPFManifest = ManifestWrap();
    m_postUploadLatestFinalizedPFManifest = ManifestWrap();

#if _DEBUG // just for debug stats
    m_numFilesInFinalizedManifest = 0;
#endif
}

const ManifestWrap& UploadStep::GetPostUploadPendingPFManifest() const
{
    return m_postUploadPendingPFManifest;
}

const ManifestWrap& UploadStep::GetPostUploadLatestFinalizedPFManifest() const
{
    return m_postUploadLatestFinalizedPFManifest;
}

uint64_t UploadStep::GetTotalCompressedSizeBytesUploaded() const
{
    return m_totalCompressedSizeBytes;
}

bool UploadStep::IsUploadDone() const
{
    return m_stage == UploadStage::UploadDone;
}

bool UploadStep::IsDeletePendingManifestDone() const
{
    return m_deleteManifestStage == DeleteManifestStage::DeleteDone;
}

const FileDetail* UploadStep::PopThumbnail(
    _In_ Vector<const FileDetail*>& filesToUpload)
{
    for (auto it = filesToUpload.begin(); it != filesToUpload.end(); it++)
    {
        const FileDetail* curFileDetail = *it;
        if (curFileDetail->isThumbnail)
        {
            // Remove element from vector, and return element
            if (filesToUpload.size() > 1)
            {
                std::iter_swap(it, filesToUpload.end() - 1);
                filesToUpload.pop_back();
            }
            else
            {
                filesToUpload.clear();
            }
            return curFileDetail;
        }
    }

    return nullptr;
}

Result<Vector<ExtendedManifestCompressedFileDetail>> UploadStep::CompressFiles(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const String& saveFolder,
    _In_ const String& version)
{
    Vector<const FileDetail*> filesToUpload = localFileFolderSet->GetFilesToUpload();
    const FileDetail* thumbnail = PopThumbnail(filesToUpload);    
    
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
                TRACE_INFORMATION("[GAME SAVE] UploadStep: Cleaning up old zip file %s", fullFilePath.c_str());

                HRESULT deleteResult = FilePAL::DeleteLocalFile(fullFilePath);
                if (FAILED(deleteResult))
                {
                    TRACE_ERROR("[GAME SAVE] UploadStep: Failed to delete zip file %s, HR:0x%0.8x", fullFilePath.c_str(), deleteResult);
                }
            }
        }
    }
    
    Vector<ExtendedManifestCompressedFileDetail> uploads;
    Vector<Vector<const FileDetail*>> zipBatchSets = SplitUploadsIntoZipBatches(filesToUpload);
    for (const Vector<const FileDetail*>& zipBatchSet : zipBatchSets)
    {
        ExtendedManifestCompressedFileDetail u{};

        String fileId = CreateGUID();
        String zipFileName = FormatString("%s.zip", fileId.c_str());
        String fullZipFilePath;
        JoinPathHelper(cloudSyncFolder, zipFileName, fullZipFilePath);
        TRACE_INFORMATION("[GAME SAVE] UploadStep: Compressing to %s", fullZipFilePath.c_str());
        u.fileName = zipFileName.c_str();
        u.fullFilePath = fullZipFilePath;
        u.fileId = fileId;
        u.timeLastModified = time(nullptr);
        u.compression = CompressionType::Zip;
        u.archiveContext = MakeShared<ArchiveContext>();

        for (const FileDetail* fileToUpload : zipBatchSet)
        {
            u.uncompressedSizeBytes += fileToUpload->fileSizeBytes;
            AddCompressedFile(localFileFolderSet, fileToUpload, saveFolder, u);
        }

        RETURN_IF_FAILED(ZipUtils::ZipFilesIntoSingleZip(u.archiveContext, fullZipFilePath));
        u.compressedSizeBytes = u.archiveContext->GetTotalCompressedSize();
        TRACE_INFORMATION("[GAME SAVE] UploadStep: Compressed zip %s. size %llu", fullZipFilePath.c_str(), u.compressedSizeBytes);

        m_totalUncompressedSizeBytes += u.uncompressedSizeBytes;
        m_totalCompressedSizeBytes += u.compressedSizeBytes;

        uploads.push_back(std::move(u));
    }

    if (thumbnail)
    {
        ExtendedManifestCompressedFileDetail u{};
        String fullZipFilePath;
        JoinPathHelper(saveFolder, THUMBNAIL_FILE_NAME, fullZipFilePath);
        TRACE_INFORMATION("[GAME SAVE] UploadStep: Thumbnail at %s", fullZipFilePath.c_str());
        u.fileName = FormatString("pfthumbnail_%s.png", version.c_str());
        u.fullFilePath = fullZipFilePath;
        u.fileId = CreateGUID();
        u.timeLastModified = thumbnail->timeLastModified;
        u.compressedSizeBytes = thumbnail->fileSizeBytes;
        u.uncompressedSizeBytes = thumbnail->fileSizeBytes;
        u.compression = CompressionType::None;

        const FolderDetail& folderDetail = localFileFolderSet->GetFileFolder(thumbnail);
        ExtendedManifestExtractedFileDetail efd{};
        efd.fileName = THUMBNAIL_FILE_NAME;
        efd.fileId = CreateGUID();
        efd.folderId = folderDetail.folderId;
        efd.relFolderPath = folderDetail.relFolderPath;
        efd.folderName = folderDetail.folderName;
        efd.uncompressedSizeBytes = thumbnail->fileSizeBytes;
        efd.timeLastModified = thumbnail->timeLastModified;
        efd.timeCreated = thumbnail->timeCreated;
        u.extractedFiles.push_back(std::move(efd));

        uploads.push_back(std::move(u));
    }

    Vector<ExtendedManifestCompressedFileDetail> uploadsCopy = uploads;
    localFileFolderSet->SetCompressedFilesToUpload(std::move(uploadsCopy));

    return uploads;
}

HRESULT UploadStep::AddCompressedFile(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const FileDetail* fileToUpload,
    _In_ const String& saveFolder,
    _Inout_ ExtendedManifestCompressedFileDetail& u)
{
    const FolderDetail& folderDetail = localFileFolderSet->GetFileFolder(fileToUpload);
    ExtendedManifestExtractedFileDetail efd{};
    efd.fileName = fileToUpload->fileName;
    efd.fileId = fileToUpload->fileId;
    efd.folderId = folderDetail.folderId;
    efd.relFolderPath = folderDetail.relFolderPath;
    efd.folderName = folderDetail.folderName;
    efd.uncompressedSizeBytes = fileToUpload->fileSizeBytes;
    efd.timeLastModified = fileToUpload->timeLastModified;
    efd.timeCreated = fileToUpload->timeCreated;
    u.extractedFiles.push_back(std::move(efd));

    ArchiveFileDetail afd{};
    String relFilePath = localFileFolderSet->GetRelFilePath(fileToUpload);
    RETURN_IF_FAILED(JoinPathHelper(saveFolder, relFilePath, afd.fullPath));
    afd.uncompressedSize = fileToUpload->fileSizeBytes;
    TRACE_INFORMATION("[GAME SAVE] UploadStep: Compressing file %s. size %llu", afd.fullPath.c_str(), afd.uncompressedSize);
    u.archiveContext->AddFile(relFilePath, std::move(afd));
    return S_OK;
}

HRESULT UploadStep::AddUncompressedFile(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet, 
    _In_ const FileDetail* fileToUpload,
    _In_ const String& saveFolder,
    _Inout_ ExtendedManifestCompressedFileDetail& u)
{
    const FolderDetail& folderDetail = localFileFolderSet->GetFileFolder(fileToUpload);
    u.fileName = fileToUpload->fileName;
    String relFilePath;
    RETURN_IF_FAILED(JoinPathHelper(folderDetail.relFolderPath, fileToUpload->fileName, relFilePath));
    RETURN_IF_FAILED(JoinPathHelper(saveFolder, relFilePath, u.fullFilePath));
    u.fileId = CreateGUID();
    u.compressedSizeBytes = fileToUpload->fileSizeBytes;
    u.uncompressedSizeBytes = fileToUpload->fileSizeBytes;
    u.timeLastModified = fileToUpload->timeLastModified;
    u.compression = CompressionType::None;
    TRACE_INFORMATION("[GAME SAVE] UploadStep: Compress file %s", u.fullFilePath.c_str());

    ExtendedManifestExtractedFileDetail efd{};
    efd.fileName = fileToUpload->fileName;
    efd.fileId = fileToUpload->fileId;
    efd.folderId = folderDetail.folderId;
    efd.relFolderPath = folderDetail.relFolderPath;
    efd.uncompressedSizeBytes = fileToUpload->fileSizeBytes;
    efd.timeLastModified = fileToUpload->timeLastModified;
    efd.timeCreated = fileToUpload->timeCreated;
    u.extractedFiles.push_back(std::move(efd));
    return S_OK;
}


const FileDetail* UploadStep::PopFileDetailBelowSize(
    _In_ Vector<const FileDetail*>& filesToUpload, 
    _In_ uint64_t sizeLimit)
{
    for (auto it = filesToUpload.begin(); it != filesToUpload.end(); it++)
    {
        const FileDetail* curFileDetail = *it;
        if (curFileDetail->fileSizeBytes <= sizeLimit)
        {
            // Remove element from vector, and return element
            if (filesToUpload.size() > 1)
            {
                std::iter_swap(it, filesToUpload.end() - 1);
                filesToUpload.pop_back();
            }
            else
            {
                filesToUpload.clear();
            }
            return curFileDetail;
        }
    }

    return nullptr;
}



Vector<Vector<const FileDetail*>> UploadStep::SplitUploadsIntoZipBatches(
    _In_ Vector<const FileDetail*> filesToUpload // copy of vector on purpose
    )
{
    Vector<Vector<const FileDetail*>> fileZipBatchSets;
    const uint64_t maxUncompressedSize = 1048576ull * 64; // 64 MB

    uint64_t uncompressedSizeInCurrentBatch{ 0 };

    Vector<const FileDetail*> fileZipBatchSet;
    while (filesToUpload.size() > 0)
    {
        uint64_t fileSizeLimit = maxUncompressedSize - uncompressedSizeInCurrentBatch;
        const FileDetail* fileDetailToAdd = PopFileDetailBelowSize(filesToUpload, fileSizeLimit);
        if (fileDetailToAdd)
        {
            uncompressedSizeInCurrentBatch += fileDetailToAdd->fileSizeBytes;
            fileZipBatchSet.push_back(fileDetailToAdd);
        }
        else
        {
            if (fileZipBatchSet.size() == 0 && filesToUpload.size() > 0)
            {
                // If there was nothing yet in set but files remain, meaning the remaining file(s) 
                // are bigger than size limit so just push it to a new batch all by itself
                fileZipBatchSet.push_back(filesToUpload.back());
                filesToUpload.pop_back();
            }

            if (fileZipBatchSet.size() > 0)
            {
                // No more files will fit in this batch, so start a new batch
                fileZipBatchSets.push_back(std::move(fileZipBatchSet));
                fileZipBatchSet.clear();
                uncompressedSizeInCurrentBatch = 0;
            }
        }
    }

    if (fileZipBatchSet.size() > 0)
    {
        // Add the remaining the final zip if needed
        fileZipBatchSets.push_back(std::move(fileZipBatchSet));
    }

    return fileZipBatchSets;
}

void UploadStep::UploadFileFinally(
    _In_ ISchedulableTask& task,
    _In_ UICallbackManager& uiCallbackManager,
    _In_ std::recursive_mutex& folderSyncMutex,
    _In_ const SharedPtr<InnerProgressContext>& innerProgressContext,
    _In_ HRESULT hr
    )
{
    std::lock_guard<std::recursive_mutex> lock(folderSyncMutex); // Prevent any of the Finally blocks from changing the state while DoWork thread is active
    TRACE_TASK(FormatString("UploadFileFinally HR:0x%0.8x", hr));

    auto& detail = m_compressedFilesToUpload[m_compressedFilesToUploadCurIndex];
    if (detail.archiveContext)
    {
        m_currentUncompressedSizeBytes += detail.uncompressedSizeBytes;
        HRESULT deleteResult = FilePAL::DeleteLocalFile(detail.fullFilePath);
        if (FAILED(deleteResult))
        {
            m_telemetryManager->SetContextSyncHResult(deleteResult);
            m_telemetryManager->EmitContextSyncErrorEvent();
            m_stage = UploadStage::WaitForFailedUI_UploadFile;
            if (false == uiCallbackManager.ShowSyncFailedUI(task, m_localUser, deleteResult, PFGameSaveFilesSyncState::Uploading) )
            {
                // No fail callback set, so just fail API
                m_stage = UploadStage::LockStepFailure;
                m_failureHR = deleteResult;
                task.ScheduleNow();
            }
            return;
        }
    }

    if (FAILED(hr))
    {
        m_telemetryManager->SetContextSyncHResult(hr);
        m_telemetryManager->EmitContextSyncErrorEvent();
        m_stage = UploadStage::WaitForFailedUI_UploadFile;
        if( false == uiCallbackManager.ShowSyncFailedUI(task, m_localUser, hr, PFGameSaveFilesSyncState::Uploading) )
        {
            // No fail callback set, so just fail API
            m_stage = UploadStage::LockStepFailure;
            m_failureHR = hr;
            task.ScheduleNow();
        }
    }
    else
    {
        if (m_compressedFilesToUploadCurIndex == m_compressedFilesToUpload.size() - 1)
        {
            // uploaded last file (extended manifest)
            m_stage = UploadStage::FinalizeManifest;
            m_currentUncompressedSizeBytes = m_totalUncompressedSizeBytes;
        }
        else
        {
            if (detail.archiveContext)
            {
                m_telemetryManager->AddContextSyncFileCount(static_cast<uint32_t>(detail.archiveContext->GetFiles().size()));
            }
            
            m_telemetryManager->IncrementContextSyncBlockCount();
            m_telemetryManager->AddContextSyncOriginalSize(detail.uncompressedSizeBytes);
            m_telemetryManager->AddContextSyncSyncSize(detail.compressedSizeBytes);

            // loop until done
            m_compressedFilesToUploadCurIndex++;
        }
        
        innerProgressContext->callback(PFGameSaveFilesSyncState::Uploading, m_currentUncompressedSizeBytes, m_totalUncompressedSizeBytes, innerProgressContext->callbackContext);
        task.ScheduleNow();
    }
}

HRESULT UploadStep::Upload(
    _In_ const RunContext& runContext,
    _In_ ISchedulableTask& task,
    _In_ const SharedPtr<ManifestInternal>& latestPendingManifest,
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet,
    _In_ const String& saveFolder,
    _In_ PFGameSaveFilesUploadOption option,
    _In_ UICallbackManager& uiCallbackManager,
    _In_ FolderSyncManagerProgress& syncProgress,
    _In_ std::recursive_mutex& folderSyncMutex,
    _In_ ProgressCallback progressCallback,
    _In_ void* progressCallbackContext,
    _In_ const String& shortSaveDescription
)
{
    ScopeTracer scopeTracer(FormatString("UploadStep - %s", EnumName(m_stage)));

    switch (m_stage)
    {
        case UploadStage::UploadStart:
        {
            m_telemetryManager->ResetContextSync();
            m_telemetryManager->SetContextSyncStartTime();
            m_telemetryManager->SetContextSyncSyncDownload(false);
            m_telemetryManager->SetContextSyncSyncErrorSource(SyncErrorSource::SER_RegisterUpload);
            m_telemetryManager->SetContextSyncContextVersion(latestPendingManifest->GetManifest().GetVersion());
            syncProgress.syncState = PFGameSaveFilesSyncState::Uploading;
            uiCallbackManager.ShowProgressUI(task, m_localUser, PFGameSaveFilesSyncState::Uploading); // no issue if callback not set
            this->m_stage = UploadStage::CompressFiles;
            task.ScheduleNow();
            break;
        }

        case UploadStage::CompressFiles:
        {
            // TODO: Task 50948316: PFGameSave: hook up cancel from progression dialog
            m_compressedFilesToUpload.clear();
            m_compressedFilesToUploadCurIndex = 0;
            auto compressResult = CompressFiles(localFileFolderSet, saveFolder, latestPendingManifest->VersionString());
            RETURN_IF_FAILED(compressResult.hr);
            m_compressedFilesToUpload = compressResult.ExtractPayload();
            if (m_compressedFilesToUpload.size() == 0 &&
                localFileFolderSet->GetFilesToDeleteUponUpload().size() == 0 &&
                localFileFolderSet->GetFoldersToCreateUponUpload().size() == 0 &&
                localFileFolderSet->GetFoldersToDeleteUponUpload().size() == 0)
            {
                // Nothing to do, so just complete
                m_stage = UploadStage::UploadDone;
                task.ScheduleNow();
                return S_OK;
            }

            m_telemetryManager->SetContextSyncTotalSize(m_totalUncompressedSizeBytes);
            String metadataFolderPath, extendedManifestFullFilePath;
            RETURN_IF_FAILED(JoinPathHelper(saveFolder, "cloudsync", metadataFolderPath));
            FilePAL::CreatePath(metadataFolderPath);
            String extendedManifestName = FormatString("extended-%u-manifest.json", static_cast<uint32_t>(latestPendingManifest->Version()));
            RETURN_IF_FAILED(JoinPathHelper(metadataFolderPath, extendedManifestName, extendedManifestFullFilePath));

            auto manifestResult = ExtendedManifest::WriteExtendedManifest(
                m_compressedFilesToUpload,
                localFileFolderSet,
                remoteFileFolderSet, 
                saveFolder,
                false);

            RETURN_IF_FAILED(manifestResult.hr);
            auto& manifestContent = manifestResult.Payload();

            // add extended manifest to list to upload
            ExtendedManifestCompressedFileDetail detail{};
            detail.fileName = extendedManifestName;
            detail.fullFilePath = extendedManifestFullFilePath;
            detail.fileId = extendedManifestName; // fixed fileID on the extended manifest
            detail.uncompressedSizeBytes = manifestContent.size(); 
            detail.compressedSizeBytes = detail.uncompressedSizeBytes; 
            detail.compression = CompressionType::None; // don't compress this stand alone file

            ExtendedManifestExtractedFileDetail efd{};
            efd.fileName = detail.fileName;
            efd.fileId = extendedManifestName; // fixed fileID on the extended manifest
            efd.relFolderPath = RemoveRootPath(metadataFolderPath, saveFolder);
            efd.folderId = "";
            efd.uncompressedSizeBytes = detail.uncompressedSizeBytes;
            //time_t lastModified;
            //time_t createTime;
            detail.extractedFiles.push_back(std::move(efd));

            m_compressedFilesToUpload.push_back(std::move(detail));

            m_stage = UploadStage::InitiateUpload;
            progressCallback(PFGameSaveFilesSyncState::Uploading, 0, m_totalUncompressedSizeBytes, progressCallbackContext);
            task.ScheduleNow();
            return S_OK;
        }

        case UploadStage::InitiateUpload:
        {
            InitiateUploadRequest initRequest;
            initRequest.SetVersion(Uint64ToString(latestPendingManifest->Version()));

            FileToUploadWrapVector files;
            for (const ExtendedManifestCompressedFileDetail& uploadDetail : m_compressedFilesToUpload)
            {
                FileToUploadWrap file;
                file.SetFileName(uploadDetail.fileName);
                files.push_back(std::move(file));
            }
            initRequest.SetFiles(files);

            Vector<UploadFileDetail> mockFilesRequest;
            if (GameSaveServiceSelector::useMocks)
            {
                for (const ExtendedManifestCompressedFileDetail& uploadDetail : m_compressedFilesToUpload)
                {
                    UploadFileDetail ufd{};
                    ufd.fileName = uploadDetail.fileId; // set the filename to be the fileId
                    ufd.fullFilePath = uploadDetail.fullFilePath;
                    mockFilesRequest.push_back(std::move(ufd));
                }
            }

            TRACE_TASK("InitiateUpload");
            GameSaveServiceSelector::InitiateUpload(m_entity.value(), initRequest, runContext, mockFilesRequest) // mockFilesRequest only needed for mocks
            .Finally([this, &task, &uiCallbackManager, &folderSyncMutex](Result<InitiateUploadResponse> result)
            {
                std::lock_guard<std::recursive_mutex> lock(folderSyncMutex); // Prevent any of the Finally blocks from changing the state while DoWork thread is active
                TRACE_TASK(FormatString("InitiateUploadFinally HR:0x%0.8x", result.hr));
                m_telemetryManager->SetContextSyncHttpInfo(result.httpResult);

                if (GetForceSyncFailedError() || FAILED(result.hr))
                {
                    m_telemetryManager->SetContextSyncHResult(result.hr);
                    m_telemetryManager->EmitContextSyncErrorEvent();
                    m_stage = UploadStage::WaitForFailedUI_InitiateUpload;
                    if( false == uiCallbackManager.ShowSyncFailedUI(task, m_localUser, result.hr, PFGameSaveFilesSyncState::Uploading) )
                    {
                        // No fail callback set, so just fail API
                        m_stage = UploadStage::LockStepFailure;
                        m_failureHR = result.hr;
                        task.ScheduleNow();
                    }
                }
                else
                {
                    m_initiateResult = result.ExtractPayload();
                    m_stage = UploadStage::UploadFile;
                    task.ScheduleNow();
                }
            });

            return S_OK;
        }

        case UploadStage::UploadFile:
        {
            UploadFileDetail fileDetail{};
            ExtendedManifestCompressedFileDetail& curUploadDetail = m_compressedFilesToUpload[m_compressedFilesToUploadCurIndex];
            fileDetail.fileName = curUploadDetail.fileName;
            fileDetail.fullFilePath = curUploadDetail.fullFilePath;
            fileDetail.archiveContext = curUploadDetail.archiveContext;

            TRACE_INFORMATION("[GAME SAVE] UploadStep: UploadFile [%d/%llu] file %s", m_compressedFilesToUploadCurIndex, m_compressedFilesToUpload.size(), fileDetail.fileName.c_str());
            m_telemetryManager->SetContextSyncBlockName(fileDetail.fileName);
            m_telemetryManager->SetContextSyncSyncErrorSource(SyncErrorSource::SER_UploadData);

            // Extended manifest is last file in list
            if (m_compressedFilesToUploadCurIndex == m_compressedFilesToUpload.size() - 1)
            {
                if (localFileFolderSet->GetFilesToDeleteUponUpload().size() > 0)
                {
                    m_telemetryManager->ResetContextDelete();
                    m_telemetryManager->SetContextDeleteStartTime();
                    m_telemetryManager->SetContextDeleteDeleteType(DeleteType::DT_All);

                    uint64_t totalSize{};
                    for (auto file : localFileFolderSet->GetFilesToDeleteUponUpload())
                    {
                        totalSize += file->fileSizeBytes;
                    }
                    m_telemetryManager->SetContextDeleteTotalSize(totalSize);
                }

                auto& manifestDetail = m_compressedFilesToUpload.back();

                auto manifestResult = ExtendedManifest::WriteExtendedManifest(
                    m_compressedFilesToUpload,
                    localFileFolderSet,
                    remoteFileFolderSet,
                    saveFolder,
                    true);

                String manifest = manifestResult.ExtractPayload();
                manifestDetail.uncompressedSizeBytes = manifest.size();
                manifestDetail.compressedSizeBytes = manifest.size();

                auto innerProgressContext = MakeShared<InnerProgressContext>(progressCallback, progressCallbackContext, task, m_localUser, PFGameSaveFilesSyncState::Uploading);

                TRACE_TASK("UploadFileFromStringToCloud");
                GameSaveServiceSelector::UploadFileFromStringToCloud(runContext, fileDetail, manifest, m_initiateResult.GetFiles())
                .Finally([this, &task, &uiCallbackManager, &folderSyncMutex, innerProgressContext](Result<void> result)
                {
                    m_telemetryManager->SetContextDeleteHttpInfo(result.httpResult);
                    m_telemetryManager->SetContextSyncHttpInfo(result.httpResult);

                    TRACE_TASK(FormatString("UploadFileFromStringToCloud HR:0x%0.8x", result.hr));
                    UploadFileFinally(task, uiCallbackManager, folderSyncMutex, innerProgressContext, result.hr);
                });
            }
            else
            {
                auto innerProgressContext = MakeShared<InnerProgressContext>(progressCallback, progressCallbackContext, task, m_localUser, PFGameSaveFilesSyncState::Uploading);

                TRACE_TASK("UploadSingleFileToCloud");
                TRACE_INFORMATION("[GAME SAVE] UploadStep: file %s path %s", fileDetail.fileName.c_str(), fileDetail.fullFilePath.c_str());
                GameSaveServiceSelector::UploadSingleFileToCloud(runContext, fileDetail, m_initiateResult.GetFiles(), InnerProgressCallback, innerProgressContext.get(), m_totalUncompressedSizeBytes, m_currentUncompressedSizeBytes)
                .Finally([this, &task, &uiCallbackManager, &folderSyncMutex, innerProgressContext](Result<void> result)
                {
                    m_telemetryManager->SetContextSyncHttpInfo(result.httpResult);
                    TRACE_TASK(FormatString("UploadSingleFileToCloud HR:0x%0.8x", result.hr));
                    UploadFileFinally(task, uiCallbackManager, folderSyncMutex, innerProgressContext, result.hr);
                });
            }

            return S_OK;
        }

        case UploadStage::FinalizeManifest:
        {
            if (!m_hasStartedFinalizeManifest)
            {
                m_hasStartedFinalizeManifest = true; // mark that finalize has begun
            }
            FinalizeManifestRequest finalizeRequest;
            finalizeRequest.SetVersion(Uint64ToString(latestPendingManifest->Version()));
            m_telemetryManager->SetContextSyncSyncErrorSource(SyncErrorSource::SER_FinalizeUpload);

            PlayFab::Wrappers::ModelVector<PlayFab::Wrappers::PFGameSaveFinalizedFileDetailsWrapper<Allocator>, Allocator> filesToRequestList;
            for (ExtendedManifestCompressedFileDetail& compressedFileDetail : m_compressedFilesToUpload)
            {
                PlayFab::Wrappers::PFGameSaveFinalizedFileDetailsWrapper<Allocator> fileToRequest;
                fileToRequest.SetFileName(compressedFileDetail.fileName);
                uint64_t compressedSize = compressedFileDetail.compressedSizeBytes;
                if (compressedFileDetail.archiveContext != nullptr && compressedFileDetail.archiveContext->GetTotalCompressedSize() > 0)
                {
                    compressedSize = compressedFileDetail.archiveContext->GetTotalCompressedSize();
                }
                fileToRequest.SetFileSizeBytes(Uint64ToString(compressedSize));
                filesToRequestList.push_back(std::move(fileToRequest));
            }

            const Vector<CompressedFile>& compressedFiles = remoteFileFolderSet->GetCompressedFiles();
            const Vector<size_t>& compressedFilesToKeep = remoteFileFolderSet->GetCompressedFilesToKeep();

            for (const size_t& compressedFileToKeepIndex : compressedFilesToKeep)
            {
                const CompressedFile& compressedFileToKeep = compressedFiles[compressedFileToKeepIndex];
                PlayFab::Wrappers::PFGameSaveFinalizedFileDetailsWrapper<Allocator> fileToRequest;
                fileToRequest.SetFileName(compressedFileToKeep.fileName);
                uint64_t compressedSize = compressedFileToKeep.compressedSizeBytes;
                if (compressedFileToKeep.archiveContext != nullptr && compressedFileToKeep.archiveContext->GetTotalCompressedSize() > 0)
                {
                    compressedSize = compressedFileToKeep.archiveContext->GetTotalCompressedSize();
                }
                fileToRequest.SetFileSizeBytes(Uint64ToString(compressedSize));
                filesToRequestList.push_back(std::move(fileToRequest));
            }
#if _DEBUG // just for debug stats
            m_numFilesInFinalizedManifest = filesToRequestList.size();
#endif
            finalizeRequest.SetFilesToFinalize(std::move(filesToRequestList));
            finalizeRequest.SetManifestDescription(shortSaveDescription);
                
            TRACE_TASK("FinalizeManifest");
            GameSaveServiceSelector::FinalizeManifest(m_entity.value(), finalizeRequest, runContext)
            .Finally([this, &task, saveFolder, latestPendingManifest, localFileFolderSet, remoteFileFolderSet, &uiCallbackManager, &folderSyncMutex, shortSaveDescription](Result<PlayFab::GameSaveWrapper::FinalizeManifestResponse> result)
            {
                std::lock_guard<std::recursive_mutex> lock(folderSyncMutex); // Prevent any of the Finally blocks from changing the state while DoWork thread is active
                TRACE_TASK(FormatString("FinalizeManifestFinally HR:0x%0.8x", result.hr));
                m_telemetryManager->SetContextSyncHttpInfo(result.httpResult);

                if( FAILED(result.hr) )
                {
                    m_telemetryManager->SetContextSyncHResult(result.hr);
                    m_telemetryManager->EmitContextSyncErrorEvent();

                    if (result.hr == E_PF_INVALID_PARAMS && this->m_uploadFullSetRetryCount == 0)
                    {
                        // when finalizing with 100+ zips in manifest, service returns:
                        // {"code":400,"status":"BadRequest","error":"InvalidParams","errorCode":1000,"errorMessage":"Invalid input parameters","errorDetails":{"FilesToFinalize":["The field FilesToFinalize must be a string or array type with a maximum length of '100'."]}}

                        // To handle, re-upload entire local file set re-compressed
                        // This should cause there to be less than 100 64MB zips due to quota

                        // Delete all prior compressed files
                        for (const ExtendedManifestCompressedFileDetail& uploadDetail : m_compressedFilesToUpload)
                        {
                            FilePAL::DeleteLocalFile(uploadDetail.fullFilePath);
                        }

                        SetToUploadFullSet(localFileFolderSet, remoteFileFolderSet);
                        Reset(); // clear out all previous upload data

                        this->m_uploadFullSetRetryCount++;
                        this->m_stage = UploadStage::CompressFiles;
                        task.ScheduleNow();
                    }
                    else if (result.hr == E_PF_GAME_SAVE_NEWER_MANIFEST_EXISTS)
                    {
                        // force go offline
                        this->m_stage = UploadStage::ForceDisconnectFromCloud;
                        task.ScheduleNow();
                    }
                    else
                    {
                        // TODO: should this show UI or just fail?
                        this->m_uploadFullSetRetryCount = 0; // reset the count for next time
                        m_stage = UploadStage::WaitForFailedUI_FinalizeManifest;
                        if( false == uiCallbackManager.ShowSyncFailedUI(task, m_localUser, result.hr, PFGameSaveFilesSyncState::Uploading) )
                        {
                            // No fail callback set, so just fail API
                            m_stage = UploadStage::LockStepFailure;
                            m_failureHR = result.hr;
                            task.ScheduleNow();
                        }
                    }
                }
                else
                {
                    this->m_uploadFullSetRetryCount = 0; // reset the count for next time
                    for (const ExtendedManifestCompressedFileDetail& uploadDetail : m_compressedFilesToUpload)
                    {
                        if (uploadDetail.compression == CompressionType::Zip)
                        {
                            FilePAL::DeleteLocalFile(uploadDetail.fullFilePath);
                        }
                        else 
                        {
                            if (uploadDetail.extractedFiles.size() == 1 && 
                                uploadDetail.extractedFiles[0].fileName == THUMBNAIL_FILE_NAME)
                            {
                                // leave uncompressed thumbnail alone
                            }
                        }
                    }

                    localFileFolderSet->UpdateFilesWithUploadData();
                    LocalStateManifest::WriteLocalManifest(saveFolder, localFileFolderSet, shortSaveDescription);
                    m_stage = UploadStage::ListManifestsAfterUpload;
                    task.ScheduleNow();
                }
            });
            return S_OK;
        }

        case UploadStage::ForceDisconnectFromCloud:
        {
            // Error handled by FolderSyncManager
            return E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD;
        }

        case UploadStage::ListManifestsAfterUpload:
        {
            ListManifestsRequest request{};

            // Clean up all zip files in the cloudsync folder since upload is complete
            String cloudSyncFolder;
            RETURN_IF_FAILED(JoinPathHelper(saveFolder, "cloudsync", cloudSyncFolder));
            
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
                        TRACE_INFORMATION("[GAME SAVE] UploadStep: Cleaning up zip file after upload: %s", fullFilePath.c_str());
                        FilePAL::DeleteLocalFile(fullFilePath);
                    }
                }
            }
                    
            GameSaveServiceSelector::ListManifests(m_entity.value(), request, runContext)
            .Finally([this, &task, &uiCallbackManager, &folderSyncMutex](Result<ListManifestsResponse> result)
            {
                std::lock_guard<std::recursive_mutex> lock(folderSyncMutex); // Prevent any of the Finally blocks from changing the state while DoWork thread is active
                TRACE_TASK(FormatString("ListManifestsAfterUploadFinally HR:0x%0.8x", result.hr));
                m_telemetryManager->SetContextSyncHttpInfo(result.httpResult);

                if (FAILED(result.hr))
                {
                    m_telemetryManager->SetContextSyncHResult(result.hr);
                    m_telemetryManager->EmitContextSyncErrorEvent();
                    m_stage = UploadStage::WaitForFailedUI_ListManifestsAfterUpload;
                    if( false == uiCallbackManager.ShowSyncFailedUI(task, m_localUser, result.hr, PFGameSaveFilesSyncState::Uploading) )
                    {
                        // No fail callback set, so just fail API
                        m_stage = UploadStage::LockStepFailure;
                        m_failureHR = result.hr;
                        task.ScheduleNow();
                    }
                }
                else
                {
                    m_manifests = result.Payload().GetManifests();
                    m_stage = UploadStage::TakeLockAfterUpload;
                    task.ScheduleNow();
                }
            });

            return S_OK;
        }

        case UploadStage::TakeLockAfterUpload:
        {
            m_postUploadLatestFinalizedPFManifest = ManifestWrap();
            LockStep::TryGetLatestFinalizedManifest(m_manifests, m_postUploadLatestFinalizedPFManifest);

            TRACE_INFORMATION("[GAME SAVE] UploadStep: TakeLockAfterUpload %s. FinalizedManifest v%s", 
                (option == PFGameSaveFilesUploadOption::KeepDeviceActive) ? "KeepDeviceActive" : "ReleaseDeviceAsActive", 
                m_postUploadLatestFinalizedPFManifest.GetVersion().c_str());

            if (option == PFGameSaveFilesUploadOption::KeepDeviceActive)
            {
                InitializeManifestRequest initManifestRequest;
                uint64_t newManifestVersion = StringToUint64(m_postUploadLatestFinalizedPFManifest.GetVersion()) + 1;
                uint64_t baseManifestVersion = StringToUint64(m_postUploadLatestFinalizedPFManifest.GetVersion());
                LockStep::CreateInitManifestRequest(m_entity.value(), initManifestRequest, baseManifestVersion, newManifestVersion, m_manifestVersionOffset, saveFolder);

                TRACE_TASK("TakeLockAfterUpload InitializeManifest");
                GameSaveServiceSelector::InitializeManifest(m_entity.value(), initManifestRequest, runContext)
                .Finally([this, &task, &uiCallbackManager, &folderSyncMutex](Result<PlayFab::GameSaveWrapper::InitializeManifestResponse> result)
                {
                    std::lock_guard<std::recursive_mutex> lock(folderSyncMutex); // Prevent any of the Finally blocks from changing the state while DoWork thread is active
                    TRACE_TASK(FormatString("TakeLockAfterUpload InitializeManifestFinally HR:0x%0.8x", result.hr));
                    m_telemetryManager->SetContextSyncHttpInfo(result.httpResult);

                    if (FAILED(result.hr))
                    {
                        m_telemetryManager->SetContextSyncHResult(result.hr);
                        m_telemetryManager->EmitContextSyncErrorEvent();

                        // service can return this if somehow reusing an previously used manifest number:
                        // {"code":409,"status":"Conflict","error":"GameSaveManifestVersionAlreadyExists","errorCode":20301,"errorMessage":"GameSaveManifestVersionAlreadyExists"}
                        if (result.hr == E_PF_GAME_SAVE_MANIFEST_VERSION_ALREADY_EXISTS)
                        {
                            m_stage = UploadStage::TakeLockAfterUpload;
                            m_manifestVersionOffset++;
                            task.ScheduleNow();
                        }
                        else
                        {
                            m_stage = UploadStage::WaitForFailedUI_TakeLockAfterUpload;
                            if( false == uiCallbackManager.ShowSyncFailedUI(task, m_localUser, result.hr, PFGameSaveFilesSyncState::Uploading) )
                            {
                                // No fail callback set, so just fail API
                                m_stage = UploadStage::LockStepFailure;
                                m_failureHR = result.hr;
                                task.ScheduleNow();
                            }
                        }
                    }
                    else
                    {
                        auto& resultManifest = result.Payload().GetManifest();
                        assert(resultManifest.has_value());
                        if (resultManifest.has_value())
                        {
                            m_postUploadPendingPFManifest = resultManifest.value();
                        }
                        m_stage = UploadStage::UploadDone;
                        task.ScheduleNow();
                    }
                });
            }
            else
            {
                m_postUploadPendingPFManifest = ManifestWrap();
                m_stage = UploadStage::UploadDone;
                task.ScheduleNow();
            }

            return S_OK;
        }

        case UploadStage::WaitForFailedUI_InitiateUpload:
        {
            return uiCallbackManager.HandleFailedUI(task, [this]() { m_stage = UploadStage::InitiateUpload; }, [this]() { assert(false); m_stage = UploadStage::UploadDone; });
        }

        case UploadStage::WaitForFailedUI_UploadFile:
        {
            return uiCallbackManager.HandleFailedUI(task, [this]() { m_stage = UploadStage::UploadFile; }, [this]() { assert(false); m_stage = UploadStage::UploadDone; });
        }

        case UploadStage::WaitForFailedUI_FinalizeManifest:
        {
            return uiCallbackManager.HandleFailedUI(task, [this]() { m_stage = UploadStage::FinalizeManifest; }, [this]() { assert(false); m_stage = UploadStage::UploadDone; });
        }

        case UploadStage::WaitForFailedUI_ListManifestsAfterUpload:
        {
            return uiCallbackManager.HandleFailedUI(task, [this]() { m_stage = UploadStage::ListManifestsAfterUpload; }, [this]() { assert(false); m_stage = UploadStage::UploadDone; });
        }

        case UploadStage::WaitForFailedUI_TakeLockAfterUpload:
        {
            return uiCallbackManager.HandleFailedUI(task, [this]() { m_stage = UploadStage::TakeLockAfterUpload; }, [this]() { assert(false); m_stage = UploadStage::UploadDone; });
        }

        case UploadStage::LockStepFailure:
        {
            return m_failureHR;
        }

        case UploadStage::UploadDone:
        {
            assert(false);
            return S_OK;
        }
    }

    return S_OK;
}

void UploadStep::SetToUploadFullSet(
    const SharedPtr<FileFolderSet>& localFileFolderSet, 
    const SharedPtr<FileFolderSet>& remoteFileFolderSet)
{
    const Vector<FileDetail>& localFiles = localFileFolderSet->GetFiles();
    const Vector<FolderDetail>& localFolders = localFileFolderSet->GetFolders();

    // change FilesToUpload to full set of local files
    Vector<const FileDetail*> filesToUpload;
    for (const FileDetail& localFile : localFiles)
    {
        filesToUpload.push_back(&localFile);
    }
    localFileFolderSet->SetFilesToUpload(std::move(filesToUpload));

    // Change FilesToDeleteUponUpload to empty
    Vector<const FileDetail*> filesToDeleteUponUpload;
    localFileFolderSet->SetFilesToDeleteUponUpload(std::move(filesToDeleteUponUpload));

    // Change FoldersToCreateUponUpload to all folders except root
    Vector<const FolderDetail*> foldersToCreateUponUpload;
    for (const FolderDetail& localFolder : localFolders)
    {
        if (localFolder.relFolderPath.length() == 0)
        {
            continue; // skip the root folder
        }

        foldersToCreateUponUpload.push_back(&localFolder);
    }
    localFileFolderSet->SetFoldersToCreateUponUpload(std::move(foldersToCreateUponUpload));

    // Change CompressedFilesToKeep to empty
    Vector<size_t> compressedFileIndies;
    remoteFileFolderSet->SetCompressedFilesToKeep(std::move(compressedFileIndies));
}

HRESULT UploadStep::DeletePendingManifest(
    _In_ const RunContext& runContext,
    _In_ ISchedulableTask& task,
    _In_ const SharedPtr<ManifestInternal>& latestPendingManifest,
    _In_ std::recursive_mutex& folderSyncMutex
    )
{
    ScopeTracer scopeTracer(FormatString("DeletePendingManifest - %s", EnumName(m_deleteManifestStage)));

    switch (m_deleteManifestStage)
    {
        case DeleteManifestStage::DeleteManifestStart:
        {
            m_telemetryManager->ResetContextDelete();
            m_telemetryManager->SetContextDeleteStartTime();
            m_telemetryManager->SetContextDeleteDeleteType(DeleteType::DT_Version);
            m_telemetryManager->SetContextDeleteContextVersion(latestPendingManifest->GetManifest().GetVersion());

            // delete pending manifest
            DeleteManifestRequest deleteRequest;
            deleteRequest.SetVersion(Uint64ToString(latestPendingManifest->Version()));
            GameSaveServiceSelector::DeleteManifest(m_entity.value(), deleteRequest, runContext)
            .Finally([this, &task, &folderSyncMutex](Result<void> result)
            {
                std::lock_guard<std::recursive_mutex> lock(folderSyncMutex); // Prevent any of the Finally blocks from changing the state while DoWork thread is active
                TRACE_TASK(FormatString("DeleteManifestFinally HR:0x%0.8x", result.hr));
                m_telemetryManager->SetContextDeleteHttpInfo(result.httpResult);

                m_deleteManifestStage = DeleteManifestStage::DeleteDone;
                task.ScheduleNow();
            });

            return S_OK;
        }

        case DeleteManifestStage::DeleteDone:
        {
            assert(false);
            return S_OK;
        }
    }

    return S_OK;
}


} // namespace GameSave
} // namespace PlayFab

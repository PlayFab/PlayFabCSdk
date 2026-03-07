// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "FolderSyncManager.h"
#include "ProgressHelpers.h"
#include "Platform/Platform.h"
#include "ActiveDevicePollWorker.h"

using namespace PlayFab::GameSaveWrapper;

namespace PlayFab
{
namespace GameSave
{

void FolderSyncManagerProgressCallback(PFGameSaveFilesSyncState syncState, uint64_t current, uint64_t total, void* context)
{
    auto folderSyncManager = static_cast<FolderSyncManager*>(context);
    folderSyncManager->SetSyncStateProgress(syncState, current, total);
}

FolderSyncManager::FolderSyncManager(_In_ LocalUser const& localUser) :
    m_telemetryManager{ MakeShared<GameSaveTelemetryManager>() },
    m_lockStep(localUser, m_telemetryManager),
    m_compareStep(localUser, m_telemetryManager),
    m_downloadStep(localUser, m_telemetryManager),
    m_uploadStep(localUser, m_telemetryManager),
    m_resetCloudStep(localUser, m_telemetryManager),
    m_setSaveDescriptionStep(localUser, m_telemetryManager),
    m_localUser{localUser}
{
    SharedPtr<GameSaveGlobalState> state;
    HRESULT hr = GameSaveGlobalState::Get(state);
    if (SUCCEEDED(hr))
    {
        String rootOverride = state->GetDebugRootFolderOverride();
        if (!rootOverride.empty())
        {
            m_saveFolder = rootOverride;
        }
        else
        {
            m_saveFolder = state->GetInitArgsSaveRootFolder(); // Might be empty if not set by init args on some platforms
        }
    }

    TRACE_TASK(FormatString("FolderSyncManager ctor. SaveFolder: %s", m_saveFolder.c_str()));
}

FolderSyncManager::~FolderSyncManager()
{
    TRACE_TASK("FolderSyncManager dtor");
}

void FolderSyncManager::FireActivationFailedTelemetry(HRESULT hr, bool offline)
{
    if (hr == E_PF_GAMESAVE_USER_CANCELLED)
    {
        m_telemetryManager->SetContextActivationCanceled(true);
    }
    else if (hr == E_ABORT)
    {
        m_telemetryManager->SetContextActivationAborted(true);
    }
    m_telemetryManager->SetContextActivationHResult(hr);
    m_telemetryManager->SetContextActivationSyncState(offline ? SyncState::SS_NotStarted : SyncState::SS_Preparing);
    m_telemetryManager->EmitContextActivationFailureEvent();
    m_telemetryManager->EmitContextActivationEvent();
}

HRESULT FolderSyncManager::DoWorkFolderDownload(_In_ const RunContext& runContext, _In_ ISchedulableTask& task, _In_ std::recursive_mutex& folderSyncMutex)
{
    ScopeTracer scopeTracer("FolderSyncManager::DoWorkFolderDownload");

    // Early cancellation check (user called XAsyncCancel on AddUserWithUiAsync)
    if (runContext.CancellationToken().IsCancelled())
    {
        FireActivationFailedTelemetry(E_ABORT, m_lockStep.IsForceDisconnectFromCloud());
        SetSyncStateProgress(PFGameSaveFilesSyncState::NotStarted, 0, 0);
        return E_ABORT;
    }

    // 1. Lock acquisition w/ active device contention UX if needed & sync error UX if needed
    if (!m_lockStep.IsLockAcquired())
    {
        m_lockStep.SetAddUserOptions(m_addUserOptions);
        HRESULT hr = m_lockStep.AcquireActiveDevice(m_uiManager, runContext, task, folderSyncMutex, m_saveFolder);
        if (FAILED(hr))
        {
            FireActivationFailedTelemetry(hr, m_lockStep.IsForceDisconnectFromCloud());
            return hr;
        }

        return E_PENDING;
    }

    m_isForcedDisconnectFromCloud = m_lockStep.IsForceDisconnectFromCloud();
    if (m_isForcedDisconnectFromCloud)
    {
        m_telemetryManager->SetContextActivationSyncState(SyncState::SS_NotStarted);
        m_telemetryManager->EmitContextActivationEvent();
        SetSyncStateProgress(PFGameSaveFilesSyncState::NotStarted, 0, 0);
        SetStatsForDebug();
        return S_OK;
    }

    if (m_latestFinalizedManifest == nullptr)
    {
        std::optional<Entity> entity = m_lockStep.GetEntity();
        m_compareStep.SetEntity(entity.value());
        m_downloadStep.SetEntity(entity.value());
        m_uploadStep.SetEntity(entity.value());
        m_setSaveDescriptionStep.SetEntity(entity.value());

        m_latestFinalizedManifest = MakeShared<ManifestInternal>(m_lockStep.GetLatestFinalizedPFManifest());
        m_latestPendingManifest = MakeShared<ManifestInternal>(m_lockStep.GetLatestPendingPFManifest());
        m_localFileFolderSet = MakeShared<FileFolderSet>();
        m_remoteFileFolderSet = MakeShared<FileFolderSet>();
    }

    // 2. Compare cloud vs local metadata w/ conflict UX if needed & sync error UX if needed
    if (!m_compareStep.IsCompareDone())
    {
        HRESULT hr = m_compareStep.CompareWithCloud(
            runContext,
            task,
            m_saveFolder,
            m_uiManager,
            true,
            m_latestFinalizedManifest,
            m_localFileFolderSet,
            m_remoteFileFolderSet,
            folderSyncMutex
        );
        if (FAILED(hr))
        {
            FireActivationFailedTelemetry(hr, m_compareStep.IsForceDisconnectFromCloud());
            return hr;
        }

        return E_PENDING;
    }

    m_isForcedDisconnectFromCloud = m_compareStep.IsForceDisconnectFromCloud();
    if (m_isForcedDisconnectFromCloud)
    {
        m_telemetryManager->SetContextActivationSyncState(SyncState::SS_NotStarted);
        m_telemetryManager->EmitContextActivationEvent();
        SetSyncStateProgress(PFGameSaveFilesSyncState::NotStarted, 0, 0);
        SetStatsForDebug();
        return S_OK;
    }

    m_telemetryManager->EmitContextActivationEvent();

    if (!m_downloadStep.IsLocalOperationsDone())
    {
        m_telemetryManager->ResetContextSync();
        m_telemetryManager->SetContextSyncContextVersion(m_latestFinalizedManifest->GetManifest().GetVersion());

        m_telemetryManager->ResetContextDelete();
        m_telemetryManager->SetContextDeleteStartTime();
        m_telemetryManager->SetContextDeleteDeleteType(DeleteType::DT_Local);
        TRACE_INFORMATION("[GAME SAVE] m_localFileFolderSet->GetFilesToDeleteUponDownload().size() %zd", m_localFileFolderSet->GetFilesToDeleteUponDownload().size());
        if (m_localFileFolderSet->GetFilesToDeleteUponDownload().size() > 0)
        {
            uint64_t totalSize{};
            for (auto file : m_localFileFolderSet->GetFilesToDeleteUponDownload())
            {
                totalSize += file->fileSizeBytes;
            }
            m_telemetryManager->SetContextDeleteTotalSize(totalSize);

            HRESULT hr = m_downloadStep.DeleteFiles(m_saveFolder, m_localFileFolderSet);
            if (FAILED(hr))
            {
                m_telemetryManager->SetContextDeleteHResult(hr);
            }
        }
        TRACE_INFORMATION("[GAME SAVE] m_localFileFolderSet->GetFoldersToDeleteUponDownload().size() %zd", m_localFileFolderSet->GetFoldersToDeleteUponDownload().size());
        if (m_localFileFolderSet->GetFoldersToDeleteUponDownload().size() > 0)
        {
            HRESULT hr = m_downloadStep.DeleteFolders(m_saveFolder, m_localFileFolderSet);
            if (FAILED(hr))
            {
                m_telemetryManager->SetContextDeleteHResult(hr);
            }
        }

        m_telemetryManager->EmitContextDeleteEvent();

        TRACE_INFORMATION("[GAME SAVE] m_localFileFolderSet->GetFoldersToCreateUponDownload().size() %zd", m_localFileFolderSet->GetFoldersToCreateUponDownload().size());
        if (m_remoteFileFolderSet->GetFoldersToCreateUponDownload().size() > 0)
        {
            m_downloadStep.CreateEmptyFolders(m_saveFolder, m_remoteFileFolderSet);
        }

        m_downloadStep.SetLocalOperationsDone(true);
    }

    m_isForcedDisconnectFromCloud = m_downloadStep.IsForceDisconnectFromCloud();
    if (m_isForcedDisconnectFromCloud)
    {
        SetSyncStateProgress(PFGameSaveFilesSyncState::NotStarted, 0, 0);
        SetStatsForDebug();
        return S_OK;
    }

    if (m_remoteFileFolderSet->GetCompressedFilesToDownload().size() > 0)
    {
        // 3. Download what's needed w/ progress UX if needed & sync error UX if needed
        if (!m_downloadStep.IsDownloadDone())
        {
            String shortSaveDescription = m_latestFinalizedManifest->GetManifest().GetManifestDescription();

            HRESULT hr = m_downloadStep.Download(runContext, task, m_saveFolder, m_uiManager, m_localFileFolderSet, m_remoteFileFolderSet, m_syncProgress, folderSyncMutex, FolderSyncManagerProgressCallback, this, shortSaveDescription);
            if (FAILED(hr))
            {
                m_telemetryManager->SetContextSyncHResult(hr);
                m_telemetryManager->EmitContextSyncErrorEvent();
                m_telemetryManager->EmitContextSyncEvent();
                return hr;
            }

            return E_PENDING;
        }
    }

    m_telemetryManager->EmitContextSyncEvent();

    SetSyncStateProgress(PFGameSaveFilesSyncState::SyncComplete, 0, 0);
    SetStatsForDebug();
    m_pollingForActiveDeviceChange = true;

    SharedPtr<GameSaveGlobalState> state;
    HRESULT hr = GameSaveGlobalState::Get(state);
    if (SUCCEEDED(hr))
    {
        // runContext is created via state->RunContext().DeriveOnQueue(async->queue));
        // so use original glboal's RunContext() for background queue
        auto backgroundRunContext = state->RunContext().Derive();
        std::optional<Entity> entity = m_lockStep.GetEntity();
        m_tokenRefreshWorker = ActiveDevicePollWorker::MakeAndStart(entity.value(), m_localUser, std::move(backgroundRunContext));
    }

    return S_OK;
}

void FolderSyncManager::SetStatsForDebug()
{
#if _DEBUG
    JsonValue rootJson { JsonValue::object() };

    JsonValue fileDownloadJsonArray{ JsonValue::array() };
    if (m_remoteFileFolderSet)
    {
        const PlayFab::Vector<const FileDetail*>& filesToDownload = m_remoteFileFolderSet->GetFilesToDownload();
        for (const FileDetail* fileToDownload : filesToDownload)
        {
            JsonValue jsonObj{ JsonValue::object() };
            JsonUtils::ObjectAddMember(jsonObj, "Name", fileToDownload->fileName);
            JsonUtils::ObjectAddMember(jsonObj, "Size", fileToDownload->fileSizeBytes);
            JsonUtils::ObjectAddMember(jsonObj, "TimeCreated", fileToDownload->timeCreated);
            JsonUtils::ObjectAddMember(jsonObj, "TimeLastModified", fileToDownload->timeLastModified);
            fileDownloadJsonArray.push_back(jsonObj);
        }
    }
    JsonUtils::ObjectAddMember(rootJson, "FilesToDownload", std::move(fileDownloadJsonArray));

    JsonValue fileUploadedJsonArray{ JsonValue::array() };
    if (m_localFileFolderSet)
    {
        const PlayFab::Vector<const FileDetail*>& filesToUpload = m_localFileFolderSet->GetFilesToUpload();
        for (const FileDetail* fileToUpload : filesToUpload)
        {
            JsonValue jsonObj{ JsonValue::object() };
            JsonUtils::ObjectAddMember(jsonObj, "Name", fileToUpload->fileName);
            JsonUtils::ObjectAddMember(jsonObj, "Size", fileToUpload->fileSizeBytes);
            JsonUtils::ObjectAddMember(jsonObj, "TimeCreated", fileToUpload->timeCreated);
            JsonUtils::ObjectAddMember(jsonObj, "TimeLastModified", fileToUpload->timeLastModified);
            fileUploadedJsonArray.push_back(jsonObj);
        }
    }
    JsonUtils::ObjectAddMember(rootJson, "FilesToUpload", std::move(fileUploadedJsonArray));

    if (m_remoteFileFolderSet)
    {
        const PlayFab::Vector<PlayFab::GameSave::CompressedFile>& compressedFiles = m_remoteFileFolderSet->GetCompressedFiles();
        const PlayFab::Vector<size_t>& compressedFilesToDownload = m_remoteFileFolderSet->GetCompressedFilesToDownload();
        JsonValue fileCompressedToDownloadJsonArray{ JsonValue::array() };
        for (const size_t index : compressedFilesToDownload)
        {
            JsonValue jsonObj{ JsonValue::object() };
            const PlayFab::GameSave::CompressedFile& file = compressedFiles.at(index);
            JsonUtils::ObjectAddMember(jsonObj, "Name", file.fileName);
            JsonUtils::ObjectAddMember(jsonObj, "Size", file.uncompressedSizeBytes);
            uint64_t compressedSize = file.compressedSizeBytes;
            if (file.archiveContext && file.archiveContext->GetTotalCompressedSize() > 0)
            {
                compressedSize = file.archiveContext->GetTotalCompressedSize();
            }
            JsonUtils::ObjectAddMember(jsonObj, "CompressedSize", compressedSize);
            JsonUtils::ObjectAddMember(jsonObj, "DownloadedLocally", file.hasDownloadedLocally);
            fileCompressedToDownloadJsonArray.push_back(jsonObj);
        }
        JsonUtils::ObjectAddMember(rootJson, "CompressedFilesToDownload", std::move(fileCompressedToDownloadJsonArray));
    }

    JsonValue fileCompressedToUploadJsonArray{ JsonValue::array() };
    if (m_localFileFolderSet)
    {
        Vector<ExtendedManifestCompressedFileDetail> compressedFilesToUpload = m_localFileFolderSet->GetCompressedFilesToUpload();
        for (const ExtendedManifestCompressedFileDetail& file : compressedFilesToUpload)
        {
            JsonValue jsonObj{ JsonValue::object() };
            JsonUtils::ObjectAddMember(jsonObj, "Name", file.fileName);
            JsonUtils::ObjectAddMember(jsonObj, "Size", file.uncompressedSizeBytes);
            uint64_t compressedSize = file.compressedSizeBytes;
            if (file.archiveContext && file.archiveContext->GetTotalCompressedSize() > 0)
            {
                compressedSize = file.archiveContext->GetTotalCompressedSize();
            }
            JsonUtils::ObjectAddMember(jsonObj, "CompressedSize", compressedSize);
            fileCompressedToUploadJsonArray.push_back(jsonObj);
        }
    }
    JsonUtils::ObjectAddMember(rootJson, "CompressedFilesToUpload", std::move(fileCompressedToUploadJsonArray));

    JsonValue allFilesArray{ JsonValue::array() };
    if (m_localFileFolderSet)
    {
        const PlayFab::Vector<PlayFab::GameSave::FileDetail>& skippedFiles = m_localFileFolderSet->GetSkippedFiles();
        for (const PlayFab::GameSave::FileDetail& file : skippedFiles)
        {
            JsonValue jsonObj{ JsonValue::object() };
            JsonUtils::ObjectAddMember(jsonObj, "Name", file.fileName);
            JsonUtils::ObjectAddMember(jsonObj, "Size", file.fileSizeBytes);
            JsonUtils::ObjectAddMember(jsonObj, "SkipFile", file.skipFile);
            allFilesArray.push_back(jsonObj);
        }
    }
    JsonUtils::ObjectAddMember(rootJson, "SkippedFiles", std::move(allFilesArray));

    JsonValue jsonObjUpload{ JsonValue::object() };
    JsonUtils::ObjectAddMember(jsonObjUpload, "NumFilesInFinalizedManifest", m_uploadStep.GetNumFilesInFinalizedManifest());
    JsonUtils::ObjectAddMember(rootJson, "Upload", std::move(jsonObjUpload));

    m_statsJsonForDebug = JsonUtils::WriteToString(rootJson);
#endif
}

HRESULT FolderSyncManager::DoWorkFolderUpload(_In_ RunContext& runContext, _In_ ISchedulableTask& task, _In_ std::recursive_mutex& folderSyncMutex, _In_ PFGameSaveFilesUploadOption option)
{
    std::lock_guard<std::recursive_mutex> lock(folderSyncMutex); // Prevent any of the Finally blocks from changing the state while the DoWork thread is active
    ScopeTracer scopeTracer("FolderSyncManager::DoWorkFolderUpload");

    // Early cancellation check (user called XAsyncCancel on UploadWithUiAsync)
    if (runContext.CancellationToken().IsCancelled())
    {
        return E_ABORT;
    }

    if (IsForcedDisconnectFromCloud())
    {
        // This error is returned regardless if the network is restored as the user is still considered offline.
        return E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD;
    }

    if (m_latestPendingManifest == nullptr || m_latestFinalizedManifest == nullptr)
    {
        TRACE_ERROR("DoWorkFolderUpload null manifest");
        assert(false); // shouldn't happen
        return E_UNEXPECTED;
    }
    if (m_localFileFolderSet == nullptr || m_remoteFileFolderSet == nullptr)
    {
        TRACE_ERROR("DoWorkFolderUpload null fileset");
        assert(false); // shouldn't happen
        return E_UNEXPECTED;
    }

    // 1. Compare cloud vs local metadata
    if (!m_compareStep.IsCompareDone())
    {
        HRESULT hr = m_compareStep.CompareWithCloud(
            runContext,
            task,
            m_saveFolder,
            m_uiManager,
            false,
            m_latestFinalizedManifest,
            m_localFileFolderSet,
            m_remoteFileFolderSet,
            folderSyncMutex
        );
        if (FAILED(hr))
        {
            return hr;
        }

        return E_PENDING;
    }

    if (m_localFileFolderSet->GetFilesToUpload().size() > 0 ||
        m_localFileFolderSet->GetFilesToDeleteUponUpload().size() > 0 ||
        m_localFileFolderSet->GetFoldersToCreateUponUpload().size() > 0 ||
        m_localFileFolderSet->GetFoldersToDeleteUponUpload().size() > 0)
    {
        // 2. Upload what's needed w/ progress UX if needed
        if (!m_uploadStep.IsUploadDone())
        {
            HRESULT hr = m_uploadStep.Upload(
                runContext, task, 
                m_latestPendingManifest, m_localFileFolderSet, m_remoteFileFolderSet, 
                m_saveFolder, option, m_uiManager, m_syncProgress, 
                folderSyncMutex, FolderSyncManagerProgressCallback, this,
                m_lastShortSaveDescription);
            if (hr == E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD)
            {
                // If we get disconnected from the cloud (aka the manifest we uploaded is outdated), we need to do the following:

                // a) force go offline
                SetForcedDisconnectFromCloud(true);

                // b) trigger active device change callback
                PFGameSaveDescriptor activeDevice{};
                FolderSyncManager::ConvertToPFGameSaveDescriptor(m_latestPendingManifest->GetManifest(), activeDevice);
                UICallbackManager::TriggerActiveDeviceChangedCallback(runContext, m_localUser, activeDevice); // no issue if callback not set

                return hr;
            }
            else if (FAILED(hr))
            {
                m_telemetryManager->SetContextDeleteHResult(hr);
                m_telemetryManager->EmitContextDeleteEvent();
                m_telemetryManager->SetContextSyncHResult(hr);
                m_telemetryManager->EmitContextSyncErrorEvent();
                m_telemetryManager->EmitContextSyncEvent();
                return hr;
            }

            return E_PENDING;
        }

        m_telemetryManager->EmitContextDeleteEvent();
        m_telemetryManager->EmitContextSyncEvent();

        m_latestFinalizedManifest = MakeShared<ManifestInternal>(m_uploadStep.GetPostUploadLatestFinalizedPFManifest());
        m_latestPendingManifest = MakeShared<ManifestInternal>(m_uploadStep.GetPostUploadPendingPFManifest());
        m_remoteFileFolderSet = MakeShared<FileFolderSet>(); // this will be automatically filled next time upload is done in CompareStep
    }
    else
    {
        // There's no changes, so nothing to upload
        if (option == PFGameSaveFilesUploadOption::ReleaseDeviceAsActive)
        {
            // if we are releasing the device as active, we need to delete the pending manifest
            if (!m_uploadStep.IsDeletePendingManifestDone())
            {
                HRESULT hr = m_uploadStep.DeletePendingManifest(runContext, task, m_latestPendingManifest, folderSyncMutex);
                if (FAILED(hr))
                {
                    m_telemetryManager->SetContextDeleteHResult(hr);
                    m_telemetryManager->EmitContextDeleteEvent();
                    return hr;
                }

                return E_PENDING;
            }

            m_telemetryManager->EmitContextDeleteEvent();

            m_deviceReleasedAsActive = true;
        }
    }

    SetSyncStateProgress(PFGameSaveFilesSyncState::SyncComplete, 0, 0);
    SetStatsForDebug();

    return S_OK;
}

HRESULT FolderSyncManager::DoWorkResetCloud(_In_ const RunContext& runContext, _In_ ISchedulableTask& task, _In_ std::recursive_mutex& folderSyncMutex)
{
    std::lock_guard<std::recursive_mutex> lock(folderSyncMutex); // Prevent any of the Finally blocks from changing the state while the DoWork thread is active
    ScopeTracer scopeTracer("FolderSyncManager::DoWorkResetCloud");

    if (runContext.CancellationToken().IsCancelled())
    {
        return E_ABORT;
    }

    // 1. Reset cloud
    if (!m_resetCloudStep.IsResetDone())
    {
        HRESULT hr = m_resetCloudStep.ResetCloud(
            runContext,
            task,
            m_saveFolder,
            folderSyncMutex
        );
        if (FAILED(hr))
        {
            m_telemetryManager->SetContextDeleteHResult(hr);
            m_telemetryManager->EmitContextDeleteEvent();
            return hr;
        }

        return E_PENDING;
    }

    return S_OK;
}

HRESULT FolderSyncManager::DoWorkSetSaveDescription(_In_ const RunContext& runContext, _In_ ISchedulableTask& task, _In_ std::recursive_mutex& folderSyncMutex, _In_ const String& shortSaveDescription)
{
    std::lock_guard<std::recursive_mutex> lock(folderSyncMutex); // Prevent any of the Finally blocks from changing the state while the DoWork thread is active
    ScopeTracer scopeTracer("FolderSyncManager::DoWorkSetSaveDescription");

    if (runContext.CancellationToken().IsCancelled())
    {
        return E_ABORT;
    }

    // 1. SetSaveDescription
    if (!m_setSaveDescriptionStep.IsSetDone())
    {
        HRESULT hr = m_setSaveDescriptionStep.SetSaveDescription(
            runContext,
            task,
            folderSyncMutex,
            shortSaveDescription,
            m_latestPendingManifest
        );
        if (FAILED(hr))
        {
            return hr;
        }

        return E_PENDING;
    }

    return S_OK;
}


HRESULT FolderSyncManager::InitForDownload()
{
    m_lockStep.Reset();
    m_compareStep.Reset();
    m_downloadStep.Reset();
    return S_OK;
}

HRESULT FolderSyncManager::InitForUpload()
{
    m_compareStep.Reset();
    m_uploadStep.Reset();
    return S_OK;
}

int64_t FolderSyncManager::GetRemainingQuota() const
{
    // Retrieve per-player quota parsed in LockStep. INT64_MAX means unlimited.
    int64_t perPlayerQuota = m_lockStep.GetPerPlayerQuotaBytes();
    if (perPlayerQuota == std::numeric_limits<int64_t>::max())
    {
        return std::numeric_limits<int64_t>::max(); // Unlimited
    }

    uint64_t localSize = 0;
    if (m_localFileFolderSet)
    {
        localSize = m_localFileFolderSet->GetTotalUncompressedSize();
    }

    // Allow negative (over-quota) results per spec.
    int64_t remaining = perPlayerQuota - static_cast<int64_t>(localSize);
    return remaining;
}

const PlayFab::String& FolderSyncManager::GetFolder() const
{
    return m_saveFolder;
}

const PlayFab::String& FolderSyncManager::GetStatsJsonForDebug() const
{
    return m_statsJsonForDebug;
}

UICallbackManager& FolderSyncManager::GetUIManager()
{
    return m_uiManager;
}

void FolderSyncManager::SetSyncStateProgress(_In_ PFGameSaveFilesSyncState state, _In_ uint64_t cur, _In_ uint64_t total)
{
    std::lock_guard<std::mutex> lock{ m_progressMutex };
    m_syncProgress.syncState = state;
    m_syncProgress.current = cur;
    m_syncProgress.total = total;
}

FolderSyncManagerProgress FolderSyncManager::GetSyncProgress()
{
    FolderSyncManagerProgress result;
    {
        std::lock_guard<std::mutex> lock{ m_progressMutex };
        result = m_syncProgress;
    }
    return result;
}

void FolderSyncManager::ResetSetSaveDescriptionStep()
{
    m_setSaveDescriptionStep.Reset();
}

void FolderSyncManager::SetLastShortSaveDescription(const String& shortSaveDescription)
{
    m_lastShortSaveDescription = shortSaveDescription;
}

HRESULT FolderSyncManager::ConvertToPFGameSaveDescriptor(const ManifestWrap& manifest, PFGameSaveDescriptor& gameSave)
{
    gameSave.time = manifest.GetCreationTimestamp();
    auto uploadProgress = manifest.GetUploadProgress();
    if (uploadProgress.has_value())
    {
        gameSave.totalBytes = StringToUint64(uploadProgress.value().GetTotalBytes());
        gameSave.uploadedBytes = StringToUint64(uploadProgress.value().GetUploadedBytes());
    }
    else
    {
        gameSave.totalBytes = 0;
        gameSave.uploadedBytes = 0;
    }

    auto metadata = manifest.GetMetadata();
    if (metadata.has_value())
    {
        String deviceType = manifest.GetMetadata()->GetDeviceType();
        String deviceId = metadata.value().GetDeviceId();
        String deviceFriendlyName = metadata.value().GetDeviceName();
        StrCpy(gameSave.deviceType, sizeof(gameSave.deviceType), deviceType.c_str());
        StrCpy(gameSave.deviceId, sizeof(gameSave.deviceId), deviceId.c_str());
        StrCpy(gameSave.deviceFriendlyName, sizeof(gameSave.deviceFriendlyName), deviceFriendlyName.c_str());
        
    }
    else
    {
        gameSave.deviceType[0] = '\0';
        gameSave.deviceId[0] = '\0';
        gameSave.deviceFriendlyName[0] = '\0';
    }

    String manifestDescription = manifest.GetManifestDescription();
    StrCpy(gameSave.shortSaveDescription, sizeof(gameSave.shortSaveDescription), manifestDescription.c_str());
    gameSave.thumbnailUri[0] = '\0'; // No thumbnail URI in manifest, might be set after

    return S_OK;
}

} // namespace GameSave
} // namespace PlayFab
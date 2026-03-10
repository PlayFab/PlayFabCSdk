// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "FolderSyncManager.h"
#include "ProgressHelpers.h"
#include "Platform/Platform.h"
#include "ActiveDevicePollWorker.h"
#include "Wrappers/GameSaveServiceSelector.h"
#include "PlatformUtils.h"
#include "LocalStateManifest.h"

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
    m_localUser{ localUser },
    m_telemetryManager{ MakeShared<GameSaveTelemetryManager>() },
    m_lockStep(localUser, m_telemetryManager),
    m_compareStep(localUser, m_telemetryManager),
    m_downloadStep(localUser, m_telemetryManager),
    m_uploadStep(localUser, m_telemetryManager),
    m_resetCloudStep(localUser, m_telemetryManager),
    m_setSaveDescriptionStep(localUser, m_telemetryManager)
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
    
    // Cancel any pending UI wait to prevent hangs during shutdown
    m_uiManager.CancelPendingUIWait();
}

void FolderSyncManager::CancelPendingUIWaitForCleanup()
{
    TRACE_TASK("FolderSyncManager::CancelPendingUIWaitForCleanup");
    m_uiManager.CancelPendingUIWait();
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
        // Even when offline, create local file folder set so SetSaveDescription can persist to localstate.json
        if (m_localFileFolderSet == nullptr)
        {
            m_localFileFolderSet = MakeShared<FileFolderSet>();
        }
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

        m_latestFinalizedManifest = MakeShared<ManifestInternal>(m_lockStep.GetBaselineFinalizedManifest());
        m_latestPendingManifest = MakeShared<ManifestInternal>(m_lockStep.GetLatestPendingPFManifest());
        m_localFileFolderSet = MakeShared<FileFolderSet>();
        m_remoteFileFolderSet = MakeShared<FileFolderSet>();

        // Record original activation baseline version for later Known Good promotion eligibility.
        if (m_latestFinalizedManifest)
        {
            auto version = m_latestFinalizedManifest->GetManifest().GetVersion();
            uint64_t baselineVer = version.empty() ? 0 : StringToUint64(version);
            m_uploadStep.SetOriginalActivationBaselineVersion(baselineVer);
            TRACE_INFORMATION("[GAME SAVE] Recorded original activation baseline v:%llu for Known Good promotion criteria", baselineVer);
        }
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

    // Restore the description loaded from localstate.json (persists offline descriptions across reinitialize)
    const String& loadedDescription = m_compareStep.GetLoadedShortSaveDescription();
    bool loadedDirty = m_compareStep.GetLoadedDescriptionDirty();
    if (!loadedDescription.empty() && m_lastShortSaveDescription.empty())
    {
        m_lastShortSaveDescription = loadedDescription;
        m_descriptionDirty = loadedDirty;
        TRACE_INFORMATION("[GAME SAVE] Restored shortSaveDescription='%s' dirty=%d from localstate.json", loadedDescription.c_str(), loadedDirty);
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

    // Conflict upload integration: if a conflict occurred during compare, perform a single upload of local divergent branch
    if (m_compareStep.ConflictRequiresUpload() && !m_conflictUploadCompleted)
    {
        // Prepare sets if first time
        if (!m_conflictUploadStarted)
        {
            // Mark files/folders scheduled in compare for upload; UploadStep will inspect vectors
            // Always keep device active after AddUser (so we request KeepDeviceActive)
            uint64_t origBaseline = m_uploadStep.GetOriginalActivationBaselineVersion();
            m_uploadStep.Reset();
            if (origBaseline != 0)
            {
                m_uploadStep.SetOriginalActivationBaselineVersion(origBaseline);
            }
            m_conflictUploadStarted = true;
            TRACE_INFORMATION("[GAME SAVE] ConflictUpload: starting single upload during AddUser path");
        }

        // Calculate conflict metadata BEFORE starting upload (needed for FinalizeManifest)
        ConflictMetadata conflictMetadata;
        if (m_compareStep.DidConflictOccur())
        {
            uint64_t baselineVer = m_uploadStep.GetOriginalActivationBaselineVersion();
            TakeUIChoice choice = m_compareStep.GetConflictChoice();
            
            switch (choice)
            {
                case TakeUIChoice::TakeLocal:
                    // User chose local. New upload is winner; baseline cloud version is loser.
                    conflictMetadata = ConflictMetadata(true, Uint64ToString(baselineVer));
                    TRACE_INFORMATION("[GAME SAVE] ConflictUpload: KEEP_LOCAL - new upload wins against cloud v%llu", baselineVer);
                    break;
                case TakeUIChoice::TakeRemote:
                    // User chose remote. New upload is loser; baseline cloud version is winner.
                    conflictMetadata = ConflictMetadata(false, Uint64ToString(baselineVer));
                    TRACE_INFORMATION("[GAME SAVE] ConflictUpload: KEEP_CLOUD - new upload loses to cloud v%llu", baselineVer);
                    break;
                default:
                    TRACE_WARNING("[GAME SAVE] ConflictUpload: Unknown conflict choice, no metadata set");
                    break;
            }
        }

        if (!m_uploadStep.IsUploadDone())
        {
            HRESULT hrUp = m_uploadStep.Upload(
                runContext, task,
                m_latestPendingManifest, m_localFileFolderSet, m_remoteFileFolderSet,
                m_saveFolder, PFGameSaveFilesUploadOption::KeepDeviceActive,
                m_uiManager, m_syncProgress, folderSyncMutex,
                FolderSyncManagerProgressCallback, this,
                m_lastShortSaveDescription, conflictMetadata);
            if (FAILED(hrUp))
            {
                if (hrUp == E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD)
                {
                    SetForcedDisconnectFromCloud(true);
                }
                return hrUp; // propagate failure or pending
            }
            return E_PENDING; // continue pumping until upload completes
        }

        // Upload finished
        m_conflictUploadCompleted = true;
        m_descriptionDirty = false;  // Clear dirty flag - description is now synced to cloud
        TRACE_INFORMATION("[GAME SAVE] ConflictUpload: completed single upload during AddUser path");
        // Update latest manifests from upload step output (post-upload vectors)
        const ManifestWrap& postLatestFinal = m_uploadStep.GetPostUploadLatestFinalizedPFManifest();
        if (!postLatestFinal.GetVersion().empty())
        {
            m_latestFinalizedManifest = MakeShared<ManifestInternal>(postLatestFinal);
        }
        const ManifestWrap& postPending = m_uploadStep.GetPostUploadPendingPFManifest();
        if (!postPending.GetVersion().empty())
        {
            m_latestPendingManifest = MakeShared<ManifestInternal>(postPending);
        }
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
            // Use locally stored description if it was set offline (dirty), otherwise use cloud description
            String shortSaveDescription = m_descriptionDirty 
                ? m_lastShortSaveDescription
                : m_latestFinalizedManifest->GetDecodedManifestDescription();

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

    // After download completes, update local description with cloud description if local wasn't dirty
    // This ensures GetSaveDescription returns the cloud description when local hasn't been modified offline
    if (!m_descriptionDirty && m_latestFinalizedManifest)
    {
        String cloudDescription = m_latestFinalizedManifest->GetDecodedManifestDescription();
        if (!cloudDescription.empty() && cloudDescription != m_lastShortSaveDescription)
        {
            TRACE_INFORMATION("[GAME SAVE] Updating local description to cloud description='%s' (dirty=false)", cloudDescription.c_str());
            SetLastShortSaveDescription(cloudDescription, false);
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
    JsonValue rootJson = JsonValue::object();

    JsonValue fileDownloadJsonArray = JsonValue::array();
    if (m_remoteFileFolderSet)
    {
        const PlayFab::Vector<const FileDetail*>& filesToDownload = m_remoteFileFolderSet->GetFilesToDownload();
        for (const FileDetail* fileToDownload : filesToDownload)
        {
            JsonValue jsonObj = JsonValue::object();
            JsonUtils::ObjectAddMember(jsonObj, "Name", fileToDownload->fileName);
            JsonUtils::ObjectAddMember(jsonObj, "Size", fileToDownload->fileSizeBytes);
            JsonUtils::ObjectAddMember(jsonObj, "TimeCreated", fileToDownload->timeCreated);
            JsonUtils::ObjectAddMember(jsonObj, "TimeLastModified", fileToDownload->timeLastModified);
            fileDownloadJsonArray.push_back(jsonObj);
        }
    }
    JsonUtils::ObjectAddMember(rootJson, "FilesToDownload", std::move(fileDownloadJsonArray));

    JsonValue fileUploadedJsonArray = JsonValue::array();
    if (m_localFileFolderSet)
    {
        const PlayFab::Vector<const FileDetail*>& filesToUpload = m_localFileFolderSet->GetFilesToUpload();
        for (const FileDetail* fileToUpload : filesToUpload)
        {
            JsonValue jsonObj = JsonValue::object();
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
        JsonValue fileCompressedToDownloadJsonArray = JsonValue::array();
        for (const size_t index : compressedFilesToDownload)
        {
            JsonValue jsonObj = JsonValue::object();
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

    JsonValue fileCompressedToUploadJsonArray = JsonValue::array();
    if (m_localFileFolderSet)
    {
        Vector<ExtendedManifestCompressedFileDetail> compressedFilesToUpload = m_localFileFolderSet->GetCompressedFilesToUpload();
        for (const ExtendedManifestCompressedFileDetail& file : compressedFilesToUpload)
        {
            JsonValue jsonObj = JsonValue::object();
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

    JsonValue allFilesArray = JsonValue::array();
    if (m_localFileFolderSet)
    {
        const PlayFab::Vector<PlayFab::GameSave::FileDetail>& skippedFiles = m_localFileFolderSet->GetSkippedFiles();
        for (const PlayFab::GameSave::FileDetail& file : skippedFiles)
        {
            JsonValue jsonObj = JsonValue::object();
            JsonUtils::ObjectAddMember(jsonObj, "Name", file.fileName);
            JsonUtils::ObjectAddMember(jsonObj, "Size", file.fileSizeBytes);
            JsonUtils::ObjectAddMember(jsonObj, "SkipFile", file.skipFile);
            allFilesArray.push_back(jsonObj);
        }
    }
    JsonUtils::ObjectAddMember(rootJson, "SkippedFiles", std::move(allFilesArray));

    JsonValue jsonObjUpload = JsonValue::object();
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
                m_lastShortSaveDescription, ConflictMetadata()); // No conflict metadata for normal uploads
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
        
        // Clear dirty flag - description is now synced to cloud
        m_descriptionDirty = false;
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
    TRACE_INFORMATION("[GAME SAVE] FolderSyncManager::DoWorkFolderUpload Done");
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

    // When disconnected from cloud, store description locally only (skip server update)
    // Description will be committed to cloud on next successful upload when reconnected
    if (IsForcedDisconnectFromCloud())
    {
        TRACE_INFORMATION("DoWorkSetSaveDescription: offline - storing description locally only (dirty=true)");
        SetLastShortSaveDescription(shortSaveDescription, true);  // Mark dirty so it persists through reconnect
        return S_OK;
    }

    if (m_latestPendingManifest == nullptr)
    {
        // No manifest available yet - user may not have completed initial sync
        TRACE_ERROR("DoWorkSetSaveDescription: no pending manifest available");
        return E_PF_GAMESAVE_USER_NOT_ADDED;
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
            // Network call failed - fall back to storing description locally with dirty flag
            // Description will be synced to cloud on next successful upload
            TRACE_INFORMATION("DoWorkSetSaveDescription: network call failed (hr=0x%08X), storing description locally (dirty=true)", hr);
            SetLastShortSaveDescription(shortSaveDescription, true);
            m_setSaveDescriptionStep.Reset();  // Reset step so it can be retried on next call
            return S_OK;  // Return success - description is stored locally
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
    m_isForcedDisconnectFromCloud = false;
    m_latestFinalizedManifest = nullptr;
    m_latestPendingManifest = nullptr;
    m_localFileFolderSet = nullptr;
    m_remoteFileFolderSet = nullptr;
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

void FolderSyncManager::SetLastShortSaveDescription(const String& shortSaveDescription, bool dirty)
{
    m_lastShortSaveDescription = shortSaveDescription;
    m_descriptionDirty = dirty;
    
    // Persist to localstate.json so description survives across sessions and is available for conflict UI
    if (m_localFileFolderSet && !m_saveFolder.empty())
    {
        HRESULT hr = LocalStateManifest::WriteLocalManifest(m_saveFolder, m_localFileFolderSet, shortSaveDescription, dirty);
        if (FAILED(hr))
        {
            TRACE_WARNING("[GAME SAVE] SetLastShortSaveDescription: Failed to persist to localstate.json hr=0x%08X", hr);
        }
        else
        {
            TRACE_INFORMATION("[GAME SAVE] SetLastShortSaveDescription: Persisted description='%s' dirty=%d to localstate.json", shortSaveDescription.c_str(), dirty);
        }
    }
}

String FolderSyncManager::GetSaveDescriptionForDebug() const
{
    // Prefer locally set description (from SetSaveDescriptionAsync or deferred) as it's the most recent.
    // This ensures that after SetSaveDescriptionAsync succeeds, we return the new description
    // even if m_latestFinalizedManifest hasn't been refreshed yet.
    if (!m_lastShortSaveDescription.empty())
    {
        return m_lastShortSaveDescription;
    }
    // Fall back to manifest description
    if (m_latestFinalizedManifest)
    {
        return m_latestFinalizedManifest->GetDecodedManifestDescription();
    }
    return String{};
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

    // Decode the base64-encoded description for client consumption
    String manifestDescription = Base64Decode(manifest.GetManifestDescription());
    if (manifestDescription.empty())
    {
        TRACE_WARNING("[GAME SAVE] ConvertToPFGameSaveDescriptor: ManifestDescription is EMPTY for manifest v:%s", manifest.GetVersion().c_str());
    }
    else
    {
        TRACE_INFORMATION("[GAME SAVE] ConvertToPFGameSaveDescriptor: ManifestDescription='%s' for manifest v:%s", manifestDescription.c_str(), manifest.GetVersion().c_str());
    }
    StrCpy(gameSave.shortSaveDescription, sizeof(gameSave.shortSaveDescription), manifestDescription.c_str());
    gameSave.thumbnailUri[0] = '\0'; // No thumbnail URI in manifest, might be set after

    return S_OK;
}

} // namespace GameSave
} // namespace PlayFab
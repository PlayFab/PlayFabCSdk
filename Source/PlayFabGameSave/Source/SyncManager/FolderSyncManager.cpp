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

namespace
{

// Deletes the extraction temporary files (IsArchiveExtractTempFileName) a crash or a failed cleanup
// left in the save root, where a full-sync extraction writes them. A file of that name this device
// has a record of is the title's own (the scan never records a temporary file) and stays. Only safe
// where no extraction can be in flight for the user: AddUser's local operations.
void SweepArchiveExtractTempFiles(_In_ const String& saveFolder, _In_ const SharedPtr<FileFolderSet>& localFileFolderSet)
{
    if (saveFolder.empty() || !FilePAL::DoesDirectoryExist(saveFolder))
    {
        return;
    }
    Result<Vector<String>> filesResult = FilePAL::EnumFiles(saveFolder);
    if (FAILED(filesResult.hr))
    {
        return;
    }
    for (const String& fileName : filesResult.Payload())
    {
        if (!IsArchiveExtractTempFileName(fileName))
        {
            continue;
        }
        bool recorded = false;
        if (localFileFolderSet != nullptr)
        {
            const Vector<FolderDetail>& folders = localFileFolderSet->GetFolders();
            for (const FileDetail& file : localFileFolderSet->GetFiles())
            {
                if (file.fileName == fileName && file.folderIndex < folders.size() && folders[file.folderIndex].folderName.empty())
                {
                    recorded = true;
                    break;
                }
            }
        }
        String fullPath;
        if (recorded || FAILED(JoinPathHelper(saveFolder, fileName, fullPath)))
        {
            continue;
        }
        HRESULT hr = FilePAL::DeleteLocalFile(fullPath);
        TRACE_INFORMATION("[GAME SAVE] Removed a leftover extraction temporary file %s hr=0x%08X", fullPath.c_str(), static_cast<uint32_t>(hr));
    }
}

} // anonymous namespace

FolderSyncManager::FolderSyncManager(_In_ LocalUser const& localUser) :
    m_localUser{ localUser },
    m_telemetryManager{ MakeShared<GameSaveTelemetryManager>() },
    m_lockStep(localUser, m_telemetryManager),
    m_relockStep(localUser),
    m_compareStep(localUser, m_telemetryManager),
    m_downloadStep(localUser, m_telemetryManager),
    m_uploadStep(localUser, m_telemetryManager),
    m_resetCloudStep(localUser, m_telemetryManager),
    m_setSaveDescriptionStep(localUser, m_telemetryManager)
{
    // context_sync_error is sent once per attempt: a SyncFailed Retry starts a new one, so a
    // different failure after it is reported too (the failure before it already was).
    m_uiManager.SetSyncFailedRetryObserver([telemetry = m_telemetryManager]()
    {
        telemetry->RearmContextSyncError();
    });

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
            m_saveFolder = state->GetInitArgsSaveRootFolder(); // Might be empty if not set by init args on some platforms.
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
    std::lock_guard<std::recursive_mutex> lock(folderSyncMutex); // Prevent any of the Finally blocks from changing the state while the DoWork thread is active
    ScopeTracer scopeTracer("FolderSyncManager::DoWorkFolderDownload");

    // Early cancellation check (user called XAsyncCancel on AddUserWithUiAsync)
    if (runContext.CancellationToken().IsCancelled())
    {
        FireActivationFailedTelemetry(E_ABORT, m_lockStep.IsForceDisconnectFromCloud());
        SetSyncStateProgress(PFGameSaveFilesSyncState::NotStarted, 0, 0);
        return E_ABORT;
    }

    // Debug fault injection: holds (Pause), fails or abandons an AddUser at its start.
    {
        HRESULT faultHr = S_OK;
        switch (CheckFaultPointForDebug("AddUser.Start", faultHr))
        {
            case PFGameSaveFaultAction::Pause:
                task.ScheduleAfter(20);
                return E_PENDING;
            case PFGameSaveFaultAction::Fail:
                FireActivationFailedTelemetry(faultHr, false);
                return faultHr;
            case PFGameSaveFaultAction::Abandon:
                return E_ABORT;
            default:
                break;
        }
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
        CompleteActivation();
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

        m_rollbackCommitPending = m_lockStep.IsRollbackBaselineSelected();
        if (m_rollbackCommitPending)
        {
            TRACE_INFORMATION("[GAME SAVE] Rollback to v:%s selected; the next upload will commit it as the latest save even without local changes",
                m_latestFinalizedManifest->GetManifest().GetVersion().c_str());
        }

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
        CompleteActivation();
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
                FireActivationFailedTelemetry(hrUp, false);
                return hrUp; // propagate failure or pending
            }
            return E_PENDING; // continue pumping until upload completes
        }

        // Upload finished
        m_conflictUploadCompleted = true;
        m_descriptionDirty = false;  // Clear dirty flag - description is now synced to cloud
        // With Keep Local the conflict upload is the winner and the newest non-loser manifest, so a
        // pending rollback no longer needs committing. With Keep Cloud the upload is the loser and the
        // save the player rolled back from is still the latest non-loser, so the commit stays pending.
        if (m_compareStep.GetConflictChoice() == TakeUIChoice::TakeLocal)
        {
            m_rollbackCommitPending = false;
        }
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

        // Re-acquire game storage if it was released during the conflict upload's compression step.
        // The download step needs access to game files to write downloaded content.
        SharedPtr<GameSaveGlobalState> globalState;
        if (SUCCEEDED(GameSaveGlobalState::Get(globalState)))
        {
            String acquiredPath;
            HRESULT acquireHr = globalState->ApiProvider().AcquireGameStorage(acquiredPath);
            if (FAILED(acquireHr))
            {
                TRACE_ERROR("[GAME SAVE] Failed to re-acquire game storage after conflict upload hr=0x%08X", acquireHr);
                return acquireHr;
            }
            // Update save folder in case the mount point changed (e.g., /savedata0 -> /savedata1)
            if (!acquiredPath.empty())
            {
                m_saveFolder = acquiredPath;
                TRACE_INFORMATION("[GAME SAVE] Updated save folder to '%s' after re-acquiring game storage", m_saveFolder.c_str());
            }
        }
    }

    m_telemetryManager->EmitContextActivationEvent();

    // Ensure game storage is fully acquired before local file operations begin.
    // Some platforms defer write preparation until this point so that the storage
    // transaction window does not span earlier network/UI steps.
    if (!m_downloadStep.IsLocalOperationsDone())
    {
        SharedPtr<GameSaveGlobalState> gsAcquire;
        if (FAILED(GameSaveGlobalState::Get(gsAcquire)))
        {
            TRACE_ERROR("[GAME SAVE] Failed to get global state before local operations");
            return E_UNEXPECTED;
        }

        String acquiredPath;
        HRESULT acquireHr = gsAcquire->ApiProvider().AcquireGameStorage(acquiredPath);
        if (FAILED(acquireHr))
        {
            TRACE_ERROR("[GAME SAVE] Failed to acquire game storage before local operations hr=0x%08X", acquireHr);
            return acquireHr;
        }
        if (!acquiredPath.empty())
        {
            m_saveFolder = acquiredPath;
        }

        // Persist device ID to info.json if it was generated before storage was writable
        EnsureDeviceIdPersisted(m_saveFolder);

        // Reclaim upload staging directories orphaned by a crash, a suspend, or an upload whose
        // provider was torn down mid-transfer. AddUser is the one point where this is safe: an
        // upload cannot be in flight for this user here, because TryReserveUpload requires the
        // user to be added and TryReserveDownload rejects once they are. Deferring cleanup to
        // this point is what lets an upload attempt stop deleting other attempts' staged files.
        //
        // Done after AcquireGameStorage so m_saveFolder is the final, mounted path.
        SweepUploadStagingFolders(m_saveFolder);
        // Extraction temporary files a crash (or a failed delete) left in the save root: the scan
        // already ignored them, and no extraction is in flight here.
        SweepArchiveExtractTempFiles(m_saveFolder, m_localFileFolderSet);
    }

    if (!m_downloadStep.IsLocalOperationsDone())
    {
        m_telemetryManager->ResetContextSync();
        m_telemetryManager->SetContextSyncStartTime();
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
            DropBaselinesOfAppliedRemoteDeletes();
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
        CompleteActivation();
        SetSyncStateProgress(PFGameSaveFilesSyncState::NotStarted, 0, 0);
        SetStatsForDebug();
        return S_OK;
    }

    if (!m_downloadStep.HasStarted())
    {
        // Debug fault point (core unit test plan H-5): the local deletes and folder creates are done,
        // no transfer has started.
        HRESULT faultResult = S_OK;
        if (StopAtStageFaultPointForDebug("Download.AfterLocalOps", task, E_PENDING, faultResult))
        {
            if (FAILED(faultResult) && faultResult != E_PENDING)
            {
                FireActivationFailedTelemetry(faultResult, false);
            }
            return faultResult;
        }
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

            // OQ-15: Retry after an expired link fetches fresh links for the version being downloaded.
            m_downloadStep.SetLinkRefreshVersion(m_latestFinalizedManifest->Version());
            HRESULT hr = m_downloadStep.Download(runContext, task, m_saveFolder, m_uiManager, m_localFileFolderSet, m_remoteFileFolderSet, m_syncProgress, folderSyncMutex, FolderSyncManagerProgressCallback, this, shortSaveDescription, m_descriptionDirty);
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

    // The download succeeded: report success even when an earlier attempt failed and was retried.
    m_telemetryManager->SetContextSyncHResult(S_OK);
    m_telemetryManager->EmitContextSyncEvent();

    // Ensure the game storage marker exists. This covers the case where no files
    // were downloaded (e.g. new user with no cloud data), which skips DownloadStep
    // entirely and would otherwise leave the marker missing. Without the marker,
    // IsGameStorageWiped() cannot distinguish a genuinely wiped container from one
    // that was never marked.
    HRESULT markerHr = EnsureGameStorageMarker(m_saveFolder);
    if (FAILED(markerHr))
    {
        TRACE_WARNING("[GAME SAVE] FolderSyncManager: EnsureGameStorageMarker failed hr=0x%08X (non-fatal)", markerHr);
    }

    CompleteActivation();
    SetSyncStateProgress(PFGameSaveFilesSyncState::SyncComplete, 0, 0);
    SetStatsForDebug();
    m_pollingForActiveDeviceChange = true;

    SharedPtr<GameSaveGlobalState> state;
    HRESULT hr = GameSaveGlobalState::Get(state);
    if (SUCCEEDED(hr))
    {
        // Stop the previous poller before replacing it. A re-AddUser that isn't an offline
        // reconnect (e.g. after Upload(ReleaseDeviceAsActive)) leaves the old worker running -
        // it resubmits itself after every poll and its only other exit is a forced disconnect,
        // which this AddUser just cleared - so each cycle would add one more permanent poller.
        if (m_tokenRefreshWorker)
        {
            m_tokenRefreshWorker->Stop();
        }

        // runContext is created via state->RunContext().DeriveOnQueue(async->queue));
        // so use original glboal's RunContext() for background queue
        auto backgroundRunContext = state->RunContext().Derive();
        std::optional<Entity> entity = m_lockStep.GetEntity();
        m_tokenRefreshWorker = ActiveDevicePollWorker::MakeAndStart(entity.value(), m_localUser, std::move(backgroundRunContext));
    }
    else
    {
        TRACE_ERROR("[GAME SAVE] Failed to get GameSaveGlobalState for starting ActiveDevicePollWorker, hr=0x%08X", hr);
    }

    return S_OK;
}

void FolderSyncManager::DropBaselinesOfAppliedRemoteDeletes()
{
    // The cloud deleted these files and so did this AddUser: they no longer have a sync baseline.
    // localstate.json is otherwise only rewritten by a download (which re-reads it from disk first)
    // or an upload, so the baseline survived: the next upload asked the cloud to delete the file
    // again, and its re-creation on another device read as "deleted here, changed there" (a
    // spurious conflict prompt).
    bool dropped = false;
    for (const FileDetail* deleted : m_localFileFolderSet->GetFilesToDeleteUponDownload())
    {
        String fullFilePath;
        if (FAILED(JoinPathHelper(m_saveFolder, m_localFileFolderSet->GetRelFilePath(deleted), fullFilePath)) || FilePAL::DoesFileExist(fullFilePath))
        {
            continue; // still on disk (a delete that failed): keep its baseline
        }
        deleted->existsLocally = false;
        if (deleted->hasLastSync)
        {
            deleted->hasLastSync = false;
            dropped = true;
        }
    }
    if (dropped)
    {
        HRESULT writeHr = LocalStateManifest::WriteLocalManifest(m_saveFolder, m_localFileFolderSet, m_lastShortSaveDescription, m_descriptionDirty);
        if (FAILED(writeHr))
        {
            // Non-fatal: the baselines stay as they were before this AddUser.
            TRACE_WARNING("[GAME SAVE] FolderSyncManager: writing localstate.json after remote deletes failed hr=0x%08X", writeHr);
        }
    }
}

void FolderSyncManager::SetStatsForDebug()
{
#if defined(_DEBUG)
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

    // The telemetry events this user's manager emitted (oldest first, payload without the common
    // fields), so unit tests can assert them with the mock service (core unit test plan U23).
    {
        JsonValue telemetryJson = JsonValue::array();
        for (const auto& emitted : m_telemetryManager->GetEmittedEventsForDebug())
        {
            JsonValue eventJson = JsonValue::object();
            JsonUtils::ObjectAddMember(eventJson, "Name", emitted.first);
            JsonValue payload;
            try
            {
                payload = JsonValue::parse(emitted.second);
            }
            catch (...)
            {
                payload = JsonValue::object();
            }
            JsonUtils::ObjectAddMember(eventJson, "Payload", std::move(payload));
            telemetryJson.push_back(std::move(eventJson));
        }
        JsonUtils::ObjectAddMember(rootJson, "Telemetry", std::move(telemetryJson));
    }

    m_statsJsonForDebug = JsonUtils::WriteToString(rootJson);
#endif
}

void FolderSyncManager::ReleaseUploadStaging()
{
    DeleteUploadStagingFolder(m_uploadStep.GetUploadStagingFolder());
    m_uploadStep.ClearUploadStagingFolder();
}

void FolderSyncManager::RecoverFailedUpload(HRESULT hr)
{
    // Finalization may have succeeded even if a later request or UI wait was cancelled.
    // Preserve its result before InitForUpload resets the step on the next attempt.
    if (m_uploadStep.HasStartedFinalizeManifest())
    {
        const ManifestWrap& postFinal = m_uploadStep.GetPostUploadLatestFinalizedPFManifest();
        if (!postFinal.GetVersion().empty())
        {
            m_latestFinalizedManifest = MakeShared<ManifestInternal>(postFinal);
            // Finalization also committed any pending rollback; don't commit it a second time.
            m_rollbackCommitPending = false;
        }
        m_latestPendingManifest = nullptr;
        m_remoteFileFolderSet = MakeShared<FileFolderSet>();
        m_descriptionDirty = false;
        m_compareStep.Reset();
        m_uploadStep.Reset();
        TRACE_WARNING("[GAME SAVE] DoWorkFolderUpload: upload failed post-finalize (HR:0x%0.8x), cleared stale pending manifest and reset steps", hr);
    }
}

HRESULT FolderSyncManager::DoWorkFolderUpload(_In_ RunContext& runContext, _In_ ISchedulableTask& task, _In_ std::recursive_mutex& folderSyncMutex, _In_ PFGameSaveFilesUploadOption option)
{
    std::lock_guard<std::recursive_mutex> lock(folderSyncMutex); // Prevent any of the Finally blocks from changing the state while the DoWork thread is active
    ScopeTracer scopeTracer("FolderSyncManager::DoWorkFolderUpload");

    // Early cancellation check (user called XAsyncCancel on UploadWithUiAsync)
    if (runContext.CancellationToken().IsCancelled())
    {
        RecoverFailedUpload(E_ABORT);
        return E_ABORT;
    }

    if (IsForcedDisconnectFromCloud())
    {
        // This error is returned regardless if the network is restored as the user is still considered offline.
        return E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD;
    }

    // ForDebug: simulate post-TakeLock-cancel state by nulling pending manifest
    {
        bool fnp = GetForceNullPendingManifest();
        if (fnp && m_latestPendingManifest != nullptr && m_latestFinalizedManifest != nullptr)
        {
            m_latestPendingManifest = nullptr;
            ClearForceNullPendingManifest();
        }
    }

    if (m_latestPendingManifest == nullptr && m_latestFinalizedManifest != nullptr)
    {
        // Previous upload's TakeLock failed/cancelled after FinalizeManifest.
        // Re-acquire a pending manifest via RelockStep before upload can proceed.
        if (m_relockStep.IsRelockDone())
        {
            if (m_relockStep.IsForceDisconnectFromCloud())
            {
                TRACE_INFORMATION("[GAME SAVE] DoWorkFolderUpload: RelockStep completed with UseOffline, disconnecting");
                SetForcedDisconnectFromCloud(true);
                m_relockStep.Reset();
                return E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD;
            }
            m_latestPendingManifest = m_relockStep.ExtractPendingManifest();
            m_relockStep.Reset();
            TRACE_INFORMATION("[GAME SAVE] DoWorkFolderUpload: RelockStep completed, pending manifest re-acquired");
        }
        else
        {
            auto entity = m_lockStep.GetEntity();
            if (!entity.has_value())
            {
                TRACE_ERROR("[GAME SAVE] DoWorkFolderUpload: cannot re-acquire lock, no entity");
                return E_UNEXPECTED;
            }

            HRESULT hr = m_relockStep.Relock(runContext, task, m_uiManager, folderSyncMutex,
                m_saveFolder, entity.value(), m_latestFinalizedManifest->Version());
            if (FAILED(hr))
            {
                if (m_relockStep.IsForceDisconnectFromCloud())
                {
                    SetForcedDisconnectFromCloud(true);
                }
                m_relockStep.Reset();
                return hr;
            }
            return E_PENDING;
        }
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
            m_telemetryManager->SetContextSyncHResult(hr);
            m_telemetryManager->EmitContextSyncErrorEvent();
            m_telemetryManager->EmitContextSyncEvent();
            return hr;
        }

        return E_PENDING;
    }

    if (m_localFileFolderSet->GetFilesToUpload().size() > 0 ||
        m_localFileFolderSet->GetFilesToDeleteUponUpload().size() > 0 ||
        m_localFileFolderSet->GetFoldersToCreateUponUpload().size() > 0 ||
        m_localFileFolderSet->GetFoldersToDeleteUponUpload().size() > 0 ||
        m_rollbackCommitPending)
    {
        // 2. Upload what's needed w/ progress UX if needed
        if (!m_uploadStep.IsUploadDone())
        {
            m_uploadStep.SetForceCommit(m_rollbackCommitPending);
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

                // b) queue the active device change callback. It is fired by UploadAsyncProvider
                // (FirePendingActiveDeviceChangedCallback) only after the provider has moved the
                // sync state out of the upload states. Firing it here, while this upload still owns
                // admission, let a title that re-adds the user from the callback (the documented
                // recovery) race the provider's teardown: TryReserveDownload saw an upload in flight
                // and rejected the reconnect with E_PF_GAMESAVE_OPERATION_IN_PROGRESS (H17).
                m_pendingActiveDeviceChanged = MakeUnique<PFGameSaveDescriptor>();
                *m_pendingActiveDeviceChanged = PFGameSaveDescriptor{};
                FolderSyncManager::ConvertToPFGameSaveDescriptor(m_latestPendingManifest->GetManifest(), *m_pendingActiveDeviceChanged);

                m_telemetryManager->SetContextSyncHResult(hr);
                m_telemetryManager->EmitContextSyncErrorEvent();
                m_telemetryManager->EmitContextSyncEvent();
                return hr;
            }
            else if (FAILED(hr))
            {
                RecoverFailedUpload(hr);

                m_telemetryManager->SetContextDeleteHResult(hr);
                m_telemetryManager->EmitContextDeleteEvent();
                m_telemetryManager->SetContextSyncHResult(hr);
                m_telemetryManager->EmitContextSyncErrorEvent();
                m_telemetryManager->EmitContextSyncEvent();
                SetStatsForDebug(); // debug stats: a failed upload's telemetry events
                return hr;
            }

            return E_PENDING;
        }

        // The upload succeeded: its events report success even when an earlier attempt of this
        // operation failed and was retried (that attempt's failure is in context_sync_error).
        m_telemetryManager->SetContextDeleteHResult(S_OK);
        m_telemetryManager->EmitContextDeleteEvent();
        m_telemetryManager->SetContextSyncHResult(S_OK);
        m_telemetryManager->EmitContextSyncEvent();

        m_latestFinalizedManifest = MakeShared<ManifestInternal>(m_uploadStep.GetPostUploadLatestFinalizedPFManifest());
        // TakeLock deliberately skips creating a new pending manifest when the device is being
        // released as active, which leaves a version-less manifest here. Wrapping it anyway would
        // produce a non-null pending manifest whose version is "", and a later SetSaveDescription
        // would send UpdateManifest with an empty Version (rejected by the service, description
        // silently lost). Mirror the post-finalize failure path and null it out instead.
        const ManifestWrap& postUploadPending = m_uploadStep.GetPostUploadPendingPFManifest();
        if (!postUploadPending.GetVersion().empty())
        {
            m_latestPendingManifest = MakeShared<ManifestInternal>(postUploadPending);
        }
        else
        {
            m_latestPendingManifest = nullptr;
        }
        m_remoteFileFolderSet = MakeShared<FileFolderSet>(); // this will be automatically filled next time upload is done in CompareStep

        // Clear dirty flag - description is now synced to cloud
        m_descriptionDirty = false;
        // The rolled-back state is now the latest finalized save.
        m_rollbackCommitPending = false;
    }
    else
    {
        // There's no changes to upload, but a description set while offline (or otherwise not yet
        // pushed) still has to reach the cloud - otherwise other devices keep showing stale save
        // metadata until the next time a file happens to change.
        //
        // Skipped for ReleaseDeviceAsActive: that path deletes the pending manifest a few lines
        // below, so writing the description into it would throw the update away and clearing the
        // dirty flag would stop any later session from retrying it.
        if (m_descriptionDirty &&
            option != PFGameSaveFilesUploadOption::ReleaseDeviceAsActive &&
            m_latestPendingManifest != nullptr && m_latestPendingManifest->HasVersion())
        {
            if (!m_setSaveDescriptionStep.IsSetDone())
            {
                HRESULT hr = m_setSaveDescriptionStep.SetSaveDescription(
                    runContext,
                    task,
                    folderSyncMutex,
                    m_lastShortSaveDescription,
                    m_latestPendingManifest
                );
                if (FAILED(hr))
                {
                    // Leave the description cached and dirty so the next upload retries it; a
                    // failed metadata update must not fail the upload itself.
                    TRACE_WARNING("[GAME SAVE] DoWorkFolderUpload: flushing dirty description failed (HR:0x%0.8x), keeping it cached", hr);
                    m_setSaveDescriptionStep.Reset();
                }
                else
                {
                    return E_PENDING;
                }
            }
            else
            {
                m_descriptionDirty = false;
                m_setSaveDescriptionStep.Reset();

                // Persist the cleared flag. Otherwise localstate.json still says "dirty" on the
                // next launch, so the SDK re-pushes an already-synced description and refuses to
                // adopt a newer one written by another device.
                if (m_localFileFolderSet != nullptr && !m_saveFolder.empty())
                {
                    HRESULT writeHr = LocalStateManifest::WriteLocalManifest(m_saveFolder, m_localFileFolderSet, m_lastShortSaveDescription, false /*descriptionDirty*/);
                    if (FAILED(writeHr))
                    {
                        // Couldn't record that the description is synced - keep it dirty so the
                        // in-memory state matches what will be reloaded from disk.
                        TRACE_WARNING("[GAME SAVE] DoWorkFolderUpload: WriteLocalManifest failed after description flush (HR:0x%0.8x)", writeHr);
                        m_descriptionDirty = true;
                    }
                }
            }
        }

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

            // Check if the async delete succeeded
            HRESULT deleteHr = m_uploadStep.GetDeleteManifestFailureHR();
            if (FAILED(deleteHr))
            {
                m_telemetryManager->SetContextDeleteHResult(deleteHr);
                m_telemetryManager->EmitContextDeleteEvent();
                return deleteHr;
            }

            m_telemetryManager->EmitContextDeleteEvent();

            // The pending manifest no longer exists on the service; keeping the pointer would let
            // a later SetSaveDescription issue UpdateManifest against a deleted version.
            m_latestPendingManifest = nullptr;
        }
    }

    if (option == PFGameSaveFilesUploadOption::ReleaseDeviceAsActive)
    {
        m_deviceReleasedAsActive = true;
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

    SetStatsForDebug(); // debug stats: the reset's context_delete events
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
        // No pending manifest available. This can happen in two cases:
        // 1. User has not completed initial sync (AddUser) - m_latestFinalizedManifest is also nullptr
        // 2. Previous upload's TakeLock failed/cancelled after FinalizeManifest - m_latestFinalizedManifest exists
        // In case 2, cache the description locally; it will be synced when the next upload re-acquires the lock.
        if (m_latestFinalizedManifest != nullptr)
        {
            TRACE_INFORMATION("DoWorkSetSaveDescription: pending manifest unavailable (post-finalize failure recovery), storing description locally (dirty=true)");
            SetLastShortSaveDescription(shortSaveDescription, true);
            return S_OK;
        }
        TRACE_ERROR("DoWorkSetSaveDescription: no pending manifest available - user not added");
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
    m_uploadStep.Reset();
    m_conflictUploadStarted = false;
    m_conflictUploadCompleted = false;
    m_isForcedDisconnectFromCloud = false;
    m_rollbackCommitPending = false;
    m_latestFinalizedManifest = nullptr;
    m_latestPendingManifest = nullptr;
    m_localFileFolderSet = nullptr;
    m_remoteFileFolderSet = nullptr;
    m_deviceReleasedAsActive = false;
    return S_OK;
}

HRESULT FolderSyncManager::InitForUpload()
{
    m_compareStep.Reset();
    // Preserve originalActivationBaselineVersion across reset so
    // PromoteIfNeeded can mark the prior baseline as IsKnownGood
    // after a successor is finalized (same pattern as ConflictUpload path).
    uint64_t origBaseline = m_uploadStep.GetOriginalActivationBaselineVersion();
    m_uploadStep.Reset();
    if (origBaseline != 0)
    {
        m_uploadStep.SetOriginalActivationBaselineVersion(origBaseline);
    }
    m_relockStep.Reset();
    m_pendingActiveDeviceChanged.reset();
    return S_OK;
}

SyncOperationKind FolderSyncManager::OperationKindFromState(PFGameSaveFilesSyncState state)
{
    switch (state)
    {
        case PFGameSaveFilesSyncState::PreparingForDownload:
        case PFGameSaveFilesSyncState::Downloading:
            return SyncOperationKind::AddUser;
        case PFGameSaveFilesSyncState::PreparingForUpload:
        case PFGameSaveFilesSyncState::Uploading:
            return SyncOperationKind::Upload;
        default:
            return SyncOperationKind::None;
    }
}

SyncOperationKind FolderSyncManager::OperationKindLocked() const
{
    return OperationKindFromState(m_syncProgress.syncState);
}

SyncOperationKind FolderSyncManager::GetActiveOperationKind()
{
    std::lock_guard<std::mutex> lock{ m_progressMutex };
    return OperationKindLocked();
}

bool FolderSyncManager::TrySetSyncStateIfOwnedBy(std::initializer_list<SyncOperationKind> owners, PFGameSaveFilesSyncState newState)
{
    std::lock_guard<std::mutex> lock{ m_progressMutex };
    SyncOperationKind current = OperationKindLocked();
    if (current == SyncOperationKind::None || std::find(owners.begin(), owners.end(), current) == owners.end())
    {
        return false;
    }
    m_syncProgress.syncState = newState;
    m_syncProgress.current = 0;
    m_syncProgress.total = 0;
    return true;
}

void FolderSyncManager::FirePendingActiveDeviceChangedCallback(_In_ const RunContext& runContext)
{
    if (m_pendingActiveDeviceChanged == nullptr)
    {
        return;
    }
    UniquePtr<PFGameSaveDescriptor> activeDevice = std::move(m_pendingActiveDeviceChanged);
    UICallbackManager::TriggerActiveDeviceChangedCallback(runContext, m_localUser, *activeDevice); // no issue if callback not set
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

    // Clamp localSize to prevent undefined behavior from uint64_t -> int64_t conversion
    if (localSize > static_cast<uint64_t>(std::numeric_limits<int64_t>::max()))
    {
        localSize = static_cast<uint64_t>(std::numeric_limits<int64_t>::max());
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

HRESULT FolderSyncManager::TryReserveUpload(_Out_ PFGameSaveFilesSyncState& previousSyncState)
{
    // The admission test and the state transition have to happen in one critical section:
    // separate GetSyncProgress()/SetSyncStateProgress() calls let two concurrent uploads both
    // observe SyncComplete and both reserve, after which their providers call InitForUpload()
    // on this manager and reset each other's step state machines.
    std::lock_guard<std::mutex> lock{ m_progressMutex };

    previousSyncState = m_syncProgress.syncState;

    if (m_resetCloudReserved)
    {
        // A reset is snapshotting and deleting manifests; an upload finalizing a new manifest
        // underneath it would survive the reset and be reported as deleted.
        return E_PF_GAMESAVE_OPERATION_IN_PROGRESS;
    }

    if (m_syncProgress.syncState == PFGameSaveFilesSyncState::NotStarted)
    {
        return E_PF_GAMESAVE_USER_NOT_ADDED;
    }

    if (m_deviceReleasedAsActive)
    {
        return E_PF_GAMESAVE_DEVICE_NO_LONGER_ACTIVE;
    }

    if (m_syncProgress.syncState == PFGameSaveFilesSyncState::PreparingForDownload ||
        m_syncProgress.syncState == PFGameSaveFilesSyncState::Downloading)
    {
        return E_PF_GAMESAVE_DOWNLOAD_IN_PROGRESS;
    }

    if (m_syncProgress.syncState == PFGameSaveFilesSyncState::PreparingForUpload ||
        m_syncProgress.syncState == PFGameSaveFilesSyncState::Uploading)
    {
        return E_PF_GAMESAVE_OPERATION_IN_PROGRESS;
    }

    // The byte total is genuinely unknown until the save folder is enumerated on the async path.
    // Reporting total == 0 is the documented "not yet known" signal; UploadStep replaces it with a
    // real total before the title's progress callback ever fires.
    m_syncProgress.syncState = PFGameSaveFilesSyncState::PreparingForUpload;
    m_syncProgress.current = 0;
    m_syncProgress.total = 0;
    return S_OK;
}

HRESULT FolderSyncManager::TryReserveDownload(_In_ PFGameSaveFilesAddUserOptions options, _Out_ PFGameSaveFilesSyncState& previousSyncState, _Out_ bool& previousForcedDisconnectFromCloud)
{
    // Same reasoning as TryReserveUpload: the admission checks and the transition to
    // PreparingForDownload have to be one critical section. Reading the reset reservation and the
    // sync state through separate GetSyncProgress()/IsResetCloudReserved() calls let a ResetCloud
    // reserve itself in the gap, after which both operations ran - the reset snapshotted the
    // manifest list while this AddUser created a new pending manifest that survived the
    // "successful" reset. It also let two concurrent AddUser calls both observe NotStarted.
    std::lock_guard<std::mutex> lock{ m_progressMutex };

    previousSyncState = m_syncProgress.syncState;

    // Captured here, under the same lock, so the snapshot and the clear below are atomic with
    // respect to other lock holders. The reconnect path clears this flag before the caller's
    // provider has actually started; if provider startup then fails, the caller restores it,
    // otherwise the manager reports "connected" while no reconnect ever ran.
    previousForcedDisconnectFromCloud = m_isForcedDisconnectFromCloud.load();

    if (m_resetCloudReserved)
    {
        return E_PF_GAMESAVE_OPERATION_IN_PROGRESS;
    }

    // Allow calling AddUserWithUiAsync if:
    // 1. User has never been added (NotStarted state), OR
    // 2. User is disconnected from cloud and wants to reconnect
    //    (per documentation: "When disconnected from cloud, AddUserWithUiAsync() can be called again")
    if (m_syncProgress.syncState != PFGameSaveFilesSyncState::NotStarted)
    {
        bool isReconnectAttempt = m_isForcedDisconnectFromCloud.load();
        if (!isReconnectAttempt && !m_deviceReleasedAsActive)
        {
            // User already added and not in offline mode - reject duplicate AddUser
            return E_PF_GAMESAVE_USER_ALREADY_ADDED;
        }

        if (m_deviceReleasedAsActive)
        {
            TRACE_INFORMATION("[GAME SAVE] AddUserWithUiAsync: allowing re-add after device release");
        }

        // H17: a forced disconnect (the active-device poller) can land while an upload or AddUser
        // still owns this manager. Admitting the reconnect then would reset the shared steps and
        // file sets (InitForDownload) underneath that operation's live continuations. Reject until
        // it drains; its provider moves the state out of the in-flight states when it completes.
        // An upload that ends disconnected fires the active-device-changed callback only after
        // that, so a reconnect from the callback is admitted.
        SyncOperationKind owner = OperationKindLocked();
        if (owner != SyncOperationKind::None)
        {
            TRACE_WARNING("[GAME SAVE] AddUserWithUiAsync: reconnect rejected while a sync operation (%d) owns admission", static_cast<int>(owner));
            return E_PF_GAMESAVE_OPERATION_IN_PROGRESS;
        }

        // User is disconnected from cloud - allow reconnection attempt
        // Per documentation: "When disconnected from cloud, PFGameSaveFilesAddUserWithUiAsync()
        // can be called again if you want to try connect to the cloud."
        //
        // The steps, manifests and file/folder sets are NOT reset here. This function holds only
        // m_progressMutex, and those fields are protected by m_syncMutex, so resetting them from
        // here races a concurrent SetSaveDescription that is reading them. That is the reason the
        // call was removed; calling InitForDownload() bare is not itself a deadlock, since it takes
        // no locks. (Taking m_syncMutex here to close the race WOULD deadlock, because it inverts
        // the established order: the sync workers hold m_syncMutex and then call
        // SetSyncStateProgress, which takes m_progressMutex.)
        // DownloadAsyncProvider's constructor already calls InitForDownload() under m_syncMutex a
        // few statements later, which is the correct place. Leaving it to the provider also means a
        // failed provider creation no longer tears down usable state for a reservation that never
        // ran.
        m_isForcedDisconnectFromCloud.store(false);
    }

    m_syncProgress.syncState = PFGameSaveFilesSyncState::PreparingForDownload;
    m_syncProgress.current = 0;
    m_syncProgress.total = 0;
    m_addUserOptions = options;
    return S_OK;
}

bool FolderSyncManager::TryReserveResetCloud()
{
    std::lock_guard<std::mutex> lock{ m_progressMutex };
    if (m_resetCloudReserved)
    {
        return false;
    }

    // Reset and sync work are mutually exclusive: an upload or download running across the
    // reset's ListManifests snapshot would leave manifests behind that the reset reports as gone.
    if (m_syncProgress.syncState == PFGameSaveFilesSyncState::PreparingForDownload ||
        m_syncProgress.syncState == PFGameSaveFilesSyncState::Downloading ||
        m_syncProgress.syncState == PFGameSaveFilesSyncState::PreparingForUpload ||
        m_syncProgress.syncState == PFGameSaveFilesSyncState::Uploading)
    {
        return false;
    }

    m_resetCloudReserved = true;
    m_resetCloudStep.Reset();
    return true;
}

bool FolderSyncManager::IsResetCloudReserved()
{
    std::lock_guard<std::mutex> lock{ m_progressMutex };
    return m_resetCloudReserved;
}

void FolderSyncManager::ReleaseResetCloudReservation(){
    std::lock_guard<std::mutex> lock{ m_progressMutex };
    m_resetCloudReserved = false;
}

void FolderSyncManager::ResetSetSaveDescriptionStep()
{
    m_setSaveDescriptionStep.Reset();
}

void FolderSyncManager::ResetResetCloudStep()
{
    m_resetCloudStep.Reset();
}

void FolderSyncManager::SetLastShortSaveDescription(const String& shortSaveDescription, bool dirty)
{
    // Serialized with the sync workers: the deferred-description path writes these from the
    // caller's API thread while an upload's FinalizeManifest reads them, and the persist below
    // iterates m_localFileFolderSet, which CompareStep::ReadLocalManifest clears and rebuilds.
    // Recursive, so the worker paths that already hold this lock can call in unchanged.
    std::lock_guard<std::recursive_mutex> lock{ m_syncMutex };

    m_lastShortSaveDescription = shortSaveDescription;
    m_descriptionDirty = dirty;
    
    // Persist to localstate.json so description survives across sessions and is available for conflict UI.
    // Only the description: the in-memory file set isn't always the scan localstate.json was built
    // from (an offline activation holds an empty set), and rewriting the whole file from it dropped
    // every file's sync baseline, so the next connected AddUser read the device's own synced save
    // as local edits (a spurious conflict). The file records are written by the sync steps.
    if (m_localFileFolderSet && !m_saveFolder.empty())
    {
        HRESULT hr = LocalStateManifest::WriteMetadataOnly(m_saveFolder, shortSaveDescription, dirty);
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
        StrCpyUtf8(gameSave.deviceType, sizeof(gameSave.deviceType), deviceType.c_str());
        StrCpyUtf8(gameSave.deviceId, sizeof(gameSave.deviceId), deviceId.c_str());
        StrCpyUtf8(gameSave.deviceFriendlyName, sizeof(gameSave.deviceFriendlyName), deviceFriendlyName.c_str());
        
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
    StrCpyUtf8(gameSave.shortSaveDescription, sizeof(gameSave.shortSaveDescription), manifestDescription.c_str());
    gameSave.thumbnailUri[0] = '\0'; // No thumbnail URI in manifest, might be set after

    return S_OK;
}

void FolderSyncManager::SetSaveFolderOverride(const String& folder)
{
    // This function internal to the SDK allows platforms that mount the underlying file system folder to override
    // where the files are stored, as it might not correspond to the save folder specified in the public API calls.
    // It is not supposed to be thread safe as it should only be called by the same task as the other functions of FolderSyncManager.
    m_saveFolder = folder;
}

} // namespace GameSave
} // namespace PlayFab

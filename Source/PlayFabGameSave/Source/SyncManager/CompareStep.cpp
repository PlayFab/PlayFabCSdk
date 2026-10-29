// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "CompareStep.h"
#include "ApiHelpers.h"
#include "FileFolderSet.h"
#include "Metadata.h"
#include "LockStep.h" // for ManifestWrap forward usage in conflict helpers
#include "Platform/PFGameSaveFilesAPIProvider.h"
#include "LocalStateManifest.h"

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

                        // ManifestVersionNotFinalized and ManifestVersionQuarantined are permanent failures
                        // for this manifest version — retrying will never succeed. Force disconnect so the
                        // user can proceed offline rather than looping in the retry UI.
                        if (result.hr == E_PF_GAME_SAVE_MANIFEST_VERSION_NOT_FINALIZED ||
                            result.hr == E_PF_GAME_SAVE_MANIFEST_VERSION_QUARANTINED)
                        {
                            TRACE_WARNING("[GAME SAVE] CompareStep: manifest version not downloadable (HR:0x%0.8x), forcing offline", static_cast<uint32_t>(result.hr));
                            m_forceDisconnectFromCloud = true;
                            m_stage = CompareStage::CompareDone;
                            task.ScheduleNow();
                        }
                        else
                        {
                            m_stage = CompareStage::WaitForFailedUI_GetManifestDownloadDetails;
                            if (false == uiCallbackManager.ShowSyncFailedUI(task, m_localUser, result.hr, (downloading) ? PFGameSaveFilesSyncState::PreparingForDownload : PFGameSaveFilesSyncState::PreparingForUpload))
                            {
                                // No fail callback set, so just fail API
                                m_stage = CompareStage::CompareStepFailure;
                                m_failureHR = result.hr;
                                task.ScheduleNow();
                            }
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
            String extendedManifestName = FormatString("extended-%llu-manifest.json", static_cast<uint64_t>(latestFinalizedManifest->Version()));
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
                if (FAILED(m_extendedManifestFailureHR))
                {
                    // A previous attempt for this same manifest version DID list an extended manifest
                    // and failed downloading it, so its disappearance from the refreshed details is an
                    // inconsistency rather than a version that legitimately has no extended manifest.
                    //
                    // Skipping here would leave remoteFileFolderSet empty, and ReadLocalManifest feeds
                    // it straight into MarkFilesToSync with no "was the remote set ever populated"
                    // guard. An empty remote set reads as "the cloud has no files", which queues every
                    // previously-synced local file and folder for deletion. Surface the original
                    // download failure instead.
                    TRACE_ERROR("[GAME SAVE] CompareStep: extended manifest %s vanished from refreshed download details after a failed download (HR:0x%0.8x); failing rather than treating the cloud as empty", extendedManifestName.c_str(), static_cast<uint32_t>(m_extendedManifestFailureHR));
                    m_telemetryManager->SetContextActivationHResult(m_extendedManifestFailureHR);
                    m_telemetryManager->EmitContextActivationFailureEvent();
                    m_stage = CompareStage::WaitForFailedUI_GetExtendedManifest;
                    if (false == uiCallbackManager.ShowSyncFailedUI(task, m_localUser, m_extendedManifestFailureHR, (downloading) ? PFGameSaveFilesSyncState::PreparingForDownload : PFGameSaveFilesSyncState::PreparingForUpload))
                    {
                        // No fail callback set, so just fail API
                        m_stage = CompareStage::CompareStepFailure;
                        m_failureHR = m_extendedManifestFailureHR;
                        task.ScheduleNow();
                    }
                }
                else
                {
                    // Not found so skip processing it
                    m_stage = CompareStage::ReadLocalManifest;
                    task.ScheduleNow();
                }
            }
            else
            {
                String folderPath, manifestFilePath;
                if (GameSaveServiceSelector::useMocks || GetWriteManifestsToDisk())
                {
                    RETURN_IF_FAILED(JoinPathHelper(saveFolder, "cloudsync", folderPath));
                    RETURN_IF_FAILED(FilePAL::CreatePath(folderPath));
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
                        // Remember the failure so a later pass can tell "this version legitimately has
                        // no extended manifest" apart from "we already failed to fetch the one it does
                        // have" - see the empty-URL branch above.
                        m_extendedManifestFailureHR = result.hr;
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
                        Vector<char> manifestBytes = result.ExtractPayload();
                        if (GetWriteManifestsToDisk())
                        {
                            HRESULT writeHr = WriteEntireFile(manifestFilePath, manifestBytes);
                            if (FAILED(writeHr))
                            {
                                TRACE_WARNING("[GAME SAVE] Failed to write debug manifest to disk: 0x%0.8x", writeHr);
                            }
                        }
                        HRESULT initHr = remoteFileFolderSet->InitWithExtendedManifest(manifestBytes, latestFinalizedManifest->GetRemoteFileDetails(), saveFolder);
                        TRACE_INFORMATION("[GAME SAVE] CompareStep: InitWithExtendedManifest completed hr=0x%08X remoteFiles=%zu remoteFolders=%zu",
                            initHr, remoteFileFolderSet->GetFiles().size(), remoteFileFolderSet->GetFolders().size());
                        if (FAILED(initHr))
                        {
                            TRACE_ERROR("[GAME SAVE] CompareStep: InitWithExtendedManifest failed hr=0x%08X", initHr);
                            m_telemetryManager->SetContextActivationHResult(initHr);
                            m_telemetryManager->EmitContextActivationFailureEvent();

                            // Clear FIRST: this resets m_initializedFromExtendedManifest, which is what
                            // makes the remote set read as "unknown" rather than "the cloud is empty".
                            // MarkFilesToDeleteUponDownload / MarkFoldersToDeleteUponDownload check that
                            // flag and skip every deletion list, so going offline below cannot propagate
                            // a partial or empty remote set into local deletions.
                            remoteFileFolderSet->Clear();

                            // A manifest this client cannot parse is a permanent condition for this
                            // version - it is durable cloud state, so retrying re-downloads the same
                            // bytes and fails identically. Force offline instead of failing the API, so
                            // the player keeps playing on local data rather than being unable to start
                            // at all, and a later version (or a service-side fix) can still reconcile.
                            // Same treatment ManifestVersionNotFinalized / ManifestVersionQuarantined
                            // already get in GetManifestDownloadDetails above.
                            TRACE_WARNING("[GAME SAVE] CompareStep: extended manifest unreadable (HR:0x%0.8x), forcing offline", static_cast<uint32_t>(initHr));
                            m_forceDisconnectFromCloud = true;
                            m_stage = CompareStage::CompareDone;
                            task.ScheduleNow();
                            return;
                        }
                        m_stage = CompareStage::ReadLocalManifest;
                        task.ScheduleNow();
                    }
                });
            }

            return S_OK;
        }

        case CompareStage::ReadLocalManifest:
        {
            TRACE_INFORMATION("[GAME SAVE] CompareStep::ReadLocalManifest - begin InitWithLocalFilesAndFolders");
            String shortSaveDescription;
            bool descriptionDirty = false;
            bool localStateFound = false;
            HRESULT initHr = localFileFolderSet->InitWithLocalFilesAndFolders(saveFolder, &shortSaveDescription, &descriptionDirty, &localStateFound);
            TRACE_INFORMATION("[GAME SAVE] CompareStep::ReadLocalManifest - InitWithLocalFilesAndFolders hr=0x%08X localFiles=%zu localFolders=%zu localStateFound=%d",
                initHr, localFileFolderSet->GetFiles().size(), localFileFolderSet->GetFolders().size(), localStateFound);
            if (FAILED(initHr))
            {
                if (!downloading)
                {
                    // OQ-31: an upload must never act on a scan that failed. Read as an empty tree,
                    // the upload compared nothing and reported success while the player's edits
                    // stayed local (and a forced commit would have finalized a version without the
                    // cloud's files). Nothing is committed; the next upload rescans.
                    TRACE_ERROR("[GAME SAVE] CompareStep: the local scan failed hr=0x%08X - failing the upload", initHr);
                    return E_PF_GAMESAVE_LOCAL_FILE_UNAVAILABLE;
                }
                TRACE_WARNING("[GAME SAVE] CompareStep: InitWithLocalFilesAndFolders failed hr=0x%08X (may be expected for new saves)", initHr);
                // Don't fail here - continue with empty manifest (this is expected for new saves)
                localFileFolderSet->Clear(); // Ensure localFileFolderSet is in a valid empty state
            }
            
            // Store the loaded description so FolderSyncManager can restore it
            m_loadedShortSaveDescription = shortSaveDescription;
            m_loadedDescriptionDirty = descriptionDirty;

            // On platforms with separate metadata storage, detect if the game file container
            // was deleted externally while cloud sync metadata survived. Clear local state
            // to force a full re-download from cloud.
            // Only check when no game files exist on disk, to avoid unnecessary I/O.
            SharedPtr<GameSaveGlobalState> globalState;
            if (SUCCEEDED(GameSaveGlobalState::Get(globalState)) && globalState->ApiProvider().HasSeparateMetadataStorage())
            {
                bool anyLocalFileOnDisk = false;
                for (const FileDetail& lf : localFileFolderSet->GetFiles())
                {
                    if (lf.existsLocally)
                    {
                        anyLocalFileOnDisk = true;
                        break;
                    }
                }

                if (!anyLocalFileOnDisk && globalState->ApiProvider().IsGameStorageWiped(saveFolder))
                {
                    TRACE_WARNING("[GAME SAVE] CompareStep: Game storage was deleted externally - clearing local state to force full re-download");
                    localFileFolderSet->Clear();
                }
            }

            // Detect metadata-loss scenario and reconcile if local files match cloud.
            // Treat a corrupt localstate.json (file exists but produced no valid lastSync data)
            // the same as a missing file, since downstream parsing would have yielded empty
            // lastSync fields — exactly the condition this recovery path addresses.
            bool hasValidLastSyncData = false;
            if (localStateFound)
            {
                for (const FileDetail& lf : localFileFolderSet->GetFiles())
                {
                    // hasLastSync, not lastSyncFileSize: a save made only of synced zero-byte
                    // files has perfectly valid metadata.
                    if (lf.hasLastSync)
                    {
                        hasValidLastSyncData = true;
                        break;
                    }
                }
            }

            if (!hasValidLastSyncData && !m_conflictOccurred)
            {
                ReconcileMetadataLoss(localFileFolderSet, remoteFileFolderSet, saveFolder, shortSaveDescription, descriptionDirty);
            }

            if (hasValidLastSyncData && !latestFinalizedManifest->HasVersion())
            {
                // OQ-24: the cloud holds no finalized version (ResetCloud, an administrative wipe,
                // every version quarantined) while localstate.json still has sync baselines. They
                // describe a cloud copy that no longer exists. Kept, an unchanged file was never sent
                // again: the next upload committed a version without it and the following
                // activation read it as deleted in the cloud and deleted the local copy. Dropped,
                // every local file and folder is new to this cloud: nothing is deleted (no cloud
                // file, no baseline) and the next upload sends the whole save.
                size_t dropped = 0;
                for (const FileDetail& file : localFileFolderSet->GetFiles())
                {
                    if (file.hasLastSync)
                    {
                        file.hasLastSync = false;
                        file.lastSyncFileSize = 0;
                        file.lastSyncTimeLastModified = 0;
                        file.lastSyncRemoteTimeLastModified = 0;
                        dropped++;
                    }
                }
                for (const FolderDetail& folder : localFileFolderSet->GetFolders())
                {
                    folder.hasLastSync = false;
                }
                TRACE_WARNING("[GAME SAVE] CompareStep: the cloud has no finalized version but localstate.json has sync baselines (cloud reset or wiped); dropped %zu stale file baseline(s)", dropped);
                // Persisted now: an AddUser on an empty cloud writes localstate.json only when it
                // downloads something, so without this the stale baselines would survive on disk
                // until this device uploads, and an AddUser after another device committed a version
                // would read every file missing from that version as deleted in the cloud. A failed
                // write fails the operation: going on would leave this device working from baselines
                // that exist only in memory while the stale ones stay on disk.
                HRESULT writeHr = LocalStateManifest::WriteLocalManifest(saveFolder, localFileFolderSet, shortSaveDescription, descriptionDirty);
                if (FAILED(writeHr))
                {
                    TRACE_ERROR("[GAME SAVE] CompareStep: failed to persist the dropped baselines hr=0x%08X - failing the operation", static_cast<uint32_t>(writeHr));
                    return writeHr;
                }
            }

            TRACE_INFORMATION("[GAME SAVE] CompareStep::ReadLocalManifest - begin MarkFilesToSync remoteFiles=%zu remoteFolders=%zu",
                remoteFileFolderSet->GetFiles().size(), remoteFileFolderSet->GetFolders().size());
            bool conflictFound = false;
            MarkFilesToSync(localFileFolderSet, remoteFileFolderSet, saveFolder, conflictFound);
            TRACE_INFORMATION("[GAME SAVE] CompareStep::ReadLocalManifest - MarkFilesToSync done conflictFound=%d downloading=%d", conflictFound, downloading);

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
            TRACE_INFORMATION("[GAME SAVE] CompareStep - WaitForConflictUI: waiting for user to resolve conflict between local and cloud saves");
            UIAction uiAction = uiCallbackManager.GetAction();

            if (uiAction == UIAction::UIConflictTakeLocal)
            {
                m_telemetryManager->SetContextActivationConflictResolution(ConflictResolution::CR_KeepLocal);
                m_takeUIChoice = TakeUIChoice::TakeLocal;
                TRACE_INFORMATION("[GAME SAVE] CompareStep - WaitForConflictUI: user chose KEEP LOCAL save");
                m_conflictOccurred = true;
#ifdef PF_GAMESAVE_ENABLE_CONFLICT_LOSER_UPLOAD // Disabled until ADO 61899649, 61899650, 61899651 are fixed
                m_conflictRequiresUpload = true; // Always upload local branch once
#endif
                m_stage = CompareStage::ReadLocalManifest;
                task.ScheduleNow();
            }
            else if (uiAction == UIAction::UIConflictTakeRemote)
            {
                m_telemetryManager->SetContextActivationConflictResolution(ConflictResolution::CR_TakeRemote);
                m_takeUIChoice = TakeUIChoice::TakeRemote;
                TRACE_INFORMATION("[GAME SAVE] CompareStep - WaitForConflictUI: user chose KEEP CLOUD save");
                m_conflictOccurred = true;
#ifdef PF_GAMESAVE_ENABLE_CONFLICT_LOSER_UPLOAD // Disabled until ADO 61899649, 61899650, 61899651 are fixed
                m_conflictRequiresUpload = true; // still upload local divergent branch for rollback path
#endif
                m_stage = CompareStage::ReadLocalManifest;
                task.ScheduleNow();
            }
            else if (uiAction == UIAction::UIConflictCancel)
            {
                m_telemetryManager->SetContextActivationConflictResolution(ConflictResolution::CR_NoResolutionChosen);
                TRACE_WARNING("[GAME SAVE] CompareStep - WaitForConflictUI: user CANCELLED conflict resolution");
                return E_PF_GAMESAVE_USER_CANCELLED;
            }
            else if (uiAction == UIAction::UISyncFailedCancel)
            {
                // The wait was woken by uninitialize or XAsyncCancel (UICallbackManager::CancelPendingUIWait
                // / CancelUIWaitForTask), not answered. Waiting on (E_PENDING) left the AddUser parked
                // with nobody to wake it when this pump ran before the run context was cancelled:
                // neither the AddUser nor uninitialize ever completed.
                m_telemetryManager->SetContextActivationConflictResolution(ConflictResolution::CR_NoResolutionChosen);
                TRACE_WARNING("[GAME SAVE] CompareStep - WaitForConflictUI: prompt ended by cancellation or shutdown");
                return E_ABORT;
            }
            else
            {
                // No action set yet - user hasn't responded to the conflictCallback.
                // Return E_PENDING to keep the async operation alive.
                return E_PENDING;
            }
            return S_OK;
        }

        case CompareStage::WaitForFailedUI_GetManifestDownloadDetails:
        {
            TRACE_INFORMATION("[GAME SAVE] CompareStep - WaitForFailedUI_GetManifestDownloadDetails: waiting for user response");
            return uiCallbackManager.HandleFailedUI(task, 
                [this]() { 
                    TRACE_INFORMATION("[GAME SAVE] CompareStep - WaitForFailedUI_GetManifestDownloadDetails: user chose RETRY");
                    m_stage = CompareStage::GetManifestDownloadDetails; 
                },
                [this]() { 
                    TRACE_WARNING("[GAME SAVE] CompareStep - WaitForFailedUI_GetManifestDownloadDetails: user chose OFFLINE MODE. Cloud sync will be skipped.");
                    m_forceDisconnectFromCloud = true; 
                    m_stage = CompareStage::CompareDone; 
                }
            );
        }

        case CompareStage::WaitForFailedUI_GetExtendedManifest:
        {
            TRACE_INFORMATION("[GAME SAVE] CompareStep - WaitForFailedUI_GetExtendedManifest: waiting for user response");
            return uiCallbackManager.HandleFailedUI(task, 
                [this, latestFinalizedManifest]() { 
                    // Retry from GetManifestDownloadDetails rather than GetExtendedManifest so the
                    // retry fetches a fresh set of download URLs. Retrying at GetExtendedManifest
                    // re-scanned the cached details and re-requested the identical URL for the
                    // identical blob, so every retry failed identically and the only ways out were
                    // going offline or cancelling (Bug 63588284). Re-fetching recovers the cases the
                    // client can actually fix, most notably an expired SAS token.
                    //
                    // Clearing the cached details first is required for correctness, not just
                    // freshness: GetManifestDownloadDetails short-circuits to ReadLocalManifest
                    // while GotRemoteFileDetails() is true, which would skip the extended manifest
                    // and leave remoteFileFolderSet empty - making the cloud look empty to the
                    // compare logic.
                    TRACE_INFORMATION("[GAME SAVE] CompareStep - WaitForFailedUI_GetExtendedManifest: user chose RETRY; refreshing manifest download details");
                    latestFinalizedManifest->ClearRemoteFileDetails();
                    m_stage = CompareStage::GetManifestDownloadDetails; 
                },
                [this]() { 
                    TRACE_WARNING("[GAME SAVE] CompareStep - WaitForFailedUI_GetExtendedManifest: user chose OFFLINE MODE. Cloud sync will be skipped.");
                    m_forceDisconnectFromCloud = true; 
                    m_stage = CompareStage::CompareDone; 
                }
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

        default:
        {
            TRACE_ERROR("[GAME SAVE] CompareStep: unexpected stage %d", static_cast<int>(m_stage));
            return E_UNEXPECTED;
        }
    }

    return S_OK;
}

void CompareStep::ReconcileMetadataLoss(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet,
    _In_ const String& saveFolder,
    _In_ const String& shortSaveDescription,
    _In_ bool descriptionDirty)
{
    // Detect metadata-loss scenario: localstate.json is missing but game files exist
    // on disk and match the cloud by size (and timestamp on platforms that support it).
    // Without this check, every file gets lastSync=0, causing HasLocalFileChanged and
    // HasRemoteFileChanged to both flag changes, producing a false conflict.
    // When local files match cloud, pre-populate lastSync fields so the comparison
    // functions see no difference and no conflict is raised.
    //
    // PlatformNeedsRemoteTimestamp() == true means the platform cannot set file
    // timestamps, so we compare by size only on those platforms.
    // On all other platforms we also compare timestamps for extra safety.

    bool hasCloudData = !remoteFileFolderSet->GetFiles().empty();
    if (!hasCloudData)
    {
        return;
    }

    bool skipTimestampCheck = false;
    SharedPtr<GameSaveGlobalState> globalState;
    if (SUCCEEDED(GameSaveGlobalState::Get(globalState)))
    {
        skipTimestampCheck = globalState->ApiProvider().PlatformNeedsRemoteTimestamp();
    }

    bool allLocalFilesMatchCloud = true;
    bool anyLocalFileOnDisk = false;

    for (const FileDetail& localFile : localFileFolderSet->GetFiles())
    {
        if (localFile.timeLastModified == 0)
        {
            continue; // file not on disk, skip
        }

        anyLocalFileOnDisk = true;
        String relFilePath = localFileFolderSet->GetRelFilePath(&localFile);
        const FileDetail* remoteFile = remoteFileFolderSet->GetFileDetailFromRelFilePath(relFilePath);

        if (remoteFile == nullptr)
        {
            // Local file exists but not in cloud - not a match
            TRACE_INFORMATION("[GAME SAVE] CompareStep: Metadata-loss match rejected - local file '%s' not found in cloud",
                relFilePath.c_str());
            allLocalFilesMatchCloud = false;
            break;
        }

        // Compare size
        if (localFile.fileSizeBytes != remoteFile->fileSizeBytes)
        {
            TRACE_INFORMATION("[GAME SAVE] CompareStep: Metadata-loss match rejected - size mismatch for '%s' (local=%llu remote=%llu)",
                relFilePath.c_str(), localFile.fileSizeBytes, remoteFile->fileSizeBytes);
            allLocalFilesMatchCloud = false;
            break;
        }

        // On platforms with reliable timestamps, also compare timestamps
        if (!skipTimestampCheck && localFile.timeLastModified != remoteFile->timeLastModified)
        {
            TRACE_INFORMATION("[GAME SAVE] CompareStep: Metadata-loss match rejected - timestamp mismatch for '%s' (local=%lld remote=%lld)",
                relFilePath.c_str(), static_cast<long long>(localFile.timeLastModified), static_cast<long long>(remoteFile->timeLastModified));
            allLocalFilesMatchCloud = false;
            break;
        }
    }

    // Also verify cloud doesn't have extra files missing locally
    if (allLocalFilesMatchCloud && anyLocalFileOnDisk)
    {
        for (const FileDetail& remoteFile : remoteFileFolderSet->GetFiles())
        {
            String relFilePath = remoteFileFolderSet->GetRelFilePath(&remoteFile);
            const FileDetail* localFile = localFileFolderSet->GetFileDetailFromRelFilePath(relFilePath);
            if (localFile == nullptr || localFile->timeLastModified == 0)
            {
                TRACE_INFORMATION("[GAME SAVE] CompareStep: Metadata-loss match rejected - cloud file '%s' not found locally",
                    relFilePath.c_str());
                allLocalFilesMatchCloud = false;
                break;
            }
        }
    }

    if (allLocalFilesMatchCloud && anyLocalFileOnDisk)
    {
        TRACE_WARNING("[GAME SAVE] CompareStep: Metadata lost but local files match cloud - treating as re-sync (skipping conflict). skipTimestampCheck=%d", skipTimestampCheck);

        // Pre-populate lastSync fields from cloud data so the comparison functions
        // see no difference and don't mark files for upload or download.
        for (const FileDetail& localFile : localFileFolderSet->GetFiles())
        {
            if (localFile.timeLastModified == 0)
            {
                continue;
            }

            String relFilePath = localFileFolderSet->GetRelFilePath(&localFile);
            const FileDetail* remoteFile = remoteFileFolderSet->GetFileDetailFromRelFilePath(relFilePath);
            if (remoteFile != nullptr)
            {
                localFile.lastSyncFileSize = localFile.fileSizeBytes;
                localFile.hasLastSync = true;
                localFile.lastSyncTimeLastModified = localFile.timeLastModified;
                localFile.lastSyncRemoteTimeLastModified = remoteFile->timeLastModified;
            }
        }

        // Only mark folders as having last sync data if they exist in both local and
        // remote sets. Folders that exist only locally or only remotely should not get
        // hasLastSync=true, so that MarkFoldersToDeleteUponDownload and related logic
        // can correctly handle folder creation/deletion signals.
        for (const FolderDetail& folder : localFileFolderSet->GetFolders())
        {
            const FolderDetail* remoteFolder = remoteFileFolderSet->GetFolderDetailFromRelFilePath(folder.relFolderPath);
            if (remoteFolder != nullptr)
            {
                folder.hasLastSync = true;
            }
        }

        // Persist the reconstructed localstate.json immediately. Without this,
        // the re-sync path produces no downloads, so DownloadStep::UpdateLocalManifest
        // is never reached and localstate.json remains missing on disk. Subsequent
        // operations (upload, next app launch) would re-enter this detection path
        // every time. Writing now ensures localStateFound=true on all future passes.
        HRESULT writeHr = LocalStateManifest::WriteLocalManifest(saveFolder, localFileFolderSet, shortSaveDescription, descriptionDirty);
        if (FAILED(writeHr))
        {
            TRACE_WARNING("[GAME SAVE] CompareStep: Failed to write reconstructed localstate.json hr=0x%08X (non-fatal)", writeHr);
        }
        else
        {
            TRACE_INFORMATION("[GAME SAVE] CompareStep: Successfully wrote reconstructed localstate.json after metadata-loss re-sync");
        }
    }
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
            // Find the most recent modification time from local files
            time_t mostRecentTime = 0;
            localGameSave.totalBytes = 0;
            for (const FileDetail& localFile : localFileFolderSet->GetFiles())
            {
                localGameSave.totalBytes += localFile.fileSizeBytes;
                if (localFile.timeLastModified > mostRecentTime)
                {
                    mostRecentTime = localFile.timeLastModified;
                }
            }
            // Use most recent file time if available, otherwise fall back to current time
            localGameSave.time = (mostRecentTime > 0) ? mostRecentTime : GetTimeTNow();
            localGameSave.uploadedBytes = localGameSave.totalBytes;
            String deviceId = GetLocalDeviceID(saveFolder);
            String deviceType = GetDeviceType();
            String deviceFriendlyName = GetDeviceFriendlyName();
            StrCpyUtf8(localGameSave.deviceType, sizeof(localGameSave.deviceType), deviceType.c_str());
            StrCpyUtf8(localGameSave.deviceId, sizeof(localGameSave.deviceId), deviceId.c_str());
            StrCpyUtf8(localGameSave.deviceFriendlyName, sizeof(localGameSave.deviceFriendlyName), deviceFriendlyName.c_str());
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
            StrCpyUtf8(localGameSave.shortSaveDescription, sizeof(localGameSave.shortSaveDescription), shortSaveDescription.c_str());

            PFGameSaveDescriptor remoteGameSave{};
            FolderSyncManager::ConvertToPFGameSaveDescriptor(latestFinalizedManifest->GetManifest(), remoteGameSave);
            remoteGameSave.totalBytes = remoteFileFolderSet->GetTotalUncompressedSize();
            remoteGameSave.uploadedBytes = remoteGameSave.totalBytes;
            const CompressedFile* remoteThumbnail = remoteFileFolderSet->GetThumbnailFromCompressedList(latestFinalizedManifest->VersionString());
            if (remoteThumbnail)
            {
                StrCpy(remoteGameSave.thumbnailUri, sizeof(remoteGameSave.thumbnailUri), remoteThumbnail->downloadUrl.c_str());
            }

            // Log conflict UI descriptions for diagnostics
            TRACE_INFORMATION("[GAME SAVE] CompareStep: ShowConflictUI - localDesc='%s' remoteDesc='%s' localTime=%lld remoteTime=%lld",
                localGameSave.shortSaveDescription,
                remoteGameSave.shortSaveDescription,
                static_cast<long long>(localGameSave.time),
                static_cast<long long>(remoteGameSave.time));
            if (localGameSave.shortSaveDescription[0] == '\0')
            {
                TRACE_WARNING("[GAME SAVE] CompareStep: Local save description is EMPTY for conflict UI");
            }
            if (remoteGameSave.shortSaveDescription[0] == '\0')
            {
                TRACE_WARNING("[GAME SAVE] CompareStep: Remote save description is EMPTY for conflict UI (manifest v:%s)", 
                    latestFinalizedManifest->VersionString().c_str());
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
            // OQ-1: the units the choice resolves, from this pass's lists (before they're trimmed below).
            const Set<String> conflictingUnits = CollectConflictingUnitKeys(localFileFolderSet, remoteFileFolderSet);

            // Take local means the local tree is authoritative: skip all downloads AND drop every
            // pending local mutation the comparison derived from the cloud state. The re-entry
            // into ReadLocalManifest repopulates those lists, and DoWorkFolderDownload applies
            // them unconditionally - so without this, "keep local" would still delete local files
            // and folders the cloud no longer has.
            Vector<size_t> noDownloads{};
            remoteFileFolderSet->SetCompressedFilesToDownload(std::move(noDownloads));

            Vector<const FileDetail*> noFileDeletes{};
            localFileFolderSet->SetFilesToDeleteUponDownload(std::move(noFileDeletes));

            Vector<const FolderDetail*> noFolderDeletes{};
            localFileFolderSet->SetFoldersToDeleteUponDownload(std::move(noFolderDeletes));

            Vector<const FolderDetail*> noFolderCreates{};
            remoteFileFolderSet->SetFoldersToCreateUponDownload(std::move(noFolderCreates));

            if (!conflictingUnits.empty())
            {
                RETURN_IF_FAILED(PersistTakeLocalAuthority(conflictingUnits, localFileFolderSet, saveFolder));
            }

            m_stage = CompareStage::CompareDone;
            task.ScheduleNow();
            break;
        }

        case TakeUIChoice::TakeRemote:
        {
            // Take remote downloads what changed in the cloud, as usual; OQ-1: the conflicting units
            // also become an exact copy of the cloud's.
            const Set<String> conflictingUnits = CollectConflictingUnitKeys(localFileFolderSet, remoteFileFolderSet);
            if (!conflictingUnits.empty())
            {
                MirrorConflictingUnitsFromCloud(conflictingUnits, localFileFolderSet, remoteFileFolderSet, saveFolder);
            }
            m_stage = CompareStage::CompareDone;
            task.ScheduleNow();
            break;
        }
    }

    return S_OK;
}

void CompareStep::MirrorConflictingUnitsFromCloud(
    _In_ const Set<String>& conflictingUnits,
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const SharedPtr<FileFolderSet>& remoteFileFolderSet,
    _In_ const String& saveFolder)
{
    // Unit keys as ScanForConflicts computes them (the root files' unit is "").
    auto inConflict = [&conflictingUnits](const String& relFolderPath)
    {
        return conflictingUnits.find(GetTopLevelFolder(relFolderPath)) != conflictingUnits.end();
    };

    // 1. A file of the unit whose local copy differs from the cloud's (edited or deleted locally,
    //    whatever its own cloud copy did) loses its baseline, so the selection below downloads it.
    //    Without this, a file changed only locally kept the edit, which the next upload then sent
    //    over the cloud copy: a unit mixing both devices' files.
    size_t restored = 0;
    for (const FileDetail& localFile : localFileFolderSet->GetFiles())
    {
        if (!inConflict(localFileFolderSet->GetFileFolder(&localFile).relFolderPath))
        {
            continue;
        }
        const FileDetail* remoteFile = remoteFileFolderSet->GetFileDetailFromRelFilePath(localFileFolderSet->GetRelFilePath(&localFile));
        if (remoteFile == nullptr)
        {
            continue;
        }
        if (localFile.existsLocally && localFile.fileSizeBytes == remoteFile->fileSizeBytes && localFile.timeLastModified == remoteFile->timeLastModified)
        {
            continue; // already the cloud's copy
        }
        localFile.hasLastSync = false;
        localFile.lastSyncFileSize = 0;
        localFile.lastSyncTimeLastModified = 0;
        localFile.lastSyncRemoteTimeLastModified = 0;
        restored++;
    }

    // 2. Rebuild the transfer selection with those baselines. Every other unit keeps today's selection.
    MarkFilesToTransferUponDownload(localFileFolderSet, remoteFileFolderSet, saveFolder);

    // 3. What the cloud's copy of the unit doesn't have goes: local-only files (synced or never
    //    synced) and folders. Folder deletes prune, so anything that can't be deleted keeps its folder.
    Vector<const FileDetail*> fileDeletes = localFileFolderSet->GetFilesToDeleteUponDownload();
    for (const FileDetail& localFile : localFileFolderSet->GetFiles())
    {
        if (!localFile.existsLocally || !inConflict(localFileFolderSet->GetFileFolder(&localFile).relFolderPath) ||
            remoteFileFolderSet->GetFileDetailFromRelFilePath(localFileFolderSet->GetRelFilePath(&localFile)) != nullptr ||
            std::find(fileDeletes.begin(), fileDeletes.end(), &localFile) != fileDeletes.end())
        {
            continue;
        }
        fileDeletes.push_back(&localFile);
    }
    Vector<const FolderDetail*> folderDeletes = localFileFolderSet->GetFoldersToDeleteUponDownload();
    for (const FolderDetail& localFolder : localFileFolderSet->GetFolders())
    {
        if (localFolder.relFolderPath.empty() || !localFolder.existsLocally || !inConflict(localFolder.relFolderPath) ||
            remoteFileFolderSet->GetFolderDetailFromRelFilePath(localFolder.relFolderPath) != nullptr ||
            std::find(folderDeletes.begin(), folderDeletes.end(), &localFolder) != folderDeletes.end())
        {
            continue;
        }
        folderDeletes.push_back(&localFolder);
    }
    const size_t fileDeleteCount = fileDeletes.size();
    const size_t folderDeleteCount = folderDeletes.size();
    localFileFolderSet->SetFilesToDeleteUponDownload(std::move(fileDeletes));
    localFileFolderSet->SetFoldersToDeleteUponDownload(std::move(folderDeletes));

    // 4. The cloud's folders of the unit that were deleted locally come back (the folder-create pass
    //    only creates folders this device has no record of).
    Vector<const FolderDetail*> creates = remoteFileFolderSet->GetFoldersToCreateUponDownload();
    for (const FolderDetail& remoteFolder : remoteFileFolderSet->GetFolders())
    {
        if (remoteFolder.relFolderPath.empty() || !inConflict(remoteFolder.relFolderPath))
        {
            continue;
        }
        const FolderDetail* localFolder = localFileFolderSet->GetFolderDetailFromRelFilePath(remoteFolder.relFolderPath);
        if (localFolder != nullptr && !localFolder->existsLocally &&
            std::find(creates.begin(), creates.end(), &remoteFolder) == creates.end())
        {
            creates.push_back(&remoteFolder);
        }
    }
    remoteFileFolderSet->SetFoldersToCreateUponDownload(std::move(creates));

    TRACE_INFORMATION("[GAME SAVE] CompareStep: Take Remote mirrors %zu conflicting unit(s): %zu file(s) restored, downloads=%zu fileDeletes=%zu folderDeletes=%zu folderCreates=%zu",
        conflictingUnits.size(), restored, remoteFileFolderSet->GetCompressedFilesToDownload().size(), fileDeleteCount, folderDeleteCount,
        remoteFileFolderSet->GetFoldersToCreateUponDownload().size());
}

HRESULT CompareStep::PersistTakeLocalAuthority(
    _In_ const Set<String>& conflictingUnits,
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet,
    _In_ const String& saveFolder)
{
    // Without this, a file of the unit that only the cloud changed kept its old baseline: the next
    // upload carried the cloud's copy forward next to this device's edits (a unit mixing both
    // devices' files), and this device downloaded the cloud copy at its next AddUser.
    size_t rebased = 0;
    for (const FileDetail& localFile : localFileFolderSet->GetFiles())
    {
        if (!localFile.existsLocally || !localFile.hasLastSync ||
            conflictingUnits.find(GetTopLevelFolder(localFileFolderSet->GetFileFolder(&localFile).relFolderPath)) == conflictingUnits.end())
        {
            continue;
        }
        localFile.lastSyncTimeLastModified = 0;
        localFile.lastSyncRemoteTimeLastModified = 0;
        rebased++;
    }
    TRACE_INFORMATION("[GAME SAVE] CompareStep: Take Local makes %zu conflicting unit(s) authoritative (%zu file baseline(s))", conflictingUnits.size(), rebased);
    if (rebased == 0)
    {
        return S_OK;
    }
    HRESULT hr = LocalStateManifest::WriteLocalManifest(saveFolder, localFileFolderSet, m_loadedShortSaveDescription, m_loadedDescriptionDirty);
    if (FAILED(hr))
    {
        TRACE_ERROR("[GAME SAVE] CompareStep: failed to record Take Local hr=0x%08X", hr);
    }
    return hr;
}

void CompareStep::Reset()
{
    m_stage = CompareStage::GetManifestDownloadDetails;
    m_takeUIChoice = TakeUIChoice::NoChoiceYet;
    m_forceDisconnectFromCloud = false;
    m_conflictOccurred = false;
    m_conflictRequiresUpload = false;
    m_extendedManifestFailureHR = S_OK;
}

} // namespace GameSave
} // namespace PlayFab

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
    m_conflictOccurred = false;
    m_conflictRequiresUpload = false;
    m_extendedManifestFailureHR = S_OK;
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
    MarkFoldersToCreateUponUpload(localFileFolderSet, remoteFileFolderSet);
    MarkFoldersToCreateUponDownload(localFileFolderSet, remoteFileFolderSet);
    MarkFoldersToDeleteUponUpload(localFileFolderSet, remoteFileFolderSet);
    MarkFoldersToDeleteUponDownload(localFileFolderSet, remoteFileFolderSet);
    // TODO: ScanForConflicts currently only checks file uploads/deletions against
    // topLevelFoldersNeedingDownload. Folder-only changes (empty folder create/delete)
    // in an atomic unit that also has remote changes will not trigger conflict UI.
    // To fully enforce the all-or-nothing conflict model, the scan should also check
    // local folder creates/deletes against topLevelFoldersNeedingDownload.
    //
    // Note: a remotely-deleted folder is also invisible here, because it has no incoming
    // downloads and so never lands in changedRemoteFolderIndexes. That used to cause silent
    // data loss when the folder still held never-synced local files. DownloadStep::DeleteFolders
    // now prunes instead of recursively deleting, so those files survive (Bug 63588283); the
    // conflict-detection gap described above is still open.
    ScanForConflicts(localFileFolderSet, remoteFileFolderSet, conflictFound);
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

    // Conflict resolution re-enters ReadLocalManifest, which runs this selection again against the
    // same remote set (and therefore the same ArchiveContext objects). Start from a clean slate so
    // re-adding the same files doesn't accumulate against the archive size limits.
    for (const CompressedFile& compressedFile : compressedFiles)
    {
        if (compressedFile.archiveContext != nullptr)
        {
            compressedFile.archiveContext->ClearFiles();
        }
    }

#if defined(_DEBUG)
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
                ArchiveFileDetail afd{};
                if (FAILED(JoinPathHelper(saveFolder, relFilePath, afd.fullPath)))
                {
                    TRACE_WARNING("[GAME SAVE] CompareStep: JoinPathHelper failed for download file '%s'", relFilePath.c_str());
                    continue;
                }
                afd.uncompressedSize = remoteFile.fileSizeBytes;
                afd.timeLastModified = remoteFile.timeLastModified;
                afd.timeCreated = remoteFile.timeCreated;
                HRESULT addHr = compressedFiles[remoteFile.compressedFileIndex].archiveContext->AddFile(relFilePath, std::move(afd));
                if (FAILED(addHr))
                {
                    // Rejected by the archive's path/size validation - don't schedule an
                    // extraction the archive layer will never perform.
                    TRACE_WARNING("[GAME SAVE] CompareStep: AddFile rejected download file '%s' (hr=0x%08X)", relFilePath.c_str(), addHr);
                    continue;
                }

                // Only schedule the bundle once the file is actually queued for extraction,
                // otherwise a rejected file still causes its bundle to be downloaded and the file
                // to be recorded as synced without ever having been written.
                compressedFileIndexSet.insert(remoteFile.compressedFileIndex); // Set<> doesn't allow duplicates
                changedRemoteFolderIndexSet.insert(remoteFile.folderIndex); // Set<> doesn't allow duplicates.
#if defined(_DEBUG)
                filesToDownload.push_back(&remoteFile);
#endif
            }
        }
        else
        {
            // no local file, so need to download it
            ArchiveFileDetail afd{};
            if (FAILED(JoinPathHelper(saveFolder, relFilePath, afd.fullPath)))
            {
                TRACE_WARNING("[GAME SAVE] CompareStep: JoinPathHelper failed for new download file '%s'", relFilePath.c_str());
                continue;
            }
            afd.uncompressedSize = remoteFile.fileSizeBytes;
            afd.timeLastModified = remoteFile.timeLastModified;
            afd.timeCreated = remoteFile.timeCreated;
            HRESULT addHr = compressedFiles[remoteFile.compressedFileIndex].archiveContext->AddFile(relFilePath, std::move(afd));
            if (FAILED(addHr))
            {
                // Rejected by the archive's path/size validation - don't schedule an extraction
                // the archive layer will never perform.
                TRACE_WARNING("[GAME SAVE] CompareStep: AddFile rejected new download file '%s' (hr=0x%08X)", relFilePath.c_str(), addHr);
                continue;
            }

            compressedFileIndexSet.insert(remoteFile.compressedFileIndex); // Set<> doesn't allow duplicates
            changedRemoteFolderIndexSet.insert(remoteFile.folderIndex); // Set<> doesn't allow duplicates.
#if defined(_DEBUG)
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

#if defined(_DEBUG)
    remoteFileFolderSet->SetFilesToDownload(std::move(filesToDownload));  // used only for debug stats
#endif
}


String GetTopLevelFolder(const String& relFolderPath)
{
    if (relFolderPath.empty())
    {
        return relFolderPath;
    }

    // Find the first path separator, skipping a leading one if present
    size_t startPos = (relFolderPath[0] == FilePAL::GetPathSeparatorChar()) ? 1 : 0;
    const char* searchStart = relFolderPath.c_str() + startPos;
    const char* firstSep = strchr(searchStart, FilePAL::GetPathSeparatorChar());

    if (firstSep)
    {
        // Found a separator, return everything up to it (including leading sep if present)
        size_t firstSepIndex = firstSep - relFolderPath.c_str();
        return relFolderPath.substr(0, firstSepIndex);
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
    _Inout_ bool& conflictFound
    )
{
    const FolderDetail& folder = localFileFolderSet->GetFileFolder(localFileToUpload);
    String topLevelFolder = GetTopLevelFolder(folder.relFolderPath);

    auto iter = std::find_if(
        topLevelFoldersNeedingDownload.begin(),
        topLevelFoldersNeedingDownload.end(),
        [&topLevelFolder](const String& str)
        {
            return (str.compare(topLevelFolder.c_str()) == 0);
        });
    conflictFound = conflictFound || (iter != topLevelFoldersNeedingDownload.end());
}

void CompareStep::MarkFilesToDeleteUponUpload(
    _In_ const SharedPtr<FileFolderSet>& localFileFolderSet)
{
    const Vector<FileDetail>& localFiles = localFileFolderSet->GetFiles();
    Vector<const FileDetail*> filesToDeleteUponUpload;
    for (const FileDetail& localFile : localFiles)
    {
        // If the file has been synced before but is no longer on disk, it was deleted locally.
        // hasLastSync (not lastSyncFileSize) is the existence record: a synced zero-byte file has
        // a last-sync size of 0 and would otherwise never be deletable.
        if (localFile.hasLastSync && !localFile.existsLocally)
        {
            TRACE_INFORMATION("[GAME SAVE] CompareStep: FilesToDeleteUponUpload file:%s lastSyncFileSize:%llu existsLocally:%d", localFile.fileName.c_str(), localFile.lastSyncFileSize, localFile.existsLocally);
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
    // If we never parsed the cloud's extended manifest, this set is "unknown", not "empty". Every
    // previously-synced local file would look absent from the cloud and be queued for deletion,
    // wiping the player's save. Preserve instead - a later sync that can read the manifest will
    // reconcile properly.
    if (!remoteFileFolderSet->IsInitializedFromExtendedManifest())
    {
        TRACE_ERROR("[GAME SAVE] CompareStep: remote file set was never populated from an extended manifest; skipping FilesToDeleteUponDownload so local files are not treated as cloud deletions");
        localFileFolderSet->SetFilesToDeleteUponDownload({});
        return;
    }

    // If there's last sync data but no remote file, then remote file was deleted
    Vector<const FileDetail*> filesToDeleteUponDownload;
    const Vector<FileDetail>& localFiles = localFileFolderSet->GetFiles();
    for (const FileDetail& localFile : localFiles)
    {
        String relFilePath = localFileFolderSet->GetRelFilePath(&localFile);
        const FileDetail* remoteFile = remoteFileFolderSet->GetFileDetailFromRelFilePath(relFilePath);

        TRACE_INFORMATION("[GAME SAVE] CompareStep: FilesToDeleteUponDownload: file:%s hasLastSync:%d", localFile.fileName.c_str(), localFile.hasLastSync);
        if (localFile.hasLastSync &&
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

        // Skip folders that don't exist locally - they were deleted and should not be
        // re-created on the remote side. Without this check, a folder that was deleted
        // locally (and already removed from cloud) would be perpetually re-marked for
        // creation because it remains in localstate.json but is absent from the remote
        // manifest, causing every subsequent upload to perform a full upload cycle.
        if (!localFolder.existsLocally)
        {
            continue;
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
            TRACE_INFORMATION("[GAME SAVE] CompareStep: MarkFoldersToDeleteUponUpload %s: existsLocally: %d", remoteFolder.relFolderPath.c_str(), localFolder->existsLocally);
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
    // Same reasoning as MarkFilesToDeleteUponDownload: an unpopulated remote set is "unknown", not
    // "empty", and would otherwise queue every synced folder for recursive deletion.
    if (!remoteFileFolderSet->IsInitializedFromExtendedManifest())
    {
        TRACE_ERROR("[GAME SAVE] CompareStep: remote file set was never populated from an extended manifest; skipping FoldersToDeleteUponDownload so local folders are not treated as cloud deletions");
        localFileFolderSet->SetFoldersToDeleteUponDownload({});
        return;
    }

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
    // existsLocally is set by the disk scan in MergeLocalFiles. File size can't be used as an
    // existence proxy: a legitimate zero-byte save file would be reported as deleted here and
    // then dropped from the cloud manifest on the next upload (and deleted on other devices).
    if (localFile.existsLocally)
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
    // On platforms where SetFileLastModifiedTime is a no-op, lastSyncTimeLastModified contains
    // the local disk timestamp which won't match the cloud's timestamp. Use the dedicated
    // lastSyncRemoteTimeLastModified field which stores the cloud timestamp at last sync.
    time_t lastSyncTimestampForRemoteComparison = localFile.lastSyncTimeLastModified;

    SharedPtr<GameSaveGlobalState> globalState;
    if (SUCCEEDED(GameSaveGlobalState::Get(globalState)))
    {
        if (globalState->ApiProvider().PlatformNeedsRemoteTimestamp() && localFile.lastSyncRemoteTimeLastModified != 0)
        {
            lastSyncTimestampForRemoteComparison = localFile.lastSyncRemoteTimeLastModified;
        }
    }

    bool hasRemoteFileChanged =
        (localFile.lastSyncFileSize != remoteFile.fileSizeBytes ||
         lastSyncTimestampForRemoteComparison != remoteFile.timeLastModified);

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

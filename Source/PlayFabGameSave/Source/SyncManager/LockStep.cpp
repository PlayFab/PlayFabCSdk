// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "LockStep.h"
#include "LocalUserLoginOperation.h"
#include "Metadata.h"

using namespace PlayFab::GameSaveWrapper;

namespace PlayFab
{
namespace GameSave
{

LockStep::LockStep(_In_ LocalUser const& localUser, _In_ SharedPtr<GameSaveTelemetryManager> telemetryManager) :
    m_localUser{ localUser },
    m_telemetryManager{ telemetryManager }
{
}

bool LockStep::IsLockAcquired() const
{
    return m_stage == LockStage::LockDone;
}

HRESULT LockStep::TryGetLatestFinalizedManifest(_In_ const ManifestWrapVector& manifests, _In_ ManifestWrap& latestFinalizedManifest)
{
    // Pass 1: Find highest finalized manifest that is either non-conflict or the winner of a conflict.
    uint64_t latestEligibleVersion = 0;
    for (uint64_t i = 0; i < manifests.size(); ++i)
    {
        const ManifestWrap& manifest = manifests[i];
        if (ConvertToManifestStatusEnum(manifest.GetStatus()) != ManifestStatus::Finalized)
        {
            continue;
        }

        bool isLoser = false;
        if (auto const& conflictOpt = manifest.GetConflict(); conflictOpt.has_value())
        {
            if (!conflictOpt->GetIsWinner())
            {
                isLoser = true;
            }
        }
        if (isLoser)
        {
            continue;
        }

        uint64_t ver = StringToUint64(manifest.GetVersion());
        if (ver > latestEligibleVersion)
        {
            latestEligibleVersion = ver;
            latestFinalizedManifest = manifest;
        }
    }

    if (latestEligibleVersion != 0)
    {
        TRACE_INFORMATION("[GAME SAVE] LockStep: Latest Finalized Manifest v:%llu", latestEligibleVersion);
        return S_OK;
    }

    // Pass 2: Pathological fallback - no eligible winner/non-conflict found. Use highest loser.
    uint64_t highestLoserVersion = 0;
    ManifestWrap highestLoserManifest;
    for (uint64_t i = 0; i < manifests.size(); ++i)
    {
        const ManifestWrap& manifest = manifests[i];
        if (ConvertToManifestStatusEnum(manifest.GetStatus()) != ManifestStatus::Finalized)
        {
            continue;
        }
        bool loser = false;
        if (auto const& conflictOpt = manifest.GetConflict(); conflictOpt.has_value() && !conflictOpt->GetIsWinner())
        {
            loser = true;
        }
        if (loser)
        {
            uint64_t ver = StringToUint64(manifest.GetVersion());
            if (ver > highestLoserVersion)
            {
                highestLoserVersion = ver;
                highestLoserManifest = manifest;
            }
        }
    }

    if (highestLoserVersion != 0)
    {
        latestFinalizedManifest = highestLoserManifest;
        TRACE_WARNING("[GAME SAVE] Pathological manifest state: only conflict losers present. Using highest loser as latest finalized fallback v:%llu", highestLoserVersion);
    }
    else
    {
        // No finalized manifests at all.
        TRACE_INFORMATION("[GAME SAVE] No finalized manifests present when computing Latest Finalized (L)");
        latestFinalizedManifest = ManifestWrap(); // Ensure the output manifest is reset to a default state
    }
    return S_OK;
}

const ManifestWrap* LockStep::TryGetLatestPendingManifest(_In_ const ManifestWrapVector& manifests, _In_ ManifestWrap& latestFinalizedManifest)
{
    const ManifestWrap* latestPendingManifest = nullptr;
    uint64_t latestPendingVersion = 0;
    for (uint64_t i = 0; i < manifests.size(); i++)
    {
        const ManifestWrap& manifest = manifests[i];
        ManifestStatus manifestStatus = ConvertToManifestStatusEnum(manifest.GetStatus());
        if (manifestStatus == ManifestStatus::Initialized)
        {
            uint64_t manifestVer = StringToUint64(manifest.GetVersion());
            uint64_t latestFinalizedManifestVer = StringToUint64(latestFinalizedManifest.GetVersion());
            if (manifestVer <= latestFinalizedManifestVer)
            {
                continue; // ignore any old pending manifests
            }

            if (manifestVer > latestPendingVersion)
            {
                latestPendingVersion = manifestVer;
                latestPendingManifest = &manifest;
            }
        }
    }

    return latestPendingManifest;
}

HRESULT LockStep::AcquireActiveDevice(
    _In_ UICallbackManager& uiCallbackManager, 
    _In_ const RunContext& runContext, 
    _In_ ISchedulableTask& task, 
    _In_ std::recursive_mutex& folderSyncMutex,
    _In_ const String& saveFolder)
{
    ScopeTracer scopeTracer(FormatString("LockStep - %s", EnumName(m_stage)));
    m_telemetryManager->SetContextActivationCallingLocation(EnumValue<StateMachineLocation>(EnumName(m_stage)));

    switch (m_stage)
    {
        case LockStage::Login:
        {
            PFEntityHandle entityHandle = nullptr;
            HRESULT hr = PFLocalUserTryGetEntityHandle(
                m_localUser.Handle(),
                &entityHandle);
            if (SUCCEEDED(hr) && entityHandle != nullptr)
            {
                m_entity = Entity::Wrap(entityHandle);
                m_stage = LockStage::ListManifests;
                task.ScheduleNow();
            }
            else
            {
                TRACE_TASK("LocalUserLoginOperation");
                LocalUserLoginOperation::Run(m_localUser, runContext)
                .Finally([this, &task, &uiCallbackManager, &folderSyncMutex](Result<LoginResult> result)
                {
                    std::lock_guard<std::recursive_mutex> lock(folderSyncMutex); // Prevent any of the Finally blocks from changing the state while DoWork thread is active
                    TRACE_TASK(FormatString("LocalUserLoginOperationFinally HR:0x%0.8x", result.hr));
                    m_telemetryManager->SetContextActivationHttpInfo(result.httpResult);

                    if (SUCCEEDED(result.hr))
                    {
                        m_entity = std::move(result.ExtractPayload().entity);
                        m_stage = LockStage::ListManifests;
                        task.ScheduleNow();
                    }
                    else
                    {
                        m_stage = LockStage::WaitForFailedUI_Login;
                        if (false == uiCallbackManager.ShowSyncFailedUI(task, m_localUser, result.hr, PFGameSaveFilesSyncState::PreparingForDownload))
                        {
                            // No fail callback set, so just fail API
                            m_stage = LockStage::LockStepFailure;
                            m_failureHR = result.hr;
                            task.ScheduleNow();
                        }
                    }
                });
            }

            return S_OK;
        }

        case LockStage::ListManifests:
        {
            m_telemetryManager->CreateEntityTelemetryPipeline(m_localUser, runContext.TaskQueueHandle());
            m_telemetryManager->ResetContextActivation();
            m_telemetryManager->SetContextSyncSyncErrorSource(SyncErrorSource::SER_ListManifests);

            ListManifestsRequest request{};
            //request.SetEntity(m_entity.value().EntityKey());

            TRACE_TASK("ListManifests");
            GameSaveServiceSelector::ListManifests(m_entity.value(), request, runContext)
            .Finally([this, &task, &uiCallbackManager, &folderSyncMutex](Result<ListManifestsResponse> result)
            {
                std::lock_guard<std::recursive_mutex> lock(folderSyncMutex); // Prevent any of the Finally blocks from changing the state while DoWork thread is active
                TRACE_TASK(FormatString("ListManifestsFinally HR:0x%0.8x", result.hr));
                m_telemetryManager->SetContextActivationHttpInfo(result.httpResult);

                if (FAILED(result.hr))
                {
                    m_telemetryManager->SetContextActivationHResult(result.hr);
                    m_telemetryManager->EmitContextActivationFailureEvent();
                    m_stage = LockStage::WaitForFailedUI_ListManifests;
                    if( false == uiCallbackManager.ShowSyncFailedUI(task, m_localUser, result.hr, PFGameSaveFilesSyncState::PreparingForDownload) )
                    {
                        // No fail callback set, so just fail API
                        m_stage = LockStage::LockStepFailure;
                        m_failureHR = result.hr;
                        task.ScheduleNow();
                    }
                }
                else
                {
                    m_manifests = result.Payload().GetManifests();
                    m_nextAvailableVersion = result.Payload().GetNextAvailableVersion();

                    // Parse PerPlayerQuotaBytes from TitleConfiguration (if present)
                    m_parsedPerPlayerQuotaBytes = std::numeric_limits<int64_t>::max(); // default unlimited
                    auto titleConfig = result.Payload().GetTitleConfiguration();
                    if (titleConfig.has_value())
                    {
                        const String& quotaStr = titleConfig->GetPerPlayerQuotaBytes();
                        if (!quotaStr.empty())
                        {
                            // stoull may throw; wrap in try/catch to avoid exception crossing boundary
                            try
                            {
                                // std::stoull only has overloads for std::string / std::wstring. PlayFab::String uses a custom allocator,
                                // so it won't implicitly bind to the std::string overload. Construct a temporary std::string view of the data.
                                uint64_t parsed = std::stoull(std::string{ quotaStr.c_str() });
                                if (parsed == 0)
                                {
                                    // Spec: if parsed value is 0 treat as 256MB default
                                    parsed = 256ull * 1024ull * 1024ull;
                                }
                                if (parsed > static_cast<uint64_t>(std::numeric_limits<int64_t>::max()))
                                {
                                    m_parsedPerPlayerQuotaBytes = std::numeric_limits<int64_t>::max();
                                }
                                else
                                {
                                    m_parsedPerPlayerQuotaBytes = static_cast<int64_t>(parsed);
                                }
                            }
                            catch(...)
                            {
                                // Leave unlimited on parse failure per spec (missing/invalid -> unlimited)
                                m_parsedPerPlayerQuotaBytes = std::numeric_limits<int64_t>::max();
                            }
                        }

                        bool disableLastConflictLoser = titleConfig->GetDisableClientRollbackToLastConflictLoser();
                        bool disableLastKnownGood = titleConfig->GetDisableClientRollbackToLastKnownGood();
                        TRACE_INFORMATION("[GAME SAVE] TitleConfiguration rollback gates: disableLastConflictLoser=%s disableLastKnownGood=%s", 
                            disableLastConflictLoser ? "true" : "false", 
                            disableLastKnownGood ? "true" : "false");

                        PFGameSaveFilesAddUserOptions gated = m_addUserOptions;
                        if (disableLastKnownGood && (gated & PFGameSaveFilesAddUserOptions::RollbackToLastKnownGood) == PFGameSaveFilesAddUserOptions::RollbackToLastKnownGood)
                        {
                            gated = static_cast<PFGameSaveFilesAddUserOptions>(static_cast<uint32_t>(gated) & ~static_cast<uint32_t>(PFGameSaveFilesAddUserOptions::RollbackToLastKnownGood));
                            TRACE_INFORMATION("[GAME SAVE] Gated RollbackToLastKnownGood by title config");
                        }
                        if (disableLastConflictLoser && (gated & PFGameSaveFilesAddUserOptions::RollbackToLastConflict) == PFGameSaveFilesAddUserOptions::RollbackToLastConflict)
                        {
                            gated = static_cast<PFGameSaveFilesAddUserOptions>(static_cast<uint32_t>(gated) & ~static_cast<uint32_t>(PFGameSaveFilesAddUserOptions::RollbackToLastConflict));
                            TRACE_INFORMATION("[GAME SAVE] Gated RollbackToLastConflict by title config");
                        }

                        m_addUserOptions = gated;
                        if ((gated & PFGameSaveFilesAddUserOptions::RollbackToLastKnownGood) == PFGameSaveFilesAddUserOptions::RollbackToLastKnownGood)
                        {
                            TRACE_INFORMATION("[GAME SAVE] Rollback intent: LastKnownGood");
                        }
                        else if ((gated & PFGameSaveFilesAddUserOptions::RollbackToLastConflict) == PFGameSaveFilesAddUserOptions::RollbackToLastConflict)
                        {
                            TRACE_INFORMATION("[GAME SAVE] Rollback intent: LastConflict");
                        }
                        else if (m_addUserOptions != PFGameSaveFilesAddUserOptions::None)
                        {
                            TRACE_INFORMATION("[GAME SAVE] Proceeding with latest manifest");
                        }
                    }
                    m_stage = LockStage::SelectBaselineAndCheckContention;
                    task.ScheduleNow();
                }
            });

            return S_OK;
        }

        case LockStage::SelectBaselineAndCheckContention:
        {
            // Always start from the latest finalized manifest (L)
            ManifestWrap latestFinalizedL;
            // TODO: Previously passed ConflictOverlay for temporary conflict tracking
            // Now relies entirely on service-persisted conflict metadata
            TryGetLatestFinalizedManifest(m_manifests, latestFinalizedL);

            bool rollbackBaselineSelected = false;
            m_baselineFinalizedManifest = SelectBaselineManifest(latestFinalizedL, rollbackBaselineSelected);
            if (rollbackBaselineSelected)
            {
                TRACE_INFORMATION("[GAME SAVE] Using rollback baseline manifest v:%s instead of latest v:%s", m_baselineFinalizedManifest.GetVersion().c_str(), latestFinalizedL.GetVersion().c_str());
            }
            else
            {
                TRACE_INFORMATION("[GAME SAVE] Using latest finalized manifest v:%s as baseline", m_baselineFinalizedManifest.GetVersion().c_str());
            }

            // Log manifest description for diagnostics (helps debug empty description issues)
            const String& baselineDesc = m_baselineFinalizedManifest.GetManifestDescription();
            if (baselineDesc.empty())
            {
                TRACE_WARNING("[GAME SAVE] LockStep: Baseline manifest v:%s has EMPTY ManifestDescription", m_baselineFinalizedManifest.GetVersion().c_str());
            }
            else
            {
                String decodedDesc = Base64Decode(baselineDesc);
                TRACE_INFORMATION("[GAME SAVE] LockStep: Baseline manifest v:%s description='%s'", 
                    m_baselineFinalizedManifest.GetVersion().c_str(), 
                    decodedDesc.c_str());
            }

            m_telemetryManager->SetContextActivationBaseVersion(m_baselineFinalizedManifest.GetVersion());

            if (m_baselineFinalizedManifest.GetUploadProgress().has_value())
            {
                m_telemetryManager->SetContextActivationTotalSize(StringToUint64(m_baselineFinalizedManifest.GetUploadProgress().value().GetTotalBytes()));
                m_telemetryManager->SetContextActivationCompressedSize(StringToUint64(m_baselineFinalizedManifest.GetUploadProgress().value().GetUploadedBytes()));
            }

            const ManifestWrap* latestPendingManifest = TryGetLatestPendingManifest(m_manifests, m_baselineFinalizedManifest);

            bool showActiveDeviceContentionUI = false;
            if (latestPendingManifest != nullptr)
            {
                if (latestPendingManifest->GetMetadata().has_value())
                {
                    String localDeviceId = GetLocalDeviceID(saveFolder);
                    TRACE_INFORMATION("[GAME SAVE] LockStep: Latest Pending Manifest v:%s DeviceID:%s", 
                        latestPendingManifest->GetVersion().c_str(),
                        latestPendingManifest->GetMetadata()->GetDeviceId().c_str());
                    TRACE_INFORMATION("[GAME SAVE] LockStep: LocalID:%s", localDeviceId.c_str());

                    if (latestPendingManifest->GetMetadata()->GetDeviceId() != localDeviceId)
                    {
                        showActiveDeviceContentionUI = true;
                    }
                }
            }
            else
            {
                TRACE_INFORMATION("[GAME SAVE] LockStep: Latest Pending Manifest not found");
            }

            if (showActiveDeviceContentionUI && latestPendingManifest != nullptr)
            {
                m_stage = LockStage::WaitForActiveDeviceContentionUI;

                PFGameSaveDescriptor localGameSave{};
                FolderSyncManager::ConvertToPFGameSaveDescriptor(*latestPendingManifest, localGameSave);

                PFGameSaveDescriptor remoteGameSave{};
                FolderSyncManager::ConvertToPFGameSaveDescriptor(m_baselineFinalizedManifest, remoteGameSave);

                if (false == uiCallbackManager.ShowActiveDeviceContentionUI(task, m_localUser, localGameSave, remoteGameSave))
                {
                    // No callback set, so just assume cancel
                    return E_PF_GAMESAVE_USER_CANCELLED;
                }
            }
            else
            {
                m_stage = LockStage::CreatePendingManifest;
                task.ScheduleNow();
            }
            return S_OK;
        }

        case LockStage::CreatePendingManifest:
        {
            const ManifestWrap* latestPendingManifest = TryGetLatestPendingManifest(m_manifests, m_baselineFinalizedManifest);

            bool createNewManifest = false;
            uint64_t newManifestVersion = 0;
            uint64_t baseManifestVersion = 0;
            if (latestPendingManifest != nullptr)
            {
                uint64_t pendingVersion = StringToUint64(latestPendingManifest->GetVersion());
                uint64_t finalizedVersion = StringToUint64(m_baselineFinalizedManifest.GetVersion());

                // Only reuse a pending manifest if:
                // 1. It belongs to this device (DeviceId matches)
                // 2. Its version is greater than the finalized version (not stale)
                bool canReusePending = latestPendingManifest->GetMetadata().has_value() &&
                    latestPendingManifest->GetMetadata()->GetDeviceId() == GetLocalDeviceID(saveFolder) &&
                    pendingVersion > finalizedVersion;

                if (canReusePending)
                {
                    // No need to create a new manifest.  Just this one
                    m_latestPendingPFManifest = *latestPendingManifest;
                    m_telemetryManager->SetContextActivationManifestState(ManifestState::MS_Initialized);
                    m_telemetryManager->SetContextActivationContextVersion(m_latestPendingPFManifest.GetVersion());
                    TRACE_INFORMATION("[GAME SAVE] LockStep: Reusing latest pending v:%llu (finalized v:%llu)", pendingVersion, finalizedVersion);
                }
                else
                {
                    // Create a new manifest because either:
                    // - Different device owns the pending manifest, OR
                    // - Pending manifest is stale (version <= finalized version)
                    createNewManifest = true;
                    if (m_nextAvailableVersion.length() > 0)
                    {
                        newManifestVersion = StringToUint64(m_nextAvailableVersion);
                    }
                    else
                    {
                        newManifestVersion = StringToUint64(latestPendingManifest->GetVersion()) + 1;
                    }
                    if (newManifestVersion == 0)
                    {
                        newManifestVersion++; // don't use 0 as that mess with other logic in our client
                    }
                    baseManifestVersion = finalizedVersion;
                    if (baseManifestVersion == 0)
                    {
                        baseManifestVersion = newManifestVersion; // if no finalized manifest, set base version as itself
                    }
                    TRACE_INFORMATION("[GAME SAVE] LockStep: Creating new manifest v:%llu (pending v:%llu was stale or different device)", newManifestVersion, pendingVersion);
                }
            }
            else
            {
                // No previous manifest so create one
                if (m_nextAvailableVersion.length() > 0)
                {
                    newManifestVersion = StringToUint64(m_nextAvailableVersion);
                }
                else
                {
                    newManifestVersion = StringToUint64(m_baselineFinalizedManifest.GetVersion()) + 1;
                }                
                if (newManifestVersion == 0)
                {
                    newManifestVersion++; // don't use 0 as that mess with other logic in our client
                }
                baseManifestVersion = StringToUint64(m_baselineFinalizedManifest.GetVersion());
                if (baseManifestVersion == 0)
                {
                    baseManifestVersion = newManifestVersion; // if no finalized manifest, set base version as itself
                }
                TRACE_INFORMATION("[GAME SAVE] LockStep: No previous manifest. New v:%llu", newManifestVersion);
                createNewManifest = true;
            }

            if (createNewManifest)
            {
                TRACE_TASK("CreatePendingManifest.InitializeManifest");

                int64_t debugManifestOffset = GetDebugManifestOffset();
                InitializeManifestRequest initManifestRequest;
                CreateInitManifestRequest(m_entity.value(), initManifestRequest, baseManifestVersion, newManifestVersion, m_manifestVersionOffset + debugManifestOffset, saveFolder);

                GameSaveServiceSelector::InitializeManifest(m_entity.value(), initManifestRequest, runContext)
                .Finally([this, &task, &uiCallbackManager, &folderSyncMutex](Result<PlayFab::GameSaveWrapper::InitializeManifestResponse> result)
                {
                    std::lock_guard<std::recursive_mutex> lock(folderSyncMutex); // Prevent any of the Finally blocks from changing the state while DoWork thread is active
                    TRACE_TASK(FormatString("CreatePendingManifest.InitializeManifestFinally HR:0x%0.8x", result.hr));
                    m_telemetryManager->SetContextActivationHttpInfo(result.httpResult);

                    if (FAILED(result.hr))
                    {
                        m_telemetryManager->SetContextActivationHResult(result.hr);
                        m_telemetryManager->EmitContextActivationFailureEvent();

                        // service can return this if somehow reusing an previously used manifest number:
                        // {"code":409,"status":"Conflict","error":"GameSaveManifestVersionAlreadyExists","errorCode":20301,"errorMessage":"GameSaveManifestVersionAlreadyExists"}
                        if (result.hr == E_PF_GAME_SAVE_MANIFEST_VERSION_ALREADY_EXISTS)
                        {
                            m_stage = LockStage::CreatePendingManifest;
                            m_manifestVersionOffset++;
                            task.ScheduleNow();
                        }
                        else
                        {
                            m_stage = LockStage::WaitForFailedUI_CreatePendingManifest;
                            if (false == uiCallbackManager.ShowSyncFailedUI(task, m_localUser, result.hr, PFGameSaveFilesSyncState::PreparingForDownload))
                            {
                                // No fail callback set, so just fail API
                                m_stage = LockStage::LockStepFailure;
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
                            m_latestPendingPFManifest = resultManifest.value();
                            m_telemetryManager->SetContextActivationManifestState(ManifestState::MS_Initialized); // TODO: maybe setting context version always sets manifest state to initialized?
                            m_telemetryManager->SetContextActivationContextVersion(m_latestPendingPFManifest.GetVersion());
                        }
                        m_stage = LockStage::LockDone;

                        task.ScheduleNow();
                    }
                });
            }
            else
            {
                TRACE_TASK("CreatePendingManifest.InitializeManifestFinally.Skip");

                // Nothing needs else needs to happen. 
                // Don't call ScheduleNow() when moving to Done state outside async call.
                m_stage = LockStage::LockDone;

                task.ScheduleNow();
            }

            return S_OK;
        }

        case LockStage::WaitForActiveDeviceContentionUI:
        {
            TRACE_INFORMATION("[GAME SAVE] LockStep - WaitForActiveDeviceContentionUI: waiting for user response to active device contention");
            UIAction uiAction = uiCallbackManager.GetAction();

            if (uiAction == UIAction::UIActiveDeviceContentionSyncLastSaved)
            {
                TRACE_INFORMATION("[GAME SAVE] LockStep - WaitForActiveDeviceContentionUI: user chose SyncLastSaved");
                m_stage = LockStage::CreatePendingManifest;
                task.ScheduleNow();
                return S_OK;
            }
            else if (uiAction == UIAction::UIActiveDeviceContentionRetry)
            {
                TRACE_INFORMATION("[GAME SAVE] LockStep - WaitForActiveDeviceContentionUI: user chose Retry");
                m_stage = LockStage::ListManifests;
                task.ScheduleNow();
                return S_OK;
            }
            else if (uiAction == UIAction::UIActiveDeviceContentionCancel)
            {
                TRACE_WARNING("[GAME SAVE] LockStep - WaitForActiveDeviceContentionUI: user CANCELLED");
                return E_PF_GAMESAVE_USER_CANCELLED;
            }
            else
            {
                TRACE_VERBOSE("[GAME SAVE] LockStep - WaitForActiveDeviceContentionUI: waiting for user response (UIAction=%d)", static_cast<int>(uiAction));
                // No action set yet - user hasn't responded to the activeDeviceContentionCallback.
                // Return E_PENDING to keep the async operation alive.
                return E_PENDING;
            }
        }

        case LockStage::WaitForFailedUI_Login:
        {
            TRACE_INFORMATION("[GAME SAVE] LockStep - WaitForFailedUI_Login: waiting for user response to login failure");
            return uiCallbackManager.HandleFailedUI(task, 
                [this]() { 
                    TRACE_INFORMATION("[GAME SAVE] LockStep - WaitForFailedUI_Login: user chose RETRY, returning to Login stage");
                    m_stage = LockStage::Login; 
                },
                [this]() { 
                    TRACE_WARNING("[GAME SAVE] LockStep - WaitForFailedUI_Login: user chose OFFLINE MODE after login failure. Cloud sync will be skipped.");
                    m_forceDisconnectFromCloud = true; 
                    m_stage = LockStage::LockDone; 
                    m_telemetryManager->SetContextActivationStartedOnline(false); 
                }
            );
        }

        case LockStage::WaitForFailedUI_ListManifests:
        {
            TRACE_INFORMATION("[GAME SAVE] LockStep - WaitForFailedUI_ListManifests: waiting for user response to ListManifests failure");
            return uiCallbackManager.HandleFailedUI(task,
                [this]() { 
                    TRACE_INFORMATION("[GAME SAVE] LockStep - WaitForFailedUI_ListManifests: user chose RETRY");
                    m_stage = LockStage::ListManifests; 
                },
                [this]() { 
                    TRACE_WARNING("[GAME SAVE] LockStep - WaitForFailedUI_ListManifests: user chose OFFLINE MODE. Cloud sync will be skipped.");
                    m_forceDisconnectFromCloud = true; 
                    m_stage = LockStage::LockDone; 
                    m_telemetryManager->SetContextActivationStartedOnline(false); 
                }
            );
        }

        case LockStage::WaitForFailedUI_CreatePendingManifest:
        {
            TRACE_INFORMATION("[GAME SAVE] LockStep - WaitForFailedUI_CreatePendingManifest: waiting for user response");
            return uiCallbackManager.HandleFailedUI(task,
                [this]() { 
                    TRACE_INFORMATION("[GAME SAVE] LockStep - WaitForFailedUI_CreatePendingManifest: user chose RETRY");
                    m_stage = LockStage::CreatePendingManifest; 
                },
                [this]() { 
                    TRACE_WARNING("[GAME SAVE] LockStep - WaitForFailedUI_CreatePendingManifest: user chose OFFLINE MODE. Cloud sync will be skipped.");
                    m_forceDisconnectFromCloud = true; 
                    m_stage = LockStage::LockDone; 
                    m_telemetryManager->SetContextActivationStartedOnline(false); 
                }
            );
        }



        case LockStage::LockStepFailure:
        {
            return m_failureHR;
        }

        case LockStage::LockDone:
        {
            assert(false);
            return S_OK;
        }
    }

    return S_OK;
}

// Given the latest finalized manifest (L), apply any surviving rollback intent
// (LastKnownGood or LastConflict) to choose an alternate baseline. Returns the chosen
// baseline manifest. Sets baselineFromRollback=true if a rollback-specific manifest
// was selected instead of L.
// Selects the baseline manifest given L (latest finalized) and current rollback intent.
// Returns either L or an alternate rollback baseline; sets rollbackBaselineSelected=true if alternate chosen.
ManifestWrap LockStep::SelectBaselineManifest(const ManifestWrap& latestFinalizedL, bool& rollbackBaselineSelected)
{
    rollbackBaselineSelected = false;
    ManifestWrap selectedBaseline = latestFinalizedL; // default L

    // LastKnownGood intent
    if ((m_addUserOptions & PFGameSaveFilesAddUserOptions::RollbackToLastKnownGood) == PFGameSaveFilesAddUserOptions::RollbackToLastKnownGood)
    {
        uint64_t bestKGVersion = 0;
        ManifestWrap bestKGManifest;
        for (size_t i = 0; i < m_manifests.size(); ++i)
        {
            const auto& manifest = m_manifests[i];
            if (ConvertToManifestStatusEnum(manifest.GetStatus()) != ManifestStatus::Finalized)
            {
                continue;
            }
            auto isKnownGood = manifest.GetIsKnownGood();
            if (!isKnownGood.has_value() || !isKnownGood.value())
            {
                continue;
            }
            uint64_t ver = StringToUint64(manifest.GetVersion());
            if (ver > bestKGVersion)
            {
                bestKGVersion = ver;
                bestKGManifest = manifest;
            }
        }
        if (bestKGVersion != 0)
        {
            rollbackBaselineSelected = true;
            TRACE_INFORMATION("[GAME SAVE] Selected rollback baseline (LastKnownGood) v:%llu", bestKGVersion);
            return bestKGManifest;
        }
        TRACE_INFORMATION("[GAME SAVE] RollbackToLastKnownGood requested but no IsKnownGood manifest present. Falling back to latest v:%s", latestFinalizedL.GetVersion().c_str());
        return selectedBaseline; // fallback
    }

    // LastConflict intent
    if ((m_addUserOptions & PFGameSaveFilesAddUserOptions::RollbackToLastConflict) == PFGameSaveFilesAddUserOptions::RollbackToLastConflict)
    {
        uint64_t bestPairVersion = 0; // max(manifestVersion, conflictingVersion)
        ManifestWrap bestLoser;
        bool foundLoser = false;
        for (size_t i = 0; i < m_manifests.size(); ++i)
        {
            const auto& manifest = m_manifests[i];
            if (ConvertToManifestStatusEnum(manifest.GetStatus()) != ManifestStatus::Finalized)
            {
                continue;
            }

            // Conflict metadata lives in the optional Conflict wrapper, not directly on the manifest.
            String conflictingVersion; // empty means no conflict metadata
            bool isWinner = false;
            auto const& conflictOpt = manifest.GetConflict();
            if (conflictOpt.has_value())
            {
                conflictingVersion = conflictOpt->GetConflictingVersion();
                isWinner = conflictOpt->GetIsWinner();
            }

            if (conflictingVersion.empty() || isWinner)
            {
                continue; // need a loser side with conflicting version
            }

            uint64_t thisVer = StringToUint64(manifest.GetVersion());
            uint64_t conflictVer = conflictingVersion.empty() ? 0 : StringToUint64(conflictingVersion.c_str());
            uint64_t pairHigh = (thisVer > conflictVer) ? thisVer : conflictVer;
            if (pairHigh > bestPairVersion)
            {
                bestPairVersion = pairHigh;
                bestLoser = manifest;
                foundLoser = true;
            }
        }
        if (foundLoser)
        {
            rollbackBaselineSelected = true;
            uint64_t chosenVer = StringToUint64(bestLoser.GetVersion());
            TRACE_INFORMATION("[GAME SAVE] Selected rollback baseline (LastConflict loser) v:%llu", chosenVer);
            return bestLoser;
        }
        TRACE_INFORMATION("[GAME SAVE] RollbackToLastConflict requested but no conflict loser manifest present. Falling back to latest v:%s", latestFinalizedL.GetVersion().c_str());
        return selectedBaseline; // fallback
    }

    // No rollback intent
    return selectedBaseline;
}

void LockStep::CreateInitManifestRequest(
    _In_ const Entity& entity, 
    _In_ InitializeManifestRequest& request, 
    _In_ uint64_t baseManifestVersion,
    _In_ uint64_t newManifestVersion,
    _In_ uint64_t manifestVersionOffset,
    _In_ const String& saveFolder)
{
    UNREFERENCED_PARAMETER(entity);
    //request.SetEntity(entity.EntityKey());
    request.SetVersion(Uint64ToString(newManifestVersion + manifestVersionOffset));
    if (newManifestVersion == baseManifestVersion)
    {
        // keep base version in sync with the manifest version as we increment it
        request.SetBaseVersion(Uint64ToString(baseManifestVersion + manifestVersionOffset));
    }
    else
    {
        request.SetBaseVersion(Uint64ToString(baseManifestVersion));
    }

    PlayFab::Wrappers::PFGameSaveManifestMetadataWrapper<Allocator> metadata;
    metadata.SetDeviceId(GetLocalDeviceID(saveFolder));
    metadata.SetDeviceName(GetDeviceFriendlyName());
    metadata.SetDeviceType(GetDeviceType());
    metadata.SetDeviceVersion(GetDeviceVersion());
    request.SetMetadata(std::move(metadata));
}

int64_t LockStep::GetPerPlayerQuotaBytes() const
{
    return m_parsedPerPlayerQuotaBytes;
}

std::optional<PlayFab::Entity> LockStep::GetEntity() const
{
    return m_entity;
}

const ManifestWrap& LockStep::GetLatestPendingPFManifest() const
{
    return m_latestPendingPFManifest;
}

const ManifestWrap& LockStep::GetBaselineFinalizedManifest() const
{
    return m_baselineFinalizedManifest;
}

void LockStep::Reset()
{
    m_stage = LockStage::Login;
    m_forceDisconnectFromCloud = false;
    m_parsedPerPlayerQuotaBytes = std::numeric_limits<int64_t>::max();
    m_manifests.clear();
    m_entity.reset();
    m_latestPendingPFManifest = ManifestWrap();
    m_baselineFinalizedManifest = ManifestWrap();
}

} // namespace GameSave
} // namespace PlayFab
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
            {
                // Debug fault point (core unit test plan H-5). Fail is reported like a sign-in failure.
                HRESULT faultHr = S_OK;
                switch (CheckFaultPointForDebug("Login", faultHr))
                {
                    case PFGameSaveFaultAction::Pause:
                        task.ScheduleAfter(20);
                        return S_OK;
                    case PFGameSaveFaultAction::Abandon:
                        return E_ABORT;
                    case PFGameSaveFaultAction::Fail:
                        faultHr = FAILED(faultHr) ? faultHr : E_FAIL;
                        m_stage = LockStage::WaitForFailedUI_Login;
                        if (false == uiCallbackManager.ShowSyncFailedUI(task, m_localUser, faultHr, PFGameSaveFilesSyncState::PreparingForDownload))
                        {
                            m_stage = LockStage::LockStepFailure;
                            m_failureHR = faultHr;
                            task.ScheduleNow();
                        }
                        return S_OK;
                    default:
                        break;
                }
            }
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
            // The reset cleared the failure event's calling location (set on entry above): a
            // ListManifests failure must not be reported as StateMachineLocation::Unknown.
            m_telemetryManager->SetContextActivationCallingLocation(StateMachineLocation::ListManifests);
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
                                TRACE_WARNING("[GAME SAVE] LockStep: Failed to parse per-player quota, defaulting to unlimited");
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
            // A rollback that resolves to the latest manifest anyway changes nothing and needs no commit.
            m_rollbackBaselineSelected = rollbackBaselineSelected &&
                m_baselineFinalizedManifest.GetVersion() != latestFinalizedL.GetVersion();
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

                // These are NOT a local-vs-cloud choice. The first descriptor is the CURRENT lock
                // holder - the device whose Initialized/Uploading manifest blocks this one - and the
                // second is the PREVIOUS lock holder - the device of the finalized manifest this
                // activation will sync (the latest, or the rollback target when a rollback option
                // was passed), which completed its upload.
                PFGameSaveDescriptor currentLockHolderGameSave{};
                FolderSyncManager::ConvertToPFGameSaveDescriptor(*latestPendingManifest, currentLockHolderGameSave);
                // Last activity on the pending manifest (a description update, upload progress), not
                // merely when the lock was first taken.
                if (latestPendingManifest->GetLastUpdateTimestamp() != 0)
                {
                    currentLockHolderGameSave.time = latestPendingManifest->GetLastUpdateTimestamp();
                }

                PFGameSaveDescriptor previousLockHolderGameSave{};
                FolderSyncManager::ConvertToPFGameSaveDescriptor(m_baselineFinalizedManifest, previousLockHolderGameSave);
                // A finalized manifest's relevant time is when its upload completed; the creation
                // time is when that device took the lock, possibly long before it saved anything.
                const std::optional<time_t>& finalizationTimestamp = m_baselineFinalizedManifest.GetFinalizationTimestamp();
                if (finalizationTimestamp.has_value() && finalizationTimestamp.value() != 0)
                {
                    previousLockHolderGameSave.time = finalizationTimestamp.value();
                }
                // Nothing is pending on a finalized manifest.
                previousLockHolderGameSave.uploadedBytes = 0;

                TRACE_INFORMATION("[GAME SAVE] LockStep: contention current holder v:%s device:%s time=%lld, previous holder v:%s device:%s time=%lld",
                    latestPendingManifest->GetVersion().c_str(), currentLockHolderGameSave.deviceId, static_cast<long long>(currentLockHolderGameSave.time),
                    m_baselineFinalizedManifest.GetVersion().c_str(), previousLockHolderGameSave.deviceId, static_cast<long long>(previousLockHolderGameSave.time));

                if (false == uiCallbackManager.ShowActiveDeviceContentionUI(task, m_localUser, currentLockHolderGameSave, previousLockHolderGameSave))
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
                    // m_baselineFinalizedManifest is already the data-selection baseline
                    // (winner/non-conflict, or rollback target). Use it directly as the base.
                    // It was set by TryGetLatestFinalizedManifest or SelectBaselineManifest.
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
                // m_baselineFinalizedManifest is already the data-selection baseline
                // (winner/non-conflict, or rollback target). Use it directly as the base.
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
                            // Bounded like BASE_VERSION_NOT_AVAILABLE below. Without a cap a
                            // service that keeps returning 409 turns this into a hot retry loop
                            // that never completes the async op and never notifies the title.
                            constexpr uint32_t maxVersionExistsRetries = 5;
                            m_versionExistsRetryCount++;
                            if (m_versionExistsRetryCount <= maxVersionExistsRetries)
                            {
                                m_telemetryManager->ResetContextActivation();
                                m_stage = LockStage::CreatePendingManifest;
                                m_manifestVersionOffset++;
                                task.ScheduleNow();
                            }
                            else
                            {
                                TRACE_ERROR("[GAME SAVE] LockStep: MANIFEST_VERSION_ALREADY_EXISTS retry limit exceeded (%u attempts)", m_versionExistsRetryCount);
                                m_stage = LockStage::WaitForFailedUI_CreatePendingManifest;
                                if (false == uiCallbackManager.ShowSyncFailedUI(task, m_localUser, result.hr, PFGameSaveFilesSyncState::PreparingForDownload))
                                {
                                    m_stage = LockStage::LockStepFailure;
                                    m_failureHR = result.hr;
                                    task.ScheduleNow();
                                }
                            }
                        }
                        else if (result.hr == E_PF_GAME_SAVE_BASE_VERSION_NOT_AVAILABLE)
                        {
                            constexpr uint32_t maxBaseVersionRetries = 3;
                            m_baseVersionRetryCount++;
                            if (m_baseVersionRetryCount <= maxBaseVersionRetries)
                            {
                                // Base version is stale - re-fetch manifests to discover actual current version
                                TRACE_WARNING("[GAME SAVE] LockStep: BASE_VERSION_NOT_AVAILABLE (attempt %u/%u), re-fetching manifests",
                                    m_baseVersionRetryCount, maxBaseVersionRetries);
                                m_stage = LockStage::ListManifests;
                                m_manifestVersionOffset = 0;
                                task.ScheduleNow();
                            }
                            else
                            {
                                TRACE_ERROR("[GAME SAVE] LockStep: BASE_VERSION_NOT_AVAILABLE retry limit exceeded (%u attempts)", m_baseVersionRetryCount);
                                m_stage = LockStage::WaitForFailedUI_CreatePendingManifest;
                                if (false == uiCallbackManager.ShowSyncFailedUI(task, m_localUser, result.hr, PFGameSaveFilesSyncState::PreparingForDownload))
                                {
                                    m_stage = LockStage::LockStepFailure;
                                    m_failureHR = result.hr;
                                    task.ScheduleNow();
                                }
                            }
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
            else if (uiAction == UIAction::UISyncFailedCancel)
            {
                // Woken by uninitialize or XAsyncCancel (UICallbackManager::CancelPendingUIWait /
                // CancelUIWaitForTask), not answered: end the AddUser instead of waiting on with
                // nobody left to wake it (see CompareStep's WaitForConflictUI).
                TRACE_WARNING("[GAME SAVE] LockStep - WaitForActiveDeviceContentionUI: prompt ended by cancellation or shutdown");
                return E_ABORT;
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
                    m_baseVersionRetryCount = 0;
                    m_versionExistsRetryCount = 0;
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

ManifestWrap LockStep::SelectBaselineManifest(const ManifestWrap& latestFinalizedL, bool& rollbackBaselineSelected)
{
    return SelectBaselineManifest(m_manifests, m_addUserOptions, latestFinalizedL, rollbackBaselineSelected);
}

int64_t LockStep::GetPerPlayerQuotaBytes() const
{
    // Debug: PFGameSaveFilesSetTitleConfigOverrideForDebug simulates a quota the test service can't set.
    int64_t debugOverride = GetDebugPerPlayerQuotaOverride();
    if (debugOverride > 0)
    {
        return debugOverride;
    }
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
    m_rollbackBaselineSelected = false;
    m_manifestVersionOffset = 0;
    m_baseVersionRetryCount = 0;
    m_versionExistsRetryCount = 0;
}

} // namespace GameSave
} // namespace PlayFab
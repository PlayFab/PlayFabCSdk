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
    uint64_t latestFinalizedVersion = 0;
    for (uint64_t i = 0; i < manifests.size(); i++)
    {
        const ManifestWrap& manifest = manifests[i];
        if (ConvertToManifestStatusEnum(manifest.GetStatus()) != ManifestStatus::Finalized)
        {
            continue;
        }

        uint64_t curVer = StringToUint64(manifest.GetVersion());
        if (curVer > latestFinalizedVersion)
        {
            latestFinalizedVersion = curVer;
            latestFinalizedManifest = manifest;
        }
    }
    TRACE_INFORMATION("[GAME SAVE] LockStep: Latest Finalized Manifest v:%llu", latestFinalizedVersion);

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
                    m_stage = LockStage::ShowUIAsNeeded;
                    task.ScheduleNow();
                }
            });

            return S_OK;
        }

        case LockStage::ShowUIAsNeeded:
        {
            m_latestFinalizedPFManifest = ManifestWrap();
            TryGetLatestFinalizedManifest(m_manifests, m_latestFinalizedPFManifest);
            m_telemetryManager->SetContextActivationBaseVersion(m_latestFinalizedPFManifest.GetVersion());

            if (m_latestFinalizedPFManifest.GetUploadProgress().has_value())
            {
                m_telemetryManager->SetContextActivationTotalSize(StringToUint64(m_latestFinalizedPFManifest.GetUploadProgress().value().GetTotalBytes()));
                m_telemetryManager->SetContextActivationCompressedSize(StringToUint64(m_latestFinalizedPFManifest.GetUploadProgress().value().GetUploadedBytes()));
            }

            const ManifestWrap* latestPendingManifest = TryGetLatestPendingManifest(m_manifests, m_latestFinalizedPFManifest);

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
                FolderSyncManager::ConvertToPFGameSaveDescriptor(m_latestFinalizedPFManifest, remoteGameSave);

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
            const ManifestWrap* latestPendingManifest = TryGetLatestPendingManifest(m_manifests, m_latestFinalizedPFManifest);

            bool createNewManifest = false;
            uint64_t newManifestVersion = 0;
            uint64_t baseManifestVersion = 0;
            if (latestPendingManifest != nullptr)
            {
                if (latestPendingManifest->GetMetadata().has_value() && latestPendingManifest->GetMetadata()->GetDeviceId() == GetLocalDeviceID(saveFolder))
                {
                    // No need to create a new manifest.  Just this one
                    m_latestPendingPFManifest = *latestPendingManifest;
                    m_telemetryManager->SetContextActivationManifestState(ManifestState::MS_Initialized);
                    m_telemetryManager->SetContextActivationContextVersion(m_latestPendingPFManifest.GetVersion());
                    TRACE_INFORMATION("[GAME SAVE] LockStep: Reusing latest pending");
                }
                else
                {
                    // Different device was latest, so create a new Initialized manifest
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
                    baseManifestVersion = StringToUint64(m_latestFinalizedPFManifest.GetVersion());
                    if (baseManifestVersion == 0)
                    {
                        baseManifestVersion = newManifestVersion; // if no finalized manifest, set base version as itself
                    }
                    TRACE_INFORMATION("[GAME SAVE] LockStep: Creating new manifest v:%llu", newManifestVersion);
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
                    newManifestVersion = StringToUint64(m_latestFinalizedPFManifest.GetVersion()) + 1;
                }                
                if (newManifestVersion == 0)
                {
                    newManifestVersion++; // don't use 0 as that mess with other logic in our client
                }
                baseManifestVersion = StringToUint64(m_latestFinalizedPFManifest.GetVersion());
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
            UIAction uiAction = uiCallbackManager.GetAction();

            if (uiAction == UIAction::UIActiveDeviceContentionSyncLastSaved)
            {
                m_stage = LockStage::CreatePendingManifest;
                task.ScheduleNow();
                return S_OK;
            }
            else if (uiAction == UIAction::UIActiveDeviceContentionRetry)
            {
                m_stage = LockStage::ListManifests;
                task.ScheduleNow();
                return S_OK;
            }
            else if (uiAction == UIAction::UIActiveDeviceContentionCancel)
            {
                return E_PF_GAMESAVE_USER_CANCELLED;
            }
            return S_OK;
        }

        case LockStage::WaitForFailedUI_Login:
        {
            return uiCallbackManager.HandleFailedUI(task, 
                [this]() { m_stage = LockStage::Login; },
                [this]() { m_forceDisconnectFromCloud = true; m_stage = LockStage::LockDone; m_telemetryManager->SetContextActivationStartedOnline(false); }
            );
        }

        case LockStage::WaitForFailedUI_ListManifests:
        {
            return uiCallbackManager.HandleFailedUI(task,
                [this]() { m_stage = LockStage::ListManifests; },
                [this]() { m_forceDisconnectFromCloud = true; m_stage = LockStage::LockDone; m_telemetryManager->SetContextActivationStartedOnline(false); }
            );
        }

        case LockStage::WaitForFailedUI_CreatePendingManifest:
        {
            return uiCallbackManager.HandleFailedUI(task,
                [this]() { m_stage = LockStage::CreatePendingManifest; },
                [this]() { m_forceDisconnectFromCloud = true; m_stage = LockStage::LockDone; m_telemetryManager->SetContextActivationStartedOnline(false); }
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

const ManifestWrap& LockStep::GetLatestFinalizedPFManifest() const
{
    return m_latestFinalizedPFManifest;
}

void LockStep::Reset()
{
    m_stage = LockStage::Login;
    m_forceDisconnectFromCloud = false;
    m_parsedPerPlayerQuotaBytes = std::numeric_limits<int64_t>::max();
    m_manifests.clear();
    m_entity.reset();
    m_latestPendingPFManifest = ManifestWrap();
    m_latestFinalizedPFManifest = ManifestWrap();
}

} // namespace GameSave
} // namespace PlayFab
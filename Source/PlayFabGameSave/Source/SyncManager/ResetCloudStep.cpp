// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "LocalStateManifest.h"
#include "ResetCloudStep.h"
#include "LocalUserLoginOperation.h"

using namespace PlayFab::GameSaveWrapper;

namespace PlayFab
{
namespace GameSave
{

ResetCloudStep::ResetCloudStep(_In_ LocalUser const& localUser, _In_ SharedPtr<GameSaveTelemetryManager> telemetryManager) :
    m_localUser{ localUser },
    m_telemetryManager{ telemetryManager }
{
}

void ResetCloudStep::SetEntity(_In_ const Entity& entity)
{
    m_entity = entity;
}

bool ResetCloudStep::IsResetDone() const
{
    return m_stage == ResetCloudStage::ResetCloudDone;
}

bool ResetCloudStep::IsResetInProgress() const
{
    return m_started && m_stage != ResetCloudStage::ResetCloudDone && m_stage != ResetCloudStage::ResetCloudStepFailure;
}

void ResetCloudStep::Reset()
{
    // FolderSyncManager is reused for the lifetime of the user, so without this a second
    // ResetCloud would either report success without deleting anything (stage still Done) or
    // replay the cached failure forever.
    m_stage = ResetCloudStage::Login;
    m_started = false;
    m_resetHR = S_OK;
    m_deleteFailureHR = S_OK;
    m_manifests.clear();
    m_nextAvailableVersion.clear();
    m_manifestDeleteIndex = 0;
}

HRESULT ResetCloudStep::ResetCloud(
    _In_ const RunContext& runContext,
    _In_ ISchedulableTask& task,
    _In_ const String& saveFolder,
    _In_ std::recursive_mutex& folderSyncMutex
    )
{
    UNREFERENCED_PARAMETER(saveFolder);

    m_started = true;

    ScopeTracer scopeTracer(FormatString("ResetCloudStep - %s", EnumName(m_stage)));

    switch (m_stage)
    {
        case ResetCloudStage::Login:
        {
            PFEntityHandle entityHandle = nullptr;
            HRESULT hr = PFLocalUserTryGetEntityHandle(m_localUser.Handle(), &entityHandle);
            if (SUCCEEDED(hr) && entityHandle != nullptr)
            {
                m_entity = Entity::Wrap(entityHandle);
                m_stage = ResetCloudStage::ListManifests;
                task.ScheduleNow();
            }
            else
            {
                TRACE_TASK("LocalUserLoginOperation");
                LocalUserLoginOperation::Run(m_localUser, runContext)
                .Finally([this, &task, &folderSyncMutex](Result<LoginResult> result)
                {
                    std::lock_guard<std::recursive_mutex> lock(folderSyncMutex); // Prevent any of the Finally blocks from changing the state while DoWork thread is active
                    TRACE_TASK(FormatString("LocalUserLoginOperationFinally HR:0x%0.8x", result.hr));

                    if (SUCCEEDED(result.hr))
                    {
                        m_entity = std::move(result.ExtractPayload().entity);
                        m_stage = ResetCloudStage::ListManifests;
                        task.ScheduleNow();
                    }
                    else
                    {
                        m_stage = ResetCloudStage::ResetCloudStepFailure;
                        m_resetHR = result.hr;
                        task.ScheduleNow();
                    }
                });
            }

            return S_OK;
        }

        case ResetCloudStage::ResetCloudStepFailure:
        {
            return m_resetHR;
        }

        case ResetCloudStage::ListManifests:
        {
            m_telemetryManager->CreateEntityTelemetryPipeline(m_localUser, runContext.TaskQueueHandle());
            m_telemetryManager->ResetContextDelete();

            ListManifestsRequest request{};
            // Deliberately NOT requesting unavailable manifests. Asking for them
            // (SetIncludeUnavailable(true)) also returns every PendingDeletion version the title
            // has ever accumulated: on a well-used test title that turned a ~3 KB / 6-manifest
            // response into a 341 KB one, and materializing that many manifests wedged the
            // operation before the completion below ever ran, so ResetCloud never finished.
            // PendingDeletion versions are already being torn down service-side, so listing them
            // only to skip them below is pure cost. Quarantined versions are still deleted when
            // they appear in a normal listing (see the status check in DeleteManifests).
            TRACE_TASK("ListManifests");
            GameSaveServiceSelector::ListManifests(m_entity.value(), request, runContext)
            .Finally([this, &task, &folderSyncMutex](Result<ListManifestsResponse> result)
            {
                std::lock_guard<std::recursive_mutex> lock(folderSyncMutex); // Prevent any of the Finally blocks from changing the state while DoWork thread is active
                TRACE_TASK(FormatString("ListManifestsFinally HR:0x%0.8x", result.hr));

                if (SUCCEEDED(result.hr))
                {
                    m_manifests = result.Payload().GetManifests();
                    m_nextAvailableVersion = result.Payload().GetNextAvailableVersion();
                    m_manifestDeleteIndex = 0;
                    m_stage = ResetCloudStage::DeleteManifests;
                    task.ScheduleNow();
                }
                else
                {
                    m_stage = ResetCloudStage::ResetCloudStepFailure;
                    m_resetHR = result.hr;
                    task.ScheduleNow();
                }
            });

            return S_OK;
        }

        case ResetCloudStage::DeleteManifests:
        {
            for (; m_manifestDeleteIndex < m_manifests.size(); m_manifestDeleteIndex++)
            {
                const ManifestWrap& manifest = m_manifests[m_manifestDeleteIndex];
                ManifestStatus manifestStatus = ConvertToManifestStatusEnum(manifest.GetStatus());
                // Quarantined versions are cloud state too - skipping them meant ResetCloud
                // reported success while leaving the player's data behind. Only PendingDeletion is
                // skipped, since the service is already tearing those down.
                if (manifestStatus == ManifestStatus::Initialized ||
                    manifestStatus == ManifestStatus::Uploading ||
                    manifestStatus == ManifestStatus::Finalized ||
                    manifestStatus == ManifestStatus::Quarantined)
                {
                    m_telemetryManager->ResetContextDelete();
                    m_telemetryManager->SetContextDeleteStartTime();
                    m_telemetryManager->SetContextDeleteDeleteType(DeleteType::DT_Version);
                    m_telemetryManager->SetContextDeleteContextVersion(manifest.GetVersion());

                    // move forward since the for loop won't cycle due to early exit
                    m_manifestDeleteIndex++;

                    // Delete the manifest
                    DeleteManifestRequest deleteRequest;
                    deleteRequest.SetVersion(manifest.GetVersion());
                    GameSaveServiceSelector::DeleteManifest(m_entity.value(), deleteRequest, runContext)
                    .Finally([this, &task, &folderSyncMutex](Result<void> result)
                    {
                        std::lock_guard<std::recursive_mutex> lock(folderSyncMutex); // Prevent any of the Finally blocks from changing the state while DoWork thread is active
                        TRACE_TASK(FormatString("DeleteManifestFinally HR:0x%0.8x", result.hr));
                        m_telemetryManager->SetContextDeleteHttpInfo(result.httpResult);
                        if (FAILED(result.hr))
                        {
                            m_telemetryManager->SetContextDeleteHResult(result.hr);
                        }
                        m_telemetryManager->EmitContextDeleteEvent();

                        if (FAILED(result.hr))
                        {
                            // Keep going so the remaining versions are still attempted, but
                            // remember the first failure: reporting success while cloud saves
                            // survive would let a later AddUser recover data the title believes
                            // was deleted.
                            TRACE_WARNING("[GAME SAVE] ResetCloudStep: DeleteManifest failed HR:0x%0.8x, continuing with next manifest", result.hr);
                            if (SUCCEEDED(m_deleteFailureHR))
                            {
                                m_deleteFailureHR = result.hr;
                            }
                        }

                        // Loop back to delete the next manifest
                        m_stage = ResetCloudStage::DeleteManifests;
                        task.ScheduleNow();
                    });

                    // We found a manifest to delete, so wait for the async call to complete
                    return S_OK;
                }
            }

            // No more manifests to delete. If any deletion failed the cloud was not actually
            // reset, so surface that instead of reporting success.
            if (FAILED(m_deleteFailureHR))
            {
                TRACE_ERROR("[GAME SAVE] ResetCloudStep: one or more manifest deletions failed (HR:0x%0.8x)", m_deleteFailureHR);
                m_resetHR = m_deleteFailureHR;
                m_stage = ResetCloudStage::ResetCloudStepFailure;
                task.ScheduleNow();
                return S_OK;
            }

            m_stage = ResetCloudStage::ResetCloudDone;
            task.ScheduleNow();
            return S_OK;
        }

        case ResetCloudStage::ResetCloudDone:
        {
            assert(false);
            return S_OK;
        }
    }

    return S_OK;
}

} // namespace GameSave
} // namespace PlayFab

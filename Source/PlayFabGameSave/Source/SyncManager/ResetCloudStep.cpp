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

HRESULT ResetCloudStep::ResetCloud(
    _In_ const RunContext& runContext,
    _In_ ISchedulableTask& task,
    _In_ const String& saveFolder,
    _In_ std::recursive_mutex& folderSyncMutex
    )
{
    UNREFERENCED_PARAMETER(saveFolder);

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
                        m_stage = ResetCloudStage::LockStepFailure;
                        m_resetHR = result.hr;
                        task.ScheduleNow();
                    }
                });
            }

            return S_OK;
        }

        case ResetCloudStage::LockStepFailure:
        {
            return m_resetHR;
        }

        case ResetCloudStage::ListManifests:
        {
            m_telemetryManager->CreateEntityTelemetryPipeline(m_localUser, runContext.TaskQueueHandle());
            m_telemetryManager->ResetContextDelete();

            ListManifestsRequest request{};

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
                    m_stage = ResetCloudStage::LockStepFailure;
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
                if (manifestStatus == ManifestStatus::Initialized ||
                    manifestStatus == ManifestStatus::Uploading ||
                    manifestStatus == ManifestStatus::Finalized)
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
                        m_telemetryManager->EmitContextDeleteEvent();

                        // Loop back to delete the next manifest
                        m_stage = ResetCloudStage::DeleteManifests;
                        task.ScheduleNow();
                    });

                    // We found a manifest to delete, so wait for the async call to complete
                    return S_OK;
                }
            }

            // No more manifests to delete, so move to the next stage
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

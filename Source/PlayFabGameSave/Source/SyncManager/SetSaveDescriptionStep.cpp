// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "LocalStateManifest.h"
#include "SetSaveDescriptionStep.h"
#include "LocalUserLoginOperation.h"
#include "PlatformUtils.h"

using namespace PlayFab::GameSaveWrapper;

namespace PlayFab
{
namespace GameSave
{

SetSaveDescriptionStep::SetSaveDescriptionStep(_In_ LocalUser const& localUser, _In_ SharedPtr<GameSaveTelemetryManager> telemetryManager) :
    m_localUser{ localUser },
    m_telemetryManager{ telemetryManager }
{
}

void SetSaveDescriptionStep::Reset()
{
    m_stage = SetSaveDescriptionStage::Start;
    m_setHR = S_OK;
}

void SetSaveDescriptionStep::SetEntity(_In_ const Entity& entity)
{
    m_entity = entity;
}

bool SetSaveDescriptionStep::IsSetDone() const
{
    return m_stage == SetSaveDescriptionStage::Done;
}

HRESULT SetSaveDescriptionStep::SetSaveDescription(
    _In_ const RunContext& runContext,
    _In_ ISchedulableTask& task,
    _In_ std::recursive_mutex& folderSyncMutex,
    _In_ const String& shortSaveDescription,
    _In_ SharedPtr<ManifestInternal> latestPendingManifest
    )
{
    ScopeTracer scopeTracer(FormatString("SetSaveDescriptionStep - %s", EnumName(m_stage)));

    switch (m_stage)
    {
        case SetSaveDescriptionStage::Start:
        {
            if (!m_entity.has_value())
            {
                TRACE_ERROR("[GAME SAVE] SetSaveDescriptionStep: m_entity not set");
                return E_UNEXPECTED;
            }
            if (m_entity.value().Handle() == nullptr)
            {
                TRACE_ERROR("[GAME SAVE] SetSaveDescriptionStep: m_entity has null handle");
                return E_UNEXPECTED;
            }
            if (!latestPendingManifest)
            {
                TRACE_ERROR("[GAME SAVE] SetSaveDescriptionStep: latestPendingManifest is null");
                return E_UNEXPECTED;
            }

            String encodedDescription = Base64Encode(shortSaveDescription);

            TRACE_INFORMATION("[GAME SAVE] SetSaveDescriptionStep: setting description on manifest v:%s",
                latestPendingManifest->VersionString().c_str());

            UpdateManifestRequest request{};
            request.SetManifestDescription(encodedDescription);
            request.SetVersion(latestPendingManifest->VersionString());

            TRACE_TASK("SetSaveDescription");
            GameSaveServiceSelector::UpdateManifest(m_entity.value(), request, runContext)
            .Finally([this, &task, &folderSyncMutex](Result<UpdateManifestResponse> result)
            {
                std::lock_guard<std::recursive_mutex> lock(folderSyncMutex); // Prevent any of the Finally blocks from changing the state while DoWork thread is active
                TRACE_TASK(FormatString("SetSaveDescriptionFinally HR:0x%0.8x", result.hr));

                if (SUCCEEDED(result.hr))
                {
                    m_stage = SetSaveDescriptionStage::Done;
                    task.ScheduleNow();
                }
                else
                {
                    m_setHR = result.hr;
                    m_stage = SetSaveDescriptionStage::SetSaveDescStepFailure;
                    task.ScheduleNow();
                }
            });

            return S_OK;
        }

        case SetSaveDescriptionStage::SetSaveDescStepFailure:
        {
            return m_setHR;
        }

        case SetSaveDescriptionStage::Done:
        {
            assert(false);
            return S_OK;
        }
    }

    return S_OK;
}

} // namespace GameSave
} // namespace PlayFab

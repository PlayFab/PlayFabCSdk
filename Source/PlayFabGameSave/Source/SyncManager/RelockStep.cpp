// Copyright (C) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include "RelockStep.h"
#include "LockStep.h"

using namespace PlayFab::GameSaveWrapper;

namespace PlayFab
{
namespace GameSave
{

RelockStep::RelockStep(_In_ LocalUser const& localUser) :
    m_localUser{ localUser }
{
}

bool RelockStep::IsRelockDone() const
{
    return m_stage == RelockStage::RelockDone;
}

SharedPtr<ManifestInternal> RelockStep::ExtractPendingManifest()
{
    return MakeShared<ManifestInternal>(m_pendingManifest);
}

void RelockStep::Reset()
{
    m_stage = RelockStage::CreatePendingManifest;
    m_failureHR = S_OK;
    m_manifestVersionOffset = 0;
    m_refreshedBaseFinalizedVersion = 0;
    m_useRefreshedBase = false;
    m_nextAvailableVersion.clear();
    m_baseVersionRetryCount = 0;
    m_versionExistsRetryCount = 0;
    m_forceDisconnectFromCloud = false;
    m_pendingManifest = ManifestWrap();
}

HRESULT RelockStep::Relock(
    _In_ const RunContext& runContext,
    _In_ ISchedulableTask& task,
    _In_ UICallbackManager& uiCallbackManager,
    _In_ std::recursive_mutex& folderSyncMutex,
    _In_ const String& saveFolder,
    _In_ const Entity& entity,
    _In_ uint64_t baseFinalizedVersion)
{
    switch (m_stage)
    {
        case RelockStage::CreatePendingManifest:
        {
            TRACE_TASK("RelockStep.InitializeManifest");

            InitializeManifestRequest initManifestRequest;
            uint64_t effectiveBase = m_useRefreshedBase ? m_refreshedBaseFinalizedVersion : baseFinalizedVersion;
            // Prefer service-provided NextAvailableVersion (accounts for filtered-out manifests).
            // Fall back to effectiveBase + 1 if not available or invalid.
            uint64_t newVersion = 0;
            if (!m_nextAvailableVersion.empty())
            {
                newVersion = StringToUint64(m_nextAvailableVersion);
                if (newVersion == 0)
                {
                    TRACE_WARNING("[GAME SAVE] RelockStep: Invalid NextAvailableVersion '%s', using effectiveBase + 1", m_nextAvailableVersion.c_str());
                }
            }
            if (newVersion == 0)
            {
                newVersion = effectiveBase + 1;
            }
            newVersion = std::max(newVersion, effectiveBase + 1);
            LockStep::CreateInitManifestRequest(entity, initManifestRequest, effectiveBase, newVersion, m_manifestVersionOffset, saveFolder);

            TRACE_INFORMATION("[GAME SAVE] RelockStep: re-acquiring pending manifest (base v%llu, offset %llu)",
                effectiveBase, m_manifestVersionOffset);

            GameSaveServiceSelector::InitializeManifest(entity, initManifestRequest, runContext)
            .Finally([this, &task, &uiCallbackManager, &folderSyncMutex](Result<InitializeManifestResponse> result)
            {
                std::lock_guard<std::recursive_mutex> lock(folderSyncMutex);
                TRACE_TASK(FormatString("RelockStep.InitializeManifestFinally HR:0x%0.8x", result.hr));

                bool forceFail = GetForceSyncFailedError() && SUCCEEDED(result.hr);

                if (forceFail || FAILED(result.hr))
                {
                    HRESULT failHr = forceFail ? E_FAIL : result.hr;

                    if (result.hr == E_PF_GAME_SAVE_MANIFEST_VERSION_ALREADY_EXISTS)
                    {
                        // Bounded like BASE_VERSION_NOT_AVAILABLE below - an uncapped immediate
                        // retry would spin on the service and never complete the async op.
                        constexpr uint32_t maxVersionExistsRetries = 5;
                        m_versionExistsRetryCount++;
                        if (m_versionExistsRetryCount <= maxVersionExistsRetries)
                        {
                            m_stage = RelockStage::CreatePendingManifest;
                            m_manifestVersionOffset++;
                            task.ScheduleNow();
                        }
                        else
                        {
                            TRACE_ERROR("[GAME SAVE] RelockStep: MANIFEST_VERSION_ALREADY_EXISTS retry limit exceeded (%u attempts)", m_versionExistsRetryCount);
                            m_stage = RelockStage::WaitForFailedUI_CreatePendingManifest;
                            if (false == uiCallbackManager.ShowSyncFailedUI(task, m_localUser, result.hr, PFGameSaveFilesSyncState::Uploading))
                            {
                                m_stage = RelockStage::RelockStepFailure;
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
                            // Base version is stale - call ListManifests to discover actual current version
                            TRACE_WARNING("[GAME SAVE] RelockStep: BASE_VERSION_NOT_AVAILABLE (attempt %u/%u), refreshing manifests",
                                m_baseVersionRetryCount, maxBaseVersionRetries);
                            m_stage = RelockStage::RefreshManifests;
                            m_manifestVersionOffset = 0;
                            task.ScheduleNow();
                        }
                        else
                        {
                            TRACE_ERROR("[GAME SAVE] RelockStep: BASE_VERSION_NOT_AVAILABLE retry limit exceeded (%u attempts)", m_baseVersionRetryCount);
                            m_stage = RelockStage::WaitForFailedUI_CreatePendingManifest;
                            if (false == uiCallbackManager.ShowSyncFailedUI(task, m_localUser, result.hr, PFGameSaveFilesSyncState::Uploading))
                            {
                                m_stage = RelockStage::RelockStepFailure;
                                m_failureHR = result.hr;
                                task.ScheduleNow();
                            }
                        }
                    }
                    else
                    {
                        m_stage = RelockStage::WaitForFailedUI_CreatePendingManifest;
                        if (false == uiCallbackManager.ShowSyncFailedUI(task, m_localUser, failHr, PFGameSaveFilesSyncState::Uploading))
                        {
                            // No fail callback set, so just fail API
                            m_stage = RelockStage::RelockStepFailure;
                            m_failureHR = failHr;
                            task.ScheduleNow();
                        }
                        // When ShowSyncFailedUI returns true, SetAction already called ScheduleNow.
                        // Do NOT call task.ScheduleNow() again — the task may have already completed
                        // via re-entrant DoWork triggered by the synchronous callback.
                    }
                }
                else
                {
                    auto& resultManifest = result.Payload().GetManifest();
                    if (resultManifest.has_value())
                    {
                        m_pendingManifest = resultManifest.value();
                        m_stage = RelockStage::RelockDone;
                        task.ScheduleNow();
                    }
                    else
                    {
                        TRACE_ERROR("[GAME SAVE] RelockStep: InitializeManifest succeeded but returned no manifest");
                        m_stage = RelockStage::RelockStepFailure;
                        m_failureHR = E_UNEXPECTED;
                        task.ScheduleNow();
                    }
                }
            });
            return S_OK;
        }

        case RelockStage::WaitForFailedUI_CreatePendingManifest:
        {
            HRESULT uiHr = uiCallbackManager.HandleFailedUI(task,
                [this]() {
                    m_baseVersionRetryCount = 0;
                    m_versionExistsRetryCount = 0;
                    m_stage = RelockStage::CreatePendingManifest;
                },
                [this]() {
                    m_forceDisconnectFromCloud = true;
                    m_stage = RelockStage::RelockDone;
                }
            );
            return uiHr;
        }

        case RelockStage::RefreshManifests:
        {
            TRACE_TASK("RelockStep.RefreshManifests");

            ListManifestsRequest request{};
            GameSaveServiceSelector::ListManifests(entity, request, runContext)
            .Finally([this, &task, &uiCallbackManager, &folderSyncMutex](Result<ListManifestsResponse> result)
            {
                std::lock_guard<std::recursive_mutex> lock(folderSyncMutex);
                TRACE_TASK(FormatString("RelockStep.RefreshManifestsFinally HR:0x%0.8x", result.hr));

                if (FAILED(result.hr))
                {
                    m_stage = RelockStage::WaitForFailedUI_RefreshManifests;
                    if (false == uiCallbackManager.ShowSyncFailedUI(task, m_localUser, result.hr, PFGameSaveFilesSyncState::Uploading))
                    {
                        m_stage = RelockStage::RelockStepFailure;
                        m_failureHR = result.hr;
                        task.ScheduleNow();
                    }
                }
                else
                {
                    // Use data-selection baseline (winner/non-conflict) for relock base
                    ManifestWrapVector refreshedManifests = result.Payload().GetManifests();
                    ManifestWrap latestFinalizedForBase;
                    LockStep::TryGetLatestFinalizedManifest(refreshedManifests, latestFinalizedForBase);

                    if (!latestFinalizedForBase.GetVersion().empty())
                    {
                        m_refreshedBaseFinalizedVersion = StringToUint64(latestFinalizedForBase.GetVersion());
                        m_useRefreshedBase = true;
                        m_nextAvailableVersion = result.Payload().GetNextAvailableVersion();
                        TRACE_INFORMATION("[GAME SAVE] RelockStep: refreshed base version to v%llu", m_refreshedBaseFinalizedVersion);
                        m_stage = RelockStage::CreatePendingManifest;
                        task.ScheduleNow();
                    }
                    else
                    {
                        // No finalized manifest found — cannot determine base version, fail the operation
                        TRACE_ERROR("[GAME SAVE] RelockStep: ListManifests returned no finalized manifest, cannot determine base version");
                        m_stage = RelockStage::WaitForFailedUI_RefreshManifests;
                        if (false == uiCallbackManager.ShowSyncFailedUI(task, m_localUser, E_PF_GAME_SAVE_BASE_VERSION_NOT_AVAILABLE, PFGameSaveFilesSyncState::Uploading))
                        {
                            m_stage = RelockStage::RelockStepFailure;
                            m_failureHR = E_PF_GAME_SAVE_BASE_VERSION_NOT_AVAILABLE;
                            task.ScheduleNow();
                        }
                    }
                }
            });
            return S_OK;
        }

        case RelockStage::WaitForFailedUI_RefreshManifests:
        {
            HRESULT uiHr = uiCallbackManager.HandleFailedUI(task,
                [this]() {
                    m_stage = RelockStage::RefreshManifests;
                },
                [this]() {
                    m_forceDisconnectFromCloud = true;
                    m_stage = RelockStage::RelockDone;
                }
            );
            return uiHr;
        }

        case RelockStage::RelockStepFailure:
        {
            return m_failureHR;
        }

        case RelockStage::RelockDone:
        {
            assert(false);
            return E_UNEXPECTED;
        }
    }

    return E_UNEXPECTED;
}

} // namespace GameSave
} // namespace PlayFab

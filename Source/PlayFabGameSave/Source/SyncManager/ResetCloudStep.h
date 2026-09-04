// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include "ExtendedManifest.h"
#include "Manifest.h"

namespace PlayFab
{
namespace GameSave
{
    
// Forward declare
class FolderSyncManager;

enum class ResetCloudStage
{
    Login = 0,
    ListManifests,
    DeleteManifests,
    ResetCloudStepFailure,
    ResetCloudDone
};

class ResetCloudStep
{
public:
    ResetCloudStep(_In_ LocalUser const& localUser, _In_ SharedPtr<GameSaveTelemetryManager> telemetryManager);
    void SetEntity(_In_ const Entity& entity);

    bool IsResetDone() const;
    // True once ResetCloud has started and has not finished. Callers use this to reject a second,
    // overlapping ResetCloud rather than resetting the state machine underneath the running one.
    bool IsResetInProgress() const;
    void Reset();
    HRESULT ResetCloud(
        _In_ const RunContext& runContext,
        _In_ ISchedulableTask& task,
        _In_ const String& saveFolder,
        _In_ std::recursive_mutex& folderSyncMutex
    );

private:
    LocalUser m_localUser;
    std::optional<Entity> m_entity;
    ResetCloudStage m_stage{ ResetCloudStage::Login };
    bool m_started{ false }; // set once ResetCloud() has been entered; cleared by Reset()
    HRESULT m_resetHR{ S_OK };
    HRESULT m_deleteFailureHR{ S_OK }; // first DeleteManifest failure seen during this reset
    ManifestWrapVector m_manifests;
    String m_nextAvailableVersion;
    uint64_t m_manifestDeleteIndex{ 0 };
    SharedPtr<GameSaveTelemetryManager> m_telemetryManager;
};

} // namespace GameSave
} // namespace PlayFab

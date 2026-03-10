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
    HRESULT m_resetHR{ S_OK };
    ManifestWrapVector m_manifests;
    String m_nextAvailableVersion;
    uint64_t m_manifestDeleteIndex{ 0 };
    SharedPtr<GameSaveTelemetryManager> m_telemetryManager;
};

} // namespace GameSave
} // namespace PlayFab

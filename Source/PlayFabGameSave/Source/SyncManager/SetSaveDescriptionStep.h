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

enum class SetSaveDescriptionStage
{
    Start = 0,
    LockStepFailure,
    Done
};

class SetSaveDescriptionStep
{
public:
    SetSaveDescriptionStep(_In_ LocalUser const& localUser, _In_ SharedPtr<GameSaveTelemetryManager> telemetryManager);
    void SetEntity(_In_ const Entity& entity);

    bool IsSetDone() const;
    HRESULT SetSaveDescription(
        _In_ const RunContext& runContext,
        _In_ ISchedulableTask& task,
        _In_ std::recursive_mutex& folderSyncMutex,
        _In_ const String& shortSaveDescription,
        _In_ SharedPtr<ManifestInternal> latestPendingManifest
    );

    void Reset();

private:
    LocalUser m_localUser;
    std::optional<Entity> m_entity;
    SetSaveDescriptionStage m_stage{ SetSaveDescriptionStage::Start };
    HRESULT m_setHR{ S_OK };
    SharedPtr<GameSaveTelemetryManager> m_telemetryManager;
};

} // namespace GameSave
} // namespace PlayFab

// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

namespace PlayFab
{
namespace GameSave
{
    
// Forward declare
class FolderSyncManager;

enum class LockStage
{
    Login = 0,
    ListManifests,
    ShowUIAsNeeded,
    CreatePendingManifest,
    WaitForActiveDeviceContentionUI,
    WaitForFailedUI_Login,
    WaitForFailedUI_ListManifests,
    WaitForFailedUI_CreatePendingManifest,
    LockStepFailure,
    LockDone
};

class LockStep
{
public:
    LockStep(_In_ LocalUser const& localUser, _In_ SharedPtr<GameSaveTelemetryManager> telemetryManager);
    bool IsLockAcquired() const;
    bool IsForceDisconnectFromCloud() const { return m_forceDisconnectFromCloud; }
    HRESULT AcquireActiveDevice(
        _In_ UICallbackManager& uiCallbackManager, 
        _In_ const RunContext& runContext, 
        _In_ ISchedulableTask& task, 
        _In_ std::recursive_mutex& folderSyncMutex,
        _In_ const String& saveFolder
    );
    int64_t GetPerPlayerQuotaBytes() const;
    std::optional<Entity> GetEntity() const;
    const ManifestWrap& GetLatestPendingPFManifest() const;
    const ManifestWrap& GetLatestFinalizedPFManifest() const;
    void Reset();

    static void CreateInitManifestRequest(
        _In_ const Entity& entity, 
        _In_ PlayFab::GameSaveWrapper::InitializeManifestRequest& request, 
        _In_ uint64_t baseManifestVersion,
        _In_ uint64_t newManifestVersion,
        _In_ uint64_t manifestVersionOffset,
        _In_ const String& saveFolder);
    static HRESULT TryGetLatestFinalizedManifest(_In_ const ManifestWrapVector& manifests, _In_ ManifestWrap& latestFinalizedManifest);
    static const ManifestWrap* TryGetLatestPendingManifest(_In_ const ManifestWrapVector& manifests, _In_ ManifestWrap& latestFinalizedManifest);

    void SetAddUserOptions(PFGameSaveFilesAddUserOptions o) { m_addUserOptions = o; }
    PFGameSaveFilesAddUserOptions GetAddUserOptions() const { return m_addUserOptions; }
	
private:
    LocalUser m_localUser;
    std::optional<Entity> m_entity;
    int64_t m_parsedPerPlayerQuotaBytes{ std::numeric_limits<int64_t>::max() };
    ManifestWrapVector m_manifests;
    String m_nextAvailableVersion;
    ManifestWrap m_latestPendingPFManifest;
    ManifestWrap m_latestFinalizedPFManifest;
    bool m_forceDisconnectFromCloud{ false };
    LockStage m_stage{ LockStage::Login };
    HRESULT m_failureHR{ S_OK };
    uint64_t m_manifestVersionOffset{ 0 };
    SharedPtr<GameSaveTelemetryManager> m_telemetryManager;
    PFGameSaveFilesAddUserOptions m_addUserOptions{ PFGameSaveFilesAddUserOptions::None };
};

} // namespace GameSave
} // namespace PlayFab
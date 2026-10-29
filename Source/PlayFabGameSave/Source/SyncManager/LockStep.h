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
    SelectBaselineAndCheckContention,
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
    const ManifestWrap& GetBaselineFinalizedManifest() const; // Returns the finalized manifest chosen as the baseline for this activation (may be latest or a rollback-selected alternate)
    // True when the baseline is an older manifest chosen by a rollback option rather than the latest.
    bool IsRollbackBaselineSelected() const { return m_rollbackBaselineSelected; }
    ManifestWrap SelectBaselineManifest(const ManifestWrap& latestFinalizedL, bool& rollbackBaselineSelected);
    // The rule behind the member above, on an explicit manifest list and AddUser options.
    // LockStepRules.cpp, with the other static rules below.
    static ManifestWrap SelectBaselineManifest(
        _In_ const ManifestWrapVector& manifests,
        _In_ PFGameSaveFilesAddUserOptions addUserOptions,
        _In_ const ManifestWrap& latestFinalizedL,
        _Out_ bool& rollbackBaselineSelected);
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

    // The AddUser options of this activation. Taken only while the step is at its start: the
    // ListManifests stage removes the rollback options TitleConfiguration disables
    // (DisableClientRollbackToLastKnownGood / ...LastConflictLoser), and FolderSyncManager passes the
    // requested options again on every pump until the lock is acquired, which put them back.
    void SetAddUserOptions(PFGameSaveFilesAddUserOptions o)
    {
        if (m_stage == LockStage::Login)
        {
            m_addUserOptions = o;
        }
    }
    PFGameSaveFilesAddUserOptions GetAddUserOptions() const { return m_addUserOptions; }
	
private:
    LocalUser m_localUser;
    std::optional<Entity> m_entity;
    int64_t m_parsedPerPlayerQuotaBytes{ std::numeric_limits<int64_t>::max() };
    ManifestWrapVector m_manifests;
    String m_nextAvailableVersion;
    ManifestWrap m_latestPendingPFManifest;
    ManifestWrap m_baselineFinalizedManifest;
    bool m_rollbackBaselineSelected{ false };
    bool m_forceDisconnectFromCloud{ false };
    LockStage m_stage{ LockStage::Login };
    HRESULT m_failureHR{ S_OK };
    uint64_t m_manifestVersionOffset{ 0 };
    uint32_t m_versionExistsRetryCount{ 0 }; // Retry cap for MANIFEST_VERSION_ALREADY_EXISTS
    uint32_t m_baseVersionRetryCount{ 0 }; // Retry cap for BASE_VERSION_NOT_AVAILABLE
    SharedPtr<GameSaveTelemetryManager> m_telemetryManager;
    PFGameSaveFilesAddUserOptions m_addUserOptions{ PFGameSaveFilesAddUserOptions::None };
};

} // namespace GameSave
} // namespace PlayFab
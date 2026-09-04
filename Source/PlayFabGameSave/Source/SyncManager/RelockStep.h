// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include "Manifest.h"

namespace PlayFab
{
namespace GameSave
{

// Re-acquire a pending manifest after a previous upload's TakeLock failed or was cancelled.
// Follows the same state-machine pattern as LockStep::CreatePendingManifest.
enum class RelockStage
{
    CreatePendingManifest = 0,
    RefreshManifests,       // ListManifests to discover actual base version after BASE_VERSION_NOT_AVAILABLE
    WaitForFailedUI_CreatePendingManifest,
    WaitForFailedUI_RefreshManifests,
    RelockStepFailure,
    RelockDone
};

class RelockStep
{
public:
    RelockStep(_In_ LocalUser const& localUser);

    HRESULT Relock(
        _In_ const RunContext& runContext,
        _In_ ISchedulableTask& task,
        _In_ UICallbackManager& uiCallbackManager,
        _In_ std::recursive_mutex& folderSyncMutex,
        _In_ const String& saveFolder,
        _In_ const Entity& entity,
        _In_ uint64_t baseFinalizedVersion
    );

    bool IsRelockDone() const;
    bool IsForceDisconnectFromCloud() const { return m_forceDisconnectFromCloud; }
    SharedPtr<ManifestInternal> ExtractPendingManifest();
    void Reset();

private:
    LocalUser m_localUser;
    RelockStage m_stage{ RelockStage::CreatePendingManifest };
    HRESULT m_failureHR{ S_OK };
    uint64_t m_manifestVersionOffset{ 0 };
    uint64_t m_refreshedBaseFinalizedVersion{ 0 }; // Updated base version after ListManifests refresh
    bool m_useRefreshedBase{ false };               // True after successful ListManifests refresh
    String m_nextAvailableVersion;                   // From ListManifests response
    uint32_t m_baseVersionRetryCount{ 0 };          // Retry cap for BASE_VERSION_NOT_AVAILABLE
    uint32_t m_versionExistsRetryCount{ 0 };        // Retry cap for MANIFEST_VERSION_ALREADY_EXISTS
    bool m_forceDisconnectFromCloud{ false };
    ManifestWrap m_pendingManifest;
};

} // namespace GameSave
} // namespace PlayFab

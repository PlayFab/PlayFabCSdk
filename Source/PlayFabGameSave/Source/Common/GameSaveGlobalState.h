// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include <chrono>
#include "FolderSyncManager.h"
#include "PFGameSaveFilesForDebug.h"

// Forward declaration of HCInitArgs
struct HCInitArgs;

namespace PlayFab
{
namespace GameSave
{

class GameSaveAPIProvider;

class GameSaveGlobalState : public ITerminationListener
{
public:
    virtual ~GameSaveGlobalState() noexcept;

    static HRESULT Create(_In_opt_ HCInitArgs* args, _In_ uint64_t options, _In_opt_ XTaskQueueHandle backgroundQueue) noexcept;
    static HRESULT Get(SharedPtr<GameSaveGlobalState>& state) noexcept;
    static HRESULT CleanupAsync(XAsyncBlock* async) noexcept;

    String GetDebugRootFolderOverride() { std::lock_guard<std::recursive_mutex> lock(m_managersMutex); return m_debugRootSaveFolderOverride; }
    String GetDebugDeviceIdOverride() { std::lock_guard<std::recursive_mutex> lock(m_managersMutex); return m_debugDeviceIdOverride; }
    String GetDebugMockDataOverride() { std::lock_guard<std::recursive_mutex> lock(m_managersMutex); return m_debugMockDataOverride; }
    bool GetForceOutOfStorageError() { return m_forceOutOfStorageError; }
    bool GetForceSyncFailedError() { return m_forceSyncFailedError; }
    bool GetForceNullPendingManifest() { return m_forceNullPendingManifest; }
    bool GetWriteManifestsToDisk() { return m_writeManifests; }
    int64_t GetDebugManifestOffset() { return m_debugManifestOffset; }
    // Debug: TitleConfiguration.PerPlayerQuotaBytes override; 0 means no override.
    int64_t GetDebugPerPlayerQuotaOverride() { return m_debugPerPlayerQuotaOverride; }
    void SetDebugPerPlayerQuotaOverride(int64_t quotaBytes) { m_debugPerPlayerQuotaOverride = quotaBytes; }
    // Debug: simulate a provider whose metadata lives apart from game storage (wipe detection).
    bool GetDebugSimulateSeparateMetadataStorage() const { return m_debugSimulateSeparateMetadataStorage.load(); }
    void SetDebugSimulateSeparateMetadataStorage(bool enable) { m_debugSimulateSeparateMetadataStorage.store(enable); }

    // Debug fault injection (PFGameSaveFilesSetFaultInjectionForDebug). Check returns the action to
    // take at `point` (None when unset) and bumps its hit count; Fail and Abandon are one-shot.
    void SetDebugFault(const String& point, PFGameSaveFaultAction action, HRESULT hr)
    {
        std::lock_guard<std::recursive_mutex> lock(m_managersMutex);
        if (action == PFGameSaveFaultAction::None)
        {
            m_debugFaults.erase(point);
            return;
        }
        NoteFaultInjectionSetForDebug(); // fault checks leave their fast path for the rest of the process
        m_debugFaults[point] = DebugFault{ action, hr, 0 };
    }
    void ResumeDebugFault(const String& point)
    {
        std::lock_guard<std::recursive_mutex> lock(m_managersMutex);
        auto it = m_debugFaults.find(point);
        if (it != m_debugFaults.end() && (it->second.action == PFGameSaveFaultAction::Pause || it->second.action == PFGameSaveFaultAction::PauseOnce))
        {
            it->second.action = PFGameSaveFaultAction::None;
            it->second.holding = false;
        }
    }
    // PauseOnce: true while the one call it caught is held (until resumed or cleared).
    bool IsDebugFaultHolding(const String& point)
    {
        std::lock_guard<std::recursive_mutex> lock(m_managersMutex);
        auto it = m_debugFaults.find(point);
        return it != m_debugFaults.end() && it->second.action == PFGameSaveFaultAction::PauseOnce && it->second.holding;
    }
    uint32_t GetDebugFaultHitCount(const String& point)
    {
        std::lock_guard<std::recursive_mutex> lock(m_managersMutex);
        auto it = m_debugFaults.find(point);
        return it == m_debugFaults.end() ? 0 : it->second.hits;
    }
    PFGameSaveFaultAction CheckDebugFault(const String& point, _Out_ HRESULT& hr)
    {
        std::lock_guard<std::recursive_mutex> lock(m_managersMutex);
        hr = S_OK;
        auto it = m_debugFaults.find(point);
        if (it == m_debugFaults.end())
        {
            return PFGameSaveFaultAction::None;
        }
        it->second.hits++;
        PFGameSaveFaultAction action = it->second.action;
        if (action == PFGameSaveFaultAction::Count)
        {
            return PFGameSaveFaultAction::None; // counted only; the operation is unaffected
        }
        hr = it->second.hr;
        if (action == PFGameSaveFaultAction::FailAlways)
        {
            return PFGameSaveFaultAction::Fail; // every hit fails until the point is cleared
        }
        if (action == PFGameSaveFaultAction::PauseOnce)
        {
            if (it->second.holding)
            {
                return PFGameSaveFaultAction::None; // only the first call is held
            }
            it->second.holding = true;
            return PFGameSaveFaultAction::Pause;
        }
        if (action == PFGameSaveFaultAction::Fail || action == PFGameSaveFaultAction::Abandon)
        {
            it->second.action = PFGameSaveFaultAction::None; // one-shot; the hit count stays readable
        }
        return action;
    }

    // Debug: download link expiry (PFGameSaveFilesSetDownloadUrlExpiryForDebug). Links are "issued"
    // by every successful GetManifestDownloadDetails; a download through an expired link fails with
    // HTTP 403 like an expired SAS. ExpireNow: the next download finds its link expired (one-shot,
    // then Off); LifetimeSeconds: links older than the lifetime are expired; ExpireIssued: every link
    // issued so far is expired, links issued later are valid.
    void SetDebugDownloadUrlExpiry(PFGameSaveUrlExpiryMode mode, uint32_t seconds)
    {
        std::lock_guard<std::recursive_mutex> lock(m_managersMutex);
        m_debugUrlExpiryMode = mode;
        m_debugUrlLifetimeSeconds = seconds;
        m_debugUrlExpiredGeneration = m_debugUrlLinksGeneration;
    }
    void OnDebugDownloadLinksIssued()
    {
        std::lock_guard<std::recursive_mutex> lock(m_managersMutex);
        m_debugUrlIssuedAt = std::chrono::steady_clock::now();
        m_debugUrlLinksGeneration++;
    }
    bool IsDebugDownloadUrlExpired()
    {
        std::lock_guard<std::recursive_mutex> lock(m_managersMutex);
        switch (m_debugUrlExpiryMode)
        {
            case PFGameSaveUrlExpiryMode::ExpireNow:
                m_debugUrlExpiryMode = PFGameSaveUrlExpiryMode::Off;
                return true;
            case PFGameSaveUrlExpiryMode::LifetimeSeconds:
                return std::chrono::steady_clock::now() - m_debugUrlIssuedAt > std::chrono::seconds(m_debugUrlLifetimeSeconds);
            case PFGameSaveUrlExpiryMode::ExpireIssued:
                return m_debugUrlLinksGeneration == m_debugUrlExpiredGeneration; // no fresh links since
            default:
                return false;
        }
    }

    void SetDebugRootFolderOverride(const String& s) { std::lock_guard<std::recursive_mutex> lock(m_managersMutex); m_debugRootSaveFolderOverride = s; }
    void SetDebugDeviceIdOverride(const String& s) { std::lock_guard<std::recursive_mutex> lock(m_managersMutex); m_debugDeviceIdOverride = s; }
    void SetDebugMockDataOverride(const String& s) { std::lock_guard<std::recursive_mutex> lock(m_managersMutex); m_debugMockDataOverride = s; }
    void SetForceOutOfStorageError(_In_ bool forceError) { m_forceOutOfStorageError = forceError; }
    void SetForceSyncFailedError(_In_ bool forceError) { m_forceSyncFailedError = forceError; }
    void SetForceNullPendingManifest(_In_ bool force) { m_forceNullPendingManifest = force; }
    void SetWriteManifestsToDisk(_In_ bool writeManifests) { m_writeManifests = writeManifests; }
    void SetDebugManifestOffset(int64_t offset) { m_debugManifestOffset = offset; }

    String GetInitArgsSaveRootFolder() { std::lock_guard<std::recursive_mutex> lock(m_managersMutex); return m_initArgsRootSaveFolder; }
    void SetInitArgsSaveRootFolder(const String& s) { std::lock_guard<std::recursive_mutex> lock(m_managersMutex); m_initArgsRootSaveFolder = s; }

    String GetLocalDeviceID() { std::lock_guard<std::recursive_mutex> lock(m_managersMutex); return m_localDeviceID; }
    void SetLocalDeviceID(const String& s) { std::lock_guard<std::recursive_mutex> lock(m_managersMutex); m_localDeviceID = s; }

    bool GetForceInproc() const { return m_forceInproc; }
    void SetForceInproc(bool forceInproc) { m_forceInproc = forceInproc; }

    // Pre-init static flag: can be set before Create() to force in-proc provider selection
    static bool GetPreInitForceInproc() noexcept;
    static void SetPreInitForceInproc(_In_ bool forceInproc) noexcept;


    SharedPtr<FolderSyncManager> GetFolderSyncManagerFromLocalUser(PFLocalUserHandle handle, bool createOnDemand);

public:
    RunContext RunContext() const noexcept;
    GameSaveAPIProvider& ApiProvider() noexcept;
    
    // Cancel any pending UI waits across all FolderSyncManagers.
    // Must be called before termination to prevent hangs when providers are waiting for UI callbacks.
    void CancelAllPendingUIWaits() noexcept;

private:
    GameSaveGlobalState(bool uninitPlayFabCore, _In_opt_ XTaskQueueHandle backgroundQueue) noexcept;

    void OnTerminated(void* context) noexcept override;
    static HRESULT CALLBACK CleanupAsyncProvider(XAsyncOp op, XAsyncProviderData const* data);

    PlayFab::RunContext m_runContext;
    bool m_uninitPlayFabCore{ false };
    RecursiveMutex m_managersMutex;
    Map<String, SharedPtr<FolderSyncManager>> m_managers; // LocalUserId -> SharedPtr<FolderSyncManager>
    String m_initArgsRootSaveFolder;
    String m_mountedRootSaveFolder;
    String m_debugRootSaveFolderOverride;
    String m_debugDeviceIdOverride;
    std::atomic<int64_t> m_debugManifestOffset{ 0 };
    std::atomic<int64_t> m_debugPerPlayerQuotaOverride{ 0 };
    std::atomic<bool> m_debugSimulateSeparateMetadataStorage{ false };
    struct DebugFault
    {
        PFGameSaveFaultAction action{ PFGameSaveFaultAction::None };
        HRESULT hr{ S_OK };
        uint32_t hits{ 0 };
        bool holding{ false }; // PauseOnce: the first call is held
    };
    Map<String, DebugFault> m_debugFaults; // guarded by m_managersMutex
    PFGameSaveUrlExpiryMode m_debugUrlExpiryMode{ PFGameSaveUrlExpiryMode::Off }; // guarded by m_managersMutex
    uint32_t m_debugUrlLifetimeSeconds{ 0 };
    std::chrono::steady_clock::time_point m_debugUrlIssuedAt{ std::chrono::steady_clock::now() };
    uint64_t m_debugUrlLinksGeneration{ 0 };   // guarded by m_managersMutex: download-details responses so far
    uint64_t m_debugUrlExpiredGeneration{ 0 }; // guarded by m_managersMutex: ExpireIssued's cut-off
    String m_debugMockDataOverride;
    String m_localDeviceID;
    std::atomic<bool> m_forceOutOfStorageError{ false };
    std::atomic<bool> m_forceSyncFailedError{ false };
    std::atomic<bool> m_forceNullPendingManifest{ false };
    std::atomic<bool> m_forceInproc{ false };
    std::atomic<bool> m_writeManifests{ false };
    UniquePtr<GameSaveAPIProvider> m_apiProvider{};

};

} // namespace GameSave
} // namespace PlayFab
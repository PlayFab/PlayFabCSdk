// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include "FolderSyncManager.h"

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
    std::recursive_mutex m_managersMutex;
    Map<String, SharedPtr<FolderSyncManager>> m_managers; // LocalUserId -> SharedPtr<FolderSyncManager>
    String m_initArgsRootSaveFolder;
    String m_mountedRootSaveFolder;
    String m_debugRootSaveFolderOverride;
    String m_debugDeviceIdOverride;
    std::atomic<int64_t> m_debugManifestOffset{ 0 };
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
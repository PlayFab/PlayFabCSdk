// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include "FolderSyncManager.h"
#include "Platform/PFGameSaveFilesAPIProvider.h"

// Forward declaration of HCInitArgs
struct HCInitArgs;

namespace PlayFab
{
namespace GameSave
{

class GameSaveGlobalState : public ITerminationListener
{
public:
    virtual ~GameSaveGlobalState() noexcept;

    static HRESULT Create(_In_opt_ HCInitArgs* args, _In_ uint64_t options, _In_opt_ XTaskQueueHandle backgroundQueue) noexcept;
    static HRESULT Get(SharedPtr<GameSaveGlobalState>& state) noexcept;
    static HRESULT CleanupAsync(XAsyncBlock* async) noexcept;

    String GetDebugRootFolderOverride() { return m_debugRootSaveFolderOverride; }
    String GetDebugDeviceIdOverride() { return m_debugDeviceIdOverride; }
    String GetDebugMockDataOverride() { return m_debugMockDataOverride; }
    bool GetForceOutOfStorageError() { return m_forceOutOfStorageError; }
    bool GetForceSyncFailedError() { return m_forceSyncFailedError; }
    bool GetWriteManifestsToDisk() { return m_writeManifests; }
    int64_t GetDebugManifestOffset() { return m_debugManifestOffset; }

    void SetDebugRootFolderOverride(const String& s) { m_debugRootSaveFolderOverride = s; }
    void SetDebugDeviceIdOverride(const String& s) { m_debugDeviceIdOverride = s; }
    void SetDebugMockDataOverride(const String& s) { m_debugMockDataOverride = s; }
    void SetForceOutOfStorageError(_In_ bool forceError) { m_forceOutOfStorageError = forceError; }
    void SetForceSyncFailedError(_In_ bool forceError) { m_forceSyncFailedError = forceError; }
    void SetWriteManifestsToDisk(_In_ bool writeManifests) { m_writeManifests = writeManifests; }
    void SetDebugManifestOffset(size_t offset) { m_debugManifestOffset = offset; }

    String GetInitArgsSaveRootFolder() { return m_initArgsRootSaveFolder; }
    void SetInitArgsSaveRootFolder(const String& s) { m_initArgsRootSaveFolder = s; }

    const String& GetLocalDeviceID() { return m_localDeviceID; }
    void SetLocalDeviceID(const String& s) { m_localDeviceID = s; }

    bool GetForceInproc() const { return m_forceInproc; }
    void SetForceInproc(bool forceInproc) { m_forceInproc = forceInproc; }


    SharedPtr<FolderSyncManager> GetFolderSyncManagerFromLocalUser(PFLocalUserHandle handle, bool createOnDemand);

public:
    RunContext RunContext() const noexcept;
    GameSaveAPIProvider& ApiProvider() noexcept;

private:
    GameSaveGlobalState(bool uninitPlayFabCore, _In_opt_ XTaskQueueHandle backgroundQueue) noexcept;

    void OnTerminated(void* context) noexcept override;
    static HRESULT CALLBACK CleanupAsyncProvider(XAsyncOp op, XAsyncProviderData const* data);

    PlayFab::RunContext m_runContext;
    bool m_uninitPlayFabCore{ false };
    Map<String, SharedPtr<FolderSyncManager>> m_managers; // LocalUserId -> SharedPtr<FolderSyncManager>
    String m_initArgsRootSaveFolder;
    String m_debugRootSaveFolderOverride;
    String m_debugDeviceIdOverride;
    size_t m_debugManifestOffset{ 0 };
    String m_debugMockDataOverride;
    String m_localDeviceID;
    bool m_forceOutOfStorageError{ false };
    bool m_forceSyncFailedError{ false };
    bool m_forceInproc{ false };
    bool m_writeManifests{ false };
    UniquePtr<GameSaveAPIProvider> m_apiProvider{};

};

} // namespace GameSave
} // namespace PlayFab
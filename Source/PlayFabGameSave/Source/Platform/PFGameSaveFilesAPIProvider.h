// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once
#include "stdafx.h"
#include "ApiHelpers.h"
#include "DownloadAsyncProvider.h"
#include "UploadAsyncProvider.h"
#include "FileResetCloudAsyncProvider.h"
#include "PFGameSaveFilesForDebug.h"
#include "GameSaveServiceMock.h"

namespace PlayFab
{
namespace GameSave
{

// Sentinel marker file written to game storage after a successful sync.
// Used on platforms with separate metadata storage to detect if the game
// file container was deleted externally (e.g. by the user via system UI).
constexpr const char* PFGS_GAME_STORAGE_MARKER_FILENAME = "pfgs_marker";
constexpr const char* PFGS_GAME_STORAGE_MARKER_FOLDER = "cloudsync";

class GameSaveAPIProvider
{
public:
    virtual ~GameSaveAPIProvider() = default;
    virtual HRESULT Initialize(_In_ PFGameSaveInitArgs* args) noexcept = 0;
    virtual HRESULT UninitializeAsync(_Inout_ XAsyncBlock* async) noexcept = 0;
    virtual HRESULT UninitializeResult(_Inout_ XAsyncBlock* async) noexcept = 0;
    virtual HRESULT SetActiveDeviceChangedCallback(
        _In_opt_ XTaskQueueHandle callbackQueue,
        _In_opt_ PFGameSaveFilesActiveDeviceChangedCallback* callback,
        _In_opt_ void* context
    ) noexcept = 0;
    virtual HRESULT SetUiCallbacks(
        _In_ PFGameSaveUICallbacks* callbacks
    ) noexcept = 0;
    virtual HRESULT UiProgressGetProgress(
        _In_ PFLocalUserHandle localUserHandle,
        _Out_opt_ PFGameSaveFilesSyncState* syncState,
        _Out_opt_ uint64_t* current,
        _Out_opt_ uint64_t* total
    ) noexcept = 0;
    virtual HRESULT GetFolderSize(
        _In_ PFLocalUserHandle localUserHandle,
        _Out_ size_t* saveRootFolderSize
    ) noexcept = 0;
    virtual HRESULT GetFolder(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ size_t saveRootFolderSize,
        _Out_writes_(saveRootFolderSize) char* saveRootFolderBuffer,
        _Out_opt_ size_t* saveRootFolderUsed
    ) noexcept = 0;
    virtual HRESULT AddUserWithUiAsync(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveFilesAddUserOptions options,
        _Inout_ XAsyncBlock* async
    ) noexcept = 0;
    virtual HRESULT AddUserWithUiResult(_Inout_ XAsyncBlock* async) noexcept = 0;
    virtual HRESULT GetRemainingQuota(
        _In_ PFLocalUserHandle localUserHandle,
        _Out_ int64_t* remainingQuota
    ) noexcept = 0;
    virtual HRESULT IsConnectedToCloud(
        _In_ PFLocalUserHandle localUserHandle,
        _Out_ bool* isConnectedToCloud
    ) noexcept = 0;
    virtual HRESULT UploadWithUiAsync(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveFilesUploadOption option,
        _Inout_ XAsyncBlock* async
    ) noexcept = 0;
    virtual HRESULT UploadWithUiResult(_Inout_ XAsyncBlock* async) noexcept = 0;
    virtual HRESULT SetUiProgressResponse(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveFilesUiProgressUserAction action
    ) noexcept = 0;
    virtual HRESULT SetUiSyncFailedResponse(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveFilesUiSyncFailedUserAction action
    ) noexcept = 0;
    virtual HRESULT SetUiActiveDeviceContentionResponse(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveFilesUiActiveDeviceContentionUserAction action
    ) noexcept = 0;
    virtual HRESULT SetUiConflictResponse(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveFilesUiConflictUserAction action
    ) noexcept = 0;
    virtual HRESULT SetUiOutOfStorageResponse(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ PFGameSaveFilesUiOutOfStorageUserAction action
    ) noexcept = 0;
    virtual HRESULT SetMockDeviceIdForDebug(_In_ const char* deviceId) noexcept = 0;
    virtual HRESULT SetMockManifestOffsetForDebug(_In_ size_t offset) noexcept = 0;
    virtual HRESULT SetMockDataFolderForDebug(_In_ const char* mockDataFolder) noexcept = 0;
    virtual HRESULT GetStatsJsonSizeForDebug(_In_ PFLocalUserHandle localUserHandle, _Out_ size_t* jsonSize) noexcept = 0;
    virtual HRESULT GetStatsJsonForDebug(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ size_t jsonSize,
        _Out_writes_(jsonSize) char* jsonBuffer,
        _Out_opt_ size_t* jsonSizeUsed
    ) noexcept = 0;
    virtual HRESULT GetSaveDescriptionSizeForDebug(_In_ PFLocalUserHandle localUserHandle, _Out_ size_t* descriptionSize) noexcept = 0;
    virtual HRESULT GetSaveDescriptionForDebug(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ size_t descriptionSize,
        _Out_writes_(descriptionSize) char* descriptionBuffer,
        _Out_opt_ size_t* descriptionSizeUsed
    ) noexcept = 0;
    virtual HRESULT SetForceOutOfStorageErrorForDebug(_In_ bool forceError) noexcept = 0;
    virtual HRESULT SetForceSyncFailedErrorForDebug(_In_ bool forceError) noexcept = 0;
    virtual HRESULT SetForceNullPendingManifestForDebug(_In_ bool force) noexcept = 0;
    virtual HRESULT SetWriteManifestsToDiskForDebug(_In_ bool writeManifests) noexcept = 0;
    virtual HRESULT PauseUploadForDebug() noexcept = 0;
    virtual HRESULT SetMockForceOfflineForDebug(_In_ GameSaveServiceMockForcedOffline mode) noexcept = 0;
    virtual HRESULT ResumeUploadForDebug() noexcept = 0;
    virtual HRESULT ResetCloudAsync(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ XAsyncBlock* async
    ) noexcept = 0;
    virtual HRESULT ResetCloudResult(_Inout_ XAsyncBlock* async) noexcept = 0;
    virtual HRESULT SetSaveDescriptionAsync(
        _In_ PFLocalUserHandle localUserHandle,
        _In_ const char* shortSaveDescription,
        _In_ XAsyncBlock* async
    ) noexcept = 0;
    virtual HRESULT SetSaveDescriptionResult(_Inout_ XAsyncBlock* async) noexcept = 0;

    // Optional callbacks for in-process download/upload scenarios - only some platforms need these to be defined
    virtual HRESULT DownloadProcessingComplete() noexcept { return S_OK; }
    virtual HRESULT UploadProcessingComplete() noexcept { return S_OK; }

    // Get the temp storage path for compressed files (zips).
    // Returns empty string on platforms that don't use temp storage.
    virtual String GetTempCloudSyncPath() noexcept { return String{}; }

    // Clean up temporary files in the temp cloud sync folder.
    // Default implementation is a no-op for platforms that don't use temp storage.
    virtual HRESULT CleanupTempCloudSyncFiles() noexcept { return S_OK; }

    // Returns true on platforms where SetFileLastModifiedTime is a no-op.
    // When true, localstate.json uses lastSyncRemoteTimeLastModified for remote change detection.
    virtual bool PlatformNeedsRemoteTimestamp() noexcept { return false; }

    // Returns true on platforms where cloud sync metadata (localstate.json) is stored
    // in a separate container from game files. When true, the game file container can be
    // deleted independently of the metadata, requiring sentinel-based wipe detection.
    // Default is false for platforms that store metadata alongside game files.
    virtual bool HasSeparateMetadataStorage() noexcept { return false; }

    // Prepare cloud sync metadata storage for reading or writing localstate.json.
    // On platforms that store metadata separately from game files, this acquires access
    // to that storage. Must be paired with ReleaseCloudSyncStorage().
    // If cloudSyncRootPath is set to non-empty, it overrides the saveFolder for localstate.json I/O.
    // Default is a no-op for platforms that store metadata alongside game files.
    virtual HRESULT AcquireCloudSyncStorage(_Out_ String& cloudSyncRootPath) noexcept
    {
        cloudSyncRootPath = String{};
        return S_OK;
    }

    // Release cloud sync metadata storage after reading or writing localstate.json.
    // Commits any pending writes and releases the storage.
    virtual HRESULT ReleaseCloudSyncStorage() noexcept { return S_OK; }

    // Re-acquire game file storage (e.g. after it was released early by ReleaseGameStorage).
    // Needed when a subsequent step requires access to game files (e.g. download after conflict upload).
    // If acquiredPath is set to non-empty, the caller should use it as the new save folder path
    // (the mount point may have changed).
    virtual HRESULT AcquireGameStorage(_Out_ String& acquiredPath) noexcept
    {
        acquiredPath = String{};
        return S_OK;
    }

    // Release game file storage early (e.g. after file compression, before network upload).
    // Allows the platform to free game storage while upload proceeds over the network.
    virtual HRESULT ReleaseGameStorage() noexcept { return S_OK; }

    // Check if the game file storage was deleted externally (e.g. user removed the storage
    // container from system UI) while cloud sync metadata still exists.
    // Only meaningful on platforms where HasSeparateMetadataStorage() returns true.
    // Returns true if a forced full re-download is needed.
    virtual bool IsGameStorageWiped(const String& /*saveFolder*/) noexcept { return false; }
};

} // namespace GameSave
} // namespace PlayFab

// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

extern "C"
{

/// <summary>
/// Progress of the current operation callback.
/// </summary>
/// <param name="localUserHandle">Local user handle.</param>
/// <param name="syncState">Sync state of the operation.</param>
typedef void CALLBACK PFGameSaveFilesUiProgressCallback(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFGameSaveFilesSyncState syncState,
    _In_opt_ void* context);

/// <summary>
/// Sync failed callback.
/// </summary>
/// <param name="localUserHandle">Local user handle.</param>
/// <param name="syncState">Sync state of the operation.</param>
/// <param name="error">The failure error code.</param>
/// <param name="context">Context to pass to the callback.</param>
typedef void CALLBACK PFGameSaveFilesUiSyncFailedCallback(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFGameSaveFilesSyncState syncState,
    _In_ HRESULT error,
    _In_opt_ void* context);

/// <summary>
/// Active device contention callback.
/// </summary>
/// <param name="localUserHandle">Local user handle.</param>
/// <param name="localGameSave">Descriptor of local save game.</param>
/// <param name="remoteGameSave">Descriptor of remote save game.</param>
/// <param name="context">Context to pass to the callback.</param>
typedef void CALLBACK PFGameSaveFilesUiActiveDeviceContentionCallback(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFGameSaveDescriptor* localGameSave,
    _In_ PFGameSaveDescriptor* remoteGameSave,
    _In_opt_ void* context);

/// <summary>
/// Conflict callback.
/// </summary>
/// <param name="localGameSave">Descriptor of local save game.</param>
/// <param name="remoteGameSave">Descriptor of remote save game.</param>
/// <param name="context">Context to pass to the callback.</param>
typedef void CALLBACK PFGameSaveFilesUiConflictCallback(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFGameSaveDescriptor* localGameSave,
    _In_ PFGameSaveDescriptor* remoteGameSave,
    _In_opt_ void* context);

/// <summary>
/// Callback triggered when the local device is out of storage space.
/// </summary>
/// <param name="localUserHandle">Local user handle.</param>
/// <param name="requiredBytes">Required bytes.</param>
/// <param name="context">Context to pass to the callback.</param>
typedef void CALLBACK PFGameSaveFilesUiOutOfStorageCallback(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ uint64_t requiredBytes,
    _In_opt_ void* context);


/// <summary>
/// UI callbacks for game save operations.
/// </summary>
typedef struct PFGameSaveUICallbacks
{
    /// <summary>
    /// Optional callback for upload and download progress.
    /// </summary>
    PFGameSaveFilesUiProgressCallback* progressCallback;

    /// <summary>
    /// Context to pass to the progressCallback.
    /// </summary>
    void* progressContext;

    /// <summary>
    /// Optional callback for sync failure.
    /// </summary>
    PFGameSaveFilesUiSyncFailedCallback* syncFailedCallback;

    /// <summary>
    /// Context to pass to the syncFailedCallback.
    /// </summary>
    void* syncFailedContext;

    /// <summary>
    /// Optional callback for active device contention
    /// </summary>
    PFGameSaveFilesUiActiveDeviceContentionCallback* activeDeviceContentionCallback;

    /// <summary>
    /// Context to pass to the activeDeviceContentionCallback.
    /// </summary>
    void* activeDeviceContentionContext;

    /// <summary>
    /// Optional callback for conflict between local and remote save data.
    /// </summary>
    PFGameSaveFilesUiConflictCallback* conflictCallback;
    
    /// <summary>
    /// Context to pass to the conflictCallback.
    /// </summary>
    void* conflictContext;

    /// <summary>
    /// Optional callback for out of local storage.
    /// </summary>
    PFGameSaveFilesUiOutOfStorageCallback* outOfStorageCallback;

    /// <summary>
    /// Context to pass to the outOfStorageCallback.
    /// </summary>
    void* outOfStorageContext;
} PFGameSaveUICallbacks;

/// <summary>
/// Sets the UI callbacks.  These UI callbacks will trigger during PFGameSaveFilesAddUserWithUiAsync() or 
/// PFGameSaveFilesUploadWithUiAsync().
/// 
/// PFGameSaveFilesAddUserWithUiAsync() may trigger any UI callback while 
/// PFGameSaveFilesUploadWithUiAsync() will only trigger PFGameSaveFilesUiProgressCallback and/or 
/// PFGameSaveFilesUiSyncFailedCallback.
/// 
/// On non-Windows platforms, setting these callbacks is required to trigger game rendered UI dialogs.
/// 
/// On Xbox and Windows platforms, file sync is done in out of process and stock UI is provided by the platform
/// however the title can set these callbacks to render custom UI dialogs as desired.
/// </summary>
/// <param name="callbacks">The callbacks to set.</param>
PF_API PFGameSaveFilesSetUiCallbacks(_In_ PFGameSaveUICallbacks* callbacks) noexcept;

/// <summary>
/// For use inside PFGameSaveFilesUiProgressCallback.
/// Get the progress of the current operation.
/// </summary>
/// <param name="localUserHandle">Local user handle.</param>
/// <param name="syncState">Sync state of the operation.</param>
/// <param name="current">Current progress.</param>
/// <param name="total">Total progress.</param>
PF_API PFGameSaveFilesUiProgressGetProgress(
    _In_ PFLocalUserHandle localUserHandle, 
    _Out_opt_ PFGameSaveFilesSyncState* syncState, 
    _Out_opt_ uint64_t* current, 
    _Out_opt_ uint64_t* total);

/// <summary>The user action of the PFGameSaveFilesUiProgressCallback.</summary>
enum class PFGameSaveFilesUiProgressUserAction : uint32_t
{
    /// <summary>
    /// Cancel the current operation.
    /// </summary>
    Cancel = 0
};

/// <summary>
/// Sets the user response for the PFGameSaveFilesUiProgressCallback.
/// This can be called inside or outside of the callback.  The state machine won't progress until the 
/// the user chooses a response, or the async API is canceled.
/// </summary>
/// <param name="localUserHandle">Local user handle.</param>
/// <param name="action">The user action.</param>
PF_API PFGameSaveFilesSetUiProgressResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiProgressUserAction action);

/// <summary>The user action of the PFGameSaveFilesUiSyncFailedCallback.</summary>
enum class PFGameSaveFilesUiSyncFailedUserAction : uint32_t
{
    /// <summary>
    /// Cancel the current operation.
    /// </summary>
    Cancel = 0,

    /// <summary>
    /// Retry the current operation.
    /// </summary>
    Retry,

    /// <summary>
    /// Treat the user as disconnected from cloud.
    /// This action may only be set during PFGameSaveFilesAddUserWithUiAsync()
    /// 
    /// If PFGameSaveFilesAddUserWithUiAsync() was called without network access and 
    /// the user chooses PFGameSaveFilesUiSyncFailedUserAction::UseOffline in the PFGameSaveFilesUiSyncFailedCallback
    /// then the user is considered disconnected from cloud.
    /// 
    /// When the user is disconnected from cloud, PFGameSaveFilesAddUserWithUiAsync() can be called again if you want to try to 
    /// make the user connected to cloud. 
    /// It will show the failure UI again if the network is still offline.  No need to re-init gamesave but you can if desired.
    /// 
    /// While the user is disconnected from cloud, PFGameSaveFilesUploadWithUiAsync() will not do anything but return 
    /// E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD in the async completion even if there's network access.
    /// </summary>
    UseOffline
};

/// <summary>
/// Sets the user response for the PFGameSaveFilesUiSyncFailedCallback.
/// This can be called inside or outside of the callback.  The state machine won't progress until the 
/// the user chooses a response, or the async API is canceled.
/// </summary>
/// <param name="localUserHandle">Local user handle.</param>
/// <param name="action">The user action.</param>
PF_API PFGameSaveFilesSetUiSyncFailedResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiSyncFailedUserAction action);

/// <summary>The user action of the PFGameSaveFilesUiActiveDeviceContentionCallback.</summary>
enum class PFGameSaveFilesUiActiveDeviceContentionUserAction : uint32_t
{
    /// <summary>
    /// Cancel the current operation.
    /// </summary>
    Cancel = 0,

    /// <summary>
    /// Retry the current operation.
    /// </summary>
    Retry,

    /// <summary>
    /// Sync the last saved data.  This makes the local device active.
    /// 
    /// After this, the remote device will not be able to upload since it is no longer the active device.
    /// This prevents unsynchronized progression from multiple devices 
    /// </summary>
    SyncLastSavedData,
};

/// <summary>
/// Sets the user response for the PFGameSaveFilesUiActiveDeviceContentionCallback.
/// This can be called inside or outside of the callback.  The state machine won't progress until the 
/// the user chooses a response, or the async API is canceled.
/// </summary>
/// <param name="localUserHandle">Local user handle.</param>
/// <param name="action">The user action.</param>
PF_API PFGameSaveFilesSetUiActiveDeviceContentionResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiActiveDeviceContentionUserAction action);

/// <summary>The user action of the PFGameSaveFilesUiConflictCallback.</summary>
enum class PFGameSaveFilesUiConflictUserAction : uint32_t
{
    /// <summary>
    /// Cancel the current operation.
    /// </summary>
    Cancel = 0,

    /// <summary>
    /// Take the local version.
    /// </summary>
    TakeLocal,

    /// <summary>
    /// Take the remote version.
    /// </summary>
    TakeRemote,
};

/// <summary>
/// Sets the user response for the PFGameSaveFilesUiConflictCallback.
/// This can be called inside or outside of the callback.  The state machine won't progress until the 
/// the user chooses a response, or the async API is canceled.
/// </summary>
/// <param name="localUserHandle">Local user handle.</param>
/// <param name="action">The user action.</param>
PF_API PFGameSaveFilesSetUiConflictResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiConflictUserAction action);

/// <summary>The user action of the PFGameSaveFilesUiOutOfStorageCallback.</summary>
enum class PFGameSaveFilesUiOutOfStorageUserAction : uint32_t
{
    /// <summary>
    /// Cancel the current operation.
    /// </summary>
    Cancel = 0,

    /// <summary>
    /// Local storage space was cleared, so retry
    /// </summary>
    Retry
};

/// <summary>
/// Sets the user response for the PFGameSaveFilesUiOutOfStorageCallback.
/// This can be called inside or outside of the callback.  The state machine won't progress until the 
/// the user chooses a response, or the async API is canceled.
/// </summary>
/// <param name="localUserHandle">Local user handle.</param>
/// <param name="action">The user action.</param>
PF_API PFGameSaveFilesSetUiOutOfStorageResponse(_In_ PFLocalUserHandle localUserHandle, _In_ PFGameSaveFilesUiOutOfStorageUserAction action);

}
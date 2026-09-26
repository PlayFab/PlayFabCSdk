// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/PFCore.h>
#if HC_PLATFORM != HC_PLATFORM_GDK
#include <httpClient/httpClient.h>
#endif

extern "C"
{

/// <summary>
/// User cancelled.  This is returned by the async operation if after a UI callback the response is set to cancel.
/// </summary>
#define E_PF_GAMESAVE_USER_CANCELLED           0x800704c7 // same as __HRESULT_FROM_WIN32(ERROR_CANCELLED)

/// <summary>
/// Not initialized.  This is returned if PFGameSaveFilesInitialize() was not called.
/// </summary>
#define E_PF_GAMESAVE_NOT_INITIALIZED          MAKE_E_HC(0x7000L) // 0x89237000

/// <summary>
/// Already initialized.  This is returned if PFGameSaveFilesInitialize() was already called without a matching PFGameSaveFilesUninitializeAsync().
/// </summary>
#define E_PF_GAMESAVE_ALREADY_INITIALIZED      MAKE_E_HC(0x7001L) // 0x89237001

/// <summary>
/// User already added.  This is returned if PFGameSaveFilesAddUserWithUiAsync() is called with the same user.  
/// To re-add a user, you must re-init by calling PFGameSaveFilesUninitializeAsync() and when that 
/// completes call PFGameSaveFilesInitialize().
/// </summary>
#define E_PF_GAMESAVE_USER_ALREADY_ADDED       MAKE_E_HC(0x7002L) // 0x89237002

/// <summary>
/// User not found.  
/// This is returned if PFGameSaveFilesAddUserWithUiAsync() was not called or has not completed yet.
/// </summary>
#define E_PF_GAMESAVE_USER_NOT_ADDED           MAKE_E_HC(0x7003L) // 0x89237003

/// <summary>
/// Disconnected from cloud.
/// 
/// This is returned by PFGameSaveFilesUploadWithUiAsync() and PFGameSaveFilesGetRemainingQuota()
/// if PFGameSaveFilesAddUserWithUiAsync() was called without network access and 
/// the user chooses PFGameSaveFilesUiSyncFailedUserAction::UseOffline in the PFGameSaveFilesUiSyncFailedCallback.
/// The user can also be disconnected to cloud at anytime if another device is becomes the active device.
/// 
/// When the disconnected from cloud, PFGameSaveFilesAddUserWithUiAsync() can be called again if you want to try connect to the cloud.
/// It will show the failure UI again if the network is still offline.  No need to re-init gamesave but you can if desired.
/// 
/// While the disconnected from cloud, PFGameSaveFilesUploadWithUiAsync() will not do anything but return E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD 
/// in the async completion if even if there's network access.
/// </summary>
#define E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD  MAKE_E_HC(0x7004L) // 0x89237004

/// <summary>
/// Network failure.
/// 
/// This is returned by PFGameSaveFilesUploadWithUiAsync() if PFGameSaveFilesAddUserWithUiAsync() was 
/// called without network access when the user is still connected to cloud.
/// 
/// The user is considered connected to cloud when user was added successfully by PFGameSaveFilesUploadWithUiAsync().
/// The the game can show the network status to user as desired.
/// The game can call PFGameSaveFilesUploadWithUiAsync() again as desired without issue and it will succeed if network access was restored.
/// </summary>
#define E_PF_GAMESAVE_NETWORK_FAILURE          MAKE_E_HC(0x7005L)   // 0x89237005

/// <summary>
/// Download in progress.  This can be returned by PFGameSaveFilesUploadWithUiAsync() before PFGameSaveFilesAddUserWithUiAsync() has completed.
/// </summary>
#define E_PF_GAMESAVE_DOWNLOAD_IN_PROGRESS     MAKE_E_HC(0x7006L)   // 0x89237006

/// <summary>
/// Device no longer active
/// 
/// This is returned when calling PFGameSaveFilesUploadWithUiAsync() and the local device is no longer active. 
/// This can happen if the user chooses to make another device active by responding to the 
/// PFGameSaveFilesUiActiveDeviceContentionCallback with PFGameSaveFilesUiActiveDeviceContentionUserAction::SyncLastSavedData
/// </summary>
#define E_PF_GAMESAVE_DEVICE_NO_LONGER_ACTIVE   MAKE_E_HC(0x7007L)   // 0x89237007

/// <summary>
/// There is not enough space on the disk.
/// 
/// This is returned when the game is trying to download a file and there is not enough space on the disk.
/// The game should prompt the user to free up disk space, then retry the operation.
/// </summary>
#define E_PF_GAMESAVE_DISK_FULL                MAKE_E_HC(0x7008L)   // 0x89237008

/// <summary>
/// A conflicting operation is already in progress for this user.
///
/// Covers any operation rejected because another mutually exclusive one holds the user's sync
/// state. It is returned by:
/// - PFGameSaveFilesUploadWithUiAsync() when an upload for the same user has not completed yet, or
///   when a cloud reset is in progress.
/// - PFGameSaveFilesAddUserWithUiAsync() when a cloud reset is in progress.
/// - PFGameSaveFilesResetCloudAsync() when another cloud reset is already in progress.
///
/// In every case the operation was not started and nothing was modified. Wait for the in-flight
/// operation to complete, then retry.
/// </summary>
#define E_PF_GAMESAVE_OPERATION_IN_PROGRESS    MAKE_E_HC(0x7009L)   // 0x89237009

/// <summary>
/// A local save file could not be read while the operation was running.
///
/// This is returned by PFGameSaveFilesUploadWithUiAsync() when a file that was present when the
/// save folder was enumerated is no longer readable by the time it is compressed or transferred --
/// it was deleted, replaced, or is momentarily held open by another writer (the title's own save
/// code, anti-virus, or a platform cloud-sync agent). It is most likely when something stretches
/// the gap between enumeration and transfer, such as the title writing new save data or a
/// thumbnail while an upload is in flight, or a suspend/resume that leaves an upload parked.
///
/// Nothing was committed to the cloud. The condition is transient: call
/// PFGameSaveFilesUploadWithUiAsync() again once the title is no longer writing that file, and the
/// upload re-enumerates the save folder and proceeds normally.
/// </summary>
#define E_PF_GAMESAVE_LOCAL_FILE_UNAVAILABLE   MAKE_E_HC(0x700AL)   // 0x8923700A

/// <summary>
/// The maximum length, in characters, of the short save description passed to
/// PFGameSaveFilesSetSaveDescriptionAsync(). The description is Base64 encoded before it is sent to
/// the service, which caps the encoded value at 1024 characters, so the raw value is limited to the
/// largest length that still encodes within that cap.
/// </summary>
#define PF_GAMESAVE_MAX_SHORT_SAVE_DESCRIPTION_LENGTH 768

/// <summary>
/// Options to use when initializing the game save system.
/// </summary>
enum class PFGameSaveInitOptions : uint64_t
{
    /// <summary>
    /// Other options might be added in future release
    /// </summary>
    None = 0x00, 
};
DEFINE_ENUM_FLAG_OPERATORS(PFGameSaveInitOptions);

/// <summary>
/// Init args used by PFGameSave
/// </summary>
typedef struct PFGameSaveInitArgs {
    /// <summary>
    /// An XTaskQueue that should be used for background work. If no queue is provided 
    /// then a default (threadpool) queue will be used.
    /// </summary>
    XTaskQueueHandle backgroundQueue;

    /// <summary>
    /// Options from PFGameSaveInitOptions to use when initializing the game save system.
    /// Set to PFGameSaveInitOptions::None for default behavior.
    /// </summary>
    uint64_t options;

    /// <summary>
    /// On some platforms, the game can store save files in various locations (e.g. My Docs, local app data, etc)
    /// On those platforms, this lets the game specify where save files are stored.
    /// This should be the root folder of where the game save files are stored.
    /// All its files and subfolders will be synchronized with the cloud.
    /// This param is ignored on platforms such as Xbox where the game save files are stored in a fixed location.
    /// On other platforms such as Steam Deck, it is required to be set to a valid path.
    /// </summary>
    const char* saveFolder;

} PFGameSaveInitArgs;

/// <summary>
/// Options to use when adding a user to the game save system.
/// </summary>
enum class PFGameSaveFilesAddUserOptions : uint32_t
{
    /// <summary>
    /// Default behavior
    /// </summary>
    None = 0x00,
    
    /// <summary>
    /// Sync using the most recently verified prior cloud save state (a previously loaded save that was later replaced by a newer upload).
    /// Use when you suspect the latest upload is bad (e.g. load failure, failed integrity / version check, crash during or immediately after save,
    /// player reports corrupted / regressed state) and you want to automatically recover to the last known good manifest. If no earlier verified
    /// state exists this behaves like None and the current latest is kept.
    /// </summary>
    RollbackToLastKnownGood = 0x01,

    /// <summary>
    /// Sync using the save state that was kept aside (the "losing" choice) from the most recent conflict resolution; falls back to latest if none.
    /// Use when you determine (or the player reports) that they chose the wrong side during the last conflict resolution dialog and you want to
    /// restore the alternate snapshot that was preserved for exactly that recovery opportunity. If no preserved conflict snapshot remains this
    /// quietly falls back to the current latest.
    /// </summary>
    RollbackToLastConflict = 0x02,
};
DEFINE_ENUM_FLAG_OPERATORS(PFGameSaveFilesAddUserOptions);

/// <summary>
/// Sync state of the game save system
/// </summary>
enum class PFGameSaveFilesSyncState : uint32_t
{
    /// <summary>
    /// Not started
    /// </summary>
    NotStarted = 0,

    /// <summary>
    /// Preparing for download
    /// </summary>
    PreparingForDownload,

    /// <summary>
    /// Downloading
    /// </summary>
    Downloading,

    /// <summary>
    /// The system is reading and compressing local save files in preparation
    /// for upload. Do not modify files in the save folder during this state.
    /// </summary>
    PreparingForUpload,

    /// <summary>
    /// Local save files have been captured and the upload to the cloud is in
    /// progress. It is safe to write to the save folder again during this
    /// state; the system no longer reads from the original files.
    /// </summary>
    Uploading,

    /// <summary>
    /// Sync complete
    /// </summary>
    SyncComplete,
};

/// <summary>
/// Options to use when uploading game save files
/// </summary>
enum class PFGameSaveFilesUploadOption : uint32_t
{
    /// <summary>
    /// Default behavior
    /// The device will be kept active after the upload
    /// </summary>
    KeepDeviceActive = 0,

    /// <summary>
    /// Release the device as active.
    /// After this, this device can no longer be used to upload game save files.
    /// To upload again on the same session, call PFGameSaveUninitializeAsync and wait for it to complete.
    /// </summary>
    ReleaseDeviceAsActive
};

typedef struct PFGameSaveDescriptor
{
    /// <summary>
    /// Relevant time of the descriptor (can differ depending upon state and usage)
    /// </summary>
    time_t time;

    /// <summary>
    /// total bytes of the save
    /// </summary>
    uint64_t totalBytes;

    /// <summary>
    /// size of the pending upload, if any.
    /// </summary>
    uint64_t uploadedBytes;

    /// <summary>
    /// Device Type
    /// </summary>
    char deviceType[256];

    /// <summary>
    /// A unique identifier for the device
    /// </summary>
    char deviceId[256];

    /// <summary>
    /// User friendly name for the device, e.g. "My Xbox One"
    /// </summary>
    char deviceFriendlyName[256];

    /// <summary>
    /// The URI to pfthumbnail.png if that file exists at the game saves root folder. Empty string if that file doesn't exist at the root.
    /// </summary>
    char thumbnailUri[2048];

    /// <summary>
    /// Short description of the save, can be shown in the Conflict or Device Contention UX along with the thumbnail.
    /// </summary>
    char shortSaveDescription[4096];
} PFGameSaveDescriptor;


}
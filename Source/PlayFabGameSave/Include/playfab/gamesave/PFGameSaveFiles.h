// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/gamesave/PFGameSaveFilesTypes.h>
#include <playfab/gamesave/PFGameSaveFilesUi.h>

extern "C"
{

/// <summary>
/// Initializes the PlayFab Game Save library instance.
/// </summary>
/// <remarks>
/// This will internally call PFInitialize(nullptr) if it hasn't been called already by the
/// title. If control of PFCore background work is needed, the title should explicitly call
/// PFInitialize and PFUninitialize.
/// </remarks>
/// <returns>Result code for this API operation.</returns>
PF_API PFGameSaveFilesInitialize(_In_ PFGameSaveInitArgs* args) noexcept;

/// <summary>
/// Adds a user to the game save system.
/// This downloads save state from the cloud as needed, and might trigger these UI callbacks:
///     PFGameSaveFilesUiProgressCallback, 
///     PFGameSaveFilesUiSyncFailedCallback, 
///     PFGameSaveFilesUiActiveDeviceContentionCallback,
///     PFGameSaveFilesUiConflictCallback,
///     PFGameSaveFilesUiOutOfStorageCallback
/// 
/// See PFGameSaveFilesSetUiCallbacks for more detail.
/// PFGameSaveFilesInitialize must be called prior.
/// 
/// On Xbox and Windows platforms, stock UI is provided by the system.
/// </summary>
/// <param name="localUserHandle">Local user to add.</param>
/// <param name="options">Options to use when adding the user.</param>
/// <param name="async">An XAsyncBlock for polling the call's status and retrieving the call results.</param>
/// <returns>
/// To retrieve the result of a call to PFGameSaveFilesAddUserWithUiAsync, call 
/// PFGameSaveFilesAddUserWithUiResult. After this call completes, you can call 
/// PFGameSaveFilesGetFolder to get the root folder of the game save files 
/// and folders for this user.  All the root folder's files and subfolders will 
/// be synchronized with the cloud.
/// 
/// This can only be called once per user, typically before showing the game's main menu.
/// To re-trigger the download the same user, call PFGameSaveFilesUninitializeAsync and wait 
/// for it to complete first
/// </returns>
PF_API PFGameSaveFilesAddUserWithUiAsync(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFGameSaveFilesAddUserOptions options,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Retrieves the result of a call to PFGameSaveFilesAddUserWithUiAsync.
/// </summary>
/// <param name="async">The XAsyncBlock that we pass to PFGameSaveFilesAddUserWithUiAsync.</param>
/// <returns>Result code for this API operation.</returns>
PF_API PFGameSaveFilesAddUserWithUiResult(
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Returns the size in bytes of the root folder of the game save files and folders for this user.
/// The user must be added first using PFGameSaveFilesAddUserWithUiAsync.
/// </summary>
/// <param name="localUserHandle">Local user whose game save folder to return.</param>
/// <param name="saveRootFolderSize">The size in bytes of the root folder of the game save files 
/// and folders.</param>
/// <returns>Result code for this API operation.</returns>
PF_API PFGameSaveFilesGetFolderSize(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ size_t* saveRootFolderSize
) noexcept;

/// <summary>
/// Returns the root folder of the game save files and folders for this user.
/// All its files and subfolders will be synchronized with the cloud.
/// The user must be added first using PFGameSaveFilesAddUserWithUiAsync.
/// </summary>
/// <param name="localUserHandle">Local user whose game save folder to return.</param>
/// <param name="saveRootFolderSize">The size in bytes of the root folder of the game save 
/// files and folders.</param>
/// <param name="saveRootFolderBuffer">The buffer to receive the root folder of the game save 
/// files and folders.</param>
/// <param name="saveRootFolderUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>Result code for this API operation.</returns>
PF_API PFGameSaveFilesGetFolder(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ size_t saveRootFolderSize,
    _Out_writes_(saveRootFolderSize) char* saveRootFolderBuffer,
    _Out_opt_ size_t* saveRootFolderUsed
) noexcept;

/// <summary>
/// Uploads all the files and folders in the folder returned by PFGameSaveFilesGetFolder().
/// The user must be added first.
/// 
/// On non-Windows platforms, the upload happens in the same process as the game and 
/// might trigger these UI callbacks:
///    PFGameSaveFilesUiProgressCallback,
///    PFGameSaveFilesUiSyncFailedCallback
/// 
/// On Xbox and Windows platforms, the upload is done out of process and stock UI is 
/// provided by the system.  Calling this API is optional on Xbox and Windows platforms as the 
/// upload will automatically happen when the game isn't running.
/// </summary>
/// <param name="localUserHandle">Local user whose game save files and folders to upload.</param>
/// <param name="option">Option to use when uploading the files and folders.</param>
/// <param name="async">An XAsyncBlock for polling the call's status and retrieving the call results.</param>
/// <returns>
/// To retrieve the result of a call to PFGameSaveFilesUploadWithUiAsync, call PFGameSaveFilesUploadWithUiResult.
/// </returns>
PF_API PFGameSaveFilesUploadWithUiAsync(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFGameSaveFilesUploadOption option,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Retrieves the result of a call to PFGameSaveFilesUploadWithUiAsync.
/// </summary>
/// <param name="async">The XAsyncBlock that we pass to PFGameSaveFilesUploadWithUiAsync.</param>
/// <returns>Result code for this API operation.</returns>
PF_API PFGameSaveFilesUploadWithUiResult(
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Returns the amount of data available to store save data in bytes using the PFGameSaveFiles API.
/// Going over quota will return a negative number and cause the service to block the upload.
/// </summary>
/// <param name="localUserHandle">Local user whose quota to return.</param>
/// <param name="remainingQuota">The amount of data available to store save data in bytes.
/// Going over quota will return a negative number and cause the service to block the upload
/// </param>
/// <returns>Result code for this API operation. 
/// If called when disconnected from cloud, it will return E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD.
/// </returns>
PF_API PFGameSaveFilesGetRemainingQuota(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ int64_t* remainingQuota
) noexcept;

/// <summary>
/// Returns if the user is connected to cloud.
/// 
/// The user can be disconnected from cloud when calling PFGameSaveFilesAddUserWithUiAsync() without network access and 
/// the user chooses PFGameSaveFilesUiSyncFailedUserAction::UseOffline in the PFGameSaveFilesUiSyncFailedCallback.
/// The user can also be disconnected from cloud at anytime if another device becomes the active device.
/// 
/// When disconnected from cloud, PFGameSaveFilesAddUserWithUiAsync() can be called again if you want to try connect to the cloud.
/// It will show the failure UI again if the network is still offline.  No need to re-init gamesave but you can if desired.
/// 
/// While disconnected from cloud, PFGameSaveFilesUploadWithUiAsync() will not do anything but return E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD 
/// in the async completion even if there's network access.
/// </summary>
/// <param name="localUserHandle">Local user whose quota to return.</param>
/// <param name="isConnectedToCloud">Set to true if the user is connected to cloud</param>
/// <returns>Result code for this API operation.</returns>
PF_API PFGameSaveFilesIsConnectedToCloud(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ bool* isConnectedToCloud
) noexcept;

/// <summary>
/// Callback for when the active device changes.
/// When this callback is triggered, it means the user moved to another device so 
/// this title should return to main menu
/// </summary>
/// <param name="localUserHandle">Local user handle.</param>
/// <param name="activeDevice">Active device metadata.</param>
/// <param name="context">Context passed to the callback.</param>
typedef void CALLBACK PFGameSaveFilesActiveDeviceChangedCallback(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ PFGameSaveDescriptor* activeDevice,
    _In_opt_ void* context);

/// <summary>
/// Sets the active device changed callback.
/// When this callback is triggered, it means the user moved to another device so 
/// this title should return to main menu
/// </summary>
/// <param name="callbackQueue">The async queue the callback should be invoked on.</param>
/// <param name="callback">The callback to be invoked when the active device changes.</param>
/// <param name="context">The context to be passed to the callback.</param>
/// <returns>Result code for this API operation.</returns>
PF_API PFGameSaveFilesSetActiveDeviceChangedCallback(
    _In_opt_ XTaskQueueHandle callbackQueue,
    _In_ PFGameSaveFilesActiveDeviceChangedCallback* callback,
    _In_opt_ void* context
) noexcept;

/// <summary>
/// Set a short save description of the pending game save.
/// This can be seen in the conflict or active device contention UI
/// </summary>
/// <param name="localUserHandle">Local user whose game save files and folders to upload.</param>
/// <param name="shortSaveDescription">Short save description of the pending game save.</param>
/// <param name="async">An XAsyncBlock for polling the call's status and retrieving the call results.</param>
PF_API PFGameSaveFilesSetSaveDescriptionAsync(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ const char* shortSaveDescription,
    _In_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Retrieves the result of a call to PFGameSaveFilesSetSaveDescriptionAsync.
/// </summary>
/// <param name="async">The XAsyncBlock that we pass to PFGameSaveFilesSetSaveDescriptionAsync.</param>
/// <returns>Result code for this API operation.</returns>
PF_API PFGameSaveFilesSetSaveDescriptionResult(_In_ XAsyncBlock* async) noexcept;

/// <summary>
/// This resets the cloud game save state for this user.
/// Normally this does not need to be called but might be useful during development or testing.
/// This does not delete or alter the local game save.
/// </summary>
/// <param name="localUserHandle">Local user to operate on.</param>
/// <param name="async">An XAsyncBlock for polling the call's status and retrieving the call results.</param>
/// <returns>Result code for this API operation.</returns>
PF_API PFGameSaveFilesResetCloudAsync(
    _In_ PFLocalUserHandle localUserHandle,
    _In_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Retrieves the result of a call to PFGameSaveFilesResetCloudAsync.
/// </summary>
/// <param name="async">The XAsyncBlock that we pass to PFGameSaveFilesResetCloudAsync.</param>
/// <returns>Result code for this API operation.</returns>
PF_API PFGameSaveFilesResetCloudResult(_In_ XAsyncBlock* async) noexcept;

/// <summary>
/// Cleanup PlayFab Game Save library instance.
/// </summary>
/// <param name="async">An XAsyncBlock for polling the call's status and retrieving the call results.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// To retrieve the result of a call to PFGameSaveFilesUninitializeAsync, call PFGameSaveFilesUninitializeResult.
/// This will internally call PFUninitializeAsync() if PFGameSaveInitialize() needed 
/// to call PFInitialize() internally.
/// </remarks>
PF_API PFGameSaveFilesUninitializeAsync(
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Retrieves the result of a call to PFGameSaveFilesUninitializeAsync.
/// </summary>
/// <param name="async">The XAsyncBlock that we pass to PFGameSaveFilesUninitializeAsync.</param>
/// <returns>Result code for this API operation.</returns>
PF_API PFGameSaveFilesUninitializeResult(
    _Inout_ XAsyncBlock* async
) noexcept;

}

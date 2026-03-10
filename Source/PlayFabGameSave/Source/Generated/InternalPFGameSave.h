// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/PFEntity.h>

namespace PlayFab
{


/// <summary>
/// Deletes a GameSave Manifest version. All files used by this version of the Manifest and not used
/// by any other versions will be cleaned up.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_GAME_SAVE_BAD_REQUEST, E_PF_GAME_SAVE_MANIFEST_NOT_FOUND, E_PF_GAME_SAVE_SERVICE_NOT_ENABLED_FOR_TITLE,
/// E_PF_GAME_SAVE_SERVICE_ONBOARDING_PENDING, E_PF_INTERNAL_SERVER_ERROR, E_PF_SERVICE_UNAVAILABLE, E_PF_SERVICE_UNAVAILABLE
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </remarks>
HRESULT PFGameSaveDeleteManifestAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFGameSaveDeleteManifestRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Finalizes a GameSave Manifest version with all the associated files. The GameSave is considered immutable
/// after this operation.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// When the asynchronous task is complete, call <see cref="PFGameSaveFinalizeManifestGetResultSize"/>
/// and <see cref="PFGameSaveFinalizeManifestGetResult"/> to get the result.
/// </remarks>
HRESULT PFGameSaveFinalizeManifestAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFGameSaveFinalizeManifestRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a FinalizeManifest call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_GAME_SAVE_BAD_REQUEST,
/// E_PF_GAME_SAVE_CONFLICT_UPDATING_MANIFEST, E_PF_GAME_SAVE_DATA_STORAGE_QUOTA_EXCEEDED, E_PF_GAME_SAVE_FILE_NOT_UPLOADED,
/// E_PF_GAME_SAVE_MANIFEST_FILES_LIMIT_EXCEEDED, E_PF_GAME_SAVE_MANIFEST_NOT_ELIGIBLE_AS_CONFLICTING_VERSION,
/// E_PF_GAME_SAVE_MANIFEST_NOT_FOUND, E_PF_GAME_SAVE_MANIFEST_UPDATES_NOT_ALLOWED, E_PF_GAME_SAVE_NEWER_MANIFEST_EXISTS,
/// E_PF_GAME_SAVE_SERVICE_NOT_ENABLED_FOR_TITLE, E_PF_GAME_SAVE_SERVICE_ONBOARDING_PENDING, E_PF_GAME_SAVE_UNKNOWN_FILE_IN_MANIFEST,
/// E_PF_INTERNAL_SERVER_ERROR, E_PF_SERVICE_UNAVAILABLE or any of the global PlayFab Service errors.
/// See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
HRESULT PFGameSaveFinalizeManifestGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFGameSaveFinalizeManifestAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_GAME_SAVE_BAD_REQUEST,
/// E_PF_GAME_SAVE_CONFLICT_UPDATING_MANIFEST, E_PF_GAME_SAVE_DATA_STORAGE_QUOTA_EXCEEDED, E_PF_GAME_SAVE_FILE_NOT_UPLOADED,
/// E_PF_GAME_SAVE_MANIFEST_FILES_LIMIT_EXCEEDED, E_PF_GAME_SAVE_MANIFEST_NOT_ELIGIBLE_AS_CONFLICTING_VERSION,
/// E_PF_GAME_SAVE_MANIFEST_NOT_FOUND, E_PF_GAME_SAVE_MANIFEST_UPDATES_NOT_ALLOWED, E_PF_GAME_SAVE_NEWER_MANIFEST_EXISTS,
/// E_PF_GAME_SAVE_SERVICE_NOT_ENABLED_FOR_TITLE, E_PF_GAME_SAVE_SERVICE_ONBOARDING_PENDING, E_PF_GAME_SAVE_UNKNOWN_FILE_IN_MANIFEST,
/// E_PF_INTERNAL_SERVER_ERROR, E_PF_SERVICE_UNAVAILABLE or any of the global PlayFab Service errors.
/// See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
HRESULT PFGameSaveFinalizeManifestGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFGameSaveFinalizeManifestResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

/// <summary>
/// Gets a Title's configuration for the Game Save service.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// When the asynchronous task is complete, call <see cref="PFGameSaveGetConfigForTitleGetResultSize"/>
/// and <see cref="PFGameSaveGetConfigForTitleGetResult"/> to get the result.
/// </remarks>
HRESULT PFGameSaveGetConfigForTitleAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFGameSaveGetConfigForTitleRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a GetConfigForTitle call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_GAME_SAVE_BAD_REQUEST,
/// E_PF_GAME_SAVE_SERVICE_NOT_ENABLED_FOR_TITLE, E_PF_GAME_SAVE_SERVICE_ONBOARDING_PENDING, E_PF_INTERNAL_SERVER_ERROR,
/// E_PF_SERVICE_UNAVAILABLE, E_PF_SERVICE_UNAVAILABLE or any of the global PlayFab Service errors. See
/// doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
HRESULT PFGameSaveGetConfigForTitleGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFGameSaveGetConfigForTitleAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_GAME_SAVE_BAD_REQUEST,
/// E_PF_GAME_SAVE_SERVICE_NOT_ENABLED_FOR_TITLE, E_PF_GAME_SAVE_SERVICE_ONBOARDING_PENDING, E_PF_INTERNAL_SERVER_ERROR,
/// E_PF_SERVICE_UNAVAILABLE, E_PF_SERVICE_UNAVAILABLE or any of the global PlayFab Service errors. See
/// doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
HRESULT PFGameSaveGetConfigForTitleGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFGameSaveGetConfigForTitleResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

/// <summary>
/// Gets the download details for a specific Manifest version.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// When the asynchronous task is complete, call <see cref="PFGameSaveGetManifestDownloadDetailsGetResultSize"/>
/// and <see cref="PFGameSaveGetManifestDownloadDetailsGetResult"/> to get the result.
/// </remarks>
HRESULT PFGameSaveGetManifestDownloadDetailsAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFGameSaveGetManifestDownloadDetailsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a GetManifestDownloadDetails call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_GAME_SAVE_BAD_REQUEST,
/// E_PF_GAME_SAVE_MANIFEST_NOT_FOUND, E_PF_GAME_SAVE_MANIFEST_VERSION_QUARANTINED, E_PF_GAME_SAVE_SERVICE_NOT_ENABLED_FOR_TITLE,
/// E_PF_GAME_SAVE_SERVICE_ONBOARDING_PENDING, E_PF_INTERNAL_SERVER_ERROR, E_PF_SERVICE_UNAVAILABLE, E_PF_SERVICE_UNAVAILABLE
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
HRESULT PFGameSaveGetManifestDownloadDetailsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFGameSaveGetManifestDownloadDetailsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_GAME_SAVE_BAD_REQUEST,
/// E_PF_GAME_SAVE_MANIFEST_NOT_FOUND, E_PF_GAME_SAVE_MANIFEST_VERSION_QUARANTINED, E_PF_GAME_SAVE_SERVICE_NOT_ENABLED_FOR_TITLE,
/// E_PF_GAME_SAVE_SERVICE_ONBOARDING_PENDING, E_PF_INTERNAL_SERVER_ERROR, E_PF_SERVICE_UNAVAILABLE, E_PF_SERVICE_UNAVAILABLE
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
HRESULT PFGameSaveGetManifestDownloadDetailsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFGameSaveGetManifestDownloadDetailsResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

#if 0
/// <summary>
/// Gets the quota information for a Player's GameSave data.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// When the asynchronous task is complete, call <see cref="PFGameSaveGetQuotaForPlayerGetResultSize"/>
/// and <see cref="PFGameSaveGetQuotaForPlayerGetResult"/> to get the result.
/// </remarks>
HRESULT PFGameSaveGetQuotaForPlayerAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFGameSaveGetQuotaForPlayerRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a GetQuotaForPlayer call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_GAME_SAVE_BAD_REQUEST,
/// E_PF_GAME_SAVE_SERVICE_NOT_ENABLED_FOR_TITLE, E_PF_GAME_SAVE_SERVICE_ONBOARDING_PENDING, E_PF_INTERNAL_SERVER_ERROR,
/// E_PF_SERVICE_UNAVAILABLE, E_PF_SERVICE_UNAVAILABLE or any of the global PlayFab Service errors. See
/// doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
HRESULT PFGameSaveGetQuotaForPlayerGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFGameSaveGetQuotaForPlayerAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_GAME_SAVE_BAD_REQUEST,
/// E_PF_GAME_SAVE_SERVICE_NOT_ENABLED_FOR_TITLE, E_PF_GAME_SAVE_SERVICE_ONBOARDING_PENDING, E_PF_INTERNAL_SERVER_ERROR,
/// E_PF_SERVICE_UNAVAILABLE, E_PF_SERVICE_UNAVAILABLE or any of the global PlayFab Service errors. See
/// doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
HRESULT PFGameSaveGetQuotaForPlayerGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFGameSaveGetQuotaForPlayerResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

/// <summary>
/// Initializes a new GameSave Manifest version for uploading the associated files.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// When the asynchronous task is complete, call <see cref="PFGameSaveInitializeManifestGetResultSize"/>
/// and <see cref="PFGameSaveInitializeManifestGetResult"/> to get the result.
/// </remarks>
HRESULT PFGameSaveInitializeManifestAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFGameSaveInitializeManifestRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a InitializeManifest call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_GAME_SAVE_BAD_REQUEST,
/// E_PF_GAME_SAVE_BASE_VERSION_NOT_AVAILABLE, E_PF_GAME_SAVE_MANIFEST_UPDATES_NOT_ALLOWED, E_PF_GAME_SAVE_MANIFEST_VERSION_ALREADY_EXISTS,
/// E_PF_GAME_SAVE_NEWER_MANIFEST_EXISTS, E_PF_GAME_SAVE_SERVICE_NOT_ENABLED_FOR_TITLE, E_PF_GAME_SAVE_SERVICE_ONBOARDING_PENDING,
/// E_PF_INTERNAL_SERVER_ERROR, E_PF_SERVICE_UNAVAILABLE or any of the global PlayFab Service errors.
/// See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
HRESULT PFGameSaveInitializeManifestGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFGameSaveInitializeManifestAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_GAME_SAVE_BAD_REQUEST,
/// E_PF_GAME_SAVE_BASE_VERSION_NOT_AVAILABLE, E_PF_GAME_SAVE_MANIFEST_UPDATES_NOT_ALLOWED, E_PF_GAME_SAVE_MANIFEST_VERSION_ALREADY_EXISTS,
/// E_PF_GAME_SAVE_NEWER_MANIFEST_EXISTS, E_PF_GAME_SAVE_SERVICE_NOT_ENABLED_FOR_TITLE, E_PF_GAME_SAVE_SERVICE_ONBOARDING_PENDING,
/// E_PF_INTERNAL_SERVER_ERROR, E_PF_SERVICE_UNAVAILABLE or any of the global PlayFab Service errors.
/// See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
HRESULT PFGameSaveInitializeManifestGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFGameSaveInitializeManifestResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

/// <summary>
/// Initiates uploads of files for the Manifest version, requesting an upload URL for each file.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// When the asynchronous task is complete, call <see cref="PFGameSaveInitiateUploadGetResultSize"/>
/// and <see cref="PFGameSaveInitiateUploadGetResult"/> to get the result.
/// </remarks>
HRESULT PFGameSaveInitiateUploadAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFGameSaveInitiateUploadRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a InitiateUpload call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_GAME_SAVE_BAD_REQUEST,
/// E_PF_GAME_SAVE_CONFLICT_UPDATING_MANIFEST, E_PF_GAME_SAVE_FILE_ALREADY_EXISTS, E_PF_GAME_SAVE_MANIFEST_NOT_FOUND,
/// E_PF_GAME_SAVE_MANIFEST_UPDATES_NOT_ALLOWED, E_PF_GAME_SAVE_SERVICE_NOT_ENABLED_FOR_TITLE, E_PF_GAME_SAVE_SERVICE_ONBOARDING_PENDING,
/// E_PF_INTERNAL_SERVER_ERROR, E_PF_SERVICE_UNAVAILABLE, E_PF_SERVICE_UNAVAILABLE or any of the global
/// PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
HRESULT PFGameSaveInitiateUploadGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFGameSaveInitiateUploadAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_GAME_SAVE_BAD_REQUEST,
/// E_PF_GAME_SAVE_CONFLICT_UPDATING_MANIFEST, E_PF_GAME_SAVE_FILE_ALREADY_EXISTS, E_PF_GAME_SAVE_MANIFEST_NOT_FOUND,
/// E_PF_GAME_SAVE_MANIFEST_UPDATES_NOT_ALLOWED, E_PF_GAME_SAVE_SERVICE_NOT_ENABLED_FOR_TITLE, E_PF_GAME_SAVE_SERVICE_ONBOARDING_PENDING,
/// E_PF_INTERNAL_SERVER_ERROR, E_PF_SERVICE_UNAVAILABLE, E_PF_SERVICE_UNAVAILABLE or any of the global
/// PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
HRESULT PFGameSaveInitiateUploadGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFGameSaveInitiateUploadResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

/// <summary>
/// Lists all the manifest versions for the player.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// When the asynchronous task is complete, call <see cref="PFGameSaveListManifestsGetResultSize"/> and
/// <see cref="PFGameSaveListManifestsGetResult"/> to get the result.
/// </remarks>
HRESULT PFGameSaveListManifestsAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFGameSaveListManifestsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ListManifests call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_GAME_SAVE_BAD_REQUEST,
/// E_PF_GAME_SAVE_SERVICE_NOT_ENABLED_FOR_TITLE, E_PF_GAME_SAVE_SERVICE_ONBOARDING_PENDING, E_PF_INTERNAL_SERVER_ERROR,
/// E_PF_SERVICE_UNAVAILABLE, E_PF_SERVICE_UNAVAILABLE or any of the global PlayFab Service errors. See
/// doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
HRESULT PFGameSaveListManifestsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFGameSaveListManifestsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_GAME_SAVE_BAD_REQUEST,
/// E_PF_GAME_SAVE_SERVICE_NOT_ENABLED_FOR_TITLE, E_PF_GAME_SAVE_SERVICE_ONBOARDING_PENDING, E_PF_INTERNAL_SERVER_ERROR,
/// E_PF_SERVICE_UNAVAILABLE, E_PF_SERVICE_UNAVAILABLE or any of the global PlayFab Service errors. See
/// doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
HRESULT PFGameSaveListManifestsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFGameSaveListManifestsResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

#if 0
/// <summary>
/// Issues a request to onboard a Title to the Game Save service.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_GAME_SAVE_BAD_REQUEST, E_PF_GAME_SAVE_TITLE_ALREADY_ONBOARDED, E_PF_INTERNAL_SERVER_ERROR,
/// E_PF_SERVICE_UNAVAILABLE, E_PF_SERVICE_UNAVAILABLE or any of the global PlayFab Service errors. See
/// doc page "Handling PlayFab Errors" for more details on error handling.
/// </remarks>
HRESULT PFGameSaveRequestOnboardingAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFGameSaveRequestOnboardingRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

/// <summary>
/// Rolls back the game save state to the specified version.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// When the asynchronous task is complete, call <see cref="PFGameSaveRollbackToManifestGetResultSize"/>
/// and <see cref="PFGameSaveRollbackToManifestGetResult"/> to get the result.
/// </remarks>
HRESULT PFGameSaveRollbackToManifestAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFGameSaveRollbackToManifestRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a RollbackToManifest call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_GAME_SAVE_BAD_REQUEST,
/// E_PF_GAME_SAVE_BASE_VERSION_NOT_AVAILABLE, E_PF_GAME_SAVE_SERVICE_NOT_ENABLED_FOR_TITLE, E_PF_GAME_SAVE_SERVICE_ONBOARDING_PENDING,
/// E_PF_INTERNAL_SERVER_ERROR, E_PF_SERVICE_UNAVAILABLE, E_PF_SERVICE_UNAVAILABLE or any of the global
/// PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
HRESULT PFGameSaveRollbackToManifestGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFGameSaveRollbackToManifestAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_GAME_SAVE_BAD_REQUEST,
/// E_PF_GAME_SAVE_BASE_VERSION_NOT_AVAILABLE, E_PF_GAME_SAVE_SERVICE_NOT_ENABLED_FOR_TITLE, E_PF_GAME_SAVE_SERVICE_ONBOARDING_PENDING,
/// E_PF_INTERNAL_SERVER_ERROR, E_PF_SERVICE_UNAVAILABLE, E_PF_SERVICE_UNAVAILABLE or any of the global
/// PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
HRESULT PFGameSaveRollbackToManifestGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFGameSaveRollbackToManifestResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

#if 0
/// <summary>
/// Issues a request to update a title's configuration for the Game Save service.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// When the asynchronous task is complete, call <see cref="PFGameSaveUpdateConfigForTitleGetResultSize"/>
/// and <see cref="PFGameSaveUpdateConfigForTitleGetResult"/> to get the result.
/// </remarks>
HRESULT PFGameSaveUpdateConfigForTitleAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFGameSaveUpdateConfigForTitleRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a UpdateConfigForTitle call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_GAME_SAVE_BAD_REQUEST,
/// E_PF_GAME_SAVE_SERVICE_NOT_ENABLED_FOR_TITLE, E_PF_GAME_SAVE_SERVICE_ONBOARDING_PENDING, E_PF_INTERNAL_SERVER_ERROR,
/// E_PF_SERVICE_UNAVAILABLE, E_PF_SERVICE_UNAVAILABLE or any of the global PlayFab Service errors. See
/// doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
HRESULT PFGameSaveUpdateConfigForTitleGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFGameSaveUpdateConfigForTitleAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_GAME_SAVE_BAD_REQUEST,
/// E_PF_GAME_SAVE_SERVICE_NOT_ENABLED_FOR_TITLE, E_PF_GAME_SAVE_SERVICE_ONBOARDING_PENDING, E_PF_INTERNAL_SERVER_ERROR,
/// E_PF_SERVICE_UNAVAILABLE, E_PF_SERVICE_UNAVAILABLE or any of the global PlayFab Service errors. See
/// doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
HRESULT PFGameSaveUpdateConfigForTitleGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFGameSaveUpdateConfigForTitleResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

/// <summary>
/// Updates a GameSave Manifest version with the progress in upload of the associated files.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// When the asynchronous task is complete, call <see cref="PFGameSaveUpdateManifestGetResultSize"/>
/// and <see cref="PFGameSaveUpdateManifestGetResult"/> to get the result.
/// </remarks>
HRESULT PFGameSaveUpdateManifestAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFGameSaveUpdateManifestRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a UpdateManifest call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_GAME_SAVE_BAD_REQUEST,
/// E_PF_GAME_SAVE_CONFLICT_UPDATING_MANIFEST, E_PF_GAME_SAVE_MANIFEST_DESCRIPTION_UPDATE_NOT_ALLOWED,
/// E_PF_GAME_SAVE_MANIFEST_NOT_FOUND, E_PF_GAME_SAVE_MANIFEST_UPDATES_NOT_ALLOWED, E_PF_GAME_SAVE_MANIFEST_UPLOAD_PROGRESS_UPDATE_NOT_ALLOWED,
/// E_PF_GAME_SAVE_NOT_FINALIZED_MANIFEST_NOT_ELIGIBLE_AS_KNOWN_GOOD, E_PF_GAME_SAVE_NO_UPDATES_REQUESTED,
/// E_PF_GAME_SAVE_SERVICE_NOT_ENABLED_FOR_TITLE, E_PF_GAME_SAVE_SERVICE_ONBOARDING_PENDING, E_PF_INTERNAL_SERVER_ERROR,
/// E_PF_SERVICE_UNAVAILABLE, E_PF_SERVICE_UNAVAILABLE or any of the global PlayFab Service errors. See
/// doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
HRESULT PFGameSaveUpdateManifestGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFGameSaveUpdateManifestAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_GAME_SAVE_BAD_REQUEST,
/// E_PF_GAME_SAVE_CONFLICT_UPDATING_MANIFEST, E_PF_GAME_SAVE_MANIFEST_DESCRIPTION_UPDATE_NOT_ALLOWED,
/// E_PF_GAME_SAVE_MANIFEST_NOT_FOUND, E_PF_GAME_SAVE_MANIFEST_UPDATES_NOT_ALLOWED, E_PF_GAME_SAVE_MANIFEST_UPLOAD_PROGRESS_UPDATE_NOT_ALLOWED,
/// E_PF_GAME_SAVE_NOT_FINALIZED_MANIFEST_NOT_ELIGIBLE_AS_KNOWN_GOOD, E_PF_GAME_SAVE_NO_UPDATES_REQUESTED,
/// E_PF_GAME_SAVE_SERVICE_NOT_ENABLED_FOR_TITLE, E_PF_GAME_SAVE_SERVICE_ONBOARDING_PENDING, E_PF_INTERNAL_SERVER_ERROR,
/// E_PF_SERVICE_UNAVAILABLE, E_PF_SERVICE_UNAVAILABLE or any of the global PlayFab Service errors. See
/// doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
HRESULT PFGameSaveUpdateManifestGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFGameSaveUpdateManifestResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;


}
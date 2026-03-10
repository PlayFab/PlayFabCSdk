// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFAccountManagementTypes.h>
#include <playfab/core/PFEntity.h>

extern "C"
{

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Adds or updates a contact email to the player's profile.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// This API adds a contact email to the player's profile. If the player's profile already contains a
/// contact email, it will update the contact email to the email address specified.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be one of global PlayFab Service errors. See doc page "Handling PlayFab Errors"
/// for more details on error handling.
/// </remarks>
PF_API PFAccountManagementClientAddOrUpdateContactEmailAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementAddOrUpdateContactEmailRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

/// <summary>
/// Retrieves the user's PlayFab account details
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementClientGetAccountInfoGetResultSize"/>
/// and <see cref="PFAccountManagementClientGetAccountInfoGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementClientGetAccountInfoAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementGetAccountInfoRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetAccountInfo call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementClientGetAccountInfoGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementClientGetAccountInfoAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementClientGetAccountInfoGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetAccountInfoResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

/// <summary>
/// Retrieves all of the user's different kinds of info.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementClientGetPlayerCombinedInfoGetResultSize"/>
/// and <see cref="PFAccountManagementClientGetPlayerCombinedInfoGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementClientGetPlayerCombinedInfoAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementGetPlayerCombinedInfoRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetPlayerCombinedInfo call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
PF_API PFAccountManagementClientGetPlayerCombinedInfoGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementClientGetPlayerCombinedInfoAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementClientGetPlayerCombinedInfoGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayerCombinedInfoResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

/// <summary>
/// Retrieves the player's profile
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// This API allows for access to details regarding a user in the PlayFab service, usually for purposes
/// of customer support. Note that data returned may be Personally Identifying Information (PII), such
/// as email address, and so care should be taken in how this data is stored and managed. Since this call
/// will always return the relevant information for users who have accessed the title, the recommendation
/// is to not store this data locally.
///
/// When the asynchronous task is complete, call <see cref="PFAccountManagementClientGetPlayerProfileGetResultSize"/>
/// and <see cref="PFAccountManagementClientGetPlayerProfileGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementClientGetPlayerProfileAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementGetPlayerProfileRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetPlayerProfile call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_PLAYER_NOT_IN_GAME,
/// E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED or any of the global PlayFab Service errors. See doc
/// page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementClientGetPlayerProfileGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementClientGetPlayerProfileAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_PLAYER_NOT_IN_GAME,
/// E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED or any of the global PlayFab Service errors. See doc
/// page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementClientGetPlayerProfileGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayerProfileResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of Battle.net account identifiers.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsGetResultSize"/>
/// and <see cref="PFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsGetResult"/> to get the
/// result.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromBattleNetAccountIdsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetPlayFabIDsFromBattleNetAccountIds call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromBattleNetAccountIdsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromBattleNetAccountIdsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of Facebook identifiers.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, Android, iOS, and macOS.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementClientGetPlayFabIDsFromFacebookIDsGetResultSize"/>
/// and <see cref="PFAccountManagementClientGetPlayFabIDsFromFacebookIDsGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromFacebookIDsAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromFacebookIDsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetPlayFabIDsFromFacebookIDs call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementClientGetPlayFabIDsFromFacebookIDsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementClientGetPlayFabIDsFromFacebookIDsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromFacebookIDsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromFacebookIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of Facebook Instant Game identifiers.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsGetResultSize"/>
/// and <see cref="PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsGetResult"/> to get
/// the result.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetPlayFabIDsFromFacebookInstantGamesIds call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromFacebookInstantGamesIdsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_IOS || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of Game Center identifiers (referenced
/// in the Game Center Programming Guide as the Player Identifier).
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, iOS, and macOS.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsGetResultSize"/>
/// and <see cref="PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromGameCenterIDsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetPlayFabIDsFromGameCenterIDs call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromGameCenterIDsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromGameCenterIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of Google identifiers. The Google identifiers
/// are the IDs for the user accounts, available as 'id' in the Google+ People API calls.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, Android, and macOS.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementClientGetPlayFabIDsFromGoogleIDsGetResultSize"/>
/// and <see cref="PFAccountManagementClientGetPlayFabIDsFromGoogleIDsGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromGoogleIDsAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromGoogleIDsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetPlayFabIDsFromGoogleIDs call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementClientGetPlayFabIDsFromGoogleIDsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementClientGetPlayFabIDsFromGoogleIDsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromGoogleIDsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromGoogleIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_ANDROID || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of Google Play Games identifiers. The
/// Google Play Games identifiers are the IDs for the user accounts, available as 'playerId' in the Google
/// Play Games Services - Players API calls.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, Android, and macOS.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsGetResultSize"/>
/// and <see cref="PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsGetResult"/> to get
/// the result.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetPlayFabIDsFromGooglePlayGamesPlayerIDs call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromGooglePlayGamesPlayerIDsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromGooglePlayGamesPlayerIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of Kongregate identifiers. The Kongregate
/// identifiers are the IDs for the user accounts, available as 'user_id' from the Kongregate API methods(ex:
/// http://developers.kongregate.com/docs/client/getUserId).
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementClientGetPlayFabIDsFromKongregateIDsGetResultSize"/>
/// and <see cref="PFAccountManagementClientGetPlayFabIDsFromKongregateIDsGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromKongregateIDsAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromKongregateIDsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetPlayFabIDsFromKongregateIDs call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementClientGetPlayFabIDsFromKongregateIDsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementClientGetPlayFabIDsFromKongregateIDsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromKongregateIDsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromKongregateIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of Nintendo Service Account identifiers.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Nintendo Switch, Linux, and macOS.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsGetResultSize"/>
/// and <see cref="PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsGetResult"/> to
/// get the result.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetPlayFabIDsFromNintendoServiceAccountIds call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromNintendoServiceAccountIdsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of Nintendo Switch Device identifiers.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResultSize"/>
/// and <see cref="PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResult"/> to get
/// the result.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetPlayFabIDsFromNintendoSwitchDeviceIds call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5 || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of PlayStation :tm: Network identifiers.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Sony PlayStation®, Linux, and macOS.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsGetResultSize"/>
/// and <see cref="PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetPlayFabIDsFromPSNAccountIDs call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_INVALID_PSN_AUTH_CODE
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
PF_API PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_INVALID_PSN_AUTH_CODE
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromPSNAccountIDsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of PlayStation :tm: Network identifiers.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsGetResultSize"/>
/// and <see cref="PFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetPlayFabIDsFromPSNOnlineIDs call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_INVALID_PSN_AUTH_CODE
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
PF_API PFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_INVALID_PSN_AUTH_CODE
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromPSNOnlineIDsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of Steam identifiers. The Steam identifiers
/// are the profile IDs for the user accounts, available as SteamId in the Steamworks Community API calls.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementClientGetPlayFabIDsFromSteamIDsGetResultSize"/>
/// and <see cref="PFAccountManagementClientGetPlayFabIDsFromSteamIDsGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromSteamIDsAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromSteamIDsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetPlayFabIDsFromSteamIDs call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementClientGetPlayFabIDsFromSteamIDsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementClientGetPlayFabIDsFromSteamIDsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromSteamIDsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromSteamIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of Steam identifiers. The Steam identifiers
/// are persona names.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementClientGetPlayFabIDsFromSteamNamesGetResultSize"/>
/// and <see cref="PFAccountManagementClientGetPlayFabIDsFromSteamNamesGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromSteamNamesAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromSteamNamesRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetPlayFabIDsFromSteamNames call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementClientGetPlayFabIDsFromSteamNamesGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementClientGetPlayFabIDsFromSteamNamesAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromSteamNamesGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromSteamNamesResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of Twitch identifiers. The Twitch identifiers
/// are the IDs for the user accounts, available as '_id' from the Twitch API methods (ex: https://github.com/justintv/Twitch-API/blob/master/v3_resources/users.md#get-usersuser).
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementClientGetPlayFabIDsFromTwitchIDsGetResultSize"/>
/// and <see cref="PFAccountManagementClientGetPlayFabIDsFromTwitchIDsGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromTwitchIDsAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromTwitchIDsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetPlayFabIDsFromTwitchIDs call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementClientGetPlayFabIDsFromTwitchIDsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementClientGetPlayFabIDsFromTwitchIDsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromTwitchIDsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromTwitchIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of XboxLive identifiers.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsGetResultSize"/>
/// and <see cref="PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetPlayFabIDsFromXboxLiveIDs call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementClientGetPlayFabIDsFromXboxLiveIDsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

#if HC_PLATFORM == HC_PLATFORM_GDK
/// <summary>
/// Links the Battle.net account associated with the token to the user's PlayFab account.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows.
/// See also ClientLoginWithBattleNetAsync, ClientUnlinkBattleNetAccountAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_BATTLE_NET_NOT_ENABLED_FOR_TITLE, E_PF_INVALID_IDENTITY_PROVIDER_ID,
/// E_PF_LINKED_IDENTIFIER_ALREADY_CLAIMED, E_PF_TOKEN_SIGNING_KEY_NOT_FOUND or any of the global PlayFab
/// Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </remarks>
PF_API PFAccountManagementClientLinkBattleNetAccountAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementClientLinkBattleNetAccountRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

/// <summary>
/// Links the custom identifier, generated by the title, to the user's PlayFab account
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// See also ClientLoginWithCustomIDAsync, ClientUnlinkCustomIDAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_ACCOUNT_LINKED_TO_A_BANNED_PLAYER, E_PF_LINKED_IDENTIFIER_ALREADY_CLAIMED
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </remarks>
PF_API PFAccountManagementClientLinkCustomIDAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementLinkCustomIDRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Links an OpenID Connect account to a user's PlayFab account, based on an existing relationship between
/// a title and an Open ID Connect provider and the OpenId Connect JWT from that provider.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// See also ClientLoginWithOpenIdConnectAsync, ClientUnlinkOpenIdConnectAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_ACCOUNT_LINKED_TO_A_BANNED_PLAYER, E_PF_INVALID_IDENTITY_PROVIDER_ID,
/// E_PF_LINKED_IDENTIFIER_ALREADY_CLAIMED or any of the global PlayFab Service errors. See doc page "Handling
/// PlayFab Errors" for more details on error handling.
/// </remarks>
PF_API PFAccountManagementClientLinkOpenIdConnectAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementLinkOpenIdConnectRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Links the Steam account associated with the provided Steam authentication ticket to the user's PlayFab
/// account
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// Steam authentication is accomplished with the Steam Session Ticket. More information on the Ticket
/// can be found in the Steamworks SDK, here: https://partner.steamgames.com/documentation/auth (requires
/// sign-in). NOTE: For Steam authentication to work, the title must be configured with the Steam Application
/// ID and Publisher Key in the PlayFab Game Manager (under Properties). Information on creating a Publisher
/// Key (referred to as the Secret Key in PlayFab) for your title can be found here: https://partner.steamgames.com/documentation/webapi#publisherkey.
/// See also ClientLoginWithSteamAsync, ClientUnlinkSteamAccountAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_ACCOUNT_ALREADY_LINKED, E_PF_ACCOUNT_LINKED_TO_A_BANNED_PLAYER, E_PF_INVALID_STEAM_TICKET,
/// E_PF_LINKED_ACCOUNT_ALREADY_CLAIMED, E_PF_STEAM_NOT_ENABLED_FOR_TITLE, E_PF_STEAM_USER_NOT_FOUND or
/// any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </remarks>
PF_API PFAccountManagementClientLinkSteamAccountAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementLinkSteamAccountRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

/// <summary>
/// Links the Xbox Live account associated with the provided access code to the user's PlayFab account
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// See also ClientLoginWithXboxAsync, ClientUnlinkXboxAccountAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_ACCOUNT_ALREADY_LINKED, E_PF_EXPIRED_XBOX_LIVE_TOKEN, E_PF_INVALID_XBOX_LIVE_TOKEN,
/// E_PF_LINKED_ACCOUNT_ALREADY_CLAIMED or any of the global PlayFab Service errors. See doc page "Handling
/// PlayFab Errors" for more details on error handling.
/// </remarks>
PF_API PFAccountManagementClientLinkXboxAccountAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementClientLinkXboxAccountRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Removes a contact email from the player's profile.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// This API removes an existing contact email from the player's profile.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be one of global PlayFab Service errors. See doc page "Handling PlayFab Errors"
/// for more details on error handling.
/// </remarks>
PF_API PFAccountManagementClientRemoveContactEmailAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementRemoveContactEmailRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

/// <summary>
/// Submit a report for another player (due to bad bahavior, etc.), so that customer service representatives
/// for the title can take action concerning potentially toxic players.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementClientReportPlayerGetResult"/>
/// to get the result.
/// </remarks>
PF_API PFAccountManagementClientReportPlayerAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementReportPlayerClientRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementClientReportPlayerAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="result">PFAccountManagementReportPlayerClientResult object that will be populated with the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementClientReportPlayerGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFAccountManagementReportPlayerClientResult* result
) noexcept;

#if HC_PLATFORM == HC_PLATFORM_GDK
/// <summary>
/// Unlinks the related Battle.net account from the user's PlayFab account.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows.
/// See also ClientLinkBattleNetAccountAsync, ClientLoginWithBattleNetAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_ACCOUNT_NOT_LINKED, E_PF_BATTLE_NET_NOT_ENABLED_FOR_TITLE or any of
/// the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error
/// handling.
/// </remarks>
PF_API PFAccountManagementClientUnlinkBattleNetAccountAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementClientUnlinkBattleNetAccountRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

/// <summary>
/// Unlinks the related custom identifier from the user's PlayFab account
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// See also ClientLinkCustomIDAsync, ClientLoginWithCustomIDAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_ACCOUNT_NOT_LINKED, E_PF_CUSTOM_ID_NOT_LINKED or any of the global PlayFab
/// Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </remarks>
PF_API PFAccountManagementClientUnlinkCustomIDAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementUnlinkCustomIDRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Unlinks an OpenID Connect account from a user's PlayFab account, based on the connection ID of an
/// existing relationship between a title and an Open ID Connect provider.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// See also ClientLinkOpenIdConnectAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_ACCOUNT_NOT_LINKED or any of the global PlayFab Service errors. See
/// doc page "Handling PlayFab Errors" for more details on error handling.
/// </remarks>
PF_API PFAccountManagementClientUnlinkOpenIdConnectAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementUnlinkOpenIdConnectRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Unlinks the related Steam account from the user's PlayFab account
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// See also ClientLinkSteamAccountAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_ACCOUNT_NOT_LINKED or any of the global PlayFab Service errors. See
/// doc page "Handling PlayFab Errors" for more details on error handling.
/// </remarks>
PF_API PFAccountManagementClientUnlinkSteamAccountAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementUnlinkSteamAccountRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

/// <summary>
/// Unlinks the related Xbox Live account from the user's PlayFab account
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// See also ClientLinkXboxAccountAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_ACCOUNT_NOT_LINKED, E_PF_INVALID_XBOX_LIVE_TOKEN or any of the global
/// PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </remarks>
PF_API PFAccountManagementClientUnlinkXboxAccountAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementClientUnlinkXboxAccountRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Update the avatar URL of the player
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be one of global PlayFab Service errors. See doc page "Handling PlayFab Errors"
/// for more details on error handling.
/// </remarks>
PF_API PFAccountManagementClientUpdateAvatarUrlAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementClientUpdateAvatarUrlRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Updates the title specific display name for the user
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// In addition to the PlayFab username, titles can make use of a DisplayName which is also a unique
/// identifier, but specific to the title. This allows for unique names which more closely match the theme
/// or genre of a title, for example.
///
/// When the asynchronous task is complete, call <see cref="PFAccountManagementClientUpdateUserTitleDisplayNameGetResultSize"/>
/// and <see cref="PFAccountManagementClientUpdateUserTitleDisplayNameGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementClientUpdateUserTitleDisplayNameAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementUpdateUserTitleDisplayNameRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientUpdateUserTitleDisplayName call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ACCOUNT_NOT_FOUND,
/// E_PF_CONCURRENT_EDIT_ERROR, E_PF_ENTITY_PROFILE_CONSTRAINT_VALIDATION_FAILED, E_PF_ENTITY_PROFILE_VERSION_MISMATCH,
/// E_PF_INVALID_PARTNER_RESPONSE, E_PF_NAME_NOT_AVAILABLE, E_PF_PROFANE_DISPLAY_NAME or any of the global
/// PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementClientUpdateUserTitleDisplayNameGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementClientUpdateUserTitleDisplayNameAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ACCOUNT_NOT_FOUND,
/// E_PF_CONCURRENT_EDIT_ERROR, E_PF_ENTITY_PROFILE_CONSTRAINT_VALIDATION_FAILED, E_PF_ENTITY_PROFILE_VERSION_MISMATCH,
/// E_PF_INVALID_PARTNER_RESPONSE, E_PF_NAME_NOT_AVAILABLE, E_PF_PROFANE_DISPLAY_NAME or any of the global
/// PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementClientUpdateUserTitleDisplayNameGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementUpdateUserTitleDisplayNameResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Bans users by PlayFab ID with optional IP address for the provided game.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// The existence of each user will not be verified. When banning by IP, multiple players may be affected,
/// so use this feature with caution. Returns information about the new bans. See also ServerGetUserBansAsync,
/// ServerRevokeAllBansForUserAsync, ServerRevokeBansAsync, ServerUpdateBansAsync.
///
/// When the asynchronous task is complete, call <see cref="PFAccountManagementServerBanUsersGetResultSize"/>
/// and <see cref="PFAccountManagementServerBanUsersGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementServerBanUsersAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementBanUsersRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerBanUsers call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_USERIS_NOT_VALID
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
PF_API PFAccountManagementServerBanUsersGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementServerBanUsersAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_USERIS_NOT_VALID
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementServerBanUsersGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementBanUsersResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Removes a user's player account from a title and deletes all associated data
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// Deletes all data associated with the player, including statistics, custom data, inventory, purchases,
/// virtual currency balances, characters and shared group memberships. Removes the player from all leaderboards
/// and player search indexes. Does not delete PlayStream event history associated with the player. Does
/// not delete the publisher user account that created the player in the title nor associated data such
/// as username, password, email address, account linkages, or friends list. Note, this API queues the
/// player for deletion and returns immediately. It may take several minutes or more before all player
/// data is fully deleted. Until the player data is fully deleted, attempts to recreate the player with
/// the same user account in the same title will fail with the 'AccountDeleted' error. This API must be
/// enabled for use as an option in the game manager website. It is disabled by default.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_API_NOT_ENABLED_FOR_GAME_SERVER_ACCESS or any of the global PlayFab
/// Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </remarks>
PF_API PFAccountManagementServerDeletePlayerAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementDeletePlayerRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Returns whatever info is requested in the response for the user. Note that PII (like email address,
/// facebook id) may be returned. All parameters default to false.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementServerGetPlayerCombinedInfoGetResultSize"/>
/// and <see cref="PFAccountManagementServerGetPlayerCombinedInfoGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementServerGetPlayerCombinedInfoAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementGetPlayerCombinedInfoRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetPlayerCombinedInfo call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementServerGetPlayerCombinedInfoGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementServerGetPlayerCombinedInfoAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementServerGetPlayerCombinedInfoGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayerCombinedInfoResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves the player's profile
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// This API allows for access to details regarding a user in the PlayFab service, usually for purposes
/// of customer support. Note that data returned may be Personally Identifying Information (PII), such
/// as email address, and so care should be taken in how this data is stored and managed. Since this call
/// will always return the relevant information for users who have accessed the title, the recommendation
/// is to not store this data locally.
///
/// When the asynchronous task is complete, call <see cref="PFAccountManagementServerGetPlayerProfileGetResultSize"/>
/// and <see cref="PFAccountManagementServerGetPlayerProfileGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementServerGetPlayerProfileAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementGetPlayerProfileRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetPlayerProfile call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_PLAYER_NOT_IN_GAME
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
PF_API PFAccountManagementServerGetPlayerProfileGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementServerGetPlayerProfileAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_PLAYER_NOT_IN_GAME
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementServerGetPlayerProfileGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayerProfileResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of Battle.net account identifiers.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsGetResultSize"/>
/// and <see cref="PFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsGetResult"/> to get the
/// result.
/// </remarks>
PF_API PFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromBattleNetAccountIdsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetPlayFabIDsFromBattleNetAccountIds call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementServerGetPlayFabIDsFromBattleNetAccountIdsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromBattleNetAccountIdsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of Facebook identifiers.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementServerGetPlayFabIDsFromFacebookIDsGetResultSize"/>
/// and <see cref="PFAccountManagementServerGetPlayFabIDsFromFacebookIDsGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementServerGetPlayFabIDsFromFacebookIDsAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromFacebookIDsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetPlayFabIDsFromFacebookIDs call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementServerGetPlayFabIDsFromFacebookIDsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementServerGetPlayFabIDsFromFacebookIDsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementServerGetPlayFabIDsFromFacebookIDsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromFacebookIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of Facebook Instant Games identifiers.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsGetResultSize"/>
/// and <see cref="PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsGetResult"/> to get
/// the result.
/// </remarks>
PF_API PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetPlayFabIDsFromFacebookInstantGamesIds call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementServerGetPlayFabIDsFromFacebookInstantGamesIdsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromFacebookInstantGamesIdsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of Nintendo Service Account identifiers.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsGetResultSize"/>
/// and <see cref="PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsGetResult"/> to
/// get the result.
/// </remarks>
PF_API PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetPlayFabIDsFromNintendoServiceAccountIds call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementServerGetPlayFabIDsFromNintendoServiceAccountIdsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromNintendoServiceAccountIdsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of Nintendo Switch Device identifiers.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResultSize"/>
/// and <see cref="PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResult"/> to get
/// the result.
/// </remarks>
PF_API PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetPlayFabIDsFromNintendoSwitchDeviceIds call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementServerGetPlayFabIDsFromNintendoSwitchDeviceIdsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromNintendoSwitchDeviceIdsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of PlayStation :tm: Network identifiers.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsGetResultSize"/>
/// and <see cref="PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromPSNAccountIDsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetPlayFabIDsFromPSNAccountIDs call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementServerGetPlayFabIDsFromPSNAccountIDsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromPSNAccountIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of PlayStation :tm: Network identifiers.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsGetResultSize"/>
/// and <see cref="PFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetPlayFabIDsFromPSNOnlineIDs call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementServerGetPlayFabIDsFromPSNOnlineIDsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromPSNOnlineIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of Steam identifiers. The Steam identifiers
/// are the profile IDs for the user accounts, available as SteamId in the Steamworks Community API calls.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementServerGetPlayFabIDsFromSteamIDsGetResultSize"/>
/// and <see cref="PFAccountManagementServerGetPlayFabIDsFromSteamIDsGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementServerGetPlayFabIDsFromSteamIDsAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromSteamIDsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetPlayFabIDsFromSteamIDs call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementServerGetPlayFabIDsFromSteamIDsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementServerGetPlayFabIDsFromSteamIDsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementServerGetPlayFabIDsFromSteamIDsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromSteamIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of Steam identifiers. The Steam identifiers
/// are persona names.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementServerGetPlayFabIDsFromSteamNamesGetResultSize"/>
/// and <see cref="PFAccountManagementServerGetPlayFabIDsFromSteamNamesGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementServerGetPlayFabIDsFromSteamNamesAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromSteamNamesRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetPlayFabIDsFromSteamNames call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementServerGetPlayFabIDsFromSteamNamesGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementServerGetPlayFabIDsFromSteamNamesAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementServerGetPlayFabIDsFromSteamNamesGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromSteamNamesResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of Twitch identifiers. The Twitch identifiers
/// are the IDs for the user accounts, available as '_id' from the Twitch API methods (ex: https://github.com/justintv/Twitch-API/blob/master/v3_resources/users.md#get-usersuser).
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementServerGetPlayFabIDsFromTwitchIDsGetResultSize"/>
/// and <see cref="PFAccountManagementServerGetPlayFabIDsFromTwitchIDsGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementServerGetPlayFabIDsFromTwitchIDsAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromTwitchIDsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetPlayFabIDsFromTwitchIDs call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementServerGetPlayFabIDsFromTwitchIDsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementServerGetPlayFabIDsFromTwitchIDsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementServerGetPlayFabIDsFromTwitchIDsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromTwitchIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of XboxLive identifiers.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsGetResultSize"/>
/// and <see cref="PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementGetPlayFabIDsFromXboxLiveIDsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetPlayFabIDsFromXboxLiveIDs call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementServerGetPlayFabIDsFromXboxLiveIDsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetPlayFabIDsFromXboxLiveIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves the associated PlayFab account identifiers for the given set of server custom identifiers.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// When the asynchronous task is complete, call <see cref="PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsGetResultSize"/>
/// and <see cref="PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementGetServerCustomIDsFromPlayFabIDsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetServerCustomIDsFromPlayFabIDs call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementServerGetServerCustomIDsFromPlayFabIDsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetServerCustomIDsFromPlayFabIDsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves the relevant details for a specified user
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// This API allows for access to details regarding a user in the PlayFab service, usually for purposes
/// of customer support. Note that data returned may be Personally Identifying Information (PII), such
/// as email address, and so care should be taken in how this data is stored and managed. Since this call
/// will always return the relevant information for users who have accessed the title, the recommendation
/// is to not store this data locally. See also ServerGetUserInventoryAsync.
///
/// When the asynchronous task is complete, call <see cref="PFAccountManagementServerGetUserAccountInfoGetResultSize"/>
/// and <see cref="PFAccountManagementServerGetUserAccountInfoGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementServerGetUserAccountInfoAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementGetUserAccountInfoRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetUserAccountInfo call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementServerGetUserAccountInfoGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementServerGetUserAccountInfoAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementServerGetUserAccountInfoGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetUserAccountInfoResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Gets all bans for a user.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// Get all bans for a user, including inactive and expired bans.  See also ServerBanUsersAsync, ServerRevokeAllBansForUserAsync,
/// ServerRevokeBansAsync, ServerUpdateBansAsync.
///
/// When the asynchronous task is complete, call <see cref="PFAccountManagementServerGetUserBansGetResultSize"/>
/// and <see cref="PFAccountManagementServerGetUserBansGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementServerGetUserBansAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementGetUserBansRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetUserBans call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_USERIS_NOT_VALID
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
PF_API PFAccountManagementServerGetUserBansGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementServerGetUserBansAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_USERIS_NOT_VALID
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementServerGetUserBansGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetUserBansResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
/// <summary>
/// Links the Battle.net account associated with the token to the user's PlayFab account.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows.
/// See also ServerUnlinkBattleNetAccountAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_BATTLE_NET_NOT_ENABLED_FOR_TITLE, E_PF_LINKED_IDENTIFIER_ALREADY_CLAIMED
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </remarks>
PF_API PFAccountManagementServerLinkBattleNetAccountAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementServerLinkBattleNetAccountRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Links the Nintendo account associated with the token to the user's PlayFab account
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// See also ServerLinkNintendoServiceAccountSubjectAsync, ServerUnlinkNintendoServiceAccountAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_INVALID_IDENTITY_PROVIDER_ID, E_PF_LINKED_IDENTIFIER_ALREADY_CLAIMED,
/// E_PF_NINTENDO_SWITCH_NOT_ENABLED_FOR_TITLE or any of the global PlayFab Service errors. See doc page
/// "Handling PlayFab Errors" for more details on error handling.
/// </remarks>
PF_API PFAccountManagementServerLinkNintendoServiceAccountAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementServerLinkNintendoServiceAccountRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Links the Nintendo account associated with the Nintendo Service Account subject or id to the user's
/// PlayFab account
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// See also ServerLinkNintendoServiceAccountAsync, ServerUnlinkNintendoServiceAccountAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_INVALID_IDENTITY_PROVIDER_ID, E_PF_LINKED_IDENTIFIER_ALREADY_CLAIMED,
/// E_PF_NINTENDO_SWITCH_NOT_ENABLED_FOR_TITLE or any of the global PlayFab Service errors. See doc page
/// "Handling PlayFab Errors" for more details on error handling.
/// </remarks>
PF_API PFAccountManagementServerLinkNintendoServiceAccountSubjectAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementLinkNintendoServiceAccountSubjectRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Links the NintendoSwitchDeviceId to the user's PlayFab account
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// See also ServerUnlinkNintendoSwitchDeviceIdAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_LINKED_ACCOUNT_ALREADY_CLAIMED or any of the global PlayFab Service
/// errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </remarks>
PF_API PFAccountManagementServerLinkNintendoSwitchDeviceIdAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementServerLinkNintendoSwitchDeviceIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Links the PlayStation :tm: Network account associated with the provided access code to the user's
/// PlayFab account
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// See also ServerUnlinkPSNAccountAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_ACCOUNT_ALREADY_LINKED, E_PF_ACCOUNT_NOT_FOUND, E_PF_INVALID_NAMESPACE_MISMATCH,
/// E_PF_INVALID_PSN_AUTH_CODE, E_PF_INVALID_PSN_ISSUER_ID, E_PF_LINKED_ACCOUNT_ALREADY_CLAIMED, E_PF_PSN_INACCESSIBLE,
/// E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED or any of the global PlayFab Service errors. See doc
/// page "Handling PlayFab Errors" for more details on error handling.
/// </remarks>
PF_API PFAccountManagementServerLinkPSNAccountAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementServerLinkPSNAccountRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
/// <summary>
/// Links the PlayStation :tm: Network account associated with the provided user id to the user's PlayFab
/// account
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows.
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_ACCOUNT_ALREADY_LINKED, E_PF_ACCOUNT_NOT_FOUND, E_PF_INVALID_NAMESPACE_MISMATCH,
/// E_PF_INVALID_PSN_AUTH_CODE, E_PF_INVALID_PSN_ISSUER_ID, E_PF_LINKED_ACCOUNT_ALREADY_CLAIMED, E_PF_PSN_INACCESSIBLE
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </remarks>
PF_API PFAccountManagementServerLinkPSNIdAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementLinkPSNIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Links the custom server identifier, generated by the title, to the user's PlayFab account.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_ACCOUNT_NOT_FOUND, E_PF_LINKED_IDENTIFIER_ALREADY_CLAIMED or any of
/// the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error
/// handling.
/// </remarks>
PF_API PFAccountManagementServerLinkServerCustomIdAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementLinkServerCustomIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Links the Steam account associated with the provided Steam ID to the user's PlayFab account 
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// See also ServerLoginWithSteamIdAsync, ServerUnlinkSteamIdAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_ACCOUNT_ALREADY_LINKED, E_PF_INVALID_STEAM_TICKET, E_PF_LINKED_ACCOUNT_ALREADY_CLAIMED,
/// E_PF_STEAM_NOT_ENABLED_FOR_TITLE, E_PF_STEAM_USER_NOT_FOUND or any of the global PlayFab Service errors.
/// See doc page "Handling PlayFab Errors" for more details on error handling.
/// </remarks>
PF_API PFAccountManagementServerLinkSteamIdAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementLinkSteamIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Links the Xbox Live account associated with the provided access code to the user's PlayFab account
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// See also ServerLoginWithXboxAsync, ServerUnlinkXboxAccountAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_ACCOUNT_ALREADY_LINKED, E_PF_INVALID_XBOX_LIVE_TOKEN, E_PF_LINKED_ACCOUNT_ALREADY_CLAIMED
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </remarks>
PF_API PFAccountManagementServerLinkXboxAccountAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementServerLinkXboxAccountRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Revoke all active bans for a user.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// Setting the active state of all non-expired bans for a user to Inactive. Expired bans with an Active
/// state will be ignored, however. Returns information about applied updates only. See also ServerBanUsersAsync,
/// ServerGetUserBansAsync, ServerRevokeBansAsync, ServerUpdateBansAsync.
///
/// When the asynchronous task is complete, call <see cref="PFAccountManagementServerRevokeAllBansForUserGetResultSize"/>
/// and <see cref="PFAccountManagementServerRevokeAllBansForUserGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementServerRevokeAllBansForUserAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementRevokeAllBansForUserRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerRevokeAllBansForUser call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_USERIS_NOT_VALID
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
PF_API PFAccountManagementServerRevokeAllBansForUserGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementServerRevokeAllBansForUserAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_USERIS_NOT_VALID
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementServerRevokeAllBansForUserGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementRevokeAllBansForUserResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Revoke all active bans specified with BanId.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// Setting the active state of all bans requested to Inactive regardless of whether that ban has already
/// expired. BanIds that do not exist will be skipped. Returns information about applied updates only.
///  See also ServerBanUsersAsync, ServerGetUserBansAsync, ServerRevokeAllBansForUserAsync, ServerUpdateBansAsync.
///
/// When the asynchronous task is complete, call <see cref="PFAccountManagementServerRevokeBansGetResultSize"/>
/// and <see cref="PFAccountManagementServerRevokeBansGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementServerRevokeBansAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementRevokeBansRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerRevokeBans call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementServerRevokeBansGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementServerRevokeBansAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementServerRevokeBansGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementRevokeBansResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Forces an email to be sent to the registered contact email address for the user's account based on
/// an account recovery email template
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// PlayFab accounts which have valid email address or username will be able to receive a password reset
/// email using this API.The email sent must be an account recovery email template. The username or email
/// can be passed in to send the email.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_EMAIL_CLIENT_CANCELED_TASK, E_PF_EMAIL_CLIENT_TIMEOUT, E_PF_EMAIL_MESSAGE_TO_ADDRESS_IS_MISSING,
/// E_PF_EMAIL_TEMPLATE_MISSING, E_PF_NO_CONTACT_EMAIL_ADDRESS_FOUND, E_PF_SMTP_ADDON_NOT_ENABLED or any
/// of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error
/// handling.
/// </remarks>
PF_API PFAccountManagementServerSendCustomAccountRecoveryEmailAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementSendCustomAccountRecoveryEmailRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Sends an email based on an email template to a player's contact email 
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// Sends an email for only players that have contact emails associated with them. Takes in an email
/// template ID specifyingthe email template to send.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_EMAIL_CLIENT_CANCELED_TASK, E_PF_EMAIL_CLIENT_TIMEOUT, E_PF_EMAIL_TEMPLATE_MISSING,
/// E_PF_NO_CONTACT_EMAIL_ADDRESS_FOUND, E_PF_SMTP_ADDON_NOT_ENABLED or any of the global PlayFab Service
/// errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </remarks>
PF_API PFAccountManagementServerSendEmailFromTemplateAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementSendEmailFromTemplateRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
/// <summary>
/// Unlinks the related Battle.net account from the user's PlayFab account.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows.
/// See also ServerLinkBattleNetAccountAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_ACCOUNT_NOT_LINKED, E_PF_BATTLE_NET_NOT_ENABLED_FOR_TITLE or any of
/// the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error
/// handling.
/// </remarks>
PF_API PFAccountManagementServerUnlinkBattleNetAccountAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementServerUnlinkBattleNetAccountRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Unlinks the related Nintendo account from the user's PlayFab account
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// See also ServerLinkNintendoServiceAccountAsync, ServerLinkNintendoServiceAccountSubjectAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_ACCOUNT_NOT_LINKED, E_PF_NINTENDO_SWITCH_NOT_ENABLED_FOR_TITLE or any
/// of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error
/// handling.
/// </remarks>
PF_API PFAccountManagementServerUnlinkNintendoServiceAccountAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementServerUnlinkNintendoServiceAccountRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Unlinks the related NintendoSwitchDeviceId from the user's PlayFab account
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// See also ServerLinkNintendoSwitchDeviceIdAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_ACCOUNT_NOT_LINKED, E_PF_NINTENDO_SWITCH_DEVICE_ID_NOT_LINKED or any
/// of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error
/// handling.
/// </remarks>
PF_API PFAccountManagementServerUnlinkNintendoSwitchDeviceIdAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementServerUnlinkNintendoSwitchDeviceIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Unlinks the related PlayStation :tm: Network account from the user's PlayFab account
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// See also ServerLinkPSNAccountAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_ACCOUNT_NOT_LINKED or any of the global PlayFab Service errors. See
/// doc page "Handling PlayFab Errors" for more details on error handling.
/// </remarks>
PF_API PFAccountManagementServerUnlinkPSNAccountAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementServerUnlinkPSNAccountRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Unlinks the custom server identifier from the user's PlayFab account.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// See also ServerLinkServerCustomIdAsync, ServerLoginWithServerCustomIdAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_ACCOUNT_NOT_LINKED, E_PF_IDENTIFIER_NOT_LINKED or any of the global
/// PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </remarks>
PF_API PFAccountManagementServerUnlinkServerCustomIdAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementUnlinkServerCustomIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Unlinks the Steam account associated with the provided Steam ID to the user's PlayFab account 
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// See also ServerLinkSteamIdAsync, ServerLoginWithSteamIdAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_ACCOUNT_NOT_LINKED or any of the global PlayFab Service errors. See
/// doc page "Handling PlayFab Errors" for more details on error handling.
/// </remarks>
PF_API PFAccountManagementServerUnlinkSteamIdAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementUnlinkSteamIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Unlinks the related Xbox Live account from the user's PlayFab account
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// See also ServerLinkXboxAccountAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_ACCOUNT_NOT_LINKED, E_PF_INVALID_XBOX_LIVE_TOKEN or any of the global
/// PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </remarks>
PF_API PFAccountManagementServerUnlinkXboxAccountAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementServerUnlinkXboxAccountRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Update the avatar URL of the specified player
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be one of global PlayFab Service errors. See doc page "Handling PlayFab Errors"
/// for more details on error handling.
/// </remarks>
PF_API PFAccountManagementServerUpdateAvatarUrlAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementServerUpdateAvatarUrlRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Updates information of a list of existing bans specified with Ban Ids.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// For each ban, only updates the values that are set. Leave any value to null for no change. If a ban
/// could not be found, the rest are still applied. Returns information about applied updates only. See
/// also ServerBanUsersAsync, ServerGetUserBansAsync, ServerRevokeAllBansForUserAsync, ServerRevokeBansAsync.
///
/// When the asynchronous task is complete, call <see cref="PFAccountManagementServerUpdateBansGetResultSize"/>
/// and <see cref="PFAccountManagementServerUpdateBansGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementServerUpdateBansAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFAccountManagementUpdateBansRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerUpdateBans call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementServerUpdateBansGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementServerUpdateBansAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementServerUpdateBansGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementUpdateBansResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

/// <summary>
/// Retrieves the title player accounts associated with the given XUIDs.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// Given a collection of Xbox IDs (XUIDs), returns all title player accounts.
///
/// When the asynchronous task is complete, call <see cref="PFAccountManagementGetTitlePlayersFromXboxLiveIDsGetResultSize"/>
/// and <see cref="PFAccountManagementGetTitlePlayersFromXboxLiveIDsGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementGetTitlePlayersFromXboxLiveIDsAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementGetTitlePlayersFromXboxLiveIDsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a GetTitlePlayersFromXboxLiveIDs call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementGetTitlePlayersFromXboxLiveIDsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementGetTitlePlayersFromXboxLiveIDsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementGetTitlePlayersFromXboxLiveIDsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementGetTitlePlayersFromProviderIDsResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

/// <summary>
/// Update the display name of the entity
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// Given an entity profile, will update its display name to the one passed in if the profile's version
/// is equal to the specified value See also ProfileGetProfileAsync.
///
/// When the asynchronous task is complete, call <see cref="PFAccountManagementSetDisplayNameGetResultSize"/>
/// and <see cref="PFAccountManagementSetDisplayNameGetResult"/> to get the result.
/// </remarks>
PF_API PFAccountManagementSetDisplayNameAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementSetDisplayNameRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a SetDisplayName call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ACCOUNT_NOT_FOUND,
/// E_PF_CONCURRENT_EDIT_ERROR, E_PF_ENTITY_PROFILE_CONSTRAINT_VALIDATION_FAILED, E_PF_ENTITY_PROFILE_VERSION_MISMATCH,
/// E_PF_INVALID_PARTNER_RESPONSE, E_PF_NAME_NOT_AVAILABLE, E_PF_PROFANE_DISPLAY_NAME or any of the global
/// PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAccountManagementSetDisplayNameGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAccountManagementSetDisplayNameAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ACCOUNT_NOT_FOUND,
/// E_PF_CONCURRENT_EDIT_ERROR, E_PF_ENTITY_PROFILE_CONSTRAINT_VALIDATION_FAILED, E_PF_ENTITY_PROFILE_VERSION_MISMATCH,
/// E_PF_INVALID_PARTNER_RESPONSE, E_PF_NAME_NOT_AVAILABLE, E_PF_PROFANE_DISPLAY_NAME or any of the global
/// PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAccountManagementSetDisplayNameGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAccountManagementSetDisplayNameResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;


}
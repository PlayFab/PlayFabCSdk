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

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves the unique PlayFab identifiers for the given set of XboxLive identifiers.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Win32, GDK, Linux, and macOS.
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
/// the async result will be E_PF_LINKED_IDENTIFIER_ALREADY_CLAIMED or any of the global PlayFab Service
/// errors. See doc page "Handling PlayFab Errors" for more details on error handling.
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
/// the async result will be E_PF_INVALID_IDENTITY_PROVIDER_ID, E_PF_LINKED_IDENTIFIER_ALREADY_CLAIMED
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </remarks>
PF_API PFAccountManagementClientLinkOpenIdConnectAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAccountManagementLinkOpenIdConnectRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Links the Xbox Live account associated with the provided access code to the user's PlayFab account
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Win32, GDK, Linux, and macOS.
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

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Unlinks the related Xbox Live account from the user's PlayFab account
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Win32, GDK, Linux, and macOS.
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
#endif

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


}
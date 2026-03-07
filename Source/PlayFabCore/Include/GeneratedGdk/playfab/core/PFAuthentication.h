// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/PFAuthenticationTypes.h>
#include <playfab/core/PFServiceConfig.h>
#include <playfab/core/PFEntity.h>
#if HC_PLATFORM == HC_PLATFORM_GDK
#include <playfab/core/PFAuthentication_Xbox.h>
#endif

extern "C"
{

#if HC_PLATFORM == HC_PLATFORM_GDK
/// <summary>
/// Sign in the user with a Battle.net identity token
/// </summary>
/// <param name="serviceConfigHandle">PFServiceConfigHandle returned from PFServiceConfigCreateHandle call.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows.
/// See also ClientLinkBattleNetAccountAsync, ClientUnlinkBattleNetAccountAsync.
///
/// When the asynchronous task is complete, call <see cref="PFAuthenticationLoginWithBattleNetGetResult"/>
/// to get the result.
/// </remarks>
PF_API PFAuthenticationLoginWithBattleNetAsync(
    _In_ PFServiceConfigHandle serviceConfigHandle,
    _In_ const PFAuthenticationLoginWithBattleNetRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a PFAuthenticationLoginWithBattleNetAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ACCOUNT_NOT_FOUND,
/// E_PF_BATTLE_NET_NOT_ENABLED_FOR_TITLE, E_PF_ENCRYPTION_KEY_MISSING, E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED,
/// E_PF_INVALID_AUTH_TOKEN, E_PF_INVALID_SIGNATURE, E_PF_INVALID_SIGNATURE_TIME, E_PF_INVALID_TITLE_ID,
/// E_PF_PLAYER_SECRET_ALREADY_CONFIGURED, E_PF_PLAYER_SECRET_NOT_CONFIGURED, E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
PF_API PFAuthenticationLoginWithBattleNetGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Get the result from a PFAuthenticationLoginWithBattleNetAsync call. The PFEntityHandle will always be returned, but the additional info
/// in the PFAuthenticationLoginResult is only returned if a buffer is provided.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="entityHandle">PFEntityHandle which can be used to authenticate other PlayFab API calls.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the Login result value and its fields.</param>
/// <param name="result">Pointer to the LoginResult object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ACCOUNT_NOT_FOUND,
/// E_PF_BATTLE_NET_NOT_ENABLED_FOR_TITLE, E_PF_ENCRYPTION_KEY_MISSING, E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED,
/// E_PF_INVALID_AUTH_TOKEN, E_PF_INVALID_SIGNATURE, E_PF_INVALID_SIGNATURE_TIME, E_PF_INVALID_TITLE_ID,
/// E_PF_PLAYER_SECRET_ALREADY_CONFIGURED, E_PF_PLAYER_SECRET_NOT_CONFIGURED, E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
/// <remarks>
/// If the PFAuthenticationLoginWithBattleNetAsync call fails, entityHandle with be null. Otherwise, the handle must be closed with PFEntityCloseHandle
/// when it is no longer needed. If returned, 'result' is a pointer within 'buffer' and does not need to be freed separately.
/// </remarks>
PF_API PFAuthenticationLoginWithBattleNetGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

/// <summary>
/// Reauthenticates an existing PFEntityHandle. Used to address situations where the EntityToken expired and the PlayFab SDK is unable to refresh it.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to re-login.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation.
/// If successful, the cached EntityToken for the PFEntityHandle will be updated in place.
/// </remarks>
PF_API PFAuthenticationReLoginWithBattleNetAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithBattleNetRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

#endif

/// <summary>
/// Signs the user in using a custom unique identifier generated by the title, returning a session identifier
/// that can subsequently be used for API calls which require an authenticated user
/// </summary>
/// <param name="serviceConfigHandle">PFServiceConfigHandle returned from PFServiceConfigCreateHandle call.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// It is highly recommended that developers ensure that it is extremely unlikely that a customer could
/// generate an ID which is already in use by another customer. If this is the first time a user has signed
/// in with the Custom ID and CreateAccount is set to true, a new PlayFab account will be created and
/// linked to the Custom ID. In this case, no email or username will be associated with the PlayFab account.
/// Otherwise, if no PlayFab account is linked to the Custom ID, an error indicating this will be returned,
/// so that the title can guide the user through creation of a PlayFab account. See also ClientLinkCustomIDAsync,
/// ClientUnlinkCustomIDAsync.
///
/// When the asynchronous task is complete, call <see cref="PFAuthenticationLoginWithCustomIDGetResult"/>
/// to get the result.
/// </remarks>
PF_API PFAuthenticationLoginWithCustomIDAsync(
    _In_ PFServiceConfigHandle serviceConfigHandle,
    _In_ const PFAuthenticationLoginWithCustomIDRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a PFAuthenticationLoginWithCustomIDAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ENCRYPTED_REQUEST_NOT_ALLOWED,
/// E_PF_ENCRYPTION_KEY_MISSING, E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED, E_PF_INVALID_SIGNATURE, E_PF_INVALID_SIGNATURE_TIME,
/// E_PF_PLAYER_CREATION_DISABLED, E_PF_PLAYER_SECRET_ALREADY_CONFIGURED, E_PF_PLAYER_SECRET_NOT_CONFIGURED,
/// E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED, E_PF_SIGNED_REQUEST_NOT_ALLOWED or any of the global
/// PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAuthenticationLoginWithCustomIDGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Get the result from a PFAuthenticationLoginWithCustomIDAsync call. The PFEntityHandle will always be returned, but the additional info
/// in the PFAuthenticationLoginResult is only returned if a buffer is provided.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="entityHandle">PFEntityHandle which can be used to authenticate other PlayFab API calls.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the Login result value and its fields.</param>
/// <param name="result">Pointer to the LoginResult object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ENCRYPTED_REQUEST_NOT_ALLOWED,
/// E_PF_ENCRYPTION_KEY_MISSING, E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED, E_PF_INVALID_SIGNATURE, E_PF_INVALID_SIGNATURE_TIME,
/// E_PF_PLAYER_CREATION_DISABLED, E_PF_PLAYER_SECRET_ALREADY_CONFIGURED, E_PF_PLAYER_SECRET_NOT_CONFIGURED,
/// E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED, E_PF_SIGNED_REQUEST_NOT_ALLOWED or any of the global
/// PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// If the PFAuthenticationLoginWithCustomIDAsync call fails, entityHandle with be null. Otherwise, the handle must be closed with PFEntityCloseHandle
/// when it is no longer needed. If returned, 'result' is a pointer within 'buffer' and does not need to be freed separately.
/// </remarks>
PF_API PFAuthenticationLoginWithCustomIDGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

/// <summary>
/// Reauthenticates an existing PFEntityHandle. Used to address situations where the EntityToken expired and the PlayFab SDK is unable to refresh it.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to re-login.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation.
/// If successful, the cached EntityToken for the PFEntityHandle will be updated in place.
/// </remarks>
PF_API PFAuthenticationReLoginWithCustomIDAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithCustomIDRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;


/// <summary>
/// Logs in a user with an Open ID Connect JWT created by an existing relationship between a title and
/// an Open ID Connect provider.
/// </summary>
/// <param name="serviceConfigHandle">PFServiceConfigHandle returned from PFServiceConfigCreateHandle call.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// See also ClientLinkOpenIdConnectAsync, ClientUnlinkOpenIdConnectAsync.
///
/// When the asynchronous task is complete, call <see cref="PFAuthenticationLoginWithOpenIdConnectGetResult"/>
/// to get the result.
/// </remarks>
PF_API PFAuthenticationLoginWithOpenIdConnectAsync(
    _In_ PFServiceConfigHandle serviceConfigHandle,
    _In_ const PFAuthenticationLoginWithOpenIdConnectRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a PFAuthenticationLoginWithOpenIdConnectAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ENCRYPTION_KEY_MISSING,
/// E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED, E_PF_INVALID_IDENTITY_PROVIDER_ID, E_PF_INVALID_SIGNATURE,
/// E_PF_INVALID_SIGNATURE_TIME, E_PF_PLAYER_SECRET_ALREADY_CONFIGURED, E_PF_PLAYER_SECRET_NOT_CONFIGURED,
/// E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED or any of the global PlayFab Service errors. See doc
/// page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAuthenticationLoginWithOpenIdConnectGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Get the result from a PFAuthenticationLoginWithOpenIdConnectAsync call. The PFEntityHandle will always be returned, but the additional info
/// in the PFAuthenticationLoginResult is only returned if a buffer is provided.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="entityHandle">PFEntityHandle which can be used to authenticate other PlayFab API calls.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the Login result value and its fields.</param>
/// <param name="result">Pointer to the LoginResult object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ENCRYPTION_KEY_MISSING,
/// E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED, E_PF_INVALID_IDENTITY_PROVIDER_ID, E_PF_INVALID_SIGNATURE,
/// E_PF_INVALID_SIGNATURE_TIME, E_PF_PLAYER_SECRET_ALREADY_CONFIGURED, E_PF_PLAYER_SECRET_NOT_CONFIGURED,
/// E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED or any of the global PlayFab Service errors. See doc
/// page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// If the PFAuthenticationLoginWithOpenIdConnectAsync call fails, entityHandle with be null. Otherwise, the handle must be closed with PFEntityCloseHandle
/// when it is no longer needed. If returned, 'result' is a pointer within 'buffer' and does not need to be freed separately.
/// </remarks>
PF_API PFAuthenticationLoginWithOpenIdConnectGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

/// <summary>
/// Reauthenticates an existing PFEntityHandle. Used to address situations where the EntityToken expired and the PlayFab SDK is unable to refresh it.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to re-login.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation.
/// If successful, the cached EntityToken for the PFEntityHandle will be updated in place.
/// </remarks>
PF_API PFAuthenticationReLoginWithOpenIdConnectAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithOpenIdConnectRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;


#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Signs the user in using a Steam authentication ticket, returning a session identifier that can subsequently
/// be used for API calls which require an authenticated user
/// </summary>
/// <param name="serviceConfigHandle">PFServiceConfigHandle returned from PFServiceConfigCreateHandle call.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// Steam sign-in is accomplished with the Steam Session Ticket. More information on the Ticket can be
/// found in the Steamworks SDK, here: https://partner.steamgames.com/documentation/auth. NOTE: For Steam
/// authentication to work, the title must be configured with the Steam Application ID and Web API Key
/// in the PlayFab Game Manager (under Steam in the Add-ons Marketplace). You can obtain a Web API Key
/// from the Permissions page of any Group associated with your App ID in the Steamworks site. If this
/// is the first time a user has signed in with the Steam account and CreateAccount is set to true, a
/// new PlayFab account will be created and linked to the provided account's Steam ID. In this case, no
/// email or username will be associated with the PlayFab account. Otherwise, if no PlayFab account is
/// linked to the Steam account, an error indicating this will be returned, so that the title can guide
/// the user through creation of a PlayFab account. See also ClientLinkSteamAccountAsync, ClientUnlinkSteamAccountAsync.
///
/// When the asynchronous task is complete, call <see cref="PFAuthenticationLoginWithSteamGetResult"/>
/// to get the result.
/// </remarks>
PF_API PFAuthenticationLoginWithSteamAsync(
    _In_ PFServiceConfigHandle serviceConfigHandle,
    _In_ const PFAuthenticationLoginWithSteamRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a PFAuthenticationLoginWithSteamAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ENCRYPTION_KEY_MISSING,
/// E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED, E_PF_INVALID_STEAM_TICKET, E_PF_PLAYER_SECRET_ALREADY_CONFIGURED,
/// E_PF_PLAYER_SECRET_NOT_CONFIGURED, E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED, E_PF_STEAM_NOT_ENABLED_FOR_TITLE,
/// E_PF_STEAM_USER_NOT_FOUND or any of the global PlayFab Service errors. See doc page "Handling PlayFab
/// Errors" for more details on error handling.
/// </returns>
PF_API PFAuthenticationLoginWithSteamGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Get the result from a PFAuthenticationLoginWithSteamAsync call. The PFEntityHandle will always be returned, but the additional info
/// in the PFAuthenticationLoginResult is only returned if a buffer is provided.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="entityHandle">PFEntityHandle which can be used to authenticate other PlayFab API calls.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the Login result value and its fields.</param>
/// <param name="result">Pointer to the LoginResult object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ENCRYPTION_KEY_MISSING,
/// E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED, E_PF_INVALID_STEAM_TICKET, E_PF_PLAYER_SECRET_ALREADY_CONFIGURED,
/// E_PF_PLAYER_SECRET_NOT_CONFIGURED, E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED, E_PF_STEAM_NOT_ENABLED_FOR_TITLE,
/// E_PF_STEAM_USER_NOT_FOUND or any of the global PlayFab Service errors. See doc page "Handling PlayFab
/// Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// If the PFAuthenticationLoginWithSteamAsync call fails, entityHandle with be null. Otherwise, the handle must be closed with PFEntityCloseHandle
/// when it is no longer needed. If returned, 'result' is a pointer within 'buffer' and does not need to be freed separately.
/// </remarks>
PF_API PFAuthenticationLoginWithSteamGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_opt_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

/// <summary>
/// Reauthenticates an existing PFEntityHandle. Used to address situations where the EntityToken expired and the PlayFab SDK is unable to refresh it.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to re-login.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation.
/// If successful, the cached EntityToken for the PFEntityHandle will be updated in place.
/// </remarks>
PF_API PFAuthenticationReLoginWithSteamAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationLoginWithSteamRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
/// <summary>
/// Signs the user in using the Android device identifier, returning a session identifier that can subsequently
/// be used for API calls which require an authenticated user
/// </summary>
/// <param name="serviceConfigHandle">PFServiceConfigHandle returned from PFServiceConfigCreateHandle call.</param>
/// <param name="secretKey">Title Secret Key used to authenticate the service request.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows.
/// On Android devices, the recommendation is to use the Settings.Secure.ANDROID_ID as the AndroidDeviceId,
/// as described in this blog post (http://android-developers.blogspot.com/2011/03/identifying-app-installations.html).
/// More information on this identifier can be found in the Android documentation (http://developer.android.com/reference/android/provider/Settings.Secure.html).
/// If this is the first time a user has signed in with the Android device and CreateAccount is set to
/// true, a new PlayFab account will be created and linked to the Android device ID. In this case, no
/// email or username will be associated with the PlayFab account. Otherwise, if no PlayFab account is
/// linked to the Android device, an error indicating this will be returned, so that the title can guide
/// the user through creation of a PlayFab account. Please note that while multiple devices of this type
/// can be linked to a single user account, only the one most recently used to login (or most recently
/// linked) will be reflected in the user's account information. We will be updating to show all linked
/// devices in a future release.
///
/// When the asynchronous task is complete, call <see cref="PFAuthenticationServerLoginWithAndroidDeviceIDGetResult"/>
/// to get the result.
/// </remarks>
PF_API PFAuthenticationServerLoginWithAndroidDeviceIDAsync(
    _In_ PFServiceConfigHandle serviceConfigHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationServerLoginWithAndroidDeviceIDRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a PFAuthenticationServerLoginWithAndroidDeviceIDAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED,
/// E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED or any of the global PlayFab Service errors. See doc
/// page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAuthenticationServerLoginWithAndroidDeviceIDGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Get the result from a PFAuthenticationServerLoginWithAndroidDeviceIDAsync call. The PFEntityHandle will always be returned, but the additional info
/// in the PFAuthenticationLoginResult is only returned if a buffer is provided.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="entityHandle">PFEntityHandle which can be used to authenticate other PlayFab API calls.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the Login result value and its fields.</param>
/// <param name="result">Pointer to the LoginResult object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED,
/// E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED or any of the global PlayFab Service errors. See doc
/// page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// If the PFAuthenticationServerLoginWithAndroidDeviceIDAsync call fails, entityHandle with be null. Otherwise, the handle must be closed with PFEntityCloseHandle
/// when it is no longer needed. If returned, 'result' is a pointer within 'buffer' and does not need to be freed separately.
/// </remarks>
PF_API PFAuthenticationServerLoginWithAndroidDeviceIDGetResult(
    _Inout_ XAsyncBlock* async,
    _Outptr_ PFAuthenticationEntityTokenResponse const** entityTokenResponse,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
/// <summary>
/// Sign in the user with a Battle.net identity token
/// </summary>
/// <param name="serviceConfigHandle">PFServiceConfigHandle returned from PFServiceConfigCreateHandle call.</param>
/// <param name="secretKey">Title Secret Key used to authenticate the service request.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows.
/// See also ServerLinkBattleNetAccountAsync, ServerUnlinkBattleNetAccountAsync.
///
/// When the asynchronous task is complete, call <see cref="PFAuthenticationServerLoginWithBattleNetGetResult"/>
/// to get the result.
/// </remarks>
PF_API PFAuthenticationServerLoginWithBattleNetAsync(
    _In_ PFServiceConfigHandle serviceConfigHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationServerLoginWithBattleNetRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a PFAuthenticationServerLoginWithBattleNetAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ACCOUNT_NOT_FOUND,
/// E_PF_BATTLE_NET_NOT_ENABLED_FOR_TITLE, E_PF_ENCRYPTION_KEY_MISSING, E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED,
/// E_PF_INVALID_AUTH_TOKEN, E_PF_INVALID_SIGNATURE, E_PF_INVALID_SIGNATURE_TIME, E_PF_INVALID_TITLE_ID,
/// E_PF_PLAYER_SECRET_ALREADY_CONFIGURED, E_PF_PLAYER_SECRET_NOT_CONFIGURED, E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
PF_API PFAuthenticationServerLoginWithBattleNetGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Get the result from a PFAuthenticationServerLoginWithBattleNetAsync call. The PFEntityHandle will always be returned, but the additional info
/// in the PFAuthenticationLoginResult is only returned if a buffer is provided.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="entityHandle">PFEntityHandle which can be used to authenticate other PlayFab API calls.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the Login result value and its fields.</param>
/// <param name="result">Pointer to the LoginResult object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ACCOUNT_NOT_FOUND,
/// E_PF_BATTLE_NET_NOT_ENABLED_FOR_TITLE, E_PF_ENCRYPTION_KEY_MISSING, E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED,
/// E_PF_INVALID_AUTH_TOKEN, E_PF_INVALID_SIGNATURE, E_PF_INVALID_SIGNATURE_TIME, E_PF_INVALID_TITLE_ID,
/// E_PF_PLAYER_SECRET_ALREADY_CONFIGURED, E_PF_PLAYER_SECRET_NOT_CONFIGURED, E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
/// <remarks>
/// If the PFAuthenticationServerLoginWithBattleNetAsync call fails, entityHandle with be null. Otherwise, the handle must be closed with PFEntityCloseHandle
/// when it is no longer needed. If returned, 'result' is a pointer within 'buffer' and does not need to be freed separately.
/// </remarks>
PF_API PFAuthenticationServerLoginWithBattleNetGetResult(
    _Inout_ XAsyncBlock* async,
    _Outptr_ PFAuthenticationEntityTokenResponse const** entityTokenResponse,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
/// <summary>
/// Signs the user in using a custom unique identifier generated by the title, returning a session identifier
/// that can subsequently be used for API calls which require an authenticated user
/// </summary>
/// <param name="serviceConfigHandle">PFServiceConfigHandle returned from PFServiceConfigCreateHandle call.</param>
/// <param name="secretKey">Title Secret Key used to authenticate the service request.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows.
/// It is highly recommended that developers ensure that it is extremely unlikely that a customer could
/// generate an ID which is already in use by another customer. If this is the first time a user has signed
/// in with the Custom ID and CreateAccount is set to true, a new PlayFab account will be created and
/// linked to the Custom ID. In this case, no email or username will be associated with the PlayFab account.
/// Otherwise, if no PlayFab account is linked to the Custom ID, an error indicating this will be returned,
/// so that the title can guide the user through creation of a PlayFab account.
///
/// When the asynchronous task is complete, call <see cref="PFAuthenticationServerLoginWithCustomIDGetResult"/>
/// to get the result.
/// </remarks>
PF_API PFAuthenticationServerLoginWithCustomIDAsync(
    _In_ PFServiceConfigHandle serviceConfigHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationServerLoginWithCustomIDRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a PFAuthenticationServerLoginWithCustomIDAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED,
/// E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED or any of the global PlayFab Service errors. See doc
/// page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAuthenticationServerLoginWithCustomIDGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Get the result from a PFAuthenticationServerLoginWithCustomIDAsync call. The PFEntityHandle will always be returned, but the additional info
/// in the PFAuthenticationLoginResult is only returned if a buffer is provided.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="entityHandle">PFEntityHandle which can be used to authenticate other PlayFab API calls.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the Login result value and its fields.</param>
/// <param name="result">Pointer to the LoginResult object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED,
/// E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED or any of the global PlayFab Service errors. See doc
/// page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// If the PFAuthenticationServerLoginWithCustomIDAsync call fails, entityHandle with be null. Otherwise, the handle must be closed with PFEntityCloseHandle
/// when it is no longer needed. If returned, 'result' is a pointer within 'buffer' and does not need to be freed separately.
/// </remarks>
PF_API PFAuthenticationServerLoginWithCustomIDGetResult(
    _Inout_ XAsyncBlock* async,
    _Outptr_ PFAuthenticationEntityTokenResponse const** entityTokenResponse,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
/// <summary>
/// Signs the user in using the iOS device identifier, returning a session identifier that can subsequently
/// be used for API calls which require an authenticated user
/// </summary>
/// <param name="serviceConfigHandle">PFServiceConfigHandle returned from PFServiceConfigCreateHandle call.</param>
/// <param name="secretKey">Title Secret Key used to authenticate the service request.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows.
/// On iOS devices, the identifierForVendor (https://developer.apple.com/library/ios/documentation/UIKit/Reference/UIDevice_Class/index.html#//apple_ref/occ/instp/UIDevice/identifierForVendor)
/// must be used as the DeviceId, as the UIDevice uniqueIdentifier has been deprecated as of iOS 5, and
/// use of the advertisingIdentifier for this purpose will result in failure of Apple's certification
/// process. If this is the first time a user has signed in with the iOS device and CreateAccount is set
/// to true, a new PlayFab account will be created and linked to the vendor-specific iOS device ID. In
/// this case, no email or username will be associated with the PlayFab account. Otherwise, if no PlayFab
/// account is linked to the iOS device, an error indicating this will be returned, so that the title
/// can guide the user through creation of a PlayFab account.
///
/// When the asynchronous task is complete, call <see cref="PFAuthenticationServerLoginWithIOSDeviceIDGetResult"/>
/// to get the result.
/// </remarks>
PF_API PFAuthenticationServerLoginWithIOSDeviceIDAsync(
    _In_ PFServiceConfigHandle serviceConfigHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationServerLoginWithIOSDeviceIDRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a PFAuthenticationServerLoginWithIOSDeviceIDAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED,
/// E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED or any of the global PlayFab Service errors. See doc
/// page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAuthenticationServerLoginWithIOSDeviceIDGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Get the result from a PFAuthenticationServerLoginWithIOSDeviceIDAsync call. The PFEntityHandle will always be returned, but the additional info
/// in the PFAuthenticationLoginResult is only returned if a buffer is provided.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="entityHandle">PFEntityHandle which can be used to authenticate other PlayFab API calls.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the Login result value and its fields.</param>
/// <param name="result">Pointer to the LoginResult object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED,
/// E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED or any of the global PlayFab Service errors. See doc
/// page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// If the PFAuthenticationServerLoginWithIOSDeviceIDAsync call fails, entityHandle with be null. Otherwise, the handle must be closed with PFEntityCloseHandle
/// when it is no longer needed. If returned, 'result' is a pointer within 'buffer' and does not need to be freed separately.
/// </remarks>
PF_API PFAuthenticationServerLoginWithIOSDeviceIDGetResult(
    _Inout_ XAsyncBlock* async,
    _Outptr_ PFAuthenticationEntityTokenResponse const** entityTokenResponse,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

#endif

#if HC_PLATFORM == HC_PLATFORM_GDK
/// <summary>
/// Signs the user in using a PlayStation :tm: Network authentication code, returning a session identifier
/// that can subsequently be used for API calls which require an authenticated user
/// </summary>
/// <param name="serviceConfigHandle">PFServiceConfigHandle returned from PFServiceConfigCreateHandle call.</param>
/// <param name="secretKey">Title Secret Key used to authenticate the service request.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows.
/// If this is the first time a user has signed in with the PlayStation :tm: Network account and CreateAccount
/// is set to true, a new PlayFab account will be created and linked to the PlayStation :tm: Network account.
/// In this case, no email or username will be associated with the PlayFab account. Otherwise, if no PlayFab
/// account is linked to the PlayStation :tm: Network account, an error indicating this will be returned,
/// so that the title can guide the user through creation of a PlayFab account. See also ServerLinkPSNAccountAsync,
/// ServerUnlinkPSNAccountAsync.
///
/// When the asynchronous task is complete, call <see cref="PFAuthenticationServerLoginWithPSNGetResult"/>
/// to get the result.
/// </remarks>
PF_API PFAuthenticationServerLoginWithPSNAsync(
    _In_ PFServiceConfigHandle serviceConfigHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationServerLoginWithPSNRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a PFAuthenticationServerLoginWithPSNAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ENCRYPTION_KEY_MISSING,
/// E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED, E_PF_FEATURE_NOT_CONFIGURED_FOR_TITLE, E_PF_INVALID_PSN_AUTH_CODE,
/// E_PF_INVALID_PSN_ISSUER_ID, E_PF_PLAYER_SECRET_ALREADY_CONFIGURED, E_PF_PLAYER_SECRET_NOT_CONFIGURED,
/// E_PF_PSN_INACCESSIBLE, E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED or any of the global PlayFab
/// Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAuthenticationServerLoginWithPSNGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Get the result from a PFAuthenticationServerLoginWithPSNAsync call. The PFEntityHandle will always be returned, but the additional info
/// in the PFAuthenticationLoginResult is only returned if a buffer is provided.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="entityHandle">PFEntityHandle which can be used to authenticate other PlayFab API calls.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the Login result value and its fields.</param>
/// <param name="result">Pointer to the LoginResult object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ENCRYPTION_KEY_MISSING,
/// E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED, E_PF_FEATURE_NOT_CONFIGURED_FOR_TITLE, E_PF_INVALID_PSN_AUTH_CODE,
/// E_PF_INVALID_PSN_ISSUER_ID, E_PF_PLAYER_SECRET_ALREADY_CONFIGURED, E_PF_PLAYER_SECRET_NOT_CONFIGURED,
/// E_PF_PSN_INACCESSIBLE, E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED or any of the global PlayFab
/// Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// If the PFAuthenticationServerLoginWithPSNAsync call fails, entityHandle with be null. Otherwise, the handle must be closed with PFEntityCloseHandle
/// when it is no longer needed. If returned, 'result' is a pointer within 'buffer' and does not need to be freed separately.
/// </remarks>
PF_API PFAuthenticationServerLoginWithPSNGetResult(
    _Inout_ XAsyncBlock* async,
    _Outptr_ PFAuthenticationEntityTokenResponse const** entityTokenResponse,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Securely login a game client from an external server backend using a custom identifier for that player.
/// Server Custom ID and Client Custom ID are mutually exclusive and cannot be used to retrieve the same
/// player account.
/// </summary>
/// <param name="serviceConfigHandle">PFServiceConfigHandle returned from PFServiceConfigCreateHandle call.</param>
/// <param name="secretKey">Title Secret Key used to authenticate the service request.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// When the asynchronous task is complete, call <see cref="PFAuthenticationServerLoginWithServerCustomIdGetResult"/>
/// to get the result.
/// </remarks>
PF_API PFAuthenticationServerLoginWithServerCustomIdAsync(
    _In_ PFServiceConfigHandle serviceConfigHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationLoginWithServerCustomIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a PFAuthenticationServerLoginWithServerCustomIdAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
PF_API PFAuthenticationServerLoginWithServerCustomIdGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Get the result from a PFAuthenticationServerLoginWithServerCustomIdAsync call. The PFEntityHandle will always be returned, but the additional info
/// in the PFAuthenticationLoginResult is only returned if a buffer is provided.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="entityHandle">PFEntityHandle which can be used to authenticate other PlayFab API calls.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the Login result value and its fields.</param>
/// <param name="result">Pointer to the LoginResult object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
/// <remarks>
/// If the PFAuthenticationServerLoginWithServerCustomIdAsync call fails, entityHandle with be null. Otherwise, the handle must be closed with PFEntityCloseHandle
/// when it is no longer needed. If returned, 'result' is a pointer within 'buffer' and does not need to be freed separately.
/// </remarks>
PF_API PFAuthenticationServerLoginWithServerCustomIdGetResult(
    _Inout_ XAsyncBlock* async,
    _Outptr_ PFAuthenticationEntityTokenResponse const** entityTokenResponse,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Signs the user in using an Steam ID, returning a session identifier that can subsequently be used
/// for API calls which require an authenticated user
/// </summary>
/// <param name="serviceConfigHandle">PFServiceConfigHandle returned from PFServiceConfigCreateHandle call.</param>
/// <param name="secretKey">Title Secret Key used to authenticate the service request.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// If this is the first time a user has signed in with the Steam ID and CreateAccount is set to true,
/// a new PlayFab account will be created and linked to the Steam account. In this case, no email or username
/// will be associated with the PlayFab account. Otherwise, if no PlayFab account is linked to the Steam
/// account, an error indicating this will be returned, so that the title can guide the user through creation
/// of a PlayFab account. Steam users that are not logged into the Steam Client app will only have their
/// Steam username synced, other data, such as currency and country will not be available until they login
/// while the Client is open.
///
/// When the asynchronous task is complete, call <see cref="PFAuthenticationServerLoginWithSteamIdGetResult"/>
/// to get the result.
/// </remarks>
PF_API PFAuthenticationServerLoginWithSteamIdAsync(
    _In_ PFServiceConfigHandle serviceConfigHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationLoginWithSteamIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a PFAuthenticationServerLoginWithSteamIdAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED,
/// E_PF_INVALID_SIGNATURE, E_PF_INVALID_SIGNATURE_TIME, E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED,
/// E_PF_STEAM_NOT_ENABLED_FOR_TITLE, E_PF_STEAM_USER_NOT_FOUND or any of the global PlayFab Service errors.
/// See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAuthenticationServerLoginWithSteamIdGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Get the result from a PFAuthenticationServerLoginWithSteamIdAsync call. The PFEntityHandle will always be returned, but the additional info
/// in the PFAuthenticationLoginResult is only returned if a buffer is provided.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="entityHandle">PFEntityHandle which can be used to authenticate other PlayFab API calls.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the Login result value and its fields.</param>
/// <param name="result">Pointer to the LoginResult object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED,
/// E_PF_INVALID_SIGNATURE, E_PF_INVALID_SIGNATURE_TIME, E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED,
/// E_PF_STEAM_NOT_ENABLED_FOR_TITLE, E_PF_STEAM_USER_NOT_FOUND or any of the global PlayFab Service errors.
/// See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// If the PFAuthenticationServerLoginWithSteamIdAsync call fails, entityHandle with be null. Otherwise, the handle must be closed with PFEntityCloseHandle
/// when it is no longer needed. If returned, 'result' is a pointer within 'buffer' and does not need to be freed separately.
/// </remarks>
PF_API PFAuthenticationServerLoginWithSteamIdGetResult(
    _Inout_ XAsyncBlock* async,
    _Outptr_ PFAuthenticationEntityTokenResponse const** entityTokenResponse,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Signs the user in using a Xbox Live Token from an external server backend, returning a session identifier
/// that can subsequently be used for API calls which require an authenticated user
/// </summary>
/// <param name="serviceConfigHandle">PFServiceConfigHandle returned from PFServiceConfigCreateHandle call.</param>
/// <param name="secretKey">Title Secret Key used to authenticate the service request.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// If this is the first time a user has signed in with the Xbox Live account and CreateAccount is set
/// to true, a new PlayFab account will be created and linked to the Xbox Live account. In this case,
/// no email or username will be associated with the PlayFab account. Otherwise, if no PlayFab account
/// is linked to the Xbox Live account, an error indicating this will be returned, so that the title can
/// guide the user through creation of a PlayFab account. See also ServerLinkXboxAccountAsync, ServerUnlinkXboxAccountAsync.
///
/// When the asynchronous task is complete, call <see cref="PFAuthenticationServerLoginWithXboxGetResult"/>
/// to get the result.
/// </remarks>
PF_API PFAuthenticationServerLoginWithXboxAsync(
    _In_ PFServiceConfigHandle serviceConfigHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationServerLoginWithXboxRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a PFAuthenticationServerLoginWithXboxAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED,
/// E_PF_EXPIRED_XBOX_LIVE_TOKEN, E_PF_INVALID_XBOX_LIVE_TOKEN, E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED,
/// E_PF_XBOX_INACCESSIBLE, E_PF_XBOX_REJECTED_XSTS_EXCHANGE_REQUEST, E_PF_XBOX_XASS_EXCHANGE_FAILURE
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
PF_API PFAuthenticationServerLoginWithXboxGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Get the result from a PFAuthenticationServerLoginWithXboxAsync call. The PFEntityHandle will always be returned, but the additional info
/// in the PFAuthenticationLoginResult is only returned if a buffer is provided.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="entityHandle">PFEntityHandle which can be used to authenticate other PlayFab API calls.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the Login result value and its fields.</param>
/// <param name="result">Pointer to the LoginResult object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED,
/// E_PF_EXPIRED_XBOX_LIVE_TOKEN, E_PF_INVALID_XBOX_LIVE_TOKEN, E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED,
/// E_PF_XBOX_INACCESSIBLE, E_PF_XBOX_REJECTED_XSTS_EXCHANGE_REQUEST, E_PF_XBOX_XASS_EXCHANGE_FAILURE
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
/// <remarks>
/// If the PFAuthenticationServerLoginWithXboxAsync call fails, entityHandle with be null. Otherwise, the handle must be closed with PFEntityCloseHandle
/// when it is no longer needed. If returned, 'result' is a pointer within 'buffer' and does not need to be freed separately.
/// </remarks>
PF_API PFAuthenticationServerLoginWithXboxGetResult(
    _Inout_ XAsyncBlock* async,
    _Outptr_ PFAuthenticationEntityTokenResponse const** entityTokenResponse,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Signs the user in using an Xbox ID and Sandbox ID, returning a session identifier that can subsequently
/// be used for API calls which require an authenticated user
/// </summary>
/// <param name="serviceConfigHandle">PFServiceConfigHandle returned from PFServiceConfigCreateHandle call.</param>
/// <param name="secretKey">Title Secret Key used to authenticate the service request.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// If this is the first time a user has signed in with the Xbox ID and CreateAccount is set to true,
/// a new PlayFab account will be created and linked to the Xbox Live account. In this case, no email
/// or username will be associated with the PlayFab account. Otherwise, if no PlayFab account is linked
/// to the Xbox Live account, an error indicating this will be returned, so that the title can guide the
/// user through creation of a PlayFab account.
///
/// When the asynchronous task is complete, call <see cref="PFAuthenticationServerLoginWithXboxIdGetResult"/>
/// to get the result.
/// </remarks>
PF_API PFAuthenticationServerLoginWithXboxIdAsync(
    _In_ PFServiceConfigHandle serviceConfigHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationLoginWithXboxIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a PFAuthenticationServerLoginWithXboxIdAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED,
/// E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED, E_PF_XBOX_INACCESSIBLE, E_PF_XBOX_REJECTED_XSTS_EXCHANGE_REQUEST,
/// E_PF_XBOX_XASS_EXCHANGE_FAILURE or any of the global PlayFab Service errors. See doc page "Handling
/// PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAuthenticationServerLoginWithXboxIdGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Get the result from a PFAuthenticationServerLoginWithXboxIdAsync call. The PFEntityHandle will always be returned, but the additional info
/// in the PFAuthenticationLoginResult is only returned if a buffer is provided.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="entityHandle">PFEntityHandle which can be used to authenticate other PlayFab API calls.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the Login result value and its fields.</param>
/// <param name="result">Pointer to the LoginResult object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_EVALUATION_MODE_PLAYER_COUNT_EXCEEDED,
/// E_PF_REQUEST_VIEW_CONSTRAINT_PARAMS_NOT_ALLOWED, E_PF_XBOX_INACCESSIBLE, E_PF_XBOX_REJECTED_XSTS_EXCHANGE_REQUEST,
/// E_PF_XBOX_XASS_EXCHANGE_FAILURE or any of the global PlayFab Service errors. See doc page "Handling
/// PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// If the PFAuthenticationServerLoginWithXboxIdAsync call fails, entityHandle with be null. Otherwise, the handle must be closed with PFEntityCloseHandle
/// when it is no longer needed. If returned, 'result' is a pointer within 'buffer' and does not need to be freed separately.
/// </remarks>
PF_API PFAuthenticationServerLoginWithXboxIdGetResult(
    _Inout_ XAsyncBlock* async,
    _Outptr_ PFAuthenticationEntityTokenResponse const** entityTokenResponse,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_opt_ PFAuthenticationLoginResult const** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Create a game_server entity token and return a new or existing game_server entity.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// Create or return a game_server entity token. Caller must be a title entity.
///
/// When the asynchronous task is complete, call <see cref="PFAuthenticationAuthenticateGameServerWithCustomIdGetResultSize"/>
/// and <see cref="PFAuthenticationAuthenticateGameServerWithCustomIdGetResult"/> to get the result.
/// </remarks>
PF_API PFAuthenticationAuthenticateGameServerWithCustomIdAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationAuthenticateCustomIdRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the result from a PFAuthenticationAuthenticateGameServerWithCustomIdAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="entityHandle">PFEntityHandle which can be used to authenticate other PlayFab API calls.</param>
/// <param name="newlyCreated">Will be set to true if the account was newly created on this authentication and false otherwise</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// If the PFAuthenticationAuthenticateGameServerWithCustomIdAsync call fails, entityHandle with be null. Otherwise, the handle must be closed with PFEntityCloseHandle
/// when it is no longer needed.
/// </remarks>
PF_API PFAuthenticationAuthenticateGameServerWithCustomIdGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle,
    _Out_opt_ bool* newlyCreated
) noexcept;

#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Delete a game_server entity.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// Delete a game_server entity. The caller can be the game_server entity attempting to delete itself.
/// Or a title entity attempting to delete game_server entities for this title.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be one of global PlayFab Service errors. See doc page "Handling PlayFab Errors"
/// for more details on error handling.
/// </remarks>
PF_API PFAuthenticationDeleteAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationDeleteRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Method to exchange a legacy AuthenticationTicket or title SecretKey for an Entity Token or to refresh
/// a still valid Entity Token.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// This API must be called with X-SecretKey, X-Authentication or X-EntityToken headers. An optional
/// EntityKey may be included to attempt to set the resulting EntityToken to a specific entity, however
/// the entity must be a relation of the caller, such as the master_player_account of a character. If
/// sending X-EntityToken the account will be marked as freshly logged in and will issue a new token.
/// If using X-Authentication or X-EntityToken the header must still be valid and cannot be expired or
/// revoked.
///
/// When the asynchronous task is complete, call <see cref="PFAuthenticationGetEntityGetResult"/> to
/// get the result.
/// </remarks>
PF_API PFAuthenticationGetEntityAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationGetEntityRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Gets the result of a successful PFAuthenticationGetEntityAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="entityHandle">PFEntityHandle which can be used to authenticate other PlayFab API calls.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ENCRYPTION_KEY_BROKEN,
/// E_PF_NO_VALID_CERTIFICATE_FOR_AAD or any of the global PlayFab Service errors. See doc page "Handling
/// PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// If the PFAuthenticationGetEntityAsync call fails, entityHandle with be null. Otherwise, the handle must be closed with PFEntityCloseHandle
/// when it is no longer needed.
/// </remarks>
PF_API PFAuthenticationGetEntityGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle
) noexcept;

#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Method to exchange a legacy AuthenticationTicket or title SecretKey for an Entity Token or to refresh
/// a still valid Entity Token.
/// </summary>
/// <param name="serviceConfigHandle">PFServiceConfigHandle returned from PFServiceConfigCreateHandle call.</param>
/// <param name="secretKey">Title Secret Key used to authenticate the service request.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// This API must be called with X-SecretKey, X-Authentication or X-EntityToken headers. An optional
/// EntityKey may be included to attempt to set the resulting EntityToken to a specific entity, however
/// the entity must be a relation of the caller, such as the master_player_account of a character. If
/// sending X-EntityToken the account will be marked as freshly logged in and will issue a new token.
/// If using X-Authentication or X-EntityToken the header must still be valid and cannot be expired or
/// revoked.
///
/// When the asynchronous task is complete, call <see cref="PFAuthenticationGetEntityWithSecretKeyGetResult"/>
/// to get the result.
/// </remarks>
PF_API PFAuthenticationGetEntityWithSecretKeyAsync(
    _In_ PFServiceConfigHandle serviceConfigHandle,
    _In_z_ const char* secretKey,
    _In_ const PFAuthenticationGetEntityRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Gets the result of a successful PFAuthenticationGetEntityWithSecretKeyAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="entityHandle">PFEntityHandle which can be used to authenticate other PlayFab API calls.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ENCRYPTION_KEY_BROKEN,
/// E_PF_NO_VALID_CERTIFICATE_FOR_AAD or any of the global PlayFab Service errors. See doc page "Handling
/// PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// If the PFAuthenticationGetEntityWithSecretKeyAsync call fails, entityHandle with be null. Otherwise, the handle must be closed with PFEntityCloseHandle
/// when it is no longer needed.
/// </remarks>
PF_API PFAuthenticationGetEntityWithSecretKeyGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEntityHandle* entityHandle
) noexcept;

#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Method for a server to validate a client provided EntityToken. Only callable by the title entity.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// Given an entity token, validates that it hasn't expired or been revoked and will return details of
/// the owner.
///
/// When the asynchronous task is complete, call <see cref="PFAuthenticationValidateEntityTokenGetResultSize"/>
/// and <see cref="PFAuthenticationValidateEntityTokenGetResult"/> to get the result.
/// </remarks>
PF_API PFAuthenticationValidateEntityTokenAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFAuthenticationValidateEntityTokenRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ValidateEntityToken call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFAuthenticationValidateEntityTokenGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFAuthenticationValidateEntityTokenAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFAuthenticationValidateEntityTokenGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFAuthenticationValidateEntityTokenResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

#endif

}

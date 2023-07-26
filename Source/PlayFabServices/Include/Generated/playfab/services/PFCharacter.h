// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFCharacterTypes.h>
#include <playfab/core/PFEntity.h>

extern "C"
{

#if 0
/// <summary>
/// Lists all of the characters that belong to a specific user. CharacterIds are not globally unique;
/// characterId must be evaluated with the parent PlayFabId to guarantee uniqueness.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// Returns a list of every character that currently belongs to a user. See also ClientGetCharacterDataAsync.
///
/// When the asynchronous task is complete, call <see cref="PFCharacterClientGetAllUsersCharactersGetResultSize"/>
/// and <see cref="PFCharacterClientGetAllUsersCharactersGetResult"/> to get the result.
/// </remarks>
PF_API PFCharacterClientGetAllUsersCharactersAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFCharacterListUsersCharactersRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetAllUsersCharacters call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFCharacterClientGetAllUsersCharactersGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFCharacterClientGetAllUsersCharactersAsync call.
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
PF_API PFCharacterClientGetAllUsersCharactersGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFCharacterListUsersCharactersResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if 0
/// <summary>
/// Retrieves the title-specific custom data for the character which is readable and writable by the
/// client
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// Data is stored as JSON key-value pairs. If the Keys parameter is provided, the data object returned
/// will only contain the data specific to the indicated Keys. Otherwise, the full set of custom character
/// data will be returned. See also ClientGetCharacterReadOnlyDataAsync, ClientGetUserDataAsync, ClientUpdateCharacterDataAsync.
///
/// When the asynchronous task is complete, call <see cref="PFCharacterClientGetCharacterDataGetResultSize"/>
/// and <see cref="PFCharacterClientGetCharacterDataGetResult"/> to get the result.
/// </remarks>
PF_API PFCharacterClientGetCharacterDataAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFCharacterGetCharacterDataRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetCharacterData call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_CHARACTER_NOT_FOUND,
/// E_PF_USERIS_NOT_VALID or any of the global PlayFab Service errors. See doc page "Handling PlayFab
/// Errors" for more details on error handling.
/// </returns>
PF_API PFCharacterClientGetCharacterDataGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFCharacterClientGetCharacterDataAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_CHARACTER_NOT_FOUND,
/// E_PF_USERIS_NOT_VALID or any of the global PlayFab Service errors. See doc page "Handling PlayFab
/// Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFCharacterClientGetCharacterDataGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFCharacterClientGetCharacterDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if 0
/// <summary>
/// Retrieves the title-specific custom data for the character which can only be read by the client
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// Data is stored as JSON key-value pairs. If the Keys parameter is provided, the data object returned
/// will only contain the data specific to the indicated Keys. Otherwise, the full set of custom character
/// data will be returned. See also ClientGetCharacterDataAsync, ClientGetUserDataAsync, ClientUpdateCharacterDataAsync.
///
/// When the asynchronous task is complete, call <see cref="PFCharacterClientGetCharacterReadOnlyDataGetResultSize"/>
/// and <see cref="PFCharacterClientGetCharacterReadOnlyDataGetResult"/> to get the result.
/// </remarks>
PF_API PFCharacterClientGetCharacterReadOnlyDataAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFCharacterGetCharacterDataRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetCharacterReadOnlyData call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_CHARACTER_NOT_FOUND,
/// E_PF_USERIS_NOT_VALID or any of the global PlayFab Service errors. See doc page "Handling PlayFab
/// Errors" for more details on error handling.
/// </returns>
PF_API PFCharacterClientGetCharacterReadOnlyDataGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFCharacterClientGetCharacterReadOnlyDataAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_CHARACTER_NOT_FOUND,
/// E_PF_USERIS_NOT_VALID or any of the global PlayFab Service errors. See doc page "Handling PlayFab
/// Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFCharacterClientGetCharacterReadOnlyDataGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFCharacterClientGetCharacterDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if 0
/// <summary>
/// Grants the specified character type to the user. CharacterIds are not globally unique; characterId
/// must be evaluated with the parent PlayFabId to guarantee uniqueness.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// Grants a character to the user of the type specified by the item ID. The user must already have an
/// instance of this item in their inventory in order to allow character creation. This item can come
/// from a purchase or grant, which must be done before calling to create the character.
///
/// When the asynchronous task is complete, call <see cref="PFCharacterClientGrantCharacterToUserGetResultSize"/>
/// and <see cref="PFCharacterClientGrantCharacterToUserGetResult"/> to get the result.
/// </remarks>
PF_API PFCharacterClientGrantCharacterToUserAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFCharacterClientGrantCharacterToUserRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGrantCharacterToUser call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_INVALID_ITEM_PROPERTIES,
/// E_PF_ITEM_NOT_OWNED or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors"
/// for more details on error handling.
/// </returns>
PF_API PFCharacterClientGrantCharacterToUserGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFCharacterClientGrantCharacterToUserAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_INVALID_ITEM_PROPERTIES,
/// E_PF_ITEM_NOT_OWNED or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors"
/// for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFCharacterClientGrantCharacterToUserGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFCharacterClientGrantCharacterToUserResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if 0
/// <summary>
/// Creates and updates the title-specific custom data for the user's character which is readable and
/// writable by the client
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This function performs an additive update of the arbitrary strings containing the custom data for
/// the character. In updating the custom data object, keys which already exist in the object will have
/// their values overwritten, while keys with null values will be removed. New keys will be added, with
/// the given values. No other key-value pairs will be changed apart from those specified in the call.
/// See also ClientGetCharacterDataAsync, ClientGetCharacterReadOnlyDataAsync, ClientGetUserDataAsync.
///
/// When the asynchronous task is complete, call <see cref="PFCharacterClientUpdateCharacterDataGetResult"/>
/// to get the result.
/// </remarks>
PF_API PFCharacterClientUpdateCharacterDataAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFCharacterClientUpdateCharacterDataRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Gets the result of a successful PFCharacterClientUpdateCharacterDataAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="result">PFCharacterUpdateCharacterDataResult object that will be populated with the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_CHARACTER_NOT_FOUND
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
PF_API PFCharacterClientUpdateCharacterDataGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFCharacterUpdateCharacterDataResult* result
) noexcept;
#endif

#if 0
/// <summary>
/// Deletes the specific character ID from the specified user.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This function will delete the specified character from the list allowed by the user, and will also
/// delete any inventory or VC currently held by that character. It will NOT delete any statistics associated
/// for this character, in order to preserve leaderboard integrity.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_CHARACTER_NOT_FOUND or any of the global PlayFab Service errors. See
/// doc page "Handling PlayFab Errors" for more details on error handling.
/// </remarks>
PF_API PFCharacterServerDeleteCharacterFromUserAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFCharacterDeleteCharacterFromUserRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

#if 0
/// <summary>
/// Lists all of the characters that belong to a specific user. CharacterIds are not globally unique;
/// characterId must be evaluated with the parent PlayFabId to guarantee uniqueness.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// Returns a list of every character that currently belongs to a user.
///
/// When the asynchronous task is complete, call <see cref="PFCharacterServerGetAllUsersCharactersGetResultSize"/>
/// and <see cref="PFCharacterServerGetAllUsersCharactersGetResult"/> to get the result.
/// </remarks>
PF_API PFCharacterServerGetAllUsersCharactersAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFCharacterListUsersCharactersRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetAllUsersCharacters call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFCharacterServerGetAllUsersCharactersGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFCharacterServerGetAllUsersCharactersAsync call.
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
PF_API PFCharacterServerGetAllUsersCharactersGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFCharacterListUsersCharactersResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if 0
/// <summary>
/// Retrieves the title-specific custom data for the user which is readable and writable by the client
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// Data is stored as JSON key-value pairs. If the Keys parameter is provided, the data object returned
/// will only contain the data specific to the indicated Keys. Otherwise, the full set of custom user
/// data will be returned. See also ServerGetUserDataAsync, ServerUpdateCharacterDataAsync, ServerUpdateCharacterInternalDataAsync,
/// ServerUpdateCharacterReadOnlyDataAsync.
///
/// When the asynchronous task is complete, call <see cref="PFCharacterServerGetCharacterDataGetResultSize"/>
/// and <see cref="PFCharacterServerGetCharacterDataGetResult"/> to get the result.
/// </remarks>
PF_API PFCharacterServerGetCharacterDataAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFCharacterGetCharacterDataRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetCharacterData call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_CHARACTER_NOT_FOUND,
/// E_PF_USERIS_NOT_VALID or any of the global PlayFab Service errors. See doc page "Handling PlayFab
/// Errors" for more details on error handling.
/// </returns>
PF_API PFCharacterServerGetCharacterDataGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFCharacterServerGetCharacterDataAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_CHARACTER_NOT_FOUND,
/// E_PF_USERIS_NOT_VALID or any of the global PlayFab Service errors. See doc page "Handling PlayFab
/// Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFCharacterServerGetCharacterDataGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFCharacterServerGetCharacterDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if 0
/// <summary>
/// Retrieves the title-specific custom data for the user's character which cannot be accessed by the
/// client
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// Data is stored as JSON key-value pairs. If the Keys parameter is provided, the data object returned
/// will only contain the data specific to the indicated Keys. Otherwise, the full set of custom user
/// data will be returned. See also ServerGetUserInternalDataAsync, ServerUpdateCharacterDataAsync, ServerUpdateCharacterInternalDataAsync,
/// ServerUpdateCharacterReadOnlyDataAsync.
///
/// When the asynchronous task is complete, call <see cref="PFCharacterServerGetCharacterInternalDataGetResultSize"/>
/// and <see cref="PFCharacterServerGetCharacterInternalDataGetResult"/> to get the result.
/// </remarks>
PF_API PFCharacterServerGetCharacterInternalDataAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFCharacterGetCharacterDataRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetCharacterInternalData call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_CHARACTER_NOT_FOUND,
/// E_PF_USERIS_NOT_VALID or any of the global PlayFab Service errors. See doc page "Handling PlayFab
/// Errors" for more details on error handling.
/// </returns>
PF_API PFCharacterServerGetCharacterInternalDataGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFCharacterServerGetCharacterInternalDataAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_CHARACTER_NOT_FOUND,
/// E_PF_USERIS_NOT_VALID or any of the global PlayFab Service errors. See doc page "Handling PlayFab
/// Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFCharacterServerGetCharacterInternalDataGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFCharacterServerGetCharacterDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if 0
/// <summary>
/// Retrieves the title-specific custom data for the user's character which can only be read by the client
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// Data is stored as JSON key-value pairs. If the Keys parameter is provided, the data object returned
/// will only contain the data specific to the indicated Keys. Otherwise, the full set of custom data
/// will be returned. See also ServerGetCharacterDataAsync, ServerGetCharacterInternalDataAsync, ServerGetUserReadOnlyDataAsync,
/// ServerUpdateCharacterReadOnlyDataAsync.
///
/// When the asynchronous task is complete, call <see cref="PFCharacterServerGetCharacterReadOnlyDataGetResultSize"/>
/// and <see cref="PFCharacterServerGetCharacterReadOnlyDataGetResult"/> to get the result.
/// </remarks>
PF_API PFCharacterServerGetCharacterReadOnlyDataAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFCharacterGetCharacterDataRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetCharacterReadOnlyData call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_CHARACTER_NOT_FOUND,
/// E_PF_USERIS_NOT_VALID or any of the global PlayFab Service errors. See doc page "Handling PlayFab
/// Errors" for more details on error handling.
/// </returns>
PF_API PFCharacterServerGetCharacterReadOnlyDataGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFCharacterServerGetCharacterReadOnlyDataAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_CHARACTER_NOT_FOUND,
/// E_PF_USERIS_NOT_VALID or any of the global PlayFab Service errors. See doc page "Handling PlayFab
/// Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFCharacterServerGetCharacterReadOnlyDataGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFCharacterServerGetCharacterDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if 0
/// <summary>
/// Grants the specified character type to the user. CharacterIds are not globally unique; characterId
/// must be evaluated with the parent PlayFabId to guarantee uniqueness.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// Grants a character to the user of the type and name specified in the request.
///
/// When the asynchronous task is complete, call <see cref="PFCharacterServerGrantCharacterToUserGetResultSize"/>
/// and <see cref="PFCharacterServerGrantCharacterToUserGetResult"/> to get the result.
/// </remarks>
PF_API PFCharacterServerGrantCharacterToUserAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFCharacterServerGrantCharacterToUserRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGrantCharacterToUser call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFCharacterServerGrantCharacterToUserGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFCharacterServerGrantCharacterToUserAsync call.
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
PF_API PFCharacterServerGrantCharacterToUserGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFCharacterServerGrantCharacterToUserResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if 0
/// <summary>
/// Updates the title-specific custom data for the user's character which is readable and writable by
/// the client
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This function performs an additive update of the arbitrary JSON object containing the custom data
/// for the user. In updating the custom data object, keys which already exist in the object will have
/// their values overwritten, while keys with null values will be removed. No other key-value pairs will
/// be changed apart from those specified in the call. See also ServerGetCharacterDataAsync, ServerUpdateCharacterInternalDataAsync,
/// ServerUpdateCharacterReadOnlyDataAsync, ServerUpdateUserDataAsync.
///
/// When the asynchronous task is complete, call <see cref="PFCharacterServerUpdateCharacterDataGetResult"/>
/// to get the result.
/// </remarks>
PF_API PFCharacterServerUpdateCharacterDataAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFCharacterServerUpdateCharacterDataRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Gets the result of a successful PFCharacterServerUpdateCharacterDataAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="result">PFCharacterUpdateCharacterDataResult object that will be populated with the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_CHARACTER_NOT_FOUND,
/// E_PF_USERIS_NOT_VALID or any of the global PlayFab Service errors. See doc page "Handling PlayFab
/// Errors" for more details on error handling.
/// </returns>
PF_API PFCharacterServerUpdateCharacterDataGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFCharacterUpdateCharacterDataResult* result
) noexcept;
#endif

#if 0
/// <summary>
/// Updates the title-specific custom data for the user's character which cannot be accessed by the client
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This function performs an additive update of the arbitrary JSON object containing the custom data
/// for the user. In updating the custom data object, keys which already exist in the object will have
/// their values overwritten, keys with null values will be removed. No other key-value pairs will be
/// changed apart from those specified in the call. See also ServerGetCharacterInternalDataAsync, ServerUpdateCharacterDataAsync,
/// ServerUpdateCharacterReadOnlyDataAsync, ServerUpdateUserInternalDataAsync.
///
/// When the asynchronous task is complete, call <see cref="PFCharacterServerUpdateCharacterInternalDataGetResult"/>
/// to get the result.
/// </remarks>
PF_API PFCharacterServerUpdateCharacterInternalDataAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFCharacterServerUpdateCharacterDataRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Gets the result of a successful PFCharacterServerUpdateCharacterInternalDataAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="result">PFCharacterUpdateCharacterDataResult object that will be populated with the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_CHARACTER_NOT_FOUND,
/// E_PF_USERIS_NOT_VALID or any of the global PlayFab Service errors. See doc page "Handling PlayFab
/// Errors" for more details on error handling.
/// </returns>
PF_API PFCharacterServerUpdateCharacterInternalDataGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFCharacterUpdateCharacterDataResult* result
) noexcept;
#endif

#if 0
/// <summary>
/// Updates the title-specific custom data for the user's character which can only be read by the client
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This function performs an additive update of the arbitrary JSON object containing the custom data
/// for the user. In updating the custom data object, keys which already exist in the object will have
/// their values overwritten, keys with null values will be removed. No other key-value pairs will be
/// changed apart from those specified in the call. See also ServerGetCharacterDataAsync, ServerGetCharacterInternalDataAsync,
/// ServerGetCharacterReadOnlyDataAsync, ServerGetUserReadOnlyDataAsync.
///
/// When the asynchronous task is complete, call <see cref="PFCharacterServerUpdateCharacterReadOnlyDataGetResult"/>
/// to get the result.
/// </remarks>
PF_API PFCharacterServerUpdateCharacterReadOnlyDataAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFCharacterServerUpdateCharacterDataRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Gets the result of a successful PFCharacterServerUpdateCharacterReadOnlyDataAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="result">PFCharacterUpdateCharacterDataResult object that will be populated with the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_CHARACTER_NOT_FOUND,
/// E_PF_USERIS_NOT_VALID or any of the global PlayFab Service errors. See doc page "Handling PlayFab
/// Errors" for more details on error handling.
/// </returns>
PF_API PFCharacterServerUpdateCharacterReadOnlyDataGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFCharacterUpdateCharacterDataResult* result
) noexcept;
#endif


}
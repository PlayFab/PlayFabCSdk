// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFPlayerDataManagementTypes.h>
#include <playfab/core/PFEntity.h>

extern "C"
{

/// <summary>
/// Retrieves the title-specific custom data for the user which is readable and writable by the client
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// Data is stored as JSON key-value pairs. Every time the data is updated via any source, the version
/// counter is incremented. If the Version parameter is provided, then this call will only return data
/// if the current version on the system is greater than the value provided. If the Keys parameter is
/// provided, the data object returned will only contain the data specific to the indicated Keys. Otherwise,
/// the full set of custom user data will be returned. See also ClientGetUserReadOnlyDataAsync, ClientUpdateUserDataAsync.
///
/// When the asynchronous task is complete, call <see cref="PFPlayerDataManagementClientGetUserDataGetResultSize"/>
/// and <see cref="PFPlayerDataManagementClientGetUserDataGetResult"/> to get the result.
/// </remarks>
PF_API PFPlayerDataManagementClientGetUserDataAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetUserData call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFPlayerDataManagementClientGetUserDataGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFPlayerDataManagementClientGetUserDataAsync call.
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
PF_API PFPlayerDataManagementClientGetUserDataGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementClientGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

/// <summary>
/// Retrieves the publisher-specific custom data for the user which is readable and writable by the client
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// Data is stored as JSON key-value pairs. If the Keys parameter is provided, the data object returned
/// will only contain the data specific to the indicated Keys. Otherwise, the full set of custom user
/// data will be returned. See also ClientGetUserPublisherReadOnlyDataAsync, ClientUpdateUserPublisherDataAsync.
///
/// When the asynchronous task is complete, call <see cref="PFPlayerDataManagementClientGetUserPublisherDataGetResultSize"/>
/// and <see cref="PFPlayerDataManagementClientGetUserPublisherDataGetResult"/> to get the result.
/// </remarks>
PF_API PFPlayerDataManagementClientGetUserPublisherDataAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetUserPublisherData call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_PUBLISHER_NOT_SET
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
PF_API PFPlayerDataManagementClientGetUserPublisherDataGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFPlayerDataManagementClientGetUserPublisherDataAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_PUBLISHER_NOT_SET
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFPlayerDataManagementClientGetUserPublisherDataGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementClientGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

/// <summary>
/// Retrieves the publisher-specific custom data for the user which can only be read by the client
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// Data is stored as JSON key-value pairs. If the Keys parameter is provided, the data object returned
/// will only contain the data specific to the indicated Keys. Otherwise, the full set of custom user
/// data will be returned. See also ClientGetUserPublisherDataAsync, ClientUpdateUserPublisherDataAsync.
///
/// When the asynchronous task is complete, call <see cref="PFPlayerDataManagementClientGetUserPublisherReadOnlyDataGetResultSize"/>
/// and <see cref="PFPlayerDataManagementClientGetUserPublisherReadOnlyDataGetResult"/> to get the result.
/// </remarks>
PF_API PFPlayerDataManagementClientGetUserPublisherReadOnlyDataAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetUserPublisherReadOnlyData call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_PUBLISHER_NOT_SET
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
PF_API PFPlayerDataManagementClientGetUserPublisherReadOnlyDataGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFPlayerDataManagementClientGetUserPublisherReadOnlyDataAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_PUBLISHER_NOT_SET
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFPlayerDataManagementClientGetUserPublisherReadOnlyDataGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementClientGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

/// <summary>
/// Retrieves the title-specific custom data for the user which can only be read by the client
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// Data is stored as JSON key-value pairs. Every time the data is updated via any source, the version
/// counter is incremented. If the Version parameter is provided, then this call will only return data
/// if the current version on the system is greater than the value provided. If the Keys parameter is
/// provided, the data object returned will only contain the data specific to the indicated Keys. Otherwise,
/// the full set of custom user data will be returned. See also ClientGetUserDataAsync, ClientUpdateUserDataAsync.
///
/// When the asynchronous task is complete, call <see cref="PFPlayerDataManagementClientGetUserReadOnlyDataGetResultSize"/>
/// and <see cref="PFPlayerDataManagementClientGetUserReadOnlyDataGetResult"/> to get the result.
/// </remarks>
PF_API PFPlayerDataManagementClientGetUserReadOnlyDataAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFPlayerDataManagementGetUserDataRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetUserReadOnlyData call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFPlayerDataManagementClientGetUserReadOnlyDataGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFPlayerDataManagementClientGetUserReadOnlyDataAsync call.
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
PF_API PFPlayerDataManagementClientGetUserReadOnlyDataGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayerDataManagementClientGetUserDataResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

/// <summary>
/// Creates and updates the title-specific custom data for the user which is readable and writable by
/// the client
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// This function performs an additive update of the arbitrary strings containing the custom data for
/// the user. In updating the custom data object, keys which already exist in the object will have their
/// values overwritten, while keys with null values will be removed. New keys will be added, with the
/// given values. No other key-value pairs will be changed apart from those specified in the call. See
/// also ClientGetUserDataAsync, ClientGetUserReadOnlyDataAsync.
///
/// When the asynchronous task is complete, call <see cref="PFPlayerDataManagementClientUpdateUserDataGetResult"/>
/// to get the result.
/// </remarks>
PF_API PFPlayerDataManagementClientUpdateUserDataAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFPlayerDataManagementClientUpdateUserDataRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Gets the result of a successful PFPlayerDataManagementClientUpdateUserDataAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="result">PFPlayerDataManagementUpdateUserDataResult object that will be populated with the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFPlayerDataManagementClientUpdateUserDataGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFPlayerDataManagementUpdateUserDataResult* result
) noexcept;

/// <summary>
/// Creates and updates the publisher-specific custom data for the user which is readable and writable
/// by the client
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// This function performs an additive update of the arbitrary strings containing the custom data for
/// the user. In updating the custom data object, keys which already exist in the object will have their
/// values overwritten, while keys with null values will be removed. New keys will be added, with the
/// given values. No other key-value pairs will be changed apart from those specified in the call. See
/// also ClientGetUserPublisherDataAsync, ClientGetUserPublisherReadOnlyDataAsync.
///
/// When the asynchronous task is complete, call <see cref="PFPlayerDataManagementClientUpdateUserPublisherDataGetResult"/>
/// to get the result.
/// </remarks>
PF_API PFPlayerDataManagementClientUpdateUserPublisherDataAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFPlayerDataManagementClientUpdateUserDataRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Gets the result of a successful PFPlayerDataManagementClientUpdateUserPublisherDataAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="result">PFPlayerDataManagementUpdateUserDataResult object that will be populated with the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_PUBLISHER_NOT_SET
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
PF_API PFPlayerDataManagementClientUpdateUserPublisherDataGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFPlayerDataManagementUpdateUserDataResult* result
) noexcept;


}
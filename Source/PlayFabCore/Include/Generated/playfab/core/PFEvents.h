// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/PFEventsTypes.h>
#include <playfab/core/PFServiceConfig.h>
#include <playfab/core/PFEntity.h>

extern "C"
{

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
/// <summary>
/// Creates a new telemetry key for the title.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Win32, Nintendo Switch, and Sony PlayStation®.
/// When the asynchronous task is complete, call <see cref="PFEventsCreateTelemetryKeyGetResultSize"/>
/// and <see cref="PFEventsCreateTelemetryKeyGetResult"/> to get the result.
/// </remarks>
PF_API PFEventsCreateTelemetryKeyAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFEventsCreateTelemetryKeyRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a CreateTelemetryKey call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_TELEMETRY_KEY_ALREADY_EXISTS,
/// E_PF_TELEMETRY_KEY_INVALID_NAME, E_PF_TELEMETRY_KEY_LONG_INSIGHTS_RETENTION_NOT_ALLOWED or any of
/// the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error
/// handling.
/// </returns>
PF_API PFEventsCreateTelemetryKeyGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFEventsCreateTelemetryKeyAsync call.
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
PF_API PFEventsCreateTelemetryKeyGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFEventsCreateTelemetryKeyResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
/// <summary>
/// Deletes a telemetry key configured for the title.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Win32, Nintendo Switch, and Sony PlayStation®.
/// When the asynchronous task is complete, call <see cref="PFEventsDeleteTelemetryKeyGetResult"/> to
/// get the result.
/// </remarks>
PF_API PFEventsDeleteTelemetryKeyAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFEventsDeleteTelemetryKeyRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Gets the result of a successful PFEventsDeleteTelemetryKeyAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="result">PFEventsDeleteTelemetryKeyResponse object that will be populated with the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFEventsDeleteTelemetryKeyGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFEventsDeleteTelemetryKeyResponse* result
) noexcept;

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
/// <summary>
/// Gets information about a telemetry key configured for the title.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Win32, Nintendo Switch, and Sony PlayStation®.
/// When the asynchronous task is complete, call <see cref="PFEventsGetTelemetryKeyGetResultSize"/> and
/// <see cref="PFEventsGetTelemetryKeyGetResult"/> to get the result.
/// </remarks>
PF_API PFEventsGetTelemetryKeyAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFEventsGetTelemetryKeyRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a GetTelemetryKey call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_TELEMETRY_KEY_NOT_FOUND
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
PF_API PFEventsGetTelemetryKeyGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFEventsGetTelemetryKeyAsync call.
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
PF_API PFEventsGetTelemetryKeyGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFEventsGetTelemetryKeyResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
/// <summary>
/// Lists all telemetry keys configured for the title.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Win32, Nintendo Switch, and Sony PlayStation®.
/// When the asynchronous task is complete, call <see cref="PFEventsListTelemetryKeysGetResultSize"/>
/// and <see cref="PFEventsListTelemetryKeysGetResult"/> to get the result.
/// </remarks>
PF_API PFEventsListTelemetryKeysAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFEventsListTelemetryKeysRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ListTelemetryKeys call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFEventsListTelemetryKeysGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFEventsListTelemetryKeysAsync call.
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
PF_API PFEventsListTelemetryKeysGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFEventsListTelemetryKeysResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

#endif

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_NINTENDO_SWITCH || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_4 || HC_PLATFORM == HC_PLATFORM_SONY_PLAYSTATION_5
/// <summary>
/// Sets a telemetry key to the active or deactivated state.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Win32, Nintendo Switch, and Sony PlayStation®.
/// When the asynchronous task is complete, call <see cref="PFEventsSetTelemetryKeyActiveGetResultSize"/>
/// and <see cref="PFEventsSetTelemetryKeyActiveGetResult"/> to get the result.
/// </remarks>
PF_API PFEventsSetTelemetryKeyActiveAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFEventsSetTelemetryKeyActiveRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a SetTelemetryKeyActive call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_TELEMETRY_KEY_NOT_FOUND
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
PF_API PFEventsSetTelemetryKeyActiveGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFEventsSetTelemetryKeyActiveAsync call.
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
PF_API PFEventsSetTelemetryKeyActiveGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFEventsSetTelemetryKeyActiveResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

#endif

/// <summary>
/// Write batches of entity based events to PlayStream. The namespace of the Event must be 'custom' or
/// start with 'custom.'.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// When the asynchronous task is complete, call <see cref="PFEventsWriteEventsGetResultSize"/> and <see
/// cref="PFEventsWriteEventsGetResult"/> to get the result.
/// </remarks>
PF_API PFEventsWriteEventsAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFEventsWriteEventsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a WriteEvents call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ENCRYPTION_KEY_BROKEN,
/// E_PF_EVENT_ENTITY_NOT_ALLOWED, E_PF_EVENT_NAMESPACE_NOT_ALLOWED, E_PF_INVALID_EVENT_CONTENTS, E_PF_INVALID_JSON_CONTENT,
/// E_PF_PER_ENTITY_EVENT_RATE_LIMIT_EXCEEDED or any of the global PlayFab Service errors. See doc page
/// "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFEventsWriteEventsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFEventsWriteEventsAsync call.
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
PF_API PFEventsWriteEventsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFEventsWriteEventsResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;


/// <summary>
/// Write batches of entity based events to as Telemetry events (bypass PlayStream). The namespace must
/// be 'custom' or start with 'custom.'
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// This API must be called with either X-EntityToken or X-TelemetryKey headers. If sending X-EntityToken
/// the value must be a valid EntityToken for your title. If using X-TelemetryKey the value must be a
/// Telemetry Key configured for your title set to 'Active'. If both are provided, X-TelemetryKey will
/// be ignored.
///
/// When the asynchronous task is complete, call <see cref="PFEventsWriteTelemetryEventsGetResultSize"/>
/// and <see cref="PFEventsWriteTelemetryEventsGetResult"/> to get the result.
/// </remarks>
PF_API PFEventsWriteTelemetryEventsAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFEventsWriteEventsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a WriteTelemetryEvents call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ENCRYPTION_KEY_BROKEN,
/// E_PF_EVENT_ENTITY_NOT_ALLOWED, E_PF_EVENT_NAMESPACE_NOT_ALLOWED, E_PF_INVALID_JSON_CONTENT, E_PF_PER_ENTITY_EVENT_RATE_LIMIT_EXCEEDED,
/// E_PF_TELEMETRY_KEY_DEACTIVATED, E_PF_TELEMETRY_KEY_INVALID or any of the global PlayFab Service errors.
/// See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFEventsWriteTelemetryEventsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFEventsWriteTelemetryEventsAsync call.
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
PF_API PFEventsWriteTelemetryEventsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFEventsWriteEventsResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;


}

// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFSegmentsTypes.h>
#include <playfab/core/PFEntity.h>

extern "C"
{

/// <summary>
/// List all segments that a player currently belongs to at this moment in time.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// When the asynchronous task is complete, call <see cref="PFSegmentsClientGetPlayerSegmentsGetResultSize"/>
/// and <see cref="PFSegmentsClientGetPlayerSegmentsGetResult"/> to get the result.
/// </remarks>
PF_API PFSegmentsClientGetPlayerSegmentsAsync(
    _In_ PFEntityHandle entityHandle,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetPlayerSegments call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_PRODUCT_DISABLED_FOR_TITLE
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
PF_API PFSegmentsClientGetPlayerSegmentsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFSegmentsClientGetPlayerSegmentsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_PRODUCT_DISABLED_FOR_TITLE
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFSegmentsClientGetPlayerSegmentsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFSegmentsGetPlayerSegmentsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

/// <summary>
/// Get all tags with a given Namespace (optional) from a player profile.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// This API will return a list of canonical tags which includes both namespace and tag's name. If namespace
/// is not provided, the result is a list of all canonical tags. TagName can be used for segmentation
/// and Namespace is limited to 128 characters.
///
/// When the asynchronous task is complete, call <see cref="PFSegmentsClientGetPlayerTagsGetResultSize"/>
/// and <see cref="PFSegmentsClientGetPlayerTagsGetResult"/> to get the result.
/// </remarks>
PF_API PFSegmentsClientGetPlayerTagsAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFSegmentsGetPlayerTagsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ClientGetPlayerTags call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFSegmentsClientGetPlayerTagsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFSegmentsClientGetPlayerTagsAsync call.
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
PF_API PFSegmentsClientGetPlayerTagsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFSegmentsGetPlayerTagsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Adds a given tag to a player profile. The tag's namespace is automatically generated based on the
/// source of the tag.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// This API will trigger a player_tag_added event and add a tag with the given TagName and PlayFabID
/// to the corresponding player profile. TagName can be used for segmentation and it is limited to 256
/// characters. Also there is a limit on the number of tags a title can have. See also ServerGetPlayerTagsAsync,
/// ServerRemovePlayerTagAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be E_PF_PLAYER_TAG_COUNT_LIMIT_EXCEEDED or any of the global PlayFab Service
/// errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </remarks>
PF_API PFSegmentsServerAddPlayerTagAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFSegmentsAddPlayerTagRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Retrieves an array of player segment definitions. Results from this can be used in subsequent API
/// calls such as ExportPlayersInSegment which requires a Segment ID. While segment names can change the
/// ID for that segment will not change.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// Request has no paramaters. See also ServerExportPlayersInSegmentAsync, ServerGetSegmentExportAsync.
///
/// When the asynchronous task is complete, call <see cref="PFSegmentsServerGetAllSegmentsGetResultSize"/>
/// and <see cref="PFSegmentsServerGetAllSegmentsGetResult"/> to get the result.
/// </remarks>
PF_API PFSegmentsServerGetAllSegmentsAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetAllSegments call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_PRODUCT_DISABLED_FOR_TITLE
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
PF_API PFSegmentsServerGetAllSegmentsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFSegmentsServerGetAllSegmentsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_PRODUCT_DISABLED_FOR_TITLE
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFSegmentsServerGetAllSegmentsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFSegmentsGetAllSegmentsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// List all segments that a player currently belongs to at this moment in time.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// See also ServerGetAllSegmentsAsync.
///
/// When the asynchronous task is complete, call <see cref="PFSegmentsServerGetPlayerSegmentsGetResultSize"/>
/// and <see cref="PFSegmentsServerGetPlayerSegmentsGetResult"/> to get the result.
/// </remarks>
PF_API PFSegmentsServerGetPlayerSegmentsAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFSegmentsGetPlayersSegmentsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetPlayerSegments call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_PRODUCT_DISABLED_FOR_TITLE
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
PF_API PFSegmentsServerGetPlayerSegmentsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFSegmentsServerGetPlayerSegmentsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_PRODUCT_DISABLED_FOR_TITLE
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFSegmentsServerGetPlayerSegmentsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFSegmentsGetPlayerSegmentsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Get all tags with a given Namespace (optional) from a player profile.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// This API will return a list of canonical tags which includes both namespace and tag's name. If namespace
/// is not provided, the result is a list of all canonical tags. TagName can be used for segmentation
/// and Namespace is limited to 128 characters. See also ServerAddPlayerTagAsync, ServerRemovePlayerTagAsync.
///
/// When the asynchronous task is complete, call <see cref="PFSegmentsServerGetPlayerTagsGetResultSize"/>
/// and <see cref="PFSegmentsServerGetPlayerTagsGetResult"/> to get the result.
/// </remarks>
PF_API PFSegmentsServerGetPlayerTagsAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFSegmentsGetPlayerTagsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetPlayerTags call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFSegmentsServerGetPlayerTagsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFSegmentsServerGetPlayerTagsAsync call.
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
PF_API PFSegmentsServerGetPlayerTagsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFSegmentsGetPlayerTagsResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Remove a given tag from a player profile. The tag's namespace is automatically generated based on
/// the source of the tag.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// This API will trigger a player_tag_removed event and remove a tag with the given TagName and PlayFabID
/// from the corresponding player profile. TagName can be used for segmentation and it is limited to 256
/// characters See also ServerAddPlayerTagAsync, ServerGetPlayerTagsAsync.
///
/// Call <see cref="XAsyncGetStatus"/> to get the status of the operation. If the service call is unsuccessful,
/// the async result will be one of global PlayFab Service errors. See doc page "Handling PlayFab Errors"
/// for more details on error handling.
/// </remarks>
PF_API PFSegmentsServerRemovePlayerTagAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFSegmentsRemovePlayerTagRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;
#endif


}
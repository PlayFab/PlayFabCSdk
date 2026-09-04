// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFPlayStreamTypes.h>
#include <playfab/core/PFEntity.h>

extern "C"
{

#if 0
/// <summary>
/// Starts an export for the player profiles in a segment. This API creates a snapshot of all the player
/// profiles which match the segment definition at the time of the API call. Profiles which change while
/// an export is in progress will not be reflected in the results.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// Request must contain the Segment ID See also ServerGetAllSegmentsAsync, ServerGetSegmentExportAsync.
///
/// When the asynchronous task is complete, call <see cref="PFPlayStreamServerExportPlayersInSegmentGetResultSize"/>
/// and <see cref="PFPlayStreamServerExportPlayersInSegmentGetResult"/> to get the result.
/// </remarks>
PF_API PFPlayStreamServerExportPlayersInSegmentAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFPlayStreamExportPlayersInSegmentRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerExportPlayersInSegment call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ASYNC_EXPORT_NOT_IN_FLIGHT,
/// E_PF_ASYNC_EXPORT_RATE_LIMIT_EXCEEDED, E_PF_EXPORT_UNKNOWN_ERROR, E_PF_INVALID_SEGMENT, E_PF_PRODUCT_DISABLED_FOR_TITLE,
/// E_PF_SEGMENT_NOT_FOUND or any of the global PlayFab Service errors. See doc page "Handling PlayFab
/// Errors" for more details on error handling.
/// </returns>
PF_API PFPlayStreamServerExportPlayersInSegmentGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFPlayStreamServerExportPlayersInSegmentAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ASYNC_EXPORT_NOT_IN_FLIGHT,
/// E_PF_ASYNC_EXPORT_RATE_LIMIT_EXCEEDED, E_PF_EXPORT_UNKNOWN_ERROR, E_PF_INVALID_SEGMENT, E_PF_PRODUCT_DISABLED_FOR_TITLE,
/// E_PF_SEGMENT_NOT_FOUND or any of the global PlayFab Service errors. See doc page "Handling PlayFab
/// Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFPlayStreamServerExportPlayersInSegmentGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayStreamExportPlayersInSegmentResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if 0
/// <summary>
/// Retrieves the result of an export started by ExportPlayersInSegment API. If the ExportPlayersInSegment
/// is successful and complete, this API returns the IndexUrl from which the index file can be downloaded.
/// The index file has a list of urls from which the files containing the player profile data can be downloaded.
/// Otherwise, it returns the current 'State' of the export
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// Request must contain the ExportId See also ServerExportPlayersInSegmentAsync, ServerGetAllSegmentsAsync.
///
/// When the asynchronous task is complete, call <see cref="PFPlayStreamServerGetSegmentExportGetResultSize"/>
/// and <see cref="PFPlayStreamServerGetSegmentExportGetResult"/> to get the result.
/// </remarks>
PF_API PFPlayStreamServerGetSegmentExportAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFPlayStreamGetPlayersInSegmentExportRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerGetSegmentExport call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ASYNC_EXPORT_NOT_IN_FLIGHT,
/// E_PF_ASYNC_EXPORT_RATE_LIMIT_EXCEEDED, E_PF_EXPORT_NOT_FOUND, E_PF_EXPORT_UNKNOWN_ERROR, E_PF_INVALID_SEGMENT,
/// E_PF_PRODUCT_DISABLED_FOR_TITLE or any of the global PlayFab Service errors. See doc page "Handling
/// PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFPlayStreamServerGetSegmentExportGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFPlayStreamServerGetSegmentExportAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_ASYNC_EXPORT_NOT_IN_FLIGHT,
/// E_PF_ASYNC_EXPORT_RATE_LIMIT_EXCEEDED, E_PF_EXPORT_NOT_FOUND, E_PF_EXPORT_UNKNOWN_ERROR, E_PF_INVALID_SEGMENT,
/// E_PF_PRODUCT_DISABLED_FOR_TITLE or any of the global PlayFab Service errors. See doc page "Handling
/// PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFPlayStreamServerGetSegmentExportGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlayStreamGetPlayersInSegmentExportResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif

#if 0
/// <summary>
/// Returns the total number of players in a given segment.
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// Request must contain a valid Segment ID. See also ServerGetAllSegmentsAsync.
///
/// When the asynchronous task is complete, call <see cref="PFPlayStreamServerGetSegmentPlayerCountGetResult"/>
/// to get the result.
/// </remarks>
PF_API PFPlayStreamServerGetSegmentPlayerCountAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFPlayStreamGetSegmentPlayerCountRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Gets the result of a successful PFPlayStreamServerGetSegmentPlayerCountAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="result">PFPlayStreamGetSegmentPlayerCountResult object that will be populated with the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_GET_SEGMENT_PLAYER_COUNT_NOT_IN_FLIGHT,
/// E_PF_GET_SEGMENT_PLAYER_COUNT_RATE_LIMIT_EXCEEDED, E_PF_INTERNAL_SERVER_ERROR, E_PF_INVALID_SEGMENT,
/// E_PF_PRODUCT_DISABLED_FOR_TITLE, E_PF_SEGMENT_NOT_FOUND or any of the global PlayFab Service errors.
/// See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFPlayStreamServerGetSegmentPlayerCountGetResult(
    _Inout_ XAsyncBlock* async,
    _Out_ PFPlayStreamGetSegmentPlayerCountResult* result
) noexcept;
#endif


}
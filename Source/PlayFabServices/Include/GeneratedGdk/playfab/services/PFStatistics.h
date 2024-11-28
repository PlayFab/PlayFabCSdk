// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFStatisticsTypes.h>
#include <playfab/core/PFEntity.h>

extern "C"
{

/// <summary>
/// Delete statistics on an entity profile. This will remove all rankings from associated leaderboards.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// See also StatisticUpdateStatisticsAsync.
///
/// When the asynchronous task is complete, call <see cref="PFStatisticsDeleteStatisticsGetResultSize"/>
/// and <see cref="PFStatisticsDeleteStatisticsGetResult"/> to get the result.
/// </remarks>
PF_API PFStatisticsDeleteStatisticsAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFStatisticsDeleteStatisticsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a DeleteStatistics call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be one of
/// global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFStatisticsDeleteStatisticsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFStatisticsDeleteStatisticsAsync call.
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
PF_API PFStatisticsDeleteStatisticsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFStatisticsDeleteStatisticsResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

/// <summary>
/// Gets statistics for the specified entity.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// See also StatisticDeleteStatisticsAsync.
///
/// When the asynchronous task is complete, call <see cref="PFStatisticsGetStatisticsGetResultSize"/>
/// and <see cref="PFStatisticsGetStatisticsGetResult"/> to get the result.
/// </remarks>
PF_API PFStatisticsGetStatisticsAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFStatisticsGetStatisticsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a GetStatistics call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_API_NOT_ENABLED_FOR_GAME_CLIENT_ACCESS,
/// E_PF_INVALID_ENTITY_TYPE, E_PF_NOT_AUTHORIZED, E_PF_STATISTIC_NOT_FOUND or any of the global PlayFab
/// Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFStatisticsGetStatisticsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFStatisticsGetStatisticsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_API_NOT_ENABLED_FOR_GAME_CLIENT_ACCESS,
/// E_PF_INVALID_ENTITY_TYPE, E_PF_NOT_AUTHORIZED, E_PF_STATISTIC_NOT_FOUND or any of the global PlayFab
/// Service errors. See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFStatisticsGetStatisticsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFStatisticsGetStatisticsResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

/// <summary>
/// Update statistics on an entity profile. Depending on the statistic definition, this may result in
/// entity being ranked on various leaderboards.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// See also StatisticDeleteStatisticsAsync.
///
/// When the asynchronous task is complete, call <see cref="PFStatisticsUpdateStatisticsGetResultSize"/>
/// and <see cref="PFStatisticsUpdateStatisticsGetResult"/> to get the result.
/// </remarks>
PF_API PFStatisticsUpdateStatisticsAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFStatisticsUpdateStatisticsRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a UpdateStatistics call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_API_NOT_ENABLED_FOR_GAME_CLIENT_ACCESS,
/// E_PF_ENTITY_TYPE_MISMATCH_WITH_STAT_DEFINITION, E_PF_INVALID_ENTITY_TYPE, E_PF_NOT_AUTHORIZED, E_PF_STATISTIC_COLUMN_LENGTH_MISMATCH,
/// E_PF_STATISTIC_NOT_FOUND, E_PF_TRANSACTION_ALREADY_APPLIED or any of the global PlayFab Service errors.
/// See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFStatisticsUpdateStatisticsGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFStatisticsUpdateStatisticsAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_API_NOT_ENABLED_FOR_GAME_CLIENT_ACCESS,
/// E_PF_ENTITY_TYPE_MISMATCH_WITH_STAT_DEFINITION, E_PF_INVALID_ENTITY_TYPE, E_PF_NOT_AUTHORIZED, E_PF_STATISTIC_COLUMN_LENGTH_MISMATCH,
/// E_PF_STATISTIC_NOT_FOUND, E_PF_TRANSACTION_ALREADY_APPLIED or any of the global PlayFab Service errors.
/// See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFStatisticsUpdateStatisticsGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFStatisticsUpdateStatisticsResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;


}
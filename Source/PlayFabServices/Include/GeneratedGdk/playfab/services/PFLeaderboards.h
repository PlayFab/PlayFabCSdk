// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFLeaderboardsTypes.h>
#include <playfab/core/PFEntity.h>

extern "C"
{

/// <summary>
/// Get the friend leaderboard for the specified entity. A maximum of 100 friend entries are listed in
/// the leaderboard.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// When the asynchronous task is complete, call <see cref="PFLeaderboardsGetFriendLeaderboardForEntityGetResultSize"/>
/// and <see cref="PFLeaderboardsGetFriendLeaderboardForEntityGetResult"/> to get the result.
/// </remarks>
PF_API PFLeaderboardsGetFriendLeaderboardForEntityAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFLeaderboardsGetFriendLeaderboardForEntityRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a GetFriendLeaderboardForEntity call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_API_NOT_ENABLED_FOR_TITLE,
/// E_PF_INVALID_ENTITY_TYPE, E_PF_LEADERBOARD_NOT_FOUND, E_PF_NO_LEADERBOARD_FOR_STATISTIC, E_PF_STATISTIC_NOT_FOUND
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
PF_API PFLeaderboardsGetFriendLeaderboardForEntityGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFLeaderboardsGetFriendLeaderboardForEntityAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_API_NOT_ENABLED_FOR_TITLE,
/// E_PF_INVALID_ENTITY_TYPE, E_PF_LEADERBOARD_NOT_FOUND, E_PF_NO_LEADERBOARD_FOR_STATISTIC, E_PF_STATISTIC_NOT_FOUND
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFLeaderboardsGetFriendLeaderboardForEntityGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFLeaderboardsGetEntityLeaderboardResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

/// <summary>
/// Get the leaderboard for a specific entity type and statistic.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// When the asynchronous task is complete, call <see cref="PFLeaderboardsGetLeaderboardGetResultSize"/>
/// and <see cref="PFLeaderboardsGetLeaderboardGetResult"/> to get the result.
/// </remarks>
PF_API PFLeaderboardsGetLeaderboardAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFLeaderboardsGetEntityLeaderboardRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a GetLeaderboard call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_LEADERBOARD_NOT_FOUND,
/// E_PF_NO_LEADERBOARD_FOR_STATISTIC, E_PF_SPECIFIED_VERSION_LEADERBOARD_NOT_FOUND, E_PF_STATISTIC_NOT_FOUND
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
PF_API PFLeaderboardsGetLeaderboardGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFLeaderboardsGetLeaderboardAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_LEADERBOARD_NOT_FOUND,
/// E_PF_NO_LEADERBOARD_FOR_STATISTIC, E_PF_SPECIFIED_VERSION_LEADERBOARD_NOT_FOUND, E_PF_STATISTIC_NOT_FOUND
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFLeaderboardsGetLeaderboardGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFLeaderboardsGetEntityLeaderboardResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;

/// <summary>
/// Get the leaderboard around a specific entity.
/// </summary>
/// <param name="entityHandle">PFEntityHandle to use for authentication.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on all platforms.
/// When the asynchronous task is complete, call <see cref="PFLeaderboardsGetLeaderboardAroundEntityGetResultSize"/>
/// and <see cref="PFLeaderboardsGetLeaderboardAroundEntityGetResult"/> to get the result.
/// </remarks>
PF_API PFLeaderboardsGetLeaderboardAroundEntityAsync(
    _In_ PFEntityHandle entityHandle,
    _In_ const PFLeaderboardsGetLeaderboardAroundEntityRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a GetLeaderboardAroundEntity call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_LEADERBOARD_NOT_FOUND,
/// E_PF_NO_LEADERBOARD_FOR_STATISTIC, E_PF_STATISTIC_NOT_FOUND or any of the global PlayFab Service errors.
/// See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
PF_API PFLeaderboardsGetLeaderboardAroundEntityGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFLeaderboardsGetLeaderboardAroundEntityAsync call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The size of the buffer for the result object.</param>
/// <param name="buffer">Byte buffer used for the result value and its fields.</param>
/// <param name="result">Pointer to the result object.</param>
/// <param name="bufferUsed">The number of bytes in the provided buffer that were used.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_LEADERBOARD_NOT_FOUND,
/// E_PF_NO_LEADERBOARD_FOR_STATISTIC, E_PF_STATISTIC_NOT_FOUND or any of the global PlayFab Service errors.
/// See doc page "Handling PlayFab Errors" for more details on error handling.
/// </returns>
/// <remarks>
/// result is a pointer within buffer and does not need to be freed separately.
/// </remarks>
PF_API PFLeaderboardsGetLeaderboardAroundEntityGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFLeaderboardsGetEntityLeaderboardResponse** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;


}
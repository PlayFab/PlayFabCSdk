// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/services/PFPlatformSpecificTypes.h>
#include <playfab/core/PFEntity.h>

extern "C"
{

#if HC_PLATFORM == HC_PLATFORM_GDK || HC_PLATFORM == HC_PLATFORM_LINUX || HC_PLATFORM == HC_PLATFORM_MAC
/// <summary>
/// Awards the specified users the specified Steam achievements
/// </summary>
/// <param name="titleEntityHandle">PFEntityHandle for a title Entity obtained using PFAuthenticationGetEntityWithSecretKeyAsync.</param>
/// <param name="request">Populated request object.</param>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This API is available on Windows, Linux, and macOS.
/// When the asynchronous task is complete, call <see cref="PFPlatformSpecificServerAwardSteamAchievementGetResultSize"/>
/// and <see cref="PFPlatformSpecificServerAwardSteamAchievementGetResult"/> to get the result.
/// </remarks>
PF_API PFPlatformSpecificServerAwardSteamAchievementAsync(
    _In_ PFEntityHandle titleEntityHandle,
    _In_ const PFPlatformSpecificAwardSteamAchievementRequest* request,
    _Inout_ XAsyncBlock* async
) noexcept;

/// <summary>
/// Get the size in bytes needed to store the result of a ServerAwardSteamAchievement call.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <param name="bufferSize">The buffer size in bytes required for the result.</param>
/// <returns>
/// Result code for this API operation. If the service call is unsuccessful, the result will be E_PF_USERIS_NOT_VALID
/// or any of the global PlayFab Service errors. See doc page "Handling PlayFab Errors" for more details
/// on error handling.
/// </returns>
PF_API PFPlatformSpecificServerAwardSteamAchievementGetResultSize(
    _Inout_ XAsyncBlock* async,
    _Out_ size_t* bufferSize
) noexcept;

/// <summary>
/// Gets the result of a successful PFPlatformSpecificServerAwardSteamAchievementAsync call.
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
PF_API PFPlatformSpecificServerAwardSteamAchievementGetResult(
    _Inout_ XAsyncBlock* async,
    _In_ size_t bufferSize,
    _Out_writes_bytes_to_(bufferSize, *bufferUsed) void* buffer,
    _Outptr_ PFPlatformSpecificAwardSteamAchievementResult** result,
    _Out_opt_ size_t* bufferUsed
) noexcept;
#endif


}
// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/PFEntity.h>
#include <playfab/core/PFLocalUser.h>

extern "C"
{

#if HC_PLATFORM == HC_PLATFORM_GDK
/// <summary>
/// Creates a PlayFab Local User from a Steam authenticated user. This is the primary authentication method for Steam platforms and supports both online and offline gameplay modes.
/// Fails with E_PF_CORE_MISSING_PLATFORM if the Steam runtime is not installed on the device.
/// </summary>
/// <param name="serviceConfigHandle">PFServiceConfigHandle that will be used to log this user into PlayFab.</param>
/// <param name="customContext">Custom context to be associated with the local user.</param>
/// <param name="localUserHandle">Pointer to a PFLocalUserHandle to write.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_PF_NOT_INITIALIZED, E_PF_CORE_MISSING_PLATFORM, or E_INVALIDARG.</returns>
PF_API PFLocalUserCreateHandleWithSteamUser(
    _In_ PFServiceConfigHandle serviceConfigHandle,
    _In_opt_ void* customContext,
    _Out_ PFLocalUserHandle* localUserHandle
) noexcept;
#endif

}

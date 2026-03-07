// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#if HC_PLATFORM == HC_PLATFORM_GDK
#include <playfab/core/PFEntity.h>
#include <playfab/core/PFLocalUser.h>
#include <XUser.h>

extern "C"
{

/// <summary>
/// Creates a PlayFab Local User from an Xbox Live authenticated user. This is the primary authentication method for Xbox/GDK platforms and supports both online and offline gameplay modes.
/// </summary>
/// <param name="serviceConfigHandle">PFServiceConfigHandle that will be used to log this user into PlayFab.</param>
/// <param name="user">XUserHandle for the Xbox user.</param>
/// <param name="customContext">Custom context to be associated with the local user.</param>
/// <param name="localUserHandle">Pointer to a PFLocalUserHandle to write.</param>
/// <returns>Result code for this API operation.  Possible values are S_OK, E_PF_NOT_INITIALIZED, or E_INVALIDARG.</returns>
PF_API PFLocalUserCreateHandleWithXboxUser(
    _In_ PFServiceConfigHandle serviceConfigHandle,
    _In_ XUserHandle user,
    _In_opt_ void* customContext,
    _Out_ PFLocalUserHandle* localUserHandle
) noexcept;

/// <summary>
/// Get the XUserHandle used to create the user if there was one. If the user was created with PFLocalUserCreateHandleWithPersistedLocalId,
/// there isn't an XUserHandle and E_PF_CORE_LOCAL_USER_NO_PLATFORM_CONTEXT will be returned.
/// </summary>
/// <param name="localUserHandle">PFLocalUserHandle for the user.</param>
/// <param name="user">Handle owned by this local user.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// The populated XUserHandle is owned by the title. After calling this API, it is the title's responsibility to make sure it
/// is appropriately cleaned up by calling XUserCloseHandle.
/// </remarks>
PF_API PFLocalUserTryGetXUser(
    _In_ PFLocalUserHandle localUserHandle,
    _Out_ XUserHandle* user
) noexcept;

}

#endif
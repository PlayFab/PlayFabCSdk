// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/PFCore.h>
#include <playfab/services/PFFriends.h>
#include <playfab/services/PFAccountManagement.h>
#include <playfab/services/PFPlatformSpecific.h>
#include <playfab/services/PFCharacter.h>
#include <playfab/services/PFSegments.h>
#include <playfab/services/PFTitleDataManagement.h>
#include <playfab/services/PFPlayerDataManagement.h>
#include <playfab/services/PFPushNotifications.h>
#include <playfab/services/PFCloudScript.h>
#include <playfab/services/PFData.h>
#include <playfab/services/PFInventory.h>
#include <playfab/services/PFCatalog.h>
#include <playfab/services/PFExperimentation.h>
#include <playfab/services/PFGroups.h>
#include <playfab/services/PFLocalization.h>
#include <playfab/services/PFMultiplayerServer.h>
#include <playfab/services/PFProfiles.h>

extern "C"
{

#if HC_PLATFORM == HC_PLATFORM_WIN32 || HC_PLATFORM == HC_PLATFORM_GDK
/// <summary>
/// Initializes PlayFab Services global state
/// </summary>
/// <param name="reserved">Reserved for future use.</param>
/// <remarks>
/// This will internally call PFInitialize(nullptr) if it hasn't been called already by the
/// title. If control of PFCore background work is needed, the title should explicitly call
/// PFInitialize and PFUninitialize.
/// </remarks>
/// <returns>Result code for this API operation.</returns>
PF_API PFServicesInitialize(
    _In_opt_ XTaskQueueHandle reserved
) noexcept;
#endif

/// <summary>
/// Cleanup PlayFab Services global state.
/// </summary>
/// <param name="async">XAsyncBlock for the async operation.</param>
/// <returns>Result code for this API operation.</returns>
/// <remarks>
/// This will internally call PFUninitializeAsync() if PFServicesInitialize() needed 
/// to call PFInitialize() interally.
/// Asynchronous result returned via XAsyncGetStatus.</remarks>
PF_API PFServicesUninitializeAsync(
    _In_ XAsyncBlock* async
) noexcept;

}


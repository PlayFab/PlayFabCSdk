// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#if HC_PLATFORM == HC_PLATFORM_GDK
#include <playfab/core/PFPal.h>

#include <playfab/core/PFTypes.h>

#include <XUser.h>

extern "C"
{

#pragma push_macro("IN")
#undef IN

/// <summary>
/// PFAuthenticationLoginWithXUserRequest data model. If this is the first time a user has signed in with
/// the Xbox Live account and CreateAccount is set to true, a new PlayFab account will be created and
/// linked to the Xbox Live account. In this case, no email or username will be associated with the PlayFab
/// account. Otherwise, if no PlayFab account is linked to the Xbox Live account, an error indicating
/// this will be returned, so that the title can guide the user through creation of a PlayFab account.
/// Request object for PFAuthenticationLoginWithXUserAsync.
/// </summary>
typedef struct PFAuthenticationLoginWithXUserRequest
{
    /// <summary>
    /// Automatically create a PlayFab account if one is not currently linked to this ID.
    /// </summary>
    bool createAccount;

    /// <summary>
    /// (Optional) The optional custom tags associated with the request (e.g. build number, external
    /// trace identifiers, etc.).
    /// </summary>
    _Maybenull_ _Field_size_(customTagsCount) struct PFStringDictionaryEntry const* customTags;

    /// <summary>
    /// Count of customTags
    /// </summary>
    uint32_t customTagsCount;

    /// <summary>
    /// (Optional) Flags for which pieces of info to return for the user.
    /// </summary>
    _Maybenull_ PFGetPlayerCombinedInfoRequestParams const* infoRequestParameters;

    /// <summary>
    /// (Optional) Player secret that is used to verify API request signatures (Enterprise Only).
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* playerSecret;

    /// <summary>
    /// XUserHandle returned from XUserAddAsync or XUserAddByIdWithUiAsync.
    /// </summary>
    XUserHandle user;

} PFAuthenticationLoginWithXUserRequest;

#pragma pop_macro("IN")

}
#endif // HC_PLATFORM_GDK

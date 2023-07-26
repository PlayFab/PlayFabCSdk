// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/PFPal.h>
#include <playfab/core/PFTypes.h>
#include <playfab/services/PFTypes.h>

extern "C"
{

#pragma push_macro("IN")
#undef IN

/// <summary>
/// PFCharacterListUsersCharactersRequest data model. Returns a list of every character that currently
/// belongs to a user.
/// </summary>
typedef struct PFCharacterListUsersCharactersRequest
{
    /// <summary>
    /// Unique PlayFab assigned ID of the user on whom the operation will be performed.
    /// </summary>
    _Null_terminated_ const char* playFabId;

} PFCharacterListUsersCharactersRequest;

/// <summary>
/// PFCharacterListUsersCharactersResult data model.
/// </summary>
typedef struct PFCharacterListUsersCharactersResult
{
    /// <summary>
    /// (Optional) The requested list of characters.
    /// </summary>
    _Maybenull_ _Field_size_(charactersCount) PFCharacterResult const* const* characters;

    /// <summary>
    /// Count of characters
    /// </summary>
    uint32_t charactersCount;

} PFCharacterListUsersCharactersResult;

/// <summary>
/// PFCharacterGetCharacterDataRequest data model. Data is stored as JSON key-value pairs. If the Keys
/// parameter is provided, the data object returned will only contain the data specific to the indicated
/// Keys. Otherwise, the full set of custom character data will be returned.
/// </summary>
typedef struct PFCharacterGetCharacterDataRequest
{
    /// <summary>
    /// Unique PlayFab assigned ID for a specific character owned by a user.
    /// </summary>
    _Null_terminated_ const char* characterId;

    /// <summary>
    /// (Optional) The version that currently exists according to the caller. The call will return the
    /// data for all of the keys if the version in the system is greater than this.
    /// </summary>
    _Maybenull_ uint32_t const* ifChangedFromDataVersion;

    /// <summary>
    /// (Optional) Specific keys to search for in the custom user data.
    /// </summary>
    _Maybenull_ _Field_size_(keysCount) const char* const* keys;

    /// <summary>
    /// Count of keys
    /// </summary>
    uint32_t keysCount;

    /// <summary>
    /// Unique PlayFab identifier of the user to load data for. Optional, defaults to yourself if not
    /// set.
    /// </summary>
    _Null_terminated_ const char* playFabId;

} PFCharacterGetCharacterDataRequest;

/// <summary>
/// PFCharacterClientGetCharacterDataResult data model.
/// </summary>
typedef struct PFCharacterClientGetCharacterDataResult
{
    /// <summary>
    /// (Optional) Unique PlayFab assigned ID for a specific character owned by a user.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* characterId;

    /// <summary>
    /// (Optional) User specific data for this title.
    /// </summary>
    _Maybenull_ _Field_size_(dataCount) struct PFUserDataRecordDictionaryEntry const* data;

    /// <summary>
    /// Count of data
    /// </summary>
    uint32_t dataCount;

    /// <summary>
    /// Indicates the current version of the data that has been set. This is incremented with every set
    /// call for that type of data (read-only, internal, etc). This version can be provided in Get calls
    /// to find updated data.
    /// </summary>
    uint32_t dataVersion;

} PFCharacterClientGetCharacterDataResult;

/// <summary>
/// PFCharacterClientGrantCharacterToUserRequest data model. Grants a character to the user of the type
/// specified by the item ID. The user must already have an instance of this item in their inventory in
/// order to allow character creation. This item can come from a purchase or grant, which must be done
/// before calling to create the character.
/// </summary>
typedef struct PFCharacterClientGrantCharacterToUserRequest
{
    /// <summary>
    /// (Optional) Catalog version from which items are to be granted.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* catalogVersion;

    /// <summary>
    /// Non-unique display name of the character being granted (1-40 characters in length).
    /// </summary>
    _Null_terminated_ const char* characterName;

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
    /// Catalog item identifier of the item in the user's inventory that corresponds to the character
    /// in the catalog to be created.
    /// </summary>
    _Null_terminated_ const char* itemId;

} PFCharacterClientGrantCharacterToUserRequest;

/// <summary>
/// PFCharacterClientGrantCharacterToUserResult data model.
/// </summary>
typedef struct PFCharacterClientGrantCharacterToUserResult
{
    /// <summary>
    /// (Optional) Unique identifier tagged to this character.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* characterId;

    /// <summary>
    /// (Optional) Type of character that was created.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* characterType;

    /// <summary>
    /// Indicates whether this character was created successfully.
    /// </summary>
    bool result;

} PFCharacterClientGrantCharacterToUserResult;

/// <summary>
/// PFCharacterClientUpdateCharacterDataRequest data model. This function performs an additive update
/// of the arbitrary strings containing the custom data for the character. In updating the custom data
/// object, keys which already exist in the object will have their values overwritten, while keys with
/// null values will be removed. New keys will be added, with the given values. No other key-value pairs
/// will be changed apart from those specified in the call.
/// </summary>
typedef struct PFCharacterClientUpdateCharacterDataRequest
{
    /// <summary>
    /// Unique PlayFab assigned ID for a specific character owned by a user.
    /// </summary>
    _Null_terminated_ const char* characterId;

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
    /// (Optional) Key-value pairs to be written to the custom data. Note that keys are trimmed of whitespace,
    /// are limited in size, and may not begin with a '!' character or be null.
    /// </summary>
    _Maybenull_ _Field_size_(dataCount) struct PFStringDictionaryEntry const* data;

    /// <summary>
    /// Count of data
    /// </summary>
    uint32_t dataCount;

    /// <summary>
    /// (Optional) Optional list of Data-keys to remove from UserData. Some SDKs cannot insert null-values
    /// into Data due to language constraints. Use this to delete the keys directly.
    /// </summary>
    _Maybenull_ _Field_size_(keysToRemoveCount) const char* const* keysToRemove;

    /// <summary>
    /// Count of keysToRemove
    /// </summary>
    uint32_t keysToRemoveCount;

    /// <summary>
    /// (Optional) Permission to be applied to all user data keys written in this request. Defaults to
    /// "private" if not set.
    /// </summary>
    _Maybenull_ PFUserDataPermission const* permission;

} PFCharacterClientUpdateCharacterDataRequest;

/// <summary>
/// PFCharacterUpdateCharacterDataResult data model.
/// </summary>
typedef struct PFCharacterUpdateCharacterDataResult
{
    /// <summary>
    /// Indicates the current version of the data that has been set. This is incremented with every set
    /// call for that type of data (read-only, internal, etc). This version can be provided in Get calls
    /// to find updated data.
    /// </summary>
    uint32_t dataVersion;

} PFCharacterUpdateCharacterDataResult;

/// <summary>
/// PFCharacterDeleteCharacterFromUserRequest data model. This function will delete the specified character
/// from the list allowed by the user, and will also delete any inventory or VC currently held by that
/// character. It will NOT delete any statistics associated for this character, in order to preserve leaderboard
/// integrity.
/// </summary>
typedef struct PFCharacterDeleteCharacterFromUserRequest
{
    /// <summary>
    /// Unique PlayFab assigned ID for a specific character owned by a user.
    /// </summary>
    _Null_terminated_ const char* characterId;

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
    /// Unique PlayFab assigned ID of the user on whom the operation will be performed.
    /// </summary>
    _Null_terminated_ const char* playFabId;

    /// <summary>
    /// If true, the character's inventory will be transferred up to the owning user; otherwise, this
    /// request will purge those items.
    /// </summary>
    bool saveCharacterInventory;

} PFCharacterDeleteCharacterFromUserRequest;

/// <summary>
/// PFCharacterServerGetCharacterDataResult data model.
/// </summary>
typedef struct PFCharacterServerGetCharacterDataResult
{
    /// <summary>
    /// (Optional) Unique PlayFab assigned ID for a specific character owned by a user.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* characterId;

    /// <summary>
    /// (Optional) User specific data for this title.
    /// </summary>
    _Maybenull_ _Field_size_(dataCount) struct PFUserDataRecordDictionaryEntry const* data;

    /// <summary>
    /// Count of data
    /// </summary>
    uint32_t dataCount;

    /// <summary>
    /// Indicates the current version of the data that has been set. This is incremented with every set
    /// call for that type of data (read-only, internal, etc). This version can be provided in Get calls
    /// to find updated data.
    /// </summary>
    uint32_t dataVersion;

    /// <summary>
    /// (Optional) Unique PlayFab assigned ID of the user on whom the operation will be performed.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* playFabId;

} PFCharacterServerGetCharacterDataResult;

/// <summary>
/// PFCharacterServerGrantCharacterToUserRequest data model. Grants a character to the user of the type
/// and name specified in the request.
/// </summary>
typedef struct PFCharacterServerGrantCharacterToUserRequest
{
    /// <summary>
    /// Non-unique display name of the character being granted (1-40 characters in length).
    /// </summary>
    _Null_terminated_ const char* characterName;

    /// <summary>
    /// Type of the character being granted; statistics can be sliced based on this value.
    /// </summary>
    _Null_terminated_ const char* characterType;

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
    /// Unique PlayFab assigned ID of the user on whom the operation will be performed.
    /// </summary>
    _Null_terminated_ const char* playFabId;

} PFCharacterServerGrantCharacterToUserRequest;

/// <summary>
/// PFCharacterServerGrantCharacterToUserResult data model.
/// </summary>
typedef struct PFCharacterServerGrantCharacterToUserResult
{
    /// <summary>
    /// (Optional) Unique identifier tagged to this character.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* characterId;

} PFCharacterServerGrantCharacterToUserResult;

/// <summary>
/// PFCharacterServerUpdateCharacterDataRequest data model. This function performs an additive update
/// of the arbitrary JSON object containing the custom data for the user. In updating the custom data
/// object, keys which already exist in the object will have their values overwritten, while keys with
/// null values will be removed. No other key-value pairs will be changed apart from those specified in
/// the call.
/// </summary>
typedef struct PFCharacterServerUpdateCharacterDataRequest
{
    /// <summary>
    /// Unique PlayFab assigned ID for a specific character owned by a user.
    /// </summary>
    _Null_terminated_ const char* characterId;

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
    /// (Optional) Key-value pairs to be written to the custom data. Note that keys are trimmed of whitespace,
    /// are limited in size, and may not begin with a '!' character or be null.
    /// </summary>
    _Maybenull_ _Field_size_(dataCount) struct PFStringDictionaryEntry const* data;

    /// <summary>
    /// Count of data
    /// </summary>
    uint32_t dataCount;

    /// <summary>
    /// (Optional) Optional list of Data-keys to remove from UserData. Some SDKs cannot insert null-values
    /// into Data due to language constraints. Use this to delete the keys directly.
    /// </summary>
    _Maybenull_ _Field_size_(keysToRemoveCount) const char* const* keysToRemove;

    /// <summary>
    /// Count of keysToRemove
    /// </summary>
    uint32_t keysToRemoveCount;

    /// <summary>
    /// (Optional) Permission to be applied to all user data keys written in this request. Defaults to
    /// "private" if not set.
    /// </summary>
    _Maybenull_ PFUserDataPermission const* permission;

    /// <summary>
    /// Unique PlayFab assigned ID of the user on whom the operation will be performed.
    /// </summary>
    _Null_terminated_ const char* playFabId;

} PFCharacterServerUpdateCharacterDataRequest;

#pragma pop_macro("IN")

}

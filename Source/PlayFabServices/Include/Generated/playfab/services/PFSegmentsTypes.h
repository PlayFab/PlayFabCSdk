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
/// PFSegmentsGetSegmentResult data model.
/// </summary>
typedef struct PFSegmentsGetSegmentResult
{
    /// <summary>
    /// (Optional) Identifier of the segments AB Test, if it is attached to one.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* aBTestParent;

    /// <summary>
    /// Unique identifier for this segment.
    /// </summary>
    _Null_terminated_ const char* id;

    /// <summary>
    /// (Optional) Segment name.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* name;

} PFSegmentsGetSegmentResult;

/// <summary>
/// PFSegmentsGetPlayerSegmentsResult data model.
/// </summary>
typedef struct PFSegmentsGetPlayerSegmentsResult
{
    /// <summary>
    /// (Optional) Array of segments the requested player currently belongs to.
    /// </summary>
    _Maybenull_ _Field_size_(segmentsCount) PFSegmentsGetSegmentResult const* const* segments;

    /// <summary>
    /// Count of segments
    /// </summary>
    uint32_t segmentsCount;

} PFSegmentsGetPlayerSegmentsResult;

/// <summary>
/// PFSegmentsGetPlayerTagsRequest data model. This API will return a list of canonical tags which includes
/// both namespace and tag's name. If namespace is not provided, the result is a list of all canonical
/// tags. TagName can be used for segmentation and Namespace is limited to 128 characters.
/// </summary>
typedef struct PFSegmentsGetPlayerTagsRequest
{
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
    /// (Optional) Optional namespace to filter results by.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* playfabNamespace;

    /// <summary>
    /// Unique PlayFab assigned ID of the user on whom the operation will be performed.
    /// </summary>
    _Null_terminated_ const char* playFabId;

} PFSegmentsGetPlayerTagsRequest;

/// <summary>
/// PFSegmentsGetPlayerTagsResult data model.
/// </summary>
typedef struct PFSegmentsGetPlayerTagsResult
{
    /// <summary>
    /// Unique PlayFab assigned ID of the user on whom the operation will be performed.
    /// </summary>
    _Null_terminated_ const char* playFabId;

    /// <summary>
    /// Canonical tags (including namespace and tag's name) for the requested user.
    /// </summary>
    _Field_size_(tagsCount) const char* const* tags;

    /// <summary>
    /// Count of tags
    /// </summary>
    uint32_t tagsCount;

} PFSegmentsGetPlayerTagsResult;

/// <summary>
/// PFSegmentsAddPlayerTagRequest data model. This API will trigger a player_tag_added event and add
/// a tag with the given TagName and PlayFabID to the corresponding player profile. TagName can be used
/// for segmentation and it is limited to 256 characters. Also there is a limit on the number of tags
/// a title can have.
/// </summary>
typedef struct PFSegmentsAddPlayerTagRequest
{
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
    /// Unique tag for player profile.
    /// </summary>
    _Null_terminated_ const char* tagName;

} PFSegmentsAddPlayerTagRequest;

/// <summary>
/// PFSegmentsGetAllSegmentsResult data model.
/// </summary>
typedef struct PFSegmentsGetAllSegmentsResult
{
    /// <summary>
    /// (Optional) Array of segments for this title.
    /// </summary>
    _Maybenull_ _Field_size_(segmentsCount) PFSegmentsGetSegmentResult const* const* segments;

    /// <summary>
    /// Count of segments
    /// </summary>
    uint32_t segmentsCount;

} PFSegmentsGetAllSegmentsResult;

/// <summary>
/// PFSegmentsGetPlayersSegmentsRequest data model.
/// </summary>
typedef struct PFSegmentsGetPlayersSegmentsRequest
{
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

} PFSegmentsGetPlayersSegmentsRequest;

/// <summary>
/// PFSegmentsRemovePlayerTagRequest data model. This API will trigger a player_tag_removed event and
/// remove a tag with the given TagName and PlayFabID from the corresponding player profile. TagName can
/// be used for segmentation and it is limited to 256 characters.
/// </summary>
typedef struct PFSegmentsRemovePlayerTagRequest
{
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
    /// Unique tag for player profile.
    /// </summary>
    _Null_terminated_ const char* tagName;

} PFSegmentsRemovePlayerTagRequest;

#pragma pop_macro("IN")

}

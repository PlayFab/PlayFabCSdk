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
/// PFPlayStreamExportPlayersInSegmentRequest data model. Request must contain the Segment ID.
/// </summary>
typedef struct PFPlayStreamExportPlayersInSegmentRequest
{
    /// <summary>
    /// Unique identifier of the requested segment.
    /// </summary>
    _Null_terminated_ const char* segmentId;

} PFPlayStreamExportPlayersInSegmentRequest;

/// <summary>
/// PFPlayStreamExportPlayersInSegmentResult data model.
/// </summary>
typedef struct PFPlayStreamExportPlayersInSegmentResult
{
    /// <summary>
    /// (Optional) Unique identifier of the export for the requested Segment.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* exportId;

    /// <summary>
    /// (Optional) Unique identifier of the requested Segment.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* segmentId;

} PFPlayStreamExportPlayersInSegmentResult;

/// <summary>
/// PFPlayStreamGetPlayersInSegmentExportRequest data model. Request must contain the ExportId.
/// </summary>
typedef struct PFPlayStreamGetPlayersInSegmentExportRequest
{
    /// <summary>
    /// Unique identifier of the export for the requested Segment.
    /// </summary>
    _Null_terminated_ const char* exportId;

} PFPlayStreamGetPlayersInSegmentExportRequest;

/// <summary>
/// PFPlayStreamGetPlayersInSegmentExportResponse data model.
/// </summary>
typedef struct PFPlayStreamGetPlayersInSegmentExportResponse
{
    /// <summary>
    /// (Optional) Url from which the index file can be downloaded.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* indexUrl;

    /// <summary>
    /// (Optional) Shows the current status of the export.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* state;

} PFPlayStreamGetPlayersInSegmentExportResponse;

/// <summary>
/// PFPlayStreamGetSegmentPlayerCountRequest data model. Request must contain a valid Segment ID.
/// </summary>
typedef struct PFPlayStreamGetSegmentPlayerCountRequest
{
    /// <summary>
    /// Unique identifier for the requested segment.
    /// </summary>
    _Null_terminated_ const char* segmentId;

} PFPlayStreamGetSegmentPlayerCountRequest;

/// <summary>
/// PFPlayStreamGetSegmentPlayerCountResult data model.
/// </summary>
typedef struct PFPlayStreamGetSegmentPlayerCountResult
{
    /// <summary>
    /// Count of profiles matching this segment.
    /// </summary>
    int32_t profilesInSegment;

} PFPlayStreamGetSegmentPlayerCountResult;

#pragma pop_macro("IN")

}

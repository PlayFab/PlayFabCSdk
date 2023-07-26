// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/PFPal.h>

#include <playfab/core/PFTypes.h>

extern "C"
{

#pragma push_macro("IN")
#undef IN

/// <summary>
/// PFEventsCreateTelemetryKeyRequest data model.
/// </summary>
typedef struct PFEventsCreateTelemetryKeyRequest
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
    /// (Optional) The optional entity to perform this action on. Defaults to the currently logged in
    /// entity.
    /// </summary>
    _Maybenull_ PFEntityKey const* entity;

    /// <summary>
    /// The name of the new key. Telemetry key names must be unique within the scope of the title.
    /// </summary>
    _Null_terminated_ const char* keyName;

} PFEventsCreateTelemetryKeyRequest;

/// <summary>
/// PFEventsTelemetryKeyDetails data model.
/// </summary>
typedef struct PFEventsTelemetryKeyDetails
{
    /// <summary>
    /// When the key was created.
    /// </summary>
    time_t createTime;

    /// <summary>
    /// Whether or not the key is currently active. Deactivated keys cannot be used for telemetry ingestion.
    /// </summary>
    bool isActive;

    /// <summary>
    /// (Optional) The key that can be distributed to clients for use during telemetry ingestion.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* keyValue;

    /// <summary>
    /// When the key was last updated.
    /// </summary>
    time_t lastUpdateTime;

    /// <summary>
    /// (Optional) The name of the key. Telemetry key names are unique within the scope of the title.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* name;

} PFEventsTelemetryKeyDetails;

/// <summary>
/// PFEventsCreateTelemetryKeyResponse data model.
/// </summary>
typedef struct PFEventsCreateTelemetryKeyResponse
{
    /// <summary>
    /// (Optional) Details about the newly created telemetry key.
    /// </summary>
    _Maybenull_ PFEventsTelemetryKeyDetails const* newKeyDetails;

} PFEventsCreateTelemetryKeyResponse;

/// <summary>
/// PFEventsDeleteTelemetryKeyRequest data model.
/// </summary>
typedef struct PFEventsDeleteTelemetryKeyRequest
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
    /// (Optional) The optional entity to perform this action on. Defaults to the currently logged in
    /// entity.
    /// </summary>
    _Maybenull_ PFEntityKey const* entity;

    /// <summary>
    /// The name of the key to delete.
    /// </summary>
    _Null_terminated_ const char* keyName;

} PFEventsDeleteTelemetryKeyRequest;

/// <summary>
/// PFEventsDeleteTelemetryKeyResponse data model.
/// </summary>
typedef struct PFEventsDeleteTelemetryKeyResponse
{
    /// <summary>
    /// Indicates whether or not the key was deleted. If false, no key with that name existed.
    /// </summary>
    bool wasKeyDeleted;

} PFEventsDeleteTelemetryKeyResponse;

/// <summary>
/// PFEventsGetTelemetryKeyRequest data model.
/// </summary>
typedef struct PFEventsGetTelemetryKeyRequest
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
    /// (Optional) The optional entity to perform this action on. Defaults to the currently logged in
    /// entity.
    /// </summary>
    _Maybenull_ PFEntityKey const* entity;

    /// <summary>
    /// The name of the key to retrieve.
    /// </summary>
    _Null_terminated_ const char* keyName;

} PFEventsGetTelemetryKeyRequest;

/// <summary>
/// PFEventsGetTelemetryKeyResponse data model.
/// </summary>
typedef struct PFEventsGetTelemetryKeyResponse
{
    /// <summary>
    /// (Optional) Details about the requested telemetry key.
    /// </summary>
    _Maybenull_ PFEventsTelemetryKeyDetails const* keyDetails;

} PFEventsGetTelemetryKeyResponse;

/// <summary>
/// PFEventsListTelemetryKeysRequest data model.
/// </summary>
typedef struct PFEventsListTelemetryKeysRequest
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
    /// (Optional) The optional entity to perform this action on. Defaults to the currently logged in
    /// entity.
    /// </summary>
    _Maybenull_ PFEntityKey const* entity;

} PFEventsListTelemetryKeysRequest;

/// <summary>
/// PFEventsListTelemetryKeysResponse data model.
/// </summary>
typedef struct PFEventsListTelemetryKeysResponse
{
    /// <summary>
    /// (Optional) The telemetry keys configured for the title.
    /// </summary>
    _Maybenull_ _Field_size_(keyDetailsCount) PFEventsTelemetryKeyDetails const* const* keyDetails;

    /// <summary>
    /// Count of keyDetails
    /// </summary>
    uint32_t keyDetailsCount;

} PFEventsListTelemetryKeysResponse;

/// <summary>
/// PFEventsSetTelemetryKeyActiveRequest data model.
/// </summary>
typedef struct PFEventsSetTelemetryKeyActiveRequest
{
    /// <summary>
    /// Whether to set the key to active (true) or deactivated (false).
    /// </summary>
    bool active;

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
    /// (Optional) The optional entity to perform this action on. Defaults to the currently logged in
    /// entity.
    /// </summary>
    _Maybenull_ PFEntityKey const* entity;

    /// <summary>
    /// The name of the key to update.
    /// </summary>
    _Null_terminated_ const char* keyName;

} PFEventsSetTelemetryKeyActiveRequest;

/// <summary>
/// PFEventsSetTelemetryKeyActiveResponse data model.
/// </summary>
typedef struct PFEventsSetTelemetryKeyActiveResponse
{
    /// <summary>
    /// (Optional) The most current details about the telemetry key that was to be updated.
    /// </summary>
    _Maybenull_ PFEventsTelemetryKeyDetails const* keyDetails;

    /// <summary>
    /// Indicates whether or not the key was updated. If false, the key was already in the desired state.
    /// </summary>
    bool wasKeyUpdated;

} PFEventsSetTelemetryKeyActiveResponse;

/// <summary>
/// PFEventsEventContents data model.
/// </summary>
typedef struct PFEventsEventContents
{
    /// <summary>
    /// (Optional) The optional custom tags associated with the event (e.g. build number, external trace
    /// identifiers, etc.). Before an event is written, this collection and the base request custom tags
    /// will be merged, but not overriden. This enables the caller to specify static tags and per event
    /// tags.
    /// </summary>
    _Maybenull_ _Field_size_(customTagsCount) struct PFStringDictionaryEntry const* customTags;

    /// <summary>
    /// Count of customTags
    /// </summary>
    uint32_t customTagsCount;

    /// <summary>
    /// (Optional) Entity associated with the event. If null, the event will apply to the calling entity.
    /// </summary>
    _Maybenull_ PFEntityKey const* entity;

    /// <summary>
    /// The namespace in which the event is defined. Allowed namespaces can vary by API.
    /// </summary>
    _Null_terminated_ const char* eventNamespace;

    /// <summary>
    /// The name of this event.
    /// </summary>
    _Null_terminated_ const char* name;

    /// <summary>
    /// (Optional) The original unique identifier associated with this event before it was posted to
    /// PlayFab. The value might differ from the EventId value, which is assigned when the event is received
    /// by the server.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* originalId;

    /// <summary>
    /// (Optional) The time (in UTC) associated with this event when it occurred. If specified, this
    /// value is stored in the OriginalTimestamp property of the PlayStream event.
    /// </summary>
    _Maybenull_ time_t const* originalTimestamp;

    /// <summary>
    /// (Optional) Arbitrary data associated with the event. Only one of Payload or PayloadJSON is allowed.
    /// </summary>
    PFJsonObject payload;

    /// <summary>
    /// (Optional) Arbitrary data associated with the event, represented as a JSON serialized string.
    /// Only one of Payload or PayloadJSON is allowed.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* payloadJSON;

} PFEventsEventContents;

/// <summary>
/// PFEventsWriteEventsRequest data model.
/// </summary>
typedef struct PFEventsWriteEventsRequest
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
    /// The collection of events to write. Up to 200 events can be written per request.
    /// </summary>
    _Field_size_(eventsCount) PFEventsEventContents const* const* events;

    /// <summary>
    /// Count of events
    /// </summary>
    uint32_t eventsCount;

} PFEventsWriteEventsRequest;

/// <summary>
/// PFEventsWriteEventsResponse data model.
/// </summary>
typedef struct PFEventsWriteEventsResponse
{
    /// <summary>
    /// (Optional) The unique identifiers assigned by the server to the events, in the same order as
    /// the events in the request. Only returned if FlushToPlayStream option is true.
    /// </summary>
    _Maybenull_ _Field_size_(assignedEventIdsCount) const char* const* assignedEventIds;

    /// <summary>
    /// Count of assignedEventIds
    /// </summary>
    uint32_t assignedEventIdsCount;

} PFEventsWriteEventsResponse;

#pragma pop_macro("IN")

}

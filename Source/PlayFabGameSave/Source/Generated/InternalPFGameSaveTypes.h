// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#if !defined(__cplusplus)
#error C++11 required
#endif

#pragma once

#include <playfab/core/PFPal.h>
#include <playfab/core/PFTypes.h>

namespace PlayFab
{

#pragma push_macro("IN")
#undef IN

/// <summary>
/// PFGameSaveDeleteManifestRequest data model. Request to delete a GameSave Manifest version.
/// </summary>
typedef struct PFGameSaveDeleteManifestRequest
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
    /// The version associated with the Manifest. A specific version is intended to be initialized and
    /// managed by a single client.
    /// </summary>
    _Null_terminated_ const char* version;

} PFGameSaveDeleteManifestRequest;

/// <summary>
/// PFGameSaveFinalizeManifestConflict data model.
/// </summary>
typedef struct PFGameSaveFinalizeManifestConflict
{
    /// <summary>
    /// The version of the manifest that was in conflict with this manifest.
    /// </summary>
    _Null_terminated_ const char* conflictingVersion;

    /// <summary>
    /// Whether this manifest was chosen to be the winner of the conflict.
    /// </summary>
    bool isWinner;

} PFGameSaveFinalizeManifestConflict;

/// <summary>
/// PFGameSaveFinalizedFileDetails data model.
/// </summary>
typedef struct PFGameSaveFinalizedFileDetails
{
    /// <summary>
    /// The name of the file being uploaded.
    /// </summary>
    _Null_terminated_ const char* fileName;

    /// <summary>
    /// The expected size, in bytes, of the file being uploaded.
    /// </summary>
    _Null_terminated_ const char* fileSizeBytes;

} PFGameSaveFinalizedFileDetails;

/// <summary>
/// PFGameSaveFinalizeManifestRequest data model. Request to finalize a GameSave Manifest version.
/// </summary>
typedef struct PFGameSaveFinalizeManifestRequest
{
    /// <summary>
    /// (Optional) The conflict resolution data for this Manifest.
    /// </summary>
    _Maybenull_ PFGameSaveFinalizeManifestConflict const* conflict;

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
    /// (Optional) The final set of files associated with this version of the Manifest.
    /// </summary>
    _Maybenull_ _Field_size_(filesToFinalizeCount) PFGameSaveFinalizedFileDetails const* const* filesToFinalize;

    /// <summary>
    /// Count of filesToFinalize
    /// </summary>
    uint32_t filesToFinalizeCount;

    /// <summary>
    /// Force the manifest to finalize when there is a newer version of the Manifest available. If force
    /// flag is not used and there is a newer version of the manifest the Finalize call will be blocked.
    /// </summary>
    bool force;

    /// <summary>
    /// (Optional) Contextual description of the progress within the gamesave files that are part of
    /// this Manifest version. This can be surfaced to players during conflict resolution or active device
    /// contention so that the players can make an informed choice to resolve the issue. .
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* manifestDescription;

    /// <summary>
    /// (Optional) A flag indicating that the version this Manifest is based on should be considered
    /// known good.
    /// </summary>
    _Maybenull_ bool const* markBaseAsKnownGood;

    /// <summary>
    /// The version associated with the Manifest. A specific version is intended to be initialized and
    /// managed by a single client.
    /// </summary>
    _Null_terminated_ const char* version;

} PFGameSaveFinalizeManifestRequest;

/// <summary>
/// PFGameSaveManifestConflict data model.
/// </summary>
typedef struct PFGameSaveManifestConflict
{
    /// <summary>
    /// (Optional) The version of the manifest that was in conflict with this manifest.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* conflictingVersion;

    /// <summary>
    /// The time that this conflict was resolved.
    /// </summary>
    time_t conflictResolutionTimestamp;

    /// <summary>
    /// Whether this manifest was chosen to be the winner of the conflict.
    /// </summary>
    bool isWinner;

    /// <summary>
    /// The time before which this manifest will not be deleted.
    /// </summary>
    time_t retainedUntilTimestamp;

} PFGameSaveManifestConflict;

/// <summary>
/// PFGameSaveFileMetadata data model.
/// </summary>
typedef struct PFGameSaveFileMetadata
{
    /// <summary>
    /// (Optional) The name of the file.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* fileName;

} PFGameSaveFileMetadata;

/// <summary>
/// PFGameSaveManifestMetadata data model.
/// </summary>
typedef struct PFGameSaveManifestMetadata
{
    /// <summary>
    /// The unique identifier of the device initializing the Manifest.
    /// </summary>
    _Null_terminated_ const char* deviceId;

    /// <summary>
    /// The name of the device initializing the Manifest.
    /// </summary>
    _Null_terminated_ const char* deviceName;

    /// <summary>
    /// The type of the device initializing the Manifest.
    /// </summary>
    _Null_terminated_ const char* deviceType;

    /// <summary>
    /// (Optional) The version of the device initializing the Manifest.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* deviceVersion;

} PFGameSaveManifestMetadata;

/// <summary>
/// PFGameSaveUploadProgress data model.
/// </summary>
typedef struct PFGameSaveUploadProgress
{
    /// <summary>
    /// The total bytes that will be uploaded across all files in this Manifest version.
    /// </summary>
    _Null_terminated_ const char* totalBytes;

    /// <summary>
    /// The current bytes uploaded across all files in this Manifest version.
    /// </summary>
    _Null_terminated_ const char* uploadedBytes;

} PFGameSaveUploadProgress;

/// <summary>
/// PFGameSaveManifest data model.
/// </summary>
typedef struct PFGameSaveManifest
{
    /// <summary>
    /// (Optional) The version of the Manifest that this Manifest is based on.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* baseVersion;

    /// <summary>
    /// (Optional) The conflict resolution data for this Manifest.
    /// </summary>
    _Maybenull_ PFGameSaveManifestConflict const* conflict;

    /// <summary>
    /// The creation time of this Manifest version.
    /// </summary>
    time_t creationTimestamp;

    /// <summary>
    /// (Optional) The list of files associated with this Manifest version.
    /// </summary>
    _Maybenull_ _Field_size_(filesCount) PFGameSaveFileMetadata const* const* files;

    /// <summary>
    /// Count of files
    /// </summary>
    uint32_t filesCount;

    /// <summary>
    /// (Optional) The finalization timestamp of this Manifest version.
    /// </summary>
    _Maybenull_ time_t const* finalizationTimestamp;

    /// <summary>
    /// Whether this Manifest was generated by a rollback operation.
    /// </summary>
    bool generatedByRollback;

    /// <summary>
    /// (Optional) A boolean indicating whether or not this manifest is verified and known to be good.
    /// </summary>
    _Maybenull_ bool const* isKnownGood;

    /// <summary>
    /// The timestamp at which this Manifest version was last updated.
    /// </summary>
    time_t lastUpdateTimestamp;

    /// <summary>
    /// (Optional) Contextual description of the progress within the gamesave files that are part of
    /// this Manifest version. This can be surfaced to players during conflict resolution or active device
    /// contention so that the players can make an informed choice to resolve the issue. .
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* manifestDescription;

    /// <summary>
    /// (Optional) The metadata associated with this Manifest version.
    /// </summary>
    _Maybenull_ PFGameSaveManifestMetadata const* metadata;

    /// <summary>
    /// (Optional) The reason for doing the rollback operation.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* rollbackReason;

    /// <summary>
    /// (Optional) The current status of this Manifest version.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* status;

    /// <summary>
    /// (Optional) The progress in upload for files associated with this Manifest version.
    /// </summary>
    _Maybenull_ PFGameSaveUploadProgress const* uploadProgress;

    /// <summary>
    /// (Optional) The version of the Manifest.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* version;

} PFGameSaveManifest;

/// <summary>
/// PFGameSaveFinalizeManifestResponse data model.
/// </summary>
typedef struct PFGameSaveFinalizeManifestResponse
{
    /// <summary>
    /// (Optional) The updated Manifest associated with the request.
    /// </summary>
    _Maybenull_ PFGameSaveManifest const* manifest;

} PFGameSaveFinalizeManifestResponse;

/// <summary>
/// PFGameSaveGetConfigForTitleRequest data model. Request to get a the game save configuration for a
/// title.
/// </summary>
typedef struct PFGameSaveGetConfigForTitleRequest
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

} PFGameSaveGetConfigForTitleRequest;

/// <summary>
/// PFGameSaveGetConfigForTitleResponse data model.
/// </summary>
typedef struct PFGameSaveGetConfigForTitleResponse
{
    /// <summary>
    /// Prevent game clients from reversing the most recent conflict resolution decision. Support and
    /// title management tools, including Game Manager, will always be able to reverse the most recent
    /// conflict resolution decision.
    /// </summary>
    bool disableClientRollbackToLastConflictLoser;

    /// <summary>
    /// Prevent game clients from initiating rollback to the last known good version. Support and title
    /// management tools, including Game Manager, will always be able to revert a corrupt save to the
    /// last known good version.
    /// </summary>
    bool disableClientRollbackToLastKnownGood;

    /// <summary>
    /// Prevent Gaming Runtime Services, where available outside of game runtime, from uploading the
    /// current state of the local game save after a game is suspended, even if the game did not provide
    /// an explicit signal to do so.
    /// </summary>
    bool disableUnsignaledOutOfProcessUpload;

    /// <summary>
    /// (Optional) The title's onboarding status.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* onboardingStatus;

    /// <summary>
    /// (Optional) How much data, in bytes, can be stored for each player in an individual game save.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* perPlayerQuotaBytes;

} PFGameSaveGetConfigForTitleResponse;

/// <summary>
/// PFGameSaveGetManifestDownloadDetailsRequest data model. Request to provide details to the download
/// the files associated with this Manifest version.
/// </summary>
typedef struct PFGameSaveGetManifestDownloadDetailsRequest
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
    /// The version associated with the Manifest. A specific version is intended to be initialized and
    /// managed by a single client.
    /// </summary>
    _Null_terminated_ const char* version;

} PFGameSaveGetManifestDownloadDetailsRequest;

/// <summary>
/// PFGameSaveFileDownloadDetails data model.
/// </summary>
typedef struct PFGameSaveFileDownloadDetails
{
    /// <summary>
    /// The time at which the access token in the download url expires.
    /// </summary>
    time_t accessTokenExpirationTime;

    /// <summary>
    /// (Optional) The URL to download the file from (the url contains the authentication tokens).
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* downloadUrl;

    /// <summary>
    /// (Optional) The name of the file being downloaded.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* fileName;

} PFGameSaveFileDownloadDetails;

/// <summary>
/// PFGameSaveGetManifestDownloadDetailsResponse data model.
/// </summary>
typedef struct PFGameSaveGetManifestDownloadDetailsResponse
{
    /// <summary>
    /// The list of files associated with this Manifest version.
    /// </summary>
    _Field_size_(filesCount) PFGameSaveFileDownloadDetails const* const* files;

    /// <summary>
    /// Count of files
    /// </summary>
    uint32_t filesCount;

} PFGameSaveGetManifestDownloadDetailsResponse;

/// <summary>
/// PFGameSaveGetQuotaForPlayerRequest data model. Request to get the total and available quota for a
/// Player's Gamesave data.
/// </summary>
typedef struct PFGameSaveGetQuotaForPlayerRequest
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

} PFGameSaveGetQuotaForPlayerRequest;

/// <summary>
/// PFGameSaveGetQuotaForPlayerResponse data model.
/// </summary>
typedef struct PFGameSaveGetQuotaForPlayerResponse
{
    /// <summary>
    /// (Optional) AvailableBytes property.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* availableBytes;

    /// <summary>
    /// (Optional) TotalBytes property.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* totalBytes;

} PFGameSaveGetQuotaForPlayerResponse;

/// <summary>
/// PFGameSaveInitializeManifestRequest data model. Request to initialize a new GameSave Manifest version.
/// </summary>
typedef struct PFGameSaveInitializeManifestRequest
{
    /// <summary>
    /// (Optional) The version that this manifest is based on.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* baseVersion;

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
    /// The metadata associated with the Manifest.
    /// </summary>
    PFGameSaveManifestMetadata const* metadata;

    /// <summary>
    /// The version associated with the Manifest. A specific version is intended to be initialized and
    /// managed by a single client.
    /// </summary>
    _Null_terminated_ const char* version;

} PFGameSaveInitializeManifestRequest;

/// <summary>
/// PFGameSaveInitializeManifestResponse data model.
/// </summary>
typedef struct PFGameSaveInitializeManifestResponse
{
    /// <summary>
    /// (Optional) The updated Manifest associated with the request.
    /// </summary>
    _Maybenull_ PFGameSaveManifest const* manifest;

} PFGameSaveInitializeManifestResponse;

/// <summary>
/// PFGameSaveFileToUpload data model.
/// </summary>
typedef struct PFGameSaveFileToUpload
{
    /// <summary>
    /// The name of the file being uploaded.
    /// </summary>
    _Null_terminated_ const char* fileName;

} PFGameSaveFileToUpload;

/// <summary>
/// PFGameSaveInitiateUploadRequest data model. Request to get the upload details for files being uploaded
/// to this Manifest version.
/// </summary>
typedef struct PFGameSaveInitiateUploadRequest
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
    /// The list of files being upload to this Manifest version.
    /// </summary>
    _Field_size_(filesCount) PFGameSaveFileToUpload const* const* files;

    /// <summary>
    /// Count of files
    /// </summary>
    uint32_t filesCount;

    /// <summary>
    /// The version associated with the Manifest. A specific version is intended to be initialized and
    /// managed by a single client.
    /// </summary>
    _Null_terminated_ const char* version;

} PFGameSaveInitiateUploadRequest;

/// <summary>
/// PFGameSaveAllocatedFile data model.
/// </summary>
typedef struct PFGameSaveAllocatedFile
{
    /// <summary>
    /// The time at which the access token in the upload url expires.
    /// </summary>
    time_t accessTokenExpirationTime;

    /// <summary>
    /// (Optional) The name of the file being uploaded.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* fileName;

    /// <summary>
    /// (Optional) The URL to upload the file to, including the authentication token.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* uploadUrl;

} PFGameSaveAllocatedFile;

/// <summary>
/// PFGameSaveInitiateUploadResponse data model.
/// </summary>
typedef struct PFGameSaveInitiateUploadResponse
{
    /// <summary>
    /// (Optional) The upload details for the files.
    /// </summary>
    _Maybenull_ _Field_size_(filesCount) PFGameSaveAllocatedFile const* const* files;

    /// <summary>
    /// Count of files
    /// </summary>
    uint32_t filesCount;

    /// <summary>
    /// A recommended chunk size in bytes for uploading files to the cloud. This provides a hint to the
    /// client on how to chunk the file for upload.
    /// </summary>
    int32_t recommendedChunkSizeBytes;

} PFGameSaveInitiateUploadResponse;

/// <summary>
/// PFGameSaveListManifestsRequest data model. Request to list the GameSave Manifests.
/// </summary>
typedef struct PFGameSaveListManifestsRequest
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
    /// A flag indicating whether to include unavailable (e.g. PendingDeletion or Quarantined) manifests
    /// in the response. These are not available as starting points for new game saves without developer
    /// intervention.
    /// </summary>
    bool includeUnavailable;

} PFGameSaveListManifestsRequest;

/// <summary>
/// PFGameSaveTitleConfiguration data model.
/// </summary>
typedef struct PFGameSaveTitleConfiguration
{
    /// <summary>
    /// Prevent game clients from reversing the most recent conflict resolution decision. Support and
    /// title management tools, including Game Manager, will always be able to reverse the most recent
    /// conflict resolution decision.
    /// </summary>
    bool disableClientRollbackToLastConflictLoser;

    /// <summary>
    /// Prevent game clients from initiating rollback to the last known good version. Support and title
    /// management tools, including Game Manager, will always be able to revert a corrupt save to the
    /// last known good version.
    /// </summary>
    bool disableClientRollbackToLastKnownGood;

    /// <summary>
    /// Prevent Gaming Runtime Services, where available outside of game runtime, from uploading the
    /// current state of the local game save after a game is suspended, even if the game did not provide
    /// an explicit signal to do so.
    /// </summary>
    bool disableUnsignaledOutOfProcessUpload;

    /// <summary>
    /// (Optional) How much data, in bytes, can be stored for each player in an individual game save.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* perPlayerQuotaBytes;

} PFGameSaveTitleConfiguration;

/// <summary>
/// PFGameSaveListManifestsResponse data model.
/// </summary>
typedef struct PFGameSaveListManifestsResponse
{
    /// <summary>
    /// (Optional) The list of Manifest versions.
    /// </summary>
    _Maybenull_ _Field_size_(manifestsCount) PFGameSaveManifest const* const* manifests;

    /// <summary>
    /// Count of manifests
    /// </summary>
    uint32_t manifestsCount;

    /// <summary>
    /// (Optional) The next available manifest version to initialize.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* nextAvailableVersion;

    /// <summary>
    /// (Optional) The gamesave configuration for the title.
    /// </summary>
    _Maybenull_ PFGameSaveTitleConfiguration const* titleConfiguration;

} PFGameSaveListManifestsResponse;

/// <summary>
/// PFGameSaveRequestOnboardingRequest data model. Request to ask to onboard the title to the game save
/// service.
/// </summary>
typedef struct PFGameSaveRequestOnboardingRequest
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

} PFGameSaveRequestOnboardingRequest;

/// <summary>
/// PFGameSaveRollbackToManifestRequest data model. Request to roll back a player's game save to the
/// state in the specified version. Creates and returns a new latest manifest that contains identical
/// data to that version.
/// </summary>
typedef struct PFGameSaveRollbackToManifestRequest
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
    /// (Optional) The reason for the roll back.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* rollbackReason;

    /// <summary>
    /// The manifest version to roll back the game save state to.
    /// </summary>
    _Null_terminated_ const char* rollbackVersion;

} PFGameSaveRollbackToManifestRequest;

/// <summary>
/// PFGameSaveRollbackToManifestResponse data model.
/// </summary>
typedef struct PFGameSaveRollbackToManifestResponse
{
    /// <summary>
    /// (Optional) The updated Manifest associated with the request.
    /// </summary>
    _Maybenull_ PFGameSaveManifest const* manifest;

} PFGameSaveRollbackToManifestResponse;

/// <summary>
/// PFGameSaveUpdateConfigForTitleRequest data model. Request to update a title's configuration in the
/// game save service.
/// </summary>
typedef struct PFGameSaveUpdateConfigForTitleRequest
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
    /// (Optional) Prevent game clients from reversing the most recent conflict resolution decision.
    /// Support and title management tools, including Game Manager, will always be able to reverse the
    /// most recent conflict resolution decision.
    /// </summary>
    _Maybenull_ bool const* disableClientRollbackToLastConflictLoser;

    /// <summary>
    /// (Optional) Prevent game clients from initiating rollback to the last known good version. Support
    /// and title management tools, including Game Manager, will always be able to revert a corrupt save
    /// to the last known good version.
    /// </summary>
    _Maybenull_ bool const* disableClientRollbackToLastKnownGood;

    /// <summary>
    /// (Optional) Prevent Gaming Runtime Services, where available outside of game runtime, from uploading
    /// the current state of the local game save after a game is suspended, even if the game did not provide
    /// an explicit signal to do so.
    /// </summary>
    _Maybenull_ bool const* disableUnsignaledOutOfProcessUpload;

    /// <summary>
    /// (Optional) The optional entity to perform this action on. Defaults to the currently logged in
    /// entity.
    /// </summary>
    _Maybenull_ PFEntityKey const* entity;

} PFGameSaveUpdateConfigForTitleRequest;

/// <summary>
/// PFGameSaveUpdateConfigForTitleResponse data model.
/// </summary>
typedef struct PFGameSaveUpdateConfigForTitleResponse
{
    /// <summary>
    /// Prevent game clients from reversing the most recent conflict resolution decision. Support and
    /// title management tools, including Game Manager, will always be able to reverse the most recent
    /// conflict resolution decision.
    /// </summary>
    bool disableClientRollbackToLastConflictLoser;

    /// <summary>
    /// Prevent game clients from initiating rollback to the last known good version. Support and title
    /// management tools, including Game Manager, will always be able to revert a corrupt save to the
    /// last known good version.
    /// </summary>
    bool disableClientRollbackToLastKnownGood;

    /// <summary>
    /// Prevent Gaming Runtime Services, where available outside of game runtime, from uploading the
    /// current state of the local game save after a game is suspended, even if the game did not provide
    /// an explicit signal to do so.
    /// </summary>
    bool disableUnsignaledOutOfProcessUpload;

    /// <summary>
    /// (Optional) The title's onboarding status.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* onboardingStatus;

    /// <summary>
    /// (Optional) How much data, in bytes, can be stored for each player in an individual game save.
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* perPlayerQuotaBytes;

} PFGameSaveUpdateConfigForTitleResponse;

/// <summary>
/// PFGameSaveUpdateManifestRequest data model. Request to update a GameSave Manifest version.
/// </summary>
typedef struct PFGameSaveUpdateManifestRequest
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
    /// (Optional) Contextual description of the progress within the gamesave files that are part of
    /// this Manifest version. This can be surfaced to players during conflict resolution or active device
    /// contention so that the players can make an informed choice to resolve the issue. .
    /// </summary>
    _Maybenull_ _Null_terminated_ const char* manifestDescription;

    /// <summary>
    /// (Optional) A flag indicating that the specified manifest was verified and should be marked as
    /// known good. Can only be set on a Finalized manifest.
    /// </summary>
    _Maybenull_ bool const* markAsKnownGood;

    /// <summary>
    /// (Optional) The progress in upload for files associated with this Manifest version.
    /// </summary>
    _Maybenull_ PFGameSaveUploadProgress const* uploadProgress;

    /// <summary>
    /// The version associated with the Manifest. A specific version is intended to be initialized and
    /// managed by a single client.
    /// </summary>
    _Null_terminated_ const char* version;

} PFGameSaveUpdateManifestRequest;

/// <summary>
/// PFGameSaveUpdateManifestResponse data model.
/// </summary>
typedef struct PFGameSaveUpdateManifestResponse
{
    /// <summary>
    /// (Optional) The updated Manifest associated with the request.
    /// </summary>
    _Maybenull_ PFGameSaveManifest const* manifest;

} PFGameSaveUpdateManifestResponse;

#pragma pop_macro("IN")

}

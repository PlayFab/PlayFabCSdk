// Copyright (C) Microsoft Corporation. All rights reserved.
#pragma once

extern "C"
{

// These functions can be used only for debugging.

// Force the in-process (Win32) provider instead of GRTS/PFX.
// Must be called BEFORE PFGameSaveFilesInitialize.
PF_API PFGameSaveFilesSetForceInprocForDebug(_In_ bool forceInproc);

PF_API PFGameSaveFilesSetMockDeviceIdForDebug(_In_ const char* deviceId);
PF_API PFGameSaveFilesSetMockRootFolderForDebug(_In_ const char* rootFolder);
PF_API PFGameSaveFilesSetMockManifestOffsetForDebug(_In_ size_t offset);
// The mock service's cloud folder: every service call and telemetry go to GameSaveServiceMock. Null or
// "" switches back to the real service path (the generated GameSave API and the real telemetry
// pipeline), which tests drive with libHttpClient mocks (core unit test plan H-6).
PF_API PFGameSaveFilesSetMockDataFolderForDebug(_In_opt_ const char* mockDataFolder);
PF_API PFGameSaveFilesSetForceOutOfStorageErrorForDebug(_In_ bool forceError);
PF_API PFGameSaveFilesSetForceSyncFailedErrorForDebug(_In_ bool forceError);
PF_API PFGameSaveFilesSetForceNullPendingManifestForDebug(_In_ bool force);
PF_API PFGameSaveFilesSetWriteManifestsToDiskForDebug(_In_ bool writeManifests);
// Forces the next active-device poll to report a change (one-shot). Cleared when GameSave uninitializes
// if no poll consumed it.
PF_API PFGameSaveFilesSetActiveDevicePollForceChangeForDebug();
PF_API PFGameSaveFilesSetActiveDevicePollIntervalForDebug(_In_ uint32_t interval);

PF_API PFGameSaveFilesPauseUploadForDebug();
PF_API PFGameSaveFilesResumeUploadForDebug();

// Simulates another process filling the disk while a sync is running. After bytesBeforeFull more
// bytes of SDK file writes, writes fail as they do on a full volume and the reported free space is
// 0. Pass enable=false to end the simulation (the equivalent of the user freeing space). Can be
// called before PFGameSaveFilesInitialize. Returns E_NOTIMPL when the file PAL does not support
// disk-space simulation, including requests to disable it.
PF_API PFGameSaveFilesSetSimulatedDiskFullForDebug(_In_ bool enable, _In_ uint64_t bytesBeforeFull);

// Simulates a volume that is already nearly full: the reported free space is freeBytes (less what
// the SDK writes afterwards), and writes past it fail as on a full disk. Ended by either this call or
// PFGameSaveFilesSetSimulatedDiskFullForDebug with enable=false. Can be called before initialization;
// returns E_NOTIMPL for both enable and disable when the file PAL lacks disk-space simulation.
PF_API PFGameSaveFilesSetSimulatedDiskFreeSpaceForDebug(_In_ bool enable, _In_ uint64_t freeBytes);

enum class GameSaveServiceMockForcedOffline
{
    None = 0,

    // Download endpoints
    ListManifests,
    InitializeManifest,
    GetManifestDownloadDetails,
    DownloadFile,

    // Upload endpoints
    InitiateUpload,
    FinalizeManifest,
    UpdateManifest,
    UploadFile,

    // The next FinalizeManifest fails once with E_PF_INVALID_PARAMS (the service's answer to an
    // over-cap or otherwise invalid file list), then the mode resets to None.
    FinalizeManifestInvalidParamsOnce,

    // The next FinalizeManifest fails once with E_PF_GAME_SAVE_NEWER_MANIFEST_EXISTS (another device
    // took over), then the mode resets to None.
    FinalizeManifestNewerManifestExistsOnce,
};

PF_API PFGameSaveFilesSetMockForceOfflineForDebug(_In_ GameSaveServiceMockForcedOffline mode);

// The six exports below (title-config override, simulated separate metadata storage, fault injection,
// download URL expiry) are compiled only in debug builds of the SDK: release builds return E_NOTIMPL
// and have no hooks on the paths they affect.

// Debug only: overrides TitleConfiguration values the test service can't set. titleConfigJson is
// an object; supported key:
//   "PerPlayerQuotaBytes"  number > 0 (anything else is E_INVALIDARG)
// Other keys are ignored. nullptr or "{}" clears the override. PerPlayerQuotaBytes applies from the
// next query (GetRemainingQuota reads it live). Call after PFGameSaveFilesInitialize; Uninitialize
// clears it.
PF_API PFGameSaveFilesSetTitleConfigOverrideForDebug(_In_opt_z_ const char* titleConfigJson) noexcept;

// Debug only: simulates a platform whose cloud sync metadata (localstate.json) is stored apart from
// game storage, so the game storage can be wiped on its own. While enabled, the in-proc provider
// reports HasSeparateMetadataStorage() true and IsGameStorageWiped() true when the game-storage
// marker (<save folder>\cloudsync\pfgs_marker) is missing. Uninitialize resets it.
PF_API PFGameSaveFilesSetSimulatedSeparateMetadataStorageForDebug(_In_ bool enable) noexcept;

// Debug only: fault injection at a named point of an operation (core unit test plan H-3, H-5, H-8).
// Points:
//   "AddUser.Start"                 every pump of an AddUser, before the lock (Pause holds it in flight)
//   "ResetCloud.Start"              every pump of a ResetCloudAsync, before any work (Pause holds it in
//                                   flight; a held reset ends E_ABORT when cancelled)
//   "Service.ListManifests", "Service.InitializeManifest", "Service.GetManifestDownloadDetails",
//   "Service.DownloadFile", "Service.InitiateUpload", "Service.UploadFile", "Service.FinalizeManifest",
//   "Service.UpdateManifest", "Service.DeleteManifest"
//                                   GameSaveServiceMock only, at the start of the mock service call.
//                                   DownloadFile: the extended manifest and every bundle (and the mock's
//                                   own manifests.json reads); UploadFile: every blob upload (bundles,
//                                   the thumbnail and the extended manifest). Fail(hr) fails that call
//                                   with hr. Pause holds the call in flight like a slow HTTP request: it
//                                   runs once resumed, or completes E_ABORT when the SDK operation is
//                                   cancelled (XAsyncCancel, uninitialize).
//   "Login"                         AddUser, before the player's sign-in. Fail(hr) is reported like a
//                                   sign-in failure (the SyncFailed prompt, or the AddUser fails with hr
//                                   when no callback is set).
//   "Upload.BeforeCompress"         an upload, after the compare, before anything is staged
//   "Upload.BeforeTransfer"         an upload, before each blob transfer (bundles, then the extended manifest)
//   "Upload.BeforeFinalize"         an upload, before FinalizeManifest
//   "Upload.AfterFinalize"          an upload, after FinalizeManifest succeeded, before localstate.json is
//                                   written (Fail/Abandon only: Fail(hr) fails like that write would)
//   "Upload.TakeLock"               an upload, before the lock hand-off (keep or release)
//   "Download.AfterLocalOps"        AddUser, after the local deletes and folder creates, before any transfer
//   "Download.BeforeExtract"        AddUser, after the bundles were downloaded, before they are extracted
//   "Download.BeforeLocalManifest"  AddUser, after extraction, before localstate.json is written
//                                   For the step points, Fail(hr) fails the operation there with hr,
//                                   Abandon stops it there (E_ABORT, staging left behind) and Pause holds
//                                   it there until resumed.
//   "Telemetry.CreatePipeline"      the telemetry pipeline's creation at the start of every activation:
//                                   Fail(hr) makes it fail with hr; no pipeline is created, so the
//                                   operation's events are skipped.
//   "Telemetry.Emit"                every telemetry event: Fail(hr) makes the pipeline refuse it with hr.
//   "Api.RunProvider"               AddUserWithUiAsync / UploadWithUiAsync: Fail(hr) fails the provider's
//                                   start the way XAsyncProviderBase::Run does (the provider is destroyed).
//   "Api.AfterRunFailed"            after such a failed start: Pause holds the API call (bounded, 30 s)
//                                   until resumed.
// Actions: None clears the point. Fail(hr) fails the operation at the point once with hr. Abandon
// stops the operation at the point once without any cleanup or rollback (a simulated crash; the
// operation completes with E_ABORT and leaves its staging folders behind for the next activation).
// Pause holds the operation at the point until PFGameSaveFilesResumeFaultPointForDebug (cancel,
// uninitialize or sign-out still work while held).
// Count leaves the operation alone and only counts the hits (a call counter for "Service.*" points).
// FailAlways fails every hit with hr (not one-shot) until the point is cleared with None.
// PauseOnce ("Service.*" points only) holds only the first call that reaches the point; later calls
// go through while it is held. PFGameSaveFilesResumeFaultPointForDebug releases it.
// Uninitialize clears every point. Until a point has been set in the process, a fault point costs one
// atomic load.
enum class PFGameSaveFaultAction : uint32_t
{
    None = 0,
    Fail = 1,
    Abandon = 2,
    Pause = 3,
    Count = 4,
    FailAlways = 5,
    PauseOnce = 6,
};

PF_API PFGameSaveFilesSetFaultInjectionForDebug(_In_z_ const char* point, _In_ PFGameSaveFaultAction action, _In_ HRESULT hr) noexcept;
PF_API PFGameSaveFilesResumeFaultPointForDebug(_In_z_ const char* point) noexcept;
// How many times an operation reached `point` since it was set (a paused operation counts once per
// pump, so > 0 means "held there").
PF_API PFGameSaveFilesGetFaultPointHitCountForDebug(_In_z_ const char* point, _Out_ uint32_t* hitCount) noexcept;

// Debug only: simulates expiring download links (SAS). Links are issued by every successful
// GetManifestDownloadDetails; a blob or extended-manifest download through an expired link fails
// with HTTP 403 (HTTP_E_STATUS_FORBIDDEN), on the mock and on the real service alike.
//   Off              links never expire (default)
//   ExpireNow        the next download finds its link expired (one-shot; then Off)
//   LifetimeSeconds  a link expires `seconds` after it was issued
//   ExpireIssued     every link issued so far is expired; links issued after this call are valid
// Uninitialize resets it to Off. Returns E_INVALIDARG for an unknown mode.
enum class PFGameSaveUrlExpiryMode : uint32_t
{
    Off = 0,
    ExpireNow = 1,
    LifetimeSeconds = 2,
    ExpireIssued = 3,
};

PF_API PFGameSaveFilesSetDownloadUrlExpiryForDebug(_In_ PFGameSaveUrlExpiryMode mode, _In_ uint32_t seconds) noexcept;

// These functions can be used only for debugging and will only return data in DEBUG builds
PF_API PFGameSaveFilesGetStatsJsonSizeForDebug(_In_ PFLocalUserHandle localUserHandle, _Out_ size_t* jsonSize) noexcept;
PF_API PFGameSaveFilesGetStatsJsonForDebug(_In_ PFLocalUserHandle localUserHandle, _In_ size_t jsonSize, _Out_writes_(jsonSize) char* jsonBuffer, _Out_opt_ size_t* jsonSizeUsed);

PF_API PFGameSaveFilesGetSaveDescriptionSizeForDebug(_In_ PFLocalUserHandle localUserHandle, _Out_ size_t* descriptionSize) noexcept;
PF_API PFGameSaveFilesGetSaveDescriptionForDebug(_In_ PFLocalUserHandle localUserHandle, _In_ size_t descriptionSize, _Out_writes_(descriptionSize) char* descriptionBuffer, _Out_opt_ size_t* descriptionSizeUsed) noexcept;

}
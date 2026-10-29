# PFGameSave Source Code Map

Quick-reference for navigating `Source/PlayFabGameSave/`. Files are grouped by architectural layer, top-down.

## Include/playfab/gamesave/ (Public Headers)

| File | Contents |
|------|----------|
| `PFGameSaveFiles.h` | All public C API declarations (Init, AddUser, Upload, GetFolder, Quota, ResetCloud, SetDescription, ActiveDeviceChanged, Uninit) |
| `PFGameSaveFilesTypes.h` | Public enums (`PFGameSaveInitOptions`, `PFGameSaveFilesAddUserOptions`, `PFGameSaveFilesSyncState`, `PFGameSaveFilesUploadOption`), `PFGameSaveInitArgs`, `PFGameSaveDescriptor`, error codes (`E_PF_GAMESAVE_*`) |
| `PFGameSaveFilesUi.h` | UI callback typedefs (Progress, SyncFailed, ActiveDeviceContention, Conflict, OutOfStorage), `PFGameSaveUICallbacks` struct, response enums and `SetUi*Response` APIs |

## Source/Api/ (API Entry Points)

| File | Role |
|------|------|
| `PFGameSaveFilesAPI.cpp` | Implements every public `PF_API` function. Routes calls through `GameSaveGlobalState` to `FolderSyncManager` and async providers. This is the top of the call stack for all public APIs. |
| `PFGameSaveFilesForDebug.h` | Debug-only APIs: mock device/folder overrides, forced errors, active device poll control, upload pause/resume, forced-offline per endpoint |

## Source/Providers/ (Async Providers)

Each provider wraps a `FolderSyncManager` workflow step into an `XAsyncBlock`-compatible operation.

| File | Class | Wraps |
|------|-------|-------|
| `DownloadAsyncProvider.h/cpp` | `DownloadAsyncProvider` | `FolderSyncManager::DoWorkFolderDownload` |
| `UploadAsyncProvider.h/cpp` | `UploadAsyncProvider` | `FolderSyncManager::DoWorkFolderUpload` |
| `FileResetCloudAsyncProvider.h/cpp` | `FileResetCloudAsyncProvider` | `FolderSyncManager::DoWorkResetCloud` |
| `SetSaveDescriptionProvider.h/cpp` | `SetSaveDescriptionProvider` | `FolderSyncManager::DoWorkSetSaveDescription` |

## Source/Common/ (Shared Infrastructure)

| File | Class/Functions | Role |
|------|-----------------|------|
| `GameSaveGlobalState.h/cpp` | `GameSaveGlobalState` | Singleton owning all state. Creates/finds `FolderSyncManager` per user. Holds debug overrides, device ID, run context. Implements `ITerminationListener` for cleanup. |
| `UICallbackManager.h/cpp` | `UICallbackManager` | Fires UI callbacks on the caller's thread via task queue, blocks state machine until user responds via `SetUi*Response`. Each callback type has its own wait/signal pair. |
| `GameSaveHttpClient.h/cpp` | `GameSaveHttpClient` (static) | Makes HTTP calls to PlayFab GameSave service endpoints. Handles auth token injection, JSON parsing, error mapping. |
| `GameSaveTelemetryManager.h/cpp` | `GameSaveTelemetryManager` | Emits telemetry events for sync operations (activation, upload, download, errors) via PFCore EventPipeline. |
| `GameSavePlatform.h/cpp` | `GameSaveInitializePlatformHooks()` | One-time platform hook setup (local user change callbacks from PFCore). |
| `GameSaveUICallbackInfo.h/cpp` | `GameSaveUiCallbackInfo` | Static storage for UI callback function pointers set by `PFGameSaveFilesSetUiCallbacks`. |
| `ApiHelpers.h/cpp` | Template helpers | Boilerplate for getting global state, looking up FolderSyncManager, validating user handles. |
| `ZipUtils.h/cpp` | `ZipUtils` (static) | Compresses files into ZIP archives for upload, decompresses downloaded ZIPs. Uses libarchive. |
| `Utils.h/cpp` | `CreateGUID()`, `ReadEntireFile()`, `WriteEntireFile()`, path helpers, ISO8601 time conversion | File I/O, GUID generation, string utilities, debug tracing macros. |
| `ProgressHelpers.h/cpp` | `InnerProgressContext`, `ProgressCallback` | Progress tracking context passed to HTTP upload/download for byte-level progress reporting. |
| `Metadata.h` | `GetDeviceType()`, `GetDeviceFriendlyName()` | Platform-specific device metadata (implemented per-platform in Platform/ dirs). |
| `ISchedulableTask.h` | `ISchedulableTask` interface | Abstract interface for schedulable work items; used by step classes to schedule next iteration. |

## Source/SyncManager/ (Workflow State Machines)

The core sync logic. `FolderSyncManager` orchestrates these steps in sequence.

| File | Class | State Enum | Role |
|------|-------|------------|------|
| `FolderSyncManager.h/cpp` | `FolderSyncManager` | N/A (orchestrator) | Per-user sync orchestrator. Owns all step objects, manifests, file/folder sets. Routes `DoWorkFolderDownload`/`DoWorkFolderUpload` through the step sequence. |
| `LockStep.h/cpp` | `LockStep` | `LockStage` (Login, ListManifests, SelectBaselineAndCheckContention, CreatePendingManifest, WaitForActiveDeviceContentionUI, ..., LockDone) | First step in download. Logs in, lists manifests, selects baseline (with rollback support), detects active device contention, creates pending manifest. |
| `CompareStep.h/cpp` | `CompareStep` | `CompareStage` (GetManifestDownloadDetails, GetExtendedManifest, ReadLocalManifest, WaitForConflictUI, ..., CompareDone) | Fetches extended manifest, scans local files, marks files for upload/download/delete, detects conflicts at root-subfolder level, triggers conflict UI. |
| `DownloadStep.h/cpp` | `DownloadStep` | `DownloadStage` (DownloadStart, QueryStorage, Download, UncompressFiles, UpdateLocalManifest, ..., DownloadDone) | Downloads compressed files from Azure, checks disk space, uncompresses into save folder, updates local state manifest. |
| `UploadStep.h/cpp` | `UploadStep` | `UploadStage` (UploadStart, CompressFiles, InitiateUpload, UploadFile, FinalizeManifest, ListManifestsAfterUpload, PromoteIfNeeded, TakeLock, ..., UploadDone) | Compresses changed files into ZIP bundles, uploads via Azure URLs, finalizes manifest, handles known-good promotion and re-lock after upload. |
| `ResetCloudStep.h/cpp` | `ResetCloudStep` | `ResetCloudStage` (Login, ListManifests, DeleteManifests, ..., ResetCloudDone) | Dev/test utility. Logs in, lists all manifests, deletes them all. |
| `SetSaveDescriptionStep.h/cpp` | `SetSaveDescriptionStep` | `SetSaveDescriptionStage` (Start, ..., Done) | Updates pending manifest with short save description via service call. |
| `ActiveDevicePollWorker.h/cpp` | `ActiveDevicePollWorker` | N/A (timer-based) | Background timer that polls service to detect if another device took active status. Fires `PFGameSaveFilesActiveDeviceChangedCallback` if detected. |

## Source/Types/ (Data Models)

| File | Class/Struct | Role |
|------|--------------|------|
| `FileFolderSet.h/cpp` | `FileFolderSet` | Central data structure holding all known files and folders (local + remote). Methods to mark files for upload, download, delete, create. Used by CompareStep to determine sync operations. |
| `FileFolderSetTypes.h` | `FileDetail`, `FolderDetail`, `CompressedFile` | Core structs. `FileDetail` tracks file ID, size, timestamps, last-sync state, skip flag, thumbnail flag. `FolderDetail` tracks folder ID, path, local/remote existence. |
| `ExtendedManifest.h/cpp` | `ExtendedManifest` (static) | Reads/writes the `extended-<ver>-manifest.json` file. Populates `FileFolderSet` from JSON. Generates JSON from file/folder state for upload. |
| `ExtendedManifestTypes.h` | `ExtendedManifestCompressedFileDetail`, `ExtendedManifestExtractedFileDetail`, `ExtendedManifestNestedFolder`, `CompressionType` | Type definitions for extended manifest data structures. |
| `Manifest.h/cpp` | `ManifestInternal` | Wraps a service manifest (from ListManifests). Provides version, status, download details. Bridges service types to internal types. |
| `LocalStateManifest.h/cpp` | `LocalStateManifest` (static), `FileFolderSet::InitWithLocalFilesAndFolders` | Reads/writes `cloudsync/localstate.json`. Tracks per-file last-sync timestamps for conflict detection. Also stores `Metadata` section (shortSaveDescription, descriptionDirty). |
| `InfoManifest.h/cpp` | `InfoManifestData` | Reads/writes `cloudsync/info-manifest.json`. Stores device ID for this install. |

## Source/Generated/ (Code-Generated Service Types)

| File | Role |
|------|------|
| `CacheId.h` | Cache ID enum for service endpoint routing |
| `GameSave.h/cpp` | `GameSaveAPI` static class -- typed wrappers for service calls (DeleteManifest, FinalizeManifest, GetManifestDownloadDetails, InitializeManifest, InitiateUpload, ListManifests, UpdateManifest) |
| `GameSaveTypes.h/cpp` | Request/response model classes for each service endpoint |
| `GameSaveTypeWrappers.h` | RAII wrapper types for service models |
| `InternalPFGameSave.h/cpp` | Internal C API binding layer |
| `InternalPFGameSaveTypes.h` | Internal type definitions |

## Source/Wrappers/ (Type Adaptation)

| File | Class | Role |
|------|-------|------|
| `GameSaveServiceSelector.h/cpp` | Type aliases (`ManifestWrap`, `ManifestWrapVector`, etc.) | Defines convenient typedefs for wrapped service types. Selects mock vs real service. |
| `GameSaveServiceMock.h/cpp` | `GameSaveServiceMock` | Mock service for testing. Can force specific endpoints offline. |
| `LocalUserLoginOperation.h/cpp` | `LocalUserLoginOperation`, `LoginResult` | Async login helper. Handles PFAuthenticationLoginWithCustomIDAsync and entity handle extraction. |
| `CoreTypes.h/cpp` | `EntityKey`, `PFOperationTypes` | Core wrapper types and JSON serialization. |
| `Types.h` | Type aliases (Entity, ServiceConfig, EventPipeline) | Aliases PFCore wrapper types with custom allocator. |

## Source/Platform/ (Platform Providers)

| File | Class | Role |
|------|-------|------|
| `PFGameSaveFilesAPIProvider.h` | `GameSaveAPIProvider` (abstract) | Interface for platform-specific implementations. Methods: AddUser, Upload, GetFolder, IsConnectedToCloud, Uninit, etc. |
| `Platform.h` | `PlatformGetAPIProvider()` | Factory: returns Win32 or GDK provider based on build. |
| `Windows/PFGameSaveFilesAPIProvider_Win32.h/cpp` | `GameSaveAPIProviderWin32` | In-process provider. Delegates to `FolderSyncManager` via async providers. Used for non-Xbox platforms. |
| `Windows/PFGameSaveFilesAPIProvider_GRTS.h/cpp` | `GameSaveAPIProviderGRTS` | Out-of-process provider. Delegates to Xbox Gaming Runtime Services (xgameruntime). Upload happens out-of-process after game exits. |
| `Windows/Metadata_Win32.cpp` | Device metadata (Win32) | Returns "WindowsDesktop" device type, computer name, etc. |
| `Windows/Platform_Win32.cpp` | Platform init (Win32) | Creates Win32 or GRTS provider based on `ForceInproc` flag. |
| `GDK/Metadata_GDK.cpp` | Device metadata (GDK) | Returns Xbox-specific device type and name. |
| `GDK/Platform_GDK.cpp` | Platform init (GDK) | Creates GDK provider. |

## Local Files on Disk

These files are created/managed by the SDK at runtime inside the save root folder:

| Path | Purpose |
|------|---------|
| `<saveRoot>/cloudsync/localstate.json` | Tracks last-synced file sizes and timestamps for conflict detection. Contains `Folders` array and `Metadata` section. |
| `<saveRoot>/cloudsync/info-manifest.json` | Stores this device's unique ID. |
| `<saveRoot>/pfthumbnail.png` | Optional. If present, uploaded separately and URI exposed in `PFGameSaveDescriptor.thumbnailUri`. |

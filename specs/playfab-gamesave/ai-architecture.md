# PFGameSave Architecture & Code Flow Guide

How to navigate and modify the PFGameSave codebase. Read `ai-source-map.md` first for the file-level reference.

## Layer Diagram

```
┌──────────────────────────────────────────────────────────┐
│  Public C API    (PFGameSaveFilesAPI.cpp)                │
│  PFGameSaveFilesInitialize / AddUserWithUiAsync / ...    │
├──────────────────────────────────────────────────────────┤
│  Async Providers    (Providers/)                         │
│  DownloadAsyncProvider / UploadAsyncProvider / ...       │
│  Wrap FolderSyncManager calls into XAsyncBlock ops       │
├──────────────────────────────────────────────────────────┤
│  FolderSyncManager    (SyncManager/)                     │
│  Per-user orchestrator. Owns all step objects.            │
│  DoWorkFolderDownload / DoWorkFolderUpload               │
├──────────────┬───────────┬───────────┬──────────────────┤
│  LockStep    │ CompareStep│DownloadStep│  UploadStep     │
│  (acquire    │ (diff local│ (fetch +   │ (compress +     │
│   active     │  vs cloud, │  decompress│  upload +       │
│   device)    │  conflict  │  files)    │  finalize)      │
│              │  detection)│            │                  │
├──────────────┴───────────┴───────────┴──────────────────┤
│  Types Layer    (Types/)                                 │
│  FileFolderSet / ExtendedManifest / LocalStateManifest   │
├──────────────────────────────────────────────────────────┤
│  Generated Service Types    (Generated/)                 │
│  GameSaveAPI / GameSaveTypes / TypeWrappers               │
├──────────────────────────────────────────────────────────┤
│  Wrappers + Platform    (Wrappers/ + Platform/)          │
│  ServiceSelector / ServiceMock / Win32 / GDK providers   │
├──────────────────────────────────────────────────────────┤
│  Common Infrastructure    (Common/)                      │
│  GlobalState / HttpClient / UICallbackManager / Utils     │
└──────────────────────────────────────────────────────────┘
```

## How a Public API Call Flows

### Example: PFGameSaveFilesAddUserWithUiAsync

```
PFGameSaveFilesAddUserWithUiAsync()          [PFGameSaveFilesAPI.cpp]
  │
  ├─ GameSaveGlobalState::Get()              Get singleton
  ├─ GetFolderSyncManagerFromLocalUser()     Get/create per-user manager
  │
  ├─ Platform check: is GRTS provider?
  │   ├─ Yes → GameSaveAPIProviderGRTS::AddUser()    [out-of-process, Xbox/Windows]
  │   └─ No  → DownloadAsyncProvider::Begin()        [in-process]
  │
  └─ DownloadAsyncProvider schedules FolderSyncManager::DoWorkFolderDownload()
       │
       ├─ LockStep::AcquireActiveDevice()
       │   ├─ Login (get entity token)
       │   ├─ ListManifests (service call)
       │   ├─ SelectBaselineManifest (rollback logic if flags set)
       │   ├─ Check contention → UICallbackManager fires ActiveDeviceContention
       │   └─ CreatePendingManifest (InitializeManifest service call)
       │
       ├─ CompareStep::CompareWithCloud(downloading=true)
       │   ├─ GetManifestDownloadDetails (service call)
       │   ├─ Download extended manifest JSON → parse into FileFolderSet
       │   ├─ LocalStateManifest::InitWithLocalFilesAndFolders()  (read localstate.json + scan disk)
       │   ├─ MarkFilesToSync / ScanForConflicts
       │   └─ If conflict → UICallbackManager fires Conflict callback → wait for TakeLocal/TakeRemote
       │
       ├─ DownloadStep::Download()
       │   ├─ QueryStorage (check disk space → OutOfStorage callback if needed)
       │   ├─ Download compressed files from Azure blob URLs
       │   ├─ UncompressFiles (ZipUtils → libarchive)
       │   ├─ Delete/Create folders as needed
       │   └─ LocalStateManifest::WriteLocalManifest()
       │
       └─ If conflict required upload → UploadStep (single finalize to record winner/loser)
```

### Example: PFGameSaveFilesUploadWithUiAsync

```
PFGameSaveFilesUploadWithUiAsync()           [PFGameSaveFilesAPI.cpp]
  │
  └─ UploadAsyncProvider → FolderSyncManager::DoWorkFolderUpload()
       │
       ├─ CompareStep::CompareWithCloud(downloading=false)
       │   ├─ MarkFilesToTransferUponUpload / MarkFilesToDeleteUponUpload
       │   └─ MarkCompressedFilesToKeep (reuse unchanged ZIP bundles)
       │
       └─ UploadStep::Upload()
            ├─ CompressFiles (split into <=64MB ZIP batches, thumbnail uploaded separately)
            ├─ InitiateUpload (service call → get Azure upload URLs)
            ├─ Upload each ZIP to Azure (with progress callback)
            ├─ FinalizeManifest (service call → manifest becomes "Finalized")
            ├─ ListManifestsAfterUpload
            ├─ PromoteIfNeeded (mark baseline as "known good" if eligible)
            └─ TakeLock (create new pending manifest to retain active device,
                         or skip if ReleaseDeviceAsActive)
```

## Key Patterns

### State Machine Pattern
Every step class uses an enum-based state machine (e.g., `LockStage`, `CompareStage`, `UploadStage`). The `DoWork*` method on `FolderSyncManager` is called repeatedly by the async provider. Each call advances the step's state by one transition. When the step reaches its terminal state (e.g., `LockDone`), the orchestrator moves to the next step.

Failed service calls transition to a `WaitForFailedUI_*` state, which fires the SyncFailed callback and waits for user response (Retry/Cancel/UseOffline). Retry loops back to the failed state; Cancel propagates the error up.

### UI Callback Blocking
`UICallbackManager` blocks the state machine until the user responds. It posts the callback to the caller's task queue, then enters a wait state. The `SetUi*Response` API signals the wait, and the next `DoWork` call reads the response and advances.

### FileFolderSet as Central Data Structure
`FileFolderSet` is the working memory for sync operations. It holds two instances:
- `m_localFileFolderSet` -- built from `localstate.json` + disk scan
- `m_remoteFileFolderSet` -- built from extended manifest JSON

`CompareStep` marks entries in both sets with flags (needsUpload, needsDownload, needsDelete, skipFile) that `DownloadStep` and `UploadStep` then act on.

### Manifest Lifecycle
```
Initialized  →  Uploading  →  Finalized  →  (PendingDeletion)
     ↑                              │
     └──────── new pending ─────────┘  (retain active device)
```
- `Initialized`: Created by `InitializeManifest`. Represents active device lock.
- `Uploading`: Set by `InitiateUpload`. Upload in progress.
- `Finalized`: Set by `FinalizeManifest`. Upload complete, data is the new truth.
- After finalize, a new `Initialized` manifest is created (version + 1) to retain the active device for future uploads.

### Conflict Detection Logic
Conflict = same root-level subfolder has BOTH local changes AND remote changes.
- "Local changed" = file size or last-modified time differs from `lastSyncTimeLastModified` in `localstate.json`
- "Remote changed" = file differs between last-synced extended manifest and current cloud extended manifest

Resolution is all-or-nothing per atomic unit (root subfolder). TakeLocal keeps local files unchanged and marks them for upload. TakeRemote overwrites local files with cloud versions.

### Known-Good Promotion
After a successful upload, if the baseline manifest (the one downloaded during AddUser) has a successor that's now finalized, the baseline is eligible to be marked as "known good" via `UpdateManifest(IsKnownGood=true)`. This enables `RollbackToLastKnownGood` to find it later.

## Where to Make Changes

| Task | Where to look |
|------|---------------|
| Add a new public API | `PFGameSaveFiles.h` (declaration), `PFGameSaveFilesAPI.cpp` (implementation), possibly a new async provider |
| Change sync behavior | `SyncManager/` -- the relevant step class |
| Fix a conflict bug | `CompareStep.cpp` -- `MarkFilesToSync`, `ScanForConflicts`, `HandleConflict` |
| Fix upload/download issue | `UploadStep.cpp` or `DownloadStep.cpp` |
| Change compression | `ZipUtils.cpp` (uses libarchive) |
| Change local state tracking | `LocalStateManifest.cpp`, `FileFolderSet.cpp` |
| Change extended manifest format | `ExtendedManifest.cpp` |
| Add a service endpoint | `Generated/GameSave.h` (or the generator template if auto-generated) |
| Change platform behavior | `Platform/Windows/` or `Platform/GDK/` |
| Add/change UI callback | `PFGameSaveFilesUi.h` (public), `UICallbackManager.h/cpp` (internal), `GameSaveUICallbackInfo.h` (storage) |
| Add telemetry event | `GameSaveTelemetryManager.h/cpp` |
| Change debug/mock APIs | `PFGameSaveFilesForDebug.h`, `GameSaveServiceMock.h/cpp` |

## Build Quick Reference

| Target | How |
|--------|-----|
| GDK (Xbox/Windows) | Open `PlayFabGameSave.C.GDK.vs2022.sln`, build `Release\|Gaming.Desktop.x64` |
| Win32 (in-process) | Open `PlayFab.C.vs2022.sln`, build PlayFabGameSave.Win32 for `Release\|x64` |
| Test device | Build `PFGameSaveTestDeviceWindows` from `PlayFab.C.vs2022.sln` (Gaming.Desktop.x64) |
| Test controller | `dotnet build Test\PFGameSaveTestController\PFGameSaveTestController.csproj` |
| All (scripted) | `& Utilities\Scripts\gamesave-build.ps1` |
| Run tests | `py Utilities\Scripts\tests-run.py gamesave-inproc` |

Key preprocessor defines:
- `HC_PLATFORM_MSBUILD_GUESS=HC_PLATFORM_GDK` -- GDK builds
- `PF_GAMESAVE_USE_GDK_PROVIDER` -- selects GRTS out-of-process provider

Output goes to `Out\<Platform>\<Configuration>\PlayFabGameSave.*\`.

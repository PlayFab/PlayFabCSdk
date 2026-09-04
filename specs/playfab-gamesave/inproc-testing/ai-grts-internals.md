# GRTS Internals Guide

> How the Gaming Runtime Services (GRTS) game save system works under the hood.  
> For AI agents debugging PC GRTS test failures or understanding SDK ↔ GRTS interaction.

**Source code**: `C:\git\ConnectedStorage`  
**On-disk state**: `C:\XboxGames\GameSave\pgs`  
**Registry**: `HKLM\SOFTWARE\Microsoft\XGameSaveStorage`

## What GRTS Is

GRTS is the **out-of-process game save service** provided by the Xbox/GDK platform. When a game uses the PlayFab Game Save SDK with the `pc-grts` engine, the SDK calls into `xgameruntime.dll` which communicates over RPC to the GRTS service process. GRTS handles:

- Cloud sync (download on activation, upload on game exit)
- Conflict and contention detection + stock UI dialogs
- Local save storage management
- Background upload that survives game termination
- Version tracking and manifest management

The SDK (PlayFab.C) and GRTS are **separate codebases** with different owners. Many test failures are GRTS bugs, not SDK bugs.

## Architecture

```
Game Process                          GRTS Service Process
┌────────────────────┐                ┌────────────────────────────────┐
│  PFGameSave SDK    │                │  ConnectedStorage Service      │
│  (PlayFab.C)       │                │  (C:\git\ConnectedStorage)     │
│                    │   RPC/COM      │                                │
│  Provider_GRTS ────┼───────────────►│  PFXGameSaveServiceImpl        │
│                    │                │    ├── PFActivator (download)   │
│  Registers:        │◄───────────────┤    ├── PFContext (active save)  │
│  - UIProvider      │   Callbacks    │    └── PFUploadContext (upload) │
│  - FileSpaceHandler│                │                                │
│  - InterruptHandler│                │  NtmWebService ──► PlayFab API │
└────────────────────┘                └────────────────────────────────┘
```

### Communication Flow

1. **SDK → GRTS**: Via COM/RPC through `IPFXGameSaveService` interface
2. **GRTS → SDK**: Via callback interfaces (`IPFXGameSaveUIProvider`, `IPFXGameSaveFileSpaceHandler`)
3. **GRTS → PlayFab Service**: Via `NtmWebService` (HTTP to PlayFab REST APIs)

### Key Source Directories

| Directory | Contents |
|-----------|----------|
| `service/lib/` | Core service: `PFContext.h/cpp`, `PFActivator`, `PFUploadContext`, `Uploader`, `UiProvider` |
| `service/exe/` | Service entry point (`main.cpp`) |
| `common/` | Shared types: `PFCommon.h` (states, versions), `Manifests.h` (file/folder schemas) |
| `windows/lib/` | Windows-specific: alternate `PFXGameSaveServiceImpl`, registry helpers, PLM integration |
| `idl/` | Interface definitions, error codes (`ConnectedStorageErrorCodes.mc`) |
| `tools/` | Developer utilities (`xbstorage`) |
| `docs/` | State machines, design docs, bug investigations |

## IPFXGameSaveService Interface

This is the COM interface the SDK calls into. Defined in `service/lib/PFXGameSaveServiceImpl.h`:

| Method | Purpose |
|--------|---------|
| `PrepareContext` | Initialize/sync a save context (download from cloud, handle conflicts) |
| `UploadContext` | Trigger upload of modified save data to cloud |
| `RegisterProvider` | Register the game's UI provider for stock dialogs |
| `GetQuota` | Query remaining storage quota |
| `SetSaveDescription` | Set user-facing save description |
| `CancelOperation` | Cancel an in-progress async operation |
| `AppSuspended` / `AppResumed` | Lifecycle notifications |
| `Reset` | Reset/clear cloud data |

## Two State Machines

GRTS uses two primary state machines for the PlayFab integration, both defined in `PFContext.h/cpp`:

### PFActivator — Download/Sync (context activation)

State enum: `PFActivatorState` in `common/PFCommon.h`

```
PFAS_None → PFAS_MountStorage → PFAS_PlayFabLogin → PFAS_ListManifests
  → PFAS_ParseListManifests → PFAS_GetManifest → PFAS_ParseManifest
  → PFAS_GetExtManifest → PFAS_CreateSyncPlan → PFAS_StorageChecks
  → PFAS_HandleSyncConflicts → PFAS_SyncData → PFAS_InitManifest
  → PFAS_ContextCreated → PFAS_Complete
```

Branch states:
- `PFAS_InitManifestOffline` — offline activation (user chose "Play offline")
- `PFAS_RollbackManifest` — rollback to earlier version
- `PFAS_SyncDataRetry` — retry failed chunk downloads
- `PFAS_AbortUpload` — abort an in-progress upload from another device

### PFUploadContext — Upload (game exit → cloud sync)

State enum: `PFUploadState` in `common/PFCommon.h`

```
PFUS_None → PFUS_Starting → PFUS_LoginStart → PFUS_LoginComplete
  → PFUS_DataExtracted → PFUS_DataTagged → PFUS_ManifestRegistered
  → PFUS_Chunking → PFUS_Uploading → PFUS_UploadExtManifest
  → PFUS_FinalizeManifest → PFUS_Finalizing → PFUS_Complete
```

Branch states:
- `PFUS_UploadRetry` / `PFUS_UploadRetryWait` — retry with exponential backoff
- `PFUS_UploadAbandoned` — network lost, cleanup temp data
- `PFUS_CompleteOffline` — offline-mode completion
- `PFUS_Recovery` — resume from previous incomplete upload

## Upload Trigger Mechanisms

GRTS has two completely separate upload systems — the **XGS path** (legacy XGameSave containers)
and the **PF path** (PlayFab Game Save file-based). Both are in the same service process but
use different detection and upload strategies.

### How GRTS Detects Process Termination (Both Paths)

When a game connects to GRTS (`PrepareContext` / `EnsurePackageTracked`), GRTS:

1. Opens a **handle to the game process** via `CallerInfo::GetProcessHandle()`
2. Creates a `ProcessTerminatedHandler` — registers `CreateThreadpoolWait()` on that handle
3. When the process exits **for any reason** (clean exit, crash, taskkill), the kernel signals the handle
4. The threadpool callback fires → `PlmIntegration::OnPackageStateChanged(PES_TERMINATED)`
5. This flows through `Service::OnPackageStateChangeFromPlm()` to both context systems

**This is NOT PLM-dependent.** The kernel signals a process handle regardless of how the process
dies. GRTS **always** knows when the game exits on PC.

Source: `windows/lib/PlmIntegration.cpp` lines 7–72 (`ProcessTerminatedHandler` class)

### PF Path Upload Triggers (PlayFab Game Save)

The PF path stores game files directly in `u_<version>/` folders. Upload is triggered by:

| Trigger | What Happens | Source |
|---------|-------------|--------|
| **Process termination** | `PFContext::OnPackageTerminated()` → creates `PFUploadContext` → `UploadWorker()` | `PFContext.cpp:5636` |
| **User sign-out** | Same as termination — calls `OnPackageTerminated()` | `PFContext.cpp:5698` |
| **Service boot** | `PFContexts::OnBoot()` → `StartPendingUploads()` checks registry for `PendingPlayFabSessions` | `PFContexts.cpp:39` |
| **Network reconnect** | `OnNetworkConnectivityChanged(true)` → `StartPendingUploads()` | `PFContexts.cpp:48` |
| **DoWork timer** (15s) | Periodic — but only does peer-usage checks, not uploads directly | `PFContexts.cpp:389` |

**Key:** The PF path does NOT have a periodic upload timer. Upload ONLY happens on one of the
events above. Between those events, data sits locally with `E_PENDING` status.

#### PFUploadContext::UploadWorker() — What It Does

1. Reads game files from `sourceDirectory` (the versioned folder, e.g., `u_12345/`)
2. Loads the previous `extendedManifest` to know what was already uploaded
3. Calls `Manifests::CreateUploadPlan()` which compares current files against manifest:
   - `FindModifiedFiles()` — size changed OR lastWriteTime changed (with 2-sec tolerance)
   - `FindAddedFiles()` — new files not in manifest
   - `FindMissingFiles()` — files in manifest but deleted locally
4. Creates compressed chunks from modified/added files
5. Uploads chunks to Azure Blob Storage
6. Calls `FinalizeManifest` on PlayFab service to commit the new version
7. Cleans up temp files and clears `PendingPlayFabSession` from registry

#### PF Upload Guards (things that block upload)

- `!_isNetworkConnected` — defers upload, leaves pending session for later retry
- `_contextData.disableOutOfProcUpload` — title opted out → `PFUS_UploadAbandoned`
- `_uploadManifest.version.find(PF_OFFLINE_PREFIX) == 0` — offline version → `PFUS_CompleteOffline`
- `_cancelEvent` is set — upload was cancelled (network lost timeout, or game restarted)
- HTTP 400 with specific error codes (quota, conflict, not found) → `PFUS_UploadAbandoned`

#### PF Upload Registry Tracking

`PendingUploads::RegisterPlayFabSession()` writes session info to:
```
HKLM\SOFTWARE\Microsoft\XGameSaveStorage\PlayFab\{userId}\{gameId}
```
Fields: entityId, entityToken, version, contextStatus, lastHr, syncErrorCode

On boot, `StartPendingUploads()` enumerates these registry entries and creates upload
contexts for each one. This is how uploads survive service restarts.

### XGS Path Upload Triggers (Legacy XGameSave Containers)

The XGS path uses an in-memory container state machine inside `Uploader`. Containers are
tracked as DIRTY → UPLOADING → (removed from map). This path is used for traditional
XGameSave where the game writes blobs through `SubmitUpdatesAsync`.

| Trigger | What Happens | Source |
|---------|-------------|--------|
| **Container write** | `Context::SetDirtyContainer()` → adds to Uploader's dirty map | `Context.cpp:1321` |
| **Timer aging** | `Uploader::CheckForWork()` uploads DIRTY containers older than `MaxAgeBeforeUpload` | `Uploader.cpp:124` |
| **Process termination** | For FileSync contexts: `SnapshotXgs()` copies files → marks containers dirty | `Context.cpp:2026` |
| **Service boot** | `Context::SchedulePendingUploads()` re-queues containers where `IsModified()` | `Context.cpp:200` |
| **Network reconnect** | `ResumeUploadPolicy::OnNetworkConnectivityChanged()` triggers action | `ResumeUploadPolicy.cpp` |
| **User sign-in** | `Service::OnUserEvent()` → `CheckForWork()` | `Service.cpp:1707` |
| **PLM state change** | `Service::OnPackageStateChangeFromPlm()` → `CheckForWork()` | `Service.cpp:2002` |
| **Immediate mode** | `SetImmediateUploads(true)` — used by tests, uploads on every dirty | `Uploader.cpp:88` |

#### XGS Uploader Container States

```
DIRTY ──── upload starts ────► UPLOADING ──── upload completes ────► (removed)
  ▲                                │
  │                                │ container updated while uploading
  │                                ▼
  └──── upload completes ──── DIRTY_UPLOADING
```

#### XGS Upload Timing

- `DEFAULT_MAX_AGE_BEFORE_UPLOAD_IN_SECONDS` = 1800 (30 minutes)
- `DoWork` timer period = 15 seconds
- Each `DoWork` calls `Uploader::CheckForWork()` which scans for containers older than threshold
- `SetImmediateUploads(true)` bypasses the age check (test mode)

#### FileSync (XGameSaveFiles) — Folder Snapshot Detection

For XGameSaveFiles mode (`_fileSync != nullptr`), GRTS watches a game's save folder:
- `Context::DoWork()` periodically calls `SnapshotXgs()` if idle time > `SNAPSHOT_XGS_IDLE_TIME` (30 min)
- On termination: `SnapshotXgs(SNAPSHOT_XGS_IGNORE_TIME)` forces immediate snapshot
- `FileSync::SnapshotXgs()` walks the folder tree, finds files with changed timestamps/sizes
- Changed files are written into XGS containers → marked dirty → uploaded via Uploader

### Xbox vs PC: Key Differences

| Aspect | Xbox | PC |
|--------|------|-----|
| Process termination detection | PLM sends PES_SUSPENDED/TERMINATED AND process handle wait | Process handle wait only (no PLM) |
| Suspend notification | Game receives Suspend → calls back to GRTS | Game sends `SendPackageStateChange(PES_SUSPENDED)` manually |
| Upload timing on exit | PLM gives service time to complete | Process handle fires → upload starts, no guaranteed execution time |
| Connected Standby | Power state events trigger upload | No connected standby on PC |
| File system | XVD (encrypted virtual disk) | Plain files on regular NTFS |
| Service lifetime | Always running | Starts on demand, shuts down when idle |

### Common Upload Failure Modes on PC

1. **Service shuts down during upload** — If GRTS service goes idle and shuts down before
   upload completes, upload is abandoned. On next boot, `StartPendingUploads()` resumes.
2. **Network lost during upload** — 2-minute timer, then cancel. Retries on reconnect.
3. **No `PendingPlayFabSession` in registry** — If the session wasn't registered (race, crash
   during initialization), `StartPendingUploads()` finds nothing to do.
4. **`disableOutOfProcUpload` flag set** — Title configuration prevents background upload.
5. **Service restarted but game still running** — `StartPendingUploads()` checks if game
   process is still alive. If so, creates a tracking `PFContext` instead of uploading (waits
   for game to exit).

## Version Tracking

GRTS tracks versions in `PFActivationVersions` (`common/PFCommon.h`):

```cpp
struct PFActivationVersions
{
    PFVersion LatestLocal;        // Latest version in local storage
    PFVersion LatestLocalBase;    // Base version for LatestLocal
    PFVersion LatestRemote;       // Latest version from server
    PFVersion LatestOffline;      // Latest offline-only version
    PFVersion ConflictRemote;     // Remote version in conflict
    PFVersion ConflictLocal;      // Local version in conflict
    PFVersion LastKnownGood;      // Last known good version
    PFVersion LastConflictWinner; // Set on FinalizeManifest success
    PFVersion LastConflictLoser;  // Set at conflict resolution time
    ConflictDesc PendingConflictDesc; // Conflict metadata for uploads
};
```

Versions are monotonically increasing integers assigned by the PlayFab service. Each `InitManifest` call allocates a new version number.

## Conflict & Contention Resolution

### Contention (Lock)

When another device is actively uploading:
1. `PFAS_ParseListManifests` detects a manifest with `Status = Uploading`
2. GRTS shows **Lock Contention UI** via `RequestLockContentionUI()`
3. User choices:
   - **Break Lock**: Download last finalized version, upload local as new
   - **Retry**: Re-check (loops back to `PFAS_ListManifests`)
   - **Cancel**: Go offline (`PFAS_InitManifestOffline`)

### Conflict (Data)

When local and remote data have diverged:
1. `PFAS_HandleSyncConflicts` detects version mismatch
2. GRTS shows **Conflict Resolution UI** via `RequestConflictResolutionUI()`
3. User choices:
   - **Take Local**: Keep local, discard remote. Upload local as new version.
   - **Take Remote**: Discard local, download remote version.
   - **Cancel**: Go offline with local data.

### IsWinner Semantics (Important!)

In `ConflictDesc`:
```cpp
struct ConflictDesc {
    bool isWinner;       // Whether the REFERENCED version is the winner
    std::wstring version; // The conflict reference version
};
```

GRTS convention: `isWinner` describes the **referenced version**, not the current upload.

| User Choice | isWinner | version | Meaning |
|-------------|----------|---------|---------|
| Take Local  | `false`  | Remote version | "The referenced remote version is NOT the winner" |
| Take Remote | `true`   | Remote version | "The referenced remote version IS the winner" |

⚠️ **The SDK uses opposite semantics** — see `bugs/gamesave-iswinner-semantics-inverted.md`. This is a known HIGH-severity bug.

## On-Disk State

GRTS stores all PlayFab game save state under `C:\XboxGames\GameSave\pgs\`.

### Directory Layout

```
pgs/
├── t_{XUID}_{TitleID}/        # Title-level (sync coordination)
│   └── {Version}-sync/        # Download plan for a version
├── u_{XUID}_{TitleID}/        # User-level (save data + manifests)
│   ├── {XUID}_{TitleID}.json  # Context state file
│   ├── {Version}.json         # Manifest per version
│   ├── extended-{Version}-manifest.json  # Extended manifest (file details)
│   └── {Version}/             # Actual save data
│       └── {folder}/{file}    # Game save files
```

Prefix meanings:
| Prefix | Scope | Example |
|--------|-------|---------|
| `t_` | Title-level (sync temp data) | `t_2814640093565666_E18D7/` |
| `u_` | User-level (save data + manifests) | `u_2814640093565666_E18D7/` |

### Context State File (`{XUID}_{TitleID}.json`)

Tracks the current state of this user's save context:

```json
{
  "Context": {
    "Aumid": "41336MicrosoftATG.XboxLiveE2E_dspnxghe87tn0!Game",
    "InitFlags": "8",
    "SaveLocation": "C:\\gamesaves-test\\DeviceB\\",
    "SessionId": "{CCB5EF45-8C34-4489-9C4D-FA056EED7345}",
    "ProcessId": "42696",
    "SyncStatus": "2",
    "SyncErrorCode": "0",
    "LastHR": "0",
    "TotalQuota": "1073741824",
    "TotalSize": "10240",
    "UploadSize": "0",
    "UploadVersion": "606",
    "LastConflictWinner": "",
    "LastConflictLoser": "603",
    "PendingConflictIsWinner": "0",
    "PendingConflictVersion": "603",
    "PackageFullName": "41336MicrosoftATG.XboxLiveE2E_1.7.0.0_x64__dspnxghe87tn0"
  }
}
```

Key fields:
| Field | Meaning |
|-------|---------|
| `SyncStatus` | 0=BROKEN, 1=SYNCHRONIZED, 2=NOT_SYNCHRONIZED |
| `SaveLocation` | Where the game reads/writes save files |
| `UploadVersion` | Version being/last uploaded |
| `LastConflictWinner/Loser` | Conflict resolution history |
| `PendingConflictIsWinner/Version` | Conflict metadata for next upload's FinalizeManifest. NOTE: These fields appear in the context JSON but the **active** flag lives in GRTS service memory. The on-disk value may be stale — runtime behavior is governed by in-memory state. |
| `TotalQuota` | Per-user quota in bytes (default 1 GB) |

### Manifest File (`{Version}.json`)

Describes a specific version's upload state and data chunks:

```json
{
  "Manifest": {
    "UserId": "2814640093565666",
    "GameId": "E18D7",
    "DeviceId": "{8D41D6C0-8EED-4182-B329-BC8351A89B5E}",
    "Version": "606",
    "BaseVersion": "605",
    "Status": 2,
    "Created": "2026-05-02T06:16:05.537Z",
    "LastWrite": "2026-05-02T06:16:05.537Z",
    "UploadProgress": { "Size": "0", "Expected": "0" },
    "Chunks": [{
      "Id": "{41FE1C70-AC1F-F25A-2EBF-156AA042DA72}",
      "Name": "41FE1C70-AC1F-F25A-2EBF-156AA042DA72.zip",
      "CompressSize": 10240,
      "Size": 0
    }]
  }
}
```

Status values (from `PFManifestStatus`):
| Value | Name | Meaning |
|-------|------|---------|
| 0 | `PFS_Initialized` | Version created, not yet uploaded |
| 1 | `PFS_Uploading` | Upload in progress |
| 2 | `PFS_Finalized` | Upload complete (current active version) |
| 3 | `PFS_Quarantined` | Version quarantined (data integrity issue) |

### Extended Manifest (`extended-{Version}-manifest.json`)

Contains the full file/folder tree for a version:

```json
{
  "v1": {
    "DeviceId": "{8D41D6C0-...}",
    "Version": "606",
    "Created": "2026-05-02T06:16:03.989Z",
    "LastModified": "2026-05-02T06:16:04.495Z",
    "Folders": [
      { "Id": "{ADB460BF-...}", "Name": "save" }
    ],
    "Files": [{
      "FileId": "{41FE1C70-...}",
      "Name": "41FE1C70-....zip",
      "CompressSize": 10240,
      "Size": 10240,
      "Compression": "zip",
      "Extract": [{
        "FileId": "{F055E888-...}",
        "Name": "data.bin",
        "Size": 10240,
        "FolderId": "{ADB460BF-...}"
      }]
    }]
  }
}
```

This manifest maps compressed chunks (zip files with GUID names) back to the original folder/file structure.

### Retained Versions

GRTS keeps **multiple manifest versions** on disk (e.g., 498, 574, 606 for the same user). This supports:
- Rollback to last known good version
- Conflict resolution (comparing local vs remote)
- Power-cycle recovery (resume from last good state)

## Error Codes

GRTS error codes use facility `0x83` (FACILITY_GAME_SAVE). All are HRESULT values.

### Public Error Codes (returned to games)

| Code | Symbol | SDK Alias | Meaning |
|------|--------|-----------|---------|
| `0x80830001` | `CS_E_INVALID_CONTAINER_NAME` | — | Invalid container name |
| `0x80830002` | `CS_E_NO_ACCESS` | — | SCID invalid for title |
| `0x80830003` | `CS_E_OUT_OF_LOCAL_STORAGE` | `E_GS_OUT_OF_LOCAL_STORAGE` | Insufficient disk space |
| `0x80830004` | `CS_E_USER_CANCELED` | `E_GS_USER_CANCELED` | User canceled sync |
| `0x80830005` | `CS_E_UPDATE_TOO_BIG` | — | Update exceeds max |
| `0x80830006` | `CS_E_QUOTA_EXCEEDED` | `E_GS_QUOTA_EXCEEDED` | Data exceeds quota |
| `0x80830007` | `CS_E_PROVIDED_BUFFER_TOO_SMALL` | — | Buffer too small |
| `0x80830008` | `CS_E_BLOB_NOT_FOUND` | — | Blob not found |
| `0x80830009` | `CS_E_NO_XBOX_LIVE_INFO` | — | Missing SCID |
| `0x8083000A` | `CS_E_CONTAINER_NOT_IN_SYNC` | — | Container not synced |
| `0x8083000B` | `CS_E_CONTAINER_SYNC_FAILED` | — | Sync failed |
| `0x8083000C` | `CS_E_USER_NOT_REGISTERED` | — | User not configured |
| `0x8083000D` | `CS_E_HANDLE_EXPIRED` | — | Stale handle |
| `0x8083000E` | `CS_E_ASYNC_FUNCTION_REQUIRED` | — | Can't block TST thread |
| `0x8083000F` | `CS_E_PROVIDER_MISMATCH` | — | Mixed provider types |
| `0x80830010` | `CS_E_USER_QUIT` | — | User quit game |

### Internal Error Codes (not returned to games)

| Code | Symbol | Meaning |
|------|--------|---------|
| `0x80832001` | `CS_E_INVALID_USER` | Invalid user context |
| `0x80832002` | `CS_E_USER_SELECTED_NO_RETRY` | User chose not to retry |
| `0x80832003` | `CS_E_SERVICE_ERROR` | PlayFab service error |
| `0x80832004` | `CS_E_CONTAINERINDEX_FAILURE` | Container index corruption |
| `0x80832005` | `CS_E_MISSING_ATOM` | Missing blob/atom data |
| `0x80832006` | `CS_E_CONTAINER_INVALID_VERSION` | Invalid version number |

### Most Common in Test Failures

**`0x80830004` (`E_GS_USER_CANCELED`)** — the single most frequent failure cause. GRTS returns this when:
- User clicks "Cancel" in a stock TCUI dialog
- User clicks "Play offline" (also returns this code)
- Stock TCUI intercepts a SyncFailed event before the SDK's callback fires

## UI Provider System

GRTS uses stock TCUI dialogs for user-facing sync decisions. The SDK registers a UI provider, and GRTS calls it when user interaction is needed.

### UI Callback Interfaces

| Interface | When Invoked | User Options |
|-----------|-------------|--------------|
| `IPFXGameSaveUIProgressCallback` | During sync progress | Cancel |
| `IPFXGameSaveUIRetryCallback` | Sync/network failure | Retry, Cancel |
| `IPFXGameSaveUILockContentionCallback` | Another device is uploading | Break Lock, Retry, Cancel |
| `IPFXGameSaveUIConflictResolutionCallback` | Local/remote data conflict | Take Local, Take Remote, Cancel |
| `IPFXGameSaveUIOutOfLocalStorageCallback` | Insufficient disk space | (retry after user frees space) |

### Stock TCUI Dialog Pages

In the test harness, the GameSaveUiNavigator auto-clicks these dialogs:

| Dialog Page | Displayed When | Auto-Response Config |
|-------------|---------------|---------------------|
| `SyncFailedPage` | Network/service error | `syncFailedAutoResponse` in YAML |
| `ContentionPage` | Lock contention detected | `contentionAutoResponse` |
| `ConflictPage` | Data conflict detected | `conflictAutoResponse` |
| `SyncProgressPage` | Download/upload in progress | Waits for completion or timeout |

## PlayFab API Calls

GRTS makes these PlayFab service calls (defined in `PFCommon.h`):

| Request Type | PlayFab API | When |
|-------------|-------------|------|
| `PFR_LOGIN` | `LoginWithXbox` | Authentication |
| `PFR_LIST_MANIFESTS` | `ListManifests` | Get available versions |
| `PFR_GET_MANIFEST_DETAILS` | `GetManifestDetails` | Get specific version metadata |
| `PFR_GET_CHUNK` | Download chunk URI | Download save data |
| `PFR_INIT_MANIFEST` | `InitManifest` | Create new version |
| `PFR_INIT_UPLOAD` | `InitUpload` | Get upload URIs for chunks |
| `PFR_PUSH_UPLOAD` | Upload to blob URI | Push save data |
| `PFR_UPDATE_UPLOAD` | `UpdateUpload` | Report upload progress |
| `PFR_FINALIZE_MANIFEST` | `FinalizeManifest` | Mark upload complete |
| `PFR_DELETE_MANIFEST` | `DeleteManifest` | Remove a version |
| `PFR_GET_QUOTA` | `GetQuota` | Query remaining quota |
| `PFR_ROLLBACK_MANIFEST` | `RollbackManifest` | Rollback to earlier version |

## Observed Behavior: Test 01 (Golden Path)

Running xplat test 01 (single-device golden path: write files → upload → teardown → re-add user → verify download) and observing how GRTS state changes on disk.

### Timeline (from device logs)

```
23:39:46  DeleteLocalFolder → cleans C:\gamesaves-test\DeviceA
23:39:46  PFGameSaveFilesInitialize → SDK selects GRTS provider
23:39:47  AddUserWithUiAsync → SDK calls PFXGameSaveFilesGetFolderWithUiAsync
          (GRTS does: PlayFab login → ListManifests → sync plan → download data)
23:39:55  AddUser complete → GRTS returns folder, isConnectedToCloud=true (8s)
23:39:55  DeleteSaveRoot → removes GRTS-synced data, keeps manifest
23:39:55  WriteGameSaveData → writes progress/payload.bin (10KB, 0xAA,0xBB,0xCC,0xDD)
23:39:55  UploadWithUiAsync(ReleaseDeviceAsActive) → triggers GRTS upload
23:39:57  Upload complete (2s) → GRTS creates new version, uploads to PlayFab
23:39:57  DeleteSaveRoot(preserveManifest=false) → cleans everything
23:39:57  Uninitialize → closes handles, frees config
          [SDK fully torn down — simulates game exit]
23:39:58  Re-init SDK, re-init GRTS provider with same saveFolder
23:39:59  AddUserWithUiAsync → GRTS re-syncs from cloud
23:40:01  AddUser complete → data downloaded from cloud (2s)
23:40:01  DeleteSaveRoot → test verification/cleanup
23:40:01  Uninitialize → done
```

### On-Disk Changes

```
BEFORE (version 606)                   AFTER (version 609)
─────────────────────                  ─────────────────────
u_2814640093565666_E18D7/              u_2814640093565666_E18D7/
├─ 2814640093565666_E18D7.json         ├─ 2814640093565666_E18D7.json  ← UPDATED
├─ 498.json                  (kept)    ├─ 498.json                     (kept)
├─ 574.json                  (kept)    ├─ 574.json                     (kept)
├─ 606.json                  ← GONE    ├─ 609.json                     ← NEW
├─ extended-498-manifest.json (kept)   ├─ extended-498-manifest.json   (kept)
├─ extended-574-manifest.json (kept)   ├─ extended-574-manifest.json   (kept)
├─ extended-606-manifest.json ← GONE   ├─ extended-609-manifest.json   ← NEW
└─ 606/save/data.bin (10KB)  ← GONE    └─ 609/                         ← EMPTY
t_2814640093565666_E18D7/    ← GONE
└─ 606-sync/                 ← GONE
```

### Key Observations

1. **Version numbers jump by 3** (606 → 609): GRTS consumed 3 version numbers:
   - 607: Session-one `InitManifest` (allocate version for upload)
   - 608: Session-one `FinalizeManifest` (upload complete, but version then superseded)
   - 609: Session-two `InitManifest` (re-sync from cloud, this is the final stored version)

2. **Old manifests cleaned up**: GRTS removed 606.json, extended-606-manifest.json, and the 606/ data directory. Only the new version (609) and historical versions (498, 574) are kept.

3. **Title-level temp data cleaned up**: The `t_` directory and its `606-sync/` download plan were removed after the test completed.

4. **Data directory empty after session-two**: The test's `DeleteSaveRoot` in session-two removed the downloaded game files, so `609/` is empty. But the manifest still records what was there.

5. **Context file updated atomically**: All context fields change together — new ProcessId, SessionId, SaveLocation (DeviceB→DeviceA), UploadVersion (606→609).

6. **Upload is fast**: The GRTS upload completed in ~2 seconds for 10KB of data. This includes: diff detection → chunking → compress → HTTP upload → FinalizeManifest.

7. **Download (re-sync) is fast**: Session-two AddUser re-downloaded from cloud in ~2 seconds.

8. **Test harness logs ExtManifest snapshots**: Before/after AddUser and after Upload, the harness dumps the latest extended manifest from `C:\XboxGames\GameSave\pgs\u_{XUID}_{TitleID}\`. This is useful for debugging.

### Context File Diff

| Field | Before | After | Notes |
|-------|--------|-------|-------|
| `SaveLocation` | `C:\gamesaves-test\DeviceB\` | `C:\gamesaves-test\DeviceA\` | Changed to match new test |
| `UploadVersion` | `606` | `609` | New version from this test |
| `ProcessId` | `42696` | `32276` | New process |
| `SessionId` | `{CCB5EF45-...}` | `{E743C526-...}` | New session |
| `TotalSize` | `10240` | `0` | Save root was deleted |
| `LastConflictLoser` | `603` | `607` | Updated during activation |
| `PendingConflictIsWinner` | `0` | `1` | Updated during activation. NOTE: This on-disk value may not reflect runtime state — the active flag lives in GRTS service memory (see Rule 14) |
| `PendingConflictVersion` | `603` | `(empty)` | Consumed during upload |
| `SyncStatus` | `2` | `2` | Both NOT_SYNCHRONIZED (normal end state) |

## Observed Behavior: Test 110 (Cross-Platform Conflict)

Test 110 triggers a **real GRTS conflict** by creating divergent data on two different engines (GRTS and in-proc), then reconnecting GRTS to force conflict detection.

### How to Trigger a GRTS Conflict

The conflict recipe has 4 steps:

1. **Seed cloud** — DeviceA (inproc) uploads baseline data `[0xAA]` to cloud
2. **GRTS syncs, goes offline, writes divergent data** — DeviceB (GRTS) syncs from cloud, then `ChangeTargetDeviceState: DisableNetwork`, writes `[0xBB]` locally, exits
3. **Advance cloud** — DeviceA (inproc) uploads `[0xCC]` to cloud, advancing past GRTS's offline version
4. **GRTS reconnects** — DeviceB (GRTS) re-enables network, re-initializes, calls `AddUserWithUiAsync`. GRTS detects local data differs from cloud → **conflict fires**

```
                Cloud State                     GRTS Local State
                ───────────                     ────────────────
Step 1:  [0xAA] (DeviceA uploads)               (empty)
Step 2:  [0xAA]                                  [0xAA] → [0xBB] (offline write)
Step 3:  [0xAA] → [0xCC] (DeviceA advances)      [0xBB] (stale, offline)
Step 4:  [0xCC] vs [0xBB]                         CONFLICT DETECTED
```

### Critical Test Design Rule

**Each device MUST use a different save folder.** GRTS (DeviceB) must use `C:\gamesaves-test\DeviceB\` and inproc (DeviceA) must use `C:\gamesaves-test\DeviceA\`. If they share the same folder, DeviceA's `DeleteLocalFolder` wipes DeviceB's divergent data and the conflict never triggers.

Also: **Do NOT `DeleteLocalFolder` before GRTS reconnect.** The GRTS device must find its stale local data to trigger the conflict. If you delete the folder before `AddUserWithUiAsync`, GRTS sees no local data and just downloads the latest cloud version — no conflict.

### How GRTS Resolves Conflicts

GRTS handles conflict resolution **internally** through its stock TCUI (Title Call UI):

1. During `PFXGameSaveFilesGetFolderWithUiAsync`, GRTS detects local vs cloud mismatch
2. GRTS shows a system dialog: "Your saved data conflicts with the cloud" with options
3. The test auto-responds via `PFGameSaveFilesSetUiConflictAutoResponse` + `AutoNavigateGameSaveUi`
4. GRTS applies the chosen resolution (UseLocal or UseCloud)
5. SDK sees only `PFXPALGetFolderComplete: folder obtained, isConnectedToCloud=true`

The SDK provider (`GameSaveAPIProviderGRTS`) does NOT see the conflict — GRTS resolves it before returning control. The SDK's conflict callback (`PFGameSaveFilesSetUiCallbacks`) is **not invoked** for GRTS conflicts (it's only for inproc engine conflicts).

### Test 110 Fix Applied

The original test had DeviceB (GRTS) using `C:\gamesaves-test\DeviceA\` — same folder as DeviceA (inproc). This caused two problems:
- DeviceA's `DeleteLocalFolder` in the `pc-advances-cloud` block wiped DeviceB's divergent `[0xBB]` data
- DeviceB's reconnect block had `DeleteLocalFolder` which deleted any remaining local data before `AddUserWithUiAsync`

**Fix**: Changed DeviceB to use `C:\gamesaves-test\DeviceB\` and removed `DeleteLocalFolder` from the reconnect block.

### Version Consumption During Conflict

GRTS consumed 3 version numbers during test 110 (609 → 612):

| Version | Event | Notes |
|---------|-------|-------|
| 610 | Step 2: First `AddUserWithUi` | GRTS syncs cloud [0xAA], allocates version |
| 611 | Step 4: Second `AddUserWithUi` (conflict) | GRTS resolves conflict, allocates version |
| 612 | Step 4: Upload resolved data | GRTS uploads [0xBB] (UseLocal winner) to cloud |

### GRTS State After Conflict Resolution

```
Context file after test 110:
  UploadVersion:           612
  SaveLocation:            C:\gamesaves-test\DeviceB\
  TotalSize:               10240  (save/data.bin preserved)
  LastConflictLoser:       607    (unchanged from prior test)
  PendingConflictIsWinner: 1
  SyncStatus:              2      (NOT_SYNCHRONIZED — normal end state)

extended-612-manifest.json:
  Folders: [save, progress]
  Files:   [save/data.bin — 10240 bytes, compressed to 244 bytes]
  BaseVersion: 611 (conflict-resolved version)
```

### Related Conflict Tests

| Test | Pattern | Status | Notes |
|------|---------|--------|-------|
| **14** | GRTS seeds → inproc diverges → GRTS advances → inproc hits conflict (UseLocal) | PASS | Gold standard |
| **15** | Same as 14 but conflict resolves UseCloud | PASS | Gold standard |
| **16** | Conflict → PlayOffline → relaunch → re-prompt → UseLocal | FAIL | Pre-existing: `E_GS_USER_CANCELED` (0x800704C7) on relaunch AddUser |
| **24** | Contention + conflict chained: contention fires first, then conflict | FAIL | Pre-existing: upload fails (0x89237004) after contention+conflict |
| **78** | Upload → offline diverge → conflict → UseLocal → rollback to conflict loser | FAIL | Pre-existing: `E_GS_USER_CANCELED` (0x80830004) on reconnect AddUser |
| **110** | Inproc seeds → GRTS offline diverges → inproc advances → GRTS hits conflict (UseLocal) | PASS | Fixed: Upload+Terminate/Relaunch pattern (test 125 style), re-write after UseLocal to propagate |
| **112** | Xbox uploads → PC offline diverges → PC resolves UseLocal → Xbox rollbacks | FAIL | Was passing by accident (shared folder made snapshot compare trivial) |
| **114** | Conflict winner data retention across sessions | — | — |
| **115** | Cross-platform conflict winner retention | — | — |

## GRTS Behavioral Rules (Verified via ETL)

Hard-won facts about how GRTS actually behaves, verified through ETL traces and test iterations.
These prevent future agents from wasting cycles rediscovering them.

### Rule 1: GRTS Is an Independent System Service

GRTS (`GamingServices.exe`) runs as a Windows service, completely independent of the game process.
It monitors save folders and manages uploads/downloads on its own schedule.

- **Calling `PFGameSaveFilesUninitializeAsync` does NOT stop GRTS tracking.** A user can write to the save folder when the game isn't even running — GRTS will detect it.
- **Killing the game process does NOT stop GRTS.** GRTS detects the process exit via `GameProcessStateChange` and continues managing the save folder.
- **`StopGamingServices` is machine-wide** — it kills GRTS for ALL titles and ALL processes, not just one game. Never use it in tests.

### Rule 2: GRTS Detects Dirty Save Folders Automatically

After a successful `UploadWithUi(KeepDeviceActive)`, GRTS re-initializes the context (allocates next version). GRTS then monitors the save folder for changes. When the game writes new files (via `WriteGameSaveData` or raw file I/O — there is no difference), GRTS detects the mismatch between the save folder contents and the last committed state.

ETL event sequence for dirty detection:
```
PFUploadWorkerEntered                     — GRTS starts upload background worker
PFUploadCopyGameLocation                  — copies save folder → pgs/{version}/ blob cache
PFUploadWorkerLoopStart                   — begins upload planning
PFModifiedFile                            — identifies which files changed
PFUploadPlanStart (ModifiedFiles: N)      — N files differ from committed state
PFUploadPlanComplete (UploadChunks: N)    — upload plan ready
```

**Timing**: GRTS does NOT detect dirty data instantly. In ETL traces, the dirty detection + upload typically starts 10-20 seconds after the file write.

### Rule 3: GRTS Upload Retry After 409 Clears Session State

When GRTS tries to upload dirty data and the server returns **HTTP 409 (Conflict)** on `PFR_FINALIZE_MANIFEST`, GRTS **abandons the upload AND clears the session state**:

```
PFResponse (PFR_FINALIZE_MANIFEST)        — HttpStatus: 409, Hresult: Conflict (409).
PFUploadWorkerHandleResponse              — Version: N, HttpStatus: 409
PFUploadContextAbandoned                  — upload abandoned, TempDataSize cleared
PFUploadAbandonedClearSession             — syncErrorCode: 20312, session state cleared
```

**Critical consequence**: After `PFUploadAbandonedClearSession`, the next `AddUserWithUiAsync` call shows `conflictVersion: (empty)` in `PFContextCheckSyncConflicts`. GRTS just downloads the latest cloud version — **no conflict is triggered**.

This happens because during the upload retry, `PFUploadCopyGameLocation` already copied the save folder into the version's blob cache. So the local blob cache matches the save folder. When the next AddUser compares save folder vs local state, they match → "no modifications" → no conflict.

### Rule 4: Offline Upload Deferral Preserves Dirty State

When GRTS detects dirty data but has **no network**, it defers the upload:

```
PFContextsOfflineUploadSkipLogin          — can't reach PlayFab, skipping login
PFUploadRecoverUpload                     — attempting recovery
PFUploadRecoveryState (LastError: ...)    — notes the previous error
PFUploadContextDeferredNoNetwork          — upload deferred, dirty state PRESERVED
```

The dirty state survives deferral. When network returns, GRTS retries automatically (~10s interval). This is important: **offline dirty state is the only state that reliably produces a conflict on the next AddUser**.

### Rule 5: The Conflict Detection Race Condition

Triggering a GRTS conflict on a single machine is inherently a **race condition** between:
1. GRTS's background upload retry (starts within seconds of network restoration)
2. The new process's `AddUserWithUiAsync` (runs conflict detection)

If GRTS retries the upload before AddUser:
- Upload succeeds → dirty state cleared → no conflict
- Upload gets 409 → session cleared → no conflict (Rule 3)

If AddUser runs before GRTS retries:
- Conflict detection sees: local dirty data (from deferred upload) vs newer cloud → **CONFLICT** ✅

**Test 110's unreliable conflict** is explained by this race. When test 110 works, it's because AddUser runs before GRTS's upload retry. When it fails, GRTS got there first.

### Rule 6: KeepDeviceActive Re-Initialization

After `UploadWithUi(KeepDeviceActive)`, GRTS automatically re-initializes:

```
PFContextLoadedSession                    — session reloaded
PFResponse (PFR_LIST_MANIFESTS)           — check for newer cloud versions
PFContextInitManifestRequest              — allocate next version (N+1)
PFContextInitStart                        — begin re-init
PFContextInitFinish                       — re-init complete
PFContextCreated (Version: N+1)           — new context ready at version N+1
```

This means after uploading v4487, GRTS creates context v4488. Any file writes after this point create dirty state relative to v4488's committed state.

### Rule 7: ETL Is the Ground Truth for GRTS Behavior

The game's SDK-level logs (`device-DeviceB-log.txt`) show only what the SDK sees. They do NOT show:
- GRTS's background upload detection and retry cycles
- HTTP 409 errors on FINALIZE_MANIFEST
- `PFUploadAbandonedClearSession` events
- Upload deferral/recovery state

**Always read the ETL** (`read-grts-etl.py --last-minutes N`) to understand what GRTS is actually doing. The ETL provider `Microsoft.Gaming.PlayFab.GameSaveTrace` has the full picture.

### Rule 8: Version Chain Bookkeeping

GRTS consumes version numbers aggressively:
- First `AddUserWithUi`: allocates version N
- `UploadWithUi(KeepDeviceActive)`: uploads data as version N, allocates N+1
- Failed upload (409): consumes version N+1 for the attempt, cleared on abandon
- Next `AddUserWithUi`: allocates version N+2 or N+3

In a single test run, GRTS may consume 3-5 version numbers. This is normal.

### Rule 9: TakeRemote Conflict Resolution Sets PendingConflictIsWinner

After a conflict is resolved with UseCloud (TakeRemote), GRTS sets `PendingConflictIsWinner=1` **in memory** (not on disk). This flag is carried in the HTTP request body's `Conflict` JSON block on the next upload. The flag persists within the session but is **cleared on clean uninitialize/relaunch** (proven test 129, pass165).

> ⚠️ **CORRECTION (May 2026, tests 126-129):** The ETL field `UploadOptions` in `PFXGameSaveServiceUploadContext` is the **upload mode enum** (0=KeepDeviceActive, 1=ReleaseDeviceAsActive), NOT the conflict flag. Prior analysis incorrectly attributed the stomp to UploadOptions:1. The actual conflict metadata (isWinner, conflict version) is carried in the HTTP request body and is NOT visible in ETL.

ETL evidence:
```
PFContextConflictResolution
    ConflictVersion: 4498
    LocalVersion: 4497
    RemoteVersion: 4498
    Choice: 1                              ← UseCloud / TakeRemote
PFActivatorTakeRemoteLoserUploadDisabled
    loserVersion: 4497                     ← GRTS disables upload of the loser's data
    winnerVersion: 4498                    ← cloud version is the winner

... (game writes new data, SDK calls UploadWithUi) ...

PFXGameSaveServiceUploadContext
    UploadOptions: 0                       ← Upload MODE (0=KeepActive, 1=ReleaseActive)
PFUploadWorkerEntered
PFUploadCopyGameLocation → pgs/4499/
PFUploadContextFinalized                   ← HTTP 200, upload appears to succeed
```

Pre-fix: The upload finalizes with HTTP 200 (appears successful), but the conflict block in the HTTP body causes the server to treat the uploaded data as a conflict-winner replay. Other devices downloading afterward get the old conflict-winner data instead of the new save. **With the server fix (commit `69e0b63`), the server inverts isWinner for non-SDK callers, correctly treating GRTS uploads as winners — data propagates correctly.**

### Rule 10: PFActivatorTakeRemoteLoserUploadDisabled

When UseCloud is chosen during conflict resolution, GRTS emits:
```
PFActivatorTakeRemoteLoserUploadDisabled
    loserVersion: N
    winnerVersion: N+1
```

This disables the upload of the "loser" version (the local dirty data that lost the conflict). The `PendingConflictIsWinner` flag is set **in memory** and persists within the session. It is carried in the HTTP Conflict JSON block on subsequent uploads. The flag is:
- **Session-scoped**: Cleared on clean `PFGameSaveFilesUninitializeAsync` + process exit (test 129)
- **Not cleared by TakeLocal**: A subsequent TakeLocal conflict in the same session does not override it (test 133)
- **Consumed by auto-upload**: GRTS immediately performs a manifest-only upload after conflict resolution (test 130)

### Rule 10b: GRTS Auto-Upload After Conflict Resolution

After TakeRemote conflict resolution, GRTS **immediately** (within ~0.2s of `PFContextCreated`) performs a manifest-only upload:
```
PFUploadWorkerEntered (InitialState: 0)
PFUploadPlanComplete
    TouchedChunks: 0          ← NO blob data
    UploadPendingSize: 0      ← Manifest only
PFUploadContextFinalized
    TotalSize: 0              ← Zero bytes uploaded
```

This auto-upload:
- Commits the conflict resolution to the server
- Carries the PendingConflictIsWinner flag in the HTTP body
- Cannot be prevented by the game (happens before any game code runs)
- Makes "crash before upload" scenarios nearly impossible (test 130, pass166)

### Rule 10c: TakeLocal Uses Different Code Path

TakeLocal (UseLocal, Choice:0) uses a completely different activator from TakeRemote:

| Aspect | TakeRemote (UseCloud) | TakeLocal (UseLocal) |
|--------|----------------------|---------------------|
| Choice value | 1 | 0 |
| Activator | `PFActivatorTakeRemoteLoserUploadDisabled` | `PFActivatorTakeLocalConflict` |
| Activator detail | Sets PendingConflictIsWinner | `reason: NoUploadNeeded` |
| Downloads cloud? | Yes (replaces local) | No (downloadSize: 0) |
| Sets PendingConflictIsWinner? | Yes | No |
| Server fix impact | Inverts isWinner (fix active) | Not triggered (no conflict block) |

### Rule 11: E_XAL_NETWORK After Network Re-enable

After calling `ChangeTargetDeviceState: EnableNetwork` (which removes firewall rules), Xbox Authentication Library (XAL) needs time to re-establish connectivity to Xbox Live services.

- **Error**: `E_XAL_NETWORK (0x89235106)` — XAL network error
- **Symptom**: `PFAccountManagementClientLinkXboxAccountAsync` fails (or any XAL-dependent API)
- **Fix**: Add `SmokeDelay 3000` between EnableNetwork and the first Xbox Live API call (XUserAddAsync)
- **Note**: `LoginWithCustomIDAsync` uses PlayFab's HTTP stack (not XAL) and works immediately after EnableNetwork. Only XAL-dependent calls (LinkXboxAccount, LoginWithXbox) need the delay.

### Rule 12: WaitForGameSaveSync May Report Stale State

`WaitForGameSaveSync` polls GRTS status via the Gaming Services API. It reports `InSync=true` when GRTS believes its last committed state matches the cloud. However:

- If the game writes data and is immediately terminated, GRTS may not have detected the dirty state yet
- `InSync=true` reflects the state of the *last completed upload*, not the current save folder contents
- In test 125 v9, WaitForGameSaveSync returned InSync=true in 76ms after Terminate — GRTS hadn't detected the Phase 5 write yet

**Implication**: WaitForGameSaveSync is only reliable for confirming that a *previous SDK-driven upload* has synced. It cannot confirm that GRTS has detected and uploaded background file changes.

### Implications for Test Design

1. **To trigger a conflict deterministically**, the GRTS device must be offline when dirty data is written AND must stay offline until AddUser is called. There is no safe window to enable network before AddUser without risking GRTS uploading first.

2. **On a single machine**, DisableNetwork is machine-wide (firewall rules). This creates a chicken-and-egg problem: DeviceA needs network to advance cloud, but DeviceB must stay offline to preserve dirty state.

3. **Test 110's pattern** (multi-device, Terminate/Relaunch, verified pass198-199): Phase 2 syncs + uploads baseline (SDK stays alive) → Phase 3 DeviceA advances cloud + DisableNetwork → Phase 3b DeviceB writes DIVERGENT offline + Terminate → Phase 4 Relaunch (offline) + EnableNetwork + SmokeDelay(3s) + AddUser → conflict fires → UseLocal + re-write + Upload → DeviceA verifies.

4. **Test 125's pattern** (single-device, Terminate/Relaunch) is the same core mechanism as test 110. Both require: Upload (commit to PGS) → DisableNetwork → Write DIVERGENT → Terminate → Relaunch offline → EnableNetwork late → AddUser.

5. **After UseLocal, Upload is a no-op** — GRTS syncs the PGS manifest to the cloud version during UseLocal resolution. Upload checks PGS version vs cloud and finds them equal. To propagate UseLocal's choice to cloud, the game must re-write data (same content) after UseLocal to mark it dirty in PGS, then Upload will detect the difference and push it.

6. **Multi-device ordering constraint**: The dirty-write + Terminate MUST happen in a separate block AFTER the other device advances cloud AND disables network. If Terminate is in an earlier block, GRTS can race-upload the dirty data when network is re-enabled for the other device's upload phase.

5. **Test 125 v9 working sequence** (verified): Phase 2 syncs + uploads INITIAL (SDK stays active) → Phase 3 DeviceA advances cloud + DisableNetwork → offline-terminate-grts writes DIVERGENT + Terminate → Phase 4 Relaunch (offline) + EnableNetwork + SmokeDelay(3s) + AddUser → **conflict fires** ✅

6. **The isWinner stomp is confirmed** (test 125 v9): After TakeRemote conflict resolution, the next upload carries the PendingConflictIsWinner flag in the HTTP body's Conflict JSON block. The upload finalizes successfully (HTTP 200), but the server treats the data as a conflict-winner replay. Other devices downloading afterward get the old conflict-winner data instead of the new save. **With server fix deployed, the server inverts isWinner for GRTS callers — data propagates correctly (test 132, pass167).**

### Rule 13: Server Fix Prevents Self-Reinforcing 409 Loop (Test 132)

The Benjamin Dow bug pattern involved 3 consecutive uploads within one session — all stomped because PendingConflictIsWinner persisted across multiple uploads within the same session. With the server fix deployed:

- Multiple consecutive uploads after a conflict all succeed (HTTP 200)
- Versions increment normally (e.g., 4544→4545→4546)
- Zero 409 rejections
- Data propagates correctly to other devices

Without the server fix, each stomped upload would leave the cloud in a stale state, potentially triggering additional conflicts on other devices — creating a self-reinforcing loop.

### Rule 14: PendingConflictIsWinner Scope & Lifecycle

Summary of the flag's lifecycle (consolidated from tests 126, 129, 130, 132, 133):

| Scenario | Flag persists? | Evidence |
|----------|---------------|----------|
| Same session, multiple uploads | **YES** | Test 126/132: 2nd/3rd uploads still carry flag |
| Clean uninit → new session | **NO** | Test 129: Flag gone after PFGameSaveFilesUninitialize + relaunch |
| TakeRemote → TakeLocal in same session | **YES** from first conflict | Test 133: TakeLocal doesn't clear TakeRemote's flag |
| Process crash (Terminate) | **Depends** | Flag survives in GRTS service memory if it auto-uploaded; otherwise lost |
| Clean uninit only (no relaunch) | **NO** | Uninit explicitly tears down GRTS context |
| Uninit → re-init (same process) | **NO** → new flag from new conflict | Test 139: Uninit clears old flag; re-init AddUser triggers fresh conflict with new PendingConflictIsWinner |

**Key insight:** The flag exists in GRTS service process memory (not game process memory, not disk). `PFGameSaveFilesUninitializeAsync` tears down the GRTS context object, destroying the flag. But Terminate (kill game process) does NOT destroy the GRTS context — GRTS continues running as a separate service process.

### Rule 15: UploadWithUi Reports LOCAL Sync Status, Not Cloud Upload

> **Discovered in test 135 (May 2026).**

`PFGameSaveFilesUploadWithUiAsync` has a two-phase architecture:

1. **Phase 1 (visible to game):** Sync save folder → GRTS blob cache (local disk, instant)
2. **Phase 2 (internal to GRTS):** Upload blob cache → cloud server (async, retried internally)

The `SyncFailed` callback reports Phase 1 status only. `state=SyncComplete, hr=0x00000000` means the local sync completed — even if network is disabled. Cloud upload failures in Phase 2 are handled internally by GamingServices.exe and are **NOT visible to the game**.

**Implications:**
- Cannot force `UploadWithUi` to return an error by disabling network adapters
- DisableNetwork prevents new connections but doesn't affect local blob cache sync
- Cloud upload retries are managed entirely by GRTS (invisible to game)

### Rule 16: GRTS Only Uploads Pre-Registered Containers

> **Discovered in test 138 (May 2026).**

Writing files to new directories in the save folder does NOT create GRTS containers. Only containers listed in the ExtManifest (from cloud registration) are tracked and uploaded. The ExtManifest typically contains `progress`, `roundtrip`, and `slotA` — additional containers must be registered through the GRTS API, not via filesystem writes.

**PendingConflictIsWinner is per-context (not per-container)** — it applies to the entire upload. There is no cross-container "leak" because the flag's scope is the upload context, which covers all containers.

### Rule 17: Two GRTS Devices on Same Machine → Contention, Not Conflict

> **Discovered in test 134 (May 2026).**

When two GRTS device roles run on the same physical machine, both share one `GamingServices.exe` process. This makes it **impossible** to trigger a true dual-GRTS conflict on a single machine:

1. DeviceB's `AddUser` → GRTS detects another device is active → **CONTENTION** (BreakLock), not conflict
2. After DeviceA's lock is broken, its deferred upload hits HTTP 409 → `PFUploadAbandonedClearSession` → dirty state cleared
3. DeviceA's next `AddUser` finds no dirty data → clean download → NO CONFLICT

**Implication:** "Two GRTS devices both TakeRemote" scenarios require two separate physical machines (or a real Xbox + PC). Single-machine tests always produce contention instead.

### Rule 18: No-Write Upload After TakeRemote Is Idempotent

> **Discovered in test 136 (May 2026).**

After resolving a conflict with TakeRemote, calling `PFGameSaveFilesUploadWithUiAsync` without writing any new data succeeds gracefully:

- Upload completes normally (server contacted, ~1689ms)
- No duplicate version created, no data corruption
- Server handles idempotent uploads correctly

**Implication:** Games do not need to guard `UploadWithUi` with "has data changed?" checks after conflict resolution.

### Rule 19: PendingConflictVersion = Cloud Winner Version in Conflict Block

> **Documented from test 137 ETL analysis (May 2026).**

The version reference in the conflict block is the **cloud version** that triggered the conflict:

| Field | Source | Example | Visible in ETL? |
|-------|--------|---------|-----------------|
| `PFContextLockContention.ConflictVersion` | ETL event | 4455 | Yes |
| `PFContextInitStart.conflictVersion` | ETL event (stored as PendingConflictVersion) | 4455 | Yes |
| `PendingConflictVersion` in HTTP body | Upload request | 4455 | **No** — only in HTTP request body |

After conflict resolution, GRTS creates 2-3 internal auto-upload versions before the game's explicit upload:

```
V4453 — Local loser version
V4454 — Content version downloaded from cloud
V4455 — Cloud version that triggered conflict (PendingConflictVersion)
V4456 — Abandoned internal version (dead)
V4457 — Auto-upload after conflict resolution (internal, manifest-only)
V4458 — Abandoned internal version (dead)
V4459 — Game's explicit upload (carries PendingConflictVersion=4455)
```

### Rule 20: Uninit/Re-Init Within Same Process Triggers Correct Conflict

> **Discovered in test 139 (May 2026).**

Calling `PFGameSaveFilesUninitializeAsync` → re-`PFGameSaveFilesInitialize` → `PFGameSaveFilesAddUserWithUiAsync` within the same process (without terminate/relaunch) correctly triggers the conflict callback when the cloud has advanced past the local state:

- The uninit tears down the GRTS context (Rule 14: clears PendingConflictIsWinner)
- The re-init creates a fresh context
- AddUser compares local state vs cloud and fires conflict callback

**Difference from terminate/relaunch:** Uninit/re-init is cleaner — it doesn't leave orphaned GRTS uploads. However, the post-conflict upload still carries PendingConflictIsWinner (set during the new conflict resolution), which causes the isWinner stomp on the server side until the server fix addresses it.

**Test 139 evidence:** DeviceB uninit/re-init → AddUser → ConflictCallback fired (local 3566 bytes, remote 10240 bytes) → TakeRemote → downloaded cloud data → wrote new data → uploaded → DeviceA re-downloaded → **hash mismatch** (isWinner stomp — DeviceB's upload was treated as old conflict winner replay).

### Rule 21: SDK Inproc Conflict Requires Dirty Local Data (Test 140)

The SDK inproc engine handles stale local data differently from GRTS:

| Scenario | GRTS Behavior | SDK Inproc Behavior |
|----------|---------------|---------------------|
| Stale-but-clean local (matches cloudsync manifest) + cloud advanced | CONFLICT | NO conflict — silently downloads newer version |
| Dirty local (differs from cloudsync manifest) + cloud advanced | CONFLICT | CONFLICT |
| Dirty local + cloud same as last upload | Local wins (no sync needed) | Local wins (no sync needed) |

**Key insight:** The inproc engine compares local files against its own cloudsync manifest first. If they match (data was already uploaded), it knows there's no real conflict — the local device just missed an update. It downloads the newer cloud version silently (`conflictFound=0, downloading=1`).

A conflict only fires when local files have been modified since the last upload AND the cloud has also advanced. This is arguably smarter than GRTS behavior.

**Test 140 evidence (pass177):** Phase 2 wrote [0x55,0x66,0x77,0x88] AFTER uploading [0x11,0x22,0x33,0x44] → dirty local state. Phase 3 advanced cloud. Phase 4 AddUser → `conflictFound=1` → ConflictCallback fired → UseCloud → POST_CONFLICT data uploaded → DeviceA snapshot MATCHED. No isWinner stomp.

**SDK vs GRTS upload behavior:** SDK inproc does NOT include `PendingConflictIsWinner` or any conflict metadata in its upload HTTP body. Post-conflict uploads are clean, making the server-side isWinner fix irrelevant for SDK callers.

### Rule 22: GRTS Post-Conflict Upload Requires KeepActive First (Test 143)

After GRTS resolves a conflict (e.g., TakeRemote/UseCloud), the `PendingConflictIsWinner` flag persists in the GRTS session. A single `ReleaseDeviceAsActive` upload after conflict resolution **appears to succeed** (`hr=0x00000000`) but the data may NOT propagate to other devices — the server stomps/discards it due to the conflict metadata.

**The fix:** Use a dual-upload pattern after conflict resolution:
1. Write data → Upload with `KeepDeviceActive` (processes conflict metadata)
2. Wait ≥5 seconds
3. Write final data → Upload with `ReleaseDeviceAsActive` (clean upload)

**Test 143 evidence:**
- pass181 (FAIL): Single `ReleaseDeviceAsActive` upload after conflict → DeviceA got stale v11 data, not DeviceB's post-conflict data. Upload returned `hr=0x00000000` (silent failure).
- pass182 (PASS): Dual upload (KeepActive first, then Release) → DeviceA got correct data. Snapshots matched.

**Why it works:** The KeepDeviceActive upload triggers the server's conflict resolution logic. The server processes and flips the `PendingConflictIsWinner` flag on this first upload. The second upload arrives with cleared conflict state and propagates normally.

**This matches test 126's proven pattern**, which also used KeepActive→Release dual uploads after conflict and passed successfully.

**Game developer impact:** Games using GRTS that write new data after conflict resolution and immediately upload with `ReleaseDeviceAsActive` may silently lose that data. Always do a KeepActive upload first.

### Rule 23: GRTS Upload-Time Conflict Does NOT Clear With Dual Upload (Test 144)

When GRTS detects a stale manifest during `UploadWithUiAsync` (not during `AddUserWithUiAsync`), it fires the conflict callback **mid-upload**. The upload completes with `hr=0`, but:

1. The conflict was resolved DURING the upload, so PendingConflictIsWinner is set on this upload
2. The PendingConflictIsWinner flag **persists to subsequent uploads** in the same session
3. Even the dual-upload pattern (Rule 22) doesn't clear the flag, because the KeepActive upload IS the conflict resolution upload

**Test 144 evidence (pass183):**
- Phase 4 Upload #1 (KeepActive): ConflictCallback fired at 23:52:12 → UseCloud → hr=0 → data STOMPED
- Phase 4 Upload #2 (Release): no conflict → hr=0 → data STILL STOMPED
- DeviceA got DeviceA's own Phase 3 data, not DeviceB's Phase 4 data

**Contrast with Rule 22 (test 143):** Rule 22's dual-upload works when conflict is resolved during AddUser (BEFORE any upload). Rule 23 shows it fails when conflict fires during upload.

**GRTS does not expose HTTP 409 to games.** Instead, it internally detects the stale manifest, fires the conflict callback, and resolves. Games never see a 409 error code. But the silent data loss from PendingConflictIsWinner persists.

**Workaround:** After an upload-time conflict, uninitialize → re-initialize → AddUser → dual-upload pattern. Full session reset is needed to clear PendingConflictIsWinner when it was set during an upload.

### Rule 24: isWinner Only Sent on Conflict Uploads, Not Clean Uploads (Test 121)

The isWinner flag (and the entire `Conflict` JSON block) is **only sent to the service when the upload is treated as a conflict upload**
(gated behind `if (!conflictDesc.version.empty())` in NtmWebService.cpp:3892).

If a device resolves a conflict via UseCloud first (syncing to the latest version), then writes
new data on top and uploads, that upload is **often** a clean upload with no conflict descriptor.
In that case, isWinner is never sent and the isWinner bug cannot manifest.

However, this interacts with Rules 9/10/22/23: after TakeRemote, GRTS can keep a non-empty
PendingConflictDesc in memory and carry it onto one or more subsequent uploads in the same session.
Those uploads are **not clean** (they include the conflict block), which is why post-UseCloud uploads
can still be stomped unless you follow the KeepActive-first / session-reset guidance.

**Evidence (test 121):** DeviceB (GRTS) resolves UseCloud conflict → syncs to v3 → writes v4 →
uploads cleanly. The v4 upload succeeds and cloud correctly has v4. The isWinner regression does
NOT trigger because the upload has no conflict context.

**Implication:** To trigger the isWinner bug in a test, you need an upload where `conflictDesc.version` is non-empty. This can happen via:
1. Upload-time conflict detection (see Rule 23)
2. Direct upload without prior AddUser conflict resolution
3. Race conditions where another device uploads between AddUser and the device's upload
4. Post-TakeRemote uploads that still carry PendingConflictDesc (see Rules 9/10/22)

### Rule 25: PC GRTS TCUI Limitations (Batch Analysis, May 2026)

PC GRTS has several limitations compared to Xbox GRTS related to TCUI (Title Callable UI):

1. **AddUserWithUiAsync hangs offline** — When network is unavailable and SyncFailedAutoResponse=UseOffline is configured, AddUser still hangs for 180-240s (timeout). The callback never fires or navigator can't dismiss the dialog. *Regression introduced at pass75.*

2. **Descriptor callbacks don't fire** — `VerifyContentionDescriptor`, `VerifyConflictDescriptor`, and `VerifyQuotaDelta` either return immediate E_FAIL or hang indefinitely. PC GRTS either doesn't generate these descriptors or the TCUI callback mechanism is broken.

3. **Cancel operations unsupported** — Cancel-during-AddUser and cancel-during-Upload both return E_FAIL (0x80004005). The cancel flow requires TCUI cancel buttons that don't exist on PC.

4. **Background upload on process exit is not PLM-based (but is less reliable on PC)** — On PC, GRTS detects game termination via a process-handle wait and will start the PF upload path on exit. However, there is no PLM-managed grace window to keep the service alive while the upload finishes; if the service shuts down or is interrupted mid-upload, the session remains pending and is resumed later via `StartPendingUploads()` (registry `PendingPlayFabSessions`) on service boot/network reconnect.

5. **XUserSignOutAsync returns E_NOTIMPL** — PC has no Xbox sign-out concept.

6. **Invalid path validation deferred** — `PFGameSaveFilesInitialize` with non-existent path (e.g., `Z:\nonexistent\badpath\`) succeeds on PC instead of returning `E_GS_PLAYFAB_INVALID_LOCATION`. Path validation is deferred to first sync/write.

**Implication for test classification:** Tests that depend on TCUI callbacks, cancel UI, PLM notifications, sign-out, or eager path validation should be marked `pc-grts: ignore` in the test suite.

### Rule 26: UseLocal (TakeLocal) Makes Upload a No-Op — Re-Write Required (Test 110)

When GRTS resolves a conflict with UseLocal (TakeLocal, Choice:0), the PGS manifest is updated to match the **cloud** version number. This makes PGS and cloud appear in sync from GRTS's perspective. Subsequent `UploadWithUiAsync` calls compare PGS version vs cloud version, find them equal, and skip the upload entirely (no-op).

**Mechanism:**
1. Conflict fires: PGS has version N, cloud has version N+1
2. UseLocal chosen: save folder data preserved (local bytes), but PGS manifest synced to version N+1
3. Upload called: PGS version (N+1) == cloud version (N+1) → no upload needed
4. Result: local data never propagates to cloud

**Fix pattern (test 110, pass198):**
After UseLocal + GetFolder (to confirm local data is intact), **re-write the same data** to the save folder. This creates a mismatch between save folder content and PGS blob (which now contains cloud data). Upload then detects the difference and pushes local data to cloud.

```yaml
# After UseLocal conflict resolution:
- action: GetFolder           # Confirms local data intact
- action: Write               # Re-write same data — marks save folder dirty vs PGS
  value: "0xBB"
- action: UploadWithUiAsync   # Now detects mismatch, pushes to cloud
  option: ReleaseDeviceAsActive
```

**Test 110 evidence (pass198):** Without re-write, Upload was a no-op and DeviceA got 0xCC (cloud data). With re-write, Upload pushed 0xBB and DeviceA got 0xBB (correct UseLocal result).

**Contrast with UseRemote (TakeRemote):** UseRemote downloads cloud data into both PGS and save folder, so a subsequent upload of new data works normally (new write creates genuine dirty state).

## Test YAML Audit: Multi-Device Folder Rules

An audit of all 68 multi-device xplat tests found **7 tests with YAML recipe violations**. These were fixed in May 2026.

### Rules for Multi-Device Tests

1. **Each device role MUST use its own save folder**: `C:\gamesaves-test\DeviceA` for DeviceA, `C:\gamesaves-test\DeviceB` for DeviceB. Both `DeleteLocalFolder` and `PFGameSaveFilesInitialize` must use the correct device-specific path.

2. **Do NOT `DeleteLocalFolder` before a conflict-triggering `AddUserWithUiAsync`**: GRTS needs to find stale local data that differs from cloud to detect a conflict. Deleting the folder first removes the divergent state.

3. **Contention tests don't need divergent data**, but still need separate folders per device to avoid cross-contamination.

### Fixed Tests

| Test | Violation | Fix Applied |
|------|-----------|-------------|
| **07** | `session-b-verify` used bare `C:\gamesaves-test` (no subfolder) | Changed to `C:\gamesaves-test\DeviceB` |
| **107** | DeviceB block used `C:\gamesaves-test\DeviceA` | Changed to `C:\gamesaves-test\DeviceB` |
| **109** | Both DeviceB blocks (`d1-xbox-holds-lock`, `d2-xbox-contends`) used `DeviceA` path | Changed to `C:\gamesaves-test\DeviceB` |
| **110** | DeviceB blocks used `DeviceA` path; reconnect block had premature `DeleteLocalFolder` | Changed to `DeviceB` path; removed `DeleteLocalFolder` |
| **112** | DeviceB blocks used `DeviceA` path; `pc-reconnect-resolve-uselocal` had premature `DeleteLocalFolder` | Changed to `DeviceB` path; removed delete |
| **16** | `device-b-relaunch-resolve-local` had premature `DeleteLocalFolder` | Removed delete (local data needed for conflict re-prompt) |
| **24** | `device-a-launch-contention-then-conflict` had premature `DeleteLocalFolder` | Removed delete (divergent data needed for conflict) |
| **78** | `session-three-reconnect-resolve` had premature `DeleteLocalFolder` | Removed delete (divergent v2 data needed for conflict) |

### Post-Fix Results

- **07, 107, 109**: PASS ✓ (folder isolation fixes worked)
- **110**: PASS ✓ (folder fix + Terminate/Relaunch restructure + UseLocal re-write, Rule 26)
- **16, 24, 78**: Still FAIL — pre-existing failures unrelated to folder issues (GRTS returns `E_GS_USER_CANCELED` or upload fails). These are likely GRTS service bugs.
- **112**: Now FAIL — was previously passing by accident because both devices shared the same folder, making snapshot comparison trivially match. The test logic needs redesign.

## Debugging GRTS

### Inspect Current State

```powershell
# View all contexts and manifests
Get-ChildItem C:\XboxGames\GameSave\pgs -Recurse

# Read context state for a user
Get-Content C:\XboxGames\GameSave\pgs\u_{XUID}_{TitleID}\{XUID}_{TitleID}.json | ConvertFrom-Json | ConvertTo-Json -Depth 5

# Check latest manifest version
Get-ChildItem C:\XboxGames\GameSave\pgs\u_{XUID}_{TitleID}\*.json | Where-Object { $_.Name -match '^\d+\.json$' }
```

### Registry

GRTS uses registry keys under `HKLM\SOFTWARE\Microsoft\XGameSaveStorage`:
- `PlayFab` — PlayFab-specific configuration (currently empty in dev)
- `Uploads` — Upload tracking (currently empty in dev)

### Key Files to Read for Debugging

| File | When to Read |
|------|-------------|
| `service/lib/PFContext.cpp` | Activation flow, conflict resolution, upload flow |
| `common/PFCommon.h` | State enums, version tracking, error parsing |
| `common/Manifests.h` | File/folder manifest structures |
| `docs/PlayFab_State_Machines.md` | Complete state machine diagrams |
| `idl/ConnectedStorageErrorCodes.mc` | Error code reference |

### Common Debug Scenarios

**"E_GS_USER_CANCELED but user didn't cancel"**: GRTS stock TCUI intercepted a SyncFailed event and the user's response (e.g., "Play offline") maps to `E_GS_USER_CANCELED`. The SDK never sees the SyncFailed callback.

**"Upload seems stuck"**: Check the context file's `SyncStatus` and `UploadVersion`. If `SyncStatus=2` (NOT_SYNCHRONIZED) and `UploadVersion` is set, the upload may be in progress. Check `SyncErrorCode` for any errors.

**"Contention dialog won't go away"**: GRTS polls `QueryUploadProgress` on the other device's upload. If the other device's upload is stalled or the service returns unexpected data, the contention dialog stays until timeout.

**"Conflict data doesn't match"**: Check `LastConflictWinner` and `LastConflictLoser` in the context file. Cross-reference with the manifest versions to verify which version was kept vs discarded.

### ETW Tracing (The Definitive Debug Tool)

GRTS emits detailed ETW events via the `Microsoft.Gaming.PlayFab.GameSaveTrace` provider. These events are the **definitive** source of truth for debugging — they show exact HTTP requests/responses, upload lifecycle, context creation/destruction, and internal state transitions.

#### Quick Start

```powershell
# Decode and print all GRTS events from the last 10 minutes:
py Utilities\Scripts\read-grts-etl.py --last-minutes 10

# Write to file:
py Utilities\Scripts\read-grts-etl.py -o trace.txt

# JSON output for programmatic analysis:
py Utilities\Scripts\read-grts-etl.py --json -o trace.json

# Filter to just the PlayFab GameSaveTrace provider:
py Utilities\Scripts\read-grts-etl.py --provider "Microsoft.Gaming.PlayFab.GameSaveTrace"

# Skip flushing (if already flushed manually):
py Utilities\Scripts\read-grts-etl.py --no-flush
```

#### How It Works

The GamingServices auto-logger session writes ETL files to `C:\Windows\System32\LogFiles\WMI\`. The ETL files have **no embedded manifests** — the schema is built into the telemetry and decoded by Windows TDH using system-registered providers. The session buffers events in memory and periodically flushes to disk. To ensure you see the latest events:

```powershell
# Force flush buffered events to disk:
logman update "GamingServices" -ets -fd
```

The `read-grts-etl.py` script does this automatically (unless `--no-flush` is passed).

#### ETL Session Details

| Property | Value |
|----------|-------|
| Session name | `GamingServices` |
| ETL location | `C:\Windows\System32\LogFiles\WMI\GamingServices.etl` |
| Provider name | `Microsoft.Gaming.PlayFab.GameSaveTrace` |
| Provider GUID | `{bd401bba-fe3f-4e6b-85bf-4f5c8f52137b}` |
| Trace level | 5 (Verbose) — all levels logged |
| Providers registered | 22 total on this session |

#### Providers in the GamingServices ETL

The ETL files contain events from multiple providers (all decoded by `read-grts-etl.py`):

| Provider | Events | What It Tells You |
|----------|--------|-------------------|
| `Microsoft.Xbox.XAL.GRTS-All` | ~8500 | XAL authentication (verbose — most events) |
| `Microsoft-Xbox-GameInput` | ~180 | Game input device events |
| `Microsoft.Gaming.Install` | ~140 | Package install/registration |
| `Microsoft.Gaming.GameFlt` | ~40 | **Process lifecycle** (game start/exit, PID tracking) |
| `Microsoft.Gaming.PlayFab.GameSaveTrace` | ~35 | **Core game save** (upload, download, context, HTTP) |
| `Microsoft.Xbox.XAL.GRTS` | ~20 | XAL auth (summary events) |
| `Microsoft.Xbox.GameSave.Trace` | ~13 | Legacy game save events |
| `Microsoft.Gaming.GRTS` | ~4 | GRTS service lifecycle |
| `Microsoft.Gaming.PlayFab.GameSave` | ~3 | PlayFab GameSave telemetry |

For debugging game save issues, focus on `Microsoft.Gaming.PlayFab.GameSaveTrace` and `Microsoft.Gaming.GameFlt`.

#### Key Events to Watch For

| Event Name | Level | Meaning |
|------------|-------|---------|
| `PFXGameSaveServicePrepareContextWithConfig` | VERB | Game initializing (shows TitleId, EntityId, flags) |
| `NewPlayFabCaller` | INFO | GRTS registered the game process (shows PID, AUMID) |
| `PFResponse` | INFO | HTTP response from PlayFab (RequestType, HttpStatus, ElapsedMs) |
| `PFContextCreated` | INFO | Context fully activated (download complete) |
| `PFUploadWorkerEntered` | INFO | Background upload starting |
| `PFUploadPlanComplete` | VERB | Upload plan ready (UploadPendingSize, UploadChunks) |
| `PFUploadContextUploadChunk` | INFO | Chunk uploaded (uploadUri, uploadSize, hr) |
| `PFUploadContextFinalized` | INFO | Manifest finalized on server — upload succeeded |
| `PFUploadContextComplete` | INFO | Upload lifecycle complete (FileCount, TotalSize, ElapsedMs) |
| `PFUploadContextUploadError` | ERROR | Upload failed (state, ElapsedMs) |
| `GameProcessStateChange` | INFO | Process state changed (PID, IsGameProcessListEmpty) |

#### Common PFResponse RequestTypes

| RequestType | PlayFab API |
|-------------|-------------|
| `PFR_LOGIN` | Entity login |
| `PFR_LIST_MANIFESTS` | List game save manifests |
| `PFR_INIT_UPLOAD` | Start upload session |
| `PFR_PUSH_UPLOAD` | Upload blob chunk |
| `PFR_FINALIZE_MANIFEST` | Commit upload |
| `PFR_DOWNLOAD_BLOBS` | Download save blobs |

#### Workflow: Debugging a Test Failure with ETL

1. Run the test: `py tests-run.py gamesave-pc --only 03`
2. Immediately after: `py Utilities\Scripts\read-grts-etl.py --last-minutes 5 -o trace.txt`
3. Cross-reference timestamps in `trace.txt` with `controller.log` in the test output folder
4. Look for: ERROR-level events, unexpected `Hresult` values, missing `PFUploadContextComplete` events

#### Manual ETL Commands (if script isn't available)

```powershell
# Query the active session:
logman query "GamingServices" -ets

# Flush buffers to disk:
logman update "GamingServices" -ets -fd

# Decode with tracerpt (basic XML — no field names, less useful):
tracerpt C:\Windows\System32\LogFiles\WMI\GamingServices.etl -of XML -lr -o trace.xml
```

The `read-grts-etl.py` script is strongly preferred over `tracerpt` because it uses Windows TDH to decode structured field names and values from the system-registered provider manifest.

## SDK-Side GRTS Provider (The Bridge Layer)

The SDK talks to GRTS through `GameSaveAPIProviderGRTS` (`Source/PlayFabGameSave/Source/Platform/Windows/PFGameSaveFilesAPIProvider_GRTS.cpp`, ~1566 lines). This is a thin bridge that translates PFGameSave API calls into `PFXGameSave*` calls (the GDK's game save API that talks to the GRTS service over RPC).

### Key Data Structures

```cpp
// Per-user state cached in the SDK
struct PFXPALGameSaveUserState {
    PFLocalUserHandle localUser;
    XUserHandle xUser;
    void* configHandle;          // PFXGameSaveConfigHandle from GRTS
    char saveFolder[1024];       // Folder returned by GRTS after AddUser
    bool isConnectedToCloud;     // false if user went offline
};

// Global SDK-GRTS context (up to 16 users)
struct PFXPALGameSaveContext {
    PFGameSaveFilesUiProgressCallback* progressCallback;
    PFGameSaveFilesUiSyncFailedCallback* syncFailedCallback;
    PFGameSaveFilesUiActiveDeviceContentionCallback* activeDeviceContentionCallback;
    PFGameSaveFilesUiConflictCallback* conflictCallback;
    PFGameSaveFilesUiOutOfStorageCallback* outOfStorageCallback;
    PFXPALGameSaveUserState users[16];
};
```

### AddUserWithUiAsync Flow (The Critical Path)

This is the most complex method — it initializes a user's save context with GRTS:

```
AddUserWithUiAsync
  ├── PFXPALAddUserBegin
  │     ├── Get service config (titleId, endpoint)
  │     ├── Try XUser path (GDK auth)
  │     │     └── PFXPALCallGetFolderWithUiAsync (direct)
  │     └── Fallback: Entity auth path
  │           ├── PFLocalUserLoginAsync → PFXPALLocalUserLoginComplete
  │           │     ├── Get entity key + entity token
  │           │     ├── PFEntityGetEntityTokenAsync → PFXPALEntityTokenComplete
  │           │     └── PFXPALCallGetFolderWithUiAsync (with entity auth)
  │           └── No auth available
  │                 └── PFXPALCallGetFolderWithUiAsync (without auth)
  │
  └── PFXPALCallGetFolderWithUiAsync
        ├── Build PFXGameSaveConfigRequest (flags, titleId, apiUrl, fileLocation)
        ├── PFXGameSaveInitializeConfig → configHandle
        ├── Assign user slot (up to 16 users)
        └── PFXGameSaveFilesGetFolderWithUiAsync → PFXPALGetFolderComplete
              ├── SUCCESS → store saveFolder, isConnectedToCloud = true
              ├── E_GS_USER_CANCELED → store saveFolder, isConnectedToCloud = false
              └── OTHER FAILURE → complete with error
```

### E_GS_USER_CANCELED Handling (Important!)

When GRTS returns `E_GS_USER_CANCELED` (0x80830004), the SDK translates it to S_OK but marks the user as offline:

```cpp
if (hr == E_GS_USER_CANCELED) {
    // User went offline via stock UI
    isConnectedToCloud = false;
    hr = S_OK;  // Still succeed — user has local data
}
```

This is the key to the "E_GS_USER_CANCELED" test failures — GRTS returns this code for multiple situations:
- User clicked "Cancel" in stock TCUI
- User clicked "Play offline" in stock TCUI  
- Stock TCUI intercepted SyncFailed before SDK's callback fired

### UI Callback Bridge

The SDK registers bridge callbacks with GRTS that translate `PFXGameSave*` types into `PFGameSave*` types:

| SDK Callback | GRTS Bridge Function | GRTS API |
|-------------|---------------------|----------|
| `progressCallback` | `MyPFXPALGameSaveProgressUiCallback` | `PFXGameSaveSetUiCallbacks` |
| `syncFailedCallback` | `MyPFXPALGameSaveSyncFailedUiCallback` | (same) |
| `activeDeviceContentionCallback` | `MyPFXPALGameSaveActiveDeviceContentionUiCallback` | (same) |
| `conflictCallback` | `MyPFXPALGameSaveConflictUiCallback` | (same) |
| `outOfStorageCallback` | `MyPFXPALGameSaveOutOfStorageUiCallback` | (same) |

Each bridge function:
1. Finds the `PFLocalUserHandle` from the `XUserHandle` GRTS passes
2. Converts `PFXGameSaveDescriptor` → `PFGameSaveDescriptor`
3. Calls the game's registered callback

### Config Request Flags

```cpp
enum PFXGameSaveConfigRequestInitFlags {
    InitFlagNone                    = 0,
    InitFlagUseEntityAuth           = 0x4,   // Pass entity token to GRTS
    InitFlagUseFileLocation         = 0x8,   // Override save folder path
    InitFlagRollbackToLastKnownGood = 0x10,  // Request rollback
    InitFlagRollbackToLastConflict  = 0x20,  // Rollback to conflict version
};
```

### What's NOT Implemented in GRTS Provider

These methods return `E_NOTIMPL` or no-op:
- `ResetCloudAsync` — returns `E_NOTIMPL`
- `GetSaveDescriptionSizeForDebug` / `GetSaveDescriptionForDebug` — return `E_NOTIMPL`
- `SetMockDeviceIdForDebug`, `SetMockManifestOffsetForDebug`, `SetMockDataFolderForDebug` — no-op (only useful for inproc)
- `SetForceOutOfStorageErrorForDebug`, `SetForceSyncFailedErrorForDebug` — no-op
- `PauseUploadForDebug`, `ResumeUploadForDebug` — no-op
- `GetStatsJsonForDebug` — returns empty string

## Related Documentation

- [XPlat Testing Guide](ai-xplat-testing-guide.md) — test infrastructure, scripts, known failures
- [GRTS State Machines](C:\git\ConnectedStorage\docs\PlayFab_State_Machines.md) — full state diagrams with code line references
- [IsWinner Bug](C:\git\PlayFab.C\specs\playfab-gamesave\xplat-testing\status\gamesave-iswinner-semantics-inverted.md) — SDK vs GRTS semantics mismatch

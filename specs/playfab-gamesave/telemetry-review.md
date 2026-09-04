# PFGameSave Telemetry Events — Codebase Review

This document maps each PowerBI telemetry event from `gamesave-telemetry-events.md` to its codebase emit sites, validates field population, and identifies gaps or concerns.

---

## PlatformType During In-Proc Execution

`PlatformType` is resolved by `PFPlatformGetPlatformType()` in `Source/PlayFabCore/Source/Platform/Windows/Platform_Windows.cpp` and is **independent** of the in-proc / out-of-proc game-save routing decision. The `ForceUseInprocGameSaves` registry key (under `HKLM\SOFTWARE\Microsoft\GamingServices`) only controls which `GameSaveAPIProvider` is used — it does **not** change the `PlatformType`.

| Scenario | PlatformType | In-Proc? | Notes |
|----------|-------------|----------|-------|
| Xbox Console (GDK) | `Xbox` | No (GRTS) | `IsRunningOnXboxConsoleGdk()` → true |
| Xbox Console + `ForceUseInprocGameSaves=1` | `Xbox` | **Yes** | PlatformType unchanged; only API provider switches |
| Windows PC with GRTS available | `Windows` | No (GRTS) | `IsOutOfProcGRTSAvailable()` true |
| Windows PC with GRTS + `ForceUseInprocGameSaves=1` | `Windows` | **Yes** | PlatformType still `Windows`; game saves forced in-proc |
| Windows PC with `ForceUseLocalServices=1` | `Windows` | **Yes** (local) | `GRTSAvailable` NOT set; Win32 provider used |
| Windows PC without GRTS (no registry key) | `Windows` | **Yes** (local) | GRTS unavailable → Win32 provider |
| Steam PC with GRTS | `SteamPc` | No (GRTS) | `IsRunningOnSteam()` + GRTS available |
| Steam PC with GRTS + `ForceUseInprocGameSaves=1` | `SteamPc` | **Yes** | PlatformType still `SteamPc` |
| Steam Deck (no GRTS) | `SteamDeck` | **Yes** (always) | `IsRunningOnSteam()` + no GRTS → always in-proc |
| Generic/Linux/Other | `Unknown` | **Yes** (always) | `Platform_Generic.cpp` returns `Unknown` |

> **Action:** When `ForceUseInprocGameSaves=1` is set, `PFPlatformGetPlatformType()` should return `WindowsInproc` instead of `Windows`. This requires adding a `WindowsInproc` value to `PFPlatformType` (in `PFPlatform.h`) and updating the detection logic in `Platform_Windows.cpp` to check the `ForceUseInprocGameSaves` registry key. This lets PowerBI slice telemetry by in-proc vs out-of-proc without adding a new field to every event.

---

## Telemetry Architecture

- **Manager class**: `GameSaveTelemetryManager` (`Source/PlayFabGameSave/Source/Common/GameSaveTelemetryManager.h/.cpp`)
- **Pipeline**: Created via `CreateEntityTelemetryPipeline()` using `PFEventPipelineCreateTelemetryPipelineHandleWithEntity`. Events are emitted to `playfab.gamesave.internal` namespace.
- **Common fields on every event**: `userId` (platform local ID), `platformType`, and `sessionId` — added by `PopulateCommonFields()`.
- **`sessionId`**: A GUID generated per activation attempt via `CreateGUID()` in `ResetContextActivation()`. All events emitted during a single gameplay session (activation, activation failure, sync, sync error, delete) share the same `sessionId`, allowing PowerBI to correlate uploads and subsequent events back to the originating activation.
- **`platformType`**: Now serialized as a string. When the `ForceUseInprocGameSaves` registry key is set and the platform is Windows, the value is overridden from `"Windows"` to `"WindowsInproc"` to distinguish in-proc from out-of-proc execution in PowerBI.
- **Emit guards**:
  - `EmitContextActivationEvent()`: once-only guard via `m_contextActivationEventEmitted`.
  - `EmitContextActivationFailureEvent()`: skips if `m_contextActivationEventEmitted` is already true (i.e., only fires when the success event has NOT yet been emitted).
  - `EmitContextSyncEvent()`: once-only guard via `m_contextSyncEventEmitted`.
  - `EmitContextSyncErrorEvent()`: skips if `m_contextSyncEventEmitted` is already true.
  - `EmitContextDeleteEvent()`: no once-only guard, but skips if `startTime == 0` or if `(DT_Local|DT_All)` with `totalSizeBytes == 0`.

---

## Event 1: ContextActivationFailure

**PowerBI metric**: Activation Error Failure Rate = `COUNT(HResultUint) / COUNT(All ContextActivationFailures)`, sliced by `PlatformType`.  
**Emit method**: `EmitContextActivationFailureEvent()` → wire name `"context_activation_failure"`.

### Emit Sites

| Location | File | Trigger |
|----------|------|---------|
| ListManifests HTTP failure | `LockStep.cpp:208-209` | `SetContextActivationHResult(result.hr)` then `EmitContextActivationFailureEvent()` |
| InitializeManifest HTTP failure | `LockStep.cpp:482-483` | Same pattern |
| GetManifestDownloadDetails HTTP failure | `CompareStep.cpp:75-76` | Same pattern |
| Extended manifest download failure | `CompareStep.cpp:141-142` | Same pattern |
| InitWithExtendedManifest parse failure | `CompareStep.cpp:169-170` | Same pattern |
| Top-level activation failure catch-all | `FolderSyncManager.cpp:67-80` (`FireActivationFailedTelemetry`) | Sets `canceled`/`aborted`/`hresult`/`syncState` then emits. Called from lines 90, 101, 163 |

### Field Population Review

| Field | Set By | Coverage |
|-------|--------|----------|
| `hresult` | `SetContextActivationHResult()` — called at every emit site above | ✅ All paths |
| `syncState` | `SetContextActivationSyncState()` — set in `FireActivationFailedTelemetry` (Preparing or NotStarted), and shared via `SetContextActivationSyncState` in LockStep login flow | ✅ All paths |
| `callingLocation` | `SetContextActivationCallingLocation()` — called at entry of both `LockStep::AcquireActiveDevice` (line 140) and `CompareStep::CompareWithCloud` (line 45) | ✅ Automatically updated before each stage |
| `httpStatus` | `SetContextActivationHttpInfo()` — called in LockStep (lines 164, 204, 478) and CompareStep (line 71) | ✅ Set whenever HTTP result is available |
| `canceled` | `SetContextActivationCanceled(true)` — in `FireActivationFailedTelemetry` when `hr == E_PF_GAMESAVE_USER_CANCELLED` | ✅ |
| `aborted` | `SetContextActivationAborted(true)` — in `FireActivationFailedTelemetry` when `hr == E_ABORT` | ✅ |
| `retryAllowed` | `SetContextActivationRetryAllowed()` — defaults to `true` | ✅ |
| `startedOnline` | `SetContextActivationStartedOnline(false)` — set in LockStep offline-mode callbacks (lines 605, 622, 640) | ✅ |
| `correlationVector` | Via `SetContextActivationHttpInfo()` | ✅ |

### Guard Correctness

The failure event checks `if (m_contextActivationEventEmitted) return S_OK`. This means:
- In `LockStep` and `CompareStep`: failure events are emitted **before** returning the error to `FolderSyncManager`, which is correct because `EmitContextActivationEvent()` has not been called yet at that point.
- In `FireActivationFailedTelemetry()` (FolderSyncManager.cpp:79-80): both failure and success events are emitted in sequence. The failure fires first (guard not yet set), then the success event sets `m_contextActivationEventEmitted = true`. ✅ Correct order.

### Findings

- ✅ All identified failure paths during `AddUserWithUiAsync` emit this event.
- ⚠️ Login failure path(LockStep `WaitForFailedUI_Login` → `LockStepFailure`): the `LockStepFailure` case just returns `m_failureHR`. The failure event is NOT emitted inside LockStep for Login failure; it relies on `FolderSyncManager::FireActivationFailedTelemetry` at line 101. This works because `AcquireActiveDevice` returns the failure HR, and `FolderSyncManager` catches it. ✅ Covered.

---

## Event 2: ContextSyncError

**PowerBI metric**: Sync Error Failure Rate = `COUNT(HResultUint) / COUNT(All ContextSyncErrors)`, filtered by `SyncDownload`.  
**Emit method**: `EmitContextSyncErrorEvent()` → wire name `"context_sync_error"`.

### Emit Sites

| Location | File | errorSource |
|----------|------|-------------|
| Download: per-block HTTP failure | `DownloadStep.cpp:249-250` | `SER_DownloadData` (set line 119) |
| Download: post-extract IO error | `DownloadStep.cpp:318-319` | `SER_DownloadData` |
| Upload: delete-before-upload failure | `UploadStep.cpp:367-368` | `SER_RegisterUpload` (set line 452) |
| Upload: RegisterUpload HTTP failure | `UploadStep.cpp:569-570` | `SER_RegisterUpload` |
| Upload: per-block upload HTTP failure | `UploadStep.cpp:755-756` | `SER_UploadData` (set line 609) |
| Upload: per-block continued | `UploadStep.cpp:880-881` | `SER_UploadData` |
| Upload: per-block continued | `UploadStep.cpp:990-991` | `SER_UploadData` |
| Upload: FinalizeManifest stage failures | Set at `UploadStep.cpp:681` | `SER_FinalizeUpload` |
| FolderSyncManager download IO rollup | `FolderSyncManager.cpp:339` | Inherits from DownloadStep |
| FolderSyncManager upload failure rollup | `FolderSyncManager.cpp:642` | Inherits from UploadStep |

### Field Population Review

| Field | Set By | Coverage |
|-------|--------|----------|
| `hresult` | `SetContextSyncHResult()` — called at every emit site | ✅ |
| `errorSource` | `SetContextSyncSyncErrorSource()` — set before each async phase (ListManifests in LockStep:193, GetManifest in CompareStep:103, DownloadData in DownloadStep:119, RegisterUpload in UploadStep:452, UploadData in UploadStep:609, FinalizeUpload in UploadStep:681) | ✅ All 6 enum values used |
| `blockName` | `SetContextSyncBlockName()` — set in DownloadStep:220 and UploadStep:608 per-block | ✅ |
| `contextVersion` | `SetContextSyncContextVersion()` — set in FolderSyncManager:275, UploadStep:453, also via shared setter | ✅ |
| `retryCount` | Via `SetContextSyncHttpInfo()` | ✅ |
| `correlationVector` | Via `SetContextSyncHttpInfo()` | ✅ |
| `elapsedMs` | Computed from `startTime` set by `SetContextSyncStartTime()` | ✅ |

### Guard Correctness

`EmitContextSyncErrorEvent()` checks `if (m_contextSyncEventEmitted) return S_OK`. Error events must fire before the sync-success event. At every emit site above, the error is emitted immediately on failure, before any path to `EmitContextSyncEvent()`. ✅ Correct.

### Findings

- ✅ All identified HTTP and IO failure paths emit this event with correct `errorSource`.
- ✅ `SER_ListManifests` (LockStep:193) and `SER_GetManifest` (CompareStep:103) are set as error sources but their failures route to `ContextActivationFailure` rather than `ContextSyncError`. These sources are preparatory — the actual sync-error emit sites are in Download/Upload steps. This is correct behavior: ListManifests/GetManifest failures are activation failures, not sync errors.

---

## Event 3: ContextDelete

**PowerBI metric**: Delete Failure Rate = `COUNT(HResultUint != 0) / COUNT(All ContextDelete)`.  
**Emit method**: `EmitContextDeleteEvent()` → wire name `"context_delete"`.

### Emit Sites

| Location | File | deleteType |
|----------|------|------------|
| Download: local file/folder deletion before extraction | `FolderSyncManager.cpp:306` | `DT_Local` (set line 279) |
| Download: download failure cleanup | `FolderSyncManager.cpp:640, 650, 672, 679` | Inherits `DT_Local` |
| Upload: success path emit | `FolderSyncManager.cpp:650-651` | Inherits from UploadStep |
| Upload: failure path emit | `FolderSyncManager.cpp:639-640` | Inherits from UploadStep |
| Upload: no-change release-device pending manifest delete | `FolderSyncManager.cpp:671-672, 679` | Inherits from UploadStep |
| Upload: old version cleanup after FinalizeManifest | `UploadStep.cpp:1262-1275` | `DT_Version` (set line 1264) |
| Upload: delete-all before re-upload | `UploadStep.cpp:616-648` | `DT_All` (set line 618) |
| ResetCloud: per-manifest deletion | `ResetCloudStep.cpp:128-145` | `DT_Version` (set line 130) |

### Field Population Review

| Field | Set By | Coverage |
|-------|--------|----------|
| `deleteType` | `SetContextDeleteDeleteType()` — always set before emit | ✅ |
| `totalSizeBytes` | `SetContextDeleteTotalSize()` — set for DT_Local (FSM:288), DT_All (UploadStep:625) | ⚠️ Not set for DT_Version (UploadStep:1264, ResetCloud:130) — defaults to 0 |
| `contextVersion` | `SetContextDeleteContextVersion()` — set for DT_Version (UploadStep:1265, ResetCloud:131) | ✅ For DT_Version; empty for DT_Local (by design) |
| `hresult` | `SetContextDeleteHResult()` — set on failure paths | ✅ |
| `correlationVector` | Via `SetContextDeleteHttpInfo()` — set for UploadStep (line 647, 1275) and ResetCloud (line 144) | ✅ For HTTP-based deletes; not applicable for local deletes |
| `elapsedMs` | From `startTime` set by `SetContextDeleteStartTime()` | ✅ |

### Emit Suppression Logic

`EmitContextDeleteEvent()` skips if:
1. `startTime == 0` — prevents emit when delete was never actually attempted.
2. `(DT_Local || DT_All) && totalSizeBytes == 0` — suppresses when no data was actually deleted.

For `DT_Version`, `totalSizeBytes` defaults to 0 but the suppression only applies to `DT_Local`/`DT_All`, so version deletes always emit. ✅ Correct.

### Findings

- ✅ All identified deletion paths emit this event.
- ✅ `ResetContextDelete()` is called before each new delete cycle (FSM:277, FSM:131/UploadStep:616/1262, ResetCloud:88/128).
- ⚠️ `totalSizeBytes` is 0 for `DT_Version` deletes. This is technically accurate (the SDK doesn't know the server-side blob size), but means PowerBI `Delete Failure Rate` counts version deletes even though their "size" is 0.

---

## Event 4: ContextSync

**PowerBI metric**: Compression Rate = `SyncSizeBytes / OriginalSizeBytes`.  
**Emit method**: `EmitContextSyncEvent()` → wire name `"context_sync"`.

### Emit Sites

| Location | File | Direction |
|----------|------|-----------|
| Download success | `FolderSyncManager.cpp:360` | `syncDownload=true` (set by DownloadStep:118) |
| Download failure (IO) | `FolderSyncManager.cpp:340` | `syncDownload=true` |
| Upload success | `FolderSyncManager.cpp:651` | `syncDownload=false` (set by UploadStep:450) |
| Upload failure | `FolderSyncManager.cpp:643` | `syncDownload=false` |
| Download error during DT_Local cleanup | `FolderSyncManager.cpp:643, 651` | Inherits direction from last phase |

### Field Accumulation Review

**Download path** (`DownloadStep.cpp`):
- `blockCount`: Incremented per downloaded block (line 306) ✅
- `fileCount`: Added per compressed file's archive entries (line 85) ✅
- `syncSizeBytes`: Added per block's compressed size (line 307) ✅
- `originalSizeBytes`: Added per block's uncompressed size (line 308) ✅
- `totalSizeBytes`: Set after all blocks calculated (line 175) ✅
- `startTime`: Set before HTTP calls begin (line 117) ✅

**Upload path** (`UploadStep.cpp`):
- `blockCount`: Incremented per upload block (line 409) ✅
- `fileCount`: Added per archive's file list (line 406) ✅
- `syncSizeBytes`: Added per block's compressed size (line 411) ✅
- `originalSizeBytes`: Added per block's uncompressed size (line 410) ✅
- `totalSizeBytes`: Set after compare (line 489) ✅
- `startTime`: Set before upload begins (line 449) ✅

### Reset Lifecycle

- `ResetContextSync()` called at start of download phase (FSM:274) and upload init (UploadStep:448). ✅ Prevents stale data from previous cycle.
- Once-only guard `m_contextSyncEventEmitted` prevents double-emit. Reset in `ResetContextSync()`. ✅

### Findings

- ✅ All sync completions (success and failure) emit this event.
- ✅ `syncDownload` is correctly set for each direction.
- ✅ Accumulated size/count fields match the actual blocks transferred.
- ✅ `ResetContextSync()` properly separates download and upload telemetry cycles.

---

## Event 5: ContextActivation (Success/Completion)

**Not a separate PowerBI dashboard** but is the complement to `ContextActivationFailure`. Also carries `ConflictResolution` which feeds PowerBI event #2 filter options.  
**Emit method**: `EmitContextActivationEvent()` → wire name `"context_activation"`.

### Emit Sites

| Location | File | Scenario |
|----------|------|----------|
| Offline mode (lock offline) | `FolderSyncManager.cpp:117-118` | `syncState=NotStarted` |
| Offline mode (compare offline) | `FolderSyncManager.cpp:183-184` | `syncState=NotStarted` |
| Normal completion (post lock+compare) | `FolderSyncManager.cpp:270` | Default `syncState` |
| Failure fallback (via `FireActivationFailedTelemetry`) | `FolderSyncManager.cpp:80` | Paired with failure event |

### ConflictResolution Accuracy

Set in `CompareStep.cpp`:
- `CR_KeepLocal` (line 242): User chose "Take Local" ✅
- `CR_TakeRemote` (line 252): User chose "Take Remote" ✅
- `CR_NoResolutionChosen` (line 262): User cancelled conflict dialog ✅
- `CR_NoResolutionChosen` (line 399): No conflict callback set → cancel ✅
- `CR_NoConflictsExpected`: Default value (0) — used when no conflict occurred ✅
- `CR_SelectVersion`: Not currently used in any code path ⚠️ (enum exists but never set)

### Field Population Review

| Field | Set By | Coverage |
|-------|--------|----------|
| `totalSizeBytes` | `SetContextActivationTotalSize()` — LockStep:336 from finalized manifest | ✅ |
| `compressedSizeBytes` | `SetContextActivationCompressedSize()` — LockStep:337 | ✅ |
| `manifestState` | `SetContextActivationManifestState()` — LockStep:411, 537 (MS_Initialized for pending) | ✅ |
| `syncState` | `SetContextActivationSyncState()` — FSM:78, 117, 183 | ✅ |
| `conflictResolution` | See above | ✅ |
| `canceled` | Via `SetContextActivationCanceled()` in `FireActivationFailedTelemetry` | ✅ |
| `contextVersion` | `SetContextActivationContextVersion()` — LockStep:412, 538 | ✅ |
| `conflictVersion` | `SetContextActivationConflictVersion()` — CompareStep:215 | ✅ Only set when conflict detected |
| `baseVersion` | `SetContextActivationBaseVersion()` — LockStep:332 | ✅ |
| `hresult` | `SetContextActivationHResult()` — various paths | ✅ |

### Findings

- ✅ All activation completions (success, offline, failure) emit this event.
- ⚠️ `CR_SelectVersion` is defined in the enum but never set anywhere. It appears in the PowerBI spec as a filter option. If this resolution type is intended for future rollback UI, the emit path does not exist yet.
- ✅ `baseVersion` field is missing from `ToJson()` serialization — **wait, let me verify this**.

Checking `ContextActivationEvent::ToJson()` (GameSaveTelemetryManager.cpp:64-80): Fields serialized are `totalSizeBytes`, `compressedSizeBytes`, `manifestState`, `syncState`, `conflictResolution`, `canceled`, `contextVersion`, `conflictVersion`. **`baseVersion` and `hresult` are NOT serialized in `ToJson()`**.

- ❌ **`baseVersion` is not serialized** in `ContextActivationEvent::ToJson()` despite being tracked via `SetContextActivationBaseVersion()`. The field exists in the struct but is not written to JSON.
- ❌ **`hresult` is not serialized** in `ContextActivationEvent::ToJson()` despite being tracked via `SetContextActivationHResult()`. The field exists in the struct but is not written to JSON.

---

## Events 6–8: Users / Xuids / Devices / Events

**PowerBI metrics**: Distinct count of `EntityId` (Users), `Xuid` (Xuids), `HardwareId`/`device_id` (Devices), total event count.

These are server-side aggregations computed from the common fields and entity context attached to every emitted event. No separate emit calls exist.

### Common Field Population

`PopulateCommonFields()` (GameSaveTelemetryManager.cpp:58-62) writes:
- `userId`: Set from `localUser.LocalId()` in `CreateEntityTelemetryPipeline` ✅
- `platformType`: Set from `PFPlatformGetPlatformType()` in `CreateEntityTelemetryPipeline` ✅

`entityId` and `Xuid` are NOT directly written to the event payload. They are expected to be resolved by the PlayFab telemetry backend from the entity handle passed to `PFEventPipelineCreateTelemetryPipelineHandleWithEntity`. ✅ This is the standard PlayFab telemetry pattern.

### Pipeline Creation Timing

`CreateEntityTelemetryPipeline()` is called at `LockStep.cpp:191` (the `ListManifests` stage), which is the first stage after successful login. This means:
- ✅ Pipeline is created before any events could be emitted.
- ⚠️ If login fails before reaching `ListManifests`, the pipeline is null. `EmitEvent()` handles this gracefully by logging a warning and returning S_OK (line 159).
- ⚠️ Offline-only sessions (user never connects) produce zero telemetry events. This is by design — the pipeline cannot be created without an entity handle.

### Findings

- ✅ Common fields are correctly populated for all emit paths.
- ✅ Pipeline null-check prevents crashes in offline scenarios.
- ⚠️ Users who only play offline are invisible to all PowerBI metrics. This is an inherent limitation of the entity-based telemetry pipeline.

---

## Cross-Cutting: Reset Lifecycle

| Reset Method | Called At | Purpose |
|-------------|----------|---------|
| `ResetContextActivation()` | `LockStep.cpp:192` (start of ListManifests) | Clear stale activation data from prior retry |
| `ResetContextDelete()` | `FolderSyncManager.cpp:277` (before download deletes), `UploadStep.cpp:616` (before DT_All), `UploadStep.cpp:1262` (before DT_Version), `ResetCloudStep.cpp:88, 128` | Clear between separate delete operations |
| `ResetContextSync()` | `FolderSyncManager.cpp:274` (before download sync), `UploadStep.cpp:448` (before upload sync) | Separate download and upload telemetry cycles |

### Findings

- ✅ `ResetContextActivation` is called once at the start of each activation attempt, after login succeeds and entity is available.
- ✅ `ResetContextSync` separates download-phase telemetry from upload-phase telemetry correctly.
- ✅ `ResetContextDelete` is called before each independent delete operation.
- ✅ No resets occur mid-operation that would lose accumulated data.

---

## Cross-Cutting: ToJson Completeness

| Event | Fields in Struct | Fields in ToJson | Missing from ToJson |
|-------|-----------------|------------------|---------------------|
| `ContextActivationEvent` | totalSizeBytes, compressedSizeBytes, manifestState, syncState, conflictResolution, canceled, contextVersion, conflictVersion, baseVersion, hresult | totalSizeBytes, compressedSizeBytes, manifestState, syncState, conflictResolution, canceled, contextVersion, conflictVersion (conditional) | ❌ **`baseVersion`**, ❌ **`hresult`** |
| `ContextActivationFailureEvent` | canceled, aborted, retryAllowed, startedOnline, syncState, callingLocation, httpStatus, hresult, correlationVector | All 9 fields | ✅ Complete |
| `ContextDeleteEvent` | elapsedMs, totalSizeBytes, hresult, deleteType, contextVersion (conditional), correlationVector (conditional) | All fields (conditional handled) | ✅ Complete |
| `ContextSyncEvent` | blockCount, fileCount, syncSizeBytes, originalSizeBytes, totalSizeBytes, elapsedMs, hresult, httpStatus, retryCount, contextVersion, syncDownload, inGame | All 12 fields | ✅ Complete |
| `ContextSyncErrorEvent` | elapsedMs, correlationVector, hresult, retryCount, contextVersion, blockName, errorSource | All 7 fields | ✅ Complete |

### Findings

- ❌ **`ContextActivationEvent::ToJson()` does not serialize `baseVersion` or `hresult`**. The struct tracks both fields and setters are called, but they are silently dropped during JSON serialization. This means PowerBI cannot use these fields for the activation success event.

---

## Cross-Cutting: Offline / Mock Modes

- **`useMocks == true`**: All `EmitEvent()` calls log and return S_OK. `CreateEntityTelemetryPipeline()` also returns S_OK without creating a real pipeline. This is controlled by a static bool `GameSaveTelemetryManager::useMocks` — verified to only be set in test harness code. ✅
- **Pipeline null (offline)**: `EmitEvent()` logs warning and returns S_OK. No events reach PowerBI. ✅ By design.

---

## Gap Analysis: Missing Emit Sites

### GAP-1: No activation telemetry on conflict-upload failure during AddUser

**Location**: `FolderSyncManager.cpp:233-248` (conflict upload path in `DoWorkFolderDownload`)

When a conflict is detected during `AddUserWithUiAsync`, the SDK performs a single upload of the local divergent branch before completing activation. If this conflict upload fails (user cancellation, `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD`, or compression error), the function returns the error directly:

```
return hrUp; // propagate failure or pending
```

Neither `EmitContextActivationEvent()` nor `EmitContextActivationFailureEvent()` is called. The activation event at line 270 is never reached. PowerBI loses visibility into this failure scenario.

**Affected error codes**: `E_PF_GAMESAVE_USER_CANCELLED`, `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD`, local IO/compression failures.

**Suggested fix**: Add `FireActivationFailedTelemetry(hrUp, false)` before the `return hrUp` at line 248.

---

### GAP-2: No sync telemetry when upload returns E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD

**Location**: `FolderSyncManager.cpp:604-616` (in `DoWorkFolderUpload`)

When `UploadStep::Upload()` returns `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD` (server has a newer manifest), the FSM sets forced-offline and triggers an active device change callback, but returns without emitting `ContextSyncEvent` or `ContextSyncErrorEvent`:

```cpp
if (hr == E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD)
{
    SetForcedDisconnectFromCloud(true);
    // ... trigger callback ...
    return hr;   // <-- no telemetry emitted
}
```

The non-disconnected failure path at lines 639-643 correctly emits all three events (delete, sync error, sync). This path is missing the same treatment.

**Suggested fix**: Before `return hr` at line 616, emit:
```cpp
m_telemetryManager->SetContextSyncHResult(hr);
m_telemetryManager->EmitContextSyncErrorEvent();
m_telemetryManager->EmitContextSyncEvent();
```

---

### GAP-3: CompareStep failure during upload has no telemetry

**Location**: `FolderSyncManager.cpp:569-585` (CompareStep during `DoWorkFolderUpload`)

When `CompareStep::CompareWithCloud()` fails during the upload path (e.g., `GetManifestDownloadDetails` HTTP error or `InitWithExtendedManifest` parse error), the FSM returns the error with no sync telemetry:

```cpp
if (FAILED(hr))
{
    return hr;  // <-- no sync or delete telemetry
}
```

The `ContextActivationFailureEvent` that CompareStep internally emits (lines 75-76, 141-142) is suppressed by the guard because activation already succeeded during AddUser.

**Impact**: If a metadata fetch fails before the upload data transfer begins, PowerBI sees no record of the upload attempt at all.

**Suggested fix**: Emit sync-level telemetry:
```cpp
if (FAILED(hr))
{
    m_telemetryManager->SetContextSyncHResult(hr);
    m_telemetryManager->EmitContextSyncErrorEvent();
    m_telemetryManager->EmitContextSyncEvent();
    return hr;
}
```

---

### GAP-4: ResetCloudStep DeleteManifest failure doesn't set `hresult`

**Location**: `ResetCloudStep.cpp:140-150` (DeleteManifest Finally callback)

The delete Finally callback always emits `ContextDeleteEvent` regardless of success or failure, but never calls `SetContextDeleteHResult()`:

```cpp
.Finally([this, &task, &folderSyncMutex](Result<void> result)
{
    m_telemetryManager->SetContextDeleteHttpInfo(result.httpResult);
    m_telemetryManager->EmitContextDeleteEvent();
    // result.hr is not checked — hresult defaults to 0 even on failure
});
```

If `DeleteManifest` fails, the event is emitted with `hresult=0` (implying success), but the HTTP info does contain the actual error status code. PowerBI's `Delete Failure Rate` metric (`COUNT(HResultUint != 0)`) would miss these failures.

**Suggested fix**: Add before `EmitContextDeleteEvent()`:
```cpp
if (FAILED(result.hr))
{
    m_telemetryManager->SetContextDeleteHResult(result.hr);
}
```

---

### GAP-5: ContextSyncEvent emitted without startTime when no download needed

**Location**: `FolderSyncManager.cpp:325→360` (download phase of `DoWorkFolderDownload`)

When `GetCompressedFilesToDownload().size() == 0`, the `DownloadStep` is never invoked, so `SetContextSyncStartTime()` (called in `DownloadStep.cpp:117`) is never executed. However, `EmitContextSyncEvent()` is still called at line 360. The `elapsedMs` field is computed from a startTime of 0 (default after `ResetContextSync`), producing a misleading value.

**Impact**: PowerBI sees a sync event with `blockCount=0`, `fileCount=0`, `syncSizeBytes=0` (correct) but an incorrect `elapsedMs` (time since epoch).

**Suggested fix**: Either:
- (a) Skip `EmitContextSyncEvent()` when no download was needed, or  
- (b) Call `m_telemetryManager->SetContextSyncStartTime()` before line 306 in FSM (right after `ResetContextSync`), so `elapsedMs` reflects the local-operations time.

---

## Summary of Findings

### ❌ Issues

1. ~~**`ContextActivationEvent::ToJson()` is missing `baseVersion` and `hresult`** — these fields are tracked in the struct and populated by setters, but not serialized to the JSON payload sent to PlayFab telemetry. PowerBI cannot use these fields for the success activation event.~~ ✅ **Fixed**
2. ~~**GAP-1: Conflict-upload failure during AddUser emits no activation telemetry** — `FolderSyncManager.cpp:248` returns error without `FireActivationFailedTelemetry`.~~ ✅ **Fixed**
3. ~~**GAP-2: Disconnected-from-cloud during upload emits no sync telemetry** — `FolderSyncManager.cpp:616` returns error without `EmitContextSyncEvent`/`EmitContextSyncErrorEvent`.~~ ✅ **Fixed**

### ⚠️ Potential Concerns

1. ~~**GAP-3: CompareStep failure during upload has no sync telemetry** — metadata fetch failure during upload path goes unreported.~~ ✅ **Fixed**
2. ~~**GAP-4: ResetCloudStep delete failure reports hresult=0** — PowerBI `Delete Failure Rate` undercounts.~~ ✅ **Fixed**
3. ~~**GAP-5: elapsedMs is wrong when no download is needed** — sync event emitted with startTime=0.~~ ✅ **Fixed**
4. ~~**Multiple failure events per activation attempt** — `CreatePendingManifest` retries for `E_PF_GAME_SAVE_MANIFEST_VERSION_ALREADY_EXISTS` emit separate `ContextActivationFailureEvent` per HTTP call without `ResetContextActivation`. Inflates PowerBI failure counts.~~ ✅ **Fixed**
5. **`CR_SelectVersion` conflict resolution is never used** — defined in the enum and listed in the PowerBI spec but no code path sets it. May be intended for a future feature.
6. **`totalSizeBytes` is 0 for `DT_Version` deletes** — technically accurate but may cause confusion in PowerBI delete analytics.
7. **Offline-only users are invisible** to all PowerBI metrics — inherent limitation of entity-based telemetry pipeline.

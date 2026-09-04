# Cancel Bug Root Cause Analysis

**Bug:** `cancel-bug.md` -- Uploads permanently fail with `E_PF_GAME_SAVE_MANIFEST_UPDATES_NOT_ALLOWED` after cancelling a rate-limited upload via `PFGameSaveFilesUiSyncFailedUserAction::Cancel`.

**Status:** Root cause identified. Fix implemented.

---

## Summary

When a user cancels the SyncFailed UI during the **TakeLock** step of the upload state machine (the final step, *after* data has already been finalized on the server), the SDK fails to update its internal manifest pointers. The `m_latestPendingManifest` in `FolderSyncManager` continues to reference a manifest version that has already been finalized. Every subsequent upload attempt uses this stale version for `InitiateUpload`, and the service correctly rejects it with `MANIFEST_UPDATES_NOT_ALLOWED` (0x89235786). The SDK is permanently stuck in this state until the user restarts the game.

---

## Upload State Machine Background

A successful upload progresses through these stages in `UploadStep`:

```
UploadStart → CompressFiles → InitiateUpload → UploadFile → FinalizeManifest
  → ListManifestsAfterUpload → PromoteIfNeeded → TakeLock → UploadDone
```

- **FinalizeManifest** -- marks the uploaded manifest version as finalized on the server. After this, the data is committed and the manifest version can no longer be modified.
- **TakeLock** -- calls `InitializeManifest` to create a *new* pending manifest version for the next upload. This is the last step before `UploadDone`.

When `UploadDone` is reached successfully, `FolderSyncManager::DoWorkFolderUpload` (line 578-579) updates its two key pointers:

```cpp
m_latestFinalizedManifest = MakeShared<ManifestInternal>(m_uploadStep.GetPostUploadLatestFinalizedPFManifest());
m_latestPendingManifest   = MakeShared<ManifestInternal>(m_uploadStep.GetPostUploadPendingPFManifest());
```

These pointers are consumed on the *next* upload: `InitiateUpload` sends `latestPendingManifest->Version()` as the manifest version to write to.

---

## Bug Sequence (from trace log 01)

| Time | Event | Detail |
|---|---|---|
| 12:16:20 | `InitiateUploadFinally HR:0x00000000` | 3rd upload -- InitiateUpload succeeds |
| 12:16:20 | `FinalizeManifestFinally HR:0x00000000` | Data finalized as **v343** on server |
| 12:17:11 | `TakeLock KeepDeviceActive. FinalizedManifest v343` | TakeLock begins -- tries to create v344 pending |
| 12:17:12 | `TakeLock InitializeManifestFinally HR:0x892354dd` | **HTTP 429** -- rate limited |
| 12:17:17 | `WaitForFailedUI_TakeLock: user chose RETRY` | 1st retry |
| 12:17:17 | `TakeLock InitializeManifestFinally HR:0x892354dd` | Still 429 |
| 12:17:22 | `WaitForFailedUI_TakeLock: user chose RETRY` | 2nd retry |
| 12:17:22 | `TakeLock InitializeManifestFinally HR:0x892354dd` | Still 429 |
| **12:17:57** | **`HandleFailedUI: user CANCELLED sync operation`** | **User cancels** |
| 12:18:00 | `InitiateUploadFinally HR:0x89235786` | **Next upload fails -- MANIFEST_UPDATES_NOT_ALLOWED** |
| 12:18:07 | `InitiateUploadFinally HR:0x89235786` | Retry -- same error |
| 12:19:08 | `InitiateUploadFinally HR:0x89235786` | Later attempt -- same error |
| 12:21:52 | `InitiateUploadFinally HR:0x89235786` | 5 min later -- still broken |
| 12:24:54 | `InitiateUploadFinally HR:0x89235786` | 8 min later -- still broken |

The error is **permanent** for the session. Every subsequent upload hits 0x89235786 at `InitiateUpload`.

---

## Root Cause

### The Stale Manifest Problem

The bug is in `FolderSyncManager::DoWorkFolderUpload` (FolderSyncManager.cpp).

When the cancel occurs during `WaitForFailedUI_TakeLock`, the control flow is:

1. `UICallbackManager::HandleFailedUI` sees `UISyncFailedCancel` and returns `E_PF_GAMESAVE_USER_CANCELLED` immediately (line 165 of UICallbackManager.cpp). **Neither the retry nor the offline lambda is called.**

2. `UploadStep::Upload()` returns `E_PF_GAMESAVE_USER_CANCELLED` to `DoWorkFolderUpload`.

3. `DoWorkFolderUpload` enters the `FAILED(hr)` branch (line 562) and returns the error **without updating manifest pointers** (lines 578-579 are skipped):

```cpp
// Line 562-570 -- error exit, no manifest update
else if (FAILED(hr))
{
    m_telemetryManager->SetContextDeleteHResult(hr);
    m_telemetryManager->EmitContextDeleteEvent();
    m_telemetryManager->SetContextSyncHResult(hr);
    m_telemetryManager->EmitContextSyncErrorEvent();
    m_telemetryManager->EmitContextSyncEvent();
    return hr;  // ← returns without updating m_latestPendingManifest
}

// Line 578-579 -- NEVER REACHED on error
m_latestFinalizedManifest = MakeShared<ManifestInternal>(m_uploadStep.GetPostUploadLatestFinalizedPFManifest());
m_latestPendingManifest   = MakeShared<ManifestInternal>(m_uploadStep.GetPostUploadPendingPFManifest());
```

### Why This Causes Permanent Failure

Before the failed upload:
- `m_latestPendingManifest` → **v343** (pending on server)
- `m_latestFinalizedManifest` → **v340** (latest finalized on server)

During the upload that gets cancelled at TakeLock:
- `FinalizeManifest` succeeds → **v343 is now finalized on the server**
- `ListManifestsAfterUpload` succeeds → `m_postUploadLatestFinalizedPFManifest` = v343
- `TakeLock` tries to create v344 pending → fails (429) → user cancels

After the cancel:
- `m_latestPendingManifest` → **v343** (STALE -- this version is now finalized, not pending)
- `m_latestFinalizedManifest` → **v340** (STALE -- v343 is actually the latest finalized)
- `m_postUploadPendingPFManifest` → **empty** (TakeLock never succeeded, so it was never set)

Next upload starts (`UploadAsyncProvider` → `InitForUpload` → resets `UploadStep`, but `FolderSyncManager`'s manifest pointers are untouched):
- `DoWorkFolderUpload` line 499: null check on `m_latestPendingManifest` → **passes** (it's non-null, just stale)
- `InitiateUpload` line 534: `initRequest.SetVersion(latestPendingManifest->Version())` → sends **v343**
- Server rejects: v343 is finalized, not a pending manifest → **`MANIFEST_UPDATES_NOT_ALLOWED`**

This repeats forever because `m_latestPendingManifest` is never corrected.

---

## Affected Code Locations

| File | Lines | Role |
|---|---|---|
| `Source/SyncManager/FolderSyncManager.cpp` | 562-570 | `FAILED(hr)` exit skips manifest update |
| `Source/SyncManager/FolderSyncManager.cpp` | 578-579 | Manifest update (only on success path) |
| `Source/SyncManager/FolderSyncManager.h` | 87-88 | `m_latestPendingManifest`, `m_latestFinalizedManifest` declarations |
| `Source/SyncManager/UploadStep.cpp` | 993-1000 | TakeLock failure → `WaitForFailedUI_TakeLock` |
| `Source/SyncManager/UploadStep.cpp` | 1100-1113 | `WaitForFailedUI_TakeLock` handler (cancel returns error before lambdas fire) |
| `Source/Common/UICallbackManager.cpp` | 162-165 | `HandleFailedUI` cancel path -- returns `E_PF_GAMESAVE_USER_CANCELLED` immediately |

---

## Error Codes

| Code | Name | Meaning |
|---|---|---|
| `0x892354DD` | `E_PF_API_CLIENT_REQUEST_RATE_LIMIT_EXCEEDED` | HTTP 429 -- triggers SyncFailed UI at TakeLock |
| `0x89235786` | `E_PF_GAME_SAVE_MANIFEST_UPDATES_NOT_ALLOWED` | InitiateUpload on a finalized manifest -- the persistent bug symptom |
| `0x800704C7` | `E_PF_GAMESAVE_USER_CANCELLED` | Cancel chosen in SyncFailed UI |
| `0x89235780` | (transient server error) | Seen during earlier TakeLock attempts; resolved on retry |

---

## Fix Direction

The core issue is that the `FAILED(hr)` branch in `DoWorkFolderUpload` unconditionally skips manifest updates, even when the upload data was already finalized on the server. The fix must ensure that after FinalizeManifest succeeds, manifest pointers are updated regardless of whether TakeLock (or any later post-finalize step) succeeds.

### Recommended Approach

**In `DoWorkFolderUpload` (FolderSyncManager.cpp, line 562-570):**

When `Upload()` returns a failure but `m_uploadStep.HasStartedFinalizeManifest()` is true (data was committed to the server), update the manifest pointers before returning the error:

```cpp
else if (FAILED(hr))
{
    // If data was already finalized but a post-finalize step (TakeLock)
    // failed or was cancelled, update manifests to prevent stale state.
    // The upload data IS on the server; only the lock for the next upload
    // was not acquired.
    if (m_uploadStep.HasStartedFinalizeManifest())
    {
        m_latestFinalizedManifest = MakeShared<ManifestInternal>(
            m_uploadStep.GetPostUploadLatestFinalizedPFManifest());
        m_latestPendingManifest = nullptr; // No valid pending -- force re-acquire
    }

    m_telemetryManager->SetContextDeleteHResult(hr);
    m_telemetryManager->EmitContextDeleteEvent();
    m_telemetryManager->SetContextSyncHResult(hr);
    m_telemetryManager->EmitContextSyncErrorEvent();
    m_telemetryManager->EmitContextSyncEvent();
    return hr;
}
```

**In `DoWorkFolderUpload` (FolderSyncManager.cpp, line 499):**

When `m_latestPendingManifest` is null (because a previous upload's TakeLock failed), acquire a new pending manifest instead of asserting:

```cpp
if (m_latestPendingManifest == nullptr)
{
    // Re-acquire lock: previous upload finalized but TakeLock failed.
    // Need to call ListManifests + InitializeManifest to get a new pending manifest.
    // (Implementation details TBD -- may delegate to LockStep or add a re-lock sub-step)
}
```

### Considerations

- `HasStartedFinalizeManifest()` already exists on `UploadStep` and tracks whether `FinalizeManifest` was entered. It is reset on `UploadStep::Reset()`. This makes it a reliable indicator of whether data was committed.
- Setting `m_latestPendingManifest = nullptr` requires handling the null case at line 499, which currently asserts. The re-acquire path needs careful design to avoid breaking the existing state machine.
- An alternative approach is to make TakeLock failure non-fatal (treat the upload as successful since data was committed) and defer lock acquisition to the start of the next upload. This would avoid the null-pending-manifest complication.

---

## Reproduction

- **Test scenario:** `Test/GameTestScenarios/scenario-38-upload-cancel-manifest-blocked.yml`
- **Customer logs:** `Out/log-manifest-bug/SteamInproc_UploadFailure_ManifestUpdateNotAllowed_TraceLog_01.txt` (primary), `_02.txt` (secondary)

---

# Regression Bug: SetSaveDescription Fails After Cancel

**Bug:** After the fix for the stale manifest problem was implemented, `PFGameSaveFilesSetSaveDescriptionAsync` fails with `E_PF_GAMESAVE_USER_NOT_ADDED` (0x89237003) if called after cancelling a rate-limited upload.

**Status:** Root cause identified. Fix pending.

---

## Summary

The fix for the original stale manifest bug correctly sets `m_latestPendingManifest = nullptr` when an upload fails post-finalize. This prevents the stale manifest from being reused and triggers re-lock logic on the next upload. However, `DoWorkSetSaveDescription` returns `E_PF_GAMESAVE_USER_NOT_ADDED` whenever `m_latestPendingManifest == nullptr`, without distinguishing between:

1. User never completed initial sync (truly not added)
2. User was added, but pending manifest was cleared due to post-finalize failure recovery

This causes `SetSaveDescription` to fail incorrectly in case 2.

---

## Bug Sequence

| Step | Action | Result |
|------|--------|--------|
| 1 | User completes `AddUserWithUiAsync` | `m_latestPendingManifest` and `m_latestFinalizedManifest` are valid |
| 2 | User performs successful upload | Both manifests updated |
| 3 | User modifies save data | -- |
| 4 | User calls `UploadWithUiAsync` | Upload proceeds through `FinalizeManifest` |
| 5 | `TakeLock` (InitializeManifest) hits rate limit (429) | SyncFailed UI shown |
| 6 | User cancels via SyncFailed UI | Upload returns `E_PF_GAMESAVE_USER_CANCELLED` |
| 7 | Fix clears `m_latestPendingManifest = nullptr` | Correct -- prevents stale manifest reuse |
| 8 | User calls `SetSaveDescriptionAsync` | **BUG:** Returns `E_PF_GAMESAVE_USER_NOT_ADDED` |

---

## Root Cause

In `FolderSyncManager::DoWorkSetSaveDescription` (FolderSyncManager.cpp, lines 737-742):

```cpp
if (m_latestPendingManifest == nullptr)
{
    // No manifest available yet - user may not have completed initial sync
    TRACE_ERROR("DoWorkSetSaveDescription: no pending manifest available");
    return E_PF_GAMESAVE_USER_NOT_ADDED;
}
```

This check does not distinguish between:
- **Case 1:** `m_latestPendingManifest == nullptr` AND `m_latestFinalizedManifest == nullptr` → User truly not added
- **Case 2:** `m_latestPendingManifest == nullptr` AND `m_latestFinalizedManifest != nullptr` → User was added, but pending manifest cleared due to post-finalize recovery

In case 2, the error `E_PF_GAMESAVE_USER_NOT_ADDED` is incorrect and misleading.

---

## Affected Code Locations

| File | Lines | Role |
|------|-------|------|
| `Source/SyncManager/FolderSyncManager.cpp` | 737-742 | `DoWorkSetSaveDescription` null check on `m_latestPendingManifest` |
| `Source/SyncManager/FolderSyncManager.cpp` | 611-627 | Fix that sets `m_latestPendingManifest = nullptr` on post-finalize failure |

---

## Fix Direction

Modify `DoWorkSetSaveDescription` to handle case 2 by caching the description locally with `dirty=true`, consistent with existing offline mode and network failure handling:

```cpp
if (m_latestPendingManifest == nullptr)
{
    // No pending manifest available. This can happen in two cases:
    // 1. User has not completed initial sync (AddUser) - m_latestFinalizedManifest is also nullptr
    // 2. Previous upload's TakeLock failed/cancelled after FinalizeManifest - m_latestFinalizedManifest exists
    // In case 2, cache the description locally; it will be synced when the next upload re-acquires the lock.
    if (m_latestFinalizedManifest != nullptr)
    {
        TRACE_INFORMATION("DoWorkSetSaveDescription: pending manifest unavailable (post-finalize failure recovery), storing description locally (dirty=true)");
        SetLastShortSaveDescription(shortSaveDescription, true);
        return S_OK;
    }
    TRACE_ERROR("DoWorkSetSaveDescription: no pending manifest available - user not added");
    return E_PF_GAMESAVE_USER_NOT_ADDED;
}
```

This approach:
- Returns success and caches the description locally when user was added but pending manifest is unavailable
- Preserves correct `E_PF_GAMESAVE_USER_NOT_ADDED` error when user truly hasn't been added
- Is consistent with existing offline mode and network failure handling patterns
- Description will be synced to cloud on next successful upload when pending manifest is re-acquired

---

## Reproduction

- **Test scenario:** `Test/GameTestScenarios/scenario-38-upload-cancel-manifest-blocked.yml` (Block: `set-description-after-cancel`)

# Bug: BaseVersionNotAvailable After Cancelling Rate-Limited ListManifests

**Bug:** Uploads permanently fail with `E_PF_GAME_SAVE_BASE_VERSION_NOT_AVAILABLE` (0x89235798) after cancelling an upload whose `ListManifestsAfterUpload` was rate-limited (HTTP 429).

**Status:** Root cause identified. Fix implemented.

**Related:** `bug-cancel-rate-limited-upload.md` (different stage — TakeLock vs ListManifests)

---

## Summary

When `FinalizeManifest` succeeds but `ListManifestsAfterUpload` fails (e.g., rate-limited with HTTP 429), the `PromoteIfNeeded` stage is never reached and `m_postUploadLatestFinalizedPFManifest` is never populated. When the user cancels via the SyncFailed UI, the post-finalize failure handler in `DoWorkFolderUpload` checks `postFinal.GetVersion().empty()` and skips updating `m_latestFinalizedManifest`. This leaves the finalized manifest pointer at the **old** version (N), even though the server just finalized version N+1.

On the next upload, `m_latestPendingManifest` is null (correctly cleared), so the RelockStep fires. It calls `InitializeManifest` with `baseFinalizedVersion = N` (stale). The service rejects this because the actual latest finalized version is N+1, returning `E_PF_GAME_SAVE_BASE_VERSION_NOT_AVAILABLE`. This is permanent — every subsequent upload attempt uses the same stale base.

---

## Distinction from bug-cancel-rate-limited-upload.md

| Aspect | Previous bug (scenario 38) | This bug (scenario 40) |
|--------|---------------------------|------------------------|
| Cancel happens at | TakeLock (post-ListManifests) | ListManifestsAfterUpload (pre-PromoteIfNeeded) |
| `postFinal` populated? | YES (from ListManifests) | NO (never reached PromoteIfNeeded) |
| Existing fix handles? | YES (updates `m_latestFinalizedManifest` from postFinal) | NO (falls through empty check) |
| Symptom | `MANIFEST_UPDATES_NOT_ALLOWED` | `BASE_VERSION_NOT_AVAILABLE` |

---

## Upload State Machine Background

```
UploadStart → CompressFiles → InitiateUpload → UploadFile → FinalizeManifest
  → ListManifestsAfterUpload → PromoteIfNeeded → TakeLock → UploadDone
```

- **FinalizeManifest** — commits the upload; manifest version transitions from Uploading to Finalized.
- **ListManifestsAfterUpload** — fetches current manifest list from service.
- **PromoteIfNeeded** — populates `m_postUploadLatestFinalizedPFManifest` from ListManifests result.
- **TakeLock** — creates next pending manifest for future uploads.

When cancel occurs between FinalizeManifest and PromoteIfNeeded, the data IS committed on the server but the SDK has no record of the new finalized version.

---

## Bug Sequence (from trace log)

| Time | Event | Detail |
|------|-------|--------|
| 11:52:18 | `FinalizeManifestFinally HR:0x00000000` | Upload v(N+1) finalized on server |
| 11:52:19 | `ListManifestsAfterUploadFinally HR:0x892354dd` | Rate-limited (429) |
| 11:52:24 | `ListManifestsAfterUploadFinally HR:0x892354dd` | Retry — still 429 |
| 11:52:29 | `ListManifestsAfterUploadFinally HR:0x892354dd` | Retry — still 429 |
| 11:53:05 | `UploadAsyncProvider::DoWork HR:0x800704c7` | User cancels |
| 11:56:28 | `RelockStep.InitializeManifestFinally HR:0x89235798` | **Next upload: BASE_VERSION_NOT_AVAILABLE** |
| 11:56:35 | `RelockStep.InitializeManifestFinally HR:0x89235798` | Retry — same error |
| 11:56:41 | `UploadAsyncProvider::DoWork HR:0x800704c7` | User cancels again |
| 11:56:43 | `RelockStep.InitializeManifestFinally HR:0x89235798` | Next upload — still broken |

---

## Root Cause

In `FolderSyncManager::DoWorkFolderUpload` (FolderSyncManager.cpp, lines 620–636), the post-finalize failure handler:

```cpp
if (m_uploadStep.HasStartedFinalizeManifest())
{
    const ManifestWrap& postFinal = m_uploadStep.GetPostUploadLatestFinalizedPFManifest();
    if (!postFinal.GetVersion().empty())
    {
        m_latestFinalizedManifest = MakeShared<ManifestInternal>(postFinal);
    }
    // ← BUG: No else branch — when postFinal is empty (ListManifests never
    //   completed), m_latestFinalizedManifest stays at the OLD version.
    m_latestPendingManifest = nullptr;
    ...
}
```

When ListManifests fails, `postFinal.GetVersion()` is empty because `m_postUploadLatestFinalizedPFManifest` is only populated in the `PromoteIfNeeded` stage (via `LockStep::TryGetLatestFinalizedManifest`), which requires ListManifests to succeed first.

The pending manifest (`m_latestPendingManifest`) holds the version that was just finalized. It is still valid at this point but is about to be set to nullptr. Using it as the fallback finalized version is correct.

---

## Fix

Populate `m_postUploadLatestFinalizedPFManifest` immediately after `FinalizeManifest` succeeds, before transitioning to `ListManifestsAfterUpload`. This fixes the problem at its source — when the truth is known — rather than patching the consumer.

In `UploadStep.cpp`, inside the `FinalizeManifest` success handler (after `WriteLocalManifest`, before setting `m_stage = ListManifestsAfterUpload`):

```cpp
// Save the pending manifest as the preliminary post-finalize manifest.
// If ListManifestsAfterUpload fails and the user cancels, this ensures
// FolderSyncManager has the correct finalized version for RelockStep.
// PromoteIfNeeded will overwrite this with the authoritative server data.
m_postUploadLatestFinalizedPFManifest = latestPendingManifest->GetManifest();
```

This ensures `postFinal.GetVersion()` is never empty in the post-finalize failure handler, so the existing `m_latestFinalizedManifest` update path in `FolderSyncManager::DoWorkFolderUpload` works correctly without any changes needed there.

---

## Error Codes

| Code | Name | Meaning |
|------|------|---------|
| `0x892354DD` | `E_PF_API_CLIENT_REQUEST_RATE_LIMIT_EXCEEDED` | HTTP 429 — triggers SyncFailed at ListManifests |
| `0x89235798` | `E_PF_GAME_SAVE_BASE_VERSION_NOT_AVAILABLE` | RelockStep uses stale base — the persistent bug symptom |
| `0x800704C7` | `E_PF_GAMESAVE_USER_CANCELLED` | Cancel chosen in SyncFailed UI |

---

## Affected Code Locations

| File | Lines | Role |
|------|-------|------|
| `Source/SyncManager/UploadStep.cpp` | 823–831 | **FIX HERE** — FinalizeManifest success handler, saves preliminary postFinal |
| `Source/SyncManager/FolderSyncManager.cpp` | 620–636 | Post-finalize failure handler — consumes `GetPostUploadLatestFinalizedPFManifest()` |
| `Source/SyncManager/UploadStep.cpp` | 894–898 | PromoteIfNeeded — overwrites postFinal with authoritative server data |
| `Source/SyncManager/UploadStep.cpp` | 870–881 | ListManifestsAfterUpload failure → WaitForFailedUI |
| `Source/SyncManager/RelockStep.cpp` | 48–57 | CreatePendingManifest — uses baseFinalizedVersion from m_latestFinalizedManifest |

---

## Reproduction

- **Test scenario:** `Test/GameTestScenarios/scenario-39-upload-hang-relock-loop.yml` (block 2: cancel-at-listmanifests)
- **Customer logs:** `Out/bug2-logs/2026-02-17_UplaodFailure_BaseVersionNotAvailable_TraceLog.txt`
- **Customer report:** `Out/bug2-logs/bug.md`

# Bug: Upload Hangs in Infinite Re-Lock Loop After Cancelling Rate-Limited TakeLock

## Status
**Fixed** — SDK fix applied (RelockStep class), test scenario-39 passes

## Summary
After the fix for the cancel-rate-limited-upload bug (scenario-38), a new regression
causes uploads to hang permanently. When a user cancels during `WaitForFailedUI_TakeLock`
(rate-limited `InitializeManifest`), the fix correctly sets `m_latestPendingManifest = nullptr`.
But on the **next** upload, the re-acquire path in `DoWorkFolderUpload` enters an infinite
`InitializeManifest` retry loop with no `WaitForFailedUI` escape — the upload hangs forever.

## Reproduction
- **Test scenario**: `scenario-39-upload-hang-relock-loop.yml`
- **Original traces**: `Out\bug1-logs\2026-02-16_UploadHang_PostRateLimitCancel_TraceLog_01.txt`

## Key Log Lines

From original bug trace (TraceLog_01):

```
# User cancels at WaitForFailedUI_ListManifestsAfterUpload during rate-limited upload
15-26-26.523 | [GAME SAVE] HandleFailedUI: user CANCELLED sync operation

# Next upload triggers re-acquire path — loops 945 times with no UI callback
15-26-30.051 | [GAME SAVE] PFGameSaveFilesUploadWithUiAsync
15-26-31.490 | [GAME SAVE] DoWorkFolderUpload: re-acquire lock HR:0x89235798  (BASE_VERSION_NOT_AVAILABLE)
15-26-31.706 | [GAME SAVE] DoWorkFolderUpload: re-acquire lock HR:0x89235798
...
15-26-36.410 | [GAME SAVE] DoWorkFolderUpload: re-acquire lock HR:0x892354dd  (429 rate limited)
... (945 total iterations over ~3 minutes, no WaitForFailedUI ever fired)
```

## Root Cause

The re-acquire lock path in `FolderSyncManager::DoWorkFolderUpload` was added as part of
the scenario-38 fix. When a previous upload's TakeLock fails and the user cancels,
`m_latestPendingManifest` is set to `nullptr`. The next upload detects this and tries to
re-acquire a pending manifest by calling `InitializeManifest` directly.

The inline code only handled two cases:
1. **Success** → set `m_latestPendingManifest`
2. **MANIFEST_VERSION_ALREADY_EXISTS** → increment offset and retry

All other errors (429 rate-limiting, BASE_VERSION_NOT_AVAILABLE, network errors, etc.)
fell through to the default path: reset flags and `task.ScheduleNow()`. This caused
immediate re-entry into InitializeManifest, hitting the same error, looping 945 times
in 3 minutes with no user-facing escape.

## Fix Applied

### 1. RelockStep class (`RelockStep.h`, `RelockStep.cpp`)

Refactored the inline re-acquire code into a proper step class following the exact
LockStep pattern. RelockStep has a state machine with stages:
- **CreatePendingManifest** — calls InitializeManifest via service
- **WaitForFailedUI_CreatePendingManifest** — waits for user Cancel/Retry/UseOffline
- **RelockStepFailure** — returns stored failure HRESULT
- **RelockDone** — pending manifest re-acquired successfully

The Finally callback handles three branches:
- VERSION_ALREADY_EXISTS → increment offset, ScheduleNow (retry)
- Other errors → ShowSyncFailedUI, wait for user response
- Success → store pending manifest, RelockDone

### 2. FolderSyncManager integration

Replaced ~110 lines of inline re-acquire code with `m_relockStep` member.
DoWorkFolderUpload delegates to RelockStep using the standard step pattern:
```cpp
if (!m_relockStep.IsRelockDone()) {
    HRESULT hr = m_relockStep.Relock(runContext, task, ...);
    if (FAILED(hr)) { /* handle error, return hr */ }
    return E_PENDING;
}
```

## Repro Steps

1. Call `PFGameSaveFilesUploadWithUiAsync` to upload save data.
2. During the upload, the service rate-limits the TakeLock (`InitializeManifest`) call, triggering the `syncFailedCallback`.
3. In the callback, respond with `PFGameSaveFilesSetUiSyncFailedResponse` using `Cancel`.
4. Call `PFGameSaveFilesUploadWithUiAsync` again to start a new upload.
5. **Expected**: The upload either succeeds (if the service has recovered) or fires `syncFailedCallback` again so the user can retry or cancel.
6. **Actual**: The upload hangs indefinitely. The `syncFailedCallback` is never fired. The API never returns.

## Release Notes

Fixed a bug where `PFGameSaveFilesUploadWithUiAsync` could hang indefinitely if a previous upload was cancelled during a service error. The sync-failed UI callback is now correctly surfaced on retry, allowing the user to cancel or retry the operation.

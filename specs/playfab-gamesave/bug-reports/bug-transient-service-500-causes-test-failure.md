# Bug: Transient E_PF_INTERNAL_SERVER_ERROR (500) Causes Test Failures

## Classification
- **Type:** Test flakiness / transient service error
- **Severity:** Low (intermittent, not a product bug)
- **Affected tests:** 11 (Contention Full Re-Init), 14 (Conflict Local Wins), and potentially any test that syncs with the PlayFab service
- **First observed:** pass111 (2026-05-05)

## Symptoms

Tests that normally pass intermittently fail with `E_PF_INTERNAL_SERVER_ERROR` (0x89235485) during `AddUserWithUiAsync` or `UploadWithUiAsync`. The `SyncFailed` UI callback fires, and the default auto-response (`Cancel`) immediately aborts the operation.

### Test 11 failure
```
PFGameSaveFilesUiProgressCallback (state=Uploading, current=0, total=12288)
PFGameSaveFilesUiSyncFailedCallback (state=Uploading, hr=0x89235485)
Auto responder: PFGameSaveFilesSetUiSyncFailedResponse action=Cancel
PFGameSaveFilesUploadWithUiAsync (waitHr=0x800704C7)  ← E_CANCELLED
```

### Test 14 failure
```
PFGameSaveFilesUiSyncFailedCallback (state=PreparingForDownload, hr=0x89235485)
Auto responder: PFGameSaveFilesSetUiSyncFailedResponse action=Cancel
PFGameSaveFilesAddUserWithUiAsync (waitHr=0x800704C7)  ← E_CANCELLED
```

Both failures occurred within 60 seconds of each other (16:30:02–16:30:41), suggesting the PlayFab service experienced a brief availability issue during that window.

## Root Cause

The PlayFab service returned HTTP 500 (Internal Server Error) to the SDK's ListManifests or Upload call. This is a transient service-side issue — not a client bug.

The test infrastructure's `SyncFailed` auto-response defaults to `Cancel`, which converts any transient error into an immediate test failure.

## Mitigation Options

### Option 1: Set SyncFailed auto-response to Retry (recommended)
Add `PFGameSaveFilesSetUiSyncFailedAutoResponse` with `action: Retry` to tests that should survive transient errors:
```yaml
- command: PFGameSaveFilesSetUiSyncFailedAutoResponse
  parameters:
    enable: true
    action: Retry
    maxRetries: 3
```
This matches real game behavior where transient errors are retried.

### Option 2: Accept as known flakiness
Document that tests 11/14 may intermittently fail when PlayFab service has transient issues. Re-run to confirm.

### Option 3: Add test retry logic in the runner
The `tests-run.py` script could auto-retry tests that fail with E_PF_INTERNAL_SERVER_ERROR.

## Impact

- Not a product bug — the SDK correctly surfaces the server error via the SyncFailed callback
- Real games would show a "sync failed, retry?" dialog and the user would retry
- Tests using `Cancel` as the SyncFailed response are overly sensitive to transient errors

## Log References
- `Out/gamesave-pc-tests/pass111/11/device-DeviceB-fetched-log.txt`
- `Out/gamesave-pc-tests/pass111/14/device-DeviceB-fetched-log.txt`

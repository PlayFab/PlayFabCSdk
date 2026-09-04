# Section O: Rate Limiting / Upload Stress — Scenario Details

> Common init/cleanup/reset blocks are in [`_common-blocks.md`](./_common-blocks.md).

---

### ID84. Rapid Upload Rate Limit

**Priority**: P2 | **Category**: Rate Limiting and Upload Stress | **Devices**: 1

**Scenario**: Upload save data rapidly — more than 10 times per minute. Verify the system handles the rate limit gracefully.

**Why This Matters**: Documented: "Title players are limited to 100 service endpoint requests in any 2-minute period." A game with frequent auto-saves could hit this. The system must not crash or lose data.

1. **Init**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Wait for progress callback → sync completes

2. **Rapid Upload Loop** (15 iterations, ~4 seconds apart = ~15 uploads per minute):
    - For i = 1 to 15:
        - `PFGameSaveFilesGetFolder` → get save path
        - Write incremental data: `save.dat` with `"upload_iteration_{i}"`
        - `PFGameSaveFilesUploadWithUiAsync` → `{}`
        - Record the `XAsyncBlock` completion HRESULT and any callbacks that fire
        - Wait 4 seconds
    - After all 15 iterations, record which uploads succeeded, which failed, and what errors were returned

3. **Verify Final State**:
    - `CaptureSaveContainerSnapshot`
    - `PFGameSaveFilesGetRemainingQuota` → verify quota is coherent
    - `PFGameSaveFilesUninitialize` → `{}`
    - `PFCleanupAsync`

4. *Key assertions*:
    - No crash or hang during rapid uploads
    - Rate limit surfaces as either: SyncFailed callback with retry option, specific HRESULT, or internal queuing
    - Final snapshot matches the most recently successfully uploaded data
    - No data corruption — last successful upload is coherent
    - Quota reported correctly after stress test

---

---

### ID85. Upload Sync Failure — Rate Limit During UploadWithUiAsync

**Priority**: P2 | **Category**: Rate Limiting and Upload Stress | **Devices**: 1

**Scenario**: Trigger a sync failure specifically during `UploadWithUiAsync` (not during init). Use rapid uploads to hit the rate limit. Verify the SyncFailed callback fires during the upload, and test both Retry and Cancel responses.

**Why This Matters**: All our existing sync failure tests trigger the failure during `AddUserWithUiAsync`. The docs state callbacks fire during both AddUser and Upload. Upload-time sync failure has zero coverage.

1. **Init**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Wait for progress callback → sync completes

2. **Burn Through Rate Limit**:
    - Rapidly upload 12–15 times with minimal delay (2-second intervals):
        - Write small data, call `PFGameSaveFilesUploadWithUiAsync` → `{}`
        - Wait for completion (success or failure)
    - Eventually, a SyncFailed callback should fire during one of the uploads

3. **Test Retry Response**:
    - When SyncFailed fires during upload:
    - `PFGameSaveFilesSetUiSyncFailedResponse` → `{ "response": "Retry" }`
    - Wait — callback may fire again if rate limit still active
    - Eventually upload should succeed (after rate limit window passes)
    - Record final `XAsyncBlock` HRESULT

4. **Test Cancel Response** (separate run or subsequent attempt):
    - When SyncFailed fires during upload:
    - `PFGameSaveFilesSetUiSyncFailedResponse` → `{ "response": "Cancel" }`
    - `XAsyncBlock` should complete with error HRESULT
    - Verify local data is still intact

5. **Cleanup**:
    - `PFGameSaveFilesUninitialize` → `{}`
    - `PFCleanupAsync`

6. *Key assertions*:
    - SyncFailed callback fires during `UploadWithUiAsync` (not just during AddUser)
    - Retry response eventually succeeds after backoff
    - Cancel response returns error HRESULT, local data intact
    - No crash, no orphaned async state


# Section G: Out of Storage — Scenario Details

> Common init/cleanup/reset blocks are in [`_common-blocks.md`](./_common-blocks.md).

---

### ID44. Storage Full During Init

1. Run Common Account Reset (Xbox).
2. **Device B — Create Stale Lock** (so Device A sees contention dialog during init):
    - `XGameRuntimeInitialize`
    - `PFInitialize`
    - `PFServicesInitialize`
    - `PFServiceConfigCreateHandle` (endpoint: `https://E18D7.playfabapi.com`, titleId: `E18D7`)
    - `XTaskQueueCreate` (workMode: ThreadPool, completionMode: ThreadPool, setAsProcessQueue: true)
    - `PFGameSaveFilesInitialize`
    - `PFGameSaveFilesSetUiCallbacks` (enable: true)
    - `PFGameSaveFilesSetActiveDeviceChangedCallback`
    - `XUserAddAsync`
    - `PFLocalUserCreateHandleWithXboxUser`
    - `PFGameSaveFilesAddUserWithUiAsync`
    - `PFGameSaveFilesGetFolder`
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/payload.bin`, bytes: 10240, pattern: [0xAA, 0xBB, 0xCC, 0xDD]
    - `PFGameSaveFilesUploadWithUiAsync` (mode: KeepDeviceActive)
    - **[DISCONNECT]** Device B from network
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
3. **Device A — Init With Storage Pressure**:
    - `XGameRuntimeInitialize`
    - `PFInitialize`
    - `PFServicesInitialize`
    - `PFServiceConfigCreateHandle` (endpoint: `https://E18D7.playfabapi.com`, titleId: `E18D7`)
    - `XTaskQueueCreate` (workMode: ThreadPool, completionMode: ThreadPool, setAsProcessQueue: true)
    - `PFGameSaveFilesInitialize`
    - `PFGameSaveFilesSetUiCallbacks` (enable: true)
    - `PFGameSaveFilesSetActiveDeviceChangedCallback`
    - `XUserAddAsync`
    - `PFLocalUserCreateHandleWithXboxUser`
    - `DeleteSaveRoot`
    - `PFGameSaveFilesAddUserWithUiAsync` — begins init; contention dialog appears
    - While contention dialog is showing, `ConsumeDiskSpace` — fill storage to capacity
    - Respond with `PFGameSaveFilesSetUiActiveDeviceContentionAutoResponse` (action: SyncLastSavedData) — attempt download
    - Verify out-of-storage error appears when download runs out of space
    - Verify app does not crash and handles error gracefully (retry, cancel, or guidance)
    - `FreeDiskSpace` — free sufficient disk space while app is still showing the error
    - Retry sync — select "Try Again" or close and relaunch
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot`
    - Verify no partial or corrupt save containers, data from Device B present after retry
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
4. *Key assertions*: Out-of-storage callback fires, no crash, retry after freeing space succeeds, no partial containers. Uses `ConsumeDiskSpace` / `FreeDiskSpace` harness commands.

---

---

### ID45. Storage Full During Download

1. Run Common Account Reset (Xbox).
2. **Device B — Seed Cloud Data**:
    - `XGameRuntimeInitialize`
    - `PFInitialize`
    - `PFServicesInitialize`
    - `PFServiceConfigCreateHandle` (endpoint: `https://E18D7.playfabapi.com`, titleId: `E18D7`)
    - `XTaskQueueCreate` (workMode: ThreadPool, completionMode: ThreadPool, setAsProcessQueue: true)
    - `PFGameSaveFilesInitialize`
    - `PFGameSaveFilesSetUiCallbacks` (enable: true)
    - `PFGameSaveFilesSetActiveDeviceChangedCallback`
    - `XUserAddAsync`
    - `PFLocalUserCreateHandleWithXboxUser`
    - `PFGameSaveFilesAddUserWithUiAsync`
    - `PFGameSaveFilesGetFolder`
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/payload.bin`, bytes: 10240, pattern: [0xAA, 0xBB, 0xCC, 0xDD]
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `CaptureSaveContainerSnapshot` (slot: left)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
3. **Device A — Download With Storage Pressure**:
    - `XGameRuntimeInitialize`
    - `PFInitialize`
    - `PFServicesInitialize`
    - `PFServiceConfigCreateHandle` (endpoint: `https://E18D7.playfabapi.com`, titleId: `E18D7`)
    - `XTaskQueueCreate` (workMode: ThreadPool, completionMode: ThreadPool, setAsProcessQueue: true)
    - `PFGameSaveFilesInitialize`
    - `PFGameSaveFilesSetUiCallbacks` (enable: true)
    - `PFGameSaveFilesSetActiveDeviceChangedCallback`
    - `XUserAddAsync`
    - `PFLocalUserCreateHandleWithXboxUser`
    - `DeleteSaveRoot` — clear local data
    - `ConsumeDiskSpace` — fill storage to capacity before starting download
    - `PFGameSaveFilesAddUserWithUiAsync` — begins cloud download; should fail due to no space
    - Verify out-of-storage error appears
    - Verify download aborts cleanly — no partial files, no corruption
    - `FreeDiskSpace` — free sufficient disk space
    - Retry: close and relaunch, or select "Try Again" on error prompt
    - `PFGameSaveFilesAddUserWithUiAsync` — download restarts
    - Verify download completes successfully
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot` (slot: right)
    - `CompareSaveContainerSnapshots` (ignoreTimestamps: true)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
4. *Key assertions*: Out-of-storage error fires, download aborts cleanly, no corruption, retry after freeing space succeeds, snapshot matches Device B's data.

---

---

### ID46. Storage Full During Upload

1. Run Common Account Reset (Xbox).
2. **Device A — Write With Storage Pressure**:
    - `XGameRuntimeInitialize`
    - `PFInitialize`
    - `PFServicesInitialize`
    - `PFServiceConfigCreateHandle` (endpoint: `https://E18D7.playfabapi.com`, titleId: `E18D7`)
    - `XTaskQueueCreate` (workMode: ThreadPool, completionMode: ThreadPool, setAsProcessQueue: true)
    - `PFGameSaveFilesInitialize`
    - `PFGameSaveFilesSetUiCallbacks` (enable: true)
    - `PFGameSaveFilesSetActiveDeviceChangedCallback`
    - `XUserAddAsync`
    - `PFLocalUserCreateHandleWithXboxUser`
    - `PFGameSaveFilesAddUserWithUiAsync`
    - `PFGameSaveFilesGetFolder`
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/payload.bin`, bytes: 10240, pattern: [0xAA, 0xBB, 0xCC, 0xDD]
    - `CaptureSaveContainerSnapshot` (slot: left)
    - `ConsumeDiskSpace` — fill storage to capacity after write
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
    - App closes — GRTS background upload begins under storage pressure
3. **Wait**: Controller waits 5 minutes — the upload reads from local save data (should not need free space to upload, but verify).
4. **Device B — Verify**:
    - `XGameRuntimeInitialize`
    - `PFInitialize`
    - `PFServicesInitialize`
    - `PFServiceConfigCreateHandle` (endpoint: `https://E18D7.playfabapi.com`, titleId: `E18D7`)
    - `XTaskQueueCreate` (workMode: ThreadPool, completionMode: ThreadPool, setAsProcessQueue: true)
    - `PFGameSaveFilesInitialize`
    - `PFGameSaveFilesSetUiCallbacks` (enable: true)
    - `PFGameSaveFilesSetActiveDeviceChangedCallback`
    - `PFGameSaveFilesSetUiActiveDeviceContentionAutoResponse` (enable: true, action: SyncLastSavedData)
    - `XUserAddAsync`
    - `PFLocalUserCreateHandleWithXboxUser`
    - `PFGameSaveFilesAddUserWithUiAsync`
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot` (slot: right)
    - `CompareSaveContainerSnapshots` (ignoreTimestamps: true)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
5. **If upload failed** — Device B has no matching data:
    - `FreeDiskSpace` on Device A
    - Relaunch Device A and re-upload
    - Retry Device B verification
    - Document behavior and whether retry occurs after freeing space
6. *Key assertions*: Upload reads from local data and should not need free space; verify on Device B. Document behavior if upload fails under full-storage conditions.

---

---

### ID47. Cloud Quota Exhaustion — 256 MB Limit

1. Run Common Account Reset (Xbox).
2. **Device A — Approach Quota Limit**:
    - `XGameRuntimeInitialize`
    - `PFInitialize`
    - `PFServicesInitialize`
    - `PFServiceConfigCreateHandle` (endpoint: `https://E18D7.playfabapi.com`, titleId: `E18D7`)
    - `XTaskQueueCreate` (workMode: ThreadPool, completionMode: ThreadPool, setAsProcessQueue: true)
    - `PFGameSaveFilesInitialize`
    - `PFGameSaveFilesSetUiCallbacks` (enable: true)
    - `PFGameSaveFilesSetActiveDeviceChangedCallback`
    - `XUserAddAsync`
    - `PFLocalUserCreateHandleWithXboxUser`
    - `PFGameSaveFilesAddUserWithUiAsync`
    - `PFGameSaveFilesGetFolder`
    - Write large save data approaching 256 MB limit:
      - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/bigfile1.bin`, bytes: 67108864 (64 MB), pattern: [0xAA]
      - `PFGameSaveFilesUploadWithUiAsync` (mode: KeepDeviceActive)
      - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/bigfile2.bin`, bytes: 67108864 (64 MB), pattern: [0xBB]
      - `PFGameSaveFilesUploadWithUiAsync` (mode: KeepDeviceActive)
      - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/bigfile3.bin`, bytes: 67108864 (64 MB), pattern: [0xCC]
      - `PFGameSaveFilesUploadWithUiAsync` (mode: KeepDeviceActive)
      - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/bigfile4.bin`, bytes: 67108864 (64 MB), pattern: [0xDD] — total now 256 MB
      - `PFGameSaveFilesUploadWithUiAsync` (mode: KeepDeviceActive)
    - Attempt one more write that would exceed quota:
      - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/overflow.bin`, bytes: 10485760 (10 MB), pattern: [0xEE]
      - `PFGameSaveFilesUploadWithUiAsync` — should fail with quota error
    - Verify upload is blocked with appropriate error — document exact error code/message
    - Verify no silent data loss (prior saves still intact)
3. **Device A — Recover Quota**:
    - Delete some uploaded data to free quota:
      - `DeleteSaveRoot` or targeted file deletion to remove bigfile4.bin
    - `PFGameSaveFilesUploadWithUiAsync` (mode: KeepDeviceActive) — upload deletion
    - Retry the overflow write:
      - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/overflow.bin`, bytes: 10485760 (10 MB), pattern: [0xEE]
      - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - Verify upload succeeds after quota recovery
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
4. **Wait**: Controller waits 3–5 minutes for background upload.
5. **Device B — Verify**:
    - `XGameRuntimeInitialize`
    - `PFInitialize`
    - `PFServicesInitialize`
    - `PFServiceConfigCreateHandle` (endpoint: `https://E18D7.playfabapi.com`, titleId: `E18D7`)
    - `XTaskQueueCreate` (workMode: ThreadPool, completionMode: ThreadPool, setAsProcessQueue: true)
    - `PFGameSaveFilesInitialize`
    - `PFGameSaveFilesSetUiCallbacks` (enable: true)
    - `PFGameSaveFilesSetActiveDeviceChangedCallback`
    - `PFGameSaveFilesSetUiActiveDeviceContentionAutoResponse` (enable: true, action: SyncLastSavedData)
    - `XUserAddAsync`
    - `PFLocalUserCreateHandleWithXboxUser`
    - `PFGameSaveFilesAddUserWithUiAsync`
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot`
    - Verify progress from Device A is present (bigfile1-3 + overflow, bigfile4 deleted)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
6. *Key assertions*: Quota error surfaces correctly, no silent data loss, quota recovery works after deleting data, Device B verifies final state.

---

---

### ID48. OutOfStorage — Cancel Response

1. Run Common Account Reset (Xbox).
2. **Device B — Seed Cloud Data**:
    - Full init on Device B
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/payload.bin`, bytes: 10240, pattern: [0xAA, 0xBB, 0xCC, 0xDD]
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - Full cleanup
3. **Wait**: 3–5 minutes for GRTS background upload.
4. **Device A — Fill Storage, Start Download, Cancel**:
    - Full init on Device A
    - `DeleteSaveRoot` — clear local
    - `ConsumeDiskSpace` — fill storage to capacity
    - `PFGameSaveFilesSetUiOutOfStorageAutoResponse` (enable: true, action: Cancel)
    - `PFGameSaveFilesAddUserWithUiAsync` — begins download, out-of-storage callback fires, auto-responds Cancel
    - Verify: `AddUserWithUiAsync` returns `E_PF_GAMESAVE_USER_CANCELLED` (0x800704c7)
    - Verify: no partial files on disk
    - `FreeDiskSpace`
    - Full cleanup
5. **Device A — Retry After Freeing Space**:
    - Full init on Device A
    - `PFGameSaveFilesAddUserWithUiAsync` — download succeeds now
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot`
    - Full cleanup
6. *Key assertions*: Cancel returns correct HRESULT, no partial files, retry after freeing space succeeds.

---

---

### ID49. OutOfStorage — Cancel During Upload

**Priority**: P2 | **Category**: Out of Storage | **Devices**: 1

**Scenario**: Fill storage. Write data. Start upload. If the out-of-storage callback fires during upload (rather than download), respond with `Cancel`. Verify the upload aborts with `E_PF_GAMESAVE_USER_CANCELLED` and local data remains intact.

**Why This Matters**: Scenario ID48 tests Cancel during download-time out-of-storage. This tests Cancel during upload-time out-of-storage, which may have different behavior since GRTS handles uploads out-of-process.

1. **Fill Storage**:
    - `ConsumeDiskSpace` → leave minimal free space

2. **Write and Upload**:
    - Init, write save data
    - `PFGameSaveFilesUploadWithUiAsync` → `{}`
    - If out-of-storage callback fires:
        - `PFGameSaveFilesSetUiOutOfStorageResponse` → `{ "response": "Cancel" }`
        - **KEY CHECK**: Upload returns `E_PF_GAMESAVE_USER_CANCELLED`
    - If upload succeeds (GRTS may not need local free space for upload):
        - Document that out-of-storage doesn't fire during upload

3. **Verify Local Data Intact**:
    - `PFGameSaveFilesGetFolder` → path valid
    - Verify save data readable
    - Cleanup: `ReleaseDiskSpace`, uninit

4. *Key assertions*:
    - If callback fires: Cancel returns correct HRESULT, local data intact
    - If callback doesn't fire: document behavior (upload may not need local free space)


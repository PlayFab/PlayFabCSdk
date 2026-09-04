# Section E: Progress, Cancel, and Mid-Sync Actions — Scenario Details

> Common init/cleanup/reset blocks are in [`_common-blocks.md`](./_common-blocks.md).

---

### ID32. Progress Query During Download

1. Run Common Account Reset (Xbox).
2. **Device B — Seed Moderately Large Data**:
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
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/large.bin`, bytes: 10485760 (10 MB), pattern: [0xAA]
    - `CaptureSaveContainerSnapshot` (slot: left)
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
3. **Wait**: Controller waits 3–5 minutes for GRTS background upload.
4. **Device A — Download With Progress Query**:
    - `XGameRuntimeInitialize`
    - `PFInitialize`
    - `PFServicesInitialize`
    - `PFServiceConfigCreateHandle` (endpoint: `https://E18D7.playfabapi.com`, titleId: `E18D7`)
    - `XTaskQueueCreate` (workMode: ThreadPool, completionMode: ThreadPool, setAsProcessQueue: true)
    - `PFGameSaveFilesInitialize`
    - `PFGameSaveFilesSetUiCallbacks` (enable: true) — register progress callback
    - `PFGameSaveFilesSetActiveDeviceChangedCallback`
    - `XUserAddAsync`
    - `PFLocalUserCreateHandleWithXboxUser`
    - `DeleteSaveRoot` — clear local data to force download
    - Inside the progress callback implementation, call:
      - `PFGameSaveFilesUiProgressGetProgress` — query syncState, current bytes, total bytes
      - Record all values in a progress log
    - `PFGameSaveFilesAddUserWithUiAsync` — begins download; progress callback fires multiple times
    - After completion, analyze progress log:
      - Verify `syncState` transitions are monotonic: NotStarted (0) → PreparingForDownload (1) → Downloading (2) → SyncComplete (5)
      - Verify `current` bytes increase monotonically
      - Verify `total` is > 0 and approximately 10 MB
      - Verify `GetProgress` returns `S_OK` each time
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
5. *Key assertions*: `GetProgress` returns `S_OK`, state transitions are monotonic, byte counts are sane (increasing, total matches), download completes and snapshot matches.

---

---

### ID33. Cancel Sync During Download

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
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/large.bin`, bytes: 10485760 (10 MB), pattern: [0xAA]
    - `CaptureSaveContainerSnapshot` (slot: left)
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
3. **Wait**: Controller waits 3–5 minutes for GRTS background upload.
4. **Device A — Download Then Cancel**:
    - `XGameRuntimeInitialize`
    - `PFInitialize`
    - `PFServicesInitialize`
    - `PFServiceConfigCreateHandle` (endpoint: `https://E18D7.playfabapi.com`, titleId: `E18D7`)
    - `XTaskQueueCreate` (workMode: ThreadPool, completionMode: ThreadPool, setAsProcessQueue: true)
    - `PFGameSaveFilesInitialize`
    - `PFGameSaveFilesSetUiCallbacks` (enable: true) — register progress callback
    - `PFGameSaveFilesSetActiveDeviceChangedCallback`
    - Configure progress callback to call `PFGameSaveFilesSetUiProgressResponse` (action: Cancel) after the first progress update
    - `XUserAddAsync`
    - `PFLocalUserCreateHandleWithXboxUser`
    - `DeleteSaveRoot`
    - `PFGameSaveFilesAddUserWithUiAsync` — begins download, progress callback fires, Cancel response sent
    - Verify: `AddUserWithUiAsync` returns `E_PF_GAMESAVE_USER_CANCELLED` (0x800704c7)
    - Verify: no partial files left on disk
5. **Device A — Relaunch and Complete Download**:
    - Remove the Cancel auto-response (let progress complete normally)
    - `PFGameSaveFilesAddUserWithUiAsync` — download restarts cleanly
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
6. *Key assertions*: Cancel returns `E_PF_GAMESAVE_USER_CANCELLED`, no partial files on disk, relaunch download completes cleanly and snapshot matches.

---

---

### ID34. Cancel Sync During Upload

1. Run Common Account Reset (Xbox).
2. **Device A — Write and Attempt Upload With Cancel**:
    - `XGameRuntimeInitialize`
    - `PFInitialize`
    - `PFServicesInitialize`
    - `PFServiceConfigCreateHandle` (endpoint: `https://E18D7.playfabapi.com`, titleId: `E18D7`)
    - `XTaskQueueCreate` (workMode: ThreadPool, completionMode: ThreadPool, setAsProcessQueue: true)
    - `PFGameSaveFilesInitialize`
    - `PFGameSaveFilesSetUiCallbacks` (enable: true) — register progress callback
    - `PFGameSaveFilesSetActiveDeviceChangedCallback`
    - `XUserAddAsync`
    - `PFLocalUserCreateHandleWithXboxUser`
    - `PFGameSaveFilesAddUserWithUiAsync`
    - `PFGameSaveFilesGetFolder`
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/payload.bin`, bytes: 10485760 (10 MB), pattern: [0xAA, 0xBB, 0xCC, 0xDD]
    - `CaptureSaveContainerSnapshot` (slot: left)
    - Configure progress callback to call `PFGameSaveFilesSetUiProgressResponse` (action: Cancel) when sync state reaches `Uploading` (0x4)
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive) — upload starts, Cancel sent during upload
    - Verify: `UploadWithUiAsync` returns `E_PF_GAMESAVE_USER_CANCELLED` (0x800704c7)
    - Verify: local data still intact
3. **Device A — Retry Upload Successfully**:
    - Remove the Cancel auto-response
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive) — upload succeeds this time
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
4. **Wait**: Controller waits 3–5 minutes for GRTS background upload.
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
    - `CaptureSaveContainerSnapshot` (slot: right)
    - `CompareSaveContainerSnapshots` (ignoreTimestamps: true)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
6. *Key assertions*: Cancel returns correct HRESULT, local data intact after cancel, cloud state coherent (no partial upload from cancelled attempt), retry upload succeeds, Device B receives correct data.

---

---

### ID35. Write to Save Folder During Upload

**Priority**: P2 | **Category**: Write During Upload | **Devices**: 1

**Scenario**: Start an upload. Monitor the progress callback for sync state `Uploading` (files captured). Once `Uploading`, immediately write new data to the save folder. Verify the current upload completes with the pre-write data, and a subsequent upload captures the new data.

**Why This Matters**: Documented: "Once the sync state transitions to `Uploading`, the system has finished reading your files and it's safe to write to the save folder again." Games with frequent saves need to write during upload.

1. **Init and Create Initial Save**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Wait for progress callback → sync completes
    - `PFGameSaveFilesGetFolder` → get save path
    - Write initial save: `save.dat` with "initial_data_v1"

2. **Start Upload and Monitor Progress**:
    - `PFGameSaveFilesUploadWithUiAsync` → `{}`
    - In the progress callback, check `PFGameSaveFilesUiProgressGetProgress()`:
        - Wait for sync state to reach `Uploading` (value 0x4 — files captured, now uploading)
        - Once `Uploading` state detected:
            - Immediately write new data: `save.dat` with "updated_data_v2"
            - Record the timestamp of the write
    - Wait for upload to complete
    - `CaptureSaveContainerSnapshot` → "snapshot_after_first_upload"

3. **Upload Again to Capture New Data**:
    - `PFGameSaveFilesUploadWithUiAsync` → `{}`
    - Wait for upload complete
    - `CaptureSaveContainerSnapshot` → "snapshot_after_second_upload"

4. **Cleanup**:
    - `PFGameSaveFilesUninitialize` → `{}`
    - `PFCleanupAsync`

5. *Key assertions*:
    - Write during `Uploading` state succeeds (no file lock error)
    - First upload snapshot = "initial_data_v1" (pre-write data — the upload captured data before the write)
    - Second upload snapshot = "updated_data_v2" (new data captured in second upload)
    - No corruption in either snapshot
    - Note: Monitoring sync state within the progress callback requires the handler to check `PFGameSaveFilesUiProgressGetProgress()` and conditionally write — this may require a specialized test handler

---

---

### ID36. Sync State Transition Sequence Verification

**Priority**: P2 | **Category**: Progress / Cancel | **Devices**: 2

**Scenario**: During a download, record every sync state reported by the progress callback. Verify the transitions follow the expected sequence: `NotStarted` → `PreparingForDownload` → `Downloading` → `SyncComplete`. During an upload, verify: `NotStarted` → `PreparingForUpload` → `Uploading` → `SyncComplete`.

**Why This Matters**: Games use sync state for progress bars and "safe to write" detection. Out-of-order or skipped states cause incorrect UI or premature writes.

1. **Download State Sequence**:
    - Device B: Init, write 10+ MB, upload, uninit
    - Device A: Init
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - In every progress callback invocation, record `PFGameSaveFilesUiProgressGetProgress()` → `(syncState, currentBytes, totalBytes)`
    - Wait for sync complete
    - **KEY CHECK**: States appeared in order. No backward transitions. `totalBytes` > 0 once downloading. `currentBytes` monotonically increases.

2. **Upload State Sequence**:
    - Write 10+ MB save data
    - `PFGameSaveFilesUploadWithUiAsync` → `{}`
    - Record every progress callback: `(syncState, currentBytes, totalBytes)`
    - Wait for upload complete
    - **KEY CHECK**: States appeared in order for upload path.

3. **Cleanup**:
    - `PFGameSaveFilesUninitialize`, `PFCleanupAsync`

4. *Key assertions*:
    - No backward state transitions (e.g., `Downloading` → `PreparingForDownload` is a bug)
    - `SyncComplete` is always the final state
    - `totalBytes` is consistent across callbacks (doesn't jump around)
    - `currentBytes` ≤ `totalBytes` at all times


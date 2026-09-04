# Section N: Multi-File / Large Data / Network — Scenario Details

> Common init/cleanup/reset blocks are in [`_common-blocks.md`](./_common-blocks.md).

---

### ID80. Network Flapping During Sync

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
3. **Device A — Download With Network Flapping**:
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
    - `PFGameSaveFilesAddUserWithUiAsync` — begins cloud download
    - During download, rapidly toggle network connectivity:
      - **[DISCONNECT]** — wait 3 seconds
      - **[RECONNECT]** — wait 3 seconds
      - **[DISCONNECT]** — wait 3 seconds
      - **[RECONNECT]** — wait 3 seconds
      - **[DISCONNECT]** — wait 3 seconds
      - **[RECONNECT]** — wait 3 seconds
    - Wait for download to settle (up to 2 minutes)
    - **If sync succeeded**:
      - `PFGameSaveFilesGetFolder`
      - `CaptureSaveContainerSnapshot` (slot: right)
      - `CompareSaveContainerSnapshots` (ignoreTimestamps: true)
    - **If sync failed**:
      - Verify clean error (no hang, no corruption)
      - Close and relaunch — verify clean recovery on retry
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
4. *Key assertions*: No hang, no corruption, either succeeds or provides clean retry path, snapshot matches after eventual success. **[DISCONNECT]** / **[RECONNECT]** depend on automation investigation (managed switch or dev kit network control).

---

---

### ID81. Multi-File Sync Across Devices

1. Run Common Account Reset (Xbox).
2. **Device A — Create Multiple Save Files**:
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
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/save1.bin`, bytes: 10240, pattern: [0xAA]
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/save2.bin`, bytes: 20480, pattern: [0xBB]
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `settings/config.bin`, bytes: 4096, pattern: [0xCC]
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `achievements/stats.bin`, bytes: 8192, pattern: [0xDD]
    - `CaptureSaveContainerSnapshot` (slot: left) — snapshot with all 4 files
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
3. **Wait**: Controller waits 3–5 minutes for GRTS background upload.
4. **Device B — Sync and Verify (Clean Local)**:
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
    - `DeleteSaveRoot` — ensure clean local
    - `PFGameSaveFilesAddUserWithUiAsync` — downloads all files from cloud
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot` (slot: right)
    - `CompareSaveContainerSnapshots` (ignoreTimestamps: true) — all 4 files present and matching
    - Verify: `progress/save1.bin`, `progress/save2.bin`, `settings/config.bin`, `achievements/stats.bin` all present
5. **Device B — Modify and Upload**:
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/save1.bin`, bytes: 10240, pattern: [0xEE] — modified save1
    - `CaptureSaveContainerSnapshot` (slot: left) — snapshot "Device B modified"
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
6. **Wait**: Controller waits 3–5 minutes for GRTS background upload.
7. **Device A — Re-Sync and Verify (Stale Local)**:
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
    - `PFGameSaveFilesAddUserWithUiAsync` — Device A has stale local data; should download updates
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot` (slot: right)
    - `CompareSaveContainerSnapshots` (ignoreTimestamps: true) — compare against Device B's modified snapshot
    - Verify save1.bin is updated (pattern [0xEE]), other files unchanged
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
8. *Key assertions*: All files propagate, no partial state, modifications apply consistently, stale local data is overwritten correctly.

---

---

### ID82. File/Folder Deletion Propagation Across Devices

1. Run Common Account Reset (Xbox).
2. **Device A — Create Multiple Save Items**:
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
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/keep-this.bin`, bytes: 10240, pattern: [0xAA]
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/delete-this.bin`, bytes: 10240, pattern: [0xBB]
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `settings/config.bin`, bytes: 4096, pattern: [0xCC]
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
3. **Wait**: Controller waits 3–5 minutes for GRTS background upload.
4. **Device B — Sync All Files**:
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
    - `DeleteSaveRoot`
    - `PFGameSaveFilesAddUserWithUiAsync` — downloads all 3 files
    - `PFGameSaveFilesGetFolder`
    - Verify all 3 files present: `keep-this.bin`, `delete-this.bin`, `config.bin`
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
5. **Device A — Delete Specific Item and Upload**:
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
    - Delete `progress/delete-this.bin` (via harness file deletion command or `WriteGameSaveData` with delete operation)
    - `CaptureSaveContainerSnapshot` (slot: left) — should show only `keep-this.bin` and `config.bin`
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
6. **Wait**: Controller waits 3–5 minutes for GRTS background upload.
7. **Device B — Re-Sync and Verify Deletion**:
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
    - `CompareSaveContainerSnapshots` (ignoreTimestamps: true) — compare against Device A's post-deletion snapshot
    - Verify `delete-this.bin` is gone, `keep-this.bin` and `config.bin` intact
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
8. *Key assertions*: Deletion propagates across devices, no stray remnants, unrelated data untouched.

---

---

### ID83. Large Dataset Sync — 100 MB+

1. Run Common Account Reset (Xbox).
2. **Device A — Create 100 MB+ Save Data**:
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
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/large1.bin`, bytes: 52428800 (50 MB), pattern: [0xAA]
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/large2.bin`, bytes: 52428800 (50 MB), pattern: [0xBB]
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/large3.bin`, bytes: 10485760 (10 MB), pattern: [0xCC]
    - `CaptureSaveContainerSnapshot` (slot: left) — snapshot of ~110 MB total
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
3. **Wait**: Controller waits 5–10 minutes for GRTS background upload of large dataset.
4. **Device B — Sync Large Dataset (Clean Local)**:
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
    - `DeleteSaveRoot` — ensure clean local
    - `PFGameSaveFilesAddUserWithUiAsync` — begins large download
    - Verify progress dialog shows meaningful progress (not frozen) — extend timeout to 10 minutes
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot` (slot: right)
    - `CompareSaveContainerSnapshots` (ignoreTimestamps: true) — all 3 large files present and matching
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
5. *Key assertions*: No timeout/stall on large dataset, progress callbacks fire, snapshot matches, no corruption, download completes within extended timeout.


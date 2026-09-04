# Section Q: Power State — Scenario Details

> Common init/cleanup/reset blocks are in [`_common-blocks.md`](./_common-blocks.md).

---

### ID90. Structured Shutdown During Init

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
3. **Device A — Init Then Shutdown**:
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
    - During contention dialog, **[SHUTDOWN]** — structured console shutdown
    - Wait for device to fully shut down
4. **Device A — Power On and Verify**:
    - Power on device
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
    - `PFGameSaveFilesAddUserWithUiAsync` — contention dialog should re-appear
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot`
    - Verify no corruption, data from Device B present
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
5. *Key assertions*: No corruption after shutdown, contention re-prompts, sync completes. **[SHUTDOWN]** depends on automation investigation.

---

---

### ID91. Structured Shutdown During Download

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
3. **Device A — Download Then Shutdown**:
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
    - During download, **[SHUTDOWN]** — structured console shutdown
    - Wait for device to fully shut down
4. **Device A — Power On and Verify**:
    - Power on device
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
    - `PFGameSaveFilesAddUserWithUiAsync` — download restarts
    - Verify download completes
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
5. *Key assertions*: Clean restart after shutdown, snapshot matches Device B's data. **[SHUTDOWN]** depends on automation investigation.

---

---

### ID92. Structured Shutdown During Local Write

1. Run Common Account Reset (Xbox).
2. **Device A — Write Then Shutdown**:
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
    - `DeleteSaveRoot`
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/payload.bin`, bytes: 10240, pattern: [0xAA, 0xBB, 0xCC, 0xDD]
    - Immediately after write, **[SHUTDOWN]** — structured console shutdown
    - Wait for device to fully shut down
3. **Device A — Power On and Verify**:
    - Power on device
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
    - `CaptureSaveContainerSnapshot` (slot: left)
    - Note any anomalies — data may or may not have persisted depending on shutdown timing
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
4. **Wait**: Controller waits 3–5 minutes for GRTS background upload (if data persisted).
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
    - Verify data either persisted and uploaded, or cleanly rolled back
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
6. *Key assertions*: Data either persisted and uploads, or cleanly rolled back. No corruption. **[SHUTDOWN]** depends on automation investigation.

---

---

### ID93. Structured Shutdown During Upload Window

1. Run Common Account Reset (Xbox).
2. **Device A — Write and Close (Trigger Background Upload)**:
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
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
    - App closes — GRTS background upload begins
3. **Device A — Immediate Shutdown Before Upload Completes**:
    - **[SHUTDOWN]** — structured shutdown immediately (within seconds of app close)
    - Wait for device to fully shut down
4. **Device A — Power On**:
    - Power on device
    - Wait 3–5 minutes for GRTS to potentially retry upload after boot
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
    - Verify cloud state is coherent — Device B receives data OR gets clean first-launch
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
6. *Key assertions*: Cloud state is coherent (either upload completed before shutdown or can be retried), Device B receives data or gets clean first-launch. **[SHUTDOWN]** depends on automation investigation.

---

---

### ID94. Power Loss During Init

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
3. **Device A — Init Then Power Pull**:
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
    - During contention dialog, **[POWER-PULL]** — sudden power loss
    - Wait 1 minute
    - Reconnect power, boot device
4. **Device A — Relaunch and Verify**:
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
    - Verify sync completes cleanly, no corrupt containers, no orphaned locks
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
5. *Key assertions*: Sync completes cleanly after power loss, no corrupt containers, no orphaned locks. **[POWER-PULL]** depends on automation investigation (PDU hardware).

---

---

### ID95. Power Loss During Download

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
3. **Device A — Download Then Power Pull**:
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
    - During download, **[POWER-PULL]** — sudden power loss
    - Wait 1 minute
    - Reconnect power, boot device
4. **Device A — Relaunch and Verify**:
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
    - `PFGameSaveFilesAddUserWithUiAsync` — download restarts
    - Verify download completes
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
5. *Key assertions*: No partial files from power loss, clean recovery, snapshot matches Device B's data. **[POWER-PULL]** depends on automation investigation (PDU hardware).

---

---

### ID96. Power Loss During Local Write

1. Run Common Account Reset (Xbox).
2. **Device A — Write Then Power Pull**:
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
    - `DeleteSaveRoot`
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/payload.bin`, bytes: 10240, pattern: [0xAA, 0xBB, 0xCC, 0xDD]
    - Immediately after write, **[POWER-PULL]** — sudden power loss
    - Wait 1 minute
    - Reconnect power, boot device
3. **Device A — Relaunch and Verify**:
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
    - `CaptureSaveContainerSnapshot` (slot: left)
    - Note any anomalies — data may or may not have persisted
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
4. **Wait**: Controller waits 3–5 minutes for GRTS background upload (if data persisted).
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
    - Verify data is either intact (and uploaded) or cleanly rolled back
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
6. *Key assertions*: Data is either intact or cleanly rolled back after power loss, no corruption. **[POWER-PULL]** depends on automation investigation (PDU hardware).

---

---

### ID97. Power Loss During Upload Window

1. Run Common Account Reset (Xbox).
2. **Device A — Write and Close (Trigger Background Upload)**:
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
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
    - App closes — GRTS background upload begins
3. **Device A — Immediate Power Pull**:
    - **[POWER-PULL]** — sudden power loss immediately (within seconds of app close, before upload completes)
    - Wait 1 minute
    - Reconnect power, boot device
4. **Wait**: Controller waits 3–5 minutes after boot for GRTS to potentially retry upload.
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
    - Verify cloud manifest is coherent — either upload finalized or rolled back
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
6. *Key assertions*: Cloud manifest is coherent (either upload finalized or rolled back), no partial data on cloud. **[POWER-PULL]** depends on automation investigation (PDU hardware).


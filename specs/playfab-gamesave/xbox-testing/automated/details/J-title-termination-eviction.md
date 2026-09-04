# Section J: Title Termination / Eviction — Scenario Details

> Common init/cleanup/reset blocks are in [`_common-blocks.md`](./_common-blocks.md).

---

### ID60. Title Termination During Init

1. Run Common Account Reset (Xbox).
2. **Device B — Create Stale Lock** (so Device A sees contention during init):
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
3. **Device A — Init Then Terminate**:
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
    - During contention dialog, **[TERMINATE]** — force-kill the game process
    - Wait 30 seconds
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
    - `PFGameSaveFilesAddUserWithUiAsync` — sync starts fresh; contention dialog should re-appear
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot`
    - Verify no partial or corrupt containers, data from Device B present
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
5. *Key assertions*: Termination during init leaves no corrupt state, relaunch starts fresh, contention re-prompts, sync completes. **[TERMINATE]** depends on automation investigation (e.g., Xbox Device Portal API).

---

---

### ID61. Title Termination During Download

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
3. **Device A — Download Then Terminate**:
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
    - During sync/download, **[TERMINATE]** — force-kill the game process
    - Wait 30 seconds
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
    - Verify download completes normally
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
5. *Key assertions*: No partial files from interrupted download, relaunch restarts download cleanly, snapshot matches Device B's data. **[TERMINATE]** depends on automation investigation.

---

---

### ID62. Title Termination During Local Write

1. Run Common Account Reset (Xbox).
2. **Device A — Write Then Terminate**:
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
    - Immediately after write, **[TERMINATE]** — force-kill the game process
    - Wait 30 seconds
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
    - Note any anomalies or crashes — data may or may not have persisted depending on timing
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
    - Verify data consistency — either the write persisted and uploaded, or cleanly rolled back
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
6. *Key assertions*: No corruption from terminated write, data is either intact (and uploads) or cleanly rolled back, no crash on relaunch. **[TERMINATE]** depends on automation investigation.

---

---

---

### ID63. Title Switching During Init

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
3. **Device A — Init Then Switch Title**:
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
    - During contention dialog, **[EVICT-GAME]** — launch another title on Device A (evicts the game)
    - Wait 2 minutes playing the other title
    - **Return to the original title via Quick Resume** (if available) or relaunch
4. **Device A — Verify Recovery**:
    - Verify app recovers from eviction
    - If Quick Resume: verify contention dialog re-prompts or re-init occurs
    - If relaunched: verify sync starts fresh
    - `PFGameSaveFilesSetUiActiveDeviceContentionAutoResponse` (enable: true, action: SyncLastSavedData)
    - Respond to any contention dialog
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot`
    - Verify sync completes and data from Device B present
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
5. *Key assertions*: App recovers from eviction, contention dialog re-prompts, sync completes. **[EVICT-GAME]** depends on automation investigation.

---

---

### ID64. Title Switching During Download

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
3. **Device A — Download Then Switch Title**:
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
    - During download, **[EVICT-GAME]** — launch another title on Device A
    - Wait 2 minutes playing the other title
    - **Return to the original title via Quick Resume** or relaunch
4. **Device A — Verify Recovery**:
    - Verify download resumes or restarts cleanly
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot` (slot: right)
    - `CompareSaveContainerSnapshots` (ignoreTimestamps: true) — compare against Device B
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
5. *Key assertions*: Either download resumes or restarts cleanly, no corruption, snapshot matches. **[EVICT-GAME]** depends on automation investigation.

---

---

### ID65. Title Switching During Local Write / Upload

1. Run Common Account Reset (Xbox).
2. **Device A — Write Then Switch Title**:
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
    - Immediately after write, **[EVICT-GAME]** — launch another title on Device A
    - Wait 2 minutes playing the other title
    - **Return to the original title via Quick Resume** or relaunch
3. **Device A — Verify Recovery**:
    - Verify save state — data may or may not have persisted depending on eviction timing
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot`
    - Note any anomalies
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
4. **Wait**: Controller waits 3–5 minutes for GRTS background upload (if data was committed before eviction).
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
6. *Key assertions*: Local write persists or rolls back cleanly, background upload completes if data was committed. **[EVICT-GAME]** depends on automation investigation.

---

---

### ID66. Quick Resume Invalidation

1. Run Common Account Reset (Xbox).
2. **Device A — Create Save and Enter Quick Resume**:
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
    - `CaptureSaveContainerSnapshot` (slot: left) — Device A's local state
    - **[EVICT-GAME]** — launch another title to put the game into Quick Resume suspended state
    - Wait 2 minutes playing the other title
3. **Device B — Modify Cloud State**:
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
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/payload.bin`, bytes: 10240, pattern: [0x11, 0x22, 0x33, 0x44] — different data
    - `CaptureSaveContainerSnapshot` (slot: left) — Device B's version
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
4. **Wait**: Controller waits 3–5 minutes for GRTS background upload from Device B.
5. **Device A — Return Via Quick Resume**:
    - Return to the original title on Device A via Quick Resume
    - Verify game detects cloud state change — expects re-init process or cloud sync
    - Verify init/sync completes within 60 seconds (no hang at AddUser or "Resuming" screen)
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot` (slot: right)
    - `CompareSaveContainerSnapshots` (ignoreTimestamps: true) — should match Device B's version, not Device A's stale state
    - Verify Device B's progress (pattern [0x11, 0x22, 0x33, 0x44]) is reflected
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
6. *Key assertions*: Re-init triggers on Quick Resume, cloud data from Device B is received, no stale data served, no hang. **[EVICT-GAME]** and Quick Resume return depend on automation investigation.


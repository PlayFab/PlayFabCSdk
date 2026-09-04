# Section A: Golden Path / Smoke — Scenario Details

> Common init/cleanup/reset blocks are in [`_common-blocks.md`](./_common-blocks.md).

---

### ID1. Single-Device Golden Path Sync — Xbox Live Auth

1. Run Common Account Reset (Xbox).
2. **Session 1 — Upload**:
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
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `CaptureSaveContainerSnapshot` (slot: left)
    - `DeleteSaveRoot` (preserveManifest: false)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
3. **Session 2 — Download**:
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
    - `CaptureSaveContainerSnapshot` (slot: right)
    - `CompareSaveContainerSnapshots`
    - `DeleteSaveRoot` (preserveManifest: false)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
4. *Key assertions*: Snapshot comparison passes (file hashes match), all HRESULTs succeed, no unexpected UI callbacks fire, `PFGameSaveFilesIsConnectedToCloud` returns `true` in both sessions.

---

---

### ID2. Two-Device Golden Path Sync — Xbox Live Auth

1. Run Common Account Reset (Xbox).
2. **Device A — Upload**:
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
    - `PFGameSaveFilesSetSaveDescriptionAsync` (description: "Device A - Initial Upload")
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `PFGameSaveFilesGetSaveDescription` (expectedDescription: "Device A - Initial Upload")
    - `CaptureSaveContainerSnapshot` (slot: left)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
3. **Device B — Download + Modify + Upload**:
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
    - `PFGameSaveFilesGetSaveDescription` (expectedDescription: "Device A - Initial Upload")
    - `WriteGameSaveData` — operations: CreateRandomBinaryFile, relativePath: `progress/payload.bin`, bytes: 12288
    - `PFGameSaveFilesSetSaveDescriptionAsync` (description: "Device B - Updated Save")
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `PFGameSaveFilesGetSaveDescription` (expectedDescription: "Device B - Updated Save")
    - `CaptureSaveContainerSnapshot` (slot: right)
    - `DeleteSaveRoot` (preserveManifest: false)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
4. **Device A — Verify**:
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
    - `PFGameSaveFilesGetSaveDescription` (expectedDescription: "Device B - Updated Save")
    - `CaptureSaveContainerSnapshot` (slot: left)
    - `CompareSaveContainerSnapshots` (ignoreTimestamps: true)
    - `DeleteSaveRoot` (preserveManifest: false)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
5. *Key assertions*: Snapshots match across devices, descriptions propagate correctly, contention auto-response fired on Device B, no data loss.

---

---

### ID3. Background Upload Verification — Out-of-Game Upload

1. Run Common Account Reset (Xbox).
2. **Device A — Write and Close**:
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
    - `CaptureSaveContainerSnapshot` (slot: left)
    - Do NOT call `PFGameSaveFilesUploadWithUiAsync` — the upload should happen out-of-process after app close.
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
3. **Wait**: Controller waits 3–5 minutes for the GRTS background upload to complete. This is a controller-level delay step (`SmokeDelay` or equivalent), not a device command.
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
    - `DeleteSaveRoot` (preserveManifest: false)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
5. *Key assertions*: Snapshot comparison passes — background upload completed after Device A closed. If contention fires on Device B, auto-response handles it. No data loss.

---

---

### ID4. Stock TCUI — Golden Path (No Custom UI Callbacks)

1. Run Common Account Reset (Xbox).
2. **Device A — Init WITHOUT Custom Callbacks**:
    - `XGameRuntimeInitialize`
    - `PFInitialize`
    - `PFServicesInitialize`
    - `PFServiceConfigCreateHandle` (endpoint: `https://E18D7.playfabapi.com`, titleId: `E18D7`)
    - `XTaskQueueCreate` (workMode: ThreadPool, completionMode: ThreadPool, setAsProcessQueue: true)
    - `PFGameSaveFilesInitialize`
    - **Do NOT call `PFGameSaveFilesSetUiCallbacks`** — OR call with `enable: false` — stock TCUI will be used
    - `PFGameSaveFilesSetActiveDeviceChangedCallback`
    - `XUserAddAsync`
    - `PFLocalUserCreateHandleWithXboxUser`
    - `PFGameSaveFilesAddUserWithUiAsync` — stock TCUI renders any progress dialog
    - `PFGameSaveFilesGetFolder`
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/payload.bin`, bytes: 10240, pattern: [0xAA, 0xBB, 0xCC, 0xDD]
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
4. **Device A — Relaunch Without Callbacks and Verify**:
    - Full init WITHOUT `PFGameSaveFilesSetUiCallbacks`
    - `DeleteSaveRoot`
    - `PFGameSaveFilesAddUserWithUiAsync` — stock TCUI renders download progress
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot` (slot: right)
    - `CompareSaveContainerSnapshots` (ignoreTimestamps: true)
    - Full cleanup
5. *Key assertions*: Full round-trip works with stock TCUI, no crash from missing custom callbacks, snapshot matches. Note: visual appearance of stock TCUI dialogs is a manual verification step.


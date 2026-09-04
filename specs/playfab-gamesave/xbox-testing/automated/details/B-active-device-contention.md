# Section B: Active Device Contention — Scenario Details

> Common init/cleanup/reset blocks are in [`_common-blocks.md`](./_common-blocks.md).

---

### ID5. Active Device Contention — Take Over

1. Run Common Account Reset (Xbox).
2. **Device A — Acquire Lock and Upload**:
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
    - `PFGameSaveFilesUploadWithUiAsync` (mode: KeepDeviceActive) — do NOT release the lock
    - `CaptureSaveContainerSnapshot` (slot: left)
    - Keep session alive — do NOT run cleanup yet.
3. **Device B — Contend and Take Over**:
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
    - `PFGameSaveFilesAddUserWithUiAsync` — contention callback fires, auto-responds with SyncLastSavedData
    - `PFGameSaveFilesGetFolder`
    - Verify Device A's payload present via snapshot or file enumeration
    - `WriteGameSaveData` — operations: CreateRandomBinaryFile, relativePath: `progress/payload.bin`, bytes: 12288
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `CaptureSaveContainerSnapshot` (slot: right)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
4. **Device A — Verify Transition**:
    - Verify active-device-changed callback fired on Device A during step 3.
    - `PFGameSaveFilesAddUserWithUiAsync` — re-sync to get Device B's data
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot` (slot: left)
    - `CompareSaveContainerSnapshots` (ignoreTimestamps: true) — comparing against Device B's right snapshot
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
5. *Key assertions*: Contention auto-response fires on Device B, active-device-changed callback fires on Device A, snapshots match Device B's latest data after re-sync.

---

---

### ID6. Active Device Contention — During Download

1. Run Common Account Reset (Xbox).
2. **Device A — Seed Cloud Data**:
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
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/payload.bin`, bytes: 102400 (100 KB for visible download time), pattern: [0xAA, 0xBB, 0xCC, 0xDD]
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
3. **Device B — Start Download (create stale lock by disconnecting Device B before close)**:
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
    - `DeleteSaveRoot` — clear local data so download is required
    - `PFGameSaveFilesAddUserWithUiAsync` — begins download/sync from cloud
    - While download is in progress, Device A takes over (step 4 executes concurrently or immediately after)
4. **Device A — Take Over While B Downloads**:
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
    - `PFGameSaveFilesAddUserWithUiAsync` — takes over active device from B
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot` (slot: left)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
5. **Device B — Observe Interruption**:
    - Verify Device B's download/sync fails gracefully or title terminates
    - Verify no partial or corrupt containers on Device B
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
6. **Device B — Verify Recovery**:
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
    - Verify data integrity — no corruption from interrupted download
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
7. *Key assertions*: No data corruption on either device, no orphaned partial downloads, Device B recovers cleanly on relaunch, Device A's data is intact.

---

---

### ID7. Active Device Contention — During Upload

1. Run Common Account Reset (Xbox).
2. **Device B — Write Data and Close (Trigger Background Upload)**:
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
    - `CaptureSaveContainerSnapshot` (slot: right)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
    - Background upload is now in progress via GRTS.
3. **Device A — Immediately Launch and Take Over** (while Device B's background upload may still be in flight):
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
    - `PFGameSaveFilesAddUserWithUiAsync` — may encounter contention if B's upload hasn't completed, auto-responds with SyncLastSavedData
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot` (slot: left)
    - Verify cloud state is coherent — either Device B's upload completed and data is present, or it was interrupted and cloud still has prior data
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
4. **Device B — Relaunch and Verify**:
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
    - Verify data from step 2 is either present (upload succeeded before takeover) or cloud state is consistent (a conflict may occur if new data was created on Device A)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
5. *Key assertions*: Cloud manifest is coherent — either Device B's upload completed before the takeover or it cleanly reverted. No corruption. Device A syncs without errors. Device B can relaunch and access a valid state.

---

---

### ID8. Contention — Retry Response

1. Run Common Account Reset (Xbox).
2. **Device A — Hold Active Lock**:
    - Full init on Device A
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/payload.bin`, bytes: 10240, pattern: [0xAA, 0xBB, 0xCC, 0xDD]
    - `PFGameSaveFilesUploadWithUiAsync` (mode: KeepDeviceActive) — keep lock
3. **Device B — Launch and Retry Contention**:
    - `XGameRuntimeInitialize`
    - `PFInitialize`
    - `PFServicesInitialize`
    - `PFServiceConfigCreateHandle` (endpoint: `https://E18D7.playfabapi.com`, titleId: `E18D7`)
    - `XTaskQueueCreate` (workMode: ThreadPool, completionMode: ThreadPool, setAsProcessQueue: true)
    - `PFGameSaveFilesInitialize`
    - `PFGameSaveFilesSetUiCallbacks` (enable: true)
    - `PFGameSaveFilesSetActiveDeviceChangedCallback`
    - Configure contention callback to:
      - First invocation: respond with `Retry`
      - Second invocation: respond with `SyncLastSavedData`
    - `XUserAddAsync`
    - `PFLocalUserCreateHandleWithXboxUser`
    - `DeleteSaveRoot`
    - `PFGameSaveFilesAddUserWithUiAsync` — contention fires, responds Retry
    - **Between retries**: Device A releases the lock:
      - On Device A: `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
      - Cleanup Device A
    - Device B's retry should now succeed (lock cleared)
    - Verify sync completes on Device B
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot`
    - Verify data from Device A present
    - Full cleanup Device B
4. *Key assertions*: Retry re-checks lock state, contention callback may fire again or proceed if lock cleared, eventual success when lock releases, no hang.

---

---

### ID9. Stock TCUI — Contention Resolution

1. Run Common Account Reset (Xbox).
2. **Device A — Hold Active Lock (With Callbacks)**:
    - Full init on Device A (with UI callbacks for reliable automation)
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/payload.bin`, bytes: 10240, pattern: [0xAA, 0xBB, 0xCC, 0xDD]
    - `CaptureSaveContainerSnapshot` (slot: left)
    - `PFGameSaveFilesUploadWithUiAsync` (mode: KeepDeviceActive) — keep lock
3. **Device B — Launch WITHOUT Custom Callbacks, Use Auto-Response**:
    - `XGameRuntimeInitialize`
    - `PFInitialize`
    - `PFServicesInitialize`
    - `PFServiceConfigCreateHandle` (endpoint: `https://E18D7.playfabapi.com`, titleId: `E18D7`)
    - `XTaskQueueCreate` (workMode: ThreadPool, completionMode: ThreadPool, setAsProcessQueue: true)
    - `PFGameSaveFilesInitialize`
    - **Do NOT call `PFGameSaveFilesSetUiCallbacks`** — stock TCUI mode
    - `PFGameSaveFilesSetActiveDeviceChangedCallback`
    - `PFGameSaveFilesSetUiActiveDeviceContentionAutoResponse` (enable: true, action: SyncLastSavedData) — auto-response should work even without custom callbacks
    - `XUserAddAsync`
    - `PFLocalUserCreateHandleWithXboxUser`
    - `DeleteSaveRoot`
    - `PFGameSaveFilesAddUserWithUiAsync` — stock TCUI contention dialog appears; auto-response fires
    - Verify sync completes
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot` (slot: right)
    - `CompareSaveContainerSnapshots` (ignoreTimestamps: true)
    - Full cleanup for both devices
4. *Key assertions*: Auto-response fires even without custom callbacks (in stock TCUI mode), contention resolved, sync completes, snapshot matches.

---

---

### ID10. IsConnectedToCloud After Active Device Changed

**Priority**: P2 | **Category**: Offline Mode API Behavior | **Devices**: 2

**Scenario**: Device A is active. Device B takes over via `AddUserWithUiAsync` with `SyncLastSavedData`. The active device changed callback fires on Device A. After the callback, verify `IsConnectedToCloud` returns `false` on Device A.

**Why This Matters**: Documented: "Active device changed: Another device takes over as the active device, automatically putting this device in offline mode." The game must detect this to prevent further upload attempts.

1. **Device A — Init and Upload**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesSetActiveDeviceChangedCallback` → register callback
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Wait for progress callback → sync completes
    - `PFGameSaveFilesIsConnectedToCloud` → returns `true`

2. **Device B — Take Over**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesSetUiActiveDeviceContentionAutoResponse` → `{ "response": "SyncLastSavedData" }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Contention callback fires → auto-response takes over
    - Wait for sync completes

3. **Device A — Verify Disconnected**:
    - Wait for active device changed callback to fire on Device A
    - `PFGameSaveFilesIsConnectedToCloud` → **KEY CHECK**: returns `false`
    - Attempt `PFGameSaveFilesUploadWithUiAsync` → **KEY CHECK**: returns `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD` or `E_PF_GAMESAVE_DEVICE_NO_LONGER_ACTIVE`

4. **Cleanup Both Devices**:
    - Device A: `PFGameSaveFilesUninitialize` → `{}`, `PFCleanupAsync`
    - Device B: `PFGameSaveFilesUninitialize` → `{}`, `PFCleanupAsync`

5. *Key assertions*:
    - Active device changed callback fires on Device A
    - `IsConnectedToCloud` returns `false` on Device A after takeover
    - Upload attempt on Device A returns appropriate error HRESULT
    - No crash on Device A

---

---

### ID11. Active Device Changed — Full Re-Init Lifecycle

**Priority**: P1 | **Category**: Active Device Lifecycle | **Devices**: 2

**Scenario**: Device A is active and playing. Device B takes over. Device A receives the active device changed callback, uninitializes, then re-initializes and calls `AddUserWithUiAsync` again. Verify Device A re-syncs with Device B's latest data.

**Why This Matters**: Documented best practice: "Return to a safe state (main menu)" then "Re-initialize Game Saves system when ready to play again." The full uninit → reinit → resync lifecycle is not tested elsewhere.

1. **Device A — Init, Upload Initial Data, Stay Active**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesSetActiveDeviceChangedCallback` → register callback
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Wait for progress callback → sync completes
    - `PFGameSaveFilesGetFolder` → get save path
    - Write initial save: `save.dat` with "DeviceA_initial"
    - `PFGameSaveFilesUploadWithUiAsync` → `{}`
    - Wait for upload complete
    - Device A stays initialized and active (do NOT uninit)

2. **Device B — Take Over and Upload New Data**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesSetUiActiveDeviceContentionAutoResponse` → `{ "response": "SyncLastSavedData" }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Contention callback fires → auto-response takes over
    - Wait for sync completes — Device B now has Device A's data
    - Write new data: `save.dat` with "DeviceB_updated"
    - `PFGameSaveFilesUploadWithUiAsync` → `{}`
    - Wait for upload complete
    - `PFGameSaveFilesUninitialize` → `{}`
    - `PFCleanupAsync`

3. **Device A — Handle Active Device Changed, Re-Init, Re-Sync**:
    - Active device changed callback fires on Device A (from step 2 takeover)
    - `PFGameSaveFilesUninitialize` → `{}` (simulates returning to main menu)
    - Do NOT call `PFCleanupAsync` — keep PlayFab core alive
    - Re-initialize:
        - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
        - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
        - `PFGameSaveFilesSetActiveDeviceChangedCallback` → register callback again
        - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
        - Wait for progress callback → sync completes
    - `CaptureSaveContainerSnapshot`

4. **Cleanup Device A**:
    - `PFGameSaveFilesUninitialize` → `{}`
    - `PFCleanupAsync`

5. *Key assertions*:
    - Active device changed callback fires on Device A when Device B takes over
    - Uninitialize succeeds cleanly
    - Re-initialize + AddUser succeeds (no stale state from previous session)
    - Device A's snapshot after re-sync = "DeviceB_updated" (Device B's latest data)
    - No orphaned callbacks, no double-free, no stale handles

---

---

### ID12. Contention — "Go Back" from Confirmation Dialog

**Priority**: P2 | **Category**: Active Device Contention | **Devices**: 2

**Scenario**: Device B holds the active lock. Device A launches and gets the contention callback. Instead of immediately resolving, simulate the "Go Back" path — the sample app's UI shows a confirmation dialog before sending `SyncLastSavedData`, and the user can press "Go Back" to return to the main contention dialog. Test that the callback system handles re-entry correctly.

**Why This Matters**: The sample app (and any game copying it) has a 2-step flow: Contention Dialog → "Continue from Cloud Data" → Confirmation Dialog → "Go Back" → back to Contention Dialog. If the callback system doesn't handle this re-entry, the async operation may hang or double-fire.

1. **Device B — Init and Hold Lock**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Wait for sync completes
    - Write save data, upload with `KeepDeviceActive`
    - Device B stays initialized (holds lock)

2. **Device A — Launch, Contention Fires, Simulate "Go Back"**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Contention callback fires on Device A
    - **Do NOT respond immediately** — wait 5 seconds (simulating user reading confirmation dialog, then pressing "Go Back")
    - After 5 seconds, respond with `Retry` (simulating "I went back and chose Retry instead")
    - `PFGameSaveFilesSetUiActiveDeviceContentionResponse` → `{ "response": "Retry" }`
    - Contention callback should fire again (Device B still holds lock)
    - Now respond with `SyncLastSavedData`
    - `PFGameSaveFilesSetUiActiveDeviceContentionResponse` → `{ "response": "SyncLastSavedData" }`
    - Wait for sync complete

3. **Verify**:
    - `CaptureSaveContainerSnapshot` on Device A
    - `PFGameSaveFilesUninitialize` on both devices
    - `PFCleanupAsync` on both

4. *Key assertions*:
    - Delayed response does not cause hang or timeout
    - Retry fires the contention callback again
    - Final `SyncLastSavedData` resolves correctly
    - Snapshot matches Device B's data
    - No double-response errors

---

---

### ID13. Contention — Cancel Response

**Priority**: P1 | **Category**: Active Device Contention | **Devices**: 2

**Scenario**: Device B holds the active lock. Device A launches, contention callback fires. Respond with `Cancel`. Verify `AddUserWithUiAsync` returns `E_PF_GAMESAVE_USER_CANCELLED` and the app can retry later.

**Why This Matters**: Every contention test uses `SyncLastSavedData` or `Retry`. The Cancel path — where the player decides not to proceed — has zero coverage.

1. **Device B — Init and Hold Lock**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Wait for sync completes
    - Write save data, upload with `KeepDeviceActive`

2. **Device A — Launch, Contention, Cancel**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Contention callback fires
    - `PFGameSaveFilesSetUiActiveDeviceContentionResponse` → `{ "response": "Cancel" }`
    - **KEY CHECK**: `XAsyncBlock` completes with `E_PF_GAMESAVE_USER_CANCELLED` (0x800704c7)

3. **Verify Recovery**:
    - Device A: `PFGameSaveFilesAddUserWithUiAsync` → `{}` (retry)
    - Contention callback fires again (Device B still holds lock)
    - This time respond with `SyncLastSavedData`
    - Wait for sync complete
    - `CaptureSaveContainerSnapshot`
    - Cleanup both devices

4. *Key assertions*:
    - Cancel returns `E_PF_GAMESAVE_USER_CANCELLED`
    - No partial state left after cancel
    - Retry after cancel works — contention fires again, can resolve normally
    - No orphaned lock state on cancel


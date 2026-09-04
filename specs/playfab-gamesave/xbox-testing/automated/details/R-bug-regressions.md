# Section R: Bug Regressions — Scenario Details

> Common init/cleanup/reset blocks are in [`_common-blocks.md`](./_common-blocks.md).

---

### ID98. Bug Regression — Delete Everywhere While Running (Bug 61136195)

1. Run Common Account Reset (Xbox).
2. **Device A (Console) — Create Save and Leave Running**:
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
    - `CaptureSaveContainerSnapshot` (slot: left) — note the saved progress
    - **Keep the game RUNNING — do not close**
3. **Device A — Dashboard Delete Everywhere**:
    - Navigate to dashboard: Xbox button → My games & apps → highlight test app → Menu (☰) → Manage game & add-ons → Saved data
    - Select profile → choose "Delete everywhere"
    - **[DELETE-EVERYWHERE]** — this action depends on automation investigation (dashboard navigation)
    - Wait for eviction — may take up to 6 minutes if the game resists PLM termination
    - After eviction, return to home screen
4. **Device A — Relaunch and Verify Fresh Start**:
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
    - Verify game starts fresh — no save data present (delete was successful locally)
    - Verify no orphaned device lock — title syncs as if first-time setup
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
5. **Device B — Verify Clean Cloud State**:
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
    - `CaptureSaveContainerSnapshot`
    - Verify: no device contention TCUI appears on Device B (device lock from Device A was properly released)
    - Verify: cloud save data is also deleted — Device B shows fresh start (no old save data)
    - If Device B shows contention dialog or finds old cloud data → **BUG REPRO (61136195)**
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
6. *Key assertions*: Local and cloud data deleted, no orphaned device lock, Device B gets clean first-launch. **[DELETE-EVERYWHERE]** depends on automation investigation (dashboard navigation).

---

---

### ID99. Bug Regression — Same-Device Contention After Crash (Bug 61456073)

1. Run Common Account Reset (Xbox).
2. **Device A — Create Save and Force Crash**:
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
    - **[TERMINATE]** — force-kill the game process (simulates crash, no graceful shutdown)
    - Wait 30 seconds for system to register the ungraceful termination
3. **Device A — Relaunch (Crash Cycle 1)**:
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
    - If contention dialog appears (same device conflicting with itself):
      - `PFGameSaveFilesSetUiActiveDeviceContentionAutoResponse` (enable: true, action: StopSyncingAndContinue)
    - `PFGameSaveFilesAddUserWithUiAsync`
    - Verify `AddUser` completes within 2 minutes — no indefinite hang
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot`
    - Verify save data from step 2 is intact
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
4. **Device A — Verify Clean Subsequent Launch**:
    - Wait 1 minute
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
    - Verify: no contention TCUI appears on this subsequent launch — device lock was properly cleared
    - Verify: `AddUser` completes within 60 seconds (no hang)
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot`
    - Verify save data intact
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
5. **Repeat crash cycle two more times** (steps 2–4) to confirm the issue does not recur.
6. *Key assertions*: No indefinite hang at `AddUser`, contention clears on subsequent launch, save data intact across crash cycles. If `AddUser` hangs indefinitely after "Stop Sync" or on subsequent launches → **BUG REPRO (61456073)**. **[TERMINATE]** depends on automation investigation.

---

---

### ID100. Bug Regression — Suspend Online → Go Offline → Resume Hang (Bug 61557570)

1. Run Common Account Reset (Xbox).
2. **Device A (Console) — Launch and Sync Online**:
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
    - `PFGameSaveFilesAddUserWithUiAsync` — sync completes online
    - `PFGameSaveFilesGetFolder`
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/payload.bin`, bytes: 10240, pattern: [0xAA, 0xBB, 0xCC, 0xDD]
    - `CaptureSaveContainerSnapshot` (slot: left)
3. **Device A — Suspend, Go Offline, Resume**:
    - **[SUSPEND]** — trigger PLM suspend
    - While suspended, **[DISCONNECT]** — go offline (Settings → Network → Go offline, or managed switch)
    - **[RESUME]** — resume the app
    - Verify: PlayFab/GRTS prompt appears offering "Retry" or "Use Offline" (or similar network error dialog)
    - `PFGameSaveFilesSetUiSyncFailureAutoResponse` (enable: true, action: UseOffline) — or equivalent auto-response
    - Verify: game resumes within 60 seconds — no hang at "Resuming" screen or AddUser
    - Verify: save data from before suspend is accessible and correct
4. **Device A — Go Online and Upload**:
    - **[RECONNECT]** — go back online
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
    - Wait 3–5 minutes for background upload
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
    - Verify save data eventually syncs
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
6. *Key assertions*: No hang at AddUser after selecting "Use Offline", save data accessible, eventual upload after reconnect. If game hangs indefinitely at resume → **BUG REPRO (61557570)**. **[SUSPEND]**, **[RESUME]**, **[DISCONNECT]**, **[RECONNECT]** depend on automation investigation.

---

---

### ID101. Bug Regression — Uninitialize During OS Termination (Bug 61444916)

1. Run Common Account Reset (Xbox).
2. **Device A (Console) — Large Save Write**:
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
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/large-save.bin`, bytes: 10485760 (10 MB), pattern: [0xAA, 0xBB, 0xCC, 0xDD] — large write to extend uninitialize time
3. **Device A — Quit Then Evict While Uninitializing**:
    - Immediately initiate app quit:
      - `PFLocalUserCloseHandle`
      - `XUserCloseHandle`
      - `PFGameSaveFilesUninitializeAsync` — starts save flush
    - While uninitialize is still running (within 1–2 seconds), **[EVICT-GAME]** — launch another title to force OS termination
    - Goal: OS terminates the game while `PFGameSaveFilesUninitializeAsync` is still running
    - Wait 2 minutes for all processes to settle
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
    - `PFGameSaveFilesAddUserWithUiAsync`
    - Verify: game launches normally — no indefinite hang at splash screen (HANG_QUIESCE)
    - Verify: no crash or error dialog on launch
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot` (slot: left)
    - Verify: save data is either intact (uninitialize completed) or cleanly rolled back (no corruption)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
5. **Wait**: 3–5 minutes for background upload.
6. **Device B — Verify**:
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
    - Verify data is consistent
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
7. *Key assertions*: No indefinite hang (HANG_QUIESCE), no crash, data either intact or cleanly rolled back. If game hangs on relaunch or save is corrupted → **BUG REPRO (61444916)**. **[EVICT-GAME]** depends on automation investigation.

---

---

### ID102. Bug Regression — TCUI Dialog Stuck After Eviction Mid-Sync (Bug 61173408)

1. Run Common Account Reset (Xbox).
2. **Device A (Console) — Go Offline and Launch**:
    - **[DISCONNECT]** — go offline
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
    - `PFGameSaveFilesAddUserWithUiAsync` — sync attempt fails; sync failure TCUI dialog appears
    - While the sync failure dialog is displayed on screen, **[EVICT-GAME]** — launch another title to evict the game
    - Wait 30–60 seconds for eviction to complete
3. **Device A — Verify No Stuck TCUI**:
    - Return to Home screen (Xbox button)
    - Verify: no stuck TCUI dialog remains visible on screen after eviction
    - Verify: no flashing/flickering dialog loop (rapid show→dismiss→show cycle)
    - Verify: Home screen and Guide are responsive — no UI soft-lock
4. **Device A — Reconnect and Relaunch**:
    - **[RECONNECT]** — go back online
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
    - Verify: game syncs normally on relaunch — no residual TCUI error state from the interrupted dialog
    - Verify: no rapid-fire sync failure dialogs (should be at most 1 sync attempt, not dozens)
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot`
    - Verify save data loads correctly
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
5. *Key assertions*: No residual TCUI after eviction, no rapid-fire dialog loops, clean sync on relaunch. If stuck/flashing dialogs appear or sync failure repeats in loop → **BUG REPRO (61173408)**. **[DISCONNECT]**, **[RECONNECT]**, **[EVICT-GAME]** depend on automation investigation.


# Section C: Conflict Resolution — Scenario Details

> Common init/cleanup/reset blocks are in [`_common-blocks.md`](./_common-blocks.md).

---

### ID14. Conflict Resolution — Local Wins via UI Callback

1. Run Common Account Reset (Xbox).
2. **Device A — Seed Cloud with Baseline**:
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
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `slotA/character.sav`, content: "CLOUD_BASELINE"
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
3. **Device B — Download Baseline and Create Divergent Local Data**:
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
    - `PFGameSaveFilesAddUserWithUiAsync` — downloads "CLOUD_BASELINE"
    - `PFGameSaveFilesGetFolder`
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `slotA/character.sav`, content: "LOCAL_OVERRIDE" (overwrites baseline)
    - Do NOT upload — keep session alive with divergent local state.
4. **Device A — Advance Cloud State to Create Conflict**:
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
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `slotA/character.sav`, content: "CLOUD_UPDATED"
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
5. **Device B — Trigger Conflict and Choose Local**:
    - `PFGameSaveFilesAddUserWithUiAsync` — conflict callback fires (local has "LOCAL_OVERRIDE", cloud has "CLOUD_UPDATED")
    - Respond with `PFGameSaveFilesSetUiConflictResolutionResponse` (action: UseLocal)
    - Verify resolution HRESULT indicates conflict-success
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot` — verify `slotA/character.sav` contains "LOCAL_OVERRIDE"
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
6. **Device A — Verify Local Won**:
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
    - `CaptureSaveContainerSnapshot` — verify `slotA/character.sav` contains "LOCAL_OVERRIDE"
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
7. *Key assertions*: Conflict callback fires once on Device B with expected folder list, UseLocal resolves correctly, `slotA/character.sav` contains "LOCAL_OVERRIDE" on both devices after resolution.

---

---

### ID15. Conflict Resolution — Cloud Wins via UI Callback

1. Run Common Account Reset (Xbox).
2. **Device A — Seed Cloud with Baseline**:
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
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `slotA/character.sav`, content: "CLOUD_BASELINE"
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
3. **Device B — Download Baseline and Create Divergent Local Data**:
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
    - `PFGameSaveFilesAddUserWithUiAsync` — downloads "CLOUD_BASELINE"
    - `PFGameSaveFilesGetFolder`
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `slotA/character.sav`, content: "LOCAL_OVERRIDE" (overwrites baseline)
    - Do NOT upload — keep session alive with divergent local state.
4. **Device A — Advance Cloud State to Create Conflict**:
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
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `slotA/character.sav`, content: "CLOUD_UPDATED"
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
5. **Device B — Trigger Conflict and Choose Cloud**:
    - `PFGameSaveFilesAddUserWithUiAsync` — conflict callback fires (local has "LOCAL_OVERRIDE", cloud has "CLOUD_UPDATED")
    - Respond with `PFGameSaveFilesSetUiConflictResolutionResponse` (action: UseCloud)
    - Verify resolution HRESULT indicates conflict-success
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot` — verify `slotA/character.sav` contains "CLOUD_UPDATED" (local "LOCAL_OVERRIDE" is discarded)
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
6. **Device A — Verify Cloud Won**:
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
    - `CaptureSaveContainerSnapshot` — verify `slotA/character.sav` contains "CLOUD_UPDATED"
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
7. *Key assertions*: Conflict callback fires once on Device B, UseCloud resolves correctly, `slotA/character.sav` contains "CLOUD_UPDATED" (not "LOCAL_OVERRIDE") on both devices. Discarded local data does not resurface.

---

---

### ID16. Conflict Resolution — Play Offline

1. Run Common Account Reset (Xbox).
2. **Device A — Seed Cloud with Baseline**:
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
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `slotA/character.sav`, content: "CLOUD_BASELINE"
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
3. **Device B — Download Baseline and Create Divergent Local Data**:
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
    - `PFGameSaveFilesAddUserWithUiAsync` — downloads "CLOUD_BASELINE"
    - `PFGameSaveFilesGetFolder`
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `slotA/character.sav`, content: "LOCAL_OFFLINE_DATA" (overwrites baseline)
    - Do NOT upload — keep session alive with divergent local state.
4. **Device A — Advance Cloud State to Create Conflict**:
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
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `slotA/character.sav`, content: "CLOUD_UPDATED"
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
5. **Device B — Trigger Conflict and Dismiss / Play Offline**:
    - `PFGameSaveFilesAddUserWithUiAsync` — conflict callback fires (local has "LOCAL_OFFLINE_DATA", cloud has "CLOUD_UPDATED")
    - Respond with `PFGameSaveFilesSetUiConflictResolutionResponse` (action: PlayOffline / Cancel / dismiss dialog)
    - Verify result indicates offline mode
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot` — verify `slotA/character.sav` contains "LOCAL_OFFLINE_DATA" (cloud data NOT merged in)
    - Verify cloud data "CLOUD_UPDATED" is NOT present in the local save
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `slotA/character.sav`, content: "LOCAL_OFFLINE_DATA_V2" (additional offline progress)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
6. **Device B — Relaunch and Verify Conflict Re-Prompts**:
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
    - `PFGameSaveFilesAddUserWithUiAsync` — conflict should re-prompt (local still diverges from cloud)
    - Verify conflict callback fires again
    - Respond with `PFGameSaveFilesSetUiConflictResolutionResponse` (action: UseLocal) — resolve this time
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot` — verify `slotA/character.sav` contains "LOCAL_OFFLINE_DATA_V2"
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
7. *Key assertions*: Dismissing the conflict dialog enters offline mode with local-only data. Cloud data is not mixed in. On next online launch, the conflict re-prompts. Eventual resolution (UseLocal) works and data propagates correctly.

---

---

---

### ID17. Stock TCUI — Conflict Resolution

1. Run Common Account Reset (Xbox).
2. **Setup Conflict** (same as Scenario ID14 setup):
    - Device A uploads with pattern [0xAA, 0xBB, 0xCC, 0xDD], captures snapshot (slot: left)
    - Device B creates divergent data offline with pattern [0x11, 0x22, 0x33, 0x44]
3. **Device B — Reconnect and Resolve Without Custom Callbacks**:
    - **[RECONNECT]** Device B
    - `XGameRuntimeInitialize`
    - `PFInitialize`
    - `PFServicesInitialize`
    - `PFServiceConfigCreateHandle` (endpoint: `https://E18D7.playfabapi.com`, titleId: `E18D7`)
    - `XTaskQueueCreate` (workMode: ThreadPool, completionMode: ThreadPool, setAsProcessQueue: true)
    - `PFGameSaveFilesInitialize`
    - **Do NOT call `PFGameSaveFilesSetUiCallbacks`** — stock TCUI mode
    - `PFGameSaveFilesSetActiveDeviceChangedCallback`
    - `PFGameSaveFilesSetUiSyncConflictAutoResponse` (enable: true, action: UseLocal) — auto-response in stock TCUI mode
    - `XUserAddAsync`
    - `PFLocalUserCreateHandleWithXboxUser`
    - `PFGameSaveFilesAddUserWithUiAsync` — stock TCUI conflict dialog appears; auto-responds UseLocal
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot` (slot: right)
    - Verify local data wins (pattern [0x11, 0x22, 0x33, 0x44])
    - Full cleanup
4. *Key assertions*: Auto-response works in stock TCUI mode for conflict resolution, correct data wins, no crash.

---

---

### ID18. Multi-Folder Merge — No Conflict

**Priority**: P1 | **Category**: Atomic Unit Conflict Granularity | **Devices**: 2

**Scenario**: Device A modifies files only inside `Save1/`. Device B modifies files only inside `Save2/`. Both upload. On the next sync, both changes merge automatically — no conflict dialog fires.

**Why This Matters**: The docs state that each root-level subfolder is an independent "atomic unit" for conflict detection. Changes in different atomic units MUST NOT trigger a conflict. A false positive here blocks the player unnecessarily.

**Setup**: Two consoles/PCs. Both devices start with a clean synced state containing `Save1/file.dat` and `Save2/file.dat`.

1. **Device A — Init and Upload Changes to Save1/**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Wait for progress callback → sync completes
    - `PFGameSaveFilesGetFolder` → confirm `Save1/file.dat` and `Save2/file.dat` exist
    - Write modified data to `Save1/file.dat` (e.g., append "DeviceA_Save1")
    - Do NOT modify anything in `Save2/`
    - `PFGameSaveFilesUploadWithUiAsync` → `{}`
    - Wait for upload complete
    - `PFGameSaveFilesUninitialize` → `{}`
    - `PFCleanupAsync`

2. **Device B — Init and Upload Changes to Save2/**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Wait for progress callback → sync completes (Device B still has OLD data — it doesn't see Device A's upload yet because it already synced before Device A uploaded)
    - Write modified data to `Save2/file.dat` (e.g., append "DeviceB_Save2")
    - Do NOT modify anything in `Save1/`
    - `PFGameSaveFilesUploadWithUiAsync` → `{}`
    - Wait for upload — this triggers sync with Device A's changes
    - **KEY CHECK**: No conflict callback fires — changes are in different atomic units
    - `CaptureSaveContainerSnapshot` → snapshot after merged sync
    - `PFGameSaveFilesUninitialize` → `{}`
    - `PFCleanupAsync`

3. **Device A — Re-sync and Verify Merge**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Wait for progress callback → sync completes
    - `CaptureSaveContainerSnapshot` → snapshot after merge
    - `PFGameSaveFilesUninitialize` → `{}`
    - `PFCleanupAsync`

4. *Key assertions*:
    - No conflict callback fires on either device during upload/sync
    - Device A snapshot: `Save1/file.dat` = Device A's modification, `Save2/file.dat` = Device B's modification
    - Device B snapshot: same — both changes present
    - No data loss in either folder

---

---

### ID19. Same-Subfolder Conflict — Different Files

**Priority**: P1 | **Category**: Atomic Unit Conflict Granularity | **Devices**: 2

**Scenario**: Device A modifies `Save1/stats.json`. Device B modifies `Save1/inventory.json`. Both files are different, but they share the same atomic unit (root subfolder `Save1/`). On sync, a conflict IS triggered.

**Why This Matters**: Conflict detection is per-atomic-unit (subfolder), NOT per-file. This is counter-intuitive behavior. Modifying different files in the same subfolder still triggers a conflict.

**Setup**: Two consoles/PCs. Both devices start with a clean synced state containing `Save1/stats.json` and `Save1/inventory.json`.

1. **Device A — Init, Modify stats.json, Upload**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Wait for progress callback → sync completes
    - `PFGameSaveFilesGetFolder` → confirm `Save1/stats.json` and `Save1/inventory.json` exist
    - Write modified data to `Save1/stats.json` (e.g., set `"wins": 10`)
    - Do NOT modify `Save1/inventory.json`
    - `PFGameSaveFilesUploadWithUiAsync` → `{}`
    - Wait for upload complete
    - `PFGameSaveFilesUninitialize` → `{}`
    - `PFCleanupAsync`

2. **Device B — Init, Modify inventory.json, Upload → Expect Conflict**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Wait for progress callback → sync completes (Device B has OLD cloud data)
    - Write modified data to `Save1/inventory.json` (e.g., add `"sword": true`)
    - Do NOT modify `Save1/stats.json`
    - `PFGameSaveFilesUploadWithUiAsync` → `{}`
    - **KEY CHECK**: Conflict callback fires — different files, but same atomic unit `Save1/`
    - `PFGameSaveFilesSetUiSyncConflictResponse` → `{ "response": "UseLocal" }`
    - Wait for upload complete
    - `CaptureSaveContainerSnapshot`
    - `PFGameSaveFilesUninitialize` → `{}`
    - `PFCleanupAsync`

3. *Key assertions*:
    - Conflict callback DOES fire on Device B (same subfolder = same atomic unit)
    - After "Keep Local": `Save1/inventory.json` = Device B's modification, `Save1/stats.json` = Device B's original (pre-Device-A-change) version
    - Device A's change to `stats.json` is LOST — this is the expected all-or-nothing behavior within an atomic unit

---

---

### ID20. Root-Level File Conflict

**Priority**: P1 | **Category**: Atomic Unit Conflict Granularity | **Devices**: 2

**Scenario**: Device A modifies `settings.json` (at save root). Device B modifies `progress.json` (also at save root, different file). On sync, a conflict IS triggered because all root-level files share one atomic unit.

**Why This Matters**: Root-level files are a special case in the conflict model — they ALL share a single atomic unit. Changing any root-level file conflicts with any other root-level file change. Games should organize saves into subfolders to avoid unnecessary conflicts.

**Setup**: Two consoles/PCs. Both devices start synced with `settings.json` and `progress.json` at the save root.

1. **Device A — Modify settings.json, Upload**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Wait for progress callback → sync completes
    - Write modified data to `settings.json` (e.g., `"volume": 80`)
    - Do NOT modify `progress.json`
    - `PFGameSaveFilesUploadWithUiAsync` → `{}`
    - Wait for upload complete
    - `PFGameSaveFilesUninitialize` → `{}`
    - `PFCleanupAsync`

2. **Device B — Modify progress.json, Upload → Expect Conflict**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Wait for progress callback → sync completes (old cloud state)
    - Write modified data to `progress.json` (e.g., `"level": 5`)
    - Do NOT modify `settings.json`
    - `PFGameSaveFilesUploadWithUiAsync` → `{}`
    - **KEY CHECK**: Conflict callback fires — different root-level files, but all root files share ONE atomic unit
    - `PFGameSaveFilesSetUiSyncConflictResponse` → `{ "response": "UseCloud" }`
    - Wait for upload complete
    - `CaptureSaveContainerSnapshot`
    - `PFGameSaveFilesUninitialize` → `{}`
    - `PFCleanupAsync`

3. *Key assertions*:
    - Conflict callback fires on Device B
    - After "Keep Cloud": `settings.json` = Device A's modification (`"volume": 80`), `progress.json` = cloud version (Device B's change LOST)
    - This confirms all root files = one atomic unit

---

---

### ID21. All-or-Nothing Conflict Resolution — Keep Local Loses Cloud-Only Changes

**Priority**: P1 | **Category**: Atomic Unit Conflict Granularity | **Devices**: 2

**Scenario**: Device A modifies `SlotA/` and `SlotC/`. Device B modifies `SlotA/` and `SlotB/`. Conflict on `SlotA/` (both modified). User chooses "Keep Local". Verify: SlotA = Device B local data, SlotB = Device B local data uploaded, but SlotC = Device B's old copy (Device A's cloud-only change to SlotC is LOST).

**Why This Matters**: This is the documented all-or-nothing warning: "Choosing Keep Local discards cloud changes in non-conflicting atomic units." This is a data-loss risk that games MUST understand.

**Setup**: Two consoles/PCs. Both start synced with `SlotA/save.dat`, `SlotB/save.dat`, and `SlotC/save.dat`.

1. **Device A — Modify SlotA/ and SlotC/, Upload**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Wait for progress callback → sync completes
    - Write modified data to `SlotA/save.dat` (e.g., "DeviceA_SlotA_v2")
    - Write modified data to `SlotC/save.dat` (e.g., "DeviceA_SlotC_v2")
    - Do NOT modify `SlotB/`
    - `PFGameSaveFilesUploadWithUiAsync` → `{}`
    - Wait for upload complete
    - `PFGameSaveFilesUninitialize` → `{}`
    - `PFCleanupAsync`

2. **Device B — Modify SlotA/ and SlotB/, Upload → Conflict → Keep Local**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Wait for progress callback → sync completes (old cloud state — before Device A's upload)
    - Write modified data to `SlotA/save.dat` (e.g., "DeviceB_SlotA_v2")
    - Write modified data to `SlotB/save.dat` (e.g., "DeviceB_SlotB_v2")
    - Do NOT modify `SlotC/`
    - `PFGameSaveFilesUploadWithUiAsync` → `{}`
    - **Conflict callback fires** (SlotA modified on both)
    - `PFGameSaveFilesSetUiSyncConflictResponse` → `{ "response": "UseLocal" }`
    - Wait for upload complete
    - `CaptureSaveContainerSnapshot` → snapshot after resolution
    - `PFGameSaveFilesUninitialize` → `{}`
    - `PFCleanupAsync`

3. **Verify on Device A — Re-sync**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Wait for progress callback → sync completes
    - `CaptureSaveContainerSnapshot`
    - `PFGameSaveFilesUninitialize` → `{}`
    - `PFCleanupAsync`

4. *Key assertions*:
    - `SlotA/save.dat` = "DeviceB_SlotA_v2" (Device B's local version kept — conflict resolved)
    - `SlotB/save.dat` = "DeviceB_SlotB_v2" (Device B's local change — no conflict)
    - `SlotC/save.dat` = old baseline version (Device A's cloud-only change is **LOST** — this is the all-or-nothing behavior)
    - Both devices converge on the same final state

---

---

### ID22. All-or-Nothing Conflict Resolution — Keep Cloud Loses Local-Only Changes

**Priority**: P1 | **Category**: Atomic Unit Conflict Granularity | **Devices**: 2

**Scenario**: Same mixed setup as Scenario ID21. User chooses "Keep Cloud" instead. Verify: SlotA = Device A cloud data, SlotC = Device A cloud data, but SlotB = old baseline (Device B's local-only change LOST).

**Why This Matters**: Mirrors Scenario ID21 for the opposite resolution path. "Keep Cloud" discards local-only changes in non-conflicting atomic units.

**Setup**: Two consoles/PCs. Both start synced with `SlotA/save.dat`, `SlotB/save.dat`, and `SlotC/save.dat`.

1. **Device A — Modify SlotA/ and SlotC/, Upload**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Wait for progress callback → sync completes
    - Write modified data to `SlotA/save.dat` (e.g., "DeviceA_SlotA_v2")
    - Write modified data to `SlotC/save.dat` (e.g., "DeviceA_SlotC_v2")
    - Do NOT modify `SlotB/`
    - `PFGameSaveFilesUploadWithUiAsync` → `{}`
    - Wait for upload complete
    - `PFGameSaveFilesUninitialize` → `{}`
    - `PFCleanupAsync`

2. **Device B — Modify SlotA/ and SlotB/, Upload → Conflict → Keep Cloud**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Wait for progress callback → sync completes (old cloud state)
    - Write modified data to `SlotA/save.dat` (e.g., "DeviceB_SlotA_v2")
    - Write modified data to `SlotB/save.dat` (e.g., "DeviceB_SlotB_v2")
    - Do NOT modify `SlotC/`
    - `PFGameSaveFilesUploadWithUiAsync` → `{}`
    - **Conflict callback fires** (SlotA modified on both)
    - `PFGameSaveFilesSetUiSyncConflictResponse` → `{ "response": "UseCloud" }`
    - Wait for upload/sync complete
    - `CaptureSaveContainerSnapshot` → snapshot after resolution
    - `PFGameSaveFilesUninitialize` → `{}`
    - `PFCleanupAsync`

3. **Verify on Device A — Re-sync**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Wait for progress callback → sync completes
    - `CaptureSaveContainerSnapshot`
    - `PFGameSaveFilesUninitialize` → `{}`
    - `PFCleanupAsync`

4. *Key assertions*:
    - `SlotA/save.dat` = "DeviceA_SlotA_v2" (cloud version kept — Device A's change)
    - `SlotC/save.dat` = "DeviceA_SlotC_v2" (cloud version — Device A's change, no conflict on SlotC)
    - `SlotB/save.dat` = old baseline (Device B's local-only change is **LOST** — all-or-nothing)
    - Both devices converge on the same final state

---

---

### ID23. Delete on Both Sides — No Conflict

**Priority**: P2 | **Category**: Atomic Unit Conflict Granularity | **Devices**: 2

**Scenario**: Device A deletes `Save1/old-backup.dat`. Device B also deletes `Save1/old-backup.dat`. On sync, no conflict occurs — both devices agree the file should be removed.

**Why This Matters**: Documented behavior: "If both sides delete the same data, this is not treated as a conflict." Ensures silent merge for agreement-deletes.

**Setup**: Two consoles/PCs. Both start synced with `Save1/old-backup.dat` and `Save1/main.dat`.

1. **Device A — Delete File, Upload**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Wait for progress callback → sync completes
    - `PFGameSaveFilesGetFolder` → confirm `Save1/old-backup.dat` exists
    - Delete `Save1/old-backup.dat` from the save folder
    - `PFGameSaveFilesUploadWithUiAsync` → `{}`
    - Wait for upload complete
    - `PFGameSaveFilesUninitialize` → `{}`
    - `PFCleanupAsync`

2. **Device B — Delete Same File, Upload → Expect No Conflict**:
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Wait for progress callback → sync completes (Device B still has old local state, hasn't seen Device A's upload)
    - Delete `Save1/old-backup.dat` from the save folder (same file Device A deleted)
    - `PFGameSaveFilesUploadWithUiAsync` → `{}`
    - **KEY CHECK**: No conflict callback fires — both sides deleted the same file
    - Wait for upload complete
    - `CaptureSaveContainerSnapshot`
    - `PFGameSaveFilesUninitialize` → `{}`
    - `PFCleanupAsync`

3. *Key assertions*:
    - No conflict callback fires on Device B
    - `Save1/old-backup.dat` is absent from final snapshot
    - `Save1/main.dat` still present and unmodified
    - Both devices converge to the same state

---

---

### ID24. Multi-Callback Sequence — Contention then Conflict

**Priority**: P2 | **Category**: Conflict Resolution | **Devices**: 2

**Scenario**: Set up a situation where BOTH contention AND conflict fire in the same sync operation. Device B holds the lock AND has divergent data. Device A launches — first the contention callback fires (Device B holds lock), then after contention resolves, the conflict callback fires (divergent data detected). Verify both callbacks fire in sequence and both resolutions apply correctly.

**Why This Matters**: Games must handle back-to-back callbacks in a single `AddUserWithUiAsync` call. If the system only expects one callback per operation, the second may be silently dropped.

1. **Setup — Device B Holds Lock with Data**:
    - Device B: Init, write `save.dat` with "DeviceB_v1", upload with `KeepDeviceActive`
    - Device B stays initialized (holds lock)

2. **Device A — Create Divergent Local Data**:
    - Device A: Init, write `save.dat` with "DeviceA_divergent", upload with `ReleaseDeviceAsActive`
    - Device A: Uninit, then re-init WITHOUT `DeleteSaveRoot` (local data stays)
    - Device A now has local data that diverges from Device B's cloud data

3. **Device A — Launch with Contention + Conflict**:
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - **FIRST**: Contention callback fires (Device B holds lock)
    - `PFGameSaveFilesSetUiActiveDeviceContentionResponse` → `{ "response": "SyncLastSavedData" }`
    - **SECOND**: Conflict callback fires (local data ≠ cloud data)
    - `PFGameSaveFilesSetUiConflictResponse` → `{ "response": "UseLocal" }`
    - Wait for sync complete

4. **Verify**:
    - `CaptureSaveContainerSnapshot` on Device A
    - Cleanup both devices

5. *Key assertions*:
    - Both contention and conflict callbacks fire in sequence (not just one)
    - Contention resolves first, then conflict
    - Final data reflects the conflict resolution choice (UseLocal = Device A's data)
    - No hang between callbacks


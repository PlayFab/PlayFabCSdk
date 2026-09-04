# Section U: Cross-Platform — Xbox GRTS and In-Proc — Scenario Details

> Common init/cleanup/reset blocks are in [`_common-blocks.md`](./_common-blocks.md).

---

### ID108. Cross-Platform Golden Path — In-Proc to GRTS Bidirectional Sync

1. Run Common Account Reset for the shared PlayFab user on both platforms.
2. **Phase 1 — PC (In-Proc) Uploads**:
    - Device A (PC): Standard in-proc init (`PFLocalUserCreateHandleWithPersistedLocalId` or entity auth)
    - `PFGameSaveFilesAddUserWithUiAsync`
    - `PFGameSaveFilesGetFolder`
    - `DeleteSaveRoot`
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `crossplat/payload.bin`, bytes: 10240, pattern: [0xCC, 0xDD, 0xEE, 0xFF]
    - `PFGameSaveFilesSetSaveDescriptionAsync` (description: "PC In-Proc Upload")
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `CaptureSaveContainerSnapshot` (slot: left)
    - Cleanup Device A
3. **Phase 1 — Xbox (GRTS) Downloads**:
    - Device B (Xbox): Xbox Init Block
    - `PFGameSaveFilesSetUiActiveDeviceContentionAutoResponse` (SyncLastSavedData)
    - `PFGameSaveFilesAddUserWithUiAsync`
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot` (slot: right)
    - `CompareSaveContainerSnapshots` — expect MATCH
    - `PFGameSaveFilesGetSaveDescription` — expect "PC In-Proc Upload"
4. **Phase 2 — Xbox (GRTS) Modifies and Uploads**:
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `crossplat/payload.bin`, bytes: 10240, pattern: [0x11, 0x22, 0x33, 0x44]
    - `PFGameSaveFilesSetSaveDescriptionAsync` (description: "Xbox GRTS Upload")
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `CaptureSaveContainerSnapshot` (slot: left)
    - Cleanup Device B
5. **Phase 2 — PC (In-Proc) Re-Syncs**:
    - Device A (PC): Re-init
    - `PFGameSaveFilesAddUserWithUiAsync`
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot` (slot: right)
    - `CompareSaveContainerSnapshots` — expect MATCH
    - `PFGameSaveFilesGetSaveDescription` — expect "Xbox GRTS Upload"
    - Cleanup Device A
6. *Key assertions*: Snapshot match both directions, save descriptions propagate across providers, no format errors.

---

### ID109. Cross-Platform Active Device Contention

1. Run Common Account Reset for shared PlayFab user.
2. **Direction 1 — Xbox Holds Lock, PC Requests**:
    - Device A (Xbox): Xbox Init Block
    - `PFGameSaveFilesAddUserWithUiAsync`
    - `WriteGameSaveData` — pattern: [0xAA]
    - `PFGameSaveFilesUploadWithUiAsync` (mode: KeepDeviceActive) — Xbox holds lock
    - Device B (PC): In-proc init
    - `PFGameSaveFilesAddUserWithUiAsync` — contention fires (Xbox holds lock)
    - Inspect contention callback `PFGameSaveDescriptor` — verify `deviceType` shows Xbox device
    - Respond with `SyncLastSavedData`
    - Verify Device A (Xbox) active device changed callback fires
    - Device B (PC) uploads modified data
    - Cleanup both
3. **Direction 2 — PC Holds Lock, Xbox Requests**:
    - Device B (PC): Init and upload with KeepDeviceActive
    - Device A (Xbox): Init, `PFGameSaveFilesAddUserWithUiAsync` — contention fires
    - Inspect `deviceType` — verify it shows PC device
    - Respond with `SyncLastSavedData`
    - Verify Device B (PC) active device changed callback fires
    - Cleanup both
4. *Key assertions*: Contention works in both cross-provider directions, `deviceType` reflects actual platform, lock handoff clean.

---

### ID110. Cross-Platform Conflict Resolution

1. Run Common Account Reset for shared PlayFab user.
2. **Setup Divergent Data**:
    - Device A (Xbox, GRTS): Upload baseline — pattern [0xAA] in `save/data.bin`
    - Device A: `ReleaseDeviceAsActive`, cleanup
    - Device B (PC, in-proc): Go offline (use `ConfigureHttpMock` for in-proc). Write divergent data — pattern [0xBB] in `save/data.bin`
3. **Trigger Conflict — UseLocal (PC Wins)**:
    - Device B (PC): Reconnect
    - `PFGameSaveFilesAddUserWithUiAsync` — conflict fires (local=0xBB, cloud=0xAA)
    - Respond with `UseLocal`
    - Upload resolved state
    - Device A (Xbox): Re-init, sync. Verify data = pattern [0xBB] (PC's local won)
4. **Reset and Trigger Conflict — UseCloud (Xbox Wins)**:
    - Re-seed: Xbox uploads [0xCC], PC creates divergent [0xDD] offline
    - Device B (PC): Reconnect, conflict fires
    - Respond with `UseCloud`
    - Device B (PC): Verify local data replaced with [0xCC]
5. *Key assertions*: Conflict detected across providers, both resolution paths work, resolved state readable by both.

---

### ID111. GRTS Background Upload then In-Proc Download

1. Run Common Account Reset for shared PlayFab user.
2. **Device A (Xbox, GRTS) — Write and Close**:
    - Xbox Init Block
    - `PFGameSaveFilesAddUserWithUiAsync`
    - `PFGameSaveFilesGetFolder`
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `bgsync/payload.bin`, bytes: 51200 (50 KB), pattern: [0xDE, 0xAD, 0xBE, 0xEF]
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `CaptureSaveContainerSnapshot` (slot: left)
    - Close the app — GRTS background upload begins
    - **[TERMINATE]**: `xbapp terminate $packageName`
3. **Wait for Background Upload**:
    - `Start-Sleep -Seconds 180` (3 minutes for GRTS to complete background upload)
4. **Device B (PC, In-Proc) — Download and Verify**:
    - Standard in-proc init
    - `PFGameSaveFilesAddUserWithUiAsync`
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot` (slot: right)
    - `CompareSaveContainerSnapshots` — expect MATCH
5. Cleanup.
6. *Key assertions*: GRTS background upload completed after game exit, in-proc device received data intact, snapshot match, no contention.

---

### ID112. Cross-Platform Rollback Compatibility

1. Run Common Account Reset for shared PlayFab user.
2. **Create Cross-Provider Conflict**:
    - Device A (Xbox, GRTS): Upload baseline — pattern [0xAA]
    - Device A: ReleaseDeviceAsActive, cleanup
    - Device B (PC, in-proc): Go offline, write divergent — pattern [0xBB]
    - Device B: Reconnect, conflict fires, choose `UseLocal` (PC wins)
    - Device B: Upload resolved state (cloud now has PC's [0xBB])
3. **Rollback on Xbox (Losing Side)**:
    - Device A (Xbox): Re-init with `RollbackToLastConflict` flag
    - `PFGameSaveFilesAddUserWithUiAsync` (with rollback)
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot` (slot: right)
    - Verify restored data matches Xbox's pre-conflict state — pattern [0xAA]
4. Cleanup.
5. *Key assertions*: Rollback works when the conflict was resolved by a different provider, restored data correct, no metadata format errors.


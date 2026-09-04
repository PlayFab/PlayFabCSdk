# Section T: Multi-User Xbox — Scenario Details

> Common init/cleanup/reset blocks are in [`_common-blocks.md`](./_common-blocks.md).

---

### ID104. Two Users Active Simultaneously — Data Isolation

1. Run Common Account Reset (Xbox) for **User A**.
2. Run Common Account Reset (Xbox) for **User B**.
3. **User A — Initialize and Upload**:
    - Xbox Init Block (using User A credentials)
    - `PFGameSaveFilesAddUserWithUiAsync`
    - `PFGameSaveFilesGetFolder`
    - `DeleteSaveRoot`
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/payload.bin`, bytes: 10240, pattern: [0xAA, 0xAA, 0xAA, 0xAA]
    - `PFGameSaveFilesUploadWithUiAsync` (mode: KeepDeviceActive)
    - `CaptureSaveContainerSnapshot` (slot: left, label: "User A data")
4. **User B — Initialize and Upload (without uninitializing User A)**:
    - `XUserAddAsync` (User B credentials)
    - `PFLocalUserCreateHandleWithXboxUser` (User B)
    - `PFGameSaveFilesAddUserWithUiAsync` (User B handle)
    - `PFGameSaveFilesGetFolder` (User B) — verify path differs from User A's folder
    - `DeleteSaveRoot`
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/payload.bin`, bytes: 10240, pattern: [0xBB, 0xBB, 0xBB, 0xBB]
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - `CaptureSaveContainerSnapshot` (slot: right, label: "User B data")
5. **Verify Isolation**:
    - `CompareSaveContainerSnapshots` — expect MISMATCH (User A and User B wrote different patterns)
    - Verify User A's folder still contains pattern [0xAA] (no bleed from User B)
    - Verify User B's folder contains pattern [0xBB] (no bleed from User A)
6. Cleanup both users.
7. *Key assertions*: Both AddUser succeed, separate save folders, no cross-user data bleed, no unexpected callbacks fire on User A when User B initializes.

---

### ID105. Second User — No Cross-User Contention

1. Run Common Account Reset (Xbox) for **User A** and **User B**.
2. **User A — Initialize and Hold Lock**:
    - Xbox Init Block (User A)
    - `PFGameSaveFilesAddUserWithUiAsync`
    - `PFGameSaveFilesGetFolder`
    - `WriteGameSaveData` — relativePath: `save/data.bin`, bytes: 1024, pattern: [0xAA]
    - `PFGameSaveFilesUploadWithUiAsync` (mode: KeepDeviceActive) — User A holds lock
    - Register active device changed callback assertion: expect it does NOT fire
3. **User B — Initialize on Same Console**:
    - `XUserAddAsync` (User B)
    - `PFLocalUserCreateHandleWithXboxUser` (User B)
    - `PFGameSaveFilesAddUserWithUiAsync` (User B) — should succeed immediately, NO contention
    - `PFGameSaveFilesGetFolder` (User B)
    - `WriteGameSaveData` — relativePath: `save/data.bin`, bytes: 1024, pattern: [0xBB]
    - `PFGameSaveFilesUploadWithUiAsync` (mode: KeepDeviceActive) — User B holds its own lock
4. **Verify Independence**:
    - User A's active device changed callback did NOT fire
    - User A can still call `PFGameSaveFilesIsConnectedToCloud` — returns `true`
    - User B's `IsConnectedToCloud` also returns `true`
    - Both users hold independent active device locks
5. Cleanup both users.
6. *Key assertions*: No contention between different users on same console, independent locks, no cross-user callbacks.

---

### ID106. User Switch During Active Upload

1. Run Common Account Reset (Xbox) for **User A**.
2. **User A — Write and Start Upload**:
    - Xbox Init Block (User A)
    - `PFGameSaveFilesAddUserWithUiAsync`
    - `PFGameSaveFilesGetFolder`
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `bigfile/payload.bin`, bytes: 5242880 (5 MB), pattern: [0xAA, 0xBB, 0xCC, 0xDD]
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - During progress callback (sync state = Uploading): **[SWITCH-USER]** — `xbuser signout` User A, `xbuser signin` User B
3. **Observe Upload Outcome**:
    - Record whether upload completed or returned an error HRESULT
    - Record any crash/hang behavior
4. **Recovery — Sign User A Back In**:
    - `xbuser signout` User B
    - `xbuser signin` User A
    - Re-initialize and call `PFGameSaveFilesAddUserWithUiAsync`
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot` (slot: right)
    - Verify data state is deterministic (either upload succeeded or local data intact for retry)
5. Cleanup.
6. *Key assertions*: No crash or hang, upload outcome deterministic, User A's data recoverable, no orphaned locks.

---

### ID107. Multi-User Multi-Device — Cross-User Isolation Under Contention

1. Run Common Account Reset (Xbox) for **User A** and **User B**.
2. **Console 1 — Initialize Both Users**:
    - Xbox Init Block (User A on Console 1)
    - `PFGameSaveFilesAddUserWithUiAsync` (User A)
    - `WriteGameSaveData` (User A) — pattern: [0xAA]
    - `PFGameSaveFilesUploadWithUiAsync` (User A, mode: KeepDeviceActive)
    - Initialize User B on Console 1 (separate XUser + PFLocalUser handles)
    - `PFGameSaveFilesAddUserWithUiAsync` (User B)
    - `WriteGameSaveData` (User B) — pattern: [0xBB]
    - `PFGameSaveFilesUploadWithUiAsync` (User B, mode: KeepDeviceActive)
    - Register active device changed callback on User B — expect it does NOT fire
3. **Console 2 — User A Triggers Contention**:
    - Xbox Init Block (User A on Console 2)
    - `PFGameSaveFilesSetUiActiveDeviceContentionAutoResponse` (SyncLastSavedData)
    - `PFGameSaveFilesAddUserWithUiAsync` (User A) — contention fires (Console 1 holds User A's lock)
    - Contention resolves — Console 2 takes over User A's active device
4. **Verify User B on Console 1 Unaffected**:
    - User B's active device changed callback did NOT fire
    - User B's `PFGameSaveFilesIsConnectedToCloud` returns `true`
    - User B's data unchanged (snapshot still matches pattern [0xBB])
    - User A on Console 1 received active device changed callback (expected)
5. Cleanup all.
6. *Key assertions*: User A contention works normally, User B completely unaffected, no cross-user state leakage.


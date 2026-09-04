# Section P: GRTS API Edge Cases — Scenario Details

> Common init/cleanup/reset blocks are in [`_common-blocks.md`](./_common-blocks.md).

---

### ID86. Custom File Location — Init and Sync

1. Run Common Account Reset (Xbox).
2. **Device A — Init With Custom Save Folder**:
    - `XGameRuntimeInitialize`
    - `PFInitialize`
    - `PFServicesInitialize`
    - `PFServiceConfigCreateHandle` (endpoint: `https://E18D7.playfabapi.com`, titleId: `E18D7`)
    - `XTaskQueueCreate` (workMode: ThreadPool, completionMode: ThreadPool, setAsProcessQueue: true)
    - `PFGameSaveFilesInitialize` with `saveFolder` set to a custom path (e.g., `D:\CustomSaves\TestApp\`)
    - `PFGameSaveFilesSetUiCallbacks` (enable: true)
    - `PFGameSaveFilesSetActiveDeviceChangedCallback`
    - `XUserAddAsync`
    - `PFLocalUserCreateHandleWithXboxUser`
    - `PFGameSaveFilesAddUserWithUiAsync`
    - `PFGameSaveFilesGetFolder` — verify returned path is under the custom location
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/payload.bin`, bytes: 10240, pattern: [0xAA, 0xBB, 0xCC, 0xDD]
    - Verify file exists at `D:\CustomSaves\TestApp\<user>\progress\payload.bin` (or equivalent subfolder)
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
4. **Device A — Relaunch With Same Custom Folder**:
    - Full init with same `saveFolder` path
    - `DeleteSaveRoot` — clear local custom folder
    - `PFGameSaveFilesAddUserWithUiAsync` — download into custom folder
    - `PFGameSaveFilesGetFolder` — verify path
    - `CaptureSaveContainerSnapshot` (slot: right)
    - `CompareSaveContainerSnapshots` (ignoreTimestamps: true)
    - Full cleanup
5. **Device B — Verify Cross-Device**:
    - Full init on Device B (with default save folder, not custom)
    - `PFGameSaveFilesSetUiActiveDeviceContentionAutoResponse` (enable: true, action: SyncLastSavedData)
    - `PFGameSaveFilesAddUserWithUiAsync` — download from cloud
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot`
    - Verify data from Device A is present — custom file location on Device A does not affect cloud storage
    - Full cleanup
6. *Key assertions*: Save files written to custom path, `GetFolder` returns custom location, snapshot comparison passes, cross-device sync unaffected by custom location.

---

---

### ID87. Custom File Location — Invalid Path

1. Run Common Account Reset (Xbox).
2. **Device A — Attempt Init With Invalid Path**:
    - `XGameRuntimeInitialize`
    - `PFInitialize`
    - `PFServicesInitialize`
    - `PFServiceConfigCreateHandle` (endpoint: `https://E18D7.playfabapi.com`, titleId: `E18D7`)
    - `XTaskQueueCreate` (workMode: ThreadPool, completionMode: ThreadPool, setAsProcessQueue: true)
    - `PFGameSaveFilesInitialize` with `saveFolder` set to an invalid/inaccessible path (e.g., `Z:\NonExistent\BadPath\`)
    - Verify: `PFGameSaveFilesInitialize` or `PFXGameSaveInitializeConfig` returns `E_GS_PLAYFAB_INVALID_LOCATION` (0x80832305)
    - Verify: no crash, no side effects
3. **Device A — Retry With Valid Path**:
    - `PFGameSaveFilesInitialize` with a valid `saveFolder` (or default)
    - `PFGameSaveFilesSetUiCallbacks` (enable: true)
    - `PFGameSaveFilesSetActiveDeviceChangedCallback`
    - `XUserAddAsync`
    - `PFLocalUserCreateHandleWithXboxUser`
    - `PFGameSaveFilesAddUserWithUiAsync`
    - Verify init succeeds
    - Full cleanup
4. *Key assertions*: `E_GS_PLAYFAB_INVALID_LOCATION` returned for invalid path, no crash, retry with valid path succeeds.

---

---

### ID88. ResetCloud — E_NOTIMPL Verification

1. Run Common Account Reset (Xbox).
2. **Device A — Upload Then Attempt ResetCloud**:
    - Full init on Device A
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/payload.bin`, bytes: 10240, pattern: [0xAA, 0xBB, 0xCC, 0xDD]
    - `PFGameSaveFilesUploadWithUiAsync` (mode: KeepDeviceActive)
    - `PFGameSaveFilesResetCloudAsync`
    - Verify: returns `E_NOTIMPL`
    - Verify: no side effects — data still intact
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot`
    - Verify snapshot shows data still present (ResetCloud did not destroy anything)
    - Full cleanup
3. *Key assertions*: `ResetCloud` returns `E_NOTIMPL` on GRTS, no side effects on existing data, no crash.

---

---

### ID89. GRTS Error Codes — Specific HRESULT Validation

This scenario triggers and verifies multiple GRTS error codes in a single test session.

1. Run Common Account Reset (Xbox).

#### 72a. E_PF_GAMESAVE_USER_CANCELLED (0x800704c7)
- Full init on Device A
- Seed cloud data on Device B (10 MB payload)
- On Device A, configure progress callback to Cancel during download
- `PFGameSaveFilesAddUserWithUiAsync`
- Verify: returns `E_PF_GAMESAVE_USER_CANCELLED` (0x800704c7)
- Full cleanup

#### 72b. E_GS_PLAYFAB_INVALID_LOCATION (0x80832305)
- `PFGameSaveFilesInitialize` with invalid `saveFolder` path
- Verify: returns `E_GS_PLAYFAB_INVALID_LOCATION`
- Full cleanup

#### 72c. E_PF_GAMESAVE_NOT_INITIALIZED (0x89237000)
- Attempt to call `PFGameSaveFilesAddUserWithUiAsync` without calling `PFGameSaveFilesInitialize`
- Verify: returns `E_PF_GAMESAVE_NOT_INITIALIZED`

#### 72d. E_PF_GAMESAVE_USER_NOT_ADDED (0x89237003)
- `PFGameSaveFilesInitialize`
- Attempt to call `PFGameSaveFilesGetFolder` without calling `PFGameSaveFilesAddUserWithUiAsync`
- Verify: returns `E_PF_GAMESAVE_USER_NOT_ADDED`

#### 72e. E_PF_GAMESAVE_ALREADY_INITIALIZED (0x89237001)
- `PFGameSaveFilesInitialize`
- Call `PFGameSaveFilesInitialize` again without uninitializing
- Verify: returns `E_PF_GAMESAVE_ALREADY_INITIALIZED`
- `PFGameSaveFilesUninitializeAsync`

#### 72f. E_PF_GAMESAVE_USER_ALREADY_ADDED (0x89237002)
- Full init on Device A
- Call `PFGameSaveFilesAddUserWithUiAsync` a second time with the same user
- Verify: returns `E_PF_GAMESAVE_USER_ALREADY_ADDED`
- Full cleanup

*Key assertions*: Each triggered error returns the documented HRESULT, no crash or hang in any case, no side effects from error paths.


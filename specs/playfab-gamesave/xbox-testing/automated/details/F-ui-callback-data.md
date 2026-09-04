# Section F: UI Callback Data Verification — Scenario Details

> Common init/cleanup/reset blocks are in [`_common-blocks.md`](./_common-blocks.md).

---

### ID37. Descriptor Fields — Contention Callback

1. Run Common Account Reset (Xbox).
2. **Device A — Create Save With Description and Thumbnail**:
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
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `pfthumbnail.png`, bytes: 1024, pattern: [0x89, 0x50, 0x4E, 0x47] — PNG header bytes for thumbnail
    - `PFGameSaveFilesSetSaveDescriptionAsync` (description: "Test Save — Level 5 Checkpoint")
    - `PFGameSaveFilesUploadWithUiAsync` (mode: KeepDeviceActive) — keep lock so Device B triggers contention
    - Record: expected totalBytes (~11264), expected description ("Test Save — Level 5 Checkpoint")
3. **Device B — Trigger Contention and Inspect Descriptors**:
    - `XGameRuntimeInitialize`
    - `PFInitialize`
    - `PFServicesInitialize`
    - `PFServiceConfigCreateHandle` (endpoint: `https://E18D7.playfabapi.com`, titleId: `E18D7`)
    - `XTaskQueueCreate` (workMode: ThreadPool, completionMode: ThreadPool, setAsProcessQueue: true)
    - `PFGameSaveFilesInitialize`
    - `PFGameSaveFilesSetUiCallbacks` (enable: true) — register contention callback that captures descriptors
    - `PFGameSaveFilesSetActiveDeviceChangedCallback`
    - `XUserAddAsync` — same user
    - `PFLocalUserCreateHandleWithXboxUser`
    - `DeleteSaveRoot`
    - `PFGameSaveFilesAddUserWithUiAsync` — contention callback fires with local and remote descriptors
    - Inside the contention callback, capture both `PFGameSaveDescriptor` structs:
      - **Remote descriptor** (Device A's cloud data):
        - Verify `deviceType` is non-empty string (e.g., "XboxScarlett", "PC")
        - Verify `deviceId` is non-empty string
        - Verify `friendlyName` is non-empty string
        - Verify `time` is within the last 24 hours (reasonable timestamp)
        - Verify `totalBytes` is approximately 11264 (payload + thumbnail)
        - Verify `shortSaveDescription` == "Test Save — Level 5 Checkpoint"
        - Verify `thumbnailUri` is non-null (thumbnail was uploaded)
      - **Local descriptor** (Device B's state — empty/minimal since local was cleared):
        - Verify `time` is either 0 or a reasonable default
        - Verify `totalBytes` is 0 or minimal
    - Respond with `PFGameSaveFilesSetUiActiveDeviceContentionResponse` (action: SyncLastSavedData)
    - `PFGameSaveFilesGetFolder`
    - Full cleanup for both devices
4. *Key assertions*: All descriptor fields populated on remote descriptor, `shortSaveDescription` matches what was set, `thumbnailUri` non-null, timestamps reasonable, sizes match expected payload.

---

---

### ID38. Descriptor Fields — Conflict Callback

1. Run Common Account Reset (Xbox).
2. **Device A — Create and Upload Save With Description**:
    - Full init on Device A
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/payload.bin`, bytes: 10240, pattern: [0xAA, 0xBB, 0xCC, 0xDD]
    - `PFGameSaveFilesSetSaveDescriptionAsync` (description: "Device A — Cloud Version")
    - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
    - Full cleanup
3. **Wait**: 3–5 minutes for GRTS background upload.
4. **Device B — Create Conflicting Save Offline With Description**:
    - **[DISCONNECT]** Device B from network
    - Full init on Device B
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/payload.bin`, bytes: 20480, pattern: [0x11, 0x22, 0x33, 0x44] — different size and pattern
    - `PFGameSaveFilesSetSaveDescriptionAsync` (description: "Device B — Local Version")
    - Full cleanup (without upload — offline)
5. **Device B — Reconnect and Trigger Conflict**:
    - **[RECONNECT]** Device B
    - Full init on Device B with conflict callback that captures descriptors
    - `PFGameSaveFilesAddUserWithUiAsync` — conflict callback fires
    - Inside the conflict callback, capture both `PFGameSaveDescriptor` structs:
      - **Local descriptor** (Device B's divergent data):
        - Verify `totalBytes` ≈ 20480
        - Verify `shortSaveDescription` == "Device B — Local Version"
        - Verify `time` reflects Device B's write time
      - **Remote descriptor** (Device A's cloud data):
        - Verify `totalBytes` ≈ 10240
        - Verify `shortSaveDescription` == "Device A — Cloud Version"
        - Verify `time` reflects Device A's upload time
        - Verify `time` < local `time` (Device A uploaded first, Device B wrote later)
    - Respond with `PFGameSaveFilesSetUiConflictResponse` (action: TakeLocal)
    - Full cleanup
6. *Key assertions*: Both descriptors populated correctly, sizes match respective payloads, descriptions match what was set, timestamps reflect correct ordering.

---

---

### ID39. Thumbnail in Descriptors

1. Run Common Account Reset (Xbox).
2. **Device A — Write Save With Thumbnail**:
    - Full init on Device A
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/payload.bin`, bytes: 10240, pattern: [0xAA]
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `pfthumbnail.png`, bytes: 2048, pattern: [0x89, 0x50, 0x4E, 0x47] — PNG header
    - `PFGameSaveFilesUploadWithUiAsync` (mode: KeepDeviceActive) — keep lock for contention
3. **Device B — Trigger Contention and Verify Thumbnail URI**:
    - Full init on Device B with contention callback
    - `PFGameSaveFilesAddUserWithUiAsync` — contention callback fires
    - Inside contention callback:
      - Capture remote descriptor's `thumbnailUri`
      - Verify `thumbnailUri` is non-null and non-empty
      - If URI is a file path, verify the file exists and is readable
      - Optionally: read the file and verify the content starts with [0x89, 0x50, 0x4E, 0x47] (PNG magic bytes)
    - Respond with SyncLastSavedData
    - Full cleanup for both devices
4. **Negative test — No Thumbnail**:
    - Repeat the same test but do NOT write `pfthumbnail.png`
    - Verify `thumbnailUri` is null or empty in the descriptor
5. *Key assertions*: `thumbnailUri` populated when thumbnail exists, null when absent, file accessible at URI path, content matches.

---

---

### ID40. GetRemainingQuota — Explicit Verification

1. Run Common Account Reset (Xbox).
2. **Device A — Check Quota Before and After Upload**:
    - Full init on Device A
    - `PFGameSaveFilesGetRemainingQuota` — record initial quota as `Q0`
    - Verify `Q0` > 0 (should be ~256 MB for a fresh account)
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/payload.bin`, bytes: 1048576 (1 MB), pattern: [0xAA]
    - `PFGameSaveFilesUploadWithUiAsync` (mode: KeepDeviceActive)
    - `PFGameSaveFilesGetRemainingQuota` — record as `Q1`
    - Verify `Q1` < `Q0` (quota decreased)
    - Verify `Q0 - Q1` ≈ 1048576 (delta matches payload size, within margin for metadata)
    - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/payload2.bin`, bytes: 5242880 (5 MB), pattern: [0xBB]
    - `PFGameSaveFilesUploadWithUiAsync` (mode: KeepDeviceActive)
    - `PFGameSaveFilesGetRemainingQuota` — record as `Q2`
    - Verify `Q1 - Q2` ≈ 5242880
    - Full cleanup
3. *Key assertions*: `GetRemainingQuota` returns `S_OK`, values decrease as expected, deltas match payload sizes.

---

---

### ID41. Thumbnail Absent — Verify Empty URI

**Priority**: P2 | **Category**: UI Callback Data | **Devices**: 2

**Scenario**: Upload save data WITHOUT a `pfthumbnail.png` file. Set up contention or conflict. Verify the `thumbnailUri` field in the `PFGameSaveDescriptor` is empty (empty string, not garbage).

**Why This Matters**: Scenario ID39 tests with a thumbnail present. This scenario verifies the absence case — games that check `thumbnailUri[0] != '\0'` need it to be a clean empty string.

1. **Device A — Upload Without Thumbnail**:
    - Init, write `save.dat` (no `pfthumbnail.png`)
    - Upload with `KeepDeviceActive`

2. **Device B — Trigger Contention, Inspect Descriptor**:
    - Init, `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Contention callback fires
    - Inspect `remoteGameSave->thumbnailUri`
    - **KEY CHECK**: `thumbnailUri` is empty string (first byte is `\0`)
    - Respond with `SyncLastSavedData`
    - Cleanup both

3. *Key assertions*:
    - `thumbnailUri` is empty (not null, not garbage)
    - No crash when processing empty thumbnail
    - All other descriptor fields still populated correctly

---

---

### ID42. shortSaveDescription Round-Trip in Conflict Descriptor

**Priority**: P2 | **Category**: UI Callback Data | **Devices**: 2

**Scenario**: Set a save description via `PFGameSaveFilesSetSaveDescriptionAsync`, upload, then trigger a conflict on another device. Verify the `shortSaveDescription` field in the conflict callback's `PFGameSaveDescriptor` matches what was set.

**Why This Matters**: Scenario ID37 checks descriptor fields in contention, but doesn't specifically verify the description round-trips through conflict. Games display this in the "This Device" vs "Cloud Save" comparison.

1. **Device A — Set Description and Upload**:
    - Init, write save data
    - `PFGameSaveFilesSetSaveDescriptionAsync` → `{ "description": "Level 5 Boss Room - 2h30m" }`
    - Upload with `ReleaseDeviceAsActive`
    - Uninit

2. **Device B — Create Divergent Data with Different Description**:
    - Init, write divergent save data
    - `PFGameSaveFilesSetSaveDescriptionAsync` → `{ "description": "Level 3 Tutorial - 0h45m" }`
    - Upload (creates conflict)

3. **Trigger Conflict**:
    - Device B uploads — conflict callback fires
    - Inspect `localGameSave->shortSaveDescription` → should be "Level 3 Tutorial - 0h45m"
    - Inspect `remoteGameSave->shortSaveDescription` → should be "Level 5 Boss Room - 2h30m"
    - Respond with `UseCloud`
    - Cleanup both

4. *Key assertions*:
    - Local descriptor has Device B's description
    - Remote descriptor has Device A's description (round-tripped through cloud)
    - Descriptions are not truncated or corrupted
    - Null/empty description case: if no description set, field is empty string

---

---

### ID43. Out of Storage — requiredBytes Accuracy

**Priority**: P2 | **Category**: UI Callback Data | **Devices**: 1

**Scenario**: Fill storage to leave exactly N bytes free. Trigger a download that requires more than N bytes. When the out-of-storage callback fires, verify the `requiredBytes` value is reasonable — it should reflect the actual shortfall, not a meaningless default.

**Why This Matters**: Games display "Need X MB free" to the user based on `requiredBytes`. An inaccurate value leads to bad UX (user frees space but it's still not enough, or frees way more than needed).

1. **Setup — Seed Cloud Data**:
    - Device A: Init, write known-size data (e.g., 50 MB), upload, uninit

2. **Fill Storage on Target Device**:
    - `ConsumeDiskSpace` → leave ~10 MB free (less than the 50 MB needed)

3. **Trigger Download**:
    - Init, `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Out-of-storage callback fires
    - **KEY CHECK**: `requiredBytes` is approximately 40 MB (50 MB needed - 10 MB free = ~40 MB shortfall), within reasonable margin
    - Respond with `Cancel`

4. **Cleanup**:
    - `ReleaseDiskSpace`
    - `PFGameSaveFilesUninitialize`, `PFCleanupAsync`

5. *Key assertions*:
    - `requiredBytes` > 0
    - `requiredBytes` is within 20% of expected shortfall
    - Value is not a hardcoded default or suspiciously round number
    - No crash when reporting storage requirement


# Section S: Soak / Stress — Scenario Details

> Common init/cleanup/reset blocks are in [`_common-blocks.md`](./_common-blocks.md).

---

### ID103. Long-Running Sync Soak — Xbox

1. Run Common Account Reset (Xbox).
2. **Loop Configuration**:
    - Total iterations: 50 (approximately 2–4 hours depending on payload sizes and network speed)
    - Payload sizes: randomized per iteration using chaos mode parameters (range: 1 KB – 1 MB)
    - Pattern: alternate between Device A and Device B each iteration
3. **Each Iteration (i = 1 to 50)**:
    - **Writer Device** (Device A if i is odd, Device B if i is even):
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
      - `WriteGameSaveData` — operations: CreateBinaryFile, relativePath: `progress/soak-iter-{i}.bin`, bytes: random(1024, 1048576), pattern: [random byte]
      - `CaptureSaveContainerSnapshot` (slot: left)
      - `PFGameSaveFilesUploadWithUiAsync` (mode: ReleaseDeviceAsActive)
      - `PFLocalUserCloseHandle`
      - `XUserCloseHandle`
      - `PFGameSaveFilesUninitializeAsync`
      - `PFServicesUninitializeAsync`
      - `PFUninitializeAsync`
      - `PFServiceConfigCloseHandle`
      - `XTaskQueueCloseHandle`
    - **Wait**: Controller waits 3–5 minutes for GRTS background upload.
    - **Reader Device** (Device B if i is odd, Device A if i is even):
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
    - **Per-Iteration Assertions**:
      - Snapshot comparison passes
      - No error HRESULTs returned
      - Sync duration recorded (for drift detection)
4. **End-of-Soak Assertions**:
    - Memory/handle usage stays bounded (no leaks) — compare process memory at iteration 1 vs iteration 50
    - Manifests remain consistent across all iterations
    - Sync duration is stable (no progressive slowdown > 20% over baseline)
    - GRTS service connection remained stable throughout
    - No unexpected contention or conflict dialogs after the first iteration
5. *Key assertions*: Memory/handle usage bounded, manifests consistent, sync duration stable, GRTS service stable over extended operation.


# Section D: Sync Failure / Offline — Scenario Details

> Common init/cleanup/reset blocks are in [`_common-blocks.md`](./_common-blocks.md).

---

### ID25. SyncFailed — Cancel Response

1. Run Common Account Reset (Xbox).
2. **Device A — Go Offline and Launch**:
    - **[DISCONNECT]** Device A from network
    - `XGameRuntimeInitialize`
    - `PFInitialize`
    - `PFServicesInitialize`
    - `PFServiceConfigCreateHandle` (endpoint: `https://E18D7.playfabapi.com`, titleId: `E18D7`)
    - `XTaskQueueCreate` (workMode: ThreadPool, completionMode: ThreadPool, setAsProcessQueue: true)
    - `PFGameSaveFilesInitialize`
    - `PFGameSaveFilesSetUiCallbacks` (enable: true)
    - `PFGameSaveFilesSetActiveDeviceChangedCallback`
    - `PFGameSaveFilesSetUiSyncFailedAutoResponse` (enable: true, action: Cancel)
    - `XUserAddAsync`
    - `PFLocalUserCreateHandleWithXboxUser`
    - `PFGameSaveFilesAddUserWithUiAsync` — sync fails due to network; sync failure callback fires; auto-responds Cancel
    - Verify: `AddUserWithUiAsync` returns `E_PF_GAMESAVE_USER_CANCELLED` (0x800704c7)
    - Verify: no partial state, no crash
    - `PFLocalUserCloseHandle`
    - `XUserCloseHandle`
    - `PFGameSaveFilesUninitializeAsync`
    - `PFServicesUninitializeAsync`
    - `PFUninitializeAsync`
    - `PFServiceConfigCloseHandle`
    - `XTaskQueueCloseHandle`
3. **Device A — Go Online and Retry**:
    - **[RECONNECT]** Device A
    - Full init with no auto-response (or UseOffline fallback)
    - `PFGameSaveFilesAddUserWithUiAsync` — sync succeeds online
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot`
    - Full cleanup
4. *Key assertions*: Cancel response returns correct HRESULT, no partial data, relaunch after reconnect succeeds. **[DISCONNECT]** / **[RECONNECT]** depend on automation investigation.

---

---

### ID26. SyncFailed — Retry Response

1. Run Common Account Reset (Xbox).
2. **Device A — Go Offline, Launch, Retry While Offline, Then Retry Online**:
    - **[DISCONNECT]** Device A from network
    - `XGameRuntimeInitialize`
    - `PFInitialize`
    - `PFServicesInitialize`
    - `PFServiceConfigCreateHandle` (endpoint: `https://E18D7.playfabapi.com`, titleId: `E18D7`)
    - `XTaskQueueCreate` (workMode: ThreadPool, completionMode: ThreadPool, setAsProcessQueue: true)
    - `PFGameSaveFilesInitialize`
    - `PFGameSaveFilesSetUiCallbacks` (enable: true)
    - `PFGameSaveFilesSetActiveDeviceChangedCallback`
    - Configure sync failure callback to:
      - First invocation: respond with `Retry` (while still offline)
      - Second invocation (after going online): respond with `Retry`
    - `XUserAddAsync`
    - `PFLocalUserCreateHandleWithXboxUser`
    - `PFGameSaveFilesAddUserWithUiAsync` — sync fails, callback fires, responds Retry
    - First retry should fail (still offline) — callback fires again
    - **[RECONNECT]** Device A — go online between retries
    - Respond with `Retry` again
    - Verify sync succeeds on second retry (now online)
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot`
    - Full cleanup
3. *Key assertions*: First retry fails (still offline), second retry succeeds (online), data syncs correctly, no hang between retries. **[DISCONNECT]** / **[RECONNECT]** depend on automation investigation.

---

---

### ID27. Stock TCUI — Sync Failure Offline Fallback

1. Run Common Account Reset (Xbox).
2. **Device A — Go Offline and Launch Without Custom Callbacks**:
    - **[DISCONNECT]** Device A from network
    - `XGameRuntimeInitialize`
    - `PFInitialize`
    - `PFServicesInitialize`
    - `PFServiceConfigCreateHandle` (endpoint: `https://E18D7.playfabapi.com`, titleId: `E18D7`)
    - `XTaskQueueCreate` (workMode: ThreadPool, completionMode: ThreadPool, setAsProcessQueue: true)
    - `PFGameSaveFilesInitialize`
    - **Do NOT call `PFGameSaveFilesSetUiCallbacks`** — stock TCUI mode
    - `PFGameSaveFilesSetActiveDeviceChangedCallback`
    - `PFGameSaveFilesSetUiSyncFailedAutoResponse` (enable: true, action: UseOffline)
    - `XUserAddAsync`
    - `PFLocalUserCreateHandleWithXboxUser`
    - `PFGameSaveFilesAddUserWithUiAsync` — stock TCUI sync failure dialog appears; auto-responds UseOffline
    - Verify offline mode activates — `AddUserWithUiAsync` completes (not cancelled)
    - `PFGameSaveFilesGetFolder`
    - Verify local data is accessible
    - Full cleanup
3. **Device A — Go Online and Verify Sync**:
    - **[RECONNECT]** Device A
    - Full init (still no custom callbacks)
    - `PFGameSaveFilesAddUserWithUiAsync` — sync succeeds online
    - `PFGameSaveFilesGetFolder`
    - `CaptureSaveContainerSnapshot`
    - Full cleanup
4. *Key assertions*: Auto-response works in stock TCUI mode for sync failure, offline mode activates, no hang, relaunch online succeeds. **[DISCONNECT]** / **[RECONNECT]** depend on automation investigation.

---

---

### ID28. Upload in Offline Mode — E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD

**Priority**: P1 | **Category**: Offline Mode API Behavior | **Devices**: 1

**Scenario**: Go offline. Complete AddUser with `UseOffline`. Write save data. Call `UploadWithUiAsync`. Verify the call itself returns `S_OK`, but the async completion returns `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD`.

**Why This Matters**: A game that doesn't check the async result will think the upload succeeded. This specific HRESULT pairing (S_OK start, error completion) is documented and must work correctly.

1. **Init and Enter Offline Mode**:
    - **[DISCONNECT]** — Disable network on the device
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesSetUiSyncFailedAutoResponse` → `{ "response": "UseOffline" }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Sync failed callback fires → auto-response sends `UseOffline`
    - `XAsyncBlock` completes with `S_OK` — now in offline mode

2. **Write Data and Attempt Upload**:
    - `PFGameSaveFilesGetFolder` → get save path
    - Write save data to the local folder (e.g., `save.dat` with "offline_data_v1")
    - `PFGameSaveFilesUploadWithUiAsync` → `{}`
    - **KEY CHECK**: The initial call returns `S_OK` (accepted)
    - **KEY CHECK**: The `XAsyncBlock` completion returns `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD` (0x89237004)
    - Verify no crash, no hang

3. **Verify Local Data Intact**:
    - `PFGameSaveFilesGetFolder` → path still valid
    - Read `save.dat` — data still matches what was written
    - `PFGameSaveFilesUninitialize` → `{}`
    - `PFCleanupAsync`
    - **[RECONNECT]** — Restore network

4. *Key assertions*:
    - `UploadWithUiAsync` call itself returns `S_OK`
    - Async completion HRESULT = `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD` (0x89237004)
    - Local data is intact and readable after the failed upload
    - No crash, no hang, no orphaned async state
    - **[DISCONNECT]** / **[RECONNECT]** depend on automation investigation

---

---

### ID29. GetRemainingQuota in Offline Mode

**Priority**: P2 | **Category**: Offline Mode API Behavior | **Devices**: 1

**Scenario**: Enter offline mode. Call `PFGameSaveFilesGetRemainingQuota`. Verify it returns `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD`.

**Why This Matters**: Games that display remaining quota to the user need to handle this error gracefully in offline mode.

1. **Init and Enter Offline Mode**:
    - **[DISCONNECT]** — Disable network on the device
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesSetUiSyncFailedAutoResponse` → `{ "response": "UseOffline" }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Sync failed callback fires → auto-response sends `UseOffline`
    - `XAsyncBlock` completes with `S_OK` — now in offline mode

2. **Call GetRemainingQuota**:
    - `PFGameSaveFilesGetRemainingQuota` → `{}`
    - **KEY CHECK**: Returns `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD` (0x89237004)
    - No crash

3. **Cleanup**:
    - `PFGameSaveFilesUninitialize` → `{}`
    - `PFCleanupAsync`
    - **[RECONNECT]** — Restore network

4. *Key assertions*:
    - `GetRemainingQuota` returns `E_PF_GAMESAVE_DISCONNECTED_FROM_CLOUD`
    - No crash, no hang
    - **[DISCONNECT]** / **[RECONNECT]** depend on automation investigation

---

---

### ID30. Return to Online Without Re-Init

**Priority**: P1 | **Category**: Offline Mode API Behavior | **Devices**: 1

**Scenario**: Go offline. Enter offline mode. Write local data. Reconnect network. Call `AddUserWithUiAsync` again WITHOUT calling Uninitialize/Initialize. Verify the system reconnects to cloud and syncs the local data.

**Why This Matters**: The docs explicitly state: "Call `PFGameSaveFilesAddUserWithUiAsync()` again to attempt reconnection. No need to fully re-initialize the Game Saves system." This must work.

1. **Init and Enter Offline Mode**:
    - **[DISCONNECT]** — Disable network on the device
    - `SetServiceConfig` → `{ "pfTitleId": "XXXXX", "connectionString": "…" }`
    - `PFGameSaveFilesInitialize` → `{ "scid": "00000000-0000-0000-0000-00000XXXXX" }`
    - `PFGameSaveFilesSetUiCallbacks` → `{ "enable": true }`
    - `PFGameSaveFilesSetUiSyncFailedAutoResponse` → `{ "response": "UseOffline" }`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Sync failed callback fires → auto-response sends `UseOffline`
    - `XAsyncBlock` completes with `S_OK` — now in offline mode

2. **Write Data Locally While Offline**:
    - `PFGameSaveFilesGetFolder` → get save path
    - Write save data to the local folder (e.g., `save.dat` with "offline_data_v1")
    - Confirm `PFGameSaveFilesIsConnectedToCloud` returns `false`

3. **Reconnect and Re-Add User (No Re-Init)**:
    - **[RECONNECT]** — Restore network
    - Wait a few seconds for network to stabilize
    - Do NOT call `PFGameSaveFilesUninitialize` or `PFGameSaveFilesInitialize`
    - `PFGameSaveFilesAddUserWithUiAsync` → `{}`
    - Wait for progress callback → sync should complete successfully this time
    - **KEY CHECK**: `XAsyncBlock` completes with `S_OK`
    - **KEY CHECK**: `PFGameSaveFilesIsConnectedToCloud` now returns `true`

4. **Verify Data Synced to Cloud**:
    - `CaptureSaveContainerSnapshot` → verify local data is now in cloud
    - `PFGameSaveFilesGetRemainingQuota` → should succeed (no longer offline)
    - `PFGameSaveFilesUninitialize` → `{}`
    - `PFCleanupAsync`

5. *Key assertions*:
    - `AddUserWithUiAsync` succeeds without re-init
    - `IsConnectedToCloud` transitions from `false` to `true`
    - `GetRemainingQuota` succeeds after reconnect
    - Local data synced to cloud (snapshot matches written data)
    - **[DISCONNECT]** / **[RECONNECT]** depend on automation investigation

---

---

### ID31. SyncFailed Error Code Specificity

**Priority**: P2 | **Category**: Sync Failure / Offline | **Devices**: 1

**Scenario**: Trigger sync failures with different root causes and verify the HRESULT passed to the SyncFailed callback matches the failure type.

**Why This Matters**: The sync failed callback provides an HRESULT error code that games display to users. We test that the callback fires but never assert the specific error code matches the failure type.

1. **Test A — Network Failure**:
    - **[DISCONNECT]** — Disable network
    - Init, `PFGameSaveFilesAddUserWithUiAsync`
    - SyncFailed callback fires
    - **KEY CHECK**: Error HRESULT is a network-related code (e.g., `E_GS_PLAYFAB_SYNC_FAILURE` 0x80832302 or `E_PF_GAMESAVE_NETWORK_FAILURE` 0x89237005)
    - Respond with `UseOffline`
    - Cleanup
    - **[RECONNECT]**

2. **Test B — Auth Failure** (if entity auth path available):
    - Init with invalid entity token
    - SyncFailed callback fires
    - **KEY CHECK**: Error HRESULT is `E_GS_PLAYFAB_LOGIN_FAILURE` (0x80832300)
    - Respond with `Cancel`
    - Cleanup

3. *Key assertions*:
    - Each failure type produces a distinct, documented HRESULT
    - Error code passed to callback matches what the `XAsyncBlock` returns
    - No generic/catch-all error code hiding the real failure
    - **[DISCONNECT]** / **[RECONNECT]** depend on automation investigation


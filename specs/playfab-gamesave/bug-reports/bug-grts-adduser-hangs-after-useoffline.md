# Bug: GRTS Hangs After UseOffline Response During AddUserWithUiAsync

**Status:** Open  
**Severity:** P1 — Blocks offline-first game save scenarios  
**Component:** GRTS (`PFXGameSave` / xgameruntime)  
**Date:** 2026-04-01

---

## Summary

When `PFGameSaveFilesAddUserWithUiAsync` triggers a SyncFailed UI callback and the title responds with `UseOffline`, GRTS accepts the response (`hr=S_OK`) but never completes the async operation. The `AddUserWithUiAsync` call hangs indefinitely until the caller times out. GRTS does not invoke the async completion callback, leaving the game save state machine stuck.

Report a problem feedback:

https://aka.ms/AA10dztl

---

## PFX API Calling Pattern (GRTS Layer)

The SDK's `AddUserWithUiAsync` implementation calls into GRTS via these PFX APIs in sequence:

```
┌─ PFGameSaveFilesAddUserWithUiAsync (SDK public API)
│
├─ 1. PFXGameSaveInitializeConfig(&configRequest, &configHandle)
│      configRequest.requestingUser = <valid XUserHandle>
│      configRequest.titleId        = "E18D7"
│      configRequest.apiUrl         = "https://E18D7.playfabapi.com"
│      configRequest.flags          = 0x00000008 (InitFlagUseFileLocation)
│      configRequest.fileLocation   = "C:\gamesave\"
│      → hr = S_OK, configHandle allocated
│
├─ 2. PFXGameSaveFilesGetFolderWithUiAsync(configHandle, &asyncBlock)
│      → hr = S_OK, async operation begins
│      → GRTS starts sync with cloud service
│
│   ┌─ GRTS fires callbacks during sync:
│   │
│   ├─ 3a. PFXGameSaveProgressUiCallback(requestingUser, syncState=1)
│   │       syncState = PreparingForDownload, current=0, total=0
│   │
│   ├─ 3b. PFXGameSaveSyncFailedUiCallback(requestingUser, syncState=1, error=S_OK)
│   │       *** GRTS reports sync failure during PreparingForDownload ***
│   │
│   └─ 4. SDK responds: PFXGameSaveSetSyncFailedUiResponse(requestingUser, UseOffline)
│          → hr = S_OK
│          *** GRTS NEVER COMPLETES THE ASYNC OPERATION AFTER THIS ***
│
└─ 5. PFXGameSaveFilesGetFolderWithUiResult (never called — async never completes)
```

### Source Code Reference (`PFGameSaveFilesAPIProvider_GRTS.cpp`)

**Step 1–2** — `PFXPALCallGetFolderWithUiAsync()` (line ~218):
```cpp
PFXGameSaveConfigRequest configRequest{};
configRequest.requestingUser = context->xuser;  // valid XUserHandle
configRequest.titleId = context->playfabTitleId;
configRequest.apiUrl = context->apiEndpoint;
configRequest.flags = InitFlagUseFileLocation;
configRequest.fileLocation = context->saveFolderOverride;

hr = PFXGameSaveInitializeConfig(&configRequest, &configHandle);  // → S_OK
hr = PFXGameSaveFilesGetFolderWithUiAsync(configHandle, &asyncBlock);  // → S_OK
```

**Step 4** — `GameSaveAPIProviderGRTS::SetUiSyncFailedResponse()` (line ~1325):
```cpp
XUserHandle requestingUser = PFXPALGetXUserFromLocalUser(localUserHandle);
// requestingUser is valid (non-null) — same handle used in configRequest
HRESULT hr = PFXGameSaveSetSyncFailedUiResponse(
    requestingUser,
    PFXGameSaveSyncFailedUiResponse::UseOffline);  // action=2
// hr = S_OK, but GRTS never completes the GetFolderWithUi async after this
```

---

## Reproduction Steps

1. Initialize PFGameSave with GRTS provider (out-of-proc GRTS available, `ForceUseInprocGameSaves` not set).
2. Sign in with `XUserAddAsync` (valid XUser with package identity).
3. Create `PFLocalUserHandle` via `PFLocalUserCreateHandleWithXboxUser`.
4. Set UI callbacks via `PFGameSaveFilesSetUiCallbacks`.
5. Set auto-response: `PFGameSaveFilesSetUiSyncFailedAutoResponse(enable=true, action=UseOffline)`.
6. Call `PFGameSaveFilesAddUserWithUiAsync`.
7. GRTS calls `PFXGameSaveInitializeConfig` → `PFXGameSaveFilesGetFolderWithUiAsync` → both return `S_OK`.
8. GRTS fires `PFXGameSaveProgressUiCallback` (`syncState=PreparingForDownload`).
9. GRTS fires `PFXGameSaveSyncFailedUiCallback` (`syncState=PreparingForDownload`, `error=S_OK`).
10. SDK responds with `PFXGameSaveSetSyncFailedUiResponse(requestingUser, UseOffline)` → returns `S_OK`.
11. **Expected:** GRTS completes the `PFXGameSaveFilesGetFolderWithUiAsync` operation, transitioning user to offline mode. The async completion callback fires with `S_OK` or an offline-specific HRESULT.
12. **Actual:** GRTS goes silent. No further callbacks, no async completion. `PFXGameSaveFilesGetFolderWithUiResult` is never callable. The operation hangs until the caller times out.

---

## Evidence From Logs

Test scenario: `gamesave-41-offline-adduser-network-disabled.yml`

### Device Log (timestamps are key)

```
[07:21:31] PFGameSaveFilesAddUserWithUiAsync called
[07:21:31] PFXPALCallGetFolderWithUiAsync: configRequest.requestingUser=000002CE27406600  (valid XUser)
[07:21:32] PFXGameSaveFilesGetFolderWithUiAsync hr=0x00000000
[07:21:37] XAsyncBegin succeeded, returning S_OK
[07:21:37] MyPFXPALGameSaveProgressUiCallback: syncState=1 (PreparingForDownload)
[07:21:38] MyPFXPALGameSaveSyncFailedUiCallback: syncState=1, error=0x00000000
[07:21:44] PFGameSaveFilesUiSyncFailedCallback (state=PreparingForDownload, hr=0x00000000)
[07:21:56] GameSaveAPIProviderGRTS::SetUiSyncFailedResponse: action=2 (UseOffline)
[07:21:59] GameSaveAPIProviderGRTS::SetUiSyncFailedResponse: hr=0x00000000
[07:22:01] Auto responder: PFGameSaveFilesSetUiSyncFailedResponse action=UseOffline (hr=0x00000000)
           *** NO FURTHER LOG OUTPUT FROM GRTS ***
```

### Controller Log

```
[07:21:31] Sent command 'PFGameSaveFilesAddUserWithUiAsync' (timeout=120s)
[07:23:31] Command 'PFGameSaveFilesAddUserWithUiAsync' timed out after 120 seconds.
[07:23:31] Scenario failed: command 'PFGameSaveFilesAddUserWithUiAsync' timed out
```

### Timeline Analysis

| Time | Event | Gap |
|------|-------|-----|
| 07:21:31 | `AddUserWithUiAsync` called | — |
| 07:21:37 | Progress callback: `PreparingForDownload` | +6s |
| 07:21:38 | SyncFailed callback fires | +1s |
| 07:21:44 | SDK processes SyncFailed callback | +6s |
| 07:21:56 | `SetUiSyncFailedResponse(UseOffline)` called | +12s |
| 07:21:59 | `SetUiSyncFailedResponse` returns `S_OK` | +3s |
| 07:22:01 | Auto-responder logs success | +2s |
| 07:23:31 | Controller times out (120s) | +90s silence |

**Total dead time after UseOffline response: ~90 seconds of silence before timeout.**

Note: The delays between callback receipt and response (6-12s) are also suspicious — the auto-responder should invoke `SetUiSyncFailedResponse` immediately, but there appears to be significant latency in the callback dispatch.

---

## Environment

- **GDK Edition:** 260400 (April 2026)
- **Engine:** GRTS (out-of-proc, `IsOutOfProcGRTSAvailable: true`)

Version           : 35.112.1001.0
PackageFullName   : Microsoft.GamingServices_35.112.1001.0_x64__8wekyb3d8bbwe
InstallLocation   : C:\Program Files\WindowsApps\Microsoft.GamingServices_35.112.1001.0_x64__8wekyb3d8bbwe

- **Platform:** x64 Desktop with package identity (`MicrosoftGameConfig.mgc`)
- **Network:** Online (network adapter was NOT disabled — ChangeTargetDeviceState was commented out)
- **XUser:** Valid signed-in user via `XUserAddAsync(AddDefaultUserSilently)`

---

## Key Observations

1. **`requestingUser` is valid** — the log shows `configRequest.requestingUser=000002CE27406600` (non-null). This is NOT the null-XUser bug from `bug-grts-null-xuser-ui-response.md`.
2. **`SetUiSyncFailedResponse` returns S_OK** — GRTS accepted the UseOffline response without error.
3. **Network was online** — The SyncFailed callback fired with `error=S_OK` (not a network error), suggesting GRTS encountered a service-side or auth issue during initial sync, not a connectivity failure.
4. **SyncState is `PreparingForDownload` (1)** — Failure occurs very early in the sync process.
5. **No crash** — The process stays alive; it simply stops doing anything.

---

## Impact

- **Offline-first scenarios are broken** on GRTS: any title that wants to respond `UseOffline` to a sync failure during `AddUserWithUiAsync` will hang.
- **Retry and Cancel paths may also be affected** — if the GRTS state machine doesn't advance after UseOffline, it may also fail to advance after Retry or Cancel responses in similar early-sync-failure conditions.
- This blocks testing of scenario 41 (offline AddUser with network disabled) beyond the AddUser step.

---

## Suggested Investigation

1. **GRTS internal state machine**: After `PFXGameSaveSetSyncFailedUiResponse` is called with `UseOffline`, what state transition does GRTS expect? Is there a code path where the `UseOffline` action during `PreparingForDownload` is not handled?
2. **Callback dispatch latency**: The 6-12 second delays between GRTS firing a callback and the SDK processing it suggest either thread contention or the callback being dispatched on a queue that isn't being pumped promptly.
3. **SyncFailed with `error=S_OK`**: Why is SyncFailed firing with a success HRESULT? This may indicate GRTS is reporting a state transition (not an error), and `UseOffline` is not an expected response in this context.

---

## Workaround

None known. The `AddUserWithUiAsync` operation cannot complete once GRTS enters this state. The only option is to time out, uninitialize, and retry — but the same hang will recur.

---

## Related Files

| File | Relevance |
|------|-----------|
| `PFXGameSave.h` (GDK 260400) | GRTS API — `PFXGameSaveSetSyncFailedUiResponse` declaration |
| `Source/PlayFabGameSave/Source/GRTS/PFGameSaveFilesAPIProvider_GRTS.cpp` | SDK provider — `SetUiSyncFailedResponse` implementation |
| `Test/GameTestScenarios/gamesave-41-offline-adduser-network-disabled.yml` | Test scenario that reproduces this |
| `specs/playfab-gamesave/bug-reports/bug-grts-null-xuser-ui-response.md` | Related but different bug (null XUser); this bug has a valid XUser |

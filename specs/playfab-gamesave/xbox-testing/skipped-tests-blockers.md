# Xbox GameSave Tests — Skipped Test Blockers

> This document catalogs the 45 tests marked `xbox: skip` and what would be needed to make them pass.
> Use this for future work planning and to evaluate whether automated testing is feasible for each category.
>
> Last updated: 2026-04-30

## Summary

| Category | Tests | Count | Automation Feasible? |
|----------|-------|-------|---------------------|
| PLM Suspend/Resume (data loss) | 50-55 | 6 | ❌ Requires SDK fix |
| Connected Standby (remote wake fails) | 56-58 | 3 | ❌ Hardware limitation |
| Admin-required (network manipulation) | 38, 42, 90, 94 | 4 | ⚠️ Yes, with elevated controller |
| Cross-platform sync (separate backends) | 05, 11, 82 | 3 | ❌ Architecture limitation |
| Multi-user (second Xbox user) | 104-107 | 4 | ⚠️ Needs second signed-in user on devkit |
| Entity auth (PlayFab entity tokens) | 76, 108-112 | 6 | ⚠️ Needs entity auth infrastructure |
| Stub package (title switching) | 63-66 | 4 | ⚠️ Needs stub app deployed |
| Stock TCUI automation | 08, 09, 13 | 3 | ❌ UI can't be dismissed programmatically |
| Progress callbacks (not registered) | 36 | 1 | ❌ By-design Xbox CS limitation |
| Thumbnail descriptor (empty on Xbox CS) | 39 | 1 | ❌ Xbox CS doesn't populate field |
| XUserSignOut (E_NOTIMPL) | 113 | 1 | ❌ API not implemented on Xbox |
| Entity auth expired token | 77 | 1 | ⚠️ Needs entity auth infrastructure |
| Retry deadlock | 08 | 1 | ❌ TCUI retry loop can't auto-dismiss |
| Cancel doesn't dismiss TCUI | 13 | 1 | ❌ TCUI cancel behavior differs from PC |

---

## Category Details

### 1. PLM Suspend/Resume — Data Loss (Tests 50-55)

**Tests:** 50, 51, 52, 53, 54, 55

**What happens:** When `xbapp suspend` is called on the Xbox app, Connected Storage completely invalidates the local session. After `xbapp resume`, the save folder contains **0 files** — all local data is gone.

**Confirmed by:** Running test 51 (no admin needed). After suspend/resume cycle, snapshot shows 0 files vs expected 3 files (10,274 bytes).

**What would fix it:**
- **SDK-level fix:** Implement suspend/resume lifecycle handling in the GameSave layer that re-syncs from cloud after resume. This would require calling `PFGameSaveFilesAddUserWithUiAsync` again internally after PLM resume events.
- **Alternative:** Accept this as expected Xbox CS behavior and document that games must handle PLM resume by re-initializing their game save session.

**Automation feasibility:** Not automatable without SDK changes. The tests verify that data survives suspend/resume, which it currently doesn't.

**Additional notes:**
- Tests 50 and 53 also require `DisableNetwork` (admin) to create a stale lock
- Tests 51, 52, 54, 55 don't need admin — they fail purely from data loss
- The suspend uses `xbapp suspend <AUMID>` which is standard PLM simulation

---

### 2. Connected Standby / Instant On (Tests 56-58)

**Tests:** 56, 57, 58

**What happens:** The test puts the console into connected standby (`xbreboot /P`), waits, then attempts to wake it (`xbreboot /W`). The wake command times out with error `0x80070102`.

**Confirmed by:** Running test 57 after setting `xbconfig PowerMode=InstantOn`. The standby works, but `xbreboot /W` fails with timeout. Console becomes unreachable and requires SMC reset (`sflash /resetcycle`).

**What would fix it:**
- **Hardware/firmware fix:** The devkit may need specific firmware or network configuration for remote wake to function.
- **Physical wake:** A physical power button press or Wake-on-LAN packet could wake the console, but this isn't automatable in the current setup.
- **Different devkit:** Some devkit models may support remote wake more reliably.

**Automation feasibility:** Not automatable with current devkit. Remote wake is non-functional. Test 56 also requires admin for `DisableNetwork`.

**Risk:** Running these tests leaves the console in standby, requiring manual intervention (SMC reset via sflash or physical power cycle) to recover.

---

### 3. Admin-Required — Network Manipulation (Tests 38, 42, 90, 94)

**Tests:** 38, 42, 90, 94

**What happens:** These tests call `ChangeTargetDeviceState` with `action: DisableNetwork` on the PC (DeviceB). This runs PowerShell to disable network adapters, which requires the controller process to run as Administrator.

**What would fix it:**
- **Run controller elevated:** Start `GameTestController.exe` with admin privileges (runas or elevated terminal)
- **Alternative approach:** Modify the controller to use a less privileged mechanism (e.g., Windows Firewall rules instead of adapter disable, or use `xbstress simulate network=broken` for Xbox targets)

**Automation feasibility:** ⚠️ **Yes** — straightforward with an elevated controller process. Could be automated in CI with a service account that has admin privileges.

**Test purposes:**
- Test 38: Descriptor field conflict resolution with network-isolated device
- Test 42: Description roundtrip with network conflict
- Test 90: Shutdown during init (network isolated for stale lock)
- Test 94: Power loss during init (network isolated for stale lock)

---

### 4. Cross-Platform Sync (Tests 05, 11, 82)

**Tests:** 05, 11, 82

**What happens:** These tests expect DeviceA (Xbox) to see data uploaded by DeviceB (PC using GRTS/PlayFab blob storage). This fails because Xbox Connected Storage syncs with **Xbox CS Cloud** while PC GRTS syncs with **PlayFab blob storage** — these are completely separate cloud backends.

**Confirmed by:** Running with `ResetAllStorage` + 30-second cloud propagation delay. DeviceA still shows 0 files from cloud after DeviceB uploads.

**What would fix it:**
- **Backend integration:** A server-side sync bridge between PlayFab blob storage and Xbox CS Cloud. This doesn't exist today.
- **Same-engine testing:** Run both devices on the same engine (both Xbox, or both PC-GRTS) so they share the same cloud backend.

**Automation feasibility:** Not automatable. This is a fundamental architecture limitation — two separate cloud backends that don't sync in real-time.

---

### 5. Multi-User (Tests 104-107)

**Tests:** 104, 105, 106, 107

**What happens:** These tests require two Xbox users to be signed in simultaneously on the devkit. The test calls `XUserAddAsync` twice with different user contexts.

**What would fix it:**
- **Second test account:** Configure a second Xbox Live test account on the devkit (e.g., via `xbuser add`)
- **YAML parameter:** The tests may need parameters specifying which user email/gamertag to use for the second user
- **Controller support:** Verify `XUserAddAsync` handler supports selecting a specific user (not just the default signed-in user)

**Automation feasibility:** ⚠️ **Likely feasible** — requires one-time devkit setup (second test user signed in) and potentially minor test/handler changes to specify which user to add.

---

### 6. Entity Auth (Tests 76, 77, 108-112)

**Tests:** 76, 77, 108, 109, 110, 111, 112

**What happens:** These tests use PlayFab entity authentication tokens (not just Xbox Live tokens). The test infrastructure doesn't currently support obtaining/refreshing entity tokens on Xbox in a way that works with the GameSave tests.

**What would fix it:**
- **Entity auth handler:** Implement or wire up `PFAuthenticationGetEntityTokenAsync` in the Xbox test app
- **Token refresh:** For test 77 (expired token), implement a mechanism to let tokens expire and then refresh
- **Cross-platform entity:** Tests 108-112 need entity auth working across Xbox and PC simultaneously

**Automation feasibility:** ⚠️ **Feasible with test infrastructure work** — requires implementing entity auth command handlers in GameTestAppXbox and verifying token flow works in the test sandbox.

---

### 7. Stub Package / Title Switching (Tests 63-66)

**Tests:** 63, 64, 65, 66

**What happens:** These tests simulate "Quick Resume" scenarios where another game launches (evicting the current game) and then the original game resumes. They use `ChangeTargetDeviceState` with `action: EvictGame` which requires a `stubPackageName` parameter pointing to a deployed stub app.

**What would fix it:**
- **Deploy stub app:** Build and deploy a minimal Xbox app package that can be launched to evict the test app
- **Register stub AUMID:** Configure the stub package's AUMID in the test parameters
- **Verify Quick Resume:** Ensure the devkit supports Quick Resume for the test app (GDK requirement)

**Automation feasibility:** ⚠️ **Feasible with setup** — requires building/deploying a stub package (one-time setup) and configuring the tests with the correct package name.

---

### 8. Stock TCUI Automation (Tests 08, 09, 13)

**Tests:** 08, 09, 13

**What happens:**
- **Test 08 (Retry):** TCUI contention dialog "Retry" button creates an infinite loop — each retry re-shows the dialog with no way to auto-dismiss it
- **Test 09 (Stock TCUI):** Stock TCUI (without custom UI callbacks) presents a system dialog that cannot be programmatically interacted with
- **Test 13 (Cancel):** TCUI "Cancel" button doesn't dismiss the dialog on Xbox as it does on PC — the dialog remains visible

**What would fix it:**
- These are fundamental limitations of Xbox system TCUI. The dialogs are rendered by the system shell and cannot be automated without Xbox OS-level test hooks.
- Microsoft internal test frameworks (XDP, TAEF with Xbox hooks) might be able to dismiss these.

**Automation feasibility:** ❌ **Not automatable** with current tools. Would require Xbox OS team support for automated UI interaction hooks.

---

### 9. Progress Callbacks (Test 36)

**Test:** 36

**What happens:** The test verifies sync state transition callbacks (progress reporting). The Xbox test app explicitly does NOT register progress callbacks for Xbox/PcGrts engines (line 87-91 of `PFGameSaveFilesHandlers.cpp`).

**What would fix it:**
- **Register callbacks:** Remove the `if (state->engineType != DeviceEngineType::Xbox && state->engineType != DeviceEngineType::PcGrts)` guard and register progress callbacks for Xbox
- **However:** Xbox CS may not actually fire these callbacks even if registered (system limitation)

**Automation feasibility:** ❌ **Likely not fixable** — Xbox Connected Storage doesn't expose progress callbacks at the same granularity as PC GRTS. The guard exists for a reason.

---

### 10. Thumbnail Descriptor (Test 39)

**Test:** 39

**What happens:** Xbox Connected Storage does not populate `thumbnailUri` in container descriptors. The field is always empty on Xbox CS, though it works on PC GRTS (which uses PlayFab blob storage with thumbnail support).

**Automation feasibility:** ❌ **Not fixable** — Xbox CS platform limitation. The entire test purpose is verifying thumbnail functionality.

---

### 11. XUserSignOut (Test 113)

**Test:** 113

**What happens:** `XUserSignOutAsync` returns `E_NOTIMPL` on Xbox devkits. This API is not implemented in the GDK for devkit use.

**What would fix it:**
- **GDK update:** Wait for Microsoft to implement `XUserSignOutAsync` on devkits
- **Alternative:** Use `xbuser signout` via `ChangeTargetDeviceState` from the controller side, but this signs out at the system level (not gracefully through the API)

**Automation feasibility:** ❌ **Not fixable** — API not implemented.

---

## Prioritization for Future Work

### High Value (likely to pass with moderate effort)
1. **Admin tests (38, 42, 90, 94)** — Just run controller elevated
2. **Stub package tests (63-66)** — One-time stub app deployment
3. **Multi-user tests (104-107)** — Second test account setup

### Medium Value (significant infrastructure work)
4. **Entity auth tests (76, 77, 108-112)** — Handler implementation needed

### Not Fixable (platform/architecture limitations)
5. PLM data loss (50-55) — Requires SDK redesign
6. Connected standby (56-58) — Hardware/remote-wake limitation
7. Cross-platform sync (05, 11, 82) — Separate cloud backends
8. TCUI automation (08, 09, 13) — System UI not automatable
9. Progress callbacks (36) — Xbox CS limitation
10. Thumbnail (39) — Xbox CS limitation
11. XUserSignOut (113) — API not implemented

---

## Prerequisites for Running Full Test Suite

Before running the Xbox GameSave test suite, ensure:

1. **Xbox devkit** is powered on and reachable at configured IP (default: 10.0.0.104)
2. **Xbox user** is signed in (sandbox: XDKS.1)
3. **Power mode** is set to Energy Saving (`xbconfig PowerMode=EnergySaving`) — do NOT use Instant On as it can strand the console
4. **Connected Storage** is clean (`xbstorage reset /force /x:<ip>`)
5. **Test app** is deployed (`xbapp deploy`)
6. **PC test app** is built and available at `Out\x64\Debug\GameTestAppWindows\GameTestAppWindows.exe`
7. **Controller** is built at `Out\x64\Debug\GameTestController\GameTestController.exe`
8. **Network** — both Xbox and PC must be on the same subnet with WebSocket connectivity on port 5000

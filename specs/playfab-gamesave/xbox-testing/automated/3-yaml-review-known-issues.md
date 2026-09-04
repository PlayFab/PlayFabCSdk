# YAML Test Scenario Review — Known Issues

Review date: April 2025
Scope: All 113 `gamesave-xbox-*.yml` files reviewed against official docs and detail specs.

**Summary: 49 OK, 36 Fixed, 28 Known Issues**

The 36 fixed files had critical bugs (missing auto-responses that cause hangs, wrong auth patterns, wrong parameter names, etc.) and are now corrected. The 28 known issues below require either harness changes or deeper restructuring.

---

## Category 1: Harness Timing Limitation (13 files)

`PFGameSaveFilesAddUserWithUiAsync` runs synchronously in the test harness — the next YAML step only executes AFTER AddUser returns. Tests that intend to interrupt "during" init/download cannot achieve that timing with sequential YAML steps.

**Impact:** These tests still exercise the operation + action + recovery flow, but they fire the interrupting action (suspend/terminate/evict/signout) AFTER the target operation completes, not mid-flight as the spec intends.

**Resolution:** Add async/parallel step execution support to the harness (e.g., `fireAndForget: true` on AddUserWithUiAsync, or parallel block execution).

| File | ID | Section | Intended Timing |
|------|----|---------|-----------------|
| `gamesave-xbox-50-fg-bg-during-init.yml` | ID50 | H — FG/BG | Suspend during init contention dialog |
| `gamesave-xbox-51-fg-bg-during-download.yml` | ID51 | H — FG/BG | Suspend during download |
| `gamesave-xbox-53-suspend-resume-init.yml` | ID53 | I — Suspend | PLM suspend during init contention |
| `gamesave-xbox-54-suspend-resume-download.yml` | ID54 | I — Suspend | PLM suspend during download |
| `gamesave-xbox-56-connected-standby-init.yml` | ID56 | I — Standby | Standby during init contention |
| `gamesave-xbox-57-connected-standby-download.yml` | ID57 | I — Standby | Standby during download |
| `gamesave-xbox-61-terminate-download.yml` | ID61 | J — Terminate | Terminate during download |
| `gamesave-xbox-63-title-switch-init.yml` | ID63 | J — Evict | EvictGame during init |
| `gamesave-xbox-64-title-switch-download.yml` | ID64 | J — Evict | EvictGame during download |
| `gamesave-xbox-68-signout-during-download.yml` | ID68 | K — Signout | Signout during download |
| `gamesave-xbox-72-spop-download.yml` | ID72 | L — SPOP | SPOP trigger during download |
| `gamesave-xbox-80-network-flapping-sync.yml` | ID80 | M — Network | NetworkFlapping during sync |

> **Note:** Tests where the action fires after a synchronous local write (e.g., ID55, 58, 62, 65) are **OK** — the spec also intends the action to happen after write completes.

---

## Category 2: Scenario Mismatch / Missing Setup (3 files)

These tests deviate from the spec's intended scenario structure.

### ID60 — `gamesave-xbox-60-terminate-init.yml`
**Section:** J — Terminate/Evict
**Issue:** Spec says Device B holds a stale lock → Device A begins init → contention dialog appears → terminate Device A during dialog. YAML instead has Device A upload with KeepDeviceActive, get terminated, then Device B takes over. This tests "terminate after upload" not "terminate during init."
**Fix:** Restructure so Device B seeds stale lock (upload + disconnect), Device A starts init (hits contention), then Device A is terminated mid-contention.

### ID67 — `gamesave-xbox-67-signout-during-init.yml`
**Section:** K — Signout
**Issue:** Spec requires Device B to create a stale lock so Device A sees a contention dialog during init, then signout fires during the dialog. YAML is single-device only — no contention dialog is created. Signout happens during a plain init.
**Fix:** Add Device B stale-lock seed phase (init → upload KeepDeviceActive → DisableNetwork). Add contention auto-response to Device A's retry block.

### ID71 — `gamesave-xbox-71-spop-init.yml`
**Section:** L — SPOP
**Issue:** Spec says Device B must disconnect after uploading with KeepDeviceActive to preserve the stale lock. Without disconnecting, `PFGameSaveFilesUninitializeAsync` in cleanup may release the lock cleanly, meaning Device A never sees contention.
**Fix:** Add `ChangeTargetDeviceState(action: DisableNetwork)` after Device B's upload, and `EnableNetwork` at start of Device B's SPOP trigger block.

---

## Category 3: Missing Verification / Assertions (8 files)

### ID46 — `gamesave-xbox-46-storage-full-upload.yml`
**Section:** G — Out of Storage
**Issue:** No Device B verification block to confirm the upload under storage pressure actually reached the cloud.
**Fix:** Add Device B sync + snapshot comparison block.

### ID47 — `gamesave-xbox-47-cloud-quota-exhaustion.yml`
**Section:** G — Out of Storage
**Issue:** Missing Device B verify block to confirm final cloud state. Also missing `expectedHr` assertion on the quota-exceeding upload.
**Fix:** Add Device B verify + expected error assertion.

### ID69 — `gamesave-xbox-69-signout-write.yml`
**Section:** K — Signout
**Issue:** Missing 3-5 min GRTS wait and Device B verification to prove whether the post-signout write persisted/uploaded or rolled back.
**Fix:** Add SmokeDelay (180000ms) + Device B sync-and-verify block.

### ID70 — `gamesave-xbox-70-user-switch-same-device.yml`
**Section:** K — Signout
**Issue:** No explicit check that User A data is absent in User B's folder. Missing cleanup between relaunch-verify phases (handle leak).
**Fix:** Add User A absence assertion + cleanup block between phases.

### ID32 — `gamesave-xbox-32-progress-during-download.yml`
**Section:** E — Progress
**Issue:** Missing progress state recording and verification. Test verifies data correctness but not progress callback behavior.
**Fix:** Add `PFGameSaveFilesSetUiProgressStateRecording` + `PFGameSaveFilesVerifyProgressStateSequence` (like ID36 does).

### ID43 — `gamesave-xbox-43-out-of-storage-required-bytes.yml`
**Section:** G — Out of Storage
**Issue:** Uses `SimulateOutOfStorage(forceOutOfStorage)` which leaves zero free space. Spec intends a partial shortfall (~50MB data with ~10MB free) to test `requiredBytes` accuracy.
**Fix:** Use partial storage simulation if harness supports it, or write larger payload.

### ID44 — `gamesave-xbox-44-storage-full-init.yml`
**Section:** G — Out of Storage
**Issue:** Missing integrity assertion after retry. Spec requires verifying "no partial or corrupt files" after storage restored and retry succeeds.
**Fix:** Add `CaptureSaveContainerSnapshot` after retry `AddUserWithUiAsync`.

### ID35 — `gamesave-xbox-35-write-during-upload.yml`
**Section:** E — Progress
**Issue:** First snapshot (pre-write) missing `expectedContent` assertion. Only second snapshot verifies content.
**Fix:** Add `expectedContent: { "save.dat": "initial_data_v1" }` to first `CaptureSaveContainerSnapshot`.

---

## Category 4: Harness Cross-Device Pipelining (2 files)

These tests depend on one device's action starting while another device is blocked in a callback.

### ID8 — `gamesave-xbox-08-contention-retry.yml`
**Section:** B — Contention
**Issue:** Device B's AddUserWithUiAsync with Retry auto-response loops while Device A still holds the lock. Device A's release step can't run until Device B's step completes (sequential execution). If the harness doesn't pipeline cross-device steps, this hangs.
**Fix:** Verify the harness supports cross-device pipelining. If not, restructure with timeout or async execution.

### ID12 — `gamesave-xbox-12-contention-go-back.yml`
**Section:** B — Contention
**Issue:** Same-device blocks are sequential. Block 1 starts AddUser with Retry auto-response (loops forever). Block 2 switches auto-response to SyncLastSavedData. But Block 2 can't execute until Block 1 completes → infinite loop.
**Fix:** Merge the auto-response escalation into the same block, or add `maxRetries: 1, fallbackAction: SyncLastSavedData` if harness supports it.

---

## Category 5: Miscellaneous (2 files)

### ID17 — `gamesave-xbox-17-conflict-stock-tcui.yml`
**Section:** C — Conflicts
**Issue:** `PFGameSaveFilesGetFolder` and `WriteGameSaveData` are called before `PFGameSaveFilesAddUserWithUiAsync` in the second session. If the API requires AddUser before GetFolder, this fails.
**Fix:** Investigate whether GetFolder works from a prior session's path. If not, restructure to call AddUser first.

### ID30 — `gamesave-xbox-30-return-online-no-reinit.yml`
**Section:** D — Offline
**Issue:** Minor — spec step 4 says verify `GetRemainingQuota` succeeds after reconnect (proves no longer offline). YAML omits this.
**Fix:** Add `PFGameSaveFilesGetRemainingQuota` after `IsConnectedToCloud` check.

---

## Priority Order for Fixes

1. **Category 2** (scenario mismatch) — ID60, ID67, ID71 — highest value, tests are exercising wrong thing
2. **Category 4** (pipelining) — ID8, ID12 — likely hang, need harness investigation
3. **Category 3** (missing verification) — ID46, ID47, ID69, ID70 — tests pass but don't prove what they should
4. **Category 1** (timing) — 13 files — systemic harness limitation, fix once in harness
5. **Category 5** (minor) — ID17, ID30 — low risk

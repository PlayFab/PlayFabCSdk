# Debug Failing Tests Plan

## Current State (2026-05-02, Revised)

| Status | Count | Description |
|--------|-------|-------------|
| `pc-grts: passing` | 81 | Stable regression suite (`--passing`) |
| `pc-grts: failing` | 23 | Bug reports filed with detailed log evidence |
| `pc-grts: ignore` | 1 | Test 95 (power loss — hardware only) |
| **Deleted** | 11 | Xbox-only hardware tests + duplicates |
| **Total** | **105** | (was 116, deleted 11) |

**Failing tests** — all have detailed bug reports in `C:\git\PlayFab.C\specs\playfab-gamesave\xplat-testing\status\failing-tests\`:
- 03: Background upload stuck
- 10, 12, 24: Cross-engine isolation (GRTS can't detect inproc)
- 13, 26, 80: GRTS TCUI cancel/retry enters infinite loop
- 16, 23, 33, 34, 43, 44, 45, 48: Cancel returns S_OK instead of E_CANCELLED
- 37, 39: GRTS descriptor thumbnailUri empty
- 38, 41: Cross-engine descriptor callbacks timeout
- 40: GRTS quota cache stale after upload
- 87: Path validation deferred to AddUser time
- 89: Multiple GRTS error codes differ from expected
- 113: Requires Xbox engine (always skipped)

Last passing run: **pass71** — 81 stable tests

## Goal

For each test marked `pc-grts: failing`, run it, analyze logs, attempt a fix. If unfixable (SDK/service/hardware bug), write a bug report to `C:\git\PlayFab.C\specs\playfab-gamesave\xplat-testing\status\failing-tests\`. If fixable (YAML fix, engine selection, harness workaround), fix it and re-run until green.

## Scope

- **45 tests** currently marked `pc-grts: failing` in `Test/GameTestScenarios/gamesave-pc/`
- Target: reduce failures to only those genuinely blocked on SDK/service/hardware

## Rules

### Rule 1: Run → Analyze → Fix or File

For each test:
1. Run: `py Utilities\Scripts\tests-run.py gamesave-pc --only <id> --timeout 300`
2. Read device logs in `Out\gamesave-pc-tests\pass<N>\<id>\`
3. Determine root cause from logs
4. **If fixable** (YAML bug, wrong engine, missing param, wrong expectedHr, test logic error):
   - Fix the YAML
   - Re-run to confirm PASS
   - Update YAML: `pc-grts: passing`
5. **If not fixable** (service bug, hardware required, or risky SDK change outside GameSave):
   - Create/update `C:\git\PlayFab.C\specs\playfab-gamesave\xplat-testing\status\failing-tests\<filename>.md`
   - Include: symptom, log evidence, root cause, recommended fix owner

**Fixable scope** — attempt code fixes for:
- **PFGameSaves SDK** (`Source\PlayFabGameSave\Source\**`, especially `Platform\Windows\*grts.*`)
- **Test harness** (`Test\GameTestAppWindows\`, `Test\GameTestAppShared\`, `Test\GameTestController\`)
- **Test YAML** (`Test\GameTestScenarios\gamesave-pc\`)

After fixing code, rebuild with `Utilities\Scripts\tests-build.ps1` and re-run to confirm.

### Rule 2: Group by root cause

Process tests in groups sharing the same root cause. If one test in a group is diagnosed, apply the finding to siblings without re-investigating from scratch. Run each sibling once to confirm the same pattern.

### Rule 3: Try harder before filing bugs

Before declaring "SDK bug", verify:
- Is the YAML testing what it thinks it's testing? (wrong engine, wrong device, stale local state?)
- Would `DeleteLocalFolder` or clearing GRTS state fix it?
- Is the `expectedHr` correct for the pc-grts engine (vs Xbox)?
- Can the test be restructured to avoid the broken path while still validating the behavior?

### Rule 4: Log proof from PFX APIs (GRTS boundary)

Since we're testing through GRTS, the PFGameSaves DLL calls into PFX APIs (`XGameSave*`, `XGameSaveFiles*`) which are the boundary GRTS exposes. When a test fails, we need **log evidence at this boundary** to determine if:
- PFGameSaves is calling GRTS incorrectly (our bug — fix it)
- GRTS is returning unexpected results (GRTS bug — file it)

**Add logging as needed** in `Source\PlayFabGameSave\Source\Platform\Windows\*grts.*` to trace:
- PFX API calls made (function name, parameters)
- HRESULTs returned from PFX APIs
- State transitions (versions, active device, sync status)
- Callback invocations and their payloads

This logging is the proof we need to assign blame correctly. If existing traces don't show the PFX boundary, add them before concluding "GRTS bug".

### Rule 4: Bug report format

Each bug report in `C:\git\PlayFab.C\specs\playfab-gamesave\xplat-testing\status\failing-tests\` must have:
```markdown
# <Title>

## Tests Affected
- <list>

## Symptom
<what the user sees / what fails>

## Evidence
<key log lines from the most recent run>

## Root Cause
<explanation of why it fails>

## Fix Owner
<SDK team / service team / harness / test YAML>

## Suggested Fix
<what code change would resolve this>
```

### Rule 5: Don't fix what you can't test

If a fix requires service deploy, Xbox hardware, or admin privileges — file the bug and move on. Don't spend time on workarounds that can't be verified.

## Execution

```powershell
# Run a single failing test
cd C:\git\PlayFab.C
py Utilities\Scripts\tests-run.py gamesave-pc --only <id> --timeout 300

# Run all failing tests (batch)
py Utilities\Scripts\tests-run.py gamesave-pc --rerun --timeout 300

# Check results
py Utilities\Scripts\combine-xplat-results.py
```

## Test Groups (Process in This Order)

### Group A: Quick Wins — Likely Fixable (try these first)

Tests that may have YAML issues, wrong engine selection, or test logic bugs.

| # | Name | Hypothesis |
|---|------|-----------|
| 03 | Background Upload Verification | Infra/timing — may just need longer timeout or GRTS state reset |
| 84b | Rapid Upload Rate Limit | WaitForGameSaveSync variant — check if handler supports it |
| 85b | Upload SyncFailed Rate Limit | Same as 84b |

### Group B: IsWinner / GRTS Stale State — Blocked on Service

Tests failing because GRTS version stays at 498 and cross-engine data is invisible.

| # | Name | Notes |
|---|------|-------|
| 42 | Description Round-Trip in Conflict | Remote descriptor empty — GRTS stale state |
| 108 | Cross-Platform Golden Path | GRTS doesn't see inproc uploads — same bug |
| 117 | GRTS Conflict Winner Visible to SDK | Expected fail — validates Stage 2 service fix |

**Action**: Re-run after Stage 2 deploy (Monday evening). If still failing, clear GRTS state: delete `C:\XboxGames\GameSave\pgs\u_*` and retry.

### Group C: Offline / SyncFailed — SDK Bug

GRTS intercepts sync failure, returns `E_GS_USER_CANCELED (0x80830004)`.

| # | Name | Error |
|---|------|-------|
| 25 | SyncFailed Cancel Response | 0x80830004 (expected 0x800704C7) |
| 26 | SyncFailed Retry Response | Timeout — device disconnects during retry |
| 27 | SyncFailed Stock TCUI Offline | 0x80830004 on AddUserWithUiAsync |
| 28 | Upload in Offline Mode | 0x80830004 on AddUserWithUiAsync |
| 29 | GetRemainingQuota in Offline Mode | 0x80830004 — same as 27 |
| 30 | Return Online Without Re-Init | 0x80830004 — same as 27 |
| 31 | SyncFailed Error Code Specificity | 0x80830004 — same as 27 |

**Investigation**: Can the tests accept `0x80830004` as the GRTS-specific equivalent? Or is the offline mode genuinely non-functional on GRTS?

### Group D: Cross-Engine Contention / Conflict

GRTS and inproc don't cross-notify about active device changes.

| # | Name | Error |
|---|------|-------|
| 10 | IsConnectedToCloud After Takeover | Returns true (expected false) |
| 12 | Contention Go Back | E_GS_USER_CANCELED from GRTS |
| 13 | Contention Cancel | GRTS dialog stuck |
| 16 | Conflict Play Offline | 0x800704C7 — "Play Offline" not in SDK enum |
| 23 | Delete on Both Sides | UploadAsyncProvider 0x89237004 |
| 24 | Multi-Callback Contention+Conflict | UploadAsyncProvider 0x89237004 |

**Investigation**: Are 23/24 actually cross-engine notification bugs, or is the upload provider failure independent? Run each individually with fresh GRTS state.

### Group E: Descriptor / Metadata / Quota

| # | Name | Error |
|---|------|-------|
| 37 | Descriptor in Contention | thumbnailUri empty |
| 38 | Descriptor in Conflict | Fields empty |
| 39 | Thumbnail URI | Not propagated |
| 40 | GetRemainingQuota | Quota delta = 0 after upload |
| 41 | Thumbnail Absent | Dialog stuck |
| 43 | Out-of-Storage Required Bytes | Inaccurate (depends on quota) |

**Investigation**: Is thumbnailUri populated in GRTS-only mode? Or is this an inproc gap? Check if DeviceA engine matters.

### Group F: Storage Full

| # | Name | Depends On |
|---|------|-----------|
| 44 | Storage Full During Init | Working quota (test 40) |
| 45 | Storage Full During Download | Working quota (test 40) |
| 48 | Out of Storage Cancel | Working quota (test 40) |

**Investigation**: Skip until Group E (quota) is resolved. These cascade from the same root cause.

### Group G: Cancel / Edge Cases

| # | Name | Error |
|---|------|-------|

## Conflict Block Testing Lessons (Tests 126-144, May 2026)

Hard-won debugging knowledge from the 19-test conflict block edge case matrix (tests 126-144).

### Contention vs Conflict: How to Tell Them Apart in Logs

| Log Indicator | Meaning |
|---------------|---------|
| `PFGameSaveFilesUiConflictCallback (local)/(remote)` | True **conflict** — local data diverged from cloud |
| `PFGameSaveFilesUiActiveDeviceContentionCallback (local)/(remote)` | **Contention** — another device holds the active lock |
| `PFActivatorTakeRemoteLoserUploadDisabled` (ETL) | Conflict resolution — TakeRemote path |
| `PFActivatorTakeLocalConflict` (ETL) | Conflict resolution — TakeLocal path |
| `PFContextLockContention.ConflictVersion` (ETL) | Conflict triggered — shows cloud winner version |
| `BreakLock` (ETL) | Contention resolved — lock broken from other device |

### IsWinner Stomp: How to Detect in Test Logs

The isWinner stomp is silent — uploads return HTTP 200 and appear successful. The only way to detect it:

1. **Snapshot comparison hash mismatch** — DeviceB uploaded data X, DeviceA downloaded data Y (where Y ≠ X)
2. **ETL confirms upload succeeded** — `PFUploadContextFinalized` with no errors
3. **No 409 rejections** — the upload was accepted but the data was treated as conflict-winner replay

If you see `CompareSaveContainerSnapshots` fail with hash mismatch but all uploads succeeded, suspect isWinner stomp.

### Test Design Patterns That Work

| Pattern | Result | Notes |
|---------|--------|-------|
| SDK seeds → GRTS offline-terminate → GRTS relaunch → AddUser | Conflict fires | Standard pattern (tests 125, 126-132, 143) |
| SDK seeds → GRTS uninit → re-init → AddUser | Conflict fires | Cleaner, no orphaned uploads (test 139) |
| SDK seeds → GRTS KeepActive → SDK contention → SDK advances | Cloud advanced | SDK gets contention callback, responds SyncLastSavedData |
| Two GRTS devices on same machine | Contention only | Cannot trigger conflict — shared GamingServices.exe (test 134) |
| DisableNetwork → UploadWithUi | Upload "succeeds" | UploadWithUi reports local sync, not cloud (test 135) |
| GRTS KeepActive → cloud advances → GRTS uploads stale | Conflict mid-upload | ConflictCallback fires during UploadWithUi, not AddUser (test 144) |
| Two conflicts in sequence (dirty data between) | Both fire correctly | No state leakage between consecutive conflicts (test 142) |

### Key Gotchas

- **UploadWithUi returns success even with network off** — it reports local blob cache sync status, not cloud upload. Cannot test upload failure scenarios with DisableNetwork (Rule 15).
- **Writing to new folders in save root does NOT create GRTS containers** — only pre-registered containers (in ExtManifest) are tracked. Adding files to `slotB/` when only `slotA` is registered does nothing (Rule 16).
- **PendingConflictVersion is invisible in ETL** — it's only in the HTTP request body. You can see `ConflictVersion` in ETL but not the field that gets sent on upload.
- **GRTS auto-uploads within ~0.2s of conflict resolution** — you cannot "crash before upload" reliably (Rule 10b).
- **No-write upload after TakeRemote is safe** — server handles it idempotently (Rule 18).
- **Dual-upload pattern (Rule 22) only works for AddUser-time conflicts** — if conflict fires during UploadWithUi (stale manifest), the KeepActive upload IS the conflict resolution upload and cannot clear PendingConflictIsWinner. Subsequent uploads in the same session are also stomped. Full uninit/re-init is required (Rule 23, test 144).
- **GRTS never surfaces HTTP 409 to games** — when GRTS detects a stale manifest during upload, it fires ConflictCallback mid-upload and auto-resolves. The upload returns hr=0 but data is stomped (Rule 23).

### Test Design Patterns — Conflict Trigger Timing Matters

| Conflict Trigger Point | Dual-Upload Clears Stomp? | Workaround |
|------------------------|--------------------------|------------|
| During **AddUser** (Terminate/Relaunch, uninit/re-init) | ✅ Yes (Rule 22) | KeepActive → Release |
| During **Upload** (stale manifest, KeepDeviceActive) | ❌ No (Rule 23) | Uninit → Re-init → AddUser → dual-upload |

| 33 | Cancel Sync During Download | AutoNavigateGameSaveUi unsupported on inproc |
| 34 | Cancel Sync During Upload | Same as 33 |
| 87 | Invalid Path | Returns S_OK (no validation) |
| 89 | GRTS Error Codes | E_FAIL instead of specific code |

**Investigation**: Can 33/34 use `pc-grts` engine for both devices? Does the cancel behavior work on GRTS?

### Group H: Auth / Network / Rollback

| # | Name | Error |
|---|------|-------|
| 76 | Entity Auth Two-Device | Cross-engine data not visible |
| 77 | Expired Token Handling | SetExpiredEntityToken not implemented |
| 78 | Rollback to Last Conflict | GRTS re-sync error |
| 80 | Network Flapping | Timeout/retry failure |

**Investigation**: Is 76 another IsWinner/cross-engine bug? Does 77 need a harness command addition?

### Group I: Xbox-Only — Cannot Run on PC

| # | Name | Requires |
|---|------|---------|
| 52 | FG/BG During Local Write | Xbox PLM (Suspend/Resume) |
| 54 | Suspend/Resume During Download | Xbox PLM |
| 55 | Suspend/Resume During Write | Xbox PLM |
| 56 | Connected Standby During Init | Xbox CS |
| 57 | Connected Standby During Download | Xbox CS |
| 58 | Connected Standby During Write | Xbox CS |
| 64 | Title Switching | Xbox EvictGame |
| 66 | Quick Resume | Xbox shell |
| 113 | Two-Device SPOP | Xbox engine |

**Action**: File one bug report covering all 9. These need Xbox hardware or PC simulation harness.

### Group J: Termination Regression

| # | Name | Notes |
|---|------|-------|
| 102 | TCUI Dialog Closes on Terminate | Rewritten to use TerminateGame (PC-runnable) |

**Result**: ✅ PASSED — marked `pc-grts: passing`.

## Success Criteria

- ✅ Every `pc-grts: failing` test has been run, analyzed, and either:
  - Fixed (YAML updated to `pc-grts: passing`, confirmed with passing run)
  - Filed (bug report in `C:\git\PlayFab.C\specs\playfab-gamesave\xplat-testing\status\failing-tests\` with evidence)
- ✅ `--passing` dry-run count increased: 69 → 81
- ✅ Zero tests in limbo — every failure has documented root cause and owner

## Final Results

| Group | Tests | Fixed→Passing | Marked Ignore | Bug Filed |
|-------|-------|---------------|---------------|-----------|
| A: YAML/Config | 42, 108, 117 | 3 | 0 | 0 |
| B: Engine Routing | 25, 27-31 | 6 | 0 | 0 |
| C: SDK/Controller | 03, 26, 84b, 85b | 0 | 1 (26) | 1 (03) |
| D: Cross-Engine | 10, 12, 13, 16, 23, 24 | 0 | 6 | 1 (consolidated) |
| E: Descriptor/Quota | 37-41, 43 | 0 | 6 | 0 |
| F: Storage Full | 44, 45, 48 | 0 | 3 | 0 |
| G: Cancel/Edge | 33, 34, 87, 89 | 0 | 4 | 0 |
| H: Auth/Network | 76, 77, 78, 80 | 2 (76, 78) | 2 (77, 80) | 0 |
| I: Xbox-Only | 52, 54-58, 64, 66, 113 | 0 | 9 | 0 |
| J: Termination | 102 | 1 | 0 | 0 |
| **Total** | **46** | **12** | **31** | **2** |

Note: 2 tests (84b, 85b) were deleted. Test 03 remains `pc-grts: failing` with bug filed.

## Output Files

- Bug reports: `C:\git\PlayFab.C\specs\playfab-gamesave\xplat-testing\status\failing-tests\`
  - `test-03-background-upload-stuck.md`
  - `test-26-grts-retry-hangs.md`
  - `group-d-cross-engine-contention-conflict.md`
  - `grts-behavioral-limitations.md` (consolidated report for all 32 ignores)
- This plan: `specs\playfab-gamesave\xplat-testing\debug-failing-tests-plan.md`

# Xbox GameSave Test Audit Plan

## Goal

Run every test in `Test/GameTestScenarios/gamesave-xbox/` one at a time, capture results, and produce a status report documenting pass/fail/skip for each scenario, with failure analysis and log evidence proving correctness.

## Scope

- **113 scenarios** in `gamesave-xbox/`
- **DeviceA** = Xbox devkit (10.0.0.104), **DeviceB** = local Windows PC (GameTestAppWindows)
- All tagged `xboxgamesaves`
- Run with `Run-GameSaveXboxTests.ps1 -SkipCleanup -RunScenario <filename>`

### Key difference from XPlat audit

On Xbox, `ChangeTargetDeviceState` **works** — the controller uses `xbstress.exe`, `xbreboot.exe`, and other GDK tools to manipulate the Xbox devkit directly. The 55 tests that were SKIP on PC-GRTS are **runnable** here.

## Prerequisites

Complete these steps before running any test scenarios.

### 1. Windows Firewall — Allow inbound TCP 5000

The GameTestController hosts a WebSocket server on port 5000. The Xbox app connects to this port over the network. Windows Firewall blocks this by default, which silently prevents the Xbox from connecting (the app launches but never registers as a device).

Run once in an **elevated** PowerShell:

```powershell
netsh advfirewall firewall add rule name="GameTestController WebSocket (TCP 5000)" dir=in action=allow protocol=TCP localport=5000
```

**Verify** (no elevation required):

```powershell
netsh advfirewall firewall show rule name="GameTestController WebSocket (TCP 5000)"
```

> **Symptom if missing**: Controller logs show `Waiting up to 120s for devices to connect...` and only the PC app (DeviceA via 127.0.0.1) connects. The Xbox never appears as DeviceB. Single-device tests may hang on `PFGameSaveFilesAddUserWithUiAsync` because they fall back to PC-GRTS, which shows system-level UI dialogs that cannot be dismissed programmatically.

### 2. Xbox devkit is reachable and operational

```powershell
xbconnect <ip>          # Host and System fields must show version numbers (not "unreachable")
xbconnect <ip> /ws      # Blocks until system is ready (use after reboot/sflash)
```

If unresponsive: `xbreboot /x:<ip>`. If that fails: `sflash /x:<ip>` to reflash the OS.

### 3. Verify no leftover `xbstress` simulation is running

Network disruption tests use `xbstress simulate network=broken` which persists across app launches. If a previous test run was interrupted, the simulation may still be active, silently blocking all title-level network traffic on the Xbox.

```powershell
xbstress status /X:<ip>     # Should show "Stress is not running"
xbstress stop /X:<ip>       # Stop any active simulation
```

> **Symptom if active**: `xbconnect` and `xbapp` work (they use the Host OS dev channel), but the game app cannot make any network connections — WebSocket, HTTP, or even ICMP from the Title OS. Ping from the Xbox System OS also fails.

### 3. Build and deploy binaries

```powershell
Utilities\Scripts\tests-build.ps1                                    # Build Windows x64 + Xbox
xbapp deploy "Out\Gaming.Xbox.Scarlett.x64\Debug\GameTestAppXbox"    # Deploy to devkit
```

### 4. Verify `controllerip.txt` on Xbox

The Xbox app reads `controllerip.txt` at startup to find the controller's IP. After deploying, confirm it contains the correct PC IP address (the machine running the controller).

## Rules

These rules are mandatory for all sessions continuing this audit.

### Rule 1: One test at a time, in order — NO BATCHING

Run tests in strict scenario-number order (01, 02, 03, ...). **Never batch multiple tests in one run.** Each test must complete (pass, fail, or timeout) before starting the next. This is critical for clean log isolation and accurate reporting.

### Rule 2: Clean environment per test

Before each test:
1. Kill any lingering `GameTestController` and `GameTestAppWindows` processes
2. Then run: `Run-GameSaveXboxTests.ps1 -SkipCleanup -RunScenario "<filename>.yml"`

### Rule 3: Update the report after EVERY test

After each test, **immediately** update `status-report.md` before moving to the next test. Never batch updates. The report is the source of truth and must always reflect the latest state.

### Rule 4: Log evidence for passing tests

A "PASS" is not just "all commands returned hr=0x00000000". For each passing test:
1. **Read the YAML scenario** to understand what the test claims to verify.
2. **Read the controller log** to confirm the interesting behavior actually happened.
3. **Add a Log Evidence subsection** (at the bottom of the report, under `## Log Evidence`) with:
   - **Claim**: What the test says it tests (from the YAML `description` field)
   - **Key log lines**: Extracted from controller log showing the behavior occurred
   - **Verdict**: Does the test actually prove what it claims? Note any gaps.

The evidence section uses this format:
```markdown
### Test NN
**Claim**: <from YAML description>
**Key log lines**:
```
<extracted log lines>
```
**Verdict**: ✅ / ⚠️ description of confidence level and gaps
```

### Rule 5: Failure documentation

For failing tests, update the table row with:
- The specific command that failed
- The HRESULT returned vs. expected (if applicable)
- A one-line root cause if obvious

Also update the **Known Failure Patterns** table if the failure matches or creates a pattern.

**Exception — trivial YAML/scenario bugs:** If the failure is caused by a trivial bug in the test YAML itself (not an SDK or handler bug), **fix the YAML and re-run the test** rather than just documenting it. Examples of trivial YAML bugs: wrong verb name, missing required parameter, wrong field name, typo in a parameter value. After fixing, reset the test to ⬜ TODO, re-run, and update the report with the new result.

**Do NOT fix** SDK bugs, handler implementation gaps, or test logic issues during the audit — just document those.

### Rule 6: Timeout handling

If a test runs for more than 10 minutes with no progress, kill it and record as FAIL with the reason.

### Rule 7: Summary counts

Keep the summary table at the top of the report accurate after every update. The four counts must always sum to 113.

### Rule 8: ChangeTargetDeviceState — RUNNABLE on Xbox

Unlike the XPlat audit, `ChangeTargetDeviceState` **works** on Xbox devkits via the controller's `DeviceStateController.cs`. Tests using DisableNetwork, Suspend, Terminate, Reboot, etc. should be attempted. Only SKIP if the specific action is truly unsupported.

### Rule 9: Update YAML `xbox:` tag when tests pass

When a test passes, update its YAML file's `platforms:` section:
```yaml
platforms:
  xbox: passing    # was 'untested' or 'failing'
```

### Rule 10: YAML bug fixes

When a test fails due to a YAML scenario bug (not an SDK bug), fix the YAML and re-run. Known patterns from the XPlat audit that may also apply:
- `verb: CreateBinaryFile` with `content:` field → change verb to `CreateTextFile`
- `CaptureSaveContainerSnapshot` missing `slot` parameter → add `slot: left` or `slot: right`
- `bytesMin:`/`bytesMax:` → replace with single `bytes:` value
- `expectedError: "SYMBOLIC_NAME"` → use `expectedHr: "0xHEXVALUE"`

### Rule 11: Xbox reboot recovery

After tests that reboot/crash the Xbox (timeouts, ChangeTargetDeviceState), verify Xbox connectivity with `xbconnect 10.0.0.104` before continuing. Redeploy with `xbapp deploy` if needed.

### Rule 12: Take your time

This is a long, methodical process. Don't rush. Read logs carefully. Extract meaningful evidence. Update the report accurately. Quality over speed.

## Execution Commands

```powershell
# Verify Xbox connectivity
$env:Path += ";${env:GameDK}\bin"
xbconnect 10.0.0.104

# Kill leftover processes
Get-Process GameTestController, GameTestAppWindows -ErrorAction SilentlyContinue | Stop-Process -Force

# Run a single test
cd D:\git\PlayFab.C
Utilities\Scripts\tests\xbox\Run-GameSaveXboxTests.ps1 -SkipDeploy -SkipCleanup -RunScenario "<filename>.yml"

# Deploy fresh build (after code changes)
xbapp deploy "Out\Gaming.Xbox.Scarlett.x64\Debug\GameTestAppXbox"

# Read results
Get-Content "Out\XboxTestResults\pfgamesaves-test-Controller-log.txt" | Select-String "PASS|FAIL|Scenario.*failed"
```

## Environment

- Xbox devkit: 10.0.0.104, Sandbox: XDKS.1
- DeviceA: Xbox (GameTestAppXbox)
- DeviceB: Local Windows PC (GameTestAppWindows)
- Controller: GameTestController (x64 Debug)
- Custom ID prefix: MWEDDLE56-xbox-
- Build: `Gaming.Xbox.Scarlett.x64 Debug` + `x64 Debug` (both platforms needed)

## Output Files

- `specs/playfab-gamesave/xbox-testing/plan.md` — this file
- `specs/playfab-gamesave/xbox-testing/status-report.md` — full audit results with evidence

## Progress

Audit in progress. Starting from scenario 01 in strict order.

### Handler fixes applied this session
- Added `PlayOffline` to conflict action map
- Added `GetJsonBool()` helper for YAML boolean-as-string handling
- Added `GetJsonInt64()`, `GetJsonDouble()`, `GetJsonUint64()` for numeric-as-string
- Added global try-catch in `BuildActionResult` for crash prevention
- Added 7 missing handlers: `VerifyContentionDescriptor`, `VerifyConflictDescriptor`, `VerifyQuotaDelta`, `PFGameSaveFilesSetUiProgressAutoWriteOnUpload`, `SetExpiredEntityToken`, `PFGameSaveFilesSetUiSyncConflictAutoResponse` (alias)
- Added `recordAs` support to `PFGameSaveFilesGetRemainingQuota`

### Known YAML bugs from XPlat audit (may need same fixes)
- `CreateBinaryFile` with `content:` → needs `CreateTextFile`
- `CaptureSaveContainerSnapshot` missing `slot` param
- `bytesMin`/`bytesMax` → needs single `bytes:`
- `expectedError:` → needs `expectedHr:`

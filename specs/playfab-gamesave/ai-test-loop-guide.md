# AI Build-Test-Fix Loop Guide for PFGameSaves

This guide explains how an AI agent (e.g. GitHub Copilot, Cursor, or any LLM-powered coding assistant) can perform an automated **build → test → read logs → fix → rebuild → retest** loop against the PFGameSave SDK using the existing headless test harness.

## Architecture Overview

The test system has three components:

```
┌─────────────────────────────────────────────────────┐
│  Test Controller (C# .NET WinForms + CLI)           │
│  - Hosts WebSocket server on port 15080              │
│  - Loads & orchestrates scenario YAML files         │
│  - Headless mode for CI and AI automation           │
│  - Reports results as JSON + JUnit XML              │
└────────────┬────────────────────────┬───────────────┘
             │ WebSocket              │ WebSocket
    ┌────────▼─────────┐    ┌────────▼─────────┐
    │  Device A (C++)  │    │  Device B (C++)  │
    │  PFGameSave SDK  │    │  PFGameSave SDK  │
    │  Executes cmds   │    │  (multi-device   │
    │  from controller │    │   scenarios)     │
    └──────────────────┘    └──────────────────┘
```

- **Controller** (`GameTestController.exe`): A C# app that reads scenario YAML files, sends commands to devices via WebSocket, validates results, and writes test reports.
- **Device** (`GameTestAppWindows.exe`): A C++ app that links the PFGameSave SDK, connects to the controller, and executes commands (API calls, file operations, snapshots).
- **Scenarios** (`Test/GameTestScenarios\*.yml`): YAML files that define test sequences, one per scenario.

## Prerequisites

- Visual Studio 2022 with C++ desktop workload and GDK components
- .NET SDK (for building the C# test controller)
- MSBuild on PATH
- Administrator access (for registry keys)
- **For Xbox testing**: Windows Firewall inbound rule allowing `GameTestController.exe` or TCP port 15080 (see [Xbox Prerequisites](#prerequisites-1))

## Helper Scripts

PowerShell scripts are provided in `Utilities\Scripts\`:

### PC-Only Testing (In-Process Mode)

| Script | Purpose |
|--------|---------|
| [`gamesave-build.ps1`](../../Utilities/Scripts/gamesave-build.ps1) | Builds the SDK for x64 (PC only) |
| [`tests-run.py`](../../Utilities/Scripts/tests-run.py) | Unified test runner — runs scenarios headlessly with per-test isolation |
| [`read-test-logs.ps1`](../../Utilities/Scripts/read-test-logs.ps1) | Parses results JSON and log files for diagnosis |

### Xbox + PC Testing (GRTS Mode)

| Script | Purpose |
|--------|---------|
| [`gamesave-build-xbox.ps1`](../../Utilities/Scripts/gamesave-build-xbox.ps1) | Builds for Xbox (Scarlett) and PC (x64) |
| [`gamesave-launch-xbox.ps1`](../../Utilities/Scripts/gamesave-launch-xbox.ps1) | Deploys to Xbox, launches controller + devices |

All scripts auto-detect paths relative to the repo root. Run from any directory.

---

## Step-by-Step: The Build-Test-Fix Loop

### Step 1: Build

```powershell
& C:\git\PlayFab.C\Utilities\Scripts\gamesave-build.ps1
```

**On failure**: The script extracts MSBuild error lines. Fix compile errors in the reported files, then re-run.

Use `-Clean` for a clean rebuild if you suspect stale objects:
```powershell
& C:\git\PlayFab.C\Utilities\Scripts\gamesave-build.ps1 -Clean
```

### Step 2: Run Tests

Run specific scenarios by ID:
```powershell
# Single scenario
py C:\git\PlayFab.C\Utilities\Scripts\tests-run.py gamesave-inproc --only 41

# Multiple scenarios (comma-separated IDs)
py C:\git\PlayFab.C\Utilities\Scripts\tests-run.py gamesave-inproc --only 01,02
```

Run all GameSave scenarios:
```powershell
py C:\git\PlayFab.C\Utilities\Scripts\tests-run.py gamesave-inproc
```

Run all scenarios, stop on first failure (recommended for AI agents):
```powershell
py C:\git\PlayFab.C\Utilities\Scripts\tests-run.py gamesave-inproc --stop-on-fail
```

Resume from a specific test after fixing a failure:
```powershell
py C:\git\PlayFab.C\Utilities\Scripts\tests-run.py gamesave-inproc --stop-on-fail --start 09
```

**Exit codes from the controller**:
| Code | Meaning |
|------|---------|
| 0 | All tests passed |
| 1 | One or more tests failed |
| 2 | Configuration error (bad args, missing scenarios) |
| 3 | Timeout waiting for devices to connect |
| 4 | Fatal error (unhandled exception) |

### Step 3: Read Logs on Failure

```powershell
& C:\git\PlayFab.C\Utilities\Scripts\read-test-logs.ps1 -FailedOnly
```

This reads `Out\logs\` and outputs:
1. **Results summary** -- total/passed/failed/skipped from `test-results.json`
2. **Failed scenario details** -- error messages, failed commands, HRESULTs
3. **Controller log** -- the orchestration trace showing what happened
4. **Device stdout** -- SDK TRACE output with HRESULT codes and stack context
5. **Diagnosis summary** -- actionable next-step hints

### Step 4: Fix and Repeat

Based on the log output:
1. Identify the failing scenario and the command that failed
2. Search the source code for the relevant HRESULT or error message
3. Make the fix in the source files under `Source\PlayFabGameSave\`
4. Go back to **Step 1** (build) and **Step 2** (re-run the failing scenario)

### Step 4a: Stop-on-First-Error Loop (Recommended for AI Agents)

When running many tests, use `--stop-on-fail` to stop at the first failure and debug it:

```
Loop:
  1. Build: gamesave-build.ps1
  2. Run:   py tests-run.py gamesave-inproc --stop-on-fail --start N
  3. If all pass → done
  4. Read logs: read-test-logs.ps1 -FailedOnly
  5. Fix the failing test
  6. Set N = ID of the failing test
  7. Go to 1, passing --start N to skip already-passed tests
```

- `--stop-on-fail` stops execution after the first failure, marking remaining scenarios as skipped
- `--start N` starts from test ID N (the one that failed), saving time on re-runs
- After fixing the last failure, do a final full run without `--start` to confirm everything passes

---

## Scenario Reference

There are 51 scenario YAML files in `Test/GameTestScenarios\`. The key tags are:

### `passing` -- Known-good scenarios (use for regression testing)

| File | Description |
|------|-------------|
| `scenario-01-single-device-golden-path.yml` | Initialize, upload, relaunch, verify |
| `scenario-02-two-device-golden-path.yml` | Two devices sync same save |
| `scenario-03-offline-to-online-reconnect.yml` | Offline writes, reconnect, sync |
| `scenario-04-active-device-handoff.yml` | Active device ownership transfer |
| `scenario-05a-large-dataset-text.yml` | Large text payload sync |
| `scenario-05b-large-dataset-binary.yml` | Large binary payload sync |
| `scenario-06-conflict-local-wins.yml` | Conflict resolved with local data |
| `scenario-07-conflict-cloud-wins.yml` | Conflict resolved with cloud data |
| `scenario-08-multi-atomic-conflict.yml` | Multi-file atomic conflict resolution |
| `scenario-11-upload-interruption.yml` | Upload interrupted, recovery |
| `scenario-13-user-canceled-recovery.yml` | User cancels sync, state recovery |
| `scenario-22-file-folder-deletion-propagation.yml` | Delete propagation across devices |
| `scenario-23-mutating-during-upload-guard.yml` | Guard against mutation during upload |
| `scenario-24-online-to-offline-recovery.yml` | Online→offline→recovery |
| `scenario-26-setsavedescription.yml` | SetDescription API |
| `scenario-27-progress-cancel.yml` | Progress reporting and cancellation |
| `scenario-28-set-description-offline-after-adduser.yml` | Offline description after AddUser |
| `scenario-29-set-description-offline-before-adduser.yml` | Offline description before AddUser |
| `scenario-30-ui-callback-wait-hang.yml` | UI callback hang detection |
| `scenario-31-upload-fail-syncstate-reset.yml` | Upload failure state reset |
| `scenario-32-uninitialize-hang-no-ui-response.yml` | Uninitialize hang without UI response |
| `scenario-33-PFGameSaveFilesUninitializeAsync.yml` | UninitializeAsync API |
| `scenario-34-setdescription-offline-after-saves.yml` | Offline description after saves |
| `scenario-35-conflict-ui-metadata.yml` | Conflict UI metadata |
| `scenario-36-cloud-description-not-dirty.yml` | Cloud description dirty flag |
| `scenario-37-telemetry-crash-offline-login-fail.yml` | Telemetry after offline login fail |
| `scenario-38-upload-cancel-manifest-blocked.yml` | Upload cancel with manifest blocked |
| `scenario-39-upload-hang-relock-loop.yml` | Upload hang in relock loop |
| `scenario-40-takelock-stale-manifest-after-conflict.yml` | Stale manifest after conflict |
| `scenario-41-deleted-folder-upload-noop.yml` | Deleted folder no longer triggers unnecessary uploads (bug 61594760) |

### `failing` -- Known failures (use to verify fixes)

| File | Description |
|------|-------------|
| `scenario-09-rollback-last-known-good.yml` | Rollback to last known good state |
| `scenario-10-rollback-last-conflict.yml` | Rollback to last conflict state |
| `scenario-12-out-of-storage.yml` | Out-of-storage handling |
| `scenario-14-concurrent-upload-arbitration.yml` | Concurrent upload conflict |
| `scenario-15-disk-exhaustion-recovery.yml` | Disk full recovery |
| `scenario-16-large-payload-incremental-sync.yml` | Incremental large sync |
| `scenario-17-quota-limit-handling.yml` | Service quota limits |
| `scenario-18-high-file-count-upload.yml` | Many small files |
| `scenario-19-filesystem-boundary-validation.yml` | Path/name limits |
| `scenario-20-manifest-corruption-detection.yml` | Manifest corruption handling |
| `scenario-21-interrupted-download-resume.yml` | Download resume after interrupt |
| `scenario-25-long-running-sync-soak.yml` | Soak/endurance test |

### Other tags

| Tag | Count | Description |
|-----|-------|-------------|
| `harness` | 6 | Controller self-tests (no SDK code) |
| `grts` | 4 | Gaming Runtime Services tests (need GRTS) |
| `interactive` | 3 | Require manual UI interaction |
| `chaos` | 1 | Chaos/fault injection test |

---

## AI Agent Workflow: Complete Example

Here is a concrete example of how an AI agent should execute the loop:

```
AI AGENT SESSION
────────────────────────────────────────────────────

1. BUILD
   > & .\Utilities\Scripts\gamesave-build.ps1
   ✓ BUILD SUCCEEDED

2. RUN BASELINE (all passing scenarios)
   > py Utilities\Scripts\tests-run.py gamesave-inproc
   ✗ Exit code 1 -- 24/26 passed, 2 failed

3. READ LOGS
   > & .\Utilities\Scripts\read-test-logs.ps1 -FailedOnly
   
   [FAIL] scenario-06-conflict-local-wins.yml
          Command: PFGameSaveFilesSyncChanges
          Error: 0x80004005 (E_FAIL)
          Failed Step: compare-local-and-cloud
   
   [FAIL] scenario-35-conflict-ui-metadata.yml
          Command: PFGameSaveFilesSyncChanges
          Error: 0x80004005 (E_FAIL)

4. DIAGNOSE
   Both failures are in conflict scenarios during CompareStep.
   Search source for the error:
   > grep -rn "E_FAIL" Source\PlayFabGameSave\Source\SyncManager\CompareStep.cpp
   
   Found: conflictFound flag is being reset incorrectly.

5. FIX
   Edit Source\PlayFabGameSave\Source\SyncManager\CompareStep.cpp
   Remove the incorrect `conflictFound = false;` line.

6. REBUILD
   > & .\Utilities\Scripts\gamesave-build.ps1
   ✓ BUILD SUCCEEDED

7. RETEST (just the failing scenarios)
   > py Utilities\Scripts\tests-run.py gamesave-inproc --only 06
   ✓ PASSED
   
   > py Utilities\Scripts\tests-run.py gamesave-inproc --only 35
   ✓ PASSED

8. FULL REGRESSION
   > py Utilities\Scripts\tests-run.py gamesave-inproc
   ✓ 26/26 passed
```

---

## Xbox + PC Testing (GRTS Mode)

For testing with real Xbox hardware using Gaming Runtime Services (GRTS), use the Xbox-specific scripts. This is required for:
- Two-device scenarios with Xbox console
- Testing GRTS provider behavior (system-managed save paths)
- Xbox-specific features like conflict resolution UI

### Prerequisites

- Xbox Dev Kit with development mode enabled
- GDK installed with `xbapp.exe` in `C:\Program Files (x86)\Microsoft GDK\bin`
- Console set as default via `xbconfig` or use `-XboxConsole` parameter
- **Windows Firewall**: The controller listens on port 15080 for Xbox WebSocket connections. An inbound firewall rule must allow the controller executable or the port. Run as Administrator:
  ```powershell
  # Option A: Allow by program (recommended)
  netsh advfirewall firewall add rule name="GameTestController" dir=in action=allow program="C:\git\PlayFab.C\Out\x64\Debug\GameTestController\GameTestController.exe" protocol=TCP profile=private,public enable=yes

  # Option B: Allow by port
  netsh advfirewall firewall add rule name="GameTestController Port 15080" dir=in action=allow protocol=TCP localport=15080 profile=private,public enable=yes
  ```
  **Symptom if missing**: Xbox app logs show `[WSAutoConnect] Connect attempt stuck for >15s` and the controller reports `no device with engine 'xbox' connected within 60 seconds`.

### Quick Start

```powershell
# Build and launch everything in one command
.\Utilities\Scripts\gamesave-launch-xbox.ps1 -Build

# Build and run headless with specific scenario
.\Utilities\Scripts\gamesave-launch-xbox.ps1 -Build -Headless -Scenario xbox-02
```

### Build for Xbox

```powershell
# Build for both Xbox (Scarlett) and PC (x64)
.\Utilities\Scripts\gamesave-build-xbox.ps1

# Clean build
.\Utilities\Scripts\gamesave-build-xbox.ps1 -Clean

# Release configuration
.\Utilities\Scripts\gamesave-build-xbox.ps1 -Configuration Release
```

This builds:
- `Gaming.Xbox.Scarlett.x64` — Xbox Series X|S test app
- `x64` — PC test app and controller

### Launch Test Environment (GUI Mode)

```powershell
# Launch controller + Xbox + PC devices
.\Utilities\Scripts\gamesave-launch-xbox.ps1

# Build first, then launch
.\Utilities\Scripts\gamesave-launch-xbox.ps1 -Build

# Specify Xbox console by name/IP
.\Utilities\Scripts\gamesave-launch-xbox.ps1 -XboxConsole "XBOXONE123"

# Skip deploy (already deployed)
.\Utilities\Scripts\gamesave-launch-xbox.ps1 -SkipDeploy

# Controller only (no devices)
.\Utilities\Scripts\gamesave-launch-xbox.ps1 -ControllerOnly
```

The script:
1. Kills existing processes (controller, PC device, Xbox app)
2. Optionally builds (`-Build`)
3. Launches GameTestController
4. Deploys and launches Xbox app via `xbapp deploy` / `xbapp launch`
5. Waits 3 seconds for Xbox to initialize
6. Launches PC test app

### Run Tests Headless (Xbox)

```powershell
# Run specific scenario headless
.\Utilities\Scripts\gamesave-launch-xbox.ps1 -Headless -Scenario xbox-02

# Run all Xbox scenarios by tag
.\Utilities\Scripts\gamesave-launch-xbox.ps1 -Headless -Tag xboxgamesaves

# Build and run headless
.\Utilities\Scripts\gamesave-launch-xbox.ps1 -Build -Headless -Scenario xbox-02
```

Results are written to `Out\logs\` just like PC-only tests.

### Xbox Scenario Tags

| Tag | Description |
|-----|-------------|
| `xboxgamesaves` | Xbox-specific game save scenarios (require Xbox device) |
| `gamesaves` | All game save scenarios (PC + Xbox) |

### Xbox Test Scenarios

| Scenario | Description |
|----------|-------------|
| `gamesave-xbox-01-single-device-golden-path.yml` | Single Xbox device upload/download |
| `gamesave-xbox-02-two-device-golden-path.yml` | Xbox + PC sync with account linking |
| `gamesave-xbox-03-two-device-spop.yml` | Xbox + PC with SPOP (same Xbox account) |

### Account Linking for Two-Device Tests

When testing with two devices (Xbox + PC or two Xboxes), use account linking to share cloud data without SPOP issues. See [`testing/xbox-two-device-linking-flow.md`](testing/xbox-two-device-linking-flow.md) for details.

The flow:
1. Sign into Xbox Live (`XUserAddAsync`)
2. Login with shared CustomID (`PFAuthenticationLoginWithCustomIDAsync`)
3. Link Xbox to CustomID entity (`PFAccountManagementClientLinkXboxAccountAsync`)
4. Create local user from Xbox (`PFLocalUserCreateHandleWithXboxUser`)

This allows different Xbox accounts on each device to share the same PlayFab entity and cloud saves.

---

## Registry Keys

The test harness requires two registry keys to run in in-process mode (no GDK Gaming Services needed):

```
HKLM:\SOFTWARE\Microsoft\GamingServices
    ForceUseLocalServices    = 1  (DWORD)
    ForceUseInprocGameSaves  = 1  (DWORD)
```

The `tests-run.py` script sets the correct engine automatically when running GameSave suites. For in-process testing, you may need to set registry keys manually (requires Administrator). They can be cleaned up after tests complete.

## Output Locations

| Artifact | Path |
|----------|------|
| Test results JSON | `Out\logs\test-results.json` |
| Controller log | `Out\logs\controller.log` |
| Controller stdout | `Out\logs\controller-stdout.txt` |
| Controller stderr | `Out\logs\controller-stderr.txt` |
| Device A stdout | `Out\logs\DeviceA-stdout.txt` |
| Device B stdout | `Out\logs\DeviceB-stdout.txt` |
| Device runtime logs | `Out\logs\*.log` (copied from device working dir) |

## Test Results JSON Format

```json
{
  "summary": {
    "total": 26,
    "passed": 24,
    "failed": 2,
    "skipped": 0
  },
  "scenarios": [
    {
      "id": "Scenario-01-Single-Device-Golden-Path",
      "name": "1. Single-Device Golden Path Sync",
      "status": 0,
      "durationSeconds": 12.3
    },
    {
      "id": "Scenario-06-Conflict-Local-Wins",
      "name": "6. Conflict - Local Wins",
      "status": 1,
      "error": "Command PFGameSaveFilesSyncChanges failed: 0x80004005",
      "failedStep": "compare-local-and-cloud",
      "failedCommand": "PFGameSaveFilesSyncChanges",
      "durationSeconds": 5.1
    }
  ]
}
```

Status values: `0` = Passed, `1` = Failed, `2` = Skipped.

## Common Failure Patterns

| Pattern in logs | Likely cause | Where to look |
|----------------|--------------|---------------|
| `0x80004005` (E_FAIL) | Generic failure in SDK operation | Device stdout for the TRACE line preceding the error |
| `0x80070057` (E_INVALIDARG) | Null or invalid parameter | API validation in `PFGameSaveFilesAPI.cpp` |
| `0x8007000E` (E_OUTOFMEMORY) | Allocation failure / oversized file | `Utils.cpp` ReadEntireFile or FileFolderSet |
| `Exit code 3` | Devices didn't connect in time | Check device stderr; maybe build failed |
| `Exit code 2` | Bad CLI arguments or missing scenarios | Check controller stderr for the config error |
| `HRESULT_FROM_WIN32(ERROR_PATH_NOT_FOUND)` | Missing directory | `CreatePath` calls in the step classes |
| Timeout in scenario step | Operation hung | UI callback deadlock or infinite loop in step |

## Tips for AI Agents

1. **Start with `scenario-01`** -- it's the simplest golden-path test. If it passes, the SDK basics work.
2. **Run failing scenarios individually** -- don't re-run the entire `passing` suite when iterating on a fix.
3. **Read Device stdout first** -- it has SDK TRACE output with file/line info and HRESULTs.
4. **Controller log shows orchestration** -- it tells you which command was sent and what response came back.
5. **The source code is at** `Source\PlayFabGameSave\Source\` -- that's where fixes go.
6. **Public headers are at** `Source\PlayFabGameSave\Include\playfab\gamesave\` -- these define the API surface.
7. **After fixing, always rebuild before re-running** -- the test device is a native C++ binary.
8. **Use `-Clean` build if you change headers** -- PCH and incremental builds can miss header changes.

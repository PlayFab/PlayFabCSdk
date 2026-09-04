# PFGameSave Cross-Platform (XPlat) Testing Guide

> For AI coding agents working on PC GRTS testing. Covers infrastructure, scripts, test status, known bugs, and failure patterns.  
> See also: [GRTS Internals Guide](ai-grts-internals.md) — how the GRTS service works under the hood (state machines, on-disk state, error codes, debugging).

## What Is XPlat Testing?

The xplat test suite validates PFGameSave behavior on the **PC GRTS** (Gaming Runtime Services) engine — the out-of-process provider that Xbox and Windows use for game saves. Tests run via the same controller+device harness as the inproc tests, but exercise the GRTS code path where sync happens through `xgameruntime.dll` instead of the SDK's built-in `FolderSyncManager`.

Key differences from inproc testing:
- **Out-of-process upload**: GRTS manages uploads in a separate OS process (continues after game exit)
- **Stock TCUI**: GRTS provides platform-owned dialog UI for sync failures, contention, and conflicts
- **System-managed save paths**: GRTS controls where saves land on disk (`C:\XboxGames\GameSave\...`)
- **Different error codes**: GRTS returns its own error codes (e.g., `E_GS_USER_CANCELED` 0x80830004) instead of SDK error codes

## Architecture

```
┌─────────────────────────────────────────────────┐
│  GameTestController (C# .NET, headless CLI)     │
│  Loads YAML scenarios, sends WebSocket commands  │
│  Auto-navigates GRTS stock TCUI dialogs          │
│  Writes test-results.json + controller.log       │
└──────────┬──────────────────────┬───────────────┘
           │ WebSocket            │ WebSocket
  ┌────────▼────────┐    ┌───────▼─────────┐
  │  DeviceA (C++)  │    │  DeviceB (C++)  │
  │  pc-grts engine │    │  pc-grts or     │
  │  (xgameruntime) │    │  pc-inproc      │
  └─────────────────┘    └─────────────────┘
```

The controller has a **GameSaveUiNavigator** that uses UI Automation to detect GRTS TCUI dialogs (SyncFailedPage, ContentionPage, ConflictPage, SyncProgressPage) and clicks buttons automatically based on YAML-configured auto-responses.

## Scripts & Commands

All scripts are in `Utilities\Scripts\` and auto-detect repo paths. Run from any directory.

### Build

```powershell
# Build test apps (GameTestAppWindows + GameTestController)
Utilities\Scripts\tests-build.ps1            # Debug build
Utilities\Scripts\tests-build.ps1 -Clean     # Clean rebuild
```

Output binaries:
- Controller: `Out\x64\Debug\GameTestController\GameTestController.exe`
- Device: `Out\x64\Debug\GameTestAppWindows\GameTestAppWindows.exe`

### Run Tests

The main runner is `tests-run.py`. It runs scenarios one-by-one, collecting per-test logs.

```powershell
# Run all xplat tests
py Utilities\Scripts\tests-run.py gamesave-pc

# Run stable regression suite (tests annotated 'pc-grts: passing')
py Utilities\Scripts\tests-run.py gamesave-pc --passing

# Run specific tests by ID
py Utilities\Scripts\tests-run.py gamesave-pc --only 01,02,14

# Start from a specific test
py Utilities\Scripts\tests-run.py gamesave-pc --start 50

# Re-run known-failing tests
py Utilities\Scripts\tests-run.py gamesave-pc --rerun

# Dry run (show what would execute)
py Utilities\Scripts\tests-run.py gamesave-pc --dry-run

# Custom timeout per test (default: 600s)
py Utilities\Scripts\tests-run.py gamesave-pc --timeout 300
```

### Combine Results Across Passes

Each run creates a `pass<N>` folder under `Out\gamesave-pc-tests\`. The combine script merges all passes into a single summary:

```powershell
python Utilities\Scripts\combine-xplat-results.py
```

Output: `Out\gamesave-pc-tests\combined-summary.csv`

Priority logic: if any pass has PASS for a test → use that; otherwise use the latest failing result.

### Read Logs After Failure

```powershell
# Per-test logs in:
Out\gamesave-pc-tests\pass<N>\<test-id>\controller.log      # Controller orchestration trace
Out\gamesave-pc-tests\pass<N>\<test-id>\test-results.json    # Machine-readable result
Out\gamesave-pc-tests\pass<N>\<test-id>\controller-stdout.txt
Out\gamesave-pc-tests\pass<N>\<test-id>\device-*.log         # Device SDK trace output
```

## Test Infrastructure

### Python Modules

| File | Role |
|------|------|
| `tests-run.py` | Unified test runner — `py tests-run.py gamesave-pc --passing`, `--rerun`, `--quick`, etc. |
| `testrunner.py` | Shared infrastructure: process management, log collection, single-test execution, result parsing, CSV output |
| `combine-xplat-results.py` | Merges pass1..passN folders into `combined-summary.csv` with best-result-per-test logic |

### How `testrunner.py` Works

1. **Kill leftover processes** — terminates `GameTestController.exe` and `GameTestAppWindows.exe` between tests (leaves `AdminHelper` alone for network control)
2. **Launch controller** — headless mode with `--run-scenario`, `--allowed-engines pc-grts,pc-inproc`, device wait timeout 30s
3. **Wait for completion** — up to `--timeout` seconds per test
4. **Collect logs** — copies `Out\logs\*` into `pass<N>\<test-id>\`
5. **Read result** — parses `test-results.json` for PASS/FAIL/SKIP status

### YAML Scenario Format

Scenarios live in `Test\GameTestScenarios\gamesave-pc\`. Each has platform annotations:

```yaml
id: Scenario-PC-01-Single-Device-Golden-Path
name: GameSave PC 01 - Single-Device Golden Path Sync
tags:
  - gamesave-pc
platforms:
  xbox: passing        # Status on real Xbox hardware
  pc-grts: passing     # Status on PC GRTS engine
  pc-inproc: ignore    # Not applicable (GRTS-only test)
  psx: ignore          # Not applicable
devices:
  DeviceA:
    engine: [xbox, pc-grts]   # Which engines can run this role
```

Platform status values: `passing`, `failing`, `ignore` (skip), `untested`

The `--passing` flag in `tests-run.py` reads the `pc-grts:` annotation from each YAML file and only runs tests marked `passing`.

## Quick Start for AI Agents

### 1. Run the stable regression suite

```powershell
py Utilities\Scripts\tests-run.py gamesave-pc --passing
```

This runs the 66 tests marked `pc-grts: passing`. If any regress, investigate immediately.

### 2. Build → Test → Fix loop

```
1. Build:   Utilities\Scripts\tests-build.ps1
2. Run:     py Utilities\Scripts\tests-run.py gamesave-pc --only <test-id>
3. Logs:    Out\gamesave-pc-tests\pass<N>\<test-id>\controller.log
4. Fix:     Edit source under Source\PlayFabGameSave\
5. Repeat from step 1
```

### 3. After a round of testing, combine results

```powershell
python Utilities\Scripts\combine-xplat-results.py
# Output: Out\gamesave-pc-tests\combined-summary.csv
```

### 4. Before investigating a failure, check if it's a known bug

Look in:
- `C:\git\PlayFab.C\specs\playfab-gamesave\xplat-testing\status\failing-tests\` — per-test analysis files
- `C:\git\PlayFab.C\specs\playfab-gamesave\xplat-testing\status\` — cross-cutting bug reports
- The "Failing Tests by Root Cause" section above

Many failures are **GRTS product bugs** (not SDK bugs) and cannot be fixed in this repo.

## Key Differences: PC GRTS vs PC Inproc

| Aspect | PC GRTS | PC Inproc |
|--------|---------|-----------|
| Provider | `GameSaveAPIProviderGRTS` | `GameSaveAPIProviderWin32` |
| Upload execution | Out-of-process (xgameruntime) | In-process (`FolderSyncManager`) |
| UI dialogs | Stock TCUI (system-provided) | Game-provided via callbacks |
| Save folder | System-managed (`C:\XboxGames\GameSave\...`) | Game-specified (`saveFolder` in `PFGameSaveInitArgs`) |
| Error codes | GRTS-specific (`E_GS_*`) | SDK-specific (`E_PF_GAMESAVE_*`) |
| ResetCloud | Returns `E_NOTIMPL` | Fully implemented |
| Offline mode | `E_GS_USER_CANCELED` path | `UseOffline` via SyncFailed callback |
| ActiveDeviceChanged | Not cross-engine aware | Polling-based detection |
| Empty container dirs | Only creates dirs with files | Creates all manifest dirs |

## AdminHelpCli — Command-Line Client for AdminHelper

`Test\AdminHelpCli\` is a .NET 8 console app that sends actions to AdminHelper over its named pipe (`PlayFabTestAdminHelper`). Use it from scripts, terminals, or AI agents to trigger privileged operations without needing the interactive AdminHelper keyboard shortcuts.

### Build

```powershell
dotnet build Test\AdminHelpCli -c Release --nologo
# Output: Out\x64\Release\AdminHelpCli\AdminHelpCli.exe
```

### Usage

```powershell
AdminHelpCli <action> [key=value ...]
```

### Examples

```powershell
# Check AdminHelper is alive
AdminHelpCli Ping

# Network control
AdminHelpCli DisableNetwork
AdminHelpCli EnableNetwork
AdminHelpCli NetworkFlapping cycles=3 intervalMs=1000

# GRTS service control
AdminHelpCli StopGamingServices
AdminHelpCli StartGamingServices
```

### Exit Codes

| Code | Meaning |
|------|---------|
| 0 | Success (action completed) |
| 1 | AdminHelper reported failure |
| 2 | Connection error (AdminHelper not running) |
| 3 | Usage error (bad arguments) |

### Prerequisites

- **AdminHelper must be running** as Administrator (it owns the named pipe).
- AdminHelpCli itself does NOT require elevation — it's just a pipe client.

### Protocol

AdminHelpCli sends newline-delimited JSON over the pipe:

```json
{"action":"StopGamingServices","parameters":null}
```

And reads back a single-line JSON response:

```json
{"success":true,"message":"GamingServices stopped"}
```

This is the same protocol used internally by `GameTestController` (`DeviceStateController.TryRunViaAdminHelper`).

## File Locations

| Path | Description |
|------|-------------|
| `Test\GameTestScenarios\gamesave-pc\` | 116 xplat YAML scenario files |
| `Utilities\Scripts\tests-run.py` | Unified test runner (use `py tests-run.py gamesave-pc` for xplat) |
| `Utilities\Scripts\testrunner.py` | Shared runner infrastructure |
| `Utilities\Scripts\combine-xplat-results.py` | Cross-pass result combiner |
| `Utilities\Scripts\normalize-xplat-engines.py` | Normalizes engine assignments in YAML scenarios |
| `Utilities\Scripts\tests-build.ps1` | Build script for test apps |
| `Utilities\Scripts\read-grts-etl.py` | Decodes GRTS ETW traces from the GamingServices auto-logger |
| `Utilities\Scripts\gamesave-maintenance.ps1` | GRTS service management (restart, clean, reset state) |
| `Utilities\Scripts\gamesave-build.ps1` | Build GameSave SDK libraries |
| `Utilities\Scripts\grts-reinstall.ps1` | Reinstall GRTS (GamingServices) from msixbundle |
| `Utilities\Scripts\grts-enable-dumps.ps1` | Enable crash dumps for GamingServices |
| `Utilities\Scripts\grts-disable-dumps.ps1` | Disable crash dumps for GamingServices |
| `Utilities\Scripts\grts-collect-dump.ps1` | Collect GRTS crash dump after failure |
| `Utilities\Scripts\grts-crash-report.ps1` | Generate crash report from dump file |
| `Out\gamesave-pc-tests\` | Test output: pass folders + combined summary |
| `Out\gamesave-pc-tests\combined-summary.csv` | Aggregate results across all passes |
| `C:\git\PlayFab.C\specs\playfab-gamesave\xplat-testing\status\` | Bug reports (cross-cutting + per-test) |
| `Source\PlayFabGameSave\Source\Platform\Windows\PFGameSaveFilesAPIProvider_GRTS.h/cpp` | GRTS provider implementation |
| `Source\PlayFabGameSave\Source\Platform\Windows\PFGameSaveFilesAPIProvider_Win32.h/cpp` | Inproc provider implementation |
| `specs\playfab-gamesave\xplat-testing\ai-grts-internals.md` | GRTS internals guide (state machines, on-disk state, error codes) |
| `Test\AdminHelpCli\` | CLI client for AdminHelper (pipe-based, no elevation needed) |
| `Test\AdminHelper\` | Elevated helper service (network control, GRTS service control) |

## Diagnostic Scripts

### `read-grts-etl.py` — GRTS ETW Trace Reader

Decodes ETW events from the GamingServices auto-logger session. Uses ctypes + TDH (no external deps).

```powershell
# Flush and decode all events from last 5 minutes
py Utilities\Scripts\read-grts-etl.py --last-minutes 5 -o grts-trace.txt

# All events (no time filter)
py Utilities\Scripts\read-grts-etl.py -o grts-trace.txt

# Filter to specific provider
py Utilities\Scripts\read-grts-etl.py --provider "Microsoft.Gaming.PlayFab*"

# Skip flushing (read stale data)
py Utilities\Scripts\read-grts-etl.py --no-flush

# Output as JSON
py Utilities\Scripts\read-grts-etl.py --json -o grts-trace.json
```

Default ETL directory: `C:\Windows\System32\LogFiles\WMI\` (GamingServices*.etl files).

Key events to look for:
- `PFUploadWorkerEntered` — background upload started
- `PFUploadContextComplete` — upload finished successfully
- `PFUploadContextFinalized` — manifest finalized on server
- `PFXGameSaveServiceNoAumid` — AUMID lookup failed (common on PC)
- `GameFltGameProcessCreated/Destroyed` — game process lifecycle
- `CallerPendingCleanup` — delayed cleanup timer firing

### `gamesave-maintenance.ps1` — GRTS Service Management

```powershell
# Restart GRTS service
Utilities\Scripts\gamesave-maintenance.ps1 --restart

# Clean all local GRTS state (deletes C:\XboxGames\GameSave\pgs\*)
Utilities\Scripts\gamesave-maintenance.ps1 --clean

# Full reset (restart + clean)
Utilities\Scripts\gamesave-maintenance.ps1 --reset
```

### `normalize-xplat-engines.py` — Engine Assignment Normalizer

Ensures all xplat YAML scenarios have correct `engines:` fields matching their expected
test behavior (e.g., `pc-grts` vs `pc-inproc` vs both).

```powershell
# Dry-run (show what would change)
py Utilities\Scripts\normalize-xplat-engines.py --dry-run

# Apply changes
py Utilities\Scripts\normalize-xplat-engines.py
```

### `combine-xplat-results.py` — Cross-Pass Result Combiner

Aggregates results from multiple test passes into a single CSV for trend analysis.

```powershell
# Combine all passes in Out\gamesave-pc-tests\
py Utilities\Scripts\combine-xplat-results.py

# Output: Out\gamesave-pc-tests\combined-summary.csv
```

### GRTS Crash Diagnostics

```powershell
# Enable crash dumps (run once, persists across reboots)
Utilities\Scripts\grts-enable-dumps.ps1

# After a crash, collect the dump
Utilities\Scripts\grts-collect-dump.ps1

# Generate crash report from dump
Utilities\Scripts\grts-crash-report.ps1 -DumpFile <path>

# Disable crash dumps when done
Utilities\Scripts\grts-disable-dumps.ps1
```

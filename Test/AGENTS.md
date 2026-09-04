# Test Framework Guide

## Repo Overview

PlayFab C/C++ SDK — cross-platform (GDK, Win32, Linux, iOS, Android).
Provides authentication, data services, multiplayer, game save, and party functionality.

## Build

All commands run from the repo root (`C:\git\PlayFab.C`).

```powershell
# Build test apps (GameTestAppWindows + GameTestController)
Utilities\Scripts\tests-build.ps1          # Debug build
Utilities\Scripts\tests-build.ps1 -Clean   # Clean rebuild
```

Solution: `PlayFabGameSave.C.GDK.vs2022.sln` (includes GameTestAppWindows).
Do NOT use `PlayFab.C.vs2022.sln` for test apps — it doesn't include them.

## Test

Tests are YAML-driven scenarios in `Test/GameTestScenarios\`.
A C# controller (GameTestController) sends commands over WebSocket to a C++ device app (GameTestAppWindows).

```powershell
# Run tests by suite
py Utilities\Scripts\tests-run.py pfcore
py Utilities\Scripts\tests-run.py pfservices
py Utilities\Scripts\tests-run.py xsapi
py Utilities\Scripts\tests-run.py gamesave-inproc

# Run specific scenarios (shorthand IDs)
py Utilities\Scripts\tests-run.py pfcore --only 1,2,3
py Utilities\Scripts\tests-run.py xsapi --only 01
py Utilities\Scripts\tests-run.py pfcore --only 06

# List available suites
py Utilities\Scripts\tests-run.py --list
```

Results go to `Out\logs\` — check `controller.log` and `test-results.json`.

## Test Tags

YAML scenarios are tagged with category + status:

| Tag | Description |
|-----|-------------|
| `pfcore` | PlayFab Core SDK (init, auth, entity, telemetry) |
| `pfservices` | PlayFab Services (catalog, inventory, leaderboards, groups, etc.) |
| `xsapi` | Xbox Services API (profile, achievements, presence, multiplayer, etc.) |
| `gamesave-inproc` | PlayFab Game Save (inproc engine) |
| `gamesave-pc` | PlayFab Game Save (PC/GRTS cross-platform) |
| `gamesave-xbox` | PlayFab Game Save (Xbox) |
| `passing` | Scenario passes the test runner |
| `failing` | Scenario fails (or has `expectFailure` commands) |

## Key Paths

All paths relative to repo root.

| Path | Description |
|------|-------------|
| `Source/` | SDK source code (PlayFabCore, PlayFabServices) |
| `Test/GameTestController/` | C# test controller + YAML scenarios |
| `Test/GameTestAppShared/` | C++ command handlers (XSAPI, PFCore, PFServices) |
| `Test/GameTestAppWindows/` | Windows GDK test device app |
| `Out/` | Build output and test logs |
| `specs/` | Test documentation and triage reports |
| `Utilities/Scripts/` | Build and test runner scripts |

## Adding/Editing Tests

Each YAML scenario has: `id`, `name`, `tags`, `blocks` (command sequences), `executionOrder`, and `cleanup`.
Commands map to C++ handler functions registered in `Test/GameTestAppShared/`.

- Use `expectFailure: true` at the step level for commands expected to return errors
- The controller auto-supplies SCID for `XblInitialize` commands
- XSAPI scenarios need `XblInitialize` + `XblCleanupAsync` (see existing `xsapi-*.yml` for pattern)
- Tag new scenarios with their category (`pfcore`, `xsapi`, etc.) and `passing` or `failing`

## Sandbox Configuration

Both the Xbox devkit and the Windows PC must be in the **`XDKS.1`** sandbox for game save and two-device tests to work. Mismatched sandbox causes `PFAuthenticationLoginWithCustomIDAsync` to fail silently with `E_FAIL (0x80004005)` after ~8 seconds — no HTTP traffic is generated because the SDK rejects the request locally.

```powershell
# Set Xbox devkit sandbox
xbconfig SandboxId=XDKS.1 /x:<devkit-ip>

# Set Windows PC sandbox (run as admin)
xbox-sandbox XDKS.1

# Verify
xbconfig /x:<devkit-ip> SandboxId
xbox-sandbox
```

The test apps log the current sandbox at startup and emit a warning if it doesn't match `XDKS.1`.

The test runner (`Run-GameSaveXboxTests.ps1`) automatically validates sandbox and user configuration before launching tests. If checks fail, the script stops with clear remediation steps — see `Step-PreflightChecks.ps1`.

## User Sign-In Requirements

Two-device scenarios require an Xbox user signed in on **both** devices:
- **Xbox devkit**: A user must be signed in on the console.
- **Windows PC**: Sign into Xbox App or Game Bar under the `XDKS.1` sandbox.

Without a signed-in user, `XUserAddAsync` fails with `0x89245106` and all two-device scenarios fail on the affected device. The test apps perform a silent user check at startup and log a warning if no user is available. The pre-flight script (`Step-PreflightChecks.ps1`) also checks for users before tests begin.

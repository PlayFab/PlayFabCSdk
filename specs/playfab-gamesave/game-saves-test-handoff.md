# PlayFab Game Saves — Test Handoff Guide

**Date:** February 2026 (updated March 2026)  
**Author:** Jason Sandlin  
**Audience:** Adam Drake and anyone picking up Game Saves test ownership  
**Origin:** Victor Kulaga's handoff request from the [February 26, 2026 delivery review](https://microsoft-my.sharepoint.com/personal/jlaflen_microsoft_com/_layouts/15/stream.aspx?id=%2Fpersonal%2Fjlaflen%5Fmicrosoft%5Fcom%2FDocuments%2FRecordings%2FGame%20Saves%20delivery%20review%2D20260226%5F161235%2DMeeting%20Recording%2Emp4)  
**Updated from:** [March 6, 2026 sync meeting with Adam Drake](sync on PFGameSaves-20260306_140711) — Sections 5–9 added

---

## 1. Environment Setup

### 1.1 Software Prerequisites

| Requirement | Details |
|-------------|---------|
| Visual Studio 2022 | Desktop C++ workload + GDK (Gaming Development Kit) components |
| .NET 8 SDK | For the C# test controller (`GameTestController`) |
| MSBuild | On `PATH` (comes with VS 2022) |
| GDK | October 2025 (2510) or later — required for building `Gaming.Desktop.x64` and Xbox targets |

### 1.1.1 Windows Firewall (Xbox Testing)

The test controller hosts a WebSocket server on **port 15080**. Xbox devices connect to this port over the local network. If Windows Firewall blocks inbound connections, Xbox devices will fail to connect (the Xbox app logs `[WSAutoConnect] Connect attempt stuck for >15s` repeatedly).

**Add a firewall rule** (run as Administrator):

```powershell
# Option A: Allow by program (recommended — scoped to the controller binary)
netsh advfirewall firewall add rule name="GameTestController" dir=in action=allow ^
    program="C:\git\PlayFab.C\Out\x64\Debug\GameTestController\GameTestController.exe" ^
    protocol=TCP profile=private,public enable=yes

# Option B: Allow by port (simpler, works regardless of build output path)
netsh advfirewall firewall add rule name="GameTestController Port 15080" dir=in action=allow ^
    protocol=TCP localport=15080 profile=private,public enable=yes
```

> **Note:** PC-only tests use loopback connections and are not affected by firewall settings. This rule is only required when an Xbox console (or any remote device) connects to the controller over the network.

### 1.2 Title Configuration

Tests use the shared PlayFab test title defined in `Test\testTitleData.json`:

```json
{
  "titleId": "E18D7",
  "connectionString": "https://E18D7.playfabapi.com"
}
```

Scenario YAML files reference this title ID directly (e.g., `gamesave-01-single-device-golden-path.yml`). If you need a different title, update both the YAML files and `testTitleData.json`.

### 1.3 Sandbox

- **Xbox/GDK sandbox:** The test title uses sandbox `XDKS.1`. The `MicrosoftGameConfig.mgc` in `Test\GameTestAppWindows` has `TitleId: 76029B4D` and `MSAAppId: 000000004C26FED0`.
- **Steam Deck:** Registry key `HKLM\SOFTWARE\Microsoft\XboxLive\Sandbox` must be set to `XDKS.1` before Xbox Live services initialize. Only needed for Xbox ecosystem authentication in non-retail sandboxes.

### 1.4 Authentication

Automated tests use **CustomID authentication** (`PFLocalUserCreateHandleWithPersistedLocalId`) — no Xbox Live sign-in UI required, fully headless. The persisted local ID `"GameTestHarness"` is the default across all scenarios.

### 1.5 Registry Keys for In-Process Mode

For headless testing on machines without GDK Gaming Services (including ADO build VMs), two registry keys enable in-process mode:

```
HKLM\SOFTWARE\Microsoft\GamingServices
  ForceUseLocalServices    = DWORD 1
  ForceUseInprocGameSaves  = DWORD 1
```

The pipeline sets these automatically. For local testing, either set them manually or the test device app accepts command-line control. **Remove these keys when done** — they are not for production.

### 1.6 Key Paths

All paths relative to repo root (`C:\git\PlayFab.C`).

| Path | Description |
|------|-------------|
| `Test\GameTestController\` | C# test controller (WebSocket server, scenario engine, headless CLI) |
| `Test/GameTestScenarios\` | YAML scenario files (gamesave-*, lhc-*, pfcore-*, etc.) |
| `Test\GameTestAppShared\` | Cross-platform C++ command handlers shared by all device apps |
| `Test\GameTestAppWindows\` | Windows GDK test device application |
| `Test\GameTestAppXbox\` | Xbox console test device application |
| `Test\testTitleData.json` | Title ID and endpoint configuration |
| `Source\PlayFabGameSave\` | Game Saves SDK source code |
| `Source\PlayFabGameSave\Include\playfab\gamesave\` | Public API headers |
| `Samples\PlayFabGameSaveSample-Windows\` | Reference sample application |
| `specs\playfab-gamesave\` | All specs, design docs, and test documentation |
| `Utilities\Scripts\` | Build and test runner scripts |
| `Out\` | Build output and test logs |

### 1.7 ADO Pipelines

| Pipeline | File | Purpose |
|----------|------|---------|
| **PlayFab.C-OB-PullRequest-GDK** | `Pipelines\PlayFab.C-OB-PullRequest-GDK.yml` | Builds SDK + test harness artifacts (Gaming.Desktop.x64 Debug/Release, Xbox Scarlett Debug) on every PR |
| **PlayFab.C.GameSaveTests** | `Pipelines\PlayFab.C.GameSaveTests.yml` | Runs Game Save automated tests. Auto-triggered when the PR build completes. Can also be triggered manually with a specific build ID |
| **PlayFab.C.GameSave.GDK** | `Pipelines\PlayFab.C.GameSave.GDK.yml` | GDK release build for Game Saves |

The test pipeline runs on pool `playfab_poolA`. It downloads the test harness artifact, sets in-process registry keys, launches the controller + two device instances, runs scenarios tagged `passing`, and publishes JUnit results to ADO.

---

## 2. Running Automated Tests

### 2.1 Build

```powershell
# From repo root
Utilities\Scripts\tests-build.ps1           # Debug build
Utilities\Scripts\tests-build.ps1 -Clean    # Clean rebuild
```

This builds the solution `PlayFabGameSave.C.GDK.vs2022.sln` (which includes `GameTestAppWindows`) and the C# controller (`GameTestController.csproj`). Output lands in `Out\`.

> **Important:** Use `PlayFabGameSave.C.GDK.vs2022.sln`, not `PlayFab.C.vs2022.sln` — the latter doesn't include test apps.

### 2.2 Run Tests Locally

```powershell
# Run all passing Game Saves tests
py Utilities\Scripts\tests-run.py gamesave-inproc

# Run a specific scenario by shorthand ID
py Utilities\Scripts\tests-run.py gamesave-inproc --only 01

# Run only passing tests
py Utilities\Scripts\tests-run.py gamesave-inproc --passing

# Stop at first failure (useful for debugging)
py Utilities\Scripts\tests-run.py gamesave-inproc --stop-on-fail

# Resume from test N after fixing a failure
py Utilities\Scripts\tests-run.py gamesave-inproc --stop-on-fail --start 06
```

The script automatically starts the controller, launches one or two device instances (based on scenario needs), waits for completion, and reports pass/fail.

### 2.3 Results and Logs

| Artifact | Location |
|----------|----------|
| Test results (JSON) | `Out\logs\test-results.json` |
| Controller log | `Out\logs\controller.log` |
| Controller stdout/stderr | `Out\logs\controller-stdout.txt` / `controller-stderr.txt` |
| Device logs | `Out\logs\Device*-stdout.txt` |

Read logs quickly after a failure:

```powershell
Utilities\Scripts\read-test-logs.ps1 -FailedOnly
```

### 2.4 Available Test Tags

| Tag | Description |
|-----|-------------|
| `gamesaves` | PlayFab Game Save scenarios (39 scenarios) |
| `pfcore` | PlayFab Core SDK (init, auth, entity, telemetry) |
| `pfservices` | PlayFab Services (catalog, inventory, leaderboards, groups) |
| `xsapi` | Xbox Services API (profile, achievements, presence, multiplayer) |
| `passing` | All scenarios that currently pass |
| `failing` | Scenarios that are expected to fail (mocking not complete, etc.) |

### 2.5 Test Architecture

```
┌───────────────────────────────────────────────────┐
│  GameTestController.exe (C# .NET 8 WinForms/CLI) │
│  - Hosts WebSocket server on port 15080            │
│  - Loads YAML scenarios, orchestrates execution    │
│  - Headless mode for CI: --headless --exit-on-complete │
└────────────┬────────────────────┬─────────────────┘
             │ WebSocket          │ WebSocket
    ┌────────▼─────────┐  ┌──────▼───────────┐
    │  Device A (C++)  │  │  Device B (C++)  │
    │  GameTestApp     │  │  (optional)      │
    │  Windows.exe     │  │  GameTestApp     │
    │                  │  │  Windows.exe     │
    └──────────────────┘  └──────────────────┘
```

Each scenario YAML defines: `id`, `name`, `tags`, `devices`, `blocks` (ordered command sequences), `executionOrder`, and `cleanup`. Commands map 1:1 to C++ handler functions in `Test\GameTestAppShared\`.

### 2.6 Pipeline Behavior

On every PR to `PlayFab.C`:
1. **PlayFab.C-OB-PullRequest-GDK** builds the SDK and test harness artifacts
2. On successful build, **PlayFab.C.GameSaveTests** is auto-triggered
3. The test pipeline downloads the artifact, launches controller + 2 devices, runs scenarios tagged `passing`
4. Results are published as JUnit XML to ADO — failures block the PR

To run manually with a specific build ID:
- Navigate to the `PlayFab.C.GameSaveTests` pipeline in ADO
- Click "Run pipeline" and provide the `buildId` parameter

### 2.7 Scenario Status

As of February 2026, ~25–26 scenarios pass in CI. The 13 failing scenarios are not bug failures — they require mocking infrastructure that isn't complete yet (disk exhaustion, storage handle mocking, etc.). See `specs\playfab-gamesave\testing\failing-scenarios-priority.md` for the prioritized list.

---

## 3. Manual Testing Requirements

Automated tests cover deterministic multi-device synchronization, conflict resolution, rollback, offline mode, and API behavior. Two sets of manual tests exist that cannot be fully automated:

### 3.1 Platform Manual Tests (TestX — Xbox Platform Team)

**Owner:** Xbox console platform team (TestX, contact: relyons@microsoft.com)  
**Test app:** ShamWow  
**Spec:** [Platform test cases (SharePoint)](https://microsoftapc-my.sharepoint.com/:w:/g/personal/relyons_microsoft_com/ERje7N7uutNCk8MDW3U_oBIBaXpCV99c1dYmqbDgq1nsaA?e=xLPK2W) and `specs\playfab-gamesave\testing\platform-test-cases.md`

These tests cover hardware-specific and UX-perception scenarios that require physical devices and human judgment:

- **Sync dialog prompts** — verify dialog appears during cloud sync
- **Upload on suspend/terminate/sign-out/power-pull** — verify background upload completes or state is preserved across OS lifecycle events
- **Cross-device progression** — verify saves transfer between all platform combinations (Gen8, Gen9, MSIXVC, Steam PC, Steam Deck)
- **Connectivity dialogs** — verify correct UX when connection drops mid-sync
- **Conflict resolution UX** — verify dialog copy, choices, and pacing when local/cloud saves diverge
- **Active device contention UX** — verify messaging when another device holds the lock

Platform combinations to test (per the TestX spec):

| | Gen8 | Gen9 | MSIXVC PC | Steam PC | Steam Deck |
|---|---|---|---|---|---|
| Gen8 | — | P1 | — | | |
| Gen9 | | — | P1 | P1 | P1 |
| MSIXVC PC | — | | | P1 | |
| Steam PC | | | | | P1 |
| Steam Deck | | | P1 | | |

### 3.2 PlayStation Manual Tests (Victor's Team)

**Owner:** Victor Kulaga's team (PlayStation platform)  
**Scope:** PlayStation-specific UI and account conflict scenarios that cannot be automated due to platform restrictions and tenting requirements.

### 3.3 Manual vs. Automated Decision Summary

See `specs\playfab-gamesave\testing\testing-strategy-summary.md` for the full decision rubric. In brief:

- **Automate:** Deterministic state machines, conflict guard rails, quota math, concurrency, error codes
- **Manual:** Copy/wording clarity, visual affordances, pacing perception, hardware lifecycle events (suspend, power-pull), Steam Deck QR authentication flows
- **Hybrid:** Automation preconditions the scenario; human validates UX on real hardware

---

## 4. Adding New Tests with AI Tools

### 4.1 The Build-Test-Fix Loop

The test harness is designed for AI-assisted iteration. The workflow is documented in `specs\ai-test-loop-guide.md`. The core loop for Game Saves:

```
Loop:
  1. Build:     Utilities\Scripts\tests-build.ps1
  2. Run:       py Utilities\Scripts\tests-run.py gamesave-inproc --stop-on-fail --start N
  3. If pass:   Done
  4. Read logs: Utilities\Scripts\read-test-logs.ps1 -FailedOnly
  5. Fix first failure
  6. Set N = ID of the failing test
  7. Go to 1
```

### 4.2 Adding a New Test Scenario

1. **Create a YAML file** in `Test/GameTestScenarios\` following the naming convention `gamesave-NN-description.yml`

2. **Define the scenario structure:**
   ```yaml
   id: Scenario-NN-Description
   name: GameSave NN - Human Readable Name
   tags:
     - passing      # Add when the test passes
     - gamesaves
   description: What this test validates.
   devices:
     DeviceA: {}
     # DeviceB: {}  # Add if multi-device
   defaults:
     stepTimeoutSeconds: 180
   blocks:
     setup:
       - command: XGameRuntimeInitialize
       - command: PFInitialize
       - command: PFServicesInitialize
       - command: PFServiceConfigCreateHandle
         parameters:
           endpoint: "https://E18D7.playfabapi.com"
           titleId: "E18D7"
       # ... init sequence
     test:
       - command: <YourTestCommands>
   cleanup:
     DeviceA:
       - command: PFGameSaveFilesUninitializeAsync
       # ... cleanup sequence
   executionOrder:
     - role: DeviceA
       block: setup
     - role: DeviceA
       block: test
   ```

3. **Use existing commands.** All available commands are in `Test\GameTestAppShared\` — the handler headers list every supported command. Key Game Saves commands include all `PFGameSaveFiles*` APIs plus harness commands like `WriteGameSaveData`, `DeleteSaveRoot`, `CaptureSaveContainerSnapshot`, `CompareSaveContainerSnapshot`.

4. **Add a new command handler** if needed: create a handler function in the appropriate `*Handlers.cpp` file in `Test\GameTestAppShared\` and register it in the command registry.

5. **Tag appropriately:** Use `passing` only when the scenario reliably passes. Use `failing` for work-in-progress scenarios so CI doesn't break.

### 4.3 The "Reproduce and Fix Bug" AI Skill

When a bug is reported (e.g., from Playground or a partner), use the AI skill at:

[`sdk.projects/ai/skills/reproduce-and-fix-bug/SKILL.md`](https://microsoft.visualstudio.com/Xbox.Services/_git/sdk.projects?version=GBuser/jasonsa/main&path=/ai/skills/reproduce-and-fix-bug/SKILL.md)

The skill follows this workflow:
1. **Ingest the bug report / logs** — feed the failure logs into the AI
2. **Write a test that reproduces the failure** — the AI creates a YAML scenario that triggers the bug
3. **Verify the test fails** — confirm the new test captures the defect
4. **Diagnose and fix** — the AI traces the failure to source code and applies a minimal fix
5. **Verify the test passes** — rebuild and rerun to confirm the fix

This ensures every bug fix ships with a regression test. Every Playground bug over the last several weeks has a test added through this workflow.

### 4.4 Tips for AI-Assisted Test Development

1. **Layer by layer** — don't try to generate the entire scenario at once. Start with init + one command, verify it works, then add complexity.

2. **Use `--StopOnFirstError`** — when iterating, stop at first failure and fix before moving on.

3. **Read device logs first** — they contain the concrete HRESULT and error context. The controller log shows orchestration flow.

4. **Mock injection** — use `HttpMock` commands to simulate network failures, rate limits, and service errors without needing real failure conditions. See `Test\GameTestAppShared\Misc\HttpMock.cpp`.

5. **Keep fixes surgical** — avoid broad refactors during triage. One fix, one rebuild, one retest.

6. **Run a full pass before declaring success** — after fixing individual failures, run the full tag to catch regressions.

---

## 5. Test Controller GUI Mode

The handoff sections above focus on headless/CLI execution, but the controller (`Test\GameTestController\`) is a full WinForms app with three modes:

| Mode | Launch | Use Case |
|------|--------|----------|
| **GUI** | Double-click or F5 from VS | Interactive development, manual testing, browsing scenarios |
| **CLI** | `GameTestController.exe -cli` | Terminal-based interactive mode |
| **Headless** | `GameTestController.exe --headless --exit-on-complete` | CI pipelines, fully unattended |

### 5.1 GUI Walkthrough

When the controller opens in GUI mode:

1. **IP address display** — Shows the WebSocket endpoint so remote devices (Xbox, other PCs) can connect.
2. **Component filter** — Combo box to filter scenarios by component (`gamesaves`, `pfcore`, `pfservices`, `xsapi`, etc.).
3. **Tag filter** — Filter by `passing`, `failing`, `untested`, etc.
4. **Custom ID prefix** — Set this to your alias or machine name. **Critical:** If two people use the same prefix, their tests share the same PlayFab player identity and will interfere with each other's cloud state.
5. **Auto-launch local devices** — When checked, the controller automatically starts the required device instances (Device A, Device B) without manual intervention.
6. **Run All / Stop on Failure / Repeat** — Buttons to run the filtered set, stop at first failure, or loop.
7. **Logs button** — Opens the logs folder and automatically pulls logs from all connected devices (including remote ones via WebSocket).

### 5.2 Manual Mode

From the GUI, switch to **Manual** mode to send individual commands to a connected device. The device publishes its full command list to the controller on connect — this includes every GDK API (XUser, XStore, XGameSave, etc.), all PlayFab APIs, and all test harness commands. Useful for exploratory testing without writing a YAML scenario.

### 5.3 Chaos Testing Mode

The controller includes a chaos testing mode for injecting random failures during scenario execution. Accessible from the GUI mode selector.

---

## 6. Debugging Pipeline Failures

### 6.1 Downloading Artifacts from a Failed Run

1. Open the **PlayFab.C.GameSaveTests** pipeline run in ADO.
2. Click **Artifacts** — the logs folder contains controller and device logs for the entire run.
3. Download the logs folder locally.
4. Feed the logs folder to an AI assistant with bootstrap context (see Section 8) and ask it to debug the failure.

### 6.2 Working with Build Artifacts

The **PlayFab.C-OB-PullRequest-GDK** pipeline produces artifacts containing:

| Artifact Folder | Contents |
|-----------------|----------|
| `GRDK\` | GDK SDK binaries (can be copied directly on top of a local GDK installation for testing) |
| `TestHarness\` (Desktop Debug) | Controller + Windows test device executables |
| `TestHarness\` (Xbox Scarlett) | Xbox test device executable |

To test updated SDK bits locally against your GDK installation, copy the `GRDK\` artifact contents directly over your local GDK — the folder structure is designed to overlay cleanly.

### 6.3 Log Analysis Tips

- **Start with the summary log** — each device has a summary file showing command→status→HRESULT for each step.
- **Verbose device logs** contain full LHC (libHttpClient) messages, HTTP request/response details, and internal state transitions.
- **Controller log** shows orchestration flow — which command was dispatched to which device and in what order.
- Copy the entire `Out\logs\` folder and hand it to an AI assistant for analysis rather than reading logs manually.

---

## 7. Utility Tools

### 7.1 Game Save Maintenance Script

`Utilities\Scripts\gamesave-maintenance.ps1` manages registry keys, services, and local state for in-process Game Saves testing.

```powershell
# Show current status (reg keys, folder size, service state)
Utilities\Scripts\gamesave-maintenance.ps1

# Toggle in-process game saves on/off
Utilities\Scripts\gamesave-maintenance.ps1 --inprocgamesaveson
Utilities\Scripts\gamesave-maintenance.ps1 --inprocgamesavesoff

# Toggle Xbox Runtime local services (Steam Deck emulation on PC)
Utilities\Scripts\gamesave-maintenance.ps1 --inprocxgameruntimeon
Utilities\Scripts\gamesave-maintenance.ps1 --inprocxgameruntimeoff

# Enable debug trace logging for in-proc GRTS
Utilities\Scripts\gamesave-maintenance.ps1 --inprocxgameruntimelogson

# Full cleanup (delete save folder + registry state + restart services)
Utilities\Scripts\gamesave-maintenance.ps1 --fullclean

# Collect diagnostic logs (ETW, WMI, PGS data) into a zip
Utilities\Scripts\gamesave-maintenance.ps1 --collectlogs
```

> **Tip:** Keep `ForceUseInprocGameSaves` ON during development so you test your code, not the GRTS code path. Keep `ForceUseLocalServices` OFF unless you specifically need to emulate Steam Deck sign-in behavior on PC.

### 7.2 Game Save Test Tool (pfgamesaveutil)

`Tools\pfgamesaveutil\` is a C# console utility that talks to PlayFab REST endpoints directly (bypasses the C client SDK). Requires a PlayFab secret key and title ID.

Capabilities:
- **Download** — Pull a player's cloud save state via the service API
- **Compare** — Diff local vs. cloud state
- **Reset** — Iterate all manifests and mark them for deletion (clears cloud state for a player)
- **Collect status** — Dump manifest metadata and service-side state

This tool is useful for:
- Resetting a player's cloud state when tests leave stale data
- Inspecting manifest details (manifest numbers, zip contents, extended manifest JSON)
- Verifying server-side state independently of the C client
- Debugging discrepancies between what the client sees and what the service has

> **Note:** The in-proc client also has a `PFGameSaveFilesResetCloudAsync` API for resetting cloud state programmatically within tests.

---

## 8. AI Onboarding Workflow

### 8.1 Bootstrap Prompt

Before giving an AI assistant any Game Saves task, paste the contents of `specs\playfab-gamesave\ai-bootstrap.md` into the conversation. This tells the AI to read all relevant headers, specs, docs, and test infrastructure before acting — essentially a "new hire orientation" in a single prompt.

```
# Quick start
1. Open specs\playfab-gamesave\ai-bootstrap.md
2. Select All → Copy → Paste into your AI conversation
3. Wait for the AI to finish reading all referenced files
4. Then give it your actual task
```

For VS Code Copilot CLI, the `AGENTS.md` at repo root serves a similar bootstrapping purpose automatically.

### 8.2 The Build-Test-Fix Loop ("Ralph Loop")

The AI-assisted iteration loop described in `specs\ai-test-loop-guide.md` is the primary development workflow. The key insight: **don't review AI code that doesn't build and pass tests.** Let the AI iterate through build→test→read-logs→fix until tests pass, then review the final result.

Practical tips from the sync:
- **Don't let the AI run the full suite on every iteration** — if working on a single scenario, just run that scenario. The pipeline will run the full suite.
- **Use `-SkipScenarios N`** to skip known-passing scenarios and jump to where the failure is.
- **If the AI can't debug from existing logs,** it's good at adding more logging to the source, rebuilding, and retesting — the log coverage improves over time.

### 8.3 Bug Reproduction Workflow

When a bug comes in from Playground or a partner:

1. Collect logs from the failing game/environment
2. Bootstrap the AI with the prompt from Section 8.1
3. Feed it the logs plus the [reproduce-and-fix-bug skill](https://microsoft.visualstudio.com/Xbox.Services/_git/sdk.projects?version=GBuser/jasonsa/main&path=/ai/skills/reproduce-and-fix-bug/SKILL.md)
4. The AI will: write a YAML test that reproduces the failure → confirm it fails → diagnose and fix → confirm the test passes
5. Review the resulting test + fix + bug report

Every Playground bug in recent weeks has followed this workflow and ships with a regression test.

---

## 9. Architecture Quick Reference

This section distills key Game Saves architecture concepts from the [March 2026 sync meeting](https://microsoft-my.sharepoint.com/) for quick reference. For full details, see `specs\playfab-gamesave\ai-summary.md` and `specs\playfab-gamesave\design\client-dev-spec.md`.

### 9.1 API Flow (Game Developer's Perspective)

```
1. PFInitialize / PFServicesInitialize          — Init PlayFab Core
2. PFServiceConfigCreateHandle                   — Set endpoint + title ID
3. PFLocalUserCreateHandle (with identity)       — Works offline, no sign-in UI needed
4. PFGameSaveFilesInitialize                     — Init Game Saves subsystem
5. PFGameSaveFilesSetSaveFolderPath              — Set local save root (PC/Steam Deck only; Xbox mounts XBD)
6. PFGameSaveFilesSetCustomUI*                   — Wire UI callbacks (optional on Xbox, required elsewhere)
7. PFGameSaveFilesAddUserWithUIAsync             — The "download dance": acquire lock → detect conflicts → sync from cloud
8. PFGameSaveFilesGetFolder                      — Get save root path; read/write files normally
9. PFGameSaveFilesUploadUserWithUIAsync          — The "upload dance": diff → compress → upload deltas
10. PFGameSaveFilesUninitializeAsync             — Clean up on exit
```

Steps 7 and 9 are the only async I/O calls. Everything between them is normal file I/O by the game.

### 9.2 Two Architecture Paths

| | Xbox / Windows (GRTS) | Steam Deck / PlayStation / In-Proc |
|---|---|---|
| **Owner** | Kelly Con's team (console platform) | This SDK (PlayFab.C) |
| **Layer** | Thin passthrough to GRTS APIs | Full state machine (folder sync manager) |
| **Background upload** | OS-level (survives game exit) | In-process only (game must stay running) |
| **Reg key to swap** | N/A | `ForceUseInprocGameSaves = 1` routes GDK apps through in-proc path |

The C API is identical on both paths — the SDK detects the platform and routes internally.

### 9.3 Key Internal Concepts

- **Active Device** — Only one device holds the "pending manifest" lock at a time. The lock step checks this via `ListManifest` and looks for the latest pending manifest matching the current device ID.
- **Atomic Unit** — A top-level subfolder under the save root. Conflict detection operates at this granularity: if *any* file in an atomic unit needs both upload and download, the *entire* unit is in conflict. This matches how games group related save data (e.g., `map.dat` + `player.dat` in one folder).
- **Manifest & Zips** — The service stores save data as a manifest pointing to 64 MB zip bundles. An extended manifest (JSON) tracks every file across all zips with skip flags for deleted/superseded entries. Only delta zips are uploaded; old zips are reused until fully superseded and then garbage-collected server-side.
- **Local State JSON** — A local file tracking the last sync timestamp. Used for conflict detection: "has this file changed locally since the last upload?"
- **Rollback** — Two rollback paths: (1) *conflict loser* — upload the unchosen side of a conflict so it can be restored later; (2) *last known good* — revert to the last manifest that was successfully downloaded and then uploaded on top of.

### 9.4 Folder Sync Manager Steps

The in-proc path uses a per-user `FolderSyncManager` with discrete step objects:

| Step | Runs During | What It Does |
|------|-------------|--------------|
| **Lock** | AddUser | Login → ListManifest → check contention → create pending manifest |
| **Compare** | AddUser, Upload | Download extended manifest → parse into FileFolderSet → mark files for sync → scan for conflicts → fire conflict callback if needed |
| **Download** | AddUser | Check disk space → download changed zips → decompress → write files → update local state JSON |
| **Upload** | Upload | Compress changed files into zips → upload → finalize manifest → optionally re-lock |

---

## Reference Documentation

| Document | Location |
|----------|----------|
| Test automation plan (scenarios 1–14+) | `specs\playfab-gamesave\testing\test-automation\1-test-automation-plan.md` |
| Controller & device spec | `specs\playfab-gamesave\testing\test-automation\2-test-controller-and-device-spec.md` |
| Test harness guide | `specs\playfab-gamesave\testing\test-automation\3-pf-gamesave-test-harness.md` |
| Delivery plan | `specs\playfab-gamesave\testing\test-automation\4-test-delivery-plan.md` |
| ADO pipeline integration | `specs\playfab-gamesave\testing\test-automation\6-ado-pipeline-integration.md` |
| Xbox test device spec | `specs\playfab-gamesave\testing\test-automation\7-xbox-test-device-spec.md` |
| Failing scenario priorities | `specs\playfab-gamesave\testing\failing-scenarios-priority.md` |
| Test gap analysis (144 gaps) | `specs\playfab-gamesave\testing\test-gap-analysis.md` |
| Manual testing strategy | `specs\playfab-gamesave\testing\testing-strategy-summary.md` |
| Platform manual test cases (TestX) | `specs\playfab-gamesave\testing\platform-test-cases.md` |
| AI build-test-fix loop guide | `specs\ai-test-loop-guide.md` |
| Client dev spec | `specs\playfab-gamesave\design\client-dev-spec.md` |
| Public API docs | `specs\playfab-gamesave\docs\game-saves\` |

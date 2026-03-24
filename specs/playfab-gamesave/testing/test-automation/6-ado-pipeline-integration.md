# ADO Pipeline Integration for Test Automation

## Purpose
Define the approach for integrating the PFGameSave test controller and PFGameSaveTestDeviceWindows.exe into Azure DevOps (ADO) pipelines to enable automated regression testing on every PR and nightly builds. This document covers the pipeline modifications, artifact packaging, and phased rollout plan.

## Goals
- **Automated Regression**: Run smoke tests automatically on every PR to catch regressions before merge
- **No Manual Intervention**: Tests must run headlessly without UI prompts (achieved via CustomID auth and in-proc PFGameSaves)
- **Clear Pass/Fail Signal**: ADO pipeline must fail if any test fails, with logs published for debugging
- **Incremental Rollout**: Phase 1 runs on build machine only; Phase 2 adds Xbox device for cross-platform validation

## Section Map
- **Phase 1: Single-Machine Automation** - Build machine runs controller + 1-2 devices using in-proc mode
- **Phase 2: Cross-Platform Testing** - Add Xbox as second device for GRTS ↔ in-proc interop tests
- **Pipeline Modifications** - Changes to existing PR pipeline and new test pipeline
- **Command-Line Interface** - New parameters for headless test execution
- **Artifact Packaging** - What goes into the test artifact zip
- **Test Selection** - Which scenarios run in automated mode
- **Results and Logging** - How test results are reported to ADO

---

## Phase 1: Single-Machine Automation

### Overview
The build machine runs both the test controller and 1-2 PFGameSaveTestDeviceWindows.exe instances. All apps use:
- **In-proc PFGameSaves** (no external service dependencies for save operations)
- **CustomID authentication** (no Xbox Live sign-in UI required)
- **Local file system** for save folder simulation

This configuration is fully headless and can run in a standard ADO build agent without GPU or display.

### Architecture
```
┌─────────────────────────────────────────────────────────┐
│                   ADO Build Agent                       │
│  ┌─────────────────────────────────────────────────┐   │
│  │          PFGameSaveTestController.exe           │   │
│  │  - Loads scenario YAML files                    │   │
│  │  - Orchestrates test execution                  │   │
│  │  - Reports pass/fail to stdout + log files      │   │
│  └─────────────────┬───────────────────────────────┘   │
│                    │ WebSocket via LHC (localhost)     │
│          ┌─────────┴─────────┐                         │
│          ▼                   ▼                         │
│  ┌───────────────┐   ┌───────────────┐                 │
│  │ TestDevice A  │   │ TestDevice B  │                 │
│  │ (DeviceA)     │   │ (DeviceB)     │                 │
│  │ in-proc mode  │   │ in-proc mode  │                 │
│  │ CustomID auth │   │ CustomID auth │                 │
│  └───────────────┘   └───────────────┘                 │
└─────────────────────────────────────────────────────────┘
```

### PFGameSaveTestDeviceWindows.exe Configuration
Each PFGameSaveTestDeviceWindows.exe instance is launched with command-line parameters:
```
PFGameSaveTestDeviceWindows.exe --controller-ip 127.0.0.1 --device-name DeviceA
PFGameSaveTestDeviceWindows.exe --controller-ip 127.0.0.1 --device-name DeviceB
```

### Test Controller Configuration
The controller is launched with parameters to run in headless/automated mode:
```
PFGameSaveTestController.exe --headless --run-tag passing --exit-on-complete --results-file results.json
```

---

## Phase 2: Cross-Platform Testing

### Overview
Extends Phase 1 by adding an Xbox devkit as a second device. This validates:
- GRTS ↔ in-proc interoperability
- Real Xbox Game Runtime Services integration
- Cross-platform save synchronization

### Architecture
```
┌───────────────────────────────────────┐     ┌──────────────────────┐
│           ADO Build Agent             │     │    Xbox Console      │
│  ┌─────────────────────────────────┐  │     │  ┌────────────────┐  │
│  │     PFGameSaveTestController    │◄─┼─────┼─►│  TestDevice B  │  │
│  └─────────────────────────────────┘  │     │  │  GRTS mode     │  │
│                   │                   │     │  │  XUser auth    │  │
│                   ▼                   │     │  └────────────────┘  │
│  ┌─────────────────────────────────┐  │     └──────────────────────┘
│  │         TestDevice A            │  │
│  │         in-proc mode            │  │
│  └─────────────────────────────────┘  │
└───────────────────────────────────────┘
```

### Prerequisites for Phase 2
- Xbox devkit accessible from build agent network
- Xbox signed into a test account (or using test credentials)
- ADO agent pool with Xbox devkit connectivity (follow pattern established by Xbox.Bumblion pipelines which already have an ADO pool configured to connect to Xbox devkits)

---

## Pipeline Modifications

### 1. Modify Existing PR Pipeline (`PlayFab.C.PullRequest.yml`)

The existing GDK build job already compiles the test controller and PFGameSaveTestDeviceWindows.exe. Modify the artifact packaging step to include these binaries.

#### Changes to `gdk-vs2022-build.yml`

Add a new section after the SDK packaging to collect test binaries:

```yaml
# Collect test harness binaries for automation
- script: |
    echo on
    set "testArtifactDir=$(Pipeline.Workspace)\b\ArtifactStaging\TestHarness"
    
    mkdir "%testArtifactDir%\Controller" 2>nul
    mkdir "%testArtifactDir%\Device" 2>nul
    mkdir "%testArtifactDir%\Scenarios" 2>nul
    
    REM Copy test controller (C# .NET app)
    xcopy /E /Y "$(Build.SourcesDirectory)\Out\x64\${{ parameters.configuration }}\PFGameSaveTestController\*" "%testArtifactDir%\Controller\"
    
    REM Copy PFGameSaveTestDeviceWindows.exe (C++ GDK app)
    xcopy /E /Y "$(Build.SourcesDirectory)\Out\Gaming.Desktop.x64\${{ parameters.configuration }}\PFGameSaveTestDeviceWindows\*" "%testArtifactDir%\Device\"
    
    REM Copy scenario YAML files
    xcopy /E /Y "$(Build.SourcesDirectory)\Test\PFGameSaveTestController\Scenarios\*" "%testArtifactDir%\Scenarios\"
    
    echo Test harness packaging completed
    dir /s "%testArtifactDir%"
  displayName: Package Test Harness
  condition: and(succeeded(), eq('${{ parameters.configuration }}', 'Debug'))

- task: PublishPipelineArtifact@1
  displayName: 'Publish Test Harness Artifact'
  inputs:
    targetPath: '$(Pipeline.Workspace)\b\ArtifactStaging\TestHarness'
    artifact: 'TestHarness_${{ parameters.configuration }}'
  condition: and(succeeded(), eq('${{ parameters.configuration }}', 'Debug'))
```

### 2. Create New Test Pipeline (`PlayFab.C.GameSaveTests.yml`)

Create a new pipeline file that:
1. Downloads the TestHarness artifact from the PR build
2. Launches PFGameSaveTestDeviceWindows.exe instances
3. Runs the test controller in headless mode
4. Publishes results and logs
5. Fails if any test fails

```yaml
# PlayFab.C.GameSaveTests.yml
# Automated GameSave test execution pipeline

trigger: none  # Only triggered manually or by PR pipeline

resources:
  pipelines:
    - pipeline: BuildPipeline
      source: 'PlayFab.C.PullRequest'
      trigger:
        branches:
          include:
            - main

pool:
  name: playfab_poolA

variables:
  TestTimeout: 600  # 10 minutes max for smoke tests

jobs:
  - job: GameSaveTests
    displayName: 'Run GameSave Smoke Tests'
    timeoutInMinutes: 30
    
    steps:
      - task: DownloadPipelineArtifact@2
        displayName: 'Download Test Harness'
        inputs:
          buildType: 'specific'
          project: '$(System.TeamProjectId)'
          definition: 'PlayFab.C.PullRequest'
          buildVersionToDownload: 'latest'
          artifactName: 'TestHarness_Debug'
          targetPath: '$(Pipeline.Workspace)\TestHarness'

      - task: PowerShell@2
        displayName: 'Run GameSave Tests'
        inputs:
          targetType: 'inline'
          script: |
            $testRoot = "$(Pipeline.Workspace)\TestHarness"
            $resultsDir = "$(Pipeline.Workspace)\TestResults"
            New-Item -ItemType Directory -Path $resultsDir -Force
            
            # Start PFGameSaveTestDeviceWindows.exe instances in background
            $deviceAProcess = Start-Process -FilePath "$testRoot\Device\PFGameSaveTestDeviceWindows.exe" `
              -ArgumentList "--controller-ip", "127.0.0.1", "--device-name", "DeviceA" `
              -PassThru -RedirectStandardOutput "$resultsDir\DeviceA-stdout.txt" -RedirectStandardError "$resultsDir\DeviceA-stderr.txt"
            
            $deviceBProcess = Start-Process -FilePath "$testRoot\Device\PFGameSaveTestDeviceWindows.exe" `
              -ArgumentList "--controller-ip", "127.0.0.1", "--device-name", "DeviceB" `
              -PassThru -RedirectStandardOutput "$resultsDir\DeviceB-stdout.txt" -RedirectStandardError "$resultsDir\DeviceB-stderr.txt"
            
            Start-Sleep -Seconds 5  # Wait for devices to register
            
            # Run test controller in headless mode
            $controllerArgs = @(
              "--headless",
              "--run-tag", "passing",
              "--scenarios-path", "$testRoot\Scenarios",
              "--exit-on-complete",
              "--results-file", "$resultsDir\test-results.json",
              "--log-file", "$resultsDir\controller.log"
            )
            
            $controllerProcess = Start-Process -FilePath "$testRoot\Controller\PFGameSaveTestController.exe" `
              -ArgumentList $controllerArgs `
              -PassThru -NoNewWindow -Wait
            
            # Cleanup device processes
            if ($deviceAProcess) { Stop-Process -Id $deviceAProcess.Id -Force -ErrorAction SilentlyContinue }
            if ($deviceBProcess) { Stop-Process -Id $deviceBProcess.Id -Force -ErrorAction SilentlyContinue }
            
            # Check results
            if ($controllerProcess.ExitCode -ne 0) {
              Write-Error "Tests failed with exit code $($controllerProcess.ExitCode)"
              exit 1
            }
            
            Write-Host "All tests passed!"
          errorActionPreference: 'stop'

      - task: PublishPipelineArtifact@1
        displayName: 'Publish Test Results'
        inputs:
          targetPath: '$(Pipeline.Workspace)\TestResults'
          artifact: 'GameSaveTestResults'
        condition: always()

      - task: PublishTestResults@2
        displayName: 'Publish Test Results to ADO'
        inputs:
          testResultsFormat: 'JUnit'
          testResultsFiles: '**/test-results.xml'
          searchFolder: '$(Pipeline.Workspace)\TestResults'
          failTaskOnFailedTests: true
        condition: always()
```

---

## Command-Line Interface Changes

### Test Controller New Parameters

Add the following command-line parameters to `PFGameSaveTestController`:

| Parameter | Description | Example |
|-----------|-------------|---------|
| `--headless` | Run without GUI, exit when complete | `--headless` |
| `--run-tag <tag>` | Run all scenarios with specified tag | `--run-tag passing` |
| `--run-scenario <id>` | Run specific scenario by ID | `--run-scenario Scenario-01` |
| `--scenarios-path <path>` | Override scenarios folder location | `--scenarios-path C:\Scenarios` |
| `--exit-on-complete` | Exit process when all tests finish | `--exit-on-complete` |
| `--results-file <path>` | Write JSON results to file | `--results-file results.json` |
| `--log-file <path>` | Write controller log to file | `--log-file controller.log` |

### Exit Codes

| Code | Meaning |
|------|---------|
| 0 | All tests passed |
| 1 | One or more tests failed |
| 2 | Configuration error (invalid arguments, missing scenarios) |
| 3 | Timeout waiting for devices |
| 4 | Fatal error during execution |

### PFGameSaveTestDeviceWindows.exe Parameters

Add/verify the following command-line parameters for `PFGameSaveTestDeviceWindows.exe`:

| Parameter | Description | Example |
|-----------|-------------|---------|
| `--controller-ip <ip>` | Controller IP address | `--controller-ip 127.0.0.1` |
| `--device-name <name>` | Device identifier | `--device-name DeviceA` |


---

## Artifact Packaging

### TestHarness Artifact Structure

```
TestHarness/
├── Controller/
│   ├── PFGameSaveTestController.exe
│   ├── PFGameSaveTestController.dll
│   ├── *.dll (dependencies)
│   └── appsettings.json
├── Device/
│   ├── PFGameSaveTestDeviceWindows.exe
│   ├── *.dll (PlayFab SDK, libHttpClient, etc.)
│   └── *.pdb (for debugging)
└── Scenarios/
    ├── scenario-01-single-device-golden-path.yml
    ├── scenario-02-two-device-golden-path.yml
    └── ... (all scenario files)
```

---

## Test Selection

### Phase 1 Smoke Tests (Tag: `passing`)

The following scenarios are currently tagged `passing` and will run in automated smoke tests:

| Scenario | Description | Devices |
|----------|-------------|---------|
| scenario-01-single-device-golden-path.yml | Init, upload, restart, download, verify | 1 |
| scenario-02-two-device-golden-path.yml | Device A uploads, Device B downloads | 2 |
| scenario-03-offline-to-online-reconnect.yml | Offline mode, reconnect, upload | 1 |
| scenario-04-active-device-handoff.yml | Device B takes control, Device A regains | 2 |
| scenario-05a-large-dataset-text.yml | ~100MB across ~1000 files | 1 |
| scenario-05b-large-dataset-binary.yml | ~100MB across ~1000 files | 1 |

### Phase 2: Component-Specific Tags

| Tag | Scenarios | Purpose |
|-----|-----------|---------|
| `grts` | Xbox Game Runtime scenarios | GRTS integration |
| `smoke` | Fast, critical-path tests | PR blocking |
| `nightly` | Longer-running tests | Nightly regression |

---

## Results and Logging

### JSON Results Format

```json
{
  "runId": "20260114-153045-abc123",
  "startTime": "2026-01-14T15:30:45Z",
  "endTime": "2026-01-14T15:35:22Z",
  "totalDuration": 277,
  "summary": {
    "total": 5,
    "passed": 4,
    "failed": 1,
    "skipped": 0
  },
  "scenarios": [
    {
      "id": "Scenario-01",
      "name": "Single Device Golden Path",
      "status": "passed",
      "duration": 45,
      "steps": [...]
    },
    {
      "id": "Scenario-02",
      "name": "Two Device Golden Path",
      "status": "failed",
      "duration": 62,
      "error": "Timeout waiting for DeviceB upload confirmation",
      "steps": [...]
    }
  ]
}
```

### Log Files Published

| File | Content |
|------|---------|
| `controller.log` | Full controller execution log |
| `DeviceA-stdout.txt` | Device A console output |
| `DeviceA-stderr.txt` | Device A error output |
| `DeviceB-stdout.txt` | Device B console output |
| `DeviceB-stderr.txt` | Device B error output |
| `test-results.json` | Structured test results |
| `test-results.xml` | JUnit format for ADO |

---

## Implementation Checklist

### Phase 1 Tasks

- [x] **Controller CLI**: Add `--headless`, `--run-tag`, `--exit-on-complete`, `--results-file` parameters
- [x] **Controller Results**: Implement JSON and JUnit result file writers
- [x] **Controller Exit Codes**: Return proper exit codes based on test results
- [ ] **Pipeline: Build**: Modify `gdk-vs2022-build.yml` to package test harness
- [ ] **Pipeline: Test**: Create `PlayFab.C.GameSaveTests.yml` to run tests
- [ ] **Scenarios**: Ensure `passing` tag is on the 5-6 smoke test scenarios

### Phase 2 Tasks

- [ ] **PFGameSaveTestDeviceXbox**: Create Xbox console version of the test device app by porting PFGameSaveTestDeviceWindows to DXToolkit (use PlayFabGameSaveSample-XboxConsole as baseline)
- [ ] **Xbox Deployment**: Create pipeline task to deploy PFGameSaveTestDeviceXbox to Xbox via `xbrun`
- [ ] **Xbox Launch**: Script to launch PFGameSaveTestDeviceXbox on Xbox via `xbrun`
- [ ] **GRTS Scenarios**: Tag scenarios that require GRTS with `grts` tag
- [ ] **Mixed Mode Tests**: Create scenarios that test inproc ↔ GRTS interop

---

## Risk Considerations

| Risk | Mitigation |
|------|------------|
| Flaky tests due to timing | Add retries, increase timeouts, use explicit wait conditions |
| Device process hangs | Implement process watchdog, force kill after timeout |
| Test pollution between runs | Clean save folder before each scenario, use unique CustomIDs |

---

## Appendix: Scenario Tag Reference

Current tags used in scenario YAML files:

| Tag | Count | Description |
|-----|-------|-------------|
| `passing` | 6 | Tests verified to pass reliably |
| `untested` | 15 | Tests not yet validated |
| `failing` | 1 | Known failing test |
| `harness` | 6 | Controller/harness infrastructure tests |
| `interactive` | 2 | Requires manual intervention |
| `chaos` | 1 | Chaos/stress testing |
| `grts` | 4 | Xbox Game Runtime Services tests |

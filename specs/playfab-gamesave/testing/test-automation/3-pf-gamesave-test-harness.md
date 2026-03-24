# PFGameSave Test Harness Guide

This document describes the PFGameSave test controller and device harnesses. It covers:

- Controller modes (GUI and CLI)
- Scenario manifest (`.yml`) structure used by the controller
- Lifecycle of commands issued to device agents
- Command reference, including parameters and expected behaviour

The guidance below applies to the Win32 test device located under `Test/PFGameSaveTestDeviceWindows` and the .NET 8 controller in `Test/PFGameSaveTestController`.

## Setup and Connection

### Controller Setup

Launch the controller (`PFGameSaveTestController.exe`) which starts a WebSocket server on port 5000. The controller GUI displays its IP address in the top-left panel, which remote devices will need to connect.

#### Windows Firewall Configuration

For remote device connections, Windows Firewall must allow incoming connections on TCP port 5000:

- **First-time setup**: Windows may prompt to allow the application when the controller first starts. Click "Allow access" for the appropriate network types (Private and/or Public).

- **Manual configuration**: If the prompt doesn't appear or was dismissed, create a firewall rule manually:
  ```powershell
  # Run PowerShell as Administrator
  New-NetFirewallRule -DisplayName "PFGameSave Test Controller" -Direction Inbound -Protocol TCP -LocalPort 5000 -Action Allow
  ```

- **Troubleshooting**: If remote devices cannot connect, verify the firewall rule exists:
  ```powershell
  Get-NetFirewallRule -DisplayName "PFGameSave Test Controller"
  ```

**Note**: Localhost connections (device and controller on the same machine) do not require firewall configuration.

#### HTTP Listener Permissions

The controller uses `HttpListener` to accept WebSocket connections on all network interfaces. This requires either running as Administrator or configuring a URL ACL (Access Control List).

**Option 1: Run as Administrator**

Simply right-click the controller executable and select "Run as administrator". This is the simplest approach for occasional use.

**Option 2: Configure URL ACL (Recommended for regular use)**

Register a URL ACL to allow the controller to run without Administrator privileges:

```powershell
# Run PowerShell as Administrator (one-time setup)
netsh http add urlacl url=http://+:5000/ws/ user=Everyone
```

After this, the controller can run without elevation.

**Managing URL ACLs**

```powershell
# List all URL ACLs (to verify configuration)
netsh http show urlacl

# Show specific URL ACL
netsh http show urlacl url=http://+:5000/ws/

# Remove URL ACL (cleanup)
netsh http delete urlacl url=http://+:5000/ws/
```

**Note**: The URL in the ACL must match exactly what the controller uses, including the trailing slash.

### Device Setup

Launch the test device with optional controller IP address:

```powershell
# Local connection (default)
PFGameSaveTestDeviceWindows.exe

# Remote connection
PFGameSaveTestDeviceWindows.exe /controller 192.168.1.100
```

The `/controller` parameter specifies the IP address where the controller is running. If omitted, the device defaults to `localhost`. The device will automatically attempt to connect to the controller's WebSocket server at `ws://<controller-ip>:5000/ws/` and will retry every 2 seconds until successful.

## Controller Modes

The test controller supports two operational modes:

### GUI Mode (Default)

Launch `PFGameSaveTestController.exe` without arguments to start the Windows Forms GUI. The GUI provides:

- **Chaos Testing**: Automated random mutation testing with configurable parameters
- **Test Harness**: Execute individual test scenarios with device selection
- **Test Scenarios**: Run scenario manifests from the `Scenarios/` folder
- **Manual API**: Interactive command execution with parameter dialogs

### CLI Mode

Launch with the `-cli` argument to enter command-line interface mode:

```powershell
PFGameSaveTestController.exe -cli
```

The CLI provides a PowerShell-like experience with:

- **Tab completion**: Press Tab to cycle through command and argument completions
- **Command history**: Use Up/Down arrows to navigate command history
- **Line editing**: Home, End, Left, Right arrows, Backspace, Delete, Escape to clear
- **Log interception**: Logs appear without disrupting the current input line

#### CLI Commands

| Command | Aliases | Description |
|---------|---------|-------------|
| `list-devices` | - | Lists all connected test devices with status and capabilities |
| `run <scenario>` | - | Executes a scenario file from the `Scenarios/` folder. Tab completion available for scenario files. |
| `chaos` | - | Runs a single iteration of chaos mode using `chaos-single-device.yml` |
| `chaos-forever` | - | Runs chaos mode continuously until Ctrl+C or failure. Tracks pass/fail statistics. |
| `set-chaos <param> <value>` | - | Configures chaos mode parameters. Available parameters: `FileCreate`, `FileModify`, `FileDelete`, `FolderCreate`, `FolderDelete`, `BinaryFiles`, `TextFiles`, `LargeFiles`, `UnicodeFiles`, `UnicodeFolders`, `OperationsPerUpload` (int), `NumUploads` (int). Boolean values default to `true`/`false`. |
| `manual-mode` | `manual` | Enters Manual API mode with access to all device commands |
| `manual-mode-exit` | - | Exits Manual API mode and returns to normal CLI |
| `help [command]` | - | Shows available commands or detailed help for a specific command |
| `exit` | - | Exits the CLI |

#### Manual API Mode

Within the CLI, enter Manual API mode to execute individual GameSave and platform commands interactively:

```powershell
> manual-mode
manual> PFInitialize
manual> WriteGameSaveData --fileCount=5 --maxFileSizeBytes=2048
manual> PFGameSaveFilesUploadWithUiAsync
manual> exit
```

Manual API mode provides:

- Direct access to all 54+ device commands (see Command Reference section)
- Parameter support via `--param=value` syntax
- Automatic type detection (boolean, integer, string)
- Tab completion for command names and parameters
- JSON response logging when commands return additional data
- Automatic device selection (uses first connected device)

Example commands with parameters:

```powershell
manual> WriteGameSaveData --fileCount=10 --maxFileSizeBytes=1024 --unicodeFiles=true
manual> DoChaosMode --fileCreate=true --operationsPerUpload=5 --numUploads=3
manual> SmokeDelay --delayMs=1000
```

## Scenario Manifest Overview

Scenarios live under `Test/PFGameSaveTestController/Scenarios/` and are authored in YAML. Each manifest defines the devices that participate in a run, the ordered blocks of commands for each role, and optional cleanup steps.

### Top-Level Fields

| Field | Required | Description |
|-------|----------|-------------|
| `id` | yes | Stable identifier for reports, e.g. `Scenario-03-Cross-Device-Snapshot`. |
| `name` | yes | Friendly title displayed in controller UI. |
| `description` | no | Short summary of scenario intent. |
| `devices` | yes | Map of role -> device metadata (may be empty). |
| `defaults` | no | Common settings such as `stepTimeoutSeconds`. |
| `blocks` | yes | Named command blocks. Each block is an array of steps that can be executed on any device. |
| `executionOrder` | yes | Specifies which blocks to execute on which devices (by role name) and in what order. |
| `cleanup` | no | Optional per-role cleanup commands executed after the main scenario completes (pass or fail). |

### Blocks and Steps

The `blocks` section defines named sequences of commands. Each block is an array of steps where each entry describes a single command invocation. Blocks can be reused across multiple devices by referencing the same block name in different execution order entries.

A step supports the following keys:

| Step Field | Required | Description |
|------------|----------|-------------|
| `command` | yes | Command name dispatched to the device or controller. |
| `parameters` | no | JSON object encoded in YAML. Interpreted by the command handler. |
| `timeoutSeconds` | no | Overrides the default timeout for the step. |
| `expectedCallbacks` | no | (Future use) Specifies callbacks the controller should respond to. |

### Cleanup Blocks

Cleanup commands run after the main execution finishes (even on failure) unless the scenario is cancelled. The controller continues through cleanup even if individual cleanup commands fail.

### Example Scenario

```yaml
id: Scenario-03-Cross-Device-Snapshot
name: Cross-Device Snapshot Parity
devices:
  DeviceA: {}
  DeviceB: {}
defaults:
  stepTimeoutSeconds: 120
blocks:
  seed-and-capture:
    - command: DeleteSaveRoot
    - command: WriteGameSaveData
      parameters:
        operations:
          - verb: CreateBinaryFile
            relativePath: parity/payload.bin
            bytes: 2048
            pattern: [0xDE, 0xAD, 0xBE, 0xEF]
    - command: CaptureSaveContainerSnapshot
  compare:
    - command: CompareSaveContainerSnapshots
executionOrder:
  - role: DeviceA
    block: seed-and-capture
  - role: DeviceB
    block: seed-and-capture
  - role: DeviceA
    block: compare
cleanup:
  DeviceA:
    - command: DeleteSaveRoot
  DeviceB:
    - command: DeleteSaveRoot
```

## Command Reference

The controller dispatches commands to device agents over WebSockets. Most commands execute on the device; a few (currently only `CompareSaveContainerSnapshots`) run locally on the controller. Commands are grouped below by subsystem.

### Controller-Managed Commands

| Command | Parameters | Description |
|---------|------------|-------------|
| `CompareSaveContainerSnapshots` | none | Performed by the controller. Reads the two most recent `CaptureSaveContainerSnapshot` results for participating devices, compares manifests, and logs any differences. Fails the scenario when differences are detected or a snapshot is missing. |

### Smoke & Infrastructure Commands

| Command | Parameters | Description |
|---------|------------|-------------|
| `SmokeCommandEcho` | `payload` (string) | Device echoes the supplied payload to validate request/response plumbing. |
| `SmokeDelay` | `durationMs` (int) | Introduces an artificial delay for timing tests. |
| `SmokeTestFailure` | none | Deliberately fails so the controller can verify error-handling paths. |

### XGame Runtime & Platform Initialization

| Command | Parameters | Description |
|---------|------------|-------------|
| `XGameRuntimeInitialize` | none | Initializes the GDK runtime on the device. |
| `XTaskQueueCreate` | `workMode`, `completionMode`, `setAsProcessQueue` (bool, optional) | Creates an `XTaskQueue`. Modes accept `ThreadPool` or `SerializedThread`. |
| `XTaskQueueCloseHandle` | none | Closes the queue created earlier. |
| `XUserAddAsync` | `signInOptions` (optional), `ageGroup` (optional) | Starts an Xbox Live sign-in sequence. Parameters follow the Xbox Game Runtime mock options (see handler for optional extensions). |
| `XUserCloseHandle` | none | Releases the signed-in user handle. |

### PlayFab Core Runtime Commands

| Command | Parameters | Description |
|---------|------------|-------------|
| `PFInitialize` | none | Boots the PlayFab core runtime. |
| `PFUninitializeAsync` | none | Asynchronously tears down PlayFab core. |
| `PFServicesInitialize` | none | Enables PlayFab services, HTTP pipeline, and auth. |
| `PFServicesUninitializeAsync` | none | Shuts down the services layer. |
| `PFServiceConfigCreateHandle` | `titleId` (string), `endpoint` (string) | Creates a title configuration handle for subsequent API calls. |
| `PFServiceConfigCloseHandle` | none | Releases the title configuration handle. |

### GameSave Specific API Commands

| Command | Parameters | Description |
|---------|------------|-------------|
| `PFGameSaveFilesInitialize` | `saveFolder` (optional string) | Initializes the GameSave Files module and binds it to the active queue. |
| `PFGameSaveFilesSetUiCallbacks` | `enable` (bool, default `true`) | Registers or clears the built-in UI callbacks for conflict, sync failed, progress, etc. When disabled the harness will not receive those notifications. |
| `PFGameSaveFilesSetActiveDeviceChangedCallback` | none | Registers a callback that fires when the active device changes. |
| `PFGameSaveFilesResetCloudAsync` | none | Resets cloud state for the active GameSave container. |
| `PFGameSaveFilesAddUserWithUiAsync` | fields covering mount behaviour (e.g., `rollbackOption`) | Mounts a user’s save container via interactive UI flow. |
| `PFGameSaveFilesSetUiSyncFailedResponse` | `action` (string enum) | Sets the scripted response to SyncFailed callbacks (`Retry`, `UseOffline`, `Cancel`). |
| `PFGameSaveFilesSetUiActiveDeviceContentionResponse` | `action` (enum) | Sets response to contention prompts (`Retry`, `SyncLastSavedData`, `Cancel`). |
| `PFGameSaveFilesSetUiConflictResponse` | `action` (enum) | Chooses how to resolve conflict prompts (`UseLocal`, `UseCloud`, `Cancel`). |
| `PFGameSaveFilesSetUiOutOfStorageResponse` | `action` (enum) | Controls the OutOfStorage prompt behaviour (`Retry`, `Cancel`). |
| `PFGameSaveFilesSetUiProgressResponse` | `action` (enum) | Chooses the response to progress prompts (`Cancel` is currently the only supported action). |
| `PFGameSaveFilesSetUiSyncFailedAutoResponse` | `enable` (bool, default `true`), `action` (enum, required when enabling) | Enables automatic responses for SyncFailed callbacks. When enabled the device immediately calls `PFGameSaveFilesSetUiSyncFailedResponse` with the stored action whenever the callback fires. |
| `PFGameSaveFilesSetUiActiveDeviceContentionAutoResponse` | `enable` (bool, default `true`), `action` (enum, required when enabling) | Configures automatic responses for ActiveDeviceContention callbacks. |
| `PFGameSaveFilesSetUiConflictAutoResponse` | `enable` (bool, default `true`), `action` (enum, required when enabling) | Configures automatic responses for Conflict callbacks. |
| `PFGameSaveFilesSetUiOutOfStorageAutoResponse` | `enable` (bool, default `true`), `action` (enum, required when enabling) | Configures automatic responses for OutOfStorage callbacks. |
| `PFGameSaveFilesSetUiProgressAutoResponse` | `enable` (bool, default `true`), `action` (enum, required when enabling) | Configures automatic responses for Progress callbacks. |
| `PFGameSaveFilesUploadWithUiAsync` | `mode` (enum e.g. `ReleaseDeviceAsActive`, `KeepDeviceActive`, `OfflineDeferred`) | Uploads current save data via UI workflow. |
| `PFGameSaveFilesGetFolder` | none | Returns the local save root path. |
| `PFGameSaveFilesGetRemainingQuota` | none | Reads remaining cloud quota for the current title/user. |
| `PFGameSaveFilesIsConnectedToCloud` | none | Indicates whether the device is online with GameSave services. |
| `PFGameSaveFilesUninitializeAsync` | none | Uninitializes the GameSave Files subsystem. |

When an auto-response command is disabled (`enable: false`) the harness clears the stored action and falls back to manual responses via the `PFGameSaveFilesSetUi*Response` commands. Enabling requires both `enable: true` (or omission) and an `action` value that matches the corresponding manual handler.

#### GameSave Debug Helpers

| Command | Parameters | Description |
|---------|------------|-------------|
| `PFGameSaveFilesSetMockDeviceIdForDebug` | `deviceId` (string) | Spoofs the device ID for subsequent API calls to simulate multi-device behaviour. |
| `PFGameSaveFilesSetForceOutOfStorageErrorForDebug` | `forceError` (bool) | Forces the next storage call to report an out-of-storage condition when `true`. |
| `PFGameSaveFilesSetForceSyncFailedErrorForDebug` | `forceError` (bool) | Forces the next sync to fail with a controlled error when `true`. |
| `PFGameSaveFilesSetWriteManifestsToDiskForDebug` | `writeManifests` (bool) | Persists manifest snapshots to disk for offline inspection. |
| `PFGameSaveFilesSetActiveDevicePollForceChangeForDebug` | none | Immediately triggers the active-device poller to treat the current device as changed. |
| `PFGameSaveFilesSetActiveDevicePollIntervalForDebug` | `interval` (unsigned int milliseconds) | Updates the active-device polling cadence used by the debug helpers. |

### PlayFab Local User Commands

| Command | Parameters | Description |
|---------|------------|-------------|
| `PFLocalUserCreateHandleWithXboxUser` | none | Wraps the current Xbox user in a PlayFab local user handle. |
| `PFLocalUserCreateHandleWithPersistedLocalId` | `customId` (string), optional auth fields | Rehydrates a custom ID user from persisted state. |
| `PFLocalUserCloseHandle` | none | Releases the PlayFab local user handle. |

### Harness Utility Commands

#### Mutation & Cleanup

| Command | Parameters | Description |
|---------|------------|-------------|
| `WriteGameSaveData` | Either `operations` array or `chaos` object | Performs deterministic or random file mutations inside the save root. `operations` supports verbs such as `CreateBinaryFile`, `RenameFile`, `DeleteFolder`, etc. `chaos` describes a PRNG-based mutation batch with `operationCount`, optional `seed`, and verb filters. |
| `DoChaosMode` | `scenarios` (object) | Applies the chaos plan emitted by `Test/PFGameSaveTestController/ChaosModeScenarioParameters`, allowing randomized stress of GameSave APIs during a scenario. |
| `DeleteSaveRoot` | `preserveManifest` (bool, optional) | Recursively deletes the harness-managed save root. Optional flag preserves manifests so later comparisons remain possible. |
| `DeleteLocalFolder` | `folderPath` (string) | Removes an arbitrary folder on disk; useful for clearing auxiliary data that sits outside the save root. |
| `ConsumeDiskSpace` | `bytes` (int64), optional `scope` (`user`, `system`, `custom`), optional `customPath`, optional `token` | Uses a mock API to reserve disk space and report allocation details. Emits a token used to release the reservation. |
| `ReleaseDiskSpace` | Either `token` (string) or `scope` | Frees disk space previously reserved by `ConsumeDiskSpace`. |

#### Capture & Diagnostics

| Command | Parameters | Description |
|---------|------------|-------------|
| `CaptureSaveContainerSnapshot` | `includeHashes` (bool, default `true`) | Enumerates the save root and returns a manifest (`snapshot` field) containing entries, timestamps, attributes, and optional SHA-256 hashes. |
| `GatherSnapshot` | none | Produces a `.zip` archive (capped at 16 MiB) of the current save root so the controller can persist a forensic package (returned as base64 plus device metadata). A legacy alias `GatherSnapsnot` maps to the same handler. |
| `GatherLogs` | `maxBytes` (optional) | Flushes the device log, streams back up to 512 KiB from the log tail, and reports the originating file path. |
| `GetDebugStats` | none | Calls `PFGameSaveFilesGetStatsJsonForDebug` and returns the raw JSON plus device metadata for post-scenario auditing. |

### Snapshot Comparison Support Types

When `CaptureSaveContainerSnapshot` succeeds, the action result includes:

- Aggregate counts (`filesCaptured`, `directoriesCaptured`, `totalBytes`)
- `snapshot` object (schema version 1) containing `entries`, `totals`, and metadata such as `capturedAtUtc`

The controller persists the raw JSON in-memory for later comparisons and logs it to verbose output to aid triage.

## Execution Flow Summary

1. Operator selects a scenario manifest.
2. Controller matches required roles to connected devices.
3. For each step, the controller sends a JSON command envelope to the target device (or runs it locally for controller-managed commands).
4. Device executes the handler, emits logs, and returns an `actionResult` payload with `status`, `hresult`, timings, and optional artifacts or snapshot JSON.
5. Controller logs the action result (verbose channel retains the raw JSON) and moves to the next step.
6. After all primary steps finish, cleanup commands run as best-effort tasks.

Refer to the source under `Test/PFGameSaveTestController` and `Test/PFGameSaveTestDeviceWindows` for implementation details and the most current list of commands.

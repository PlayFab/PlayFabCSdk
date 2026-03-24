# PFGameSave Test Controller & Device Orchestration Specification

## Purpose & Scope
This document defines the controller and device-agent capabilities required to execute the PFGameSave Test Automation Plan located at `specs/playfab-gamesave/testing/test-automation/test-automation-plan.md`. The goal is to deliver a resilient, multi-device automation harness that can run the full test scenario list, expose actionable logs, and scale across Xbox, Windows MSIXVC, Steam, and Steam Deck targets without bespoke one-off scripts. The specification covers controller responsibilities, device-agent behaviour, and data flows.

## Guiding Goals
- **Parity with the automation plan**: Every scenario from the test plan must be expressible as a controller job without manual glue code or ad-hoc device setup.
- **Deterministic orchestration**: Runs should be reproducible, with scenario scheduling, timing, and resource cleanup controlled centrally.
- **Device-class agility**: The controller stays platform-neutral while device-specific agents implement the shared protocol so new hardware SKUs or emulators can be added by delivering a compatible agent build.
- **Evidence-first diagnostics**: Capture HRESULTs, logs, and artifacts in a standard bundle that can be consumed by CI systems and engineers.
- **Operational sustainability**: The solution should support unattended execution (per-commit, nightly, soak) with health monitoring and human-friendly failure summaries.

## Functional Overview
The system consists of:
1. **Test Controller**: Holds the scenario list and, when a scenario is executed, steps through each action in order while issuing commands to connected devices one at a time. All staging and execution work is triggered through these call-and-response commands; devices never act autonomously. After each device reports back—or the configured timeout is hit—the controller issues the next action or cancel on the appropriate device until the scenario completes.
2. **Device Agents**: Lightweight listeners on each device that receive the controller’s call, execute the requested action (launch harness step, toggle mock, call APIs, capture output), and issue a response with success or failure data before the controller proceeds.
3. **Scenario Definitions**: Ordered call-and-response scripts that map directly to the test automation plan’s steps so the controller knows which device to ask for each action and what result to expect.

## Test Controller Responsibilities
- Parse scenario manifests, resolve required assets (payload seeds, mock definitions, quota baselines), and drive all staging actions through explicit call-and-response commands to each device.
- Pick devices that match the scenario requirements (platform, hardware capabilities, input devices) before launching each scenario.
- Establish channels to each device agent (plain WebSocket or custom UDP/TCP packets) for command dispatch and log streaming; internal lab deployments run without TLS, so isolation and shared secrets provide the security boundary.
- Enforce per-step timeouts and cancel actions when devices stop responding; fail the scenario when a timeout cannot be recovered.
- Execute scenario steps as state machines: each call waits for the device response, records start/end timestamps, HRESULTs, artifacts, and pass/fail state.
- Collect scenario metadata, per-step metrics, device health snapshots, and artifact links.
- Provide post-run summaries (Markdown or plain text) suitable for CI gating and manual triage.

## Device Agent Responsibilities
- Execute controller-issued commands exactly as requested (deploy assets, launch harness binaries, toggle mocks) without autonomous staging between steps, always waiting for the next call.
- Implement platform-specific actions behind the shared protocol so commands behave consistently even when device capabilities differ.
- Listen for controller commands, run the requested action, and return status plus any requested outputs as the response in the call-and-response loop.
- Send log lines back to the controller during and after each action.
- Capture artifacts on demand (screenshots, dumps, manifest copies) and reset local state when the controller asks.

## Scenario Definition Schema
Each scenario (matching the automation plan numbering) should call out:
- **Overview**: short name plus any prerequisites.
- **Devices**: which device plays each role.
- **Steps**: ordered actions with the expected result or HRESULT.
- **Timeouts**: per-step limits plus the controller action when a step exceeds its window.
- **Mocks & data**: required mock profiles, payload seeds, or service overrides to load before each step begins.
- **Callbacks**: scripted UI responses the agent should return.
- **Cleanup**: anything that must be reset before the next scenario.

## Device Matrix & Lab Requirements
- **Windows MSIXVC PCs**: Run the Windows build of the harness with Xbox identity enabled.
- **Xbox GDK Consoles**: Developer-enabled consoles able to launch the harness title and send logs.
- **Steam PC & Steam Deck**: Steam runtimes with controller agent installed and ready to answer prompts.
- **Future Platforms**: Keep a slot reserved so new device types can be added when the scenarios expand.

## Logging & Diagnostics
- Capture step-by-step logs with timestamps, HRESULTs, agent build hash, and device identity (hardware ID, sandbox, signed-in account).
- Save requested artifacts (manifests, screenshots, dumps) when the controller asks.
- Package the logs and artifacts together so runs can be reviewed later, tagging bundles with scenario manifest version and controller commit.

## Scenario Actions

### API Actions
- `XGameRuntimeInitialize`: Start the GDK/Windows runtime on devices that require it.
- `PFInitialize`: Bring up the PlayFab core runtime before issuing service calls.
- `PFServicesInitialize`: Enable PlayFab service access (auth, HTTP stack) for the job.
- `PFServiceConfigCreateHandle`: Create the title configuration handle reused by later calls.
- `XTaskQueueCreate`: Provision the async queue that backs SDK callbacks.
- `PFGameSaveFilesInitialize`: Initialize the GameSave Files module for the active queue.
- `PFGameSaveFilesSetUiCallbacks`: Register UI handlers (progress, conflict, sync-failed, out-of-storage, contention).
- `PFGameSaveFilesSetActiveDeviceChangedCallback`: Listen for active device ownership changes.
- `XUserAddAsync`: Sign in an Xbox Live user on GDK targets.
- `PFLocalUserCreateHandleWithXboxUser`: Wrap the signed-in Xbox user for PlayFab calls.
- `PFLocalUserCreateHandleWithPersistedLocalId`: Rehydrate a custom ID user (logs in via PlayFab).
- `PFGameSaveFilesResetCloudAsync`: Clear cloud state so scenarios start from a known baseline.
- `PFGameSaveFilesAddUserWithUiAsync`: Mount the save root and download the current manifest.
- `PFGameSaveFilesAddUserWithUiAsync` w/ `RollbackToLastKnownGood`: Hydrate the last promoted good manifest.
- `PFGameSaveFilesAddUserWithUiAsync` w/ `RollbackToLastConflict`: Hydrate the preserved conflict loser manifest.
- `PFGameSaveFilesSetUiSyncFailedResponse(... Retry)`: Tell the agent to retry a transient sync failure.
- `PFGameSaveFilesSetUiSyncFailedResponse(... UseOffline)`: Drop into offline mode and defer work.
- `PFGameSaveFilesSetUiActiveDeviceContentionResponse(... Retry)`: Retry when another device holds the lock.
- `PFGameSaveFilesSetUiActiveDeviceContentionResponse(... SyncLastSavedData)`: Accept cloud state and release the lock holder.
- `PFGameSaveFilesSetUiConflictResolutionResponse(... UseLocal)`: Keep the local edits during a conflict prompt.
- `PFGameSaveFilesSetUiConflictResolutionResponse(... UseCloud)`: Overwrite local edits with the cloud version.
- `PFGameSaveFilesSetUiOutOfStorageResponse(... Retry)`: Resume after freeing disk space.
- `PFGameSaveFilesSetUiOutOfStorageResponse(... Cancel)`: Abort when storage cannot be reclaimed.
- `PFGameSaveFilesSetUiProgressResponse(... Cancel)`: Simulate a user cancel mid-progress.
- `PFGameSaveFilesUploadWithUiAsync`: Upload the active payload using default options.
- `PFGameSaveFilesUploadWithUiAsync` w/ `ReleaseDeviceAsActive`: Upload and relinquish the active lock.
- `PFGameSaveFilesUploadWithUiAsync` w/ `KeepDeviceActive`: Upload while retaining the active lock.
- `PFGameSaveFilesUploadWithUiAsync` (offline deferred): Queue work while offline so it flushes on reconnect.
- `PFGameSaveFilesGetFolder`: Retrieve and enumerate the save root for file IO, hashing, and deletion checks.
- `PFGameSaveFilesGetRemainingQuota`: Read remaining cloud quota before large uploads.
- `PFGameSaveFilesIsConnectedToCloud`: Confirm whether the device is online or offline.
- `PFLocalUserCloseHandle`: Dispose of the active PlayFab user handle.
- `PFGameSaveFilesUninitializeAsync`: Tear down the GameSave Files module.
- `PFServicesUninitializeAsync`: Shut down PlayFab service access.
- `PFUninitializeAsync`: Uninitialize the PlayFab core runtime.
- `PFServiceConfigCloseHandle`: Close the title configuration handle.
- `XTaskQueueCloseHandle`: Destroy the task queue that was created for the run.
- `XUserCloseHandle`: Sign out and clean up the Xbox user handle on GDK devices.

### Harness Commands
- `WriteGameSaveData`: Use harness tooling to seed multi-file payload datasets.
- `DeleteSaveRoot`: Recursively remove the save root folder and local metadata.
- `CaptureSaveContainerSnapshot`: Return metadata (file and folder list, sizes, file dates, hashes) to compare content between devices.
- `CompareSaveContainerSnapshots`: Diff two captured snapshots and report any mismatches.
- `ConsumeDiskSpace`: Use mock API to fill local storage to trigger out-of-space behaviours.
- `ReleaseDiskSpace`: Remove mock API to restore normal free space levels.
- `ConfigureHttpMock`: Enable or disable HTTP fault injection for specified PlayFab routes.

## Device Events
- **agentStatus**: Emitted when the agent connects (includes device identity, platform sku, harness build/hash).
- **actionResult**: Response to a controller-issued command; reports success/failure, HRESULT, elapsed time, and any artifacts captured.
- **callbackEvent**: Sent when an SDK UI callback fires so the controller can choose the matching `PFGameSaveFilesSetUi*Response`.

	```json
	{
		"type": "callbackEvent",
		"actionId": "addUser-step-6",
		"deviceId": "steamdeck-05",
		"timestamp": "2025-11-09T17:42:11.320Z",
		"callback": {
			"name": "SyncFailed",
			"context": {
				"hresult": "E_PF_GAMESAVE_NETWORK_ERROR"
			}
		}
	}
	```

	The controller records the callback, chooses the scripted response, and sends the matching command (for example `PFGameSaveFilesSetUiSyncFailedResponse`).
- **logChunk**: Batched harness/SDK logs streamed during long actions for real-time diagnostics.
- **artifactReady**: Notification that a requested artifact (screenshot, manifest snapshot, dump) has been saved, including its name, size, and URI.
- **deviceAlert**: Unplanned issues such as harness crashes, lost network, or low disk so the controller can fail the scenario or requeue the device.

### Device Event Payloads
- `agentStatus`
  - `type` (string, value `agentStatus`)
  - `agentId` (string, required)
  - `deviceId` (string, required)
  - `lastActionId` (string, optional)
  - `timestamp` (ISO 8601 string, required)
- `actionResult`
  - `type` (string, value `actionResult`)
  - `commandId` (string, required)
  - `deviceId` (string, required)
  - `status` (string enum: `succeeded`, `failed`, `cancelled`)
  - `hresult` (string, required when `status != succeeded`)
  - `elapsedMs` (integer, required)
  - `artifacts` (array of strings, optional; names that will later appear in `artifactReady` events)
  - `metrics` (object, optional; step-specific counters such as bytesTransferred)
  - `timestamp` (ISO 8601 string, required)
- `logChunk`
  - `type` (string, value `logChunk`)
  - `deviceId` (string, required)
  - `actionId` (string, optional; populated when tied to an in-flight command)
  - `sequence` (integer, required; monotonically increasing per action to preserve order)
  - `lines` (array of strings, required; newline-stripped log entries)
  - `timestamp` (ISO 8601 string, required)
- `artifactReady`
  - `type` (string, value `artifactReady`)
  - `deviceId` (string, required)
  - `artifactName` (string, required; relative path inside the scenario bundle)
  - `sizeBytes` (integer, required)
  - `contentType` (string, optional; MIME type hint for consumers)
  - `timestamp` (ISO 8601 string, required)

### Artifact Transfer Channel
- **Transport**: Artifacts are streamed over the same WebSocket connection using binary frames. Each payload contains the raw artifact bytes without additional framing; the controller associates a binary frame with the most recent `artifactReady` event emitted by the agent.
- **Ordering**: Agents must send the `artifactReady` event before pushing the corresponding binary frame so the controller has the metadata needed to route the bytes into the run bundle.
- **Chunking**: Large artifacts are split across multiple binary frames. The `artifactReady` event includes the total `sizeBytes`; the controller considers the transfer complete once it has received that many bytes across consecutive frames.
- **Back-pressure**: Controllers acknowledge each artifact via a `artifactAck` command (command envelope) when ingestion is complete; agents wait for the ack before sending the next artifact to avoid overwhelming the controller.

## Control Channel Contract
- **Transport**: Agents open an outbound WebSocket to the controller (`ws://<controller-host>/pf-gs-agent` in the lab environment) and refresh when the socket is dropped. When production hardening is required, re-enable TLS and update endpoints to `wss://`.
- **Registration**: On connect the agent immediately sends an `agentStatus` frame with `agentId`, `agentVersion`, and `sessionToken`; the controller records capabilities and may respond with queued work once validation succeeds.
- **Reconnect**: Agents back off exponentially (5s, 15s, 45s, 120s) when reconnecting; controller replays any unacknowledged command if the previous session ended mid-action.
- **Shutdown**: Controller sends `terminate` when maintenance is needed; agents stop accepting new work, finish current action, send final `agentStatus`, then close.

## Command Envelope Schema
- **Purpose**: All controller→agent calls use a shared envelope so commands and parameters are parsed consistently across platforms.
- **Fields**:
  - `commandId` (string, required): unique per action instance; reused in `actionResult`.
  - `command` (string, required): name from Scenario Actions list.
  - `parameters` (object, optional): command-specific JSON payload.
  - `timeoutSeconds` (int, required): hard deadline for agent execution.
  - `requestedArtifacts` (array, optional): list of artifact kinds to capture before returning.
  - `scenarioContext` (object, optional): includes scenario ID and step order for logging.

  ```json
  {
    "commandId": "upload-step-14",
    "command": "PFGameSaveFilesUploadWithUiAsync",
    "parameters": {
      "mode": "ReleaseDeviceAsActive"
    },
    "timeoutSeconds": 300,
    "requestedArtifacts": [
      "manifestSnapshot",
      "logs"
    ],
    "scenarioContext": {
      "scenarioId": "Scenario-12",
      "step": 14
    }
  }
  ```

- **Parameter Encoding**: Primitive JSON types only (strings, numbers, booleans); binary data referenced by `artifactReady`. Enum values use PascalCase strings matching harness APIs. Agents must return `invalidCommand` if required parameters are missing.
- **Standard Commands**: In addition to scenario actions, controllers may send infrastructure commands such as `artifactAck` with parameters `{ "artifactName": "manifests/deviceA-manifest.json" }` to confirm artifact ingestion before the agent streams the next file. When TLS is disabled for lab runs, rely on shared secrets in these envelopes to guard against spoofed traffic.

## Scenario Manifest Format
- **Location**: Stored under `Test/PFGameSaveTestController/Scenarios/` with `.yml` extension.
- **Schema**: Each manifest includes metadata, devices, blocks, execution ordering, and cleanup.

  ```yaml
  id: Scenario-05
  name: Cloud Reset then Upload
  description: Reset cloud state, seed payload, upload while retaining active lock
  devices:
    DeviceA:
      role: windowsHarness
  defaults:
    stepTimeoutSeconds: 120
    cleanupTimeoutSeconds: 120
  blocks:
    resetAndUpload:
      - command: EnsureHarnessReady
      - command: PFGameSaveFilesResetCloudAsync
      - command: WriteGameSaveData
        parameters:
          dataset: largePayload
      - command: PFGameSaveFilesUploadWithUiAsync
        parameters:
          mode: KeepDeviceActive
        timeoutSeconds: 300
        expectedCallbacks:
          - name: Progress
            response: Continue
  executionOrder:
    - role: DeviceA
      block: resetAndUpload
  cleanup:
    DeviceA:
      - command: DeleteLocalSaveContainer
  ```

- **Validation**: Controller rejects manifests missing required sections, unknown commands, or devices that cannot be matched. Blocks execute according to `executionOrder`; within a block the controller uses step array position as the sequence index. When `timeoutSeconds` is omitted the controller applies the scenario defaults, falling back to block-level overrides when present.

## Artifact Packaging
- **Bundle Layout**: Each scenario run produces `artifacts/<scenarioId>/<timestamp>-<runId>/` containing:
  - `logs/controller.log`
  - `logs/<deviceId>.log`
  - `manifests/<deviceId>-manifest.json`
  - `saveroot/<deviceId>/` (recursive save of the root tree)
  - `screenshots/<deviceId>-<step>-<label>.png`
  - `dumps/` (optional crash dumps)
- **Compression**: Controller zips the bundle (`.zip`) before uploading to CI artifact storage; large dumps (>200 MB) remain as separate blobs referenced in `artifactReady`.

## Failure & HRESULT Contract
- **Command Outcomes**: Agents set `actionResult.status` to `succeeded`, `failed`, or `cancelled`. When `failed`, the `hresult` field carries the precise HRESULT returned by the SDK or harness; the controller persists this value verbatim in logs and manifests it in run summaries.
- **Transport vs. Execution Failures**: If the agent cannot deliver an `actionResult` (socket drop, timeout), the controller treats it as a transport failure and cancels the scenario unless the action is explicitly marked retriable in orchestration logic.
- **Retry Policy**: The controller does not auto-retry failed actions. Any retry semantics must be authored as explicit additional steps in the scenario manifest.
- **Fatal vs. Recoverable**: Agents classify internal faults: unrecoverable issues (e.g., harness crash, corrupted install) emit `deviceAlert` with severity `fatal`, prompting the controller to mark the device unavailable. Recoverable issues (e.g., transient HRESULTs) should surface through `actionResult` so the scenario script can decide how to proceed.
- **Telemetry Expectations**: Both controller and agent log every HRESULT alongside timestamps, device identifiers, and command context to support postmortems.

## Controller Lifecycle
- **Startup Sequence**:
  1. Load scenario manifests and validate schema.
  2. Initialize persistent storage (artifact root, logs) and prune bundles beyond retention policy.
  3. Start the WebSocket listener and accept inbound agent connections.
  4. Reconcile agent roster—clear stale device entries and mark previously connected agents as pending until a fresh `agentStatus` arrives.
  5. Begin scheduling scenarios once the required roles are online.
- **Shutdown Sequence**:
  1. Stop assigning new scenarios and wait for in-flight actions to reach a terminal state.
  2. Issue `terminate` to connected agents so they unwind cleanly.
  3. Flush logs, finalize artifact bundles, and upload any remaining evidence.
  4. Persist controller state (last run IDs, manifest hashes) for continuity on next startup.

## Operator UI Expectations
- **Controller Dashboard**: A C# WinForms desktop console surfaces queue state (pending/running/completed scenarios), per-device status cards (platform SKU, timestamp of the latest `agentStatus`, current command if any), and a notification log that records failure summaries (scenario ID, failing command, HRESULT) with deep links to artifact bundles. Long-running scenarios display progress bars keyed off step numbers. Operators can pause scheduling, cancel a scenario, or download the latest artifact bundle from this view. No manual command injection is supported; interactions are limited to selecting scenarios, viewing logs, and issuing `cancel`/`pause` controls provided by the controller.
- **Device Agent Shell**: Agents may operate headless. When a visible UI is requested to present, a minimalist status window should show connection state (connected/disconnected), current command name, and a rolling log of last 20 entries. All other operations remain controller-driven. The window also indicates when the agent is in `terminate` drain mode so techs know to leave the device idle.

### Harness Command Details
The following commands are fulfilled by the device agent harness rather than the PlayFab SDK. They combine local file system operations, harness utilities, and PlayFab SDK helpers to prepare or validate save-state before and after service calls.

#### WriteGameSaveData
- **Purpose**: Writes save file or folder (single file, multi-file, or dataset manifest) so subsequent uploads exercise size and conflict scenarios.
- **Primary APIs**: `PFGameSaveFilesGetFolder`, Win32/Posix file I/O (`CreateDirectory`, `CopyFile`, `WriteFile`), harness hashing utilities for post-write verification.
- **Parameters**:

  | Parameter | Type | Required | Default | Description |
  |-----------|------|----------|---------|-------------|
  | `operations` | array<object> | conditional | — | Ordered list of folder mutations. Required unless a `chaos` block is supplied. Each entry specifies a `verb` and the fields described below so the agent can reproduce the legacy folder monitor save-folder edits. |
  | `chaos` | object | conditional | — | Instructs the agent to synthesize random edits. Required when `operations` is omitted; may be combined with `operations` to append deterministic steps after the chaotic batch. |

  **Supported verbs**

  | Verb | Required Fields | Behaviour |
  |------|-----------------|-----------|
  | `CreateBinaryFile` | `relativePath`, `bytes`, `pattern` | Writes a binary file inside the save root using the supplied byte count and repeating `pattern` (array of up to 16 bytes). |
  | `CreateRandomBinaryFile` | `relativePath`, `bytes` | Emits a pseudo-random binary payload of the requested size so diffing tools can verify entropy coverage. |
  | `CreateRandomText` | `relativePath`, `bytes` | Emits a random-text file of the requested size at the given relative path. |
  | `CreateFolder` | `relativePath` | Ensures a directory exists beneath the save root, creating intermediate folders as needed. |
  | `CopyFile` | `sourceRelativePath`, `destinationRelativePath` | Copies a file that already exists under the save root from `sourceRelativePath` to `destinationRelativePath`, both expressed relative to the save root. |
  | `RenameFile` | `sourceRelativePath`, `destinationRelativePath` | Moves or renames a file within the save root, using relative paths for both the original and new locations. |
  | `RenameFolder` | `sourceRelativePath`, `destinationRelativePath` | Moves or renames a folder (and its contents) within the save root using relative paths for source and destination. |
  | `DeleteFolder` | `relativePath` | Recursively deletes the folder beneath the save root. |
  | `DeleteFile` | `relativePath` | Deletes a single file beneath the save root using the provided relative path. |

  Example scenario snippet that uses the verbs above:

  ```yaml
  blocks:
    writeOperations:
      - command: WriteGameSaveData
        parameters:
          operations:
            - verb: CreateBinaryFile
              relativePath: seeds/payload.bin
              bytes: 4096
              pattern: [0xAA, 0x55]
            - verb: CreateRandomBinaryFile
              relativePath: entropy/random-blob.bin
              bytes: 2048
            - verb: CreateRandomText
              relativePath: notes/run-log.txt
              bytes: 512
            - verb: CreateFolder
              relativePath: temp/cache
            - verb: CopyFile
              sourceRelativePath: datasets/base/manifest.json
              destinationRelativePath: manifests/current.json
            - verb: RenameFile
              sourceRelativePath: manifests/current.json
              destinationRelativePath: manifests/current-v2.json
            - verb: RenameFolder
              sourceRelativePath: seeds
              destinationRelativePath: seeds-archived
            - verb: DeleteFolder
              relativePath: temp/cache
            - verb: DeleteFile
              relativePath: old-payloads/cloud.bin
  ```

  **Chaos configuration**

  When the `chaos` block is present the agent generates a reproducible stream of random file and folder mutations before replaying any explicit `operations`. The harness uses a deterministic PRNG so the same seed and initial save-root contents yield identical edits across devices. The block supports the following fields:

  | Field | Type | Required | Default | Description |
  |-------|------|----------|---------|-------------|
  | `operationCount` | int | yes | — | Number of random mutations to perform. Each mutation maps to one of the supported verbs listed above. |
  | `seed` | int | no | derived from `commandId` | Overrides the PRNG seed so multiple devices can execute the exact same chaos run. |
  | `verbs` | array<string> | no | `[CreateRandomBinaryFile, CreateRandomText, CreateFolder, RenameFile, RenameFolder, DeleteFile, DeleteFolder]` | Limits the verb pool used by chaos mode. Verbs omitted here are never selected. |
  | `maxFileBytes` | int | no | `32768` | Upper bound (inclusive) for bytes written by `CreateRandomBinaryFile` and `CreateRandomText`. |
  | `minFileBytes` | int | no | `256` | Lower bound (inclusive) for random file sizes. |
  
  Use an explicit `seed` when multiple devices must produce identical chaos sequences. When the save roots start from the same baseline the deterministic PRNG yields the same verb stream, so parity checks do not require sharing intermediate artifacts.

  Chaos runs always honor folder boundaries: `RenameFolder` cascades through child files, `DeleteFolder` is skipped when the randomly chosen target is the root, and `CopyFile` selections ignore manifest JSON files unless explicitly added via `verbs`. Paths beneath the reserved `cloudsync/` metadata directory are never touched. The agent logs each generated verb so the controller can correlate individual mutations with later snapshot diffs.

  Example scenario snippet that exercises chaos mode across two devices and validates parity:

  ```yaml
  blocks:
    chaosEdit:
      - command: WriteGameSaveData
        parameters:
          chaos:
            operationCount: 50
            seed: 1337
      - command: CaptureSaveContainerSnapshot
    chaosValidate:
      - command: CompareSaveContainerSnapshots
    chaosReplay:
      - command: WriteGameSaveData
        parameters:
          chaos:
            operationCount: 50
            seed: 1337
      - command: CaptureSaveContainerSnapshot
  executionOrder:
    - role: DeviceA
      block: chaosEdit
    - role: DeviceB
      block: chaosReplay
    - role: DeviceA
      block: chaosValidate
  cleanup:
    DeviceA:
      - command: DeleteSaveRoot
    DeviceB:
      - command: DeleteSaveRoot
  ```

  In this flow DeviceA generates 50 random edits using a fixed seed and captures a snapshot. DeviceB executes the same chaos parameters after establishing the same starting dataset, captures its snapshot, and `CompareSaveContainerSnapshots` diff the artifacts—skipping `cloudsync/` entries—to confirm byte-for-byte parity before cleanup.

#### DeleteSaveRoot
- **Purpose**: Clear all local game save content and manifest files so scenarios can re-hydrate or validate cloud rollback behaviour from a cold start.
- **Primary APIs**: `PFGameSaveFilesGetFolder`, Win32/Posix directory traversal (`FindFirstFile`, `RemoveDirectory`, `DeleteFile`).
- **Parameters**:

  | Parameter | Type | Required | Default | Description |
  |-----------|------|----------|---------|-------------|
  | `preserveManifest` | bool | no | `false` | Keeps the harness-generated snapshot JSON if set to `true`. |

#### CaptureSaveContainerSnapshot
- **Purpose**: Produce a deterministic inventory of files, directories, sizes, timestamps, and hashes inside the save root for later comparison across devices.
- **Primary APIs**: `PFGameSaveFilesGetFolder`, Win32/Posix directory enumeration, harness hashing (`HarnessHash::ComputeSha256`).
- **Parameters**:

  | Parameter | Type | Required | Default | Description |
  |-----------|------|----------|---------|-------------|
  | `includeHashes` | bool | no | `true` | When `false`, hash calculation is skipped to reduce runtime. |

  Snapshot generation always excludes entries beneath the reserved `cloudsync/` metadata directory so PlayFab-managed state does not surface as a diff.

- **Response**:

  | Field | Type | Description |
  |-------|------|-------------|
  | `includeHashes` | bool | Indicates whether SHA-256 hashes were collected for file entries. |
  | `saveFolder` | string | Absolute save-root path on the device. Useful for troubleshooting when a snapshot fails. |
  | `filesCaptured` | int | Count of file entries included in the snapshot (hashes optional). |
  | `directoriesCaptured` | int | Count of directories that were inventoried. |
  | `cloudsyncEntriesSkipped` | int | Number of entries excluded because they live under the reserved `cloudsync/` metadata path. |
  | `totalBytes` | int64 | Sum of file sizes included in the snapshot. |
  | `entriesCaptured` | int | Total number of entries inside `snapshot.entries`. |
  | `skippedEntries` | array<string> | Present only when reserved paths were skipped; entries are relative paths using forward slashes. |
  | `snapshot` | object | Serialized manifest describing every captured entry (see below). |

  The `snapshot` object currently uses `schemaVersion = 1` and has the following shape:

  - `deviceId`: string — input device identifier provided by the agent (falls back to `TestDevice` if not assigned).
  - `scenarioId` / `scenarioName`: strings — copied from the active scenario metadata when available.
  - `saveFolder`: string — same as the top-level path.
  - `hashesIncluded`: bool — matches `includeHashes`.
  - `capturedAtUtc`: string — ISO 8601 timestamp of when the snapshot was recorded (UTC, millisecond precision).
  - `cloudsyncEntriesSkipped`: int — same value surfaced at the top level.
  - `totals`: object — aggregate counts (`files`, `directories`, `bytes`).
  - `entries`: array<object> — lexicographically sorted by relative path; each entry contains:
    - `path`: string — relative path using forward slashes.
    - `type`: string — one of `file`, `directory`, `symlink`, or the underlying filesystem category (`unknown`, `block`, `character`, `fifo`, `socket`, `none`).
    - `lastWriteTimeEpochMs`: int64 — UTC timestamp as milliseconds since the Unix epoch.
    - `lastWriteTime`: string — ISO 8601 representation of the same timestamp.
    - `attributes`: uint32 — Windows file attribute mask, present for every entry.
    - `size`: uint64 — included for `file` and `symlink` entries.
    - `sha256`: string — lowercase hex digest, present only when `includeHashes` is `true`.

  The controller caches the returned `snapshot` manifest for later comparisons and may rely on `schemaVersion` to accommodate future additions without breaking compatibility.

#### CompareSaveContainerSnapshots
- **Purpose**: Diff two previously captured snapshot artifacts and return a verdict when inconsistencies are detected.
- **Primary APIs**: Harness snapshot comparer (`SnapshotComparer::Diff`), JSON parsing (`nlohmann::json`).
- **Behaviour**: The harness caches the latest snapshot for each device role (for example `DeviceA`, `DeviceB`) when `CaptureSaveContainerSnapshot` runs. Invoking this command compares the remembered pair for the active scenario, ignores reserved metadata paths such as `cloudsync/`, surfaces any differences in the `actionResult.metrics.diff` payload, and fails if either side is missing or diverges beyond byte-for-byte equality. No parameters are required—the controller relies on the agent’s stored role metadata.

#### ConsumeDiskSpace
- **Purpose**: Exhaust free disk space to trigger cloud sync out-of-storage conditions.
- **Primary APIs**: // TODO wire up debug mock C API exported from the `PFGameSaves` DLL to reserve disk space during test runs.
- **Parameters**:

  | Parameter | Type | Required | Default | Description |
  |-----------|------|----------|---------|-------------|
  | `bytes` | int64 | yes | — | Total bytes to allocate via filler files. |
  | `scope` | string enum | no | `user` | `user` (save root), `system` (temp drive), or `custom`. |
  | `customPath` | string | conditional | — | Required when `scope` is `custom`; path where filler files are written. |
  | `token` | string | no | auto-generated | Identifier used by `ReleaseDiskSpace` to free the allocation. |

- **Response**:

  | Field | Type | Description |
  |-------|------|-------------|
  | `scope` | string | Echo of the resolved allocation scope (`user`, `system`, or `custom`). |
  | `resolvedPath` | string | Absolute directory where filler content was written. |
  | `bytesRequested` | int64 | Input request value from `bytes`. |
  | `bytesAllocated` | int64 | Actual bytes materialized on disk (may be less than requested if the volume runs out of space). |
  | `filesCreated` | int | Count of filler files produced by the harness. |
  | `directoriesCreated` | int | Number of directories staged to hold filler files. |
  | `token` | string | Allocation token used when releasing the disk reservation. Auto-generated when not supplied. |
  | `notes` | array<string> | Optional informational strings (for example, warnings about partial allocations). |

#### ReleaseDiskSpace
- **Purpose**: Remove filler files created by `ConsumeDiskSpace` to restore normal free-space levels.
- **Primary APIs**: // TODO wire up debug mock C API exported from the `PFGameSaves` DLL to release disk space reserved for test runs.
- **Parameters**:

  | Parameter | Type | Required | Default | Description |
  |-----------|------|----------|---------|-------------|
  | `token` | string | conditional | — | Frees the allocation created with the matching token. |
  | `scope` | string enum | no | `user` | When present, releases all filler files for the specified scope if `token` is omitted. |

- **Response**:

  | Field | Type | Description |
  |-------|------|-------------|
  | `scope` | string | Scope that was cleaned up (`user`, `system`, or `custom`). |
  | `token` | string | Token that was released when the cleanup targeted a specific allocation. Empty when a scope-wide purge was performed. |
  | `filesRemoved` | int | Number of filler files deleted. |
  | `directoriesRemoved` | int | Count of empty directories removed as part of the cleanup. |
  | `bytesFreed` | int64 | Total bytes reclaimed (best effort; derived from file sizes prior to deletion). |
  | `residualFiles` | array<string> | Optional list of relative paths that could not be deleted (permission issues, locks, etc.). |
  | `notes` | array<string> | Optional informational strings describing partial cleanup or follow-up actions. |

#### ConfigureHttpMock
- **Purpose**: Enable or disable HTTP fault injection profiles so scenarios can validate retry and error handling paths without external tooling.
- **Primary APIs**: Harness HTTP mock manager (`HttpMockManager::Configure`) sourced from `Test/PlayFabServicesManualTestApp/TestFramework/HttpMock.cpp` in the SDK (`C:\git\PlayFab.C\Test\PlayFabServicesManualTestApp\TestFramework\HttpMock.cpp`). Copy this class directly into the C++ test device app so libHttpClient mocks behave identically to the manual test harness.
- **Parameters**:

  | Parameter | Type | Required | Default | Description |
  |-----------|------|----------|---------|-------------|
  | `enabled` | bool | yes | — | Turns mocking on (`true`) or tears down every active mock (`false`). When disabling, the harness destroys all `HttpMock` instances created for the current scenario. |
  | `routes` | array<object> | conditional | `[]` | Required when enabling. Each entry maps directly to a `PlayFab::Test::HttpMock` constructor plus optional configuration calls described below. |

  **Route fields**

  | Field | Type | Required | Description |
  |-------|------|----------|-------------|
  | `method` | string | yes | HTTP verb passed to the `HttpMock` constructor (for example `POST`, `GET`). |
  | `url` | string | yes | Absolute URL string given to the `HttpMock` constructor so the libHttpClient matcher fires on the intended route. |
  | `statusCode` | int | yes | HTTP status supplied to the constructor and via `SetResponseHttpStatus`. Must be a valid `uint32_t`. |
  | `responseBody` | string | no | UTF-8 payload forwarded to `SetResponseBody(const char*)`. When absent the response body is left empty. |
  | `responseBodyBase64` | string | no | Base64-encoded bytes provided to `SetResponseBody(const uint8_t*, size_t)`. Use when the mock needs binary data. Mutually exclusive with `responseBody`. |
  | `responseHeaders` | object<string,string> | no | Key/value map passed to `SetResponseHeaders` so deterministic headers ship with the mock response. |
  | `clearBody` | bool | no | When `true`, the harness invokes `ClearReponseBody()` after applying other body fields so the mock returns an empty payload. |

  The harness automatically wires `SetCallback` to record every hit (URL, request body, hit count) into the action’s artifact payload so scenarios can assert on request cadence without custom plumbing.

  Example scenario fragment that enables two mocks, exercises them, then tears them down:

  ```yaml
  blocks:
    mockTest:
      - command: ConfigureHttpMock
        parameters:
          enabled: true
          routes:
            - method: POST
              url: https://titleId.playfabapi.com/Client/LoginWithCustomID
              statusCode: 200
              responseBody: '{"data":{"SessionTicket":"mock-ticket","PlayFabId":"12345"}}'
              responseHeaders:
                Content-Type: application/json
            - method: GET
              url: https://titleId.playfabapi.com/Client/GetPlayerProfile
              statusCode: 503
              responseBody: '{"code":503,"status":"ServiceUnavailable"}'
              clearBody: false
      - command: PFGameSaveFilesAddUserWithUiAsync
      - command: ConfigureHttpMock
        parameters:
          enabled: false
  ```

All harness commands follow the standard command envelope contract: they emit an `actionResult` with success or failure state and attach artifacts (where applicable) via `artifactReady` events so the controller can archive the outputs alongside PlayFab API results.

